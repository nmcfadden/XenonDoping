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
// CLASS IMPLEMENTATION:  MJDemoPolyAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the Poly Shield
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
 * AUTHOR: Ralph Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: never
 * 
 * REVISION:
 * 
 * 09-01-2017, Created, R. Massarczyk
 */
// --------------------------------------------------------------------------//
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
#include "mjdemonstrator/MJDemoPoly_Panel18.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoPoly_Panel18::MJDemoPoly_Panel18(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "PolyEastWall_ChillerOuter", serialNumber)
{;}

MJDemoPoly_Panel18::MJDemoPoly_Panel18(const MJDemoPoly_Panel18 & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoPoly_Panel18::~MJDemoPoly_Panel18()
{;}

G4LogicalVolume* MJDemoPoly_Panel18::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
		G4Box* box1 = new G4Box("box1", 25.4*mm, 466.725*mm, 1143*mm);    		//raw panel 36.75 x (60+5x6)
		G4Box* cutout1 = new G4Box("cutout1",  25.5*mm, 100.838*mm, 64.262*mm);    	//7.94x5.06
	  G4SubtractionSolid* box2 = new G4SubtractionSolid("box2", box1, cutout1, 0, G4ThreeVector(0, -80.391*mm, -680.72*mm));
		G4Box* cutout2 = new G4Box("cutout2",  25.5*mm, 154.178*mm, 149.225*mm);    	//12.14x11.75
    G4SubtractionSolid* box3 = new G4SubtractionSolid("box3", box2, cutout2, 0, G4ThreeVector(0, -31.623*mm, -200.787*mm));
		G4Box* cutout3 = new G4Box("cutout3", 25.5*mm, 101.6*mm, 50.8*mm);    	//8 x4 in 3rd panel from above
  	G4SubtractionSolid* poly_Panel18 = new G4SubtractionSolid("poly_Panel18", box3, cutout3, 0, G4ThreeVector(0, -88.011*mm, 787.4*mm));
	

    G4VisAttributes* greenVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // green
    greenVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(poly_Panel18, material, logicalName);
    pVol->SetVisAttributes(greenVisAtt); 
    MGLog(routine) << "Created a Solid Panel Logical" << endlog;
  }
  else  MGLog(routine) << "Using pre-existing Solid Panel Logical" << endlog; 
  return pVol;
}  
