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
// CLASS IMPLEMENTATION:  MJDemoTSShieldBlock.cc
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
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoTSShieldBlock.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoTSShieldBlock::MJDemoTSShieldBlock(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "TSShieldBlockdwg", "Copper-EF")
{;}

MJDemoTSShieldBlock::MJDemoTSShieldBlock(const MJDemoTSShieldBlock & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoTSShieldBlock::~MJDemoTSShieldBlock()
{;}

G4LogicalVolume* MJDemoTSShieldBlock::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* cyl = new G4Tubs("flange", 0.750*25.4*mm, 1.5*25.4*mm, 0.75*25.4*mm, 0, 2*pi);
    G4Box* box = new G4Box("box", 2.0*25.4*mm, 2.0*25.4*mm, 2.0*25.4*mm); 
    G4Tubs* drillhole = new G4Tubs("drillhole", 0, 0.125*25.4*mm, 1.0*25.4*mm, 0, 2*pi);
    G4Box* box2 = new G4Box("box", 0.750*25.4*mm, 0.345*25.4*mm, 2.0*25.4*mm);
    
    G4RotationMatrix* rot = new G4RotationMatrix;
    G4SubtractionSolid* block1 = new G4SubtractionSolid("block1", cyl, box, rot, 
    									G4ThreeVector(0, 2.840*25.4*mm, 0));
    G4SubtractionSolid* block2 = new G4SubtractionSolid("block2", block1, box, rot, 
    									G4ThreeVector(3.125*25.4*mm, 2.050*25.4*mm, 0));
    G4SubtractionSolid* block3 = new G4SubtractionSolid("block3", block2, box, rot, 
    									G4ThreeVector(-3.125*25.4*mm, 2.050*25.4*mm, 0));									
    G4SubtractionSolid* block4 = new G4SubtractionSolid("block4", block3, drillhole, rot, 
    									G4ThreeVector(0*25.4*mm, -1.06*25.4*mm, 0));
    G4SubtractionSolid* block5 = new G4SubtractionSolid("block5", block4, drillhole, rot, 
    									G4ThreeVector(0.938*25.4*mm, -0.06*25.4*mm, 0));
    G4SubtractionSolid* block6 = new G4SubtractionSolid("block6", block5, drillhole, rot, 
    									G4ThreeVector(-0.938*25.4*mm, -0.06*25.4*mm, 0));
    G4SubtractionSolid* block7 = new G4SubtractionSolid("block7", block6, box2, rot, 
    									G4ThreeVector(0, 0.345*25.4*mm, 0));   


    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(block7, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created TS Shield Block Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing TS Shield Block Logical" << endlog; 
  return pVol;
}  
