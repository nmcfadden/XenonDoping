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
 * CLASS DECLARATION: GEGeometryHolder.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   10-09-2007, Added cable guides for HV and Signal cables, Daniel 
 *   02-04-2008, phase-1 crystals got individual holders and z positions, Jens S.
 *   23-04-2016, Added holders from top detectors to electronics plate (stars and bars), Janina
 *
 */

#ifndef _GEGEOMETRYHOLDERVERSION2_HH 
#define _GEGEOMETRYHOLDERVERSION2_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh" 

#include "GEGeometryDetectorDB.hh"

// --------------------------------------------------------------------------- 

class GEGeometryHolderPhase1Parts;

class GEGeometryHolder 
{

public:

  /// Default constructor 
  GEGeometryHolder(G4int col, G4int row, GEGeometryDetectorDB* detectordb); 

  /// Ddestructor
  ~GEGeometryHolder();

  // methods

  void ConstructHolder(); 

  G4ThreeVector GetHolderCopperPosition(); 
  G4ThreeVector GetHolderTeflonPosition(); 
  G4ThreeVector GetHolderSignalCablePosition(); 
  G4ThreeVector GetHolderHVCablePosition(); 
  G4ThreeVector GetPhase2HolderStarPosition();
  G4ThreeVector GetPhase2HolderGreatStarPosition();
  G4ThreeVector GetPhase2HolderBartoEPlatePosition();

private: 

  G4int fHolderColumn; 
  G4int fHolderRow; 
  G4int fHolderGeometry1Number; 
  G4int fHolderGeometry2Number; 
  G4int fHolderGeometry3Number; 
  G4int fHolderGeometry4Number; // This is not a new holder. Just a different counter for BEGe type 1 holders
  G4int fHolderGeometry5Number; // This is not a new holder, just a different counter for individual BEGe Phase II holders
  G4int fHolderGeometry5NumberAll;
  G4int fHolderGeometry11Number; // not a new holder, just a different counter for coax PhaseII holder
  G4int fHolderGeometryP2Number; // not a new holder, just a counter of all PhaseII holders
  G4int fHolderGeometryP2NumberAll;
  GEGeometryDetectorParameters* fDetectorParameters; 

  G4int fCrystalGeometry; 
  G4int fCrystalType; 

  G4double fHolderUpperBarHeight; 
  G4double fHolderTeflonTubeHeight; 
  G4double fHolderHVCableOuterHeight; 
  G4double fHolderSignalCableInnerHeight; 
  G4double fHolderInnerHeight; 

  G4double fPhase2HolderStarPos;
  G4double fPhase2HolderStarAngle;
  G4double fPhase2HolderBartoEPlateLength;
  G4double fPhase2HolderStarHeight;
  G4double fHolderCrystalRadius; 
  
  GEGeometryHolderPhase1Parts* fHolderPhase1Parts;
  GEGeometryHolderPhase2Parts* fHolderPhase2Parts;

  G4ThreeVector fHolderPosition; 

  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fHolderCopperLogical; 
  G4LogicalVolume* fHolderTeflonLogical; 

  G4LogicalVolume* fHolderHVCableLogical; 
  G4LogicalVolume* fHolderSignalCableLogical; 
  
  G4LogicalVolume* fPhase2HolderBarsToEPlateLogical;
  G4LogicalVolume* fPhase2HolderStarLogical;
  G4LogicalVolume* fPhase2HolderGreatStarLogical;

  G4VPhysicalVolume* fHolderCopperPhysical; 
  G4VPhysicalVolume* fHolderTeflonPhysical; 

  G4VPhysicalVolume* fHolderHVCablePhysical; 
  G4VPhysicalVolume* fHolderSignalCablePhysical; 

  //G4VPhysicalVolume* fHolderStarPhysical; 

  G4bool fFlagTeflonEncapsulation;
  G4int fTeflonEncapsulationRow;
  G4int fTeflonEncapsulationColumn;

  GEGeometryDetectorDB* fDetectorDB;

};

#endif

