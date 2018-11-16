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
// CLASS IMPLEMENTATION:  MJDemoRadShieldCuInner.cc
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 20, 2010
 * 
 * REVISION:
 * 
 * 09-20-2010, Created, M. Green
 * 01-12-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoRadShieldCuInner.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldCuInner::MJDemoRadShieldCuInner(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldCuInnerDwg", serialNumber)
{;}

MJDemoRadShieldCuInner::MJDemoRadShieldCuInner(const MJDemoRadShieldCuInner & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoRadShieldCuInner::~MJDemoRadShieldCuInner()
{;}

G4LogicalVolume* MJDemoRadShieldCuInner::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 18*25.4*mm, 9.875*25.4*mm, 11.923*25.4*mm);
    G4Box* pocket = new G4Box("pocket", 16.031*25.4*mm, 7.906*25.4*mm, 9.954*25.4*mm);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, pocket);
    
    G4Tubs* tubeCut = new G4Tubs("tubeCut", 0, 1.751*25.4*mm, 20*25.4*mm, 0, 2*pi);//1.75
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateY(pi/2);
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, tubeCut, rotation,
    									G4ThreeVector(30*25.4*mm, 0*25.4*mm, 4.4265*25.4*mm));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateX(pi/2);
    G4SubtractionSolid* shield = new G4SubtractionSolid("shield", body3, tubeCut, rotation2,
    						G4ThreeVector(-8.1417*25.4*mm, -20*25.4*mm, 4.4265*25.4*mm));    
    
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(shield, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Inner Copper Shield Logical: " << fPartMaterial << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Inner Copper Shield Logical" << endlog; 
  return pVol;
}  
