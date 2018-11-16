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
// $Id: MJOutputSolidBlock.cc,v 1.5 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputSolidBlock.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *   Until MaGe officially runs with Geant4.9.5, some of the functionality of
 *   this output class is limited.  Specifically, the GetTargetNucleus() function
 *   in G4HadronicProcess is non-existent in previous versions of Geant4.  If you
 *   are running with Geant4.9.5, you can uncomment the sections noted below.  
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: 02-03-2012
 * 
 * REVISION:
 *
 * 01-26-2012, Created, M. Kidd
 *
 */
//---------------------------------------------------------------------------//
//

#include <iostream>
#include <vector>

#include "TH1D.h"
#include "TTree.h"
#include "TMath.h"

#include "globals.hh"
#include "G4DynamicParticle.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4TrackingManager.hh"
#include "management/MGManager.hh"
#include "management/MGManagementTrackingAction.hh"
#include "G4HadronicProcess.hh"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputFastn.hh"

using namespace std;
using namespace CLHEP;

//---------------------------------------------------------------------------//

MJOutputFastn::MJOutputFastn(G4bool isMother):
  MGOutputRoot(isMother)
{
  SetSchemaDefined(false);
}

//---------------------------------------------------------------------------//

MJOutputFastn::~MJOutputFastn()
{
}

//---------------------------------------------------------------------------//

void MJOutputFastn::Clear()
{
//  MGLog(routine)<<"number of neutrons created in event:  "<<fNNeutrons<<"\n";
  Edep = 0.0;
  fNNeutrons = 0;
  nStep = 0;
  fTheProcessName.clear();
}

//---------------------------------------------------------------------------//

void MJOutputFastn::KillTrack(G4Track* aTrack)
{ 
  aTrack->SetTrackStatus(fStopAndKill);
  aTrack->SetBelowThresholdFlag();
  aTrack->SetGoodForTrackingFlag(false);
}

//---------------------------------------------------------------------------//

void MJOutputFastn::BeginOfEventAction(const G4Event *event)
{
  Clear();

  EventNumber = G4ToRoot(event->GetEventID());
  MGLog(debugging) << "BeginOfEventAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputFastn::BeginOfRunAction()
{
  if(IsMother())
    OpenFile();

  kMaxNeutrons = 1000000;
  fNNeutrons = 0;

  fTheProcessName.reserve(kMaxNeutrons);
  fTheParentName.reserve(kMaxNeutrons);
  MGLog(routine)<<"reserved vector of capacity "<<fTheProcessName.capacity()<<"\n";


  NPositionX0 = new Float_t[kMaxNeutrons];
  NPositionY0 = new Float_t[kMaxNeutrons];
  NPositionZ0 = new Float_t[kMaxNeutrons];
  nEnergy = new Float_t[kMaxNeutrons];
  totalDistance = new Float_t[kMaxNeutrons];
  orthDistance = new Float_t[kMaxNeutrons];
//  targetNucN = new Int_t[kMaxNeutrons];		//***needs Geant4.9.5***
//  targetNucZ = new Int_t[kMaxNeutrons];		//***needs Geant4.9.5***
  theParent = new Int_t[kMaxNeutrons];
  theNeutronTrackID = new Int_t[kMaxNeutrons];
  theParentID = new Int_t[kMaxNeutrons];
  nEdep = new Float_t[kMaxNeutrons];

  DefineSchema();
  MGLog(trace) << "BeginOfRunAction() Finished." << endlog;
}

//---------------------------------------------------------------------------//


void MJOutputFastn::DefineSchema()
{
  if(!SchemaDefined()){
    TTree *nT;
    if(fTree)
       nT = fTree;
    else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "Fast Neutron Output Tree");
    }

    // MC Header info.
    nT->Branch("EventNumber", &EventNumber, "EventNumber/i");					//Event Number
    nT->Branch("fNNeutrons", &fNNeutrons,"fNNeutrons/I");					//Number of neutrons generated per event
    nT->Branch("theParent", theParent,"theParent[fNNeutrons]/I");				//Parent particle which generated neutron
    nT->Branch("theParentID", theParentID,"theParentID[fNNeutrons]/I");				//Track ID of parent
    nT->Branch("theParentName", &fTheParentName);						//Name of parent
    nT->Branch("theNeutronTrackID", theNeutronTrackID,"theNeutronTrackID[fNNeutrons]/I");	//Track ID of neutron
    nT->Branch("PositionX", &PositionX, "PositionX/F");						//X Position of primary muon
    nT->Branch("PositionY", &PositionY, "PositionY/F");						//Y Position of primary muon
    nT->Branch("PositionZ", &PositionZ, "PositionZ/F");						//Z Position of primary muon
    nT->Branch("NPositionX0", NPositionX0, "NPositionX0[fNNeutrons]/F");			//X Initial Position of neutron
    nT->Branch("NPositionY0", NPositionY0, "NPositionY0[fNNeutrons]/F");			//Y Initial Position of neutron
    nT->Branch("NPositionZ0", NPositionZ0, "NPositionZ0[fNNeutrons]/F");			//Z Initial Position of neutron
    nT->Branch("PEnergy", &PEnergy, "PEnergy/F");						//energy of primary muon
    nT->Branch("nEnergy", nEnergy, "nEnergy[fNNeutrons]/F");					//energy of created neutron
    nT->Branch("nEdep", nEdep, "nEdep[fNNeutrons]/F");						//energy deposited by neutron in step
    nT->Branch("totalDistance", totalDistance, "totalDistance[fNNeutrons]/F");			//distance from initial primary muon
    nT->Branch("orthDistance", orthDistance, "orthDistance[fNNeutrons]/F");			//orthogonal distance from muon track
    nT->Branch("fTheProcessName", &fTheProcessName);						//process which created neutron
//    nT->Branch("targetNucN", targetNucN, "targetNucN[fNNeutrons]/I");				//***needs Geant4.9.5***  N of target nucleus involve in process
//    nT->Branch("targetNucZ", targetNucZ, "targetNucZ[fNNeutrons]/I");				//***needs Geant4.9.5***  Z of target nucleus involve in process
    nT->Branch("Edep", &Edep, "Edep/F");							// total energy deposited in event
    MGLog(trace) << "Created Branch Edep\n" << endlog;

    SetSchemaDefined(true);
  }
}


//---------------------------------------------------------------------------//

void MJOutputFastn::EndOfEventAction(const G4Event*)
{

  Edep /= MeV;
  
  if(IsMother())
    FillTree();

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputFastn::EndOfRunAction()
{
  if(IsMother()) CloseFile();
  MGLog(trace) << "EndOfRunAction finished." << endlog;
}

//---------------------------------------------------------------------------//
//*  Note that in the RootSteppingAction below, that any neutron in the step
//*  is counted as a "new" neutron.  If it's a neutron in a scattering process
//*  it will be reflected in the neutron track ID.  You should use your analysis 
//*  code to sort out the scattered neutrons from the created ones.
//---------------------------------------------------------------------------//

void MJOutputFastn::RootSteppingAction(const G4Step *step)
{
  int stepNeutrons = 0;  //keeps track of number of neutrons created in the step.

  if(nStep == 0){
	PEnergy = step->GetPreStepPoint()->GetKineticEnergy() / MeV;
	G4ThreeVector position0 = step->GetPreStepPoint()->GetPosition();
	PositionX = position0.x();
	PositionY = position0.y();
	PositionZ = position0.z();
  }

  G4Track *track = step->GetTrack();
  G4ThreeVector position = track->GetPosition();

  Edep += step->GetTotalEnergyDeposit();

  G4TrackingManager *fpTrackingManager = MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager();

  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();

if(secondaries)
  {
    size_t nSeco = secondaries->size();

    for(size_t i=0;i<nSeco;i++)  //cycling over the list of secondaries
    { 

      G4Track* tr = (*secondaries)[i] ;

      if(tr->GetDefinition()->GetPDGEncoding() == 2112){     // if the secondary is a neutron
	if(fNNeutrons < kMaxNeutrons)	
	{
	theNeutronTrackID[fNNeutrons] = track->GetTrackID();
	theParentID[fNNeutrons] = track->GetParentID();
        theParent[fNNeutrons] = track->GetDefinition()->GetPDGEncoding();
	theParentName = track->GetDefinition()->GetParticleType();
	fTheParentName.push_back(theParentName);

//	MGLog(routine)<<"Neutron with track ID:  "<<theNeutronTrackID[fNNeutrons]<<" created by particle:  "<<theParent[fNNeutrons]<<" with track number:  "<<theParentID[fNNeutrons]<<"\n";

        theProcess = tr->GetCreatorProcess()->GetProcessName();

 	const G4HadronicProcess* activeProcess = dynamic_cast<const G4HadronicProcess*>(tr->GetCreatorProcess());

	if(activeProcess == 0) MGLog(routine)<<"Null pointer for process:  "<<theProcess<<".\n";
	else  {
//		targetNucN[fNNeutrons] = activeProcess->GetTargetNucleus()->GetN();		//***needs Geant4.9.5***
//		targetNucZ[fNNeutrons] = activeProcess->GetTargetNucleus()->GetZ();		//***needs Geant4.9.5***
//		MGLog(routine)<<"target nucleus Z = "<<targetNucZ[fNNeutrons]<<"\tN = "<<targetNucN[fNNeutrons]<<"\n";		//***needs Geant4.9.5***
	}

	fTheProcessName.push_back(theProcess);
	
//	MGLog(routine)<<"process stored in index "<<fNNeutrons<<" as "<<fTheProcessName.at(fNNeutrons)<<"\n";

	NPositionX0[fNNeutrons] = position.x();
	NPositionY0[fNNeutrons] = position.y();
	NPositionZ0[fNNeutrons] = position.z();

	orthDistance[fNNeutrons] = TMath::Sqrt(pow((NPositionX0[fNNeutrons]-PositionX),2)+pow((NPositionY0[fNNeutrons]-PositionY),2));
	totalDistance[fNNeutrons] = TMath::Sqrt(pow((NPositionX0[fNNeutrons]-PositionX),2)+pow((NPositionY0[fNNeutrons]-PositionY),2)+pow((NPositionZ0[fNNeutrons]-PositionZ),2));

	nEnergy[fNNeutrons] = tr->GetTotalEnergy();
	nEdep[fNNeutrons] = step->GetTotalEnergyDeposit();

	fNNeutrons++;		//keeps track of number of neutrons in the event
        stepNeutrons++;		//number of neutrons in step

	} //end if fNNeutrons < kMaxNeutrons
	else{  //if fNNeutrons > kMaxNeutrons
	    MGLog(warning) << "Multiplicity has exceeded: " << kMaxNeutrons << endlog;
	    MGLog(warning) << "Data is being lost..." << endlog;
	    MGLog(warning) << "Please reset kMaxNeutrons...killing track." << endlog;
	    KillTrack(track);
 	}  //end else
      }//end if Neutron

    }//end for over secondaries
//	if(stepNeutrons>0){
//		MGLog(routine)<<stepNeutrons<<" neutron(s) made in this step.\n";
//		MGLog(routine)<<fNNeutrons<<" neutrons made in this event so far.\n";
//		}
  }//end if(secondaries)
  nStep++;
} //end RootSteppingAction


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
