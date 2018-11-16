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
// CLASS IMPLEMENTATION:  MJDemo51624Bolt.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for a generic 5/16-24 hex-head bolt.  Part origin is 
 *coaxial with screw, coincident with shoulder.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Oct 21, 2010
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
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemo51624Bolt.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo51624Bolt::MJDemo51624Bolt(G4String partName, G4String serialNumber, 
								G4double length) :
  MJVDemoPart(partName, serialNumber, "5/16-24", "Copper-EF"), 
  fScrewLength(length*mm)
{;}

MJDemo51624Bolt::MJDemo51624Bolt(const MJDemo51624Bolt & rhs) : 
  MJVDemoPart(rhs), fScrewLength(rhs.fScrewLength)
{;}

MJDemo51624Bolt::~MJDemo51624Bolt()
{;}

G4LogicalVolume* MJDemo51624Bolt::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "x" + G4UIcommand::ConvertToString(fScrewLength) 
  										+ "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zPlanes[] = {0*25.4*mm, 0.2031*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.25*25.4*mm, 0.25*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);    

    G4Tubs* thread = new G4Tubs("thread", 0, 0.135*25.4*mm, fScrewLength/2 + 0.1*mm, 0, 2*pi);//0.136

    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, thread, 0, 
    										G4ThreeVector(0, 0, -fScrewLength/2 + 0.1*mm));    

    // G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // grayVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, material, logicalName);
    // pVol->SetVisAttributes(grayVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created 5/16-24x " << fScrewLength << "mm Screw Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 5/16-24x " << fScrewLength << "mm Screw Logical" << endlog;
  return pVol;
}  


