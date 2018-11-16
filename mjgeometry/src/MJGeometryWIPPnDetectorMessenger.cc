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
// $Id: MJGeometryWIPPnDetectorMessenger.cc,v 1.1 2009-05-29 17:12:56 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryWIPPnDetectorMessenger.cc
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
 * AUTHOR: Rob Johnson
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 17 13:09:53 PST 2006
 * 
 * REVISION:
 *
 * 02-23-2007, Initial submission, R. Johnson
 * 20 May, 2009, Combine MJGeometryWIPPn and MJGeometryWIPPnNewShield into one.
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "mjgeometry/MJGeometryWIPPnDetector.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryWIPPnDetectorMessenger.hh"      //Present MJ Class Headers 

//---------------------------------------------------------------------------//

MJGeometryWIPPnDetectorMessenger::MJGeometryWIPPnDetectorMessenger(MJGeometryWIPPnDetector* newShield):
fWIPPnDetector( newShield )
{
  fWIPPnDetectorDir = new G4UIdirectory("/MG/geometry/WIPPn/");
 
  // turn the use of samples on or off
  fUseSampleCmd = 
    new G4UIcmdWithABool("/MG/geometry/WIPPn/useSample",this);
  fUseSampleCmd->SetGuidance("Command to insert a sample into geometry.");
  
 // choose which sample to insert into geometry
  fChooseSampleCmd = new G4UIcmdWithAString(
    "/MG/geometry/WIPPn/chooseSample",this);
  fChooseSampleCmd->SetGuidance("Select sample");
  fChooseSampleCmd->SetCandidates("CuBalls CuSO4 Beaker CTFG Foils PbFoils Button");
  fChooseSampleCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
  

  // choose which material to use in the button sample
  fChooseButtonMatCmd = new G4UIcmdWithAString(
    "/MG/geometry/WIPPn/chooseButtonMat",this);
  fChooseButtonMatCmd->SetGuidance("Select material for button source sample");
  fChooseButtonMatCmd->SetCandidates("Sodium Manganese Cobalt Cesium");
  fChooseButtonMatCmd->AvailableForStates( G4State_PreInit, G4State_Idle);


  // choose the shield configuration
   fChooseShieldCmd = new G4UIcmdWithAString(
     "/MG/geometry/WIPPn/chooseShield",this);
   fChooseShieldCmd->SetGuidance("Select Shield Configuration");
   fChooseShieldCmd->SetCandidates("copper lead");
   fChooseShieldCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
  
  // choose the desired thickness of the cryostat
  fSetCryostatThicknessCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/WIPPn/cryostatThickness", this);
  fSetCryostatThicknessCmd->SetGuidance("Set thickness of cryostat");
  fSetCryostatThicknessCmd->SetDefaultUnit("cm");
  fSetCryostatThicknessCmd->SetUnitCandidates("m cm mm micrometer");
  
  // choose the desired distance from crystal to cryostat
  fSetCryoXtalDistanceCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/WIPPn/cryostatCrystalDistance", this);
  fSetCryoXtalDistanceCmd->SetGuidance("Set distance from cryostat to crystal");
  fSetCryoXtalDistanceCmd->SetDefaultUnit("cm");
  fSetCryoXtalDistanceCmd->SetUnitCandidates("m cm mm micrometer");
}

MJGeometryWIPPnDetectorMessenger::~MJGeometryWIPPnDetectorMessenger()
{
  delete fWIPPnDetectorDir;
  delete fUseSampleCmd;
  delete fChooseSampleCmd;
  delete fChooseButtonMatCmd;
  delete fChooseShieldCmd;
  delete fSetCryoXtalDistanceCmd;
  delete fSetCryostatThicknessCmd;
}

void MJGeometryWIPPnDetectorMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  if (command == fUseSampleCmd) 
  {
    fWIPPnDetector->SetUseSamples(
      fUseSampleCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Inserted Sample." << endlog; 
  }
  else if (command == fChooseSampleCmd)
  {
    fWIPPnDetector->SetSample( newValues );
  }
  else if (command == fChooseButtonMatCmd)
  {
    fWIPPnDetector->SetButtonMat( newValues );
  }
  else if (command == fChooseShieldCmd)
  {
    fWIPPnDetector->SetShieldConfiguration( newValues );
  }
  else if (command == fSetCryoXtalDistanceCmd)
  {
    fWIPPnDetector->SetCryostatCrystalDistance(
    fSetCryoXtalDistanceCmd->GetNewDoubleValue(newValues) );
  }
  else if (command == fSetCryostatThicknessCmd)
  {
    fWIPPnDetector->SetCryostatThickness(
    fSetCryostatThicknessCmd->GetNewDoubleValue(newValues) );
  }

}

