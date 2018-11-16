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
 * CLASS DECLARATION:  GEGeometryCable.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION: MM-DD-YYYY
 *
 * 10-09-2007, Added aluminum bondpads on detector, Daniel
 * 09-07-2008, Added phase-1 cables, Jens
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *            - removed hv connection cable (hv-vertical cable has been extended)
 * 04-10-2016 - Added physical volumes of signal and HV cables of phase 2 (status december 2015), Janina
 */

#ifndef _GEGEOMETRYCABLE_HH 
#define _GEGEOMETRYCABLE_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

class GEGeometryDetectorDB;
class GEGeometryStringStuff;
class GEGeometryDetectorParameters;
class GEGeometryHolderPhase2Parts;

// --------------------------------------------------------------------------- 

class GEGeometryCable 
{

public:

  /// Default constructor
  GEGeometryCable(G4int col, G4int row, GEGeometryDetectorDB* theGeometryDB);

  /// Destructor
  ~GEGeometryCable();

  // methods 

  G4double      GetSegmentAngle(G4int seg); 
  G4ThreeVector GetSegmentPosition(G4int segrow); 
  void          ConstructCable(); 

private:
  G4ThreeVector Get_CablePosition_Phase1_Lower();
  G4ThreeVector Get_CablePosition_Phase1_Upper();
  G4ThreeVector Get_CablePosition_Phase1_UL(G4bool isUpper);


private: 

  G4int fCableColumn; 
  G4int fCableRow; 

  G4int fCrystalType; 
  G4int fCrystalTypeL;
  G4int fCrystalNumber;

  GEGeometryDetectorParameters* fDetectorParameters; 
   GEGeometryDetectorParameters* fDetectorParametersL;
  
  G4double fCrystalPairGap;
  G4int fCableGeometry1Number; 
  G4int fCableGeometry2Number; 

  G4int fCableCrystal5SlotNumber;
  G4int fCableCrystal5Number;
  G4int fCableCrystal11Number;	
  G4int fCablePhase2Total;
  G4int fCablePhase2TotalAll; 

  G4double fCrystalHeight; 
  G4double fCrystalHeightPairLowerOne;
  G4double fCrystalOuterRadius; 
  G4double fCrystalDLTopThickness; 
  G4double fCrystalDLBottomThickness; 
  G4double fCrystalPassivationThickness; 
  G4double fHolderInnerHeight; 
  G4double fCableThickness; 
  G4double fCableLoopHeight; 
  G4double fCableStringWidth; 
  G4double fHolderSideBarHeight; 
  G4double fElectronicsBoardHeight; 
  G4double fCrystalDeadLayerTopThickness; 
  G4double fSegmentHeight; 
  G4double fSegmentStartingAngle;
  G4double fSegmentStartingAngleDataBase;
  G4double fRelativeSegmentZPosition; 
  G4double fRelativeSegmentZMiddlePosition ;
  G4double fRelativeSegmentZBottomPosition ;
  
  G4bool fIsPair;

    // Phase1 Cable
  G4double fElectronicsSideBarZPosition;
  G4double fElectronicsSideBarXOffset;
  G4double fRotationAngle_Phase1Cable;

  G4double fPhase1Cable_thickness;
  G4double fElectronicsPhaseIHorizontalBar_YFrameThickness;


  //G4double fBondPadSpanningAngle; 
  //G4double fBondPadAlThickness; 

  G4int fCrystalGeometry; 
  
  G4int fZSegmentsPerCrystal; 
  G4int fPhiSegmentsPerCrystal; 

  G4ThreeVector fCablePosition; 
  G4ThreeVector fCrystalPosition_Uppermost_Crystal;

  G4double fHVCablePhase2Vertical_ZPos;
  G4double fHVCablePhase2Horizontal_relZPos;
  G4double fHVCablePhase2Horizontal_RadiusOffset;
  G4double fHVCable_AngleRotZ;

  G4double fPhase2Cable_thickness;
  G4double fPhase2Cable_LengthOnHolderHV;
  G4double fPhase2Cable_LengthOnHolderSignal;
  GEGeometryHolderPhase2Parts* holderPhase2;
  G4double fZDistanceToEndOfSiliconPlate;
  G4double fZDistanceToEndOfSiliconPlatePair;
  G4double fHVPhase2ToEPlate_LengthZ;
  G4double fHVTopPhase2ToEPlate_LengthZ;
  G4double fHolderPlateRot_angle;
  G4double fHolderPhase2lenghtY;

  GEGeometryStringStuff* fStringStuff;


  G4LogicalVolume* fDetectorMotherVolume; 
  G4LogicalVolume* fCableLogical; 
  G4LogicalVolume* fCableCopperLinesLogical; 
  G4LogicalVolume* fBondPadAlLogical; 

  G4LogicalVolume* fHVCableHorizontalLogical; 
  G4LogicalVolume* fHVCableVerticalLogical; 
  G4LogicalVolume* fPhase1CableLower;
  G4LogicalVolume* fPhase1CableUpper;

  G4LogicalVolume* fPhaseHVCableAtHolderLogical;
  G4LogicalVolume* fPhaseHVCableToEPlateLogical;
  G4LogicalVolume* fPhaseHVCableAtHolderTopLogical;
  G4LogicalVolume* fPhaseHVCableTopToEPlateLogical;
  
  G4LogicalVolume* fPhaseSignalCableAtHolderLogical;
  G4LogicalVolume* fPhaseSignalCableToEPlateLogical;
  G4LogicalVolume* fPhaseSignalCableAtHolderTopLogical;
  G4LogicalVolume* fPhaseSignalCableTopToEPlateLogical; 

  G4VPhysicalVolume* fCablePhysical; 
  G4VPhysicalVolume* fCableCopperLinesPhysical; 
  std::vector<G4VPhysicalVolume*> fBondPadAlContainer; 
};


#endif

