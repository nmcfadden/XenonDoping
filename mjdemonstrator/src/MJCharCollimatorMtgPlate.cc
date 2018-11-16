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
// CLASS IMPLEMENTATION:  MJCharCollimatorMtgPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Collimator mounting plate
 * Note that the part coaxial to the rest of the assembly is not at the origin
 * Part Origin: center of original box, (hx,hy,hz) from corner
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJCharCollimatorMtgPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJCharCollimatorMtgPlate::MJCharCollimatorMtgPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CollimatorMtgPlate", "Aluminum") 
{;}

MJCharCollimatorMtgPlate::MJCharCollimatorMtgPlate(const MJCharCollimatorMtgPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJCharCollimatorMtgPlate::~MJCharCollimatorMtgPlate()
{;}

G4LogicalVolume* MJCharCollimatorMtgPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Define the basic shape of the mounting plate
    G4double hx_block = 1.750 * 25.4 * mm;
    G4double hy_block = 1.625 * 25.4 * mm;
    G4double hz_block = 0.250 * 25.4 * mm;
    G4Box* block = new G4Box("Block", hx_block, hy_block, hz_block);

    // remove a cutout for the source
    G4Tubs* thruCut = new G4Tubs("ThruCut", 0, 0.875/2*25.4*mm, hz_block + 0.1*25.4*mm, 0, 2*pi);
    G4Tubs* partCut = new G4Tubs("PartCut", 0, 1.75/2*25.4*mm, 0.375/2*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation0 = new G4RotationMatrix();
    G4UnionSolid* cutout = new G4UnionSolid("Cutout", thruCut, partCut, rotation0, G4ThreeVector(0,0,0.0625*25.4*mm));

    G4SubtractionSolid* mtgPlate = new G4SubtractionSolid("MtgPlate", block, cutout, rotation0, G4ThreeVector(0.125*25.4*mm,0,0));

    // Define screw holes for the 1/4-20 screws, should match up with screw holes in MJCharCollimatorHousing
    G4double zPlane[] = {0.001, -0.25*25.4*mm, -0.25*25.4*mm, -0.501*25.4*mm};
    G4double rInner[] = {0,0,0,0};
    G4double rOuter[] = {0.438/2.0*25.4*mm, 0.438/2.0*25.4*mm, 0.281/2.0*25.4*mm, 0.281/2.0*25.4*mm}; 
    G4Polycone* screwHole = new G4Polycone("ScrewHole", 0, 2*pi, 4, zPlane, rInner, rOuter);

    // Subtract screw holes from the MtgPlate
    G4ThreeVector translate_screw1(-hx_block + 0.687*25.4*mm, 0.5*25.4*mm, 0.25*25.4*mm);
    G4ThreeVector translate_screw2(-hx_block + 0.687*25.4*mm, -0.5*25.4*mm, 0.25*25.4*mm);
    G4ThreeVector translate_screw3(hx_block - 0.438*25.4*mm, 0.5*25.4*mm, 0.25*25.4*mm);
    G4ThreeVector translate_screw4(hx_block - 0.438*25.4*mm, -0.5*25.4*mm, 0.25*25.4*mm);

    mtgPlate = new G4SubtractionSolid("MinusScrews", mtgPlate, screwHole, rotation0, translate_screw1);
    mtgPlate = new G4SubtractionSolid("MinusScrews", mtgPlate, screwHole, rotation0, translate_screw2);
    mtgPlate = new G4SubtractionSolid("MinusScrews", mtgPlate, screwHole, rotation0, translate_screw3);
    mtgPlate = new G4SubtractionSolid("MinusScrews", mtgPlate, screwHole, rotation0, translate_screw4);

    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.75,0.75,0.75)); // light grey
    greyVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(mtgPlate, material, logicalName);
    pVol->SetVisAttributes(greyVisAtt);  
    MGLog(debugging) << "Created Collimator Mounting Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Collimator Mounting Plate Logical" << endlog; 
  return pVol;
}  
