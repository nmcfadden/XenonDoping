//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id: MJOutputDetectorEvent.cc,v 1.10 2009-02-25 01:32:36 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputDetectorEvent.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Sat Jan 29 16:53:57 PST 2005
 * 
 * REVISION:
 *
 * 01-29-2005, Created, R. Henning
 * 08-16-2005, Added pulse-height defect, R. Henning
 * 02-20-2009, Updated particle id per Chao Zhang recommendation, A. Schubert
 * 11-24-2009, added check to be sure fVolumeNames size matches number of
 *             steps recorded by MGOutputG4Steps, added duplicate info to
 *             fVolumeNames if MGOutputG4Steps recorded prestep info,
 *             set compression to 1, A. Schubert
 */
//---------------------------------------------------------------------------//
//

#include <list>
#include <math.h>

#include "globals.hh"
#include "G4ios.hh"
#include "G4Navigator.hh"
#include "G4TouchableHandle.hh"
#include "G4TransportationManager.hh"
#include "G4Step.hh"

#include "TTree.h"

//#include "geometry/MGGeometryDetectorConstruction.hh"
#include "io/MGLogger.hh"
#include "io/MGOutputG4Steps.hh"
#include "io/MGOutputG4StepsData.hh"
#include "io/MGOutputRoot.hh"
#include "management/MGManager.hh"
#include "mjio/MJOutputMCEvent.hh"
#include "mjio/MJOutputSegXtal.hh"
#include "mjio/MJOutputDetectorEventData.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputDetectorEvent.hh" 

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJOutputDetectorEvent::MJOutputDetectorEvent(MJOutputMCEvent *ptr, 
					     G4bool isMother):
  MGOutputRoot(isMother), fMCEvent(ptr), fNavigator(0)
{
  fDetectorEventData = new MJOutputDetectorEventData();
  fDAQTime = 0.001 * second; // Default : 1 ms. 
}

//---------------------------------------------------------------------------//

MJOutputDetectorEvent::~MJOutputDetectorEvent()
{
  delete fDetectorEventData;
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::BeginOfEventAction(const G4Event*)
{
  fSeedTimes.clear();
  fVolumeNames.clear();
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::BeginOfRunAction()
{
  DefineSchema();
  fDetectorEventNum = 0;
  G4String specialFileName = GetFileName();
  size_t pos = specialFileName.find(".root");
  specialFileName.insert(pos, "_det");
  fSpecialFile = new TFile(specialFileName, "RECREATE");
  fSpecialTree->SetDirectory(fSpecialFile);
  fNavigator = G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking();
}
 
//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::DefineSchema()
{
  if(!SchemaDefined()) {
    fSpecialTree = new TTree("DetectorTree", "Detector Output Tree");
    TBranch* branch = fSpecialTree->Branch("Detector_hits",
    "MJOutputDetectorEventData", &fDetectorEventData, 32000, 3);

    Int_t compressionLevel = 1;
    branch->SetCompressionLevel( compressionLevel ); 
    MGLog(routine) << "MCEvent compression set to " << compressionLevel 
    << endlog;


    if(MGLogger::GetSeverity() >= MGLogger::routine)
      fSpecialTree->Print();
    SetSchemaDefined(true);
    MGLog(trace) << "Detector output schema defined." << endlog; 
  } else 
    MGLog(error) << "Schema already defined." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::EndOfEventAction(const G4Event*)
{
  WriteEvent();
}

void MJOutputDetectorEvent::WriteEvent()
{
  /*

    This breaks a single Geant 4 event into several detector events. Certain
    generators, ie. Geant 4 RDM, generates entire decay chains as single 
    events. This means that some events can span years! This method breaks
    such an event into multiple detector events separated in time. The idea
    is to find steps that are clustered in time. These are found by finding
    "seed" times that are separated by the DAQ time of the electronics.
    The seeds are found during tracking in the RootStepping() method.
    Each such seed is used to build an event by finding steps withing the DAQ
    time of this seed. This method allows pile-up to be simulated in the 
    future as well.

 */

  Double_t currentSeedTime;
  Int_t    i, j; 
  G4bool   xtalFound;
  std::list<std::string>::iterator copyNamesIterator;
  MGOutputG4StepsData *g4Steps = fMCEvent->GetG4Steps()->GetG4StepsData();

// Loop over all seed times/detector events
  while(fSeedTimes.size() > 0) { 
    fDetectorEventData->fSeedTime = fSeedTimes.front() / second;
    currentSeedTime = G4ToRoot(fDetectorEventData->fSeedTime);
    fDetectorEventData->Clear();

    // The size of fVolumeNames must match the number of G4Steps steps for the
    // following loop to work correctly. 24 Nov 20009, A. Schubert

    if ( fMCEvent->GetG4Steps()->GetNumberOfSteps() - fVolumeNames.size() != 0 ) {
      MGLog( error ) << "G4Steps recorded " <<
      fMCEvent->GetG4Steps()->GetNumberOfSteps() << " steps, but there are " <<
      fVolumeNames.size() << " volumes in fVolumeNames" << endlog;
    }

    // Loop over all Geant 4 steps recorded in MGOutputG4StepsData class.

    for(i = 0, copyNamesIterator = fVolumeNames.begin();
	i < fMCEvent->GetG4Steps()->GetNumberOfSteps(); 
	i++, copyNamesIterator++) {

      // Are we in a sensistive volume and ...
      // is the current step within range of the current time seed?

      if(g4Steps->fStatus01[i] == 1 && // Hack .. need to standardize bit mask
	 fabs(g4Steps->fGlobalTime[i] - currentSeedTime) < fDAQTime/second &&
	 g4Steps->fEdep[i] > 0.0){ // Ignore steps with no energy deposit.

	xtalFound = false;
	for(j = 0; j < fDetectorEventData->fNumberOfCrystals; j++) {

          // Is the current crystal a previously hit crystal?
          xtalFound = ((MJOutputSegXtal*)((*fDetectorEventData->
		      fCrystals)[j]))->fXtalName.c_str() == *copyNamesIterator;
          if(xtalFound){

            // If so, add edep to it.
	    // Account for quenching in recoil Ge nuclei.
            if(g4Steps->fParticleID[i] > 1000000000 &&
               (g4Steps->fParticleID[i]%1000000)/10000 == 32) // 20 Fed 2009
	      ((MJOutputSegXtal*)((*fDetectorEventData->fCrystals)[j]))->fEdep 
              	+= EdepWithPulseHeightDefect(
		   (g4Steps->fPx[i]*g4Steps->fPx[i] + 
		    g4Steps->fPy[i]*g4Steps->fPy[i] + 
		    g4Steps->fPz[i]*g4Steps->fPz[i]), 
		   g4Steps->fEdep[i], 
		   g4Steps->fParticleID[i]);
	    else
	      ((MJOutputSegXtal*)((*fDetectorEventData->fCrystals)[j]))->fEdep 
              	+= g4Steps->fEdep[i];
	    break;
	  }	  
	}

        // The current crystal is not a hit crystal. Add crystal to list.
        if(!xtalFound) { 
          new((*fDetectorEventData->fCrystals)[j]) 
            MJOutputSegXtal(*copyNamesIterator, j);

            // If so, add edep to it.
	    // Account for quenching in recoil Ge nuclei.
            if(g4Steps->fParticleID[i] > 1000000000 &&
               (g4Steps->fParticleID[i]%1000000)/10000 == 32) // 20 Feb 2009
	      ((MJOutputSegXtal*)((*fDetectorEventData->fCrystals)[j]))->fEdep 
              	+= EdepWithPulseHeightDefect(
		   (g4Steps->fPx[i]*g4Steps->fPx[i] + 
		    g4Steps->fPy[i]*g4Steps->fPy[i] + 
		    g4Steps->fPz[i]*g4Steps->fPz[i]), 
		   g4Steps->fEdep[i], 
		   g4Steps->fParticleID[i]);
	    else
	      ((MJOutputSegXtal*)((*fDetectorEventData->fCrystals)[j]))->fEdep 
		= g4Steps->fEdep[i];
          fDetectorEventData->fNumberOfCrystals++;
        }
      }
    }
    fSeedTimes.pop_front();
    
    // If no crystals were hit, then this is not a true detector event.
    if(fDetectorEventData->fNumberOfCrystals > 0) {    	
      fDetectorEventData->fMCEventNum = fMCEvent->GetG4Steps()->GetEventID();
      fDetectorEventData->fDetectorEventNum = fDetectorEventNum++;
      fSpecialTree->Fill();

    }
  } 
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::EndOfRunAction()
{
  fSpecialTree->AutoSave();
  fSpecialTree->Delete();
  fSpecialTree = 0;
  fSpecialFile->Close();
  fSpecialFile->Delete("");
  fSpecialFile = 0;
  SetSchemaDefined(false);
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEvent::RootSteppingAction(const G4Step* step)
{
  G4double stepTime = step->GetTrack()->GetGlobalTime();
  std::list<G4double>::iterator stepTimeIterator = fSeedTimes.begin();
  G4bool   seedExists;

  // Generate a new seed time if current time is not close to existing 
  // seed time.
  seedExists = false;
  while(stepTimeIterator != fSeedTimes.end()) {
    if(fabs(*stepTimeIterator - stepTime) < fDAQTime) {
      seedExists = true;
      stepTimeIterator = fSeedTimes.end();
    } else stepTimeIterator++;
  }
  if(!seedExists) fSeedTimes.push_back(stepTime);

  // Save name of volume one higher in hierarchy. This is the crystal 
  // placement.

  std::string volumeName;

  G4TouchableHandle handle = step->GetPreStepPoint()->GetTouchableHandle();
  if("World" == handle->GetVolume(0)->GetName())
     volumeName = "DOG";
  else 
     volumeName = handle->GetVolume(1)->GetName();

  fVolumeNames.push_back( volumeName );
 
  // The list of volumes must match the number of steps if the energy in each
  // detector is to be calculated correctly.  The number of steps recorded by
  // G4Steps may get ahead of the number of volumes if G4Steps is recording
  // prestep info, A. Schubert 24 Nov 2009
  

  if ( fMCEvent->GetG4Steps()->GetWasPreStepWritten() ) {
     fVolumeNames.push_back( volumeName );
  }

}

//---------------------------------------------------------------------------// 

void MJOutputDetectorEvent::WritePartialEvent(const G4Event*)
{
  WriteEvent();
}


//---------------------------------------------------------------------------// 

void MJOutputDetectorEvent::ResetPartialEvent(const G4Event*)
{
  fSeedTimes.clear();
  fVolumeNames.clear();
  fOffsetTime = 0.0;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
