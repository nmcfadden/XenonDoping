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
// CLASS IMPLEMENTATION:  MJDemoFlexInsulator.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin: Coaxial with hole, coincident with crystal surface
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Mar 13, 2014
 * 
 * REVISION:
 * 
 * 03-13-2014, Created, K. Vorren
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoFlexInsulator.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoFlexInsulator::MJDemoFlexInsulator(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-***", "Teflon"),
  fDetectorRadius(34*mm)
{;}

MJDemoFlexInsulator::MJDemoFlexInsulator(const MJDemoFlexInsulator & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoFlexInsulator::~MJDemoFlexInsulator()
{;}

G4LogicalVolume* MJDemoFlexInsulator::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = /*fPartName +*/ fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* wedgeBody = new G4Box("WedgeBody", 0.24/2*25.4*mm, 0.32/2*25.4*mm, 0.600/2*25.4*mm);
    G4Box* chamfer1 = new G4Box("Chamfer1", 0.40/2*25.4*mm, 0.32/2*25.4*mm, 0.65/2*25.4*mm);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateZ(-0.395);
    G4SubtractionSolid* halfWedge = new G4SubtractionSolid("HalfWedge", wedgeBody, chamfer1, rotation1, G4ThreeVector(0.0523*25.4*mm,-0.261*25.4*mm,0));
    
    G4Box* chamfer2 = new G4Box("Chamfer2", 0.40/2*25.4*mm, 0.32/2*25.4*mm, 0.65/2*25.4*mm);
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateZ(0.395);
    G4SubtractionSolid* trapBody = new G4SubtractionSolid("TrapBody", halfWedge, chamfer2, rotation2, G4ThreeVector(0.0523*25.4*mm, 0.261*25.4*mm, 0));

    G4Box* back = new G4Box("back", 0.006*25.4*mm, 0.32/2*25.4*mm, 0.4*25.4*mm);
    G4SubtractionSolid* backOff = new G4SubtractionSolid("backOff", trapBody, back, 0, G4ThreeVector(0.12*25.4*mm, 0, 0));

    G4Box* ledge = new G4Box("ledge", 0.070*25.4*mm, 0.32*25.4*mm, 0.065*25.4*mm);
    G4SubtractionSolid* ledgeCut = new G4SubtractionSolid("ledgeCut", backOff, ledge, 0, G4ThreeVector(0.12*25.4*mm, 0, -0.300*25.4*mm));

    G4Box* body2 = new G4Box("body2", 0.36/2*25.4*mm, 0.32/2*25.4*mm, 0.600/2*25.4*mm);
    G4UnionSolid* fullBody = new G4UnionSolid("fullBody", ledgeCut, body2, 0, G4ThreeVector((-0.12 - 0.36/2)*25.4*mm, 0, 0));

    G4Tubs* snapFitting = new G4Tubs("snapFitting", 0, 0.1075*25.4*mm, 0.04*25.4*mm, 0, 2*pi);
    G4UnionSolid* bodyWithSnap = new G4UnionSolid("bodyWithSnap", fullBody, snapFitting, 0, G4ThreeVector(-0.111*25.4*mm, 0, -0.32*25.4*mm));

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.0845*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    G4Tubs* wideHole = new G4Tubs("wideHole", 0, 0.109*25.4*mm, 0.485*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* bodyThruHole = new G4SubtractionSolid("bodyThruHole", bodyWithSnap, thruHole, 0, G4ThreeVector(-0.111*25.4*mm, 0, 0));
    G4SubtractionSolid* bodyWideHole = new G4SubtractionSolid("bodyWideHole", bodyThruHole, wideHole, 0, G4ThreeVector(-0.111*25.4*mm, 0, 0.3*25.4*mm));
    
    //Now have to subtract out the portion that the detector rests on. Determine to first order by detector radius

    G4double testZ = 1*mm;

    G4Cons* topBevel = new G4Cons("topBevel", 0, fDetectorRadius, 0, fDetectorRadius + 0.09*25.4*mm, 0.09/2*25.4*mm, 0, 2*pi);
    G4Tubs* detRad = new G4Tubs("detRad", 0, fDetectorRadius, 0.25*25.4*mm + testZ, 0, 2*pi);
    G4Cons* botBevel = new G4Cons("botBevel", 0, fDetectorRadius - 0.105*25.4*mm, 0, fDetectorRadius - 0.03*25.4*mm, 0.0375*25.4*mm, 0, 2*pi);
    G4Box* bottomOut = new G4Box("bottomOut", fDetectorRadius - 0.105*25.4*mm, 0.4*25.4*mm, 0.6*25.4*mm);
    G4SubtractionSolid* topBevelSub = new G4SubtractionSolid("topBevelSub", bodyWideHole, topBevel, 0, G4ThreeVector((-0.111-1.490)*25.4*mm, 0, 0.255*25.4*mm));
    G4SubtractionSolid* detRadSub = new G4SubtractionSolid("detRadSub", topBevelSub, detRad, 0, G4ThreeVector((-0.111-1.490)*25.4*mm, 0, 0.3*25.4*mm));
    G4SubtractionSolid* botBevelSub = new G4SubtractionSolid("botBevelSub", detRadSub, botBevel, 0, G4ThreeVector((-0.111-1.490)*25.4*mm, 0, 0.0125*25.4*mm - testZ));
    
    G4SubtractionSolid* flexInsulator = new G4SubtractionSolid("flexInsulator", botBevelSub, bottomOut, 0, G4ThreeVector((-0.111-1.490)*25.4*mm, 0, 0));
    
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(flexInsulator, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Flex Insulator Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Flex Insulator Logical" << endlog; 
  return pVol;
}  
