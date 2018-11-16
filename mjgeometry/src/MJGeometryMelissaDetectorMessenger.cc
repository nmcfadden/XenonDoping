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
//      
// CLASS IMPLEMENTATION:  MJGeometryMelissaDetectorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  Modeled after MJGeometryWIPPnNewShieldMessenger.cc
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: K. Macon
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "mjgeometry/MJGeometryMelissaDetector.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryMelissaDetectorMessenger.hh"  

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryMelissaDetectorMessenger::MJGeometryMelissaDetectorMessenger(MJGeometryMelissaDetector* Melissa):
fMelissaDetector( Melissa )
{
  fMelissaDetectorDir = new G4UIdirectory("/MG/geometry/MelissaDetector/");
 
  // turn the use of samples on or off
  fUseSampleCmd = 
    new G4UIcmdWithABool("/MG/geometry/MelissaDetector/useSample",this);
  fUseSampleCmd->SetGuidance("Command to insert a sample into geometry.");
  
  // choose which sample to insert into geometry
  fChooseSampleCmd = new G4UIcmdWithAString(
    "/MG/geometry/MelissaDetector/chooseSample",this);
  fChooseSampleCmd->SetGuidance("Select sample");

  G4String candidates = "RockBucket RockBag Steel SuperInsulation ";
  candidates += "Teflon AlRings CalibrationSource PMT Zeolite FoamInsulation Marinelli ";
  fChooseSampleCmd->SetCandidates(candidates);
  fChooseSampleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPlaceSampleCmd = new G4UIcmdWithAString(
    "/MG/geometry/MelissaDetector/placeSample",this);
  fPlaceSampleCmd->SetGuidance("Pick Sample Placement");
  fPlaceSampleCmd->SetCandidates("Default OnTop InCorner");
  fPlaceSampleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

MJGeometryMelissaDetectorMessenger::~MJGeometryMelissaDetectorMessenger()
{
  delete fMelissaDetectorDir;
  delete fUseSampleCmd;
  delete fChooseSampleCmd;
  delete fPlaceSampleCmd;
}

void MJGeometryMelissaDetectorMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  if (command == fUseSampleCmd) 
  {
    fMelissaDetector->SetUseSample(
      fUseSampleCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Inserted Sample." << endlog; 
  }
  else if (command == fChooseSampleCmd)
  {
    fMelissaDetector->SetSample( newValues );
  }
  else if (command == fPlaceSampleCmd)
  {
   fMelissaDetector->SetPlacement(newValues);
    MGLog(trace) << "Using Specified Placement Vector." << endlog;
  }
}
