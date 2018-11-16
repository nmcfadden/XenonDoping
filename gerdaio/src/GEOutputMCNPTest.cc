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
// $Id: GEOutputMCNPTest.cc,v 1.2 2006-12-14 12:24:08 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  GEOutputMCNPTest.cc
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
 * AUTHOR:  Luciano Pandola
 * CONTACT:
 * FIRST SUBMISSION:
 *  01-18-2006
 * REVISION:
 *  12-13-2006 Revised, removed useless pieces of code
 *                       
 */
//---------------------------------------------------------------------------//
//
// include files 
#include "G4Ions.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputMCNPTest.hh"    
#include "gerdaio/GETrajectory.hh"    

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputMCNPTest::GEOutputMCNPTest()
{
  fNeutronPathLenght = 0.0*cm;
}


GEOutputMCNPTest::~GEOutputMCNPTest()
{;}


void GEOutputMCNPTest::BeginOfRunAction()
{
  OpenFile();
  //This avoids the file to be appended at the start of a new job
  G4String fluxFileName = GetFileName()+".flu";
  std::ofstream fluxfile(fluxFileName);
  fluxfile.close(); 
}


void GEOutputMCNPTest::BeginOfEventAction(const G4Event*)
{
  fNeutronPathLenght = 0.0*cm;
}

void GEOutputMCNPTest::EndOfEventAction(const G4Event*)  
{
  fFile << fNeutronPathLenght/cm << G4endl;
}

void GEOutputMCNPTest::EndOfRunAction()
{
  CloseFile();
}


void GEOutputMCNPTest::SteppingAction(const G4Step *step,G4SteppingManager*)
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

  // Check for whatever falls into the crystal region
  // The "crystal region" is defined as the volume "CrystalRegion"
  //of the GEMCNPTest.cc geometry. The default radius is 25 cm

  if (step->GetTrack()->GetVolume()->GetName() == "CrystalRegion")
    {
      if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition())
	{
	  fNeutronPathLenght += step->GetStepLength(); 
	}
    }
  
  //moving from one volume to the other
  if (step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "CryoliquidBuffer" && 
      step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "CrystalRegion")
    {
      G4double energy = step->GetTrack()->GetKineticEnergy();
      G4int ID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
      G4String fluxFileName = GetFileName()+".flu";
      std::ofstream fluxfile(fluxFileName,std::ios::app);
      fluxfile << ID << " " << energy/MeV << G4endl;
      fluxfile.close();
    }
}

G4ClassificationOfNewTrack GEOutputMCNPTest::StackingAction(const G4Track*)
{ 
  return fUrgent;
}

