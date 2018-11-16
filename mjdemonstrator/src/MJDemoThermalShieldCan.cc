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
// CLASS IMPLEMENTATION:  MJDemoThermalShieldCan.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  
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
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4Torus.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermalShieldCan.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermalShieldCan::MJDemoThermalShieldCan(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldCanDwg", "Copper-EF")
{;}

MJDemoThermalShieldCan::MJDemoThermalShieldCan(const MJDemoThermalShieldCan & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermalShieldCan::~MJDemoThermalShieldCan()
{;}

G4LogicalVolume* MJDemoThermalShieldCan::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* body = new G4Tubs("body", 6.1*25.4*mm, 6.14*25.4*mm, 5.47*25.4*mm, 0, 2*pi);
    G4DisplacedSolid* body2 = new G4DisplacedSolid("body2", body, 0, 
    										G4ThreeVector(0, 0, -5.47*25.4*mm));


    G4Sphere* bottom = new G4Sphere("bottom", 52.66*25.4*mm, 52.70*25.4*mm, 0, 2*pi, 0, 6.165*deg); 
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateX(pi);
    G4UnionSolid* body3 = new G4UnionSolid("body3", body2, bottom, rotation, 
    										G4ThreeVector(0, 0, 40.92*25.4*mm));

    G4Torus* fillet = new G4Torus("fillet", 0.5*25.4*mm, 0.54*25.4*mm, 5.6*25.4*mm, 0, 2*pi);
    G4Tubs* cylCut1 = new G4Tubs("cylCut1", 0, 5.6*25.4*mm, 2*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* fillet1 = new G4SubtractionSolid("fillet1", fillet, cylCut1);
    G4Tubs* cylCut2 = new G4Tubs("cylCut2", 0, 7*25.4*mm, 1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* fillet2 = new G4SubtractionSolid("fillet2", fillet1, cylCut2, 0,
    										G4ThreeVector(0, 0, 1*25.4*mm));

    G4UnionSolid* body4 = new G4UnionSolid("body4", body3, fillet2, 0, 
    										G4ThreeVector(0, 0, -10.94*25.4*mm));

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.09675*25.4*mm, 7*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateY(pi/2);
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, thruHole, rotation1, 
    										G4ThreeVector(0, 0, -0.188*25.4*mm));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateZ(2*pi/3);
    rotation2->rotateY(pi/2);
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, thruHole, rotation2, 
    										G4ThreeVector(0, 0, -0.188*25.4*mm));
    G4RotationMatrix* rotation3 = new G4RotationMatrix();
    rotation3->rotateZ(4*pi/3);
    rotation3->rotateY(pi/2);
    G4SubtractionSolid* can = new G4SubtractionSolid("can", body6, thruHole, rotation3, 
    										G4ThreeVector(0, 0, -0.188*25.4*mm));    

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(can, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Thermal Shield Can Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermal Shield Can Logical" << endlog; 
  return pVol;
}  
