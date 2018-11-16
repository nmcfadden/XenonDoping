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
// CLASS IMPLEMENTATION:  MJDemoIRShieldTopBaffle.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Max Hays
 * CONTACT: maxhays8@gmail.com
 * FIRST SUBMISSION: June 5, 2014
 * 
 * REVISION:
 * 
 * 05-06-2010, Created, M. Hays
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoIRShieldTopBaffle.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoIRShieldTopBaffle::MJDemoIRShieldTopBaffle(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-162", "Copper-EF")
{;}

MJDemoIRShieldTopBaffle::MJDemoIRShieldTopBaffle(const MJDemoIRShieldTopBaffle & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoIRShieldTopBaffle::~MJDemoIRShieldTopBaffle()
{;}

G4LogicalVolume* MJDemoIRShieldTopBaffle::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.01*mm;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 76.217/2*mm, 6.35/2*mm, 95.25/2*mm);
    G4Box* sideSubRecPrism1 = new G4Box("Sub Rec Prism 1", 69.85/2*mm, 6.35/2*mm+epsilon, 15.875/2*mm);
    G4Box* sideSubRecPrism2 = new G4Box("Sub Rec Prism 2", 66.675/2*mm+epsilon, 6.35/2*mm+epsilon, 3.175/2*mm+epsilon);
    G4Box* sideSubRecPrism3 = new G4Box("Sub Rec Prism 3", 19.304/2*mm+epsilon, 6.35/2*mm+epsilon, 45.974/2*mm+epsilon);
    G4Box* sideSubRecPrism4 = new G4Box("Sub Rec Prism 4", 10.795/2*mm+epsilon, 6.35/2*mm+epsilon, 13.716/2*mm+epsilon);
    G4Box* sideSubRecPrism5 = new G4Box("Sub Rec Prism 5", 3.175/2*mm+epsilon, 6.35/2*mm+epsilon, 10.514/2*mm+epsilon);
    G4Box* sideSubRecPrism6 = new G4Box("Sub Rec Prism 6", 6.096/2*mm+epsilon, 6.35/2*mm+epsilon, 12.7/2*mm+epsilon);
    G4Box* sideSubRecPrism7 = new G4Box("Sub Rec Prism 7", 3.175/2*mm+epsilon, 6.35/2*mm+epsilon, 6.35/2*mm+epsilon);
    G4Box* sideSubRecPrism8 = new G4Box("Sub Rec Prism 8", 12.7/2*mm+epsilon, 6.35/2*mm+epsilon, 9.525/2*mm+epsilon);
    G4Box* sideSubRecPrism9 = new G4Box("Sub Rec Prism 9", 6.35/2*mm+epsilon, 6.35/2*mm+epsilon, 3.175/2*mm+epsilon);
    G4Tubs* sideSubCyl = new G4Tubs("Subtracted Cylinder from Side", 0.0*mm, 6.35/2*mm, 11.43/2*mm + epsilon, 0, 2*M_PI);
    G4Tubs* screwSubCyl = new G4Tubs("Cylindrical Hole", 0.0*mm, 4.915/2*mm, 12.7/2*mm + epsilon, 0, 2*M_PI);

    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix;
    G4RotationMatrix* rotZ = new G4RotationMatrix;
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(M_PI/2.*rad);
    rotZ->rotateZ(M_PI/2.*rad);
    G4SubtractionSolid* subSolid;

    subSolid = new G4SubtractionSolid("Sub Solid", mainRecPrism, sideSubRecPrism1, rotN, G4ThreeVector((76.217/2-69.85/2)*mm, 0, (95.25/2-15.875/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism2, rotN, G4ThreeVector((76.217/2-66.675/2)*mm, 0, (95.25/2-15.875-3.175/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism3, rotN, G4ThreeVector(-(76.217/2-19.304/2)*mm, 0, -(95.25/2-45.974/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism4, rotN, G4ThreeVector(-(76.217/2-10.795/2)*mm, 0, -(95.25/2-45.974-13.716/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism5, rotN, G4ThreeVector(-(76.217/2-10.795-3.175/2)*mm, 0, -(95.25/2-45.974-10.514/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism6, rotN, G4ThreeVector(-(76.217/2-19.304-6.096/2)*mm, 0, -(95.25/2-33.274-12.7/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism7, rotN, G4ThreeVector(-(76.217/2-19.304-6.096-3.175/2)*mm, 0, -(95.25/2-33.274-3.175-6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism8, rotN, G4ThreeVector((76.217/2-6.35-12.7/2)*mm, 0, -(95.25/2-9.525/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism9, rotN, G4ThreeVector((76.217/2-6.35-3.175-6.35/2)*mm, 0, -(95.25/2-9.525-3.175/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-6.35-6.35/2)*mm, 0, (95.25/2-15.875)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-10.795)*mm, 0, (95.25/2-38.735)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-25.4)*mm, 0, (95.25/2-52.451)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-25.4)*mm, 0, (95.25/2-58.801)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-60.325)*mm, 0, (95.25/2-85.725)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(76.217/2-66.675)*mm, 0, (95.25/2-85.725)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotY, G4ThreeVector(-(76.217/2-6.35/2)*mm, 0, (95.25/2-4.763)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotY, G4ThreeVector(-(76.217/2-6.35/2)*mm, 0, (95.25/2-11.113)*mm));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0));
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created IR Shield Top Baffle Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing IR Shield Top Baffle Logical" << endlog; 
  return pVol;
}  
