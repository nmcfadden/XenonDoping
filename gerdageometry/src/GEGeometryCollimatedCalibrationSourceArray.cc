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
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh" 
#include "G4ThreeVector.hh" 

#include "gerdageometry/GEGeometryCollimatedCalibrationSourceArray.hh"

// --------------------------------------------------------------------------- 

GEGeometryCollimatedCalibrationSourceArray::GEGeometryCollimatedCalibrationSourceArray(GEGeometryDetectorDB* detectordb)
{

  /* Depending on the lock we have 3 (final lock), 1 (comm lock with 2 detector strings) or 
   * 2 (comm lock with 4 detector strings) calibration source(s) on fixed positions.
   * For 3 and 4 sources the code enables the usage of volumelist.
   * The x y coordinates which corresponds to the numbers for the positioning given below
   * can be found in GEGeometryDetectorDB.cc
   */

  LockType = detectordb -> GetLockType ();

  if (LockType == 1) {                          // final lock
    fNumberOfCollimatedCalibrationSources = 3;

    GEGeometryCollimatedCalibrationSource* calibrationsource0 = new GEGeometryCollimatedCalibrationSource(3, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource0);

    GEGeometryCollimatedCalibrationSource* calibrationsource1 = new GEGeometryCollimatedCalibrationSource(7, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource1);

    GEGeometryCollimatedCalibrationSource* calibrationsource2 = new GEGeometryCollimatedCalibrationSource(11, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource2);
  }


  else if (LockType == 2) {                    // comm lock with 2 strings
    fNumberOfCollimatedCalibrationSources = 1;

    GEGeometryCollimatedCalibrationSource* calibrationsource0 = new GEGeometryCollimatedCalibrationSource(4, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource0);
  }


  else if (LockType == 3) {                    // comm lock with 4 strings
    fNumberOfCollimatedCalibrationSources = 3;

    GEGeometryCollimatedCalibrationSource* calibrationsource0 = new GEGeometryCollimatedCalibrationSource(4, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource0);

    GEGeometryCollimatedCalibrationSource* calibrationsource1 = new GEGeometryCollimatedCalibrationSource(5, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource1);

    GEGeometryCollimatedCalibrationSource* calibrationsource2 = new GEGeometryCollimatedCalibrationSource(6, detectordb);
    fCollimatedCalibrationSourceContainer.push_back(calibrationsource2);

  }

}

// --------------------------------------------------------------------------- 

GEGeometryCollimatedCalibrationSourceArray::~GEGeometryCollimatedCalibrationSourceArray()
{

  fCollimatedCalibrationSourceContainer.clear();

} 

// --------------------------------------------------------------------------- 

void GEGeometryCollimatedCalibrationSourceArray::ConstructCollimatedCalibrationSourceArray()
{

  for (G4int i = 0; i < fNumberOfCollimatedCalibrationSources; i++)
    {
      fCollimatedCalibrationSourceContainer[i] -> ConstructCollimatedCalibrationSource(); 
    }

} 

// --------------------------------------------------------------------------- 
