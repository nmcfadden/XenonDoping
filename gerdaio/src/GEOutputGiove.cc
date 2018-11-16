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
/**
 * REVISION:
 *  June 24, 2009 Alan Poon	First version
 *                 
 */
//---------------------------------------------------------------------------//
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
#include "G4Run.hh"
#include "G4Event.hh"
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

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputGiove.hh"    
#include "gerdaio/GETrajectory.hh"    
#include "gerdaio/GEOutputGioveMessenger.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputGiove::GEOutputGiove():
  MGOutputRoot(true),gNavigator(0),energyHisto(0),primaryGammaHisto(0),
  fHitOutputFile(0)
{
  SetSchemaDefined(false);
  rootFileOpened = false;
  fWriteHitDetailFlag = false;
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fMessenger = new GEOutputGioveMessenger(this);
}

//---------------------------------------------------------------------------//

GEOutputGiove::~GEOutputGiove()
{
  if (fMessenger) delete fMessenger;
}

//---------------------------------------------------------------------------//

void GEOutputGiove::DefineSchema()
{
  if(!SchemaDefined()){
    TTree *nT;
    if(fTree)
       nT = fTree;
    else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "Germanium Detector Tree");
    }
    
    //Histograms
    energyHisto = new TH1D("h1","ADC spectrum",8192,0.,8191.);
    //8k channels, 1 bin = 1 keV

    primaryGammaHisto = new TH1D("h2","Primary gammas",8192,0.,8191.);
    //1 bin = 1 keV
	  
	  neutronEnergyHisto = new TH1D("hneutron","energy spectrum of neutrons entering the Ge crystal", 1200,-4.,8.);

    nT->Branch("eventnumber",&eventnumber,"eventnumber/I");
    nT->Branch("energy",&hits_tote,"hits_tote/F");

    SetSchemaDefined(true);
  }

  //create a file
  if (fWriteHitDetailFlag)
    {
      if (!fHitOutputFile)
	fHitOutputFile = new std::ofstream();
      G4String theFile = GetFileName()+".hits";
      fHitOutputFile->open(theFile);
    }
}

//---------------------------------------------------------------------------//

void GEOutputGiove::BeginOfRunAction()
{
  //The file opening is done only at the first run
  if (!rootFileOpened)				
    {
      MGLog(trace) << "Opening root file" << endlog;
      if(IsMother()) OpenFile();
      //    OpenRootFile(GetFileName());
      DefineSchema();
      rootFileOpened = true;
    }
}

//---------------------------------------------------------------------------//

void GEOutputGiove::BeginOfEventAction(const G4Event *event)
{
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
  eventnumber = G4ToRoot(event->GetEventID());
}

//---------------------------------------------------------------------------//

void GEOutputGiove::EndOfEventAction(const G4Event *evt)  
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;

//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits


  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
  }


  G4ThreeVector tempvector;
  
  hits_tote = 0.0;
  
  if (GCHC){
    hits_totnum = GCHC->entries();  
    for (G4int i=0; i<hits_totnum; i++) {
	 G4String materialName = (*GCHC)[i]->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
 	  if (materialName == "NaturalGe" || materialName == "EnrichedGe")
	  hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
	}
   }
       
  energyHisto->Fill((Double_t) (hits_tote/keV));
  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;

  if (IsMother()) FillTree();
  //  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

  if (fWriteHitDetailFlag)
    {
      G4int eveID =  evt->GetEventID();
      if (GCHC)
	{
	  if (hits_totnum)
	    {
	      (*fHitOutputFile) << eveID << " " << hits_totnum << G4endl;
	      for (G4int i=0; i<hits_totnum; i++) 
		{
		  G4ThreeVector position = (*GCHC)[i]->GetPos();
		  G4double time = (*GCHC)[i]->GetHittime();
		  G4double edep = (*GCHC)[i]->GetEdep();
		  G4int particleID = (*GCHC)[i]->GetTrackPDG();
		  //Something to do here
		  if (!(fHitOutputFile->is_open()))
		    {
		      MGLog(error) << "Something wrong with the .hits output file" << endlog;
		    }
		  else
		    {
		      (*fHitOutputFile) << position.x()/cm << " " << position.y()/cm << " " << 
			position.z()/cm << " " << edep/keV << " " << time/ns << " " << 
			particleID << " " << (*GCHC)[i]->GetPhysicalVolume()->GetName() << G4endl;
		    }
		}
	    }
	}
      
    }
}

//---------------------------------------------------------------------------//

void GEOutputGiove::EndOfRunAction()
{
    MGLog(trace) << "EndOfRunAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void GEOutputGiove::PrepareFileForClosing()
{
  energyHisto->Write();
  if (primaryGammaHisto->GetEntries()) //write histogram only if it is not empty
    primaryGammaHisto->Write();

	neutronEnergyHisto->Write();

  //dump the information on a ASCII file
  G4String  fileName = GetFileName()+".dat";
  std::ofstream file(fileName);
  for (Int_t i=1;i<=energyHisto->GetNbinsX();i++)
    {
      file << i << " " << energyHisto->GetBinContent(i) << " " << 
	primaryGammaHisto->GetBinContent(i) << endl;
    }
  file.close();

  energyHisto->Delete();
  primaryGammaHisto->Delete();

  //close the file
  if (fHitOutputFile)
    {
      fHitOutputFile->close();
      delete fHitOutputFile;
    }

}

//---------------------------------------------------------------------------//

void GEOutputGiove::RootSteppingAction(const G4Step *step)
{
	
  // track how many neutrons entering the Germanium 	
  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName(); 
  if (particleName =="neutron"){
		G4StepPoint* point1 = step->GetPreStepPoint();
		if (point1->GetStepStatus() == fGeomBoundary) {  // neutron crossing boundary
			G4TouchableHandle touch1 = point1->GetTouchableHandle();
			G4VPhysicalVolume* volume = touch1->GetVolume();
			if ( (volume->GetName())=="crystal" ){   // neutron is in PHYSICAL VOLUME Ge crystal
				G4double energy = step->GetTrack()->GetKineticEnergy();
				if (energy>0.) {
					neutronEnergyHisto->Fill( (Double_t) log10(energy/eV) );
				}
//				cout << energy << endl;
			}
		}
  }
	
  //This Stepping action kills long-lived nuclei (they do not decay)
  G4String particleType = step->GetTrack()->GetDefinition()->GetParticleType(); 

  if (particleType == "nucleus" && step->GetTrack()->GetParentID()>0)
    {
      G4double energy = step->GetTrack()->GetKineticEnergy();
      if (energy < 0.1*keV)
        {
          G4Ions* ion = (G4Ions*) step->GetTrack()->GetDefinition();
          G4double lifetime = ion->GetPDGLifeTime();
          G4double excitationEnergy = ion->GetExcitationEnergy();
          //stable and excited nuclei --> track them as usual
          if (lifetime < 0 || excitationEnergy > 0) return;
          if (lifetime > 1.0*microsecond) //kill long-lived nuclei
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

}

G4ClassificationOfNewTrack GEOutputGiove::StackingAction(const G4Track *track)
{
  //This code looks for gammas emitted by nuclear decays
  G4int parentID = track->GetParentID();
  G4String particleName =  track->GetDefinition()->GetParticleName();;
  G4double energy = 0;
  if (parentID > 0)
    {
      //first daugthers
      G4String processName = track->GetCreatorProcess()->GetProcessName();
      if (processName == "RadioactiveDecay" && particleName == "gamma")
	energy = track->GetDynamicParticle()->GetKineticEnergy();
    }
  else if (parentID == 0 && particleName == "gamma")
    {
      energy = track->GetDynamicParticle()->GetKineticEnergy();
    }
    
  if (energy > 0)
    {
      primaryGammaHisto->Fill((Double_t) (energy/keV));
    }
  //if (parentID > 0) return kKill;
  return fUrgent;
}












