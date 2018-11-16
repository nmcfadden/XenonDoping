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
 * CLASS DECLARATION: GEGeometryDetectorDBMessenger.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   01-23-2005, added commands for segments, Kevin
 *   02-07-2005, added commands for calibration source, Kevin
 *   02-16-2005, added commands for the position and rotation of the mu-veto, Luciano&Claudia
 *   03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 *   07-05-2005  Added flag for Phase I/II holder, Luciano
 *   07-05-2005, Added flag for plastic scintillator hole (for the neck)
 *               and command to set the radius of the hole, Claudia
 *   03-06-2006, Added command to select PMT distribution
 *   08-05-2008, Added commands for setting two different schemes for the z positions of the crystal strings
 *   04-24-2009  Added command for lock type (final or comm), Francis
 *   14-06-2011  Added command to construct copper encapsulation for phase 0
 *               Added command to construct teflon encapsulation for phase 0
 *               Added command to construct both encapsulations for phase 0 at same time, Sabine
 * 26-03-2012 Added command to construct mini-shrouds, Giovanni Benato
 * 27-03-2012 - Added possibility to move each calibration source separately. Giovanni Benato
 * 22-05-2012 - Added possibility to move each calibration source separately in all 3 directions. Giovanni Benato
 * 25-05-2012, Added command to customize the distance between a pair of BEGe's, Nuno
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real confituration in GERDA (Giovanni)
 * 18-07-2012 - Added two additional commands for the new phase II lock, named dn500., Nuno Barros
 * 14-08-2012 - Added options for the LAr hybrid design., Nuno Barros
 * 16-07-2014 - Added MS control options, Bjoern Lehnert
 * 04-10-2016 - Added command for individual placement of detectors along z axis, Janina
 * 12-05-2016, JJ added macro command to turn off photon tracking
 * 28-07-2016, JJ added macro command to set LAr light yield and fiber coverage (Bjoern's code)
 * 21-11-2017, Added macro commands for MS glued ring heights, KvSturm
 */

#ifndef _GEGEOMETRYDETECTORDBMESSENGER_HH
#define _GEGEOMETRYDETECTORDBMESSENGER_HH

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"

// ---------------------------------------------------------------------------

class GEGeometryDetectorDB;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class GEGeometryDetectorDBMessenger : public G4UImessenger
{

public:

  /// Default constructor
  GEGeometryDetectorDBMessenger(GEGeometryDetectorDB* DetectorDB);

  /// Destructor
  ~GEGeometryDetectorDBMessenger();

  // methods

   void SetNewValue(G4UIcommand *cmd, G4String newValue);

private:

  GEGeometryDetectorDB* fDetectorDB;

  G4UIdirectory* fGeneralDir;
  G4UIdirectory* fShieldingDir;
  G4UIdirectory* fDetectorGeneralDir;
  G4UIdirectory* fDetectorScintillatorDir;
  G4UIdirectory* fDetectorCrystalDir;
  G4UIdirectory* fDetectorSupportDir;
  G4UIdirectory* fDetectorCalibrationDir;
  G4UIdirectory* fDetectorCollimatedCalibrationDir;
  G4UIdirectory* fDetectorCherenkovDir;
  G4UIdirectory* fLockDir;
  G4UIdirectory* fLockDN500Dir;
  G4UIdirectory* fLArInstrDir;

  G4UIcmdWithABool* fFlagConstructDetectorCmd;
  G4UIcmdWithABool* fFlagConstructCrystalsCmd;
  G4UIcmdWithABool* fFlagConstructSupportCmd;
  G4UIcmdWithABool* fFlagConstructCablingCmd;
  G4UIcmdWithABool* fFlagConstructShieldingCmd;
  G4UIcmdWithABool* fFlagConstructCalibrationSourceCmd;
  G4UIcmdWithABool* fFlagConstructCollimatedCalibrationSourceCmd;
  G4UIcmdWithABool* fFlagConstructCopperEncapsulationCmd;
  G4UIcmdWithABool* fFlagConstructTeflonEncapsulationCmd;
  G4UIcmdWithABool* fFlagConstructEncapsulationsCmd;
  G4UIcmdWithABool* fFlagConstructMiniShroudCmd;
  G4UIcmdWithABool* fFlagMinishroudToDetctorString;

  G4UIcmdWithAString* fMiniShroudMaterialCmd;
  G4UIcmdWithADoubleAndUnit* fMiniShroudBaseThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fMiniShroudSideThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fMiniShroudTopGluedRingHeightCmd;
  G4UIcmdWithADoubleAndUnit* fMiniShroudBottomGluedRingHeightCmd;

  G4UIcmdWithABool* fFlagCryoLiquidCmd;
  // LAr instrumentation. Only valid if we also have CryoLiquid active
  G4UIcmdWithABool* fFlagConstructLArInstrCmd;

  /// Array
  G4UIcmdWithADoubleAndUnit* fArrayDepthCmd;

  /// Cable chain
  G4UIcmdWithADoubleAndUnit* fCableChainDistanceCmd;


  // Scintillator

  G4UIcmdWithABool*          fScintillatorHoleCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorHeightCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorLengthXCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorLengthYCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorPosXCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorPosYCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorPosZCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorAngleZCmd;
  G4UIcmdWithADoubleAndUnit* fScintillatorHoleOuterRadiusCmd;

  // crystals

  G4UIcmdWithAString*        fCrystalGeometryFileCmd;
  G4UIcmdWithAString*        fCrystalMatrixFileCmd;
  G4UIcmdWithAString*        fCrystalZShiftSchemeCmd;
  G4UIcmdWithAnInteger*      fCrystalZShiftSchemeUnshiftedColumnCmd;
  G4UIcmdWithADoubleAndUnit* fCrystalBEGePairGapCmd;
  G4UIcmdWithABool*        fCrystalReverseNonTrueCoaxialCmd;
  G4UIcmdWithABool*	   fFlagCrystalMatrixWithZPosFileCmd;

  /// Calibration source
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionZ;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionX_S1;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionX_S2;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionX_S3;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionY_S1;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionY_S2;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionY_S3;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionZ_S1;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionZ_S2;
  G4UIcmdWithADoubleAndUnit* fCalibrationSourcePositionZ_S3;

  G4UIcmdWithABool*          fCalibrationSourceCuCap;

  /// Collimated calibration source
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionZ;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionX_S1;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionX_S2;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionX_S3;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionY_S1;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionY_S2;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionY_S3;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionZ_S1;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionZ_S2;
  G4UIcmdWithADoubleAndUnit* fCollimatedCalibrationSourcePositionZ_S3;

  /// Photomultiplier distribution
  G4UIcmdWithAnInteger* fSelectPMTDistribution;

  /// Lock
  G4UIcmdWithAString* fLockTypeCmd;
  G4UIcmdWithAnInteger* fLockNStringsCmd;
  G4UIcmdWithADoubleAndUnit* fLockDistanceStringsCmd;

  ///LAr instrumentation
  G4UIcmdWithAString*         fLArInstDesignCmd;
  G4UIcmdWithABool*           fLarInstConstructInnerFiberCmd;
  G4UIcmdWithABool*           fLarInstConstructExtSourceCmd;
  G4UIcmdWithADouble*         fLArInstScaleLArAbsLCmd;
  G4UIcmdWithADouble*         fLArInstScaleLArLYCmd;
  G4UIcmdWithADouble*         fLArInstFiberCoverageCmd;
  G4UIcmdWithADouble*         fLArInstTopPMTscaleFactCmd;
  G4UIcmdWithADouble*         fLArInstScaleMetalReflCmd;
  G4UIcmdWithAString*         fLArInstAPDDesignCmd;
  G4UIcmdWithAString*         fLArInstAPDDeploymentCmd;
  G4UIcmdWithAString*         fLArInstAPDMSCoatingCmd;
  G4UIcmdWithABool*           fLArInstPhotTrackingCmd;
  G4UIcmdWithABool*           fLArInstConstructSegmentedFiberShroudCmd;
  G4UIcmdWithADoubleAndUnit*  fLArInstSegmentedFiberShroudRotationCmd;

};

#endif
