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
 * CLASS DECLARATION:  MJDemoCableGuideInsert.hh
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
 * 05-01-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
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
#include "mjdemonstrator/MJDemoCableGuideInsert.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableGuideInsert::MJDemoCableGuideInsert(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ81-01-006", "Copper-EF")
{;}

MJDemoCableGuideInsert::MJDemoCableGuideInsert(const MJDemoCableGuideInsert & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCableGuideInsert::~MJDemoCableGuideInsert()
{;}

G4LogicalVolume* MJDemoCableGuideInsert::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4RotationMatrix* rot = new G4RotationMatrix();
    //body
    G4Box* body = new G4Box("body", 0.347/2*25.4*mm, 0.880/2*25.4*mm, 0.135/2*25.4*mm);
    //clip cut
    G4Box* clipCut = new G4Box("clipCut", 0.045*25.4*mm, 1.16/2*25.4*mm, 0.27/2*25.4*mm);
    G4SubtractionSolid* boxCutter = new G4SubtractionSolid("boxCutter", body, clipCut, rot, G4ThreeVector(-0.347/2*25.4*mm, -0.880/2*25.4*mm, 0));
    //rounded corner
    G4Tubs* roundCorner = new G4Tubs("roundCorner", 0.210*25.4*mm, 2*25.4*mm, 0.135*25.4*mm, 0, pi/2);
    boxCutter = new G4SubtractionSolid("boxCutter2", boxCutter, roundCorner, rot, G4ThreeVector(-0.0355*25.4*mm, 0.23*25.4*mm, 0));
    //right edge
    G4Tubs* roundedEdge = new G4Tubs("roundedEdge", 3.8265*25.4*mm, 4*25.4*mm, 0.135/2*25.4*mm, -0.152/2, 0.152);
    G4UnionSolid* boxAdder = new G4UnionSolid("boxAdder", boxCutter, roundedEdge, rot, G4ThreeVector(-3.815*25.4*mm, -0.0565*25.4*mm, 0));
    //right fork
    G4Tubs* innerTopCorner = new G4Tubs("innerTopCorner", 0, 0.150*25.4*mm, 0.135*25.4*mm, 0, pi/2);
    boxCutter = new G4SubtractionSolid("boxCutter3", boxAdder, innerTopCorner, rot, G4ThreeVector(-0.0355*25.4*mm, 0.23*25.4*mm, 0));
    G4Tubs* rightFork = new G4Tubs("rightFork", 2.85*25.4*mm, 3.0*25.4*mm, 0.135*25.4*mm, -0.08677, 0.20374);
    boxCutter = new G4SubtractionSolid("boxCutter5", boxCutter, rightFork, rot, G4ThreeVector(-2.865*25.4*mm, -0.103*25.4*mm, 0));
    G4Tubs* rightCurl = new G4Tubs("rightCurl", 0.150*25.4*mm, .30*25.4*mm, 0.135*25.4*mm, -pi/2, pi/2);
    boxCutter = new G4SubtractionSolid("boxCutter6", boxCutter, rightCurl, rot, G4ThreeVector(0.0265*25.4*mm, -0.36*25.4*mm, 0));
    G4Box* interior = new G4Box("interior", .1908/2*25.4*mm, .62*25.4*mm, 0.135*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter7", boxCutter, interior, rot, G4ThreeVector(0.0269*25.4*mm, -0.440*25.4*mm, 0));
    G4Box* interior2 = new G4Box("interior2", 0.245/4*25.4*mm, 0.1*25.4*mm, .135*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter8", boxCutter, interior2, rot, G4ThreeVector(0, 0.247*25.4*mm, 0));
    //left fork
    G4Box* leftFork = new G4Box("leftFork", 0.036/2*25.4*mm, 0.820*25.4*mm, 0.135*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter4", boxCutter, leftFork, rot, G4ThreeVector(-.0535*25.4*mm, -0.880/2*25.4*mm, 0));
    //clip addition
    G4Box* clipAdd = new G4Box("clipAdd", 0.0205/2*25.4*mm, 0.0777/2*25.4*mm, 0.135/2*25.4*mm);
    rot->rotateZ(-18*deg);
    boxAdder = new G4UnionSolid("boxAdder2", boxCutter, clipAdd, rot, G4ThreeVector(-.12935*25.4*mm, -.4*25.4*mm, 0));
    //Below makes the logical volume
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(boxAdder, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Inner Cable Guide Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Inner Cable Guide Logical" << endlog; 
  return pVol;
}  
