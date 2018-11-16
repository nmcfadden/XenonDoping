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
 * CLASS DECLARATION: GEGeometryCollimatedCalibrationSource.hh
 *
 * AUTHOR: GiovanniBenato
 *
 * CONTACT: gbenato * at * physik *dot *uzh *dot *ch
 *
 * FIRST SUBMISSION: 30-05-1012
 *
 * REVISION: MM-DD-YYYY
 *
 *    
 *
 */

#ifndef _GEGEOMETRYCOLLIMATEDCALIBRATIONSOURCE_HH 
#define _GEGEOMETRYCOLLIMATEDCALIBRATIONSOURCE_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryCollimatedCalibrationSource 
{

public:

  /// Default constructor 
  GEGeometryCollimatedCalibrationSource(G4int n, GEGeometryDetectorDB* theGeometryDB); 

  /// Destructor 
  ~GEGeometryCollimatedCalibrationSource();

  // methods

  G4ThreeVector GetSourcePosition(); 
  G4double GetAngle(G4ThreeVector position); 

  void ConstructCollimatedCalibrationSource(); 

private: 

  G4int fNCollimatedCalibrationSource; 
  //G4int fNumberOfColumns; 
  G4int fCollimatedCalibrationSourceCounter;

  G4ThreeVector fCollimatedCalibrationSourcePosition; 

  //G4double fColumnDistance; 
  G4double fCollimatedCalibrationSourcePositionX; 
  G4double fCollimatedCalibrationSourcePositionY; 
  G4double fCollimatedCalibrationSourcePositionZ; 
  G4double fCollimatedCalibrationCylinderHeight; 
  G4double fCollimatedCalibrationCapHeight; 
  G4double fCollimatedCalibrationCapsuleHeight; 
  G4double fCollimatedCalibrationSourceHeight;  
  G4double fCollimationDistance;

  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fCollimatedCalibrationCylinderLogical; 
  G4LogicalVolume* fCollimatedCalibrationSourceInnerLogical; 
  G4LogicalVolume* fCollimatedCalibrationSourceOuterLogical; 

  G4VPhysicalVolume* fCollimatedCalibrationCylinderPhysical; 
  G4VPhysicalVolume* fCollimatedCalibrationSourceInnerPhysical; 
  G4VPhysicalVolume* fCollimatedCalibrationSourceOuterPhysical; 

};

#endif

