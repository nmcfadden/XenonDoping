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
// CLASS IMPLEMENTATION:  MJDemoMuonVeto.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: never
 * 
 * REVISION:
 * 
 * 12-07-2010, Created, M. Kidd
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoMuonVetoSolidPanel.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoMuonVetoSolidPanel::MJDemoMuonVetoSolidPanel(G4String partName, G4String serialNumber, G4double length, G4double height) :
  MJVDemoPart(partName, serialNumber, "MuonVetoSolid", "Scintillator-Pla"), fPanelLength(length*mm), fPanelHeight(height*mm)
{;}

MJDemoMuonVetoSolidPanel::MJDemoMuonVetoSolidPanel(const MJDemoMuonVetoSolidPanel & rhs) : 
  MJVDemoPart(rhs), fPanelLength(rhs.fPanelLength), fPanelHeight(rhs.fPanelHeight)
{;}

MJDemoMuonVetoSolidPanel::~MJDemoMuonVetoSolidPanel()
{;}

G4LogicalVolume* MJDemoMuonVetoSolidPanel::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + G4UIcommand::ConvertToString(fPanelLength) + "x" + G4UIcommand::ConvertToString(fPanelHeight) + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* solidPanel = new G4Box("solidPanel", fPanelLength, fPanelHeight, 25.4*0.5*mm);

    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(solidPanel, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(routine) << "Created a Solid Panel Logical" << endlog;
  }
  else  MGLog(routine) << "Using pre-existing Solid Panel Logical" << endlog; 
  return pVol;
}  
