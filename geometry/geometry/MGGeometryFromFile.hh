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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 12-11-2006
 *
 * Geometry class to define a (simple) setup reading dimensions and materials from 
 * an external file
 *
 * REVISIONS:
 *  04-02-2007, Luciano. Added GEGeometrySD field to allow more logical volumes to be 
 *                      registered as a single SD
 *
 */

// ---------------------------------------------------------------------------

#ifndef _MGGEOMETRYFROMFILE_HH
#define _MGGEOMETRYFROMFILE_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "geometry/MGGeometryDetector.hh"
#include "geometry/MGGeometryFileGeometryObject.hh"

class GEGeometrySD;

class MGGeometryFromFile : public MGGeometryDetector
{

 public:
  
  // default constructor
  MGGeometryFromFile(G4String);

  // destructor
  ~MGGeometryFromFile();

  void ConstructDetector();

 private:
  std::vector<MGGeometryFileGeometryObject*> *fVolumes;
  G4String fFileName;
  std::vector<G4LogicalVolume*> *fLogVolumes;
  std::vector<G4VSolid*> *fSolids;

 public:
  G4String GetFileName(){return fFileName;};
  void SetFileName(G4String fn){fFileName = fn;};

 private:
  void ReadFile(G4String);
  GEGeometrySD* CrystalSD;

}; 

// --------------------------------------------------------------------------- 

#endif
