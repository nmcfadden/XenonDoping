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
 * CLASS DECLARATION: GEGeometryMiniShroud.hh
 *
 * AUTHOR: Giovanni Benato
 *
 * CONTACT: gbenato *at* physik *dot* uzh *dot* ch
 *
 * FIRST SUBMISSION: 26-03-2012
 *
 * REVISION: MM-DD-YYYY
 * 23-04-2016 Modified mini-shrouds for Phase2, Janina
 * 21-11-2017 Added glued MS rings, KvSturm
 */

#ifndef _GEGEOMETRYMINISHROUD_HH
#define _GEGEOMETRYMINISHROUD_HH

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// ---------------------------------------------------------------------------

class GEGeometryMiniShroud
{

public:

  /// Default constructor
  GEGeometryMiniShroud(GEGeometryDetectorDB* theGeometryDB);
  GEGeometryMiniShroud(GEGeometryDetectorDB* theGeometryDB, G4int row, G4int column);

  /// Destructor
  ~GEGeometryMiniShroud();

  // methods
  G4ThreeVector GetMiniShroudPosition( G4int ii )
  { return fMiniShroudPosition->at(ii); };

  G4ThreeVector GetMiniShroudPhase2TubPosition( G4int ii )
  { return fMiniShroudPhase2TubPosition->at(ii); };

  G4ThreeVector GetMiniShroudPhase2BottomPosition( G4int ii )
  { return fMiniShroudPhase2BottomPosition->at(ii); };

  G4ThreeVector GetMiniShroudPhase2TopPosition( G4int ii )
  { return fMiniShroudPhase2TopPosition->at(ii); };

  G4ThreeVector GetMiniShroudPhase2TopGluedRingPosition( G4int ii )
  { return fMiniShroudPhase2TopGluedRingPosition->at(ii); };

  G4ThreeVector GetMiniShroudPhase2BottomGluedRingPosition( G4int ii )
  { return fMiniShroudPhase2BottomGluedRingPosition->at(ii); };

  void ConstructMiniShroud();

private:

  std::vector<G4ThreeVector>* fMiniShroudPosition;
  std::vector<G4ThreeVector>* fMiniShroudPhase2TubPosition;
  std::vector<G4ThreeVector>* fMiniShroudPhase2BottomPosition;
  std::vector<G4ThreeVector>* fMiniShroudPhase2TopPosition;
  std::vector<G4ThreeVector>* fMiniShroudPhase2TopGluedRingPosition;
  std::vector<G4ThreeVector>* fMiniShroudPhase2BottomGluedRingPosition;
  std::vector<G4int>* fMiniShroudColumn;
  std::vector<G4int>* fMiniShroudRow;

  G4LogicalVolume* fDetectorMotherVolume;
  std::vector<G4LogicalVolume*>* fMiniShroudLogical;
  std::vector<G4LogicalVolume*>* fMiniShroudPhase2TubLogical;
  std::vector<G4LogicalVolume*>* fMiniShroudPhase2BottomLogical;
  std::vector<G4LogicalVolume*>* fMiniShroudPhase2TopLogical;
  std::vector<G4LogicalVolume*>* fMiniShroudPhase2TopGluedRingLogical;
  std::vector<G4LogicalVolume*>* fMiniShroudPhase2BottomGluedRingLogical;

  std::vector<G4VPhysicalVolume*>* fMiniShroudPhysical;
  std::vector<G4VPhysicalVolume*>* fMiniShroudPhase2TubPhysical;
  std::vector<G4VPhysicalVolume*>* fMiniShroudPhase2BottomPhysical;
  std::vector<G4VPhysicalVolume*>* fMiniShroudPhase2TopPhysical;
  std::vector<G4VPhysicalVolume*>* fMiniShroudPhase2TopGluedRingPhysical;
  std::vector<G4VPhysicalVolume*>* fMiniShroudPhase2BottomGluedRingPhysical;

  G4bool fMSMaterialFlag;
};

#endif
