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
// CLASS IMPLEMENTATION:  MJDemoSplitNutTieRod.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  coaxial with threaded hole, coincident with top surface.
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
 * 01-14-2012, Changed color attribute from gray to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSplitNutTieRod.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSplitNutTieRod::MJDemoSplitNutTieRod(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-101", "Copper-EF")
{;}

MJDemoSplitNutTieRod::MJDemoSplitNutTieRod(const MJDemoSplitNutTieRod & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSplitNutTieRod::~MJDemoSplitNutTieRod()
{;}

G4LogicalVolume* MJDemoSplitNutTieRod::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4double zPlanes[] = {0*25.4*mm, -0.040*25.4*mm, -0.040*25.4*mm, -0.070*25.4*mm,
    						-0.070*25.4*mm, -0.160*25.4*mm};
    G4double rInner[] = {0.0555*25.4*mm, 0.0555*25.4*mm, 0.0555*25.4*mm, 0.0555*25.4*mm, 
    						0.0555*25.4*mm, 0.0555*25.4*mm};
    G4double rOuter[] = {0.125*25.4*mm, 0.125*25.4*mm, 0.095*25.4*mm, 0.095*25.4*mm,
    						0.125*25.4*mm, 0.1205*25.4*mm};
    G4Polycone* barrel = new G4Polycone("barrel", 0, 2*pi, 6, zPlanes, rInner, rOuter);

    G4Box* split = new G4Box("split", 0.010*25.4*mm, 0.150*25.4*mm, 0.2*25.4*mm);
    
    G4SubtractionSolid* splitNut = new G4SubtractionSolid("Split Nut", barrel, split);

    // G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.7,0.7,0.7)); // gray
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // grayVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(splitNut, material, logicalName);
    // pVol->SetVisAttributes(grayVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Split Nut Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Split Nut Logical" << endlog; 
  return pVol;
}  
