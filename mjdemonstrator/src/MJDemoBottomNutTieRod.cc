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
// CLASS IMPLEMENTATION:  MJDemoBottomNutTieRod.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  coaxial with threaded hole, coincident with top mating surface.
 *
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
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoBottomNutTieRod.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoBottomNutTieRod::MJDemoBottomNutTieRod(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-102", "Copper-EF")
{;}

MJDemoBottomNutTieRod::MJDemoBottomNutTieRod(const MJDemoBottomNutTieRod & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoBottomNutTieRod::~MJDemoBottomNutTieRod()
{;}

G4LogicalVolume* MJDemoBottomNutTieRod::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* barrel = new G4Tubs("barrel", 0, 0.155*25.4*mm, 0.03*25.4*mm, 0, 2*pi);

    G4double zPlanes[] = {-0.220*25.4*mm, -0.010*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.125*25.4*mm, 0.125*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("Hex", 0, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    G4UnionSolid* body = new G4UnionSolid("body", hex, barrel, 0, G4ThreeVector(0,0,-0.03*25.4*mm));

    G4Tubs* thread = new G4Tubs("thread", 0, 0.0555*25.4*mm, 0.25*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* bottomNut = new G4SubtractionSolid("BottomNut", body, thread);											

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bottomNut, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Bottom Nut - Tie Rod Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Bottom Nut - Tie Rod Logical" << endlog; 
  return pVol;
}  
