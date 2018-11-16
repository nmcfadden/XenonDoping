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
 * CLASS DECLARATION:  GEGeometryCalibrationSourceArray.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 02-07-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   04-24-2009 - Switch for lock type, Francis
 *
 */

#ifndef _GEGEOMETRYCALIBRATIONSOURCEARRAY_HH 
#define _GEGEOMETRYCALIBRATIONSOURCEARRAY_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "io/MGLogger.hh"
#include "GEGeometryDetectorDB.hh"
#include "GEGeometryCalibrationSource.hh" 

// --------------------------------------------------------------------------- 

class GEGeometryCalibrationSourceArray 
{

public:

  // default constructor 

  GEGeometryCalibrationSourceArray(GEGeometryDetectorDB* detectordb);

  // destructor 

  ~GEGeometryCalibrationSourceArray();

  // methods 

  void ConstructCalibrationSourceArray(); 

private: 

  G4int fNumberOfCalibrationSources;

  G4int LockType;


  std::vector<GEGeometryCalibrationSource*> fCalibrationSourceContainer; 

};

#endif

