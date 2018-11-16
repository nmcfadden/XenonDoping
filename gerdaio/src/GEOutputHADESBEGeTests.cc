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
/**
 * CLASS IMPLEMENTATION:
 *   GEOutputHADESBEGeTests
 *
 * AUTHOR:  B. Lehnert
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 *  12-21-2011 Derived from GEOutputDario.cc, B. Lehnert
 *
 *
 */

//
// include files for ROOT
#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TNtuple.h"

#include <sstream>
#include <string>

// Include files for the G4 classes
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4DynamicParticle.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VProcess.hh"
#include "G4UImanager.hh"
#include "G4Ions.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4StepStatus.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VSensitiveDetector.hh"

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputHADESBEGeTests.hh"    
#include "gerdaio/GETrajectory.hh"    
#include "gerdaio/GEOutputHADESBEGeTestsMessenger.hh"
#include "munichteststand/GEHADESBEGeTests.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputHADESBEGeTests::GEOutputHADESBEGeTests() :
		MGOutputRoot(true), gNavigator(0), fHitOutputFile(0), energyInDetectors(
				0) {
	numberOfPrimaries = 0;
	fWriteHitCoordinates = false; // hit coordinates
	fWriteDLInformation = false; // hits to surface distances
	fWritePPInformation = false; //pair production flag
	fWritePVPInformation = false; //primary vertex positions

	SetSchemaDefined(false);
	rootFileOpened = false;
	gNavigator =
			G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
	fMessenger = new GEOutputHADESBEGeTestsMessenger(this);
	GeDetectorNames.clear();
}

//---------------------------------------------------------------------------//

GEOutputHADESBEGeTests::~GEOutputHADESBEGeTests() {
	if (fMessenger)
		delete fMessenger;
	if (energyInDetectors)
		delete energyInDetectors;
	GeDetectorNames.clear();
}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::DefineSchema() {
	if (!SchemaDefined()) {
		TTree *nT;
		if (fTree)
			nT = fTree;
		else {
			if (!IsMother())
				MGLog(warning)<< "No tree assigned to child output class." << endlog;
				nT = fTree = new TTree("fTree", "Germanium Detector Tree");
			}

			//Histograms
		DetEnergy = new TH1D("DetEnergy", "Energy Spectrum; E[MeV]; cnt/keV",
				3000, 0., 3.000);
		DetEnergyPP = new TH1D("DetEnergyPP",
				"Energy Spectrum Pair Production only; E[MeV]; cnt/keV", 3000,
				0., 3.000);
		DetHitsX = new TH1D("DetHitsX",
				"Detector Hits x Distrubution; x[cm];cnt/mm", 100, -5., 5.);
		DetHitsY = new TH1D("DetHitsY",
				"Detector Hits y Distrubution; y[cm];cnt/mm", 100, -5., 5.);
		DetHitsZ = new TH1D("DetHitsZ",
				"Detector Hits z Distrubution; z[cm];cnt/mm", 100, 0., 10.);

		VertexPosX = new TH1D("VertexPosX", "Vertex Position x; x[cm];cnt/mm",
				100, -5., 5.);
		VertexPosY = new TH1D("VertexPosY", "Vertex Position y; y[cm];cnt/mm",
				100, -5., 5.);
		VertexPosZ = new TH1D("VertexPosZ", "Vertex Position z; z[cm];cnt/mm",
				100, 0., 10.);
		VertexPartNum = new TH1D("VertexPartNum",
				"Vertex Particle Number; z[cm];cnt/mm", 100, 0., 10.);

		VertexDistToUpperSurface = new TH1D("VertexDistToUpperSurface",
				"Vertex Distance to Upper Surface; d[cm];cnt/mm", 50, 0., 5.);
		VertexDistToLowerSurface = new TH1D("VertexDistToLowerSurface",
				"Vertex Distance to Lower Surface; d[cm];cnt/mm", 50, 0., 5.);
		VertexDistToSideSurface = new TH1D("VertexDistToSideSurface",
				"Vertex Distance to Side Surface; d[cm];cnt/mm", 50, 0., 5.);
		VertexDistToCornerSurface = new TH1D("VertexDistToCornerSurface",
				"Vertex Distance to Corner Surface; d[cm];cnt/mm", 70, 0., 7.);
		VertexDistToDitchSurface = new TH1D("VertexDistToDitchSurface",
				"Vertex Distance to Ditch Surface; d[cm];cnt/mm", 50, 0., 5.);

		DistToUpperSurface = new TH1D("DistToUpperSurface",
				"Distance to Upper Surface; d[cm];cnt/mm", 50, 0., 5.);
		DistToLowerSurface = new TH1D("DistToLowerSurface",
				"Distance to Lower Surface; d[cm];cnt/mm", 50, 0., 5.);
		DistToSideSurface = new TH1D("DistToSideSurface",
				"Distance to Side Surface; d[cm];cnt/mm", 50, 0., 5.);
		DistToCornerSurface = new TH1D("DistToCornerSurface",
				"Distance to Corner Surface; d[cm];cnt/mm", 70, 0., 7.);
		DistToDitchSurface = new TH1D("DistToDitchSurface",
				"Distance to Ditch Surface; d[cm];cnt/mm", 50, 0., 5.);

		//Prepare output branches
		energyInDetectors = new std::vector<Float_t>;

		// hits in sensitive detectors
		nT->Branch("eventnumber", &eventnumber, "eventnumber/I");
		nT->Branch("hits_tote", &hits_tote, "hits_tote/F");
		nT->Branch("hits_totnum", &hits_totnum, "hits_totnum/I");
		nT->Branch("hits_edep", hits_edep, "hits_edep[hits_totnum]/F");

		if (fWritePPInformation) {
			nT->Branch("isPP", &isPP, "isPP/I");
		}

		if (fWriteHitCoordinates) {
			nT->Branch("hits_xpos", hits_xpos, "hits_xpos[hits_totnum]/F");
			nT->Branch("hits_ypos", hits_ypos, "hits_ypos[hits_totnum]/F");
			nT->Branch("hits_zpos", hits_zpos, "hits_zpos[hits_totnum]/F");
			nT->Branch("hits_iddet", hits_iddet, "hits_iddet[hits_totnum]/I");
		}

		if (fWriteDLInformation) {
			nT->Branch("hits_distToUpperSurf", hits_distToUpperSurf,
					"hits_distToUpperSurf[hits_totnum]/F");
			nT->Branch("hits_distToLowerSurf", hits_distToLowerSurf,
					"hits_distToLowerSurf[hits_totnum]/F");
			nT->Branch("hits_distToSideSurf", hits_distToSideSurf,
					"hits_distToSideSurf[hits_totnum]/F");
			nT->Branch("hits_distToCornerSurf", hits_distToCornerSurf,
					"hits_distToCornerSurf[hits_totnum]/F");
			nT->Branch("hits_distToDitchSurf", hits_distToDitchSurf,
					"hits_distToDitchSurf[hits_totnum]/F");
		}

		if (fWritePVPInformation) {
			nT->Branch("vertex_xpos", &vertex_xpos, "vertex_xpos/F");
			nT->Branch("vertex_ypos", &vertex_ypos, "vertex_ypos/F");
			nT->Branch("vertex_zpos", &vertex_zpos, "vertex_zpos/F");
			nT->Branch("vertex_numparticle", &vertex_numparticle,
					"vertex_numparticle/I");

			nT->Branch("vertex_distToUpperSurf", &vertex_distToUpperSurf,
					"vertex_distToUpperSurf/F");
			nT->Branch("vertex_distToLowerSurf", &vertex_distToLowerSurf,
					"vertex_distToLowerSurf/F");
			nT->Branch("vertex_distToSideSurf", &vertex_distToSideSurf,
					"vertex_distToSideSurf/F");
			nT->Branch("vertex_distToCornerSurf", &vertex_distToCornerSurf,
					"vertex_distToCornerSurf/F");
			nT->Branch("vertex_distToDitchSurf", &vertex_distToDitchSurf,
					"vertex_distToDitchSurf/F");

		}

		SetSchemaDefined(true);
	}

}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::BeginOfRunAction() {
	//The file opening is done only at the first run
	if (!rootFileOpened) {
		//Count the number of Ge Detectors in the geometry
		G4PhysicalVolumeStore* thePVStore =
				G4PhysicalVolumeStore::GetInstance();
		for (size_t i = 0; i < thePVStore->size(); i++) {
			G4VSensitiveDetector* SensDet =
					(*thePVStore)[i]->GetLogicalVolume()->GetSensitiveDetector();
			if (SensDet) {
				GeDetectorNames.push_back((*thePVStore)[i]->GetName());
			}
		}
		MGLog(trace)<< "Found: " << GeDetectorNames.size() << " sensitive detectors" << endlog;
		MGLog(trace)<< "Opening root file" << endlog;
		if (IsMother())
			OpenFile();
		//    OpenRootFile(GetFileName());
		DefineSchema();
		rootFileOpened = true;
	}
}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::BeginOfEventAction(const G4Event *event) {
	numberOfPrimaries = 0;
	isPP = 0;
	MGLog(debugging)<< "Start of event # " << event->GetEventID() << endlog;
	eventnumber = G4ToRoot(event->GetEventID());
}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::EndOfEventAction(const G4Event *evt) {
	MGLog(debugging)<< "End of event # " << evt->GetEventID() << endlog;

	this->FillHitsToNtupleTrees(evt);

	this->FillNtuple();

	if(hits_tote>0)
	{
		DetEnergy->Fill(hits_tote);
		if(isPP==1) {DetEnergyPP->Fill(hits_tote);}
	}

}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::EndOfRunAction() {
	MGLog(trace)<< "EndOfRunAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::PrepareFileForClosing() {

	DetEnergy->Write();
	DetEnergyPP->Write();
	DetHitsX->Write();
	DetHitsY->Write();
	DetHitsZ->Write();

	VertexPosX->Write();
	VertexPosY->Write();
	VertexPosZ->Write();
	VertexPartNum->Write();

	DistToUpperSurface->Write();
	DistToLowerSurface->Write();
	DistToSideSurface->Write();
	DistToCornerSurface->Write();
	DistToDitchSurface->Write();

	VertexDistToUpperSurface->Write();
	VertexDistToLowerSurface->Write();
	VertexDistToSideSurface->Write();
	VertexDistToCornerSurface->Write();
	VertexDistToDitchSurface->Write();

	DetEnergy->Delete();
	DetEnergyPP->Delete();
	DetHitsX->Delete();
	DetHitsY->Delete();
	DetHitsZ->Delete();

	DistToUpperSurface->Delete();
	DistToLowerSurface->Delete();
	DistToSideSurface->Delete();
	DistToCornerSurface->Delete();
	DistToDitchSurface->Delete();

	VertexDistToUpperSurface->Delete();
	VertexDistToLowerSurface->Delete();
	VertexDistToSideSurface->Delete();
	VertexDistToCornerSurface->Delete();
	VertexDistToDitchSurface->Delete();

}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTests::RootSteppingAction(const G4Step *step) {
	//This Stepping action kills long-lived nuclei (they do not decay)
	G4String particleType =
			step->GetTrack()->GetDefinition()->GetParticleType();

	if (particleType == "nucleus" && step->GetTrack()->GetParentID() > 0) {
	  G4double energy = step->GetTrack()->GetKineticEnergy();
	  if (energy < 0.1 * keV) {
	    G4Ions* ion = (G4Ions*) step->GetTrack()->GetDefinition();
	    G4double lifetime = ion->GetPDGLifeTime();
	    G4double excitationEnergy = ion->GetExcitationEnergy();
	    //stable and excited nuclei --> track them as usual
	    if (lifetime < 0 || excitationEnergy > 0)
	      return;
	    if (lifetime > 1.0 * microsecond) //kill long-lived nuclei
	      {
		//Notice: the StepAction is not called after step#0. Secondaries
		//are generated if RadioactiveDecay takes place at step#1
		G4TrackStatus newstatus =
		  (step->GetTrack()->GetCurrentStepNumber() > 1) ?
		  fStopAndKill : fKillTrackAndSecondaries;
		MGLog(debugging) << "Killing " <<
		  ion->GetParticleName() << " (lifetime = " << lifetime/s <<
		  "s) " << endlog;
		step->GetTrack()->SetTrackStatus(newstatus);
	      }
	    //stable nuclei are unaffected
	  }
	}
      //stable nuclei are unaffected
  
  //Check for gamma-rays produced by Compton scattering or annihilation
  
  /*
    if (fHitOutputFile)
    {
    G4String particleName =
    step->GetTrack()->GetDefinition()->GetParticleName();
    G4String volumeName = step->GetTrack()->GetVolume()->GetName();
    
    if (particleName == "alpha" && volumeName == "Substrate")
    (*fHitOutputFile) << step->GetTrack()->GetKineticEnergy()/MeV << " " <<
    step->GetStepLength()/cm << G4endl;
    }
  */
  
  /*
    G4VPhysicalVolume* detectorPre = step->GetPreStepPoint()->GetPhysicalVolume();
    G4String detPreName = "NULL";
    if (detectorPre) detPreName = detectorPre->GetName();
    
    G4VPhysicalVolume* detectorPost = step->GetPostStepPoint()->GetPhysicalVolume();
    G4String detPostName = "NULL";
    if (detectorPost) detPostName = detectorPost->GetName();
    
    if (detPreName == "physiDet" && detPostName != "physiDet")
    {
    G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int partID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4ThreeVector pos = step->GetTrack()->GetPosition();
    G4ThreeVector dir = step->GetTrack()->GetMomentumDirection();
    G4double kinE = step->GetTrack()->GetKineticEnergy();
    (*fHitOutputFile) << evID << " " << pos.x()/mm << " " << pos.y()/mm << " " <<
    pos.z()/mm << " " << dir.x() << " " << dir.y() << " " <<
    dir.z() << " " << dir.phi()/deg << " " << kinE/keV << " " << partID << endl;
    }
    }
  */
}

G4ClassificationOfNewTrack GEOutputHADESBEGeTests::StackingAction(
		const G4Track *track) {
	//This code looks for gammas emitted by nuclear decays
	//and counts how many primary particles are emitted in the decay

	G4int parentID = track->GetParentID();
	G4String particleName = track->GetDefinition()->GetParticleName();
	G4String particleType = track->GetDefinition()->GetParticleType();
	//G4double energy = 0;
	if (parentID > 0) {
		//first daugthers
		G4String processName = track->GetCreatorProcess()->GetProcessName();
		//if (processName == "RadioactiveDecay" && particleName == "gamma")
		//energy = track->GetDynamicParticle()->GetKineticEnergy();
		if (processName == "RadioactiveDecay" && parentID == 1)
			numberOfPrimaries++;
	} else if (parentID == 0) {
		if (particleType != "nucleus")
			numberOfPrimaries++;
		//if (particleName == "gamma")
		//energy = track->GetDynamicParticle()->GetKineticEnergy();
	}

	// this part searches for positrons in the event tracks
	// and sets the isPP (pair production) flag if one is found
	// For discriminating DEP events from background
	if (!isPP) //if it is already "1" do nothing!
	{
		if (track->GetDefinition()->GetParticleName() == "e+") {
			isPP = 1;
		}
	}

	return fUrgent;
}

void GEOutputHADESBEGeTests::FillHitsToNtupleTrees(const G4Event *evt) {
//
// hits in sensitive detectors 
//

	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	G4int geCollID = SDman->GetCollectionID("GermaniumHC");

	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	GEGeometrySDHitsCollection* GCHC = 0; //germanium hits

	if (HCE) {
		GCHC = (GEGeometrySDHitsCollection*) (HCE->GetHC(geCollID));
	}

	G4ThreeVector tempvector;

//  const G4RunManager* runMan = G4RunManager::GetRunManager(); 

	MGGeometryDetector* theDet =
			MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector();

	GEHADESBEGeTests* theHADESDet = dynamic_cast<GEHADESBEGeTests*>(theDet);

	hits_tote = 0.0;

	if (GCHC) {
		hits_totnum = GCHC->entries();
		if (hits_totnum > MAX_NHITS) {
			MGLog(warning)<< hits_totnum << " hits generated. Currently"
			<< " only " << MAX_NHITS <<" are supported. "
			<< " EventID : " << evt->GetEventID() << endlog;
			hits_totnum = MAX_NHITS;
		}

		for (G4int i = 0; i < hits_totnum; i++) {

			hits_tote += G4ToRoot((*GCHC)[i]->GetEdep() / MeV);
			hits_edep[i] = G4ToRoot((*GCHC)[i]->GetEdep() / MeV);
			tempvector = (*GCHC)[i]->GetPos();
			hits_xpos[i] = G4ToRoot(tempvector.getX() / cm);
			hits_ypos[i] = G4ToRoot(tempvector.getY() / cm);
			hits_zpos[i] = G4ToRoot(tempvector.getZ() / cm);
			hits_iddet[i] = (*GCHC)[i]->GetCopynumber();

			hits_distToUpperSurf[i] = G4ToRoot(
					theHADESDet->GetDistToUpperSurf(tempvector) / cm);
			hits_distToLowerSurf[i] = G4ToRoot(
					theHADESDet->GetDistToLowerSurf(tempvector) / cm);
			hits_distToSideSurf[i] = G4ToRoot(
					theHADESDet->GetDistToSideSurf(tempvector) / cm);
			hits_distToCornerSurf[i] = G4ToRoot(
					theHADESDet->GetDistToCornerSurf(tempvector) / cm);
			hits_distToDitchSurf[i] = G4ToRoot(
					theHADESDet->GetDistToDitchSurf(tempvector) / cm);

			//filling histograms
			DetHitsX->Fill(hits_xpos[i]);
			DetHitsY->Fill(hits_ypos[i]);
			DetHitsZ->Fill(hits_zpos[i]);

			DistToUpperSurface->Fill(hits_distToUpperSurf[i]);
			DistToLowerSurface->Fill(hits_distToLowerSurf[i]);
			DistToSideSurface->Fill(hits_distToSideSurf[i]);
			DistToCornerSurface->Fill(hits_distToCornerSurf[i]);
			DistToDitchSurface->Fill(hits_distToDitchSurf[i]);

		}

		// fill primary vertex positions
		G4PrimaryVertex *primaryVertex;

		primaryVertex = evt->GetPrimaryVertex(0); // only one considered. see std GERDA class for getting array of primary vertices
		if (!primaryVertex) {
			MGLog(error)<< "Bad primary vertex pointer." << endlog;
			MGLog(fatal) << endlog;
		}

		vertex_xpos = G4ToRoot(primaryVertex->GetX0() / cm);
		vertex_ypos = G4ToRoot(primaryVertex->GetY0() / cm);
		vertex_zpos = G4ToRoot(primaryVertex->GetZ0() / cm);
		vertex_numparticle = primaryVertex->GetNumberOfParticle();

		G4ThreeVector vertexvector = primaryVertex->GetPosition();

		vertex_distToUpperSurf = G4ToRoot(
				theHADESDet->GetDistToUpperSurf(vertexvector) / cm);
		vertex_distToLowerSurf = G4ToRoot(
				theHADESDet->GetDistToLowerSurf(vertexvector) / cm);
		vertex_distToSideSurf = G4ToRoot(
				theHADESDet->GetDistToSideSurf(vertexvector) / cm);
		vertex_distToCornerSurf = G4ToRoot(
				theHADESDet->GetDistToCornerSurf(vertexvector) / cm);
		vertex_distToDitchSurf = G4ToRoot(
				theHADESDet->GetDistToDitchSurf(vertexvector) / cm);

		VertexDistToUpperSurface->Fill(vertex_distToUpperSurf);
		VertexDistToLowerSurface->Fill(vertex_distToLowerSurf);
		VertexDistToSideSurface->Fill(vertex_distToSideSurf);
		VertexDistToCornerSurface->Fill(vertex_distToCornerSurf);
		VertexDistToDitchSurface->Fill(vertex_distToDitchSurf);

		VertexPosX->Fill(vertex_xpos);
		VertexPosY->Fill(vertex_ypos);
		VertexPosZ->Fill(vertex_zpos);
		VertexPartNum->Fill(vertex_numparticle);

	}

	MGLog(debugging)<< "Energy in germanium " << hits_tote/keV << " keV" << endlog;

}

void GEOutputHADESBEGeTests::FillNtuple() {

	if (hits_tote > 0.0) {
		FillTree();
	}

}

