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
// CLASS IMPLEMENTATION:  MJDemoCryostatTopLid.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with rim, coaxial with barrel
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Oct 1, 2010
 * 
 * REVISION:
 * 
 * 10-01-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Polycone.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatTopLid.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatTopLid::MJDemoCryostatTopLid(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "TopLidDwg", "Copper-EF")
{;}

MJDemoCryostatTopLid::MJDemoCryostatTopLid(const MJDemoCryostatTopLid & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCryostatTopLid::~MJDemoCryostatTopLid()
{;}

G4LogicalVolume* MJDemoCryostatTopLid::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0, 0.07*25.4*mm, 0.07*25.4*mm, 0.18*25.4*mm, 0.18*25.4*mm,
    						0.319*25.4*mm, 0.319*25.4*mm, 0.511*25.4*mm, 0.511*25.4*mm, 0.9263*25.4*mm};
                            //0.32, 0.51
    G4double rConeInner[] = {6.6582*25.4*mm, 6.6459*25.4*mm, 6.555*25.4*mm, 6.555*25.4*mm,
    							6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm,
    							6.5*25.4*mm, 6.5*25.4*mm};
    G4double rConeOuter[] = {6.759*25.4*mm, 6.759*25.4*mm, 6.759*25.4*mm, 6.759*25.4*mm, 6.759*25.4*mm, 
    						 6.759*25.4*mm, 6.695*25.4*mm, 6.695*25.4*mm, 6.76*25.4*mm, 6.76*25.4*mm};//6.76
    G4Polycone* barrel = new G4Polycone("Barrel", 0, 2*pi, 10, zConePlanes, rConeInner, rConeOuter);

    G4Sphere* top = new G4Sphere("top", 53.0125*25.4*mm, 53.2725*25.4*mm, 0, 2*pi, 0, 6.69*deg); 
    G4UnionSolid* body2 = new G4UnionSolid("body2", barrel, top, 0, 
    										G4ThreeVector(0, 0, -51.3625*25.4*mm));

    G4Torus* fillet = new G4Torus("fillet", 0.365*25.4*mm, 0.625*25.4*mm, 6.135*25.4*mm, 0, 2*pi);
    G4Tubs* cylCut1 = new G4Tubs("cylCut1", 0, 6.135*25.4*mm, 2*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* fillet1 = new G4SubtractionSolid("fillet1", fillet, cylCut1);
    G4Tubs* cylCut2 = new G4Tubs("cylCut2", 0, 7*25.4*mm, 1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* fillet2 = new G4SubtractionSolid("fillet2", fillet1, cylCut2, 0,
    										G4ThreeVector(0, 0, -1*25.4*mm));

    G4UnionSolid* toplid = new G4UnionSolid("toplid", body2, fillet2, 0, 
    										G4ThreeVector(0, 0, 0.9263*25.4*mm));

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(toplid, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cryostat Top Lid Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Top Lid Logical" << endlog; 
  return pVol;
}  
