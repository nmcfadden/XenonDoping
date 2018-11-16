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
 * CLASS DECLARATION:  GEGeometryScintillator.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   02-16-2005, pointer to GEGeometryDetectorDB registered as a private member, Luciano
 *
 */

#ifndef _GEGEOMETRYSCINTILLATOR_HH 
#define _GEGEOMETRYSCINTILLATOR_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryScintillator
{

public:

  /// Default constructor 
  GEGeometryScintillator(GEGeometryDetectorDB* detectordb); 

  /// Destructor 
  ~GEGeometryScintillator();

  // methods

  void ConstructScintillator(); 

private: 

  G4double fScintillatorHeight; 
  G4double fScintillatorPosX; 
  G4double fScintillatorPosY; 
  G4double fScintillatorPosZ; 
  G4double fScintillatorAngleZ; 
  G4double fShieldingWaterTankHeight; 
  G4double fShieldingWaterHeight; 
  G4double fCleanRoomHeight; 
  G4double fSuperStructureHeight; 

  G4LogicalVolume* fAirBoxLogical;  
  G4LogicalVolume* fScintillatorLogical; 

  G4VPhysicalVolume* fScintillatorPhysical; 

};

#endif

