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
// CLASS IMPLEMENTATION:  MJDemoCenterBushing.cc
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
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCenterBushing.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCenterBushing::MJDemoCenterBushing(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-005", "Teflon")
{;}

MJDemoCenterBushing::MJDemoCenterBushing(const MJDemoCenterBushing & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCenterBushing::~MJDemoCenterBushing()
{;}

G4LogicalVolume* MJDemoCenterBushing::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4double zPlanes[] = {0*25.4*mm, -0.05*25.4*mm, -0.05*25.4*mm, -0.085*25.4*mm,
    						-0.085*25.4*mm, -0.105*25.4*mm, -0.112*25.4*mm, -0.155*25.4*mm};
    G4double rInner[] = {0.021*25.4*mm, 0.021*25.4*mm, 0.021*25.4*mm, 0.021*25.4*mm, 
    					 	0.021*25.4*mm, 0.021*25.4*mm, 0.021*25.4*mm, 0.021*25.4*mm};
    G4double rOuter[] = {0.125*25.4*mm, 0.125*25.4*mm, 0.1075*25.4*mm, 0.1075*25.4*mm,
    						0.1145*25.4*mm, 0.1145*25.4*mm, 0.1075*25.4*mm, 0.1075*25.4*mm};
    G4Polycone* barrel = new G4Polycone("barrel", 0, 2*pi, 8, zPlanes, rInner, rOuter);

    G4Box* split = new G4Box("split", 0.20*25.4*mm, 0.01*25.4*mm, 0.1*25.4*mm);
    G4SubtractionSolid* body = new G4SubtractionSolid("body", barrel, split, 0,
    						G4ThreeVector(0, 0, -0.17*25.4*mm));

    G4Tubs* tubeCut = new G4Tubs("tubeCut", 0.045*25.4*mm, 0.0845*25.4*mm, 0.2*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, tubeCut, 0, 
    						G4ThreeVector(0, 0, -0.255*25.4*mm));
    
    G4Torus* roundCut = new G4Torus("roundCut", 0, 0.02*25.4*mm, 0.065*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* bushing = new G4SubtractionSolid("Center Bushing", body2, roundCut, 0,
    											G4ThreeVector(0, 0, -0.055*25.4*mm));


    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.7,0.7,0.7)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bushing, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Center Bushing Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Center Bushing Logical" << endlog; 
  return pVol;
}  
