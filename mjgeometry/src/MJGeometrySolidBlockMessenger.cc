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
// $Id: MJGeometrySolidBlockMessenger.cc,v 1.4 2007-08-16 18:33:18 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometrySolidBlockMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 13:08:03 PDT 2004
 * 
 * REVISION:
 *
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometrySolidBlock.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometrySolidBlockMessenger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometrySolidBlockMessenger::MJGeometrySolidBlockMessenger(
   MJGeometrySolidBlock *block): fBlock(block)
{
  // /MG/geometry/solidblock
  fSolidBlockDir = new G4UIdirectory("/MG/geometry/solidblock/");
  fSolidBlockDir->SetGuidance(
    "Controls the setting for a solid, uniform block");

  // /MG/geometry/solidblock/material
  fMaterialSelectCmd = new G4UIcmdWithAString(
    "/MG/geometry/solidblock/material", this);
  fMaterialSelectCmd->SetGuidance("Set material of block");

  // /MG/geometry/solidblock/edgeLength
  fSetEdgeLengthCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/solidblock/edgeLength", this);
  fSetEdgeLengthCmd->SetGuidance("Set the length of the block edge");

  // /MG/geometry/solidblock/setActive
  fSetActiveCmd = new G4UIcmdWithABool(
    "/MG/geometry/solidblock/setActive", this);
  fSetActiveCmd->SetGuidance("The physical volume of the block will");
  fSetActiveCmd->SetGuidance("contain the word active and output");
  fSetActiveCmd->SetGuidance("classes like G4Steps will treat it as such");

  // /MG/geometry/solidblock/deadLayer
  fSetDeadLayerCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/solidblock/deadLayer", this);
  fSetDeadLayerCmd->SetGuidance("If set to something non-zero, ");
  fSetDeadLayerCmd->SetGuidance("the outer skin of desired thickness ");
  fSetDeadLayerCmd->SetGuidance("will be a dead layer and the inner ");
  fSetDeadLayerCmd->SetGuidance("portion will be active.(a la G4Steps)");
}

//---------------------------------------------------------------------------//

MJGeometrySolidBlockMessenger::~MJGeometrySolidBlockMessenger()
{
  delete fSolidBlockDir;
  delete fMaterialSelectCmd;
  delete fSetEdgeLengthCmd;
  delete fSetActiveCmd;
  delete fSetDeadLayerCmd;
}


//---------------------------------------------------------------------------//

void MJGeometrySolidBlockMessenger::SetNewValue(
  G4UIcommand *cmd, G4String newValue ) 
{
  if(cmd == fMaterialSelectCmd) 
    fBlock->SetMaterialName(newValue);
  else if(cmd == fSetEdgeLengthCmd)
    fBlock->SetEdgeLength(fSetEdgeLengthCmd->GetNewDoubleValue(newValue));
  else if(cmd == fSetActiveCmd)
    fBlock->SetActive(fSetActiveCmd->GetNewBoolValue(newValue));
  else if(cmd == fSetDeadLayerCmd)
    fBlock->SetDeadLayerDepth(fSetDeadLayerCmd->GetNewDoubleValue(newValue));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
