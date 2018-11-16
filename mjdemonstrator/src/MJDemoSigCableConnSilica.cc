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
// CLASS IMPLEMENTATION:  MJDemoSigCableConnSilica.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Center of silica stack, bottomface of bottom wafer
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

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSigCableConnSilica.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSigCableConnSilica::MJDemoSigCableConnSilica(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "SigCableConnSilica", "Silica")
{;}

MJDemoSigCableConnSilica::MJDemoSigCableConnSilica(const MJDemoSigCableConnSilica & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSigCableConnSilica::~MJDemoSigCableConnSilica()
{;}

G4LogicalVolume* MJDemoSigCableConnSilica::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
	G4Box* body = new G4Box("body", 0.2687*25.4*mm, 0.195*25.4*mm, 0.00985*25.4*mm);
	G4UnionSolid* body2 = new G4UnionSolid("body2", body, body, 0,
												G4ThreeVector(0.250*25.4*mm, 0, 0));
	
	G4Box* tab = new G4Box("tab", 0.09845*25.4*mm, 0.0787*25.4*mm, 0.00985*25.4*mm);
	G4UnionSolid* wafer = new G4UnionSolid("wafer", body2, tab, 0, 
												G4ThreeVector(0.42025*25.4*mm, 0.195*25.4*mm, 0));

	G4RotationMatrix* flip = new G4RotationMatrix();
	flip->rotateY(pi);
    G4double spacing = 0.021*25.4*mm;

    G4DisplacedSolid* stack1 = new G4DisplacedSolid("stack1", wafer, 0, 
    											G4ThreeVector(0, 0, 0.00985*25.4*mm));
    G4UnionSolid* stack2 = new G4UnionSolid("stack2", stack1, wafer, flip, 
    											G4ThreeVector(0, 0, spacing));
    G4UnionSolid* stack3 = new G4UnionSolid("stack3", stack2, wafer, 0, 
    											G4ThreeVector(0, 0, 2*spacing));
    G4UnionSolid* stack4 = new G4UnionSolid("stack4", stack3, wafer, flip, 
    											G4ThreeVector(0, 0, 3*spacing));
    G4UnionSolid* stack5 = new G4UnionSolid("stack5", stack4, wafer, 0, 
    											G4ThreeVector(0, 0, 4*spacing));
    G4UnionSolid* stack6 = new G4UnionSolid("stack6", stack5, wafer, flip, 
    											G4ThreeVector(0, 0, 5*spacing));
    G4UnionSolid* stack7 = new G4UnionSolid("stack7", stack6, wafer, 0, 
    											G4ThreeVector(0, 0, 6*spacing));
    G4UnionSolid* stack8 = new G4UnionSolid("stack8", stack7, wafer, flip, 
    											G4ThreeVector(0, 0, 7*spacing));
    G4UnionSolid* stack9 = new G4UnionSolid("stack9", stack8, wafer, 0, 
    											G4ThreeVector(0, 0, 8*spacing));
    G4UnionSolid* stack10 = new G4UnionSolid("stack10", stack9, wafer, flip, 
    											G4ThreeVector(0, 0, 9*spacing));
    											

    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); // white
    whiteVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(stack10, material, logicalName);
    pVol->SetVisAttributes(whiteVisAtt); 
    MGLog(debugging) << "Created Signal Cable Connector Silica Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Signal Cable Connector Silica Logical" << endlog; 
  return pVol;
}  
