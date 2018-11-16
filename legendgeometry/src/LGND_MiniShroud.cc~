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
// CLASS IMPLEMENTATION:  LGND_MiniShroud.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Part Origin:  Coaxial with rod, coincident with top.
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Dec 2, 2017
 *
 * REVISION:
 *
 * 12-02-2017, Created, M. Green
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
#include "legendgeometry/LGND_MiniShroud.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_MiniShroud::LGND_MiniShroud(G4String partName, G4String serialNumber, G4double length, G4String material) :
  LGND_Part(partName, serialNumber, "LGND_MiniShroud", material),
  fLength(length*mm)
{;}

LGND_MiniShroud::LGND_MiniShroud(G4String partName, G4String serialNumber, G4double length) :
  LGND_Part(partName, serialNumber, "LGND_MiniShroud", "Nylon"),
  fLength(length*mm)
{;}

LGND_MiniShroud::LGND_MiniShroud(const LGND_MiniShroud & rhs) :
  LGND_Part(rhs)
{;}

LGND_MiniShroud::~LGND_MiniShroud()
{;}

G4LogicalVolume* LGND_MiniShroud::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial+"x"+G4UIcommand::ConvertToString(fLength);
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  
  G4double TPBthickness = 600.*nm;//anything smaller than 600 nm, won't draw...
  G4double shroudOuterDiamter = 50.0*mm;
  G4double shroudThickness = 0.1*mm;

   
  if(this->GetMaterial() == ""){
    MGLog(error) << "need to assign materials to shroud!"<<endlog;
    return NULL;
  }
  
  if(pVol == NULL && this->GetMaterial() == "TPB" && fPartName.contains("Inner")){
    G4Tubs* outer = new G4Tubs("outer", 0.0*mm, shroudOuterDiamter-shroudThickness, (fLength/2.0)-shroudThickness, 0, 2*pi);
    G4Tubs* inner = new G4Tubs("inner", 0.0*mm, shroudOuterDiamter-shroudThickness-TPBthickness, (fLength/2.0-shroudThickness-TPBthickness), 0, 2*pi);
    G4SubtractionSolid* tpbInnerShroud = new G4SubtractionSolid("tpbInnerShroud",outer,inner);

    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(tpbInnerShroud,material,logicalName+G4String("Inner"));
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(1.,0.4,1,0.3)); // pink
    greyVisAtt->SetForceWireframe( false );
    pVol->SetVisAttributes(greyVisAtt);
  }
  else if(pVol == NULL && this->GetMaterial() == "TPB" && fPartName.contains("Outer")){
    G4Tubs* outer = new G4Tubs("outer", 0.0*mm, shroudOuterDiamter+TPBthickness, (fLength/2.0)+TPBthickness, 0, 2*pi);
    G4Tubs* inner = new G4Tubs("inner", 0.0*mm, shroudOuterDiamter             , fLength/2.0             , 0, 2*pi);
    G4SubtractionSolid* tpbOuterShroud = new G4SubtractionSolid("tpbOuterShroud",outer,inner);
    
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(tpbOuterShroud,material,logicalName+G4String("Outer"));
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(1.,0.4,1,0.3)); // pink
    greyVisAtt->SetForceWireframe( false );
    pVol->SetVisAttributes(greyVisAtt);
  }
  else if (pVol == NULL && this->GetMaterial() == "Nylon"){
    G4Tubs* outer = new G4Tubs("outer", 0.0*mm, shroudOuterDiamter, fLength/2.0, 0, 2*pi);
    G4Tubs* inner = new G4Tubs("inner", 0.0*mm, shroudOuterDiamter - shroudThickness, (fLength/2.0-shroudThickness), 0, 2*pi);
    G4SubtractionSolid* shroud = new G4SubtractionSolid("shroud", outer, inner);
    
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(shroud,material,logicalName);
   
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.5,5,5,0.3)); // New copper color
    greyVisAtt->SetForceWireframe( false );
    pVol->SetVisAttributes(greyVisAtt);
  }
  
  return pVol;
}
