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
// CLASS IMPLEMENTATION:  LGND_CrystalInsulator.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Part Origin:  COM of block before cutout is applied.
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
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_CrystalInsulator.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_CrystalInsulator::LGND_CrystalInsulator(G4String partName, G4String serialNumber) :
  LGND_Part(partName, serialNumber, "LGND_CrystalInsulator", "Silica")
{;}

LGND_CrystalInsulator::LGND_CrystalInsulator(const LGND_CrystalInsulator & rhs) :
  LGND_Part(rhs)
{;}

LGND_CrystalInsulator::~LGND_CrystalInsulator()
{;}

G4LogicalVolume* LGND_CrystalInsulator::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Box* box1 = new G4Box("box", 7.5*mm, 3.5*mm, 2.1*mm);
    G4Box* box2 = new G4Box("box2", 10.0*mm, 6.0*mm, 5.0*mm);
    G4SubtractionSolid* insulator = new G4SubtractionSolid("insulator", box1, box2, 0, G4ThreeVector(-10.0*mm, 0, (5.0-0.65)*mm));

    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0,1.0)); // White
    greyVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(insulator, material, logicalName);
    pVol->SetVisAttributes(greyVisAtt);
    MGLog(debugging) << "Created Crystal Insulator Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Crystal Insulator Logical" << endlog;
  return pVol;
}
