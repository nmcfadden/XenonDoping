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
// CLASS IMPLEMENTATION:  MJDemoLMFETraces.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Pin contact
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
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoLMFETraces.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoLMFETraces::MJDemoLMFETraces(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "LMFETraces", "Gold")
{;}

MJDemoLMFETraces::MJDemoLMFETraces(const MJDemoLMFETraces & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoLMFETraces::~MJDemoLMFETraces()
{;}

G4LogicalVolume* MJDemoLMFETraces::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
	G4double traceThickness = 430*nm;		
	G4Tubs* pinContact = new G4Tubs("pinContact", 0, 1*mm, traceThickness/2, 0, 2*pi);				
    G4Box* trace1 = new G4Box("trace1", 2.5*mm, 0.38*mm, traceThickness/2);
    G4UnionSolid* traces1 = new G4UnionSolid("traces1", pinContact, trace1, 0, 
    									G4ThreeVector(-0.5*mm, 3.085*mm, 0));
    G4UnionSolid* traces2 = new G4UnionSolid("traces2", traces1, trace1, 0, 
    									G4ThreeVector(-0.5*mm, -3.085*mm, 0));
    G4Box* fetPad = new G4Box("fetPad", 1*mm, 0.5*mm, traceThickness/2);									
    G4UnionSolid* traces3 = new G4UnionSolid("traces3", traces2, fetPad, 0, 
    									G4ThreeVector(7*mm, 0, 0));
    G4Box* trace2 = new G4Box("trace2", 3.5*mm, 0.25*mm, traceThickness/2);
    G4UnionSolid* traces4 = new G4UnionSolid("traces4", traces3, trace2, 0, 
    									G4ThreeVector(3.5*mm, 0, 0));
    G4Box* trace3 = new G4Box("trace3", 1.75*mm, 0.25*mm, traceThickness/2);
    G4UnionSolid* traces5 = new G4UnionSolid("traces5", traces4, trace3, 0, 
    									G4ThreeVector(10.25*mm, 0.5*mm, 0));
    G4UnionSolid* traces6 = new G4UnionSolid("traces6", traces5, trace3, 0, 
    									G4ThreeVector(10.25*mm, -0.5*mm, 0));									
    G4Box* trace4 = new G4Box("trace4", 3*mm, 0.25*mm, traceThickness/2);
    G4UnionSolid* traces7 = new G4UnionSolid("traces7", traces6, trace4, 0, 
    									G4ThreeVector(9*mm, -1.5*mm, 0));
    G4UnionSolid* traces8 = new G4UnionSolid("traces8", traces7, trace3, 0, 
    									G4ThreeVector(10.25*mm, 1.5*mm, 0));
    G4Box* trace5 = new G4Box("trace5", 3.125*mm, 0.25*mm, traceThickness/2);
    G4UnionSolid* traces9 = new G4UnionSolid("traces9", traces8, trace5, 0, 
    									G4ThreeVector(4.375*mm, 1*mm, 0));
    G4Box* trace6 = new G4Box("trace5", 0.684*mm, 0.25*mm, traceThickness/2);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(-26.56*deg);    
    G4UnionSolid* traces10 = new G4UnionSolid("traces10", traces9, trace6, rotation, 
    									G4ThreeVector(8*mm, 1.25*mm, 0));
									


    G4VisAttributes* blackVisAtt = new G4VisAttributes(G4Colour(0, 0, 0)); // black
    blackVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(traces10, material, logicalName);
    pVol->SetVisAttributes(blackVisAtt); 
    MGLog(debugging) << "Created LMFE Traces Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing LMFE Traces Logical" << endlog; 
  return pVol;
}  
