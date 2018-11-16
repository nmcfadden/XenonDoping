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
// $Id: TSCylinderShieldTankMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  TSCylinderShieldTankMessenger.cc
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
  * AUTHOR: R.Martin
 * CONTACT: 
 * FIRST SUBMISSION: 11 Mar 2011
 * 
 * REVISION:
 * 
 * 03-11-2011 R. Martin created
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

#include "tonscalegeometry/TSCylinderShieldTank.hh" 

//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSCylinderShieldTankMessenger.hh" 

// -----------------------------------------------------------------------

TSCylinderShieldTankMessenger::TSCylinderShieldTankMessenger(TSCylinderShieldTank *Tank) : fTank(Tank)
{
  // /MG/geometry/TSCylinderShieldTank
  fDirectory = new G4UIdirectory("/MG/geometry/TSCylinderShieldTank/");

  // /MG/geometry/SCylinderShieldTank/heigh
  fTankHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSCylinderShieldTank/height", this);
  fTankHeightCmd->SetGuidance("Set height of tank");
  fTankHeightCmd->SetDefaultUnit("m");
  fTankHeightCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/TSCylinderShieldTank/radius
  fTankRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSCylinderShieldTank/radius", this);
  fTankRadiusCmd->SetGuidance("Set radius of tank");
  fTankRadiusCmd->SetDefaultUnit("m");
  fTankRadiusCmd->SetUnitCandidates("m cm mm micrometer");
  
  fTankWallThicknessCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSCylinderShieldTank/tankwallthickness", this);
  fTankWallThicknessCmd->SetGuidance("Set radius of tank");
  fTankWallThicknessCmd->SetDefaultUnit("m");
  fTankWallThicknessCmd->SetUnitCandidates("m cm mm micrometer");
 
  // /MG/geometry/TSCylinderShieldTank/shieldMaterial
  fShieldMaterialCmd = new G4UIcmdWithAString("/MG/geometry/TSCylinderShieldTank/shieldmaterial", this);
  fShieldMaterialCmd->SetGuidance("Material name for shielding.");  
  fShieldMaterialCmd->AvailableForStates(G4State_PreInit);

  fTankWallMaterialCmd = new G4UIcmdWithAString("/MG/geometry/TSCylinderShieldTank/tankwallmaterial", this);
  fTankWallMaterialCmd->SetGuidance("Material name for tank wall.");  
  fTankWallMaterialCmd->AvailableForStates(G4State_PreInit);
}

// -----------------------------------------------------------------------

TSCylinderShieldTankMessenger::~TSCylinderShieldTankMessenger()
{
  delete fTankHeightCmd;
  delete fTankRadiusCmd;
  delete fTankWallThicknessCmd;
  delete fShieldMaterialCmd;
  delete fTankWallMaterialCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void TSCylinderShieldTankMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fTankHeightCmd)
    fTank->SetTankHeight(fTankHeightCmd->GetNewDoubleValue(nV));
  else if (cmd == fTankRadiusCmd)
    fTank->SetTankRadius(fTankRadiusCmd->GetNewDoubleValue(nV));
  else if (cmd == fShieldMaterialCmd)
    fTank->SetShieldMaterial(nV);
  else if (cmd ==fTankWallThicknessCmd)
    fTank->SetTankWallThickness(fTankWallThicknessCmd->GetNewDoubleValue(nV));
  else if (cmd == fTankWallMaterialCmd)
    fTank->SetTankWallMaterial(nV);
  else{};
}

// -----------------------------------------------------------------------

