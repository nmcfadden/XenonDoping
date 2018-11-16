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
// CLASS IMPLEMENTATION:  MJDemoSignalCableTakeupLowBulkhead.cc
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
#include "mjdemonstrator/MJDemoSignalCableTakeupLowBulkhead.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSignalCableTakeupLowBulkhead::MJDemoSignalCableTakeupLowBulkhead(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-174-R001", "Copper-EF")
{;}

MJDemoSignalCableTakeupLowBulkhead::MJDemoSignalCableTakeupLowBulkhead(const MJDemoSignalCableTakeupLowBulkhead & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSignalCableTakeupLowBulkhead::~MJDemoSignalCableTakeupLowBulkhead()
{;}

G4LogicalVolume* MJDemoSignalCableTakeupLowBulkhead::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.01*mm;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 50.8/2*mm, 3.175/2*mm, 12.7/2*mm);
    G4Box* sideSubRecPrism1 = new G4Box("Sub Rec Prism 1", 3.937/2*mm+epsilon, 3.175/2*mm+epsilon, 2.939/2*mm+epsilon);
    G4Box* sideSubRecPrism2 = new G4Box("Sub Rec Prism 2", (1.2274*1.3)/2*mm+epsilon, 3.175/2*mm+epsilon, 4.253*1.5/2*mm+epsilon);
    G4Box* sideSubRecPrism3 = new G4Box("Sub Rec Prism 3", 2.245/2*mm+epsilon, 3.175/2*mm+epsilon, 3*mm+epsilon);
    G4Box* sideSubRecPrism4 = new G4Box("Sub Rec Prism 4", 9.525/2*mm+epsilon, 15.875/2*mm, 3.175/2*mm+epsilon);
    G4Box* sideSubRecPrism5 = new G4Box("Sub Rec Prism 5", 9.842/2*mm+epsilon, 3.175/2*mm+epsilon, 12.7/2*mm+epsilon);
    G4Tubs* sideSubCyl = new G4Tubs("Subtracted Cylinder from Side", 0.0*mm, 6.8/2*mm, 20/2*mm + epsilon, 0, 2*M_PI);//6.35, 11.43 MG
    G4Tubs* screwSubCyl = new G4Tubs("Cylindrical Hole", 0.0*mm, 4.496/2*mm, 12.7/2*mm, 0, 2*M_PI);
    G4Tubs* topPillar = new G4Tubs("Top Pillar", 0.0*mm, 9.525/2*mm, 15.875/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix;
    G4RotationMatrix* rotY2 = new G4RotationMatrix;
    G4RotationMatrix* rotZ = new G4RotationMatrix;
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(45*M_PI/180.*rad);
    rotY2->rotateY(-45*M_PI/180.*rad);
    rotZ->rotateZ(M_PI/2.*rad);
    G4SubtractionSolid* subSolid;
    G4UnionSolid* unionSolid;

    subSolid = new G4SubtractionSolid("Sub Solid", mainRecPrism, sideSubRecPrism1, rotN, G4ThreeVector(-(50.8/2-3.937/2)*mm, 0, -(12.7/2-2.939/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism1, rotN, G4ThreeVector((50.8/2-3.937/2)*mm, 0, (12.7/2-2.939/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism2, rotY, G4ThreeVector((50.8/2)*mm, 0, (12.7/2-6.5)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism2, rotY, G4ThreeVector(-(50.8/2)*mm, 0, -(12.7/2-6.5)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism3, rotY2, G4ThreeVector((50.8/2)*mm, 0, -(12.7/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubRecPrism3, rotY2, G4ThreeVector(-(50.8/2)*mm, 0, (12.7/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((50.8/2-0.762)*mm, 0, (12.7/2-2.39)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(50.8/2-0.762)*mm, 0, -(12.7/2-2.39)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotX, G4ThreeVector((50.8/2-3.175)*mm, 0, -(12.7/2-3.175)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotX, G4ThreeVector(-(50.8/2-3.175)*mm, 0, (12.7/2-3.175)*mm));

    unionSolid = new G4UnionSolid("Union Solid", subSolid, topPillar, rotX, G4ThreeVector((50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, (12.7/2-9.525/2)*mm)); 
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, topPillar, rotX, G4ThreeVector(-(50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, (12.7/2-9.525/2)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, topPillar, rotX, G4ThreeVector((50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, -(12.7/2-9.525/2)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, topPillar, rotX, G4ThreeVector(-(50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, -(12.7/2-9.525/2)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, sideSubRecPrism4, rotN, G4ThreeVector((50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, (12.7/2-4.673-3.175/2)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, sideSubRecPrism4, rotN, G4ThreeVector(-(50.8/2-17.463)*mm, (3.175/2+15.875/2)*mm, (12.7/2-4.673-3.175/2)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, sideSubRecPrism5, rotN, G4ThreeVector((50.8/2-7.62-9.842/2)*mm, (3.175/2+12.7+3.175/2)*mm, (0.)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, sideSubRecPrism5, rotN, G4ThreeVector(-(50.8/2-7.62-9.842/2)*mm, (3.175/2+12.7+3.175/2)*mm, (0.)*mm));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(unionSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Signal Cable Takeup Low Bulkhead Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Signal Cable Takeup Low Bulkhead Logical" << endlog; 
  return pVol;
}  
