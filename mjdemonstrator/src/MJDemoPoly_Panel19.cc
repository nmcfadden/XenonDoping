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
#include "mjdemonstrator/MJDemoPoly_Panel19.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoPoly_Panel19::MJDemoPoly_Panel19(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "PolySouthWall_ChillerInner", serialNumber)
{;}

MJDemoPoly_Panel19::MJDemoPoly_Panel19(const MJDemoPoly_Panel19 & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoPoly_Panel19::~MJDemoPoly_Panel19()
{;}

G4LogicalVolume* MJDemoPoly_Panel19::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
		G4Box* box1 = new G4Box("box1", 809.625*mm, 25.4*mm, 1143*mm);    		//raw panel 63.75 x (60+5x6)
		G4Box* cutout1 = new G4Box("cutout1",  100.838*mm, 25.5*mm, 64.262*mm);    	//7.94x5.06
	  G4SubtractionSolid* box2 = new G4SubtractionSolid("box2", box1, cutout1, 0, G4ThreeVector(100.711*mm, 0, -680.72*mm));
		G4Box* cutout2 = new G4Box("cutout2",  154.178*mm, 25.5*mm, 149.225*mm);    	//12.14x11.75
    G4SubtractionSolid* box3 = new G4SubtractionSolid("box3", box2, cutout2, 0, G4ThreeVector(56.261*mm, 0, -200.787*mm));
		G4Box* cutout3 = new G4Box("cutout3", 101.6*mm, 25.5*mm, 50.8*mm);    	//8 x4 in 3rd panel from above
  	G4SubtractionSolid* poly_Panel19 = new G4SubtractionSolid("poly_Panel19", box3, cutout3, 0, G4ThreeVector(178.435*mm, 0, 787.4*mm));
	

    G4VisAttributes* greenVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // green
    greenVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(poly_Panel19, material, logicalName);
    pVol->SetVisAttributes(greenVisAtt); 
    MGLog(routine) << "Created a Solid Panel Logical" << endlog;
  }
  else  MGLog(routine) << "Using pre-existing Solid Panel Logical" << endlog; 
  return pVol;
}  
