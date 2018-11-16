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
 * CLASS DECLARATION:  GEGeometryDetectorArray.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real confituration in GERDA (Giovanni)
 * 04-10-2016 - Adapted fCrystalType array to 7 instead of 5 possible slots per string,Janina
 */

#ifndef _GEGEOMETRYDETECTORARRAY_HH 
#define _GEGEOMETRYDETECTORARRAY_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "GEGeometryDetectorDB.hh"
#include "GEGeometryCrystal.hh" 
#include "GEGeometryHolder.hh" 
#include "GEGeometryCable.hh" 
#include "GEGeometryElectronics.hh" 

// --------------------------------------------------------------------------- 

class GEGeometryDetectorArray
{

public:

  /// Default constructor
  GEGeometryDetectorArray(GEGeometryDetectorDB* detectordb); 

  /// Destructor
  ~GEGeometryDetectorArray();

  // methods 

  void ConstructDetectorArray(); //FIXME: hab Arg hinzugefügt Anne  .. GEGeometryDetectorDB* detectordb

private: 

  G4bool fFlagConstructCrystals; 
  G4bool fFlagConstructSupport; 
  G4bool fFlagConstructCabling; 
  G4bool fFlagReverseNonTrueCoaxial;

  G4int fCrystalType[19][7]; 

  std::vector<GEGeometryCrystal*> fCrystalContainer; 
  std::vector<GEGeometryHolder*> fHolderContainer; 
  std::vector<GEGeometryCable*> fCableContainer; 
  std::vector<GEGeometryElectronics*> fElectronicsContainer; 

};

#endif

