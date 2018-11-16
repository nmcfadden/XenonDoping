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
// $Id: GEOutputNeutronYield.cc,v 1.2 2007-07-20 08:45:58 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  GEOutputNeutronYield.cc
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
 *  10-18-2006 
 *
 * REVISION:
 *   12-13-2006 Revised, Removed useless members and methods, Luciano
 */
//---------------------------------------------------------------------------//
//
// include files 
#include "G4Ions.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputNeutronYield.hh"    
#include "gerdaio/GETrajectory.hh"    

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputNeutronYield::GEOutputNeutronYield()
{
  oldParentID=-1;
  nRunNumber = 0;
  energyPrimaryRead = false;
  primaryEnergy = 0;
}


GEOutputNeutronYield::~GEOutputNeutronYield()
{
  CloseFile();
}


void GEOutputNeutronYield::BeginOfRunAction()
{
  MGLog(trace) << "Starting run #" << nRunNumber << endlog;
  if (!nRunNumber)
    OpenFile();
   nRunNumber++;
   energyPrimaryRead = false;
}


void GEOutputNeutronYield::BeginOfEventAction(const G4Event*)
{
  oldParentID=-1;
}

void GEOutputNeutronYield::EndOfEventAction(const G4Event*)  
{;}

void GEOutputNeutronYield::EndOfRunAction()
{;}


void GEOutputNeutronYield::SteppingAction(const G4Step *step,G4SteppingManager*)
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

G4ClassificationOfNewTrack GEOutputNeutronYield::StackingAction(const G4Track* track)
{ 
  //Here check for neutrons and write where they are produced
  G4int parentID = track->GetParentID();
  G4String particleName = track->GetDefinition()->GetParticleName();
  if (parentID > 0) 
    {
      G4String processName = track->GetCreatorProcess()->GetProcessName();
      if (particleName == "neutron")
	{
	  if (parentID != oldParentID) //write, avoiding repetitions
	    {
	      if (processName != "NeutronInelastic")
		fFile <<  primaryEnergy/GeV << " " << 
		  track->GetPosition().z()/cm << G4endl;
	      //<< " " << processName << " " << parentID << G4endl;
	      oldParentID = parentID;
	    }
	}
    }
  //In the case the primary energy has not been read yet --> read it!
  if (!energyPrimaryRead && !parentID)
    {
      if (particleName != "neutron") //with neutrons it does not work
	primaryEnergy = track->GetKineticEnergy();
      else
	primaryEnergy = (G4double) nRunNumber;
      energyPrimaryRead = true;
    }
  return fUrgent;
}

