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
// CLASS IMPLEMENTATION:  MJDemo1032Nut.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Geometry made from MJ80-02-071 rev1 
 * Part origin: Center of the bottom surface of the nut
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 
 * 11-14-2013, Created, J. MacMullin
 * 03-xx-2014, Fixed typo, J. MacMullin
 * 
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemo1032Nut.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo1032Nut::MJDemo1032Nut(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJDemo1032NutDwg", "Copper-EF") 
{;}

MJDemo1032Nut::MJDemo1032Nut(const MJDemo1032Nut & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemo1032Nut::~MJDemo1032Nut()
{;}

G4LogicalVolume* MJDemo1032Nut::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){  
    G4double eps = 0.001 * mm;
    
    G4double zPlanes[] = {0*mm,  2.286*mm};
    G4double rInner[] = {0*mm, 0*mm};
    G4double rOuter[] = {3.9624*mm, 3.9624*mm};
    G4Polyhedra* body = new G4Polyhedra("body", 0, 2*pi, 6, 2, zPlanes, rInner, rOuter);    
    
    G4Tubs* hole = new G4Tubs("hole", 0*mm, 2.15519*mm+eps, 3.0*mm, 0, 2*pi); //use effective diameter
    
    G4SubtractionSolid* nut = new G4SubtractionSolid("nut", body, hole,
    										0, G4ThreeVector(0, 0, 0));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(nut, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created 10-32 Nut Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 10-32 Nut Logical" << endlog; 
  return pVol;
}
