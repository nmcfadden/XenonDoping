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
// CLASS IMPLEMENTATION:  MJDemoLMFESubstrate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM
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
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoLMFESubstrate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoLMFESubstrate::MJDemoLMFESubstrate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "LMFESubstrate", "Silica")
{;}

MJDemoLMFESubstrate::MJDemoLMFESubstrate(const MJDemoLMFESubstrate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoLMFESubstrate::~MJDemoLMFESubstrate()
{;}

G4LogicalVolume* MJDemoLMFESubstrate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
			
	G4Box* body = new G4Box("body", 11.81*mm, 3.465*mm, 0.125*mm);
	G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.03*25.4*mm, 0.2*25.4*mm, 0, 2*pi);
	G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, thruHole, 0,
											G4ThreeVector(9.08*mm, 0, 0));
	G4SubtractionSolid* substrate = new G4SubtractionSolid("substrate", body2, thruHole, 0,
											G4ThreeVector(5.93*mm, 0, 0));											

    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); // white
    whiteVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(substrate, material, logicalName);
    pVol->SetVisAttributes(whiteVisAtt); 
    MGLog(debugging) << "Created LMFE Substrate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing LMFE Substrate Logical" << endlog; 
  return pVol;
}  
