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
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $
//
// CLASS IMPLEMENTATION:  LGND_CoppperShroud.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 *
 * REVISION:
 *
 * 8-06-2018, Created, N. McFadden
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_CopperShroud.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_CopperShroud::LGND_CopperShroud(G4String partName, G4String serialNumber, G4double length, G4String material) :
  LGND_Part(partName, serialNumber, "LGND_CopperShroud", material),
  fLength(length*mm)
{;}

LGND_CopperShroud::LGND_CopperShroud(const LGND_CopperShroud & rhs) :
  LGND_Part(rhs)
{;}

LGND_CopperShroud::~LGND_CopperShroud()
{;}

G4LogicalVolume* LGND_CopperShroud::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial+"x"+G4UIcommand::ConvertToString(fLength);
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  
  G4double TPBthickness = 600.*nm;//5*um;
  G4double VM2000thickness = 1*mm;
  G4double shroudOuterRadius = 500.0*mm;
  G4double shroudThickness = 2*mm;

   
  if(this->GetMaterial() == ""){
    MGLog(error) << "need to assign materials to shroud!"<<endlog;
  }

  if(pVol == NULL && this->GetMaterial() == "TPB"){
    G4Tubs* outer = new G4Tubs("outer", 0.0*mm, shroudOuterRadius - shroudThickness - VM2000thickness, fLength-shroudThickness-VM2000thickness, 0, 2*pi);
    G4Tubs* inner = new G4Tubs("inner", 0.0*mm, shroudOuterRadius - shroudThickness - VM2000thickness - TPBthickness, (fLength-shroudThickness-VM2000thickness-TPBthickness), 0, 2*pi);
    G4SubtractionSolid* tpbShroud = new G4SubtractionSolid("tpbShroudSolid",outer,inner);
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(1.0,0,1,0.3)); // magenta color

    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(tpbShroud,material,logicalName);
    pVol->SetVisAttributes(greyVisAtt);
  }
  else if (pVol == NULL && this->GetMaterial() == "VM2000"){
    G4Tubs*  outer = new G4Tubs("outer", 0.0*mm, shroudOuterRadius - shroudThickness, fLength-shroudThickness, 0, 2*pi);
    G4Tubs*  inner = new G4Tubs("inner", 0.0*mm, shroudOuterRadius - shroudThickness - VM2000thickness, (fLength-shroudThickness-VM2000thickness), 0, 2*pi);
    G4SubtractionSolid* vm2000Shroud = new G4SubtractionSolid("vm2000ShroudSolid",outer,inner);
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(1.0,1,0,0.3)); // yellow color

    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(vm2000Shroud,material,logicalName);
    pVol->SetVisAttributes(greyVisAtt);
  }
  else if (pVol == NULL && this->GetMaterial() == "Copper-EF"){
    G4Tubs* outer = new G4Tubs("outer", 0.0*mm, shroudOuterRadius, fLength, 0, 2*pi);
    G4Tubs* inner = new G4Tubs("inner", 0.0*mm, shroudOuterRadius - shroudThickness, (fLength-shroudThickness), 0, 2*pi);
    G4SubtractionSolid* coppershroud = new G4SubtractionSolid("copperShroudSolid", outer, inner);
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.5,5,5,0.3)); // New copper color
    
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(coppershroud,material,logicalName);
    pVol->SetVisAttributes(greyVisAtt);
  }
  else
    MGLog(error)<<"unknow material: "<<this->GetMaterial()<<" for CopperShroud construction"<<endlog;
  
  return pVol;
}
