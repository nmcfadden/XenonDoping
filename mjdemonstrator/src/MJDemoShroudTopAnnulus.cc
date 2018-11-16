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
// CLASS IMPLEMENTATION:  MJDemoShroudTopAnnulus.cc
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
 * FIRST SUBMISSION: June 20, 2014
 * 
 * REVISION:
 * 
 * 20-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoShroudTopAnnulus.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoShroudTopAnnulus::MJDemoShroudTopAnnulus(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "drawing number", "Copper-EF")
{;}

MJDemoShroudTopAnnulus::MJDemoShroudTopAnnulus(const MJDemoShroudTopAnnulus & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoShroudTopAnnulus::~MJDemoShroudTopAnnulus()
{;}

G4LogicalVolume* MJDemoShroudTopAnnulus::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 295.91/2*mm, 310.007/2*mm, 9.525/2*mm, 0, 2*M_PI);
    G4Box* testPrism1 = new G4Box("Test Prism 1", 10/2*mm, 10/2*mm, 10/2*mm);
    G4Box* testPrism2 = new G4Box("Test Prism 2", 5/2*mm, 5/2*mm, 5/2*mm);
    G4Tubs* adCyl1 = new G4Tubs("Added Cylinder 1", 0.0*mm, 6.35/2*mm, 9.525/2*mm, 0, M_PI);
    G4Tubs* subCyl1 = new G4Tubs("Subtracted Cylinder 1", 0.0*mm, 6.35/2*mm, 9.525/2*mm+epsilon, 0, M_PI);
    G4Tubs* subCyl2 = new G4Tubs("Subtracted Cylinder 2", 0, 4.166/2*mm, 10/2*mm, 0, 2*M_PI);
    G4Box* adPrism1 = new G4Box("Added Prism 1", 3.794/2*mm, 9.525/2*mm, 2.432/2*mm);
    G4Box* subPrism = new G4Box("Subtracted Prism", 20/2*mm, 20/2*mm, 20/2*mm);
    G4Trd* trap = new G4Trd("Trapezoid", 16.582/2*mm, 10.053/2*mm,  9.525/2*mm, 9.525/2*mm, 2.428/2*mm);
    
    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotX2 = new G4RotationMatrix;
    G4RotationMatrix* rot = new G4RotationMatrix;
    rotX->rotateX(-M_PI/2);
    rotX2->rotateX(M_PI/2);
    G4SubtractionSolid* subSolid;
    G4UnionSolid* unionSolid;
    G4UnionSolid* rotSolid;
    G4UnionSolid* rotSolid2;
    G4SubtractionSolid* pieceSub;
    G4UnionSolid* pieceUnion;

    pieceUnion = new G4UnionSolid("Union Solid", trap, adPrism1, rotN, G4ThreeVector(0, 0, (2.428+2.432)/2*mm));
    pieceUnion = new G4UnionSolid("Union Solid", pieceUnion, adCyl1, rotX, G4ThreeVector(3.794/2*mm, 0, (2.428/2+2.432-3.175)*mm));
    pieceSub = new G4SubtractionSolid("Subtraction Solid", pieceUnion, subCyl1, rotX2, G4ThreeVector(8.10*mm, 0, (2.428/2+2.432-1.69)*mm));
    pieceUnion = new G4UnionSolid("Union Solid", pieceSub, adCyl1, rotX, G4ThreeVector(-3.794/2*mm, 0, (2.428/2+2.432-3.175)*mm));
    pieceSub = new G4SubtractionSolid("Subtraction Solid", pieceUnion, subCyl1, rotX2, G4ThreeVector(-8.10*mm, 0, (2.428/2+2.432-1.69)*mm));

    rotSolid  = new G4UnionSolid("Union Solid", testPrism2, pieceSub, rotX, G4ThreeVector(0, (295.91/2-2.428/2)*mm, 0));
    rotSolid2  = new G4UnionSolid("Union Solid", testPrism2, subCyl2, rotN, G4ThreeVector(0, 148.844*mm, 0));
    unionSolid = new G4UnionSolid("Union Solid", testPrism1, rotSolid, rotN, G4ThreeVector(0, 0, 0));

    unionSolid = new G4UnionSolid("Union Solid", unionSolid, mainCyl, rotN, G4ThreeVector(0, 0, 0));

    for(G4int idx = 1; idx<8; idx++){
        rot->rotateZ(360/9*deg);
        subSolid = new G4SubtractionSolid("Subtraction Solid", unionSolid, rotSolid2, rot, G4ThreeVector(0, 0, 0));
        unionSolid = new G4UnionSolid("Union Solid", subSolid, rotSolid, rot, G4ThreeVector(0, 0, 0));        
    }

    subSolid = new G4SubtractionSolid("Subtraction Solid", unionSolid, rotSolid2, rotN, G4ThreeVector(0, 0, 0));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subPrism, rotN, G4ThreeVector(0, 0, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Shroud Top Annulus Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Shroud Top Annulus Logical" << endlog; 
  return pVol;
}  
