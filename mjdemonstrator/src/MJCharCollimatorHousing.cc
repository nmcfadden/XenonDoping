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
// CLASS IMPLEMENTATION:  MJCharCollimatorHousing.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Collimator housing
 * The actual part is composed of two pieces, symmetrically cut along what is
 * here the x-z plane, held together by four 8-32 screws.
 * This geometry here is made as one piece.  
 * Part Origin: center of original box, (hx_block,hy_block,hz_block) from corner
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
#include "mjdemonstrator/MJCharCollimatorHousing.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJCharCollimatorHousing::MJCharCollimatorHousing(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CollimatorHousing", "UHMW")
{;}

MJCharCollimatorHousing::MJCharCollimatorHousing(const MJCharCollimatorHousing & rhs) : 
  MJVDemoPart(rhs)
{;}

MJCharCollimatorHousing::~MJCharCollimatorHousing()
{;}

G4LogicalVolume* MJCharCollimatorHousing::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Define Ultra-High-Molecular-Weight (UHMW) Polyethylene
    G4double z, a, density;
    G4String name, symbol;
    G4int ncomponents, natoms;
    G4Element* elH = new G4Element(name="Hydrogen",symbol="H",z=1.,a=1.01*g/mole);
    G4Element* elC = new G4Element(name="Carbon",symbol="C",z=1.,a=12.011*g/mole);    
    G4Material* UHMW = new G4Material(name="UHMW",density=0.93*g/cm3,ncomponents=2);
    UHMW->AddElement(elH, natoms = 400000);
    UHMW->AddElement(elC, natoms = 200000);

    // Define the main block
    G4double hx_block = 1.500 * 25.4 * mm;
    G4double hy_block = 1.155 * 25.4 * mm;
    G4double hz_block = 1.765 * 25.4 * mm;
    G4Box* block = new G4Box("Block", hx_block, hy_block, hz_block);

    // Hollow out the main block with a union solid of a box and cylinders
    G4double hx_hollow = 0.9025 * 25.4 * mm;
    G4double hy_hollow = 0.9050 * 25.4 * mm;
    G4double hz_hollow = 1.7015 * 25.4 * mm;
    G4Box* hollow_box = new G4Box("Hollow", hx_hollow, hy_hollow, hz_hollow);

    // location of collimator bore
    G4double sourceHoleRadius = 0.25 * 25.4 * mm;
    G4Tubs* sourceHole = new G4Tubs("SourceHole", 0, sourceHoleRadius, 25.4*mm, 0, 2*pi);
    G4ThreeVector translate_hole(0,0,-1.0 * hz_hollow);
    G4RotationMatrix* rotate0 = new G4RotationMatrix;
    G4UnionSolid* hollow = new G4UnionSolid("AddSourceHole", hollow_box, sourceHole, rotate0, translate_hole);

    // the cylinder cutout length should be hy_hollow, but gave seg faults when creating a heprep file
    // the value was decreased by 1.1 mm to prevent issues with this
    G4double cylinderRadius = 0.125 * 25.4 * mm;
    G4Tubs* cylinder = new G4Tubs("Cylinder", 0, cylinderRadius, hy_hollow - 1.1*mm, 0, 2*pi);
    G4ThreeVector translate_cyl_1(hx_hollow,0,(-1.0*hz_hollow + 0.125*25.4*mm));
    G4ThreeVector translate_cyl_2(-1.0*hx_hollow,0,(-1.0*hz_hollow + 0.125*25.4*mm));
    G4RotationMatrix* thruSide = new G4RotationMatrix();
    thruSide->rotateX(pi/2.0);
    hollow = new G4UnionSolid("AddCylinder1", hollow, cylinder, thruSide, translate_cyl_1);
    hollow = new G4UnionSolid("AddCylinder2", hollow, cylinder, thruSide, translate_cyl_2);

    // bore holes in the side
    G4Tubs* boreHole = new G4Tubs("BoreHole", 0, 0.1005*25.4*mm, 1.1555*25.4*mm, 0, 2*pi);
    G4ThreeVector translate_bore_1(0,0,0.7015*25.4*mm);
    G4ThreeVector translate_bore_2(0,0,-0.7985*25.4*mm);
    hollow = new G4UnionSolid("AddBore1", hollow, boreHole, thruSide, translate_bore_1);
    hollow = new G4UnionSolid("AddBore2", hollow, boreHole, thruSide, translate_bore_2);

    // Subtract the union solid "hollow" from "block" to get the housing
    G4ThreeVector translate_hollow(0,0,(hz_block - hz_hollow));
    G4SubtractionSolid* housing = new G4SubtractionSolid("BlockMinusHollow", block, hollow, rotate0, translate_hollow);

    // Define screw holes for 8-32 screws
    G4double rOuter_screwHole[] = {.313/2*25.4*mm, .313/2*25.4*mm, .177/2*25.4*mm, .177/2*25.4*mm, .313/2*25.4*mm, .313/2*25.4*mm};
    G4double rInner_screwHole[] = {0,0,0,0,0,0};
    G4double zPlane_screwHole[] = {0.01*mm, -0.304*25.4*mm, -0.304*25.4*mm, -2.0*hy_block+0.304*25.4*mm, -2.0*hy_block+0.304*25.4*mm, -2.0*hy_block-0.01*mm};
    G4Polycone* screwHole832 = new G4Polycone("ScrewHole", 0, 2*pi, 6, zPlane_screwHole, rInner_screwHole, rOuter_screwHole);

    G4ThreeVector translate_screw1(1.2*25.4*mm,-hy_block,0.765*25.4*mm);
    G4ThreeVector translate_screw2(1.2*25.4*mm,-hy_block,-1.015*25.4*mm);
    G4ThreeVector translate_screw3(-1.2*25.4*mm,-hy_block,0.765*25.4*mm);
    G4ThreeVector translate_screw4(-1.2*25.4*mm,-hy_block,-1.015*25.4*mm);

    G4RotationMatrix* rotate_screw = new G4RotationMatrix();
    rotate_screw->rotateX(-pi/2.);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole832, rotate_screw, translate_screw1);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole832, rotate_screw, translate_screw2);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole832, rotate_screw, translate_screw3);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole832, rotate_screw, translate_screw4);

    // Define screw holes for 1/4-20 screws
    // radius is bigger than in drawings, need to verify which is correct, screw choice or drawing
    G4Tubs* screwHole1420 = new G4Tubs("1420ScrewHole", 0, 0.125*25.4*mm, 0.3251*25.4*mm, 0, 2*pi);
    G4ThreeVector translate_screw5(2.375/2.0*25.4*mm, 0.5*25.4*mm, 1.44*25.4*mm);
    G4ThreeVector translate_screw6(2.375/2.0*25.4*mm, -0.5*25.4*mm, 1.44*25.4*mm);
    G4ThreeVector translate_screw7(-2.375/2.0*25.4*mm, 0.5*25.4*mm, 1.44*25.4*mm);
    G4ThreeVector translate_screw8(-2.375/2.0*25.4*mm, -0.5*25.4*mm, 1.44*25.4*mm);
    rotate_screw->rotateX(pi/2.);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole1420, rotate_screw, translate_screw5);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole1420, rotate_screw, translate_screw6);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole1420, rotate_screw, translate_screw7);
    housing = new G4SubtractionSolid("BlockMinusScrews", housing, screwHole1420, rotate_screw, translate_screw8);

    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1,1,1)); // white
    whiteVisAtt->SetForceWireframe( false );
    //material lookup overridden to place custom material
    //G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(housing, UHMW, logicalName);
    pVol->SetVisAttributes(whiteVisAtt);  
    MGLog(debugging) << "Created Collimator Housing Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Collimator Housing Logical" << endlog; 
  return pVol;
}  
