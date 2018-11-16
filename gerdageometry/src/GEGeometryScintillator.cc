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

/**
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 02-16-2005, Position and orientation of the scintillator read from GEGeometryDetectorDB
 * 07-05-2005, Changed height of the WT, Claudia
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryScintillator.hh"       

// --------------------------------------------------------------------------- 

GEGeometryScintillator::GEGeometryScintillator(GEGeometryDetectorDB* detectordb) 
{

  fScintillatorHeight   = detectordb -> GetScintillatorHeight(); 
  fAirBoxLogical        = detectordb -> GetAirBoxLogical(); 
  fScintillatorLogical  = detectordb -> GetScintillatorLogical(); 
  fScintillatorPosX     = detectordb -> GetScintillatorPosX(); 
  fScintillatorPosY     = detectordb -> GetScintillatorPosY(); 
  fScintillatorPosZ     = detectordb -> GetScintillatorPosZ(); 
  fScintillatorAngleZ   = detectordb -> GetScintillatorAngleZ(); 
  fShieldingWaterTankHeight = detectordb -> GetShieldingWaterTankHeight(); 
  fShieldingWaterHeight = detectordb -> GetShieldingWaterHeight(); 
  fCleanRoomHeight      = detectordb -> GetCleanRoomHeight();  
  fSuperStructureHeight = detectordb -> GetSuperStructureHeight(); 

}

// --------------------------------------------------------------------------- 

GEGeometryScintillator::~GEGeometryScintillator()
{

  // debug 
  //  delete fScintillatorPhysical; 

} 

// --------------------------------------------------------------------------- 

void GEGeometryScintillator::ConstructScintillator()
{

  G4double xx = fScintillatorPosX; 
  G4double yy = fScintillatorPosY; 
  G4double zz = (fShieldingWaterTankHeight - fShieldingWaterHeight) 
    + 0.5 * fShieldingWaterHeight + fCleanRoomHeight
    + fSuperStructureHeight 
    + 0.5 * fScintillatorHeight; 

  G4double angle = fScintillatorAngleZ; 

  G4RotationMatrix* plate_rot = new G4RotationMatrix();
  plate_rot->rotateZ(-1.0*angle);

  fScintillatorPhysical = new G4PVPlacement(plate_rot, 
					    G4ThreeVector(xx,yy,zz),
					    fScintillatorLogical, 
					    "thescintplatephysical",
					    fAirBoxLogical, 
					    false, 
					    0);

} 

// --------------------------------------------------------------------------- 


