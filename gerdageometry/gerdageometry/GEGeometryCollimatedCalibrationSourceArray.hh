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
 * CLASS DECLARATION:  GEGeometryCollimatedCalibrationSourceArray.hh
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

#ifndef _GEGEOMETRYCOLLIMATEDCALIBRATIONSOURCEARRAY_HH 
#define _GEGEOMETRYCOLLIMATEDCALIBRATIONSOURCEARRAY_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "io/MGLogger.hh"
#include "GEGeometryDetectorDB.hh"
#include "GEGeometryCollimatedCalibrationSource.hh" 

// --------------------------------------------------------------------------- 

class GEGeometryCollimatedCalibrationSourceArray 
{

public:

  // default constructor 

  GEGeometryCollimatedCalibrationSourceArray(GEGeometryDetectorDB* detectordb);

  // destructor 

  ~GEGeometryCollimatedCalibrationSourceArray();

  // methods 

  void ConstructCollimatedCalibrationSourceArray(); 

private: 

  G4int fNumberOfCollimatedCalibrationSources;

  G4int LockType;

  std::vector<GEGeometryCollimatedCalibrationSource*> fCollimatedCalibrationSourceContainer; 

};

#endif

