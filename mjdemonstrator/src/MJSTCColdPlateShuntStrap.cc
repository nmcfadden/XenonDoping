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
// CLASS IMPLEMENTATION:  MJSTCColdPlateShuntStrap.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from MJ83-01-012 rev0 
 * Part Origin: The (x,y,z) middle of the shunt strap
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION:
 * 10-17-2013, Created, J. MacMullin
 * 11-14-2013, Updated visualization properties to make compatible with Ray Tracer, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCColdPlateShuntStrap.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCColdPlateShuntStrap::MJSTCColdPlateShuntStrap(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCColdPlateShuntStrapDwg", "Copper-OFHC") 
{;}

MJSTCColdPlateShuntStrap::MJSTCColdPlateShuntStrap(const MJSTCColdPlateShuntStrap & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCColdPlateShuntStrap::~MJSTCColdPlateShuntStrap()
{;}

G4LogicalVolume* MJSTCColdPlateShuntStrap::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

	G4Box* body = new G4Box("body", 9.525*mm, 3.175*mm, 1.5875*mm);
	
	G4Tubs* thruHoles = new G4Tubs("thruHoles", 0.0*mm, 1.8288*mm, 1.7*mm, 0, 2*pi);
	G4SubtractionSolid* thruHole01 = new G4SubtractionSolid("thruHole01", body, thruHoles,
    										0, G4ThreeVector(-(9.525-3.302)*mm, (3.302-3.175)*mm, 0));
	G4SubtractionSolid* thruHole02 = new G4SubtractionSolid("thruHole02", thruHole01, thruHoles,
    										0, G4ThreeVector((-(9.525-3.302)+12.7)*mm, (3.302-3.175)*mm, 0));
    
    G4Tubs* roundedEdges = new G4Tubs("roundedEdge", 0.8128*mm, 2.0*mm, 1.7*mm, 0, pi/2.0);
	G4SubtractionSolid* roundedEdge01 = new G4SubtractionSolid("roundedEdge01", thruHole02, roundedEdges,
    										0, G4ThreeVector((9.525-0.8128)*mm, (3.175-0.8128)*mm, 0));
    G4RotationMatrix* rotationEdge02 = new G4RotationMatrix();
    rotationEdge02->rotateZ(3.0*pi/2.0);
    G4SubtractionSolid* roundedEdge02 = new G4SubtractionSolid("roundedEdge02", roundedEdge01, roundedEdges,
                                            rotationEdge02, G4ThreeVector(-(9.525-0.8128)*mm, (3.175-0.8128)*mm, 0));                                  										
    G4RotationMatrix* rotationEdge03 = new G4RotationMatrix();
    rotationEdge03->rotateZ(pi);
    G4SubtractionSolid* roundedEdge03 = new G4SubtractionSolid("roundedEdge03", roundedEdge02, roundedEdges,
                                            rotationEdge03, G4ThreeVector(-(9.525-0.8128)*mm, -(3.175-0.8128)*mm, 0));                                  										
    G4RotationMatrix* rotationEdge04 = new G4RotationMatrix();
    rotationEdge04->rotateZ(pi/2.0);
    G4SubtractionSolid* roundedEdge04 = new G4SubtractionSolid("roundedEdge04", roundedEdge03, roundedEdges,
                                            rotationEdge04, G4ThreeVector((9.525-0.8128)*mm, -(3.175-0.8128)*mm, 0));                                  										

    //G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)); // cyan
	G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); //copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(roundedEdge04, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created STC Cold Plate Shunt Strap Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Cold Plate Shunt Strap Logical" << endlog; 
  return pVol;
}  
