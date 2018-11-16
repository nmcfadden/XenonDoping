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
// CLASS IMPLEMENTATION:  MJSTCCMSSpool.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from MJ83-01-029 rev0 (w/o grooves)
 * Part Origin: At the top of the spool, (x,y) centered.
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
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCCMSSpool.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCCMSSpool::MJSTCCMSSpool(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCCMSSpoolDwg", "Aluminum") 
{;}

MJSTCCMSSpool::MJSTCCMSSpool(const MJSTCCMSSpool & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCCMSSpool::~MJSTCCMSSpool()
{;}

G4LogicalVolume* MJSTCCMSSpool::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
	//zPlanes[4] = zPlanes[5] = -(39.37-7.62)*mm is a guess - the drawing isn't marked
  	G4double zPlanes[] = {0*mm, -6.35*mm, -6.35*mm, -(39.37-7.62)*mm, -(39.37-7.62)*mm, -39.37*mm};
  	G4double rInner[] = {0*mm, 0*mm, 38.1*mm, 38.1*mm, 38.1*mm, 38.1*mm}; 
	G4double rOuter[] = {50.8*mm, 50.8*mm, 50.8*mm, 50.8*mm, 66.675*mm, 66.675*mm};
    G4Polycone* bulk = new G4Polycone("bulk", 0, 2*pi, 6, zPlanes, rInner, rOuter);

    G4Tubs* thruHole = new G4Tubs("thruHole", 0.0*mm, 1.3589*mm, 10.0*mm, 0, 2*pi);
    G4UnionSolid* setof2 = new G4UnionSolid("setof2", thruHole, thruHole,
    										0, G4ThreeVector(sin(pi/16.0)*60.96*mm, (cos(pi/16.0)-1.0)*60.96*mm, 0));
 	G4RotationMatrix* rotationSetof2 = new G4RotationMatrix();
	rotationSetof2->rotateZ(2.0*pi/16.0);
   	G4UnionSolid* setof4 = new G4UnionSolid("setof4", setof2, setof2,
    										rotationSetof2, G4ThreeVector(sin(2.0*pi/16.0)*60.96*mm, (cos(2.0*pi/16.0)-1.0)*60.96*mm, 0));
  	G4RotationMatrix* rotationSetof4 = new G4RotationMatrix();
	rotationSetof4->rotateZ(4.0*pi/16.0);
   	G4UnionSolid* setof8 = new G4UnionSolid("setof8", setof4, setof4,
    										rotationSetof4, G4ThreeVector(sin(4.0*pi/16.0)*60.96*mm, (cos(4.0*pi/16.0)-1.0)*60.96*mm, 0));
  	G4RotationMatrix* rotationSetof8 = new G4RotationMatrix();
	rotationSetof8->rotateZ(8.0*pi/16.0);
   	G4UnionSolid* setof16 = new G4UnionSolid("setof16", setof8, setof8,
    										rotationSetof8, G4ThreeVector(sin(8.0*pi/16.0)*60.96*mm, (cos(8.0*pi/16.0)-1.0)*60.96*mm, 0));
  	G4RotationMatrix* rotationSetof16 = new G4RotationMatrix();
	rotationSetof16->rotateZ(16.0*pi/16.0);
  	G4UnionSolid* setof32 = new G4UnionSolid("setof32", setof16, setof16,
    										rotationSetof16, G4ThreeVector(sin(16.0*pi/16.0)*60.96*mm, (cos(16.0*pi/16.0)-1.0)*60.96*mm, 0));
    G4SubtractionSolid* thruHoles = new G4SubtractionSolid("thruHoles", bulk, setof32,
    										0, G4ThreeVector(0*mm, 60.96*mm, -39.37*mm));

    G4Tubs* tappedHole = new G4Tubs("tappedHole", 0.0*mm, 1.3589*mm, 9.398*mm, 0, 2*pi);
	G4SubtractionSolid* tappedHole01 = new G4SubtractionSolid("tappedHole01",thruHoles,tappedHole,
											0, G4ThreeVector(0*mm, 45.72*mm, 0*mm));
	G4SubtractionSolid* tappedHole02 = new G4SubtractionSolid("tappedHole02",tappedHole01,tappedHole,
											0, G4ThreeVector(0*mm, -45.72*mm, 0*mm));
	
	G4Tubs* topCut = new G4Tubs("topCut", 0.0*mm, 38.1*mm, 7*mm, 0, 2*pi);
	G4Box* cover = new G4Box("cover", 45.0*mm, 45.0*mm, 8*mm);
	G4SubtractionSolid* cutWithCover = new G4SubtractionSolid("cutWithCover", topCut, cover,
											0, G4ThreeVector(0*mm, -(45.0+12.7)*mm, 0*mm));
	G4Tubs* roundedEdge = new G4Tubs("roundedEdge", 6.35*mm, 8*mm, 8*mm, 0, pi/2.0);
	G4RotationMatrix* rotationRoundedEdge01 = new G4RotationMatrix();
	rotationRoundedEdge01->rotateZ(pi/2.0);
    G4SubtractionSolid* roundedEdge01 = new G4SubtractionSolid("roundedEdge01", cutWithCover, roundedEdge,
											rotationRoundedEdge01, G4ThreeVector((sqrt(38.1*38.1-6.35*6.35)-6.35)*mm, -(12.7-6.35)*mm, 0*mm));
	G4RotationMatrix* rotationRoundedEdge02 = new G4RotationMatrix();
	rotationRoundedEdge02->rotateZ(pi);
    G4SubtractionSolid* roundedEdge02 = new G4SubtractionSolid("roundedEdge02", roundedEdge01, roundedEdge,
											rotationRoundedEdge02, G4ThreeVector(-(sqrt(38.1*38.1-6.35*6.35)-6.35)*mm, -(12.7-6.35)*mm, 0*mm));
	G4SubtractionSolid* withTopCut = new G4SubtractionSolid("withTopCut", tappedHole02, roundedEdge02,
											0, G4ThreeVector(0*mm, 0*mm, 0*mm));

	G4Tubs* ring = new G4Tubs("ring", 44.45*mm, 48.895*mm, 6.35*mm, 0, 2*pi);
	G4SubtractionSolid* withRing = new G4SubtractionSolid("withRing", withTopCut, ring,
											0, G4ThreeVector(0*mm, 0*mm, -39.37*mm));
 										    
    G4VisAttributes* AlVisAtt = new G4VisAttributes(G4Colour(0.75, 0.75, 0.75)); // light gray
    AlVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(withRing, material, logicalName);
    pVol->SetVisAttributes(AlVisAtt); 
    MGLog(debugging) << "Created STC CMS Spool Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC CMS Spool Logical" << endlog; 
  return pVol;
}  
