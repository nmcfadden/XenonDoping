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
// CLASS IMPLEMENTATION:  MJDemoBulkheadTopClampLowMount.cc
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
 * FIRST SUBMISSION: June 25, 2014
 * 
 * REVISION:
 * 
 * 25-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoBulkheadTopClampLowMount.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoBulkheadTopClampLowMount::MJDemoBulkheadTopClampLowMount(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-173", "Copper-EF")
{;}

MJDemoBulkheadTopClampLowMount::MJDemoBulkheadTopClampLowMount(const MJDemoBulkheadTopClampLowMount & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoBulkheadTopClampLowMount::~MJDemoBulkheadTopClampLowMount()
{;}

G4LogicalVolume* MJDemoBulkheadTopClampLowMount::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Box* prism1 = new G4Box("Prism 1",  55.46/2*mm, 12.7/2*mm, 4.763/2*mm);  
    G4Box* prism2 = new G4Box("Prism 2",  0.591/2*mm, 4.831/2*mm, 4.763/2*mm);  
    G4Tubs* cyl1 = new G4Tubs("Cyl 1", 0, 3.15/2*mm, 6/2*mm, 0, 2*M_PI);
    G4Tubs* cyl2 = new G4Tubs("Cyl 2", 0, 3.15/2*mm, 4.763/2*mm, 0, 2*M_PI);
    G4Box* subPrism1 = new G4Box("Sub Prism 1",  4/2*mm, 2.401/2*mm, 6/2*mm);  
    G4Box* subPrism2 = new G4Box("Sub Prism 2",  0.996*mm, 2.684/2*mm, 6/2*mm);  
    G4Box* subPrism3 = new G4Box("Sub Prism 3",  1.1806/2*mm, 10/2*mm, 6/2*mm);  
    G4Box* subPrism4 = new G4Box("Sub Prism 4",  60/2*mm, 9.5/2*mm, 1.6/2*mm);
    G4Box* subPrism5 = new G4Box("Sub Prism 5",  6.35/2*mm, 4/2*mm, 0.762/2*mm+epsilon);
    G4Tubs* subCyl1 = new G4Tubs("Sub Cyl 1", 0, 4.496/2*mm, 6/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl2 = new G4Tubs("Sub Cyl 2", 0, 4/2*mm, 6/2*mm, 0, 2*M_PI);//3.65

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix; //7.336, 2.794
    G4RotationMatrix* rot = new G4RotationMatrix;
    rotY->rotateY(90.*deg);
    rot->rotateZ(-45.*deg);
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;

    partUnion = new G4UnionSolid("Part Union", prism1, prism2, rotN, G4ThreeVector((55.46+0.591)/2*mm, -(12.7-4.831)/2*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, prism2, rotN, G4ThreeVector(-(55.46+0.591)/2*mm, (12.7-4.831)/2*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, cyl1, rotN, G4ThreeVector((55.46/2+0.578)*mm, -(12.7/2-7.615)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, cyl1, rotN, G4ThreeVector(-(55.46/2+0.578)*mm, (12.7/2-7.615)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partSub, cyl2, rotN, G4ThreeVector((55.46/2-0.984)*mm, -(12.7/2-4.831)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl2, rotN, G4ThreeVector(-(55.46/2-0.984)*mm, (12.7/2-4.831)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, subPrism1, rotN, G4ThreeVector((55.46/2-0.996+4/2)*mm, (12.7/2-2.684-2.401/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism2, rotN, G4ThreeVector((55.46/2-0.996)*mm, (12.7/2-2.684/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism1, rotN, G4ThreeVector(-(55.46/2-0.996+4/2)*mm, -(12.7/2-2.684-2.401/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism2, rotN, G4ThreeVector(-(55.46/2-0.996)*mm, -(12.7/2-2.684/2)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partSub, cyl2, rotN, G4ThreeVector((55.46/2-2.571)*mm, (12.7/2-2.684)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, subPrism3, rot, G4ThreeVector((55.46/2-2.3)*mm, 12.7/2*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partSub, cyl2, rotN, G4ThreeVector(-(55.46/2-2.571)*mm, -(12.7/2-2.684)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, subPrism3, rot, G4ThreeVector(-(55.46/2-2.3)*mm, -12.7/2*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl2, rotN, G4ThreeVector((55.46/2-5.505)*mm, (12.7/2-3.175)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl1, rotN, G4ThreeVector((55.46/2-2.33)*mm, (12.7/2-9.525)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl2, rotN, G4ThreeVector(-(55.46/2-5.505)*mm, -(12.7/2-3.175)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl1, rotN, G4ThreeVector(-(55.46/2-2.33)*mm, -(12.7/2-9.525)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism4, rotN, G4ThreeVector(0, (12.7/2-0.889-9.5/2)*mm, -(4.763-1.6)/2*mm));

    for(G4double idx=0; idx<8; idx++){
          partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism5, rotN, G4ThreeVector((55.46/2-5.505-6.35/2-9.525*idx), 12.7/2*mm, -(4.763-0.762)/2*mm));
    }

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Bulkhead Top Clamp Low Mount Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Bulkhead Top Clamp Low Mount Logical" << endlog; 
  return pVol;
}  
