//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
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
// $Id: MGOutputG4Steps.cc,v 1.29 2009-06-23 17:05:06 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputG4Steps.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Dec 16 17:03:08 PST 2004
 * 
 * REVISION:
 *
 * 12-16-2004, Created, R. Henning
 * 12-22-2005, Added coordinates in local volumes to output, R. Henning
 * 01-06-2006, Added physical volume name, R. Henning.
 * 08-15-2006, Added options to stop alpha, beta and nuclei tracks
 *             outside of sensitive volumes, R. Henning
 * 09-15-2006, Merged with changes from J. Detwiler, R. Henning
 * 02-22-2007, Added options to stop gamma and neutron tracks, A. Schubert
 * 02-20-2009, removed unecessary nuclei id encoding, A. Schubert
 * 03-18-2009, Added fWriteSensitiveStepsOnly, A. Schubert
 * 06-18-2009, Added fWriteTrackPreStep, split contents of
 *             RootSteppingAction() into AssignG4StepsDataValues() and
 *             OutputDebuggingInfo(), A. Schubert
 * 11-24-2009, Added fWasPreStepWritten, removed
 *             fT->fG4Process[fT->fProcessNamePos] from OutputDebuggingInfo(),
 *             set file compression to 1, A. Schubert
 * 03-08-2009, Added additional comments to changes from 24 Nov 2009, A.
 *             Schubert
 * 10-22-2010, Changed prestep fGlobalTime units to seconds, A. Schubert
 */
//---------------------------------------------------------------------------//
//

#include <cstring>
#include <string>
#include <sstream>

//#include <CLHEP/Units/SystemOfUnits.h>
//#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "G4Event.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4GenericIon.hh"
#include "TTree.h"

#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "io/MGOutputG4StepsMessenger.hh"
#include "io/MGOutputRoot.hh"


//---------------------------------------------------------------------------//

#include "io/MGOutputG4Steps.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGOutputG4Steps::MGOutputG4Steps(G4bool isMother) :
  MGOutputRoot(isMother),
  fMaxArrayLength(50000), 
  fStepIndex(0), 
  fAvgLengthProcessName(30),
  fWriteSensitiveHitsOnly(false),
  fWriteSensitiveStepsOnly(false),
  fWriteTrackPreStep(false),
  fRecordPreStep(false), 
  fKillAlphas(false), 
  fKillBetas(false), 
  fKillGammas(false), 
  fKillNeutrons(false), 
  fStopNuclei(false),
  fWasPreStepWritten(false)
  // added kill gammas and neutrons 070219 ags messenger?
{
  fT = new MGOutputG4StepsData();
  fRadDecayProcPointer = NULL;
  fMessenger = new MGOutputG4StepsMessenger(this);
  fHasRadDecay = true;
}

//---------------------------------------------------------------------------//

MGOutputG4Steps::~MGOutputG4Steps()
{
  fT->DeallocateArrays();
  delete fT;
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::BeginOfEventAction(const G4Event *event)
{
  fEventID = event->GetEventID();
  fStepIndex = fT->fTotalNumberOfSteps = 0;
  fT->fProcessNamePos = 0;
  fT->fVolumeNamePos = 0;
  fOverflowReported = false; 
  fT->fTotalSensitiveEdep = 0;
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::BeginOfRunAction()
{
  if(!fT->ArraysAreAllocated()) {
    fMaxProcArrayLength = fT->AllocateArrays(fMaxArrayLength, fAvgLengthProcessName);
  }

  // Root init.
  if(IsMother()) 
    OpenFile();
  DefineSchema();
  if(MGLogger::GetSeverity() >= MGLogger::trace)
    fTree->Print();
  MGLog(routine) << "fWriteSensitiveHitsOnly set to : " 
		 << fWriteSensitiveHitsOnly << endlog;

  MGLog(routine) << "fWriteSensitiveStepsOnly set to : " 
		 << fWriteSensitiveStepsOnly << endlog;

  MGLog(routine) << "fUseTimeWindow set to : " << fUseTimeWindow << endlog;
  if(fUseTimeWindow){
    MGLog(routine) << "fTimeWindow set to : " << fTimeWindow << endlog;
  }
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::DefineSchema()
{
  if(!SchemaDefined()) {
    TTree *nT = 0;

    // Create output Tree if it has not been assigned.
    if(fTree) {
      nT = fTree;
    } else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "Generic step output Tree");
      if(!nT) {
	MGLog(error) << "Could not allocate memory for tree!" << endlog;
	MGLog(fatal) << endlog;
      }
    }

    /*
    When more than one tree is written to a file, there is trouble with ROOT
    file compression -- instead of the default compression level of 1, trees are
    not compressed.  Set the branch compression level explicitly here to ensure
    compression.  A. Schubert, 24 Nov 2009
    */

    TBranch* branch = nT->Branch("RawMC", "MGOutputG4StepsData", &fT, 32000, 1);

    Int_t compressionLevel = 1; 
    branch->SetCompressionLevel( compressionLevel );
    MGLog(routine) << "G4Steps compression set to " << compressionLevel <<
    endlog;


    SetSchemaDefined(true);
    MGLog(debugging) << "G4Step generic output schema defined." << G4endl; 
  } else 
    MGLog(warning) << "Schema already defined." << endlog;
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::EndOfEventAction(const G4Event*)
{
  WriteEvent();
  fOffsetTime = 0.0;
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::EndOfRunAction()
{
  if(IsMother()) {
    CloseFile();
    SetSchemaDefined(false);
  }
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::RootSteppingAction(const G4Step* step)
{
  fWasPreStepWritten = false;
  if(fStepIndex > (fMaxArrayLength - 1)) {
    if(!fOverflowReported) {
      MGLog(error) << "Event " << fEventID
		   << "too large for ROOT Tree. " << endlog;
      fOverflowReported = true;
    }
  } else {
     
    // for the first step of a track, record prestep info
    // A.G. Schubert, 18 June 2009:
    if( fWriteTrackPreStep ){  
      if( fStepIndex == 0 ){
        fRecordPreStep = true;
      } else if ( step->GetTrack()->GetTrackID() !=
      fT->fTrackID[fStepIndex-1] ) {
        fRecordPreStep = true;
      }
    }

    if(fRecordPreStep) { 
      AssignG4StepsDataValues( step );
      OutputDebuggingInfo( step );      
      fStepIndex++;
      fWasPreStepWritten = true;
      fRecordPreStep = false;
    }
      AssignG4StepsDataValues( step );
  }
  OutputDebuggingInfo( step );      
  fStepIndex++;
}

//---------------------------------------------------------------------------//

void MGOutputG4Steps::AssignG4StepsDataValues(const G4Step* step)
{
  G4String procName, volName;
  G4Track *track = step->GetTrack();
  G4Material *material = track->GetMaterial();
  G4ThreeVector position = (fRecordPreStep) ?
    step->GetPreStepPoint()->GetPosition() / cm : track->GetPosition() / cm;
  G4ThreeVector momentum = (fRecordPreStep) ?
    step->GetPreStepPoint()->GetMomentum() / keV : track->GetMomentum() / keV;
  G4ParticleDefinition *particle = track->GetDefinition();

  G4bool isInSensitiveVolume = false;
  if(track->GetVolume()->GetLogicalVolume()->GetName().
     find("active", 0) != std::string::npos) isInSensitiveVolume = true;

  // only write deposits in the sensitive volume, if desired
  // 18 Mar 2009, AGS:
  if( fWriteSensitiveStepsOnly && 
      ( !step->GetTotalEnergyDeposit() || 
          !isInSensitiveVolume ) ) return;

  fT->fStatus01[fStepIndex] = 0;
  fT->fX[fStepIndex] = G4ToRoot(position.x());
  fT->fY[fStepIndex] = G4ToRoot(position.y());
  fT->fZ[fStepIndex] = G4ToRoot(position.z());
  fT->fPx[fStepIndex] = G4ToRoot(momentum.x());
  fT->fPy[fStepIndex] = G4ToRoot(momentum.y());
  fT->fPz[fStepIndex] = G4ToRoot(momentum.z());  
  fT->fEdep[fStepIndex] = (fRecordPreStep) ? 0 : G4ToRoot(step->GetTotalEnergyDeposit()/keV);
  if( isInSensitiveVolume ) {
    fT->fTotalSensitiveEdep += (fRecordPreStep) ? 0 : fT->fEdep[fStepIndex];
    fT->fStatus01[fStepIndex] = 1; // Set Bit 0 high if in active volume
  }
  fT->fMaterialIndex[fStepIndex] = material->GetIndex();
  fT->fPreviousTrackID[fStepIndex] = G4ToRoot(track->GetParentID());
  fT->fTrackID[fStepIndex] = G4ToRoot(track->GetTrackID());
  fT->fGlobalTime[fStepIndex] = (fRecordPreStep) ?
    G4ToRoot(step->GetPreStepPoint()->GetGlobalTime() / s) : G4ToRoot(track->GetGlobalTime() / s);
  fT->fOffsetTime[fStepIndex] = G4ToRoot(fOffsetTime / s);
  
  // Save names of G4 processes that terminated this step as a 1-D string
  // of characters. Each step's process is separated by a '\0'.
  const G4VProcess* processDefinedStep = (fRecordPreStep) ? 
    step->GetPreStepPoint()->GetProcessDefinedStep() :
    step->GetPostStepPoint()->GetProcessDefinedStep();
 
  procName = (processDefinedStep) ?
    processDefinedStep->GetProcessName() : ""; 
    
  volName = track->GetVolume()->GetName();
  Int_t sizeName = procName.size() + 1;
  Int_t sizeVolName = volName.size() + 1;
  if(fT->fProcessNamePos + sizeName > fMaxProcArrayLength || 
     fT->fVolumeNamePos + sizeVolName > fMaxProcArrayLength) {
    MGLog(error) << "Process name array overflow." << endlog;
    fOverflowReported = true;
  } else {
    strncpy(&fT->fG4Process[fT->fProcessNamePos], 
           procName.c_str(), sizeName);
    strncpy(&fT->fPhysicalVolume[fT->fVolumeNamePos], 
           volName.c_str(), sizeVolName);
    fT->fProcessNamePos += sizeName;// + 1;   Add 1 for '\0'
    fT->fVolumeNamePos += sizeVolName;// + 1; 
  }
  fT->fParticleID[fStepIndex] = 
    G4ToRoot(particle->GetPDGEncoding());
  // Add location in local coordinates. Assume no rotation.
  G4TouchableHandle handle = step->GetPreStepPoint()->GetTouchableHandle();
  G4ThreeVector translation = 
    -handle->GetTranslation(0)/cm + position;
  fT->fLocalX[fStepIndex] = G4ToRoot(translation.x());
  fT->fLocalY[fStepIndex] = G4ToRoot(translation.y());
  fT->fLocalZ[fStepIndex] = G4ToRoot(translation.z()); 
  // Kill alpha or e- track  or stop nuclei if user requires it.
  if(fT->fStatus01[fStepIndex] != 1) { 
    if((fKillAlphas && fT->fParticleID[fStepIndex] == 1000020040) ||
       (fKillBetas && fT->fParticleID[fStepIndex] == 11) ||
       (fKillGammas && fT->fParticleID[fStepIndex] == 22) ||
       (fKillNeutrons && fT->fParticleID[fStepIndex] == 2112))
      track->SetTrackStatus(fKillTrackAndSecondaries);
    else if(fStopNuclei && fT->fParticleID[fStepIndex] > 100000000)
      track->SetKineticEnergy(0.0);
  }

  // Kill (anti)neutrinos irregardless
  if(fT->fParticleID[fStepIndex] == -12 || fT->fParticleID[fStepIndex] == 12)
    track->SetTrackStatus(fKillTrackAndSecondaries);
}

//---------------------------------------------------------------------------//  

void MGOutputG4Steps::OutputDebuggingInfo(const G4Step* step)
{
  MGLog(debugging) << "Step Information:" << endlog;
  MGLog(debugging) << "-----------------" << endlog;
  MGLog(debugging) << "fMaxArrayLength: " << fMaxArrayLength << endlog;
  MGLog(debugging) << "fStepIndex: " << fStepIndex << endlog;
  MGLog(debugging) << "fRecordPreStep: " << fRecordPreStep << endlog;
  MGLog(debugging) << "fOverflowReported: " << fOverflowReported << endlog;
  MGLog(debugging) << "fEventID: " << fEventID << endlog;
  MGLog(debugging) << "fParticleID: " << fT->fParticleID[fStepIndex]<<endlog;
  MGLog(debugging) << "fPreviousTrackID: " 
         << fT->fPreviousTrackID[fStepIndex] << endlog;
  MGLog(debugging) << "fTrackID: " << fT->fTrackID[fStepIndex] << endlog;
  MGLog(debugging) << "fX (cm): " << fT->fX[fStepIndex] << endlog;
  MGLog(debugging) << "fY (cm): " << fT->fY[fStepIndex] << endlog;  
  MGLog(debugging) << "fZ (cm): " << fT->fZ[fStepIndex] << endlog;    
  MGLog(debugging) << "fPx (keV): " << fT->fPx[fStepIndex] << endlog;
  MGLog(debugging) << "fPy (keV): " << fT->fPy[fStepIndex] << endlog;  
  MGLog(debugging) << "fPz (keV): " << fT->fPz[fStepIndex] << endlog;    
  MGLog(debugging) << "fEdep (keV): " << fT->fEdep[fStepIndex] << endlog;
  MGLog(debugging) << "fMaxProcArrayLength: " << fMaxProcArrayLength << endlog;
  MGLog(debugging) << "fT->fProcessNamePos: " << fT->fProcessNamePos << endlog;
  const G4VProcess* processDefinedStep = (fRecordPreStep) ? 
    step->GetPreStepPoint()->GetProcessDefinedStep() :
    step->GetPostStepPoint()->GetProcessDefinedStep();
 
  G4String procName = (processDefinedStep) ?
    processDefinedStep->GetProcessName() : ""; 
  MGLog(debugging) << "procName: " << procName << endlog;
  MGLog(debugging) << "fT->fVolumeNamePos: " << fT->fVolumeNamePos << endlog;
  MGLog(debugging) << "volName: " << step->GetTrack()->GetVolume()->GetName() << endlog;
  MGLog(debugging) << "fLocalX (cm): " << fT->fLocalX[fStepIndex] << endlog;
  MGLog(debugging) << "fLocalY (cm): " << fT->fLocalY[fStepIndex] << endlog;  
  MGLog(debugging) << "fLocalZ (cm): " << fT->fLocalZ[fStepIndex] << endlog;    
  MGLog(debugging) << "fStatus01: " << fT->fStatus01[fStepIndex] << endlog;
  MGLog(debugging) << "fMaterialIndex: " 
         << fT->fMaterialIndex[fStepIndex] << endlog;
  MGLog(debugging) << "fGlobalTime (s): " 
         << fT->fGlobalTime[fStepIndex] << endlog;
  MGLog(debugging) << "fOffsetTime (s): " 
         << fT->fOffsetTime[fStepIndex] << endlog;
}

//---------------------------------------------------------------------------//  

void MGOutputG4Steps::WriteEvent()
{
  // Only save full event if requested, otherwise just save header.
  if(!fWriteSensitiveHitsOnly || 
     (fWriteSensitiveHitsOnly && fT->fTotalSensitiveEdep>0.) ||  
     (fWriteSensitiveStepsOnly && fT->fTotalSensitiveEdep>0.)) {
    if(fStepIndex > fMaxArrayLength-1){
      MGLog(error) << "fStepIndex exceeds fMaxArrayLength: "
                   << fStepIndex 
		   << " > " 
		   << fMaxArrayLength 
		   << endlog;
      MGLog(error) << "Setting fTotalNumberOfSteps to fMaxArrayLength" 
                   << endlog;
      fT->fTotalNumberOfSteps = fMaxArrayLength;
    } else {
      fT->fTotalNumberOfSteps = fStepIndex;
      fT->fProcessNamePos++; // Convert from final array position to array length.
      fT->fVolumeNamePos++; // Convert from final array position to array length.
    }
  } else {
    fT->fTotalNumberOfSteps = 0;
    fT->fProcessNamePos = fT->fVolumeNamePos = 0;
  }
  MGLog(debugging) << "End Of event, " << fT->fTotalNumberOfSteps 
		   << " total number of steps, " 
		   << fT->fProcessNamePos 
		   << " characters in fG4Process." << endlog;
  //and finally fill the tree if IsMother()

  // don't fill the tree if only writing sensitive steps and 
  // no sensitive edep, A. Schubert 18 Mar 2009
  if( fWriteSensitiveStepsOnly && !fT->fTotalSensitiveEdep ) return; 

  if(IsMother()) FillTree();
}

//---------------------------------------------------------------------------//  

void MGOutputG4Steps::WritePartialEvent(const G4Event*)
{
  WriteEvent();
}

//---------------------------------------------------------------------------// 

void MGOutputG4Steps::ResetPartialEvent(const G4Event*)
{
  fStepIndex = 0;
  fT->fTotalNumberOfSteps = 0;
  fT->fProcessNamePos = 0;
  fT->fVolumeNamePos = 0;
  fOverflowReported = false; 
  fT->fTotalSensitiveEdep = 0;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
