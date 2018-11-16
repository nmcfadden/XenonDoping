//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJSTCCMSStand.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from MJ83-01-028 rev0
 * Geometry is not exact; some things were simplified.
 * Part Origin: The middle of the width (z) of the "baseplate" (the horizontally-flat part of the CMS stand)
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Tuesday January 21 2014
 * 
 * REVISION:
 * 01-21-2014, Created. Used to be a part of ColdPlateAssembly. I separated
 *             it to make it easier to rotate it through the messenger, J. MacMullin
 *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCCMSStand.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCCMSStand::MJSTCCMSStand(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCCMSStandDwg", "Aluminum") 
{;}

MJSTCCMSStand::MJSTCCMSStand(const MJSTCCMSStand & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCCMSStand::~MJSTCCMSStand()
{;}

G4LogicalVolume* MJSTCCMSStand::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
	G4Tubs* basePlate = new G4Tubs("basePlate", 44.704*mm, 69.85*mm, 3.175*mm, 0, 2.0*pi);
	
	G4Box* slotMain = new G4Box("slotMain", 14.605*mm, ((53.34*(1.0-0.0232290)-3.175)/2.0)*mm, 4*mm);
	G4Tubs* topCorner = new G4Tubs("topCorner", 0, 3.175*mm, 6*mm, 0, 2*pi);
	G4UnionSolid* topCorner01 = new G4UnionSolid("topCorner01", slotMain, topCorner,
											0, G4ThreeVector((14.605-3.175)*mm, ((53.34*(1.0-0.0232290)-3.175)/2.0)*mm, 0));
	G4UnionSolid* topCorner02 = new G4UnionSolid("topCorner02", topCorner01, topCorner,
											0, G4ThreeVector(-(14.605-3.175)*mm, ((53.34*(1.0-0.0232290)-3.175)/2.0)*mm, 0));
 	G4Tubs* topOfSlot = new G4Tubs("topOfSlot", 0, 53.34*mm, 6*mm, 0, 2.0*(0.215960)+pi/90.0);
  	G4RotationMatrix* rotationTopOfSlot = new G4RotationMatrix();
    rotationTopOfSlot->rotateZ(-pi/2.0+0.215960+pi/180.0);
 	G4UnionSolid* slot = new G4UnionSolid("slot", topCorner02, topOfSlot,
											rotationTopOfSlot, G4ThreeVector(0, -((53.34*(1.0-0.0232290)-3.175)/2.0)*mm, 0));
	G4SubtractionSolid* withSlot = new G4SubtractionSolid("withSlot", basePlate, slot,
											0, G4ThreeVector(0, ((53.34*(1.0-0.0232290)-3.175)/2.0)*mm, 0)); 
    
	G4Tubs* bottomCorner = new G4Tubs("bottomCorner", 1.27*mm, 4.0*mm, 9.0*mm, 0, pi/2.0-0.352563);
	G4RotationMatrix* rotationBottomCorner01 = new G4RotationMatrix();
	rotationBottomCorner01->rotateZ(pi/2.0-0.352563);
	G4SubtractionSolid* bottomCorner01 = new G4SubtractionSolid("bottomCorner01", withSlot, bottomCorner,
											rotationBottomCorner01, G4ThreeVector(-sin(0.352563)*(44.704+1.27)*mm, cos(0.352563)*(44.704+1.27)*mm, 0*mm));
	G4RotationMatrix* rotationBottomCorner02 = new G4RotationMatrix();
	rotationBottomCorner02->rotateZ(-pi);
	G4SubtractionSolid* bottomCorner02 = new G4SubtractionSolid("bottomCorner02", bottomCorner01, bottomCorner,
											rotationBottomCorner02, G4ThreeVector(sin(0.352563)*(44.704+1.27)*mm, cos(0.352563)*(44.704+1.27)*mm, 0*mm));
												
	G4Tubs* ring = new G4Tubs("ring", 44.704*mm, (44.704+2.54)*mm, (6.35+1.0)*mm, 0, 2.0*pi-2.0*0.352563);
	G4Tubs* corner = new G4Tubs("corner", 0.0*mm, 1.27*mm, (6.35+1.0)*mm, 0, pi);
	G4RotationMatrix* rotationCorner01 = new G4RotationMatrix();
	rotationCorner01->rotateZ(pi);
	G4UnionSolid* corner01 = new G4UnionSolid("corner01", ring, corner,
											rotationCorner01, G4ThreeVector((44.704+1.27)*mm, 0*mm, 0*mm));
	G4RotationMatrix* rotationCorner02 = new G4RotationMatrix();
	rotationCorner02->rotateZ(2.0*0.352563);
	G4UnionSolid* corner02 = new G4UnionSolid("corner02", corner01, corner,
											rotationCorner02, G4ThreeVector(cos(2.0*0.352563)*(44.704+1.27)*mm, -sin(2.0*0.352563)*(44.704+1.27)*mm, 0*mm));
  	G4RotationMatrix* rotationRing = new G4RotationMatrix();
	rotationRing->rotateZ(-pi/2.0-0.352563);
	G4UnionSolid* withRing = new G4UnionSolid("withRing", bottomCorner02, corner02,
											rotationRing, G4ThreeVector(0, 0, (3.175+6.35-1.0)*mm));
    
    G4Tubs* grooveOverlap = new G4Tubs("grooveOverlap", (44.704+2.54-1.5)*mm, (44.704+2.54+1.27)*mm, (1.27+1.5)*0.5*mm, 0, 2.0*pi-2.0*0.352563);
    G4Torus* grooveCut = new G4Torus("grooveCut", 0.0*mm, 1.27*mm, (44.704+2.54+1.27)*mm, 0, 2*pi);
    G4SubtractionSolid* groove = new G4SubtractionSolid("groove", grooveOverlap, grooveCut, 
    										0, G4ThreeVector(0, 0, (1.27+1.5)*0.5*mm));
    G4RotationMatrix* rotationGroove = new G4RotationMatrix();
    rotationGroove->rotateZ(-pi/2.0-0.352563);
    G4UnionSolid* withGroove = new G4UnionSolid("withGroove", withRing, groove,
    										rotationGroove, G4ThreeVector(0, 0, (3.175+(1.27-1.5)*0.5)*mm));
		
  	G4Tubs* largeHole = new G4Tubs("largeHole", 0.0*mm, 2.5527*mm, 8.0*mm, 0, 2*pi);
    G4SubtractionSolid* largeHole01 = new G4SubtractionSolid("largeHole01", withGroove, largeHole,
    										0, G4ThreeVector(-50.8*mm, -12.7*mm, 0*mm));
    G4SubtractionSolid* largeHole02 = new G4SubtractionSolid("largeHole02", largeHole01, largeHole,
    										0, G4ThreeVector(50.8*mm, -12.7*mm, 0*mm));
    
    G4Tubs* smallHole = new G4Tubs("smallHole", 0.0*mm, 0.7874*mm, 8.0*mm, 0, 2*pi);
    G4UnionSolid* setof2 = new G4UnionSolid("setof2", smallHole, smallHole,
    										0, G4ThreeVector(sin(pi/16.0)*67.31*mm, (cos(pi/16.0)-1.0)*67.31*mm, 0));
 	G4RotationMatrix* rotationSetof2 = new G4RotationMatrix();
	rotationSetof2->rotateZ(2.0*pi/16.0);
   	G4UnionSolid* setof4 = new G4UnionSolid("setof4", setof2, setof2,
    										rotationSetof2, G4ThreeVector(sin(2.0*pi/16.0)*67.31*mm, (cos(2.0*pi/16.0)-1.0)*67.31*mm, 0));
  	G4RotationMatrix* rotationSetof4 = new G4RotationMatrix();
	rotationSetof4->rotateZ(4.0*pi/16.0);
   	G4UnionSolid* setof8 = new G4UnionSolid("setof8", setof4, setof4,
    										rotationSetof4, G4ThreeVector(sin(4.0*pi/16.0)*67.31*mm, (cos(4.0*pi/16.0)-1.0)*67.31*mm, 0));
  	G4RotationMatrix* rotationSetof8 = new G4RotationMatrix();
	rotationSetof8->rotateZ(8.0*pi/16.0);
   	G4UnionSolid* setof16 = new G4UnionSolid("setof16", setof8, setof8,
    										rotationSetof8, G4ThreeVector(sin(8.0*pi/16.0)*67.31*mm, (cos(8.0*pi/16.0)-1.0)*67.31*mm, 0));
  	G4RotationMatrix* rotationSetof16 = new G4RotationMatrix();
	rotationSetof16->rotateZ(16.0*pi/16.0);
  	G4UnionSolid* setof32 = new G4UnionSolid("setof32", setof16, setof16,
    										rotationSetof16, G4ThreeVector(sin(16.0*pi/16.0)*67.31*mm, (cos(16.0*pi/16.0)-1.0)*67.31*mm, 0));
    G4SubtractionSolid* allHoles = new G4SubtractionSolid("allHoles", largeHole02, setof32,
    										0, G4ThreeVector(0, 67.31*mm, 0*mm));
   										    
    G4VisAttributes* AlVisAtt = new G4VisAttributes(G4Colour(0.75, 0.75, 0.75)); // light gray
    AlVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(allHoles, material, logicalName);
    pVol->SetVisAttributes(AlVisAtt); 
    MGLog(debugging) << "Created STC CMS Stand Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC CMS Stand Logical" << endlog; 
  return pVol;
}  
