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
#include "mjdemonstrator/MJDemoMuonVetoThroughPanel.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoMuonVetoThroughPanel::MJDemoMuonVetoThroughPanel(G4String partName, G4String serialNumber, G4double length, G4double height, G4double holePosX, G4double holePosY, G4double cutOutX, G4double cutOutY, G4double cutOutAng) :
  MJVDemoPart(partName, serialNumber, "MuonVetoThrough", "Scintillator-Pla"), fPanelLength(length*mm), fPanelHeight(height*mm), fholePosX(holePosX*mm), fholePosY(holePosY*mm), fcutOutX(cutOutX*mm), fcutOutY(cutOutY*mm), fcutOutAng(cutOutAng*deg)
{;}

MJDemoMuonVetoThroughPanel::MJDemoMuonVetoThroughPanel(const MJDemoMuonVetoThroughPanel & rhs) : 
  MJVDemoPart(rhs), fPanelLength(rhs.fPanelLength), fPanelHeight(rhs.fPanelHeight), fholePosX(rhs.fholePosX), fholePosY(rhs.fholePosY), fcutOutX(rhs.fcutOutX), fcutOutY(rhs.fcutOutY), fcutOutAng(rhs.fcutOutAng)
{;}

MJDemoMuonVetoThroughPanel::~MJDemoMuonVetoThroughPanel()
{;}

G4LogicalVolume* MJDemoMuonVetoThroughPanel::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + G4UIcommand::ConvertToString(fPanelLength) + "x" + G4UIcommand::ConvertToString(fPanelHeight) + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* solidPanel = new G4Box("solidPanel", fPanelLength, fPanelHeight, 25.4*0.5*mm);
    G4Tubs* panelHole = new G4Tubs("panelHole", 0.0, 25.4*3.2*mm, 25.4*2.0*0.5*mm, 0*deg, 360*deg);
    G4Box* cutOutPanel = new G4Box("cutOutPanel", 25.4*16.625*mm, 25.4*2.125*mm, 25.4*2.0*0.5*mm);

    G4ThreeVector hole_pos(fholePosX, fholePosY, 0);
    G4ThreeVector cutOutPos(fcutOutX, fcutOutY, 0);
    G4RotationMatrix *cutOutRot = new G4RotationMatrix();
    cutOutRot->rotateZ(fcutOutAng);

    G4SubtractionSolid* vetoPanel1 = new G4SubtractionSolid("vetoPanel1", solidPanel, panelHole, 0, hole_pos);

    G4SubtractionSolid* vetoPanel = new G4SubtractionSolid("vetoPanel", vetoPanel1, cutOutPanel, cutOutRot, cutOutPos);

    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(vetoPanel, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(routine) << "Created a Through Panel Logical" << endlog;
  }
  else  MGLog(routine) << "Using pre-existing Through Panel Logical" << endlog; 
  return pVol;
}  
