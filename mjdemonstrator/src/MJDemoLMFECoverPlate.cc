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
// CLASS IMPLEMENTATION:  MJDemoLMFECoverPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Part Origin: bolt-hole side edge, top side (away from crystal)
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoLMFECoverPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoLMFECoverPlate::MJDemoLMFECoverPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-LMFECP", "Copper-EF")
{;}

MJDemoLMFECoverPlate::MJDemoLMFECoverPlate(const MJDemoLMFECoverPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoLMFECoverPlate::~MJDemoLMFECoverPlate()
{;}

G4LogicalVolume* MJDemoLMFECoverPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* tab = new G4Box("tab", 0.6875*25.4*mm, 0.26*25.4*mm, 0.015*25.4*mm);
    G4DisplacedSolid* movedTab = new G4DisplacedSolid("movedTab", tab, 0, 
    										G4ThreeVector(-0.6875*25.4*mm, 0, -0.015*25.4*mm));
    G4Box* base = new G4Box("base", 0.125*25.4*mm, 0.26*25.4*mm, 0.040*25.4*mm);
    G4UnionSolid* body = new G4UnionSolid("body", movedTab, base, 0, 
    										G4ThreeVector(-0.125*25.4*mm, 0, -0.04*25.4*mm));

    G4Tubs* cornerRound = new G4Tubs("cornerRound", 0.04*25.4*mm, 0.1*25.4*mm, 0.3*25.4*mm, 0, pi/2);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cornerRound, 0, 
    										G4ThreeVector(-0.04*25.4*mm, 0.22*25.4*mm, 0));
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(-pi/2);
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cornerRound, rotation, 
    										G4ThreeVector(-1.335*25.4*mm, 0.22*25.4*mm, 0));
    rotation->rotateZ(-pi/2);
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cornerRound, rotation, 
    										G4ThreeVector(-1.335*25.4*mm, -0.22*25.4*mm, 0));
    rotation->rotateZ(-pi/2);
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, cornerRound, rotation, 
    										G4ThreeVector(-0.04*25.4*mm, -0.22*25.4*mm, 0));

    rotation->rotateZ(pi/2);
    rotation->rotateX(pi/2);
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, cornerRound, rotation, 
    										G4ThreeVector(-0.21*25.4*mm, 0, -0.04*25.4*mm));    

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.064*25.4*mm, 0.1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, thruHole, 0, 
    										G4ThreeVector(-0.13*25.4*mm, 0.15*25.4*mm, 0));
    G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7, thruHole, 0, 
    										G4ThreeVector(-0.13*25.4*mm, -0.15*25.4*mm, 0));    										

    G4Box* cablePass = new G4Box("cablePass", 0.5*25.4*mm, 0.035*25.4*mm, 0.1*25.4*mm);
    G4SubtractionSolid* coverPlate = new G4SubtractionSolid("Cover Plate", body8, cablePass, 0,
    										G4ThreeVector(0, 0, -0.16*25.4*mm));


    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(coverPlate, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created LMFE Cover Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing LMFE Cover Plate Logical" << endlog; 
  return pVol;
}  
