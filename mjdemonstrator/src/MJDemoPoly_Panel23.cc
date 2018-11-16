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
#include "mjdemonstrator/MJDemoPoly_Panel23.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoPoly_Panel23::MJDemoPoly_Panel23(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "PolyEastWall_ModuleOuter", serialNumber)
{;}

MJDemoPoly_Panel23::MJDemoPoly_Panel23(const MJDemoPoly_Panel23 & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoPoly_Panel23::~MJDemoPoly_Panel23()
{;}

G4LogicalVolume* MJDemoPoly_Panel23::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
		G4Box* box1 = new G4Box("box1", 25.4*mm, 1385.062*mm,  1358.9*mm);    		//raw panel 109.06 x (62+9x5)
		G4Box* cutoutCables = new G4Box("cutoutCables", 25.5*mm, 352.9346*mm, 50.8*mm);    	//27.53x4
	  G4SubtractionSolid* box2 = new G4SubtractionSolid("box2", box1, cutoutCables, 0, G4ThreeVector(0, -1035.431*mm, -876.3*mm));
		G4Tubs* cutoutCalibration = new G4Tubs("cutoutCalibration", 0,19.05*mm,25.5*mm,0,2*pi);  //1.5inch diameter
		G4RotationMatrix* rotationCalibration = new G4RotationMatrix();
		rotationCalibration->rotateY(90*degree);
  	G4SubtractionSolid* box3 = new G4SubtractionSolid("box3", box2, cutoutCalibration, rotationCalibration, G4ThreeVector(0, 165.1*mm, -101.6*mm));
		G4Box* cutoutNitrogen = new G4Box("cutoutNitrogen", 25.5*mm, 50.8*mm,  63.5*mm);    	//4 x5 in 3rd panel from above
  	G4SubtractionSolid* poly_Panel23 = new G4SubtractionSolid("poly_Panel23", box3, cutoutNitrogen, 0, G4ThreeVector(0, -773.557*mm, 1041.4*mm));
	

    G4VisAttributes* greenVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // green
    greenVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(poly_Panel23, material, logicalName);
    pVol->SetVisAttributes(greenVisAtt); 
    MGLog(routine) << "Created a Solid Panel Logical" << endlog;
  }
  else  MGLog(routine) << "Using pre-existing Solid Panel Logical" << endlog; 
  return pVol;
}  
