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
//      
// CLASS IMPLEMENTATION:  GEOutputDetectorEfficiency.cc
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
 * AUTHOR:  Luciano
 * CONTACT:
 * FIRST SUBMISSION:
 *  12-12-2006, Luciano
 * REVISION: 02-20-2007, Luciano  Changed the definition of full-energy-deposit. It is now
 *                                defined as 0.1% with respect to the true energy. In the 
 *                                future, we'll need a proper messenger
 *                       
 */
//---------------------------------------------------------------------------//
//
// include files 
#include "G4Ions.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4SDManager.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputDetectorEfficiency.hh"    
#include "gerdaio/GETrajectory.hh"    

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputDetectorEfficiency::GEOutputDetectorEfficiency()
{
  nRunNumber = 0;
  energyPrimaryRead = false;
  nEvents = 0;
  nEventsInteracting=0;
  nEventsFullEnergy=0;
  SetFileName("output_eff.dat");
}


GEOutputDetectorEfficiency::~GEOutputDetectorEfficiency()
{
  CloseFile();
}


void GEOutputDetectorEfficiency::BeginOfRunAction()
{
  MGLog(trace) << "Starting run #" << nRunNumber << endlog;
  if (!nRunNumber)
    OpenFile();
  nRunNumber++;
  energyPrimaryRead = false;
  nEvents = 0;
  nEventsInteracting=0;
  nEventsFullEnergy=0;
}


void GEOutputDetectorEfficiency::BeginOfEventAction(const G4Event*)
{
  nEvents++;
}

void GEOutputDetectorEfficiency::EndOfEventAction(const G4Event* evt)  
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
 
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
 
  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
  }
  G4double depositedEnergy = 0;
  for (G4int k=0;k<GCHC->entries();k++)
    {
      depositedEnergy += (*GCHC)[k]->GetEdep();
    }
  if (depositedEnergy > 0)
    {
      nEventsInteracting++;
    }
  //MGLog(debugging) << "Event " << evt->GetEventID() << " " << depositedEnergy/keV << G4endl;
  if (depositedEnergy>(primaryEnergy*0.999) && depositedEnergy < (primaryEnergy*1.001))
    {
      nEventsFullEnergy++;
    }
}

void GEOutputDetectorEfficiency::EndOfRunAction()
{
  fFile << primaryEnergy/keV << " " << nEvents << " " << nEventsInteracting << " " << 
    nEventsFullEnergy << std::endl;
}


void GEOutputDetectorEfficiency::SteppingAction(const G4Step *step,G4SteppingManager*)
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

G4ClassificationOfNewTrack GEOutputDetectorEfficiency::StackingAction(const G4Track* track)
{ 
    //In the case the primary energy has not been read yet --> read it!
  if (!energyPrimaryRead)
    {
      if (track->GetParentID() == 0)
	{
	  primaryEnergy = track->GetKineticEnergy();
	  energyPrimaryRead = true;
	}
    }
  return fUrgent;
}

