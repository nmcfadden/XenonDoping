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
 * AUTHOR: GiovanniBenato
 *
 * CONTACT: gbenato * at * physik *dot *uzh *dot *ch
 *
 * FIRST SUBMISSION: 30-05-1012
 *
 * REVISION:
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

//#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryCollimatedCalibrationSource.hh"       

// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryCollimatedCalibrationSource::GEGeometryCollimatedCalibrationSource(G4int n, GEGeometryDetectorDB* detectordb)
{
  detectordb -> IncreaseCollimatedCalibrationSourceCounter();

  fCollimatedCalibrationSourceCounter      = detectordb -> GetCollimatedCalibrationSourceCounter();  
  fNCollimatedCalibrationSource            = n; 
  fCollimatedCalibrationSourcePositionX    = detectordb -> GetCollimatedCalibrationSourcePositionX( n ); 
  fCollimatedCalibrationSourcePositionY    = detectordb -> GetCollimatedCalibrationSourcePositionY( n ); 
  fCollimatedCalibrationSourcePositionZ    = detectordb -> GetCollimatedCalibrationSourcePositionZ( n ); 
  fCollimatedCalibrationCylinderHeight     = detectordb -> GetCollimatedCalibrationCylinderHeight(); 
  fCollimatedCalibrationCapHeight     = detectordb -> GetCollimatedCalibrationCapHeight(); 
  fCollimatedCalibrationCapsuleHeight     = detectordb -> GetCollimatedCalibrationCapsuleHeight(); 
  fCollimatedCalibrationSourceHeight     = detectordb -> GetCollimatedCalibrationSourceHeight(); 
  fCollimationDistance     = detectordb -> GetCalibrationCollimationDistance(); 

  fCollimatedCalibrationSourcePosition     = detectordb -> GetCrystalPosition(n, 0); 

  fDetectorMotherVolume          = detectordb -> GetDetectorMotherVolume(); 
  fCollimatedCalibrationCylinderLogical    = detectordb -> GetCollimatedCalibrationCylinderLogical(); 
  fCollimatedCalibrationSourceInnerLogical = detectordb -> GetCollimatedCalibrationSourceInnerLogical(); 
  fCollimatedCalibrationSourceOuterLogical = detectordb -> GetCollimatedCalibrationSourceOuterLogical(); 

}

// --------------------------------------------------------------------------- 

GEGeometryCollimatedCalibrationSource::~GEGeometryCollimatedCalibrationSource()
{
    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------

    //delete fCollimatedCalibrationCylinderPhysical; 
    //delete fCollimatedCalibrationSourceInnerPhysical; 
    //delete fCollimatedCalibrationSourceOuterPhysical; 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryCollimatedCalibrationSource::GetSourcePosition()
{

  G4ThreeVector position = fCollimatedCalibrationSourcePosition; 
  
  return position; 

}

// --------------------------------------------------------------------------- 

G4double GEGeometryCollimatedCalibrationSource::GetAngle(G4ThreeVector /*position*/) 
{

  // debug: calculate angle from position so that the opening of the 
  // debug: sources shows towards the inside of the array 

  G4double angle; 

  angle = 0.0 * deg; 

  return angle; 
  
}

// --------------------------------------------------------------------------- 

void GEGeometryCollimatedCalibrationSource::ConstructCollimatedCalibrationSource()
{
  
  //G4double         angle; 
  G4RotationMatrix rm; 
  G4ThreeVector    position; 
  char             volumename[40]; 


  position = GetSourcePosition(); 

  // construct physical volume: absorber

  sprintf(volumename, "coll_calibration_cylinder_%d", fCollimatedCalibrationSourceCounter); 

  fCollimatedCalibrationCylinderPhysical = new G4PVPlacement(0,
							     position,
  					   fCollimatedCalibrationCylinderLogical, 
  					   volumename,
  					   fDetectorMotherVolume, 
  					   false, 
  					   fNCollimatedCalibrationSource);   
  

  // construct physical volume : source 

  position.setZ(position.getZ() + 0.5*(fCollimatedCalibrationCylinderHeight + fCollimationDistance ) );

  sprintf(volumename, "coll_calibration_source_inner_%d", fCollimatedCalibrationSourceCounter); 

  fCollimatedCalibrationSourceInnerPhysical = new G4PVPlacement(0,
						      position, 
						      fCollimatedCalibrationSourceInnerLogical, 
						      volumename,
						      fDetectorMotherVolume, 
						      false, 
						      fNCollimatedCalibrationSource);   
  


  // construct physical volume: capsule

  sprintf(volumename, "coll_calibration_source_outer_%d", fCollimatedCalibrationSourceCounter); 

  //position.setZ(position.getZ()-4.15*mm);
  // position.setZ(position.getZ()-30.5*mm);    // Used when no absorber is simulated

  fCollimatedCalibrationSourceOuterPhysical = new G4PVPlacement(0,
						      position, 
						      fCollimatedCalibrationSourceOuterLogical, 
						      volumename,
						      fDetectorMotherVolume, 
						      false, 
						      fNCollimatedCalibrationSource);   
    
} 

// --------------------------------------------------------------------------- 


