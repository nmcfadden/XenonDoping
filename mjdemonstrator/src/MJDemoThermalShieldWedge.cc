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
// CLASS IMPLEMENTATION:  MJDemoThermalShieldWedge.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Center of PEEK support hole bottom
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
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermalShieldWedge.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermalShieldWedge::MJDemoThermalShieldWedge(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldWedgeDWG", "Copper-EF")
{;}

MJDemoThermalShieldWedge::MJDemoThermalShieldWedge(const MJDemoThermalShieldWedge & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermalShieldWedge::~MJDemoThermalShieldWedge()
{;}

G4LogicalVolume* MJDemoThermalShieldWedge::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){    
    G4Box* body1 = new G4Box("body1", 0.4*25.4*mm, 0.52*25.4*mm, 0.125*25.4*mm);
    
    G4Tubs* peekHole = new G4Tubs("peek Hole", 0, 0.065*25.4*mm, 0.125*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body1, peekHole, 0, 
    											G4ThreeVector(0, 0, 0.125*25.4*mm));
    
    G4Tubs* insideArc = new G4Tubs("inside Arc", 0.9763*25.4*mm, 2*25.4*mm, 1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, insideArc, 0, 
    											G4ThreeVector(-0.8341*25.4*mm, 0, 0));
 
    G4Tubs* outsideArc = new G4Tubs("outside Arc", 0.75*25.4*mm, 2*25.4*mm, 1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, outsideArc, 0, 
    											G4ThreeVector(0.3827*25.4*mm, 0, 0));

    G4Tubs* underCut = new G4Tubs("underCut", 6.5*25.4*mm, 7*25.4*mm, 1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* wedge = new G4SubtractionSolid("wedge", body4, underCut, 0, 
    											G4ThreeVector(6.322*25.4*mm, 0, -0.999*25.4*mm));
    											
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0.0, 0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(wedge, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Thermal Shield Wedge Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermal Shield Wedge Logical" << endlog; 
  return pVol;
}  
