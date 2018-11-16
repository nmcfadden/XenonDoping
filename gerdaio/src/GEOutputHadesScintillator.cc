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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 29-06-2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

// include files for ROOT

#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TNtuple.h"
#include "TMath.h"

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

// MaGe includes 

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputHadesScintillator.hh"    
#include "gerdaio/GETrajectory.hh"    

// name space 

using namespace std;

using namespace CLHEP;

// ---------------------------------------------------------------------------


GEOutputHadesScintillator::GEOutputHadesScintillator(): MGOutputRoot(true), gNavigator(0) 
{

  SetSchemaDefined(false);

  gNavigator = G4TransportationManager::GetTransportationManager() -> GetNavigatorForTracking();

}

// ---------------------------------------------------------------------------

GEOutputHadesScintillator::~GEOutputHadesScintillator()
{
  
  ;

}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::DefineSchema()
{

  if(!SchemaDefined())
    {
    TTree* nT;

    if(fTree)
       nT = fTree;

    else 
      {
	if(!IsMother())
	  MGLog(warning) << "No tree assigned to child output class." << endlog;

	nT = fTree = new TTree("fTree", "Germanium Detector Tree");
      }

    // general information 
    
    nT -> Branch("eventnumber",         &eventnumber,         "eventnumber/I");

    // deposited energy 

    nT -> Branch("energy_gedetector1",  &energy_gedetector1,    "energy_gedetector1/F");
    nT -> Branch("energy_gedetector2",  &energy_gedetector2,    "energy_gedetector2/F");
    nT -> Branch("energy_scintillator1", &energy_scintillator1, "energy_scintillator1/F");
    nT -> Branch("energy_scintillator2", &energy_scintillator2, "energy_scintillator2/F");

    // true Monte Carlo vertex and particle information 

    // MC true vertex information

    nT -> Branch("vertex_totnum",     &vertex_nvertices,  "vertex_nvertices/I");
    nT -> Branch("vertex_xpos",        vertex_xpos,       "vertex_xpos[vertex_nvertices]/F");
    nT -> Branch("vertex_ypos",        vertex_ypos,       "vertex_ypos[vertex_nvertices]/F");
    nT -> Branch("vertex_zpos",        vertex_zpos,       "vertex_zpos[vertex_nvertices]/F");
    nT -> Branch("vertex_time",        vertex_time,       "vertex_time[vertex_nvertices]/F");
    nT -> Branch("vertex_nparticles",  vertex_nparticles, "vertex_nparticles[vertex_nvertices]/I");

    nT -> Branch("mc_nparticles", &mc_nparticles, "mc_nparticles/I");
    nT -> Branch("mc_pdgcode",     mc_pdgcode,    "mc_pdgcode[mc_nparticles]/I");
    nT -> Branch("mc_ivertex",     mc_ivertex,    "mc_ivertex[mc_nparticles]/F"); 
    nT -> Branch("mc_px",          mc_px,         "mc_px[mc_nparticles]/F");
    nT -> Branch("mc_py",          mc_py,         "mc_py[mc_nparticles]/F");
    nT -> Branch("mc_pz",          mc_pz,         "mc_pz[mc_nparticles]/F");
    nT -> Branch("mc_pe",          mc_pe,         "mc_pe[mc_nparticles]/F");
    nT -> Branch("mc_id",          mc_id,         "mc_id[mc_nparticles]/I");

    SetSchemaDefined(true);
  }
}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::BeginOfRunAction()
{

  // open file 

  if(IsMother()) 
    OpenFile();

  // define the outout scheme 

  DefineSchema(); 

}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::BeginOfEventAction(const G4Event *event)
{

  // output to logger 

  MGLog(debugging) << "Start of event # " << event -> GetEventID() << endlog;

  // get event number 

  eventnumber = G4ToRoot(event -> GetEventID());

  // get true Monte Carlo particle information 

  G4PrimaryParticle *primaryParticle;
  G4PrimaryVertex   *primaryVertex;

  vertex_nvertices = event -> GetNumberOfPrimaryVertex();

  if(vertex_nvertices > MAX_NVERTICES) 
    {    
      MGLog(warning) << vertex_nvertices<< "vertex generated. Currently"
		     << " only " << MAX_NVERTICES << " are supported. "
		     << " EventID : " << event->GetEventID() << endlog;
      vertex_nvertices = MAX_NVERTICES;
    }

  // reset number of particles 

  mc_nparticles=0;

  // loop over vertices 

  for(G4int i = 0; i < vertex_nvertices; i++) 
    {
      // get vertex 

      primaryVertex = event->GetPrimaryVertex(i);

      // check 
      
      if(!primaryVertex) 
	{
	  MGLog(error) << "Bad primary vertex pointer." << endlog;
	  MGLog(fatal) << endlog;
	}

      vertex_xpos[i]        = G4ToRoot(primaryVertex -> GetX0()/cm);
      vertex_ypos[i]        = G4ToRoot(primaryVertex -> GetY0()/cm);
      vertex_zpos[i]        = G4ToRoot(primaryVertex -> GetZ0()/cm);
      vertex_time[i]        = G4ToRoot(primaryVertex -> GetT0()/ns);
      vertex_nparticles[i]  = primaryVertex -> GetNumberOfParticle();

      // loop over particles 

    for (G4int j = 0; j < vertex_nparticles[i]; j++) 
      {
	// get primary particle 

	primaryParticle = primaryVertex -> GetPrimary(j);

	// check 

	if(!primaryParticle) 
	  {
	    MGLog(error) << "Bad primary particle pointer." << endlog;
	    MGLog(fatal) << endlog;
	  }

	mc_pdgcode[mc_nparticles] = G4ToRoot(primaryParticle -> GetPDGcode());
	mc_px[mc_nparticles]      = G4ToRoot(primaryParticle -> GetPx() / GeV);
	mc_py[mc_nparticles]      = G4ToRoot(primaryParticle -> GetPy() / GeV);
	mc_pz[mc_nparticles]      = G4ToRoot(primaryParticle -> GetPz() / GeV);

	Float_t energy = TMath::Sqrt(mc_px[mc_nparticles] * mc_px[mc_nparticles] 
				     + mc_py[mc_nparticles] * mc_py[mc_nparticles] 
				     + mc_pz[mc_nparticles] * mc_pz[mc_nparticles] 
				     + (primaryParticle -> GetMass() / GeV) * (primaryParticle -> GetMass() / GeV)); 
	
	mc_pe[mc_nparticles]      = G4ToRoot(energy);
	mc_ivertex[mc_nparticles] = i;
	
	// increase number of particles 
	
	mc_nparticles++;
      }
    }
  
  // get event number 
  
  eventnumber = G4ToRoot(event->GetEventID());

}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::EndOfEventAction(const G4Event *event)  
{

  // output to logger 

  MGLog(debugging) << "End of event # " << event -> GetEventID() << endlog;

  //
  // hits in sensitive detectors 
  //

  // get SD manager 

  G4SDManager * SDmanager = G4SDManager::GetSDMpointer();
  
  // get hits collections 

  G4int geCollID    = SDmanager -> GetCollectionID("GermaniumHC");
  G4int scintCollID = SDmanager -> GetCollectionID("ScintHC");

  G4HCofThisEvent* HCE = event -> GetHCofThisEvent();

  GEGeometrySDHitsCollection* GCHC = 0; // germanium hits
  GEGeometrySDHitsCollection* PSHC = 0; // scintillator hits

  if (HCE) 
    {
      GCHC = (GEGeometrySDHitsCollection*) (HCE -> GetHC(geCollID));
      PSHC = (GEGeometrySDHitsCollection*) (HCE -> GetHC(scintCollID));
    }
  
  // energy in germanium 

  energy_gedetector1 = 0.0; 
  energy_gedetector2 = 0.0; 

  if (GCHC)
    {
      for (G4int i = 0; i < GCHC -> entries(); i++) 
	{
	  if ((*GCHC)[i] -> GetCopynumber() == 0)
	    energy_gedetector1 += G4ToRoot((*GCHC)[i] -> GetEdep() / GeV); 
	  
	  else if ((*GCHC)[i] -> GetCopynumber() == 1)
	    energy_gedetector2 += G4ToRoot((*GCHC)[i] -> GetEdep() / GeV); 
	}
    }

  // energy in scintillator 

  energy_scintillator1 = 0.0; 
  energy_scintillator2 = 0.0; 

  if (PSHC)
    {
      for (G4int i = 0; i < PSHC -> entries(); i++) 
	{
	  if ((*PSHC)[i] -> GetCopynumber() == 0)
	  energy_scintillator1 += G4ToRoot((*PSHC)[i] -> GetEdep() / GeV); 

	  if ((*PSHC)[i] -> GetCopynumber() == 1)
	  energy_scintillator2 += G4ToRoot((*PSHC)[i] -> GetEdep() / GeV); 
	}
    }

  // fill tree 

  if (IsMother()) FillTree();

  // output to logger 

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::EndOfRunAction()
{

  // close file 

  if(IsMother()) CloseFile();

  // output to logger 

  MGLog(trace) << "EndOfRunAction finished." << endlog;

}

// ---------------------------------------------------------------------------

void GEOutputHadesScintillator::RootSteppingAction(const G4Step *step)
{
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

// ---------------------------------------------------------------------------

G4ClassificationOfNewTrack GEOutputHadesScintillator::StackingAction(const G4Track * /*track*/)
{
  return fUrgent;
}











