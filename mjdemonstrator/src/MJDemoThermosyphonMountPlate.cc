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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonMountPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with lower flat surface, midpoint of edge.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Oct 21, 2010
 * 
 * REVISION:
 * 
 * 10-21-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonMountPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonMountPlate::MJDemoThermosyphonMountPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ThermosyphonMountPlateDwg", "Copper-EF")
{;}

MJDemoThermosyphonMountPlate::MJDemoThermosyphonMountPlate(const MJDemoThermosyphonMountPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonMountPlate::~MJDemoThermosyphonMountPlate()
{;}

G4LogicalVolume* MJDemoThermosyphonMountPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 2.28*25.4*mm, 1.375*25.4*mm, 0.1500*25.4*mm);

    G4Box* sideCut = new G4Box("sideCut", 0.7*25.4*mm, 0.5397*25.4*mm, 0.5*25.4*mm);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, sideCut, 0, 
    								G4ThreeVector(-0.395*25.4*mm, 1.375*25.4*mm, 0));
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, sideCut, 0, 
    								G4ThreeVector(-0.395*25.4*mm, -1.375*25.4*mm, 0));   
    
    G4Tubs* boltHole = new G4Tubs("boltHole", 0, 0.166*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, boltHole, 0,
    								G4ThreeVector(-1.875*25.4*mm, 1.0*25.4*mm, 0));
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, boltHole, 0,
    								G4ThreeVector(-1.875*25.4*mm, -1.0*25.4*mm, 0));
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, boltHole, 0,
    								G4ThreeVector(0.875*25.4*mm, 1.0*25.4*mm, 0));
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, boltHole, 0,
    								G4ThreeVector(0.875*25.4*mm, -1.0*25.4*mm, 0));
    G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7, boltHole, 0,
    								G4ThreeVector(1.875*25.4*mm, 1.0*25.4*mm, 0));
    G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, boltHole, 0,
    								G4ThreeVector(1.875*25.4*mm, -1.0*25.4*mm, 0));


    G4DisplacedSolid* mountplate = new G4DisplacedSolid("mountplate", body9, 0, 
    								G4ThreeVector(0, 0, 0.15*25.4*mm));
    								
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(mountplate, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Thermosyphon Mount Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Mount Plate Logical" << endlog; 
  return pVol;
}  
