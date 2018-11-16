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
// CLASS IMPLEMENTATION:  MJDemoCryostatCenterRail.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with bottom (wide) rim, coaxial with ring
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
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatCenterRail.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatCenterRail::MJDemoCryostatCenterRail(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatCenterRailDwg", "Copper-EF")
{;}

MJDemoCryostatCenterRail::MJDemoCryostatCenterRail(const MJDemoCryostatCenterRail & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCryostatCenterRail::~MJDemoCryostatCenterRail()
{;}

G4LogicalVolume* MJDemoCryostatCenterRail::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0, 0.1875*25.4*mm, 0.1875*25.4*mm, 0.375*25.4*mm};
    G4double rConeInner[] = {6.815*25.4*mm, 6.815*25.4*mm, 6.875*25.4*mm, 6.875*25.4*mm};//6.813
    G4double rConeOuter[] = {7.375*25.4*mm, 7.375*25.4*mm, 7.375*25.4*mm, 7.375*25.4*mm};
    G4Polycone* barrel = new G4Polycone("Barrel", -2.5*deg, 95*deg, 4, zConePlanes, rConeInner, rConeOuter);

    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 3.0*25.4*mm, 7.5*25.4*mm, 0.2*25.4*mm, -7.51*deg, 10.02*deg);//-7.5, 10

    G4SubtractionSolid* barrel2 = new G4SubtractionSolid("barrel2", barrel, cylinderCut, 0,
    										G4ThreeVector(0, 0, -0.0115*25.4*mm)); //-0.0125
    G4RotationMatrix* cylRotation = new G4RotationMatrix();
    cylRotation->rotateZ(-95*deg);
    G4SubtractionSolid* barrel3 = new G4SubtractionSolid("barrel3", barrel2, cylinderCut, cylRotation,
    										G4ThreeVector(0, 0, 0.3865*25.4*mm));//0.3875

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.098*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* barrel4 = new G4SubtractionSolid("barrel4", barrel3, thruHole, 0,
    										G4ThreeVector(7.094*25.4*mm, 0, 0));
    G4SubtractionSolid* barrel5 = new G4SubtractionSolid("barrel5", barrel4, thruHole, 0,
    										G4ThreeVector(0, 7.094*25.4*mm, 0));										
    
    G4double zHexPlanes[] = {-0.1*25.4*mm, 0.5*25.4*mm};
    G4double rHexInner[] = {0,0};
    G4double rHexOuter[] = {0.13*25.4*mm, 0.13*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("Hex", pi/2, 2*pi, 6, 2, zHexPlanes, rHexInner, rHexOuter);  

    G4double angle = 360.0/12.0;
    G4double hexHoleRad = 7.1266*25.4*mm;
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(-0.5*angle*deg);
	G4SubtractionSolid* bodya = new G4SubtractionSolid("bodya", barrel5, hex, rotation,
					G4ThreeVector(hexHoleRad*cos(0.5*angle*deg), hexHoleRad*sin(0.5*angle*deg), 0));
    rotation->rotateZ(-angle*deg);
    G4SubtractionSolid* bodyb = new G4SubtractionSolid("bodyb", bodya, hex, rotation,
					G4ThreeVector(hexHoleRad*cos(1.5*angle*deg), hexHoleRad*sin(1.5*angle*deg), 0));
    rotation->rotateZ(-angle*deg);
    G4SubtractionSolid* rail = new G4SubtractionSolid("rail", bodyb, hex, rotation,
					G4ThreeVector(hexHoleRad*cos(2.5*angle*deg), hexHoleRad*sin(2.5*angle*deg), 0));

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(rail, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cryostat Center Rail Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Center Rail Logical" << endlog; 
  return pVol;
}  
