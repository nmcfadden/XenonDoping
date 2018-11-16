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
 * CLASS DECLARATION: GEGeometryElectronics.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   05-10-2007, Added new design for matrix and its counterpart and pogopins and pads, Daniel
 *   10-15-2007, Completly changed Junctionboard and sled design, Daniel 
 *   10-16-2007  Added Cablechain and Chain, Daniel
 *   10-17-2007  Added cable to the outside world, Daniel
 *   07-09-2008  changes due to the phase-1 cable that has been added, Jens
 *   07-29-2008 (jens)
 *              - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *                --> removed hv connection cable (hv-vertical cable has been extended)
 */

// --------------------------------------------------------------------------- 

#ifndef _GEGEOMETRYELECTRONICS_HH 
#define _GEGEOMETRYELECTRONICS_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"

class GEGeometryDetectorDB;
class GEGeometryDetectorParameters;
class GEGeometryStringStuff;

// --------------------------------------------------------------------------- 

class GEGeometryElectronics
{

public:

  /// Default constructor
  GEGeometryElectronics(G4int col, GEGeometryDetectorDB* detectordb); 

  /// Destructor
  ~GEGeometryElectronics();

  // methods

  G4ThreeVector GetBoardPosition(); 
  G4ThreeVector GetBoxPosition(); 
  G4ThreeVector GetLeftBarPosition(); 
  G4ThreeVector GetRightBarPosition(); 
  G4ThreeVector GetPosition_ElectronicsPhaseIHorizontalBar();
  G4ThreeVector GetCablePosition(); 
  G4ThreeVector GetJunctionSledPosition();
  G4ThreeVector GetKaptonPreAmpsPinsCablePosition();
  G4ThreeVector GetJunctionSledPinsPosition();
  G4ThreeVector GetJunctionSledSpacerPosition();
  G4ThreeVector GetJunctionSledCablesPosition();

  G4ThreeVector GetCableChainPosition();
  G4ThreeVector GetChainPosition();

  G4ThreeVector GetCableConnectionPosition();

  void ConstructElectronics(); 

private: 

  GEGeometryDetectorDB* fDetectorDB;

  G4int fColumnNumber; 
  G4int fColumnGeom1Number; 
  G4int fColumnGeom2Number; 
  //G4int fDetectorArrayMode; 

  G4double fElectronicsBoardHeight; 
  G4double fElectronicsBoxHeight; 
  G4double fElectronicsSideBarHeight; 
  G4double fElectronicsSideBarZPosition;
  G4double fElectronicsSideBarXOffset;
  G4double fElectronicsCableHeight; 
  G4double fHolderSideBarHeight; 
  G4double fHolderSideBarRadius; 
  G4double fElectronicsPhaseIHorizontalBar_InnerHeight;

  G4double fElectronicsKaptonPreAmpsPinsCableHeight;

  G4double fElectronicsJunctionSledHeight;
  G4double fElectronicsJunctionSledPinsHeight;
  G4double fElectronicsJunctionSledSpacerHeight;
  G4double fElectronicsJunctionSledCablesHeight;

  G4double fElectronicsCableChainDistance;
  G4double fElectronicsCableChainHeight;

  G4double fElectronicsChainRadius;
  G4double fElectronicsChainHeight;

  G4double fKaptonCableLength;
  G4double fKaptonCableThickness;

  G4double fSegmentStartingAngleDataBase;

  G4int fCrystalType; 
  G4int fCrystalGeometry; 
  GEGeometryDetectorParameters* fDetectorParameters; 

  GEGeometryStringStuff* fStringStuff;


  G4ThreeVector fCrystalPosition; 

  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fElectronicsBoxLogical; 
  G4LogicalVolume* fElectronicsSideBarLogical; 
  G4LogicalVolume* fElectronicsCableLogical; 
  G4LogicalVolume* fElectronicsJunctionBoardLogical;
  G4LogicalVolume* fElectronicsJunctionSledLogical;
  G4LogicalVolume* fElectronicsKaptonPreAmpsPinsCableLogical;
  G4LogicalVolume* fElectronicsJunctionSledPinsLogical;
  G4LogicalVolume* fElectronicsJunctionSledSpacerLogical;
  G4LogicalVolume* fElectronicsJunctionSledCablesLogical;
  G4LogicalVolume* fElectronicsCableChainLogical;
  G4LogicalVolume* fElectronicsChainLogical;
  G4LogicalVolume* fKaptonConnectionCableLogical;
  G4LogicalVolume* fCopperLinesConnectionLogical;
  G4LogicalVolume* fElectronicsPhaseIHorizontalBarLogical;


  G4VPhysicalVolume* fElectronicsBoxPhysical; 
  G4VPhysicalVolume* fElectronicsLeftBarPhysical; 
  G4VPhysicalVolume* fElectronicsRightBarPhysical; 
  G4VPhysicalVolume* fElectronicsCablePhysical; 
  G4VPhysicalVolume* fElectronicsJunctionBoardPhysical;
  //G4VPhysicalVolume* fElectronicsKaptonPreAmpsPinsCablePhysical;
  G4VPhysicalVolume* fElectronicsJunctionSledPhysical;
  G4VPhysicalVolume* fElectronicsJunctionSledPinsPhysical;
  G4VPhysicalVolume* fElectronicsJunctionSledSpacerPhysical;
  G4VPhysicalVolume* fElectronicsJunctionSledCablesPhysical;
  G4VPhysicalVolume* fElectronicsCableChainPhysical;
  G4VPhysicalVolume* fElectronicsChainPhysical;
  G4VPhysicalVolume* fKaptonConnectionCablePhysical; 
  G4VPhysicalVolume* fCopperLinesConnectionPhysical;
  G4VPhysicalVolume* fElectronicsPhaseIHorizontalBarPhysical;

};

#endif

