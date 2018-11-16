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
// CLASS IMPLEMENTATION:  MJDemoSigCableConnClamp.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jan 14, 2011
 * 
 * REVISION:
 * 
 * 01-14-2011, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSigCableConnClamp.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSigCableConnClamp::MJDemoSigCableConnClamp(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "SigCableConnClamp", "Copper-EF")
{;}

MJDemoSigCableConnClamp::MJDemoSigCableConnClamp(const MJDemoSigCableConnClamp & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSigCableConnClamp::~MJDemoSigCableConnClamp()
{;}

G4LogicalVolume* MJDemoSigCableConnClamp::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
	G4Box* body = new G4Box("body", 0.2687*25.4*mm, 0.6528*25.4*mm, 0.0419*25.4*mm);
	G4DisplacedSolid* body2 = new G4DisplacedSolid("body2", body, 0, 
										G4ThreeVector(0, -0.6528*25.4*mm, 0.0419*25.4*mm));
										
	G4Box* box = new G4Box("box", 0.2687*25.4*mm, 0.3403*25.4*mm, 0.2106*25.4*mm);
	G4UnionSolid* body3 = new G4UnionSolid("body3", body2, box, 0, 
										G4ThreeVector(0, -0.3403*25.4*mm, 0.2106*25.4*mm));
	
	G4Box* boxCut = new G4Box("boxCut", 0.5*25.4*mm, 0.197*25.4*mm, 0.134*25.4*mm);
	G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, boxCut, 0,
										G4ThreeVector(0, -0.3467*25.4*mm, 0.259*25.4*mm));

    G4Box* box2 = new G4Box("box2", 0.5787*25.4*mm, 0.035*25.4*mm, 0.186*25.4*mm);
    G4UnionSolid* clamp = new G4UnionSolid("clamp", body4, box2, 0, 
    									G4ThreeVector(0, -0.035*25.4*mm, 0.186*25.4*mm));
    
    
    //G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.6, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(clamp, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Signal Cable Connector Clamp Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Signal Cable Connector Clamp Logical" << endlog; 
  return pVol;
}  
