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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION:
 *
 * 01-21-2005, included more strings & bugfix ("or" is not a very good
 *             variable name), Kevinch
 * 01-23-2005, included segmentation of detectors, Kevin
 * 02-08-2005, included calibration source, Kevin
 * 02-16-2005, registered detector and dead layer as "sensitive regions" (cut-per-region), Luciano
 * 02-16-2005, included defaults for the position/rotation of the top mu-veto, Luciano
 * 03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 * 04-14-2005  changed deadlayer thickness, Xiang Liu
 *             inner 1mm outer 0.01mm
 *             top and bottom 0.001mm
 * 04-25-2005  modified the non-true-coaxial logical volume for
 *             deadlayer and crystal
 * 07-05-2005  Added bodies and logical volumes for Phase I holders. Logical volumes of Phase I/II
 *             switched by means of a boolean flag
 * 07-05-2005, Added a hole for the neck in the plastic scintillator and a boolean flag to switch from
 *             hole/no-hole, Claudia
 * 07-05-2005, Added bond pads on cables, Kevin
 * 11-27-2005, Major changes to the geometry structure, Kevin
 * 02-22-2005, Added cleanroom and lock, Kevin
 * 03-06-2006, Added Cherenkov veto, Markus
 * 03-07-2006, Crystal segment ID start from 0 (not -1)
 * 06-05-2006  Cryostat changed from copper to steel. Dimensions taken from a
 *             design provided by K.Tasso, still preliminary. Claudia and Assunta
 * 09-13-2006, Added some VM2000 foils, Markus
 * 12-05-2006, Changed radius and filling level of the neck according to information received
 *             by Bernhard, Luciano
 * 12-13-2006, Major change in the structure (including files), Kevin
 * 12-13-2006, Set Liquid Argon as default cryogenic liquid, Luciano
 * 12-15-2006, added HV-cable for Phase II detectors, Kevin
 * 03-15-2007 Added the cable tray for PMT cabling
 * 05-10-2007 Added new geometry for junction board, matrix, matrixcounterpart, and pogo pins and copperpads
 * 09-04-2007  Changed name of the Sensitive Region (removed blank space), Luciano
 * 10-08-2007 - bug fix (there was an overlap between the neck of cryostat and the water), Jens
 *            - define a new parameter (z position of the steel plate inside the lock), Jens
 * 10-10-2007 bug fixed yesterdays bug fix , Daniel
 *            Added BondPads and CableGuides (HolderSignalCable and HolderHVCable), Daniel
 * 10-15-2007 Completly changed JunctionBoard and Sled to hexagonal shape, Daniel
 * 10-16-2007 Added Cablechain and Chain, Daniel
 * 10-17-2007 Added cable to the outside world, Daniel
 * 11-26-2007 Added Signal and HV cable from detector to JB and some more cable changes close to detector, Daniel
 * 12-05-2007 Cryostat is now one object
 *            - neck Cu layer, and so on belong to this object
 *            - makrolon layer has been added, addition of a further layer should be easy
 *            - the former neck radius was to big by a factor of two
 *            - further little changes
 *            - all parts belonging to the Cryostat should now belong to the class "GEGeometryCryostatParts"
 * 12-10-2007 (jens) - Added to cryostat: Torlone spacers in gap and thermal shield in neck
 *            - Changed height and radii of bellow and collar above neck
 *            - a few other minor changes in geometry
 *            - delete the cryostat object in the destructor
 * 12-19-2007 - minor bug fix, set 'fReflectionFoilThickness' earlier since it is needed to set fShieldingFootHeight
 * 04-08-2008 - Add tungsten as materiel, Francis
 * 02-04-2008 - phase-1 crystals have individual holders now  --> holder object is replaced by a vector of holders
 *            - z positions of phase-1 crystals and holders has been adapted to the height of the crystals
 *            - some requirements have been defined for the format of the files geometry.dat and matrix.dat
 *            - (Jens S.)
 * 04-29-2008 - Register logical volumes and parameters related to Hall A (tunnel in LNGS). Luciano
 * 04-30-2008 - new implementation of Phase-1 crystals, the hold version did cause some problems (an overlap between
 *              the crystal and its CopperContactPlate)
 *            - (Jens s.)
 * 04-30-2008 - Register logical volumes and parameters related to Hall A (tunnel in LNGS). Luciano
 * 05-05-2008 - Turn around phase-1 crystal to be consistent with the component names "Upper" and "Lower" (Holder components and Layers in geometry.dat)
 *              (Jens)
 * 08-05-2008 - addaped size and position of phase1-string-electronics to the size and position of the phase2-string-electronics
 *            - introduce two schemes for the z-position of the crystals:
 *                o "unshifted"
 *                    --> positions are as before,
 *                        i.e. the upper edge of the crystals in the upper-most row are on one z level
 *                o "center_between_top0_and_bottom2_on_common_zlevel"
 *                    --> new scheme,
 *                        i.e. the centers between top of 0-th and bottom of 2-nd crystal are on one common z level for all strings
 *              (jens)
 *
 * 06-13-2008 - Changed DeadLayer and Crystal_geometry1 such that Crystal is daughter of liquid argon. Daniel
 * 07-02-2008 - fHolderInnerHeight changed from 90 to 80mm due to change in height of phase-2 crystals (80->70mm), jens
 * 07-09-2008 - added phase-1 cable, Jens
 *            - put some RotMatrices onto the heap instead onto the stack, Jens
 *            - the old phase-2 cables (Cable_geometry2_xxx) are now without the HV-Strip
 *              there are now the following phase-2 cables:
 *                 o HV cable connection (one per String -> see GEGEometryElectronics.cc/.hh)
 *                 o Signal cable connection (one per String -> see GEGEometryElectronics.cc/.hh)
 *                 o HV Vertical part
 *                 o HV Horizontal part
 *                 o signal cable (the one surrounding the crystal)
 * 07-09-2008 (jens)
 *            - re-arrange positions of cables and teflon-cable-holders of phase-2 detectors
 *            - let height of phase-2 detector depend on height of some default dummy-crystal (create a
 *              default phase-2-detector-parameters)
 *            - change names of some solids for better visualization with GDML/ROOT
 * 07-23-2008 (luciano)
 *            - shift water tank towards one wall of the hall A, to reproduce the real configuration
 *            - replace cout with MGLog
 * 07-09-2008 (jens)
 *            - inserted a safety check to make sure that phase-2 crystals have the same geometry as the "phase-2-standard-crystal"(
 * 07-28-2008 (jens)
 *            - set crystal-to-crystal back to 6cm (phase-2)
 *            - change distance between horizontal top-bar of copper holder and crystal top-surface:  5mm --> 7mm
 *            - change distance between horizontal bottom-bar of copper holder and crystal bottom-surface:  5mm --> 7mm
 *            - put some RotMatrices onto the heap instead onto the stack, Jens
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *              --> removed hv connection cable (hv-vertical cable has been extended)
 * 08-25-2008 - Enable volumelist for calibration sources, Francis
 * 04-24-2009 - Switch for lock type incl. new xy coordinates for detectors and calibration source in case of comm lock, Francis
 * 06-14-2011 - added possibility of encapsulation, Sabine
 * 08-30-2011 (Neslihan)
 *            - non-true coaxial geometry is modified to represent the detector geometry with groove and correct DL extensions:
 *              A thick dead layer on the n+ contact covers the outer and bottom surfaces plus the top surface until the groove and
 *              a thin dead layer on the p+ contact covers the bore-hole surface plus the top surface until the groove.
 *              --> DL thicknesses, including the groove dead layer(GeO layer), can be set in the geometry.dat file.
 *              -->!!! to use the old non-true coaxial geometry (without groove), it is enough to set the "grooveheight" to zero in the geometry.dat file.
 * 08-30-2011 - Changed indentation and bracketing for its own consistency, no code change. Alex
 * 10-14-2011 - Fixed the problem of reading the geometry.dat file with ifstream:
 *              an empty new line in the end of file is not correctly treated by ifstream, fixed by treating it manually using sstream. Alex
 * 10-19-2011 (Neslihan)
 *            - heat exchanger added to cryostat as a simple cylindrical geometry. The distance to LAr level is 1.1 m.
 * 05-11-2011 - Added a new lock type to the options based on the preliminary design for Phase II. (N. Barros)
 *              Extended crystal position canculations for new lock.
 * 11-08-2011 - fSuperStructureBeamGridLogical volume was simplified to have only 8 parallel rails, the rest 4
 *              (that were instersecting with the 8) were removed to simplify and speed up a lot the visualization and, maybe, the simulation, Alex
 * 11-17-2011 - Minor change of detector distance in 3-string arm, Bjoern
 * 29-02-2012 - Added the copper shroud, Giovanni Benato
 * 26-03-2012 - Added possibility to insert mini-shrouds. At the moment only the Phase I mini-shrouds (i.e. on the 3-strings arms) can be inserted. Giovanni Benato
 * 27-03-2012 - Added possibility to move each calibration source separately. Giovanni Benato
 * 22-05-2012 - Added possibility to move each calibration source separately in all 3 directions. Giovanni Benato
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 * 26-07-2012 - Mini-Shroud modified such that it works for any number of strings (J.Janicsko)
 *              Mini-Shroud is created automatically around strings that contain more than 2 detcetors
 *              does not work with the 'final' lock, the distance between strings should be > 10.5 cm
 *	        It works only together with CommLock4, PhaseIILock and PhaseII_DN500
 * 14-08-2012 - Added options to build an inner fiber design in LAr hybrid design. (Nuno Barros)
 * 17-01-2013 - Added BEGe design for Phase I string. (B. Lehnert)
 * 25-03-2015 - Possibility to have pairs of detectors encoded in the matrix file, using the syntax "(x,y)".
 *              Still keeps compatibility with old format. Second detector is unused at the moment. Luciano
 * 04-10-2016 - Added possibility to position detectors individually from matrix file.
 *	      - Added function for individual holder bar lenghts.
 * 	      - Removed abort command for type 5 (BEGe) and type 11 (Coax) detectors in one string, for all other types still active.
 *	      - Added solids and logical volumes for cables (HV and Signal) for Phase2 (status December 2015).
 *	      - Flag fFlagAdaptMinishroudToDetctorString set to true to have bigger mini-shroud radius for GTF detectors.
 *	      - Added holes in the mini shrouds for the cables, Janina
 *
 * 04-23-2016 - Added solids and logical volumes for holders from top detectors to electronics plate (stars and bars)
 * 	      - Modified mini-shrouds for Phase2, used if mini-shroud material is nylon, Janina
 * 13-07-2016 - Added more possibilities to move the calibration sources, Janina
 * 28-07-2016, JJ added macro command to set LAr light yield and fiber coverage (Bjoern's code)
 * 14-10-2016 - Copper cap on the calib. source for LAr veto test JJ
 * 21-11-2017, Nylon MiniShroud Height reduced to 440mm and added two glued rings to nylon MS,
 *                fLArInstScaleLArAbsL update default from 1.0 to 0.25, K. v. Sturm
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4EllipticalTube.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4Polyhedra.hh"
#include "G4Polycone.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryDetectorDBMessenger.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "gerdageometry/GEGeometryHolderPhase1Parts.hh"
#include "gerdageometry/GEGeometryHolderPhase2Parts.hh"
#include "gerdageometry/GEGeometryStringStuff.hh"


#include "geometry/MGGeometryBEGeDetector.hh"

#include <sstream>
#include <string>
#include "TString.h"

using namespace CLHEP;
using namespace MaGe;

G4double GEGeometryDetectorDB::SmallNumber = 1.e-4 * mm;

// ---------------------------------------------------------------------------

GEGeometryDetectorDB::GEGeometryDetectorDB():
  fPhase2StandardDetectorParameters(0),
  fCryostatParts(0),
  fStringStuffContainer (nrOfStrings),
  fKaptonConnectionCable(nrOfStrings),
  fCopperLinesConnection(nrOfStrings)
{

  MGLog(debugging) << "START GEGeometryDetectorDB::GEGeometryDetectorDB()" << endlog;
  // reset counters

  fSegmentNumber = 0;
  fCrystalNumber = -1;
  fCrystalGeometry1Number = -1;
  fCrystalGeometry2Number = -1;
  fCrystalGeometry3Number = 0;
  fCrystalGeometry4Number = -1;
  fCrystalGeometry5Number = 0;
  fCrystalGeometry5SlotNumber = 0;
  fCrystalGeometry11Number = 0; //TODO not consistent any longer
  fColumnNumber  = 0;
  fColumnGeom1Number  = 0;
  fColumnGeom2Number  = 0;
  fCalibrationSourceCounter = -1;
  fCollimatedCalibrationSourceCounter = -1;

  // set default values

  fFlagConstructDetector          = true;
  fFlagConstructCrystals          = true;
  fFlagConstructSupport           = true;
  fFlagConstructCabling           = true;
  fFlagConstructShielding         = true;
  fFlagConstructCalibrationSource = false;
  fFlagConstructCalibrationSourceCuCap = false;
  fFlagConstructCollimatedCalibrationSource = false;
  fFlagConstructCopperEncapsulation = false;
  fFlagConstructTeflonEncapsulation = false;
  fFlagConstructMiniShroud = false;
  fFlagAdaptMinishroudToDetctorString = true;//to have a larger shroud radius for the GTF detectors
  fFlagReverseNonTrueCoaxial = true;

  fFlagCrystalMatrixWithZPosFile = true; //flag for individual z position of detectors

  fFlagCryoLiquid                 = false; // false = LAr, true = LN

  fFlagLArInstrumentation         = false;
  fLArInstDesign                  = LArInstr_hybrid;
  fLArInstHybridInnerFiber        = false;
  fFlagLArPhotTrack               = false; // was true
  fLArInstHybridSegmentedFiber    = true; // was false
  fLArInstHybridSegmentedFiberShroudRotation = -7 * deg;

  // shielding
  fHallSizeWidth  = 20*m;
  fHallSizeLength = 80.0*m;
  fHallSizeHeight = 18*m;

  fShieldingWaterTankThickness = 0.7 * cm;
  fShieldingWaterTankHeight    = 890.0 * cm;

  fShieldingWaterRadius        = 500.0 * cm;
  fShieldingWaterHeight        = 840.0 * cm;

  fShieldingAirBufferRadius = fShieldingWaterRadius;
  fShieldingAirBufferHeight = fShieldingWaterTankHeight - 2.0 * fShieldingWaterTankThickness - fShieldingWaterHeight;


  /****************************************new steel cryostat - only two walls ************************************/

  // steel cryostat

  fCryostatParts = new GEGeometryCryostatParts();
  SetCryostatGeometry();


  // copper shielding

  fShieldingCopperShieldingLargeHeight = 7.0 * cm;
  fShieldingCopperShieldingLargeRadius = 300.0 * cm;

  fShieldingCopperShieldingSmallHeight = 6.0 * cm;
  fShieldingCopperShieldingSmallRadius = 200.0 * cm;


  // ugly but: foil thickness is already set here since it is needed for fShieldingFootHeight
  fReflectionFoilThickness = 0.1 * cm;

  // steel foot

  fShieldingFootThickness = 1.2 * mm;
  fShieldingFootOR        = fCryostatParts->GetCryoOuterwall_Outside()->GetRcyl();
  fShieldingFootIR        = fShieldingFootOR - fShieldingFootThickness;
  fShieldingFootHeight    =
      fShieldingWaterTankHeight
      - fShieldingWaterTankThickness
      - fShieldingCopperShieldingLargeHeight
      - fCryostatParts->GetCryoOuterwall_Outside()->GetHneck()
      - fCryostatParts->GetCryoOuterwall_Outside()->GetHtoplid()
      - fCryostatParts->GetCryoOuterwall_Outside()->GetHcyl()
      - fCryostatParts->GetCryoOuterwall_Outside()->GetSlopebotlid()*(fShieldingFootThickness+fReflectionFoilThickness)
      - 2*mm;


  // clean room

  fCleanRoomLengthXY  = 10.0 * m;
  fCleanRoomHeight    =  2.8 * m;
  fCleanRoomThickness =  2.0 * mm;
  fCleanRoomHoleRadius = 0.3 * m;


  fSuperStructureCollarThickness = 1.0 * cm;
  fSuperStructureCollarIR        = fCryostatParts->GetCryoInnerwall_Inside()->GetRneck();
  fSuperStructureCollarOR        = fSuperStructureCollarIR + fSuperStructureCollarThickness;
  //fSuperStructureCollarHeight    = 0.7 * m;
  fSuperStructureCollarHeight    = 0.3 * m;

  fSuperStructureBellowThickness = 0.5 * cm;
  fSuperStructureBellowIR        = 0.5 * 550*mm;
  fSuperStructureBellowOR        = fSuperStructureBellowIR + fSuperStructureBellowThickness;
  //fSuperStructureBellowHeight    = 0.2 * m;
  fSuperStructureBellowHeight    = 0.5 * m;

  fSSFlangeBellowToCollarIR        = fSuperStructureBellowOR;
  fSSFlangeBellowToCollarOR        = fSuperStructureCollarOR;
  fSSFlangeBellowToCollarThickness = fSuperStructureCollarOR-fSuperStructureBellowOR;
  fSSFlangeBellowToCollarHeight    = 4 * cm;

  fSuperStructureFlangeThickness = 2.0 * cm;
  fSuperStructureFlangeOR        = fSuperStructureBellowOR + fSuperStructureFlangeThickness;
  fSuperStructureFlangeIR        = fSuperStructureBellowOR;
  fSuperStructureFlangeHeight    = 2.0 * cm;


  fSuperStructureHeight          = fSuperStructureBellowHeight + fSuperStructureCollarHeight;

  // lock

  fOuterLockRadius    = 0.7 * m;
  fOuterLockHeight    = 2.4 * m;
  fOuterLockThickness = 5.0 * mm;

  fInnerLockRadius    = 0.25 * m;
  fInnerLockHeight    = 2.0 * m;
  fInnerLockThickness = 5.0 * mm;

  fLockCopperPlateRadius = fCleanRoomHoleRadius;
  fLockCopperPlateHeight = 5.0 * cm;

  fLockSteelPlateRadius        = fCleanRoomHoleRadius;
  fLockSteelPlateHeight        = 1.5 * cm;
  fLockSteelPlateZPosRelToLock = 2.0 * m;

  fLockType = LockType_PhaseII_DN500; // was LockType_final



  // phase2 crystals standard size
  fPhase2StandardDetectorParameters = CreatePhase2StandardDetectorParameters();

  // BEGe back-to-back default gap. Applies to type 3 and type 4
  fCrystalPairGap = 5.0*mm;

  // crystal matrix
  fColumnDistanceR        = 9.0 * cm;
  fArrayDepth             = 0.0 * cm;
  fDistancePhase2_CrystalToCrystal = 3.3 * cm; //formerly 6cm
  fDistanceHolderToHolder = // distance between lower bar of layer n and lower bar of layer n + 1
      fPhase2StandardDetectorParameters->GetHeight()
      + fDistancePhase2_CrystalToCrystal;
  fPhase1RadiusPos4       = 0.5 * 377.453 * mm;
  fPhase1RadiusPos5       = 0.5 * 322.235 * mm;

  fSegmentationOffsetAngle = 30.0 * deg;

  //crystal position
  fPositionOfTopCrystalinCryostat = 10*cm;
  fDistanceBetweenHolderBarsPhase2= 0.15*cm; //distance between the holder bars, where they are attached to each other

  // holder
  fHolderUpperBarHeight        =   2.0 * mm;
  fHolderLowerBarHeight        =   3.0 * mm;
  fHolderLowerTrapezoidHeight  =   5.0 * mm;
  fHolderSideBarHeight         = fDistanceHolderToHolder;
  fHolderSideBarRadius         =  42.5 * mm;
  fHolderTeflonTubeHeight      =  12.0 * mm;
  fHolder_DistanceToCrystalTop =  7.0 *mm;
  fHolder_DistanceToCrystalBot =  7.0 *mm;
  fHolderInnerHeight           =  fHolder_DistanceToCrystalTop + fHolder_DistanceToCrystalBot + fPhase2StandardDetectorParameters->GetHeight();

  fHolderSignalCableInnerHeight        = 1.5  * mm;
  fHolderSignalCableOuterHeight        = 4.0  * mm;
  fHolderSignalCableInnerSpanningAngle = 36.0 * deg;
  fHolderSignalCableOuterSpanningAngle = 30.0 * deg;

  fHolderHVCableInnerHeight            = 4.0  * mm;
  fHolderHVCableOuterHeight            = 15.0 * mm;
  fHolderHVCableSpanningAngle          = 78.0 * deg;

  fHolderBarRadiusPhase2 = 1.5*mm; //as in GEGeometryHolderPhase1Parts
  fHolderBarsPosRadPhase2 = 43.5*mm; //as in GEGeometryHolderPhase1Parts
  fGTFHolderBarsPosRadPhase2 = 48.5*mm; //as in GEGeometryHolderPhase1Parts
  fPhase2HolderTubsOuterRadius = 3.0 * mm; //radius of big holder bars from top detectors to electronics plate

  fDefaultHolderPhase1Parts = new GEGeometryHolderPhase1Parts(fCrystalPairGap);
  fDefaultHolderPhase2Parts = new GEGeometryHolderPhase2Parts(fCrystalPairGap);

  // electronics

  fElectronicsDistanceFromCrystal  = 34.05 * cm; // with 34.05 cm the lower side of Junction board is exactly 30 cm away from detector ==> Needs to be checked
  //  fElectronicsDistanceFromCrystal  = 15.0 * cm; // distance between lower edge of board and center of upper crystal
  fElectronicsBoardRadius          = 4.0 * cm;
  fElectronicsBoardHeight          = 1.0 * mm;
  fElectronicsBoxLength            = fElectronicsBoardRadius / 2;
  fElectronicsBoxHeight            = 1.0 * cm;
  fElectronicsSideBar_MinimumHeight        = 27.3 * cm;  // Minimum height of a phase-2-string ElectronicsSideBar

  fElectronicsPhaseIHorizontalBar_InnerWallYThickness = 1.0 * mm;
  fElectronicsPhaseIHorizontalBar_YFrameThickness     = 5.0 * mm;
  fElectronicsPhaseIHorizontalBar_XZFrameThickness    = 1.0 * mm;
  fElectronicsPhaseIHorizontalBar_Width               = 2.0*(fHolderSideBarRadius + 1.*mm);
  fElectronicsPhaseIHorizontalBar_InnerHeight         = 12. * mm;
  fElectronicsPhaseIHorizontalBar_OuterHeight         =  6. * mm;

  fElectronicsJunctionSledHeight  = 54.5 * mm;
  fElectronicsJunctionSledPinsHeight  = 22 * mm;
  fElectronicsJunctionSledSpacerHeight = 10 *mm;
  fElectronicsJunctionSledCablesHeight  = 15 * mm;

  fElectronicsJunctionSledZPlane[0] = 0.* mm;
  fElectronicsJunctionSledZPlane[1] = fElectronicsBoardHeight;

  fElectronicsJunctionSledRMin[0] = 0. * mm;
  fElectronicsJunctionSledRMin[1] = 0. * mm;
  fElectronicsJunctionSledRMin[2] = 0. * mm;
  fElectronicsJunctionSledRMin[3] = 0. * mm;
  fElectronicsJunctionSledRMin[4] = 0. * mm;
  fElectronicsJunctionSledRMin[5] = 0. * mm;

  fElectronicsJunctionSledRMax[0] = 44.0 * mm;
  fElectronicsJunctionSledRMax[1] = 44.0 * mm;
  fElectronicsJunctionSledRMax[2] = 44.0 * mm;
  fElectronicsJunctionSledRMax[3] = 44.0 * mm;
  fElectronicsJunctionSledRMax[4] = 44.0 * mm;
  fElectronicsJunctionSledRMax[5] = 44.0 * mm;

  // cables + cablechain + above Junction sled


  G4double cryoliquidHeightAboveCenter =
      GEGeometryCryostatMassiveShape::GeneralPolyconeZShift - fCryostatParts->CryogasHeightInNeck;
  fElectronicsCableChainDistance = 0 * m;
  fElectronicsCableChainHeight = cryoliquidHeightAboveCenter - (  fElectronicsDistanceFromCrystal + fElectronicsBoardHeight + fElectronicsJunctionSledHeight + fElectronicsJunctionSledPinsHeight + fElectronicsJunctionSledSpacerHeight + fElectronicsJunctionSledCablesHeight + fElectronicsCableChainDistance) - 33 *cm;

  fElectronicsCableChainX = 15.0 * mm;
  fElectronicsCableChainY = 27.9 * mm;

  fElectronicsChainHeight = cryoliquidHeightAboveCenter - (  fElectronicsDistanceFromCrystal + fElectronicsBoardHeight + fElectronicsJunctionSledHeight + fElectronicsJunctionSledPinsHeight + fElectronicsJunctionSledSpacerHeight + fElectronicsJunctionSledCablesHeight) - 33 *cm;

  fElectronicsChainRadius = 0.35 * mm;

  fElectronicsCableHeight = fElectronicsChainHeight;
  fElectronicsCableX = 0.4 * mm;
  fElectronicsCableY = 9   * mm;


 // cables

  fCableThickness   = 25.0 * micrometer;
  fCableLoopHeight  = 1.2 * cm;
  fCableStringWidth = 2.0 * cm;

  fCableCopperLinesThickness      = 0.035 * mm;
  fCableCopperLinesWidth          = 0.300 * mm;
  fCableCopperLinesSpanningAngle  = 14;
  fCableCopperPadsThickness       = 0.070 * mm;
  fCableCopperPadsHeight          = 5 * mm;
  fCableCopperPadsSpanningAngle   = 6;

  fKaptonConnectionCableThickness = 0.025 * mm;
  fKaptonConnectionCableWidth     = 1.6 * cm;
  fKaptonConnectionCableLength    = fElectronicsDistanceFromCrystal - fDistanceHolderToHolder + 2.6*cm;

  fCopperLinesConnectionThickness = 0.025 * mm;
  fCopperLinesConnectionWidth     = 1.4 * cm;
  fCopperLinesConnectionLength    = fKaptonConnectionCableLength;

  fHVCableThickness   = 200.0 * micrometer;
  fHVCableWidth       = 6.0 * mm;


  // Phase-1 Cables
  fPhase1Cable_thickness = 0.1 * mm ;
  fPhase1Cable_width     = 3.0 * mm ;

  // Phase-2 Cables (final integration Dec 2015)
  fPhase2Cable_Thickness = 0.1 * mm;
  fPhase2Cable_WidthOnHolder = 17. * mm;
  fPhase2Cable_LengthOnHolderHV = 17.5 * mm;
  fPhase2Cable_LengthOnHolderSignal = 41.5 * mm;
  fPhase2Cable_WidthFromHolderToElectronicsPlate = 4. * mm;


  //BondPadsAl
  fBondPadAlThickness =  1.0 * 1.e-6 * m;
  fBondPadSpanningAngle = 7.59 * deg;

  // scintillator

  fScintillatorHeight  = 6.0 * cm;
  fScintillatorLengthX = 4.0 * m;
  fScintillatorLengthY = 4.0 * m;
  fScintillatorPosX = 0.0*m;
  fScintillatorPosY = 0.0*m;
  fScintillatorPosZ = 0.0*m;
  fScintillatorAngleZ = 0.0*deg;
  fScintillatorHoleOuterRadius = 40.0*cm;

  fScintillatorHole = false;

  // calibration sources

  fCalibrationSourcePositionZ = -210.0 * mm;
  fCalibrationSourcePositionX_S1 = 106. * mm;
  fCalibrationSourcePositionX_S2 = -145. * mm;
  fCalibrationSourcePositionX_S3 = -145. * mm;
  fCalibrationSourcePositionY_S1 = 100. * mm;
  fCalibrationSourcePositionY_S2 = 197. * mm;
  fCalibrationSourcePositionY_S3 = -197. * mm;
  fCalibrationSourcePositionZ_S1 = 0. * mm;
  fCalibrationSourcePositionZ_S2 = 0. * mm;
  fCalibrationSourcePositionZ_S3 = 0. * mm;

  //Phase2 calibration source with new lock (PhaseII_DN500)
  //position optimized for the calibrations taken before the source change in July 2016
  fCalibrationSourcePhase2DN500Radius = 190.*mm;
  fCalibrationSourcePhase2DN500OffsetX = 17.5*mm*cos(-35.*deg);
  fCalibrationSourcePhase2DN500OffsetY = 17.5*mm*sin(-35.*deg);
  fCalibrationSourcePhase2DN500AngleOffset1 = -2.*deg;//S1
  fCalibrationSourcePhase2DN500AngleOffset2 = -2.*deg;//S2
  fCalibrationSourcePhase2DN500AngleOffset3 = -2.*deg;//S3

  fCalibrationSourcePhase2CoordTrafo=6700.*mm+1460.*mm+15.*mm-30*mm;//trafo from coordinate system of sources to coordinate system of MC

  fCalibrationCylinderHeight  = 6.0 * cm;

  // collimated calibration sources

  fCollimatedCalibrationSourcePositionZ = -210.0 * mm;
  fCollimatedCalibrationSourcePositionX_S1 = 106. * mm;
  fCollimatedCalibrationSourcePositionX_S2 = -145. * mm;
  fCollimatedCalibrationSourcePositionX_S3 = -145. * mm;
  fCollimatedCalibrationSourcePositionY_S1 = 100. * mm;
  fCollimatedCalibrationSourcePositionY_S2 = 197. * mm;
  fCollimatedCalibrationSourcePositionY_S3 = -197. * mm;
  fCollimatedCalibrationSourcePositionZ_S1 = 0. * mm;
  fCollimatedCalibrationSourcePositionZ_S2 = 0. * mm;
  fCollimatedCalibrationSourcePositionZ_S3 = 0. * mm;
  fCollimatedCalibrationCylinderHeight  = 6.0 * cm;
  fCollimatedCalibrationCylinderDiameter  = 3.5 * cm;
  fCollimatedCalibrationCapHeight  = 1.0 * cm;
  fCollimatedCalibrationCapDiameter  = 3.5 * cm;
  fCollimatedCalibrationCapsuleHeight  = 5.0 * mm;
  fCollimatedCalibrationCapsuleDiameter  = 5.0 * mm;
  fCollimatedCalibrationSourceHeight  = 4.0 * mm;
  fCollimatedCalibrationSourceDiameter  = 4.0 * mm;
  fCalibrationCollimationDistance = 1.0 * mm;

  // copper encapsulation

  fCopperEncapsulationOuterRadius = 46.0 * mm;
  fCopperEncapsulationHeight = 107.0 * mm;
  fCopperEncapsulationThickness = 1.5 * mm;
  fCopperEncapsulationRadiusHV = 12.5 * mm;
  fCopperEncapsulationRadiusSignal= 20.0 * mm;

  fCopperEncapsulationColumn = 18;
  fCopperEncapsulationRow = 2;

  // teflon encapsulation

  fTeflonEncapsulationOuterRadius = 45.0 * mm;
  fTeflonEncapsulationHeight = 78.0 * mm;
  fTeflonEncapsulationThickness = 1.5 * mm;
  fTeflonEncapsulationRadiusHV = 12.5 * mm;
  fTeflonEncapsulationRadiusSignal= 20.0 * mm;

  fTeflonEncapsulationCopperOuterRadius = 45.2 * mm;
  fTeflonEncapsulationCopperHeight = 78.4 * mm;
  fTeflonEncapsulationCopperThickness = 0.2 * mm;

  fTeflonEncapsulationColumn = 18;
  fTeflonEncapsulationRow = 0;

  // mini-shroud

  fLArInstMSCoating = LArInstMSCoating_TPB;

  fMiniShroudOuterRadiusPhaseI = 51.5 * mm; // default value for phase i.
  fMiniShroudOuterRadiusCoaxString = 113. * mm /2.; // default value for phase ii coax strings.
  fMiniShroudOuterRadiusBEGeString = 103. * mm /2.; // default value for phase ii bege strings.

  fMiniShroudHeightPhaseI = 480.0 * mm;
  fMiniShroudSideThickness = 0.125 * mm; // was 0.06
  fMiniShroudBaseThickness = 0.125 * mm; // was 0.12
  fMiniShroudTopGluedRingHeight = 2.0 * cm;
  fMiniShroudBottomGluedRingHeight = 2.0 * cm;

  fMiniShroudMaterial = "Nylon"; // was MetalCopper: either copper as std or nylon for NylonHybrid LAr instrumentation

  fMiniShroudColumn = new std::vector<G4int>();
  fMiniShroudRow = new std::vector<G4int>();
  fMiniShroudPosition = new std::vector<G4ThreeVector>();
  fMiniShroudRadiusVector = new std::vector<double>();
  fMiniShroudHeightVector = new std::vector<double>();

  //more detailed mini-shrouds for phaseII
  fMiniShroudHeightPhaseII = 480.0 * mm;
  fMiniShroudHeightPhaseIINylon = 440.0 * mm;
  fMiniShroudPositionPhase2Tub = new std::vector<G4ThreeVector>();
  fMiniShroudPositionPhase2Bottom = new std::vector<G4ThreeVector>();
  fMiniShroudPositionPhase2Top = new std::vector<G4ThreeVector>();
  fMiniShroudPositionPhase2TopGluedRing = new std::vector<G4ThreeVector>();
  fMiniShroudPositionPhase2BottomGluedRing = new std::vector<G4ThreeVector>();

  fFlagMSMaterialCheck = false;
  fMiniShroudHoleRadiusBEGeString =fHolderBarsPosRadPhase2;
  fMiniShroudHoleRadiusCoaxString =fGTFHolderBarsPosRadPhase2;
  fMiniShroudHoleRadiusVector = new std::vector<double>();
  fMiniInDetHeightVector = new std::vector<double>();

  // Cherenkov veto

  fPMTStartingAngle = 0.0 * deg ;
  fPMTFinalAngle    = 360.0 * deg ;

  fPhotocathodeInnerRadius = 10.0 * cm ;
  fPhotocathodeOuterRadius = 10.016 * cm ;
  fPhotocathodeThetaStart = 0.0 * deg;
  fPhotocathodeThetaEnd = 80.0 * deg;

  fPMTInnerRadius = 10.016 * cm;
  fPMTOuterRadius = 10.1 * cm;
  fPMTThetaIn = 0.0 * deg;
  fPMTThetaEnd = 90.0 * deg;

  fPMTSteelConeThickness = 0.4 * cm;
  fPMTSteelConeHeight = 30.0 * cm;
  fPMTSteelConeUpperRmin = fPMTOuterRadius;
  fPMTSteelConeUpperRmax = fPMTOuterRadius + fPMTSteelConeThickness ;
  fPMTSteelConeLowerRmin = fPMTOuterRadius/3.0;
  fPMTSteelConeLowerRmax = fPMTSteelConeLowerRmin + fPMTSteelConeThickness;

  fPMTSteelBottomHeight = 3.0 * cm;
  // //value is needed for the CryoFootHeight -> set it earlier
  // fReflectionFoilThickness = 0.1 * cm;

  //PMT cable tray

  fPMTCableTrayThickness = 1.0*cm;
  fPMTVerticalCableTrayHeight = 24.0*cm;
  fPMTHorizontalCableTrayWidth = 40.0*cm;

  // geometry files

  SetCrystalGeometryFilename("geometry.dat");
  SetCrystalMatrixFilename("matrix_phase_ii_ideal.dat");


  // z shift scheme
  fZShiftScheme = CrystalZShiftScheme_unshifted;
  fZShiftScheme_UnshiftedColumn = 18;
  for(int i=0;i<nrOfStrings;i++) {
      fZShiftScheme_CrystalOffset[i] = 0;
  }

  fNlocks = 0;
  fStringDetectorCenter = NULL;

  // initialize messenger

  // DN500 lock defaults
  fDN500NStrings = 7;
  fDN500StringDistance = 11*cm; // was 17*cm

  fLArInstBuildExtSource = false;
  fLArInstScaleLArAbsL = 0.25; //  was 1.0
  fLArInstScaleLArLY = 1.0;
  fLArInstFiberCoverage = 1.0;
  fLArInstTopPMTscaleFact = 1.0;
  fLArInstScaleMetalRefl = 1.0;


  fMessenger = new GEGeometryDetectorDBMessenger(this);

  MGLog(debugging) << "END GEGeometryDetectorDB::GEGeometryDetectorDB()" << endlog;

}

// ---------------------------------------------------------------------------

GEGeometryDetectorDB::~GEGeometryDetectorDB()
{
  for(unsigned int i=0;i<fHolderPhase1Container.size();i++) delete fHolderPhase1Container.at(i) ;
  fHolderPhase1Container.clear();

  for(unsigned int i=0;i<fHolderPhase2Container.size();i++) delete fHolderPhase2Container.at(i) ;
  fHolderPhase2Container.clear();

  for(unsigned int i=0;i<fStringStuffContainer.size();i++) delete fStringStuffContainer.at(i) ;
  fStringStuffContainer.clear();

  for (G4int iCol=0; iCol<nrOfStrings; iCol++)
	  for (G4int jRow=0; jRow < nrOfRows; ++jRow){ delete fHVCableAtHolderPhase2Logical[iCol][jRow];
	                                               delete fHVCableAtHolderTopPhase2Logical[iCol][jRow];
	   					       delete fHVCableToEPlatePhase2Logical[iCol][jRow];
	   					       delete fHVCableTopToEPlatePhase2Logical[iCol][jRow];
	                                               delete fSignalCableAtHolderPhase2Logical[iCol][jRow];
						       delete fSignalCableAtHolderTopPhase2Logical[iCol][jRow];
	   					       delete fSignalCableToEPlatePhase2Logical[iCol][jRow];
	   					       delete fSignalCableTopToEPlatePhase2Logical[iCol][jRow];
						     }

  delete fHolderCopperLogical;
  delete fHolderTeflonLogical;
  delete fHolderSignalCableLogical;
  delete fHolderHVCableLogical;
  delete fPhase2HolderStarLogical;
  delete fPhase2HolderGreatStarLogical;
  delete fPhase2HolderBarsToEPlateLogical;
  delete fElectronicsBoxLogical;
  delete fElectronicsCableLogical;
  delete fElectronicsCableChainLogical;
  delete fElectronicsChainLogical;
  delete fElectronicsJunctionSledLogical;
  //cable from preamps on junction board to pins
  delete fElectronicsKaptonPreAmpsPinsCableLogical;

  delete fElectronicsJunctionSledPinsLogical;
  delete fElectronicsJunctionSledSpacerLogical;
  delete fElectronicsJunctionSledCablesLogical;
  delete fElectronicsJunctionBoardLogical;

  delete fScintillatorLogical;
  delete fCalibrationCylinderLogical;
  delete fCalibrationSourceInnerLogical;
  delete fCalibrationSourceOuterLogical;
  delete fCollimatedCalibrationCylinderLogical;
  delete fCollimatedCalibrationSourceInnerLogical;
  delete fCollimatedCalibrationSourceOuterLogical;
  delete fCopperEncapsulationLogical;
  delete fTeflonEncapsulationLogical;
  delete fTeflonEncapsulationCopperLogical;
  delete fAirBoxLogical;
  delete fExternalRockLogical;
  delete fWaterTankLogical;
  delete fWaterLogical;
  delete fAirBufferLogical;

  delete fCryostatParts;

  delete fFootLogical;
  delete fCopperShieldingLargeLogical;
  delete fCopperShieldingSmallLogical;
  delete fPhotocathodeLogical;
  delete fPMTSteelConeLogical;
  delete fPMTSteelBottomLogical;
  delete fReflectionFoilWaterTankTubeLogical;
  delete fReflectionFoilWaterTankBottomLogical;
  delete fReflectionFoilCryoFootLogical;
  delete fReflectionFoilCryoFootTopLogical;
  delete fReflectionFoilCryoFootBottomLogical;
  delete fVerticalPMTCableTrayLogical;
  delete fHorizontalPMTCableTrayLogical;

  for(unsigned int i=0;i<fCrystalDeadLayerLogicalContainer.size();i++) delete fCrystalDeadLayerLogicalContainer.at(i) ;
  fCrystalDeadLayerLogicalContainer.clear();

  for(unsigned int i=0;i<fCrystalSegmentLogicalContainer.size();i++) delete fCrystalSegmentLogicalContainer.at(i) ;
  fCrystalSegmentLogicalContainer.clear();

  for(unsigned int i=0;i<fCrystalPassivationLogicalContainer.size();i++) delete fCrystalPassivationLogicalContainer.at(i) ;
  fCrystalPassivationLogicalContainer.clear();

  for(unsigned int i=0;i<fCableLogicalContainer.size();i++) delete fCableLogicalContainer.at(i) ;
  fCableLogicalContainer.clear();

  for(unsigned int i=0;i<fBondPadAlLogicalContainer.size();i++) delete fBondPadAlLogicalContainer.at(i) ;
  fBondPadAlLogicalContainer.clear();


  for(unsigned int i=0;i<fCableCopperLinesLogicalContainer.size();i++) delete fCableCopperLinesLogicalContainer.at(i) ;
  fCableCopperLinesLogicalContainer.clear();


  delete fCleanRoomLogical;
  delete fSuperStructureCollarLogical;
  delete fSuperStructureBellowLogical;
  delete fSuperStructureFlangeLogical;
  delete fSSFlangeBellowToCollarLogical;
  delete fSuperStructureBeamGridLogical;

  delete fOuterLockLogical;
  delete fInnerLockLogical;
  delete fLockCopperPlateLogical;
  delete fLockSteelPlateLogical;

  delete fElectronicsPhaseIHorizontalBarLogical;



  for(unsigned int i=0;i<fKaptonConnectionCable.size();i++) delete fKaptonConnectionCable.at(i) ;
  fKaptonConnectionCable.clear();

  for(unsigned int i=0;i<fCopperLinesConnection.size();i++) delete fCopperLinesConnection.at(i) ;
  fCopperLinesConnection.clear();

  delete fPhase2StandardDetectorParameters;

  delete [] fStringDetectorCenter;


}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::ConstructGEGeometryDetectorDB()
{
  MGLog(debugging) << "START GEGeometryDetectorDB::ConstructGEGeometryDetectorDB()" << endlog;

  DefineMaterials();
  DefineVisualizationAttributes();

  // read crystal geometries from file
  this -> OpenInputFiles();

  CreateHoldersForPhase1();
  CreateHoldersForPhase2();

  Set_ZShiftScheme_CrystalOffset();

  FindUppermostCrystalSurface();

  //region
  G4Region* sensitiveRegion = G4RegionStore::GetInstance()->GetRegion("SensitiveRegion");



  MGLog(debugging) << "START to construct Logical Volumes" << endlog;

  G4double TubsInnerRadius;
  G4double TubsOuterRadius;
  G4double TubsHeight;
  G4double TubsStartAngle;
  G4double TubsSpanningAngle;


  // -------------------------------------------------
  // volume: shielding
  // -------------------------------------------------

  // -------------------------------------------------
  // rock
  // -------------------------------------------------

  G4double rockThickness = 5.0*m;
  G4Box* externalRockBox = new G4Box("externalRockBox",
             (fHallSizeWidth+rockThickness)/2.,
             (fHallSizeLength+rockThickness)/2.,
             (fShieldingWaterTankHeight+fHallSizeHeight+rockThickness)/2.);

  fExternalRockLogical = new G4LogicalVolume(externalRockBox,
               lngsrock,
               "ExternalRockLogical");
  fExternalRockLogical -> SetVisAttributes(G4VisAttributes::Invisible);

  // -------------------------------------------------
  // air box
  // -------------------------------------------------
  G4double tolerance = 1.0*mm;
  G4Box* airBox = new G4Box("airBox",
              (fHallSizeWidth+tolerance)/2.,
              (fHallSizeLength+tolerance)/2.,
              (fHallSizeHeight+tolerance)/2.);

  G4EllipticalTube* airTube = new G4EllipticalTube("ellipAirTube",
              fHallSizeWidth/2.,
              fHallSizeHeight,
              fHallSizeLength/2.);

  G4RotationMatrix* xRot90deg = new G4RotationMatrix();
  xRot90deg->rotateX(pi/2.);
  G4ThreeVector solidTranslation(0,0,-0.5*fHallSizeHeight);
  G4IntersectionSolid* airSolid = new G4IntersectionSolid("airSolid",
              airBox,
              airTube,
              xRot90deg,
              solidTranslation);

  //This trick here is necessary in order to move the origin of the fAirBoxLogical
  //coordinate system in such a way that the z coordinates of all dautgthers have not
  //to be changed and that the fShieldingWaterTankPhysical leans on the Hall A basement
  G4double tankoffset = 0.5 * (fShieldingWaterTankHeight - 2.0 * fShieldingWaterTankThickness - fShieldingWaterHeight);
  G4double rockToTank = 3.0*m; //distance between the WT and the wall
  MGLog(debugging) << "Tank offset: " << tankoffset/cm << " cm " << endlog;
  // In order to have the bottom of the water tank leaning on the Hall A basement:
  // fHallSizeHeight/2 - unionSolidShift.z() = fShieldingWaterTankHeight/2. - tankoffset
  //
  G4ThreeVector unionSolidShift(0.5*fHallSizeWidth-fShieldingWaterRadius-rockToTank,
              0,
              0.5*fHallSizeHeight-0.5*fShieldingWaterTankHeight+tankoffset+tolerance);
  G4Box* airBoxNumber2 = new G4Box("FakeBox",
              1.0*cm,
              1*cm,
              0.5*(fShieldingWaterTankHeight)-tankoffset+1.0*cm);
  G4UnionSolid* airSolid2 = new G4UnionSolid("UnionSolidTest",
              airBoxNumber2,
              airSolid,
              0,
              unionSolidShift);

  fAirBoxLogical = new G4LogicalVolume(airSolid2,
              air,
              "AirBoxLogical");
  G4Colour darkred (0.5,  0.0,  0.0,  0.5);
  fAirBoxLogical -> SetVisAttributes(darkred);

  // -------------------------------------------------
  // water tank
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * cm; ;
  TubsOuterRadius   = fShieldingWaterRadius + fShieldingWaterTankThickness;
  TubsHeight        = 0.5 * fShieldingWaterTankHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* WaterTankOuterTubs = new G4Tubs("WaterTankOuterTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * cm; ;
  TubsOuterRadius   = fShieldingWaterRadius;
  TubsHeight        = 0.5 * fShieldingWaterTankHeight - fShieldingWaterTankThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* WaterTankInnerTubs = new G4Tubs("WaterTankInnerTubs",
            TubsInnerRadius,
            TubsOuterRadius,
            TubsHeight,
            TubsStartAngle,
            TubsSpanningAngle);
  TubsInnerRadius   = 0.0 * cm; ;
  TubsOuterRadius   = fCryostatParts->GetCryoOuterwall_Outside()->GetRneck();
  TubsHeight        = 0.5 * fShieldingWaterTankThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* WaterTankHoleTubs = new G4Tubs("WaterTankHoleTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid* WaterTankSubtractionSolid1 =
    new G4SubtractionSolid("WaterTankSubtractionSolid1",
              WaterTankOuterTubs,
              WaterTankInnerTubs,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));

  G4SubtractionSolid* WaterTankSubtractionSolid2 =
    new G4SubtractionSolid("WaterTankSubtractionSolid2",
              WaterTankSubtractionSolid1,
              WaterTankHoleTubs,
              0,
              G4ThreeVector(0.0, 0.0, 0.5 * (fShieldingWaterTankHeight - fShieldingWaterTankThickness)));

  fWaterTankLogical = new G4LogicalVolume(WaterTankSubtractionSolid2,
              steel,
              "WaterTankLogical");

  fWaterTankLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // water
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fShieldingWaterRadius;
  TubsHeight        = 0.5 * fShieldingWaterHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* WaterTubs = new G4Tubs("WaterTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fCryostatParts->CreateSolids();
  // due to the value of "GEGeometryCryostatMassiveShape::GeneralPolyconeZShift" no shift should be necessary
  G4SubtractionSolid* WaterSubtractionsolid1 =
    new G4SubtractionSolid("WaterSubtractionSolid1",
              WaterTubs ,
              fCryostatParts->GetCryoOuterwall_Outside()->GetSolid(),
              0,
              G4ThreeVector(0.0, 0.0, 0.0));

  fWaterLogical = new G4LogicalVolume(WaterSubtractionsolid1,
              water,
              "fWaterLogical");

  fWaterLogical -> SetVisAttributes(WaterVisAtt);

  // -------------------------------------------------
  // air buffer above the water
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fShieldingAirBufferRadius;
  TubsHeight        = 0.5 * fShieldingAirBufferHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* AirBufferTubs = new G4Tubs("AirBufferTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fCryostatParts->GetCryoOuterwall_Outside()->GetRneck();
  TubsHeight        = 0.5 * fShieldingAirBufferHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* AirBufferHoleTubs = new G4Tubs("AirBufferTubsHole",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid* AirBufferSubtractionSolid =
    new G4SubtractionSolid("AirbufferSubtractionSolid",
              AirBufferTubs,
              AirBufferHoleTubs,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));

  fAirBufferLogical = new G4LogicalVolume(AirBufferSubtractionSolid,
              air,
              "fAirBufferLogical");

  fAirBufferLogical -> SetVisAttributes(G4VisAttributes::Invisible);
  //  fAirBufferLogical -> SetVisAttributes(G4Color(0.0, 1.0, 0.0, 0.5));



  // -------------------------------------------------
  // Cryostat
  // -------------------------------------------------
  // - Keep the function 'SetCryostatGeometry' inside GEGeometryDetectorDB
  //   to have all absolute numbers inside the GEGeometryDetectorDB class
  // - The concrete construction of the solids needs not to be done here
  //   --> put it into class GEGeometryCryostatParts
  // SetCryostatGeometry();
  fCryostatParts->CreateSolids();

  // the same for setting the materials
  fCryostatParts->GetCryoOuterwall_Outside      ()->SetMaterial(steel     );
  fCryostatParts->GetCryoOuterwall_Inside       ()->SetMaterial(vacuum    );
  // in the simulation, there will be no extra layer for the superisolation (to thin)
  //fCryostatParts->GetCryoInnerwallLayer2_Outside()->SetMaterial(vacuum    );
  fCryostatParts->GetCryoInnerwallLayer1_Outside()->SetMaterial(makrolon  );
  fCryostatParts->GetCryoInnerwall_Outside      ()->SetMaterial(steel     );
  fCryostatParts->GetCryoInnerwall_Inside       ()->SetMaterial(cryoliquid);

  fCryostatParts->fMaterialCuLayer            = copper;
  fCryostatParts->fMaterialCuLayerSupportRing = steel ;
  fCryostatParts->fMaterialCryoGas            = cryogas;
  fCryostatParts->fMaterialTorlonRods         = torlon;
  fCryostatParts->fMaterialThermalShield      = copper;
  fCryostatParts->fMaterialHeatExchanger      = copper;
  fCryostatParts->fMaterialShroud = copper;

  // now create the logical volumes
  fCryostatParts->CreateLogicalVolumes();
  MGLog(debugging) << "Logical Volumes of Cryostat are created " <<  endlog;

  // and set vis attributes
  fCryostatParts->GetCryoOuterwall_Outside      ()->GetLogicalVolume()->SetVisAttributes(SteelVisAtt );
  fCryostatParts->GetCryoOuterwall_Inside       ()->GetLogicalVolume()->SetVisAttributes(VacuumVisAtt);
  //fCryostatParts->GetCryoInnerwallLayer2_Outside()->GetLogicalVolume()->SetVisAttributes(VacuumVisAtt);
  fCryostatParts->GetCryoInnerwallLayer1_Outside()->GetLogicalVolume()->SetVisAttributes(MakrolonVisAtt);
  fCryostatParts->GetCryoInnerwall_Outside      ()->GetLogicalVolume()->SetVisAttributes(SteelVisAtt );
  fCryostatParts->GetCryoInnerwall_Inside       ()->GetLogicalVolume()->SetVisAttributes(LN2VisAtt   );
  fCryostatParts->fCryoGasInNeckLogical->SetVisAttributes(N2VisAtt   );
  MGLog(debugging) << "Logical Volumes of Cryostat are created and visualization attributes are set." <<  endlog;





  // -------------------------------------------------
  // cryostat foot
  // -------------------------------------------------

  TubsInnerRadius   = fShieldingFootIR;
  TubsOuterRadius   = fShieldingFootOR;
  TubsHeight        = 0.5 * fShieldingFootHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* FootTubs = new G4Tubs("FootTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fFootLogical = new G4LogicalVolume(FootTubs,
              steel,
              "FootLogical");

  fFootLogical -> SetVisAttributes(SteelVisAtt);


  // -------------------------------------------------
  // large copper shielding
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fShieldingCopperShieldingLargeRadius;
  TubsHeight        = 0.5 * fShieldingCopperShieldingLargeHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CopperShieldingLargeTubs = new G4Tubs("CopperShieldingLargeTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fCopperShieldingLargeLogical = new G4LogicalVolume(CopperShieldingLargeTubs,
                 copper,
                 "CopperShieldingLargeLogical");

  fCopperShieldingLargeLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // small copper shielding
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fShieldingCopperShieldingSmallRadius;
  TubsHeight        = 0.5 * fShieldingCopperShieldingSmallHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CopperShieldingSmallTubs = new G4Tubs("CopperShieldingSmallTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fCopperShieldingSmallLogical = new G4LogicalVolume(CopperShieldingSmallTubs,
                 copper,
                 "CopperShieldingSmallLogical");

  fCopperShieldingSmallLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // volume: clean room
  // -------------------------------------------------

  G4double BoxLengthX = 0.5 * fCleanRoomLengthXY;
  G4double BoxLengthY = 0.5 * fCleanRoomLengthXY;
  G4double BoxLengthZ = 0.5 * fCleanRoomHeight;

  G4Box* CleanRoomBoxOuter = new G4Box("cleanroomouter",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  BoxLengthX = 0.5 * fCleanRoomLengthXY - fCleanRoomThickness;
  BoxLengthY = 0.5 * fCleanRoomLengthXY - fCleanRoomThickness;
  BoxLengthZ = 0.5 * fCleanRoomHeight - fCleanRoomThickness;

  G4Box* CleanRoomBoxInner = new G4Box("cleanroominner",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fCleanRoomHoleRadius;
  TubsHeight        = 0.5 * fCleanRoomThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CleanRoomHole = new G4Tubs("cleanroomholetubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid* CleanRoomBox1 =
    new G4SubtractionSolid("cleanroombox1",
              CleanRoomBoxOuter ,
              CleanRoomBoxInner,
              0,
              G4ThreeVector());

  G4SubtractionSolid* CleanRoomBox2 =
    new G4SubtractionSolid("cleanroombox2",
              CleanRoomBox1,
              CleanRoomHole,
              0,
              G4ThreeVector(0.0, 0.0, - 0.5 * (fCleanRoomHeight - fCleanRoomThickness)));

  fCleanRoomLogical = new G4LogicalVolume(CleanRoomBox2,
              steel,
              "CleanRoomLogical");

  fCleanRoomLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume: outer lock
  // -------------------------------------------------

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fOuterLockRadius;
  TubsHeight        = 0.5 * fOuterLockHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* OuterLockOuterTubs = new G4Tubs("outerlockoutertubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fOuterLockRadius - fOuterLockThickness;
  TubsHeight        = 0.5 * fOuterLockHeight - fOuterLockThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* OuterLockInnerTubs = new G4Tubs("outerlockinnertubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fCleanRoomHoleRadius;
  TubsHeight        = 0.5 * fOuterLockThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* OuterLockHoleTubs = new G4Tubs("outerlockholetubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid* OuterLock1 =
    new G4SubtractionSolid("outerlock1",
              OuterLockOuterTubs,
              OuterLockInnerTubs,
              0,
              G4ThreeVector());

  G4SubtractionSolid* OuterLock2 =
    new G4SubtractionSolid("outerlock2",
              OuterLock1,
              OuterLockHoleTubs,
              0,
              G4ThreeVector(0.0, 0.0, -0.5 * (fOuterLockHeight - fOuterLockThickness)));

  fOuterLockLogical = new G4LogicalVolume(OuterLock2,
              steel,
              "OuterLockLogical");

  fOuterLockLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume: inner lock
  // -------------------------------------------------

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fInnerLockRadius;
  TubsHeight        = 0.5 * fInnerLockHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* InnerLockOuterTubs = new G4Tubs("innerlockoutertubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fInnerLockRadius - fInnerLockThickness;
  TubsHeight        = 0.5 * fInnerLockHeight - fInnerLockThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* InnerLockInnerTubs = new G4Tubs("innerlockinnertubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * cm;
  TubsOuterRadius   = fCleanRoomHoleRadius;
  TubsHeight        = 0.5 * fInnerLockThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


  /*
  G4Tubs* InnerLockHoleTubs = new G4Tubs("innerlockholetubs",
           TubsInnerRadius,
           TubsOuterRadius,
           TubsHeight,
           TubsStartAngle,
           TubsSpanningAngle);
  */

  G4SubtractionSolid* InnerLock1 =
    new G4SubtractionSolid("innerlock1",
              InnerLockOuterTubs,
              InnerLockInnerTubs,
              0,
              G4ThreeVector());

  // G4SubtractionSolid* InnerLock2 =
  //   new G4SubtractionSolid("innerlock2",
  //              InnerLock1,
  //              InnerLockHoleTubs,
  //              0,
  //          G4ThreeVector(0.0, 0.0, -0.5 * (fInnerLockHeight - fInnerLockThickness)));

  fInnerLockLogical = new G4LogicalVolume(InnerLock1,
              steel,
              "InnerLockLogical");

  fInnerLockLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume: super structure collar
  // -------------------------------------------------

  TubsInnerRadius   = fSuperStructureCollarIR;
  TubsOuterRadius   = fSuperStructureCollarOR;
  TubsHeight        = 0.5 * fSuperStructureCollarHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SSCollar = new G4Tubs("sscollarlogical",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fSuperStructureCollarLogical = new G4LogicalVolume(SSCollar,
              steel,
              "SuperStructureCollarLogical");

  fSuperStructureCollarLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume: super structure bellow
  // -------------------------------------------------

  TubsInnerRadius   = fSuperStructureBellowIR;
  TubsOuterRadius   = fSuperStructureBellowOR;
  TubsHeight        = 0.5 * fSuperStructureBellowHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SSBellow = new G4Tubs("ssbellowlogical",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fSuperStructureBellowLogical = new G4LogicalVolume(SSBellow,
              steel,
              "SuperStructureBellowLogical");

  fSuperStructureBellowLogical -> SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume: super structure flange
  // -------------------------------------------------

  TubsInnerRadius   = fSuperStructureFlangeIR;
  TubsOuterRadius   = fSuperStructureFlangeOR;
  TubsHeight        = 0.5 * fSuperStructureFlangeHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SSFlange = new G4Tubs("ssflangelogical",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fSuperStructureFlangeLogical = new G4LogicalVolume(SSFlange,
              steel,
              "SuperStructureFlangeLogical");

  fSuperStructureFlangeLogical -> SetVisAttributes(SteelVisAtt);


  // -------------------------------------------------
  // volume: super structure SSFlangeBellowToCollar
  // -------------------------------------------------

  TubsInnerRadius   = fSSFlangeBellowToCollarIR;
  TubsOuterRadius   = fSSFlangeBellowToCollarOR;
  TubsHeight        = 0.5 * fSSFlangeBellowToCollarHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SSFlangeBellowToCollar_solid = new G4Tubs("ssFlangeBellowToCollarLogical",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fSSFlangeBellowToCollarLogical = new G4LogicalVolume(SSFlangeBellowToCollar_solid,
              steel,
              "SSFlangeBellowToCollarLogical");

  fSSFlangeBellowToCollarLogical -> SetVisAttributes(SteelVisAtt);



  // -------------------------------------------------
  // volume: super structure large beam
  // -------------------------------------------------

  BoxLengthX = 0.5 * 6.0 * cm;
  BoxLengthY = 0.5 * 10.0 * m;
  BoxLengthZ = 0.5 * 2.0 * cm;

  G4Box* BeamLargeTopBox = new G4Box("beamlargetopbox",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  BoxLengthX = 0.5 * 2.0 * cm;
  BoxLengthY = 0.5 * 10.0 * m;
  BoxLengthZ = 0.5 * 11.0 * cm;

  G4Box* BeamLargeMiddleBox = new G4Box("beamlargemiddlebox",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  G4UnionSolid* BeamLargeUnion1 = new G4UnionSolid("beamlargeunion1",
              BeamLargeMiddleBox,
              BeamLargeTopBox,
              0, G4ThreeVector(0.0, 0.0, 6.5 * cm));

  G4UnionSolid* BeamLargeUnion2 = new G4UnionSolid("beamlargeunion2",
              BeamLargeUnion1,
              BeamLargeTopBox,
              0, G4ThreeVector(0.0, 0.0, -6.5 * cm));

  G4UnionSolid* GridUnion1 = new G4UnionSolid("GridUnion1",
              BeamLargeUnion2,
              BeamLargeUnion2,
              0, G4ThreeVector(4.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion2 = new G4UnionSolid("GridUnion2",
              GridUnion1,
              BeamLargeUnion2,
              0, G4ThreeVector(5.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion3 = new G4UnionSolid("GridUnion3",
              GridUnion2,
              BeamLargeUnion2,
              0, G4ThreeVector(6.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion4 = new G4UnionSolid("GridUnion4",
              GridUnion3,
              BeamLargeUnion2,
              0, G4ThreeVector(7.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion5 = new G4UnionSolid("GridUnion5",
              GridUnion4,
              BeamLargeUnion2,
              0, G4ThreeVector(-1.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion6 = new G4UnionSolid("GridUnion6",
              GridUnion5,
              BeamLargeUnion2,
              0, G4ThreeVector(-2.0 * m, 0.0, 0.0));

  G4UnionSolid* GridUnion7 = new G4UnionSolid("GridUnion7",
              GridUnion6,
              BeamLargeUnion2,
              0, G4ThreeVector(-3.0 * m, 0.0, 0.0));

  fSuperStructureBeamGridLogical = new G4LogicalVolume(GridUnion7,
              steel,
              "BeamGridLogical");

  fSuperStructureBeamGridLogical -> SetVisAttributes(SteelVisAtt);


  MGLog(debugging) << "Logical Volumes \"fSuperStructureBeamGridLogical\" is created and visualization attributes are set." <<  endlog;

  // -------------------------------------------------
  // volume: lock copper plate
  // -------------------------------------------------

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fLockCopperPlateRadius;
  TubsHeight        = 0.5 * fLockCopperPlateHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* LockCopperPlate = new G4Tubs("lockcopperplate",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fLockCopperPlateLogical = new G4LogicalVolume(LockCopperPlate,
              copper,
              "LockCopperPlateLogical");

  fLockCopperPlateLogical -> SetVisAttributes(CuVisAtt);

  MGLog(debugging) << "Logical Volumes \"fLockCopperPlateLogical\" is created and visualization attributes are set." <<  endlog;
  // -------------------------------------------------
  // volume: lock steel plate
  // -------------------------------------------------

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fLockSteelPlateRadius;
  TubsHeight        = 0.5 * fLockSteelPlateHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* LockSteelPlate = new G4Tubs("lockcopperplate",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fLockSteelPlateLogical = new G4LogicalVolume(LockSteelPlate,
              copper,
              "LockSteelPlateLogical");

  fLockSteelPlateLogical -> SetVisAttributes(SteelVisAtt);
  // G4VisAttributes* MyVisAtt = new G4VisAttributes(black);
  // MyVisAtt -> SetVisibility(true);
  // MyVisAtt -> SetForceSolid(false);
  // fLockSteelPlateLogical -> SetVisAttributes(MyVisAtt);

  MGLog(debugging) << "Logical Volumes \"fLockSteelPlateLogical\" is created and visualization attributes are set." <<  endlog;
  // -------------------------------------------------
  // volume: crystal
  //
  // geometry : 1 = Non-true coaxial detectors (Phase I)
  //            2 = True coaxial detectors (Phase II)
  //	        3 = Dummy BEGe for LAr instrumentation test
  //		4 = Individual BEGe for BEGe PhaseI string
  //            5 = Individual BEGes for PhaseII
  //           11 = Non-true coaxial detectors (PhaseII)
  // -------------------------------------------------

  char volumename[100];

  //NOTE: The logical volumes are defined here.
  for (G4int i = 0; i < int(fDetectorParametersContainer.size()); i++)
    {

      // ------------------------
      // non-true coaxial volumes
      // ------------------------

      if (fDetectorParametersContainer[i] -> GetGeometry() == 1 ||
	  fDetectorParametersContainer[i] -> GetGeometry() == 3 ||
	  fDetectorParametersContainer[i] -> GetGeometry() == 11)
        {
          // if GrooveHeight is 0 in the gerdageometry/geometry.dat file:
          // no groove, the old non-true coaxial geometry
          if (fDetectorParametersContainer[i] -> GetGrooveHeight() == 0)
            {

              // passivation layer
              // ---------------------
              TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius();;
              TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius();
              TubsHeight        = 0.5 * fDetectorParametersContainer[i] -> GetPassivationThickness();
              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;


              sprintf(volumename, "CrystalPassivationLayer_solid%d", i);
              G4Tubs* CrystalPassivationLayer_solid = new G4Tubs(volumename,
                          TubsInnerRadius,
                          TubsOuterRadius,
                          TubsHeight,
                          TubsStartAngle,
                          TubsSpanningAngle);
              sprintf(volumename, "CrystalPassivationLogical%d", i);
              G4LogicalVolume* logicalvolume = new  G4LogicalVolume(CrystalPassivationLayer_solid,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetPassivationMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalPassivationVisAtt);
              fCrystalPassivationLogicalContainer.push_back(logicalvolume);


              // first construct Deadlayer Polycone then Crystal and afterwards subtract crystal from dead layer

              // dead layer
              // ---------------------
              G4double crytlDLZ[4], crytlDLRi[4], crytlDLRo[4];
              // z
              crytlDLZ[0] = 0.0*mm;
              crytlDLZ[1] = fDetectorParametersContainer[i] -> GetHeight() - fDetectorParametersContainer[i] -> GetBoreDepth();
              crytlDLZ[2] = crytlDLZ[1];
              crytlDLZ[3] = fDetectorParametersContainer[i] -> GetHeight() - fDetectorParametersContainer[i] -> GetPassivationThickness();
              // Ri
              crytlDLRi[0] = 0.0*mm;
              crytlDLRi[1] = 0.0*mm;
              crytlDLRi[2] = fDetectorParametersContainer[i] -> GetInnerRadius();
              crytlDLRi[3] = crytlDLRi[2];
              // Ro
              crytlDLRo[0] = fDetectorParametersContainer[i] -> GetOuterRadius();
              crytlDLRo[1] = crytlDLRo[0];
              crytlDLRo[2] = crytlDLRo[0];
              crytlDLRo[3] = crytlDLRo[0];

              // shift center to z=0
              for(int iii=0;iii<4;iii++) crytlDLZ[iii] -= 0.5* fDetectorParametersContainer[i] -> GetHeight();

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "CrystalDeadLayer_solid_%d", i);
              G4Polycone* CrystalDeadLayer_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          4,
                          crytlDLZ, crytlDLRi, crytlDLRo);




              // active detector volume (no segments)
              // --------------------------------------
              G4double crytlAVZ[4], crytlAVRi[4], crytlAVRo[4];
              // z
              crytlAVZ[0] = crytlDLZ[0] + fDetectorParametersContainer[i]->GetDLBottomThickness();
              crytlAVZ[1] = crytlDLZ[1] - fDetectorParametersContainer[i]->GetDLInnerThickness();
              crytlAVZ[2] = crytlAVZ[1];
              crytlAVZ[3] = crytlDLZ[3] - fDetectorParametersContainer[i]->GetDLTopThickness();
              // Ri
              crytlAVRi[0] = 0.0*mm;
              crytlAVRi[1] = 0.0*mm;
              crytlAVRi[2] = crytlDLRi[2] + fDetectorParametersContainer[i]->GetDLInnerThickness();
              crytlAVRi[3] = crytlAVRi[2] ;
              // Ro
              crytlAVRo[0] = crytlDLRo[0] - fDetectorParametersContainer[i]->GetDLOuterThickness();
              crytlAVRo[1] = crytlAVRo[0];
              crytlAVRo[2] = crytlAVRo[0];
              crytlAVRo[3] = crytlAVRo[0];

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "CrystalActiveVolume_solid_%d", i);
              G4Polycone* CrystalActiveVolume_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          4,
                          crytlAVZ, crytlAVRi, crytlAVRo);
              sprintf(volumename, "CrystalActiveVolumeLogical%d", i);
              logicalvolume = new  G4LogicalVolume(CrystalActiveVolume_solid,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetBulkMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalSegmentVisAtt);
              fCrystalSegmentLogicalContainer.push_back(logicalvolume);


        // subtract Crystal from dead layer

              sprintf(volumename, "CrystalDeadLayer_%d", i);
              G4SubtractionSolid* CrystalDeadLayer = new G4SubtractionSolid(volumename,
                          CrystalDeadLayer_solid,
                          CrystalActiveVolume_solid);


              sprintf(volumename, "CrystalDeadLayerLogical%d", i);
              logicalvolume = new  G4LogicalVolume(CrystalDeadLayer,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetDLMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalDeadLayerVisAtt);
              fCrystalDeadLayerLogicalContainer.push_back(logicalvolume);

              // Do not delete these. Right order is important in these vectors. Otherwise in GEGeometryCable something will go wrong
              fCableLogicalContainer.push_back(NULL);
              fCableCopperLinesLogicalContainer.push_back(NULL);
              fBondPadAlLogicalContainer.push_back(NULL);
            }

          else
            {
              // if GrooveHeight is NOT 0 in the gerdageometry/geometry.dat file:
              // geometry with groove, the new non-true coaxial geometry

              // passivation layer
              // ---------------------
              // these detectors have no passivation, set the passivation layer thickness to 1e-10 (not to 0) in the geometry.dat file
              TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius();
              TubsOuterRadius   = fDetectorParametersContainer[i] -> GetGrooveInnerRadius();
              TubsHeight        = 0.5 * fDetectorParametersContainer[i] -> GetPassivationThickness();
              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;


              sprintf(volumename, "CrystalPassivationLayer_solid%d", i);
              G4Tubs* CrystalPassivationLayer_solid = new G4Tubs(volumename,
                          TubsInnerRadius,
                          TubsOuterRadius,
                          TubsHeight,
                          TubsStartAngle,
                          TubsSpanningAngle);
              sprintf(volumename, "CrystalPassivationLogical%d", i);
              G4LogicalVolume* logicalvolume = new  G4LogicalVolume(CrystalPassivationLayer_solid,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetPassivationMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalPassivationVisAtt);
              fCrystalPassivationLogicalContainer.push_back(logicalvolume);


              // first construct Deadlayer Polycone then Crystal and afterwards subtract crystal from dead layer

              // dead layer
              // ---------------------
              G4double crytlDLZ[4], crytlDLRi[4], crytlDLRo[4];
              // z
              crytlDLZ[0] = 0.0*mm;
              crytlDLZ[1] = fDetectorParametersContainer[i] -> GetHeight() - fDetectorParametersContainer[i] -> GetBoreDepth();
              crytlDLZ[2] = crytlDLZ[1];
              crytlDLZ[3] = fDetectorParametersContainer[i] -> GetHeight();
              // Ri
              crytlDLRi[0] = 0.0*mm;
              crytlDLRi[1] = 0.0*mm;
              crytlDLRi[2] = fDetectorParametersContainer[i] -> GetInnerRadius();
              crytlDLRi[3] = crytlDLRi[2];
              // Ro
              crytlDLRo[0] = fDetectorParametersContainer[i] -> GetOuterRadius();
              crytlDLRo[1] = crytlDLRo[0];
              crytlDLRo[2] = crytlDLRo[0];
              crytlDLRo[3] = crytlDLRo[0];


              // shift center to z=0
              for(int iii=0;iii<4;iii++) crytlDLZ[iii] -= 0.5* fDetectorParametersContainer[i] -> GetHeight();

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "CrystalDeadLayerGr_solid_%d", i);
              G4Polycone* CrystalDeadLayerGr_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          4,
                          crytlDLZ, crytlDLRi, crytlDLRo);


              G4double GrZ[2], GrRi[2], GrRo[2];
              // z
              GrZ[0] = fDetectorParametersContainer[i] -> GetHeight() - fDetectorParametersContainer[i] -> GetGrooveHeight();
              GrZ[1] = fDetectorParametersContainer[i] -> GetHeight();

             // Ri
              GrRi[0] = fDetectorParametersContainer[i] -> GetGrooveInnerRadius();
              GrRi[1] = GrRi[0];

              // Ro
              GrRo[0] = fDetectorParametersContainer[i] -> GetGrooveOuterRadius();
              GrRo[1] = GrRo[0];

              // shift center to z=0
              for(int iii=0;iii<2;iii++) GrZ[iii] -= 0.5* fDetectorParametersContainer[i] -> GetHeight();

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "Gr_solid_%d", i);
              G4Polycone* Gr_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          2,
                          GrZ, GrRi, GrRo);
		//G4Tubs*Gr_solid = new G4Tubs("Gr_solid",GrRi[0], GrRo[0],GrZ[0], 0.,2*M_PI);

              // subtract groove from deadlayer

              sprintf(volumename, "CrystalDeadLayer_solid_%d", i);
              G4SubtractionSolid* CrystalDeadLayer_solid = new G4SubtractionSolid(volumename,
                          CrystalDeadLayerGr_solid,
                          Gr_solid);
		/*G4SubtractionSolid* CrystalDeadLayer_solid = new G4SubtractionSolid(volumename,
                          CrystalDeadLayerGr_solid,
                          Gr_solid,0,G4ThreeVector(0,0,GrZ[1]));*/



              // active detector volume (no segments)
              // --------------------------------------
              G4double crytlAVZ[6], crytlAVRi[6], crytlAVRo[6];
              // z
              crytlAVZ[0] = crytlDLZ[0] + fDetectorParametersContainer[i]->GetDLBottomThickness();
              crytlAVZ[1] = crytlDLZ[1] - fDetectorParametersContainer[i]->GetDLInnerThickness();
              crytlAVZ[2] = crytlAVZ[1];
              crytlAVZ[3] = crytlDLZ[3] - fDetectorParametersContainer[i]->GetDLTopThickness();
              crytlAVZ[4] = crytlAVZ[3];
              crytlAVZ[5] = crytlDLZ[3] - fDetectorParametersContainer[i]->GetDLInnerThickness();


              // Ri
              crytlAVRi[0] = 0.0*mm;
              crytlAVRi[1] = 0.0*mm;
              crytlAVRi[2] = crytlDLRi[2] + fDetectorParametersContainer[i]->GetDLInnerThickness();
              crytlAVRi[3] = crytlAVRi[2] ;
              crytlAVRi[4] = crytlAVRi[2] ;
              crytlAVRi[5] = crytlAVRi[2] ;

              // Ro
              crytlAVRo[0] = crytlDLRo[0] - fDetectorParametersContainer[i]->GetDLOuterThickness();
              crytlAVRo[1] = crytlAVRo[0];
              crytlAVRo[2] = crytlAVRo[0];
              crytlAVRo[3] = crytlAVRo[0];
              crytlAVRo[4] = fDetectorParametersContainer[i] -> GetGrooveInnerRadius();
              crytlAVRo[5] = crytlAVRo[4];

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "CrystalActiveVolumeGr_solid_%d", i);
              G4Polycone* CrystalActiveVolumeGr_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          6,
                          crytlAVZ, crytlAVRi, crytlAVRo);


              G4double GrAZ[2], GrARi[2], GrARo[2];
              // z
              GrAZ[0] = fDetectorParametersContainer[i] -> GetHeight()
                      - fDetectorParametersContainer[i] -> GetGrooveHeight()
                      - fDetectorParametersContainer[i] -> GetGrooveDL();
              GrAZ[1] = fDetectorParametersContainer[i] -> GetHeight();

              // Ri
              GrARi[0] = fDetectorParametersContainer[i] -> GetGrooveInnerRadius()
                       - fDetectorParametersContainer[i] -> GetGrooveDL();
              GrARi[1] = GrARi[0];

              // Ro
              GrARo[0] = fDetectorParametersContainer[i] -> GetGrooveOuterRadius()
                       + fDetectorParametersContainer[i] -> GetGrooveDL();
              GrARo[1] = GrARo[0];



              // shift center to z=0
              for(int iii=0;iii<2;iii++) GrAZ[iii] -= 0.5* fDetectorParametersContainer[i] -> GetHeight();

              TubsStartAngle    = 0.0 * deg;
              TubsSpanningAngle = 360.0 * deg;
              sprintf(volumename, "GrA_solid_%d", i);
              G4Polycone* GrA_solid = new G4Polycone(volumename,
                          TubsStartAngle, TubsSpanningAngle,
                          2,
                          GrAZ, GrARi, GrARo);



              // subtract groove from active volume

              sprintf(volumename, "CrystalActiveVolume_solid_%d", i);
              G4SubtractionSolid* CrystalActiveVolume_solid = new G4SubtractionSolid(volumename,
                          CrystalActiveVolumeGr_solid,
                          GrA_solid);




              sprintf(volumename, "CrystalActiveVolumeLogical%d", i);
              logicalvolume = new  G4LogicalVolume(CrystalActiveVolume_solid,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetBulkMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalSegmentVisAtt);
              fCrystalSegmentLogicalContainer.push_back(logicalvolume);


              // subtract Crystal from dead layer

              sprintf(volumename, "CrystalDeadLayer_%d", i);
              G4SubtractionSolid* CrystalDeadLayer = new G4SubtractionSolid(volumename,
                          CrystalDeadLayer_solid,
                          CrystalActiveVolume_solid);


              sprintf(volumename, "CrystalDeadLayerLogical%d", i);
              logicalvolume = new  G4LogicalVolume(CrystalDeadLayer,
                          G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetDLMaterial()),
                          volumename);
              logicalvolume -> SetVisAttributes(CrystalDeadLayerVisAtt);
              fCrystalDeadLayerLogicalContainer.push_back(logicalvolume);


              // Do not delete these. Right order is important in these vectors. Otherwise in GEGeometryCable something will go wrong
              fCableLogicalContainer.push_back(NULL);
              fCableCopperLinesLogicalContainer.push_back(NULL);
              fBondPadAlLogicalContainer.push_back(NULL);
            }
        }

      // --------------------
      // true coaxial volumes
      // --------------------

      else if (fDetectorParametersContainer[i] -> GetGeometry() == 2)
        {
          // passivation

          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius();
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius();
          TubsHeight        = 0.5 * fDetectorParametersContainer[i] -> GetHeight();
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = 360.0 * deg;

          sprintf(volumename, "PassivationOuterTubs%d", i);

          G4Tubs* CrystalPassivationOuterTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius()
                            + fDetectorParametersContainer[i] -> GetPassivationThickness();
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            - fDetectorParametersContainer[i] -> GetPassivationThickness();
          TubsHeight        = 0.5 * (fDetectorParametersContainer[i] -> GetHeight()
                            - 2.0 * fDetectorParametersContainer[i] -> GetPassivationThickness());
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = 360.0 * deg;

          sprintf(volumename, "PassivationInnerTubs%d", i);

          G4Tubs* CrystalPassivationInnerTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          sprintf(volumename, "PassivationSubtractionSolid%d", i);

          G4SubtractionSolid* CrystalPassivationSubtractionSolid =
            new G4SubtractionSolid(volumename,
                      CrystalPassivationOuterTubs,
                      CrystalPassivationInnerTubs,
                      0,
                      G4ThreeVector(0.0, 0.0, 0.0));

          sprintf(volumename, "CrystalPassivationLogical%d", i);

          G4LogicalVolume* logicalvolume = new  G4LogicalVolume(CrystalPassivationSubtractionSolid,
                      G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetPassivationMaterial()),
                      volumename);

          logicalvolume -> SetVisAttributes(CrystalPassivationVisAtt);

          fCrystalPassivationLogicalContainer.push_back(logicalvolume);


          // dead layer

          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius()
                            + fDetectorParametersContainer[i] -> GetPassivationThickness();
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            - fDetectorParametersContainer[i] -> GetPassivationThickness();
          TubsHeight        = 0.5 * (fDetectorParametersContainer[i] -> GetHeight()
                            - 2.0 * fDetectorParametersContainer[i] -> GetPassivationThickness());
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = 360.0 * deg;

          sprintf(volumename, "CrystalDeadLayerOuterTubs%d", i);

          G4Tubs* CrystalDeadLayerOuterTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius()
                            + fDetectorParametersContainer[i] -> GetPassivationThickness()
                            + fDetectorParametersContainer[i] -> GetDLInnerThickness();
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            - fDetectorParametersContainer[i] -> GetPassivationThickness()
                            - fDetectorParametersContainer[i] -> GetDLOuterThickness();
          TubsHeight        = 0.5 * (fDetectorParametersContainer[i] -> GetHeight()
                            - 2.0 * fDetectorParametersContainer[i] -> GetPassivationThickness()
                            - fDetectorParametersContainer[i] -> GetDLTopThickness()
                            - fDetectorParametersContainer[i] -> GetDLBottomThickness());
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = 360.0 * deg;

          sprintf(volumename, "CrystalDeadLayerInnerTubs%d", i);

          G4Tubs* CrystalDeadLayerInnerTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          sprintf(volumename, "CrystalDeadLayerSubtractionSolid%d", i);

          G4double z = 0.5 * (fDetectorParametersContainer[i] -> GetDLBottomThickness()
                    - fDetectorParametersContainer[i] -> GetDLTopThickness());

          G4ThreeVector translation;

          translation.setX(0.0 * cm);
          translation.setY(0.0 * cm);
          translation.setZ(z);

          G4SubtractionSolid* CrystalDeadLayerSubtractionSolid =
            new G4SubtractionSolid(volumename,
                      CrystalDeadLayerOuterTubs,
                      CrystalDeadLayerInnerTubs,
                      0,
                      translation);

          sprintf(volumename, "CrystalDeadLayerLogical%d", i);

          logicalvolume = new  G4LogicalVolume(CrystalDeadLayerSubtractionSolid,
                      G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetDLMaterial()),
                      volumename);

          logicalvolume -> SetVisAttributes(CrystalDeadLayerVisAtt);

          fCrystalDeadLayerLogicalContainer.push_back(logicalvolume);

          // -------------------------------------------------
          // volume: segments
          // -------------------------------------------------

          G4double dPhi;

          //!!!
          // if (fDetectorParametersContainer[i] -> GetNPhiSegments() == 1)
          dPhi = 360.0 * deg;
          // else
          // dPhi = 360.0 * deg / fDetectorParametersContainer[i] -> GetNPhiSegments();

          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetInnerRadius()
                            + fDetectorParametersContainer[i] -> GetPassivationThickness()
                            + fDetectorParametersContainer[i] -> GetDLInnerThickness();
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            - fDetectorParametersContainer[i] -> GetPassivationThickness()
                            - fDetectorParametersContainer[i] -> GetDLOuterThickness();
          TubsHeight        = 0.5 * fDetectorParametersContainer[i] -> GetHeight()
                            - fDetectorParametersContainer[i] -> GetPassivationThickness()
                            - fDetectorParametersContainer[i]-> GetDLTopThickness();
          //    TubsHeight        = 0.5 * fDetectorParametersContainer[i] -> GetSegmentHeight();
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = dPhi;

          sprintf(volumename, "CrystalSegmentTubs%d", i);

          G4Tubs* CrystalSegmentTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          sprintf(volumename, "CrystalSegmentLogical%d", i);
          logicalvolume = new G4LogicalVolume(CrystalSegmentTubs,
                      G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetBulkMaterial()),
                      volumename);

          logicalvolume -> SetVisAttributes(CrystalSegmentVisAtt);

          fCrystalSegmentLogicalContainer.push_back(logicalvolume);

          // -------------------------------------------------
          // volume: cables
          // -------------------------------------------------
          //!!!  + fBondPadAlThickness
          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            + fBondPadAlThickness + 0.0002 * m;
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsHeight        = 0.5 * fCableLoopHeight;
          TubsStartAngle    = 10.0 * deg;
          TubsSpanningAngle = 360.0 * deg;


          sprintf(volumename, "CableLoopBasis");
          G4Tubs* CableLoopBasis = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);
          G4double  SmallWinAngle = 16.0;
          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            + fBondPadAlThickness + 0.0001 * m;
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius()
                            + fCableThickness + fBondPadAlThickness+ 0.0003 * m;
          TubsHeight        = 0.5 * 7.0 *mm;
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = SmallWinAngle *deg;

          sprintf(volumename, "CableLoopSubtractionSmall");

          G4Tubs* CableLoopSubtractionSmall = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          G4double  BigWinAngle = 22.0;
          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fBondPadAlThickness + 0.0001 * m;
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0003 * m;
          TubsHeight        = 0.5 * 7.0 *mm;
          TubsStartAngle    = 0.0 * deg;
          TubsSpanningAngle = BigWinAngle * deg;

          sprintf(volumename, "CableLoopSubtractionBig");

          G4Tubs* CableLoopSubtractionBig = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          G4RotationMatrix*   windowrot1 = new G4RotationMatrix();
          windowrot1 -> rotateZ( -(30 - (BigWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop1 = new G4SubtractionSolid("G4SubtractionLoop1",
                      CableLoopBasis,
                      CableLoopSubtractionBig,
                      windowrot1,
                      G4ThreeVector());

          G4RotationMatrix*  windowrot2 = new G4RotationMatrix();
          windowrot2 -> rotateZ( -(90 - (BigWinAngle *0.5))* deg);


          G4SubtractionSolid* G4SubtractionLoop2 = new G4SubtractionSolid("G4SubtractionLoop2",
                      G4SubtractionLoop1,
                      CableLoopSubtractionBig,
                      windowrot2,
                      G4ThreeVector());

          G4RotationMatrix*   windowrot3 = new G4RotationMatrix();
          windowrot3 -> rotateZ( -( 150 - (BigWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop3 = new G4SubtractionSolid("G4SubtractionLoop3",
                      G4SubtractionLoop2,
                      CableLoopSubtractionBig,
                      windowrot3,
                      G4ThreeVector());

          G4RotationMatrix*   windowrot4 = new G4RotationMatrix();
          windowrot4 -> rotateZ( -( 210.0 - (BigWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop4 = new G4SubtractionSolid("G4SubtractionLoop4",
                      G4SubtractionLoop3,
                      CableLoopSubtractionBig,
                      windowrot4,
                      G4ThreeVector());
          G4RotationMatrix*   windowrot5 = new G4RotationMatrix();
          windowrot5 -> rotateZ( -( 270 - (BigWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop5 = new G4SubtractionSolid("G4SubtractionLoop5",
                      G4SubtractionLoop4,
                      CableLoopSubtractionBig,
                      windowrot5,
                      G4ThreeVector());


          G4RotationMatrix* windowrot6 = new G4RotationMatrix();
          windowrot6 -> rotateZ( -( 330 - (BigWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop6 = new G4SubtractionSolid("G4SubtractionLoop6",
                      G4SubtractionLoop5,
                      CableLoopSubtractionBig,
                      windowrot6,
                      G4ThreeVector());


          G4RotationMatrix*   windowrot7 = new G4RotationMatrix();
          windowrot7 -> rotateZ( -(30 + 30 - (SmallWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop7 = new G4SubtractionSolid("G4SubtractionLoop7",
                      G4SubtractionLoop6,
                      CableLoopSubtractionSmall,
                      windowrot7,
                      G4ThreeVector());


          G4RotationMatrix*   windowrot8 = new G4RotationMatrix();
          windowrot8 -> rotateZ( -(90 + 30 - (SmallWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop8 = new G4SubtractionSolid("G4SubtractionLoop8",
                      G4SubtractionLoop7,
                      CableLoopSubtractionSmall,
                      windowrot8,
                      G4ThreeVector());
          G4RotationMatrix*   windowrot9 = new G4RotationMatrix();
          windowrot9 -> rotateZ( -(210 + 30 - (SmallWinAngle *0.5)) * deg);


          G4SubtractionSolid* G4SubtractionLoop9 = new G4SubtractionSolid("G4SubtractionLoop9",
                      G4SubtractionLoop8,
                      CableLoopSubtractionSmall,
                      windowrot9,
                      G4ThreeVector());
          G4RotationMatrix*   windowrot10 = new G4RotationMatrix();
          windowrot10 -> rotateZ( -(270 + 30 - (SmallWinAngle *0.5)) * deg);


          G4SubtractionSolid* CableLoop = new G4SubtractionSolid("CableLoop",
                      G4SubtractionLoop9,
                      CableLoopSubtractionSmall,
                      windowrot10,
                      G4ThreeVector());


          TubsInnerRadius    = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsOuterRadius    = TubsInnerRadius + fCableCopperLinesThickness;
          TubsHeight         = fCableCopperLinesWidth * 0.5;
          TubsStartAngle     = 210 * deg;
          TubsSpanningAngle  = 300 * deg;

          sprintf(volumename, "CableCopperLineLong_%d",i);
          G4Tubs* CableCopperLineLong = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          TubsStartAngle     = 270 * deg;
          TubsSpanningAngle  = 180 * deg;

          sprintf(volumename, "CableCopperLineMiddle_%d",i);
          G4Tubs* CableCopperLineMiddle = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          TubsStartAngle     = 330 * deg;
          TubsSpanningAngle  = 60 * deg;

          sprintf(volumename, "CableCopperLineShort_%d",i);
          G4Tubs* CableCopperLineShort = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          sprintf(volumename, "CableCopperLines1_%d",i);
          G4UnionSolid* CableCopperLines1 = new G4UnionSolid(volumename,
                      CableCopperLineShort,
                      CableCopperLineMiddle,
                      0,
                      G4ThreeVector(0,0,0.6*mm));


          sprintf(volumename, "CableCopperLines2_%d",i);
          G4UnionSolid* CableCopperLines2 = new G4UnionSolid(volumename,
                      CableCopperLines1,
                      CableCopperLineLong,
                      0,
                      G4ThreeVector(0,0,1.2*mm));



          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsOuterRadius   = TubsInnerRadius + fCableCopperPadsThickness;
          TubsHeight        = fCableCopperPadsHeight * 0.5;
          TubsSpanningAngle = fCableCopperPadsSpanningAngle / (double) fDetectorParametersContainer[i] -> GetOuterRadius() ;//fCableCopperPadsSpanningAngle is the distance measured[mm] / Radius => Angle
          TubsStartAngle    = -0.5*TubsSpanningAngle;

          sprintf(volumename, "CableCopperPads_%d",i);
          G4Tubs* CableCopperPads = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          G4RotationMatrix* CableCopperRot1 = new G4RotationMatrix();
          CableCopperRot1 -> rotateZ(210 * deg);


          sprintf(volumename, "CableCopperPads1_%d",i);
          G4UnionSolid* CableCopperPads1 = new G4UnionSolid(volumename,
                      CableCopperLines2,
                      CableCopperPads,
                      CableCopperRot1,
                      G4ThreeVector());


          G4RotationMatrix* CableCopperRot2 = new G4RotationMatrix();
          CableCopperRot2 -> rotateZ(270 * deg);


          sprintf(volumename, "CableCopperPads2_%d",i);
          G4UnionSolid* CableCopperPads2 = new G4UnionSolid(volumename,
                      CableCopperPads1,
                      CableCopperPads,
                      CableCopperRot2,
                      G4ThreeVector());

          G4RotationMatrix* CableCopperRot3 = new G4RotationMatrix();
          CableCopperRot3 -> rotateZ(330 * deg);


          sprintf(volumename, "CableCopperPads3_%d",i);
          G4UnionSolid* CableCopperPads3 = new G4UnionSolid(volumename,
                      CableCopperPads2,
                      CableCopperPads,
                      CableCopperRot3,
                      G4ThreeVector());


          G4RotationMatrix* CableCopperRot4 = new G4RotationMatrix();
          CableCopperRot4 -> rotateZ(30 * deg);


          sprintf(volumename, "CableCopperPads4_%d",i);
          G4UnionSolid* CableCopperPads4 = new G4UnionSolid(volumename,
                      CableCopperPads3,
                      CableCopperPads,
                      CableCopperRot4,
                      G4ThreeVector());


          G4RotationMatrix* CableCopperRot5 = new G4RotationMatrix();
          CableCopperRot5 -> rotateZ(90 * deg);


          sprintf(volumename, "CableCopperPads5_%d",i);
          G4UnionSolid* CableCopperPads5 = new G4UnionSolid(volumename,
                      CableCopperPads4,
                      CableCopperPads,
                      CableCopperRot5,
                      G4ThreeVector());


          G4RotationMatrix* CableCopperRot6 = new G4RotationMatrix();
          CableCopperRot6 -> rotateZ(150 * deg);


          sprintf(volumename, "CableCopperLinesBasis_%d",i);
          G4UnionSolid* CableCopperLinesBasis = new G4UnionSolid(volumename,
                      CableCopperPads5,
                      CableCopperPads,
                      CableCopperRot6,
                      G4ThreeVector());



          //    G4double SpanningAngle = fCableCopperLinesSpanningAngle / (double) fDetectorParametersContainer[i] -> GetOuterRadius() ;
          cable_width_deg =
            fCableStringWidth / (M_PI * fDetectorParametersContainer[i] -> GetOuterRadius() + fBondPadAlThickness+ 0.0002 * m) * 180.0 * deg;

          fCableInnerRadius = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsOuterRadius   = TubsInnerRadius + fCableCopperLinesThickness;
          TubsHeight        = 0.5 * fDistanceHolderToHolder - 0.2 *mm; // to prevent overlapping with cable above
          //TubsStartAngle    = - 0.5 * cable_width_deg;
          //TubsSpanningAngle =   cable_width_deg;
          TubsSpanningAngle =  18.*fCableCopperLinesWidth/TubsOuterRadius ;
          TubsStartAngle    = - 0.5 * TubsSpanningAngle;


          sprintf(volumename, "CableCopperLinesConnection_%d",i);
          G4Tubs* CableCopperLinesConnection = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          TubsInnerRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fBondPadAlThickness + 0.0002 * m;
          TubsOuterRadius   = fDetectorParametersContainer[i] -> GetOuterRadius() + fCableThickness + fBondPadAlThickness+ 0.0002 * m;
          TubsHeight        = 0.5 * fDistanceHolderToHolder - 0.2 *mm; // to prevent overlapping with cable above
          TubsStartAngle    = - 0.5 * cable_width_deg;
          TubsSpanningAngle =   cable_width_deg;

          sprintf(volumename, "CableString_%d",i);

          G4Tubs* CableString = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);

          // add volumes

          std::vector <G4UnionSolid*> CableCollection;
          std::vector <G4UnionSolid*> CopperLinesCollection;

          if (fDetectorParametersContainer[i] -> GetNZSegments() > 1)
            {
              // add loops
              for (G4int j = 0; j < fDetectorParametersContainer[i] -> GetNZSegments()-1; j++)
                {
                  char volumename_coli[100];
                  sprintf(volumename     , "cablecollection_%d_%d", j,i);
                  sprintf(volumename_coli, "copperlinescollection_%d_%d", j,i);

                  z = - double(j+1) * fDetectorParametersContainer[i] -> GetSegmentHeight();

		  translation.setX(0.0 * cm);
                  translation.setY(0.0 * cm);
                  translation.setZ(z);

                  G4UnionSolid* unionsolid;
                  G4UnionSolid* linesunionsolid;


                  if (j == 0)
                    {
                      unionsolid = new G4UnionSolid(volumename,
                                  CableLoop,
                                  CableLoop,
                                  0, translation);

                      linesunionsolid = new G4UnionSolid(volumename_coli,
                                  CableCopperLinesBasis,
                                  CableCopperLinesBasis,
                                  0,translation);

                    }

                  else
                    {
                      unionsolid = new G4UnionSolid(volumename,
                                  CableCollection[j-1],
                                  CableLoop,
                                  0, translation);

                      linesunionsolid = new G4UnionSolid(volumename_coli,
                                  CopperLinesCollection[j-1],
                                  CableCopperLinesBasis,
                                  0,translation);
                    }
                  CableCollection.push_back(unionsolid);
                  CopperLinesCollection.push_back(linesunionsolid);
                }

              z = 0.5 * fDistanceHolderToHolder
                - double(CableCollection.size()) * fDetectorParametersContainer[i] -> GetSegmentHeight()
                - 0.5*fCableLoopHeight;

              translation.setZ(z);

              //add copperlines on string

              sprintf(volumename, "CableTheCopperLinesConnection_%d",i);
              G4UnionSolid* linesunionsolid = new G4UnionSolid(volumename,
                          CopperLinesCollection[CopperLinesCollection.size()-1],
                          CableCopperLinesConnection,
                          0,
                          translation);

              CopperLinesCollection.push_back(linesunionsolid);


              // add string


              translation.setX(0.0 * cm);
              translation.setY(0.0 * cm);
              translation.setZ(z);

              sprintf(volumename, "CableCollectionWithoutHV_%d",i);

              G4UnionSolid* unionsolid = new G4UnionSolid(volumename,
                          CableCollection[CableCollection.size()-1],
                          CableString,
                          0, translation);

              CableCollection.push_back(unionsolid);
            }
          else
            {

              MGLog(error) << "This should never happen because there is no geometry-2 crystal with only one segment." << endlog;
              abort();

//
//         Commented and saved for future. abort() will not let to reach this place anyways.
//
//                 z = 0.5 * fDistanceHolderToHolder
//                   - 0.5 * fDetectorParametersContainer[i] -> GetSegmentHeight();
//
//                 translation.setX(0.0 * cm);
//                 translation.setY(0.0 * cm);
//                 translation.setZ(z);
//
//                 sprintf(volumename, "CableCollectionWithoutHV");
//
//                 G4UnionSolid* unionsolid = new G4UnionSolid(volumename,
//                         CableLoop,
//                         CableString,
//                         0, translation);
//
//                 CableCollection.push_back(unionsolid);

            }

          sprintf(volumename, "CableLogical_%d",i);
          logicalvolume = new G4LogicalVolume(CableCollection[CableCollection.size()-1],
                      kapton,
                      volumename);
          logicalvolume -> SetVisAttributes(CuVisAtt);
          fCableLogicalContainer.push_back(logicalvolume);

          sprintf(volumename, "CableCopperLinesLogical_%d",i);
          logicalvolume = new G4LogicalVolume(CopperLinesCollection[CopperLinesCollection.size()-1],
                      copper,
                      volumename);
          logicalvolume -> SetVisAttributes(CuVisAtt);
          fCableCopperLinesLogicalContainer.push_back(logicalvolume);



          TubsInnerRadius = fDetectorParametersContainer[i]->GetOuterRadius()
                          + fDetectorParametersContainer[i]->GetPassivationThickness()
                          + fDetectorParametersContainer[i]->GetDLOuterThickness();

          TubsOuterRadius = fDetectorParametersContainer[i]->GetOuterRadius()
                          + fDetectorParametersContainer[i]->GetPassivationThickness()
                          + fDetectorParametersContainer[i]->GetDLOuterThickness()
                          + fBondPadAlThickness;

          TubsHeight        =  0.5 * 8.5 * mm;
          TubsSpanningAngle =  fBondPadSpanningAngle;
          TubsStartAngle    = -0.5*TubsSpanningAngle;

          sprintf(volumename, "BondPadsAlTubs%d", i);
          G4Tubs* BondPadsAlTubs = new G4Tubs(volumename,
                      TubsInnerRadius,
                      TubsOuterRadius,
                      TubsHeight,
                      TubsStartAngle,
                      TubsSpanningAngle);


          sprintf(volumename, "BondPadsAlLogical%d", i);
          logicalvolume = new G4LogicalVolume(BondPadsAlTubs,
                      aluminium,
                      volumename);
          fBondPadAlLogicalContainer.push_back(logicalvolume);
        }

      // Implementation of type 4 xtals: BEGes w/wo cones
      /* FIXME Note that this is a quick and dirty implementation for the PhaseI BEGes before a more
       * sustainable method is implemented.
       * The original gemetry.dat and old coaxial detector parameterization has been used. 4 old coaxial parameters
       * are miss-used for avoiding to alter the data stream and geometry.dat structure:
       * G4double boredepth; 	------------- corner height
       * G4int    nphisegments; ------------ corner on top (0), bottom (1)
       * G4int    nzsegments;   ------------ groove on top (0), bottom (1)
       * G4double angle;        ------------- corner radius
       *
      */

      else if (fDetectorParametersContainer[i] -> GetGeometry() == 4 ||
               fDetectorParametersContainer[i] -> GetGeometry() == 5)
      {

    	  // #############################
    	  // construct total DL solid
    	  // 1. construct total volume with BEGe class
    	  // 2. construct active volume with BEGe class and DL information
    	  // 3. make TV and AV solids to logical volumes and register in container
    	  // 4. create passivation dummy solid for registration only (not to be placed)

    	  // #############################
    	  // get dead layer volume: top, side and bottom DL seperated

    	  MGLog(debugging) << "Top DL thickness: " <<
    			  fDetectorParametersContainer[i] -> GetDLTopThickness() / mm << " mm" << endlog;

    	  MGLog(debugging) << "Side DL thickness: " <<
    			  fDetectorParametersContainer[i] -> GetDLOuterThickness() / mm << " mm" << endlog;

    	  MGLog(debugging) << "Bottom DL thickness: " <<
    			  fDetectorParametersContainer[i] -> GetDLBottomThickness() / mm << " mm" << endlog;

    	  MGLog(debugging) << "p+ contact DL thickness: " <<
    			  fDetectorParametersContainer[i] -> GetDLInnerThickness() / mm << " mm" << endlog;


    	  // #############################
    	  // construct total volume

    	  sprintf(volumename, "totalBEGeVolume_%i",i);
    	  MGGeometryBEGeDetector* totalBEGe = new MGGeometryBEGeDetector(
    			  volumename);

    	  totalBEGe->SetRadius(fDetectorParametersContainer[i] -> GetOuterRadius());
    	  totalBEGe->SetHeight(fDetectorParametersContainer[i] -> GetHeight());
    	  totalBEGe->SetDitchInnerRadius(fDetectorParametersContainer[i] -> GetGrooveInnerRadius());
    	  totalBEGe->SetDitchOuterRadius(fDetectorParametersContainer[i] -> GetGrooveOuterRadius());
    	  totalBEGe->SetDitchDepth(fDetectorParametersContainer[i] -> GetGrooveHeight());
    	  totalBEGe->SetG4MaterialName(fDetectorParametersContainer[i] -> GetBulkMaterial());

    	  // NOTE: this is the corner radius (radius at narrow end) packed into the old innerRadius parameter
    	  G4double BEGECornerDiameter = fDetectorParametersContainer[i] -> GetSegmentationAngle() * 2.0;
    	  totalBEGe->SetCornerDiameter(BEGECornerDiameter);

       	  // NOTE: this is the corner height (height of cone part) packed into the old boreDepth parameter
    	  G4double BEGECornerHeight = fDetectorParametersContainer[i] -> GetBoreDepth();
    	  totalBEGe->SetCornerHeight(BEGECornerHeight);

    	  G4bool XtalDitchOnBottom; // default is true
    	  if(fDetectorParametersContainer[i] -> GetNPhiSegments() == 1)
    	  	  {XtalDitchOnBottom = false;}
    	  else
    	  	  {XtalDitchOnBottom = true;}
    	  totalBEGe->SetDitchBelow(XtalDitchOnBottom);

    	  G4bool XtalCornerOnBottom; // default is false. only few BEGe where the corner is on bottom
    	  if(fDetectorParametersContainer[i] -> GetNZSegments() == 1)
    	  	  {XtalCornerOnBottom = true;}
    	  else
    	  	  {XtalCornerOnBottom = false;}
    	  totalBEGe->SetCornerOnBottom(XtalCornerOnBottom);

    	  // costruct detector in BEGe class
    	  totalBEGe->ConstructDetector();

    	  // get solid instead of logical volume in order to modify DLs
    	  G4VSolid *BEGeTotalVolumeSolid = totalBEGe->GetDetectorSolid();



    	  // #############################
    	  // construct active volume


     	  sprintf(volumename, "activeBEGeVolume_%i",i);
    	  MGGeometryBEGeDetector* activeBEGe = new MGGeometryBEGeDetector(
    			  volumename);

    	  activeBEGe->SetRadius(fDetectorParametersContainer[i] -> GetOuterRadius());
    	  activeBEGe->SetHeight(fDetectorParametersContainer[i] -> GetHeight());
    	  activeBEGe->SetDitchInnerRadius(fDetectorParametersContainer[i] -> GetGrooveInnerRadius());
    	  activeBEGe->SetDitchOuterRadius(fDetectorParametersContainer[i] -> GetGrooveOuterRadius());
    	  activeBEGe->SetDitchDepth(fDetectorParametersContainer[i] -> GetGrooveHeight());
    	  activeBEGe->SetG4MaterialName(fDetectorParametersContainer[i] -> GetBulkMaterial());
    	  activeBEGe->SetCornerDiameter(BEGECornerDiameter);
    	  activeBEGe->SetCornerHeight(BEGECornerHeight);
    	  if(fDetectorParametersContainer[i] -> GetNPhiSegments() == 1)
    	  	  {XtalDitchOnBottom = false;}
    	  else
    	  	  {XtalDitchOnBottom = true;}
    	  activeBEGe->SetDitchBelow(XtalDitchOnBottom);
    	  if(fDetectorParametersContainer[i] -> GetNZSegments() == 1)
    	  	  {XtalCornerOnBottom = true;}
    	  else
    	  	  {XtalCornerOnBottom = false;}
    	  activeBEGe->SetCornerOnBottom(XtalCornerOnBottom);

    	  //DL information
    	  activeBEGe->SetDLOuterContact(fDetectorParametersContainer[i] -> GetDLTopThickness());
    	  activeBEGe->SetDLInnerContact(fDetectorParametersContainer[i] -> GetDLInnerThickness());

    	  // costruct detector in BEGe class
    	  activeBEGe->ConstructDetector();
    	  G4VSolid *BEGeActiveVolumeSolid = activeBEGe->GetDetectorSolid();




// if one wants to avoid mother daughter dependance between DL and AV.
// Needs to be changed in GEGeometryCrystal.cc (Remember absolute position)
//          // substracting DL solid from TV solid
//    	  sprintf(volumename, "CrystalDeadLayerSolid_%i",i);
//    	  G4SubtractionSolid* CrystalDeadLayerSolid = new G4SubtractionSolid(
//    			  volumename,
//        		  BEGeTotalVolumeSolid,
//        		  BEGeActiveVolumeSolid);




    	  // #############################
          // creating logical volumes and register them in containers

          // DL
    	  sprintf(volumename, "CrystalDeadLayerLogical_%i",i);
          G4LogicalVolume* CrystalDeadLayerLogical = new  G4LogicalVolume(
        		  BEGeTotalVolumeSolid,
        		  G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetDLMaterial()),
        		  volumename);
          CrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt);
          fCrystalDeadLayerLogicalContainer.push_back(CrystalDeadLayerLogical);


          // AV
    	  sprintf(volumename, "CrystalActiveVolumeLogical_%i",i);
          G4LogicalVolume* CrystalActiveVolumeLogical = new G4LogicalVolume(
        		  BEGeActiveVolumeSolid,
                  G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetBulkMaterial()),
                  volumename);
          CrystalActiveVolumeLogical -> SetVisAttributes(CrystalSegmentVisAtt);
          fCrystalSegmentLogicalContainer.push_back(CrystalActiveVolumeLogical);



    	  // #############################
    	  // get passivation volume

    	  // no passivation layer implemented. however it is needed for further processing (registered as SD)
          // create passivation layer as thin volume inside grove

    	  sprintf(volumename, "passivationVolumeSolid_%i",i);
    	  G4Tubs* passivationVolumeSolid = new G4Tubs(volumename,
    			  	  	fDetectorParametersContainer[i] -> GetGrooveInnerRadius(),
                        fDetectorParametersContainer[i] -> GetGrooveOuterRadius(),
                        fDetectorParametersContainer[i] -> GetPassivationThickness() / 2.0,
                        0.*deg,
                        360.*deg);

       	  sprintf(volumename, "passivationVolumeLogical_%i",i);
          G4LogicalVolume* CrystalPassivationVolumeLogical = new  G4LogicalVolume(
        		  	  passivationVolumeSolid,
            		  G4Material::GetMaterial(fDetectorParametersContainer[i] -> GetPassivationMaterial()),
            		  volumename);
          CrystalDeadLayerLogical -> SetVisAttributes(CrystalPassivationVisAtt);
          fCrystalPassivationLogicalContainer.push_back(CrystalPassivationVolumeLogical);


    	  // #############################
          // get other volumes

          // Do not delete these. Right order is important in these vectors. Otherwise in
	  //GEGeometryCable something will go wrong
          fCableLogicalContainer.push_back(NULL);
          fCableCopperLinesLogicalContainer.push_back(NULL);
          fBondPadAlLogicalContainer.push_back(NULL);

      }
    }

  MGLog(debugging) << "Logical Volumes of crystals are created." <<  endlog;


  //SOLIDS for Kapton cable and copper lines connecting Detector and JB
  // -------------------------------------------------------------------

  G4double offsetBetweenKaptonConnectionCables = fKaptonConnectionCableThickness + 0.05*mm + fCopperLinesConnectionThickness + 0.05*mm;

  //  Kapton Cable
  TubsInnerRadius = fCableInnerRadius + 0.1 * mm; //+0.1*mm to avoid overlap
  TubsOuterRadius = fCableInnerRadius + fKaptonConnectionCableThickness + 0.1*mm;//+0.1*mm to avoid overlap
  TubsHeight        =  0.5 * fKaptonConnectionCableLength;
  TubsSpanningAngle = cable_width_deg ;
  TubsStartAngle    = - 0.5*TubsSpanningAngle ;

  G4Tubs* KaptonConnectionCableTubs1 = new G4Tubs("KaptonConnectionCableTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  TubsHeight =  0.5 * (fKaptonConnectionCableLength + fDistanceHolderToHolder);

  G4Tubs* KaptonConnectionCableTubs2 = new G4Tubs("KaptonConnectionCableTubs2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsHeight =  0.5 * (fKaptonConnectionCableLength + 2 * fDistanceHolderToHolder);

  G4Tubs* KaptonConnectionCableTubs3 = new G4Tubs("KaptonConnectionCableTubs3",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  sprintf(volumename, "KaptonConnectionCableUnion1");
  G4UnionSolid* KaptonConnectionCableUnion1 = new G4UnionSolid(volumename,
              KaptonConnectionCableTubs1,
              KaptonConnectionCableTubs2,
              0,
              G4ThreeVector(offsetBetweenKaptonConnectionCables,0,- 0.5 * fDistanceHolderToHolder));


  sprintf(volumename, "KaptonConnectionCableUnion2");
  G4UnionSolid* KaptonConnectionCableUnion2 = new G4UnionSolid(volumename,
              KaptonConnectionCableUnion1,
              KaptonConnectionCableTubs3,
              0,
              G4ThreeVector(2.*offsetBetweenKaptonConnectionCables,0,- fDistanceHolderToHolder));


  //  Kapton Cable's Copper lines
  TubsInnerRadius = fCableInnerRadius + fKaptonConnectionCableThickness + 0.15*mm;//to avoid overlap
  TubsOuterRadius = fCableInnerRadius + fKaptonConnectionCableThickness + fCopperLinesConnectionThickness + 0.15*mm;//to avoid overlap
  TubsHeight =  0.5 * fCopperLinesConnectionLength;
  //TubsSpanningAngle = cable_width_deg ;
  //TubsStartAngle = - 0.5 * TubsSpanningAngle;
  TubsSpanningAngle =  18.*fCableCopperLinesWidth / TubsOuterRadius;
  TubsStartAngle    = - 0.5 * TubsSpanningAngle;

  G4Tubs* CopperLinesConnectionCableTubs1 = new G4Tubs("CopperLinesConnectionCableTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  TubsHeight =  0.5 * (fCopperLinesConnectionLength + fDistanceHolderToHolder);

  G4Tubs* CopperLinesConnectionCableTubs2 = new G4Tubs("CopperLinesConnectionCableTubs2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsHeight =  0.5 * (fCopperLinesConnectionLength + 2 * fDistanceHolderToHolder);

  G4Tubs* CopperLinesConnectionCableTubs3 = new G4Tubs("CopperLinesConnectionCableBox3",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);



  sprintf(volumename, "CopperLinesConnectionCableUnion1");
  G4UnionSolid* CopperLinesConnectionCableUnion1 = new G4UnionSolid(volumename,
              CopperLinesConnectionCableTubs1,
              CopperLinesConnectionCableTubs2,
              0,
              G4ThreeVector(offsetBetweenKaptonConnectionCables,0,- 0.5 * fDistanceHolderToHolder));



  sprintf(volumename, "CopperLinesConnectionCableUnion2");
  G4UnionSolid* CopperLinesConnectionCableUnion2 = new G4UnionSolid(volumename,
              CopperLinesConnectionCableUnion1,
              CopperLinesConnectionCableTubs3,
              0,
              G4ThreeVector(2.*offsetBetweenKaptonConnectionCables,0,- fDistanceHolderToHolder));



  // Logical Volumes of (i) Kapton Cables, (ii) its copperlines, and (iii) HV composite Cable, all connecting detector and JB (phase-2)
  // -------------------------------------------------------------------------------------------------------------------------
  for(int iCol=0;iCol<nrOfStrings;iCol++)
    {
      G4int nPhase2CrystalsInString = 0;
      for(int iRow=0;iRow<nrOfRows;iRow++)
        {
          G4int crtlType = GetCrystalType(iCol,iRow);
          if(crtlType>=0)
            {
              GEGeometryDetectorParameters* aDetector = GetDetectorParameters(crtlType);
              assert(aDetector);
              if( aDetector->GetGeometry() == 2 ) nPhase2CrystalsInString++;
              else break;
            }
        }
      if(nPhase2CrystalsInString==0) continue;

      G4LogicalVolume* tmpLogVol_KaCable;
      G4LogicalVolume* tmpLogVol_KaCableCopperLines;
      char volumename_KaCable           [100];
      char volumename_KaCableCopperLines[100];
      sprintf(volumename_KaCable           , "KaptonConnectionCable_%d",iCol);
      sprintf(volumename_KaCableCopperLines, "CopperLinesConnection_%d",iCol);
      if(     nPhase2CrystalsInString==1)
        {
          tmpLogVol_KaCable = new G4LogicalVolume(KaptonConnectionCableTubs1,
                      kapton,
                      volumename_KaCable);
          tmpLogVol_KaCableCopperLines = new G4LogicalVolume(CopperLinesConnectionCableTubs1,
                      copper,
                      volumename_KaCableCopperLines);
        }
      else if(nPhase2CrystalsInString==2)
        {
          tmpLogVol_KaCable = new G4LogicalVolume(KaptonConnectionCableUnion1,
                      kapton,
                      volumename_KaCable);
          tmpLogVol_KaCableCopperLines = new G4LogicalVolume(CopperLinesConnectionCableUnion1,
                      copper,
                      volumename_KaCableCopperLines);
        }
      else
        {
          tmpLogVol_KaCable = new G4LogicalVolume(KaptonConnectionCableUnion2,
                      kapton,
                      volumename_KaCable);
          tmpLogVol_KaCableCopperLines = new G4LogicalVolume(CopperLinesConnectionCableUnion2,
                      copper,
                      volumename_KaCableCopperLines);
        }

      tmpLogVol_KaCable           -> SetVisAttributes(CuVisAtt);
      tmpLogVol_KaCableCopperLines-> SetVisAttributes(CuVisAtt);

      fKaptonConnectionCable.at(iCol)=tmpLogVol_KaCable;
      fCopperLinesConnection.at(iCol)=tmpLogVol_KaCableCopperLines;
    }



  MGLog(debugging) << "Logical Volumes of some Cables are created." <<  endlog;

  // register volume as "Sensitive Region" for cut-per-region

  for (G4int i = 0; i < int(fCrystalDeadLayerLogicalContainer.size()); i++)
    {
	  sensitiveRegion -> AddRootLogicalVolume(fCrystalPassivationLogicalContainer[i]);
	  sensitiveRegion -> AddRootLogicalVolume(fCrystalDeadLayerLogicalContainer[i]);
	  sensitiveRegion -> AddRootLogicalVolume(fCrystalSegmentLogicalContainer[i]);
    }


  // -------------------------------------------------
  // volumes: holders for phase 1
  // ------------------------------------------------
  // CreateHoldersForPhase1();
  // // this function must be executed earlier,
  // // otherwise one can not proceed with different GEGeometryCrystalZShiftScheme

  // -------------------------------------------------
  // volume: holder phase ii version 2
  // -------------------------------------------------

  // upper bar

  BoxLengthX = 0.5 * 91.0 * mm;
  BoxLengthY = 0.5 *  6.0 * mm;
  BoxLengthZ = 0.5 * fHolderUpperBarHeight;

  G4Box* HolderUpperBarBox = new G4Box("HolderUpperBarBox",
              BoxLengthX ,
              BoxLengthY,
              BoxLengthZ);

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.9 * mm ;
  TubsHeight        = 0.5 * 2.0 * mm;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* HolderSideBarHole = new G4Tubs("HolderSideBarHole",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4double posx = fHolderSideBarRadius;
  G4double posy = 0.0 * mm;
  G4double posz = 0.0 * mm;

  G4ThreeVector holeposition(posx, posy, posz);

  G4SubtractionSolid* G4SubtractionSolid1 = new G4SubtractionSolid("HolderUpperBarSubtractionSolid1",
              HolderUpperBarBox,
              HolderSideBarHole,
              0,
              G4ThreeVector(posx, posy, posz));

  posx = -fHolderSideBarRadius;
  posy =  0.0 * mm;
  posz =  0.0 * mm;

  G4SubtractionSolid* G4SubtractionSolid_upperbar = new G4SubtractionSolid("HolderUpperBarSubtractionSolid1",
              G4SubtractionSolid1,
              HolderSideBarHole,
              0,
              G4ThreeVector(posx, posy, posz));

  // lower bar

  BoxLengthX = 0.5 * 89.0 * mm;
  BoxLengthY = 0.5 *  4.0 * mm;
  BoxLengthZ = 0.5 * fHolderLowerBarHeight;

  G4Box* HolderLowerBarBox = new G4Box("HolderLowerBarBox",
              BoxLengthX ,
              BoxLengthY,
              BoxLengthZ);

  G4SubtractionSolid1 = new G4SubtractionSolid("HolderLowerBarSubtractionSolid1",
              HolderLowerBarBox,
              HolderSideBarHole,
              0,
              G4ThreeVector(fHolderSideBarRadius, 0.0, 0.0));

  G4SubtractionSolid* G4SubtractionSolid_lowerbar = new G4SubtractionSolid("HolderLowerBarSubtractionSolid1",
              G4SubtractionSolid1,
              HolderSideBarHole,
              0,
              G4ThreeVector(-fHolderSideBarRadius, 0.0, 0.0));

  // lower trapezoid

  G4double TrdDX1 = 0.5 * 40.0 * mm;
  G4double TrdDX2 = 0.5 * 83.0 * mm;
  G4double TrdDY1 = 0.5 *  4.0 * mm;
  G4double TrdDY2 = 0.5 *  4.0 * mm;
  G4double TrdDZ  = 0.5 *  fHolderLowerTrapezoidHeight;

  G4Trd* HolderLowerTrapezoid = new G4Trd("HolderLowerTrapezoid",
              TrdDX1,
              TrdDX2,
              TrdDY1,
              TrdDY2,
              TrdDZ);

  // side bars

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.9 * mm;
  TubsHeight        = 0.5 * fHolderSideBarHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* HolderSideBarTubs = new G4Tubs("HolderSideBarTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  // add componentes

  // add lower and upper bar
  G4ThreeVector translation;
  translation.setX(0.);
  translation.setY(0.);
  translation.setZ( - (0.5 * (fHolderUpperBarHeight + fHolderLowerBarHeight) + fHolderInnerHeight));

  G4UnionSolid* G4UnionSolid_bars = new G4UnionSolid("union_bar",
              G4SubtractionSolid_upperbar,
              G4SubtractionSolid_lowerbar,
              0,
              translation);

  // add trapezoid

  translation.setX(0.);
  translation.setY(0.);
  translation.setZ(-(0.5 * fHolderUpperBarHeight + fHolderLowerBarHeight + fHolderInnerHeight + 0.5 * fHolderLowerTrapezoidHeight));

  G4UnionSolid* G4UnionSolid_trapezoid = new G4UnionSolid("union_trapezoid",
              G4UnionSolid_bars,
              HolderLowerTrapezoid,
              0,
              translation);

  // add left bar

  translation.setX(- fHolderSideBarRadius);
  translation.setY(0 * mm);
  translation.setZ(- 0.5 * (fHolderUpperBarHeight + fHolderInnerHeight));

  G4UnionSolid* G4UnionSolid_left = new G4UnionSolid("union_left",
              G4UnionSolid_trapezoid,
              HolderSideBarTubs,
              0,
              translation);

  // add right bar

  translation.setX(fHolderSideBarRadius);
  translation.setY(0.0 * mm);
  translation.setZ(-0.5 * (fHolderUpperBarHeight + fHolderInnerHeight));

  G4UnionSolid* G4UnionSoldid_right = new G4UnionSolid("union_right",
              G4UnionSolid_left,
              HolderSideBarTubs,
              0,
              translation);

  fHolderCopperLogical = new G4LogicalVolume(G4UnionSoldid_right,
              copper,
              "HolderCopperLogical");

  fHolderCopperLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // volume: holder phase ii version 2 : teflon tube
  // -------------------------------------------------

  TubsInnerRadius   = 2.0 * mm;
  TubsOuterRadius   = 4.9 * mm;
  TubsHeight        = 0.5 * fHolderTeflonTubeHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* HolderTeflonTubeTubs = new G4Tubs("HolderTeflonTubeTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  translation.setX(0.0);
  translation.setY(0.0);
  translation.setZ(-(fHolderInnerHeight - fHolderTeflonTubeHeight));

  G4UnionSolid* G4UnionSolid_teflon = new G4UnionSolid("union_teflon",
              HolderTeflonTubeTubs,
              HolderTeflonTubeTubs,
              0,
              translation);

  fHolderTeflonLogical = new G4LogicalVolume(G4UnionSolid_teflon,
              teflon,
              "HolderTeflonLogical");

  fHolderTeflonLogical -> SetVisAttributes(TeflonVisAtt);


  // -------------------------------------------------
  // volume: holder phase ii version 2: Cable guides
  // -------------------------------------------------

  TubsInnerRadius   = 29.0 * mm;
  TubsOuterRadius   = 34.5 * mm;
  TubsHeight        = 0.5  * fHolderSignalCableInnerHeight;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = fHolderSignalCableInnerSpanningAngle;

  G4Tubs* HolderSignalCableTubs1 = new G4Tubs("HolderSignalCableTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  TubsInnerRadius   = 34.4 * mm;
  TubsOuterRadius   = 36.7 * mm;
  TubsHeight        = 0.5  * fHolderSignalCableInnerHeight;
  TubsStartAngle    = 20.0  * deg;
  TubsSpanningAngle = 20.0 * deg;

  G4Tubs* HolderSignalCableTubs2 = new G4Tubs("HolderSignalCableTubs2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 36.6 * mm;
  TubsOuterRadius   = 37.4 * mm;
  TubsHeight        = 0.5  * fHolderSignalCableOuterHeight;
  TubsStartAngle    = 15.0  * deg;
  TubsSpanningAngle = fHolderSignalCableOuterSpanningAngle;

  G4Tubs* HolderSignalCableTubs3 = new G4Tubs("HolderSignalCableTubs3",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4ThreeVector HolderSignal2trans;
  HolderSignal2trans.setX(0. * mm);
  HolderSignal2trans.setY(0.);
  HolderSignal2trans.setZ(0.);

  G4RotationMatrix* HolderSignal2rot = new G4RotationMatrix();
  HolderSignal2rot->rotateZ(0. * deg);

  G4UnionSolid* HolderSignalCableTmp = new G4UnionSolid("HolderSignalCableTmp",
              HolderSignalCableTubs1,
              HolderSignalCableTubs2,
              HolderSignal2rot,
              HolderSignal2trans);

  G4ThreeVector HolderSignal3trans;
  HolderSignal3trans.setX(0. * mm);
  HolderSignal3trans.setY(0.);
  HolderSignal3trans.setZ(0.);

  G4RotationMatrix* HolderSignal3rot = new G4RotationMatrix();
  HolderSignal3rot->rotateZ(0 * deg);

  G4UnionSolid* HolderSignalCable = new G4UnionSolid("HolderSignalCable",
              HolderSignalCableTmp,
              HolderSignalCableTubs3,
              HolderSignal3rot,
              HolderSignal3trans);

  fHolderSignalCableLogical = new G4LogicalVolume(HolderSignalCable,
              teflon,
              "HolderSignalCableLogical");

  fHolderSignalCableLogical -> SetVisAttributes(TeflonVisAtt);


  TubsInnerRadius   = 36.95 * mm;
  TubsOuterRadius   = 38.35 * mm;
  TubsHeight        = 0.5  *  fHolderHVCableInnerHeight;
  TubsStartAngle    = 360* deg - 33 * deg  + 180*deg;
  TubsSpanningAngle = fHolderHVCableSpanningAngle;

  G4Tubs* HolderHVCableTubs = new G4Tubs("HolderHVCableTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  BoxLengthX = 8.0 * 0.5 * mm;
  BoxLengthY = 3.0 * 0.5 * mm;
  BoxLengthZ = fHolderHVCableOuterHeight * 0.5;

  G4Box* HolderHVCableBox = new G4Box("HolderHVCableBox",
              BoxLengthX ,
              BoxLengthY,
              BoxLengthZ);


  G4ThreeVector HolderHVCableBoxtrans;
  HolderHVCableBoxtrans.setX( -(TubsOuterRadius - BoxLengthY) );
  HolderHVCableBoxtrans.setY(0.);
  HolderHVCableBoxtrans.setZ(2.6 * mm);

  G4RotationMatrix* HolderHVCableBoxrot = new G4RotationMatrix();
  HolderHVCableBoxrot->rotateZ(-90 * deg);

  G4UnionSolid* HolderHVCable = new G4UnionSolid("HolderHVCable",
              HolderHVCableTubs,
              HolderHVCableBox,
              HolderHVCableBoxrot,
              HolderHVCableBoxtrans);


  fHolderHVCableLogical = new G4LogicalVolume(HolderHVCable,
              teflon,
              "HolderHVCableLogical");

  fHolderHVCableLogical -> SetVisAttributes(TeflonVisAtt);

  // ---------------------------------------------------------------------------------
  // volume: holder phase ii from top detectors to electronics plate (status dec 2015)
  // ---------------------------------------------------------------------------------

  G4double Phase2HolderStarLengthX = 46.*mm;
  G4double Phase2HolderGreatStarLengthX = 51.*mm;
  G4double Phase2HolderStarLengthY = 4.*mm;
  fPhase2HolderStarLengthZ = 6.*mm;
  G4double Phase2HolderStarHoleInnerRadius = 0.0*mm;
  G4double Phase2HolderStarHoleOuterRadius = fHolderBarRadiusPhase2;
  G4double Phase2HolderStarHoleHeight = fPhase2HolderStarLengthZ ;

  G4double Phase2HolderTubsInnerRadius   = 0.0 * mm;
  G4double Phase2HolderTubsOuterRadius   = fPhase2HolderTubsOuterRadius;

  fPhase2HolderStarPlac = GetArrayDepth() + fPositionOfTopCrystalinCryostat - 0.5*fPhase2HolderStarLengthZ;
  G4double up_Pos = GetZUppermostCrystalSurface();
  G4double low_Pos = GetZLowermostCrystalSurface();
  G4double DistanceFromMiddleArrayToCC3=494*mm;//value calculated from placement of electronic plate in GEGeometryLArInstHybrid.cc
					    //change to function when electronics is moved
  G4double UpperEndHolderBars =0.5*(up_Pos+low_Pos)+DistanceFromMiddleArrayToCC3;//
  G4double LowerEndHolderBars =GetArrayDepth()  + fPositionOfTopCrystalinCryostat;
  fPhase2HolderTubsHeight = UpperEndHolderBars - LowerEndHolderBars;

  //one star and one bar from each string to the electronic plate

  //bar
  G4Tubs* Phase2HolderBarsToEPlate = new G4Tubs("Phase2HolderBarsToEPlate",
              Phase2HolderTubsInnerRadius,
              Phase2HolderTubsOuterRadius,
              0.5*fPhase2HolderTubsHeight,
              0.0* deg, 360.0*deg);

  fPhase2HolderBarsToEPlateLogical = new G4LogicalVolume(Phase2HolderBarsToEPlate,
              copper,
              "fPhase2HolderBarsToEPlateLogical");

  fPhase2HolderBarsToEPlateLogical -> SetVisAttributes(CuVisAtt);


  //star
 G4Box* Phase2HolderStarBox0 = new G4Box("Phase2HolderStarBox0",
              0.5*Phase2HolderStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderStarHole0 = new G4Tubs("Phase2HolderStarHole0",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);

  G4Box* Phase2HolderStarBox1 = new G4Box("Phase2HolderStarBox1",
              0.5*Phase2HolderStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderStarHole1 = new G4Tubs("Phase2HolderStarHole1",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);

  G4Box* Phase2HolderStarBox2 = new G4Box("Phase2HolderStarBox2",
              0.5*Phase2HolderStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderStarHole2 = new G4Tubs("Phase2HolderStarHole2",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);


  G4SubtractionSolid* Phase2HolderStar0 = new G4SubtractionSolid("Phase2HolderStar0",
             Phase2HolderStarBox0,
             Phase2HolderStarHole0,
             0,
             G4ThreeVector(19.3*mm,0.,0.));

  G4SubtractionSolid* Phase2HolderStar1 = new G4SubtractionSolid("Phase2HolderStar1",
             Phase2HolderStarBox1,
             Phase2HolderStarHole1,
             0,
             G4ThreeVector(19.3*mm,0.,0.));

  G4SubtractionSolid* Phase2HolderStar2= new G4SubtractionSolid("Phase2HolderStar2",
             Phase2HolderStarBox2,
             Phase2HolderStarHole2,
             0,
             G4ThreeVector(19.3*mm,0.,0.));

  G4ThreeVector HolderStarBox1trans;
  HolderStarBox1trans.setX(-(sin(30*deg)+1.)*24.2*mm);
  HolderStarBox1trans.setY(cos(30*deg)*24.2*mm);
  HolderStarBox1trans.setZ(0.);

  G4RotationMatrix* HolderStarRot1 = new G4RotationMatrix();
  HolderStarRot1->rotateZ(-120 * deg);

  G4UnionSolid* Phase2HolderStar01= new G4UnionSolid("Phase2HolderStar01",
             Phase2HolderStar0,
             Phase2HolderStar1,
             HolderStarRot1,
             HolderStarBox1trans);

  G4ThreeVector HolderStarBox2trans;
  HolderStarBox2trans.setX(-(sin(30*deg)+1.)*24.2*mm);
  HolderStarBox2trans.setY(-cos(30*deg)*24.2*mm);
  HolderStarBox2trans.setZ(0.);

  G4RotationMatrix* HolderStarRot2 = new G4RotationMatrix();
  HolderStarRot2->rotateZ(-240 * deg);

  G4UnionSolid* Phase2HolderStarCom= new G4UnionSolid("Phase2HolderStarCom",
             Phase2HolderStar01,
             Phase2HolderStar2,
             HolderStarRot2,
             HolderStarBox2trans);

  fPhase2HolderStarLogical = new G4LogicalVolume(Phase2HolderStarCom,
              copper,
              "fPhase2HolderStarLogical");

  fPhase2HolderStarLogical -> SetVisAttributes(CuVisAtt);

   //great star	for natural Coax
  G4Box* Phase2HolderGreatStarBox0 = new G4Box("Phase2HolderGreatStarBox0",
              0.5*Phase2HolderGreatStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderGreatStarHole0 = new G4Tubs("Phase2HolderGreatStarHole0",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);

  G4Box* Phase2HolderGreatStarBox1 = new G4Box("Phase2HolderGreatStarBox1",
              0.5*Phase2HolderGreatStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderGreatStarHole1 = new G4Tubs("Phase2HolderGreatStarHole1",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);

  G4Box* Phase2HolderGreatStarBox2 = new G4Box("Phase2HolderGreatStarBox2",
              0.5*Phase2HolderGreatStarLengthX ,
              0.5*Phase2HolderStarLengthY,
              0.5*fPhase2HolderStarLengthZ);

  G4Tubs* Phase2HolderGreatStarHole2 = new G4Tubs("Phase2HolderGreatStarHole2",
              Phase2HolderStarHoleInnerRadius,
              Phase2HolderStarHoleOuterRadius,
              0.5*Phase2HolderStarHoleHeight,
              0.0* deg, 360.0*deg);


  G4SubtractionSolid* Phase2HolderGreatStar0 = new G4SubtractionSolid("Phase2HolderGreatStar0",
             Phase2HolderGreatStarBox0,
             Phase2HolderGreatStarHole0,
             0,
             G4ThreeVector(21.8*mm,0.,0.));

  G4SubtractionSolid* Phase2HolderGreatStar1 = new G4SubtractionSolid("Phase2HolderGreatStar1",
             Phase2HolderGreatStarBox1,
             Phase2HolderGreatStarHole1,
             0,
             G4ThreeVector(21.8*mm,0.,0.));

  G4SubtractionSolid* Phase2HolderGreatStar2= new G4SubtractionSolid("Phase2HolderGreatStar2",
             Phase2HolderGreatStarBox2,
             Phase2HolderGreatStarHole2,
             0,
             G4ThreeVector(21.8*mm,0.,0.));

  G4ThreeVector HolderGreatStarBox1trans;
  HolderGreatStarBox1trans.setX(-(sin(30*deg)+1.)*26.7*mm);
  HolderGreatStarBox1trans.setY(cos(30*deg)*26.7*mm);
  HolderGreatStarBox1trans.setZ(0.);

  G4RotationMatrix* HolderGreatStarRot1 = new G4RotationMatrix();
  HolderGreatStarRot1->rotateZ(-120 * deg);

  G4UnionSolid* Phase2HolderGreatStar01= new G4UnionSolid("Phase2HolderGreatStar01",
             Phase2HolderGreatStar0,
             Phase2HolderGreatStar1,
             HolderGreatStarRot1,
             HolderGreatStarBox1trans);

  G4ThreeVector HolderGreatStarBox2trans;
  HolderGreatStarBox2trans.setX(-(sin(30*deg)+1.)*26.7*mm);
  HolderGreatStarBox2trans.setY(-cos(30*deg)*26.7*mm);
  HolderGreatStarBox2trans.setZ(0.);

  G4RotationMatrix* HolderGreatStarRot2 = new G4RotationMatrix();
  HolderGreatStarRot2->rotateZ(-240 * deg);

  G4UnionSolid* Phase2HolderGreatStarCom= new G4UnionSolid("Phase2HolderGreatStarCom",
             Phase2HolderGreatStar01,
             Phase2HolderGreatStar2,
             HolderGreatStarRot2,
             HolderGreatStarBox2trans);

  fPhase2HolderGreatStarLogical = new G4LogicalVolume(Phase2HolderGreatStarCom,
              copper,
              "fPhase2HolderGreatStarLogical");

  fPhase2HolderGreatStarLogical -> SetVisAttributes(CuVisAtt);
  // -------------------------------------------------
  // volume: electronics box
  // -------------------------------------------------

  BoxLengthX = fElectronicsBoxLength * 0.5;
  BoxLengthY = fElectronicsBoxLength * 0.5;
  BoxLengthZ = fElectronicsBoxHeight * 0.5;

  G4Box* ElectronicsBox = new G4Box("ElectronicsBox",
              BoxLengthX ,
              BoxLengthY,
              BoxLengthZ);

  fElectronicsBoxLogical = new G4LogicalVolume(ElectronicsBox,
              copper,
              "ElectronicsBoxLogical");

  fElectronicsBoxLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // volume: electronics side bar , also the cables for phase-1
  // -------------------------------------------------
  //FindUppermostCrystalSurface();

  for (G4int iCol = 0; iCol < nrOfStrings; iCol++)
    {
      GEGeometryStringStuff* fffStringStuff(0);
      G4int fffCrystalType = GetCrystalType(iCol,0);
      if(fffCrystalType>=0)
        {
          // there is something hanging in the string
          GEGeometryDetectorParameters* fffDetector = GetDetectorParameters(fffCrystalType);
          assert(fffDetector);

          if(fffDetector->GetGeometry()==1 || fffDetector->GetGeometry()==2 || fffDetector->GetGeometry()==4)
            {
              fffStringStuff = fStringStuffContainer.at(iCol);
              if(fffStringStuff==0)
                {
                  fffStringStuff = new GEGeometryStringStuff();
                  fStringStuffContainer.at(iCol) = fffStringStuff;
                }

              G4double         fffElectronicsSideBarHeight = -DBL_MAX;
              G4double         fffElectronicsSideBarZPosition = -DBL_MAX;
              G4LogicalVolume* fffElectronicsSideBarLogical(0);

              // +-------------------------------------+
              // |   Electronics side bar parameters   |
              // +-------------------------------------+
              GEGeometryHolderPhase1Parts* holder_uppermost = GetHolderPhase1Parts(fCrystalTypeOfUppermostCrystalSurface);
              G4double crystalPosZ = GetCrystalPosition(iCol,0).getZ();

              // same holders for type 1 and type 4 xtlas (Phase I BEGe string)
              if((fffDetector->GetGeometry()==1)||(fffDetector->GetGeometry()==4))
                {
                  // Position of horizontal bar
                  GEGeometryHolderPhase1Parts* holder = GetHolderPhase1Parts( fffCrystalType );
                  assert( holder );
                  G4double ElectronicsPhaseIHorizontalBar_Z = crystalPosZ;
                  ElectronicsPhaseIHorizontalBar_Z += holder->GetZDistanceToSuspensionPlaneTop();
                  ElectronicsPhaseIHorizontalBar_Z += (holder->GetZzHldStarLowerTube(0)-
						       holder->GetZzHldStarLowerTube(6));

                  // height of ElectronicsSideBar
                  if(fCrystalGeometryOfUppermostCrystalSurface==1 || fCrystalGeometryOfUppermostCrystalSurface==4)
                    {
                      G4double delta_zPosCrystal =
                          GetCrystalPosition(fColumnWithUppermostCrystalSurface,0).getZ()
                          - crystalPosZ;
                      G4double delta_zPosSuspension =
                          - (holder->GetZDistanceToSuspensionPlaneTop())
                          + delta_zPosCrystal
                          +  holder_uppermost->GetZDistanceToSuspensionPlaneTop();
                      fffElectronicsSideBarHeight = fElectronicsSideBar_MinimumHeight + delta_zPosSuspension;
                    }
                  else if(fCrystalGeometryOfUppermostCrystalSurface==2)
                    {
                      G4double tmpZ;
                      tmpZ = fElectronicsSideBar_MinimumHeight;
                      tmpZ += 0.5 * fHolderSideBarHeight;
                      tmpZ += (GetCrystalPosition(fColumnWithUppermostCrystalSurface,0).getZ()  -  crystalPosZ);
                      tmpZ -= (ElectronicsPhaseIHorizontalBar_Z - crystalPosZ);
                      fffElectronicsSideBarHeight = tmpZ;
                    }
                  fffElectronicsSideBarZPosition = ElectronicsPhaseIHorizontalBar_Z + 0.5*fffElectronicsSideBarHeight;
                }

              if(fffDetector->GetGeometry()==2)
                {
                  // height of ElectronicsSideBar
                  if(fCrystalGeometryOfUppermostCrystalSurface==2)
                    {
                      G4double tmpZ;
                      tmpZ = fElectronicsSideBar_MinimumHeight;
                      tmpZ += 0.5 * fHolderSideBarHeight; // from holder "Uppermost"
                      tmpZ += (GetCrystalPosition(fColumnWithUppermostCrystalSurface,0).getZ()  -  crystalPosZ);
                      tmpZ -= 0.5 * fHolderSideBarHeight; // from holder "iCol"
                      fffElectronicsSideBarHeight = tmpZ;
                    }
                  else if(fCrystalGeometryOfUppermostCrystalSurface==1 || fCrystalGeometryOfUppermostCrystalSurface==4)
                    {
                      G4double tmpZ;
                      tmpZ = fElectronicsSideBar_MinimumHeight;
                      tmpZ += (holder_uppermost->GetZzHldStarLowerTube(0)-
			       holder_uppermost->GetZzHldStarLowerTube(6));// from holder "Uppermost"
                      tmpZ += holder_uppermost->GetZDistanceToSuspensionPlaneTop();                               // from holder "Uppermost"
                      tmpZ += (GetCrystalPosition(fColumnWithUppermostCrystalSurface,0).getZ()  -  crystalPosZ);
                      tmpZ -= 0.5 * fHolderSideBarHeight; // from holder "iCol"
                      fffElectronicsSideBarHeight = tmpZ;
                    }
                  fffElectronicsSideBarZPosition = crystalPosZ +0.5*fHolderSideBarHeight + 0.5*fffElectronicsSideBarHeight;
                }
              // ElectronicsSideBar - logical Volume
              TubsInnerRadius   = 0.0 * mm;
              TubsOuterRadius   = 0.9 * mm;
              TubsHeight        = 0.5 * fffElectronicsSideBarHeight;
              TubsStartAngle    = 0.0   * deg;
              TubsSpanningAngle = 360.0 * deg;
              char tmpVolName[100];
              sprintf(tmpVolName,"ElectronicsSideBarTubs_%d",iCol);
              G4Tubs* ElectronicsSideBarTubs = new G4Tubs(tmpVolName,
                          TubsInnerRadius,
                          TubsOuterRadius,
                          TubsHeight,
                          TubsStartAngle,
                          TubsSpanningAngle);
              sprintf(tmpVolName,"ElectronicsSideBarLogical_%d",iCol);
              fffElectronicsSideBarLogical = new G4LogicalVolume(ElectronicsSideBarTubs,
                          copper,
                          tmpVolName);
              fffElectronicsSideBarLogical -> SetVisAttributes(CuVisAtt);


              fffStringStuff->SetElectronicsSideBarZPosition(fffElectronicsSideBarZPosition);
              fffStringStuff->SetElectronicsSideBarHeight(fffElectronicsSideBarHeight);
              fffStringStuff->SetElectronicsSideBarLogical(fffElectronicsSideBarLogical);
              fffStringStuff->SetElectronicsSideBarXOffset(  GetHolderSideBarRadius()  );
              MGLog(debugging) << "\""<<tmpVolName<<"\" has been created." << endlog;


              // +-------------------------------------+
              // |   cables for phase 1                |
              // +-------------------------------------+
              G4int nPhase1CrystalsInString = 0;
              G4int nPhase2CrystalsInString = 0;
              for(int iRow=0;iRow<nrOfRows;iRow++)
                {
                  //std::cout << "iRow="<<iRow<<std::endl;
                  G4int crtlType = GetCrystalType(iCol,iRow);
                  if(crtlType>=0)
                    {
                      GEGeometryDetectorParameters* aDetector = GetDetectorParameters(crtlType);
                      assert(aDetector);
                      if( aDetector->GetGeometry() == 1 || aDetector->GetGeometry() == 4) nPhase1CrystalsInString++;
                      if( aDetector->GetGeometry() == 2 ) nPhase2CrystalsInString++;
                    }
                }

              if(nPhase1CrystalsInString!=0)
                {

                  G4Box* Phase1Cable_Solid_ElectronicsSideBar = new G4Box("aName",
                              0.5*fPhase1Cable_width,
                              0.5*fPhase1Cable_thickness,
                              0.5*fffElectronicsSideBarHeight);

                  // Create list of ThreeVectors
                  GEGeometryHolderPhase1Parts* tmpHld;
                  G4ThreeVector tmpVecCablePhase1;
                  std::vector<G4ThreeVector> points_LowerContact(12); // a maximum of 12 points (assuming there are 5 rows of detectors)
                  //std::vector<G4ThreeVector> points_UpperContact(10);
                  std::vector<G4ThreeVector> points_UpperContact_Add(10);

                  MGLog(debugging) << " Create List of Vectors" << endlog;
                  // create a list of ThreeVectors
                  // top electronics side bar
                  tmpVecCablePhase1.setX( GetCrystalPosition(iCol,0).getX() + fffStringStuff->GetElectronicsSideBarXOffset() );
                  tmpVecCablePhase1.setZ( fffStringStuff->GetElectronicsSideBarZPosition() + 0.5 * fffStringStuff->GetElectronicsSideBarHeight() );
                  points_LowerContact.at(0) = tmpVecCablePhase1 ;

                  // bottom electronics side bar
                  tmpVecCablePhase1.setZ( fffStringStuff->GetElectronicsSideBarZPosition() -0.5 * fffStringStuff->GetElectronicsSideBarHeight() );
                  points_LowerContact.at(1) = tmpVecCablePhase1 ;

                  for(int iRow=0;iRow<nPhase1CrystalsInString;iRow++)
                    {
                      // 1.) Lower contact
                      // --------------------
                      int tmpIndex = 2 + iRow*2;

                      // top of crystal iRow
                      MGLog(debugging) << " A) Create List of Vectors (icol="<<iCol<<", irow="<<iRow<<")" << endlog;
                      tmpHld = GetHolderPhase1Parts(  GetCrystalType(iCol,iRow)  );
                      tmpVecCablePhase1.setX( GetCrystalPosition(iCol,iRow).getX()
					      + tmpHld->GetRadialDistance_CrystalSurface_Holder()
					      + tmpHld->GetMaximalAllowedCrystalRadius() );
                      tmpVecCablePhase1.setZ( GetCrystalPosition(iCol,iRow).getZ()
					      + 0.5*tmpHld->GetCrystalHeight()
					      + tmpHld->GetTopDistance_CrystalSurface_Holder() );
                      points_LowerContact.at(tmpIndex) = tmpVecCablePhase1 ;

                      // bottom of crystal iRow
                      MGLog(debugging) << " B) Create List of Vectors (icol="<<iCol<<", irow="<<iRow<<")" << endlog;
                      tmpVecCablePhase1.setX( GetCrystalPosition(iCol,iRow).getX()
					      + tmpHld->GetRadialDistance_CrystalSurface_Holder()
					      + tmpHld->GetMaximalAllowedCrystalRadius() );
                      tmpVecCablePhase1.setZ( GetCrystalPosition(iCol,iRow).getZ()
					      - 0.5*tmpHld->GetCrystalHeight()
					      - 0.5*tmpHld->GetBotDistance_CrystalSurface_Holder() );
                      points_LowerContact.at(tmpIndex + 1) =  tmpVecCablePhase1 ;


                      // 2.) Upper contact
                      // --------------------
                      tmpVecCablePhase1 = points_LowerContact.at(tmpIndex-1);
                      tmpVecCablePhase1.setZ(  tmpVecCablePhase1.getZ()
                          - 0.5*(points_LowerContact.at(tmpIndex-1).getZ() - points_LowerContact.at(tmpIndex).getZ() )  );
                      points_UpperContact_Add.at(0+iRow*2) = tmpVecCablePhase1;

                      tmpVecCablePhase1.setX(  GetCrystalPosition(iCol,iRow).getX() +  1.0*cm  );
                      points_UpperContact_Add.at(1+iRow*2) = tmpVecCablePhase1;

                    }
                  MGLog(debugging) << " List of Vectors is created" << endlog;


                  MGLog(debugging) << " points_LowerContact:" << endlog;
                  for(int ii=0;ii<2 + 2*nPhase1CrystalsInString;ii++)
                    {
                      MGLog(debugging) << "ii="<<ii<<"  "<<points_LowerContact.at(ii) << endlog;
                    }
                  MGLog(debugging) << " points_UpperContact:" << endlog;
                  for(int ii=0;ii<2*nPhase1CrystalsInString;ii++)
                    {
                      MGLog(debugging) << "ii="<<ii<<"  "<<points_UpperContact_Add.at(ii) << endlog;
                    }

                  G4double tmpX = 0.5*fPhase1Cable_width;
                  G4double tmpY = 0.5*fPhase1Cable_thickness;
                  char tmpnameCbl[100];
                  G4UnionSolid*   solid_Cable_LowerPart[5];
                  G4UnionSolid*   solid_Cable_LowerPart_final[5];
                  G4UnionSolid*   solid_Cable_UpperPart[5];
                  G4ThreeVector vecTot_center10 = 0.5*(points_LowerContact[1] + points_LowerContact[0]);
                  // Solids of Lower Contact Cable
                  MGLog(debugging) << " Create List of Solids" << endlog;
                  for(int iRow=0;iRow<nPhase1CrystalsInString;iRow++)
                    {
                      G4ThreeVector transVec;

                      G4ThreeVector vec_2to1 = points_LowerContact[2+2*iRow] - points_LowerContact[1+2*iRow];
                      G4ThreeVector vec_3to2 = points_LowerContact[3+2*iRow] - points_LowerContact[2+2*iRow];
                      G4ThreeVector vec_center10 = 0.5*(points_LowerContact[1+2*iRow] + points_LowerContact[0+2*iRow]);
                      G4ThreeVector vec_center21 = 0.5*(points_LowerContact[2+2*iRow] + points_LowerContact[1+2*iRow]);
                      G4ThreeVector vec_center32 = 0.5*(points_LowerContact[3+2*iRow] + points_LowerContact[2+2*iRow]);

                      // Create solids for Cable_LowerPart
                      // ----------------------------------

                      // create a first box
                      sprintf( tmpnameCbl , "Cable_Box1Lower_String%d_row%d" , iCol , iRow );
                      G4Box* CblAboveDet = new G4Box(tmpnameCbl, 0.5*tmpX, 0.5*tmpY, 0.5*(vec_2to1.mag()) );

                      // add first box to previous solid
                      G4RotationMatrix rotmat;
                      G4double angle = std::atan( vec_2to1.getX() / vec_2to1.getZ() );
                      rotmat.rotateY(  angle  );
                      transVec = vec_center21 - vecTot_center10 ;
                      sprintf( tmpnameCbl , "Cable_Union1Lower_String%d_row%d" , iCol , iRow );
                      G4UnionSolid* CblUnion1Lower;

                      if(iRow==0)
                          CblUnion1Lower = new G4UnionSolid(tmpnameCbl,
                                      Phase1Cable_Solid_ElectronicsSideBar,
                                      CblAboveDet,
                                      G4Transform3D(rotmat,transVec)  );
                      else
                          CblUnion1Lower = new G4UnionSolid(tmpnameCbl,
                                      solid_Cable_LowerPart[iRow-1],
                                      CblAboveDet,
                                      G4Transform3D(rotmat,transVec)  );

                      // create a second box
                      sprintf( tmpnameCbl , "Cable_Box2Lower_String%d_row%d" , iCol , iRow );
                      G4Box* CblBesideVerticalBar = new G4Box(tmpnameCbl,0.5*tmpX,0.5*tmpY, 0.5*(vec_3to2.mag())  );

                      // add second box to previous solid
                      transVec = vec_center32 - vecTot_center10 ;
                      sprintf( tmpnameCbl , "Cable_Union2Lower_String%d_row%d" , iCol , iRow );
                      G4UnionSolid* CblUnion2Lower = new G4UnionSolid(tmpnameCbl,
                                  CblUnion1Lower,
                                  CblBesideVerticalBar,
                                  0,
                                  transVec  );
                      solid_Cable_LowerPart[iRow] = CblUnion2Lower;


                      // add third box to previous solid
                      G4double tmp_dx = fabs(GetCrystalPosition(iCol,iRow).getX()-points_LowerContact[3+2*iRow].getX()) - 1.0*cm;
                      sprintf( tmpnameCbl , "Cable_Box3Lower_String%d_row%d" , iCol , iRow );
                      G4Box* CblBelow = new G4Box(tmpnameCbl, 0.5*tmp_dx ,0.5*tmpY, 0.5*tmpX  );
                      transVec = points_LowerContact[3+2*iRow] - vecTot_center10 ;
                      transVec.setX(  transVec.getX() - 0.5*tmp_dx );
                      sprintf( tmpnameCbl , "Cable_Union3Lower_String%d_row%d" , iCol , iRow );
                      G4UnionSolid* CblUnion3Lower = new G4UnionSolid(tmpnameCbl,
                                  CblUnion2Lower,
                                  CblBelow,
                                  0,
                                  transVec  );
                      solid_Cable_LowerPart_final[iRow] = CblUnion3Lower;

                      // Create solids for Cable_UpperPart
                      // ----------------------------------
                      vec_2to1     =      points_UpperContact_Add[0+2*iRow] - points_LowerContact[1+2*iRow];
                      vec_center21 = 0.5*(points_UpperContact_Add[0+2*iRow] + points_LowerContact[1+2*iRow]);
                      vec_center32 = 0.5*(points_UpperContact_Add[1+2*iRow] + points_UpperContact_Add[0+2*iRow]);
                      vec_3to2     =     (points_UpperContact_Add[1+2*iRow] - points_UpperContact_Add[0+2*iRow]);

                      // create a first box
                      sprintf( tmpnameCbl , "Cable_Box1Upper_String%d_row%d" , iCol , iRow );
                      G4Box* CblAboveDetUpper1 = new G4Box(tmpnameCbl, 0.5*tmpX, 0.5*tmpY, 0.5*(vec_2to1.mag()) );

                      sprintf( tmpnameCbl , "Cable_Union1Upper_String%d_row%d" , iCol , iRow );
                      transVec = vec_center21 - vecTot_center10 ;
                      G4UnionSolid* CblUnion1Upper;
                      if(iRow==0)
                          CblUnion1Upper = new G4UnionSolid(tmpnameCbl,
                                      Phase1Cable_Solid_ElectronicsSideBar,
                                      CblAboveDetUpper1,
                                      0,
                                      transVec );
                      else
                          CblUnion1Upper = new G4UnionSolid(tmpnameCbl,
                                      solid_Cable_LowerPart[iRow-1],
                                      CblAboveDetUpper1,
                                      0,
                                      transVec );
                      // create a second box
                      sprintf( tmpnameCbl , "Cable_Box2Upper_String%d_row%d" , iCol , iRow );
                      G4Box* CblHorizontal = new G4Box(tmpnameCbl,0.5*(vec_3to2.mag()),0.5*tmpY,0.5*tmpX );

                      // add second box to previous solid
                      transVec = vec_center32 - vecTot_center10 ;
                      sprintf( tmpnameCbl , "Cable_Union2Upper_String%d_row%d" , iCol , iRow );
                      G4UnionSolid* CblUnion2Upper = new G4UnionSolid(tmpnameCbl,
                                  CblUnion1Upper,
                                  CblHorizontal,
                                  0,
                                  transVec  );
                      solid_Cable_UpperPart[iRow] = CblUnion2Upper;
                    }

                  MGLog(debugging) << " List of Solids is created" << endlog;

                  // Create logical Volumes
                  G4LogicalVolume*   logVol_Cable_LowerPart[5];
                  G4LogicalVolume*   logVol_Cable_UpperPart[5];
                  for(int iRow=0;iRow<nPhase1CrystalsInString;iRow++)
                    {
                      sprintf( tmpnameCbl , "CableLower_Phase1_String%d_Det%d", iCol , iRow );
                      logVol_Cable_LowerPart[iRow] = new G4LogicalVolume( solid_Cable_LowerPart_final[iRow],
                                  HVCableCuTeflonComposite,
                                  tmpnameCbl);
                      fffStringStuff->Set_Phase1_CableLowerPart_Logical(  logVol_Cable_LowerPart[iRow] , iRow );
                      MGLog(debugging) << "Cable \""<<tmpnameCbl<<"\" has been created." << endlog;

                      sprintf( tmpnameCbl , "CableUpper_Phase1_String%d_Det%d", iCol , iRow );
                      logVol_Cable_UpperPart[iRow] = new G4LogicalVolume( solid_Cable_UpperPart[iRow],
                                  HVCableCuTeflonComposite,
                                  tmpnameCbl);
                      fffStringStuff->Set_Phase1_CableUpperPart_Logical(  logVol_Cable_UpperPart[iRow] , iRow );
                      MGLog(debugging) << "Cable \""<<tmpnameCbl<<"\" has been created." << endlog;
                    }
                }


              if(nPhase2CrystalsInString!=0)
                {
                  ;
                }
            }
        }
    }


  // +--------------------------------------------------------+
  // |   construct HV cables for phase-2                      |
  // +--------------------------------------------------------+
  Create_HVCables_Phase2();
  Create_DetectorCablesForPhase2();
  // -------------------------------------------------
  // volume: electronics phase-1 lower horizontal bar
  // -------------------------------------------------
  GEGeometryHolderPhase1Parts* dummyHolderHorizontalBar_Tube = new GEGeometryHolderPhase1Parts(*fDefaultHolderPhase1Parts);
  dummyHolderHorizontalBar_Tube->CreateSolids_LowerStar();
  G4SubtractionSolid* SuspensionTube_TShape_solid = dummyHolderHorizontalBar_Tube->GetSuspensionTube_TShape();

  G4double pDz    ;
  G4double pDy1   ;
  G4double pDx1   ;
  G4double pDx2   ;
  G4double pAlp1  ;
  G4double pDy2   ;
  G4double pDx3   ;
  G4double pDx4   ;
  G4double pAlp2  ;
  G4double pTheta ;
  G4double pPhi   ;

  pDz    = 0.5*  ( 0.5* fElectronicsPhaseIHorizontalBar_Width);
  pDy1   = 0.5*  fElectronicsPhaseIHorizontalBar_YFrameThickness;
  pDx1   = 0.5* fElectronicsPhaseIHorizontalBar_InnerHeight;
  pDx2   = pDx1;
  pAlp1  = 0.0;
  pDy2   = pDy1;
  pDx3   = 0.5* fElectronicsPhaseIHorizontalBar_OuterHeight;
  pDx4   = pDx3;
  pAlp2  = 0.0;
  pTheta = atan(  (pDx1-pDx3)/(2.*pDz) );
  pPhi   = 0.0;
  G4Trap* ElectronicsPhaseIHorizontalBar_Solid1 = new G4Trap("ElectronicsPhaseIHorizontalBar_Solid1",
              pDz, pTheta, pPhi,
              pDy1, pDx1, pDx2, pAlp1,
              pDy2, pDx3, pDx4, pAlp2 );
  G4double myTheta = atan(  (2.*(pDx1-pDx3))/(2.*pDz) );
  pDz    = pDz -  fElectronicsPhaseIHorizontalBar_XZFrameThickness;
  pDy1   = 0.5*  fElectronicsPhaseIHorizontalBar_YFrameThickness;
  pDx1   = 0.5* (2.*pDx1 - fElectronicsPhaseIHorizontalBar_XZFrameThickness - fElectronicsPhaseIHorizontalBar_XZFrameThickness/cos(myTheta));
  pDx2   = pDx1;
  pAlp1  = 0.0;
  pDy2   = pDy1;
  pDx3   = 0.5* (2.*pDx3 - fElectronicsPhaseIHorizontalBar_XZFrameThickness - fElectronicsPhaseIHorizontalBar_XZFrameThickness/cos(myTheta));
  pDx4   = pDx3;
  pAlp2  = 0.0;
  pTheta = atan(  (pDx1-pDx3)/(2.*pDz) );
  pPhi   = 0.0;
  G4Trap* ElectronicsPhaseIHorizontalBar_Solid2 = new G4Trap("ElectronicsPhaseIHorizontalBar_Solid2",
              pDz, pTheta, pPhi,
              pDy1, pDx1, pDx2, pAlp1,
              pDy2, pDx3, pDx4, pAlp2 );
  G4ThreeVector my3vec;
  my3vec.setX( 0.5*fElectronicsPhaseIHorizontalBar_XZFrameThickness*( 1./cos(myTheta) - 1.) );
  my3vec.setY( 0.5*(fElectronicsPhaseIHorizontalBar_YFrameThickness+fElectronicsPhaseIHorizontalBar_InnerWallYThickness) );
  my3vec.setZ( 0.0 );
  G4SubtractionSolid* ElectronicsPhaseIHorizontalBar_Solid3 = new G4SubtractionSolid("ElectronicsPhaseIHorizontalBar_Solid3",
              ElectronicsPhaseIHorizontalBar_Solid1,
              ElectronicsPhaseIHorizontalBar_Solid2,
              0,
              my3vec);
  my3vec.setY( - my3vec.getY() );
  G4SubtractionSolid* ElectronicsPhaseIHorizontalBar_Solid4 = new G4SubtractionSolid("ElectronicsPhaseIHorizontalBar_Solid4",
              ElectronicsPhaseIHorizontalBar_Solid3,
              ElectronicsPhaseIHorizontalBar_Solid2,
              0,
              my3vec);

  BoxLengthX = 0.5* fElectronicsPhaseIHorizontalBar_InnerHeight           + 0.001*mm;
  BoxLengthY = 0.5* fElectronicsPhaseIHorizontalBar_YFrameThickness       + 0.001*mm;
  BoxLengthZ = 0.5* dummyHolderHorizontalBar_Tube->GetRoHldStarLowerTube(0) + 0.001*mm;
  G4Box* ElectronicsPhaseIHorizontalBar_Solid4a = new G4Box("ElectronicsPhaseIHorizontalBar_Solid4a",
              BoxLengthX ,
              BoxLengthY ,
              BoxLengthZ );
  my3vec.setX( -0.25*(fElectronicsPhaseIHorizontalBar_InnerHeight-fElectronicsPhaseIHorizontalBar_OuterHeight) );
  my3vec.setY(  0.0 );
  my3vec.setZ( -0.25*fElectronicsPhaseIHorizontalBar_Width+BoxLengthZ-0.001*mm);
  G4SubtractionSolid* ElectronicsPhaseIHorizontalBar_Solid4b = new G4SubtractionSolid("ElectronicsPhaseIHorizontalBar_Solid4b",
              ElectronicsPhaseIHorizontalBar_Solid4,
              ElectronicsPhaseIHorizontalBar_Solid4a,
              0,
              my3vec);

  G4RotationMatrix mRot;
  mRot.rotateY(-90.*deg);
  my3vec.setX( -0.25*(fElectronicsPhaseIHorizontalBar_Width) - 0.01*mm );
  my3vec.setY( 0.0 );
  my3vec.setZ( -0.25*(fElectronicsPhaseIHorizontalBar_InnerHeight+fElectronicsPhaseIHorizontalBar_OuterHeight) - 10.*SmallNumber );
  G4UnionSolid*  ElectronicsPhaseIHorizontalBar_Solid5 = new G4UnionSolid("ElectronicsPhaseIHorizontalBar_Solid5",
              SuspensionTube_TShape_solid,
              ElectronicsPhaseIHorizontalBar_Solid4b,
              G4Transform3D(mRot,my3vec) );

  G4RotationMatrix mRot2;
  mRot2.rotateY(-90.*deg);
  mRot2.rotateZ(180.*deg);
  my3vec.setX( - my3vec.getX() + 0.01*mm);
  my3vec.setY(   my3vec.getY()- 10.*SmallNumber );
  my3vec.setZ(   my3vec.getZ()- 0.01*mm + 0.0* 10.*SmallNumber );
  G4UnionSolid*  ElectronicsPhaseIHorizontalBar_Solid6 = new G4UnionSolid("ElectronicsPhaseIHorizontalBar_Solid6",
              ElectronicsPhaseIHorizontalBar_Solid5,
              ElectronicsPhaseIHorizontalBar_Solid4b,
              G4Transform3D(mRot2,my3vec) );



  fElectronicsPhaseIHorizontalBarLogical = new G4LogicalVolume(ElectronicsPhaseIHorizontalBar_Solid6,
              copper,
              "ElectronicsPhaseIHorizontalBarLogical");
  fElectronicsPhaseIHorizontalBarLogical -> SetVisAttributes(CuVisAtt);


  // -------------------------------------------------
  // volume: electronics board
  // -------------------------------------------------

  G4Polyhedra* JunctionBoard1 = new G4Polyhedra("JunctionBoard1",
              0.,
              360*deg,
              6,
              2,
              fElectronicsJunctionSledZPlane,
              fElectronicsJunctionSledRMin,
              fElectronicsJunctionSledRMax);


  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.9 * mm;
  TubsHeight        = 0.5 * fElectronicsBoardHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* ElectronicsBoard1Hole = new G4Tubs("ElectronicsBoard1Hole",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid1 = new G4SubtractionSolid("ElectronicsBoardSubtractionSolid1",
              JunctionBoard1,
              ElectronicsBoard1Hole,
              0,
              G4ThreeVector(fHolderSideBarRadius, 0.0, 0.0));

  G4SubtractionSolid* G4SubtractionSolid2 = new G4SubtractionSolid("ElectronicsBoardSubtractionSolid2",
              G4SubtractionSolid1,
              ElectronicsBoard1Hole,
              0,
              G4ThreeVector(-fHolderSideBarRadius, 0.0, 0.0));


  fElectronicsJunctionBoardLogical = new G4LogicalVolume(G4SubtractionSolid2,
              copper,
              "ElectronicsJunctionBoardLogical");

  fElectronicsJunctionBoardLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // volume: junction sled
  // -------------------------------------------------



  G4double Box1Z = 24.5;
  BoxLengthX = 12 * 0.5 * mm;
  BoxLengthY = 12 * 0.5 * mm;
  BoxLengthZ = Box1Z * 0.5 * mm;


  G4Box* SledBox1 = new G4Box("SledBox1",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);
  G4double Box2Z = 30.0;
  BoxLengthX = 30 * 0.5 * mm;
  BoxLengthY = 31 * 0.5 * mm;
  BoxLengthZ = Box2Z * 0.5 * mm;


  G4Box* SledBox2 = new G4Box("SledBox2",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);


  G4UnionSolid* SledUnionBox1 = new G4UnionSolid("SledUnionBox1",
              SledBox1,
              SledBox2,
              0,
              G4ThreeVector(0.0, 0.0, (Box1Z * 0.5 + Box2Z * 0.5)*mm));


  G4double tubsradius = 6.0;
  TubsInnerRadius   = 0.0  * mm;
  TubsOuterRadius   = tubsradius * mm;
  TubsHeight        = 65.0 * mm * 0.5;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SledTubs1 = new G4Tubs("SledTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4RotationMatrix   tubsrot1;
  tubsrot1.rotateX(90 * deg);

  G4UnionSolid* SledUnionSolid1 = new G4UnionSolid("SledUnionSolid1",
              SledUnionBox1,
              SledTubs1,
              &tubsrot1,
              G4ThreeVector(0.0, 0.0, (Box1Z * 0.5 + Box2Z * 0.5 + tubsradius )*mm) );

  fElectronicsJunctionSledLogical = new G4LogicalVolume(SledUnionSolid1,copper,"JunctionSledLogical");
  fElectronicsJunctionSledLogical ->  SetVisAttributes(CuVisAtt);


  fElectronicsJunctionSledZPlane[0] = 0.* mm;
  fElectronicsJunctionSledZPlane[1] =  (Box1Z + Box2Z) * mm;

  fElectronicsKaptonPreAmpsPinsCableHeight = (Box1Z + Box2Z ) * mm;

  fElectronicsJunctionSledRMin[0] = 44.01 * mm;
  fElectronicsJunctionSledRMin[1] = 44.01 * mm;
  fElectronicsJunctionSledRMin[2] = 44.01 * mm;
  fElectronicsJunctionSledRMin[3] = 44.01 * mm;
  fElectronicsJunctionSledRMin[4] = 44.01 * mm;
  fElectronicsJunctionSledRMin[5] = 44.01 * mm;

  fElectronicsJunctionSledRMax[0] = 44.16 * mm;// thickness is 150 mu m.
  fElectronicsJunctionSledRMax[1] = 44.16 * mm;
  fElectronicsJunctionSledRMax[2] = 44.16 * mm;
  fElectronicsJunctionSledRMax[3] = 44.16 * mm;
  fElectronicsJunctionSledRMax[4] = 44.16 * mm;
  fElectronicsJunctionSledRMax[5] = 44.16 * mm;


  G4Polyhedra*  AmpsPinsCablePoly1 = new G4Polyhedra("AmpsPinsCablePoly1",
              0.,
              360*deg,
              6,
              2,
              fElectronicsJunctionSledZPlane,
              fElectronicsJunctionSledRMin,
              fElectronicsJunctionSledRMax);


  BoxLengthX =  51 * 0.5 * mm;
  BoxLengthY =  95 * 0.5 * mm;
  BoxLengthZ =  (Box1Z + Box2Z  + 5 * cm) * 0.5 * mm;

  G4Box* AmpsPinsCableSubtractionBox = new G4Box("AmpsPinsCableSubtractionBox",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  G4SubtractionSolid* G4SubtractionSolidAmpsPinsCable = new G4SubtractionSolid("G4SubtractionSolidAmpsPinsCable",
              AmpsPinsCablePoly1,
              AmpsPinsCableSubtractionBox,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));


  G4double densityKaptonCuCableCompound = 3.1712 * g/cm3;
  G4Material* KaptonCuCableCompound = new G4Material("KaptonCuCableCompound",densityKaptonCuCableCompound,2);
  KaptonCuCableCompound->AddMaterial(copper, 18.07*perCent);
  KaptonCuCableCompound->AddMaterial(kapton, 81.93*perCent);


  fElectronicsKaptonPreAmpsPinsCableLogical = new G4LogicalVolume(G4SubtractionSolidAmpsPinsCable,
              KaptonCuCableCompound,
              "fKaptonPreAmpsPinsCableLogical");

  fElectronicsKaptonPreAmpsPinsCableLogical ->  SetVisAttributes(CuVisAtt);



  //  // including pins
  G4double density = 5.828 * g/cm3; //total density including pins!!!
  G4Material* NylonCuCompound = new G4Material("NylonCuCompound",density,3);
  NylonCuCompound->AddMaterial(copper, 86.35*perCent);
  NylonCuCompound->AddMaterial(nylon, 12.89*perCent);
  NylonCuCompound->AddMaterial(brass, 0.76*perCent);

//   //without pins
//   G4double density = 5.783 * g/cm3; // density without pins!!
//   G4Material* NylonCuCompound = new G4Material("NylonCuCompound",density,2);
//   NylonCuCompound->AddMaterial(copper, 87.01*perCent);
//   NylonCuCompound->AddMaterial(nylon, 12.99*perCent);

  fElectronicsJunctionSledZPlane[0] = 0.* mm;
  fElectronicsJunctionSledZPlane[1] = 22 * mm;

  fElectronicsJunctionSledRMin[0] = 0. * mm;
  fElectronicsJunctionSledRMin[1] = 0. * mm;
  fElectronicsJunctionSledRMin[2] = 0. * mm;
  fElectronicsJunctionSledRMin[3] = 0. * mm;
  fElectronicsJunctionSledRMin[4] = 0. * mm;
  fElectronicsJunctionSledRMin[5] = 0. * mm;

  fElectronicsJunctionSledRMax[0] = 44.0 * mm;
  fElectronicsJunctionSledRMax[1] = 44.0 * mm;
  fElectronicsJunctionSledRMax[2] = 44.0 * mm;
  fElectronicsJunctionSledRMax[3] = 44.0 * mm;
  fElectronicsJunctionSledRMax[4] = 44.0 * mm;
  fElectronicsJunctionSledRMax[5] = 44.0 * mm;


  G4Polyhedra* JunctionSledHexa3 = new G4Polyhedra("JunctionSledHexa3",
              0.,
              360*deg,
              6,
              2,
              fElectronicsJunctionSledZPlane,
              fElectronicsJunctionSledRMin,
              fElectronicsJunctionSledRMax);


  fElectronicsJunctionSledPinsLogical = new G4LogicalVolume(JunctionSledHexa3,NylonCuCompound,"JunctionSledPinsLogical");
  fElectronicsJunctionSledPinsLogical ->  SetVisAttributes(NylonVisAtt);



  TubsInnerRadius   = 0.0  * mm;
  TubsOuterRadius   = 2.25 * mm;
  TubsHeight        = fElectronicsJunctionSledSpacerHeight * 0.5;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SpacerTubs1 = new G4Tubs("SpacerTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4Tubs* SpacerTubs2 = new G4Tubs("SpacerTubs2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4Tubs* SpacerTubs3 = new G4Tubs("SpacerTubs3",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4Tubs* SpacerTubs4 = new G4Tubs("SpacerTubs4",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4Tubs* SpacerTubs5 = new G4Tubs("SpacerTubs5",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4Tubs* SpacerTubs6 = new G4Tubs("SpacerTubs6",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4UnionSolid* SpacerUnionSolid1 = new G4UnionSolid("SpacerUnionSolid1",
              SpacerTubs1,
              SpacerTubs2,
              0,
              G4ThreeVector(6.0 * mm, 0.0, 0.0 ));


  G4UnionSolid* SpacerUnionSolid2 = new G4UnionSolid("SpacerUnionSolid2",
              SpacerTubs3,
              SpacerTubs4,
              0,
              G4ThreeVector(6.0 * mm, 0.0, 0.0 ));


  G4UnionSolid* SpacerUnionSolid3 = new G4UnionSolid("SpacerUnionSolid3",
              SpacerTubs5,
              SpacerTubs6,
              0,
              G4ThreeVector(90.0 * mm, 0.0, 0.0 ));

  G4UnionSolid* SpacerUnionSolid4 = new G4UnionSolid("SpacerUnionSolid4",
              SpacerUnionSolid1,
              SpacerUnionSolid2,
              0,
              G4ThreeVector(0.0, 44.0 * mm, 0.0 ));

  G4UnionSolid* SpacerUnionSolid5 = new G4UnionSolid("SpacerUnionSolid5",
              SpacerUnionSolid4,
              SpacerUnionSolid3,
              0,
              G4ThreeVector(-45.0 + 3 * mm, 22.0 * mm , 0.0 ));

  fElectronicsJunctionSledSpacerLogical = new G4LogicalVolume(SpacerUnionSolid5,copper,"JunctionSledSpacerLogical");
  fElectronicsJunctionSledSpacerLogical ->  SetVisAttributes(CuVisAtt);


  G4double  density2 = 2.113 * g/cm3;
  G4Material* NylonCuCompound2 = new G4Material("NylonCuCompound2",density2,2);
  NylonCuCompound2->AddMaterial(copper,57.86*perCent);
  NylonCuCompound2->AddMaterial(nylon, 42.14*perCent);

  fElectronicsJunctionSledZPlane[0] = 0.;
  fElectronicsJunctionSledZPlane[1] = 15.0 * mm;

  G4Polyhedra* JunctionSledHexa4 = new G4Polyhedra("JunctionSledHexa4",
              0.,
              360*deg,
              6,
              2,
              fElectronicsJunctionSledZPlane,
              fElectronicsJunctionSledRMin,
              fElectronicsJunctionSledRMax);

  fElectronicsJunctionSledCablesLogical = new G4LogicalVolume(JunctionSledHexa4,NylonCuCompound2,"JunctionSledCablesLogical");
  fElectronicsJunctionSledCablesLogical ->  SetVisAttributes(NylonVisAtt);

  // -------------------------------------------------
  // volume: cables + cablechain + chain above seld
  // -------------------------------------------------

  BoxLengthX =  fElectronicsCableChainX * 0.5 ;
  BoxLengthY =  fElectronicsCableChainY * 0.5 ;
  BoxLengthZ =  fElectronicsCableChainHeight * 0.5;


  G4Box* CableChainBox1 = new G4Box("CableChainBox1",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);


  BoxLengthX =  (fElectronicsCableChainX - 1.6 * mm ) * 0.5 ;
  BoxLengthY =  (fElectronicsCableChainY - 1.6 * mm ) * 0.5 ;
  BoxLengthZ =  fElectronicsCableChainHeight * 0.5;

  G4Box* CableChainBox2 = new G4Box("CableChainBox2",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  G4SubtractionSolid* ElectronicsCableChain = new G4SubtractionSolid("ElectronicsCableChain",
              CableChainBox1,
              CableChainBox2,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));

  fElectronicsCableChainLogical = new G4LogicalVolume(ElectronicsCableChain,steel,"CableChain");



  TubsInnerRadius   = 0.0  * mm;
  TubsOuterRadius   = fElectronicsChainRadius;
  TubsHeight        = fElectronicsChainHeight * 0.5;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* ChainTubs1 = new G4Tubs("ChainTubs1",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4Tubs* ChainTubs2 = new G4Tubs("ChainTubs2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4UnionSolid* Chain = new G4UnionSolid("Chain",
              ChainTubs1,
              ChainTubs2,
              0,
              G4ThreeVector(0.0, 35.5 * mm, 0.0 ));

  fElectronicsChainLogical = new G4LogicalVolume(Chain,steel,"Chain");


  BoxLengthX =  fElectronicsCableX * 0.5 ;
  BoxLengthY =  fElectronicsCableY * 0.5 ;
  BoxLengthZ =  fElectronicsCableHeight * 0.5;

  G4Box* CableBox1 = new G4Box("CableBox1",
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);



  G4double  densitycables = 8.715 * g/cm3;
  G4Material* CableCompound = new G4Material("CableCompound",densitycables,3);
  CableCompound ->AddMaterial(copper,99.235*perCent);
  CableCompound ->AddMaterial(teflon, 0.765 *perCent);
  CableCompound ->AddMaterial(PET, 0.000017779 *perCent); //Yes I know it doesn't matter...

  fElectronicsCableLogical = new G4LogicalVolume(CableBox1,CableCompound,"ElectronicsCable");


  // -------------------------------------------------
  // volume: muon veto
  // -------------------------------------------------

  BoxLengthX = 0.5 * fScintillatorLengthX;
  BoxLengthY = 0.5 * fScintillatorLengthY;
  BoxLengthZ = 0.5 * fScintillatorHeight;

  G4Box* ScintillatorBox = new G4Box("ScintillatorBox",
              BoxLengthX ,
              BoxLengthY,
              BoxLengthZ);

  if (fScintillatorHole)
    {
      TubsInnerRadius   = 0.0 * mm;
      TubsOuterRadius   = GetScintillatorHoleOuterRadius();
      TubsHeight        = (GetScintillatorHeight() + 1.0*micrometer) * 0.5;
      TubsStartAngle    = 0.0   * deg;
      TubsSpanningAngle = 360.0 * deg;

      G4Tubs* ScintillatorHoleTubs = new G4Tubs("ScintillatorHoleTubs",
                    TubsInnerRadius,
                    TubsOuterRadius,
                    TubsHeight,
                    TubsStartAngle,
                    TubsSpanningAngle);

      G4SubtractionSolid* ScintillatorSubtractionSolid = new G4SubtractionSolid("ScintillatorSubtractionSolid",
                  ScintillatorBox,
                  ScintillatorHoleTubs,
                  0,
                  G4ThreeVector(0.0, 0.0, 0.0));

      fScintillatorLogical = new G4LogicalVolume(ScintillatorSubtractionSolid,
                  plasticScint,
                  "ScintillatorLogical");
    }

  else if (!fScintillatorHole)
    {
      fScintillatorLogical = new G4LogicalVolume(ScintillatorBox,
                  plasticScint,
                  "ScintillatorLogical");
    }


  // -------------------------------------------------
  // volume : calibration source cylinder
  // -------------------------------------------------


  TubsInnerRadius   = 0.0   * mm;
  TubsOuterRadius   = 17.5  * mm;
  TubsHeight        = 0.5   * fCalibrationCylinderHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CalibrationCylinderTubs = new G4Tubs("CalibrationCylinderTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 3.2 * mm;
  TubsHeight        = 0.5 * 15.9 * mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CalibrationCollimatorTubs = new G4Tubs("CalibrationCollimatorTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 2.0 * mm;
  TubsHeight        = 0.5 * 4.0 * mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CalibrationInnerSphere = new G4Tubs("CalibrationInnerSphere",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4SubtractionSolid* CalibrationSubtractionSolid1 =
    new G4SubtractionSolid("CalibrationSubtractionSolid1",
              CalibrationCylinderTubs ,
              CalibrationCollimatorTubs,
              0,
              G4ThreeVector(0.0, 0.0, 30.95));


  G4SubtractionSolid* CalibrationSubtractionSolid2 =
    new G4SubtractionSolid("CalibrationSubtractionSolid2",
              CalibrationCollimatorTubs,
              CalibrationInnerSphere,
              0,
              G4ThreeVector(0.0, 0.0, 4.15));



  fCalibrationCylinderLogical = new G4LogicalVolume(CalibrationSubtractionSolid1,
              tantalum,
              "CalibrationCylinder");

  fCalibrationCylinderLogical -> SetVisAttributes(CalibrationCylinderVisAtt);

  // -------------------------------------------------
  // volume : calibration source copper cap
  // -------------------------------------------------

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 6.3 * mm;
  TubsHeight        = 0.5 * 11.2 * mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CalibrationCylinderCap = new G4Tubs("CalibrationCylinderCap",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsOuterRadius   = 3.3 * mm;
  TubsHeight        = 0.5 * 8.2 * mm;

  G4Tubs* CalibrationCylinderCap2 = new G4Tubs("CalibrationCylinderCap2",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  G4SubtractionSolid* CalibrationSubtractionSolid3 =
    new G4SubtractionSolid("CalibrationSubtractionSolid3",
              CalibrationCylinderCap,
              CalibrationCylinderCap2,
              0,
              G4ThreeVector(0.0, 0.0, -1.5*mm));

  fCalibrationCylCapLogical = new G4LogicalVolume(CalibrationSubtractionSolid3,
              copper,
              "CalibrationCylCap");

  fCalibrationCylCapLogical -> SetVisAttributes(CalibrationCylinderVisAtt);

  // -------------------------------------------------
  // volume : calibration source
  // -------------------------------------------------

  // TODO: change toeffective gold density
  fCalibrationSourceInnerLogical = new G4LogicalVolume(CalibrationInnerSphere,
		  	  capsGold,
              "CalibrationSourceInner");

  fCalibrationSourceOuterLogical = new G4LogicalVolume(CalibrationSubtractionSolid2,
              steel,
              "CalibrationSourceOuter");

  // -------------------------------------------------
  // volume : collimated calibration source cylinder
  // -------------------------------------------------


  TubsInnerRadius   = 0.0   * mm;
  TubsOuterRadius   = 0.5 * fCollimatedCalibrationCylinderDiameter;
  TubsHeight        = 0.5   * fCollimatedCalibrationCylinderHeight;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CollimatedCalibrationCylinderTubs = new G4Tubs("CollimatedCalibrationCylinderTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.5 * fCollimatedCalibrationCapDiameter;
  TubsHeight        = 0.5 * fCollimatedCalibrationCapHeight;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CollimatedCalibrationCapTubs = new G4Tubs("CollimatedCalibrationCapTubs",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.5 * fCollimatedCalibrationCapsuleDiameter;
  TubsHeight        = 0.5 * fCollimatedCalibrationCapsuleHeight;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CollimatedCalibrationCapsule = new G4Tubs("CollimatedCalibrationCapsule",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = 0.5 * fCollimatedCalibrationSourceDiameter;
  TubsHeight        = 0.5 * fCollimatedCalibrationSourceHeight;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CollimatedCalibrationSource = new G4Tubs("CollimatedCalibrationSource",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4UnionSolid* CollimatedCalibrationUnionSolid1 =
    new G4UnionSolid("CollimatedCalibrationUnionSolid1",
		     CollimatedCalibrationCylinderTubs,
		     CollimatedCalibrationCapTubs,
		     0,
		     G4ThreeVector( 0.0, 0.0, 0.5*(fCollimatedCalibrationCylinderHeight + fCollimatedCalibrationCapHeight) + fCalibrationCollimationDistance ) );

  G4SubtractionSolid* CollimatedCalibrationSubtractionSolid1 =
    new G4SubtractionSolid("CollimatedCalibrationSubtractionSolid1",
			   CollimatedCalibrationUnionSolid1 ,
			   CollimatedCalibrationCapsule,
			   0,
			   G4ThreeVector(0.0, 0.0, 0.5*(fCollimatedCalibrationCylinderHeight + fCalibrationCollimationDistance)));


  G4SubtractionSolid* CollimatedCalibrationSubtractionSolid2 =
    new G4SubtractionSolid("CollimatedCalibrationSubtractionSolid2",
              CollimatedCalibrationCapsule,
              CollimatedCalibrationSource,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));



  fCollimatedCalibrationCylinderLogical = new G4LogicalVolume(CollimatedCalibrationSubtractionSolid1,
              tantalum,
              "CollimatedCalibrationCylinder");

  fCollimatedCalibrationCylinderLogical -> SetVisAttributes(CollimatedCalibrationCylinderVisAtt);


  // -------------------------------------------------
  // volume : collimated calibration source
  // -------------------------------------------------

  fCollimatedCalibrationSourceInnerLogical = new G4LogicalVolume(CollimatedCalibrationSource,
              gold,
              "CollimatedCalibrationSourceInner");

  fCollimatedCalibrationSourceOuterLogical = new G4LogicalVolume(CollimatedCalibrationSubtractionSolid2,
              steel,
              "CollimatedCalibrationSourceOuter");


  // -------------------------------------------------
  // volume : copper encapsulation
  // -------------------------------------------------

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fCopperEncapsulationOuterRadius;
  TubsHeight        = 0.5 * fCopperEncapsulationHeight; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CopperEncapsulationTubsOuter = new G4Tubs("CopperEncapsulationTubsOuter",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fCopperEncapsulationOuterRadius-fCopperEncapsulationThickness;
  TubsHeight        = (0.5 * fCopperEncapsulationHeight)-fCopperEncapsulationThickness; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* CopperEncapsulationTubsInner = new G4Tubs("CopperEncapsulationTubsInner",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4SubtractionSolid* CopperEncapsulationSubtractionMain =
    new G4SubtractionSolid("CopperEncapsulationSubtractionMain",
              CopperEncapsulationTubsOuter,
              CopperEncapsulationTubsInner,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fCopperEncapsulationRadiusHV;
  TubsHeight        = 0.5 * fCopperEncapsulationThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* CopperEncapsulationHoleHV = new G4Tubs("CopperEncapsulationHoleHV",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4double HVPositionZ=0.5 * (fCopperEncapsulationHeight-fCopperEncapsulationThickness);


  G4SubtractionSolid* CopperEncapsulationSubtractionHV =
    new G4SubtractionSolid("CopperEncapsulationSubtractionHV",
              CopperEncapsulationSubtractionMain,
              CopperEncapsulationHoleHV,
              0,
              G4ThreeVector(0.0, 0.0, HVPositionZ));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fCopperEncapsulationRadiusSignal;
  TubsHeight        = 0.5 * fCopperEncapsulationThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CopperEncapsulationHoleSignal = new G4Tubs("CopperEncapsulationHoleSignal",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4double SignalPositionZ=-(0.5 * (fCopperEncapsulationHeight-fCopperEncapsulationThickness));

  G4SubtractionSolid* CopperEncapsulationSubtractionSignal =
    new G4SubtractionSolid("CopperEncapsulationSubtractionSignal",
              CopperEncapsulationSubtractionHV,
              CopperEncapsulationHoleSignal,
              0,
              G4ThreeVector(0.0, 0.0, SignalPositionZ));




  fCopperEncapsulationLogical = new G4LogicalVolume(CopperEncapsulationSubtractionSignal,
              copper,
              "CopperEncapsulationLogical");

  fCopperEncapsulationLogical -> SetVisAttributes(CuVisAtt);


  // -------------------------------------------------
  // volume : teflon encapsulation
  // -------------------------------------------------

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fTeflonEncapsulationOuterRadius;
  TubsHeight        = 0.5 * fTeflonEncapsulationHeight; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* TeflonEncapsulationTubsOuter = new G4Tubs("TeflonEncapsulationTubsOuter",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fTeflonEncapsulationOuterRadius-fTeflonEncapsulationThickness;
  TubsHeight        = (0.5 * fTeflonEncapsulationHeight)-fTeflonEncapsulationThickness; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* TeflonEncapsulationTubsInner = new G4Tubs("TeflonEncapsulationTubsInner",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4SubtractionSolid* TeflonEncapsulationSubtractionMain =
    new G4SubtractionSolid("TeflonEncapsulationSubtractionMain",
              TeflonEncapsulationTubsOuter,
              TeflonEncapsulationTubsInner,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fTeflonEncapsulationRadiusHV;
  TubsHeight        = 0.5 * fTeflonEncapsulationThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* TeflonEncapsulationHoleHV = new G4Tubs("TeflonEncapsulationHoleHV",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  HVPositionZ=0.5 * (fTeflonEncapsulationHeight-fTeflonEncapsulationThickness);


  G4SubtractionSolid* TeflonEncapsulationSubtractionHV =
    new G4SubtractionSolid("TeflonEncapsulationSubtractionHV",
              TeflonEncapsulationSubtractionMain,
              TeflonEncapsulationHoleHV,
              0,
              G4ThreeVector(0.0, 0.0, HVPositionZ));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fTeflonEncapsulationRadiusSignal;
  TubsHeight        = 0.5 * fTeflonEncapsulationThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* TeflonEncapsulationHoleSignal = new G4Tubs("TeflonEncapsulationHoleSignal",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  SignalPositionZ=-(0.5 * (fTeflonEncapsulationHeight-fTeflonEncapsulationThickness));

  G4SubtractionSolid* TeflonEncapsulationSubtractionSignal =
    new G4SubtractionSolid("TeflonEncapsulationSubtractionSignal",
              TeflonEncapsulationSubtractionHV,
              TeflonEncapsulationHoleSignal,
              0,
              G4ThreeVector(0.0, 0.0, SignalPositionZ));

  fTeflonEncapsulationLogical = new G4LogicalVolume(TeflonEncapsulationSubtractionSignal,
              teflon,
              "TeflonEncapsulationLogical");

  fTeflonEncapsulationLogical -> SetVisAttributes(TeflonVisAtt);

  //copper part of teflon encapsulation

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fTeflonEncapsulationCopperOuterRadius;
  TubsHeight        = 0.5 * fTeflonEncapsulationCopperHeight; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* TeflonEncapsulationCopperTubsOuter = new G4Tubs("TeflonEncapsulationCopperTubsOuter",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  TubsInnerRadius   = 0;
  TubsOuterRadius   = fTeflonEncapsulationCopperOuterRadius-fTeflonEncapsulationCopperThickness;
  TubsHeight        = (0.5 * fTeflonEncapsulationCopperHeight)-fTeflonEncapsulationCopperThickness; //The Z length has to be given as a half-length for its size in G4Tubs
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* TeflonEncapsulationCopperTubsInner = new G4Tubs("TeflonEncapsulationCopperTubsInner",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  G4SubtractionSolid* TeflonEncapsulationCopperSubtractionMain =
    new G4SubtractionSolid("TeflonEncapsulationCopperSubtractionMain",
              TeflonEncapsulationCopperTubsOuter,
              TeflonEncapsulationCopperTubsInner,
              0,
              G4ThreeVector(0.0, 0.0, 0.0));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fTeflonEncapsulationRadiusHV;
  TubsHeight        = 0.5 * fTeflonEncapsulationCopperThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Tubs* TeflonEncapsulationCopperHoleHV = new G4Tubs("TeflonEncapsulationCopperHoleHV",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  HVPositionZ=0.5 * (fTeflonEncapsulationCopperHeight-fTeflonEncapsulationCopperThickness);


  G4SubtractionSolid* TeflonEncapsulationCopperSubtractionHV =
    new G4SubtractionSolid("TeflonEncapsulationCopperSubtractionHV",
              TeflonEncapsulationCopperSubtractionMain,
              TeflonEncapsulationCopperHoleHV,
              0,
              G4ThreeVector(0.0, 0.0, HVPositionZ));



  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fTeflonEncapsulationRadiusSignal;
  TubsHeight        = 0.5 * fTeflonEncapsulationCopperThickness + 0.005*mm;
  TubsStartAngle    = 0.0  * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* TeflonEncapsulationCopperHoleSignal = new G4Tubs("TeflonEncapsulationCopperHoleSignal",
              TubsInnerRadius,
              TubsOuterRadius,
              TubsHeight,
              TubsStartAngle,
              TubsSpanningAngle);


  SignalPositionZ=-(0.5 * (fTeflonEncapsulationCopperHeight-fTeflonEncapsulationCopperThickness));

  G4SubtractionSolid* TeflonEncapsulationCopperSubtractionSignal =
    new G4SubtractionSolid("TeflonEncapsulationCopperSubtractionSignal",
              TeflonEncapsulationCopperSubtractionHV,
              TeflonEncapsulationCopperHoleSignal,
              0,
              G4ThreeVector(0.0, 0.0, SignalPositionZ));




  fTeflonEncapsulationCopperLogical = new G4LogicalVolume(TeflonEncapsulationCopperSubtractionSignal,
              copper,
              "TeflonEncapsulationCopperLogical");

  fTeflonEncapsulationCopperLogical -> SetVisAttributes(CuVisAtt);

  // -------------------------------------------------
  // volume: mini-shrouds
  // -------------------------------------------------
  // Add Mini-Shroud(s) to all strings with detectors


  G4int nMiniShrouds(0);

  G4ThreeVector position;
  G4ThreeVector positionTop;
  G4ThreeVector position2;
  G4ThreeVector positionMSTub;
  G4ThreeVector positionMSBottom;
  G4ThreeVector positionMSTop;
  G4ThreeVector positionMSTopGlued;
  G4ThreeVector positionMSBottomGlued;

 if (fLockType == LockType_CommLock4){
  for( G4int ii=0; ii<9; ii++ ){
    double verticalShift = 174.0 * mm; // Shroud height adjusted to have the base 25mm under the lowest detector bottom surface
    position = GetCrystalPosition( ii, 2 ) ;
    if(position.z() < 324.){   //simple way of checking if the string contains detectors (at least 3) but for BEGes uppermost detector is at z>324.!!
      // check for more than three detectors in string
      for(G4int jj=3; jj<nrOfRows; jj++){
	position2 = GetCrystalPosition( ii, jj );
	if(position2.z()<324.){
	  position = GetCrystalPosition( ii, jj );
	  // adjust the vertical shift for the BEGe string
	  verticalShift=202.0 * mm;
	}
      }
      position.setZ( position.z() + verticalShift );
      MGLog(debugging) << "Mini-Shroud position " << position.x() << "\t" << position.y() << "\t" << position.z()  << endlog;
      fMiniShroudPosition->push_back( position );
      nMiniShrouds++;
      fMiniShroudColumn->push_back(nMiniShrouds);
      fMiniShroudRow->push_back(2);
      // standard setting. phase i compatible
      fMiniShroudRadiusVector->push_back(fMiniShroudOuterRadiusPhaseI);
      fMiniShroudHeightVector->push_back(fMiniShroudHeightPhaseI);
     }
   }
  MGLog(debugging) << "Number of minishrouds in total " << nMiniShrouds << "\t" << endlog;
  }

   double Z_shroud(0); G4int counter(0);
   G4double positionTopZ(0);
   G4double positionTubZ(0);
   G4int type(0);
   G4int type_lower(0);
   GEGeometryDetectorParameters* detectorparams(0);
   GEGeometryDetectorParameters* detectorparams_lower(0);
   G4int HoleCounter = 0;
   G4double holderPhase2lenghtY=0.0;
   G4double coating=0.105*mm;//space for coating to avoid overlaps

   if (fLockType == LockType_PhaseII || fLockType == LockType_PhaseII_DN500){
    fMiniShroudHeightPhaseI = 500.0 * mm; // slightly bigger MS to accommodate the 4 pairs of BEGe's
     for( G4int ii=0; ii<9; ii++ ){
    	 G4int stringType = 0;
    	 G4bool isGTF = false;
    	 for(G4int kk=0; kk<nrOfRows; kk++ ){
    	   type =  GetCrystalType(ii,kk);
	   detectorparams = GetDetectorParameters(type);
    	   if((type >= 8 && type <= 13)  || (type>= 87 && type <= 92)) isGTF = true; // check if GTF in string. if so increase MS diameter
    	   if(kk==0) stringType = type; // first detector in string determines string type
	   if (type>0){
	   		if (detectorparams->GetGeometry()==5 || detectorparams->GetGeometry()==11){ //check if holes for cables are required
	   		HoleCounter++; MGLog(debugging) << "test1" << endlog;
			GEGeometryHolderPhase2Parts* holderMSCableholes = GetHolderPhase2Parts(type);
			holderPhase2lenghtY=holderMSCableholes->GetHolderPhase2Width();//all holders for phase 2 are identically, no matching to position required
			}
		}
    	   MGLog(debugging) << "MS string " << ii << "  pos  " << kk << "   det type   " << type << endlog;
	   position = GetCrystalPosition( ii, kk );
	   G4double positionZ = position.z();//position of lowest detector in string as it is last in loop
	   if(type > -1 ){
	   Z_shroud += position.z();
	   counter++;

	   //position for phase2 nylon mini-shrouds
	   GEGeometryHolderPhase2Parts* holder = GetHolderPhase2Parts( GetCrystalType(ii, kk,0) );
	   if (IsAPair(ii,kk))
	       {type_lower =  GetCrystalType(ii,kk,1);
	       detectorparams_lower = GetDetectorParameters(type_lower);
	      positionTubZ = positionZ - detectorparams_lower->GetHeight()-0.5*fCrystalPairGap-holder->GetZDistanceToEndOfSiliconPlate()-1.1*cm-coating;
	       }
	   else{
	       positionTubZ = positionZ - 0.5*detectorparams->GetHeight()-holder->GetZDistanceToEndOfSiliconPlate()-1.1*cm-coating;
		}
	     }
	   }
       positionTopZ=fPhase2HolderStarPlac - 0.5*fPhase2HolderStarLengthZ-coating;
       fMiniInDetHeightVector->push_back(positionTopZ-fMiniShroudBaseThickness-positionTubZ);//for coating

       MGLog(debugging) << "MS string  " << ii << "   Nb. of crystals   " << counter
    		   << "  Z_shroud: " << Z_shroud/counter
    		   << " string type " << stringType
    		   << " isGTF in string "<< isGTF << endlog;
       if(counter > 2){
    	 // min 3 crystals on the string, no MS for less
	 positionMSTub=position;
	 positionMSBottom=position;
	 positionMSTop=position;
     positionMSTopGlued=position;
     positionMSBottomGlued=position;

    	 position.setZ( Z_shroud / counter );
	 //position for phase2 nylon mini-shrouds
	 positionMSTub.setZ(positionTubZ+0.5*fMiniShroudHeightPhaseII-fMiniShroudBaseThickness);
	 positionMSBottom.setZ(positionTubZ-0.5*fMiniShroudBaseThickness);
	 positionMSTop.setZ(positionTopZ-0.5*fMiniShroudBaseThickness);
     positionMSTopGlued.setZ( positionTopZ + 0.5*fMiniShroudTopGluedRingHeight );
     positionMSBottomGlued.setZ( positionTubZ - fMiniShroudBaseThickness + 0.5*fMiniShroudBottomGluedRingHeight );

         fMiniShroudPosition->push_back( position );
	 fMiniShroudPositionPhase2Tub->push_back( positionMSTub );
	 fMiniShroudPositionPhase2Bottom->push_back( positionMSBottom );
	 fMiniShroudPositionPhase2Top->push_back( positionMSTop );
     fMiniShroudPositionPhase2TopGluedRing->push_back( positionMSTopGlued );
     fMiniShroudPositionPhase2BottomGluedRing->push_back( positionMSBottomGlued );

         if(fFlagAdaptMinishroudToDetctorString)
         {
        	 if(isGTF){
            	 // if GTF detecotor is in string use wider MS
        		 fMiniShroudRadiusVector->push_back(fMiniShroudOuterRadiusCoaxString);
			 fMiniShroudHoleRadiusVector->push_back(fMiniShroudHoleRadiusCoaxString);}
        	 else{
        		 fMiniShroudRadiusVector->push_back(fMiniShroudOuterRadiusBEGeString);
                         fMiniShroudHoleRadiusVector->push_back(fMiniShroudHoleRadiusBEGeString);}

        	 if(stringType==36) // if bege string
        		 fMiniShroudHeightVector->push_back(430.0*mm);
        	 else
        		 fMiniShroudHeightVector->push_back(fMiniShroudHeightPhaseII);
         }
         else
         {
        	 // standard setting. phase i compatible
        	 fMiniShroudRadiusVector->push_back(fMiniShroudOuterRadiusPhaseI);
    		 fMiniShroudHeightVector->push_back(fMiniShroudHeightPhaseI);
		 fMiniShroudHoleRadiusVector->push_back(fMiniShroudHoleRadiusCoaxString);
         }

         nMiniShrouds++;
         fMiniShroudColumn->push_back(nMiniShrouds);
         fMiniShroudRow->push_back(2);
	     MGLog(debugging) << "MS position " << position.x() << "\t" << position.y() << "\t" << position.z()  << endlog;
       }
       counter = 0; Z_shroud = 0; positionTopZ = 0; positionTubZ = 0;
     }
   }

//construct mini-shrouds

for( G4int ii=0; ii<nMiniShrouds; ii++ ){
G4Material* MSMaterial = G4Material::GetMaterial(fMiniShroudMaterial);

//Parameters for holes for cables in mini-shroud, only needed if there are type 5 or 11 detectors in array
//large enough to theoretically accommondate enough cables for a string with "nrOfRows" of BEGe pairs
G4double fCableHoleMiniShroud_angle=GetAngleForPhase2CablePosition(ii);
G4RotationMatrix* HVCableHoleMiniShroudRot = new G4RotationMatrix();
HVCableHoleMiniShroudRot->rotateZ(fCableHoleMiniShroud_angle);
G4RotationMatrix* SignalCableHoleMiniShroudRot = new G4RotationMatrix();
SignalCableHoleMiniShroudRot->rotateZ(180*deg+fCableHoleMiniShroud_angle);
G4double MiniShroudHoleCabx= 0.5*fPhase2Cable_WidthFromHolderToElectronicsPlate;
G4double MiniShroudHoleCaby= 0.5*fPhase2Cable_Thickness*2.*nrOfRows;
G4double MiniShroudHoleCabz= 0.5*fMiniShroudBaseThickness;
G4double MiniShroudHoleRadius= 0.5*holderPhase2lenghtY+MiniShroudHoleCaby;

G4Box* MiniShroudHoleHVCab = new G4Box("MiniShroudHoleHVCab",
						 	MiniShroudHoleCabx,
						 	MiniShroudHoleCaby,
						 	MiniShroudHoleCabz);

G4Box* MiniShroudHoleSignalCab = new G4Box("MiniShroudHoleSignalCab",
						 	MiniShroudHoleCabx,
						 	MiniShroudHoleCaby,
						 	MiniShroudHoleCabz);

G4Tubs* MiniShroudHoleCopperHolderBars = new G4Tubs("MiniShroudHoleCopperHolderBars",
						   0.*mm,
						   fHolderBarRadiusPhase2,
						   MiniShroudHoleCabz,
						   0.*deg,
						   360.*deg);

 if (fMiniShroudMaterial=="Nylon"){//nylon, more detailed implementation of phaseII mini-shrouds
        fFlagMSMaterialCheck = true;
       //solids
        TubsOuterRadius   = fMiniShroudRadiusVector->at(ii);
	G4double HoleRad  = fMiniShroudHoleRadiusVector->at(ii);
	TubsInnerRadius   = TubsOuterRadius - fMiniShroudSideThickness;
	TubsHeight        = 0.5 * fMiniShroudHeightPhaseIINylon;
	G4double TubsBaseThickness = 0.5*fMiniShroudBaseThickness;
	TubsStartAngle    = 0.0   * deg;
        TubsSpanningAngle = 360.0 * deg;
        G4int GluedLayers = 2;

	G4Tubs* MiniShroudTubPhaseII = new G4Tubs("MiniShroudTubPhaseII",
						   TubsInnerRadius,
						   TubsOuterRadius,
						   TubsHeight,
						   TubsStartAngle,
						   TubsSpanningAngle);

	G4Tubs* MiniShroudBottomPhaseII = new G4Tubs("MiniShroudBottomPhaseII",
						   0.0*mm,
						   TubsInnerRadius,
						   TubsBaseThickness,
						   TubsStartAngle,
						   TubsSpanningAngle);

        G4Tubs* MiniShroudTopPhaseIIa = new G4Tubs("MiniShroudTopPhaseIIa",
						   0.0*mm,
						   TubsInnerRadius,
						   TubsBaseThickness,
						   TubsStartAngle,
						   TubsSpanningAngle);
	//holes
	G4SubtractionSolid* MiniShroudTopPhaseIIb =
	 		new G4SubtractionSolid( "MiniShroudTopPhaseIIb",
				   		MiniShroudTopPhaseIIa,
				   		MiniShroudHoleHVCab,
				  		HVCableHoleMiniShroudRot,
		   		   		G4ThreeVector(MiniShroudHoleRadius*sin(fCableHoleMiniShroud_angle),  MiniShroudHoleRadius*cos(fCableHoleMiniShroud_angle), 0.));

  	G4SubtractionSolid* MiniShroudTopPhaseIIc =
	    		new G4SubtractionSolid("MiniShroudTopPhaseIIc",
				   		MiniShroudTopPhaseIIb,
				   		MiniShroudHoleSignalCab,
				   		SignalCableHoleMiniShroudRot,
				   		G4ThreeVector(MiniShroudHoleRadius*sin(180.*deg+fCableHoleMiniShroud_angle), MiniShroudHoleRadius*cos(180.*deg+fCableHoleMiniShroud_angle), 0.));

	G4SubtractionSolid* MiniShroudTopPhaseIId =
	    		new G4SubtractionSolid("MiniShroudTopPhaseIId",
				   		MiniShroudTopPhaseIIc,
				   		MiniShroudHoleCopperHolderBars,
				   		0,
				   		G4ThreeVector(HoleRad*std::cos(fCableHoleMiniShroud_angle), HoleRad*std::sin(fCableHoleMiniShroud_angle), 0.));

	G4SubtractionSolid* MiniShroudTopPhaseIIe =
	    		new G4SubtractionSolid("MiniShroudTopPhaseIIe",
				   		MiniShroudTopPhaseIId,
				   		MiniShroudHoleCopperHolderBars,
				   		0,
				   		G4ThreeVector(HoleRad*std::cos(120.*deg+fCableHoleMiniShroud_angle), HoleRad*std::sin(120.*deg+fCableHoleMiniShroud_angle), 0.));

	G4SubtractionSolid* MiniShroudTopPhaseII =
	    		new G4SubtractionSolid("MiniShroudTopPhaseII",
				   		MiniShroudTopPhaseIIe,
				   		MiniShroudHoleCopperHolderBars,
				   		0,
				   		G4ThreeVector(HoleRad*std::cos(240.*deg+fCableHoleMiniShroud_angle), HoleRad*std::sin(240.*deg+fCableHoleMiniShroud_angle), 0.));

    // Two rings top and bottom where the MS glued part is located
    // Thickness: two additional nylon layers
    G4Tubs* MiniShroudTopGluedRingPhaseII = new G4Tubs( "MiniShroudTopGluedRingPhaseII",
                            TubsInnerRadius - GluedLayers*fMiniShroudSideThickness,
                            TubsInnerRadius,
                            fMiniShroudTopGluedRingHeight/2.,
                            TubsStartAngle,
                            TubsSpanningAngle );

    G4Tubs* MiniShroudBottomGluedRingPhaseII = new G4Tubs( "MiniShroudBottomGluedRingPhaseII",
                            TubsOuterRadius,
                            TubsOuterRadius + GluedLayers*fMiniShroudSideThickness,
                            fMiniShroudBottomGluedRingHeight/2.,
                            TubsStartAngle,
                            TubsSpanningAngle );

    char index1[7];

    sprintf( index1, "%i", ii );

    std::string *LogicalVolumeName1 = new std::string( "MiniShroudTubPhase2Logical_" );
    LogicalVolumeName1->append( index1 );

    fMiniShroudTubPhase2Logical.push_back( new G4LogicalVolume( MiniShroudTubPhaseII, MSMaterial, LogicalVolumeName1->c_str() ) );
    fMiniShroudTubPhase2Logical.at(ii)->SetVisAttributes( NylonVisAtt );

    std::string *LogicalVolumeName2 = new std::string( "MiniShroudBottomPhase2Logical_" );
    LogicalVolumeName2->append( index1 );

    fMiniShroudBottomPhase2Logical.push_back( new G4LogicalVolume( MiniShroudBottomPhaseII, MSMaterial, LogicalVolumeName2->c_str() ) );
    fMiniShroudBottomPhase2Logical.at(ii)->SetVisAttributes( NylonVisAtt );

    std::string *LogicalVolumeName3 = new std::string( "MiniShroudTopPhase2Logical_" );
    LogicalVolumeName3->append( index1 );

    fMiniShroudTopPhase2Logical.push_back( new G4LogicalVolume( MiniShroudTopPhaseII, MSMaterial, LogicalVolumeName3->c_str() ) );
    fMiniShroudTopPhase2Logical.at(ii)->SetVisAttributes( NylonVisAtt );

    // Add two rings containing the MS glued parts
    std::string *LogicalVolumeName4 = new std::string( "MiniShroudTopGluedRingPhase2Logical_" );
    LogicalVolumeName4->append( index1 );

    fMiniShroudTopGluedRingPhase2Logical.push_back( new G4LogicalVolume( MiniShroudTopGluedRingPhaseII, MSMaterial, LogicalVolumeName4->c_str() ) );
    fMiniShroudTopGluedRingPhase2Logical.at(ii)->SetVisAttributes( GluedNylonVisAtt );

    std::string *LogicalVolumeName5 = new std::string( "MiniShroudBottomGluedRingPhase2Logical_" );
    LogicalVolumeName5->append( index1 );

    fMiniShroudBottomGluedRingPhase2Logical.push_back( new G4LogicalVolume( MiniShroudBottomGluedRingPhaseII, MSMaterial, LogicalVolumeName5->c_str() ) );
    fMiniShroudBottomGluedRingPhase2Logical.at(ii)->SetVisAttributes( GluedNylonVisAtt );

	}
  else {//metal copper
  fFlagMSMaterialCheck = false;

  G4double TubsSideThickness = fMiniShroudSideThickness;
  G4double TubsBaseThickness = fMiniShroudBaseThickness;
  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


	  TubsInnerRadius   = 0;
	  TubsOuterRadius   = fMiniShroudRadiusVector->at(ii);
	  TubsHeight        = 0.5 * fMiniShroudHeightVector->at(ii); //The Z length has to be given as a half-length for its size in G4Tubs

	  G4Tubs* MiniShroudTubsOuter = new G4Tubs("MiniShroudTubsOuter",
						   TubsInnerRadius,
						   TubsOuterRadius,
						   TubsHeight,
						   TubsStartAngle,
						   TubsSpanningAngle);

	  G4Tubs* MiniShroudTubsInner = new G4Tubs("MiniShroudTubsInner",
						   TubsInnerRadius,
						   TubsOuterRadius - TubsSideThickness,
						   TubsHeight - TubsBaseThickness,
						   TubsStartAngle,
						   TubsSpanningAngle);
	  G4SubtractionSolid* MiniShroudSubtractionMain;
	  if (HoleCounter>0){

	  	G4SubtractionSolid* MiniShroudSubtractionMain1 = new G4SubtractionSolid("MiniShroudSubtractionMain1",
				   							 MiniShroudTubsOuter,
				  							 MiniShroudTubsInner,
				   							 0,
				   							 G4ThreeVector(0.0, 0.0, 0.0));


  	 G4SubtractionSolid* MiniShroudSubtractionMain2 =
	 		new G4SubtractionSolid( "MiniShroudSubtractionMain2",
				   		MiniShroudSubtractionMain1,
				   		MiniShroudHoleHVCab,
				  		HVCableHoleMiniShroudRot,
		   		   		G4ThreeVector(MiniShroudHoleRadius*sin(fCableHoleMiniShroud_angle),MiniShroudHoleRadius*cos(fCableHoleMiniShroud_angle), TubsHeight-0.5*TubsBaseThickness));

  	 MiniShroudSubtractionMain =
	    		new G4SubtractionSolid("MiniShroudSubtractionMain",
				   		MiniShroudSubtractionMain2,
				   		MiniShroudHoleSignalCab,
				   		SignalCableHoleMiniShroudRot,
				   		G4ThreeVector(MiniShroudHoleRadius*sin(180*deg+fCableHoleMiniShroud_angle), MiniShroudHoleRadius*cos(180*deg+fCableHoleMiniShroud_angle), TubsHeight-0.5*TubsBaseThickness));
		}
	 else {	MiniShroudSubtractionMain = new G4SubtractionSolid("MiniShroudSubtractionMain",
				   				    MiniShroudTubsOuter,
				  				    MiniShroudTubsInner,
				   				    0,
				   				    G4ThreeVector(0.0, 0.0, 0.0));

	      }



    char index[5];
    sprintf( index, "%i", ii );

    std::string *LogicalVolumeName = new std::string( "MiniShroudLogical_" );
    LogicalVolumeName->append( index );



    fMiniShroudLogical.push_back( new G4LogicalVolume( MiniShroudSubtractionMain,
    					  MSMaterial,
						  LogicalVolumeName->c_str() ) );
    fMiniShroudLogical.at(ii)->SetVisAttributes( CuVisAtt );

  }
  }


  // -------------------------------------------------
  // volume : PMT photocathode for Cherenkov veto
  // -------------------------------------------------


  G4Sphere* fPhotocathode = new G4Sphere("Photocathode",
              fPhotocathodeInnerRadius,
              fPhotocathodeOuterRadius,
              fPMTStartingAngle,
              fPMTFinalAngle,
              fPhotocathodeThetaStart,
              fPhotocathodeThetaEnd);

  fPhotocathodeLogical = new G4LogicalVolume(fPhotocathode,
              cathodeAl,
              "fPhotocathodeLogical");

  fPhotocathodeLogical->SetVisAttributes(PhotocathodeVisAtt);


  // -------------------------------------------------
  // volume : PMT encapsulation steel cone for Cherenkov veto
  // -------------------------------------------------

  G4Cons* fPMTSteelCone = new G4Cons("PMTSteelCone",
              fPMTSteelConeLowerRmin,
              fPMTSteelConeLowerRmax,
              fPMTSteelConeUpperRmin,
              fPMTSteelConeUpperRmax,
              fPMTSteelConeHeight*0.5,
              fPMTStartingAngle,
              fPMTFinalAngle);

  fPMTSteelConeLogical = new G4LogicalVolume(fPMTSteelCone,
              steel,
              "PMTSteelConeLogical");

  fPMTSteelConeLogical->SetVisAttributes(SteelVisAtt);


  // -------------------------------------------------
  // volume : PMT encapsulation bottom for Cherenkov veto
  // -------------------------------------------------

  G4Tubs* fPMTSteelBottom = new G4Tubs("PMTSteelBottom",
              0,
              fPMTSteelConeLowerRmax,
              fPMTSteelBottomHeight,
              fPMTStartingAngle,
              fPMTFinalAngle);

  fPMTSteelBottomLogical = new G4LogicalVolume(fPMTSteelBottom,
              steel,
              "pmtbottomlogical");

  fPMTSteelBottomLogical->SetVisAttributes(SteelVisAtt);

  // -------------------------------------------------
  // volume : reflector foil covering the wall of the water tank
  // -------------------------------------------------


  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* fReflectionFoilWaterTankTube = new G4Tubs("fReflectionFoilWaterTankTube",
              fShieldingWaterRadius-fReflectionFoilThickness,
              fShieldingWaterRadius,
              fShieldingWaterHeight*0.5,
              TubsStartAngle,
              TubsSpanningAngle);

  fReflectionFoilWaterTankTubeLogical = new G4LogicalVolume(fReflectionFoilWaterTankTube,nylon,
              "fReflectionFoilWaterTankTubeLogical",
              0,
              0,
              0);

  fReflectionFoilWaterTankTubeLogical -> SetVisAttributes(VM2000VisAtt);


  // -------------------------------------------------
  // volume : reflector foil covering the bottom of the water tank
  // -------------------------------------------------


  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* fReflectionFoilWaterTankBottom = new G4Tubs("fReflectionFoilWaterTankBottom",
              fShieldingCopperShieldingLargeRadius,
              fShieldingWaterRadius-fReflectionFoilThickness,
              fReflectionFoilThickness*0.5,
              TubsStartAngle,
              TubsSpanningAngle);

  fReflectionFoilWaterTankBottomLogical = new G4LogicalVolume(fReflectionFoilWaterTankBottom,nylon,
              "fReflectionFoilWaterTankBottomLogical",
              0,
              0,
              0);

  fReflectionFoilWaterTankBottomLogical -> SetVisAttributes(VM2000VisAtt);


  // -------------------------------------------------
  // volume : reflector foil covering the foot of the cryotank
  // -------------------------------------------------


  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;
  G4Tubs* fReflectionFoilCryoFoot = new G4Tubs("fReflectionFoilCryoFoot",
              fShieldingFootIR-fReflectionFoilThickness,
              fShieldingFootIR,
              0.5 * fShieldingFootHeight,
              TubsStartAngle,
              TubsSpanningAngle);

  fReflectionFoilCryoFootLogical = new G4LogicalVolume(fReflectionFoilCryoFoot,
              nylon,
              "fReflectionFoilCryoFootLogical",
              0,
              0,
              0);

  fReflectionFoilCryoFootLogical -> SetVisAttributes(VM2000VisAtt);

  // --------------------------------------------------------------------------
  // volume : reflector foil covering the inside bottom of the foot of the cryotank
  // --------------------------------------------------------------------------

  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* fReflectionFoilCryoFootBottom = new G4Tubs("fReflectionFoilCryoFootBottom",
              0,
              fShieldingFootIR-fReflectionFoilThickness,
              0.5*fReflectionFoilThickness,
              TubsStartAngle,
              TubsSpanningAngle);

  fReflectionFoilCryoFootBottomLogical = new G4LogicalVolume(fReflectionFoilCryoFootBottom,
              nylon,
              "fReflectionFoilCryoFootBottomLogical",
              0,
              0,
              0);


  fReflectionFoilCryoFootBottomLogical -> SetVisAttributes(VM2000VisAtt);

  // --------------------------------------------------------------------------
  // volume : reflector foil covering the inside top of the foot of the cryotank
  // --------------------------------------------------------------------------


  G4Polycone* fReflectionFoilCryoFootTop = Create_ReflectionFoilCryoFootTop();
  fReflectionFoilCryoFootTopLogical = new G4LogicalVolume((G4VSolid*)fReflectionFoilCryoFootTop,
              nylon,
              "fReflectionFoilCryoFootTopLogical",
              0,
              0,
              0);

  fReflectionFoilCryoFootTopLogical -> SetVisAttributes(VM2000VisAtt);

  // --------------------------------------------------------------------------
  // volume : Cable Tray for PMT cables on Top of Water
  // --------------------------------------------------------------------------

  TubsStartAngle    = 0.0   * deg;
  TubsSpanningAngle = 360.0 * deg;


  G4Trap* fVerticalPMTCableTray = new G4Trap("fVerticlPMTCableTray",
              0.5*fPMTVerticalCableTrayHeight,
              39*deg,
              0*deg,
              0.5*fPMTCableTrayThickness,
              0.5*fPMTCableTrayThickness,
              0.5*fPMTCableTrayThickness,
              0*deg,
              0.5*fPMTCableTrayThickness,
              0.5*fPMTHorizontalCableTrayWidth,
              0.5*fPMTHorizontalCableTrayWidth,
              0*deg);


  G4Tubs* fHorizontalPMTCableTray = new G4Tubs("fHorizontalPMTCableTray",
              fShieldingWaterRadius-fReflectionFoilThickness-fPMTHorizontalCableTrayWidth,
              fShieldingWaterRadius-fReflectionFoilThickness,
              0.5*fPMTCableTrayThickness,
              TubsStartAngle,
              TubsSpanningAngle);


  fVerticalPMTCableTrayLogical = new G4LogicalVolume(fVerticalPMTCableTray,
              steel,
              "fVerticalPMTCableTreayLogical",
              0,
              0,
              0);


  fHorizontalPMTCableTrayLogical = new G4LogicalVolume(fHorizontalPMTCableTray,
              steel,
              "fHorizontalPMTCableTreayLogical",
              0,
              0,
              0);

  fHorizontalPMTCableTrayLogical -> SetVisAttributes(SteelVisAtt);
  fVerticalPMTCableTrayLogical -> SetVisAttributes(SteelVisAtt);



  // -------------------------------------------------
  // sensitive detector definition
  // -------------------------------------------------

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  GEGeometrySD* n2bufferSD     = new GEGeometrySD("/mydet/gerda/N2buffer");
  GEGeometrySD* waterbufferSD  = new GEGeometrySD("/mydet/gerda/waterbuffer");
  GEGeometrySD* CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
  GEGeometrySD* PassivationSD  = new GEGeometrySD("/mydet/gerda/passivation");
  GEGeometrySD* DeadLayerSD    = new GEGeometrySD("/mydet/gerda/deadlayer");
  GEGeometrySD* ScintillatorSD = new GEGeometrySD("/mydet/gerda/scintPlate");

  SDman -> AddNewDetector(n2bufferSD);
  SDman -> AddNewDetector(waterbufferSD);
  SDman -> AddNewDetector(CrystalSD);
  SDman -> AddNewDetector(PassivationSD);
  SDman -> AddNewDetector(DeadLayerSD);
  SDman -> AddNewDetector(ScintillatorSD);

  for (G4int i = 0; i < int(fCrystalSegmentLogicalContainer.size()); i++)
      fCrystalSegmentLogicalContainer[i] -> SetSensitiveDetector(CrystalSD);
  for (G4int i = 0; i < int(fCrystalPassivationLogicalContainer.size()); i++)
      fCrystalPassivationLogicalContainer[i] -> SetSensitiveDetector(PassivationSD);
  for (G4int i = 0; i < int(fCrystalDeadLayerLogicalContainer.size()); i++)
      fCrystalDeadLayerLogicalContainer[i] -> SetSensitiveDetector(DeadLayerSD);

  fScintillatorLogical -> SetSensitiveDetector(ScintillatorSD);
  //fLiquidN2Logical     -> SetSensitiveDetector(n2bufferSD);
  fCryostatParts->GetCryoInnerwall_Inside()->GetLogicalVolume() ->SetSensitiveDetector(n2bufferSD);
  fWaterLogical        -> SetSensitiveDetector(waterbufferSD);

  MGLog(debugging) << "END GEGeometryDetectorDB::ConstructGEGeometryDetectorDB()" << endlog;
}

// --------------------------------------------------------------------------------

void GEGeometryDetectorDB::Set_ZShiftScheme_CrystalOffset()
{

  // unshifted columns
  // -------------------
  if( fZShiftScheme==CrystalZShiftScheme_unshifted )
    {
      for(int i=0;i<nrOfStrings;i++) fZShiftScheme_CrystalOffset[i] = 0.0;
      return;
    }


  // CrystalZShiftScheme_center_between_top0_and_bottom2_on_common_zlevel
  // ----------------------------------------------------------------------
  else if( fZShiftScheme==CrystalZShiftScheme_center_between_top0_and_bottom2_on_common_zlevel)
    {

      G4int iusc = fZShiftScheme_UnshiftedColumn;
      G4double zCenter[nrOfStrings];
      G4bool   setShift[nrOfStrings];

      if(GetCrystalType(iusc,2)<0)
        {
          MGLog(error) << "The Reference column ("<< iusc <<") has less then 3 detectors --> abording." << endlog;
          abort();
        }

      // find center of each column
      for(int i=0;i<nrOfStrings;i++)
        {
          setShift[i] = true;

          GEGeometryDetectorParameters* dp[3];
          dp[0] = dp[1] = dp[2] = 0;
          for(int j=0;j<3;j++) if(GetCrystalType(i,j)>=0) dp[j] = GetDetectorParameters( GetCrystalType(i,j) );

          if( dp[0]==0 && dp[1]==0 && dp[2]==0) {setShift[i]=false;continue;}
          if(dp[2]==0)
            {
              MGLog(warning) << "WE NEED 3 CRYSTALS PER STRING - problem in col="
                             << i << " --> leave column unshifted"
                             << endlog;
              setShift[i]=false;
              continue;
            }
          if(dp[1]==0)
            {
              MGLog(warning) << "WE NEED 3 CRYSTALS PER STRING - problem in col="
                             << i << " --> leave column unshifted" << endlog;
              setShift[i]=false;
              continue;
            }

          G4double top    = GetCrystalPosition( i, 0, CrystalZShiftScheme_unshifted).getZ() + 0.5*(dp[0]->GetHeight());
          G4double bottom = GetCrystalPosition( i, 2, CrystalZShiftScheme_unshifted).getZ() - 0.5*(dp[2]->GetHeight());
          zCenter[i] = 0.5*(top+bottom);
        }

      // define shift
      for(int i=0;i<nrOfStrings;i++)
          if(setShift[i])
              fZShiftScheme_CrystalOffset[i] = zCenter[iusc]-zCenter[i];

    }
}

// --------------------------------------------------------------------------------

void GEGeometryDetectorDB::DefineVisualizationAttributes()
{

  // colors

  G4Colour myred     (1.0,  0.0,  0.0,  0.5);
  G4Colour orange  (0.75, 0.55, 0.0,  0.2);
  G4Colour darkgrey(0.7,  0.7,  0.7,  0.5);
  G4Colour silver  (0.2,  0.2,  0.2,  0.5);
  G4Colour mywhite   (1.0,  1.0,  1.0,  0.1);
//  G4Colour black   (0.0,  0.0,  0.0,  0.5);  <-- not used
  G4Colour lgrey   (0.8,  0.8,  0.8,  0.2);
  G4Colour lgrey2  (0.85, 0.85, 0.85, 0.5);
  G4Colour lgrey3  (0.93, 0.93, 0.93, 0.5);
  G4Colour lblue   (0.0,  0.0,  0.75, 0.1);
  G4Colour lgreen  (0.0,  0.75, 0.0) ;

// The following two colors repeat the standard ones (G4Colour::cyan and ::green) so are commented
//  G4Colour green   (0.0,  1.00, 0.0) ;
//  G4Colour cyan    (0.0,  1.0,  1.0) ;

  // visual attributes

  WaterVisAtt = new G4VisAttributes(lblue);
  WaterVisAtt -> SetVisibility(true);
  WaterVisAtt -> SetForceSolid(false);

  VacuumVisAtt = new G4VisAttributes(mywhite);
  VacuumVisAtt -> SetVisibility(true);
  VacuumVisAtt -> SetForceSolid(false);

  LN2VisAtt = new G4VisAttributes(lgrey);
  LN2VisAtt -> SetVisibility(true);
  LN2VisAtt -> SetForceSolid(false);

  N2VisAtt = new G4VisAttributes(lgrey2);
  N2VisAtt -> SetVisibility(true);
  N2VisAtt -> SetForceSolid(false);

  CuVisAtt = new G4VisAttributes(orange);
  CuVisAtt -> SetVisibility(true);
  CuVisAtt -> SetForceSolid(true);

  SteelVisAtt = new G4VisAttributes(mywhite);
  SteelVisAtt -> SetVisibility(true);
  SteelVisAtt -> SetForceSolid(false);

  CrystalSegmentVisAtt = new G4VisAttributes(myred);
  CrystalSegmentVisAtt -> SetVisibility(true);
  CrystalSegmentVisAtt -> SetForceSolid(true);

  CrystalPassivationVisAtt = new G4VisAttributes(orange);
  CrystalPassivationVisAtt -> SetVisibility(true);
  CrystalPassivationVisAtt -> SetForceSolid(false);

  CrystalDeadLayerVisAtt = new G4VisAttributes(green);
  CrystalDeadLayerVisAtt -> SetVisibility(true);
  CrystalDeadLayerVisAtt -> SetForceSolid(false);

  NiVisAtt = new G4VisAttributes(silver);
  NiVisAtt -> SetVisibility(true);
  NiVisAtt -> SetForceSolid(true);

  AuVisAtt = new G4VisAttributes(orange);
  AuVisAtt -> SetVisibility(true);
  AuVisAtt -> SetForceSolid(true);

  AlVisAtt = new G4VisAttributes(silver);
  AlVisAtt -> SetVisibility(true);
  AlVisAtt -> SetForceSolid(true);

  TeflonVisAtt = new G4VisAttributes(mywhite);
  TeflonVisAtt -> SetVisibility(true);
  TeflonVisAtt -> SetForceSolid(true);

  SiVisAtt = new G4VisAttributes(silver);
  SiVisAtt -> SetVisibility(true);
  SiVisAtt -> SetForceSolid(true);

  PhotocathodeVisAtt = new G4VisAttributes(lgreen);
  PhotocathodeVisAtt -> SetVisibility(true);
  PhotocathodeVisAtt -> SetForceSolid(true);

  VM2000VisAtt = new G4VisAttributes(cyan);
  VM2000VisAtt->SetVisibility(true);
  VM2000VisAtt -> SetForceSolid(true);

  NylonVisAtt = new G4VisAttributes(mywhite);
  NylonVisAtt -> SetVisibility(true);
  NylonVisAtt -> SetForceSolid(true);

  GluedNylonVisAtt = new G4VisAttributes(green);
  GluedNylonVisAtt -> SetVisibility(true);
  GluedNylonVisAtt -> SetForceSolid(true);

  MakrolonVisAtt = new G4VisAttributes(lgrey3);
  MakrolonVisAtt -> SetVisibility(true);
  MakrolonVisAtt -> SetForceSolid(false);

  CalibrationCylinderVisAtt = new G4VisAttributes(darkgrey);
  CalibrationCylinderVisAtt -> SetVisibility(true);
  CalibrationCylinderVisAtt -> SetForceSolid(true);

  CollimatedCalibrationCylinderVisAtt = new G4VisAttributes(darkgrey);
  CollimatedCalibrationCylinderVisAtt -> SetVisibility(true);
  CollimatedCalibrationCylinderVisAtt -> SetForceSolid(true);

}

// --------------------------------------------------------------------------------

void GEGeometryDetectorDB::DefineMaterials()
{
  air             = G4Material::GetMaterial("Air");
  water           = G4Material::GetMaterial("Water");
  vacuum          = G4Material::GetMaterial("Vacuum");
  liquidnitrogen  = G4Material::GetMaterial("LiquidNitrogen");
  liquidargon     = G4Material::GetMaterial("LiquidArgon");
  liquidxenon     = G4Material::GetMaterial("LiquidXenon");
  gaseousnitrogen = G4Material::GetMaterial("GaseousNitrogen");
  gaseousargon    = G4Material::GetMaterial("GaseousArgon");
  enrGe           = G4Material::GetMaterial("EnrichedGe");
  copper          = G4Material::GetMaterial("MetalCopper");
  plasticScint    = G4Material::GetMaterial("PlasticScintillator");
  lead            = G4Material::GetMaterial("MetalLead");
  steel           = G4Material::GetMaterial("Steel");
  teflon          = G4Material::GetMaterial("Teflon");
  si              = G4Material::GetMaterial("MetalSilicon");
  gold            = G4Material::GetMaterial("MetalGold");
  capsGold        = G4Material::GetMaterial("CapsGold");
  nickel          = G4Material::GetMaterial("MetalNickel");
  aluminium       = G4Material::GetMaterial("MetalAluminium");
  cathodeAl       = G4Material::GetMaterial("CathodeMetalAluminium");
  nylon           = G4Material::GetMaterial("Nylon");
  PET             = G4Material::GetMaterial("PET");
  kapton          = G4Material::GetMaterial("kapton");
  brass           = G4Material::GetMaterial("Brass");
  makrolon        = G4Material::GetMaterial("polycarbonate");
  torlon          = G4Material::GetMaterial("torlon");
  tungsten        = G4Material::GetMaterial("MetalTungsten");
  lngsrock        = G4Material::GetMaterial("LNGSRock");
  tantalum        = G4Material::GetMaterial("MetalTantalum");

  double densityHVCable = 3.636 * g/cm3;
  HVCableCuTeflonComposite = new G4Material("HVCableCuTeflonComposite",densityHVCable,2);
  HVCableCuTeflonComposite->AddMaterial(copper, 21.838 * perCent);
  HVCableCuTeflonComposite->AddMaterial(teflon, 78.162 * perCent);


  if (GetFlagCryoLiquid())
    {
      cryoliquid = liquidnitrogen;
      cryogas = gaseousnitrogen;
    }
  else
    {
      //cryoliquid = liquidxenon;
      cryoliquid = liquidargon;
      cryogas = gaseousargon;
    }

  MGLog(routine) << "The cryoliquid is " << cryoliquid -> GetName() << endlog;
}


// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::CreateHoldersForPhase1()
{
  for(unsigned int ii=0;ii<fDetectorParametersContainer.size();ii++)
    {
      GEGeometryDetectorParameters* detector = fDetectorParametersContainer[ii];
      assert(detector);
      if((detector->GetGeometry()==1) || (detector->GetGeometry()==4))
        {
          MGLog(debugging) <<
	    "   Starting to create Logical Volume of phase-1-holder, detector ID -->"<<ii<<"<--." <<  endlog;

          GEGeometryHolderPhase1Parts* holderPhase1 =
	    new GEGeometryHolderPhase1Parts(*fDefaultHolderPhase1Parts);

          // here reset parameters, e.g. star radius and so on
          if( detector->GetName()==G4String("ANG1") )
            {
              MGLog(debugging) << "Reset parameters of the holder of the ANG1 detector." << endlog;
              holderPhase1->SetMaximalAllowedCrystalRadius(detector->GetOuterRadius());
              holderPhase1->SetRadialDistance_CrystalSurface_Holder(7.5 * mm);
            }
          else if( detector->GetName().contains("GTF") )
            {
              MGLog(debugging) << "Reset parameters of the holder of an GTF detector." << endlog;
              holderPhase1->SetMaximalAllowedCrystalRadius(detector->GetOuterRadius());
            }
	  if (detector->GetGeometry()==4)
	    {
	      MGLog(debugging) << "Sono qui: " << ii << " " << detector->GetName() << " " <<
		detector->GetOuterRadius()/cm << endlog;
	    }
          holderPhase1->SetCrystalRadius(detector->GetOuterRadius());
          holderPhase1->SetCrystalInnerRadius(detector->GetInnerRadius());
          holderPhase1->SetCrystalHeight(detector->GetHeight());
          holderPhase1->SetCrystalID(detector->GetDetectorID());

          holderPhase1->ResetDependentParameters();

          holderPhase1->SetVisAtt_LowerStar(CuVisAtt);
          holderPhase1->SetVisAtt_UpperStar(CuVisAtt);
          holderPhase1->SetVisAtt_starArm(CuVisAtt);
          holderPhase1->SetVisAtt_VerticalBar(CuVisAtt);
          holderPhase1->SetVisAtt_LowerContactDisc(CuVisAtt);
          holderPhase1->SetVisAtt_LowerPTFETube(TeflonVisAtt);
          holderPhase1->SetVisAtt_LowerPressurePlate(CuVisAtt);
          holderPhase1->SetVisAtt_LowerGrubScrew(CuVisAtt);
          holderPhase1->SetVisAtt_UpperSpecialNut(CuVisAtt);
          holderPhase1->SetVisAtt_UpperCounterNut(CuVisAtt);
          holderPhase1->SetVisAtt_UpperPTFERing(TeflonVisAtt);
          holderPhase1->SetVisAtt_UpperChineseHat(CuVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_1_Isolator(TeflonVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_2_SiliconGuider(SiVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_3_Isolator(TeflonVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_4_SiliconSpring(SiVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_5_Isolator(TeflonVisAtt);
          holderPhase1->SetVisAtt_UpperSmall_6_InnerSpecialNut(CuVisAtt);


          holderPhase1->SetMaterial_LowerStar(copper);
          holderPhase1->SetMaterial_UpperStar(copper);
          holderPhase1->SetMaterial_starArm(copper);
          holderPhase1->SetMaterial_VerticalBar(copper);
          holderPhase1->SetMaterial_LowerContactDisc(copper);
          holderPhase1->SetMaterial_LowerPTFETube(teflon);
          holderPhase1->SetMaterial_LowerPressurePlate(copper);
          holderPhase1->SetMaterial_LowerGrubScrew(copper);
          holderPhase1->SetMaterial_UpperSpecialNut(copper);
          holderPhase1->SetMaterial_UpperCounterNut(copper);
          holderPhase1->SetMaterial_UpperPTFERing(teflon);
          holderPhase1->SetMaterial_UpperChineseHat(copper);
          holderPhase1->SetMaterial_UpperSmall_1_Isolator(teflon);
          holderPhase1->SetMaterial_UpperSmall_2_SiliconGuider(si);
          holderPhase1->SetMaterial_UpperSmall_3_Isolator(teflon);
          holderPhase1->SetMaterial_UpperSmall_4_SiliconSpring(si);
          holderPhase1->SetMaterial_UpperSmall_5_Isolator(teflon);
          holderPhase1->SetMaterial_UpperSmall_6_InnerSpecialNut(copper);

          holderPhase1->CreateSolids();
          holderPhase1->CreateLogicalVolumes();

          fHolderPhase1Container.push_back( holderPhase1 );

          MGLog(debugging) << "   Ending to create Logical Volume of phase-1-holder, detector ID -->"<<ii<<"<--." <<  endlog;
        }

      // this is important to get the correct Holder with the function GEGeometryDetectorDB::GetHolderStarLogical
      // Don't delete this 'else' command
      else
        {
          fHolderPhase1Container.push_back( 0 );
        }
    }
  MGLog(debugging) << "Logical Volumes of phase-1-holders are created." <<  endlog;
}

// ---------------------------------------------------------------------------
G4double GEGeometryDetectorDB::MatchDetectorIDwithHolderBarsPhase2(G4int detectorID, G4int detectorGeometry)
{
   G4int crystal_type=0;
   G4int crystal_typeL=0;
   G4double copperbarlength=10.*cm;//do not set to zero, solids for holders for all detectors in geometry file are constructed, regardless if they are in the matrix file or not
   if (fFlagCrystalMatrixWithZPosFile){
 for (G4int i=0; i<nrOfStrings; i++)
	  for (G4int j=0; j < nrOfRows; ++j )
	  {
	  crystal_type= GetCrystalType(i,j,0);
	  crystal_typeL= GetCrystalType(i,j,-1);
	  //get individual copperbarlengths if detectors are positioned individually
	  if (fFlagCrystalMatrixWithZPosFile){
	  	if (crystal_type==detectorID) copperbarlength=GetCrystalIndZPosition(i,j)*cm+fDistanceBetweenHolderBarsPhase2;
	 	 }
	  //if there is no individual detector placement shorter bars for single BEGes required to avoid overlaps
	  else if (detectorGeometry==5 && crystal_typeL<0) copperbarlength=50.*mm;
	  //MGLog(trace) <<crystal_type << " " << detectorid << endlog;
	  }
	 }
  return copperbarlength;
}

// ---------------------------------------------------------------------------
void GEGeometryDetectorDB::CreateHoldersForPhase2()
{
  for(unsigned int ii=0;ii<fDetectorParametersContainer.size();ii++)
    {
      GEGeometryDetectorParameters* detector = fDetectorParametersContainer[ii];

      assert(detector);
      if(detector->GetGeometry()==3 || detector->GetGeometry()==5 || detector->GetGeometry()==11)
        {
          MGLog(debugging) << "   Starting to create Logical Volume of phase-2-holder, detector ID -->"<<
	    ii<<"<--." <<  endlog;

          GEGeometryHolderPhase2Parts* holderPhase2 =
	    new GEGeometryHolderPhase2Parts(*fDefaultHolderPhase2Parts);

          holderPhase2->SetCrystalRadius(detector->GetOuterRadius());
          holderPhase2->SetCrystalInnerRadius(detector->GetInnerRadius());
          holderPhase2->SetCrystalHeight(detector->GetHeight());
          holderPhase2->SetCrystalID(detector->GetDetectorID());
	  holderPhase2->SetLengthOfCopperBars(MatchDetectorIDwithHolderBarsPhase2(detector->GetDetectorID(),detector->GetGeometry()));

          holderPhase2->ResetDependentParameters();

          holderPhase2->SetVisAtt_LowerPlate(SiVisAtt);
          holderPhase2->SetVisAtt_UpperPlate(SiVisAtt);
          holderPhase2->SetVisAtt_VerticalBar(CuVisAtt);

          holderPhase2->SetMaterial_LowerPlate(si);
          holderPhase2->SetMaterial_UpperPlate(si);
          holderPhase2->SetMaterial_VerticalBar(copper);

          holderPhase2->CreateSolids();
          holderPhase2->CreateLogicalVolumes();

          fHolderPhase2Container.push_back( holderPhase2 );

          MGLog(debugging) << "   Ending to create Logical Volume of phase-2-holder, detector ID -->"
			   <<ii<<"<--." <<  endlog;
        }

      // this is important to get the correct Holder with the function GEGeometryDetectorDB::GetHolderStarLogical
      // Don't delete this 'else' command
      else
        {
          fHolderPhase2Container.push_back( 0 );
        }
    }
  MGLog(debugging) << "Logical Volumes of phase-2-holders are created." <<  endlog;
}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::FindUppermostCrystalSurface()
{
  MGLog(debugging) << "START GEGeometryDetectorDB::FindUppermostCrystalSurface()" << endlog;
  G4double zMax = -1.e9*m;
  G4bool zValueWasReset=false;

  // depending on geometry1 or geometry2 there is different extra-space between
  //  upper edge of crystal; and lower edge of ElectronicsSideBar
  G4double dz_geom1, dz_geom2;
  dz_geom1 = (fDefaultHolderPhase1Parts->GetTopDistance_CrystalSurface_Holder()
              + ( fDefaultHolderPhase1Parts->GetZzHldStarUpperTube(7) -
		  fDefaultHolderPhase1Parts->GetZOverlap_UpperStarTube_SpecialNut())
              + fDefaultHolderPhase1Parts->GetSpclNut_ZPosOfSuspensionHoleTop());
  dz_geom2 = 0.5 * fHolderSideBarHeight;

  for(G4int i=0;i<nrOfStrings;i++)
    {
      MGLog(debugging) << "i = col = "<< i << endlog;

      GEGeometryDetectorParameters* dp(0);
      if(GetCrystalType(i,0)>=0) dp = GetDetectorParameters( GetCrystalType(i,0) );
      if(dp!=0)
        {
          G4double zi=0;

          zi =  GetCrystalPosition(i, 0).getZ();
          zi += 0.5*dp->GetHeight();
          G4double ztmp = zi;
          if(dp->GetGeometry()==1 || dp->GetGeometry()==4)  {
        	  zi += dz_geom1;
          }
          if(dp->GetGeometry()==2)  {
        	  zi += dz_geom2;
          }

          MGLog(debugging) << "  dp!=0  zi=="<< zi << endlog;
          if(zi>zMax)
            {
              zMax = zi;
              fZUppermostCrystalSurface = ztmp;
              fColumnWithUppermostCrystalSurface = i;
              fCrystalGeometryOfUppermostCrystalSurface = dp->GetGeometry();
              fCrystalTypeOfUppermostCrystalSurface     = dp->GetType();
              zValueWasReset=true;
            }
        }
    }
  if(!zValueWasReset) abort();
  MGLog(debugging) << "END GEGeometryDetectorDB::FindUppermostCrystalSurface()" << endlog;
}

//
// ---------------------------------------------------------------------------

G4double GEGeometryDetectorDB::GetZLowermostCrystalSurface()
{
	// This one works a bit differently.
	// We have to scan over the different rows to get the
	// lower possible position
  MGLog(debugging) << "START GEGeometryDetectorDB::FindLowermostCrystalSurface()" << endlog;

  G4double zMin = 1.e9*m;
  G4bool zValueWasReset=false;

  // depending on geometry1 or geometry2 there is different extra-space between
  //  below the lower edge of the crystal
  G4double dz_geom1, dz_geom2;
  dz_geom1 = (fDefaultHolderPhase1Parts->GetTopDistance_CrystalSurface_Holder()
              + ( fDefaultHolderPhase1Parts->GetZzHldStarUpperTube(7) -
		  fDefaultHolderPhase1Parts->GetZOverlap_UpperStarTube_SpecialNut())
              + fDefaultHolderPhase1Parts->GetSpclNut_ZPosOfSuspensionHoleTop());
  dz_geom2 = 0.5 * fHolderSideBarHeight;

  for (G4int i=0; i<nrOfStrings; i++)
	  for (G4int j=0; j < nrOfRows; ++j )
    {
      MGLog(debugging) << "i = col = "<< i << endlog;

      GEGeometryDetectorParameters* dp(0);
      if(GetCrystalType(i,0)>=0) dp = GetDetectorParameters( GetCrystalType(i,0) );
      if(dp!=0)
        {
          G4double  zb = 0;

          zb = GetCrystalPosition(i, j).getZ();
          zb -= 0.5*dp->GetHeight();
          G4double ztmp = zb;
          if(dp->GetGeometry()==1 || dp->GetGeometry()==4 )  {
        	  zb -= dz_geom1;
          }
          if(dp->GetGeometry()==2)  {
        	  zb -= dz_geom2;
          }
          if(dp->GetGeometry()==3)  {
        	  //TODO missing for type 3 crystals i.e. b2b beges
        	  // zb -= ???;
          }

          MGLog(debugging) << "  dp!=0  zb==" << zb << endlog;

          if(zb<zMin)
            {
              zMin = zb;
              fZLowermostCrystalSurface = ztmp;
              fColumnWithLowermostCrystalSurface = i;
              fCrystalGeometryOfLowermostCrystalSurface = dp->GetGeometry();
              fCrystalTypeOfLowermostCrystalSurface     = dp->GetType();
              zValueWasReset=true;
            }
        }
    }
  if(!zValueWasReset) abort();
  MGLog(debugging) << "END GEGeometryDetectorDB::FindLowermostCrystalSurface()" << endlog;
  return fZLowermostCrystalSurface;
}


// ---------------------------------------------------------------------------

G4ThreeVector GEGeometryDetectorDB::GetCrystalPosition(G4int col, G4int row,
                   GEGeometryCrystalZShiftScheme_t fScheme,
                   GEGeometryLockType_t usedLockType)
{

  MGLog(debugging) << "START GEGeometryDetectorDB::GetCrystalPosition(col=="<<col<<","<<row<<")" << endlog;

  GEGeometryCrystalZShiftScheme_t usedScheme = fScheme;
  if( usedScheme == CrystalZShiftScheme_automatic ) usedScheme = fZShiftScheme;


  if (usedLockType == LockType_automatic) usedLockType = fLockType;
  // LockType has here just an effect on the xy position of the detectors


  G4ThreeVector position;

  G4double x = 0.0;
  G4double y = 0.0;
  G4double z = 0.0;
  G4double z1 = 0.0;
  G4double z2 = 0.0;
  G4double up_check=0.0;
  G4double down_check=0.0;
  G4double  z_phase2= 0.0;
  G4double r;
  const G4double dphi = 2 * pi / 6 * rad;

  // z-position
  // ------------
  // this z-value is the standard value
  z = GetArrayDepth()  + (2.5 - float(0)) * (GetDistanceHolderToHolder());
  z_phase2=GetArrayDepth()  + fPositionOfTopCrystalinCryostat;

  if(usedScheme!=CrystalZShiftScheme_unshifted) z += fZShiftScheme_CrystalOffset[col];


  G4int crystalID = GetCrystalType(col,row);
  if( crystalID>=0 )
    {
      GEGeometryDetectorParameters* detectorparams(0);
      detectorparams = GetDetectorParameters( crystalID );
      if(detectorparams!=0)
        {
          // phase-1 string
          if( detectorparams->GetGeometry()==1 || detectorparams->GetGeometry()==4)
            {
              GEGeometryDetectorParameters* dp_row0 = GetDetectorParameters( GetCrystalType(col,0) );
              z = z
                  /*  half of phase-2 */  + 0.5 * 7.0*cm
                  /*  half of phase-1 */  - 0.5 * dp_row0->GetHeight();
              if(row<0) abort();
              for(int jj=row;jj>0;jj--)
                {
                  GEGeometryHolderPhase1Parts* holder1 = GetHolderPhase1Parts( GetCrystalType(col,jj-1) );
                  GEGeometryHolderPhase1Parts* holder2 = GetHolderPhase1Parts( GetCrystalType(col,jj)   );
                  z = z
                    - holder1->GetZDistanceToSuspensionPlaneBottom()
                    - holder2->GetZDistanceToSuspensionPlaneTop();
                }
            }
	  // phase-2 string
	  else if ( detectorparams->GetGeometry()==5 || detectorparams->GetGeometry()==11)
	    {
	    if (fFlagCrystalMatrixWithZPosFile)
  		{
	       //place the detectors
	       GEGeometryDetectorParameters* dp = GetDetectorParameters( GetCrystalType(col,row,0) );
	       GEGeometryDetectorParameters* dp_lower = GetDetectorParameters( GetCrystalType(col,row,1) );
	       GEGeometryHolderPhase2Parts* holder1 = GetHolderPhase2Parts( GetCrystalType(col,row,0) );
	       z1 = z_phase2-(GetCrystalIndZPosition(col,0)*cm+fDistanceBetweenHolderBarsPhase2);
	       z2 = z_phase2;
	       if(row<0) abort();
	       for(int jj=row;jj>0;jj--)
	       {z1 = z1 - (GetCrystalIndZPosition(col,jj)*cm+fDistanceBetweenHolderBarsPhase2);
	        z2 = z2 - (GetCrystalIndZPosition(col,jj-1)*cm+fDistanceBetweenHolderBarsPhase2); //detector above current detector for overlap checks
	       }

	       //go from end of holder bar to middle of crystal
	       if (IsAPair(col,row))
	       {z = z1 + dp_lower->GetHeight()+0.5*fCrystalPairGap+holder1->GetZDistanceToEndOfSiliconPlate()+1.1*cm+fDistanceBetweenHolderBarsPhase2;}
	       else{
		z = z1 + 0.5*dp->GetHeight()+holder1->GetZDistanceToEndOfSiliconPlate()+1.1*cm+fDistanceBetweenHolderBarsPhase2;
		}

		//check for overlaps
		if(row>0)
		{
                GEGeometryHolderPhase2Parts* holder2 = GetHolderPhase2Parts( GetCrystalType(col,row,1)   );

		if (IsAPair(col,row)){up_check=z+0.5*fCrystalPairGap+holder2->GetZDistanceToEndOfSiliconPlatePair();}
		 else {up_check= z + 0.5*dp->GetHeight();}
		down_check= z2 + 1.1*cm+fDistanceBetweenHolderBarsPhase2;

		//MGLog(debugging) <<up_check-z2<<endlog;

		if (up_check >= down_check)
		{
		    MGLog(error) << "There is an overlap between detector holder units."<<endlog;
		    MGLog(error) << "It is between Position (col="<<col<<", row="<<row-1<<") and Position (col="<<col<<", row="<<row<<")."<<endlog;
		    MGLog(error) << down_check << " " << up_check <<endlog;
		    abort();
		}

		}
	    }
	    else
	    {
              z = z - ((float)(row)) * (GetDistanceHolderToHolder());
            }
	    }
	  //anything else --> standard value
	  else
            {
              z = z - ((float)(row)) * (GetDistanceHolderToHolder());
            }

        }

      // * no detector has been set at this position
      //   --> standard value could cause overlaps if there is a phase-1 detector above this position
      // * normaly this should not happen
      else
        {
          MGLog(error) << "GetCrystalPosition(col="<<col<<", row="<<row<<") has been called."<<endlog;
          MGLog(error) << "But at this position there is no detector... ..abort now."<<endlog;
          abort();
        }
    }


  // x-y-position
  // ------------

  // Final lock

  if (usedLockType == LockType_final)
    {
      fNlocks = 1;
      fStringDetectorCenter = new G4ThreeVector[fNlocks];
      fStringDetectorCenter[0] = G4ThreeVector(0.0,0.0,0.0);

      G4double r4 = GetPhase1RadiusPos4();
      G4double r5 = GetPhase1RadiusPos5();

      r = GetColumnDistanceR();

      if (col == 0)
        {
          x = r * sin(dphi * 0) + r * sin(dphi * 0);
          y = r * cos(dphi * 0) + r * cos(dphi * 0);
        }

      else if (col == 1)
        {
          x = r * sin(dphi * 0) + r * sin(dphi * 1);
          y = r * cos(dphi * 0) + r * cos(dphi * 1);
        }

      else if (col == 2)
        {
          x = r * sin(dphi * 1) + r * sin(dphi * 1);
          y = r * cos(dphi * 1) + r * cos(dphi * 1);
        }

      else if (col == 3)  // calibration source
        {
          x = r * sin(dphi * 1) + r * sin(dphi * 2);
          y = r * cos(dphi * 1) + r * cos(dphi * 2);
        }

      else if (col == 4)  // Phase1
        {
          //x = r * sin(dphi * 2) + r * sin(dphi * 2);
          //y = r * cos(dphi * 2) + r * cos(dphi * 2);
          const G4double d3 =
            0.5*80.*mm +   // radius of Crystal #4
            0.5*80.*mm +   // radius of Crystal #5
            10.*mm;        // distance between surfaces of both crystals
          const G4double d2 = r4 - r5;
          const G4double d1 = sqrt(d3*d3-d2*d2);

          const G4ThreeVector pos5  = GetCrystalPosition(5 ,0);
          const G4double phi5  = atan( fabs(pos5.getY()) / fabs(pos5.getX()) );
          const G4double dphi2 = 2. * asin(   (d1/2.0) / r5  );
          const G4double phi4  = phi5 - dphi2;
          x =  r4 * cos(phi4);
          y = -r4 * sin(phi4);
        }

      else if (col == 5)  // Phase1
        {
          //x = r * sin(dphi * 3) + r * sin(dphi * 2);
          //y = r * cos(dphi * 3) + r * cos(dphi * 2);
          G4ThreeVector pos6  = GetCrystalPosition(6 ,0);
          G4ThreeVector pos15 = GetCrystalPosition(15,0);
          y = 0.5*  (pos6.getY() + pos15.getY());
          x = sqrt(r5*r5-y*y);
        }

      else if (col == 6)
        {
          x = r * sin(dphi * 3) + r * sin(dphi * 3);
          y = r * cos(dphi * 3) + r * cos(dphi * 3);
        }

      else if (col == 7)  // calibration source
        {
          x = r * sin(dphi * 3) + r * sin(dphi * 4);
          y = r * cos(dphi * 3) + r * cos(dphi * 4);
        }

      else if (col == 8)
        {
          x = r * sin(dphi * 4) + r * sin(dphi * 4);
          y = r * cos(dphi * 4) + r * cos(dphi * 4);
        }

      else if (col == 9)
        {
          x = r * sin(dphi * 5) + r * sin(dphi * 4);
          y = r * cos(dphi * 5) + r * cos(dphi * 4);
        }

      else if (col == 10)
        {
          x = r * sin(dphi * 5) + r * sin(dphi * 5);
          y = r * cos(dphi * 5) + r * cos(dphi * 5);
        }

      else if (col == 11)  // calibration source
        {
          x = r * sin(dphi * 5) + r * sin(dphi * 0);
          y = r * cos(dphi * 5) + r * cos(dphi * 0);
        }

      else if (col == 12)
        {
          x = r * sin(dphi * 0);
          y = r * cos(dphi * 0);
        }

      else if (col == 13)
        {
          x = r * sin(dphi * 1);
          y = r * cos(dphi * 1);
        }

      else if (col == 14)  // Phase1
        {
          // no modification -- Crystal remains at old position
          x = r * sin(dphi * 2);
          y = r * cos(dphi * 2);
        }

      else if (col == 15)
        {
          x = r * sin(dphi * 3);
          y = r * cos(dphi * 3);
        }

      else if (col == 16)
        {
          x = r * sin(dphi * 4);
          y = r * cos(dphi * 4);
        }

      else if (col == 17)
        {
          x = r * sin(dphi * 5);
          y = r * cos(dphi * 5);
        }

      else if (col == 18)
        {
          x = 0.0;
          y = 0.0;
        }
    }


  // Comm lock with 2 detector strings plus calibration source

//   else if (usedLockType == LockType_CommLock2)
//     {
//
//       if (col == 0)
//         {                // calibration source
//           x = 0.0;
//           y = 240.0 * mm;
//         }
//
//       else if (col == 1)
//         {
//           x = 150.0 * mm;
//           y = 0.0;
//         }
//
//       else if (col == 2)
//         {
//           x = -150.0 * mm;
//           y = 0.0;
//         }
//
//       else
//         {
//           MGLog(error) << "Too many strings declared in this scope. There are just 2 detector strings + 1 calibration source"<<endlog;
//         }
//
//     }


      else if (usedLockType == LockType_CommLock2)
    {
          fNlocks = 2;
          fStringDetectorCenter = new G4ThreeVector[fNlocks];
          fStringDetectorCenter[0] = G4ThreeVector(-54.575 * mm,0.0,0.0);
          fStringDetectorCenter[1] = G4ThreeVector(-165.425 * mm,0.0,0.0);


//       if (col == 0)
//         {                // calibration source
//           x = 0.0;
//           y = 240.0 * mm;
//         }

      if (col == 0)
        {
          x = -54.575 * mm;
          y = 0.0;
        }

      else if (col == 1)
        {
          x = -165.425 * mm;
          y = 0.0;
        }

      else
        {
          MGLog(error) << "Too many strings declared in this scope. There are just 2 detector strings + 1 calibration source"<<endlog;
        }

    }


  // Comm lock with 4 detector strings plus three calibration sources

  else if (usedLockType == LockType_CommLock4)
    {

      // The center of the used coordinate system is the center of the cluster flange which hosts
      // the flanges for the detector strings as well as the for the calibration sources. The x
      // axis is the connection of the centers of the two detector flanges, the bigger one on neg
      // x, the small on on positive x.
      // Det1 in the nomenclature referce to the small detector flange; Det2 to the bigger flange
      // which will host three detector strings.


      // Parameters for detector strings
      G4double xFlangeDet1 = 190.0;
      G4double xFlangeDet2 = 110.0;

      // There are two possible strings.
      fNlocks = 2;
      fStringDetectorCenter = new G4ThreeVector[fNlocks];
      fStringDetectorCenter[0] = G4ThreeVector(xFlangeDet1,0.0,0.0);
      fStringDetectorCenter[1] = G4ThreeVector(-xFlangeDet2,0.0,0.0);

      G4double distDet2 = 110.85;       // Distance between detector strings in Det2 (Nov 2011)

      // Angles to define the location of the three detectors in the big flange
      G4double alphaDet2 = 0.0; // angle between x axis and CenterDet2 - CenterString1
      G4double betaDet2 = 60.0/180.0*M_PI - alphaDet2;
      G4double gammaDet2 = 120.0/180.0*M_PI - alphaDet2;
      G4double circumradius = distDet2 / 3.0 * sqrt(3);

      // Coordinates for calibration sources hard coded below due to asymmetries


      // The Four Detector Strings

      // One string in the small flange
      if (col == 0)
        {
          x = xFlangeDet1 * mm;
          y = 0.0;
        }

      // Three strings in the large flange
      else if (col == 1)
        {
          x = - xFlangeDet2 + circumradius * cos(alphaDet2) *mm;
          y = circumradius * sin(alphaDet2) *mm;
        }

      else if (col == 2)
        {
          x = - xFlangeDet2 - circumradius * cos(betaDet2) *mm;
          if (alphaDet2 >= 60/180*M_PI)                 // Make sure that y has the right sign because
              y = circumradius * sin(betaDet2) *mm;       // point is close to y=0.
          else
              y =  - circumradius * sin(betaDet2) *mm;
        }

      else if (col == 3)
        {
          if (gammaDet2 > 45/180*M_PI)                               // Need to know if to add or substract
              x = - xFlangeDet2 + circumradius * cos(gammaDet2) *mm;    // from xFlangeDet1.
          else
              x = - xFlangeDet2 - circumradius * cos(gammaDet2) *mm;

          y = - circumradius * sin(gammaDet2) *mm;
        }


      // Three Calibration Sources
      else if (col == 4)     // top right
        {
          //x = 106.0 * mm;
          //y = 100.0 * mm;
	  if( fFlagConstructCalibrationSource == true){
	    x += fCalibrationSourcePositionX_S1;
	    y += fCalibrationSourcePositionY_S1;
	    z += fCalibrationSourcePositionZ_S1;
	  }else if( fFlagConstructCollimatedCalibrationSource == true ){
	    x += fCollimatedCalibrationSourcePositionX_S1;
	    y += fCollimatedCalibrationSourcePositionY_S1;
	    z += fCollimatedCalibrationSourcePositionZ_S1;
	  }
        }

      else if (col == 5)    // top left
        {
          //x = -145.0 * mm;
          //y =  197.0 * mm;
	  if( fFlagConstructCalibrationSource == true){
	    x += fCalibrationSourcePositionX_S2;
	    y += fCalibrationSourcePositionY_S2;
	    z += fCalibrationSourcePositionZ_S2;
	  }else if( fFlagConstructCollimatedCalibrationSource == true ){
	    x += fCollimatedCalibrationSourcePositionX_S2;
	    y += fCollimatedCalibrationSourcePositionY_S2;
	    z += fCollimatedCalibrationSourcePositionZ_S2;
	  }
        }

      else if (col == 6)     // bottom left
        {
          //x = - 145.0 * mm;
          //y = - 197.0 * mm;
	  if( fFlagConstructCalibrationSource == true){
	    x += fCalibrationSourcePositionX_S3;
	    y += fCalibrationSourcePositionY_S3;
	    z += fCalibrationSourcePositionZ_S3;
	  }else if( fFlagConstructCollimatedCalibrationSource == true ){
	    x += fCollimatedCalibrationSourcePositionX_S3;
	    y += fCollimatedCalibrationSourcePositionY_S3;
	    z += fCollimatedCalibrationSourcePositionZ_S3;
	  }
        }
    }
      // Phase II preliminary lock design.
  	  // FIXME: Need to implement the string plate to switch from 1 string to 3 strings as shown in the designs.
  	  // For the moment 6 strings are implemented. 3 for each flange

      else if ( usedLockType == LockType_PhaseII )
        {

          // The center of the used coordinate system is the center of the cluster flange which hosts
          // the flanges for the detector strings as well as the for the calibration sources. The x
          // axis is the connection of the centers of the two detector flanges.
    	  // The DN250 CF from Phase I is located on negative x, the new one on positive x.
          // Det1 in the nomenclature reference for the new flange; Det2 to the old flange
          // Each of these host three detector strings.
    	  // For the moment a "symmetric design" is implemented.


          // Parameters for detector strings
    	  // Distance between flange center and lock center along x
          G4double xFlangeDist = 156.5;

          // This lock has two identical flanges at the same distance from the center
          // There are two possible strings.
          fNlocks = 2;
          fStringDetectorCenter = new G4ThreeVector[fNlocks];
          fStringDetectorCenter[0] = G4ThreeVector(xFlangeDist,0.0,0.0);
          fStringDetectorCenter[1] = G4ThreeVector(-xFlangeDist,0.0,0.0);


          //G4double xFlangeDet2 = -156.5;

          // Distance between detector strings
          // TODO: Review this distance. Shouldn't this be sqrt(60^2) = 84.8528 ??
          G4double distDet2 = 108.0;       // Distance between detector strings in Det2 (Phase I detectors)
          G4double distDet1 = 108.0;       // Distance between detector strings in Det1 (Phase II BEGe's)

          // Angles to define the location of the three detectors in the Det2 flange (old)
          G4double alphaDet2 = M_PI; // angle between x axis and CenterDet2 - CenterString1
          G4double betaDet2 = 60.0/180.0*M_PI - alphaDet2;
          G4double gammaDet2 = 120.0/180.0*M_PI - alphaDet2;
          G4double circumradius2 = distDet2 / 3.0 * sqrt(3);

          // Angles to define the location of the three detectors in the Det1 flange
          G4double alphaDet1 = 0.0; // angle between x axis and CenterDet2 - CenterString1
          G4double betaDet1 = 60.0/180.0*M_PI - alphaDet1;
          G4double gammaDet1 = 120.0/180.0*M_PI - alphaDet1;
          G4double circumradius1 = distDet1 / 3.0 * sqrt(3);

          // Coordinates for calibration sources hard coded below due to asymmetries

          // The Six Detector Strings
          // TODO: review the string names and positions

          // Three strings in the Det1 (new) flange
          if (col == 0)
            {
              x = xFlangeDist + circumradius2 * cos(alphaDet2) *mm;
              y = circumradius2 * sin(alphaDet2) *mm;
            }

          else if (col == 1)
            {
              x = xFlangeDist - circumradius2 * cos(betaDet2) *mm;
              if (alphaDet2 >= 60/180*M_PI)                 // Make sure that y has the right sign because
                  y = circumradius2 * sin(betaDet2) *mm;       // point is close to y=0.
              else
                  y =  - circumradius2 * sin(betaDet2) *mm;
            }

          else if (col == 2)
            {
              if (gammaDet2 < 45/180*M_PI)                               // Need to know if to add or substract
                  x = xFlangeDist + circumradius2 * cos(gammaDet2) *mm;    // from xFlangeDet1.
              else
                  x = xFlangeDist - circumradius2 * cos(gammaDet2) *mm;

              y = - circumradius2 * sin(gammaDet2) *mm;
            }

          // Three strings in the Det2 (old) flange
          else if (col == 3)
            {
              x = - xFlangeDist + circumradius1 * cos(alphaDet1) *mm;
              y = circumradius1 * sin(alphaDet1) *mm;
            }

          else if (col == 4)
            {
              x = - xFlangeDist - circumradius1 * cos(betaDet1) *mm;
              if (alphaDet1 >= 60/180*M_PI)                 // Make sure that y has the right sign because
                  y = circumradius1 * sin(betaDet1) *mm;       // point is close to y=0.
              else
                  y =  - circumradius1 * sin(betaDet1) *mm;
            }

          else if (col == 5)
            {
              if (gammaDet1 > 45/180*M_PI)                               // Need to know if to add or substract
                  x = - xFlangeDist + circumradius1 * cos(gammaDet1) *mm;    // from xFlangeDet1.
              else
                  x = - xFlangeDist - circumradius1 * cos(gammaDet1) *mm;

              y = - circumradius1 * sin(gammaDet1) *mm;
            }


          // FIXME: Have to replace this by the real locations of the calibration strings.
          // Three Calibration Sources
          else if (col == 6)     // top right
            {
              x = 106.0 * mm;
              y = 100.0 * mm;
            }

          else if (col == 7)    // top left
            {
              x = -145.0 * mm;
              y =  197.0 * mm;
            }

          else if (col == 8)     // bottom left
            {
              x = - 145.0 * mm;
              y = - 197.0 * mm;
            }

      else
        {
          MGLog(error) << "Too many strings declared in this scope ( " << usedLockType << " ). There are just 6 detector strings + 3 calibration source strings. "<<endlog;
          MGLog(error) << "Currently testing " << col << endlog;
        }
        MGLog(trace) << "Deploying detector in column " << col << " , row " << row << " position ( " << x << " , " << y << ", " <<  z << " ) mm" << endlog;


        }
        else if ( usedLockType == LockType_PhaseII_DN500 ) {
        	// This defines the spacing between the strings.
        	// Always assume equal spacing between all crystals.
        	// position 0 is the central one
            fNlocks = 1;
            fStringDetectorCenter = new G4ThreeVector[fNlocks];
            fStringDetectorCenter[0] = G4ThreeVector(0.0,0.0,0.0);

        	G4int nstrings = GetDN500NStrings();

        	// Angular distance between strings
    		G4double delta_angle = (360.0/(G4double)(nstrings-1))*degree;
    		G4double angle_init = 0*degree;
        	r = this->GetDN500StringDistance();

        	// The first string is the central one.
        	if ( col == 0) {
        		x = 0;
        		y = 0;
        	} else {

        		// place the columns clockwise starting over the X axis
        		// The positioning can be done automatically.
        		// The position will be completely dependent on the angle
        		// We just need to keep rotating the position by 60 degrees to get the
        		// column position

        		x = r*cos(angle_init + (col-1)*delta_angle);
        		y = r*sin(angle_init + (col-1)*delta_angle);

        		// Should check that there won't be any overlap between the crystals
        		// But it is impossible to know where crystals are actually being placed
        	}

        } else {
        MGLog(error) << "Unknown lock type ( " << usedLockType << " )."<<endlog;
        abort();
    }


  // For all cases

  position.setX(x);
  position.setY(y);
  position.setZ(z);

  MGLog(debugging) << "END GEGeometryDetectorDB::GetCrystalPosition(col=="<<col<<","<<row<<")" << endlog;

  return position;

}

// ---------------------------------------------------------------------------

G4double GEGeometryDetectorDB::GetRelativeSegmentZPosition(GEGeometryDetectorParameters* detectorparameters, G4int row)
{

  G4double z;

  if (detectorparameters -> GetNZSegments() == 1)
      z = 0.0;
  else
      z = 0.5 * detectorparameters -> GetHeight()
        - detectorparameters -> GetPassivationThickness()
        - detectorparameters -> GetDLTopThickness()
        - 0.5 * detectorparameters -> GetSegmentHeight()
        - double(row) * detectorparameters -> GetSegmentHeight();

  return z;

}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::OpenInputFiles()
{

  // ---------------------------------------------------------------
  // read in detector geometries
  // ---------------------------------------------------------------

  fInputFileGeometry.open(GetCrystalGeometryFilename());

  // read in geometries

  std::stringstream datastream;
  std::string       datastring;

  // If there is a new line in the end of file, ifstream does not
  // reach the EOF symbol after scanning the last item. To prevent that,
  // read the file line-by-line and check if the string is empty
  while (getline(fInputFileGeometry, datastring))
    {
      // Do not read empty lines
      if (datastring.empty()) continue;

      G4int detectorid;
      char name[255];
      G4int geometry;
      G4double innerradius;
      G4double outerradius;
      G4double height;
      G4double boredepth;
      G4double dlinnerthickness;
      G4double dlouterthickness;
      G4double dltopthickness;
      G4double dlbottomthickness;
      G4double passivationthickness;
      G4double grooveinnerradius;
      G4double grooveouterradius;
      G4double grooveheight;
      G4double groovedl;
      G4int    nphisegments;
      G4int    nzsegments;
      G4double angle;
      char bulkmaterial[255];
      char dlmaterial[255];
      char passivationmaterial[255];

      //
      // Read the data from geometry.dat
      //

      // Read a line to the string stream
      datastream.clear();
      datastream.str(datastring);

      // Now, read the data to corresponding variables
      datastream >> detectorid;
      datastream >> name;
      datastream >> geometry;
      datastream >> innerradius;
      datastream >> outerradius;
      datastream >> height;
      datastream >> boredepth;
      datastream >> dlinnerthickness;
      datastream >> dlouterthickness;
      datastream >> dltopthickness;
      datastream >> dlbottomthickness;
      datastream >> passivationthickness;
      datastream >> grooveinnerradius;
      datastream >> grooveouterradius;
      datastream >> grooveheight;
      datastream >> groovedl;
      datastream >> nphisegments;
      datastream >> nzsegments;
      datastream >> angle;
      datastream >> bulkmaterial;
      datastream >> dlmaterial;
      datastream >> passivationmaterial;

      for (G4int i = 0; i < nphisegments * nzsegments + 1; i++)
        {
          G4double mmm, bbb;

          datastream >> bbb;
          datastream >> mmm;
        }

      // set parameters

      GEGeometryDetectorParameters* detector = new GEGeometryDetectorParameters();
      detector -> SetDetectorID(detectorid);
      detector -> SetName(G4String(name));
      detector -> SetGeometry(geometry);
      detector -> SetInnerRadius(innerradius * mm);
      detector -> SetOuterRadius(outerradius * mm);
      detector -> SetHeight(height * mm);
      detector -> SetBoreDepth(boredepth * mm);
      detector -> SetDLInnerThickness(dlinnerthickness * nanometer);
      detector -> SetDLOuterThickness(dlouterthickness * nanometer);
      detector -> SetDLTopThickness(dltopthickness * nanometer);
      detector -> SetDLBottomThickness(dlbottomthickness * nanometer);
      detector -> SetPassivationThickness(passivationthickness * nanometer);
      detector -> SetGrooveInnerRadius(grooveinnerradius * mm);
      detector -> SetGrooveOuterRadius(grooveouterradius * mm);
      detector -> SetGrooveHeight(grooveheight * mm);
      detector -> SetGrooveDL(groovedl * nanometer);
      detector -> SetNPhiSegments(nphisegments);
      detector -> SetNZSegments(nzsegments);
      detector -> SetSegmentationAngle(angle);
      detector -> SetBulkMaterial(G4String(bulkmaterial));
      detector -> SetDLMaterial(G4String(dlmaterial));
      detector -> SetPassivationMaterial(G4String(passivationmaterial));

      if(geometry==2)
        {
          // somehow, this part should be moved into GEGeometryDetectorParameters ...
          if(fPhase2StandardDetectorParameters->GetInnerRadius         ()!=detector->GetInnerRadius         () ||
             fPhase2StandardDetectorParameters->GetOuterRadius         ()!=detector->GetOuterRadius         () ||
             fPhase2StandardDetectorParameters->GetHeight              ()!=detector->GetHeight              () ||
             fPhase2StandardDetectorParameters->GetBoreDepth           ()!=detector->GetBoreDepth           () ||
             fPhase2StandardDetectorParameters->GetDLInnerThickness    ()!=detector->GetDLInnerThickness    () ||
             fPhase2StandardDetectorParameters->GetDLOuterThickness    ()!=detector->GetDLOuterThickness    () ||
             fPhase2StandardDetectorParameters->GetDLTopThickness      ()!=detector->GetDLTopThickness      () ||
             fPhase2StandardDetectorParameters->GetDLBottomThickness   ()!=detector->GetDLBottomThickness   () ||
             fPhase2StandardDetectorParameters->GetPassivationThickness()!=detector->GetPassivationThickness() ||
             fPhase2StandardDetectorParameters->GetNPhiSegments        ()!=detector->GetNPhiSegments        () ||
             fPhase2StandardDetectorParameters->GetNZSegments          ()!=detector->GetNZSegments          () ||
             fPhase2StandardDetectorParameters->GetSegmentationAngle   ()!=detector->GetSegmentationAngle   () ||
             fPhase2StandardDetectorParameters->GetBulkMaterial        ()!=detector->GetBulkMaterial        () ||
             fPhase2StandardDetectorParameters->GetDLMaterial          ()!=detector->GetDLMaterial          () ||
             fPhase2StandardDetectorParameters->GetPassivationMaterial ()!=detector->GetPassivationMaterial ()
              )
            {
              MGLog(error)  << "The geometry of the current phase-2 detector differs from the standard phase-2 detector." << endlog;
              MGLog(error)  << "But the standard phase-2 detector is used "<<endlog;
              MGLog(error)  << "    - to build standard phase-2 holders, " << endlog;
              MGLog(error)  << "    - to define array positions, and so on." << endlog;
              MGLog(error)  << "If you want to use phase-2 detectors with different geometries you should first implenment" << endlog;
              MGLog(error)  << "    - holders individual for each phase-2 detector," << endlog;
              MGLog(error)  << "    - array positions depending on the individual phase-2 detectors, and so on." << endlog;
              abort();
            }
        }


      if( ((unsigned int)detectorid)!=fDetectorParametersContainer.size())
        {
          MGLog(error)  << "The \"DetectorIdNumber+1\" in the file \"geometry.dat\" " << endlog;
          MGLog(error)  << "and the line number in the file \"geometry.dat\" must be identical." << endlog;
          MGLog(error)  << "Otherwise the function GEGeometryDetectorDB::GetDetectorParameters(n) returns " << endlog;
          MGLog(error)  << "the parameter-set of the n-th entry in fDetectorParametersContainer instead of the " << endlog;
          MGLog(error)  << "the parameter-set of Crystal-Type n." << endlog;
          MGLog(error)  << "====>>>> Order the detectors in the file geometry.dat!" << endlog;
          abort();
        }

      // add detector to the list of detectors
      fDetectorParametersContainer.push_back(detector);
    }

  // close file
  fInputFileGeometry.close();

  // ---------------------------------------------------------------
  // read in detector geometries
  // ---------------------------------------------------------------

  //counter to check if matrix file has one or two columns
  fInputFileMatrix.open(GetCrystalMatrixFilename());

  G4int l = 0;
  TString check_columns;
  while(1){
  fInputFileMatrix >> check_columns;
  if(!fInputFileMatrix.good())break;
  //MGLog(error) << l << " " << check_columns << endlog;
  l=l+1;

  }
  fInputFileMatrix.close();

  if (fFlagCrystalMatrixWithZPosFile && l!=2*nrOfStrings*nrOfRows)//two columns expected
    {
    	MGLog(error)  << "The format of the matrix file is not correct. " << endlog;
	MGLog(error)  << "Two columns with "<<nrOfRows<<" blocks a "<<nrOfStrings<<" entries are expected. "<< endlog;
	MGLog(error)  << "The flag for an individual position of the crystals is set. "<< endlog;
  	abort();
    }

  if (!fFlagCrystalMatrixWithZPosFile && l!=nrOfStrings*nrOfRows)//one column expected
    {
    	MGLog(error)  << "The format of the matrix file is not correct. " << endlog;
	MGLog(error)  << "One column with "<<nrOfRows<<" blocks a "<<nrOfStrings<<" entries is expected. "<< endlog;
	MGLog(error)  << "The flag for an individual position of the crystals is NOT set. "<< endlog;
  	abort();
    }


  // read in geometries
  // Use TString as reading value, to allow for the accommodation of BEGe pair on the same line.
  // BEGe pairs are written as "(x,y)"
  //

  fInputFileMatrix.open(GetCrystalMatrixFilename());

  TString readval;
  TString indzposcrystal;
  for (G4int j = 0; j < nrOfRows; j++)
    {
      for (G4int i = 0; i < nrOfStrings; i++)
        {
          G4int type = -1;
	  G4double indvidpos = -1;
	  if (fFlagCrystalMatrixWithZPosFile)
	  	{fInputFileMatrix >> readval >> indzposcrystal;
		//MGLog(debugging) << "Readin matrix file " <<
			//readval <<" "<< indzposcrystal << " " <<i <<endlog;

		}
          else {fInputFileMatrix >> readval;}

	  //Check if we are reading a pair
	  if (readval.BeginsWith("("))
	    {
	      if (readval.EndsWith(")")) //this is required
		{
		  if (readval.CountChar(',') != 1)
		    {
		      MGLog(error) << "Format error while reading matrix file " <<
			readval << endlog;
		      MGLog(error) << "No commas (or too many commas) found " << endlog;
		      exit(1);
		    }
		  //Strip parentheses
//		  readval.Remove(TString::EStripType::kBoth,'(');
//		  readval.Remove(TString::EStripType::kBoth,')');
		  readval.Remove(readval.First('('),1);
		  readval.Remove(readval.First(')'),1);

		  //Look for separator (comma)
		  G4int position = readval.First(',');
		  //Get the two substrings
		  TString det1 = readval(0,position);
		  TString det2 = readval(position+1,readval.Length()-1);
		  MGLog(trace) << "Found detector pair at position (" << i << "," <<
		    j << ")" << endlog;
		  MGLog(trace) << "Detector IDs: " << det1 << " and " << det2 << endlog;

		  fArrayDetectorTypeMatrix[i][j][0] = det1.Atoi();
		  fArrayDetectorTypeMatrix[i][j][1] = det2.Atoi();

		  //Check that detectors exist and are of BEGe type
		  for (int imul=0;imul<nrOfMultiplets;imul++)
		    {
		      GEGeometryDetectorParameters* detPar =
			GetDetectorParameters(fArrayDetectorTypeMatrix[i][j][imul]);
		      if (!detPar)
			{
			  MGLog(error) << "Error while building detector pair, id " <<
			    fArrayDetectorTypeMatrix[i][j][imul] << endlog;
			  exit(1);
			}
		      if (detPar->GetGeometry() < 4 || detPar->GetGeometry() > 5)
			{
			  MGLog(error) << "Error while building detector pair. Detector id " <<
			    fArrayDetectorTypeMatrix[i][j][imul] << "is not a BeGe" << endlog;
			  MGLog(error) << "Type: " << detPar->GetGeometry() << endlog;
			  exit(1);
			}
		    }
		}
	      else{
		MGLog(error) << "Format error while reading matrix file " << readval << endlog;
		exit(1);
	      }
	    }
	  else //single detector
	    {
	      type = readval.Atoi();
	      fArrayDetectorTypeMatrix[i][j][0] = type;
	      fArrayDetectorTypeMatrix[i][j][1] = -1;
	    }
	   if (fFlagCrystalMatrixWithZPosFile){
	  indvidpos = indzposcrystal.Atof();
          fArrayDetectorIndZPositionMatrix[i][j] = indvidpos;}
        }

      // check that the type in row n is equal to the type in row n-1
      // --> one string is only allowed to contain only type-1-holders or only-type-2-holders
      if( j!=0 ) for (G4int i = 0; i < nrOfStrings; i++)
        {
          if( fArrayDetectorTypeMatrix[i][j][0] >=0  &&  fArrayDetectorTypeMatrix[i][j-1][0]<0 )
            {
              MGLog(error) << " "  << endlog;
              MGLog(error) << "    Position (col="<<i<<", row="<<j<<") is not allowed to contain detectors because there is"  << endlog;
              MGLog(error) << "    no detector at at the position above it (col="<<i<<", row="<<j-1<<")."  << endlog;
              abort();
            }

          if( fArrayDetectorTypeMatrix[i][j  ][0] >=0  &&
              fArrayDetectorTypeMatrix[i][j-1][0] >=0      )
            {

              GEGeometryDetectorParameters* detector_Col_N       ( 0 );
              GEGeometryDetectorParameters* detector_Col_Nminus1 ( 0 );
              for(unsigned int ii=0;ii<fDetectorParametersContainer.size();ii++)
                {
                  GEGeometryDetectorParameters* detector = fDetectorParametersContainer[ii];
                  assert(detector);
                  if(detector->GetDetectorID()==fArrayDetectorTypeMatrix[i][j  ][0]) detector_Col_N       = detector;
                  if(detector->GetDetectorID()==fArrayDetectorTypeMatrix[i][j-1][0]) detector_Col_Nminus1 = detector;
                }

              if( detector_Col_N==0 )
                {
                  MGLog(error) << "     Detector type "<<fArrayDetectorTypeMatrix[i][j][0]<<" does not exist"<<endlog;
                  abort();
                }
              if( detector_Col_Nminus1==0 )
                {
                  MGLog(error) << "     Detector type "<<fArrayDetectorTypeMatrix[i][j-1][0]<<" does not exist"<<endlog;
                  abort();
                }
              if( detector_Col_N!=0 && detector_Col_Nminus1!=0 )
                {
  		 if(  detector_Col_N->GetGeometry()!=detector_Col_Nminus1->GetGeometry() ){
		    //for phase 2 (status December 2015, type 5 and 11) BEGes and Coax can be in the same string
		    if(detector_Col_N->GetGeometry()==5  && detector_Col_Nminus1->GetGeometry()==11) continue;
		    else if(detector_Col_N->GetGeometry()==11  && detector_Col_Nminus1->GetGeometry()==5) continue;
		    else {
                      	MGLog(error) << "  " << endlog;
                      	MGLog(error) << "     Check the matrix file. There are crystals with different Geometry in one string. " << endlog;
                      	MGLog(error) << "     But One String should contain only \"NON-TRUE COAXIAL\" or only \"TRUE-COAXIAL\" detectors." << endlog;
                      	abort();
                    	}
		    }
                }
            }
       }
    if(fFlagCrystalMatrixWithZPosFile) for (G4int i = 0; i < nrOfStrings; i++)
       {
       	 if (fArrayDetectorIndZPositionMatrix[i][j]>=0 && fArrayDetectorTypeMatrix[i][j][0] <0){
	 	  MGLog(error) << "	There is an individual z position without detector at Position (col="<<i<<", row="<<j<<")"<<endlog;
                  abort();
		  }
	 if (fArrayDetectorIndZPositionMatrix[i][j]<0 && fArrayDetectorTypeMatrix[i][j][0] >=0){
		  MGLog(error) << "	There is a detector at Position (col="<<i<<", row="<<j<<") without individual z position assigned. "<<endlog;
                  abort();
		  }
    }
// close file
}
  fInputFileMatrix.close();

}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::SetCrystalGeometryFilename(G4String filename)
{

  char* path = getenv("MGGERDAGEOMETRY");

  G4String pathname;
  if (!path)
    {
      MGLog(warning)<< " MGGERDAGEOMETRY environment variable not set!" << endlog;
      pathname = "./gerdageometry";
      MGLog(warning)<< " Path: " << path << endlog;
    }
  else
      pathname = path;

  G4String filenamepluspath = pathname + "/" + filename;

  std::ifstream file;

  file.open(filenamepluspath);

  if (file.is_open() == false)
    {
      MGLog(error) << " File " << filenamepluspath << " does not exist. " << endlog;
      return;
    }

  // close file

  file.close();

  // set filename with path

  fCrystalGeometryFilename = filenamepluspath;

  return;

}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::SetCrystalMatrixFilename(G4String filename)
{

  char* path = getenv("MGGERDAGEOMETRY");

  G4String pathname;
  if (!path)
    {
      MGLog(warning)<< " MGGERDAGEOMETRY environment variable not set!" << endlog;
      pathname = "./gerdageometry";
      MGLog(warning)<< " Path: " << path << endlog;
    }
  else
    pathname = path;

  G4String filenamepluspath = pathname + "/" + filename;

  std::ifstream file;

  file.open(filenamepluspath);

  if (file.is_open() == false)
    {
      MGLog(error) << " File " << filenamepluspath << " does not exist. " << endlog;
      return;
    }

  // close file

  file.close();

  // set filename with path

  fCrystalMatrixFilename = filenamepluspath;

  return;

}

// ---------------------------------------------------------------------------


void GEGeometryDetectorDB::SetCryostatGeometry()
{
  GEGeometryCryostatParts* pCP = fCryostatParts;

  pCP->Distance_TopLidOuterWall_TopLidInnerWall = 10 * cm;
  pCP->CryoSlopeLid = 0.24; //  dz/drho = 440/1860 ~= 0.24
  pCP->CryoSlopeLid_AsAngle = std::atan(pCP->CryoSlopeLid);

  G4double  CryoSlopeLid         =  pCP->CryoSlopeLid;
  G4double  CryoSlopeLid_AsAngle =  pCP->CryoSlopeLid_AsAngle;

  pCP->ThicknessInnerWallTopLid          = 12 *mm;
  pCP->ThicknessInnerWallBotLid          = 12 *mm;
  pCP->ThicknessInnerWallNeck            = 12 *mm;
  pCP->ThicknessInnerWallCyl             = 12 *mm;
  pCP->ThicknessInnerWallLittlecylBarrel = 12 *mm;
  pCP->ThicknessInnerWallLittlecylBottom = 20 *mm;

  pCP->ThicknessOuterWallTopLid          = 12 *mm;
  pCP->ThicknessOuterWallBotLid          = 20 *mm;
  pCP->ThicknessOuterWallNeck            = 18 *mm;
  pCP->ThicknessOuterWallCyl             = 20 *mm;
  pCP->ThicknessOuterWallLittlecylBarrel = 12 *mm;
  pCP->ThicknessOuterWallLittlecylBottom = 20 *mm;

  pCP->ThicknessInnerWallLayer1 = 6*mm;  // makrolon layer
  pCP->ThicknessInnerWallLayer2 = (      // superinsulation layer
      6.0e-6 +       // 6 micro meter Polyester
      2*0.04e-6      // with 400A Aluminum on both sides
      )*m
      * 2 * 15 ;// 2 times 15 times


  G4double zthickness;

  G4double jsDebug_shortenCyl;
  jsDebug_shortenCyl = -500*mm;
  jsDebug_shortenCyl = 0;

  // Define:
  // Cryo OuterWall Outside
  GEGeometryCryostatMassiveShape* OuterWall_Outside(pCP->GetCryoOuterwall_Outside());
  OuterWall_Outside->slopetoplid   = CryoSlopeLid;
  OuterWall_Outside->slopebotlid   = CryoSlopeLid;
  OuterWall_Outside->hneck         =
      (1494. + 207.3) *mm
      +100*mm; // At the upper edge, the inner neck ends ~100mm above the outer neck.
               // However, in the simulation let the outer neck end at the same height
  OuterWall_Outside->htopcylbot    = 5881.2 *mm - jsDebug_shortenCyl;
  OuterWall_Outside->hlittlecyl    =  221.0 * mm;  // 219 + 2 + 20 - 20 = 221
  OuterWall_Outside->rneck         =  0.5*1000.0 *mm;
  OuterWall_Outside->rcyl          =      2100.0 *mm;
  OuterWall_Outside->rlittlecyl    =  0.5* 500.0 *mm;


  // Define:
  // Cryo OuterWall Inside
  GEGeometryCryostatMassiveShape* OuterWall_Inside(pCP->GetCryoOuterwall_Inside());
  OuterWall_Inside->slopetoplid   = CryoSlopeLid;
  OuterWall_Inside->slopebotlid   = CryoSlopeLid;
  zthickness = pCP->ThicknessOuterWallTopLid / cos(CryoSlopeLid_AsAngle);
  OuterWall_Inside->hneck         =
      OuterWall_Outside->hneck
      + zthickness
      - CryoSlopeLid * pCP->ThicknessOuterWallNeck;
  zthickness = pCP->ThicknessOuterWallBotLid / cos(CryoSlopeLid_AsAngle);
  OuterWall_Inside->htopcylbot    =
      OuterWall_Outside->htopcylbot
      - (OuterWall_Inside->hneck-OuterWall_Outside->hneck)
      - zthickness
      + CryoSlopeLid * pCP->ThicknessOuterWallLittlecylBarrel;
  OuterWall_Inside->hlittlecyl    =
      OuterWall_Outside->hlittlecyl
      + OuterWall_Outside->htopcylbot
      - (OuterWall_Inside->hneck-OuterWall_Outside->hneck)
      - OuterWall_Inside->htopcylbot
      - pCP->ThicknessOuterWallLittlecylBottom;
  OuterWall_Inside->rneck         =  OuterWall_Outside->rneck      - pCP->ThicknessOuterWallNeck;
  OuterWall_Inside->rcyl          =  OuterWall_Outside->rcyl       - pCP->ThicknessOuterWallCyl;
  OuterWall_Inside->rlittlecyl    =  OuterWall_Outside->rlittlecyl - pCP->ThicknessOuterWallLittlecylBarrel;


  // Define:
  // Cryo InnerWall Outside
  GEGeometryCryostatMassiveShape* InnerWall_Outside(pCP->GetCryoInnerwall_Outside());;
  InnerWall_Outside->slopetoplid   = CryoSlopeLid;
  InnerWall_Outside->slopebotlid   = CryoSlopeLid;
  InnerWall_Outside->rneck         =  0.5*813.0 *mm;
  InnerWall_Outside->hneck         =
      OuterWall_Outside->hneck +
      cos(CryoSlopeLid_AsAngle)*(
          pCP->Distance_TopLidOuterWall_TopLidInnerWall
          -(OuterWall_Outside->rneck-InnerWall_Outside->rneck)*sin(CryoSlopeLid_AsAngle));
  InnerWall_Outside->htopcylbot    = 5555.0 *mm - jsDebug_shortenCyl;
  InnerWall_Outside->hlittlecyl    =  373.0 * mm;  // 366 + 2 + 20 - 15 = 373
  InnerWall_Outside->rcyl          =  2000.0 *mm;
  InnerWall_Outside->rlittlecyl    =  0.5* 324.0 *mm;


  // Define:
  // Cryo InnerWall Inside
  GEGeometryCryostatMassiveShape* InnerWall_Inside(pCP->GetCryoInnerwall_Inside());
  InnerWall_Inside->slopetoplid   = CryoSlopeLid;
  InnerWall_Inside->slopebotlid   = CryoSlopeLid;
  zthickness = pCP->ThicknessInnerWallTopLid / cos(CryoSlopeLid_AsAngle);
  InnerWall_Inside->hneck         =
      InnerWall_Outside->hneck
      + zthickness
      - CryoSlopeLid * pCP->ThicknessInnerWallNeck;
  zthickness = pCP->ThicknessInnerWallBotLid / cos(CryoSlopeLid_AsAngle);
  InnerWall_Inside->htopcylbot    =
      InnerWall_Outside->htopcylbot
      - (InnerWall_Inside->hneck-InnerWall_Outside->hneck)
      - zthickness
      + CryoSlopeLid * pCP->ThicknessInnerWallLittlecylBarrel;
  InnerWall_Inside->hlittlecyl    =
      InnerWall_Outside->hlittlecyl
      + InnerWall_Outside->htopcylbot
      - (InnerWall_Inside->hneck-InnerWall_Outside->hneck)
      - InnerWall_Inside->htopcylbot
      - pCP->ThicknessInnerWallLittlecylBottom;
  InnerWall_Inside->rneck         =  InnerWall_Outside->rneck      - pCP->ThicknessInnerWallNeck;
  InnerWall_Inside->rcyl          =  InnerWall_Outside->rcyl       - pCP->ThicknessInnerWallCyl;
  InnerWall_Inside->rlittlecyl    =  InnerWall_Outside->rlittlecyl - pCP->ThicknessInnerWallLittlecylBarrel;

  // Define:
  // Cryo InnerWallLayer1 Outside
  GEGeometryCryostatMassiveShape* InnerWallLayer1_Outside(pCP->GetCryoInnerwallLayer1_Outside());
  InnerWallLayer1_Outside->slopetoplid   = CryoSlopeLid;
  InnerWallLayer1_Outside->slopebotlid   = CryoSlopeLid;
  InnerWallLayer1_Outside->hneck         = InnerWall_Outside->hneck;
  zthickness = pCP->ThicknessInnerWallLayer1 / cos(CryoSlopeLid_AsAngle);
  InnerWallLayer1_Outside->htopcylbot    =
      InnerWall_Outside->htopcylbot
      + zthickness
      - CryoSlopeLid * pCP->ThicknessInnerWallLayer1;
  InnerWallLayer1_Outside->hlittlecyl    =
      InnerWall_Outside->hlittlecyl
      + InnerWall_Outside->htopcylbot
      - InnerWallLayer1_Outside->htopcylbot
      + pCP->ThicknessInnerWallLayer1;
  InnerWallLayer1_Outside->rneck         =  InnerWall_Outside->rneck;
  InnerWallLayer1_Outside->rcyl          =  InnerWall_Outside->rcyl       + pCP->ThicknessInnerWallLayer1;
  InnerWallLayer1_Outside->rlittlecyl    =  InnerWall_Outside->rlittlecyl + pCP->ThicknessInnerWallLayer1;


  // Define:
  // Cryo InnerWallLayer2 Outside
  GEGeometryCryostatMassiveShape* InnerWallLayer2_Outside(pCP->GetCryoInnerwallLayer2_Outside());
  InnerWallLayer2_Outside->slopetoplid   = CryoSlopeLid;
  InnerWallLayer2_Outside->slopebotlid   = CryoSlopeLid;
  zthickness = pCP->ThicknessInnerWallLayer2 / cos(CryoSlopeLid_AsAngle);
  InnerWallLayer2_Outside->hneck         =
      InnerWallLayer1_Outside->hneck
      - zthickness
      + CryoSlopeLid * pCP->ThicknessInnerWallLayer2;
  zthickness = pCP->ThicknessInnerWallLayer2 / cos(CryoSlopeLid_AsAngle);
  InnerWallLayer2_Outside->htopcylbot    =
      InnerWallLayer1_Outside->htopcylbot
      + (InnerWallLayer1_Outside->hneck-InnerWallLayer2_Outside->hneck)
      + zthickness
      - CryoSlopeLid * pCP->ThicknessInnerWallLayer2;
  InnerWallLayer2_Outside->hlittlecyl    =
      InnerWallLayer1_Outside->hlittlecyl
      + InnerWallLayer1_Outside->htopcylbot
      + (InnerWallLayer1_Outside->hneck-InnerWallLayer2_Outside->hneck)
      - InnerWallLayer2_Outside->htopcylbot
      + pCP->ThicknessInnerWallLayer2;
  InnerWallLayer2_Outside->rneck         =  InnerWallLayer1_Outside->rneck      + pCP->ThicknessInnerWallLayer2;
  InnerWallLayer2_Outside->rcyl          =  InnerWallLayer1_Outside->rcyl       + pCP->ThicknessInnerWallLayer2;
  InnerWallLayer2_Outside->rlittlecyl    =  InnerWallLayer1_Outside->rlittlecyl + pCP->ThicknessInnerWallLayer2;



  // Define
  // Cu Layer and its support ring
  pCP->CuLayerSupportRing_Thickness              = 25 * mm;
  pCP->CuLayerSupportRing_InnerRadius            = 0.5 * 3676 * mm;
  pCP->CuLayerSupportRing_OuterRadius            = InnerWall_Inside->rcyl; // 0.5 * 3974 * mm;
  pCP->CuLayerSupportRing_zPosRelToUpperEdgeCryo =
      - InnerWall_Outside->htopcylbot
      - InnerWall_Outside->hneck
      + 829*mm + 950*mm
      - 0.5 * pCP->CuLayerSupportRing_Thickness ;


  // Define
  // Cu Layer
  pCP->CuLayer_NPlatesInPhi                = 20;
  pCP->CuLayer_PlateWidth                  = 615*mm;
  pCP->CuLayer_PlateThickness              = 30*mm;
  pCP->CuLayerTop_Height1                  = 2400*mm;
  pCP->CuLayerTop_Height2                  = 2000*mm;
  pCP->CuLayerBot_Height                   = 400*mm;


  // Define
  // Cryo liquid height in neck
  G4double cryoliquid_height_in_neck = (8100.0 - (5881.2 + 1279.5))*mm;
  pCP->CryogasHeightInNeck = OuterWall_Outside->hneck - cryoliquid_height_in_neck;




  // Torlon rods
  pCP->TorlonBigRod_RadiusOfPositionInTank  = 0.5*1600*mm;
  pCP->TorlonBigRod_InnerRadius             = 0.5*100*mm;
  pCP->TorlonBigRod_OuterRadius             = 0.5*183*mm;
  pCP->TorlonBigRod_Height                  = 100*mm;

  pCP->TorlonCenteringRodTop_ZPositionRelToInnerNeckbottom = 150*mm;
  pCP->TorlonCenteringRodTop_InnerRadius    = 0.5*44*mm;
  pCP->TorlonCenteringRodTop_OuterRadius    = 0.5*95*mm;
  pCP->TorlonCenteringRodTop_Height         = (61+0)*mm;
  pCP->TorlonCenteringRodTop_RadiusOfPositionInTank        =
      InnerWall_Outside->rneck
      + 0.5*pCP->TorlonCenteringRodTop_Height
      + 2*mm;

  pCP->TorlonCenteringRodBot_ZPositionRelToLittlecylbottom = 90*mm;
  pCP->TorlonCenteringRodBot_InnerRadius    = 0.5*44*mm;
  pCP->TorlonCenteringRodBot_OuterRadius    = 0.5*95*mm;
  pCP->TorlonCenteringRodBot_Height         = (61+0)*mm;
  pCP->TorlonCenteringRodBot_RadiusOfPositionInTank        =
      InnerWall_Outside->rlittlecyl
      + 0.5*pCP->TorlonCenteringRodBot_Height
      + pCP->ThicknessInnerWallLayer1
      + 2*mm;


  // Thermal Shield
  pCP->ThermalShieldInNeck_InnerRadius =    0.5*550*mm;
  pCP->ThermalShieldInNeck_Thickness   =    3*cm;
  pCP->ThermalShieldInNeck_DistanceToUpperEdgeOfNeck = 700*mm;



  // Heat Exchanger
  pCP->HeatExchanger_Height = 800*mm;
  pCP->HeatExchanger_Radius = 350*mm;
  pCP->HeatExchanger_Thickness = 1*mm;
  pCP->HeatExchanger_InnerRadius =  pCP->HeatExchanger_Radius - pCP->HeatExchanger_Thickness;
  pCP->HeatExchanger_DistanceToLArFillLevel = 1100*mm;

  // Shroud
  pCP->Shroud_Height = 3. * m;
  pCP->Shroud_Radius = 380. * mm;
  pCP->Shroud_Thickness = 0.03 * mm;

  // till here the zero level is the top edge of the neck
  // but now shift the zero level towards the middle of the water
  GEGeometryCryostatMassiveShape::GeneralPolyconeZShift =
      fShieldingWaterTankThickness + fShieldingAirBufferHeight + 0.5*fShieldingWaterHeight;

}

// ----------------------------------------------------------------------

G4Polycone* GEGeometryDetectorDB::Create_ReflectionFoilCryoFootTop()
{
  G4double dz1          = fReflectionFoilThickness/cos(fCryostatParts->CryoSlopeLid_AsAngle);
  G4double radiusOuter  = fShieldingFootIR - (fShieldingFootThickness+fReflectionFoilThickness);
  G4double radiusInner  = fCryostatParts->GetCryoOuterwall_Outside()->GetRlittlecyl();
  G4double dz2          = (radiusOuter-radiusInner)*fCryostatParts->CryoSlopeLid;
  G4double tmpz[4]      = {0,-dz1,-dz2,-dz1-dz2};
  G4double tmpRi[4]     = {radiusOuter,
                           radiusOuter-dz1/(fCryostatParts->CryoSlopeLid),
                           radiusInner,
                           radiusInner};
  G4double tmpRo[4]     = {radiusOuter,
                           radiusOuter,
                           radiusInner+dz1/(fCryostatParts->CryoSlopeLid),
                           radiusInner};


  G4Polycone* fReflectionFoilCryoFootTop_Solid = new G4Polycone("fReflectionFoilCryoFootTop",
                                                          0.0*deg,
                                                          360.0*deg,
                                                          4,
                                                                tmpz,tmpRi,tmpRo);
  return fReflectionFoilCryoFootTop_Solid;
}

// ---------------------------------------------------------------------------

GEGeometryHolderPhase1Parts* GEGeometryDetectorDB::GetHolderPhase1Parts(G4int detectorID)
{
  return fHolderPhase1Container[detectorID];
}


GEGeometryHolderPhase2Parts* GEGeometryDetectorDB::GetHolderPhase2Parts(G4int detectorID)
{
  return fHolderPhase2Container[detectorID];
}

// ---------------------------------------------------------------------------

G4LogicalVolume* GEGeometryDetectorDB::GetElectronicsSideBarLogical(G4int col)
{
  GEGeometryStringStuff* ss = fStringStuffContainer[col];
  if(ss==0) return 0;
  return ss->GetElectronicsSideBarLogical();
}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::SetZShiftScheme(GEGeometryCrystalZShiftScheme_t zShiftScheme)
{
  fZShiftScheme = zShiftScheme;
}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::SetLockType(GEGeometryLockType_t LockType)
{
  fLockType = LockType;
}

// ---------------------------------------------------------------------------

GEGeometryDetectorParameters* GEGeometryDetectorDB::CreatePhase2StandardDetectorParameters()
{
  GEGeometryDetectorParameters* detector = new GEGeometryDetectorParameters();
  detector -> SetDetectorID( -10 );
  detector -> SetName( G4String("Phase2StandardDetector") );
  detector -> SetGeometry(2);

  detector -> SetInnerRadius(5* mm);
  detector -> SetOuterRadius(37.5 * mm);
  detector -> SetHeight(70 * mm);
  detector -> SetBoreDepth(0 * mm);

  detector -> SetDLInnerThickness (1.0e3 * nanometer);
  detector -> SetDLOuterThickness (1.0e3 * nanometer);
  detector -> SetDLTopThickness   (1.0e3 * nanometer);
  detector -> SetDLBottomThickness(1.0e3 * nanometer);

  detector -> SetPassivationThickness(1.0e3 * nanometer);

  detector -> SetNPhiSegments(6);
  detector -> SetNZSegments(3);
  detector -> SetSegmentationAngle(0);
  detector -> SetBulkMaterial       ( G4String("EnrichedGe") );
  detector -> SetDLMaterial         ( G4String("EnrichedGe") );
  detector -> SetPassivationMaterial( G4String("EnrichedGe") );

  return detector;
}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDB::Create_HVCables_Phase2()
{

  MGLog(debugging) << "START GEGeometryDetectorDB::CreatePhase2StandardDetectorParameters" << endlog;

  // find a phase-2 String and get the electronics-sidebar info from its string-stuff object
  G4double ElSideBar_Height(-DBL_MAX);
  G4double ElSideBar_ZPos(-DBL_MAX);
  for(int iCol=0;iCol<nrOfStrings;iCol++)
    {
      //std::cout << "iCol="<<iCol<<std::endl;
      G4int iCrtlType = GetCrystalType(iCol,0 /* iRow */ );
      if(iCrtlType<0) continue;
      GEGeometryDetectorParameters* detPar = GetDetectorParameters( iCrtlType );

      //std::cout << "iCol="<<iCol<<"  detPar="<<detPar<<std::endl;
      if( detPar->GetGeometry() == 2 )
        {
          GEGeometryStringStuff* ss = fStringStuffContainer[iCol];
          //std::cout << "iCol="<<iCol<<"  ss="<<ss<<std::endl;
          ElSideBar_Height = ss -> GetElectronicsSideBarHeight();
          ElSideBar_ZPos   = ss -> GetElectronicsSideBarZPosition();
          break;
        }
    }
  if(ElSideBar_Height<0)
    {
      // there are no phase-2 strings in the array
      return;
    }

  G4double TubsInnerRadius;
  G4double TubsOuterRadius;
  G4double TubsHeight;
  G4double TubsSpanningAngle;
  G4double TubsStartAngle;
  // +-------------------------+
  // | vertical HV cable       |
  // +-------------------------+
  G4double centerHolder_to_LowerHVCableEnd =
      0.5*fPhase2StandardDetectorParameters->GetHeight()
      + fHolder_DistanceToCrystalBot
      - 2*mm;
  G4double topHolder_to_LowerHVCableEnd = 0.5*fDistanceHolderToHolder + centerHolder_to_LowerHVCableEnd;


  // HV cable beside crystal
  TubsInnerRadius   = fHolderSideBarRadius;
  TubsOuterRadius   = fHolderSideBarRadius + fHVCableThickness;
  //TubsHeight        = 0.5*( ElSideBar_Height + topHolder_to_LowerHVCableEnd  );
  TubsSpanningAngle =   fHVCableWidth/TubsOuterRadius;
  TubsStartAngle    = - 0.5 * TubsSpanningAngle;

  char volumename[100];
  G4Tubs* HVCableVertical[nrOfRows];

  for(int iRow=0;iRow<nrOfRows;iRow++)
    {
      // the solid
      G4double  dHeight = topHolder_to_LowerHVCableEnd +iRow*fDistanceHolderToHolder;
      TubsHeight        = 0.5*( ElSideBar_Height + dHeight );
      sprintf(volumename, "HVCable_phase2_vertical_row%d",iRow);
      HVCableVertical[iRow] = new G4Tubs(volumename,
                  TubsInnerRadius,
                  TubsOuterRadius,
                  TubsHeight,
                  TubsStartAngle,
                  TubsSpanningAngle);
      // the logical volume
      sprintf(volumename, "HVCable_phase2_vertical_Logical_row%d",iRow);
      f_HVCablePhase2VerticalLogical[iRow] =  new G4LogicalVolume(HVCableVertical[iRow],
                  kapton,
                  volumename);
        f_HVCablePhase2VerticalLogical[iRow] -> SetVisAttributes(CuVisAtt);

        f_HVCablePhase2Vertical_ZPos[iRow] = ElSideBar_ZPos - 0.5*dHeight;
    }

  //MGLog(debugging) << "3 GEGeometryDetectorDB::CreatePhase2StandardDetectorParameters" << endlog;

  // +-------------------------+
  // | horizontal HV cable     |
  // +-------------------------+
  // HV cable below/above crystal
  G4double tmpRadiusTeflonPlug = 5.0 * mm;
  G4double BoxLengthX = 0.5 * (fHolderSideBarRadius- tmpRadiusTeflonPlug);
  G4double BoxLengthY = 0.5 * fHVCableWidth;
  G4double BoxLengthZ = 0.5 * fHVCableThickness;
  sprintf(volumename, "HVCable_phase2_horizontal");
  G4Box* HVCableHorizontal = new G4Box(volumename,
              BoxLengthX,
              BoxLengthY,
              BoxLengthZ);

  sprintf(volumename, "HVCable_phase2_horizontal");
  f_HVCablePhase2HorizontalLogical = new G4LogicalVolume(HVCableHorizontal,
              kapton,
              volumename);
  f_HVCablePhase2HorizontalLogical -> SetVisAttributes(CuVisAtt);

  f_HVCablePhase2Horizontal_relZPos      = - centerHolder_to_LowerHVCableEnd;
  f_HVCablePhase2Horizontal_RadiusOffset = tmpRadiusTeflonPlug + BoxLengthX;

  MGLog(debugging) << "END GEGeometryDetectorDB::CreatePhase2StandardDetectorParameters" << endlog;
}

// ---------------------------------------------------------------------------

G4double GEGeometryDetectorDB::GetHVCablePhase2_AnglePosition(G4int iRow)
{
  static G4double* HolderHVCable_AnglePositions(0);
  if( HolderHVCable_AnglePositions == 0 )
    {

      HolderHVCable_AnglePositions = new G4double[5];

      HolderHVCable_AnglePositions[4-1] = (-( 12 + 14                ) + 180 )* deg;
      HolderHVCable_AnglePositions[2-1] = (-(      14                ) + 180 )* deg;
      HolderHVCable_AnglePositions[1-1] = ( (           14           ) + 180 )* deg;
      HolderHVCable_AnglePositions[3-1] = ( (           14 + 12      ) + 180 )* deg;
      HolderHVCable_AnglePositions[5-1] = ( (           14 + 12 + 12 ) + 180 )* deg;
    }
  return HolderHVCable_AnglePositions[iRow];
}


// ---------------------------------------------------------------------------
// For the moment this method only works for phase 2
G4ThreeVector GEGeometryDetectorDB::GetStringDetectorCenter(G4int index) {
	if (!fNlocks) {
		MGLog(error) << "Trying to get position from inexistent detector cluster (" << index << ")." << endlog;
		abort();
	}
	return fStringDetectorCenter[index];

}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//cables for phase 2 (status December 2015)
//HV cables and signal cables from each detector to electronics plate
void GEGeometryDetectorDB::Create_DetectorCablesForPhase2()
{
 G4int counter=0;
 for (G4int iCol=0; iCol<nrOfStrings; iCol++)
	  for (G4int jRow=0; jRow < nrOfRows; ++jRow){
	    G4int iCrtlType = GetCrystalType(iCol,jRow,0);
	    fHVCableAtHolderPhase2Logical[iCol][jRow]=NULL;
	    fHVCableAtHolderTopPhase2Logical[iCol][jRow]=NULL;
	    fHVCableToEPlatePhase2Logical[iCol][jRow]=NULL;
	    fHVCableTopToEPlatePhase2Logical[iCol][jRow]=NULL;
	    fSignalCableAtHolderPhase2Logical[iCol][jRow]=NULL;
	    fSignalCableAtHolderTopPhase2Logical[iCol][jRow]=NULL;
	    fSignalCableToEPlatePhase2Logical[iCol][jRow]=NULL;
	    fSignalCableTopToEPlatePhase2Logical[iCol][jRow]=NULL;
	    //MGLog(trace) << iCrtlType << endlog;
     	    if(iCrtlType<0) continue; //if there is no crystal, continue with loop
      	    GEGeometryDetectorParameters* detPar = GetDetectorParameters( iCrtlType );
	    GEGeometryHolderPhase2Parts* holder = GetHolderPhase2Parts(iCrtlType);
	    G4double iCrtlZPos = GetCrystalPosition(iCol,jRow).getZ();
	    G4double iCrtlHeight=detPar->GetHeight();
	    if( detPar->GetGeometry() == 5 || detPar->GetGeometry() == 11){




	      // +---------------------------------------------------------------------+
  	      // | HV cable: pad on holder and cable from holder to electronics plate  |
  	      // +---------------------------------------------------------------------+
  		//pad on holder
   		G4double HVPhase2AtHolder_LengthX=0.5*fPhase2Cable_WidthOnHolder;
   		G4double HVPhase2AtHolder_LengthY=0.5*fPhase2Cable_LengthOnHolderHV;
   		G4double HVPhase2AtHolder_LengthZ=0.5*fPhase2Cable_Thickness;

   		char solidname[100];

   		sprintf(solidname, "HVCableAtHolder_Phase2_Solid_%d", counter);

   		G4Box* HVCableAtHolder = new G4Box(solidname,
  				      HVPhase2AtHolder_LengthX,
				      HVPhase2AtHolder_LengthY,
				      HVPhase2AtHolder_LengthZ);

   		char logicname[100];

   		sprintf(logicname, "HVCableAtHolder_Phase2_Logical_%d", counter);
   		G4LogicalVolume* fHVCabLogical =  new G4LogicalVolume(HVCableAtHolder, kapton, logicname);

   		fHVCabLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   		fHVCableAtHolderPhase2Logical[iCol][jRow]=fHVCabLogical;

   		//for top detector in pair
   		if(IsAPair(iCol,jRow)){
   			char solidnametop[100];

   			sprintf(solidnametop, "HVCableAtHolderTop_Phase2_Solid_%d", counter);
   			G4Box* HVCableAtHolderTop = new G4Box(solidnametop,
   					 	      	      HVPhase2AtHolder_LengthX,
					 	      	      HVPhase2AtHolder_LengthY,
					 	      	      HVPhase2AtHolder_LengthZ);

   			char logicnametop[100];

  			sprintf(logicnametop, "HVCableAtHolderTop_Phase2_Logical_%d", counter);
   			G4LogicalVolume* fHVCabTopLogical =  new G4LogicalVolume(HVCableAtHolderTop, kapton, logicnametop);

   			fHVCabTopLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   			fHVCableAtHolderTopPhase2Logical[iCol][jRow]=fHVCabTopLogical;
   			}

  		//cable from holder to electronics plate
  		//upper end at CC3, at the moment constructed from middle of array and implemented in GEGeometryLArInstHybrid.cc
   		//determine cable length
   		G4double up_Pos = GetZUppermostCrystalSurface();
   		G4double low_Pos = GetZLowermostCrystalSurface();
		G4double DistanceFromMiddleArrayToCC3=494*mm;//value calculated from placement of electronic plate in GEGeometryLArInstHybrid.cc
							     //change to function when electronics is moved
   		G4double UpperEndCable=0.5*(up_Pos+low_Pos)+DistanceFromMiddleArrayToCC3;//

   		if(IsAPair(iCol,jRow)){
   		G4int iCrtlTypePairLower = GetCrystalType(iCol,jRow,1);
   		GEGeometryDetectorParameters* detParLower = GetDetectorParameters( iCrtlTypePairLower );
   		G4double iCrtlHeightLower=detParLower->GetHeight();

   		fHVPhase2ToEPlate_LengthZ[iCol][jRow]=fabs(UpperEndCable-iCrtlZPos)+0.5*fCrystalPairGap+iCrtlHeightLower+holder->GetZDistanceToEndOfSiliconPlate()+ fPhase2Cable_Thickness;
   		fHVTopPhase2ToEPlate_LengthZ[iCol][jRow]=fabs(UpperEndCable-iCrtlZPos)-0.5*fCrystalPairGap-holder->GetZDistanceToEndOfSiliconPlatePair();
   		}
   		else fHVPhase2ToEPlate_LengthZ[iCol][jRow]=fabs(UpperEndCable-iCrtlZPos)+0.5*iCrtlHeight + holder->GetZDistanceToEndOfSiliconPlate() + fPhase2Cable_Thickness;

   		G4double fHVPhase2ToEPlate_Z = 0.5*fHVPhase2ToEPlate_LengthZ[iCol][jRow];
   		G4double fHVTopPhase2ToEPlate_Z = 0.5*fHVTopPhase2ToEPlate_LengthZ[iCol][jRow];
   		G4double HVPhase2ToEPlate_LengthX=0.5*fPhase2Cable_WidthFromHolderToElectronicsPlate;
   		G4double HVPhase2ToEPlate_LengthY=0.5*fPhase2Cable_Thickness;

   		char solidname1[100];

   		sprintf(solidname1, "HVCableToEPlate_Phase2_Solid_%d", counter);
   		G4Box* HVCableToEPlate= new G4Box(solidname1,
  				     		  HVPhase2ToEPlate_LengthX,
				     		  HVPhase2ToEPlate_LengthY,
				     		  fHVPhase2ToEPlate_Z);

   		char logicname1[100];

   		sprintf(logicname1, "HVCableToEPlate_Phase2_Logical_%d", counter);
   		G4LogicalVolume* fHVCabPlateLogical =  new G4LogicalVolume(HVCableToEPlate, kapton, logicname1);

   		fHVCabPlateLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   		fHVCableToEPlatePhase2Logical[iCol][jRow]=fHVCabPlateLogical;

   		//for top detector in pair
   		if(IsAPair(iCol,jRow)){
   			char solidname1top[100];

   			sprintf(solidname1top, "HVCableTopToEPlate_Phase2_Solid_%d", counter);
  			G4Box* HVCableTopToEPlate= new G4Box(solidname1top,
   							     HVPhase2ToEPlate_LengthX,
							     HVPhase2ToEPlate_LengthY,
							     fHVTopPhase2ToEPlate_Z);

   			char logicname1top[100];

   			sprintf(logicname1top, "HVCableTopToEPlate_Phase2_Logical_%d", counter);
   			G4LogicalVolume* fHVCabTopPlateLogical =  new G4LogicalVolume(HVCableTopToEPlate, kapton, logicname1top);

   			fHVCabTopPlateLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   			fHVCableTopToEPlatePhase2Logical[iCol][jRow]=fHVCabTopPlateLogical;
   			}

	      // +---------------------------------------------------------------------+
  	      // | Signal cable: pad on holder and cable from holder to electronics plate  |
  	      // +---------------------------------------------------------------------+
   		//pad on holder
   		G4double SignalPhase2AtHolder_LengthX=0.5*fPhase2Cable_WidthOnHolder;
   		G4double SignalPhase2AtHolder_LengthY=0.5*fPhase2Cable_LengthOnHolderSignal;
   		G4double SignalPhase2AtHolder_LengthZ=0.5*fPhase2Cable_Thickness;

   		char solidname2[100];

   		sprintf(solidname2, "SignalCableAtHolder_Phase2_Solid_%d", counter);

   		G4Box* SignalCableAtHolder = new G4Box(solidname2,
  				      		       SignalPhase2AtHolder_LengthX,
				      		       SignalPhase2AtHolder_LengthY,
				      		       SignalPhase2AtHolder_LengthZ);

   		char logicname2[100];

   		sprintf(logicname2, "SignalCableAtHolder_Phase2_Logical_%d", counter);
   		G4LogicalVolume* fSignalCabLogical =  new G4LogicalVolume(SignalCableAtHolder, kapton, logicname2);

   		fSignalCabLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   		fSignalCableAtHolderPhase2Logical[iCol][jRow]=fSignalCabLogical;

   		//for top detector in pair
   		if(IsAPair(iCol,jRow)){
   			char solidnametop2[100];

   			sprintf(solidnametop2, "SignalCableAtHolderTop_Phase2_Solid_%d", counter);
   			G4Box* SignalCableAtHolderTop = new G4Box(solidnametop2,
   					 			  SignalPhase2AtHolder_LengthX,
					 			  SignalPhase2AtHolder_LengthY,
					 			  SignalPhase2AtHolder_LengthZ);

  	 		char logicnametop2[100];

   			sprintf(logicnametop2, "SignalCableAtHolderTop_Phase2_Logical_%d", counter);
   			G4LogicalVolume* fSignalCabTopLogical =  new G4LogicalVolume(SignalCableAtHolderTop, kapton, logicnametop2);

   			fSignalCabTopLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   			fSignalCableAtHolderTopPhase2Logical[iCol][jRow]=fSignalCabTopLogical;
   			}

  		//cable from holder to electronics plate
  		//upper end at CC3, at the moment constructed from middle of array and implemented in GEGeometryLArInstHybrid.cc
   		//determine cable length
   		G4double fSignalPhase2ToEPlate_Z = 0.5*fHVPhase2ToEPlate_LengthZ[iCol][jRow];//same length as for HV
   		G4double fSignalTopPhase2ToEPlate_Z = 0.5*fHVTopPhase2ToEPlate_LengthZ[iCol][jRow];//same length as for HV
   		G4double SignalPhase2ToEPlate_LengthX=0.5*fPhase2Cable_WidthFromHolderToElectronicsPlate;
   		G4double SignalPhase2ToEPlate_LengthY=0.5*fPhase2Cable_Thickness;

   		char solidname3[100];

   		sprintf(solidname3, "SignalCableToEPlate_Phase2_Solid_%d", counter);
   		G4Box* SignalCableToEPlate= new G4Box(solidname3,
  				     		      SignalPhase2ToEPlate_LengthX,
				                      SignalPhase2ToEPlate_LengthY,
				     		      fSignalPhase2ToEPlate_Z);

   		char logicname3[100];

   		sprintf(logicname3, "SignalCableToEPlate_Phase2_Logical_%d", counter);
   		G4LogicalVolume* fSignalCabPlateLogical =  new G4LogicalVolume(SignalCableToEPlate, kapton, logicname3);

   		fSignalCabPlateLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   		fSignalCableToEPlatePhase2Logical[iCol][jRow]=fSignalCabPlateLogical;

   		//for top detector in pair
   		if(IsAPair(iCol,jRow)){
   			char solidname3top[100];

   			sprintf(solidname3top, "SignalCableTopToEPlate_Phase2_Solid_%d", counter);
   			G4Box* SignalCableTopToEPlate= new G4Box(solidname3top,
   					    	 		 SignalPhase2ToEPlate_LengthX,
					    	 		 SignalPhase2ToEPlate_LengthY,
					    	 		 fSignalTopPhase2ToEPlate_Z);

   			char logicname3top[100];

   			sprintf(logicname3top, "SignalCableTopToEPlate_Phase2_Logical_%d", counter);
   			G4LogicalVolume* fSignalCabTopPlateLogical =  new G4LogicalVolume(SignalCableTopToEPlate, kapton, logicname3top);

   			fSignalCabTopPlateLogical -> SetVisAttributes(TeflonVisAtt);//as white for visibility

   			fSignalCableTopToEPlatePhase2Logical[iCol][jRow]=fSignalCabTopPlateLogical;
   			}

   		MGLog(debugging) << "Logical volumes of PhaseII HV and Signal Cables are constructed for detector with ID " << iCrtlType << "." <<endlog;

  	counter++;
  	}
  }
}
// ---------------------------------------------------------------------------
//for rotation of holders and cables
G4double GEGeometryDetectorDB::GetAngleForPhase2CablePosition(G4int iColumn)
{
    G4int nstrings = GetDN500NStrings();
    G4double delta_angle = (360.0/(G4double)(nstrings-1))*deg;
    G4double HolderPlateRot_angle=0.*deg;
    if (iColumn>0) HolderPlateRot_angle = 180*deg-(iColumn-1)*delta_angle;
    return HolderPlateRot_angle;
}
