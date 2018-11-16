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
// CLASS IMPLEMENTATION:  MJDemoHVCableTakeupWStackingHoles.cc
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
 * FIRST SUBMISSION: June 6, 2014
 * 
 * REVISION:
 * 
 * 06-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoHVCableTakeupWStackingHoles.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVCableTakeupWStackingHoles::MJDemoHVCableTakeupWStackingHoles(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "drawing number", "Copper-EF")
{;}

MJDemoHVCableTakeupWStackingHoles::MJDemoHVCableTakeupWStackingHoles(const MJDemoHVCableTakeupWStackingHoles & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVCableTakeupWStackingHoles::~MJDemoHVCableTakeupWStackingHoles()
{;}

G4LogicalVolume* MJDemoHVCableTakeupWStackingHoles::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.01*mm;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 64.16/2*mm, 3.175/2*mm, 25.4/2*mm);
    G4Box* pillarPrism = new G4Box("Pillar Prism", 17.793/2*mm, 3.175/2*mm, 25.4/2*mm+epsilon);
    G4Tubs* pillarSubCyl = new G4Tubs("Sub Top Pillar", 0.0*mm, 7.925/2*mm, 9.525*2/2*mm, 0, 2*M_PI);
    G4Tubs* pillarSubCyl2 = new G4Tubs("Sub Top Pillar 2", 0.0*mm, 11.176/2*mm, 2.794/2*mm, 0, 2*M_PI);
    G4Tubs* topPillar = new G4Tubs("Top Pillar", 0.0*mm, 25.4/2*mm, 9.525/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotX = new G4RotationMatrix;    
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    G4SubtractionSolid* subSolid;
    G4UnionSolid* unionSolid;

    unionSolid = new G4UnionSolid("Union Solid", mainRecPrism, topPillar, rotX, G4ThreeVector((64.16/2-17.793)*mm, (3.175/2+9.525/2)*mm, (0)*mm)); 
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, topPillar, rotX, G4ThreeVector(-(64.16/2-17.793)*mm, (3.175/2+9.525/2)*mm, (0)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, pillarPrism, rotN, G4ThreeVector((64.16/2-17.793/2)*mm, (3.175/2+6.35+3.175/2)*mm, (0.)*mm));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, pillarPrism, rotN, G4ThreeVector(-(64.16/2-17.793/2)*mm, (3.175/2+6.35+3.175/2)*mm, (0.)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", unionSolid, pillarSubCyl, rotX, G4ThreeVector((64.16/2-17.793)*mm, (3.175/2+9.525/2)*mm, (0)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, pillarSubCyl, rotX, G4ThreeVector(-(64.16/2-17.793)*mm, (3.175/2+9.525/2)*mm, (0)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, pillarSubCyl2, rotX, G4ThreeVector((64.16/2-17.793)*mm, (-3.175/2+2.794/2)*mm, (0)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, pillarSubCyl2, rotX, G4ThreeVector(-(64.16/2-17.793)*mm, (-3.175/2+2.794/2)*mm, (0)*mm));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created HV Cable Takeup W Stacking Holes Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing HV Cable Takeup W Stacking Holes Logical" << endlog; 
  return pVol;
}  
