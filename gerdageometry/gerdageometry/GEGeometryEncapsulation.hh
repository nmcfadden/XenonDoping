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
 * CLASS DECLARATION: GEGeometryEncapsulation.hh
 *
 * AUTHOR: Sabine Hemmer
 *
 * CONTACT: hemmer *at* pd *dot* infn *dot* it
 *
 * FIRST SUBMISSION: 14-06-2011
 *
 * REVISION: MM-DD-YYYY
 *
 */

#ifndef _GEGEOMETRYENCAPSULATION_HH 
#define _GEGEOMETRYENCAPSULATION_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryEncapsulation 
{

public:

  /// Default constructor
  GEGeometryEncapsulation(GEGeometryDetectorDB* theGeometryDB); 

  /// Destructor
  ~GEGeometryEncapsulation();

  // methods

  G4ThreeVector GetCopperEncapsulationPosition() 
  { return fCopperEncapsulationPosition; }; 

  void ConstructCopperEncapsulation(); 

  G4ThreeVector GetTeflonEncapsulationPosition() 
  { return fTeflonEncapsulationPosition; }; 

  void ConstructTeflonEncapsulation(); 


private: 

  G4ThreeVector fCopperEncapsulationPosition; 
  G4int fCopperEncapsulationColumn;
  G4int fCopperEncapsulationRow;

  G4LogicalVolume* fDetectorMotherVolume;
  G4LogicalVolume* fCopperEncapsulationLogical;

  G4VPhysicalVolume* fCopperEncapsulationPhysical;

  G4ThreeVector fTeflonEncapsulationPosition;
  G4int fTeflonEncapsulationColumn;
  G4int fTeflonEncapsulationRow;

  G4LogicalVolume* fTeflonEncapsulationLogical;
  G4LogicalVolume* fTeflonEncapsulationCopperLogical;

  G4VPhysicalVolume* fTeflonEncapsulationPhysical;
  G4VPhysicalVolume* fTeflonEncapsulationCopperPhysical;
};

#endif

