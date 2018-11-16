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
// CLASS IMPLEMENTATION:  MJDemoTransitionStraight.cc
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
 * FIRST SUBMISSION: June 18, 2014
 * 
 * REVISION:
 * 
 * 18-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoTransitionStraight.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoTransitionStraight::MJDemoTransitionStraight(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "drawing number", "Copper-EF")
{;}

MJDemoTransitionStraight::MJDemoTransitionStraight(const MJDemoTransitionStraight & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoTransitionStraight::~MJDemoTransitionStraight()
{;}

G4LogicalVolume* MJDemoTransitionStraight::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 50.8/2*mm, 5.588/2*mm, 38.621/2*mm);
    G4Box* subPrism1 = new G4Box("Subtracted Prism 1", 25.4/2*mm, 20/2*mm, 5.666/2*mm+epsilon);
    G4Box* subPrism2 = new G4Box("Subtracted Prism 2", 19.05/2*mm, 20/2*mm, 3.175/2*mm+epsilon);
    G4Tubs* topCyl = new G4Tubs("Top Cyl", 27.813/2*mm, 34.061/2*mm, 1.524/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl1 = new G4Tubs("Subtracted Cylinder 1", 0.0*mm, 25.4/2*mm, 10*mm, 0, M_PI);
    G4Tubs* subCyl2 = new G4Tubs("Subtracted Cylinder 2", 0.0*mm, 6.35/2*mm, 10*mm, 0, 2*M_PI);


    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix;
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(M_PI/2*rad);
    G4SubtractionSolid* subSolid;
    G4UnionSolid* unionSolid;

    unionSolid = new G4UnionSolid("Union Solid", mainRecPrism, topCyl, rotX, G4ThreeVector(0, (5.588+1.524)/2*mm, (38.621/2-17.031)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", unionSolid, subCyl1, rotX, G4ThreeVector(0, 0, (38.621/2-17.031)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subPrism1, rotN, G4ThreeVector(0, 0, (38.621/2-11.365-5.666/2)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subPrism2, rotN, G4ThreeVector(0, 0, (38.621/2-8.141-3.175/2)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subCyl2, rotX, G4ThreeVector((50.8/2-15.875)*mm, 0, (38.621/2-11.316)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subCyl2, rotX, G4ThreeVector(-(50.8/2-15.875)*mm, 0, (38.621/2-11.316)*mm));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Transition Straight Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Transition Straight Logical" << endlog; 
  return pVol;
}  
