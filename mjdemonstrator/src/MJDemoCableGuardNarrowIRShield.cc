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
// CLASS IMPLEMENTATION:  MJDemoCableGuardNarrowIRShield.cc
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
 * FIRST SUBMISSION: June 24, 2014
 * 
 * REVISION:
 * 
 * 24-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoCableGuardNarrowIRShield.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableGuardNarrowIRShield::MJDemoCableGuardNarrowIRShield(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-170", "Copper-EF")
{;}

MJDemoCableGuardNarrowIRShield::MJDemoCableGuardNarrowIRShield(const MJDemoCableGuardNarrowIRShield & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCableGuardNarrowIRShield::~MJDemoCableGuardNarrowIRShield()
{;}

G4LogicalVolume* MJDemoCableGuardNarrowIRShield::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Tubs* cyl1 = new G4Tubs("Cyl 1", 80.772/2*mm, 97.157/2*mm, 7.694/2*mm, (90-114.988/2)*deg, 114.988*deg);
    G4Box* prism1 = new G4Box("Prism 1",  9.218/2*mm, 13.827/2*mm, 7.694/2*mm);  
    G4Box* prism2 = new G4Box("Prism 2",  7.028/2*mm, 37.719/2*mm, 7.694/2*mm);  
    G4Tubs* subCyl1 = new G4Tubs("Sub Cyl 1", 0, 78.74/2*mm, 40/2*mm, (180-23.327)*deg, 23.327*deg);
    G4Tubs* subCyl2 = new G4Tubs("Sub Cyl 2", 0, 78.74/2*mm, 40/2*mm, 0*deg, 23.327*deg);
    G4Tubs* subCyl3 = new G4Tubs("Sub Cyl 3", 0, 3.048/2*mm, 40/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl4 = new G4Tubs("Sub Cyl 4", 0, 4.496/2*mm, 100/2*mm, 0, 2*M_PI);
    G4Box* subPrism1 = new G4Box("Sub Prism 1",  (7.028-3.81)/2*mm+epsilon, 21.59/2*mm+epsilon, 8/2*mm);  
    G4Box* subPrism2 = new G4Box("Sub Prism 2",  4/2*mm, 4/2*mm, 8/2*mm);  

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix; //7.336, 2.794
    rotY->rotateY(90.*deg);
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;

    partUnion = new G4UnionSolid("Part Union", cyl1, prism1, rotN, G4ThreeVector((40.968-9.218/2)*mm, (26.106-13.827/2)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, prism1, rotN, G4ThreeVector(-(40.968-9.218/2)*mm, (26.106-13.827/2)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, prism2, rotN, G4ThreeVector((44.143-7.028/2)*mm, (12.279-37.719/2)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, prism2, rotN, G4ThreeVector(-(44.143-7.028/2)*mm, (12.279-37.719/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, subCyl1, rotN, G4ThreeVector(-0.963*mm, -3.85*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl2, rotN, G4ThreeVector(0.963*mm, -3.85*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism1, rotN, G4ThreeVector((40.333-(7.028-3.81)/2)*mm, -(3.85+21.59/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism1, rotN, G4ThreeVector(-(40.333-(7.028-3.81)/2)*mm, -(3.85+21.59/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl3, rotN, G4ThreeVector(42.492*mm, 12.279*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl3, rotN, G4ThreeVector(-42.492*mm, 12.279*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism2, rotN, G4ThreeVector((42.492+4/2)*mm, (10.755+4/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism2, rotN, G4ThreeVector(-(42.492+4/2)*mm, (10.755+4/2)*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl4, rotY, G4ThreeVector(0, 16.47*mm, 0));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Cable Guard Narrow IR Shield Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cable Guard Narrow IR Shield Logical" << endlog; 
  return pVol;
}  
