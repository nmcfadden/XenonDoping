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
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION:
 *
 * 04-08-2008 - Positioning of the sources in x-y plane on arbitrary diode positions (Francis)
 * 04-24-2009 - Switch for lock type, Francis
 * 04-29-2016 - 3 calibration sources for phase2 (LockType_PhaseII_DN500), Janina
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

#include "gerdageometry/GEGeometryCalibrationSourceArray.hh"

// --------------------------------------------------------------------------- 

GEGeometryCalibrationSourceArray::GEGeometryCalibrationSourceArray(GEGeometryDetectorDB* detectordb)
{

  /* Depending on the lock we have 3 (final lock), 1 (comm lock with 2 detector strings) or 
   * 2 (comm lock with 4 detector strings) calibration source(s) on fixed positions.
   * For 3 and 4 sources the code enables the usage of volumelist.
   * The x y coordinates which corresponds to the numbers for the positioning given below
   * can be found in GEGeometryDetectorDB.cc
   */

  LockType = detectordb -> GetLockType ();

  if (LockType == 1) {                          // final lock
    fNumberOfCalibrationSources = 3;

    GEGeometryCalibrationSource* calibrationsource0 = new GEGeometryCalibrationSource(3, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource0);

    GEGeometryCalibrationSource* calibrationsource1 = new GEGeometryCalibrationSource(7, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource1);

    GEGeometryCalibrationSource* calibrationsource2 = new GEGeometryCalibrationSource(11, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource2);
  }


  else if (LockType == 2) {                    // comm lock with 2 strings
    fNumberOfCalibrationSources = 1;

    GEGeometryCalibrationSource* calibrationsource0 = new GEGeometryCalibrationSource(4, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource0);
  }


  else if (LockType == 3) {                    // comm lock with 4 strings
    fNumberOfCalibrationSources = 3;

    GEGeometryCalibrationSource* calibrationsource0 = new GEGeometryCalibrationSource(4, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource0);

    GEGeometryCalibrationSource* calibrationsource1 = new GEGeometryCalibrationSource(5, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource1);

    GEGeometryCalibrationSource* calibrationsource2 = new GEGeometryCalibrationSource(6, detectordb);
    fCalibrationSourceContainer.push_back(calibrationsource2);

  }

  else if (LockType == 5) {                    //  LockType_PhaseII_DN500
    //fNumberOfCalibrationSources = 1;
    // GEGeometryCalibrationSource* calibrationsource0 = new GEGeometryCalibrationSource(0, detectordb);
    // fCalibrationSourceContainer.push_back(calibrationsource0);
   
   //For this look type the sources are positioned independent of the detectors
   //n does not correspond to a string number
   
   fNumberOfCalibrationSources = 3;

   //S1, n = 0
   GEGeometryCalibrationSource* calibrationsource0 = new GEGeometryCalibrationSource(0, detectordb);
   fCalibrationSourceContainer.push_back(calibrationsource0);
    
   //S2, n = 1
   GEGeometryCalibrationSource* calibrationsource1 = new GEGeometryCalibrationSource(1, detectordb);
   fCalibrationSourceContainer.push_back(calibrationsource1);
    
   //S3, n = 2
   GEGeometryCalibrationSource* calibrationsource2 = new GEGeometryCalibrationSource(2, detectordb);
   fCalibrationSourceContainer.push_back(calibrationsource2);
    
  }


}

// --------------------------------------------------------------------------- 

GEGeometryCalibrationSourceArray::~GEGeometryCalibrationSourceArray()
{

  fCalibrationSourceContainer.clear();

} 

// --------------------------------------------------------------------------- 

void GEGeometryCalibrationSourceArray::ConstructCalibrationSourceArray()
{

  for (G4int i = 0; i < fNumberOfCalibrationSources; i++)
    {
      fCalibrationSourceContainer[i] -> ConstructCalibrationSource(); 
    }

} 

// --------------------------------------------------------------------------- 
