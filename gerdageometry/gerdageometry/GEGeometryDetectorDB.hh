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
 * CLASS DECLARATION:  GEGeometryDetectorDB.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 * 01-21-2005, included more strings & bugfix ("or" is not a very good
 *             variable name), Kevin
 * 01-23-2005, included segmentation of detectors, Kevin
 * 02-08-2005, included calibration source, Kevin
 * 02-14-2005, added Getter/Setter for the position and rotation of the
 *             mu-veto, Luciano & Claudia
 * 03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 * 05-16-2005  Added two Setters to interface the crystal construction with
 *             other classes, Luciano
 * 07-05-2005  Added one boolean flag (+ getter and setter) for the
 *             PhaseI/PhaseII  holders, Luciano
 * 07-05-2005, Added a hole for the neck in the plastic scintillator and a
 *             boolean flag to switch from hole/no-hole,Claudia
 * 11-27-2005, Major changes to the geometry structure, Kevin  *
 * 03-06-2006, Added Cherenkov Veto, Markus
 * 06-05-2006  Cryostat changed from copper to steel. Dimensions taken from a
 *             design provided by K.Tasso, still preliminary. Claudia and Assunta
 * 10-05-2007  Changed electronics layout, added some functions. Daniel
 * 10-09-2007  Added BondPads and CableGuides (HolderSignalCable and HolderHVCable), Daniel
 * 10-15-2007  Completly changed JunctionBoard and Sled to hexagonal shape, Daniel
 * 10-16-2007  Added Cablechain and Chain, Daniel
 * 10-17-2007  Added cable to the outside world, Daniel
 * 12-05-2007  Major changes in cryostat, Jens
 * 12-10-2007 (Jens) Added to cryostat: Torlone spacers in gap and thermal shield in neck
 *            - Changed height and radii of bellow and collar above neck
 *            - a few other minor changes in geometry
 * 02-04-2008 - phase-1 crystals have individual holders now  --> holder object is replaced by
 *              a vector of holders, Jens S.
 * 04-29-2008 - added fExternalRockLogical for the logical volume of Hall A, and the
 *              corresponding getter, Luciano
 * 08-05-2008 - addaped size and position of phase1-string-electronics to the size and position of the phase2-string-electronics
 *            - introduce two schemes for the z-position of the crystals:
 *                o "unshifted"
 *                    --> positions are as before,
 *                        i.e. the upper edge of the crystals in the upper-most row are on one z level
 *                o "center_between_top0_and_bottom2_on_common_zlevel"
 *                    --> new scheme,
 *                        i.e. the centers between top of 0-th and bottom of 2-nd crystal are on one common z level for all strings
 *              (jens)
 * 07-09-2008 - added phase-1 cable, Jens
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *              --> removed hv connection cable (hv-vertical cable has been extended)
 * 08-25-2008 - Enable volumelist for calibration sources, Francis
 * 04-24-2009 - Switch for lock type incl. new xy coordinates for detectors and calibration source in case of comm lock, Francis
 * 14-06-2011 - added possibility of encapsulation, Sabine
 * 05-11-2011 - Added the possibility to instrument the LAr. A design 'enum' was added to handle design selection. (N. Barros)
 * 				Added the corresponding methods to handle the LAr Instrumentation options (construction flag and design selection)
 * 29-02-2012 - Added the copper shroud, Giovanni Benato
 * 26-03-2012 - Added possibility to insert mini-shrouds. At the moment only the Phase I mini-shrouds (i.e. on the 3-strings arms) can be inserted. Giovanni Benato
 * 27-03-2012 - Added possibility to move each calibration source separately. Giovanni Benato
 * 22-05-2012 - Added possibility to move each calibration source separately in all 3 directions. Giovanni Benato
 * 23-05-2012 - Added methods to get the X,Y position of the centre of the lock (Nuno Barros)
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real configuration in GERDA (Giovanni)
 * 18-07-2012 - Added additional options to customize the new phase II lock (DN500). (Nuno Barros)
 * 			  - Added a new type of lock (PhaseII_DN500)
 * 			  -	Originally thought for LAr Instrumentation studies but easily usable by any other study.
 * 14-08-2012 - Added a new type of LAr Instrumentation design (hybrid). (Nuno Barros)
 * 14-08-2012 - Added option to build inner fiber cylinder in hybrid design. (Nuno Barros)
 * 27-10-2012 - Added three new parameters/options to the LAr Instrumentation:
 *                  * Construct an external source to simulate backgrounds from the cable chain
 *                  * Apply a scale to the XUV argon absorption length
 *                  * Apply a scale to the reflectivities of metals
 * 23-12-2012 - Added LARGERBIER APD option (Bjoern Lehnert)
 * 04-10-2016 - Changed possible slots per string from 5 to 7.
 *	      - Added possibility to position detectors individually from matrix file.
 *	      - Added solids and logical volumes for cables (HV and Signal) for Phase2 (final integration December 2015), Janina
 * 04-23-2016 - Added solids and logical volumes for holders from top detectors to electronics plate
 *             - Modified mini-shrouds for Phase2, Janina
 * 28-07-2016, JJ added macro command to set LAr light yield and fiber coverage (Bjoern's code)
 * 21-112017, Added two glued top and bottom rings to mini-shroud and Getters, K.v.Sturm
 */

#ifndef _GEGEOMETRYDETECTORDB_HH
#define _GEGEOMETRYDETECTORDB_HH

// ---------------------------------------------------------------------------

#include <fstream>
#include "globals.hh"
#include "io/MGLogger.hh"
#include "G4LogicalVolume.hh"
#include "GEGeometryDetectorDBMessenger.hh"

#include "GEGeometryDetectorParameters.hh"

// ---------------------------------------------------------------------------

class G4Tubs;
class G4Polycone;
class GEGeometryCryostatParts;
class GEGeometryHolderPhase1Parts;
class GEGeometryHolderPhase2Parts;
class GEGeometryStringStuff;


enum GEGeometryCrystalZShiftScheme_t  {
    CrystalZShiftScheme_unshifted = 0,
    CrystalZShiftScheme_automatic = 1,
    CrystalZShiftScheme_center_between_top0_and_bottom2_on_common_zlevel = 2
};

enum GEGeometryLockType_t {
  LockType_automatic = 0,
  LockType_final = 1,         	// Final lock
  LockType_CommLock2 = 2,     	// Comm lock with 2 detector strings plus calibration source
  LockType_CommLock4 = 3,      	// Comm lock with 4 detector strings plus calibration source
  LockType_PhaseII = 4,			// Preliminary Phase II lock (fibers)
  LockType_PhaseII_DN500 = 5	// Preliminary detector centered N-string array
};

enum GEGeometryLarInstrDesign_t {
	LArInstr_test = 0, // dummy testing design. Simple "fiber optic" cylinder
	LArInstr_PMT = 1,	 // PMT design
	LArInstr_fibers = 2, // fiber design.
	LArInstr_hybrid = 3, // PMT/fiber hybrid design
	LArInstr_APD = 4, // APD design
	LArInstr_hybridAPD = 5 // PMT/fiber hybrid and APD design
};

enum GEGeometryLarInstrAPDDesign_t {
	LArInstAPDDesign_VUV = 0, // windowless VUV sensitive APD
	LArInstAPDDesign_nonVUV = 1,	 // non VUV sensitive APD
	LArInstAPDDesign_SiPM = 2 // SiPM
};

enum GEGeometryLarInstrAPDDeployment_t {
	LArInstAPDDeployment_betweenBEGePairs = 0, // in between each pair of BEGes
	LArInstAPDDeployment_betweenBEGes = 1 // in between all BEGes
};

enum GEGeometryLarInstrMSCoating_t {
	LArInstMSCoating_none = 0, 		// no coating
	LArInstMSCoating_VM2000_TPB = 1, // VM2000 and TPB
	LArInstMSCoating_TPB = 2 // TPB only
};

class GEGeometryDetectorDB
{

private:
  static const G4int nrOfStrings = 19; //19 strings available
  static const G4int nrOfRows = 7; //slots per string
  static const G4int nrOfMultiplets = 2; //how many detectors in a slot

public:

  /// Default constructor
  GEGeometryDetectorDB();

  /// Destructor
  ~GEGeometryDetectorDB();

  // --------------------------------------------------------------------
  // getters
  // --------------------------------------------------------------------
  G4int GetNumberOfStrings() {return nrOfStrings;};
  G4int GetNumberOfRows() {return nrOfRows;};
  G4int GetNumberOfMultiplets() {return nrOfMultiplets;};

  // shielding

  G4double GetShieldingWaterTankHeight()
  { return fShieldingWaterTankHeight; }

  G4double GetShieldingWaterTankThickness()
  { return fShieldingWaterTankThickness; }

  G4double GetShieldingWaterHeight()
  { return fShieldingWaterHeight; }

  G4double GetShieldingAirBufferHeight()
  { return fShieldingAirBufferHeight; }

  G4double GetHallSizeHeight()
  { return fHallSizeHeight;}

  GEGeometryCryostatParts* GetCryostatParts()
  { return fCryostatParts; }

  G4double GetShieldingFootHeight()
  { return fShieldingFootHeight; }

  G4double GetShieldingFootIR()
  { return fShieldingFootIR; }

  G4double GetShieldingCopperShieldingLargeHeight()
  { return fShieldingCopperShieldingLargeHeight; }

  G4double GetShieldingCopperShieldingSmallHeight()
  { return fShieldingCopperShieldingSmallHeight; }


  // clean room

  G4double GetCleanRoomHeight()
  { return fCleanRoomHeight; }

  G4double GetCleanRoomThickness()
  { return fCleanRoomThickness; }

  G4double GetCleanRoomHoleRadius()
  { return fCleanRoomHoleRadius; }

  G4double GetSuperStructureHeight()
  { return fSuperStructureHeight; }

  G4double GetSuperStructureCollarHeight()
  { return fSuperStructureCollarHeight; }

  G4double GetSuperStructureBellowHeight()
  { return fSuperStructureBellowHeight; }

  G4double GetSuperStructureFlangeHeight()
  { return fSuperStructureFlangeHeight; }

  G4double GetSSFlangeBellowToCollarHeight()
  { return fSSFlangeBellowToCollarHeight; }


  // lock

  G4double GetOuterLockHeight()
  { return fOuterLockHeight; }

  G4double GetInnerLockHeight()
  { return fInnerLockHeight; }

  G4double GetLockCopperPlateHeight()
  { return fLockCopperPlateHeight; }

  G4double GetLockSteelPlateHeight()
  { return fLockSteelPlateHeight; }

  G4double GetLockSteelPlateZPosRelToLock()
  { return fLockSteelPlateZPosRelToLock; }

  G4int GetLockType()
  { return fLockType; }

  // LAr Instrumentation
  // lock

  G4bool GetFlagConstructLArInstrumentation()
    { return fFlagLArInstrumentation; }

  G4int GetLArInstrumentationDesign()
  { return fLArInstDesign; }

  G4bool GetLArInstHybridInnerFiber()
  {return fLArInstHybridInnerFiber;}

  G4bool GetLArInstConstructExtSource()
  {return fLArInstBuildExtSource;}

  G4double GetLArInstArgonAbsLScale()
  {return fLArInstScaleLArAbsL;}

  G4double GetLArInstArgonLYScale()
  {return fLArInstScaleLArLY;}

  G4double GetLArInstFiberCoverage()
  {return fLArInstFiberCoverage;}

  G4double GetLArInstTopPMTscaleFact()
  {return fLArInstTopPMTscaleFact;}

  G4double GetLArInstMetalsReflScale()
  {return fLArInstScaleMetalRefl;}

  G4int GetLArInstrumentationAPDDesign()
  {   MGLog(trace) << fLArInstAPDDesign << endlog;
  	  return fLArInstAPDDesign; }

  G4int GetLArInstrumentationAPDDeployment()
  {   MGLog(trace) << fLArInstAPDDeployment << endlog;
  	  return fLArInstAPDDeployment; }

  G4int GetLArInstrumentationMSCoating()
  {   MGLog(trace) << fLArInstMSCoating << endlog;
  	  return fLArInstMSCoating; }

  G4bool GetLArInstrumentationPhotTrack()
  {return fFlagLArPhotTrack; }

  G4bool GetLArInstHybridSegmentedFiber()
  {return fLArInstHybridSegmentedFiber;}

  G4double GetLArInstHybridSegmentedFiberShroudRotation()
  {return fLArInstHybridSegmentedFiberShroudRotation;}

  // Lock alignment -- Different locks should yield different positions
  // Get the number of flanges with detectors
  G4int GetNFlanges()
  { return fNlocks; }

  // Returns the position of the center of the
  // Ge array for an indexed lock
  G4ThreeVector GetStringDetectorCenter(G4int index);

  // flags

  G4bool   GetFlagConstructDetector()
  { return fFlagConstructDetector; }

  G4bool   GetFlagConstructCrystals()
  { return fFlagConstructCrystals; }

  G4bool   GetFlagConstructSupport()
  { return fFlagConstructSupport; }

  G4bool   GetFlagConstructCabling()
  { return fFlagConstructCabling; }

  G4bool   GetFlagConstructShielding()
  { return fFlagConstructShielding; }

  G4bool   GetFlagConstructCalibrationSource()
  { return fFlagConstructCalibrationSource; }

  G4bool   GetFlagConstructCalibrationSourceCuCap()
  { return fFlagConstructCalibrationSourceCuCap; }

  G4bool   GetFlagConstructCollimatedCalibrationSource()
  { return fFlagConstructCollimatedCalibrationSource; }

  G4bool   GetFlagConstructCopperEncapsulation()
  { return fFlagConstructCopperEncapsulation; }

  G4bool   GetFlagConstructTeflonEncapsulation()
  { return fFlagConstructTeflonEncapsulation; }

  G4bool GetFlagConstructMiniShroud()
  { return fFlagConstructMiniShroud; }

  G4bool GetFlagAdaptMinishroudToDetctorString()
  { return fFlagAdaptMinishroudToDetctorString; }


  G4bool GetFlagReverseNonTrueCoaxial()
  { return fFlagReverseNonTrueCoaxial; }

  G4bool   GetFlagCryoLiquid()
  { return fFlagCryoLiquid; }


  // crystal type

  G4int GetCrystalType(G4int col, G4int row,G4int pair=0)
  { return fArrayDetectorTypeMatrix[col][row][pair]; }

  // check if the slot allocates a pair/multiplet
  G4bool IsAPair(G4int col, G4int row)
  {
    return (fArrayDetectorTypeMatrix[col][row][1] > 0);
  }

  // detector parameters

  GEGeometryDetectorParameters* GetDetectorParameters(G4int n)
  { return fDetectorParametersContainer[n]; }

  G4int GetNDetectorTypes()
  { return int(fDetectorParametersContainer.size()); }

  G4String GetCrystalGeometryFilename()
  { return fCrystalGeometryFilename; }

  G4String GetCrystalMatrixFilename()
  { return fCrystalMatrixFilename; }

  G4bool GetFlagCrystalMatrixWithZPosFile()
  { return fFlagCrystalMatrixWithZPosFile; }

  //detector position
  G4double GetCrystalIndZPosition(G4int col, G4int row)
  { return fArrayDetectorIndZPositionMatrix[col][row]; }

  // online counter

  G4int GetCrystalNumber()
  { return fCrystalNumber; }

  G4int GetSegmentNumber()
  { return fSegmentNumber; }

  G4int GetCrystalGeometry1Number()
  { return fCrystalGeometry1Number; }

  G4int GetCrystalGeometry2Number()
  { return fCrystalGeometry2Number; }

  G4int GetCrystalGeometry3Number()
  { return fCrystalGeometry3Number; }

  G4int GetCrystalGeometry4Number()
  { return fCrystalGeometry4Number; }

  G4int GetCrystalGeometry5Number()
  { return fCrystalGeometry5Number; }

  G4int GetCrystalGeometry5SlotNumber()
  { return fCrystalGeometry5SlotNumber; }

  G4int GetCrystalGeometry11Number()
  { return fCrystalGeometry11Number; }

  G4double GetCrystalPairGap()
  { return fCrystalPairGap; }

  void SetCrystalPairGap(G4double gap)
  { fCrystalPairGap = gap; }

  G4int GetColumnNumber()
  { return fColumnNumber; }

  G4int GetColumnGeom1Number()
  { return fColumnGeom1Number; }

  G4int GetColumnGeom2Number()
  { return fColumnGeom2Number; }

  G4int GetCalibrationSourceCounter()
  { return fCalibrationSourceCounter; }

  G4int GetCollimatedCalibrationSourceCounter()
  { return fCollimatedCalibrationSourceCounter; }

  // position

  G4ThreeVector GetCrystalPosition(G4int col, G4int row,
                                   GEGeometryCrystalZShiftScheme_t fScheme=CrystalZShiftScheme_automatic,
                                   GEGeometryLockType_t fLockType=LockType_automatic);

  G4double GetRelativeSegmentZPosition(GEGeometryDetectorParameters* detectorparameters, G4int row);

  G4double GetArrayDepth()
  { return fArrayDepth; }


  G4double GetColumnDistanceR() {
	  return fColumnDistanceR;
  }

  void SetColumnDistanceR(G4double distance) {
	  fColumnDistanceR = distance;
  }

  G4double GetDN500StringDistance()
  { return fDN500StringDistance; }

  void SetDN500StringDistance(G4double distance)
  { fDN500StringDistance = distance; }

  G4int GetDN500NStrings() {
	  return fDN500NStrings;
  }

  void SetDN500NStrings(G4int ncol) {
	  fDN500NStrings = ncol;
  }

  G4double GetPhase1RadiusPos4()
  { return fPhase1RadiusPos4; }

  G4double GetPhase1RadiusPos5()
  { return fPhase1RadiusPos5; }

  G4double GetSegmentationOffsetAngle()
  { return fSegmentationOffsetAngle; }

  // cable parameters

  G4double GetHVCableThickness()
  { return fHVCableThickness; }

  G4double GetCableThickness()
  { return fCableThickness; }

  G4double GetCableLoopHeight()
  { return fCableLoopHeight; }

  G4double GetCableStringWidth()
  { return fCableStringWidth; }

  G4double GetCopperLinesConnectionLength()
  { return fCopperLinesConnectionLength; }

  G4double GetKaptonConnectionCableLenght()
  { return fKaptonConnectionCableLength; }

  G4double GetCopperLinesConnectionWidth()
  { return fCopperLinesConnectionWidth; }

  G4double GetKaptonConnectionCableWidth()
  { return fKaptonConnectionCableWidth; }

  G4double GetCopperLinesConnectionThickness()
  { return fCopperLinesConnectionThickness; }

  G4double GetKaptonConnectionCableThickness()
  { return fKaptonConnectionCableThickness; }

  G4double GetPhase1Cable_thickness()
  { return fPhase1Cable_thickness; }

  G4double GetPhase2Cable_thickness()
  {return fPhase2Cable_Thickness;}

  G4double GetPhase2Cable_LengthOnHolderHV()
  {return fPhase2Cable_LengthOnHolderHV;}

  G4double GetPhase2Cable_LengthOnHolderSignal()
  {return fPhase2Cable_LengthOnHolderSignal;}

  G4double GetPhase2Cable_WidthToEPlate()
  {return fPhase2Cable_WidthFromHolderToElectronicsPlate;}

  G4double GetHVPhase2ToEPlate_LengthZ(G4int col, G4int row)
  { return fHVPhase2ToEPlate_LengthZ[col][row];}

  G4double GetHVTopPhase2ToEPlate_LengthZ(G4int col, G4int row)
  { return fHVTopPhase2ToEPlate_LengthZ[col][row];}

  G4double GetAngleForPhase2CablePosition(G4int col);


  // holder parameters

  G4double GetDistanceHolderToHolder()
  { return fDistanceHolderToHolder; }

  G4double GetHolderUpperBarHeight()
  { return fHolderUpperBarHeight; }

  G4double GetHolderLowerBarHeight()
  { return fHolderLowerBarHeight; }

  G4double GetHolderLowerTrapezoidHeight()
  { return fHolderLowerTrapezoidHeight; }

  G4double GetHolderSideBarHeight()
  { return fHolderSideBarHeight; }

  G4double GetHolderSideBarRadius()
  { return fHolderSideBarRadius; }

  G4double GetHolderTeflonTubeHeight()
  { return fHolderTeflonTubeHeight; }

  G4double GetHolderInnerHeight()
  { return fHolderInnerHeight; }

  G4double GetHolderSignalCableInnerHeight()
  { return fHolderSignalCableInnerHeight; }

  G4double GetHolderSignalCableOuterHeight()
  { return fHolderSignalCableOuterHeight; }

  G4double GetHolderHVCableOuterHeight()
  { return fHolderHVCableOuterHeight; }

  G4double GetHolderHVCableInnerHeight()
  { return fHolderSignalCableInnerHeight; }

  G4double GetHolderStar_SpacerHeight()
  { return fHolderStar_SpacerHeight; }

  G4double GetPhase2HolderStarPlacement()
  { return fPhase2HolderStarPlac; }

  G4double GetPhase2HolderBartoEPlateLength()
  { return fPhase2HolderTubsHeight; }

  G4double GetPhase2HolderStarHeight()
  { return fPhase2HolderStarLengthZ;}

  G4double GetHolderBarRadiusPhase2()
  { return fHolderBarRadiusPhase2;}

  G4double MatchDetectorIDwithHolderBarsPhase2(G4int detectorID, G4int detectorGeometry);

  GEGeometryHolderPhase1Parts* GetHolderPhase1Parts(G4int detectorID);

  GEGeometryHolderPhase2Parts* GetHolderPhase2Parts(G4int detectorID);

  // bond pad and wire parameters

  G4double GetBondPadSpanningAngle()
  { return fBondPadSpanningAngle; }

  G4double GetBondPadAlThickness()
  { return fBondPadAlThickness; }



  // electronics parameters


  inline G4double GetElectronicsPhaseIHorizontalBar_InnerHeight()
  { return fElectronicsPhaseIHorizontalBar_InnerHeight; }

  inline G4double GetElectronicsPhaseIHorizontalBar_YFrameThickness()
  { return fElectronicsPhaseIHorizontalBar_YFrameThickness; }

  G4double GetElectronicsDistanceFromCrystal()
  { return fElectronicsDistanceFromCrystal; }

  G4double GetElectronicsDistanceBoardToBoard()
  { return fElectronicsDistanceBoardToBoard; }

  G4double GetElectronicsBoardGap()
  { return fElectronicsBoardGap; }

  G4double GetElectronicsBoardRadius()
  { return fElectronicsBoardRadius; }

  G4double GetElectronicsBoardHeight()
  { return fElectronicsBoardHeight; }

  G4double GetElectronicsBoxLength()
  { return fElectronicsBoxLength; }

  G4double GetElectronicsBoxHeight()
  { return fElectronicsBoxHeight; }

  G4double GetElectronicsSideBar_MinimumHeight()
  { return fElectronicsSideBar_MinimumHeight; }

  G4double GetElectronicsKaptonPreAmpsPinsCableHeight()
  { return fElectronicsKaptonPreAmpsPinsCableHeight; }

  G4double GetElectronicsJunctionSledHeight()
  { return fElectronicsJunctionSledHeight; }

  G4double GetElectronicsJunctionSledPinsHeight()
  { return fElectronicsJunctionSledPinsHeight; }

  G4double GetElectronicsJunctionSledSpacerHeight()
  { return fElectronicsJunctionSledSpacerHeight; }

  G4double GetElectronicsJunctionSledCablesHeight()
  { return fElectronicsJunctionSledCablesHeight; }

  GEGeometryStringStuff* GetStringStuff(G4int column)
  { return fStringStuffContainer[column]; }


  //cables + cablechain + chain above Junctionsled

  G4double GetElectronicsCableChainHeight()
  { return fElectronicsCableChainHeight; }

  G4double GetElectronicsCableChainDistance()
  { return fElectronicsCableChainDistance; }

  G4double GetElectronicsChainHeight()
  { return fElectronicsChainHeight; }

  G4double GetElectronicsChainRadius()
  { return fElectronicsChainRadius; }

  G4double GetElectronicsCableHeight()
  { return fElectronicsCableHeight; }

  // scintillator parameters

  G4double GetScintillatorHeight()
  { return fScintillatorHeight; }

  G4double GetScintillatorLengthX()
  { return fScintillatorLengthX; }

  G4double GetScintillatorLengthY()
  { return fScintillatorLengthY; }

  G4double GetScintillatorPosX()
  {return fScintillatorPosX; }

  G4double GetScintillatorPosY()
  {return fScintillatorPosY; }

  G4double GetScintillatorPosZ()
  {return fScintillatorPosZ; }

  G4double GetScintillatorAngleZ()
  {return fScintillatorAngleZ; }

  G4double GetScintillatorHoleOuterRadius()
  { return fScintillatorHoleOuterRadius; }

  G4bool GetScintillatorHole()
  { return fScintillatorHole; }

  // calibration source parameters

  G4double GetCalibrationSourcePositionX( G4int n )
  {
    if( n == 0 ){
    return fCalibrationSourcePositionX_S1;
    }else  if( n == 1 ){
    return fCalibrationSourcePositionX_S2;
    }else  if( n == 2 ){
    return fCalibrationSourcePositionX_S3;
    }else{
      MGLog(warning) <<
	"GEGeometryDetectorDB::GetCalibrationSourcePositionX()";
      MGLog(warning) << "Invalid choice" << endlog;
      return 0.;
      ;//return fCalibrationSourcePositionX;
    }
  }

  G4double GetCalibrationSourcePositionY( G4int n )
  {
    if( n == 0 ){
    return fCalibrationSourcePositionY_S1;
    }else  if( n == 1 ){
    return fCalibrationSourcePositionY_S2;
    }else  if( n == 2 ){
    return fCalibrationSourcePositionY_S3;
    }else{
      MGLog(warning) <<
	"GEGeometryDetectorDB::GetCalibrationSourcePositionX()";
      MGLog(warning) << "Invalid choice" << endlog;
      return 0.;
      ;//return fCalibrationSourcePositionY;
    }
  }

  G4double GetCalibrationSourcePositionZ( G4int n )
  {
    if( n == 0 ){
    return fCalibrationSourcePositionZ_S1;
    }else  if( n == 1 ){
    return fCalibrationSourcePositionZ_S2;
    }else  if( n == 2 ){
    return fCalibrationSourcePositionZ_S3;
    }else{
    return fCalibrationSourcePositionZ;
    }
  }

  G4double GetCalibrationSourcePositionZ()
  { return fCalibrationSourcePositionZ; }

  G4double GetCalibrationSourcePositionX_S1()
  { return fCalibrationSourcePositionX_S1; }

  G4double GetCalibrationSourcePositionX_S2()
  { return fCalibrationSourcePositionX_S2; }

  G4double GetCalibrationSourcePositionX_S3()
  { return fCalibrationSourcePositionX_S3; }

  G4double GetCalibrationSourcePositionY_S1()
  { return fCalibrationSourcePositionY_S1; }

  G4double GetCalibrationSourcePositionY_S2()
  { return fCalibrationSourcePositionY_S2; }

  G4double GetCalibrationSourcePositionY_S3()
  { return fCalibrationSourcePositionY_S3; }

  G4double GetCalibrationSourcePositionZ_S1()
  { return fCalibrationSourcePositionZ_S1; }

  G4double GetCalibrationSourcePositionZ_S2()
  { return fCalibrationSourcePositionZ_S2; }

  G4double GetCalibrationSourcePositionZ_S3()
  { return fCalibrationSourcePositionZ_S3; }

  G4double GetCalibrationSourcePhase2DN500Radius()
  { return fCalibrationSourcePhase2DN500Radius; }

  G4double GetCalibrationSourcePhase2DN500OffsetX()
  { return fCalibrationSourcePhase2DN500OffsetX; }

  G4double GetCalibrationSourcePhase2DN500OffsetY()
  { return fCalibrationSourcePhase2DN500OffsetY; }

  G4double GetCalibrationSourcePhase2DN500AngleOffset( G4int n )
  {
    if( n == 0 ){
    return fCalibrationSourcePhase2DN500AngleOffset1;
    }else  if( n == 1 ){
    return fCalibrationSourcePhase2DN500AngleOffset2;
    }else  if( n == 2 ){
    return fCalibrationSourcePhase2DN500AngleOffset3;
    }else{
      MGLog(error) << "Invalid choice in GetCollimatedCalibrationSourcePositionX()" << endlog;
      return 0;
      ;
   }


  }

  G4double GetCalibrationSourcePhase2CoordTrafo()
  { return fCalibrationSourcePhase2CoordTrafo; }

  G4double GetCalibrationCylinderHeight()
  { return fCalibrationCylinderHeight; }

  // collimated calibration source parameters

  G4double GetCollimatedCalibrationSourcePositionX( G4int n )
  {
    if( n == 0 ){
    return fCollimatedCalibrationSourcePositionX_S1;
    }else  if( n == 1 ){
    return fCollimatedCalibrationSourcePositionX_S2;
    }else  if( n == 2 ){
    return fCollimatedCalibrationSourcePositionX_S3;
    }else{
      MGLog(error) << "Invalid choice in GetCollimatedCalibrationSourcePositionX()" << endlog;
      return 0;
      ;//return fCollimatedCalibrationSourcePositionX;
    }
  }

  G4double GetCollimatedCalibrationSourcePositionY( G4int n )
  {
    if( n == 0 ){
    return fCollimatedCalibrationSourcePositionY_S1;
    }else  if( n == 1 ){
    return fCollimatedCalibrationSourcePositionY_S2;
    }else  if( n == 2 ){
    return fCollimatedCalibrationSourcePositionY_S3;
    }else{
      MGLog(error) << "Invalid choice in GetCollimatedCalibrationSourcePositionY()" << endlog;
      return 0;
      ;//return fCollimatedCalibrationSourcePositionY;
    }
  }

  G4double GetCollimatedCalibrationSourcePositionZ( G4int n )
  {
    if( n == 0 ){
    return fCollimatedCalibrationSourcePositionZ_S1;
    }else  if( n == 1 ){
    return fCollimatedCalibrationSourcePositionZ_S2;
    }else  if( n == 2 ){
    return fCollimatedCalibrationSourcePositionZ_S3;
    }else{
    return fCollimatedCalibrationSourcePositionZ;
    }
  }

  G4double GetCollimatedCalibrationCylinderHeight()
  { return fCollimatedCalibrationCylinderHeight; }

  G4double GetCollimatedCalibrationCapHeight()
  { return fCollimatedCalibrationCapHeight; }

  G4double GetCollimatedCalibrationCapsuleHeight()
  { return fCollimatedCalibrationCapsuleHeight; }

  G4double GetCollimatedCalibrationSourceHeight()
  { return fCollimatedCalibrationSourceHeight; }

  G4double GetCalibrationCollimationDistance()
  { return fCalibrationCollimationDistance; }

  // copper encapsulation parameters

  G4double GetCopperEncapsulationOuterRadius()
  { return fCopperEncapsulationOuterRadius; }

  G4double GetCopperEncapsulationHeight()
  { return fCopperEncapsulationHeight; }

  G4double GetCopperEncapsulationThickness()
  { return fCopperEncapsulationThickness; }

  G4double GetCopperEncapsulationRadiusHV()
  { return fCopperEncapsulationRadiusHV; }

  G4double GetCopperEncapsulationRadiusSignal()
  { return fCopperEncapsulationRadiusSignal; }

  G4int GetCopperEncapsulationRow()
  { return fCopperEncapsulationRow; }

  G4int GetCopperEncapsulationColumn()
  {return fCopperEncapsulationColumn; }

  // teflon encapsulation parameters

  G4double GetTeflonEncapsulationOuterRadius()
  { return fTeflonEncapsulationOuterRadius; }

  G4double GetTeflonEncapsulationHeight()
  { return fTeflonEncapsulationHeight; }

  G4double GetTeflonEncapsulationThickness()
  { return fTeflonEncapsulationThickness; }

  G4double GetTeflonEncapsulationRadiusHV()
  { return fTeflonEncapsulationRadiusHV; }

  G4double GetTeflonEncapsulationRadiusSignal()
  { return fTeflonEncapsulationRadiusSignal; }

  G4double GetTeflonEncapsulationCopperOuterRadius()
  { return fTeflonEncapsulationCopperOuterRadius; }

  G4double GetTeflonEncapsulationCopperHeight()
  { return fTeflonEncapsulationCopperHeight; }

  G4double GetTeflonEncapsulationCopperThickness()
  { return fTeflonEncapsulationCopperThickness; }

  G4int GetTeflonEncapsulationRow()
  { return fTeflonEncapsulationRow; }

  G4int GetTeflonEncapsulationColumn()
  {return fTeflonEncapsulationColumn; }

  // mini-shroud parameters

  G4double GetMiniShroudOuterRadius()
  { return fMiniShroudOuterRadiusPhaseI; }

  G4double GetMiniShroudHeight()
  { return fMiniShroudHeightPhaseI; }

  G4double GetMiniShroudHeightPhaseIINylon()
  { return fMiniShroudHeightPhaseIINylon; }

  G4double GetMiniShroudSideThickness()
  { return fMiniShroudSideThickness; }

  G4double GetMiniShroudBaseThickness()
  { return fMiniShroudBaseThickness; }

  G4double GetMiniShroudTopGluedRingHeight()
  { return fMiniShroudTopGluedRingHeight; }

  G4double GetMiniShroudBottomGluedRingHeight()
  { return fMiniShroudBottomGluedRingHeight; }

  G4String GetMiniShroudMaterial()
  { return fMiniShroudMaterial; }

  std::vector<G4int>* GetMiniShroudRow()
  { return fMiniShroudRow; }

  std::vector<G4int>* GetMiniShroudColumn()
  {return fMiniShroudColumn; }

  std::vector<G4ThreeVector>* GetMiniShroudPosition()
  { return fMiniShroudPosition; }

  std::vector<G4ThreeVector>* GetMiniShroudPhase2TubPosition()
  { return fMiniShroudPositionPhase2Tub; }

  std::vector<G4ThreeVector>* GetMiniShroudPhase2BottomPosition()
  { return fMiniShroudPositionPhase2Bottom; }

  std::vector<G4ThreeVector>* GetMiniShroudPhase2TopPosition()
  { return fMiniShroudPositionPhase2Top; }

  std::vector<G4ThreeVector>* GetMiniShroudPhase2TopGluedRingPosition()
  { return fMiniShroudPositionPhase2TopGluedRing; }

  std::vector<G4ThreeVector>* GetMiniShroudPhase2BottomGluedRingPosition()
  { return fMiniShroudPositionPhase2BottomGluedRing; }

  std::vector<double>* GetMiniShroudRadiusVector()
   { return fMiniShroudRadiusVector; }

  std::vector<double>* GetMiniShroudHeightVector()
   { return fMiniShroudHeightVector; }

  std::vector<double>* GetMiniShroudHoleRadiusVector()
   { return fMiniShroudHoleRadiusVector; }

  G4bool GetFlagForMSMaterial()
  { return fFlagMSMaterialCheck;}

  std::vector<double>* GetMiniInDetHeightVector()
   { return fMiniInDetHeightVector; }

  // Cherenkov veto

  G4double GetPhotocathodeInnerRadius()
  { return fPhotocathodeInnerRadius; }

  G4double GetPhotocathodeOuterRadius()
  { return fPhotocathodeOuterRadius; }

  G4double GetPMTInnerRadius()
  { return fPMTInnerRadius; }

  G4double GetPMTOuterRadius()
  { return fPMTOuterRadius; }

  G4double GetPMTSteelConeThickness()
  { return fPMTSteelConeThickness; }

  G4double GetPMTSteelConeHeight()
  { return fPMTSteelConeHeight; }

  G4double GetPMTSteelConeUpperRmin()
  { return fPMTSteelConeUpperRmin; }

  G4double GetPMTSteelConeUpperRmax()
  { return fPMTSteelConeUpperRmax; }

  G4double GetPMTSteelConeLowerRmin()
  { return fPMTSteelConeLowerRmin; }

  G4double GetPMTSteelConeLowerRmax()
  { return fPMTSteelConeLowerRmax; }

  G4double GetPMTSteelBottomHeight()
  { return fPMTSteelBottomHeight; }

  G4double GetReflectionFoilThickness()
  { return fReflectionFoilThickness; }

  G4int GetSelectPMTDistribution()
  { return fSelectPMTDistribution; }

  // PMT cable tray

  G4double GetPMTCableTrayThickness()
  { return fPMTCableTrayThickness; }

  G4double GetPMTVerticalCableTrayHeight()
  { return fPMTVerticalCableTrayHeight; }

  G4double GetPMTHorizontalCableTrayWidth()
  { return fPMTHorizontalCableTrayWidth; }


  // --------------------------------------------------------------------
  // logical volumes
  // --------------------------------------------------------------------

  // mother volumes

  inline G4LogicalVolume* GetDetectorMotherVolume()
  { return fDetectorMotherVolume; }

  // shielding

  inline G4LogicalVolume* GetAirBoxLogical()
  { return fAirBoxLogical; }

  inline G4LogicalVolume* GetExternalRockLogical()
  {return fExternalRockLogical; }

  inline G4LogicalVolume* GetWaterTankLogical()
  { return fWaterTankLogical; }

  inline G4LogicalVolume* GetWaterLogical()
  { return fWaterLogical; }

  inline G4LogicalVolume* GetAirBufferLogical()
  { return fAirBufferLogical; }

  inline G4LogicalVolume* GetFootLogical()
  { return fFootLogical; }

  inline G4LogicalVolume* GetCopperShieldingLargeLogical()
  { return fCopperShieldingLargeLogical; }

  inline G4LogicalVolume* GetCopperShieldingSmallLogical()
  { return fCopperShieldingSmallLogical; }

  // clean room

  inline G4LogicalVolume* GetCleanRoomLogical()
  { return fCleanRoomLogical; }

  inline G4LogicalVolume* GetSuperStructureCollarLogical()
  { return fSuperStructureCollarLogical; }

  inline G4LogicalVolume* GetSuperStructureBellowLogical()
  { return fSuperStructureBellowLogical; }

  inline G4LogicalVolume* GetSuperStructureFlangeLogical()
  { return fSuperStructureFlangeLogical; }

  inline G4LogicalVolume* GetSSFlangeBellowToCollarLogical()
  { return fSSFlangeBellowToCollarLogical; }

  inline G4LogicalVolume* GetSuperStructureBeamGridLogical()
  { return fSuperStructureBeamGridLogical; }

  // lock

  inline G4LogicalVolume* GetOuterLockLogical()
  { return fOuterLockLogical; }

  inline G4LogicalVolume* GetInnerLockLogical()
  { return fInnerLockLogical; }

  inline G4LogicalVolume* GetLockCopperPlateLogical()
  { return fLockCopperPlateLogical; }

  inline G4LogicalVolume* GetLockSteelPlateLogical()
  { return fLockSteelPlateLogical; }

  // crystal volumes

  inline G4LogicalVolume* GetCrystalPassivationLogical(G4int i)
  { return fCrystalPassivationLogicalContainer[i]; }

  inline G4LogicalVolume* GetCrystalDeadLayerLogical(G4int i)
  { return fCrystalDeadLayerLogicalContainer[i]; }

  inline G4LogicalVolume* GetCrystalSegmentLogical(G4int i)
  { return fCrystalSegmentLogicalContainer[i]; }

  // holder

  inline G4LogicalVolume* GetHolderCopperLogical()
  { return fHolderCopperLogical; }

  inline G4LogicalVolume* GetHolderTeflonLogical()
  { return fHolderTeflonLogical; }

  inline G4LogicalVolume* GetHolderSignalCableLogical()
  { return fHolderSignalCableLogical; }

  inline G4LogicalVolume* GetHolderHVCableLogical()
  { return fHolderHVCableLogical; }

   inline G4LogicalVolume* GetPhase2HolderStarLogical()
  { return fPhase2HolderStarLogical; }

  inline G4LogicalVolume* GetPhase2HolderGreatStarLogical()
  { return fPhase2HolderGreatStarLogical; }

   inline G4LogicalVolume* GetPhase2HolderBarsToEPlateLogical()
  { return fPhase2HolderBarsToEPlateLogical; }

  // cables

  inline G4LogicalVolume* GetCableLogicalContainer(G4int i)
  { return fCableLogicalContainer[i]; }

  inline G4LogicalVolume* GetCableCopperLinesLogicalContainer(G4int i)
  { return fCableCopperLinesLogicalContainer[i]; }

  inline G4LogicalVolume* GetBondPadAlLogicalContainer(G4int i)
  { return fBondPadAlLogicalContainer[i]; }

  inline G4LogicalVolume* GetKaptonConnectionCable(G4int col)
  { return fKaptonConnectionCable[col]; }

  inline G4LogicalVolume* GetCopperLinesConnection(G4int col)
  { return fCopperLinesConnection[col]; }


  // electronics

  inline G4LogicalVolume* GetElectronicsBoxLogical()
  { return fElectronicsBoxLogical; }

  G4LogicalVolume* GetElectronicsSideBarLogical(G4int col);

  inline G4LogicalVolume* GetElectronicsCableLogical()
  { return fElectronicsCableLogical; }


  inline G4LogicalVolume* GetElectronicsJunctionSledLogical()
  { return fElectronicsJunctionSledLogical; }

  inline G4LogicalVolume* GetElectronicsJunctionSledPinsLogical()
  { return fElectronicsJunctionSledPinsLogical; }

  inline G4LogicalVolume* GetElectronicsJunctionSledSpacerLogical()
  { return fElectronicsJunctionSledSpacerLogical; }

  inline G4LogicalVolume* GetElectronicsJunctionSledCablesLogical()
  { return fElectronicsJunctionSledCablesLogical; }

  inline G4LogicalVolume* GetElectronicsJunctionBoardLogical()
  {return fElectronicsJunctionBoardLogical; }

  inline G4LogicalVolume* GetElectronicsPhaseIHorizontalBarLogical()
  {return fElectronicsPhaseIHorizontalBarLogical; }

  // cable from Junctionboard to Pins

  inline G4LogicalVolume* GetElectronicsKaptonPreAmpsPinsCableLogical()
  {return fElectronicsKaptonPreAmpsPinsCableLogical; }


  //cables + chain above Junctionsled

  inline G4LogicalVolume* GetElectronicsCableChainLogical()
  {return fElectronicsCableChainLogical; }

  inline G4LogicalVolume* GetElectronicsChainLogical()
  {return fElectronicsChainLogical; }


  // calibration source

  inline G4LogicalVolume* GetCalibrationCylinderLogical()
  { return fCalibrationCylinderLogical; }

  inline G4LogicalVolume* GetCalibrationSourceInnerLogical()
  { return fCalibrationSourceInnerLogical; }

  inline G4LogicalVolume* GetCalibrationSourceOuterLogical()
  { return fCalibrationSourceOuterLogical; }

  inline G4LogicalVolume* GetCalibrationSourceCylCapLogical()
  { return fCalibrationCylCapLogical; }

  // collimated calibration source

  inline G4LogicalVolume* GetCollimatedCalibrationCylinderLogical()
  { return fCollimatedCalibrationCylinderLogical; }

  inline G4LogicalVolume* GetCollimatedCalibrationSourceInnerLogical()
  { return fCollimatedCalibrationSourceInnerLogical; }

  inline G4LogicalVolume* GetCollimatedCalibrationSourceOuterLogical()
  { return fCollimatedCalibrationSourceOuterLogical; }


  // copper encapsulation

  inline G4LogicalVolume* GetCopperEncapsulationLogical()
  { return fCopperEncapsulationLogical; }

  // teflon encapsulation

  inline G4LogicalVolume* GetTeflonEncapsulationLogical()
  { return fTeflonEncapsulationLogical; }

  inline G4LogicalVolume* GetTeflonEncapsulationCopperLogical()
  { return fTeflonEncapsulationCopperLogical; }

  // mini-shroud

  inline std::vector<G4LogicalVolume*>* GetMiniShroudLogical()
  { return &fMiniShroudLogical; }

  inline std::vector<G4LogicalVolume*>* GetMiniShroudPhase2TubLogical()
  { return &fMiniShroudTubPhase2Logical; }

  inline std::vector<G4LogicalVolume*>* GetMiniShroudPhase2BottomLogical()
  { return &fMiniShroudBottomPhase2Logical; }

  inline std::vector<G4LogicalVolume*>* GetMiniShroudPhase2TopLogical()
  { return &fMiniShroudTopPhase2Logical; }

  // mini-shroud glued parts
  inline std::vector<G4LogicalVolume*>* GetMiniShroudPhase2TopGluedRingLogical()
  { return &fMiniShroudTopGluedRingPhase2Logical; }

  inline std::vector<G4LogicalVolume*>* GetMiniShroudPhase2BottomGluedRingLogical()
  { return &fMiniShroudBottomGluedRingPhase2Logical; }


  // scintillator

  inline G4LogicalVolume* GetScintillatorLogical()
  { return fScintillatorLogical; }

  // photomultiplier

  inline G4LogicalVolume* GetPhotocathodeLogical()
  { return fPhotocathodeLogical; }

  inline G4LogicalVolume* GetPMTSteelConeLogical()
  { return fPMTSteelConeLogical; }

  inline G4LogicalVolume* GetPMTSteelBottomLogical()
  { return fPMTSteelBottomLogical; }

  // VM2000

  inline G4LogicalVolume* GetReflectionFoilWaterTankTubeLogical()
  { return fReflectionFoilWaterTankTubeLogical; }

  inline G4LogicalVolume* GetReflectionFoilWaterTankBottomLogical()
  { return fReflectionFoilWaterTankBottomLogical; }

  inline G4LogicalVolume* GetReflectionFoilCryoFootLogical()
  { return fReflectionFoilCryoFootLogical; }

  inline G4LogicalVolume* GetReflectionFoilCryoFootTopLogical()
  { return fReflectionFoilCryoFootTopLogical; }

  inline G4LogicalVolume* GetReflectionFoilCryoFootBottomLogical()
  { return fReflectionFoilCryoFootBottomLogical; }

   // PMT cable tray

  inline G4LogicalVolume* GetVerticalPMTCableTrayLogical()
  { return fVerticalPMTCableTrayLogical; }

  inline G4LogicalVolume* GetHorizontalPMTCableTrayLogical()
  { return fHorizontalPMTCableTrayLogical; }


  G4double GetZUppermostCrystalSurface() const
  { return fZUppermostCrystalSurface; }

  G4double GetZLowermostCrystalSurface();

  G4int GetColumnWithUppermostCrystalSurface() const
  { return fColumnWithUppermostCrystalSurface; }



  // hv cables
  G4double GetHVCablePhase2Vertical_ZPos(G4int iRow) const
  { return f_HVCablePhase2Vertical_ZPos[iRow]; }
  G4double GetHVCablePhase2Horizontal_relZPos() const
  { return f_HVCablePhase2Horizontal_relZPos; }
  G4double GetHVCablePhase2Horizontal_RadiusOffset() const
  { return f_HVCablePhase2Horizontal_RadiusOffset; }
  G4double GetHVCablePhase2_AnglePosition(G4int row);


  G4LogicalVolume* GetHVCablePhase2VerticalLogical(G4int iRow) const
  { return f_HVCablePhase2VerticalLogical[iRow]; }
  G4LogicalVolume* GetHVCablePhase2HorizontalLogical() const
  { return f_HVCablePhase2HorizontalLogical; }

  //phase2 cables (status december 2015)
  G4LogicalVolume* GetHVCableAtHolderPhase2Logical(G4int col, G4int row) const
  { return fHVCableAtHolderPhase2Logical[col][row];}

  G4LogicalVolume* GetHVCableAtHolderTopPhase2Logical(G4int col, G4int row) const
  { return fHVCableAtHolderTopPhase2Logical[col][row];}

  G4LogicalVolume* GetHVCableToEPlatePhase2Logical(G4int col, G4int row ) const
  { return fHVCableToEPlatePhase2Logical[col][row];}

  G4LogicalVolume* GetHVCableTopToEPlatePhase2Logical(G4int col, G4int row) const
  { return fHVCableTopToEPlatePhase2Logical[col][row];}

  G4LogicalVolume* GetSignalCableAtHolderPhase2Logical(G4int col, G4int row) const
  { return fSignalCableAtHolderPhase2Logical[col][row];}

  G4LogicalVolume* GetSignalCableAtHolderTopPhase2Logical(G4int col, G4int row) const
  { return fSignalCableAtHolderTopPhase2Logical[col][row];}

  G4LogicalVolume* GetSignalCableToEPlatePhase2Logical(G4int col, G4int row ) const
  { return fSignalCableToEPlatePhase2Logical[col][row];}

  G4LogicalVolume* GetSignalCableTopToEPlatePhase2Logical(G4int col, G4int row) const
  { return fSignalCableTopToEPlatePhase2Logical[col][row];}

  // --------------------------------------------------------------------
  // setters
  // --------------------------------------------------------------------

  void SetDetectorMotherVolume(G4LogicalVolume* logicalvolume)
  { fDetectorMotherVolume = logicalvolume; }

  // flags

  void SetFlagConstructDetector(G4bool flag)
  { fFlagConstructDetector = flag; }

  void SetFlagConstructCrystals(G4bool flag)
  { fFlagConstructCrystals = flag; }

  void SetFlagConstructSupport(G4bool flag)
  { fFlagConstructSupport = flag; }

  void SetFlagConstructShielding(G4bool flag)
  { fFlagConstructShielding = flag; }

  void SetFlagConstructCabling(G4bool flag)
  { fFlagConstructCabling = flag; }

  void SetFlagConstructCalibrationSource(G4bool flag)
  { fFlagConstructCalibrationSource = flag; }

  void SetFlagConstructCalibrationSourceCuCap(G4bool flag)
  { fFlagConstructCalibrationSourceCuCap = flag; }

  void SetFlagConstructCollimatedCalibrationSource(G4bool flag)
  { fFlagConstructCollimatedCalibrationSource = flag; }

  void SetFlagConstructCopperEncapsulation(G4bool flag)
  { fFlagConstructCopperEncapsulation = flag; }

  void SetFlagConstructTeflonEncapsulation(G4bool flag)
  { fFlagConstructTeflonEncapsulation = flag; }

  void SetFlagConstructMiniShroud( G4bool flag )
  { fFlagConstructMiniShroud = flag; }

  void SetFlagAdaptMinishroudToDetctorString( G4bool flag )
  { fFlagAdaptMinishroudToDetctorString = flag; }

  void SetFlagReverseNonTrueCoaxial( G4bool flag )
  { fFlagReverseNonTrueCoaxial = flag; }

  void SetFlagCryoLiquid(G4bool flag)
  { fFlagCryoLiquid = flag; }


  // position

  void SetArrayDepth(G4double d)
  { fArrayDepth = d; }

  // crystals

  void SetCrystalGeometryFilename(G4String filename);

  void SetCrystalMatrixFilename(G4String filename);

  void SetFlagCrystalMatrixWithZPosFile( G4bool flag )
  { fFlagCrystalMatrixWithZPosFile = flag; }

  // cable parameters

  void SetCableThickness(G4double t)
  { fCableThickness = t; }

  // holder parameters

  void SetHolderUpperBarHeight(G4double h)
  { fHolderUpperBarHeight = h; }

  void SetHolderLowerBarHeight(G4double h)
  { fHolderLowerBarHeight = h; }

  void SetHolderSideBarHeight(G4double h)
  { fHolderSideBarHeight = h; }

  void SetHolderLowerTrapezoidHeight(G4double h)
  { fHolderLowerTrapezoidHeight = h; }

  // electronics parameters

  void SetElectronicsDistanceFromCrystal(G4double d)
  { fElectronicsDistanceFromCrystal = d; }

  void SetElectronicsDistanceBoardToBoard(G4double d)
  { fElectronicsDistanceBoardToBoard = d; }

  void SetElectronicsBoardGap(G4double gap)
  { fElectronicsBoardGap = gap; }

  void SetElectronicsBoardRadius(G4double r)
  { fElectronicsBoardRadius = r; }

  void SetElectronicsBoardHeight(G4double h)
  { fElectronicsBoardHeight = h; }

  void SetElectronicsBoxLength(G4double l)
  { fElectronicsBoxLength = l; }

  void SetElectronicsBoxHeight(G4double h)
  { fElectronicsBoxHeight = h; }

  void SetElectronicsCableChainDistance(G4double h)
  { fElectronicsCableChainDistance = h; }


  // scintillator paramters

  void SetScintillatorHeight(G4double h)
  { fScintillatorHeight = h; }

  void SetScintillatorLengthX(G4double l)
  { fScintillatorLengthX = l; }

  void SetScintillatorLengthY(G4double l)
  { fScintillatorLengthY = l; }

  void SetScintillatorPosX(G4double v)
  {fScintillatorPosX = v;}

  void SetScintillatorPosY(G4double v)
  {fScintillatorPosY=v;}

  void SetScintillatorPosZ(G4double v)
  {fScintillatorPosZ=v;}

  void SetScintillatorAngleZ(G4double v)
  {fScintillatorAngleZ=v;}

  void SetScintillatorHoleOuterRadius(G4double r)
  {fScintillatorHoleOuterRadius=r;}

  void SetScintillatorHole(G4bool aa)
  {fScintillatorHole = aa;}

  // calibration source parameters

  void SetCalibrationSourcePositionZ(G4double z)
  { fCalibrationSourcePositionZ = z; }

  void SetCalibrationSourcePositionX_S1(G4double x)
  { fCalibrationSourcePositionX_S1 = x; }

  void SetCalibrationSourcePositionX_S2(G4double x)
  { fCalibrationSourcePositionX_S2 = x; }

  void SetCalibrationSourcePositionX_S3(G4double x)
  { fCalibrationSourcePositionX_S3 = x; }

  void SetCalibrationSourcePositionY_S1(G4double y)
  { fCalibrationSourcePositionY_S1 = y; }

  void SetCalibrationSourcePositionY_S2(G4double y)
  { fCalibrationSourcePositionY_S2 = y; }

  void SetCalibrationSourcePositionY_S3(G4double y)
  { fCalibrationSourcePositionY_S3 = y; }

  void SetCalibrationSourcePositionZ_S1(G4double z)
  { fCalibrationSourcePositionZ_S1 = z; }

  void SetCalibrationSourcePositionZ_S2(G4double z)
  { fCalibrationSourcePositionZ_S2 = z; }

  void SetCalibrationSourcePositionZ_S3(G4double z)
  { fCalibrationSourcePositionZ_S3 = z; }

  void SetCalibrationCylinderHeight(G4double h)
  { fCalibrationCylinderHeight = h; }

  // collimated source parameters

  void SetCollimatedCalibrationSourcePositionX_S1(G4double x)
  { fCollimatedCalibrationSourcePositionX_S1 = x; }

  void SetCollimatedCalibrationSourcePositionX_S2(G4double x)
  { fCollimatedCalibrationSourcePositionX_S2 = x; }

  void SetCollimatedCalibrationSourcePositionX_S3(G4double x)
  { fCollimatedCalibrationSourcePositionX_S3 = x; }

  void SetCollimatedCalibrationSourcePositionY_S1(G4double y)
  { fCollimatedCalibrationSourcePositionY_S1 = y; }

  void SetCollimatedCalibrationSourcePositionY_S2(G4double y)
  { fCollimatedCalibrationSourcePositionY_S2 = y; }

  void SetCollimatedCalibrationSourcePositionY_S3(G4double y)
  { fCollimatedCalibrationSourcePositionY_S3 = y; }

  void SetCollimatedCalibrationSourcePositionZ_S1(G4double z)
  { fCollimatedCalibrationSourcePositionZ_S1 = z; }

  void SetCollimatedCalibrationSourcePositionZ_S2(G4double z)
  { fCollimatedCalibrationSourcePositionZ_S2 = z; }

  void SetCollimatedCalibrationSourcePositionZ_S3(G4double z)
  { fCollimatedCalibrationSourcePositionZ_S3 = z; }

  // copper encapsulation parameters

  void SetCopperEncapsulationOuterRadius(G4double r)
  { fCopperEncapsulationOuterRadius = r;}

  void SetCopperEncapsulationHeight(G4double h)
  { fCopperEncapsulationHeight = h;}

  void SetCopperEncapsulationThickness(G4double d)
  { fCopperEncapsulationThickness = d; }

  void SetCopperEncapsulationRadiusHV(G4double r)
  { fCopperEncapsulationRadiusHV = r; }

  void SetCopperEncapsulationRadiusSignal(G4double r)
  { fCopperEncapsulationRadiusSignal = r; }

  void SetCopperEncapsulationRow(G4int row)
  { fCopperEncapsulationRow = row;}

  void SetCopperEncapsulationColumn(G4int column)
  { fCopperEncapsulationColumn = column;}

  // teflon encapsulation parameters

  void SetTeflonEncapsulationOuterRadius(G4double r)
  { fTeflonEncapsulationOuterRadius = r;}

  void SetTeflonEncapsulationHeight(G4double h)
  { fTeflonEncapsulationHeight = h;}

  void SetTeflonEncapsulationThickness(G4double d)
  { fTeflonEncapsulationThickness = d; }

  void SetTeflonEncapsulationRadiusHV(G4double r)
  { fTeflonEncapsulationRadiusHV = r; }

  void SetTeflonEncapsulationRadiusSignal(G4double r)
  { fTeflonEncapsulationRadiusSignal = r; }

  void SetTeflonEncapsulationCopperOuterRadius(G4double r)
  { fTeflonEncapsulationCopperOuterRadius = r;}

  void SetTeflonEncapsulationCopperHeight(G4double h)
  { fTeflonEncapsulationCopperHeight = h;}

  void SetTeflonEncapsulationCopperThickness(G4double d)
  { fTeflonEncapsulationCopperThickness = d; }

  void SetTeflonEncapsulationRow(G4int row)
  { fTeflonEncapsulationRow = row;}

  void SetTeflonEncapsulationColumn(G4int column)
  { fTeflonEncapsulationColumn = column;}

  // mini-shroud parameters

  void SetMiniShroudOuterRadius(G4double r)
  { fMiniShroudOuterRadiusPhaseI = r;}

  void SetMiniShroudHeight(G4double h)
  { fMiniShroudHeightPhaseI = h;}

  void SetMiniShroudHeightPhaseIINylon(G4double h)
  { fMiniShroudHeightPhaseIINylon = h;}

  void SetMiniShroudSideThickness(G4double d)
  { fMiniShroudSideThickness = d; }

  void SetMiniShroudBaseThickness(G4double d)
  { fMiniShroudBaseThickness = d; }

  void SetMiniShroudTopGluedRingHeight(G4double d)
  { fMiniShroudTopGluedRingHeight = d; }

  void SetMiniShroudBottomGluedRingHeight(G4double d)
  { fMiniShroudBottomGluedRingHeight = d; }

  void SetMiniShroudMaterial(G4String s)
  { fMiniShroudMaterial = s; }

  // select photomultiplier distribution

  void SetSelectPMTDistribution(G4int dist_number)
  { fSelectPMTDistribution = dist_number; }

  // --------------------------------------------------------------------
  // LAr Instrumentation
  // --------------------------------------------------------------------
  void SetFlagConstructLArInstrumentation(G4bool flag)
  { fFlagLArInstrumentation = flag; }

  void SetLArInstrumentationDesign(GEGeometryLarInstrDesign_t design)
  {fLArInstDesign = design;}

  void SetLArInstHybridInnerFiber(G4bool status)
  {fLArInstHybridInnerFiber = status;}

  void SetLArInstConstructExtSource(G4bool status) {
    fLArInstBuildExtSource = status;
  }

  void SetLArInstArgonAbsLScale(G4double scale) {
    fLArInstScaleLArAbsL = scale;
  }

  void SetLArInstArgonLYScale(G4double scale) {
    fLArInstScaleLArLY = scale;
  }

  void SetLArInstFiberCoverage(G4double scale) {
    fLArInstFiberCoverage = scale;
  }

  void SetLArInstTopPMTscaleFact(G4double scale) {
    fLArInstTopPMTscaleFact = scale;
  }

  void SetLArInstMetalsReflScale(G4double scale) {
    fLArInstScaleMetalRefl = scale;
  }

  void SetLArInstrumentationAPDDesign(GEGeometryLarInstrAPDDesign_t APDdesign)
  {fLArInstAPDDesign = APDdesign;
  MGLog(trace) << fLArInstAPDDesign << endlog;}

  void SetLArInstrumentationAPDDeployment(GEGeometryLarInstrAPDDeployment_t APDdeploy)
  {fLArInstAPDDeployment = APDdeploy;
  MGLog(trace) << fLArInstAPDDeployment << endlog;}

  void SetLArInstrumentationMSCoating(GEGeometryLarInstrMSCoating_t MSCoating)
  {fLArInstMSCoating = MSCoating;
  MGLog(trace) << fLArInstMSCoating << endlog;}

  void SetLArInstrumentationPhotTrack(G4bool flag)
  {fFlagLArPhotTrack = flag;
  MGLog(trace) << fFlagLArPhotTrack << endlog;}

  void SetLArInstHybridSegmentedFiber(G4bool status)
  {fLArInstHybridSegmentedFiber = status;}

  void SetLArInstHybridSegmentedFiberShroudRotation(G4double scale)
  {fLArInstHybridSegmentedFiberShroudRotation = scale;}

  // --------------------------------------------------------------------
  // online counter
  // --------------------------------------------------------------------

  void IncreaseSegmentNumber()
  { fSegmentNumber++; }

  void IncreaseCrystalNumber()
  { fCrystalNumber++; }

  void IncreaseCrystalGeometry1Number()
  { fCrystalGeometry1Number++; }

  void IncreaseCrystalGeometry2Number()
  { fCrystalGeometry2Number++; }

  void IncreaseCrystalGeometry3Number()
  { fCrystalGeometry3Number++; }

  void IncreaseCrystalGeometry4Number()
  { fCrystalGeometry4Number++; }

  void IncreaseCrystalGeometry5Number()
  { fCrystalGeometry5Number++; }

  void IncreaseCrystalGeometry5SlotNumber()
  { fCrystalGeometry5SlotNumber++; }

  void IncreaseCrystalGeometry11Number()
  { fCrystalGeometry11Number++; }

  void IncreaseColumnNumber()
  { fColumnNumber++; }

  void IncreaseColumnGeom1Number()
  { fColumnGeom1Number++; }

  void IncreaseColumnGeom2Number()
  { fColumnGeom2Number++; }

  void IncreaseCalibrationSourceCounter()
  { fCalibrationSourceCounter++; }

  void IncreaseCollimatedCalibrationSourceCounter()
  { fCollimatedCalibrationSourceCounter++; }

  void SetZShiftScheme(GEGeometryCrystalZShiftScheme_t zShiftScheme);

  void SetZShiftScheme_UnshiftedColumn(G4int iUnshiftedColumn)
  { fZShiftScheme_UnshiftedColumn = iUnshiftedColumn;}

  void SetLockType(GEGeometryLockType_t fLockType);


  // methods

  void ConstructGEGeometryDetectorDB();
  void OpenInputFiles();
  void FindUppermostCrystalSurface();

private:

  // helper function to subtract neck volume from water volume correctly
    G4Polycone* Create_ReflectionFoilCryoFootTop();

  void  SetCryostatGeometry();

  void  DefineMaterials();
  void  DefineVisualizationAttributes();
  void  CreateHoldersForPhase1();
  void  CreateHoldersForPhase2();

  void  Set_ZShiftScheme_CrystalOffset();

  void  Create_HVCables_Phase2();
  void  Create_DetectorCablesForPhase2();

  GEGeometryDetectorParameters* CreatePhase2StandardDetectorParameters();
  GEGeometryDetectorParameters* fPhase2StandardDetectorParameters ;

  // general

  G4bool   fFlagConstructDetector;
  G4bool   fFlagConstructCrystals;
  G4bool   fFlagConstructSupport;
  G4bool   fFlagConstructCabling;
  G4bool   fFlagConstructShielding;
  G4bool   fFlagConstructCalibrationSource;
  G4bool   fFlagConstructCalibrationSourceCuCap;
  G4bool   fFlagConstructCollimatedCalibrationSource;
  G4bool   fFlagConstructCopperEncapsulation;
  G4bool   fFlagConstructTeflonEncapsulation;
  G4bool   fFlagConstructMiniShroud;
  G4bool   fFlagLArInstrumentation;
  G4bool   fFlagAdaptMinishroudToDetctorString;
  G4bool   fFlagReverseNonTrueCoaxial;

  // shielding

  GEGeometryCryostatParts* fCryostatParts;

  G4bool   fFlagCryoLiquid;

  G4double fHallSizeWidth;
  G4double fHallSizeLength;
  G4double fHallSizeHeight;

  G4double fShieldingWaterTankThickness;
  G4double fShieldingWaterTankHeight;

  G4double fShieldingWaterRadius;
  G4double fShieldingWaterHeight;

  G4double fShieldingAirBufferRadius;
  G4double fShieldingAirBufferHeight;

  G4double fShieldingFootThickness;
  G4double fShieldingFootIR;
  G4double fShieldingFootOR;
  G4double fShieldingFootHeight;

  G4double fShieldingCopperShieldingLargeHeight;
  G4double fShieldingCopperShieldingLargeRadius;

  G4double fShieldingCopperShieldingSmallHeight;
  G4double fShieldingCopperShieldingSmallRadius;

  //G4double fShieldingZCenterOfCryo;

  // clean room

  G4double fCleanRoomLengthXY;
  G4double fCleanRoomHeight;
  G4double fCleanRoomThickness;
  G4double fCleanRoomHoleRadius;

  G4double fSuperStructureHeight;

  G4double fSuperStructureCollarThickness;
  G4double fSuperStructureCollarOR;
  G4double fSuperStructureCollarIR;
  G4double fSuperStructureCollarHeight;

  G4double fSuperStructureBellowThickness;
  G4double fSuperStructureBellowOR;
  G4double fSuperStructureBellowIR;
  G4double fSuperStructureBellowHeight;

  G4double fSSFlangeBellowToCollarIR       ;
  G4double fSSFlangeBellowToCollarOR       ;
  G4double fSSFlangeBellowToCollarThickness;
  G4double fSSFlangeBellowToCollarHeight   ;

  G4double fSuperStructureFlangeThickness;
  G4double fSuperStructureFlangeOR;
  G4double fSuperStructureFlangeIR;
  G4double fSuperStructureFlangeHeight;

  // lock

  G4double fOuterLockRadius;
  G4double fOuterLockHeight;
  G4double fOuterLockThickness;

  G4double fInnerLockRadius;
  G4double fInnerLockHeight;
  G4double fInnerLockThickness;

  G4double fLockCopperPlateRadius;
  G4double fLockCopperPlateHeight;

  G4double fLockSteelPlateRadius;
  G4double fLockSteelPlateHeight;
  G4double fLockSteelPlateZPosRelToLock;

  // crystals

  //G4int fNCrystalsNTC;
  //G4int fNCrystalsTC;
  //G4int fNCrystals;

  // detector parameters

  std::vector<GEGeometryDetectorParameters*> fDetectorParametersContainer;

  // dynamical numbers for numbering segments, holders, etc.

  G4int fCrystalGeometry1Number;
  G4int fCrystalGeometry2Number;
  G4int fCrystalGeometry3Number;
  G4int fCrystalGeometry4Number;
  G4int fCrystalGeometry5Number;
  G4int fCrystalGeometry5SlotNumber;
  G4int fCrystalGeometry11Number;
  G4double fCrystalPairGap;
  G4int fCrystalNumber;
  G4int fSegmentNumber;
  G4int fColumnNumber;
  G4int fColumnGeom1Number;
  G4int fColumnGeom2Number;
  //G4int fColumnGeom4Number;
  G4int fCalibrationSourceCounter;
  G4int fCollimatedCalibrationSourceCounter;
  //  G4int fCableNumber; //!!!
  G4bool   fFlagCrystalMatrixWithZPosFile;
  // array

  G4double fColumnDistanceR;
  G4double fDistancePhase2_CrystalToCrystal;
  G4double fDistanceHolderToHolder;
  G4double fArrayDepth;
  G4double fPhase1RadiusPos4;
  G4double fPhase1RadiusPos5;
  G4double fSegmentationOffsetAngle;
  G4double fHolder_DistanceToCrystalTop;
  G4double fHolder_DistanceToCrystalBot;
  G4double fPositionOfTopCrystalinCryostat;
  G4double fDistanceBetweenHolderBarsPhase2;


  G4int    fArrayDetectorTypeMatrix[nrOfStrings][nrOfRows][nrOfMultiplets];
  G4double   fArrayDetectorIndZPositionMatrix[nrOfStrings][nrOfRows];

  // cable

  G4double fCableThickness;
  G4double fCableLoopHeight;
  G4double fCableStringWidth;

  G4double fHVCableThickness;
  G4double fHVCableWidth;

  G4double fCableInnerRadius;
  G4double cable_width_deg;


  G4double fCableCopperLinesThickness;
  G4double fCableCopperLinesWidth;
  G4double fCableCopperLinesSpanningAngle;
  G4double fCableCopperPadsThickness;
  G4double fCableCopperPadsHeight;
  G4double fCableCopperPadsSpanningAngle;
  G4double fCopperLinesConnectionWidth;
  G4double fKaptonConnectionCableWidth;
  G4double fCopperLinesConnectionLength;
  G4double fKaptonConnectionCableLength;
  G4double fCopperLinesConnectionThickness;
  G4double fKaptonConnectionCableThickness;

  G4double fPhase1Cable_thickness;
  G4double fPhase1Cable_width    ;

  G4double fPhase2Cable_Thickness;
  G4double fPhase2Cable_WidthOnHolder;
  G4double fPhase2Cable_LengthOnHolderHV;
  G4double fPhase2Cable_LengthOnHolderSignal;
  G4double fPhase2Cable_WidthFromHolderToElectronicsPlate;


  // holders

  G4double fHolderUpperBarHeight;
  G4double fHolderLowerBarHeight;
  G4double fHolderLowerTrapezoidHeight;
  G4double fHolderSideBarHeight;
  G4double fHolderSideBarRadius;
  G4double fHolderTeflonTubeHeight;
  G4double fHolderInnerHeight;

  G4double fHolderSignalCableInnerHeight;
  G4double fHolderSignalCableOuterHeight;
  G4double fHolderSignalCableInnerSpanningAngle;
  G4double fHolderSignalCableOuterSpanningAngle;

  G4double fHolderHVCableInnerHeight;
  G4double fHolderHVCableOuterHeight;
  G4double fHolderHVCableSpanningAngle;

  G4double fHolderStar_SpacerHeight;
  //G4double fDistance_LowerSurfaceDetN_UpperSurfaceDetNPlusOne;+

  G4double fHolderBarRadiusPhase2;
  G4double fHolderBarsPosRadPhase2;
  G4double fGTFHolderBarsPosRadPhase2;
  G4double fPhase2HolderStarPlac;
  G4double fPhase2HolderStarLengthZ;
  G4double fPhase2HolderTubsHeight;
  G4double fPhase2HolderTubsOuterRadius;
  // bond pads
  G4double fBondPadSpanningAngle;
  G4double fBondPadAlThickness;


  // electronics

  G4double fElectronicsDistanceFromCrystal;
  G4double fElectronicsDistanceBoardToBoard;
  G4double fElectronicsBoardGap;
  G4double fElectronicsBoardRadius;
  G4double fElectronicsBoardHeight;
  G4double fElectronicsBoxLength;
  G4double fElectronicsBoxHeight;
  G4double fElectronicsSideBar_MinimumHeight;
  G4double fElectronicsCableHeight;
  G4double fElectronicsCableY;
  G4double fElectronicsCableX;

  G4double fElectronicsPhaseIHorizontalBar_InnerWallYThickness ;
  G4double fElectronicsPhaseIHorizontalBar_YFrameThickness     ;
  G4double fElectronicsPhaseIHorizontalBar_XZFrameThickness    ;
  G4double fElectronicsPhaseIHorizontalBar_Width               ;
  G4double fElectronicsPhaseIHorizontalBar_InnerHeight         ;
  G4double fElectronicsPhaseIHorizontalBar_OuterHeight         ;

  G4double fElectronicsJunctionSledHeight;
  G4double fElectronicsJunctionSledPinsHeight;
  G4double fElectronicsJunctionSledSpacerHeight;
  G4double fElectronicsJunctionSledCablesHeight;
  G4double fElectronicsJunctionSledZPlane[2];
  G4double fElectronicsJunctionSledRMin[6];
  G4double fElectronicsJunctionSledRMax[6];

  G4double fElectronicsKaptonPreAmpsPinsCableHeight;

  // container for all stuff inside a string
  // (in principle good for everything above the upper most crystal of a string)
  std::vector<GEGeometryStringStuff*> fStringStuffContainer;

  //cables + cablechain + chain above Junctionsled

  G4double fElectronicsCableChainHeight;
  G4double fElectronicsCableChainX;
  G4double fElectronicsCableChainY;
  G4double fElectronicsCableChainDistance;

  G4double fElectronicsChainHeight;
  G4double fElectronicsChainRadius;

  // muon veto

  G4double fScintillatorHeight;
  G4double fScintillatorLengthX;
  G4double fScintillatorLengthY;
  G4double fScintillatorPosX;
  G4double fScintillatorPosY;
  G4double fScintillatorPosZ;
  G4double fScintillatorAngleZ;
  G4double fScintillatorHoleOuterRadius;

  G4bool fScintillatorHole;

  // calibration source

  G4double fCalibrationSourcePositionZ;
  G4double fCalibrationSourcePositionX_S1;
  G4double fCalibrationSourcePositionX_S2;
  G4double fCalibrationSourcePositionX_S3;
  G4double fCalibrationSourcePositionY_S1;
  G4double fCalibrationSourcePositionY_S2;
  G4double fCalibrationSourcePositionY_S3;
  G4double fCalibrationSourcePositionZ_S1;
  G4double fCalibrationSourcePositionZ_S2;
  G4double fCalibrationSourcePositionZ_S3;
  G4double fCalibrationCylinderHeight;

  // collimated calibration source

  G4double fCollimatedCalibrationSourcePositionZ;
  G4double fCollimatedCalibrationSourcePositionX_S1;
  G4double fCollimatedCalibrationSourcePositionX_S2;
  G4double fCollimatedCalibrationSourcePositionX_S3;
  G4double fCollimatedCalibrationSourcePositionY_S1;
  G4double fCollimatedCalibrationSourcePositionY_S2;
  G4double fCollimatedCalibrationSourcePositionY_S3;
  G4double fCollimatedCalibrationSourcePositionZ_S1;
  G4double fCollimatedCalibrationSourcePositionZ_S2;
  G4double fCollimatedCalibrationSourcePositionZ_S3;
  G4double fCalibrationSourcePhase2DN500Radius;
  G4double fCalibrationSourcePhase2DN500OffsetX;
  G4double fCalibrationSourcePhase2DN500OffsetY;
  G4double fCalibrationSourcePhase2CoordTrafo;
  G4double fCalibrationSourcePhase2DN500AngleOffset1;
  G4double fCalibrationSourcePhase2DN500AngleOffset2;
  G4double fCalibrationSourcePhase2DN500AngleOffset3;
  G4double fCollimatedCalibrationCylinderHeight;
  G4double fCollimatedCalibrationCylinderDiameter;
  G4double fCollimatedCalibrationCapHeight;
  G4double fCollimatedCalibrationCapDiameter;
  G4double fCollimatedCalibrationCapsuleHeight;
  G4double fCollimatedCalibrationCapsuleDiameter;
  G4double fCollimatedCalibrationSourceHeight;
  G4double fCollimatedCalibrationSourceDiameter;
  G4double fCalibrationCollimationDistance;

  // copper encapsulation

  G4double fCopperEncapsulationOuterRadius;
  G4double fCopperEncapsulationHeight;
  G4double fCopperEncapsulationThickness;
  G4double fCopperEncapsulationRadiusHV;
  G4double fCopperEncapsulationRadiusSignal;
  G4int fCopperEncapsulationRow;
  G4int fCopperEncapsulationColumn;

  // teflon encapsulation

  G4double fTeflonEncapsulationOuterRadius;
  G4double fTeflonEncapsulationHeight;
  G4double fTeflonEncapsulationThickness;
  G4double fTeflonEncapsulationRadiusHV;
  G4double fTeflonEncapsulationRadiusSignal;
  G4double fTeflonEncapsulationCopperOuterRadius;
  G4double fTeflonEncapsulationCopperHeight;
  G4double fTeflonEncapsulationCopperThickness;
  G4int fTeflonEncapsulationRow;
  G4int fTeflonEncapsulationColumn;

  // mini-shroud

  G4double fMiniShroudOuterRadiusPhaseI;
  G4double fMiniShroudOuterRadiusCoaxString;
  G4double fMiniShroudOuterRadiusBEGeString;
  G4double fMiniShroudHeightPhaseI;
  G4double fMiniShroudHeightPhaseII;
  G4double fMiniShroudHeightPhaseIINylon;
  G4double fMiniShroudSideThickness;
  G4double fMiniShroudBaseThickness;
  G4double fMiniShroudTopGluedRingHeight;
  G4double fMiniShroudBottomGluedRingHeight;
  G4double fMiniShroudHoleRadiusCoaxString;
  G4double fMiniShroudHoleRadiusBEGeString;
  //G4ThreeVector fMiniShroudPosition;
  G4String fMiniShroudMaterial;

  std::vector<G4int>* fMiniShroudRow;
  std::vector<G4int>* fMiniShroudColumn;
  std::vector<G4ThreeVector>* fMiniShroudPosition;
  std::vector<G4ThreeVector>* fMiniShroudPositionPhase2Tub;
  std::vector<G4ThreeVector>* fMiniShroudPositionPhase2Bottom;
  std::vector<G4ThreeVector>* fMiniShroudPositionPhase2Top;
  std::vector<G4ThreeVector>* fMiniShroudPositionPhase2TopGluedRing;
  std::vector<G4ThreeVector>* fMiniShroudPositionPhase2BottomGluedRing;

  std::vector<G4double>* fMiniShroudRadiusVector;
  std::vector<G4double>* fMiniShroudHeightVector;
  std::vector<G4double>* fMiniShroudHoleRadiusVector;
  std::vector<G4double>* fMiniInDetHeightVector;
  G4bool fFlagMSMaterialCheck;


  // cherenkov veto

  G4int fSelectPMTDistribution;
  G4double fPMTStartingAngle ;
  G4double fPMTFinalAngle    ;
  G4double fPhotocathodeInnerRadius ;
  G4double fPhotocathodeOuterRadius ;
  G4double fPhotocathodeThetaStart  ;
  G4double fPhotocathodeThetaEnd    ;
  G4double fPMTInnerRadius ;
  G4double fPMTOuterRadius ;
  G4double fPMTThetaIn     ;
  G4double fPMTThetaEnd    ;
  G4double fPMTSteelConeThickness ;
  G4double fPMTSteelConeHeight    ;
  G4double fPMTSteelConeUpperRmin ;
  G4double fPMTSteelConeUpperRmax ;
  G4double fPMTSteelConeLowerRmin ;
  G4double fPMTSteelConeLowerRmax ;
  G4double fPMTSteelBottomHeight  ;
  G4double fReflectionFoilThickness ;

  // PMT cable tray

  G4double fPMTCableTrayThickness;
  G4double fPMTVerticalCableTrayHeight;
  G4double fPMTHorizontalCableTrayWidth;


  // --------------------------------------------------------------------
  // logical volumes
  // --------------------------------------------------------------------

  // mothervolume

  G4LogicalVolume* fDetectorMotherVolume;

  // shielding
  G4LogicalVolume* fExternalRockLogical;
  G4LogicalVolume* fAirBoxLogical;
  G4LogicalVolume* fWaterTankLogical;
  G4LogicalVolume* fWaterLogical;
  G4LogicalVolume* fAirBufferLogical;
  G4LogicalVolume* fFootLogical;
  G4LogicalVolume* fCopperShieldingLargeLogical;
  G4LogicalVolume* fCopperShieldingSmallLogical;

  // clean room

  G4LogicalVolume* fCleanRoomLogical;
  G4LogicalVolume* fSuperStructureCollarLogical;
  G4LogicalVolume* fSuperStructureBellowLogical;
  G4LogicalVolume* fSuperStructureFlangeLogical;
  G4LogicalVolume* fSSFlangeBellowToCollarLogical;
  G4LogicalVolume* fSuperStructureBeamGridLogical;

  // lock

  G4LogicalVolume* fOuterLockLogical;
  G4LogicalVolume* fInnerLockLogical;
  G4LogicalVolume* fLockCopperPlateLogical;
  G4LogicalVolume* fLockSteelPlateLogical;

  // holder, phase i
  GEGeometryHolderPhase1Parts* fDefaultHolderPhase1Parts;
  std::vector<GEGeometryHolderPhase1Parts*> fHolderPhase1Container;

  // holder, phase ii

  G4LogicalVolume* fHolderCopperLogical;
  G4LogicalVolume* fHolderTeflonLogical;
  G4LogicalVolume* fHolderSignalCableLogical;
  G4LogicalVolume* fHolderHVCableLogical;

  // holder, phase II, BEGes
  GEGeometryHolderPhase2Parts* fDefaultHolderPhase2Parts;
  std::vector<GEGeometryHolderPhase2Parts*> fHolderPhase2Container;

  // holder from detectors to electronic plate, phase II, status december 2015
  G4LogicalVolume* fPhase2HolderStarLogical;
  G4LogicalVolume* fPhase2HolderGreatStarLogical;
  G4LogicalVolume* fPhase2HolderBarsToEPlateLogical;

  // electronics

  G4LogicalVolume* fElectronicsBoxLogical;
  G4LogicalVolume* fElectronicsCableLogical;

  G4LogicalVolume* fElectronicsJunctionSledLogical;
  G4LogicalVolume* fElectronicsJunctionSledPinsLogical;
  G4LogicalVolume* fElectronicsJunctionSledSpacerLogical;
  G4LogicalVolume* fElectronicsJunctionSledCablesLogical;
  G4LogicalVolume* fElectronicsJunctionBoardLogical;

  G4LogicalVolume* fElectronicsPhaseIHorizontalBarLogical;

  //cable from JunctionBoard to JunctionSled (connection from preamps to pogopins)
  G4LogicalVolume*  fElectronicsKaptonPreAmpsPinsCableLogical;

  // cables + cablechain + chain above sled
  G4LogicalVolume* fElectronicsCableChainLogical;
  G4LogicalVolume* fElectronicsChainLogical;

  // scintillator

  G4LogicalVolume* fScintillatorLogical;

  // calibration source

  G4LogicalVolume* fCalibrationCylinderLogical;
  G4LogicalVolume* fCalibrationSourceInnerLogical;
  G4LogicalVolume* fCalibrationSourceOuterLogical;
  G4LogicalVolume* fCalibrationCylCapLogical;

  // collimated calibration source

  G4LogicalVolume* fCollimatedCalibrationCylinderLogical;
  G4LogicalVolume* fCollimatedCalibrationSourceInnerLogical;
  G4LogicalVolume* fCollimatedCalibrationSourceOuterLogical;

  // copper encapsulation

  G4LogicalVolume* fCopperEncapsulationLogical;
  G4LogicalVolume* fTeflonEncapsulationLogical;
  G4LogicalVolume* fTeflonEncapsulationCopperLogical;

  // mini-shroud

  std::vector<G4LogicalVolume*> fMiniShroudLogical;
  std::vector<G4LogicalVolume*> fMiniShroudTubPhase2Logical;
  std::vector<G4LogicalVolume*> fMiniShroudBottomPhase2Logical;
  std::vector<G4LogicalVolume*> fMiniShroudTopPhase2Logical;
  std::vector<G4LogicalVolume*> fMiniShroudTopGluedRingPhase2Logical;
  std::vector<G4LogicalVolume*> fMiniShroudBottomGluedRingPhase2Logical;

  // crystals

  std::vector<G4LogicalVolume*> fCrystalPassivationLogicalContainer;
  std::vector<G4LogicalVolume*> fCrystalDeadLayerLogicalContainer;
  std::vector<G4LogicalVolume*> fCrystalSegmentLogicalContainer;
  // bond pads
  std::vector<G4LogicalVolume*> fBondPadAlLogicalContainer;

  // cables

  std::vector<G4LogicalVolume*> fCableLogicalContainer;
  std::vector<G4LogicalVolume*> fCableCopperLinesLogicalContainer;


  std::vector<G4LogicalVolume*> fKaptonConnectionCable;
  std::vector<G4LogicalVolume*> fCopperLinesConnection;


  // hv cables
  G4LogicalVolume* f_HVCablePhase2VerticalLogical[5];     // JS
  G4double         f_HVCablePhase2Vertical_ZPos[5];       // JS
  G4LogicalVolume* f_HVCablePhase2HorizontalLogical;      // JS
  G4double         f_HVCablePhase2Horizontal_relZPos;     // JS
  G4double         f_HVCablePhase2Horizontal_RadiusOffset;// JS

  //cables for phase 2 (status December 2015)
  G4LogicalVolume* fHVCableAtHolderPhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fHVCableToEPlatePhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fHVCableAtHolderTopPhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fHVCableTopToEPlatePhase2Logical[nrOfStrings][nrOfRows];

  G4LogicalVolume* fSignalCableAtHolderPhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fSignalCableToEPlatePhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fSignalCableAtHolderTopPhase2Logical[nrOfStrings][nrOfRows];
  G4LogicalVolume* fSignalCableTopToEPlatePhase2Logical[nrOfStrings][nrOfRows];

  G4double fHVPhase2ToEPlate_LengthZ[nrOfStrings][nrOfRows];
  G4double fHVTopPhase2ToEPlate_LengthZ[nrOfStrings][nrOfRows];

  // cherenkov veto

  G4LogicalVolume* fPhotocathodeLogical;
  G4LogicalVolume* fPMTSteelConeLogical;
  G4LogicalVolume* fPMTSteelBottomLogical;
  G4LogicalVolume* fReflectionFoilWaterTankTubeLogical;
  G4LogicalVolume* fReflectionFoilWaterTankBottomLogical;
  G4LogicalVolume* fReflectionFoilCryoFootLogical;
  G4LogicalVolume* fReflectionFoilCryoFootTopLogical;
  G4LogicalVolume* fReflectionFoilCryoFootBottomLogical;

  // PMT cable tray

  G4LogicalVolume* fVerticalPMTCableTrayLogical;
  G4LogicalVolume* fHorizontalPMTCableTrayLogical;

  // --------------------------------------------------------------------
  // messenger
  // --------------------------------------------------------------------

  GEGeometryDetectorDBMessenger* fMessenger;

  // --------------------------------------------------------------------
  // input file for crystal geometries
  // --------------------------------------------------------------------

  G4String fCrystalGeometryFilename;
  G4String fCrystalMatrixFilename;

  std::ifstream fInputFileGeometry;
  std::ifstream fInputFileMatrix;


  // this is to make drawings with WIRED nicer
  // -----------------------------------------
  static G4double SmallNumber;


  // in order to set zpositions of Electronics to one common z-level
  // ----------------------------------------------------------------
  G4double        fZUppermostCrystalSurface;
  G4int           fColumnWithUppermostCrystalSurface;
  G4int           fCrystalGeometryOfUppermostCrystalSurface;
  G4int           fCrystalTypeOfUppermostCrystalSurface;

  // in order to get the fiber LAr Instrumentation centered
  // ----------------------------------------------------------------
  G4double        fZLowermostCrystalSurface;
  G4int           fColumnWithLowermostCrystalSurface;
  G4int           fCrystalGeometryOfLowermostCrystalSurface;
  G4int           fCrystalTypeOfLowermostCrystalSurface;

  // materials
  // ----------------
  G4Material* air             ;
  G4Material* water           ;
  G4Material* vacuum          ;
  G4Material* liquidnitrogen  ;
  G4Material* liquidargon     ;
  G4Material* liquidxenon     ;
  G4Material* gaseousnitrogen ;
  G4Material* gaseousargon    ;
  G4Material* enrGe           ;
  G4Material* copper          ;
  G4Material* plasticScint    ;
  G4Material* lead            ;
  G4Material* steel           ;
  G4Material* teflon          ;
  G4Material* si              ;
  G4Material* gold            ;
  G4Material* capsGold        ;
  G4Material* nickel          ;
  G4Material* aluminium       ;
  G4Material* cathodeAl       ;
  G4Material* nylon           ;
  G4Material* PET             ;
  G4Material* kapton          ;
  G4Material* brass           ;
  G4Material* makrolon        ;
  G4Material* torlon          ;
  G4Material* tungsten        ;
  G4Material* lngsrock        ;
  G4Material* cryoliquid      ;
  G4Material* cryogas         ;
  G4Material* tantalum        ;

  G4Material* HVCableCuTeflonComposite;

  // visualization attributes

  G4VisAttributes* WaterVisAtt               ;
  G4VisAttributes* VacuumVisAtt              ;
  G4VisAttributes* LN2VisAtt                 ;
  G4VisAttributes* N2VisAtt                  ;
  G4VisAttributes* CuVisAtt                  ;
  G4VisAttributes* SteelVisAtt               ;
  G4VisAttributes* CrystalSegmentVisAtt      ;
  G4VisAttributes* CrystalPassivationVisAtt  ;
  G4VisAttributes* CrystalDeadLayerVisAtt    ;
  G4VisAttributes* NiVisAtt                  ;
  G4VisAttributes* AuVisAtt                  ;
  G4VisAttributes* AlVisAtt                  ;
  G4VisAttributes* TeflonVisAtt              ;
  G4VisAttributes* SiVisAtt                  ;
  G4VisAttributes* PhotocathodeVisAtt        ;
  G4VisAttributes* VM2000VisAtt              ;
  G4VisAttributes* NylonVisAtt               ;
  G4VisAttributes* GluedNylonVisAtt          ;
  G4VisAttributes* MakrolonVisAtt            ;
  G4VisAttributes* CalibrationCylinderVisAtt ;
  G4VisAttributes* CollimatedCalibrationCylinderVisAtt ;

  GEGeometryCrystalZShiftScheme_t fZShiftScheme;
  G4int    fZShiftScheme_UnshiftedColumn;
  G4double fZShiftScheme_CrystalOffset[19];

  GEGeometryLockType_t fLockType;
  GEGeometryLarInstrDesign_t fLArInstDesign;
  GEGeometryLarInstrAPDDesign_t fLArInstAPDDesign;
  GEGeometryLarInstrAPDDeployment_t fLArInstAPDDeployment;
  GEGeometryLarInstrMSCoating_t fLArInstMSCoating;
  G4bool fLArInstHybridInnerFiber;
  G4bool fFlagLArPhotTrack;
  G4bool fLArInstHybridSegmentedFiber;
  G4double fLArInstHybridSegmentedFiberShroudRotation;

  // Auxiliary variables to be able to know exactly where the center of the string is with respect to
  // a given lock scheme. This has no relation with the flange drawing
  G4int fNlocks;
  G4ThreeVector *fStringDetectorCenter;

  G4int fDN500NStrings;
  G4double fDN500StringDistance;

  //Auxiliary variables for systematics studies in LAr Instrumentation
  G4bool fLArInstBuildExtSource;
  G4double fLArInstScaleLArAbsL;
  G4double fLArInstScaleLArLY;
  G4double fLArInstFiberCoverage;
  G4double fLArInstTopPMTscaleFact;
  G4double fLArInstScaleMetalRefl;

};

#endif
