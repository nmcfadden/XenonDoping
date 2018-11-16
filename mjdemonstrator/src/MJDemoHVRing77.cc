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
// CLASS IMPLEMENTATION:  MJDemoHVRing77.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin:  Concentric with ring, coincident with flange surface.
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
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHVRing77.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVRing77::MJDemoHVRing77(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-007B", "Copper-EF")
{;}

MJDemoHVRing77::MJDemoHVRing77(const MJDemoHVRing77 & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVRing77::~MJDemoHVRing77()
{;}

G4LogicalVolume* MJDemoHVRing77::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double postRadius = 1.8400*25.4*mm;

    G4double zPlanes[] = {0*25.4*mm, -0.19*25.4*mm, -0.19*25.4*mm, -0.25*25.4*mm};
    G4double rInner[] = {1.5375*25.4*mm, 1.5375*25.4*mm, 1.25*25.4*mm, 1.25*25.4*mm};
    G4double rOuter[] = {1.5976*25.4*mm, 1.5976*25.4*mm, 1.5976*25.4*mm, 1.5976*25.4*mm};
    G4Polycone* body = new G4Polycone("body", 0, 2*pi, 4, zPlanes, rInner, rOuter);

    G4Tubs* boltFlange = new G4Tubs("boltFlange", 0,0.245*25.4*mm,
                                    0.030*25.4*mm, 0, 2*pi);
    G4Box* flangeFillet = new G4Box("flangeFillet", 0.150*25.4*mm,
                                    0.245*25.4*mm, 0.030*25.4*mm);
    G4Tubs* boltHole = new G4Tubs("boltHole", 0,0.1425*25.4*mm,
                                    0.030*25.4*mm, 0, 2*pi);                                                                
    G4UnionSolid* boltFlangeFillet = new G4UnionSolid("boltFlangeFillet", boltFlange, flangeFillet,
                                                      0, G4ThreeVector(-0.150*25.4*mm,0,0));
    G4SubtractionSolid* flangeFilletHole = new G4SubtractionSolid("flangeFilletHole", 
                                                                   boltFlangeFillet, boltHole);      
    G4UnionSolid* HVRing1flange = new G4UnionSolid("HVRing1flange", body, flangeFilletHole,
                                                   0, G4ThreeVector(postRadius, 0, -0.03*25.4*mm));

    G4RotationMatrix* rot120deg = new G4RotationMatrix();
    rot120deg->rotateZ(-120*deg);
    G4UnionSolid* HVRing2flanges = new G4UnionSolid("HVRing2flanges", HVRing1flange, 
                                                    flangeFilletHole, rot120deg, 
                    G4ThreeVector(postRadius*cos(120*deg), postRadius*sin(120*deg), -0.03*25.4*mm));

    G4RotationMatrix* rot240deg = new G4RotationMatrix();
    rot240deg->rotateZ(-240*deg);                            
    G4UnionSolid* HVRing = new G4UnionSolid("HV Ring", HVRing2flanges, flangeFilletHole, rot240deg, 
                    G4ThreeVector(postRadius*cos(240*deg), postRadius*sin(240*deg), -0.03*25.4*mm));                                

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(HVRing, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created 77mm HV Ring Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 77mm HV Ring Logical" << endlog; 
  return pVol;
}  
