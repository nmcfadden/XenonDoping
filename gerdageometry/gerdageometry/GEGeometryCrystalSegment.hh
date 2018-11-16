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
 * CLASS DECLARATION: GEGeometryCrystalSegment.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-23-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   21-04-2005, for non-true-coaxial detector,
 *               put segment into deadlayer, Xiang
 *
 */

#ifndef _GEGEOMETRYCRYSTALSEGMENT_HH 
#define _GEGEOMETRYCRYSTALSEGMENT_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryCrystalSegment
{

public:

  /// Default constructor
  GEGeometryCrystalSegment(G4int phiseg, G4int zseg, G4int col, G4int row,
                           GEGeometryDetectorDB* detectordb); 

  /// Destructor
  ~GEGeometryCrystalSegment();

  // methods

  G4double      GetSegmentAngle(G4int seg); 
  G4ThreeVector GetSegmentPosition(G4int segrow);
  void          ConstructCrystalSegment(); 

private: 

  G4int fSegmentNumber; 
  G4int fPhiSegmentNumber; 
  G4int fZSegmentNumber; 
  G4int fCrystalColumn; 
  G4int fCrystalRow; 
  G4int fPhiSegmentsPerCrystal; 
  G4int fZSegmentsPerCrystal; 
  G4int fSegmentsPerCrystal; 
  G4double fSegmentHeight; 
  G4double fCrystalHeight; 
  G4double fSegmentStartingAngle; 
  G4double fCrystalDLTopThickness; 
  G4double fCrystalDLBottomThickness; 
  G4double fCrystalPassivationThickness; 
  //G4double fCrystalSiLayerHeight; 
  //G4double fCrystalHoleDepth; 
  G4double fHolderInnerHeight; 
  G4double fRelativeSegmentZPosition; 

  G4ThreeVector fCrystalPosition; 
  G4ThreeVector fSegmentPosition; 

  GEGeometryDetectorParameters* fDetectorParameters; 

  G4int fCrystalType; 
  G4int fCrystalGeometry; 

  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fCrystalSegmentLogical; 

  G4VPhysicalVolume* fCrystalSegmentPhysical; 

};

#endif

