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
/**                                                            
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoHVFork.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the copper cable guide. Part origin: Center of the hex hole
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 04-23-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polyhedra.hh"
#include "G4DisplacedSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHVFork.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVFork::MJDemoHVFork(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-019", "Copper-EF")
{;}

MJDemoHVFork::MJDemoHVFork(const MJDemoHVFork & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVFork::~MJDemoHVFork()
{;}

G4LogicalVolume* MJDemoHVFork::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zHeight = 0.070/2*25.4*mm;
    G4RotationMatrix* rot = new G4RotationMatrix();
    G4Box* body = new G4Box("body", 0.4525/2*25.4*mm, 0.380/2*25.4*mm, zHeight);
    //Top right corner
    G4Box* corner = new G4Box("corner", .075*25.4*mm, .040*25.4*mm, .12/2*25.4*mm);
    G4SubtractionSolid* boxCutter = new G4SubtractionSolid("boxCutter", body, corner, rot, G4ThreeVector(0.22625*25.4*mm, .380/2*25.4*mm, 0));
    //Fork
    G4Tubs* forkCurve = new G4Tubs("curve", 0, 0.140*25.4*mm, .12/2*25.4*mm, 0, 2*pi);
    boxCutter = new G4SubtractionSolid("boxCutter2", boxCutter, forkCurve, rot, G4ThreeVector(-0.07625*25.4*mm, 0, 0));
    G4Box* forkRect = new G4Box("forkRect", 0.150*25.4*mm, 0.140*25.4*mm, 0.12/2*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter3", boxCutter, forkRect, rot, G4ThreeVector(-0.4525/2*25.4*mm, 0, 0));
    //Little holes
    G4Tubs* littleHole = new G4Tubs("littleHole", 0, 0.032/2*25.4*mm, 0.12/2*25.4*mm, 0, 2*pi);
    boxCutter = new G4SubtractionSolid("boxCutter4", boxCutter, littleHole, rot, G4ThreeVector(0.3975/2*25.4*mm, 0.11*25.4*mm, 0));
    boxCutter = new G4SubtractionSolid("boxCutter5", boxCutter, littleHole, rot, G4ThreeVector(0.3975/2*25.4*mm, 0.03*25.4*mm, 0));
    //big hole
    G4Tubs* bigHole = new G4Tubs("bigHole", 0, 0.041/2*25.4*mm, 0.12/2*25.4*mm, 0, 2*pi);
    boxCutter = new G4SubtractionSolid("boxCutter6", boxCutter, bigHole, rot, G4ThreeVector(0.3975/2*25.4*mm, -0.1*25.4*mm, 0));
    //ledge
    G4Box* ledge = new G4Box("ledge", 0.4525/2*25.4*mm, 0.380*25.4*mm, 0.040*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter7", boxCutter, ledge, rot, G4ThreeVector(-0.055*25.4*mm, 0, zHeight));
    //Below makes the logical volume
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(boxCutter, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Inner Cable Guide Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Inner Cable Guide Logical" << endlog; 
  return pVol;
}  
