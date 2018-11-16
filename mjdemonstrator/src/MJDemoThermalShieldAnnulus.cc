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
// CLASS IMPLEMENTATION:  MJDemoThermalShieldAnnulus.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 * 
 * REVISION:
 * 
 * 09-10-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermalShieldAnnulus.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermalShieldAnnulus::MJDemoThermalShieldAnnulus(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldAnnulusDWG", "Copper-EF")
{;}

MJDemoThermalShieldAnnulus::MJDemoThermalShieldAnnulus(const MJDemoThermalShieldAnnulus & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermalShieldAnnulus::~MJDemoThermalShieldAnnulus()
{;}

G4LogicalVolume* MJDemoThermalShieldAnnulus::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* body = new G4Tubs("body", 5.825*25.4*mm, 6.099*25.4*mm, 0.1875*25.4*mm, 0, 2*pi);//6.1

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.0725*25.4*mm, 7*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateZ((-45.83)*deg);
    rotation1->rotateY(pi/2);
    G4SubtractionSolid* body1 = new G4SubtractionSolid("body1", body, thruHole, rotation1, 
    										G4ThreeVector(0, 0, 0));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateZ((-45.83-60)*deg);
    rotation2->rotateY(pi/2);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body1, thruHole, rotation2, 
    										G4ThreeVector(0, 0, 0));
    G4RotationMatrix* rotation3 = new G4RotationMatrix();
    rotation3->rotateZ((-45.83-120)*deg);
    rotation3->rotateY(pi/2);
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, thruHole, rotation3, 
    										G4ThreeVector(0, 0, 0));
    										
    G4double zPlanes[] = {-0.125*25.4*mm, 0.125*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.238*25.4*mm, 0.238*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);    
    G4RotationMatrix* rotation4 = new G4RotationMatrix();

    G4double screwRad = 11.72/2*25.4*mm;
    G4double screwAng = 40.0*deg;
    G4double screwOffset = 40*deg;

    rotation4->rotateZ(-screwOffset);
    G4UnionSolid* body4a = new G4UnionSolid("body4a", body3, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+0*screwAng), 
    					screwRad*sin(screwOffset+0*screwAng), 0));
    G4SubtractionSolid* body4b = new G4SubtractionSolid("body4b", body4a, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+0*screwAng), 
    					screwRad*sin(screwOffset+0*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4c = new G4UnionSolid("body4c", body4b, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+1*screwAng), 
    					screwRad*sin(screwOffset+1*screwAng), 0));
    G4SubtractionSolid* body4d = new G4SubtractionSolid("body4d", body4c, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+1*screwAng), 
    					screwRad*sin(screwOffset+1*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4e = new G4UnionSolid("body4e", body4d, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+2*screwAng), 
    					screwRad*sin(screwOffset+2*screwAng), 0));
    G4SubtractionSolid* body4f = new G4SubtractionSolid("body4f", body4e, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+2*screwAng), 
    					screwRad*sin(screwOffset+2*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4g = new G4UnionSolid("body4g", body4f, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+3*screwAng), 
    					screwRad*sin(screwOffset+3*screwAng), 0));
    G4SubtractionSolid* body4h = new G4SubtractionSolid("body4h", body4g, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+3*screwAng), 
    					screwRad*sin(screwOffset+3*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4i = new G4UnionSolid("body4i", body4h, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+4*screwAng), 
    					screwRad*sin(screwOffset+4*screwAng), 0));
    G4SubtractionSolid* body4j = new G4SubtractionSolid("body4j", body4i, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+4*screwAng), 
    					screwRad*sin(screwOffset+4*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4k = new G4UnionSolid("body4k", body4j, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+5*screwAng), 
    					screwRad*sin(screwOffset+5*screwAng), 0));
    G4SubtractionSolid* body4l = new G4SubtractionSolid("body4l", body4k, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+5*screwAng), 
    					screwRad*sin(screwOffset+5*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4m = new G4UnionSolid("body4m", body4l, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+6*screwAng), 
    					screwRad*sin(screwOffset+6*screwAng), 0));
    G4SubtractionSolid* body4n = new G4SubtractionSolid("body4n", body4m, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+6*screwAng), 
    					screwRad*sin(screwOffset+6*screwAng), 0));

    rotation4->rotateZ(-screwAng);
    G4UnionSolid* body4o = new G4UnionSolid("body4o", body4n, hex, rotation4, 
    					G4ThreeVector(screwRad*cos(screwOffset+7*screwAng), 
    					screwRad*sin(screwOffset+7*screwAng), 0));
    G4SubtractionSolid* annulus = new G4SubtractionSolid("annulus", body4o, thruHole, 0,
    					G4ThreeVector(screwRad*cos(screwOffset+7*screwAng), 
    					screwRad*sin(screwOffset+7*screwAng), 0));

/*   
    G4Box* tab = new G4Box("tab", 0.1*25.4*mm, 0.192*25.4*mm, 0.125*25.4*mm);
    G4UnionSolid* annulus = new G4UnionSolid("annulus", body4, tab, 0,
    										G4ThreeVector(5.7306*25.4*mm, 0, 0));
*/

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(annulus, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Thermal Shield Annulus Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermal Shield Annulus Logical" << endlog; 
  return pVol;
}  
