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
/**                                                            
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION: MJDemoCopperSpacer.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the copper spacer
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Feb 27, 2014
 * 
 * REVISION:
 * 
 * 02-27-2014, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCopperSpacer.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCopperSpacer::MJDemoCopperSpacer(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-121", "Copper-EF"),
  fAdditionalLength(0.0)
{;}

MJDemoCopperSpacer::MJDemoCopperSpacer(const MJDemoCopperSpacer & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCopperSpacer::~MJDemoCopperSpacer()
{;}

G4LogicalVolume* MJDemoCopperSpacer::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = /*fPartName +*/ fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false); //.710, difference of .360
  if (pVol == NULL){
    G4double zPlanes[] = {0*25.4*mm, 0.02*25.4*mm, 0.08*25.4*mm, 0.080*25.4*mm, 0.40*25.4*mm + fAdditionalLength, 0.462*25.4*mm + fAdditionalLength,
			  0.51*25.4*mm + fAdditionalLength, 0.53*25.4*mm + fAdditionalLength, 0.552*25.4*mm + fAdditionalLength/*, -0.862*25.4*mm*/};
    G4double rInner[] = {0.129*25.4*mm, 0.109*25.4*mm, 0.109*25.4*mm, 0.086*25.4*mm, 0.086*25.4*mm, 0.086*25.4*mm,
			 0.086*25.4*mm,0.086*25.4*mm,0.086*25.4*mm/*, 0.086*25.4*mm*/};
    G4double rOuter[] = {0.1875*25.4*mm, 0.1875*25.4*mm, 0.1875*25.4*mm, 0.1875*25.4*mm, 0.1875*25.4*mm,
			 0.125*25.4*mm, 0.125*25.4*mm, 0.105*25.4*mm, 0.105*25.4*mm/*, 0.103*25.4*mm*/};
    G4Polycone* body = new G4Polycone("body", 0, 2*pi, 9, zPlanes, rInner, rOuter);

    //need to union a donut to for bulletization, G4 seems to have an issue with small toruses
    //G4Torus* bullet = new G4Torus("bullet", 0, 0.002*25.4*mm, 0.103*25.4*mm, 0, 2*pi);
    //G4UnionSolid* bulletization = new G4UnionSolid("bulletization", body, bullet);

    //need a donut to subtract out:
    G4Torus* donut = new G4Torus("bite", 0, 0.062*25.4*mm, (0.1875+.0001)*25.4*mm, 0, 2*pi);
    
    G4ThreeVector* translation = new G4ThreeVector(0, 0, 0.462*25.4*mm + fAdditionalLength);
    G4SubtractionSolid* bodyBite = new G4SubtractionSolid("bodyBite", body, donut, 0, *translation);
                                                                   
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bodyBite, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Copper Spacer Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Copper Spacer Logical" << endlog; 
  return pVol;
}
