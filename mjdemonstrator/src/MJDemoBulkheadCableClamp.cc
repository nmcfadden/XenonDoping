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
// CLASS IMPLEMENTATION:  MJDemoBulkheadCableClamp.cc
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
 * FIRST SUBMISSION: June 17, 2014
 * 
 * REVISION:
 * 
 * 17-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoBulkheadCableClamp.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoBulkheadCableClamp::MJDemoBulkheadCableClamp(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-172", "Copper-EF")
{;}

MJDemoBulkheadCableClamp::MJDemoBulkheadCableClamp(const MJDemoBulkheadCableClamp & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoBulkheadCableClamp::~MJDemoBulkheadCableClamp()
{;}

G4LogicalVolume* MJDemoBulkheadCableClamp::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism", 9.525/2*mm, 6.35/2*mm, 31.75/2*mm);
    G4Box* sideSubRecPrism1 = new G4Box("Subtracted Prism from Side 1", 9.525/2*mm+epsilon, 0.635/2*mm + epsilon, 21.474/2*mm);
    G4Tubs* sideSubCyl = new G4Tubs("Subtracted Cylinder from Side", 0.0*mm, 1.524/2*mm, 12.*mm + epsilon, 0, 2*M_PI);
    G4Tubs* screwSubCyl = new G4Tubs("Cylindrical Hole", 0.0*mm, 4.496/2*mm, 12.7/2*mm + epsilon, 0, 2*M_PI);

    G4RotationMatrix* rotX = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix;
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(M_PI/2*rad);
    G4SubtractionSolid* subSolid;

    subSolid = new G4SubtractionSolid("Sub Solid", mainRecPrism, sideSubRecPrism1, rotN, G4ThreeVector(0, (6.35-0.635)/2*mm, (31.75-21.474)/2*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotY, G4ThreeVector(0, (6.35/2)*mm, -(31.75/2-10.276)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotX, G4ThreeVector(0, 0, (31.75/2-3.175)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl, rotX, G4ThreeVector(0, 0, -(31.75/2-3.175)*mm));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Bulkhead Cable Clamp Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Bulkhead Cable Clamp Logical" << endlog; 
  return pVol;
}
