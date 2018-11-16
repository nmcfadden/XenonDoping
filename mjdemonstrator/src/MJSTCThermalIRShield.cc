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
// CLASS IMPLEMENTATION:  MJSTCThermalIRShield.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 11-14-2013, Updated after adding it to an assembly w/the Cu foil. J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCThermalIRShield.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCThermalIRShield::MJSTCThermalIRShield(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCThermalIRShieldDwg", "Copper-OFHC") //partName, serialNumber, drawingNumber, material
{;}

MJSTCThermalIRShield::MJSTCThermalIRShield(const MJSTCThermalIRShield & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCThermalIRShield::~MJSTCThermalIRShield()
{;}

G4LogicalVolume* MJSTCThermalIRShield::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4double zPlanes[] = {0*mm, -15.875*mm, -15.875*mm, -288.925*mm, -288.925*mm, -295.275*mm};
  	G4double rInner[] = {58.42*mm, 58.42*mm, 58.42*mm, 58.42*mm, 5.032*mm, 5.032*mm}; 
	G4double rOuter[] =   {67.31*mm, 67.31*mm, 61.595*mm, 61.595*mm, 61.595*mm, 61.595*mm};
    G4Polycone* body = new G4Polycone("body", 0, 2*pi, 6, zPlanes, rInner, rOuter);

	G4Tubs* boltHolesA = new G4Tubs("boltHolesA", 0.0*mm, 2.5525*mm, 6.35*mm, 0, 2*pi);
	G4Cons* boltHolesB = new G4Cons("boltHolesB", 0.0*mm, 0.0*mm, 0.0*mm, 2.5525*mm, 0.767*mm, 0, 2*pi);
	G4UnionSolid* boltHoles = new G4UnionSolid("boltHoles", boltHolesA, boltHolesB, 
											0, G4ThreeVector(0, 0, -7.117*mm));	
	G4SubtractionSolid* boltHole01 = new G4SubtractionSolid("boltHole01", body, boltHoles,
    										0, G4ThreeVector(62.865*mm, 0, -6.35*mm));
	G4SubtractionSolid* boltHole02 = new G4SubtractionSolid("boltHole02", boltHole01, boltHoles,
    										0, G4ThreeVector(cos(30*deg)*62.865*mm, sin(30*deg)*62.865*mm, -6.35*mm));								
	G4SubtractionSolid* boltHole03 = new G4SubtractionSolid("boltHole03", boltHole02, boltHoles,
    										0, G4ThreeVector(cos(60*deg)*62.865*mm, sin(60*deg)*62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole04 = new G4SubtractionSolid("boltHole04", boltHole03, boltHoles,
    										0, G4ThreeVector(0, 62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole05 = new G4SubtractionSolid("boltHole05", boltHole04, boltHoles,
    										0, G4ThreeVector(cos(120*deg)*62.865*mm, sin(120*deg)*62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole06 = new G4SubtractionSolid("boltHole06", boltHole05, boltHoles,
    										0, G4ThreeVector(cos(150*deg)*62.865*mm, sin(150*deg)*62.865*mm, -6.35*mm));    										
 	G4SubtractionSolid* boltHole07 = new G4SubtractionSolid("boltHole07", boltHole06, boltHoles,
    										0, G4ThreeVector(-62.865*mm, 0, -6.35*mm));
	G4SubtractionSolid* boltHole08 = new G4SubtractionSolid("boltHole08", boltHole07, boltHoles,
    										0, G4ThreeVector(cos(210*deg)*62.865*mm, sin(210*deg)*62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole09 = new G4SubtractionSolid("boltHole09", boltHole08, boltHoles,
    										0, G4ThreeVector(cos(240*deg)*62.865*mm, sin(240*deg)*62.865*mm, -6.35*mm));
 	G4SubtractionSolid* boltHole10 = new G4SubtractionSolid("boltHole10", boltHole09, boltHoles,
    										0, G4ThreeVector(0, -62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole11 = new G4SubtractionSolid("boltHole11", boltHole10, boltHoles,
    										0, G4ThreeVector(cos(300*deg)*62.865*mm, sin(300*deg)*62.865*mm, -6.35*mm));
	G4SubtractionSolid* boltHole12 = new G4SubtractionSolid("boltHole12", boltHole11, boltHoles,
    										0, G4ThreeVector(cos(330*deg)*62.865*mm, sin(330*deg)*62.865*mm, -6.35*mm));
   
	G4Tubs* rectangleSlit = new G4Tubs("rectangleSlit", 58.42*mm, 61.595*mm, (127.00+6.35)*mm, 0, 12.48*pi/180);
	G4Tubs* circleSlit = new G4Tubs("circleSlit", 6.35*mm, 15*mm, 65*mm, 0, pi);
 	G4RotationMatrix* rotationCircleSlitA = new G4RotationMatrix();
    rotationCircleSlitA->rotateZ(pi/2-6.24*pi/180);
    rotationCircleSlitA->rotateX(pi/2);
 	G4SubtractionSolid* slitA = new G4SubtractionSolid("slitA", rectangleSlit, circleSlit,
 														rotationCircleSlitA, G4ThreeVector(0, 0, -127.00*mm));		
	G4RotationMatrix* rotationCircleSlitB = new G4RotationMatrix();
    rotationCircleSlitB->rotateZ(pi/2-6.24*pi/180);
    rotationCircleSlitB->rotateX(-pi/2);
 	G4SubtractionSolid* slit = new G4SubtractionSolid("slit", slitA, circleSlit,
 														rotationCircleSlitB, G4ThreeVector(0, 0, 127.00*mm));
 	G4RotationMatrix* rotationSlit01 = new G4RotationMatrix();
    rotationSlit01->rotateZ(6.24*pi/180);
    G4SubtractionSolid* slit01 = new G4SubtractionSolid("slit01", boltHole12, slit,
                                                       rotationSlit01, G4ThreeVector(0, 0, -152.4*mm));                                                   
 	G4RotationMatrix* rotationSlit02 = new G4RotationMatrix();
    rotationSlit02->rotateZ(2*pi/3+6.24*pi/180);
    G4SubtractionSolid* slit02 = new G4SubtractionSolid("slit02", slit01, slit,
                                                       rotationSlit02, G4ThreeVector(0, 0, -152.4*mm));                                  										
	G4RotationMatrix* rotationSlit03 = new G4RotationMatrix();
    rotationSlit03->rotateZ(4*pi/3+6.24*pi/180);
    G4SubtractionSolid* slit03 = new G4SubtractionSolid("slit03", slit02, slit,
                                                       rotationSlit03, G4ThreeVector(0, 0, -152.4*mm));
    
    G4Tubs* thruHoles = new G4Tubs("thruHoles", 0.0*mm, 1.3525*mm, 10*mm, 0, 2*pi);                                                   
    G4RotationMatrix* rotationThruHoles = new G4RotationMatrix();
    rotationThruHoles->rotateY(pi/2.);
    rotationThruHoles->rotateX(-5.*pi/12.);
 	G4SubtractionSolid* thruHole01 = new G4SubtractionSolid("thruHole01", slit03, thruHoles,
    										rotationThruHoles, G4ThreeVector(cos(5.*pi/12.)*62.865*mm, sin(5.*pi/12.)*62.865*mm, -9.525*mm));
 	rotationThruHoles->rotateX(-2.*pi/3.);
 	G4SubtractionSolid* thruHole02 = new G4SubtractionSolid("thruHole02", thruHole01, thruHoles,
    										rotationThruHoles, G4ThreeVector(cos(13.*pi/12.)*62.865*mm, sin(13.*pi/12.)*62.865*mm, -9.525*mm));
    rotationThruHoles->rotateX(-2.*pi/3.);
    G4SubtractionSolid* thruHole03 = new G4SubtractionSolid("thruHole03", thruHole02, thruHoles,
    										rotationThruHoles, G4ThreeVector(cos(21.*pi/12.)*62.865*mm, sin(21.*pi/12.)*62.865*mm, -9.525*mm));                                                               
                                                      
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); //copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(thruHole03, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created STC Thermal IR Shield Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Thermal IR Shield Logical" << endlog; 
  return pVol;
}  
