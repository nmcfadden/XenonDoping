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
 * CLASS DECLARATION: GEGeometryCalibrationSource.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 02-07-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   08-25-2008 - Include counter to enable volumelist for calibration sources, Francis
 *   29-04-2016 - Source position independent of detector positions for phase2 (LockType_PhaseII_DN500),Janina
 *
 */

#ifndef _GEGEOMETRYCALIBRATIONSOURCE_HH 
#define _GEGEOMETRYCALIBRATIONSOURCE_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryCalibrationSource 
{

public:

  /// Default constructor 
  GEGeometryCalibrationSource(G4int n, GEGeometryDetectorDB* theGeometryDB); 

  /// Destructor 
  ~GEGeometryCalibrationSource();

  // methods

  G4ThreeVector GetSourcePosition(); 
  G4double GetAngle(G4ThreeVector position); 

  void ConstructCalibrationSource(); 

private: 

  GEGeometryDetectorDB* fDetectordb;

  G4int fNCalibrationSource; 
  //G4int fNumberOfColumns; 
  G4int fCalibrationSourceCounter;

  G4ThreeVector fCalibrationSourcePosition; 

  //G4double fColumnDistance; 
  G4double fCalibrationSourcePositionZ; 
  G4double fCalSourcePhase2DN500AngleOffset;
  G4double fCalSourcePhase2DN500Radius;
  G4double fCalibrationSourcePhase2DN500OffsetX;
  G4double fCalibrationSourcePhase2DN500OffsetY;
  G4double fCalSourcePhase2CoordTrafo;
  G4double fCalibrationCylinderHeight; 
  G4double fPhase2HolderStarPlacement;
  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fCalibrationCylinderLogical; 
  G4LogicalVolume* fCalibrationSourceInnerLogical; 
  G4LogicalVolume* fCalibrationSourceOuterLogical; 
  G4LogicalVolume* fCalibrationCylCapLogical;

  G4VPhysicalVolume* fCalibrationCylinderPhysical; 
  G4VPhysicalVolume* fCalibrationSourceInnerPhysical; 
  G4VPhysicalVolume* fCalibrationSourceOuterPhysical; 
  G4VPhysicalVolume* fCalibrationCylCapPhysical;
  
  G4bool fCalibrationCuCap; 
};

#endif

