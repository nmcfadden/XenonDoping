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
// CLASS IMPLEMENTATION:  MJDemoRadShieldPb.cc
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 20, 2010
 * 
 * REVISION:
 * 
 * 09-20-2010, Created, M. Green
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
#include "mjdemonstrator/MJDemoRadShieldPb.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldPb::MJDemoRadShieldPb(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldPbDwg", "Lead-Ain")
{;}

MJDemoRadShieldPb::MJDemoRadShieldPb(const MJDemoRadShieldPb & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoRadShieldPb::~MJDemoRadShieldPb()
{;}

G4LogicalVolume* MJDemoRadShieldPb::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 38.0*25.4*mm, 30.0*25.4*mm, 32.0*25.4*mm);

    G4Box* pocket = new G4Box("pocket", 20.001*25.4*mm, 13.001*25.4*mm, 14.001*25.4*mm);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, pocket);
/*
    G4Box* floorCut1 = new G4Box("floorCut1", 12.5*25.4*mm, 12.5*25.4*mm, 10*25.4*mm);
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, floorCut1, 0, 
    								G4ThreeVector(-7.0315*25.4*mm, -25.5*25.4*mm, -38.0*25.4*mm));
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, floorCut1, 0,
    								G4ThreeVector(32.5*25.4*mm, 0*25.4*mm, -38.0*25.4*mm));

    G4Box* floorCut2 = new G4Box("floorCut2", 16.0*25.4*mm, 16.0*25.4*mm, 10.0*25.4*mm);
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, floorCut2, 0, 
    								G4ThreeVector(-7.0315*25.4*mm, -39.0*25.4*mm, -38.0*25.4*mm));
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, floorCut2, 0,
    								G4ThreeVector(46*25.4*mm, 0*25.4*mm, -38.0*25.4*mm));
*/
    G4Tubs* tubeCut = new G4Tubs("tubeCut", 0, 1.751*25.4*mm, 20*25.4*mm, 0, 2*pi);//1.75
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateY(pi/2);
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body2, tubeCut, rotation,
    									G4ThreeVector(30*25.4*mm, 0*25.4*mm, 4.4265*25.4*mm));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateX(pi/2);
    G4SubtractionSolid* leadShield = new G4SubtractionSolid("leadShield", body7, tubeCut, rotation2,
    						G4ThreeVector(-8.1417*25.4*mm, -20*25.4*mm, 4.4265*25.4*mm));    


    G4VisAttributes* blackVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)); // black
    blackVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(leadShield, material, logicalName);
    pVol->SetVisAttributes(blackVisAtt); 
    MGLog(debugging) << "Created Lead Shield Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Lead Shield Logical" << endlog; 
  return pVol;
}  
