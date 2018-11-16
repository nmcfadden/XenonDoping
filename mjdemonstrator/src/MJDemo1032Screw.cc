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
// CLASS IMPLEMENTATION:  MJDemo1032Screw.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for a generic 10-32 hex-head (round slotted) machine screw.  Part origin is 
 *coaxial with screw, coincident with shoulder.
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
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemo1032Screw.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo1032Screw::MJDemo1032Screw(G4String partName, G4String serialNumber, 
								G4double length) :
  MJVDemoPart(partName, serialNumber, "10-32", "Copper-EF"), 
  fScrewLength(length*mm)
{;}

MJDemo1032Screw::MJDemo1032Screw(const MJDemo1032Screw & rhs) : 
  MJVDemoPart(rhs), fScrewLength(rhs.fScrewLength)
{;}

MJDemo1032Screw::~MJDemo1032Screw()
{;}

G4LogicalVolume* MJDemo1032Screw::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "x" + G4UIcommand::ConvertToString(fScrewLength) 
  										+ "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4double zPlanes[] = {0*25.4*mm, 0.125*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.156*25.4*mm, 0.156*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);    

    G4Tubs* thread = new G4Tubs("thread", 0, 0.0845*25.4*mm, fScrewLength/2 + 0.1*mm, 0, 2*pi);

    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, thread, 0, 
    										G4ThreeVector(0, 0, -fScrewLength/2 + 0.1*mm)); 

/*  FOR SLOTTED SCREW
    G4Tubs* shoulder = new G4Tubs("shoulder", 0, 0.161*25.4*mm, 0.026*25.4*mm, 0, 2*pi);
    G4DisplacedSolid* body = new G4DisplacedSolid("body", shoulder, 0, 
    										G4ThreeVector(0, 0, 0.026*25.4*mm));
    G4UnionSolid* body2 = new G4UnionSolid("body2", body, thread, 0, 
    										G4ThreeVector(0, 0, -fScrewLength/2 + 0.1*mm));
    										
    G4Orb* head = new G4Orb("head", 0.294*25.4*mm);
    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 0, 0.5*25.4*mm, 0.4*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* head2 = new G4SubtractionSolid("head2", head, cylinderCut, 0,
    										G4ThreeVector(0, 0, -0.1488*25.4*mm));
    
    G4UnionSolid* body3 = new G4UnionSolid("body3", body2, head2, 0, 
    										G4ThreeVector(0, 0, (0.052-0.2512)*25.4*mm));
    
    G4Box* slot = new G4Box("slot", 0.2*25.4*mm, 0.02*25.4*mm, 0.2*25.4*mm);
    G4SubtractionSolid* screw = new G4SubtractionSolid("screw", body3, slot, 0, 
    										G4ThreeVector(0, 0, 0.2456*25.4*mm));
*/

    // G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // grayVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, material, logicalName);
    // pVol->SetVisAttributes(grayVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created 10-32x " << fScrewLength << "mm Screw Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 10-32x " << fScrewLength << "mm Screw Logical" << endlog;
  return pVol;
}


