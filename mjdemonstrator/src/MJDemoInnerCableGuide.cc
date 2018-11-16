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
 * CLASS DECLARATION:  MJDemoInnerCableGuide.hh
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
 * 02-05-2015, Created, K. Vorren
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
#include "mjdemonstrator/MJDemoInnerCableGuide.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoInnerCableGuide::MJDemoInnerCableGuide(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-183", "Copper-EF")
{;}

MJDemoInnerCableGuide::MJDemoInnerCableGuide(const MJDemoInnerCableGuide & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoInnerCableGuide::~MJDemoInnerCableGuide()
{;}

G4LogicalVolume* MJDemoInnerCableGuide::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4RotationMatrix* rot = new G4RotationMatrix();
    //The body
    G4Box* body = new G4Box("body", 0.687*25.4*mm, 0.306/2*25.4*mm, 0.500/2*25.4*mm);
    G4Box* cut = new G4Box("cut", 0.1205/2*25.4*mm, 3/2*25.4*mm, .51/2*25.4*mm);
    G4Box* cut2 = new G4Box("cut2", 0.0962*25.4*mm, 3/2*25.4*mm, .51/2*25.4*mm);
    //---
    //Subtraction Solid to shear off the edges
    //---
    rot->rotateZ(17*deg);
    G4SubtractionSolid* boxCutter = new G4SubtractionSolid("boxCutter", body, cut, rot, G4ThreeVector(-0.687*25.4*mm, 0.153*25.4*mm, 0));
    rot->rotateZ(-2*17*deg);
    boxCutter = new G4SubtractionSolid("boxCutter2", boxCutter, cut, rot, G4ThreeVector(0.687*25.4*mm, 0.153*25.4*mm, 0));
    rot->rotateZ(-56*deg);
    boxCutter = new G4SubtractionSolid("boxCutter3", boxCutter, cut2, rot, G4ThreeVector(-.687*25.4*mm, -.153*25.4*mm , 0));
    rot->rotateZ(2*73*deg);
    boxCutter = new G4SubtractionSolid("boxCutter4", boxCutter, cut2, rot, G4ThreeVector(0.687*25.4*mm, -.153*25.4*mm, 0));
    //IT SEEMS THERE IS A SYMMETRY THAT ALLOW FOR THE -0.0711" in the y-coordinate instead of the x-coordinate
    //---
    //For the Clip holes
    //---
    G4Box* guide = new G4Box("guide", 0.140/2*25.4*mm, 0.34/2*25.4*mm, 0.510/2*25.4); //added .03" to guide hole length, and shifted them (guide inserts don't bend in simulated life)
    boxCutter = new G4SubtractionSolid("boxCutter5", boxCutter, guide, rot, G4ThreeVector((.458+.0287/2)*25.4*mm, (-.0025+.0088/2)*25.4*mm, 0));
    rot->rotateZ(-2*73*deg);
    boxCutter = new G4SubtractionSolid("boxCutter6", boxCutter, guide, rot, G4ThreeVector((-.458-.0287/2)*25.4*mm, (-.0025+.0088/2)*25.4*mm, 0));
    rot->rotateZ(73*deg);
    //---
    //The Hexagon
    //---
    G4Box* top = new G4Box("top", 0.085*25.4*mm, 0.250/2*25.4*mm, 0.510/2*25.4*mm);
    boxCutter = new G4SubtractionSolid("boxCutter7", boxCutter, top, rot, G4ThreeVector(0, 0.15*25.4*mm, 0));
    
    G4double zPlanes[] = {-0.510*25.4*mm, 0.510*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.127*25.4*mm, 0.127*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/3, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    boxCutter = new G4SubtractionSolid("boxCutter9", boxCutter, hex, rot, G4ThreeVector(0, 0.013*25.4*mm, 0));
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
