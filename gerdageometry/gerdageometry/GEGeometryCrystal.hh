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
 * CLASS DECLARATION: GEGeometryCrystal.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   01-23-2005, added crystal segments, Kevin 
 *   30-04-2008 - changes due to a new implementation of Phase-1 crystals, 
 *                the hold version did cause some problems (an overlap between the crystal and its CopperContactPlate),
 *              - (Jens S.)
 *   05-11-2011 - Extended with a few more parameters to hold a new type of crystal (back-to-back  BEGe's). (N. Barros)
 *   		  Modified constructor to accept an additional rotation flag to apply to the back-to-back BEGe's
 */

// ---------------------------------------------------------------------------

#ifndef _GEGEOMETRYCRYSTAL_HH 
#define _GEGEOMETRYCRYSTAL_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "GEGeometryDetectorDB.hh"
#include "GEGeometryCrystalSegment.hh" 

// --------------------------------------------------------------------------- 

class GEGeometryCrystal 
{

public:

  /// Default constructor 
  GEGeometryCrystal(GEGeometryDetectorDB* theGeometryDB,G4int col, G4int row, 
		    G4int multiplet=0);

  /// Destructor
  ~GEGeometryCrystal();

  // methods

void ConstructCrystal(); 
void SetCrystalGeometryRotation(G4bool rotate){fCrystalGeometry3Rotate = rotate;};

private: 

  G4int fCrystalColumn; 
  G4int fCrystalRow; 
  G4int fCrystalMultiplet;

  G4bool fIsInAPair;

  //GEGeometryDetectorDB* fDetectorDB;  
  //G4int fCrystalsPerColumn; 
  //G4int fDetectorArrayMode; 
  G4int fPhiSegmentsPerCrystal; 
  G4int fZSegmentsPerCrystal; 

  //G4bool fFlagCrystalTeflon;
  //G4bool fFlagCrystalTrueCoaxial; 

  G4double fCrystalHeight;
  //G4double fCrystalTeflonHeight;
  //G4double fCrystalSiHeight;
  //G4double fRelativeSegmentZPos; 

  G4double fHolderInnerHeight; 

  G4ThreeVector fCrystalPosition; 

  GEGeometryDetectorParameters* fDetectorParameters; 

  G4int fCrystalGeometry; 
  G4int fCrystalType; 
  G4int fCrystalNumber; 

  G4double fCrystalDeadLayerTop; 
  G4double fCrystalDeadLayerBottom; 
  G4double fCrystalPassivation;

  G4int fCrystalGeometry1Number; 
  G4int fCrystalGeometry2Number; 
  G4int fCrystalGeometry3Number;
  G4int fCrystalGeometry4Number;
  G4int fCrystalGeometry5Number;
  G4int fCrystalGeometry11Number;
  G4bool fCrystalGeometry3Rotate;

  G4double fCrystalGeometry3PairGap;

  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fCrystalPassivationLogical; 
  G4LogicalVolume* fCrystalDeadLayerLogical; 
  //G4LogicalVolume* fCrystalSiLogical;
  G4LogicalVolume* fCrystalLogical;


  G4VPhysicalVolume* fCrystalPassivationPhysical; 
  G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  //G4VPhysicalVolume* fCrystalSiUpperPhysical;
  //G4VPhysicalVolume* fCrystalSiLowerPhysical;
  G4VPhysicalVolume* fCrystalPhysical;

  std::vector<GEGeometryCrystalSegment*> fCrystalSegmentContainer; 

};

#endif

