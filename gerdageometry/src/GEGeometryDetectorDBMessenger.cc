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
 * 01-23-2005, added commands for segments, Kevin
 * 02-07-2005, added commands for calibration source, Kevin
 * 02-16-2005, added commands for the position/rotation of the mu-veto, Luciano
 * 02-17-2005, corrected bug in two directory names
 * 03-23-2005, Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 * 07-05-2005, Added flag for Phase I/II holder, Luciano
 * 07-05-2005, Added flag for plastic scintillator hole (for the neck)
 *             and command to set the radius of the hole, Claudia
 * 03-06-2006, Added command to select PMT distribution
 * 12-13-2006, Set Liquid Argon as default cryogenic liquid, Luciano
 * 10-16-2007, Added flag for distance of cablechain from upper end of junctionsled, Daniel
 * 08-05-2008, Added commands for setting two different schemes for the z positions of the crystal strings
 *              (jens)
 * 04-24-2009  Added command for lock type, Francis
 * 14-06-2011  Added command to construct copper encapsulation for phase 0
 *             Added command to construct teflon encapsulation for phase 0
 *             Added command to construct both encapsulations for phase 0 at same time, Sabine
 * 05-11-2011  Added command to construct LAr Instrumentation. (N. Barros)
 * 			   Added command to select LAr Instrumentation design.
 * 26-03-2012 Added command to construct mini-shrouds, Giovanni Benato
 * 27-03-2012 - Added possibility to move each calibration source separately. Giovanni Benato
 * 22-05-2012 - Added possibility to move each calibration source separately in all 3 directions. Giovanni Benato
 * 25-05-2012 - Added command to customize the distance between a pair of BEGe's, Nuno
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real confituration in GERDA (Giovanni)
 * 18-07-2012 - Added commands for the new lock type for phase-II string geometry optimization (DN500). (Nuno Barros)
 * 14-08-2012 - Added option to build inner fiber cylinder in hybrid design. (Nuno Barros)
 * 06-09-2012 - Added two commands to permit forced flushing of the TTree buffers and header. (Nuno Barros)
 * 27-10-2012 - Added three new parameters/options to the LAr Instrumentation:
 *                  * Construct an external source to simulate backgrounds from the cable chain
 *                  * Apply a scale to the XUV argon absorption length
 *                  * Apply a scale to the reflectivities of metals
 * 23-12-2012 - Added APD design messenger (Bjoern Lehnert)
 * 16-07-2014 - Added MS control options, Bjoern Lehnert
 * 04-10-2016 - Added command for individual placement of detectors along z axis, Janina
 * 12-05-2016, JJ added macro command to turn off photon tracking
 * 28-07-2016, JJ added macro command to set LAr light yield and fiber coverage (Bjoern's code)
 * 19-11-2017, KvSturm changed defaults to reflect real PhaseII setup
 * 21-11-2017, Added macro commands for MS glued ring heights, KvSturm
 */

// ---------------------------------------------------------------------------

#include "gerdageometry/GEGeometryDetectorDBMessenger.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "io/MGLogger.hh"
#include "G4UIcmdWithADouble.hh"
// ---------------------------------------------------------------------------

using namespace CLHEP;

GEGeometryDetectorDBMessenger::GEGeometryDetectorDBMessenger(GEGeometryDetectorDB *DetectorDB) : fDetectorDB(DetectorDB)
{

  // directories

  fGeneralDir =  new G4UIdirectory("/MG/geometry/general/");
  fGeneralDir -> SetGuidance("Tune general geometry parameters");

  fShieldingDir =  new G4UIdirectory("/MG/geometry/shielding/");
  fShieldingDir -> SetGuidance("Tune shielding parameters");

  fDetectorGeneralDir =  new G4UIdirectory("/MG/geometry/detector/general/");
  fDetectorGeneralDir -> SetGuidance("Tune general detector parameters");

  fDetectorScintillatorDir =  new G4UIdirectory("/MG/geometry/detector/scint/");
  fDetectorScintillatorDir -> SetGuidance("Tune scintillator parameters");

  fDetectorCrystalDir =  new G4UIdirectory("/MG/geometry/detector/crystal/");
  fDetectorCrystalDir -> SetGuidance("Tune crystal parameters");

  fDetectorSupportDir =  new G4UIdirectory("/MG/geometry/detector/support/");
  fDetectorSupportDir -> SetGuidance("Tune support parameters");

  fDetectorCalibrationDir =  new G4UIdirectory("/MG/geometry/detector/calibration/");
  fDetectorCalibrationDir -> SetGuidance("Tune calibration source parameters");

  fDetectorCollimatedCalibrationDir =  new G4UIdirectory("/MG/geometry/detector/collcalibration/");
  fDetectorCollimatedCalibrationDir -> SetGuidance("Tune collimated calibration source parameters");

  fDetectorCherenkovDir = new G4UIdirectory("/MG/geometry/cherenkov/");
  fDetectorCherenkovDir -> SetGuidance("Select different photomultiplier distributions");

  fLockDir =  new G4UIdirectory("/MG/geometry/lock/");
  fLockDir -> SetGuidance("Tune lock parameters");

  fLockDN500Dir = new G4UIdirectory("/MG/geometry/lock/dn500/");
  fLockDN500Dir->SetGuidance("Tune parameters of phase-II DN500 lock.");


  fLArInstrDir = new G4UIdirectory("/MG/geometry/LArInst/");
  fLArInstrDir -> SetGuidance("Tune LAr Instrumentation geometry parameters");

  // directory: /MG/geometry/general/

  fFlagConstructDetectorCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructdetector", this);
  fFlagConstructDetectorCmd -> SetParameterName("FlagConstructDetector", true);
  fFlagConstructDetectorCmd -> SetDefaultValue(true);

  fFlagConstructCrystalsCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructcrystals", this);
  fFlagConstructCrystalsCmd -> SetParameterName("FlagConstructCrystals", true);
  fFlagConstructCrystalsCmd -> SetDefaultValue(true);

  fFlagConstructSupportCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructsupport", this);
  fFlagConstructSupportCmd -> SetParameterName("FlagConstructSupport", true);
  fFlagConstructSupportCmd -> SetDefaultValue(true);

  fFlagConstructCablingCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructcabling", this);
  fFlagConstructCablingCmd -> SetParameterName("FlagConstructCabling", true);
  fFlagConstructCablingCmd -> SetDefaultValue(true);

  fFlagConstructShieldingCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructshielding", this);
  fFlagConstructShieldingCmd -> SetParameterName("FlagConstructShielding", true);
  fFlagConstructShieldingCmd -> SetDefaultValue(true);

  fFlagConstructCalibrationSourceCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructcalibration", this);
  fFlagConstructCalibrationSourceCmd -> SetParameterName("FlagConstructCalibrationSource", true);
  fFlagConstructCalibrationSourceCmd -> SetDefaultValue(false);

  fFlagConstructCollimatedCalibrationSourceCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructcollcalibration", this);
  fFlagConstructCollimatedCalibrationSourceCmd -> SetParameterName("FlagConstructCollimatedCalibrationSource", true);
  fFlagConstructCollimatedCalibrationSourceCmd -> SetDefaultValue(false);

  fFlagConstructCopperEncapsulationCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructCopperEncapsulation", this);
  fFlagConstructCopperEncapsulationCmd -> SetParameterName("FlagConstructCopperEncapsulation", true);
  fFlagConstructCopperEncapsulationCmd -> SetDefaultValue(false);

  fFlagConstructTeflonEncapsulationCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructTeflonEncapsulation", this);
  fFlagConstructTeflonEncapsulationCmd -> SetParameterName("FlagConstructTeflonEncapsulation", true);
  fFlagConstructTeflonEncapsulationCmd -> SetDefaultValue(false);

  fFlagConstructEncapsulationsCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructEncapsulations", this);
  fFlagConstructEncapsulationsCmd -> SetParameterName("FlagConstructEncapsulations", true);
  fFlagConstructEncapsulationsCmd -> SetDefaultValue(false);

  fFlagConstructMiniShroudCmd =  new G4UIcmdWithABool("/MG/geometry/general/constructMiniShroud", this);
  fFlagConstructMiniShroudCmd -> SetParameterName("FlagConstructMiniShroud", true);
  fFlagConstructMiniShroudCmd -> SetDefaultValue(false);

  // PhaseI - MetalCopper
  // PhaseII - Nylon (default)
  fMiniShroudMaterialCmd = new G4UIcmdWithAString("/MG/geometry/general/miniShroudMaterial",this);
  fMiniShroudMaterialCmd -> SetGuidance("Sets the MS material. Default is Nylon. Available candidates are: ");
  fMiniShroudMaterialCmd -> SetGuidance("MetalCopper, Nylon");
  fMiniShroudMaterialCmd -> SetCandidates("MetalCopper Nylon");
  fMiniShroudMaterialCmd -> SetDefaultValue("Nylon");

  fMiniShroudBaseThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/general/miniShroudBaseThickness", this);
  fMiniShroudBaseThicknessCmd -> SetGuidance("Sets the MS thickness at the top and bottom sides ");
  fMiniShroudBaseThicknessCmd -> SetDefaultValue(0.125);
  fMiniShroudBaseThicknessCmd -> SetDefaultUnit("mm");

  // default was 0.06
  fMiniShroudSideThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/general/miniShroudSideThickness", this);
  fMiniShroudSideThicknessCmd -> SetGuidance("Sets the MS thickness at the lateral sides");
  fMiniShroudSideThicknessCmd -> SetDefaultValue(0.125);
  fMiniShroudSideThicknessCmd -> SetDefaultUnit("mm");

  fMiniShroudTopGluedRingHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/general/miniShroudTopGluedRingHeight", this);
  fMiniShroudTopGluedRingHeightCmd -> SetGuidance("Sets the MS top glued ring height");
  fMiniShroudTopGluedRingHeightCmd -> SetDefaultValue(2.0);
  fMiniShroudTopGluedRingHeightCmd -> SetDefaultUnit("cm");

  fMiniShroudBottomGluedRingHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/general/miniShroudBottomGluedRingHeight", this);
  fMiniShroudBottomGluedRingHeightCmd -> SetGuidance("Sets the MS bottom glued ring height");
  fMiniShroudBottomGluedRingHeightCmd -> SetDefaultValue(2.0);
  fMiniShroudBottomGluedRingHeightCmd -> SetDefaultUnit("cm");


  fFlagMinishroudToDetctorString =  new G4UIcmdWithABool("/MG/geometry/general/adjustMinishroudToDetctorString", this);
  fFlagMinishroudToDetctorString -> SetGuidance("Adjusts MS diameter to detector string type (coax MS larger than BEGe MS)");
  fFlagMinishroudToDetctorString -> SetParameterName("FlagMinishroudToDetctorString", true);
  fFlagMinishroudToDetctorString -> SetDefaultValue(true);




  // Construct LAr instrumentation. Only valid if we also have CryoLiquid active
  fFlagConstructLArInstrCmd = new G4UIcmdWithABool("/MG/geometry/general/constructLArInstrumentation", this);
  fFlagConstructLArInstrCmd	-> SetParameterName("FlagConstructLArInstrumentation",true);
  fFlagConstructLArInstrCmd	-> SetDefaultValue(false);
  fFlagConstructLArInstrCmd -> SetGuidance("Set false (default) to bypass the LAr Instrumentation");
  fFlagConstructLArInstrCmd -> SetGuidance("Set true to build the LAr Instrumentation ");
  fFlagConstructLArInstrCmd -> SetGuidance("(Implies activation of optical processes in the physics list)");


  // directory: /MG/geometry/shielding/

  fFlagCryoLiquidCmd =  new G4UIcmdWithABool("/MG/geometry/shielding/cryoliquid", this);
  fFlagCryoLiquidCmd -> SetParameterName("FlagCryoLiquid", true);
  fFlagCryoLiquidCmd -> SetDefaultValue(false);
  fFlagCryoLiquidCmd -> SetGuidance("Set false (default) for liquid argon and");
  fFlagCryoLiquidCmd -> SetGuidance("true for liquid nitrogen");

  // directory: /MG/geometry/detector/general/

  fArrayDepthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/general/depth", this);
  fArrayDepthCmd -> SetParameterName("ArrayDepth", true);
  fArrayDepthCmd -> SetDefaultValue(0.0);
  fArrayDepthCmd -> SetDefaultUnit("cm");

  // directory: /MG/geometry/detector/cable/

  fCableChainDistanceCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/cable/cablechaindistance", this);
  fCableChainDistanceCmd -> SetParameterName("CableChainDistance", true);
  fCableChainDistanceCmd -> SetDefaultValue(1.0);
  fCableChainDistanceCmd -> SetDefaultUnit("m");


  // directory: /MG/geometry/scint/

  fScintillatorHoleCmd =  new G4UIcmdWithABool("/MG/geometry/detector/scint/hole", this);
  fScintillatorHoleCmd -> SetParameterName("ScintillatorHole", true);
  fScintillatorHoleCmd -> SetDefaultValue(false);

  fScintillatorHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/height", this);
  fScintillatorHeightCmd -> SetParameterName("ScintHeight", true);
  fScintillatorHeightCmd -> SetDefaultValue(4.0);
  fScintillatorHeightCmd -> SetDefaultUnit("cm");

  fScintillatorLengthXCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/lengthx", this);
  fScintillatorLengthXCmd -> SetParameterName("ScintLengthX", true);
  fScintillatorLengthXCmd -> SetDefaultValue(4.0);
  fScintillatorLengthXCmd -> SetDefaultUnit("m");

  fScintillatorLengthYCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/lengthy", this);
  fScintillatorLengthYCmd -> SetParameterName("ScintLengthY", true);
  fScintillatorLengthYCmd -> SetDefaultValue(4.0);
  fScintillatorLengthYCmd -> SetDefaultUnit("m");

  fScintillatorPosXCmd =
  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/positionx", this);
  fScintillatorPosXCmd->SetGuidance("Set X position of the plastic scintillator");
  fScintillatorPosXCmd->SetGuidance("Default value: 0 cm");
  fScintillatorPosXCmd->SetDefaultUnit("cm");
  fScintillatorPosXCmd->SetUnitCandidates("mm cm m");

  fScintillatorPosYCmd =
  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/positiony", this);
  fScintillatorPosYCmd->SetGuidance("Set Y position of the plastic scintillator");
  fScintillatorPosYCmd->SetGuidance("Default value: 0 cm");
  fScintillatorPosYCmd->SetDefaultUnit("cm");
  fScintillatorPosYCmd->SetUnitCandidates("mm cm m");

  fScintillatorPosZCmd =
  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/positionz", this);
  fScintillatorPosZCmd->SetGuidance("Set distance of the plastic scintillator from the water tank");
  fScintillatorPosZCmd->SetGuidance("Default value: 300 cm");
  fScintillatorPosZCmd->SetDefaultUnit("cm");
  fScintillatorPosZCmd->SetUnitCandidates("mm cm m");

  fScintillatorAngleZCmd = new
  G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/rotation", this);
  fScintillatorAngleZCmd->SetGuidance("Set rotation angle around Z axis");
  fScintillatorAngleZCmd->SetGuidance("Default value: 0 deg");
  fScintillatorAngleZCmd->SetDefaultUnit("deg");
  fScintillatorAngleZCmd->SetUnitCandidates("deg radian");

  fScintillatorHoleOuterRadiusCmd =
  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/scint/holeradius", this);
  fScintillatorHoleOuterRadiusCmd->SetGuidance("Set radius of the hole (for the neck) in the plastic scintillator");
  fScintillatorHoleOuterRadiusCmd->SetGuidance("Default value: 80 cm");
  fScintillatorHoleOuterRadiusCmd->SetDefaultUnit("cm");
  fScintillatorHoleOuterRadiusCmd->SetUnitCandidates("mm cm m");

  // directory: /MG/geometry/crystal/

  fCrystalGeometryFileCmd = new G4UIcmdWithAString("/MG/geometry/detector/geometryfile",
						   this);
  fCrystalGeometryFileCmd -> SetParameterName("GeometryFile", true);
  fCrystalGeometryFileCmd -> SetDefaultValue("./gerdageometry/geometry_phase_ii.dat");

  fCrystalMatrixFileCmd = new G4UIcmdWithAString("/MG/geometry/detector/matrixfile",
						   this);
  fCrystalMatrixFileCmd -> SetParameterName("MatrixFile", true);
  fCrystalMatrixFileCmd -> SetDefaultValue("./gerdageometry/matrix_phase_ii.dat");

  // use matrix file with two columns to choose the detector positioning
  fFlagCrystalMatrixWithZPosFileCmd = new G4UIcmdWithABool("/MG/geometry/detector/individualzposofdetectors", this);
  fFlagCrystalMatrixWithZPosFileCmd -> SetParameterName("FlagCrystalMatrixWithZPosFile", true);
  fFlagCrystalMatrixWithZPosFileCmd -> SetDefaultValue(true);
  fFlagCrystalMatrixWithZPosFileCmd -> SetGuidance("Set true (not default) for individual z position of detectors.");
  fFlagCrystalMatrixWithZPosFileCmd -> SetGuidance("They are specified in second column of matrix file.");
  fFlagCrystalMatrixWithZPosFileCmd -> SetGuidance("Relevant for PhaseII detectors.");

  fCrystalZShiftSchemeCmd = new G4UIcmdWithAString("/MG/geometry/detector/CrystalZShiftScheme", this);
  fCrystalZShiftSchemeCmd -> SetParameterName("zShiftScheme", true);
  fCrystalZShiftSchemeCmd -> SetDefaultValue("unshifted");
  fCrystalZShiftSchemeCmd -> SetGuidance("Sets the relative z position among the detector strings.");
  fCrystalZShiftSchemeCmd -> SetGuidance("Possible values are:");
  fCrystalZShiftSchemeCmd -> SetGuidance(" - unshifted");
  fCrystalZShiftSchemeCmd -> SetGuidance(" - CrystalZShiftScheme_center_between_top0_and_bottom2_on_common_zlevel");

  fCrystalZShiftSchemeUnshiftedColumnCmd = new G4UIcmdWithAnInteger("/MG/geometry/detector/CrystalZShiftScheme_UnshiftedColumn", this);
  fCrystalZShiftSchemeUnshiftedColumnCmd -> SetParameterName("UnshiftedColumn", true);
  fCrystalZShiftSchemeUnshiftedColumnCmd -> SetDefaultValue(18);
  fCrystalZShiftSchemeUnshiftedColumnCmd -> SetGuidance("Sets the column whose unshifted state is used as a reference to set z positions of the other strings.");

  fCrystalBEGePairGapCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/crystal/begepairgap",this);
  fCrystalBEGePairGapCmd -> SetParameterName("gap",true);
  fCrystalBEGePairGapCmd -> SetDefaultValue(fDetectorDB->GetCrystalPairGap()/mm);
  fCrystalBEGePairGapCmd -> SetDefaultUnit("mm");
  fCrystalBEGePairGapCmd -> SetGuidance("Sets the distance between the BEGes in a pair (type 3, 4 and 5).");

  fCrystalReverseNonTrueCoaxialCmd = new G4UIcmdWithABool("/MG/geometry/detector/crystal/reverse", this);
  fCrystalReverseNonTrueCoaxialCmd -> SetParameterName("ReverseNonTrueCoaxial", true);
  fCrystalReverseNonTrueCoaxialCmd -> SetDefaultValue(true);
  fCrystalReverseNonTrueCoaxialCmd -> SetGuidance("Reverse the non true coaxial crystal upside-down to match the real configuration");

  // directory : MG/geometry/detector/calibration/

  fCalibrationSourcePositionZ =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcez", this);
  fCalibrationSourcePositionZ -> SetParameterName("CalibrationSourcePosition", true);
  fCalibrationSourcePositionZ -> SetDefaultValue(0.0);
  fCalibrationSourcePositionZ -> SetDefaultUnit("cm");

  // Change S1, S2, S3 heights separately
  fCalibrationSourcePositionX_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcex_S1", this);
  fCalibrationSourcePositionX_S1 -> SetParameterName("CalibrationSourcePositionX_S1", true);
  fCalibrationSourcePositionX_S1 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionX_S1 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionX_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcex_S2", this);
  fCalibrationSourcePositionX_S2 -> SetParameterName("CalibrationSourcePositionX_S2", true);
  fCalibrationSourcePositionX_S2 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionX_S2 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionX_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcex_S3", this);
  fCalibrationSourcePositionX_S3 -> SetParameterName("CalibrationSourcePositionX_S3", true);
  fCalibrationSourcePositionX_S3 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionX_S3 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionY_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcey_S1", this);
  fCalibrationSourcePositionY_S1 -> SetParameterName("CalibrationSourcePositionY_S1", true);
  fCalibrationSourcePositionY_S1 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionY_S1 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionY_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcey_S2", this);
  fCalibrationSourcePositionY_S2 -> SetParameterName("CalibrationSourcePositionY_S2", true);
  fCalibrationSourcePositionY_S2 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionY_S2 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionY_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcey_S3", this);
  fCalibrationSourcePositionY_S3 -> SetParameterName("CalibrationSourcePositionY_S3", true);
  fCalibrationSourcePositionY_S3 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionY_S3 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionZ_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcez_S1", this);
  fCalibrationSourcePositionZ_S1 -> SetParameterName("CalibrationSourcePositionZ_S1", true);
  fCalibrationSourcePositionZ_S1 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionZ_S1 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionZ_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcez_S2", this);
  fCalibrationSourcePositionZ_S2 -> SetParameterName("CalibrationSourcePositionZ_S2", true);
  fCalibrationSourcePositionZ_S2 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionZ_S2 -> SetDefaultUnit("cm");

  fCalibrationSourcePositionZ_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/calibration/sourcez_S3", this);
  fCalibrationSourcePositionZ_S3 -> SetParameterName("CalibrationSourcePositionZ_S3", true);
  fCalibrationSourcePositionZ_S3 -> SetDefaultValue(0.0);
  fCalibrationSourcePositionZ_S3 -> SetDefaultUnit("cm");

  //Cu cap on the source for LAr veto calibration
  fCalibrationSourceCuCap = new G4UIcmdWithABool("/MG/geometry/detector/calibration/CuCap",this);
  fCalibrationSourceCuCap->SetParameterName("CuCap",true);
  fCalibrationSourceCuCap->SetDefaultValue(false);
  fCalibrationSourceCuCap->SetGuidance("Place a Cu cap on the source: Default is false");

  // directory : MG/geometry/detector/collimatedcalibration/
  // Change S1, S2, S3 heights separately
  fCollimatedCalibrationSourcePositionX_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcex_S1", this);
  fCollimatedCalibrationSourcePositionX_S1 -> SetParameterName("CollimatedCalibrationSourcePositionX_S1", true);
  fCollimatedCalibrationSourcePositionX_S1 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionX_S1 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionX_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcex_S2", this);
  fCollimatedCalibrationSourcePositionX_S2 -> SetParameterName("CollimatedCalibrationSourcePositionX_S2", true);
  fCollimatedCalibrationSourcePositionX_S2 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionX_S2 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionX_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcex_S3", this);
  fCollimatedCalibrationSourcePositionX_S3 -> SetParameterName("CollimatedCalibrationSourcePositionX_S3", true);
  fCollimatedCalibrationSourcePositionX_S3 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionX_S3 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionY_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcey_S1", this);
  fCollimatedCalibrationSourcePositionY_S1 -> SetParameterName("CollimatedCalibrationSourcePositionY_S1", true);
  fCollimatedCalibrationSourcePositionY_S1 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionY_S1 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionY_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcey_S2", this);
  fCollimatedCalibrationSourcePositionY_S2 -> SetParameterName("CollimatedCalibrationSourcePositionY_S2", true);
  fCollimatedCalibrationSourcePositionY_S2 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionY_S2 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionY_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcey_S3", this);
  fCollimatedCalibrationSourcePositionY_S3 -> SetParameterName("CollimatedCalibrationSourcePositionY_S3", true);
  fCollimatedCalibrationSourcePositionY_S3 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionY_S3 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionZ_S1 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcez_S1", this);
  fCollimatedCalibrationSourcePositionZ_S1 -> SetParameterName("CollimatedCalibrationSourcePositionZ_S1", true);
  fCollimatedCalibrationSourcePositionZ_S1 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionZ_S1 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionZ_S2 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcez_S2", this);
  fCollimatedCalibrationSourcePositionZ_S2 -> SetParameterName("CollimatedCalibrationSourcePositionZ_S2", true);
  fCollimatedCalibrationSourcePositionZ_S2 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionZ_S2 -> SetDefaultUnit("cm");

  fCollimatedCalibrationSourcePositionZ_S3 =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/detector/collcalibration/sourcez_S3", this);
  fCollimatedCalibrationSourcePositionZ_S3 -> SetParameterName("CollimatedCalibrationSourcePositionZ_S3", true);
  fCollimatedCalibrationSourcePositionZ_S3 -> SetDefaultValue(0.0);
  fCollimatedCalibrationSourcePositionZ_S3 -> SetDefaultUnit("cm");

  // directory : MG/geometry/cherenkov/

  fSelectPMTDistribution = new  G4UIcmdWithAnInteger("/MG/geometry/cherenkov", this);


  // directory : MG/geometry/lock/

  fLockTypeCmd = new G4UIcmdWithAString("/MG/geometry/lock/type", this);
  fLockTypeCmd-> SetParameterName("LockType", true);
  fLockTypeCmd-> SetDefaultValue("PhaseII_DN500");
  fLockTypeCmd->SetGuidance("Sets lock and used detector string configuration");
  fLockTypeCmd->SetGuidance("Possible values are:");
  fLockTypeCmd->SetGuidance(" - final");
  fLockTypeCmd->SetGuidance(" - CommLock2 (2 detector strings + 1 calibration source)");
  fLockTypeCmd->SetGuidance(" - CommLock4 (4 detector strings + 1 calibration source)");
  fLockTypeCmd->SetGuidance(" - PhaseII (twin-lock with 3 strings each)");
  fLockTypeCmd->SetGuidance(" - PhaseII_DN500 (configurable lock with N detectors: 1 central string and N-1 equally spaced strings)");

  // additional parameters if lock type is dn500
  fLockNStringsCmd = new G4UIcmdWithAnInteger("/MG/geometry/lock/dn500/n_strings",this);
  fLockNStringsCmd->SetParameterName("NumberStrings",false);
  fLockNStringsCmd->SetDefaultValue(7); // by default define 7 string positions
  fLockNStringsCmd->SetGuidance("Sets the number of strings to be created. Default value: 5.");

  fLockDistanceStringsCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/lock/dn500/string_distance", this);
  fLockDistanceStringsCmd->SetParameterName("StringDistance",false);
  fLockDistanceStringsCmd->SetDefaultValue(11.0);
  fLockDistanceStringsCmd->SetDefaultUnit("cm");
  fLockDistanceStringsCmd->SetGuidance("Sets the distance between the central string and the radial strings. By default, actual PhaseIIa number of 11 cm is used.");

  // directory : MG/geometry/LArInst/
  fLArInstDesignCmd = new G4UIcmdWithAString("/MG/geometry/LArInst/SelectDesign",this);
  fLArInstDesignCmd->SetParameterName("DesignName",false);
  fLArInstDesignCmd->SetDefaultValue("hybrid");
  fLArInstDesignCmd->SetGuidance("Selects the LAr Instrumentation design to construct");
  fLArInstDesignCmd->SetGuidance("Possible values are:");
  fLArInstDesignCmd->SetGuidance(" - test : Simple fiber optics cylinder around the strings");
  fLArInstDesignCmd->SetGuidance(" - fibers : TUM fiber design.");
  fLArInstDesignCmd->SetGuidance(" - PMT : MPIK PMT design");
  fLArInstDesignCmd->SetGuidance(" - hybrid : Dual PMT-Fiber design");
  fLArInstDesignCmd->SetGuidance(" - APD : APD Design inside MS");
  fLArInstDesignCmd->SetGuidance(" - super-hybrid : Design with PMTs, fibers AND APDs (inside MS)");

  fLarInstConstructInnerFiberCmd = new G4UIcmdWithABool("/MG/geometry/LArInst/ConstructInnerFibers",this);
  fLarInstConstructInnerFiberCmd->SetParameterName("status",false);
  fLarInstConstructInnerFiberCmd->SetDefaultValue(false);
  fLarInstConstructInnerFiberCmd->SetGuidance("Selects whether inner fiber array should be constructed.");
  fLarInstConstructInnerFiberCmd->SetGuidance("This array is placed around the central fiber (even if there are no crystals there).");

  fLarInstConstructExtSourceCmd =  new G4UIcmdWithABool("/MG/geometry/LArInst/ConstructExtSource",this);
  fLarInstConstructExtSourceCmd->SetParameterName("status",false);
  fLarInstConstructExtSourceCmd->SetDefaultValue(false);
  fLarInstConstructExtSourceCmd->SetGuidance("Selects whether an External Source solid should be built along with the LAr Instrumentation.");
  fLarInstConstructExtSourceCmd->SetGuidance("The implementation is design specific and is a placeholder to simulate backgrounds at the bottom of the cable chain..");

  fLArInstScaleLArAbsLCmd =  new G4UIcmdWithADouble("/MG/geometry/LArInst/ScaleLArXUVAbsLength",this);
  fLArInstScaleLArAbsLCmd->SetParameterName("scale",true);
  fLArInstScaleLArAbsLCmd->SetDefaultValue(0.25);
  fLArInstScaleLArAbsLCmd->SetGuidance("Set a scaling factor for the attenuation length for XUV in LAr.");
  fLArInstScaleLArAbsLCmd->SetGuidance("Default value (0.25) corresponds to an attenuation length of 60/4 cm.");
  fLArInstScaleLArAbsLCmd->SetGuidance("Useful for systematic studies.");

  fLArInstScaleLArLYCmd =  new G4UIcmdWithADouble("/MG/geometry/LArInst/ScaleLArXUVLightYield",this);
  fLArInstScaleLArLYCmd->SetParameterName("scale",false);
  fLArInstScaleLArLYCmd->SetDefaultValue(1.0);
  fLArInstScaleLArLYCmd->SetGuidance("Set a scaling factor for the light yield in LAr w.r.t the coded value of 28120 / MeV.");
  fLArInstScaleLArLYCmd->SetGuidance("Default value (1.0) corresponds to a light yield of 28120 / MeV.");
  fLArInstScaleLArLYCmd->SetGuidance("Useful for systematic studies.");

  fLArInstFiberCoverageCmd =  new G4UIcmdWithADouble("/MG/geometry/LArInst/SetFiberCoverage",this);
  fLArInstFiberCoverageCmd->SetParameterName("scale",false);
  fLArInstFiberCoverageCmd->SetDefaultValue(1.0);
  fLArInstFiberCoverageCmd->SetGuidance("Set coverage fraction of fiber shroud");
  fLArInstFiberCoverageCmd->SetGuidance("Default value (1.0) but more likeli value 0.5");
  fLArInstFiberCoverageCmd->SetGuidance("Useful for systematic studies.");

  fLArInstTopPMTscaleFactCmd =  new G4UIcmdWithADouble("/MG/geometry/LArInst/ScaleTopPMT",this);
  fLArInstTopPMTscaleFactCmd->SetParameterName("scale",false);
  fLArInstTopPMTscaleFactCmd->SetDefaultValue(1.0);
  fLArInstTopPMTscaleFactCmd->SetGuidance("Set top PMT efficiency scaling factor");
  fLArInstTopPMTscaleFactCmd->SetGuidance("Default value (1.0) but more likely value is 0.4");

  fLArInstScaleMetalReflCmd =  new G4UIcmdWithADouble("/MG/geometry/LArInst/ScaleMetalReflectivities",this);
  fLArInstScaleMetalReflCmd->SetParameterName("scale",false);
  fLArInstScaleMetalReflCmd->SetDefaultValue(1.0);
  fLArInstScaleMetalReflCmd->SetGuidance("Set a scaling factor on the metals reflectivities.");
  fLArInstScaleMetalReflCmd->SetGuidance("Default value (1.0) corresponds to measurements performed in MPIK.");
  fLArInstScaleMetalReflCmd->SetGuidance("Useful for systematic studies.");

  fLArInstAPDDesignCmd = new G4UIcmdWithAString("/MG/geometry/LArInst/SelectAPDDesign",this);
  fLArInstAPDDesignCmd->SetParameterName("APDDesignName",false);
  fLArInstAPDDesignCmd->SetDefaultValue("APD_VUV");
  fLArInstAPDDesignCmd->SetGuidance("Selects the APD LAr Instrumentation design to construct");
  fLArInstAPDDesignCmd->SetGuidance("Possible values are:");
  fLArInstAPDDesignCmd->SetGuidance(" - APD_VUV : APD - Hamamatsu S8664-1010 - VUV sensitive");
  fLArInstAPDDesignCmd->SetGuidance(" - APD_nonVUV : APD - Hamamatsu S8664-1010 - not VUV sensitive");
  fLArInstAPDDesignCmd->SetGuidance(" - SiPM : SiPM - Hamamatsu S11828-334M");

  fLArInstAPDDeploymentCmd = new G4UIcmdWithAString("/MG/geometry/LArInst/SelectAPDDeployment",this);
  fLArInstAPDDeploymentCmd->SetParameterName("APDDeploymentName",false);
  fLArInstAPDDeploymentCmd->SetDefaultValue("betweenBEGePairs");
  fLArInstAPDDeploymentCmd->SetGuidance("Selects the APD deployment scheme");
  fLArInstAPDDeploymentCmd->SetGuidance("Possible values are:");
  fLArInstAPDDeploymentCmd->SetGuidance(" - betweenBEGePairs : In between BEGe pairs");
  fLArInstAPDDeploymentCmd->SetGuidance(" - betweenBEGes : In between all BEGes");

  fLArInstAPDMSCoatingCmd = new G4UIcmdWithAString("/MG/geometry/LArInst/MinisShroudOpticalCoating",this);
  fLArInstAPDMSCoatingCmd->SetParameterName("MinisShroudOpticalCoating",false);
  fLArInstAPDMSCoatingCmd->SetDefaultValue("TPB");
  fLArInstAPDMSCoatingCmd->SetGuidance("Chooses the optical coating on the inside of the MS");
  fLArInstAPDMSCoatingCmd->SetGuidance("Possible values are:");
  fLArInstAPDMSCoatingCmd->SetGuidance(" - none");
  fLArInstAPDMSCoatingCmd->SetGuidance(" - VM2000_TPB");
  fLArInstAPDMSCoatingCmd->SetGuidance(" - TPB");

  fLArInstPhotTrackingCmd = new G4UIcmdWithABool("/MG/geometry/LArInst/PhotonTracking",this);
  fLArInstPhotTrackingCmd->SetParameterName("PhotonTracking",true);
  fLArInstPhotTrackingCmd->SetDefaultValue(false);
  fLArInstPhotTrackingCmd->SetGuidance("Turn ON photon tracking: Default is OFF");

  fLArInstConstructSegmentedFiberShroudCmd = new G4UIcmdWithABool("/MG/geometry/LArInst/ConstructSegmentedFiberShroud",this);
  fLArInstConstructSegmentedFiberShroudCmd->SetParameterName("status",true);
  fLArInstConstructSegmentedFiberShroudCmd->SetDefaultValue(true);
  fLArInstConstructSegmentedFiberShroudCmd->SetGuidance("Selects whether segmented fiber shroud should be constructed.");

  fLArInstSegmentedFiberShroudRotationCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/LArInst/SegmentedFiberShroudRotation",this);
  fLArInstSegmentedFiberShroudRotationCmd->SetParameterName("Segmented fiber shroud rotation",true);
  fLArInstSegmentedFiberShroudRotationCmd->SetGuidance("Default value: -7 deg");
  fLArInstSegmentedFiberShroudRotationCmd->SetDefaultUnit("deg");
  fLArInstSegmentedFiberShroudRotationCmd->SetUnitCandidates("deg radian");
}

// ---------------------------------------------------------------------------

GEGeometryDetectorDBMessenger::~GEGeometryDetectorDBMessenger()
{

  delete fGeneralDir;
  delete fShieldingDir;
  delete fDetectorGeneralDir;
  delete fDetectorScintillatorDir;
  delete fDetectorCrystalDir;
  delete fDetectorSupportDir;
  delete fLockDir;
  delete fLockDN500Dir;
  delete fLArInstrDir;

  delete fFlagConstructDetectorCmd;
  delete fFlagConstructCrystalsCmd;
  delete fFlagConstructSupportCmd;
  delete fFlagConstructCablingCmd;
  delete fFlagConstructShieldingCmd;
  delete fFlagConstructCalibrationSourceCmd;
  delete fFlagConstructCollimatedCalibrationSourceCmd;
  delete fFlagConstructCopperEncapsulationCmd;
  delete fFlagConstructTeflonEncapsulationCmd;
  delete fFlagConstructEncapsulationsCmd;
  delete fFlagConstructMiniShroudCmd;
  delete fMiniShroudMaterialCmd;
  delete fMiniShroudSideThicknessCmd;
  delete fMiniShroudBaseThicknessCmd;
  delete fMiniShroudTopGluedRingHeightCmd;
  delete fMiniShroudBottomGluedRingHeightCmd;
  delete fFlagMinishroudToDetctorString;

  delete fFlagConstructLArInstrCmd;

  delete fFlagCryoLiquidCmd;

  delete fArrayDepthCmd;

  delete fScintillatorHoleCmd;
  delete fScintillatorHeightCmd;
  delete fScintillatorLengthXCmd;
  delete fScintillatorLengthYCmd;
  delete fScintillatorPosXCmd;
  delete fScintillatorPosYCmd;
  delete fScintillatorPosZCmd;
  delete fScintillatorAngleZCmd;
  delete fScintillatorHoleOuterRadiusCmd;

  delete fCrystalGeometryFileCmd;
  delete fCrystalMatrixFileCmd;
  delete fFlagCrystalMatrixWithZPosFileCmd;
  delete fCrystalReverseNonTrueCoaxialCmd;

  delete fCalibrationSourcePositionZ;
  delete fCalibrationSourcePositionX_S1;
  delete fCalibrationSourcePositionX_S2;
  delete fCalibrationSourcePositionX_S3;
  delete fCalibrationSourcePositionY_S1;
  delete fCalibrationSourcePositionY_S2;
  delete fCalibrationSourcePositionY_S3;
  delete fCalibrationSourcePositionZ_S1;
  delete fCalibrationSourcePositionZ_S2;
  delete fCalibrationSourcePositionZ_S3;

  delete fCalibrationSourceCuCap;

  delete fCollimatedCalibrationSourcePositionZ;
  delete fCollimatedCalibrationSourcePositionX_S1;
  delete fCollimatedCalibrationSourcePositionX_S2;
  delete fCollimatedCalibrationSourcePositionX_S3;
  delete fCollimatedCalibrationSourcePositionY_S1;
  delete fCollimatedCalibrationSourcePositionY_S2;
  delete fCollimatedCalibrationSourcePositionY_S3;
  delete fCollimatedCalibrationSourcePositionZ_S1;
  delete fCollimatedCalibrationSourcePositionZ_S2;
  delete fCollimatedCalibrationSourcePositionZ_S3;

  delete fSelectPMTDistribution;

  delete fLockTypeCmd;
  delete fLockNStringsCmd;
  delete fLockDistanceStringsCmd;

  delete fLArInstDesignCmd;
  delete fLarInstConstructInnerFiberCmd;
  delete fLarInstConstructExtSourceCmd;
  delete fLArInstScaleLArAbsLCmd;

  delete fLArInstAPDDesignCmd;
  delete fLArInstAPDDeploymentCmd;

  delete fLArInstAPDMSCoatingCmd;
  delete fCrystalBEGePairGapCmd;

  delete fLArInstScaleLArLYCmd;
  delete fLArInstFiberCoverageCmd;
  delete fLArInstTopPMTscaleFactCmd;
  delete fLArInstScaleMetalReflCmd;

  delete fLArInstConstructSegmentedFiberShroudCmd;
  delete fLArInstSegmentedFiberShroudRotationCmd;
}

// ---------------------------------------------------------------------------

void GEGeometryDetectorDBMessenger::SetNewValue(G4UIcommand *cmd, G4String newValue)
{

  // directory: /MG/geometry/general/

  if (cmd == fFlagConstructDetectorCmd)
    {
      G4bool flag = fFlagConstructDetectorCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructDetector(flag);
    }

  else if (cmd == fFlagConstructCrystalsCmd)
    {
      G4bool flag = fFlagConstructCrystalsCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCrystals(flag);
    }

  else if (cmd == fFlagConstructSupportCmd)
    {
      G4bool flag = fFlagConstructSupportCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructSupport(flag);
    }

  else if (cmd == fFlagConstructCablingCmd)
    {
      G4bool flag = fFlagConstructCablingCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCabling(flag);
    }

  else if (cmd == fFlagConstructShieldingCmd)
    {
      G4bool flag = fFlagConstructShieldingCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructShielding(flag);
    }

  else if (cmd == fFlagConstructCalibrationSourceCmd)
    {
      G4bool flag = fFlagConstructCalibrationSourceCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCalibrationSource(flag);
    }

  else if (cmd == fFlagConstructCollimatedCalibrationSourceCmd)
    {
      G4bool flag = fFlagConstructCollimatedCalibrationSourceCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCollimatedCalibrationSource(flag);
    }

  else if (cmd == fFlagConstructCopperEncapsulationCmd)
    {
      G4bool flag = fFlagConstructCopperEncapsulationCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCopperEncapsulation(flag);
    }

  else if (cmd == fFlagConstructTeflonEncapsulationCmd)
    {
      G4bool flag = fFlagConstructTeflonEncapsulationCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructTeflonEncapsulation(flag);
    }

  else if (cmd == fFlagConstructEncapsulationsCmd)
    {
      G4bool flag = fFlagConstructEncapsulationsCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCopperEncapsulation(flag);
      fDetectorDB -> SetFlagConstructTeflonEncapsulation(flag);
    }

  else if (cmd == fFlagConstructMiniShroudCmd)
    {
      G4bool flag = fFlagConstructMiniShroudCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructMiniShroud(flag);
    }

  else if (cmd == fMiniShroudMaterialCmd)
    {
      fDetectorDB -> SetMiniShroudMaterial(newValue);
    }


  else if (cmd == fMiniShroudSideThicknessCmd)
    {
      G4double value = fMiniShroudSideThicknessCmd -> GetNewDoubleValue(newValue);
      fDetectorDB -> SetMiniShroudSideThickness(value);
    }

  else if (cmd == fMiniShroudBaseThicknessCmd)
    {
      G4double value = fMiniShroudBaseThicknessCmd -> GetNewDoubleValue(newValue);
      fDetectorDB -> SetMiniShroudBaseThickness(value);
    }

   else if ( cmd == fMiniShroudTopGluedRingHeightCmd )
   {
       G4double value = fMiniShroudTopGluedRingHeightCmd -> GetNewDoubleValue(newValue);
       fDetectorDB -> SetMiniShroudTopGluedRingHeight(value);
   }

   else if ( cmd == fMiniShroudBottomGluedRingHeightCmd )
   {
       G4double value = fMiniShroudBottomGluedRingHeightCmd -> GetNewDoubleValue(newValue);
       fDetectorDB -> SetMiniShroudBottomGluedRingHeight(value);
   }

  else if (cmd == fFlagMinishroudToDetctorString)
    {
      G4bool flag = fFlagMinishroudToDetctorString -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagAdaptMinishroudToDetctorString(flag);
    }




  else if (cmd == fFlagConstructLArInstrCmd)
      {
        G4bool flag = fFlagConstructLArInstrCmd -> GetNewBoolValue(newValue);
        MGLog(trace) << "Setting flag to build LAr Instrumentation to " << flag << endlog;
        fDetectorDB -> SetFlagConstructLArInstrumentation(flag);
      }


  // directory: /MG/geometry/shielding/

  else if (cmd == fFlagCryoLiquidCmd)
    {
      G4bool flag = fFlagCryoLiquidCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagCryoLiquid(flag);
    }

  // array

  else if (cmd == fArrayDepthCmd)
    {
      G4double depth = fArrayDepthCmd -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetArrayDepth(depth);
    }

  // cablechain

  else if (cmd ==  fCableChainDistanceCmd)
    {
      G4double distance = fCableChainDistanceCmd-> GetNewDoubleValue(newValue);

      fDetectorDB -> SetElectronicsCableChainDistance(distance);
    }



  // directory: /MG/geometry/scint

  else if (cmd == fScintillatorHoleCmd)
    {
      fDetectorDB->SetScintillatorHole(fScintillatorHoleCmd->GetNewBoolValue(newValue));
    }

  else if (cmd == fScintillatorHeightCmd)
    {
      G4double h = fScintillatorHeightCmd -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetScintillatorHeight(h);
    }

 else if (cmd == fScintillatorLengthXCmd)
    {
      G4double l = fScintillatorLengthXCmd -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetScintillatorLengthX(l);
    }

 else if (cmd == fScintillatorLengthYCmd)
    {
      G4double l = fScintillatorLengthYCmd -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetScintillatorLengthY(l);
    }
  else if (cmd == fScintillatorPosXCmd)
    {
      fDetectorDB->SetScintillatorPosX(fScintillatorPosXCmd->GetNewDoubleValue(newValue));
    }
  else if (cmd == fScintillatorPosYCmd)
    {
      fDetectorDB->SetScintillatorPosY(fScintillatorPosYCmd->GetNewDoubleValue(newValue));
    }
  else if (cmd == fScintillatorPosZCmd)
    {
       fDetectorDB->SetScintillatorPosZ(fScintillatorPosZCmd->GetNewDoubleValue(newValue));
    }
  else if (cmd == fScintillatorAngleZCmd)
    {
      fDetectorDB->SetScintillatorAngleZ(fScintillatorAngleZCmd->GetNewDoubleValue(newValue));
    }
  else if (cmd == fScintillatorHoleOuterRadiusCmd)
    {
      fDetectorDB->SetScintillatorHoleOuterRadius(fScintillatorHoleOuterRadiusCmd->GetNewDoubleValue(newValue));
    }

  // directory: /MG/geometry/crystal/

  else if (cmd == fCrystalGeometryFileCmd)
    {
      G4String filename = newValue;

      fDetectorDB -> SetCrystalGeometryFilename(filename);
    }

  else if (cmd == fCrystalMatrixFileCmd)
    {
      G4String filename = newValue;

      fDetectorDB -> SetCrystalMatrixFilename(filename);
    }

  else if(cmd==fCrystalZShiftSchemeCmd)
    {

        G4String strScheme = newValue;

        if(     strScheme.compare( "unshifted" )==0 )
            fDetectorDB -> SetZShiftScheme( CrystalZShiftScheme_unshifted );
        else if(strScheme.compare( "center_between_top0_and_bottom2_on_common_zlevel" )==0 )
            fDetectorDB -> SetZShiftScheme( CrystalZShiftScheme_center_between_top0_and_bottom2_on_common_zlevel );
        else {
            MGLog(error) << "\""<<strScheme<<"\" is an invalid parameter value." << endlog;
            abort();
        }
    }

  else if(cmd==fCrystalZShiftSchemeUnshiftedColumnCmd)
  {
      G4int iusc = fCrystalZShiftSchemeUnshiftedColumnCmd->GetNewIntValue(newValue);
      if(  iusc>18  ||  iusc<0  ) {
          MGLog(error) << "\"UnshiftedColumn\"="<<iusc<<" is an invalid parameter value. Choose value between 0 and 18." << endlog;
          abort();
      }
      fDetectorDB -> SetZShiftScheme_UnshiftedColumn(iusc);
  }
  else if (cmd == fFlagCrystalMatrixWithZPosFileCmd)
    {
      G4bool flag = fFlagCrystalMatrixWithZPosFileCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagCrystalMatrixWithZPosFile(flag);
    }
  // calibration source

  else if (cmd == fCalibrationSourcePositionZ)
    {
      G4double z = fCalibrationSourcePositionZ -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionZ(z);
    }
  else if (cmd == fCalibrationSourcePositionX_S1)
    {
      G4double x = fCalibrationSourcePositionX_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionX_S1(x);
    }
  else if (cmd == fCalibrationSourcePositionX_S2)
    {
      G4double x = fCalibrationSourcePositionX_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionX_S2(x);
    }
  else if (cmd == fCalibrationSourcePositionX_S3)
    {
      G4double x = fCalibrationSourcePositionX_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionX_S3(x);
    }
  else if (cmd == fCalibrationSourcePositionY_S1)
    {
      G4double y = fCalibrationSourcePositionY_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionY_S1(y);
    }
  else if (cmd == fCalibrationSourcePositionY_S2)
    {
      G4double y = fCalibrationSourcePositionY_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionY_S2(y);
    }
  else if (cmd == fCalibrationSourcePositionY_S3)
    {
      G4double y = fCalibrationSourcePositionY_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionY_S3(y);
    }
  else if (cmd == fCalibrationSourcePositionZ_S1)
    {
      G4double z = fCalibrationSourcePositionZ_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionZ_S1(z);
    }
  else if (cmd == fCalibrationSourcePositionZ_S2)
    {
      G4double z = fCalibrationSourcePositionZ_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionZ_S2(z);
    }
  else if (cmd == fCalibrationSourcePositionZ_S3)
    {
      G4double z = fCalibrationSourcePositionZ_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCalibrationSourcePositionZ_S3(z);
    }
  else if (cmd == fCalibrationSourceCuCap)
    {
      G4bool flag = fCalibrationSourceCuCap -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCalibrationSourceCuCap(flag);
    }

  // collimated calibration source

  else if (cmd == fCollimatedCalibrationSourcePositionX_S1)
    {
      G4double x = fCollimatedCalibrationSourcePositionX_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionX_S1(x);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionX_S2)
    {
      G4double x = fCollimatedCalibrationSourcePositionX_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionX_S2(x);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionX_S3)
    {
      G4double x = fCollimatedCalibrationSourcePositionX_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionX_S3(x);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionY_S1)
    {
      G4double y = fCollimatedCalibrationSourcePositionY_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionY_S1(y);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionY_S2)
    {
      G4double y = fCollimatedCalibrationSourcePositionY_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionY_S2(y);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionY_S3)
    {
      G4double y = fCollimatedCalibrationSourcePositionY_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionY_S3(y);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionZ_S1)
    {
      G4double z = fCollimatedCalibrationSourcePositionZ_S1 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionZ_S1(z);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionZ_S2)
    {
      G4double z = fCollimatedCalibrationSourcePositionZ_S2 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionZ_S2(z);
    }
  else if (cmd == fCollimatedCalibrationSourcePositionZ_S3)
    {
      G4double z = fCollimatedCalibrationSourcePositionZ_S3 -> GetNewDoubleValue(newValue);

      fDetectorDB -> SetCollimatedCalibrationSourcePositionZ_S3(z);
    }

  // select PMT distribution

  else if (cmd == fSelectPMTDistribution)
    {
      G4int dist_number = fSelectPMTDistribution -> GetNewIntValue(newValue);

      fDetectorDB -> SetSelectPMTDistribution(dist_number);
    }


  // lock

  else if (cmd == fLockTypeCmd)
    {
      G4String strLockType = newValue;

      if(     strLockType.compare( "final" )==0 )
	fDetectorDB -> SetLockType( LockType_final );
      else if(strLockType.compare( "CommLock2" )==0 )
	fDetectorDB -> SetLockType( LockType_CommLock2 );
      else if(strLockType.compare( "CommLock4" )==0 )
	fDetectorDB -> SetLockType( LockType_CommLock4 );
      else if(strLockType.compare( "PhaseIILock" )==0 )
    	  fDetectorDB -> SetLockType( LockType_PhaseII );
      else if(strLockType.compare( "PhaseII_DN500" )==0 )
    	  fDetectorDB -> SetLockType( LockType_PhaseII_DN500 );
     else {
	MGLog(error) << "\""<<strLockType<<"\" is an invalid parameter value." << endlog;
	abort();
      }

    }
  // DN500 lock options
  else if (cmd == fLockNStringsCmd)
  {
	  G4int n_str = fLockNStringsCmd->GetNewIntValue(newValue);
	  fDetectorDB->SetDN500NStrings(n_str);
  }

  // string distance
  else if (cmd == fLockDistanceStringsCmd)
  {
      G4double dist =  fLockDistanceStringsCmd-> GetNewDoubleValue(newValue);
      fDetectorDB->SetDN500StringDistance(dist);

  }

  // LAr instrumentation design
  else if (cmd == fLArInstDesignCmd)
  {
	  G4String strDesign = newValue;
	  if ( strDesign.compare( "test" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationDesign( LArInstr_test );
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
  	  }
  	  else if ( strDesign.compare( "PMT" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationDesign( LArInstr_PMT );
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
  	  }
	  else if ( strDesign.compare( "fibers" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationDesign( LArInstr_fibers );
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
  	  }
	  else if (strDesign.compare( "hybrid" ) == 0) {
		  fDetectorDB->SetLArInstrumentationDesign(LArInstr_hybrid);
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
	  }
	  else if (strDesign.compare( "APD" ) == 0) {
		  fDetectorDB->SetLArInstrumentationDesign( LArInstr_APD );
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
	  }
	  else if (strDesign.compare( "super-hybrid" ) == 0) {
		  fDetectorDB->SetLArInstrumentationDesign( LArInstr_hybridAPD );
		  MGLog(trace) << "LAGerBier design \"" << strDesign << "\" selected." << endlog;
	  }
  	  else {
  		  MGLog(error) << "\""<< strDesign <<"\" is not a valid LAr design." << endlog;
  		  abort();
  	  }
  }
  else if (cmd == fLArInstAPDDesignCmd)
  {
	  G4String strAPDDesign = newValue;
  	  if ( strAPDDesign.compare( "APD_VUV" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationAPDDesign( LArInstAPDDesign_VUV );
		  MGLog(trace) << "LAGerBier APD design \"" << strAPDDesign << "\" selected." << endlog;
  	  }
  	  else if ( strAPDDesign.compare( "APD_nonVUV" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationAPDDesign( LArInstAPDDesign_nonVUV );
		  MGLog(trace) << "LAGerBier APD design \"" << strAPDDesign << "\" selected." << endlog;
  	  }
  	  else if ( strAPDDesign.compare( "SiPM" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationAPDDesign( LArInstAPDDesign_SiPM );
		  MGLog(trace) << "LAGerBier APD design \"" << strAPDDesign << "\" selected." << endlog;
  	  }
  	  else {
  		  MGLog(error) << "\""<< strAPDDesign <<"\" is not a valid APD design." << endlog;
  		  abort();
  	  }
  }
  else if (cmd == fLArInstAPDDeploymentCmd)
  {
	  G4String strAPDDeployment = newValue;
  	  if ( strAPDDeployment.compare( "betweenBEGePairs" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationAPDDeployment( LArInstAPDDeployment_betweenBEGePairs );
		  MGLog(trace) << "LAGerBier APD design \"" << strAPDDeployment << "\" selected." << endlog;
  	  }
  	  else if ( strAPDDeployment.compare( "betweenBEGes" ) == 0) {
		  fDetectorDB -> SetLArInstrumentationAPDDeployment( LArInstAPDDeployment_betweenBEGes );
		  MGLog(trace) << "LAGerBier APD design \"" << strAPDDeployment << "\" selected." << endlog;
  	  }
  	  else {
  		  MGLog(error) << "\""<< strAPDDeployment <<"\" is not a valid APD deployment." << endlog;
  		  abort();
  	  }
  }
  else if (cmd == fLArInstAPDMSCoatingCmd)
  {
	  G4String strMSCoating = newValue;
	  if ( strMSCoating.compare( "none" ) == 0 ) {
		  fDetectorDB -> SetLArInstrumentationMSCoating( LArInstMSCoating_none );
		  MGLog(trace) << "LAGerBier MS coating \"" << strMSCoating << "\" selected." << endlog;
	  }
	  else if ( strMSCoating.compare( "VM2000_TPB" ) == 0 ) {
		  fDetectorDB -> SetLArInstrumentationMSCoating( LArInstMSCoating_VM2000_TPB );
		  MGLog(trace) << "LAGerBier MS coating \"" << strMSCoating << "\" selected." << endlog;
	  }
	  else if ( strMSCoating.compare( "TPB" ) == 0 ) {
		  fDetectorDB -> SetLArInstrumentationMSCoating( LArInstMSCoating_TPB );
		  MGLog(trace) << "LAGerBier MS coating \"" << strMSCoating << "\" selected." << endlog;
	  }
	  else {
		  MGLog(error) << "\""<< strMSCoating <<"\" is not a valid Mini Shroud coating." << endlog;
		  abort();
	  }
  }
  else if (cmd == fLArInstPhotTrackingCmd)
  {
  G4bool flag = fLArInstPhotTrackingCmd->GetNewBoolValue(newValue);
  fDetectorDB->SetLArInstrumentationPhotTrack(flag);
  MGLog(trace) << "Photon tracking set to " << fDetectorDB->GetLArInstrumentationPhotTrack() << endlog;
  }
  // BEGe pair gap
  else if (cmd == fCrystalBEGePairGapCmd) {
    fDetectorDB->SetCrystalPairGap(fCrystalBEGePairGapCmd->GetNewDoubleValue(newValue));
    MGLog(trace) << "BEGe crystal gap set to " << fDetectorDB->GetCrystalPairGap()/mm << endlog;
  }
  // Reverse non true coaxial detectors
  else if (cmd == fCrystalReverseNonTrueCoaxialCmd ) {
    G4bool flag = fCrystalReverseNonTrueCoaxialCmd -> GetNewBoolValue(newValue);
    fDetectorDB->SetFlagReverseNonTrueCoaxial(flag);
	  MGLog(debugging) << "Non true coaxial detectors reversed to match real orientation " << endlog;
  }

  else if (cmd == fFlagConstructCalibrationSourceCmd)
    {
      G4bool flag = fFlagConstructCalibrationSourceCmd -> GetNewBoolValue(newValue);

      fDetectorDB -> SetFlagConstructCalibrationSource(flag);
    }

  else if ( cmd == fLarInstConstructInnerFiberCmd ) {
	  G4bool flag = fLarInstConstructInnerFiberCmd->GetNewBoolValue(newValue);
	  fDetectorDB->SetLArInstHybridInnerFiber(flag);
	  MGLog(debugging) << "Construction of inner fibers in LAr Instrumentation hybrid design set to " << newValue << endlog;
  }
  else if ( cmd == fLarInstConstructExtSourceCmd ) {
    G4bool flag = fLarInstConstructExtSourceCmd->GetNewBoolValue(newValue);
    fDetectorDB->SetLArInstConstructExtSource(flag);
    MGLog(debugging) << "Construction of external source in LAr Instrumentation set to " << newValue << endlog;
  }
  else if ( cmd == fLArInstScaleLArAbsLCmd ) {
    G4double flag = fLArInstScaleLArAbsLCmd->GetNewDoubleValue(newValue);
    fDetectorDB->SetLArInstArgonAbsLScale(flag);
    MGLog(debugging) << "Scaling factor for LAr XUV absorption length set to " << flag << endlog;

  }
  else if ( cmd == fLArInstScaleLArLYCmd ) {
    G4double flag = fLArInstScaleLArLYCmd->GetNewDoubleValue(newValue);
    fDetectorDB->SetLArInstArgonLYScale(flag);
    MGLog(debugging) << "Scaling factor for LAr XUV light yield set to " << flag << endlog;
  }
  else if ( cmd == fLArInstFiberCoverageCmd ) {
    G4double flag = fLArInstFiberCoverageCmd->GetNewDoubleValue(newValue);
    fDetectorDB->SetLArInstFiberCoverage(flag);
    MGLog(debugging) << "Set coverage fraction of fiber shroud" << flag << endlog;
  }
  else if ( cmd == fLArInstTopPMTscaleFactCmd ) {
    G4double flag = fLArInstTopPMTscaleFactCmd->GetNewDoubleValue(newValue);
    fDetectorDB->SetLArInstTopPMTscaleFact(flag);
    MGLog(debugging) << "Scale down top PMT efficiency by " << flag << endlog;
  }
  else if ( cmd == fLArInstScaleMetalReflCmd ) {
    G4double flag = fLArInstScaleMetalReflCmd->GetNewDoubleValue(newValue);
    fDetectorDB->SetLArInstMetalsReflScale(flag);
    MGLog(debugging) << "Scaling factor for reflectivity of metal components in LAr Instrumentation set to " << flag << endlog;
  }
  else if ( cmd == fLArInstConstructSegmentedFiberShroudCmd ) {
	  G4bool flag = fLArInstConstructSegmentedFiberShroudCmd->GetNewBoolValue(newValue);
	  fDetectorDB->SetLArInstHybridSegmentedFiber(flag);
	  MGLog(debugging) << "Construction of segmented fibers in LAr Instrumentation hybrid design set to " << newValue << endlog;
  }
  else if ( cmd == fLArInstSegmentedFiberShroudRotationCmd ) {
	  G4double flag = fLArInstSegmentedFiberShroudRotationCmd->GetNewDoubleValue(newValue);
	  fDetectorDB->SetLArInstHybridSegmentedFiberShroudRotation(flag);
	  MGLog(debugging) << "Rotation of segmented fiber shroud in LAr Instrumentation hybrid design set to " << newValue << endlog;
  }
}

// ---------------------------------------------------------------------------
