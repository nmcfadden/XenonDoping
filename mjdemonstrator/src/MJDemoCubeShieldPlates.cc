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
// CLASS IMPLEMENTATION:  MJDemoCubeShieldPlates.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Center of curvature of TS cutout, mid-depth in block
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Mar 4, 2016
 *
 * REVISION:
 *
 * 03-04-2016, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCubeShieldPlates.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCubeShieldPlates::MJDemoCubeShieldPlates(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CubeShieldPlatesdwg", "Copper-EF")
{;}

MJDemoCubeShieldPlates::MJDemoCubeShieldPlates(const MJDemoCubeShieldPlates & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCubeShieldPlates::~MJDemoCubeShieldPlates()
{;}

G4LogicalVolume* MJDemoCubeShieldPlates::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* lowerplates = new G4Box("plates", 2.75*25.4*mm, 2.75*25.4*mm, 0.4*25.4*mm);
    G4Box* upperplates = new G4Box("upperplates", 1.25*25.4*mm, 2.75*25.4*mm, 0.4*25.4*mm);

    G4RotationMatrix* rot = new G4RotationMatrix;
    G4UnionSolid* plates = new G4UnionSolid("plates", lowerplates, upperplates, rot, 
													G4ThreeVector(0, 2.75*25.4*mm, 0));
    
    G4Tubs* hole = new G4Tubs("hole", 0*25.4*mm, 0.75*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    
    G4SubtractionSolid* shield = new G4SubtractionSolid("shield", plates, hole, rot, 
    									G4ThreeVector(0, 0, 0));
   


    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(shield, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cube Shield Plates Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cube Shield Plates Logical" << endlog; 
  return pVol;
}  
