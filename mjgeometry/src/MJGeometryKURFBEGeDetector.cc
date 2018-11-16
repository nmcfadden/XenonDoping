//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJGeometryKURFBEGeDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:  This class describes the MALBEK detector
 */
//---------------------------------------------------------------------------//
/**
 * AUTHOR: P. Finnerty
 * CONTACT: P. Finnerty paddy@physics.unc.edu
 * FIRST SUBMISSION: 03-02-2009 Kevin Macon
 * 
 * REVISION: 03-04-2009 K. Macon - corrected crystal dimensions
 *           05-15-2009 P. Finnerty - Updated dimensions for cryostat and
 *           coldfinger.  NOTE:  Before this, the cryostatheight was WAY off.
 *           01-28-2010 A. Schubert - Now using MGGeometryBEGeDetector as
 *           crystal, also changed crystal offset to +z direction.
 *           03-03-2010 A. Schubert - added teflon cup and disk, copper cup and
 *           cap, electronics base, endcap.  Modified cryostat. 
 *           03-10-2010 A. Schubert - added small parts by digitizing p. 
 *           1 of CANBERRA 8089-3 drawing.
 * 04-07-2010 A. Schubert - added more parts per email from Juan
 * 04-18-2010 A. Schubert - lowered top plate to fit cryostat in KURF Pb shield
 * 04-25-2010 A. Schubert - changed Polyethylene materials to Teflon, changed
 *            detector material from enriched to natural Ge, changed xtal
 *            location from 5 mm from endcap inner surface to 5 mm from endcap
 *            outer face..
 * 05-02-2010 A. Schubert - added solder spots
 * 05-03-2010 A. Schubert - added approximation of the zeolite package,
 *            corrected error from previous revision on 02 May -- fCylinderAngle
 *            was set to pi instead of twopi
 * 08-27-2010 A. Schubert - enlarged Pb foil patches according to email from M.
 *            Yocum; changed orientation of patches so they're visible in
 *            visualizations, added messenger
 * 08-28-2010 A. Schubert - fixed Pb foil; there were 4 spots instead of 2 after
 *            last check in.
 * 08-30-2010 A. Schubert - updated compression spring material
 * 10-13-2010 A. Schubert - added -eps to millMaxZ -- surfaces were in contact.
 * 04 Feb 2011 A. Schubert - removed rotation of BEGe crystal by pi since
 *             default ditch location was changed in MGGeometryBEGeDetector
 * 30 Aug 2011 A. Schubert - added envelopeEps for convenience during debugging;
 *             fixed location where cryostat envelope was in contact with
 *             cryostatNeckAndFlange.
 * 08 Sep 2011, Added collimator positioning functionality, P. Finnerty
 * 14 Sep 2011 A. Schubert - changed copper lid thickness from 0.06*inch (1.5 mm) to
 *             0.7*mm per study of source data from KURF.
 * 28 Sep 2011 A. Schubert - changed copper lid thickness from 0.7 mm to 0.020
 *             inch (0.51 mm) per Sept 19 email from Mike Yocum.
 * 10 Nov 2011 A. Schubert - modified dimensions to match measurments from
 *             MALBEK Lead Shim Removal document by Finnerty, Giovanetti,
 *             Henning, and Wilkerson.  Some existing dimensions match the
 *             measurements; noted these for clarity.  Removed second support
 *             post from radial hole in Cu cup; there was only one of these
 *             support posts in the detector.
 */
//---------------------------------------------------------------------------//
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4VSolid.hh"
#include "G4Color.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "TMath.h"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetectorMessenger.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "geometry/MGGeometryBEGeDetector.hh"

using namespace CLHEP;
using namespace MaGe;

//--------------------------------------------------------------------------//
MJGeometryKURFBEGeDetector::MJGeometryKURFBEGeDetector(
  G4String serNum, 
  bool messOff
  ) :
  MGGeometryDetector(serNum), 
  fG4Messenger(NULL),
  fCylinderAngle(twopi),
  fUseCollimator(false),
  fUsePbPatches(true),
  fCollimatorSideOrTop("top"),
  fCollimatorXCoordinateIn_mm(0.0*mm),
  fCollimatorYCoordinateIn_mm(0.0*mm),
  fCollimatorZCoordinateIn_mm(20.0*mm),
  fCollimatorRCoordinateIn_mm(50.0*mm),
  fCollimatorAngleIn_deg(0.0*deg),
  fTopOfCryostatZ(0),
  fTopOfStandTopPlateZ(0),
  // logical volumes:
  fEndCapLogical(NULL), 
  fIRWindowLogical(NULL), 
  fTeflonCupLogical(NULL),
  fTeflonDiskLogical(NULL),
  fCopperCupLogical(NULL),
  fCopperLidLogical(NULL),
  fColdFingerLogical(NULL),
  fRdcColdFingerLogical(NULL),
  fEBLogical(NULL),
  fSpiderTrestleLogical(NULL),
  fStandTopPlateLogical(NULL),
  fDetectorSpacerLogical(NULL),
  fThreadedCollarLogical(NULL),
  fFetBlockLogical(NULL),
  fHVPogoBarrelLogical(NULL),
  fHVPogoPlungerLogical(NULL),
  fHVContactExtensionLogical(NULL),
  fPolySpiderLogical(NULL),
  fPentaPlugLogical(NULL),
  fPentaReceptacleLogical(NULL),
  fMillMaxReceptacleLogical(NULL),
  fSupportPostLogical(NULL),
  fCompressionSpringLogical(NULL),
  fDHPinLogical(NULL),
  fDHPinInReceptacleLogical(NULL),
  fFETLogical(NULL),
  fResistorLogical(NULL),
  fPbPatchOneLogical(NULL),
  fPbPatchTwoLogical(NULL),
  fConnectorInsulatorLogical(NULL),
  // physical volumes:
  fEndCapPhysical(NULL),
  fIRWindowPhysical(NULL), 
  fCrystalPhysical(NULL),
  fTeflonCupPhysical(NULL),
  fTeflonDiskPhysical(NULL),
  fCopperCupPhysical(NULL),
  fCopperLidPhysical(NULL),
  fColdFingerPhysical(NULL),
  fRdcColdFingerPhysical(NULL),
  fEBPhysical(NULL),
  fSpiderTrestlePhysical(NULL),
  fStandTopPlatePhysical(NULL),
  fDetectorSpacerPhysical(NULL),
  fThreadedCollarPhysical(NULL),
  fFetBlockPhysical(NULL),
  fHVPogoBarrelPhysical(NULL),
  fHVPogoPlungerPhysical(NULL),
  fHVContactExtensionPhysical(NULL),
  fPolySpiderPhysical(NULL),
  fPentaPlugPhysical(NULL),
  fPentaReceptaclePhysical(NULL),
  fMillMaxReceptaclePhysical(NULL),
  fSupportPostPhysical(NULL),
  fCompressionSpringPhysical(NULL),
  fDHPinPhysical(NULL),
  fDHPinInReceptaclePhysical(NULL),
  fFETPhysical(NULL),
  fHeaterResistorPhysical(NULL),
  fInjectorResistorPhysical(NULL),
  fPbPatchOnePhysical(NULL),
  fPbPatchTwoPhysical(NULL),
  fConnectorInsulatorPhysical(NULL)
{
  SetDefaults();
  if(!messOff)
    fG4Messenger = new MJGeometryKURFBEGeDetectorMessenger(this);
  else 
    fG4Messenger = 0;

}
//---------------------------------------------------------------------------//
MJGeometryKURFBEGeDetector::~MJGeometryKURFBEGeDetector()
{
  delete fG4Messenger;
}
//---------------------------------------------------------------------------//
void MJGeometryKURFBEGeDetector::ConstructDetector()
{
  G4Material *Vacuum   = G4Material::GetMaterial("Vacuum");
  G4Material *Aluminum = G4Material::GetMaterial("Aluminum");
  G4Material *Copper = G4Material::GetMaterial("Copper-OFHC");
  G4Material *Teflon = G4Material::GetMaterial("Teflon");
  G4Material *Lead = G4Material::GetMaterial("Lead-Ain");


  // from wikipedia:
  G4Element *ZincElement = new G4Element("Zinc", "Zn", 30, 65.38*g/mole );
  G4Element *BerylliumElement = new G4Element("Beryllium", "Be", 4, 9.012182*g/mole );
  G4Element *TinElement = new G4Element("Tin", "Sn", 50, 118.710*g/mole );

  G4Element *CopperElement = G4Element::GetElement("Copper");
  G4Element *NickelElement = G4Element::GetElement("Nickel");
  G4Element *LeadElement = G4Element::GetElement("Lead");
  G4Element *ChromiumElement = G4Element::GetElement("Chromium");
  G4Element *IronElement = G4Element::GetElement("Iron");

  G4double density = 0.0;

  density = 8.650*g/cm3; 
  G4Material *NickelSilver = new G4Material("NickelSilver", density, 3);
  NickelSilver->AddElement( CopperElement, 0.60 );
  NickelSilver->AddElement( ZincElement, 0.20 );
  NickelSilver->AddElement( NickelElement, 0.20 );

  // brass alloy, as defined by mill-max:
  density = 0.307*0.45359237*kg/inch/inch/inch; // 0.307 lb/in3
  G4Material *Brass = new G4Material("Brass", density, 3);
  Brass->AddElement( CopperElement, 0.615 );
  Brass->AddElement( ZincElement, 0.354 );
  Brass->AddElement( LeadElement, 0.031 );

  density = 8.0*g/cm3;
  G4Material *StainlessSteel = new G4Material("StainlessSteel304", density, 3 );
  StainlessSteel->AddElement( NickelElement, 0.08 );
  StainlessSteel->AddElement( ChromiumElement, 0.18 );
  StainlessSteel->AddElement( IronElement, 0.74 );

  density = 8.175*g/cm3; // http://www.simetric.co.uk/si_metals.htm
  // chose typical composition from wikipedia:
  G4Material *BerylliumCopper = new G4Material("BerylliumCopper", density, 2 );
  BerylliumCopper->AddElement(CopperElement, 0.980);
  BerylliumCopper->AddElement(BerylliumElement, 0.02);

  density =7.365*g/cm3;
  G4Material *Tin = new G4Material("Tin", density, 1 );
  Tin->AddElement(TinElement, 1.0);


  if(!( Aluminum || Vacuum || Copper || Teflon || Lead  ||
  NickelSilver || Brass || StainlessSteel || BerylliumCopper || Tin )) {
    MGLog(error) << "NULL G4Material pointer!  Cannot find material!" << endlog;
    MGLog(fatal) << endlog;
  }
  

  // a small displacement to help with dimensions and placement:
  G4double eps = 0.001 * mm;
  // the value of eps used for the vacuum envelope; this can be modified for
  // easier debugging:

  G4double envelopeEps = eps; 
  //G4double envelopeEps = 5.0*mm; // for debugging only!!

  MGLog(routine) << "cylinders extend from 0 to " << fCylinderAngle/pi << "*pi"
  << endlog;

  // collimator stuff
  MGLog(routine) << "is collimator used: " << fUseCollimator << endlog;
  if (fUseCollimator){
    MGLog(routine) << "   collimator position: " << fCollimatorSideOrTop << endlog;
    if (fCollimatorSideOrTop == "side"){
      MGLog(routine) << "   collimator fZ      : " << fCollimatorZCoordinateIn_mm/mm << " mm"<< endlog;
      MGLog(routine) << "   collimator fR      : " << fCollimatorRCoordinateIn_mm/mm << " mm"<< endlog;
      MGLog(routine) << "   collimator fAngle  : " << fCollimatorAngleIn_deg/deg << " degrees"<< endlog;
    }
    else{
      MGLog(routine) << "   collimator fX      : " << fCollimatorXCoordinateIn_mm/mm << " mm" << endlog;
      MGLog(routine) << "   collimator fY      : " << fCollimatorYCoordinateIn_mm/mm << " mm"<< endlog;
      MGLog(routine) << "   collimator fZ      : " << fCollimatorZCoordinateIn_mm/mm << " mm"<< endlog;
    }
  }

  //--------------------------------------------------------------------
  // cryostat end cap:
  //--------------------------------------------------------------------

  G4double endCapZ = 0;
  fTopOfCryostatZ = endCapZ + fEndCapHeight/2.0;

  MGLog(routine) << "top of cryostat is at z=" << fTopOfCryostatZ/cm << " cm" <<
  endlog;

  G4ThreeVector endCapPlacement(0, 0, endCapZ);

  //--------------------------------------------------------------------
  // crystal:
  //--------------------------------------------------------------------

  G4double xtalZ = endCapZ + fEndCapHeight/2.0 - fCrystalOffset -
  fCrystalHeight/2.0;

  G4ThreeVector xtalPlacement(0,0, xtalZ);

  //--------------------------------------------------------------------
  // teflon cup placement
  //--------------------------------------------------------------------

  // align the top of the cup with the top of the xtal
  G4double teflonCupZ = xtalZ + fCrystalHeight/2.0 - fTeflonCupHeight/2.0;

  G4ThreeVector teflonCupPlacement(0, 0, teflonCupZ); 

  //--------------------------------------------------------------------
  // teflon disk placement
  //--------------------------------------------------------------------

  G4double teflonDiskZ = xtalZ + fCrystalHeight/2.0 + fTeflonDiskThickness/2.0 +
  eps;

  G4ThreeVector teflonDiskPlacement(0, 0, teflonDiskZ );

  //--------------------------------------------------------------------
  // detector spacer placement
  //--------------------------------------------------------------------

  G4double detectorSpacerZ = teflonCupZ - fTeflonCupHeight/2.0 +
  fTeflonCupBottomThickness + fDetectorSpacerThickness/2.0 + eps;

  G4ThreeVector detectorSpacerPlacement(0, 0, detectorSpacerZ);

  //---------------------------------------------------------------------
  // support post placement
  //--------------------------------------------------------------------

  G4double supportPostZ = teflonCupZ - fTeflonCupHeight/2.0 +
  fTeflonCupBottomThickness + fSupportPostHeight/2.0 -
  fSupportPostStepHeight + eps;


  //--------------------------------------------------------------------
  // copper cup placement
  //--------------------------------------------------------------------

  G4double copperCupZ = teflonCupZ - fTeflonCupHeight/2.0
  -fCopperCupBottomThickness + fCopperCupHeight/2.0 - eps;

  G4ThreeVector copperCupPlacement(0, 0, copperCupZ); 

  //--------------------------------------------------------------------
  // copper cap placement
  //--------------------------------------------------------------------

  G4double copperLidZ = copperCupZ + fCopperCupHeight/2.0 -
  fCopperLidHeight/2.0 + fCopperLidFlatBaseThickness + eps;

  G4ThreeVector copperLidPlacement(0, 0, copperLidZ); 

  //--------------------------------------------------------------------
  // pb patches
  //--------------------------------------------------------------------

  G4double pbPatchZ = xtalZ;

  G4ThreeVector pbPatchPlacement(0, 0, pbPatchZ);

  //--------------------------------------------------------------------
  // electonics body placement
  //--------------------------------------------------------------------

  G4double electronicsBaseZ = copperCupZ - fCopperCupHeight/2.0 - fEBHeight/2.0
  - eps;

  G4ThreeVector electronicsBasePlacement( 0, 0, electronicsBaseZ );

  //--------------------------------------------------------------------
  // FET block
  //--------------------------------------------------------------------

  G4double fetBlockZ = electronicsBaseZ - fEBHeight/2.0 + fEBFlatTopThickness +
  fFetBlockHeight/2.0 + eps;

  G4ThreeVector fetBlockPlacement( 0, 0, fetBlockZ );

  //--------------------------------------------------------------------
  // placement of cold finger nearest detector:
  //--------------------------------------------------------------------

  G4double rdcZ = electronicsBaseZ - fEBHeight/2.0 - fRdcColdFingerLength/2.0 -
  eps;
  
  G4ThreeVector rdcColdFingerPlacement(0, 0, rdcZ);


  //--------------------------------------------------------------------
  // placement of zeolite package:
  //--------------------------------------------------------------------

  G4double zeoliteZ = rdcZ - fRdcColdFingerLength/2.0 - eps - fZeoliteWidth/2.0;

  G4ThreeVector zeolitePlacement( fRdcColdFingerRadius, 0, zeoliteZ);


  //--------------------------------------------------------------------
  // placement of cryostat body:
  //--------------------------------------------------------------------
  G4double cryostatZ = endCapZ - fEndCapHeight/2.0 +
  fCryostatOringFlangeThickness/2.0;

  G4ThreeVector cryostatPlacement( 0, 0, cryostatZ );

  //--------------------------------------------------------------------
  // placement of spider trestle
  //--------------------------------------------------------------------

  G4double spiderTrestleZ = cryostatZ + fCryostatOringFlangeThickness/2.0 +
  fSpiderTrestleHeight/2.0 + eps;

  G4ThreeVector spiderTrestlePlacement( 0, 0, spiderTrestleZ );

  //--------------------------------------------------------------------
  // placement of poly spider
  //--------------------------------------------------------------------

  G4double polySpiderZ = spiderTrestleZ + fSpiderTrestleHeight/2.0 + eps +
  fPolySpiderHeight/2.0;

  G4ThreeVector polySpiderPlacement(0, 0, polySpiderZ);
  
  //--------------------------------------------------------------------
  // placement of stand top plate
  //--------------------------------------------------------------------

  G4double standTopPlateZ = endCapZ + fEndCapHeight/2.0- fStandTopPlateOffset -
  fStandTopPlateThickness/2.0;

  G4ThreeVector standTopPlatePlacement(0, 0, standTopPlateZ);

  // set z-coordinate of top of plate, account for thickness of cryostat
  // envelope mother volume:
  fTopOfStandTopPlateZ = standTopPlateZ + fStandTopPlateThickness/2.0 + eps; 

  //--------------------------------------------------------------------
  // placement of threaded collar
  //--------------------------------------------------------------------

  G4double threadedCollarZ = electronicsBaseZ - fEBHeight/2.0 -eps
  -fThreadedCollarHeight/2.0;

  G4ThreeVector threadedCollarPlacement(0, 0, threadedCollarZ);

  //--------------------------------------------------------------------
  // placement of HV contact extension -- insulator
  //--------------------------------------------------------------------

  G4double hvContactExtensionZ = electronicsBaseZ - fEBHeight/2.0 +
  fEBFlatTopThickness + eps + fHVContactExtensionLength/2.0 -
  fHVContactExtensionStepLength;

  G4double hvContactExtensionX = fCopperCupContactHoleDistance;
 
  //--------------------------------------------------------------------
  // placement of HV probe pogo 
  //--------------------------------------------------------------------

  G4double hvPogoPlungerZ = xtalZ - eps - fHVSolderHeight - eps -
  fCrystalHeight/2.0 - fHVPogoPlungerHeight/2.0 - eps;

  G4double hvPogoBarrelZ = hvContactExtensionZ + fHVContactExtensionLength/2.0 +
  fHVPogoBarrelHeight/2.0;
  
  G4double hvPogoX = fCopperCupContactHoleDistance;

  G4ThreeVector hvPogoBarrelPlacement(hvPogoX, 0, hvPogoBarrelZ);
  G4ThreeVector hvPogoPlungerPlacement(hvPogoX, 0, hvPogoPlungerZ);

  G4ThreeVector hvContactExtensionPlacement(hvContactExtensionX, 0, hvContactExtensionZ); 

  //--------------------------------------------------------------------
  // placement of center contact solder patch
  //--------------------------------------------------------------------

  G4double solderZ = xtalZ - fCrystalHeight/2.0 - eps -
  fCenterContactSolderHeight/2.0;

  G4ThreeVector centerContactSolderPlacement(0, 0, solderZ);

  //--------------------------------------------------------------------
  // placement of HV solder patch
  //--------------------------------------------------------------------

  G4double hvSolderZ = xtalZ - fCrystalHeight/2.0 - eps -
  fHVSolderHeight/2.0;

  G4ThreeVector hvSolderPlacement(hvPogoX, 0, hvSolderZ);

  //--------------------------------------------------------------------
  // placement of Mill Max center contact:
  //--------------------------------------------------------------------

  G4double dhPinLength = fDHPinTopTailLength + fDHPinShoulderHeight +
  fDHPinPressFitLength + fDHPinBottomTailLength;

  G4double centerContactPinLength = 0.09*inch;

  G4double centerContactZ = solderZ - fCenterContactSolderHeight/2.0 - eps -
  dhPinLength/2.0 + fDHPinTopTailLength - centerContactPinLength;

  G4ThreeVector centerContactPlacement(0, 0, centerContactZ);

  //--------------------------------------------------------------------
  // placement of penta plug and receptacle
  //--------------------------------------------------------------------

  G4double pentaPlugZ = centerContactZ - dhPinLength/2.0 +
  fDHPinBottomTailLength + fDHPinPressFitLength - eps - fPentaPlugHeight/2.0;

  G4ThreeVector pentaPlugPlacement( 0, 0, pentaPlugZ );

  G4double pentaReceptacleZ = pentaPlugZ - fPentaReceptacleHeight/2.0 -
  fPentaPlugSpacing - fPentaPlugHeight/2.0;

  G4ThreeVector pentaReceptaclePlacement( 0, 0, pentaReceptacleZ );

  //--------------------------------------------------------------------
  // placement of FET
  //--------------------------------------------------------------------

  G4double fetZ = pentaPlugZ - fPentaPlugHeight/2.0 - eps - fFETLength/2.0;
  G4double fetY = fDHPinPressFitOuterRadius + eps;

  G4ThreeVector fetPlacement(0, fetY, fetZ);

  //--------------------------------------------------------------------
  // placement of solder behind FET
  // FIXME -- this is a temporary location until better estimate of solder
  // location is made
  //--------------------------------------------------------------------

  G4double fetSolderZ = fetZ - fFETLength/2.0 - eps - fSolderRadius;

  G4ThreeVector fetSolderPlacement(0, 0, fetSolderZ);

  //--------------------------------------------------------------------
  // placement of Mill Max 0134 receptacle:
  //--------------------------------------------------------------------

  // there is one receptacle in the plug that is not paired with a pin:

  // added -eps on 13 oct 2010, A. Schubert
  G4double millMaxZ = pentaPlugZ - fPentaPlugHeight/2.0 +
  fMillMaxReceptacleLength/2.0 - fMillMaxReceptacleTopStepLength - eps;

  //--------------------------------------------------------------------
  // placement of resistors
  //--------------------------------------------------------------------

  G4double resistorZ = pentaReceptacleZ + fPentaReceptacleHeight/2.0 -
  fResistorOffset - fResistorLength/2.0;

  //--------------------------------------------------------------------
  // placement of compression spring
  //--------------------------------------------------------------------

  // 10 Nov 2011 -- defining compressino spring height to fit in allowed space:
  fCompressionSpringHeight = pentaReceptacleZ - fPentaReceptacleHeight/2.0 -
  fetBlockZ + fFetBlockHeight/2.0 - fFetBlockBottomThickness - 2.0*eps;

  G4double compressionSpringZ = pentaReceptacleZ - fPentaReceptacleHeight/2.0 -
  eps - fCompressionSpringHeight/2.0;

  G4ThreeVector compressionSpringPlacement(0, 0, compressionSpringZ);

  //--------------------------------------------------------------------
  // inusulators around pins in electronics base
  //--------------------------------------------------------------------

  G4double ebPinInsulatorZ = electronicsBaseZ - fEBHeight/2.0 +
  fEBFlatTopThickness - fConnectorInsulatorLength/2.0 +
  fConnectorInsulatorStepLength + eps;

  //--------------------------------------------------------------------
  // pins through electronics base
  //--------------------------------------------------------------------

  G4double ebPinZ = ebPinInsulatorZ + fConnectorInsulatorLength/2.0 + eps +
  dhPinLength/2.0 - fDHPinBottomTailLength - fDHPinPressFitLength;


  //--------------------------------------------------------------------
  // placement of cryostat components (relative to the o-ring flange):
  // define z-coords here so the detector logical definition can use them
  //--------------------------------------------------------------------

  G4double shortNeckZ = -fCryostatOringFlangeThickness/2.0
  - fCryostatShortNeckLength/2.0 + eps;

  G4ThreeVector shortNeckPlacement(0, 0, shortNeckZ);

  G4double cryostatFlangeZ = shortNeckZ - fCryostatShortNeckLength/2.0 -
  fCryostatFlangeHeight/2.0 + eps;

  G4ThreeVector cryostatFlangePlacement(0, 0, cryostatFlangeZ );


  //----------------------------- WORLD VOLUME ------------------------------//
  //-------------------------DETECTOR LOGICAL -------------------------------//
  //This is a vacuum space slightly larger than the cryostat.  All parts are
  //placed in this space. 
  //-------------------------------------------------------------------------//

  // a cylinder surrounding the endcap:
  G4Tubs *endCapEnvelope = new G4Tubs("endCapEnvelope", 0,
  fEndCapOuterRadius + envelopeEps, fEndCapHeight/2.0 + envelopeEps, 0, twopi);

  // a cylinder surrounding cryostat neck:
  G4Tubs *neckEnvelope = new G4Tubs("neckEnvelope", 0,
    fCryostatNeckOuterRadius + envelopeEps, fCryostatShortNeckLength/2.0 +
    envelopeEps, 0, twopi);
  
  G4double neckEnvelopeZ = cryostatZ + shortNeckZ;

  G4ThreeVector neckEnvelopePlacement(0, 0, neckEnvelopeZ);

  G4UnionSolid *cryostatEnvelope = new G4UnionSolid("cryostatEnvelope",
  endCapEnvelope, neckEnvelope, 0, neckEnvelopePlacement );

  // a box surrounding the stand top plate:
  G4Box *topPlateEnvelope = new G4Box("topPlateEnvelope1",
  fStandTopPlateLength/2.0 + envelopeEps, fStandTopPlateLength/2.0 + envelopeEps,
  fStandTopPlateThickness/2.0 + envelopeEps);

  cryostatEnvelope = new G4UnionSolid("cryostatEnvelope2", cryostatEnvelope,
  topPlateEnvelope, 0, standTopPlatePlacement);

  // a cylinder around the lower cryostat flange
  G4Tubs *flangeEnvelope = new G4Tubs("flangeEnvelope", 0,
    fCryostatFlangeOuterRadius + envelopeEps, fCryostatFlangeHeight/2.0 +
    envelopeEps + fZeoliteWidth/2.0, 0, twopi );

  G4double flangeEnvelopeZ = cryostatZ + cryostatFlangeZ - fZeoliteWidth/2.0;

  G4ThreeVector flangeEnvelopePlacement(0, 0, flangeEnvelopeZ);

  cryostatEnvelope = new G4UnionSolid("cryostatEnvelope3", cryostatEnvelope,
  flangeEnvelope, 0, flangeEnvelopePlacement);

  // a cylinder around the cold finger:
  G4Tubs *coldFingerEnvelope = new G4Tubs( "coldFingerEnvelope", 0,
  fRdcColdFingerRadius + envelopeEps, fRdcColdFingerLength/2.0 + envelopeEps, 0, twopi );

  cryostatEnvelope = new G4UnionSolid("cryostatEnvelope4", cryostatEnvelope,
  coldFingerEnvelope, 0, rdcColdFingerPlacement );

  // the detector logical:
  theDetectorLogical = new G4LogicalVolume(cryostatEnvelope, Vacuum,
  "cryostatLogical", 0, 0, 0, true);

  //----------------------------- END CAP ----------------------------------//
  // This is the copper outer vacuum can.
  // From end cap drawing C200510 
  //-------------------------------------------------------------------------//

  G4Tubs *endCapCylinder = new G4Tubs("endCapCylinder", 0, fEndCapOuterRadius,
  fEndCapHeight/2.0, 0, fCylinderAngle );

  G4Tubs *endCapCavity = new G4Tubs("endCapCavity", 0, fEndCapInnerRadius,
  fEndCapHeight/2.0, 0, twopi );

  G4ThreeVector endCapCavityPlacement(0,0,-fEndCapFlatThickness);

  G4SubtractionSolid *endCap = new G4SubtractionSolid("endCap", endCapCylinder,
  endCapCavity, 0, endCapCavityPlacement );

  fEndCapLogical = new G4LogicalVolume(endCap, Copper,
  "endCapLogical", 0, 0, 0, true);

  fEndCapPhysical = new G4PVPlacement(0, endCapPlacement, fEndCapLogical,
  "endCapLogical", theDetectorLogical, false, 0);

  //------------------------------ TEFLON CUP -------------------------------//
  //-------------------------------------------------------------------------//

  G4Tubs *teflonCupCylinder = new G4Tubs( "teflonCupCylinder",
  fTeflonCupHoleRadius, fTeflonCupOuterRadius, fTeflonCupHeight/2.0, 0,
  fCylinderAngle ); 

  G4ThreeVector teflonCupCavityPlacement(0,0, fTeflonCupBottomThickness);

  G4Tubs *teflonCupCavity = new G4Tubs( "teflonCupCavity", 0,
  fTeflonCupInnerRadius, fTeflonCupHeight/2.0, 0, twopi ); 

  G4SubtractionSolid *teflonCupSolid = new G4SubtractionSolid("teflonCupSolid",
  teflonCupCylinder, teflonCupCavity, 0, teflonCupCavityPlacement); 

  // there is a small cylindrical feature at the bottom of the cup:

  G4Tubs *teflonCupEndCylinder = new G4Tubs( "teflonCupEndCylinder",
  fTeflonCupHoleRadius, fTeflonCupEndCylinderRadius,
  fTeflonCupEndCylinderHeight/2.0 + eps/2.0, 0, fCylinderAngle );

  G4ThreeVector teflonEndPlacement(0, 0, -fTeflonCupEndCylinderHeight/2.0 -
  fTeflonCupHeight/2.0 + eps); 

  G4UnionSolid *teflonCup = new G4UnionSolid( "teflonCup", teflonCupSolid,
  teflonCupEndCylinder, 0, teflonEndPlacement ); 

  // there are two off-center holes through the bottom of the teflon cup.  one
  // is for making HV contact.  these also appear in the copper cup.
  G4Tubs* contactHole = new G4Tubs( "contactHole", 0,
  fCopperCupContactHoleRadius, fCopperCupHeight, 0, twopi );

  G4ThreeVector contactHoleOnePlacement(fCopperCupContactHoleDistance, 0,
  -fCopperCupHeight/2.0); 

  G4SubtractionSolid *teflonCupWithOneContactHole = new G4SubtractionSolid(
  "teflonCupWithOneContactHole", teflonCup, contactHole, 0,
  contactHoleOnePlacement ); 

  G4ThreeVector contactHoleTwoPlacement(0, fCopperCupContactHoleDistance,
  -fCopperCupHeight/2.0); 

  G4SubtractionSolid *teflonCupWithTwoContactHoles = new G4SubtractionSolid(
  "teflonCupWithTwoContactHoles", teflonCupWithOneContactHole, contactHole, 0,
  contactHoleTwoPlacement ); 

  fTeflonCupLogical = new G4LogicalVolume( teflonCupWithTwoContactHoles, Teflon,
  "teflonCupLogical", 0, 0, 0, true );

  fTeflonCupPhysical = new G4PVPlacement(0, teflonCupPlacement,
  fTeflonCupLogical, "teflonCupPhysical", theDetectorLogical, false, 0);

  //------------------------------ TEFLON DISK ------------------------------//
  //-------------------------------------------------------------------------//

  // teflon disk over face of detector, from CANBERRA Teflon Disk drawing in
  // Small_Parts.pdf:
  G4Tubs *teflonDisk = new G4Tubs("teflonDisk", 0, fTeflonCupOuterRadius,
  fTeflonDiskThickness/2.0, 0, fCylinderAngle );

  fTeflonDiskLogical = new G4LogicalVolume( teflonDisk, Teflon,
  "teflonDiskLogical", 0, 0, 0, true );

  fTeflonDiskPhysical = new G4PVPlacement( 0, teflonDiskPlacement,
  fTeflonDiskLogical, "teflonDiskPhysical", theDetectorLogical, false, 0);

 
  //--------------------------------------------------------------------
  // pb patches on sides of xtal
  //--------------------------------------------------------------------

  if ( fUsePbPatches ) {

    MGLog(routine) << "Pb patches are used!!" << endlog; 

    // first make a thin cylindrical shell -- this is so the foil fits around xtal

    G4double patchInnerRadius = fCrystalRadius + eps;
    
    G4Tubs* pbFoilShell = new G4Tubs( "pbFoilShell", patchInnerRadius,
    patchInnerRadius + fPbPatchThickness, fCrystalHeight/2.0, pi, pi );

    // rotate by pi/2 around the x-axis so the shell and rectangles intersect properly
    G4RotationMatrix* foilIntersectionRotation = new G4RotationMatrix();
    foilIntersectionRotation->rotateX(pi/2.0);

    // make foil one and place it
    G4VSolid* pbFoilOne = new G4Box( "pbFoilOneBox", fPbPatchOneWidth/2.0,
    fPbPatchOneHeight/2.0, fCrystalRadius*2.0);

    pbFoilOne = new G4IntersectionSolid( "pbFoilOne", pbFoilShell, pbFoilOne,
    foilIntersectionRotation, G4ThreeVector() ); 

    fPbPatchOneLogical = new G4LogicalVolume( pbFoilOne, Lead,
    "fPbPatchOneLogical", 0, 0, 0, true );

    fPbPatchOnePhysical = new G4PVPlacement( 0, pbPatchPlacement,
    fPbPatchOneLogical, "patchOnePhysical", theDetectorLogical, false, 0 ); 

    // make foil two and place it
    G4VSolid* pbFoilTwo = new G4Box( "pbFoilTwoBox", fPbPatchTwoWidth/2.0,
    fPbPatchTwoHeight/2.0, fCrystalRadius*2.0);

    pbFoilTwo = new G4IntersectionSolid( "pbFoilTwo", pbFoilShell, pbFoilTwo,
    foilIntersectionRotation, G4ThreeVector() ); 

    fPbPatchTwoLogical = new G4LogicalVolume( pbFoilTwo, Lead,
    "fPbPatchTwoLogical", 0, 0, 0, true );

    // rotate by 3*pi/2 around the z-axis so patch two is 90 degrees away from
    // patch one
    G4RotationMatrix* patchRotation = new G4RotationMatrix();
    patchRotation->rotateZ(3.0*pi/2.0);

    fPbPatchTwoPhysical = new G4PVPlacement( patchRotation, pbPatchPlacement,
    fPbPatchTwoLogical, "patchTwoPhysical", theDetectorLogical, false, 0 ); 

  } else {

    MGLog(routine) << "Pb patches are *NOT* used!!" << endlog; 

  }

  //--------------------------- DETECTOR SPACER -----------------------------//
  //-------------------------------------------------------------------------//

  G4Tubs *detectorSpacer = new G4Tubs("detectorSpacer",
  fDetectorSpacerInnerRadius, fDetectorSpacerOuterRadius,
  fDetectorSpacerThickness/2.0, 0, fCylinderAngle );

  fDetectorSpacerLogical = new G4LogicalVolume( detectorSpacer, Copper,
  "detectorSpacerLogical", 0, 0, 0, true );

  fDetectorSpacerPhysical = new G4PVPlacement( 0, detectorSpacerPlacement,
  fDetectorSpacerLogical, "detectorSpacerPhysical", theDetectorLogical, false, 0
  );


  //----------------------------SUPPORT POST -------------------------
  //--------------------------------------------------------------------

  // post body:
  G4Tubs *supportPostBody = new G4Tubs("supportPostBody",
  fSupportPostInnerRadius, fSupportPostOuterRadius, fSupportPostHeight/2.0, 0,
  fCylinderAngle );

  // shape to subtract:
  G4Tubs *supportPostStep = new G4Tubs("supportPostStep",
  fSupportPostSmallOuterRadius, fSupportPostOuterRadius*2.0,
  fSupportPostStepHeight, 0, twopi );

  G4ThreeVector supportPostStepPlacement(0, 0, -fSupportPostHeight/2.0 );

  G4SubtractionSolid* supportPost = new G4SubtractionSolid( "supportPost",
  supportPostBody, supportPostStep, 0, supportPostStepPlacement );

  fSupportPostLogical = new G4LogicalVolume( supportPost, Teflon,
  "supportPostLogical", 0, 0, 0, true );

  G4ThreeVector supportPostPlacement0( fCopperCupContactHoleDistance, 0,
  supportPostZ );

  fSupportPostPhysical = new G4PVPlacement( 0, supportPostPlacement0,
  fSupportPostLogical, "supportPostPhysical0", theDetectorLogical, false, 0 );


  //--------------------------- SPRING ---------------------------------
  //--------------------------------------------------------------------

  G4Tubs *compressionSpring = new G4Tubs( "compressionSpring",
  fCompressionSpringInnerRadius, fCompressionSpringOuterRadius,
  fCompressionSpringHeight/2.0, 0, fCylinderAngle );

  // updated material 29 Aug 2010, A. Schubert per M. Yocum email (Stainless
  // steel 302 has same materials as SS304)
  fCompressionSpringLogical = new G4LogicalVolume( compressionSpring,
  StainlessSteel, "compressionSpringLogical", 0, 0, 0, true );

  fCompressionSpringPhysical = new G4PVPlacement( 0, compressionSpringPlacement,
  fCompressionSpringLogical, "compressionSpringLogical", theDetectorLogical,
  false, 0 );

  //----------------------- COPPER CUP --------------------------------------//
  // from Collar LEGe-BEGe Holder1 and CANBERRA LEGe/BEGe HOLDER B201784X dwgs
  //-------------------------------------------------------------------------//

  // origin is at center of copper cup body

  // cylinder in shape of bulk part:
  G4Tubs *copperCupCylinder = new G4Tubs( "copperCupCylinder",
  fCopperCupHoleRadius, fCopperCupOuterRadius, fCopperCupHeight/2.0, 0,
  fCylinderAngle ); 

  // cylinder to subtract from the bulk part:
  G4Tubs *copperCupCavity = new G4Tubs ("copperCupCavity", 0,
  fCopperCupInnerRadius, fCopperCupHeight/2.0, 0, twopi );

  // z-coord of subtracted cavity:
  G4double copperCupCavityZ = fCopperCupBottomThickness;

  // placement of subtracted cavity:
  G4ThreeVector copperCupCavityPlacement(0, 0, copperCupCavityZ );

  // subtract cavity from cylinder to form cup:
  G4SubtractionSolid *copperCupSolid = new G4SubtractionSolid( "copperCupSolid",
  copperCupCylinder, copperCupCavity, 0, copperCupCavityPlacement ); 

  // annular set screw feature near top of cup:
  G4Tubs* setScrewAnnulus = new G4Tubs( "setScrewAnnulus",
  fCopperCupInnerRadius + eps, fCopperCupSetScrewFeatureOuterRadius,
  fCopperCupSetScrewFeatureHeight/2.0, 0, fCylinderAngle ); 
 
  // set screw feature z-coord:
  G4double setScrewFeatureZ = fCopperCupHeight/2.0 -
  fCopperCupSetScrewFeatureOffsetFromTop - fCopperCupSetScrewFeatureHeight/2.0;

  // set screw feature placement:
  G4ThreeVector setScrewFeaturePlacement( 0, 0, setScrewFeatureZ );

  // copper cup plus set screw feature:
  G4UnionSolid* copperCupWithSetScrewFeature = new G4UnionSolid(
  "copperCupWithSetScrewFeature", copperCupSolid, setScrewAnnulus, 0,
  setScrewFeaturePlacement );

  // annular feature at bottom of cup for connecting to cold finger:
  G4Tubs* copperCupBottomAnnulus = new G4Tubs( "copperCupBottomAnnulus",
  fCopperCupBottomAnnulusInnerRadius, fCopperCupBottomAnnulusOuterRadius,
  fCopperCupBottomAnnulusHeight/2.0 + eps/2.0, 0, fCylinderAngle ); 
 
  G4double bottomAnnulusZ =  - fCopperCupBottomAnnulusHeight/2.0 -
  fCopperCupHeight/2.0 + eps;

  G4ThreeVector bottomAnnulusPlacement( 0, 0, bottomAnnulusZ );

  G4UnionSolid* copperCupWithFeatures = new G4UnionSolid(
  "copperCupWithFeatures", copperCupWithSetScrewFeature, copperCupBottomAnnulus,
  0, bottomAnnulusPlacement );

  G4SubtractionSolid *copperCupWithOneContactHole = new G4SubtractionSolid(
  "copperCupWithOneContactHole", copperCupWithFeatures, contactHole, 0,
  contactHoleOnePlacement ); 

  G4SubtractionSolid *copperCupWithTwoContactHoles = new G4SubtractionSolid(
  "copperCupWithTwoContactHoles", copperCupWithOneContactHole, contactHole, 0,
  contactHoleTwoPlacement ); 

  fCopperCupLogical = new G4LogicalVolume( copperCupWithTwoContactHoles, Copper,
  "copperCupLogical", 0, 0, 0, true );

  fCopperCupPhysical = new G4PVPlacement(0, copperCupPlacement,
  fCopperCupLogical, "copperCupPhysical", theDetectorLogical, false, 0 ); 

  //----------------------- COPPER LID --------------------------------------//
  // from Collar LEGe-BEGe Holder1 and CANBERRA LEGe/BEGe HOLDER B201784X dwgs
  //-------------------------------------------------------------------------//

  G4Tubs *copperLidBody = new G4Tubs( "copperLidBody", 0, fCopperLidOuterRadius,
  fCopperLidHeight/2.0, 0, fCylinderAngle );

  G4Tubs *copperLidCavity = new G4Tubs( "copperLidCavity", 0,
  fCopperLidInnerRadius, fCopperLidHeight/2.0, 0, twopi );

  G4double copperLidCavityZ = -fCopperLidFlatBaseThickness; 
  
  G4ThreeVector copperLidCavityPlacement( 0, 0, copperLidCavityZ );

  G4SubtractionSolid *copperLid = new G4SubtractionSolid( "copperLid",
  copperLidBody, copperLidCavity, 0, copperLidCavityPlacement );

  fCopperLidLogical = new G4LogicalVolume( copperLid, Copper,
  "copperLidLogical", 0, 0, 0, true );

  fCopperLidPhysical = new G4PVPlacement( 0, copperLidPlacement,
  fCopperLidLogical, "copperLidPhysical", theDetectorLogical, false, 0 );

  //-------------------------- HV POGO----------------------------------
  //--------------------------------------------------------------------

  // barrel:

  G4Tubs *hvPogoBarrelBody = new G4Tubs( "hvPogoBarrelBody", 0,
  fHVPogoBarrelOuterRadius, fHVPogoBarrelHeight/2.0, 0, fCylinderAngle );

  // hole in barrel:

  G4Tubs *hvPogoBarrelHole = new G4Tubs("hvPogoBarrelHole", 0,
  fHVPogoBarrelInnerRadius, fHVPogoBarrelHeight/2.0 + eps, 0, twopi );

  // receptacle

  G4Tubs *hvPogoReceptacle = new G4Tubs("hvPogoReceptacle", 0,
  fHVPogoReceptacleOuterRadius, fHVPogoReceptacleLength/2.0, 0, fCylinderAngle );

  G4double pogoReceptacleZ = -fHVPogoBarrelHeight/2.0 + fHVPogoReceptacleOffset
  - fHVPogoReceptacleLength/2.0;

  G4ThreeVector pogoReceptaclePlacement(0, 0, pogoReceptacleZ );

  G4UnionSolid* hvPogoBarrelWithReceptacle = new G4UnionSolid(
  "hvPogoBarrelWithReceptacle", hvPogoBarrelBody, hvPogoReceptacle, 0,
  pogoReceptaclePlacement );

  G4SubtractionSolid* hvPogoBarrel = new G4SubtractionSolid( "hvPogoBarrel",
  hvPogoBarrelWithReceptacle, hvPogoBarrelHole, 0, G4ThreeVector() );

  fHVPogoBarrelLogical = new G4LogicalVolume( hvPogoBarrel, NickelSilver,
  "hvPogoBarrelLogical", 0, 0, 0, true );

  fHVPogoBarrelPhysical = new G4PVPlacement( 0, hvPogoBarrelPlacement,
  fHVPogoBarrelLogical, "hvPogoBarrelPhysical", theDetectorLogical, false, 0 );

  // plunger:

  G4Tubs *hvPogoPlunger = new G4Tubs( "hvPogoPlunger", 0, fHVPogoPlungerRadius,
  fHVPogoPlungerHeight/2.0, 0, fCylinderAngle );

  fHVPogoPlungerLogical = new G4LogicalVolume( hvPogoPlunger, BerylliumCopper,
  "hvPogoPlungerLogical", 0, 0, 0, true );

  fHVPogoPlungerPhysical = new G4PVPlacement( 0, hvPogoPlungerPlacement,
  fHVPogoPlungerLogical, "hvPogoPlungerPhysical", theDetectorLogical, false, 0);

  //------------------------- HV CONTACT EXTENSION ---------------------
  //--------------------------------------------------------------------

  G4Tubs *hvContactExtension = new G4Tubs( "hvContactExtension",
  fHVContactExtensionInnerRadius, fHVContactExtensionOuterRadius,
  fHVContactExtensionLength/2.0, 0, fCylinderAngle );


  // solid to subtract to form step feature -- make wider, 2x longer than
  // necessary to avoud boolean solid problems:
  G4Tubs *hvContactExtensionStep = new G4Tubs("hvContactExtensionStep",
  fHVContactExtensionStepRadius, fHVContactExtensionOuterRadius*2.0,
  fHVContactExtensionStepLength, 0, fCylinderAngle );

  G4ThreeVector hvContactExtensionStepPlacement(0, 0,
  -fHVContactExtensionLength/2.0);

  G4SubtractionSolid *hvContactExtensionWithStep = new G4SubtractionSolid(
  "hvContactExtensionWithStep", hvContactExtension, hvContactExtensionStep, 0,
  hvContactExtensionStepPlacement );

  fHVContactExtensionLogical = new G4LogicalVolume( hvContactExtensionWithStep,
  Teflon, "hvContactExtensionWithStepLogical", 0, 0, 0, true );

  fHVContactExtensionPhysical= new G4PVPlacement( 0,
  hvContactExtensionPlacement, fHVContactExtensionLogical,
  "hvContactExtensionPhysical", theDetectorLogical, false, 0 );


  //-------------------------MILL MAX RECEPTACLE ----------------------------//
  //-------------------------------------------------------------------------//

  // make a cylinder in the size of the largest radius and total length of the
  // part; cut from this to make features:

  G4Tubs *millMaxBody = new G4Tubs("millMaxBody", 0,
  fMillMaxReceptacleTopStepRadius, fMillMaxReceptacleLength/2.0, 0,
  fCylinderAngle );

  // subtract a cylindrical shell to form the top step
  G4Tubs *millMaxMiddleCut = new G4Tubs("millMaxMiddleCut",
  fMillMaxReceptacleMiddleStepRadius, fMillMaxReceptacleTopStepRadius*2,
  fMillMaxReceptacleLength/2.0, 0, twopi );

  G4ThreeVector millMaxMiddleCutPlacement(0, 0,
  -fMillMaxReceptacleTopStepLength);

  G4SubtractionSolid *millMaxReceptacle = new G4SubtractionSolid(
  "millMaxReceptacle", millMaxBody, millMaxMiddleCut, 0,
  millMaxMiddleCutPlacement);

  // subtract a cylindrical shell to form the middle step and tail
  G4Tubs *millMaxTailCut = new G4Tubs("millMaxTailCut",
  fMillMaxReceptacleTailRadius, fMillMaxReceptacleTopStepRadius*2,
  fMillMaxReceptacleLength/2.0, 0, twopi );

  G4double millMaxTailCutZ = -fMillMaxReceptacleTopStepLength -
  fMillMaxReceptacleMiddleStepLength;

  G4ThreeVector millMaxTailCutPlacement(0, 0, millMaxTailCutZ );

  millMaxReceptacle = new G4SubtractionSolid("millMaxReceptacle1",
  millMaxReceptacle, millMaxTailCut, 0, millMaxTailCutPlacement );

  // subtract a cylinder from the center to form inner cavity
  G4Tubs *millMaxCenterCut = new G4Tubs("millMaxCenterCut", 0,
  fMillMaxReceptacleInnerRadius, fMillMaxReceptacleInnerHoleDepth, 0, twopi );

  G4double millMaxCenterCutZ = fMillMaxReceptacleLength/2.0;

  G4ThreeVector millMaxCenterCutPlacement(0, 0, millMaxCenterCutZ );

  millMaxReceptacle = new G4SubtractionSolid("millMaxReceptacle2",
  millMaxReceptacle, millMaxCenterCut, 0, millMaxCenterCutPlacement );

  fMillMaxReceptacleLogical = new G4LogicalVolume( millMaxReceptacle, Brass,
  "millMaxReceptacleLogical", 0, 0, 0, true );

  //--------------------------------------------------------------------
  // center contact solder patch
  //---------------------------------------------------------------------

  G4Box* centerContactSolder = new G4Box( "centerContactSolder",
  fCenterContactSolderWidth/2.0, fCenterContactSolderWidth/2.0,
  fCenterContactSolderHeight/2.0);

  G4LogicalVolume* centerContactSolderLogical = new G4LogicalVolume(
  centerContactSolder, Tin, "centerContactSolderLogical", 0, 0, 0, true );

  /*G4PVPlacement* centerContactSolderPhysical =*/ new G4PVPlacement(0,
  centerContactSolderPlacement, centerContactSolderLogical,
  "centerContactSolderPhysical", theDetectorLogical, false, 0);


  //--------------------------------------------------------------------
  // HV contact solder patch
  //---------------------------------------------------------------------

  G4Tubs* hvSolderPatch = new G4Tubs( "hvSolderPatch", 0, fHVSolderRadius,
  fHVSolderHeight/2.0, 0, fCylinderAngle);

  G4LogicalVolume* hvSolderLogical = new G4LogicalVolume(
  hvSolderPatch, Tin, "hvSolderLogical", 0, 0, 0, true );

  /*G4PVPlacement* hvSolderPhysical =*/ new G4PVPlacement(0, hvSolderPlacement,
  hvSolderLogical, "hvSolderPhysical", theDetectorLogical, false, 0);

  //--------------------------------------------------------------------
  // solder blobs around the FET
  //--------------------------------------------------------------------

  G4Sphere* solderBlob = new G4Sphere( "solderBlob", 0, fSolderRadius, 0, twopi,
  0, pi);

  G4LogicalVolume* solderBlobLogical = new G4LogicalVolume( solderBlob, Tin,
  "solderBlob", 0, 0, 0, true );

  /*G4PVPlacement* fetSolderPhysical =*/ new G4PVPlacement(0, fetSolderPlacement,
  solderBlobLogical, "fetSolderPhysical", theDetectorLogical, false, 0);

  //--------------------------------------------------------------------
  // mill max double tail header pin
  //--------------------------------------------------------------------

  // main body
  G4Tubs* dhPinBody = new G4Tubs( "dhPinBody", 0, fDHPinShoulderRadius,
  dhPinLength/2.0, 0, fCylinderAngle );

  // cylinder for subtraction to form top pin:
  G4Tubs* dhPinTopTailSubtraction = new G4Tubs("dhPinTopTailSubtraction",
  fDHPinTopTailRadius, fDHPinShoulderRadius*2.0, fDHPinTopTailLength, 0,
  twopi );

  G4ThreeVector topTailPlacement(0, 0, dhPinLength/2.0 );

  G4SubtractionSolid* dhPin = new G4SubtractionSolid( "dhPin",
  dhPinBody, dhPinTopTailSubtraction, 0, topTailPlacement );

  // cylinder for subtraction to form bottom pin:
  G4Tubs* dhPinBotTailSubtraction = new G4Tubs("dhPinBotTailSubtraction",
  fDHPinBottomTailRadius, fDHPinShoulderRadius*2.0, fDHPinBottomTailLength,
  0, twopi );

  G4ThreeVector botTailPlacement(0, 0, -dhPinLength/2.0 );

  dhPin = new G4SubtractionSolid( "dhPin1", dhPin, dhPinBotTailSubtraction, 0,
  botTailPlacement );

  // cylinder for subtraction to form region for press fitting:
  G4Tubs* dhPinPressFitSubtraction = new G4Tubs("dhPinPressFitSubtraction",
  fDHPinPressFitOuterRadius, fDHPinShoulderRadius*2.0, fDHPinBottomTailLength +
  fDHPinPressFitLength, 0, twopi );

  G4ThreeVector pressFitPlacement(0, 0, -dhPinLength/2.0 );

  dhPin = new G4SubtractionSolid( "dhPin2", dhPin, dhPinPressFitSubtraction, 0,
  pressFitPlacement );

  // some pin ends appear clipped -- use a subtraction volume to do the
  // clipping:
  G4Tubs* pinClip = new G4Tubs("pinClip", 0, fDHPinPressFitOuterRadius,
  dhPinLength/2.0, 0, twopi );

  // digitized from penta diagram:
  G4double centerContactClipZ = dhPinLength - fDHPinTopTailLength +
  centerContactPinLength;

  G4ThreeVector centerContactClipPlacement(0, 0, centerContactClipZ);

  G4SubtractionSolid* centerContactPin = new G4SubtractionSolid(
  "centerContactPin", dhPin, pinClip, 0, centerContactClipPlacement );

  fDHPinLogical = new G4LogicalVolume( centerContactPin, Brass, "dhPinLogical",
  0, 0, 0, true );

  fDHPinPhysical = new G4PVPlacement(0, centerContactPlacement, fDHPinLogical,
  "fDHPinPhysical", theDetectorLogical, false, 0 );

  G4double dhPinClipZ = -dhPinLength + fDHPinBottomTailLength - 0.05*inch; 

  G4ThreeVector  dhPinClipPlacement(0, 0, dhPinClipZ);

  G4SubtractionSolid* clippedDHPin = new G4SubtractionSolid( "clippedDHPin",
  dhPin, pinClip, 0, dhPinClipPlacement );

  G4LogicalVolume* clippedDHPinLogical = new G4LogicalVolume( clippedDHPin,
  Brass, "clippedDHPinLogical", 0, 0, 0, true );


  //--------------------------------------------------------------------
  // pin + receptacle assembly:
  //--------------------------------------------------------------------

  G4double pinZ = fMillMaxReceptacleLength/2.0 - fMillMaxReceptacleTopStepLength
  - eps + fPentaPlugSpacing - eps + dhPinLength/2.0 - fDHPinTopTailLength -
  fDHPinShoulderHeight;
  
  G4ThreeVector pinPlacement(0, 0, pinZ);

  G4RotationMatrix *pinRot = new G4RotationMatrix;
  pinRot->rotateY(pi);

  G4UnionSolid* dhPinAndReceptacle = new G4UnionSolid("dhPinAndReceptacle",
  millMaxReceptacle, clippedDHPin, pinRot, pinPlacement );

  fDHPinInReceptacleLogical = new G4LogicalVolume( dhPinAndReceptacle, Brass,
  "fDHPinInReceptacleLogical", 0, 0, 0, true );

  //--------------------------------------------------------------------
  // hole for press fitting mill max pin/receptacle
  //--------------------------------------------------------------------

  // hole for subtraction -- used in places where mill max pin or receptacle is
  // press fit in place -- length is arbitrary -- chosen to pass through penta
  // receptacle:
  G4Tubs *millMaxPressFitHole = new G4Tubs("millMaxPressFitHole", 0,
  fDHPinPressFitOuterRadius + eps, fPentaReceptacleHeight, 0, twopi );


  //--------------------------------------------------------------------
  // insulator for connector in electronics base
  //--------------------------------------------------------------------

  // main body
  G4Tubs* connectorInsulatorBody = new G4Tubs( "connectorInsulatorBody",
  fConnectorInsulatorInnerRadius, fConnectorInsulatorOuterRadius,
  fConnectorInsulatorLength/2.0, 0, fCylinderAngle );

  // cylinder to use for subtraction to form step
  G4Tubs* connectorInsulatorStep = new G4Tubs( "connectorInsulatorStep",
  fConnectorInsulatorSmallOuterRadius, fConnectorInsulatorOuterRadius*2.0,
  fConnectorInsulatorLength/2.0, 0, twopi );

  G4ThreeVector connectorInsulatorStepPlacement(0, 0,
  - fConnectorInsulatorStepLength);

  G4SubtractionSolid* connectorInsulator = new
  G4SubtractionSolid( "connectorInsulator", connectorInsulatorBody,
  connectorInsulatorStep, 0, connectorInsulatorStepPlacement );

  fConnectorInsulatorLogical = new G4LogicalVolume( connectorInsulator, Teflon,
  "fConnectorInsulatorLogical", 0, 0, 0, true );

  //------------------------ ELECTRONICS BASE -------------------------------//
  //-------------------------------------------------------------------------//
  
  G4Tubs *electronicsBaseBody = new G4Tubs( "electronicsBaseBody", 0,
  fEBOuterRadius, fEBHeight/2.0, 0, fCylinderAngle );

  G4Tubs *electronicsBaseCavity = new G4Tubs ("electronicsBaseCavity", 0,
  fEBInnerRadius, fEBHeight/2.0, 0, twopi );

  G4double electronicsBaseCavityZ = fEBFlatTopThickness;

  G4ThreeVector electronicsBaseCavityPlacement(0, 0, electronicsBaseCavityZ);

  G4SubtractionSolid *electronicsBase = new G4SubtractionSolid(
  "electronicsBase", electronicsBaseBody, electronicsBaseCavity, 0,
  electronicsBaseCavityPlacement );

  // cylinder to subtract to form holes for HV:
  G4Tubs *electronicsBaseHole = new G4Tubs( "electronicsBaseHole", 0,
  fEBHoleRadius, fEBHeight, 0, twopi );

  electronicsBase = new G4SubtractionSolid("electronicsBaseWithSignalHole",
  electronicsBase, electronicsBaseHole, 0, G4ThreeVector() );

  G4double electronicsBaseHVHoleX = hvPogoX;

  G4ThreeVector electronicsBaseHVHolePlacement(electronicsBaseHVHoleX, 0, 0);

  electronicsBase = new G4SubtractionSolid("electronicsBaseWithHVHole",
  electronicsBase, electronicsBaseHole, 0, electronicsBaseHVHolePlacement );

  G4double ebdPhi = 20.0*degree;

  G4Tubs *ebSignalHole = new G4Tubs( "ebSignalHole", 0,
  fConnectorInsulatorSmallOuterRadius + eps, fEBHeight, 0, twopi);

  // first hole:
  G4double ebPhi = 0;
  G4double ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  G4double ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  G4ThreeVector ebHolePlacement(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase0", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  G4ThreeVector ebPinPlacement( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin0", theDetectorLogical, false, 0);
  G4ThreeVector ebPinInsulatorPlacement( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical0", theDetectorLogical,
  false, 0);

  // second hole:
  ebPhi = ebPhi + ebdPhi;
  ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  ebHolePlacement = G4ThreeVector(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase1", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  ebPinPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin1", theDetectorLogical, false, 0);
  ebPinInsulatorPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical1", theDetectorLogical,
  false, 0);

  // third hole:
  ebPhi = ebPhi + ebdPhi;
  ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  ebHolePlacement = G4ThreeVector(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase2", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  ebPinPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin2", theDetectorLogical, false, 0);
  ebPinInsulatorPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical2", theDetectorLogical,
  false, 0);

  // fourth hole:
  ebPhi = ebPhi + ebdPhi;
  ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  ebHolePlacement = G4ThreeVector(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase3", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  ebPinPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin3", theDetectorLogical, false, 0);
  ebPinInsulatorPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical3", theDetectorLogical,
  false, 0);

  // fifth hole:
  ebPhi = ebPhi + ebdPhi;
  ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  ebHolePlacement = G4ThreeVector(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase4", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  ebPinPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin4", theDetectorLogical, false, 0);
  ebPinInsulatorPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical4", theDetectorLogical,
  false, 0);

  // sixth hole:
  ebPhi = ebPhi + ebdPhi;
  ebHoleX = -fEBMillMaxHoleDistance*TMath::Cos(ebPhi);
  ebHoleY = fEBMillMaxHoleDistance*TMath::Sin(ebPhi);
  ebHolePlacement = G4ThreeVector(ebHoleX, ebHoleY, -fEBHeight/2.0 );
  electronicsBase = new G4SubtractionSolid( "eBase5", electronicsBase,
  ebSignalHole, 0, ebHolePlacement );
  ebPinPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinZ);
  fDHPinPhysical = new G4PVPlacement( 0, ebPinPlacement, clippedDHPinLogical,
  "ebDHPin5", theDetectorLogical, false, 0);
  ebPinInsulatorPlacement = G4ThreeVector( ebHoleX, ebHoleY, ebPinInsulatorZ );
  fConnectorInsulatorPhysical = new G4PVPlacement(0, ebPinInsulatorPlacement,
  fConnectorInsulatorLogical, "connectorInsulatorPhysical5", theDetectorLogical,
  false, 0);


  fEBLogical = new G4LogicalVolume( electronicsBase, Copper,
  "electronicsBaseLogical", 0, 0, 0, true );

  fEBPhysical = new G4PVPlacement(0, electronicsBasePlacement, fEBLogical,
  "electronicsBasePhysical", theDetectorLogical, false, 0 );

  //------------------------------ FET BLOCK --------------------------------//
  //-------------------------------------------------------------------------//

  G4Tubs *fetBlockBody = new G4Tubs("fetBlockBody", 0, fFetBlockOuterRadius,
  fFetBlockHeight/2.0, 0, fCylinderAngle );

  G4Tubs *fetBlockCavity = new G4Tubs("fetBlockBody", 0, fFetBlockInnerRadius,
  fFetBlockHeight, 0, twopi );

  G4double fetBlockCavityZ = fFetBlockHeight/2.0 + fFetBlockBottomThickness;

  G4ThreeVector fetBlockCavityPlacement( 0, 0, fetBlockCavityZ );

  G4SubtractionSolid *fetBlock = new G4SubtractionSolid( "fetBlock",
  fetBlockBody, fetBlockCavity, 0, fetBlockCavityPlacement );

  fFetBlockLogical = new G4LogicalVolume (fetBlock, Copper, "fetBlockLogical",
  0, 0, 0, true );

  fFetBlockPhysical = new G4PVPlacement(0, fetBlockPlacement, fFetBlockLogical,
  "fetBlockPhysical", theDetectorLogical, false, 0 );

  //------------------------ THREADED COLLAR --------------------------------//
  //-------------------------------------------------------------------------//

  G4Tubs *threadedCollar = new G4Tubs( "threadedCollar",
  fThreadedCollarInnerRadius, fThreadedCollarOuterRadius,
  fThreadedCollarHeight/2.0, 0, fCylinderAngle );

  fThreadedCollarLogical = new G4LogicalVolume( threadedCollar, Copper,
  "threadedCollarLogical", 0, 0, 0, true );

  fThreadedCollarPhysical = new G4PVPlacement(0, threadedCollarPlacement,
  fThreadedCollarLogical, "threadedCollarPhysical", theDetectorLogical, false, 0
  );

  //------------------------ RDC COLD FINGER --------------------------------//
  // from CANBERRA drawing 7064064
  //-------------------------------------------------------------------------//

  G4Tubs *rdcColdFinger = new G4Tubs( "rdcColdFinger", 0, fRdcColdFingerRadius,
  fRdcColdFingerLength/2.0, 0, fCylinderAngle );

  G4Tubs *shortColdFingerCut = new G4Tubs("shortColdFingerCut",
  fRdcColdFingerCutRadius, fRdcColdFingerRadius + 1*inch,
  fRdcColdFingerCutLength, 0, twopi );

  G4double shortCFCutZ = fRdcColdFingerLength/2.0; 

  G4ThreeVector shortCFCutPlacement( 0, 0, shortCFCutZ ); 

  G4SubtractionSolid *shortColdFingerWithCut = new G4SubtractionSolid(
  "shortColdFingerWithCut", rdcColdFinger, shortColdFingerCut, 0,
  shortCFCutPlacement );

  fRdcColdFingerLogical = new G4LogicalVolume( shortColdFingerWithCut, Copper,
  "rdcColdFinger", 0, 0, 0, true );

  fRdcColdFingerPhysical = new G4PVPlacement( 0, rdcColdFingerPlacement,
  fRdcColdFingerLogical, "rdcColdFingerPhysical", theDetectorLogical, false, 0);


  //--------------------------------------------------------------------
  // zeolite package
  //--------------------------------------------------------------------

  G4Box* zeolitePackage = new G4Box( "zeolitePackage", fZeoliteWidth/2.0,
  fZeoliteWidth/2.0, fZeoliteWidth/2.0 );

  // FIXME -- material should be zeolite:
  G4LogicalVolume* zeoliteLogical = new G4LogicalVolume( zeolitePackage, Teflon,
  "zeolitePackage", 0, 0, 0, true );

 /* G4PVPlacement* zeolitePhysical =*/ new G4PVPlacement( 0, zeolitePlacement,
  zeoliteLogical, "zeolitePhysical", theDetectorLogical, false, 0 );

  //--------------------------- CRYOSTAT ------------------------------------//
  // from CANBERRA drawing
  //-------------------------------------------------------------------------//

  // origin is in o-ring flange -- the part that fits into the endcap

  // body of the o-ring flange:
  G4Tubs *cryostatOringFlangeCylinder = new G4Tubs(
  "cryostatOringFlangeCylinder", fCryostatNeckOuterRadius - eps,
  fCryostatOringFlangeOuterRadius - eps/2.0, fCryostatOringFlangeThickness/2.0,
  0, fCylinderAngle );

  // circular cut in the flange:
  G4Tubs *cryostatOringFlangeCut = new G4Tubs ("cryostatOringFlangeCut", 0,
  fCryostatOringFlangeCutRadius, fCryostatOringFlangeCutHeight, 0, twopi );

  G4ThreeVector oringFlangeCutPlacement(0,0, fCryostatOringFlangeThickness/2.0);

  // flange body with cut:
  G4SubtractionSolid* cryostatOringFlange = new G4SubtractionSolid(
  "cryostatOringFlange", cryostatOringFlangeCylinder, cryostatOringFlangeCut, 0,
  oringFlangeCutPlacement );

  // short cold finger neck
  G4Tubs *cryostatShortNeck = new G4Tubs( "cryostatShortNeck",
  fCryostatNeckOuterRadius - fCryostatNeckThickness, fCryostatNeckOuterRadius,
  fCryostatShortNeckLength/2.0 + eps, 0, fCylinderAngle );

  G4UnionSolid *cryostatWithShortNeck = new G4UnionSolid( "cryostatWithShortNeck",
  cryostatOringFlange, cryostatShortNeck, 0, shortNeckPlacement );

  // flange at end of short neck, opposite detector:
  G4Tubs *cryostatFlange = new G4Tubs("cryostatFlange", fCryostatNeckOuterRadius
  -eps, fCryostatFlangeOuterRadius, fCryostatFlangeHeight/2.0, 0, fCylinderAngle );

  // cylider to use for subtraction:
  G4Tubs *cryostatFlangeCavity = new G4Tubs("cryostatFlangeCavity", 0,
  fCryostatFlangeInnerRadius, fCryostatFlangeHeight, 0, twopi );

  G4double cryostatFlangeCavityZ = -fCryostatFlangeHeight/2.0 -
  fCryostatFlangeTopThickness;

  G4ThreeVector cryostatFlangeCavityPlacement(0, 0, cryostatFlangeCavityZ );

  G4SubtractionSolid *cryostatFlangeWithCavity = new G4SubtractionSolid(
  "cryostatFlangeWithCavity", cryostatFlange, cryostatFlangeCavity, 0,
  cryostatFlangeCavityPlacement );

  G4UnionSolid *cryostatWithNeckAndFlange = new G4UnionSolid(
  "cryostatWithNeckAndFlange", cryostatWithShortNeck, cryostatFlangeWithCavity,
  0, cryostatFlangePlacement );

  fCryostatLogical = new G4LogicalVolume( cryostatWithNeckAndFlange,
  StainlessSteel, "cryostatNeckAndFlangeLogical", 0, 0, 0, true );

  fCryostatPhysical = new G4PVPlacement( 0, cryostatPlacement, fCryostatLogical,
  "cryostatNeckAndFlangePhysical", theDetectorLogical, false, 0 );

  //-------------------------- SPIDER TRESTLE -------------------------------//
  //-------------------------------------------------------------------------//

  G4Tubs *spiderTrestleSolid = new G4Tubs("spiderTrestleSolid",
  fSpiderTrestleInnerRadius, fSpiderTrestleOuterRadius,
  fSpiderTrestleHeight/2.0, 0, fCylinderAngle );

  // Create large and small holes in spider trestle.  The large and small holes
  // are opposite each other; join two cylinders and use them as subtraction
  // solids to form the holes.  Rotate and subtract to get all three holes. Make
  // the cylinder length twice the radius of the trestle to ensure clean subtraction.

  // cylinder to use for small hole
  G4Tubs *stSmallHole = new G4Tubs ("stSmallHole", 0,
  fSpiderTrestleSmallHoleRadius, fSpiderTrestleOuterRadius, 0, twopi );

  // cylinder to make large hole
  G4Tubs *stLargeHole = new G4Tubs ("stLargeHole", 0,
  fSpiderTrestleLargeHoleRadius, fSpiderTrestleOuterRadius, 0, twopi );

  // large hole offset

  G4double stLgHoleX = fSpiderTrestleHeight/2.0 - fSpiderTrestleLargeHoleRadius
  - fSpiderTrestleLargeHoleDistance;

  G4double stLgHoleZ = -fSpiderTrestleOuterRadius*2.0 + eps;

  G4ThreeVector stLgHolePlacement( stLgHoleX, 0, stLgHoleZ );

  // join two cylinders to make hole-forming shape:
  G4UnionSolid *spiderTrestleHoles = new G4UnionSolid("spiderTrestleHoles",
  stSmallHole, stLargeHole, 0, stLgHolePlacement );

  // there are three sets of large and small holes, offset by 2*pi/3.  specify
  // horizontal displacement and angle phi:

  G4double stHoleHorizontalOffset = fSpiderTrestleOuterRadius;

  G4double phi = 0;
  G4RotationMatrix *stHoleRotation = new G4RotationMatrix;
  stHoleRotation->rotateY(pi/2.0);

  G4double stHoleX = -stHoleHorizontalOffset*TMath::Cos(phi);
  G4double stHoleY = stHoleHorizontalOffset*TMath::Sin(phi);

  G4ThreeVector stCavityPlacement(stHoleX, stHoleY, 0 );

  // subtract two cylinders from the trestle:
  G4SubtractionSolid *spiderTrestle = new G4SubtractionSolid("spiderTrestle1",
  spiderTrestleSolid, spiderTrestleHoles, stHoleRotation, stCavityPlacement );

  phi = twopi/3;
  stHoleX = -stHoleHorizontalOffset*TMath::Cos(phi);
  stHoleY = stHoleHorizontalOffset*TMath::Sin(phi);
  G4ThreeVector stCavityPlacementTwo(stHoleX, stHoleY, 0 );

  G4RotationMatrix *stHoleRotationTwo = new G4RotationMatrix;
  stHoleRotationTwo->rotateZ(phi); // rotate around z-axis first
  stHoleRotationTwo->rotateY(pi/2.0);

  // reuse spiderTrestleHoles, subtract two cylinders from the trestle a 2nd
  // time:
  G4SubtractionSolid *spiderTrestleTwo = new G4SubtractionSolid("spiderTrestle2",
  //spiderTrestleSolid, spiderTrestleHoles, stHoleRotationTwo,
  spiderTrestle, spiderTrestleHoles, stHoleRotationTwo,
  stCavityPlacementTwo);
  
  phi = 2*twopi/3;
  stHoleX = -stHoleHorizontalOffset*TMath::Cos(phi);
  stHoleY = stHoleHorizontalOffset*TMath::Sin(phi);
  G4ThreeVector stCavityPlacementThree(stHoleX, stHoleY, 0 );

  G4RotationMatrix *stHoleRotationThree = new G4RotationMatrix;
  stHoleRotationThree->rotateZ(phi); // rotate around z-axis first
  stHoleRotationThree->rotateY(pi/2.0);

  // reuse spiderTrestleHoles, subtract two cylinders from the trestle a 3rd
  // time:
  G4SubtractionSolid *spiderTrestleThree = new G4SubtractionSolid("spiderTrestle3",
  spiderTrestleTwo, spiderTrestleHoles, stHoleRotationThree,
  stCavityPlacementThree);

  fSpiderTrestleLogical = new G4LogicalVolume( spiderTrestleThree, Copper,
  "spiderTrestleLogical", 0, 0, 0, true );

  fSpiderTrestlePhysical = new G4PVPlacement( 0, spiderTrestlePlacement,
  fSpiderTrestleLogical, "spiderTrestlePhysical", theDetectorLogical, false, 0
  );

  //-------------------------- POLY SPIDER -----------------------------
  //--------------------------------------------------------------------

  /*
  The poly spider consists of 4 concentric annular rings.  This geometry is a
  guess based on assembly drawings.  Use the same dimension for ring spacing and
  ring thickness -- 3 vacuum gaps between the rings; 4 + 3 = 7.
  */

  G4double polyAnnulusSpacing = (fPolySpiderOuterRadius - fPolySpiderInnerRadius
  ) / 7.0;

  // concentric annulus parts must be connected by something, use a long thin
  // box for this:

  G4ThreeVector centerPlacement(0, 0, 0);

  G4Box *polySpiderBox = new G4Box("polySpiderBox", fPolySpiderOuterRadius -
  eps, polyAnnulusSpacing, fPolySpiderHeight/2.0 - 1*mm);

  // make the first annulus, join it to the box:

  G4double outerRadius = fPolySpiderOuterRadius;
  G4double innerRadius = outerRadius - polyAnnulusSpacing; 

  G4Tubs *polySpiderAnnulus = new G4Tubs("polySpiderAnnulus1", innerRadius,
  outerRadius, fPolySpiderHeight/2.0, 0, fCylinderAngle );

  G4UnionSolid *polySpider = new G4UnionSolid("polySpider1", polySpiderBox,
  polySpiderAnnulus, 0, centerPlacement );

  // make the second annulus, join it to the box:

  outerRadius = innerRadius - polyAnnulusSpacing;
  innerRadius = outerRadius - polyAnnulusSpacing; 

  polySpiderAnnulus = new G4Tubs("polySpiderAnnulus2", innerRadius, outerRadius,
  fPolySpiderHeight/2.0, 0, fCylinderAngle );

  polySpider = new G4UnionSolid("polySpider2", polySpider, polySpiderAnnulus, 0,
  centerPlacement );

  // make the third annulus, join it to the box:

  outerRadius = innerRadius - polyAnnulusSpacing;
  innerRadius = outerRadius - polyAnnulusSpacing; 

  polySpiderAnnulus = new G4Tubs("polySpiderAnnulus3", innerRadius, outerRadius,
  fPolySpiderHeight/2.0, 0, fCylinderAngle );

  polySpider = new G4UnionSolid("polySpider3", polySpider, polySpiderAnnulus, 0,
  centerPlacement );

  // make the fourth annulus, join it to the box:

  outerRadius = innerRadius - polyAnnulusSpacing;
  innerRadius = 0; // zero radius so we can cut out a hole later 

  polySpiderAnnulus = new G4Tubs("polySpiderAnnulus4", innerRadius, outerRadius,
  fPolySpiderHeight/2.0, 0, fCylinderAngle );

  polySpider = new G4UnionSolid("polySpider4", polySpider, polySpiderAnnulus, 0,
  centerPlacement );

  // make the inner hole:

  G4Tubs *polySpiderHole = new G4Tubs("polySpiderHole", 0,
  fPolySpiderInnerRadius, fPolySpiderHeight, 0, twopi );

  G4SubtractionSolid *polySpiderWithHole = new G4SubtractionSolid(
  "polySpiderWithHole", polySpider, polySpiderHole, 0, centerPlacement );

  fPolySpiderLogical = new G4LogicalVolume(polySpiderWithHole, Teflon,
  "polySpiderLogical", 0, 0, 0, true );

  fPolySpiderPhysical = new G4PVPlacement(0, polySpiderPlacement,
  fPolySpiderLogical, "polySpiderPhysical", theDetectorLogical, false, 0 );
  //-------------------------- STAND TOP PLATE ------------------------------//
  //-------------------------------------------------------------------------//

  G4Box *standTopPlateSolid = new G4Box("standTopPlateSolid",
  fStandTopPlateLength/2.0, fStandTopPlateLength/2.0,
  fStandTopPlateThickness/2.0 );

  G4Tubs *standTopPlateHole = new G4Tubs("standTopPlateHole", 0,
  fStandTopPlateHoleRadius, fStandTopPlateThickness, 0, twopi );

  G4SubtractionSolid *standTopPlate = new G4SubtractionSolid("standTopPlate",
  standTopPlateSolid, standTopPlateHole, 0, G4ThreeVector(0,0,0) );

  fStandTopPlateLogical = new G4LogicalVolume( standTopPlate, Aluminum,
  "standTopPlateLogical", 0, 0, 0, true );

  fStandTopPlatePhysical = new G4PVPlacement( 0, standTopPlatePlacement,
  fStandTopPlateLogical, "standTopPlatePhysical", theDetectorLogical, false, 0
  );
 
  //--------------------------------------------------------------------
  // RESISTOR
  //--------------------------------------------------------------------

  G4Tubs* resistor = new G4Tubs("resistor", 0, fResistorRadius,
  fResistorLength/2.0, 0, fCylinderAngle );

  // FIXME -- material:

  fResistorLogical = new G4LogicalVolume( resistor, Teflon,
  "fResistorLogical", 0, 0, 0, true );

  //-------------------------- PENTA ASSY ----------------------------------//
  //-------------------------------------------------------------------------//

  // plug:
  G4Tubs *pentaPlugBody = new G4Tubs("pentaPlugBody", fPentaPlugInnerRadius,
  fPentaPlugOuterRadius, fPentaPlugHeight/2.0, 0, fCylinderAngle );

  // plug step, for subtraction:
  G4Tubs *pentaPlugStep = new G4Tubs("pentaPlugStep",
  fPentaPlugSmallOuterRadius, fPentaPlugOuterRadius*2.0, fPentaPlugHeight/2.0, 0,
  twopi );

  G4ThreeVector pentaPlugStepPlacement(0, 0, fPentaPlugStepHeight);

  G4SubtractionSolid *pentaPlug = new G4SubtractionSolid( "pentaPlug",
  pentaPlugBody, pentaPlugStep, 0, pentaPlugStepPlacement );

  // receptacle:
  G4Tubs *pentaReceptacleBody = new G4Tubs("pentaReceptacleBody",
  fPentaPlugInnerRadius, fPentaReceptacleOuterRadius,
  fPentaReceptacleHeight/2.0, 0, fCylinderAngle );

  G4ThreeVector heaterResistorPlacement(0, 0, resistorZ);

  fHeaterResistorPhysical = new G4PVPlacement( 0, heaterResistorPlacement,
  fResistorLogical, "fHeaterResistorPhysical", theDetectorLogical, false, 0 );

  // receptacle step, for subtraction:
  G4Tubs *pentaReceptacleStep = new G4Tubs("pentaReceptacleStep",
  fPentaReceptacleSmallOuterRadius, fPentaPlugOuterRadius*2.0,
  fPentaReceptacleMiddleLength/2.0, 0, twopi );

  G4SubtractionSolid *pentaReceptacle = new G4SubtractionSolid(
  "pentaReceptacle0", pentaReceptacleBody, pentaReceptacleStep, 0, G4ThreeVector());

  // make 5 holes in penta plug and receptacle for mill max pin + receptacle:

  G4double pinAndReceptacleZ = pentaReceptacleZ + fPentaReceptacleHeight/2.0 +
  eps - fMillMaxReceptacleLength/2.0 + fMillMaxReceptacleTopStepLength;

  // first hole:
  G4double pentaPhi = 0;
  G4double pentaHoleX = -fPentaPlugHoleSpacing*TMath::Cos(pentaPhi);
  G4double pentaHoleY = fPentaPlugHoleSpacing*TMath::Sin(pentaPhi);
  G4ThreeVector pentaHolePlacement(pentaHoleX, pentaHoleY, 0 );
  pentaPlug = new G4SubtractionSolid( "pentaPlug0", pentaPlug,
  millMaxPressFitHole, 0, pentaHolePlacement );
  pentaReceptacle = new G4SubtractionSolid( "pentaReceptacle0",
  pentaReceptacle, millMaxPressFitHole, 0, pentaHolePlacement);

  // the first hole in the plug contains a receptacle:

  G4ThreeVector millMaxPlacement(pentaHoleX, pentaHoleY, millMaxZ );

  G4RotationMatrix *receptacleRot = new G4RotationMatrix;
  receptacleRot->rotateY(pi);

  fMillMaxReceptaclePhysical = new G4PVPlacement( receptacleRot,
  millMaxPlacement, fMillMaxReceptacleLogical, "millMaxReceptaclePhysical",
  theDetectorLogical, false, 0 ); 

  G4ThreeVector pinAndReceptaclePlacement( pentaHoleX, pentaHoleY,
  pinAndReceptacleZ);

  G4ThreeVector injectorResistorPlacement(pentaHoleX, pentaHoleY, resistorZ);

  fInjectorResistorPhysical = new G4PVPlacement(0, injectorResistorPlacement,
  fResistorLogical, "fInjectorResistorPhysical", theDetectorLogical, false, 0 );

  // second hole:
  pentaPhi = pentaPhi + twopi/5; 
  pentaHoleX = -fPentaPlugHoleSpacing*TMath::Cos(pentaPhi);
  pentaHoleY = fPentaPlugHoleSpacing*TMath::Sin(pentaPhi);
  pentaHolePlacement = G4ThreeVector(pentaHoleX, pentaHoleY, 0 );
  pentaPlug = new G4SubtractionSolid( "pentaPlug1", pentaPlug, millMaxPressFitHole, 0,
  pentaHolePlacement );
  pentaReceptacle = new G4SubtractionSolid( "pentaReceptacle1", pentaReceptacle,
  millMaxPressFitHole, 0, pentaHolePlacement );

  pinAndReceptaclePlacement = G4ThreeVector( pentaHoleX, pentaHoleY,
  pinAndReceptacleZ);
  fDHPinInReceptaclePhysical = new G4PVPlacement( 0, pinAndReceptaclePlacement,
  fDHPinInReceptacleLogical, "fDHPinInReceptaclePhysical1", theDetectorLogical,
  false, 0);

  // third hole:
  pentaPhi = pentaPhi + twopi/5; 
  pentaHoleX = -fPentaPlugHoleSpacing*TMath::Cos(pentaPhi);
  pentaHoleY = fPentaPlugHoleSpacing*TMath::Sin(pentaPhi);
  pentaHolePlacement = G4ThreeVector(pentaHoleX, pentaHoleY, 0 );
  pentaPlug = new G4SubtractionSolid( "pentaPlug1", pentaPlug, millMaxPressFitHole, 0,
  pentaHolePlacement );
  pentaReceptacle = new G4SubtractionSolid( "pentaReceptacle1", pentaReceptacle,
  millMaxPressFitHole, 0, pentaHolePlacement );

  pinAndReceptaclePlacement = G4ThreeVector( pentaHoleX, pentaHoleY,
  pinAndReceptacleZ);
  fDHPinInReceptaclePhysical = new G4PVPlacement( 0, pinAndReceptaclePlacement,
  fDHPinInReceptacleLogical, "fDHPinInReceptaclePhysical2", theDetectorLogical,
  false, 0);

  // fourth hole:
  pentaPhi = pentaPhi + twopi/5; 
  pentaHoleX = -fPentaPlugHoleSpacing*TMath::Cos(pentaPhi);
  pentaHoleY = fPentaPlugHoleSpacing*TMath::Sin(pentaPhi);
  pentaHolePlacement = G4ThreeVector(pentaHoleX, pentaHoleY, 0 );
  pentaPlug = new G4SubtractionSolid( "pentaPlug1", pentaPlug, millMaxPressFitHole, 0,
  pentaHolePlacement );
  pentaReceptacle = new G4SubtractionSolid( "pentaReceptacle1", pentaReceptacle,
  millMaxPressFitHole, 0, pentaHolePlacement );

  pinAndReceptaclePlacement = G4ThreeVector( pentaHoleX, pentaHoleY,
  pinAndReceptacleZ);

  if( pentaPhi <= fCylinderAngle ){

    fDHPinInReceptaclePhysical = new G4PVPlacement( 0,
    pinAndReceptaclePlacement, fDHPinInReceptacleLogical,
    "fDHPinInReceptaclePhysical3", theDetectorLogical, false, 0);

  }

  // fifth hole:
  pentaPhi = pentaPhi + twopi/5; 
  pentaHoleX = -fPentaPlugHoleSpacing*TMath::Cos(pentaPhi);
  pentaHoleY = fPentaPlugHoleSpacing*TMath::Sin(pentaPhi);
  pentaHolePlacement = G4ThreeVector(pentaHoleX, pentaHoleY, 0 );
  pentaPlug = new G4SubtractionSolid( "pentaPlug1", pentaPlug, millMaxPressFitHole, 0,
  pentaHolePlacement );
  pentaReceptacle = new G4SubtractionSolid( "pentaReceptacle1", pentaReceptacle,
  millMaxPressFitHole, 0, pentaHolePlacement );

  pinAndReceptaclePlacement = G4ThreeVector( pentaHoleX, pentaHoleY,
  pinAndReceptacleZ);

  if( pentaPhi <= fCylinderAngle ){

    fDHPinInReceptaclePhysical = new G4PVPlacement( 0,
    pinAndReceptaclePlacement, fDHPinInReceptacleLogical,
    "fDHPinInReceptaclePhysical4", theDetectorLogical, false, 0);
  }

  fPentaPlugLogical = new G4LogicalVolume(pentaPlug, Teflon,
  "pentaPlugLogical", 0, 0, 0, true );

  fPentaPlugPhysical = new G4PVPlacement(0, pentaPlugPlacement,
  fPentaPlugLogical, "pentaPlugPhysical", theDetectorLogical, false, 0 );

  fPentaReceptacleLogical = new G4LogicalVolume(pentaReceptacle, Teflon,
  "pentaPlugLogical", 0, 0, 0, true );  

  fPentaReceptaclePhysical = new G4PVPlacement(0, pentaReceptaclePlacement,
  fPentaReceptacleLogical, "pentaReceptaclePhysical", theDetectorLogical, false,
  0 );

  //--------------------------------------------------------------------
  // FET
  //--------------------------------------------------------------------

  G4Box* fet = new G4Box( "fet", fFETLength/2.0, fFETWidth/2.0, fFETLength/2.0 );

  // FIXME -- material:
  fFETLogical = new G4LogicalVolume( fet, Teflon, "fFETLogical", 0, 0, 0, true
  );

  fFETPhysical = new G4PVPlacement(0, fetPlacement, fFETLogical, "fFETPhysical",
  theDetectorLogical, false, 0 );


  //----------------------------- CRYSTAL -----------------------------------//
  //-------------------------------------------------------------------------//

  MGGeometryBEGeDetector kurfBEGe("kurfBEGe");
  kurfBEGe.SetHeight(fCrystalHeight);
  kurfBEGe.SetRadius(fCrystalRadius);
  kurfBEGe.SetDitchOuterRadius(fDitchRadius);
  kurfBEGe.SetDitchInnerRadius(fDitchRadius - fDitchWidth);
  kurfBEGe.SetDitchDepth(fDitchDepth);
  kurfBEGe.SetG4MaterialName("Germanium-Nat");
  kurfBEGe.ConstructDetector();
  G4LogicalVolume* crystalLogical = kurfBEGe.GetDetectorLogical();

  					//PLACEMENT
  fCrystalPhysical
    = new G4PVPlacement(0, xtalPlacement, crystalLogical,
    "ActiveCrystal0CrystalColumn0", theDetectorLogical, false, 0);





  //*************************************************************************//
  //------------------------VISUALIZATION ATTRIBUTES-------------------------//
  //-------------------------------------------------------------------------//

  G4VisAttributes* grey = new G4VisAttributes(G4Color(0.6,0.6,0.6,.5)); 
  //G4VisAttributes* transparentGrey = new G4VisAttributes(G4Color(0.6,0.6,0.6,.2)); 
  G4VisAttributes* redva = new G4VisAttributes(G4Color(1.0,0,0,.9));
  //G4VisAttributes* darkgrey = new G4VisAttributes(G4Color(0.9,0.9,0.9,.2));
  G4VisAttributes* magentava = new G4VisAttributes(G4Color(1,0,1));
  G4VisAttributes* purple = new G4VisAttributes(G4Color(0.41,0,0.6));
  G4VisAttributes* blueva = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* greenva = new G4VisAttributes(G4Colour(0.0,1,0.0));
  G4VisAttributes* yellowva = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* cyanva = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes* orange = new G4VisAttributes(G4Colour(1.0,0.3,0.0));
  G4VisAttributes* blackva = new G4VisAttributes(G4Colour(0.0,0.0,0.0));

  theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
  //theDetectorLogical->SetVisAttributes(transparentGrey);

  crystalLogical->SetVisAttributes(grey);

  fEndCapLogical->SetVisAttributes(cyanva);

  fTeflonCupLogical->SetVisAttributes(redva);
  fTeflonDiskLogical->SetVisAttributes(redva);

  fCopperCupLogical->SetVisAttributes(greenva);
  fCopperLidLogical->SetVisAttributes(greenva);

  fRdcColdFingerLogical->SetVisAttributes(blueva);

  fCryostatLogical->SetVisAttributes(magentava);

  fEBLogical->SetVisAttributes(yellowva);

  fSpiderTrestleLogical->SetVisAttributes(orange);

  fDetectorSpacerLogical->SetVisAttributes(blueva);

  fThreadedCollarLogical->SetVisAttributes(redva);

  fFetBlockLogical->SetVisAttributes(purple);

  fHVPogoPlungerLogical->SetVisAttributes(orange);
  fHVPogoBarrelLogical->SetVisAttributes(orange);
  clippedDHPinLogical->SetVisAttributes(orange);

  fMillMaxReceptacleLogical->SetVisAttributes(orange);
  fDHPinLogical->SetVisAttributes(orange);
  fDHPinInReceptacleLogical->SetVisAttributes(orange);

  fSupportPostLogical->SetVisAttributes(magentava);

  fCompressionSpringLogical->SetVisAttributes(orange);

  fFETLogical->SetVisAttributes(blackva);

  fResistorLogical->SetVisAttributes(blueva);

  if ( fUsePbPatches ) {
    fPbPatchOneLogical->SetVisAttributes(purple);
    fPbPatchTwoLogical->SetVisAttributes(purple);
  }

  centerContactSolderLogical->SetVisAttributes(cyanva);
  hvSolderLogical->SetVisAttributes(cyanva);
  solderBlobLogical->SetVisAttributes(cyanva);

  zeoliteLogical->SetVisAttributes(yellowva);

  //-------------------------------------------------------------------------//
}


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

void MJGeometryKURFBEGeDetector::SetDefaults()
{ 
 
  G4double eps = 0.001 * mm;

  //--------------------------------------------------------------------
  // end cap
  // from CANBERRA C200510 end cap drawing
  //--------------------------------------------------------------------


  //Cryostat dimensions
  fEndCapHeight = 5.25*inch;            //CANBERRA C200510 end cap drawing

  // 10 Nov 2011 -- difference between inner and outer radius matches wall
  // thickness measured at CANBERRA:
  fEndCapInnerRadius = 3.126/2.0*inch;  //CANBERRA C200510 end cap drawing
  fEndCapOuterRadius = 3.25/2.0*inch;   //CANBERRA C200510 end cap drawing

  fEndCapFlatThickness = 0.050*inch;    // updated 10 Nov 2011 to match measurements

  fTopOfCryostatZ = 0; 


  //--------------------------------------------------------------------
  //Crystal dimensions
  //--------------------------------------------------------------------

  fCrystalOffset = 5. * mm;             //Need these dimensions of 
                                        // crystal placement within cryostat can
  fCrystalHeight = 30 * mm;

  // 10 Nov 2011 -- diameter was measured at CANBERRA as 2.396" with Cu foil
  // wrapped around; about 0.010" larger than this value -- seems consistent:
  fCrystalRadius = 60.6 / 2.0 * mm;     //-- from purchase order

  fDitchRadius = 1.187/2 * in;          //-- from purchase order -- outer radius
  fDitchWidth = 0.25*inch;              // Canberra dwg 7065004
  fDitchDepth = 0.08*inch;              // Canberra dwg 7065004 

  //--------------------------------------------------------------------
  // Copper cup
  // from CANBERRA LEGe/BEGe holder B201784X and Collar LEGe-BEGe 
  // Holder dwgs
  // updated 10 Nov 2011 with measurements from opening of cryostat
  //--------------------------------------------------------------------

  // copper cup main body:
  fCopperCupOuterRadius = 2.532/2.0*inch; // 10 Nov 2011 -- matches measured value
  fCopperCupInnerRadius = 2.468/2.0*inch;
  fCopperCupBottomThickness = 0.06*inch;
  fCopperCupHeight = 1.435*inch;  // updated 10 Nov 2011 

  fCopperCupHoleRadius = 0.927/2.0*inch;  // was measured as 0.925" at CANBERRA; leaving as specified in drawing

  // annular set screw feature:
  fCopperCupSetScrewFeatureOuterRadius = 2.75/2.0*inch;  // 10 Nov 2011 -- matches measured value
  fCopperCupSetScrewFeatureHeight = 0.34*inch;
  fCopperCupSetScrewFeatureOffsetFromTop = 0.06*inch;

  // bottom annular feature, for attaching to cold finger:
  fCopperCupBottomAnnulusHeight = 0.25*inch;
  fCopperCupBottomAnnulusOuterRadius = 1.560/2.0*inch;
  fCopperCupBottomAnnulusInnerRadius = 1.375/2.0*inch;


  // true dimension is 0.180"; adjusted  because 0.182" OD piece is
  // press fit inside:
  fCopperCupContactHoleRadius = 0.182/2.0*inch + eps; 

  fCopperCupContactHoleDistance = 0.593*inch;

  //--------------------------------------------------------------------
  // Copper lid
  //--------------------------------------------------------------------

  // 10 Nov 2011 updated the following 3 dimensions with measurements from
  // opening of cryostat:
  fCopperLidOuterRadius = 2.91/2.0*inch;
  fCopperLidHeight = 0.605*inch;
  fCopperLidFlatBaseThickness = 0.030*inch; 

  // this was measured as 2.745" when cryostat was open; leaving as 2.75+ to
  // avoid interference with Cu cup
  fCopperLidInnerRadius = fCopperCupSetScrewFeatureOuterRadius + eps;

  //--------------------------------------------------------------------
  // Teflon cup
  // from CANBERRA teflon disc and Collar LEGe-BEGe Holder dwgs
  //--------------------------------------------------------------------

  // teflon disk -- in reality this is two disks
  fTeflonDiskThickness = 0.022*inch;    // updated 10 Nov 2011 from measurements at CANBERRA

  // teflon cup
  fTeflonCupOuterRadius = 2.464/2.0*inch;   // from Collar, CANBERRA dwgs
  fTeflonCupInnerRadius = 2.422/2.0*inch;   // from Collar dwg

  // updated 10 Nov 2011 to fit in copper cup:
  fTeflonCupHeight = fCopperCupHeight - fCopperCupBottomThickness - eps -
  fTeflonDiskThickness - 2.0 * eps;  

  fTeflonCupBottomThickness = 0.062*inch; // from Collar dwg  
  fTeflonCupEndCylinderRadius = 0.245/2.0*inch;
  fTeflonCupEndCylinderHeight = 0.040*inch; 
  fTeflonCupHoleRadius = 0.120/2.0*inch;

  G4double heightOfSpaceInTeflonCup = fTeflonCupHeight - fCrystalHeight -
  fTeflonCupBottomThickness;

  MGLog(routine) << "space in teflon cup: " << heightOfSpaceInTeflonCup/inch 
  << " inches" << endlog;

  //--------------------------------------------------------------------
  // electronics base
  // digitized from CANBERRA chamber assembly drawing
  //--------------------------------------------------------------------

  fEBInnerRadius = fCopperCupBottomAnnulusOuterRadius + eps;
  fEBOuterRadius = 1.8/2.0*inch;
  fEBHeight = 1.6*inch;
  fEBFlatTopThickness = 0.15*inch;
  fEBHoleRadius = 0.182/2.0*inch + eps; // estimate from HV contact extension radius
  fEBMillMaxHoleDistance = 0.60*inch; // digitized from 7062591

  //--------------------------------------------------------------------
  // threaded collar
  // digitized from CANBERRA chamber assembly drawings except inner radius
  //--------------------------------------------------------------------

  fThreadedCollarInnerRadius = 0.755/2.0*inch; // parts list of chamber assy dwg
  fThreadedCollarOuterRadius = 1.0/2.0*inch;
  fThreadedCollarHeight = 0.73*inch;

  //--------------------------------------------------------------------
  // cold finger nearest detector:
  //--------------------------------------------------------------------

  fRdcColdFingerRadius = 1.0/2.0*inch;
  fRdcColdFingerLength = 9.5*inch;        // guess FIXME
  fRdcColdFingerCutRadius = fThreadedCollarInnerRadius - eps;
  fRdcColdFingerCutLength = fThreadedCollarHeight + 0.25*inch;

  //--------------------------------------------------------------------
  // cryostat body
  //--------------------------------------------------------------------

  // digitized from CANBERRA chamber assembly drawing:
  fCryostatOringFlangeOuterRadius = fEndCapInnerRadius - eps;
  fCryostatOringFlangeThickness = 0.5*inch;
  fCryostatOringFlangeCutRadius = 2.3/2*inch;
  fCryostatOringFlangeCutHeight = 0.2*inch;

  // from M. Yocum email:
  fCryostatNeckOuterRadius = 1.25/2.0*inch;
  fCryostatShortNeckLength = 6.03*inch;
  fCryostatNeckThickness = 0.06*inch;           // from M. Yocum email

  // digitized -- this is the flange on the short coldfinger neck, opposite the
  // detector
  fCryostatFlangeOuterRadius = 3.0/2.0*inch;
  fCryostatFlangeHeight = 1.5*inch;
  fCryostatFlangeInnerRadius = 2.8/2.0*inch;
  fCryostatFlangeTopThickness = 0.2*inch;

  //--------------------------------------------------------------------
  // spider trestle 
  // digitized from CANBERRA assy dwgs
  //--------------------------------------------------------------------

  fSpiderTrestleOuterRadius = 2.75/2.0*inch;
  fSpiderTrestleInnerRadius = 2.25/2.0*inch;
  fSpiderTrestleHeight = 0.87*inch;
  fSpiderTrestleLargeHoleRadius = 1.5/2.0*inch;
  fSpiderTrestleLargeHoleDistance = 0.25*inch;
  fSpiderTrestleSmallHoleRadius = 0.6/2.0*inch;

  //--------------------------------------------------------------------
  // stand top plate
  // from G. Swift MALBEK detector shield drawings MDS-02-00-01
  //--------------------------------------------------------------------

  fStandTopPlateLength = 21.00*inch;
  fStandTopPlateThickness = 0.5*inch;
  fStandTopPlateHoleRadius = 3.75/2.0*inch;
  // from G. Swift email 04 March 2009, distance from top of endcap
  fStandTopPlateOffset = 10.331*inch;  

  //--------------------------------------------------------------------
  // detector spacer
  // updated 10 Nov 2011 from measurements at CANBERRA
  //--------------------------------------------------------------------

  fDetectorSpacerOuterRadius = 2.392/2.0*inch;
  fDetectorSpacerInnerRadius = 1.557/2.0*inch;
  fDetectorSpacerThickness = 0.040*inch;

  //--------------------------------------------------------------------
  // penta assembly
  // digitized from PENTA -1S ASSY MODEL dwg
  //--------------------------------------------------------------------

  // receptacle
  fPentaReceptacleHeight = 0.37*inch;
  fPentaReceptacleOuterRadius = 0.31/2.0*inch;
  fPentaReceptacleSmallOuterRadius = 0.28/2.0*inch;
  fPentaReceptacleMiddleLength = 0.31*inch;

  // plug
  fPentaPlugHeight = 0.31*inch;
  fPentaPlugStepHeight = 0.12*inch;
  fPentaPlugSmallOuterRadius = 0.11/2.0*inch;
  fPentaPlugOuterRadius = 0.28/2.0*inch;

  // plug + receptacle
  fPentaPlugInnerRadius = 0.06/2.0*inch;
  fPentaPlugHoleRadius = 0.054/2.0*inch; // accomodate mill max pin
  fPentaPlugHoleSpacing = 0.08*inch;
  fPentaPlugSpacing = 0.13*inch; 

  //--------------------------------------------------------------------
  // FET block
  // digitized from CANBERRA chamber assembly drawings
  //--------------------------------------------------------------------

  fFetBlockOuterRadius = 0.5/2.0*inch;
  //fFetBlockInnerRadius = 0.3/2.0*inch;
  fFetBlockInnerRadius = fPentaReceptacleOuterRadius + eps;  // to accomodate FET
  fFetBlockHeight = 1.3*inch;
  // a guess:
  fFetBlockBottomThickness = fFetBlockOuterRadius - fFetBlockInnerRadius;

  //--------------------------------------------------------------------
  // HV pogo probe
  // from Everett Charles SPA 3 webpage
  //--------------------------------------------------------------------

  fHVPogoPlungerRadius = 0.05/2.0*inch;
  fHVPogoPlungerHeight = (0.080 + 0.250 + 0.080)*inch; // last 0.08" is guess
  fHVPogoBarrelHeight = (1.30 - 0.080 - 0.25)*inch;
  fHVPogoBarrelOuterRadius = 0.080/2.0*inch;
  fHVPogoBarrelInnerRadius = fHVPogoPlungerRadius + eps;
  fHVPogoReceptacleOuterRadius = 0.093/2.0*inch;
  fHVPogoReceptacleLength = 0.957*inch;
  fHVPogoReceptacleOffset = 0.3*inch;

  //--------------------------------------------------------------------
  // HV pogo probe
  // from CANBERRA A201579
  //--------------------------------------------------------------------

  fHVContactExtensionOuterRadius = 0.25/2.0*inch;
  // add eps to accomodate HV receptacle:
  fHVContactExtensionInnerRadius = 0.093/2.0*inch + eps; 
  fHVContactExtensionLength = 0.75*inch;
  fHVContactExtensionStepLength = 0.19*inch;
  fHVContactExtensionStepRadius = 0.182/2.0*inch;

  //--------------------------------------------------------------------
  // poly spider
  // digitized from assembly drawing
  //--------------------------------------------------------------------

  fPolySpiderOuterRadius = 2.73/2.0*inch;
  fPolySpiderInnerRadius = fThreadedCollarOuterRadius + eps;
  fPolySpiderHeight = 0.5*inch;

  //--------------------------------------------------------------------
  // Mill Max 134 receptacle
  // from Mill Max website
  //--------------------------------------------------------------------

  fMillMaxReceptacleTopStepRadius = 0.072/2.0*inch;
  fMillMaxReceptacleTopStepLength = 0.031*inch;
  fMillMaxReceptacleMiddleStepRadius = 0.053/2.0*inch;
  fMillMaxReceptacleMiddleStepLength = 0.165*inch -
  fMillMaxReceptacleTopStepLength;
  fMillMaxReceptacleTailRadius = 0.020/2.0*inch;
  // guess -- chosen to allow union with double tail header pin
  fMillMaxReceptacleInnerRadius = 0.022/2.0*inch; 
  fMillMaxReceptacleInnerHoleDepth = 0.103*inch;
  // digitized from penta assembly
  fMillMaxReceptacleLength = 0.2*inch;

  //--------------------------------------------------------------------
  // mill max double tail header pin
  // product 0290
  //--------------------------------------------------------------------

  fDHPinShoulderRadius = 0.072/2.0*inch;
  fDHPinShoulderHeight = 0.052*inch;
  fDHPinPressFitOuterRadius = 0.053/2.0*inch;
  fDHPinPressFitLength = 0.103*inch;
  fDHPinTopTailRadius = 0.025/2.0*inch;
  fDHPinTopTailLength = 0.166*inch;
  fDHPinBottomTailRadius = 0.020/2.0*inch;
  fDHPinBottomTailLength = 0.156*inch;

  //--------------------------------------------------------------------
  // support post -- used in 2 places: encircles HV pogo, supports xtal
  // digitized from CANBERRA 7062591 -- p. 3 of chamber assy drawings
  // when dims were close, assumed same as support post drawings from Juan
  //--------------------------------------------------------------------

  fSupportPostOuterRadius = 0.25/2.0*inch;
  fSupportPostInnerRadius = fHVPogoBarrelOuterRadius + eps; 
  fSupportPostSmallOuterRadius = 0.182/2.0*inch;
  fSupportPostStepHeight = 0.19*inch;

  // 10 Nov 2011 -- matched to thickness of detector spacer HV ring
  fSupportPostHeight = fSupportPostStepHeight + 0.040*inch;  

  //--------------------------------------------------------------------
  // compression spring
  // digitized from CANBERRA 7062591 -- p. 3 of chamber assy drawings
  //--------------------------------------------------------------------

  fCompressionSpringOuterRadius = 0.25/2.0*inch;
  // model the spring as a solid cylinder, reduce wall thickness by a factor of
  // 10 since spring really isn't solid -- factor of 10 is a guess:
  fCompressionSpringInnerRadius = fCompressionSpringOuterRadius -
  0.02*inch/10.0; 

  //--------------------------------------------------------------------
  // fet
  // digitized from penta assembly
  //--------------------------------------------------------------------

  fFETLength = 0.036*inch; 
  fFETWidth = 0.025*inch; 

  //--------------------------------------------------------------------
  // resistor
  // digitized from penta assembly
  //--------------------------------------------------------------------

  fResistorRadius = 0.043/2.0*inch;
  fResistorLength = 0.177*inch;
  fResistorOffset = 0.060*inch;

  //--------------------------------------------------------------------
  // pb patch -- described in email from juan
  //--------------------------------------------------------------------

  // updated 10 Nov 2011 with measurements from CANBERRA -- changed patches from
  // two identical circles to two different rectangles

  // patch one -- this has a hole in reality, but the hole is not included in
  // the model
  fPbPatchOneWidth = 1.117 * inch;
  fPbPatchOneHeight = 0.644 * inch;
  MGLog(routine) << "lead patch one has w=" << fPbPatchOneWidth/inch << " inch" << endlog; 
  MGLog(routine) << "lead patch one has h=" << fPbPatchOneHeight/inch << " inch" << endlog; 

  // patch two -- the shape is irregular, so the dimensions are the average of two
  // measurements
  fPbPatchTwoWidth =  (0.910 + 0.947) / 2.0 * inch; 
  fPbPatchTwoHeight = (0.579 + 0.600) / 2.0 * inch; 
  MGLog(routine) << "lead patch two has w=" << fPbPatchTwoWidth/inch << " inch" << endlog; 
  MGLog(routine) << "lead patch two has h=" << fPbPatchTwoHeight/inch << " inch" << endlog; 

  // thickness is defined so patches fit between xtal and teflon cup 
  fPbPatchThickness = fTeflonCupInnerRadius - fCrystalRadius - 2*eps;
  MGLog(routine) << "lead patches have t=" << fPbPatchThickness/cm << " cm (" 
  << fPbPatchThickness/inch << " inch)" << endlog; 

  //--------------------------------------------------------------------
  // insulators around electronics body pins
  // digitized from chamber assy drawing from juan -- p3
  //--------------------------------------------------------------------
  
  fConnectorInsulatorOuterRadius = 0.180/2.0*inch;
  fConnectorInsulatorInnerRadius = fDHPinPressFitOuterRadius + eps;
  fConnectorInsulatorSmallOuterRadius = 0.106/2.0*inch;
  fConnectorInsulatorLength = 0.154*inch;
  fConnectorInsulatorStepLength = 0.042*inch;


  //--------------------------------------------------------------------
  // solder patches
  //--------------------------------------------------------------------

  fCenterContactSolderHeight = 0.25*mm;
  fCenterContactSolderWidth = 2.0*mm;

  fHVSolderHeight = fCenterContactSolderHeight;
  fHVSolderRadius = fHVPogoPlungerRadius;

  //fSolderRadius = fDHPinBottomTailRadius;
  fSolderRadius = 0.5*mm;
  
  //--------------------------------------------------------------------
  // zeolite package
  //--------------------------------------------------------------------

  // this is a guess -- FIXME:
  fZeoliteWidth = 1.0*cm;

}
//-----------------------------------------------------------------------//
