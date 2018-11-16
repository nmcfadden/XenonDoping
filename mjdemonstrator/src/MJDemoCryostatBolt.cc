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
// CLASS IMPLEMENTATION:  MJDemoCryostatBolt.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with mating face, coaxial with bolt
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
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatBolt.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatBolt::MJDemoCryostatBolt(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatBoltDwg", "Copper-EF")
{;}

MJDemoCryostatBolt::MJDemoCryostatBolt(const MJDemoCryostatBolt & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCryostatBolt::~MJDemoCryostatBolt()
{;}

G4LogicalVolume* MJDemoCryostatBolt::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zPlanes[] = {-0.12*25.4*mm, -0.001*25.4*mm, -0.001*25.4*mm, 0.77*25.4*mm};//0.0
    G4double rInner[] = {0, 0, 0, 0};
    G4double rOuter[] = {0.156*25.4*mm, 0.156*25.4*mm, 0.125*25.4*mm, 0.125*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 4, zPlanes, rInner, rOuter); 
    
    G4Tubs* threads = new G4Tubs("threads", 0, 0.1131*25.4*mm, 0.75*25.4*mm, 0, 2*pi);//.1141
    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, threads, 0, G4ThreeVector(0,0,0.77*25.4*mm));
    
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cryostat Bolt Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Bolt Logical" << endlog; 
  return pVol;
}  
