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
 * CLASS DECLARATION:  GEGeometryDetector.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 * 
 * REVISION: MM-DD-YYYY
 *
 *   03-06-2006, added Cherenkov constructor, Markus
 */

#ifndef _GEGEOMETRYDETECTOR_HH 
#define _GEGEOMETRYDETECTOR_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "GEGeometryDetectorDB.hh"
#include "GEGeometryDetectorArray.hh" 
#include "GEGeometryScintillator.hh" 
#include "GEGeometryCherenkovVeto.hh"

// --------------------------------------------------------------------------- 

class GEGeometryDetector
{

public:

  /// Default constructor
  GEGeometryDetector(GEGeometryDetectorDB* detectordb);

  /// Destructor 
  ~GEGeometryDetector();

  // methods 

  void ConstructDetector();

private: 

  GEGeometryDetectorArray* fDetectorArray;
  GEGeometryScintillator*  fScintillator;
  GEGeometryCherenkovVeto* fCherenkovVeto;

};

#endif

