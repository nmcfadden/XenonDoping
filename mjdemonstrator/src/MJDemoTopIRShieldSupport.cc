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
// CLASS IMPLEMENTATION:  MJDemoTopIRShieldSupport.cc
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
 * FIRST SUBMISSION: May 29, 2014
 * 
 * REVISION:
 * 
 * 29-05-2014, Created, M. Hays
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
#include "mjdemonstrator/MJDemoTopIRShieldSupport.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoTopIRShieldSupport::MJDemoTopIRShieldSupport(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-171", "Copper-EF")
{;}

MJDemoTopIRShieldSupport::MJDemoTopIRShieldSupport(const MJDemoTopIRShieldSupport & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoTopIRShieldSupport::~MJDemoTopIRShieldSupport()
{;}

G4LogicalVolume* MJDemoTopIRShieldSupport::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;
    // Length along z-axis of main rectangle is 35.8105 mm

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 38.1/2*mm, 12.7/2*mm, 72.813/2*mm);
    G4Box* sideSubRecPrism1 = new G4Box("Subtracted Prism from Side 1", 9.525/2*mm+epsilon, 12.7/2*mm + epsilon, 56.515/2*mm);
    G4Box* sideSubRecPrism2 = new G4Box("Subtracted Prism from Side 2", 3.175/2*mm, 12.7/2*mm + epsilon, 50.165/2*mm);
    G4Box* botSubRecPrism = new G4Box("Subtracted Prism from Bottom", 23.495/2*mm+epsilon, 12.7/2*mm + epsilon, 0.635/2*mm);
    G4Tubs* sideSubCyl = new G4Tubs("Subtracted Cylinder from Side", 0.0*mm, 3.175*mm, 12.*mm + epsilon, 0, 2*M_PI);
    G4Tubs* screwSubCyl1 = new G4Tubs("Cylindrical Hole", 0.0*mm, 1.825*mm, 12.7/2*mm + epsilon, 0, 2*M_PI); //1.727
    G4Tubs* screwSubCyl2 = new G4Tubs("Cylindrical Hole", 0.0*mm, 2.35*mm, 12.7/2*mm + epsilon, 0, 2*M_PI); //2.248
    G4Tubs* botSubCyl = new G4Tubs("Subtracted Cylinder from Bottom", 0.0*mm, 6.35/2*mm, 12.7/2*mm + epsilon, 0, 2*M_PI);

    std::vector<G4TwoVector> vertices; 
    G4double var1 = 38.1/2*mm+epsilon;
    G4double var2 = 1.192/2*mm;
    G4double var3 = 12.7/2*mm;
    vertices.push_back(G4TwoVector(-var1, -var2));
    vertices.push_back(G4TwoVector(-var1,  var2));
    vertices.push_back(G4TwoVector( var1,  var2));
    vertices.push_back(G4TwoVector( var1, -var2));
    vertices.push_back(G4TwoVector(-var1, -var2));
    vertices.push_back(G4TwoVector(-var1, -var2));
    vertices.push_back(G4TwoVector( var1, -var2));
    vertices.push_back(G4TwoVector( var1, -var2));
    G4GenericTrap* trap = new G4GenericTrap("Top Trapezoid", var3, vertices);

    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix;
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(M_PI/2*rad);
    G4SubtractionSolid* subSolid;

    subSolid = new G4SubtractionSolid("Sub Solid", mainRecPrism, sideSubRecPrism1, rotN, G4ThreeVector(14.287*mm, 0, 0));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism1, rotN, G4ThreeVector(-14.287*mm, 0, 0));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism2, rotN, G4ThreeVector(7.9375*mm, 0, 0));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism2, rotN, G4ThreeVector(-7.9375*mm, 0, 0));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, botSubRecPrism, rotN, G4ThreeVector((-4.445 + 23.495/2)*mm + epsilon/2, 0, -(72.178/2+epsilon)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((6.35+3.175)*mm, 0, (50.165/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((6.35+3.175)*mm, 0, -(50.165/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(6.35+3.175)*mm, 0, (50.165/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(6.35+3.175)*mm, 0, -(50.165/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotY, G4ThreeVector(0, 0, (36.513-35.8105)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotY, G4ThreeVector(0, 0, (36.513-35.8105+6.35)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, botSubCyl, rotX, G4ThreeVector(-4.445*mm, 0, -38.9465*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotN, G4ThreeVector((38.1/2-3.175)*mm,0,(56.515+9.313)/2*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotN, G4ThreeVector(-(38.1/2-3.175)*mm,0,(56.515+9.313)/2*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotN, G4ThreeVector((38.1/2-3.175)*mm,0,-(56.515+9.313)/2*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotN, G4ThreeVector(-(38.1/2-3.175)*mm,0,-(56.515+9.313)/2*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, trap, rotX, G4ThreeVector(0, 0, (72.813/2-1.192/2)*mm));   

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0));
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Top IR Shield Support Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Top IR Shield Support Logical" << endlog; 
  return pVol;
}  
