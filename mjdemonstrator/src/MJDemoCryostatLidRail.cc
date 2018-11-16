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
// CLASS IMPLEMENTATION:  MJDemoCryostatLidRail.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with top (wide) rim, coaxial with ring
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
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatLidRail.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatLidRail::MJDemoCryostatLidRail(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatLidRailDwg", "Copper-EF")
{;}

MJDemoCryostatLidRail::MJDemoCryostatLidRail(const MJDemoCryostatLidRail & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCryostatLidRail::~MJDemoCryostatLidRail()
{;}

G4LogicalVolume* MJDemoCryostatLidRail::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {-0.0005*25.4*mm, -0.187*25.4*mm, -0.187*25.4*mm, -0.375*25.4*mm};
    G4double rConeInner[] = {6.7*25.4*mm, 6.7*25.4*mm, 6.76*25.4*mm, 6.76*25.4*mm};//6.698
    G4double rConeOuter[] = {7.385*25.4*mm, 7.385*25.4*mm, 7.385*25.4*mm, 7.385*25.4*mm};
    G4Polycone* barrel = new G4Polycone("Barrel", -2.5*deg, 95*deg, 4, zConePlanes, rConeInner, rConeOuter);

    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 3.0*25.4*mm, 7.5*25.4*mm, 0.2*25.4*mm, -7.51*deg, 10.02*deg);//-7.5, 10

    G4SubtractionSolid* barrel2 = new G4SubtractionSolid("barrel2", barrel, cylinderCut, 0,
    										G4ThreeVector(0, 0, -0.3865*25.4*mm)); //-0.3875
    G4RotationMatrix* cylRotation = new G4RotationMatrix();
    cylRotation->rotateZ(-95*deg);
    G4SubtractionSolid* barrel3 = new G4SubtractionSolid("barrel3", barrel2, cylinderCut, cylRotation,
    										G4ThreeVector(0, 0, 0.0115*25.4*mm)); //0.0125

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.098*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* barrel4 = new G4SubtractionSolid("barrel4", barrel3, thruHole, 0,
    										G4ThreeVector(7.094*25.4*mm, 0, 0));
    G4SubtractionSolid* barrel5 = new G4SubtractionSolid("barrel5", barrel4, thruHole, 0,
    										G4ThreeVector(0, 7.094*25.4*mm, 0));		

    G4Tubs* thread = new G4Tubs("thread", 0, 0.1425*25.4*mm, 0.50*25.4*mm ,0, 2*pi);  

    G4double angle = 360.0/12.0;
    G4double holeRad = 7.1266*25.4*mm;
	G4SubtractionSolid* bodya = new G4SubtractionSolid("bodya", barrel5, thread, 0,
					G4ThreeVector(holeRad*cos(0.5*angle*deg), holeRad*sin(0.5*angle*deg), 0));
    G4SubtractionSolid* bodyb = new G4SubtractionSolid("bodyb", bodya, thread, 0,
					G4ThreeVector(holeRad*cos(1.5*angle*deg), holeRad*sin(1.5*angle*deg), 0));
    G4SubtractionSolid* lidrail = new G4SubtractionSolid("lidrail", bodyb, thread, 0,
					G4ThreeVector(holeRad*cos(2.5*angle*deg), holeRad*sin(2.5*angle*deg), 0));


    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(lidrail, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cryostat Lid Rail Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Lid Rail Logical" << endlog; 
  return pVol;
}  
