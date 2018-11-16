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
// CLASS IMPLEMENTATION:  MJSTCColdPlateBody.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from MJ83-01-010 rev5 with center hole excluded
 * Made tapped holes thru holes
 * This geometry is not complete; there are some measurements that need to be updated.
 * The comments below indicate which parts of the geometry need updating.
 * Part Origin: The middle of the width (z) of the Coldplate Body
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
 * 10-17-2013, Renamed from MJSTCThermalColdPlate.cc to MJSTCColdPlateBody.cc, J. MacMullin
 * 11-14-2013, Some previously-unknown dimensions were updated, J. MacMullin
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
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCColdPlateBody.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCColdPlateBody::MJSTCColdPlateBody(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCColdPlateBodyDwg", "Copper-OFHC") 
{;}

MJSTCColdPlateBody::MJSTCColdPlateBody(const MJSTCColdPlateBody & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCColdPlateBody::~MJSTCColdPlateBody()
{;}

G4LogicalVolume* MJSTCColdPlateBody::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4Tubs* body = new G4Tubs("body", 0.0*mm, 67.5*mm, 3.175*mm, 0, 2*pi);
  	
  	// The 2.5525mm radius might not be right - it was not labeled on the drawing I had.
  	// The 2.5525mm is a guess based on the holes in the IR shield.
  	G4Tubs* boltHoles = new G4Tubs("boltHoles", 0.0*mm, 2.5525*mm, 3.2*mm, 0, 2*pi);
  	G4SubtractionSolid* boltHole01 = new G4SubtractionSolid("boltHole01", body, boltHoles,
    										0, G4ThreeVector(62.865*mm, 0, 0));
	G4SubtractionSolid* boltHole02 = new G4SubtractionSolid("boltHole02", boltHole01, boltHoles,
    										0, G4ThreeVector(cos(30*deg)*62.865*mm, sin(30*deg)*62.865*mm, 0));								
	G4SubtractionSolid* boltHole03 = new G4SubtractionSolid("boltHole03", boltHole02, boltHoles,
    										0, G4ThreeVector(cos(60*deg)*62.865*mm, sin(60*deg)*62.865*mm, 0));
	G4SubtractionSolid* boltHole04 = new G4SubtractionSolid("boltHole04", boltHole03, boltHoles,
    										0, G4ThreeVector(0, 62.865*mm, 0));
	G4SubtractionSolid* boltHole05 = new G4SubtractionSolid("boltHole05", boltHole04, boltHoles,
    										0, G4ThreeVector(cos(120*deg)*62.865*mm, sin(120*deg)*62.865*mm, 0));
	G4SubtractionSolid* boltHole06 = new G4SubtractionSolid("boltHole06", boltHole05, boltHoles,
    										0, G4ThreeVector(cos(150*deg)*62.865*mm, sin(150*deg)*62.865*mm, 0));    										
 	G4SubtractionSolid* boltHole07 = new G4SubtractionSolid("boltHole07", boltHole06, boltHoles,
    										0, G4ThreeVector(-62.865*mm, 0, 0));
	G4SubtractionSolid* boltHole08 = new G4SubtractionSolid("boltHole08", boltHole07, boltHoles,
    										0, G4ThreeVector(cos(210*deg)*62.865*mm, sin(210*deg)*62.865*mm, 0));
	G4SubtractionSolid* boltHole09 = new G4SubtractionSolid("boltHole09", boltHole08, boltHoles,
    										0, G4ThreeVector(cos(240*deg)*62.865*mm, sin(240*deg)*62.865*mm, 0));
 	G4SubtractionSolid* boltHole10 = new G4SubtractionSolid("boltHole10", boltHole09, boltHoles,
    										0, G4ThreeVector(0, -62.865*mm, 0));
	G4SubtractionSolid* boltHole11 = new G4SubtractionSolid("boltHole11", boltHole10, boltHoles,
    										0, G4ThreeVector(cos(300*deg)*62.865*mm, sin(300*deg)*62.865*mm, 0));
	G4SubtractionSolid* boltHole12 = new G4SubtractionSolid("boltHole12", boltHole11, boltHoles,
    										0, G4ThreeVector(cos(330*deg)*62.865*mm, sin(330*deg)*62.865*mm, 0));
    
  	G4double zPlanes[] = {0*mm, -3.6*mm, -3.6*mm, -6.5*mm};
  	G4double rInner[] = {0*mm, 0*mm, 0*mm, 0*mm}; 
  	// The 2.5525mm radius is not right - it was not labeled on the drawing I had and I need to talk to Matthew to check it
	G4double rOuter[] =   {2.5525*mm, 2.5525*mm, 4.953*mm, 4.953*mm};
    G4Polycone* SAPholes = new G4Polycone("SAPholes", 0, 2*pi, 4, zPlanes, rInner, rOuter);
 	// The 19.05mm radius might not be right - it was not labeled on the drawing I had.
  	// The 19.05mm is a guess based on the holes in the string adapter plate.
 	G4SubtractionSolid* SAPhole01 = new G4SubtractionSolid("SAPhole01", boltHole12, SAPholes,
    										0, G4ThreeVector(cos(90*deg)*19.05*mm, sin(90*deg)*19.05*mm, (3.175+(3.6-3.556))*mm));
 	G4SubtractionSolid* SAPhole02 = new G4SubtractionSolid("SAPhole02", SAPhole01, SAPholes,
    										0, G4ThreeVector(cos(330*deg)*19.05*mm, sin(330*deg)*19.05*mm, (3.175+(3.6-3.556))*mm));
  	G4SubtractionSolid* SAPhole03 = new G4SubtractionSolid("SAPhole03", SAPhole02, SAPholes,
    										0, G4ThreeVector(cos(210*deg)*19.05*mm, sin(210*deg)*19.05*mm, (3.175+(3.6-3.556))*mm));   										
    
    //Did not include that the holes are tapped (6-32 UNC)										                                                       
    G4Tubs* ShuntStrapHoles = new G4Tubs("ShuntStrapHoles", 0.0*mm, 1.359*mm, 2.2*mm, 0, 2*pi);
 	G4SubtractionSolid* ShuntStrapHole01 = new G4SubtractionSolid("ShuntStrapHole01", SAPhole03, ShuntStrapHoles,
    										0, G4ThreeVector(33.7312*mm, -9.0424*mm, ((3.175+2.2)-4.32)*mm));
    G4SubtractionSolid* ShuntStrapHole02 = new G4SubtractionSolid("ShuntStrapHole02", ShuntStrapHole01, ShuntStrapHoles,
    										0, G4ThreeVector(45.9994*mm, -12.319*mm, ((3.175+2.2)-4.32)*mm));
    G4SubtractionSolid* ShuntStrapHole03 = new G4SubtractionSolid("ShuntStrapHole03", ShuntStrapHole02, ShuntStrapHoles,
    										0, G4ThreeVector(16.61*mm, -41.402*mm, ((3.175+2.2)-4.32)*mm)); 
    G4SubtractionSolid* ShuntStrapHole04 = new G4SubtractionSolid("ShuntStrapHole04", ShuntStrapHole03, ShuntStrapHoles,
    										0, G4ThreeVector(-22.28*mm, -37.287*mm, ((3.175+2.2)-4.32)*mm)); 
    G4SubtractionSolid* ShuntStrapHole05 = new G4SubtractionSolid("ShuntStrapHole05", ShuntStrapHole04, ShuntStrapHoles,
    										0, G4ThreeVector(-17.8*mm, 32.512*mm, ((3.175+2.2)-4.32)*mm)); 
    G4SubtractionSolid* ShuntStrapHole06 = new G4SubtractionSolid("ShuntStrapHole06", ShuntStrapHole05, ShuntStrapHoles,
    										0, G4ThreeVector(9.042*mm, 33.731*mm, ((3.175+2.2)-4.32)*mm)); 
    G4SubtractionSolid* ShuntStrapHole07 = new G4SubtractionSolid("ShuntStrapHole07", ShuntStrapHole06, ShuntStrapHoles,
    										0, G4ThreeVector(12.32*mm, 45.999*mm, ((3.175+2.2)-4.32)*mm)); 
 
 	//Did not include that the holes are tapped (10-32 UNF)
    G4Tubs* MountingHoles = new G4Tubs("MountingHoles", 0.0*mm, 2.019*mm, 3.2*mm, 0, 2*pi);
    G4SubtractionSolid* MountingHole01 = new G4SubtractionSolid("MountingHole01", ShuntStrapHole07, MountingHoles,
    										0, G4ThreeVector(38.506*mm, 22.22*mm, 0));
    G4SubtractionSolid* MountingHole02 = new G4SubtractionSolid("MountingHole02", MountingHole01, MountingHoles,
    										0, G4ThreeVector(36.398*mm, -37.643*mm, 0));
    G4SubtractionSolid* MountingHole03 = new G4SubtractionSolid("MountingHole03", MountingHole02, MountingHoles,
    										0, G4ThreeVector(-14.4*mm, 50.343*mm, 0));

	//The 3.0mm y/2 width is not right - it was not labeled on the drawing I had and I need to talk to Matthew to check it 
	G4Box* RectangleCableHole = new G4Box("RectangleCableHole", (12.7+3.0+1.0)*mm, 3.0*mm, 3.5*mm);
 	G4Tubs* CircleCableHole = new G4Tubs("CircleCableHole", 3.0*mm, 10.0*mm, 3.6*mm, 0, pi);
  	G4RotationMatrix* rotationCircleCableHoleA = new G4RotationMatrix();
    rotationCircleCableHoleA->rotateZ(pi/2);
 	G4SubtractionSolid* CableHoleA = new G4SubtractionSolid("CableHoleA", RectangleCableHole, CircleCableHole, 
 											rotationCircleCableHoleA, G4ThreeVector(12.7*mm, 0, 0));	
  	G4RotationMatrix* rotationCircleCableHoleB = new G4RotationMatrix();
    rotationCircleCableHoleB->rotateZ(3*pi/2);
	G4SubtractionSolid* CableHoleB = new G4SubtractionSolid("CableHoleB", CableHoleA, CircleCableHole, 
											rotationCircleCableHoleB, G4ThreeVector(-12.7*mm, 0, 0));	
	G4RotationMatrix* rotationCableHole = new G4RotationMatrix();
 	rotationCableHole->rotateZ(pi/3);
	G4SubtractionSolid* CableHole = new G4SubtractionSolid("CableHole", MountingHole03, CableHoleB,
 											rotationCableHole, G4ThreeVector(-41.25*mm, -23.8*mm, 0)); 
    
	G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); //copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(CableHole, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created STC Cold Plate Body Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Cold Plate Body Logical" << endlog; 
  return pVol;
}  
