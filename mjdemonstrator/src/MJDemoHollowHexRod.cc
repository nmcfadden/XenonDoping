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
// CLASS IMPLEMENTATION:  MJDemoHollowHexRod.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin: Coaxial with bolt, coincident with top hex surface.
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
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHollowHexRod.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHollowHexRod::MJDemoHollowHexRod(G4String partName, G4String serialNumber/*, G4double addLength*/) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-006", "Copper-EF"), fAddLength(0.0)
{;}

MJDemoHollowHexRod::MJDemoHollowHexRod(const MJDemoHollowHexRod & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHollowHexRod::~MJDemoHollowHexRod()
{;}

G4LogicalVolume* MJDemoHollowHexRod::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zPlanes[] = {0*25.4*mm, -0.08*25.4*mm, -0.08*25.4*mm, -0.35*25.4*mm, -0.35*25.4*mm,
                            -1.156*25.4*mm};
    G4double rInner[] = {0, 0, 0, 0, 0, 0};
    G4double rOuter[] = {0.156*25.4*mm, 0.156*25.4*mm, 0.125*25.4*mm, 0.125*25.4*mm,
                           0.125*25.4*mm, 0.125*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 6, zPlanes, rInner, rOuter);    


    G4double zPlanes1[] = {-1.15*25.4*mm, -2.036*25.4*mm -fAddLength, -2.056*25.4*mm - fAddLength, -2.116*25.4*mm -fAddLength};
    G4double rInner1[] = {0, 0, 0, 0};
    G4double rOuter1[] = {0.114*25.4*mm, 0.114*25.4*mm, 0.105*25.4*mm, 0.105*25.4*mm};
    G4Polycone* thread = new G4Polycone("thread", 0, 2*pi, 3, zPlanes1, rInner1, rOuter1);  
    G4UnionSolid* hexAndThread = new G4UnionSolid("hexAndThread", hex, thread);
    
                                                   
    G4double zPlanes2[] = {0.1*25.4*mm, -2.5*25.4*mm - fAddLength};
    G4double rInner2[] = {0, 0};
    G4double rOuter2[] = {0.0725*25.4*mm, 0.0725*25.4*mm};
    G4Polyhedra* hexCut = new G4Polyhedra("hexCut", pi/2, 2*pi, 6, 2, zPlanes2, rInner2, rOuter2);    
    G4SubtractionSolid* RodWithHexCut = new G4SubtractionSolid("RodWithHexCut", hexAndThread,
                                                                hexCut);
                                                                
    G4double zPlanes3[] = {0.0001*25.4*mm, -0.021*25.4*mm, -0.06*25.4*mm};
    G4double rInner3[] = {0, 0, 0};
    G4double rOuter3[] = {0.129*25.4*mm, 0.109*25.4*mm, 0.109*25.4*mm};
    G4Polycone* cBore = new G4Polycone("cBore", 0, 2*pi, 3, zPlanes3, rInner3, rOuter3);  
    G4SubtractionSolid* hollowHexRod = new G4SubtractionSolid("Hollow Hex Rod", RodWithHexCut, cBore);                                                                


    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(hollowHexRod, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Hollow Hex Rod Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Hollow Hex Rod Logical" << endlog; 
  return pVol;
}  
