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
// $Id: TSLabModuleBoxMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  TSLabModuleBoxMessenger.cc
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

#include "tonscalegeometry/TSLabModuleBox.hh" 

//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSLabModuleBoxMessenger.hh" 

//---------------------------------------------------------------------------//

TSLabModuleBoxMessenger::TSLabModuleBoxMessenger(TSLabModuleBox *labmodule) : fLabModule(labmodule)
{
  // /MG/geometry/TSLabModuleBox
  fDirectory = new G4UIdirectory("/MG/geometry/TSLabModuleBox/");

  // /MG/geometry/TSLabModuleBox/labheight
  fLabSpaceHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSLabModuleBox/labheight", this);
  fLabSpaceHeightCmd->SetGuidance("Set height of lab space");
  fLabSpaceHeightCmd->SetDefaultUnit("m");
  fLabSpaceHeightCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/TSLabModuleBox/labwidth
  fLabSpaceWidthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSLabModuleBox/labwidth", this);
  fLabSpaceWidthCmd->SetGuidance("Set width of lab space");
  fLabSpaceWidthCmd->SetDefaultUnit("m");
  fLabSpaceWidthCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/TSLabModuleBox/lablength
  fLabSpaceLengthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSLabModuleBox/lablength", this);
  fLabSpaceLengthCmd->SetGuidance("Set length of lab space");
  fLabSpaceLengthCmd->SetDefaultUnit("m");
  fLabSpaceLengthCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/TSLabModuleBox/rockdepth
  fRockDepthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSLabModuleBox/rockdepth", this);
  fRockDepthCmd->SetGuidance("Set depth of rock)");
  fRockDepthCmd->SetDefaultUnit("m");
  fRockDepthCmd->SetUnitCandidates("m cm mm micrometer");
	
  // /MG/geometry/TSLabModuleBox/shotcretedepth
  fShotcreteDepthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSLabModuleBox/shotcretedepth", this);
  fShotcreteDepthCmd->SetGuidance("Set depth of shotcrete)");
  fShotcreteDepthCmd->SetDefaultUnit("m");
  fShotcreteDepthCmd->SetUnitCandidates("m cm mm micrometer");	

  // /MG/geometry/TSLabModuleBox/rockmaterial
  fRockMaterialCmd = new G4UIcmdWithAString("/MG/geometry/TSLabModuleBox/rockmaterial", this);
  fRockMaterialCmd->SetGuidance("Set material of lab module rock (eg. Rock)");

	// /MG/geometry/TSLabModuleBox/shotcretematerial
  fShotcreteMaterialCmd = new G4UIcmdWithAString("/MG/geometry/TSLabModuleBox/shotcretematerial", this);
  fShotcreteMaterialCmd->SetGuidance("Set material of shotcrete (eg. Rock)");

  // /MG/geometry/TSLabModuleBox/labspacematerial
  fLabSpaceMaterialCmd = new G4UIcmdWithAString("/MG/geometry/TSLabModuleBox/labspacematerial", this);
  fLabSpaceMaterialCmd->SetGuidance("Set material of lab space (eg. Air)");
}

// -----------------------------------------------------------------------

TSLabModuleBoxMessenger::~TSLabModuleBoxMessenger()
{
  delete fRockDepthCmd;
  delete fShotcreteDepthCmd;
  delete fLabSpaceWidthCmd;
  delete fLabSpaceLengthCmd;
  delete fLabSpaceHeightCmd;
  delete fLabSpaceMaterialCmd;
  delete fRockMaterialCmd;
  delete fShotcreteMaterialCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void TSLabModuleBoxMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fLabSpaceHeightCmd)
    fLabModule->SetLabSpaceBoxHeight(fLabSpaceHeightCmd->GetNewDoubleValue(nV));
  else if(cmd == fLabSpaceWidthCmd)
    fLabModule->SetLabSpaceBoxWidth(fLabSpaceWidthCmd->GetNewDoubleValue(nV));
  else if(cmd == fLabSpaceLengthCmd)
    fLabModule->SetLabSpaceBoxLength(fLabSpaceLengthCmd->GetNewDoubleValue(nV));
  else if(cmd == fRockDepthCmd)
    fLabModule->SetRockDepth(fRockDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fShotcreteDepthCmd)
    fLabModule->SetShotcreteDepth(fShotcreteDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fLabSpaceMaterialCmd)
    fLabModule->SetLabSpaceMaterialName(nV);
  else if(cmd == fRockMaterialCmd)
    fLabModule->SetRockMaterialName(nV);
  else if(cmd == fShotcreteMaterialCmd)
    fLabModule->SetShotcreteMaterialName(nV);
}

// -----------------------------------------------------------------------

