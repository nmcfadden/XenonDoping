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
 *      
 * CLASS DECLARATION:  MJGeometryKURFBEGeDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: P. Finnerty
 * CONTACT: paddy@physics.unc.edu 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 03-03-2010 A. Schubert - added some parts according to CANBERRA drawings and
 *            drawings from Juan Collar.
 * 03-10-2010 A. Schubert - added small parts by digitizing p. 1 of CANBERRA
 *            8089-3 drawing.
 * 04-07-2010 A. Schubert - added more parts per email from Juan
 * 05-02-2010 A. Schubert - added solder spots
 * 05-03-2010 A. Schubert - added approximation of the zeolite package,
 *            corrected error from previous revision on 02 May -- fCylinderAngle
 *            was set to pi instead of twopi
 * 08-27-2010 A. Schubert - Changed Get and Set methods for fCylinderAngle so
 *            they are in units of pi, added messenger.
 * 08-31-2011, Added set/get methods for collimator positioning, P. Finnerty
 * 10 Nov 2011 A. Schubert - modified dimensions to match measurments from
 *             MALBEK Lead Shim Removal document by Finnerty, Giovanetti,
 *             Henning, and Wilkerson.  Some existing dimensions match the
 *             measurements; noted these for clarity.
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYKURFBEGeDETECTOR_HH
#define _MJGEOMETRYKURFBEGeDETECTOR_HH

//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//
class G4Material;
class MGGeometryIdealCoaxCrystal;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class MJGeometryKURFBEGeDetectorMessenger;

class MJGeometryKURFBEGeDetector : public MGGeometryDetector
{
public:

  MJGeometryKURFBEGeDetector(G4String serNum, G4bool messengerOff);
  ~MJGeometryKURFBEGeDetector();

  void ConstructDetector();
  void SetDefaults();
  
  // get or set fCylinderAngle -- should be 2*pi for monte-carlo results, can be
  // 1*pi for visualization
  void SetCylinderAngleInUnitsOfPi(double multiplesOfPi) {fCylinderAngle = multiplesOfPi*CLHEP::pi;}
  G4double GetCylinderAngleInUnitsOfPi(){ return fCylinderAngle/CLHEP::pi; }

  // get or set fUseCollimator, this is FALSE by default.  
  void SetIsCollimatorUsed(G4bool val) {fUseCollimator = val;}
  G4bool GetIsCollimatorUsed(){ return fUseCollimator; }

  void SetArePbPatchesUsed(G4bool val) { fUsePbPatches = val; }
  G4bool GetArePbPatchesUsed() const { return fUsePbPatches; }
  
  // get or set fCollimatorSideOrTop, this is "top" by default
  void SetCollimatorSideOrTop(G4String val) {fCollimatorSideOrTop = val;}
  G4String GetCollimatorSideOrTop(){ return fCollimatorSideOrTop; }
  
  // get or set fCollimatorXCoordinateIn_mm 
  void SetCollimatorXCoordinateIn_mm(G4double val) {fCollimatorXCoordinateIn_mm = val*CLHEP::mm;}
  G4double GetCollimatorXCoordinateIn_mm(){ return fCollimatorXCoordinateIn_mm/CLHEP::mm; }
  
  // get or set fCollimatorXCoordinateIn_mm 
  void SetCollimatorYCoordinateIn_mm(G4double val) {fCollimatorYCoordinateIn_mm = val*CLHEP::mm;}
  G4double GetCollimatorYCoordinateIn_mm(){ return fCollimatorYCoordinateIn_mm/CLHEP::mm; }
  
  // get or set fCollimatorXCoordinateIn_mm 
  void SetCollimatorZCoordinateIn_mm(G4double val) {fCollimatorZCoordinateIn_mm = val*CLHEP::mm;}
  G4double GetCollimatorZCoordinateIn_mm(){ return fCollimatorZCoordinateIn_mm/CLHEP::mm; }

  // get or set fCollimatorRCoordinateIn_mm 
  void SetCollimatorRCoordinateIn_mm(G4double val) {fCollimatorRCoordinateIn_mm = val*CLHEP::mm;}
  G4double GetCollimatorRCoordinateIn_mm(){ return fCollimatorRCoordinateIn_mm/CLHEP::mm; }

  // get or set fCollimatorAngleIn_deg 
  void SetCollimatorAngleIn_deg(G4double val) {fCollimatorAngleIn_deg = val*CLHEP::degree;}
  G4double GetCollimatorAngleIn_deg(){ return fCollimatorAngleIn_deg/CLHEP::degree; }

  G4double GetZCoordinateOfTopOfCryostat(){ return fTopOfCryostatZ; }
  G4double GetZCoordinateOfTopOfStandTopPlate(){ return fTopOfStandTopPlateZ; }

  bool UsesMJPSQLDatabase() { return true; }  

protected:

  // messenger
  MJGeometryKURFBEGeDetectorMessenger *fG4Messenger;

  // delta angle for cylinders -- typically 2*pi, can be set to 1*pi for
  // visualiztion of cyostat interior.
  G4double fCylinderAngle;  
  G4bool fUseCollimator;
  G4bool fUsePbPatches;
  G4String fCollimatorSideOrTop;
  G4double fCollimatorXCoordinateIn_mm;
  G4double fCollimatorYCoordinateIn_mm;
  G4double fCollimatorZCoordinateIn_mm;
  G4double fCollimatorRCoordinateIn_mm;
  G4double fCollimatorAngleIn_deg;

  // z-coordinate of the top of the cryostat -- useful info when placing this
  // geometry in another.  The geometry is centered about 0 in x and y.
  G4double fTopOfCryostatZ;

  // z-coordiante of top of the stand top plate -- useful when placing this
  // geometry in another.  
  G4double fTopOfStandTopPlateZ;

  // logical volumes:
  G4LogicalVolume* fEndCapLogical;
  G4LogicalVolume* fIRWindowLogical;
  G4LogicalVolume* fTeflonCupLogical;
  G4LogicalVolume* fTeflonDiskLogical;
  G4LogicalVolume* fCopperCupLogical;
  G4LogicalVolume* fCopperLidLogical;
  G4LogicalVolume* fColdFingerLogical;
  G4LogicalVolume* fRdcColdFingerLogical;
  G4LogicalVolume* fCryostatLogical;
  G4LogicalVolume* fEBLogical;
  G4LogicalVolume* fSpiderTrestleLogical;
  G4LogicalVolume* fStandTopPlateLogical;
  G4LogicalVolume* fDetectorSpacerLogical;
  G4LogicalVolume* fThreadedCollarLogical;
  G4LogicalVolume* fFetBlockLogical;
  G4LogicalVolume* fHVPogoBarrelLogical;
  G4LogicalVolume* fHVPogoPlungerLogical;
  G4LogicalVolume* fHVContactExtensionLogical;
  G4LogicalVolume* fPolySpiderLogical;
  G4LogicalVolume* fPentaPlugLogical;
  G4LogicalVolume* fPentaReceptacleLogical;
  G4LogicalVolume* fMillMaxReceptacleLogical;
  G4LogicalVolume* fSupportPostLogical;
  G4LogicalVolume* fCompressionSpringLogical;
  G4LogicalVolume* fDHPinLogical;
  G4LogicalVolume* fDHPinInReceptacleLogical;
  G4LogicalVolume* fFETLogical;
  G4LogicalVolume* fResistorLogical;
  G4LogicalVolume* fPbPatchOneLogical;
  G4LogicalVolume* fPbPatchTwoLogical;
  G4LogicalVolume* fConnectorInsulatorLogical;


  // physical volumes:
  G4VPhysicalVolume* fEndCapPhysical;
  G4VPhysicalVolume* fIRWindowPhysical;
  G4VPhysicalVolume* fCrystalPhysical;
  G4VPhysicalVolume* fTeflonCupPhysical;
  G4VPhysicalVolume* fTeflonDiskPhysical;
  G4VPhysicalVolume* fCopperCupPhysical;
  G4VPhysicalVolume* fCopperLidPhysical;
  G4VPhysicalVolume* fColdFingerPhysical;
  G4VPhysicalVolume* fRdcColdFingerPhysical;
  G4VPhysicalVolume* fCryostatPhysical;
  G4VPhysicalVolume* fEBPhysical;
  G4VPhysicalVolume* fSpiderTrestlePhysical;
  G4VPhysicalVolume* fStandTopPlatePhysical;
  G4VPhysicalVolume* fDetectorSpacerPhysical;
  G4VPhysicalVolume* fThreadedCollarPhysical;
  G4VPhysicalVolume* fFetBlockPhysical;
  G4VPhysicalVolume* fHVPogoBarrelPhysical;
  G4VPhysicalVolume* fHVPogoPlungerPhysical;
  G4VPhysicalVolume* fHVContactExtensionPhysical;
  G4VPhysicalVolume* fPolySpiderPhysical;
  G4VPhysicalVolume* fPentaPlugPhysical;
  G4VPhysicalVolume* fPentaReceptaclePhysical;
  G4VPhysicalVolume* fMillMaxReceptaclePhysical;
  G4VPhysicalVolume* fSupportPostPhysical;
  G4VPhysicalVolume* fCompressionSpringPhysical;
  G4VPhysicalVolume* fDHPinPhysical;
  G4VPhysicalVolume* fDHPinInReceptaclePhysical;
  G4VPhysicalVolume* fFETPhysical;
  G4VPhysicalVolume* fHeaterResistorPhysical;
  G4VPhysicalVolume* fInjectorResistorPhysical;
  G4VPhysicalVolume* fPbPatchOnePhysical;
  G4VPhysicalVolume* fPbPatchTwoPhysical;
  G4VPhysicalVolume* fConnectorInsulatorPhysical;

  // end cap:
  G4double fEndCapHeight;
  G4double fEndCapInnerRadius;
  G4double fEndCapOuterRadius;
  G4double fEndCapFlatThickness;


  // detector:
  G4double fCrystalOffset;
  G4double fCrystalRadius;
  G4double fCrystalHeight;
  G4double fDitchRadius;
  G4double fDitchWidth;
  G4double fDitchDepth;

  // teflon cup:
  G4double fTeflonCupOuterRadius;
  G4double fTeflonCupInnerRadius;
  G4double fTeflonCupHeight;
  G4double fTeflonCupBottomThickness;
  G4double fTeflonCupEndCylinderRadius;
  G4double fTeflonCupEndCylinderHeight;
  G4double fTeflonCupHoleRadius;

  G4double fTeflonDiskThickness;

  // copper cup:
  G4double fCopperCupOuterRadius;
  G4double fCopperCupInnerRadius;
  G4double fCopperCupHeight;
  G4double fCopperCupHoleRadius;
  G4double fCopperCupBottomThickness;
  G4double fCopperCupSetScrewFeatureOuterRadius;
  G4double fCopperCupSetScrewFeatureHeight;
  G4double fCopperCupSetScrewFeatureOffsetFromTop;
  G4double fCopperCupBottomAnnulusHeight;
  G4double fCopperCupBottomAnnulusInnerRadius;
  G4double fCopperCupBottomAnnulusOuterRadius;
  G4double fCopperCupContactHoleRadius;
  G4double fCopperCupContactHoleDistance;

  // copper cap/lid
  G4double fCopperLidInnerRadius;
  G4double fCopperLidOuterRadius;
  G4double fCopperLidHeight;
  G4double fCopperLidFlatBaseThickness;

  // cold finger nearest detector
  G4double fRdcColdFingerRadius;
  G4double fRdcColdFingerLength;
  G4double fRdcColdFingerCutRadius;
  G4double fRdcColdFingerCutLength;
 
  // electronics base
  G4double fEBHeight;
  G4double fEBInnerRadius;
  G4double fEBOuterRadius;
  G4double fEBFlatTopThickness;
  G4double fEBHoleRadius;
  G4double fEBMillMaxHoleDistance;

  // cryostat
  G4double fCryostatOringFlangeOuterRadius;
  G4double fCryostatOringFlangeThickness;
  G4double fCryostatOringFlangeCutRadius;
  G4double fCryostatOringFlangeCutHeight;
  G4double fCryostatShortNeckLength;
  G4double fCryostatNeckOuterRadius;
  G4double fCryostatNeckThickness;
  G4double fCryostatFlangeOuterRadius;
  G4double fCryostatFlangeInnerRadius;
  G4double fCryostatFlangeHeight;
  G4double fCryostatFlangeTopThickness;

  // spider trestle
  G4double fSpiderTrestleInnerRadius;
  G4double fSpiderTrestleOuterRadius;
  G4double fSpiderTrestleHeight;
  G4double fSpiderTrestleSmallHoleRadius;
  G4double fSpiderTrestleLargeHoleRadius;
  G4double fSpiderTrestleLargeHoleDistance;

  // stand top plate
  G4double fStandTopPlateLength;
  G4double fStandTopPlateThickness;
  G4double fStandTopPlateHoleRadius;
  G4double fStandTopPlateOffset;

  // detector spacer
  G4double fDetectorSpacerThickness;
  G4double fDetectorSpacerOuterRadius;
  G4double fDetectorSpacerInnerRadius;

  // threaded collar
  G4double fThreadedCollarInnerRadius;
  G4double fThreadedCollarOuterRadius;
  G4double fThreadedCollarHeight;

  // fet block -- cylinder that holds FET
  G4double fFetBlockOuterRadius;
  G4double fFetBlockInnerRadius;
  G4double fFetBlockHeight;
  G4double fFetBlockBottomThickness;

  // HV probe -- pogo to make contact
  G4double fHVPogoPlungerRadius;
  G4double fHVPogoPlungerHeight;
  G4double fHVPogoBarrelHeight;
  G4double fHVPogoBarrelOuterRadius;
  G4double fHVPogoBarrelInnerRadius;
  G4double fHVPogoReceptacleOuterRadius;
  G4double fHVPogoReceptacleLength;
  G4double fHVPogoReceptacleOffset;

  // HV contact extension
  G4double fHVContactExtensionOuterRadius;
  G4double fHVContactExtensionInnerRadius;
  G4double fHVContactExtensionLength;
  G4double fHVContactExtensionStepLength;
  G4double fHVContactExtensionStepRadius;

  // poly spider
  G4double fPolySpiderOuterRadius;
  G4double fPolySpiderInnerRadius;
  G4double fPolySpiderHeight;

  // penta assembly -- encloses fet
  G4double fPentaPlugHeight;
  G4double fPentaPlugStepHeight;
  G4double fPentaPlugSmallOuterRadius;
  G4double fPentaPlugOuterRadius;
  G4double fPentaPlugInnerRadius;
  G4double fPentaPlugHoleRadius;
  G4double fPentaPlugHoleSpacing;
  G4double fPentaReceptacleHeight;
  G4double fPentaReceptacleSmallOuterRadius;
  G4double fPentaReceptacleMiddleLength;
  G4double fPentaReceptacleOuterRadius;
  G4double fPentaPlugSpacing;

  // mill max receptacle
  G4double fMillMaxReceptacleTopStepRadius;
  G4double fMillMaxReceptacleTopStepLength;
  G4double fMillMaxReceptacleMiddleStepRadius;
  G4double fMillMaxReceptacleMiddleStepLength;
  G4double fMillMaxReceptacleTailRadius;
  G4double fMillMaxReceptacleInnerRadius;
  G4double fMillMaxReceptacleInnerHoleDepth;
  G4double fMillMaxReceptacleLength;

  // mill max double-headed pin 0290
  G4double fDHPinShoulderRadius;
  G4double fDHPinShoulderHeight;
  G4double fDHPinPressFitOuterRadius;
  G4double fDHPinPressFitLength;
  G4double fDHPinTopTailRadius;
  G4double fDHPinTopTailLength;
  G4double fDHPinBottomTailRadius;
  G4double fDHPinBottomTailLength;

  // support post 
  G4double fSupportPostHeight;
  G4double fSupportPostOuterRadius;
  G4double fSupportPostInnerRadius;
  G4double fSupportPostSmallOuterRadius;
  G4double fSupportPostStepHeight;

  // compression spring
  G4double fCompressionSpringHeight;
  G4double fCompressionSpringOuterRadius;
  G4double fCompressionSpringInnerRadius;

  // FET
  G4double fFETLength;
  G4double fFETWidth;

  // resistor
  G4double fResistorRadius;
  G4double fResistorLength;
  G4double fResistorOffset;
  
  // pb patches
  G4double fPbPatchThickness;
  G4double fPbPatchOneWidth;
  G4double fPbPatchOneHeight;
  G4double fPbPatchTwoWidth;
  G4double fPbPatchTwoHeight;


  // teflon insulators around electronics base
  G4double fConnectorInsulatorOuterRadius;
  G4double fConnectorInsulatorInnerRadius;
  G4double fConnectorInsulatorSmallOuterRadius;
  G4double fConnectorInsulatorLength;
  G4double fConnectorInsulatorStepLength;

  // solder on center contact
  G4double fCenterContactSolderHeight;
  G4double fCenterContactSolderWidth;

  // solder on hv contact
  G4double fHVSolderHeight;
  G4double fHVSolderRadius;

  // solder blobsaround FET
  G4double fSolderRadius;

  // zeolite package
  G4double fZeoliteWidth;

};
#endif
