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
// $Id: MJGeometryVT1Detector.cc,v 1.4 2009/01/30 23:47:40 kmacon Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryVT1Detector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: K. Macon
 * CONTACT: mokevin@physics.unc.edu
 * FIRST SUBMISSION: 
 * 
 * REVISION: Added Calibration Source Geometry and added transparencies to 
 *           VisAttributes
 *           01-30-09, Added additional offset dimension 
 *                     fAlCylinderOffset,                                K. Macon
 *           04-09     Added PMT source geometery                        S.MacMullin
 *           04-09     Added copper rod to assembly to account for       S.MacMullin
 *                     discrepancies with experimental point source runs.
 *           07-09     Added copper plate underneath the crystal and     S.MacMullin
 *           	       for characterzatoion.
 */
//---------------------------------------------------------------------------//
//
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4Color.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "TMath.h"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryVT1Detector.hh"
#include "mjgeometry/MJGeometryVT1DetectorMessenger.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

using namespace CLHEP;
using namespace MaGe;

//--------------------------------------------------------------------------//
MJGeometryVT1Detector::MJGeometryVT1Detector(G4String serNum) :
  MGGeometryDetector(serNum), 

  //logical and physical volumes for detector components
fLeadShieldLogical(0),fCopperLayerLogical(0), fInnerVolumeLogical(0), 
fCryostatLogical(0), fCryostatCavityLogical(0), //fCrystalLogical(0), 
fIRShieldLogical(0), fMountCupWallLogical(0), fMountCupBaseLogical(0), 
fCopperPlateLogical(0), fAlCylinderLogical(0), fAlMountLogical(0), 
fColdFingerLogical(0), fCopperRodLogical(0), fCryostatCopperRodLogical(0),
fColdFingerCopperRodLogical(0),

fLeadShieldPhysical(0),fCopperLayerPhysical(0),fInnerVolumePhysical(0),
fCryostatPhysical(0), fCryostatCavityPhysical(0), fCrystalPhysical(0), 
fIRShieldPhysical(0), fMountCupWallPhysical(0), fMountCupBasePhysical(0),
fCopperPlatePhysical(0), fAlCylinderPhysical(0),fAlMountPhysical(0), 
fColdFingerPhysical(0), fCopperRodPhysical(0),fCryostatCopperRodPhysical(0),
fColdFingerCopperRodPhysical(0),

//logical and physical volumes for sample placements 
fTeflonLogical(0),fTeflonPhysical(0), 
fRockBucketLogical(0), fRockBucketPhysical(0),
fRockBagLogical(0), fRockBagPhysical(0), 
fSuperInsulationLogical(0),fSuperInsulationPhysical(0), 
fCalibrationSourceLogical(0),fCalibrationSourcePhysical(0),
fPMTLogical(0), fPMTPhysical(0),
fPicoCoaxLogical(0), fPicoCoaxPhysical(0), 
fMarinelliLogical(0), fMarinelliPhysical(0), 
fSamplePlacement(0)

{
 fMessenger = new MJGeometryVT1DetectorMessenger(this);
 fIdealCoaxCrystal = new MGGeometryIdealCoaxCrystal("VT1",false);
 fIdealCoaxCrystal->SetDefaults();
 SetDefaults();
}
//---------------------------------------------------------------------------//
MJGeometryVT1Detector::~MJGeometryVT1Detector()
{
delete fIdealCoaxCrystal;
delete fMessenger;
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::ConstructDetector(){
  G4Material *Magnesium = G4Material::GetMaterial("Magnesium");
  G4Material *AirMat = G4Material::GetMaterial("Air");
  G4Material *LeadMat = G4Material::GetMaterial("Lead-Mod");
  G4Material *Copper = G4Material::GetMaterial("Copper-OFHC");
  G4Material *Vacuum = G4Material::GetMaterial("Vacuum");
  G4Material *Aluminum = G4Material::GetMaterial("Aluminum");
  if(!(Magnesium || AirMat || LeadMat || Copper || Vacuum || Aluminum)) {
    MGLog(error) << "NULL G4Material pointer!" << endlog;
    MGLog(fatal) << endlog;
  }
  //-------------------------------------------------------------------------//
  //SOLIDS
  //-------------------------------------------------------------------------//
  G4Sphere *outerVolumeSolid
    = new G4Sphere("outerVolume", 0., fOuterVolumeRadius,
		   0.*degree, 360.0*degree, 0.0*degree, 180.0*degree);
  G4Tubs *leadShieldSolid
    = new G4Tubs("leadShield", 0., fLeadShieldRadius, 
		 fLeadShieldHeight/2., 0.,2.0*TMath::Pi());
  G4Tubs *copperLayerSolid
    = new G4Tubs("copperLayer", 0.,
		 fInnerVolumeRadius + fCopperLayerThickness,
		 fInnerVolumeHeight/2 + fCopperLayerThickness,
		 0.,2.0*TMath::Pi());
  G4Tubs *innerVolumeSolid
    = new G4Tubs("innerVolume", 0., fInnerVolumeRadius, 
		 fInnerVolumeHeight/2, 0., 2.0*TMath::Pi());
  G4Tubs *cryostatSolid
    = new G4Tubs("cryostat", 0, fCryostatOuterRadius, 
	         fCryostatHeight/2., 0, 2.0 * TMath::Pi());
  G4Tubs *cryostatCavitySolid
    = new G4Tubs("cryostatCavity", 0, 
		 fCryostatOuterRadius - fCryostatThickness, 
		 fCryostatHeight/2. - fMgPlateThickness, 
		 0., 2.0*TMath::Pi());
  G4Tubs *mountCupWallSolid
    = new G4Tubs("mountCupWall",fMountCupInnerRadius,
		 fMountCupOuterRadius,fMountCupWallHeight/2,
		 0., 2.0*TMath::Pi());
  G4Tubs *irShieldSolid
    = new G4Tubs("irShield",0, fMountCupInnerRadius-0.01 * cm, 
		 fIRShieldThickness/2, 0., 2.0*TMath::Pi());
  G4Tubs *mountCupBaseSolid 
    = new G4Tubs("mountCupBase", 0, fMountCupOuterRadius,
		 fMountCupBaseThickness/2, 0., 2.0*TMath::Pi());
  G4Tubs *copperPlateSolid
    = new G4Tubs("copperPlate", fColdFingerRadius + 0.01 *in, fCopperPlateRadius,
		 fCopperPlateThickness/2, 0., 2.0*TMath::Pi());
  G4Tubs *coldFingerSolid
    = new G4Tubs("coldFinger", 0.375, fColdFingerRadius,
		 fColdFingerLength/2-0.01 * cm, 0., 2.0*TMath::Pi());
  G4Tubs *alCylinderSolid
    = new G4Tubs("alCylinder", 0.375 + 0.01 *in, fCryostatOuterRadius, 
		 fAlCylinderHeight/2.- 0.01 *cm, 0., 2.0*TMath::Pi());
  G4Tubs *alMountSolid
    = new G4Tubs("alMount", 0.375 + 0.01 *in, fColdFingerRadius, fAlMountHeight/2.,
		 0., 2.0*TMath::Pi());
  G4Tubs *copperRodSolid
    = new G4Tubs("copperRod", 0, fCopperRodRadius, fCopperRodHeight/2.,
		 0., 2.0*TMath::Pi());
  G4Tubs *cryostatCopperRodSolid
    = new G4Tubs("cryostatCopperRod",0, fCryostatCopperRodRadius, fCryostatCopperRodHeight/2.,
		 0., 2.0*TMath::Pi());

  G4Tubs *coldFingerCopperRodSolid
    = new G4Tubs("coldFingerCopperRod",0,fColdFingerCopperRodRadius, 
		 fColdFingerCopperRodLength/2.-0.01 * cm, 0., 2.0*TMath::Pi());

  //--------------------------------------------------------------------//
  //LOGICALS
  //--------------------------------------------------------------------//
  theDetectorLogical
    = new G4LogicalVolume(outerVolumeSolid, AirMat,
			  "outerVolume", 0, 0, 0, true);
  fLeadShieldLogical 
    = new G4LogicalVolume(leadShieldSolid, LeadMat, 
			  "leadShield", 0, 0, 0, true);
  fCopperLayerLogical
    = new G4LogicalVolume(copperLayerSolid, Copper,
			  "copperLayer", 0, 0, 0, true);
  fInnerVolumeLogical
    = new G4LogicalVolume(innerVolumeSolid, AirMat,
			"innerVolume", 0, 0, 0, true);
  fCryostatLogical
    = new G4LogicalVolume(cryostatSolid, Magnesium, 
			  "cryostat", 0, 0, 0, true);
  fCryostatCavityLogical
    = new G4LogicalVolume(cryostatCavitySolid, Vacuum, 
			  "cryostatCavity", 0, 0, 0, true);
  fIRShieldLogical
    = new G4LogicalVolume(irShieldSolid, Aluminum,
			  "irShield", 0, 0, 0, true); 
  fMountCupWallLogical
    = new G4LogicalVolume(mountCupWallSolid, Copper,
			 "mountCupWall", 0, 0, 0, true);
  fMountCupBaseLogical
    = new G4LogicalVolume(mountCupBaseSolid, Copper, 
			 "mountCupBase", 0,0,0, true);
  fCopperPlateLogical
    = new G4LogicalVolume(copperPlateSolid,Copper,
			  "copperPlate", 0,0,0, true);
  fColdFingerLogical
    = new G4LogicalVolume(coldFingerSolid, Aluminum,
			  "coldFinger", 0, 0, 0, true);
  fAlCylinderLogical
    = new G4LogicalVolume(alCylinderSolid, Aluminum,
			  "alCylinder", 0, 0, 0, true);
  fAlMountLogical
    = new G4LogicalVolume(alMountSolid, Aluminum,
			  "alMount", 0, 0, 0, true);
  fCopperRodLogical
    = new G4LogicalVolume(copperRodSolid, Copper,
			  "copperRod", 0, 0, 0, true);
  fCryostatCopperRodLogical
    = new G4LogicalVolume(cryostatCopperRodSolid, Copper,
			  "cryostatCopperRod", 0, 0, 0, true);
  fColdFingerCopperRodLogical
    = new G4LogicalVolume(coldFingerCopperRodSolid, Copper,
			  "coldFingerCopperRod", 0,0,0, true);
  //-------------------------------------------------------------------------//
  //PLACEMENTS
  //-------------------------------------------------------------------------//
  //             VECTORS                //
  //------------------------------------//
  G4double 
    cavityHeight = fCryostatHeight -2.*fMgPlateThickness;
  G4double 
    xtalZ = cavityHeight/2 - fCrystalHeight/2
            - fCrystalOffset;
  G4double
    mountCWZ = cavityHeight/2 - fMountCupWallHeight/2 - fCrystalOffset 
               + fIRShieldThickness/2 + 0.01*cm;
  G4double
    mountBaseZ = cavityHeight/2 - fMountCupWallHeight 
                 - fMountCupBaseThickness/2 - fCrystalOffset
                 - fIRShieldThickness/2 - 0.01 * cm;
  G4double
    copperPlateZ = mountBaseZ - fCopperPlateThickness/2.;
  G4double
    irShieldZ = cavityHeight/2 - fCrystalOffset + 0.01 * cm;
  G4double
    coldFingerZ = - fAlCylinderHeight/2 - fAlMountHeight - fColdFingerRadius/2.
                  - fCryostatOffset - fCryostatHeight - 0.01 * cm
                  + fAlCylinderOffset;
  G4double
    alCylinderZ = - fCryostatOffset - fCryostatHeight - fAlMountHeight
                  - fAlCylinderHeight/2. + fAlCylinderOffset ;
  G4double
    alMountZ    = - fCryostatOffset - fCryostatHeight - fAlMountHeight/2.
                  + fAlCylinderOffset;
  G4double
    cryostatZ   = - fCryostatHeight/2 - fCryostatOffset + fAlCylinderOffset;
 
  G4double
    copperRodZ  = -fCryostatOffset - fCryostatHeight - fCopperRodHeight/2 + fAlCylinderOffset;

  G4double
    cryostatCopperRodZ = mountBaseZ - fMountCupBaseThickness/2 - fCryostatCopperRodHeight/2;

  G4ThreeVector xtalPlacement(0,0, xtalZ);
  G4ThreeVector cryostatPlacement(0, 0, cryostatZ);
  G4ThreeVector mountCupWallPlacement(0,0,mountCWZ);
  G4ThreeVector irShieldPlacement(0,0,irShieldZ);
  G4ThreeVector mountCupBasePlacement(0,0,mountBaseZ);
  G4ThreeVector copperPlatePlacement(0,0,copperPlateZ);
  G4ThreeVector coldFingerPlacement(-fCryostatOuterRadius
				    -fColdFingerLength/2.-0.01 * cm,
				    0., coldFingerZ);
  G4ThreeVector alCylinderPlacement(0,0,alCylinderZ);
  G4ThreeVector alMountPlacement(0,0,alMountZ);
  G4ThreeVector copperRodPlacement(0,0,copperRodZ);
  G4ThreeVector cryostatCopperRodPlacement(0,0,cryostatCopperRodZ);  

  G4RotationMatrix *yRot = new G4RotationMatrix;
  yRot->rotateY(-TMath::Pi()/2.*rad);
  //------------------------------------//
  fLeadShieldPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fLeadShieldLogical, 
		    "leadShield", theDetectorLogical, false, 0);
  fCopperLayerPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fCopperLayerLogical,
		    "copperLayer", fLeadShieldLogical, false, 0);
  fInnerVolumePhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fInnerVolumeLogical, 
		    "innerVolume", fCopperLayerLogical, false, 0);
  fCryostatPhysical
    = new G4PVPlacement(0, cryostatPlacement, fCryostatLogical, 
			"cryostat", fInnerVolumeLogical, false, 0);
  fCryostatCavityPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fCryostatCavityLogical,
			"cryostatCavity", fCryostatLogical, false, 0);
  fIRShieldPhysical
    = new G4PVPlacement(0, irShieldPlacement, fIRShieldLogical,
			"irShield", fCryostatCavityLogical, false, 0);
  fMountCupWallPhysical
    = new G4PVPlacement(0, mountCupWallPlacement, fMountCupWallLogical,
			"mountCupWall", fCryostatCavityLogical,
			false, 0);
  fMountCupBasePhysical 
    = new G4PVPlacement(0, mountCupBasePlacement, fMountCupBaseLogical,
			"mountCupBase", fCryostatCavityLogical,
			false, 0);
  fCopperPlatePhysical
    = new G4PVPlacement(0, copperPlatePlacement, fCopperPlateLogical,
			"copperPlate", fCryostatCavityLogical,
			false, 0);
  fColdFingerPhysical
    = new G4PVPlacement(yRot, coldFingerPlacement, fColdFingerLogical,
			"coldFinger", fInnerVolumeLogical, false, 0);
  fAlCylinderPhysical
    = new G4PVPlacement(0, alCylinderPlacement, fAlCylinderLogical,
			"alCylinder", fInnerVolumeLogical, false, 0);
  fAlMountPhysical
    = new G4PVPlacement(0, alMountPlacement, fAlMountLogical,
			"alMount", fInnerVolumeLogical,	false, 0);
  fCopperRodPhysical
    = new G4PVPlacement(0, copperRodPlacement, fCopperRodLogical,
			"copperRod", fInnerVolumeLogical, false, 0);
  fCryostatCopperRodPhysical
    = new G4PVPlacement(0, cryostatCopperRodPlacement, fCryostatCopperRodLogical,
			"cryostatCopperRod", fCryostatCavityLogical, false, 0);
  fIdealCoaxCrystal->ConstructDetector();
  fCrystalPhysical
    = new G4PVPlacement(0, xtalPlacement, 
		      fIdealCoaxCrystal->GetDetectorLogical(), 
		      "crystal", fCryostatCavityLogical, false, 0);
  fColdFingerCopperRodPhysical
    = new G4PVPlacement(yRot, coldFingerPlacement, fColdFingerCopperRodLogical,
			"coldFingerCopperRod", fInnerVolumeLogical, false, 0);

  if(fUsingSamples) InitializeSample();
  //-------------------------------------------------------------------------//
  //VISUALIZATIONS
  //-------------------------------------------------------------------------//
  //G4VisAttributes* white = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //G4VisAttributes* yellow = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  //G4VisAttributes* green = new G4VisAttributes(G4Colour(0.0,1,0.0));
  //G4VisAttributes* red = new G4VisAttributes(G4Colour(1.0,0.0,0.0,.5));
  //G4VisAttributes* cyan = new G4VisAttributes(G4Colour(0.0,1.0,1.0,.3));
  //G4VisAttributes* magenta = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* grey = new G4VisAttributes(G4Colour(0.6,0.6,0.6,.3));
  G4VisAttributes* leadshield = new G4VisAttributes(G4Colour(1.0,1.0,1.0,.4));
  theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fLeadShieldLogical->SetVisAttributes(leadshield); 
  fCopperLayerLogical->SetVisAttributes(yellow); 
  fInnerVolumeLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fCryostatLogical->SetVisAttributes(grey);
  fCryostatCavityLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fMountCupBaseLogical->SetVisAttributes(yellow);
  fMountCupWallLogical->SetVisAttributes(yellow);
  fCopperPlateLogical->SetVisAttributes(yellow);
  fIRShieldLogical->SetVisAttributes(green);
  fColdFingerLogical->SetVisAttributes(green);
  fAlCylinderLogical->SetVisAttributes(green);
  fAlMountLogical->SetVisAttributes(green);
  fCopperRodLogical->SetVisAttributes(yellow);
  fCryostatCopperRodLogical->SetVisAttributes(yellow);
  fColdFingerCopperRodLogical->SetVisAttributes(yellow);
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeSample(){
  if(fSample == "Teflon"){InitializeTeflon();}
  else if(fSample == "RockBucket"){InitializeRockBucket();}
  else if(fSample == "RockBag"){InitializeRockBag();}
  else if(fSample == "SuperInsulation"){InitializeSuperInsulation();}
  else if(fSample == "CalibrationSource"){InitializeCalibrationSource();}
  else if(fSample == "PMT"){InitializePMT();}
  else if(fSample == "Marinelli"){InitializeMarinelli();}
  else if(fSample == "PicoCoax"){InitializePicoCoax();}
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeTeflon(){
  fSampleHeight = 3.0 * cm;
  fSampleWidth = 3.0 * cm;
  fSampleLength = 3.0 * cm;

  SetPosition(G4ThreeVector(0, 0, -fCryostatOffset + fSampleHeight/2.+ fMgPlateThickness + fAlCylinderOffset ));

  G4Box *TeflonSolid
    = new G4Box("Teflon", fSampleLength/2., fSampleWidth/2., 
		fSampleHeight/2.);
  fTeflonLogical
    = new G4LogicalVolume(TeflonSolid, G4Material::GetMaterial("Teflon"),
			  "Teflon", 0, 0, 0, true);
  fTeflonPhysical
    = new G4PVPlacement(0, fSamplePlacement, fTeflonLogical, 
			"Teflon", fInnerVolumeLogical, false, 0);
  fTeflonLogical->SetVisAttributes(G4Colour(0.0,0.0,1.0));
  }
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeRockBucket(){
  G4Material *crushedRock
    = new G4Material("Crushed Rock", 1.474 * g/cm3, 2);
  G4Element* elO
    = new G4Element("Oxygen" ,"O" , 8., 16*g/mole);
  G4Element* elSi
    = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  crushedRock->AddElement(elSi, 1);
  crushedRock->AddElement(elO, 2);
  fSampleHeight = 5.0 * cm;
  SetPosition(G4ThreeVector(0, 0, -fCryostatOffset + fSampleHeight/2.+ 
			    fMgPlateThickness + fAlCylinderOffset + 0.01*cm));
  G4Cons *RockBucketSolid
    = new G4Cons("RockBucket", 0., 5.0 * cm ,0. ,6.5 * cm,
		 fSampleHeight/2, 0., 2.0 * TMath::Pi());
  fRockBucketLogical
    = new G4LogicalVolume(RockBucketSolid, crushedRock,
			  "RockBucket", 0, 0, 0, true); 
  fRockBucketPhysical
    = new G4PVPlacement(0, fSamplePlacement, fRockBucketLogical, 
			"RockBucket", fInnerVolumeLogical, false, 0);
  fRockBucketLogical->SetVisAttributes(G4Colour(0.0,0.0,1.0));
  }
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeRockBag(){
  G4Material *crushedRock
    = new G4Material("Crushed Rock", 1.474 * g/cm3, 2);
  G4Element* elO
    = new G4Element("Oxygen" ,"O" , 8., 16*g/mole);
  G4Element* elSi
    = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  crushedRock->AddElement(elSi, 1);
  crushedRock->AddElement(elO, 2);
  fSampleHeight = 5. * cm;
  fSampleWidth = 8. * cm;
  fSampleLength = 15. * cm;
  G4RotationMatrix *xRot = new G4RotationMatrix;
  xRot->rotateX(TMath::Pi()/2.*rad);
  
  SetPosition(G4ThreeVector(0, 0, -fCryostatOffset + fSampleHeight/2.+ 
			    fMgPlateThickness + fAlCylinderOffset + 0.01*cm));
 
  if(fPlacementOption =="InBottom"){
     G4double sampleZ = - fInnerVolumeHeight/2 + fSampleHeight/2 + 0.01 * cm;
     SetPosition(fCryostatOuterRadius-4 * in, 0. ,sampleZ);
     xRot->rotateZ(TMath::Pi()/2.*rad);
   }

  G4Ellipsoid *RockBagSolid
    = new G4Ellipsoid("RockBag", fSampleWidth/2., fSampleHeight/2.,
	      fSampleLength*3/4.,-fSampleLength/2., fSampleLength/2.);
  fRockBagLogical
    = new G4LogicalVolume(RockBagSolid, crushedRock,
			  "RockBag", 0, 0, 0, true);
  fRockBagPhysical
    = new G4PVPlacement(xRot, fSamplePlacement, fRockBagLogical, 
			"RockBag", fInnerVolumeLogical, false, 0);
  fRockBagLogical->SetVisAttributes(G4Colour(0.0,0.0,1.0));
  }
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeSuperInsulation(){
  G4Element* elO
    = new G4Element("Oxygen", "O", 8., 16*g/mole);
  G4Element* elSi
    = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  G4double density;

  G4Material* Aerog = new G4Material("Aerogel", density = 0.246*g/cm3, 2);
  Aerog->AddElement(elSi, 1);
  Aerog->AddElement(elO, 2);

  G4RotationMatrix *zRot = new G4RotationMatrix;
  G4double oRadius = 5.5 * in;
  G4double thickness = .75 * in;
  G4double iRadius = oRadius - thickness;
  G4double span = 2*12 * in/(iRadius+oRadius);

  fSampleHeight = 12. * in;
  G4double sampleZ = - fInnerVolumeHeight/2 + fSampleHeight/2 + 0.01 * cm;
  SetPosition(-1*in,0,sampleZ);
  zRot->rotateZ(span/2);

  G4Tubs *SuperInsulationSolid
    = new G4Tubs("SuperInsulation", iRadius, oRadius,
		 fSampleHeight/2.-0.01 *cm, 0, span);
  fSuperInsulationLogical
    = new G4LogicalVolume(SuperInsulationSolid, Aerog,
			  "SuperInsulation", 0, 0, 0, true);
  fSuperInsulationPhysical
    = new G4PVPlacement(zRot, fSamplePlacement, fSuperInsulationLogical, 
			"SuperInsulation", fInnerVolumeLogical,
			false, 0);
  fSuperInsulationLogical->SetVisAttributes(G4Colour(0.0,0.0,1.0,.3));

  G4double SupInsVol = SuperInsulationSolid->GetCubicVolume();

  MGLog(trace) << "Super Insulation Volume: " << SupInsVol/cm3 
	       << " cm3" << endlog;
  MGLog(trace) << "Super Insulation Mass: " <<  (SupInsVol * density)/g
	       << " g" << endlog;

  }
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeCalibrationSource(){
  fSampleHeight = 0.1*mm;
  G4double SampleRadius = 1.5*mm;
  SetPosition(G4ThreeVector(0, 0, -fCryostatOffset + fSampleHeight/2.+ 
			    fMgPlateThickness + fAlCylinderOffset +0.01*cm));  

  if(fPlacementOption =="InBottom"){
     G4double sampleZ = - fInnerVolumeHeight/2 + fSampleHeight/2 + 0.01 * cm;
     SetPosition(fInnerVolumeRadius-SampleRadius-0.1*mm,0.,sampleZ);
   }
  G4Tubs *CalibrationSourceSolid
    = new G4Tubs("CalibrationSource", 0, SampleRadius,
		 fSampleHeight/2.,0, 2.0 *TMath::Pi());
  fCalibrationSourceLogical
    = new G4LogicalVolume(CalibrationSourceSolid, 
			  G4Material::GetMaterial("Air"),
			  "CalibrationSource",0,0,0,true);
  fCalibrationSourcePhysical
    = new G4PVPlacement(0, fSamplePlacement, fCalibrationSourceLogical,
			"CalibrationSource", fInnerVolumeLogical,false,0);
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializePMT(){
  G4double fBasesRadius = 4.6*cm;
  G4double fBasesHeight = 2.25*mm;
  G4double fResistorsWidth = 4.0*cm;
  G4double fResistorsLength = 5.0*cm; 
  G4double fResistorsHeight = 2.25*mm;
  G4double fCapacitorsWidth = 3.4*cm;
  G4double fCapacitorsLength = 4.1*cm;
  G4double fCapacitorsHeight = 5.0*mm;


SetPosition(G4ThreeVector(0, 0, -fCryostatOffset + fBasesHeight/2.+ 
			    fMgPlateThickness +fAlCylinderOffset + 0.01*cm)); 

G4Tubs *BasesSolid
  = new G4Tubs("Bases", 0, fBasesRadius,
	       fBasesHeight/2.,0, 2.0 *TMath::Pi());
G4Box *ResistorsSolid
    = new G4Box("Resistors", fResistorsLength/2.,
		fResistorsWidth/2., fResistorsHeight/2.);
G4Box *CapacitorsSolid
    = new G4Box("Capacitors", fCapacitorsLength/2.,
		fCapacitorsWidth/2., fCapacitorsHeight/2.);

G4UnionSolid *PMTSolid1
  = new G4UnionSolid("PMT1", BasesSolid,ResistorsSolid,0,G4ThreeVector(0,0,fBasesHeight + fResistorsHeight/2.));

G4UnionSolid *PMTSolid
  = new G4UnionSolid("PMT",PMTSolid1,CapacitorsSolid,0,G4ThreeVector(0,0,fBasesHeight + fResistorsHeight + fCapacitorsHeight/2.));

/*
Boolean solids will create a composite solid from two solids. The rotation matrix 
arguments and translation vector arguments are optional. 
Also can use G4SubtractionSolid, G4IntersectionSolid

G4UnionSolid *NameSolid 
= new G4UnionSolid("CompositeName",Solid1,Solid2,RotationMatrix,Translation);
*/

fPMTLogical
    = new G4LogicalVolume(PMTSolid, 
			  G4Material::GetMaterial("Teflon"),
			  "PMT",0,0,0,true);

fPMTLogical->SetVisAttributes(green);

fPMTPhysical
    = new G4PVPlacement(0, fSamplePlacement, fPMTLogical,
			"PMT",fInnerVolumeLogical, false,0);
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializePicoCoax(){
  G4double fBigRingOuterRadius = 2.25 * in;
  G4double fBigRingInnerRadius = 1.75 * in;
  G4double fBigRingHeight = 1.0 * in;
  G4double fSmallRingOuterRadius = 2.0 * in;
  G4double fSmallRingInnerRadius = 1.75 * in;
  G4double fSmallRingHeight = 0.75 * in;

  SetPosition(0,0, fTopOfCryostat - 0.25 * in -fSmallRingHeight - fBigRingHeight/2.);

  G4Tubs  *BigRingSolid
  	= new G4Tubs("BigRingSolid", fBigRingInnerRadius, fBigRingOuterRadius, 
		fBigRingHeight/2., 0, 2.0*TMath::Pi());

  G4Tubs *SmallRingSolid
  	= new G4Tubs("SmallRingSolid", fSmallRingInnerRadius, fSmallRingOuterRadius,
		fSmallRingHeight/2., 0, 2.0*TMath::Pi());


  G4UnionSolid *PicoCoaxMiddleAndTopSolid
  	= new G4UnionSolid("PicoCoaxMiddleAndTopSolid",BigRingSolid,SmallRingSolid,
		0,G4ThreeVector(0,0,fBigRingHeight/2. + fSmallRingHeight/2. + 0.1 * mm));

  G4UnionSolid *PicoCoaxSolid
  	= new G4UnionSolid("PicoCoaxSolid",PicoCoaxMiddleAndTopSolid,SmallRingSolid,
		0,G4ThreeVector(0,0,-fBigRingHeight/2. - fSmallRingHeight/2. - 0.1 *mm));

   fPicoCoaxLogical
    = new G4LogicalVolume(PicoCoaxSolid, G4Material::GetMaterial("Teflon"),
			  "PicoCoax", 0,0,0,true);

  fPicoCoaxLogical->SetVisAttributes(cyan);
  
  fPicoCoaxPhysical
  	= new G4PVPlacement(0, fSamplePlacement, fPicoCoaxLogical, "PicoCoax",
		fInnerVolumeLogical, false, 0);
		
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::InitializeMarinelli(){
  G4double fBottomHeight = 2.8 * in;
  G4double fTopHeight = 2.2 * in;
  G4double SampleInnerRadius = 8.4/2 * cm;
  G4double SampleOuterRadius = 14.1/2 * cm;
  SetPosition(0, 0,  -fCryostatOffset + fBottomHeight/2.+ 
			    fMgPlateThickness +fAlCylinderOffset - 7.2 * cm);

  G4Tubs *MarinelliSolidBottom
    = new G4Tubs("MarinelliBottom", SampleInnerRadius, SampleOuterRadius,
		 fBottomHeight/2., 0, 2.0*TMath::Pi());
	
  G4Tubs *MarinelliSolidTop
	= new G4Tubs("MarinelliTop", 0, SampleOuterRadius,
		fTopHeight/2., 0, 2.0*TMath::Pi());

  G4UnionSolid *MarinelliSolid
  	= new G4UnionSolid("Marinelli", MarinelliSolidBottom, MarinelliSolidTop, 0, G4ThreeVector(0,0, fBottomHeight/2. + fTopHeight/2. + 0.01 * in));

  fMarinelliLogical
    = new G4LogicalVolume(MarinelliSolid, G4Material::GetMaterial("Teflon"),
			  "Marinelli", 0,0,0,true);
  fMarinelliLogical->SetVisAttributes(cyan);
  fMarinelliPhysical
    = new G4PVPlacement(0, fSamplePlacement, fMarinelliLogical, "Marinelli",
			fInnerVolumeLogical, false, 0);
}
//---------------------------------------------------------------------------//
void MJGeometryVT1Detector::SetDefaults()
{
  fCrystalHeight = 75.7*mm;
  fCrystalRadius = 55.8/2*mm;
  fIdealCoaxCrystal->SetHeight(fCrystalHeight);
  fIdealCoaxCrystal->SetRadius(fCrystalRadius);
  fIdealCoaxCrystal->SetCoreRadius(4.55 * mm); 
  fIdealCoaxCrystal->SetCoreDepth(63.2 * mm);
  fIdealCoaxCrystal->SetCornerRadius(1.0 * cm); // * Guesstimate
  fIdealCoaxCrystal->SetOuterDeadLayerThickness(700.0 * micrometer);
  fIdealCoaxCrystal->SetCoreDeadLayerThickness(30 * micrometer); 
  fIdealCoaxCrystal->SetExtraDeadLayerBottomZ(0.*cm);
  fIdealCoaxCrystal->SetExtraDeadLayerMiddleZ(0.*cm);
  fIdealCoaxCrystal->SetExtraDeadLayerTopZ(2. *cm);
  fIdealCoaxCrystal->SetExtraDeadLayerBottomInnerR(4.55 *mm);
  fIdealCoaxCrystal->SetExtraDeadLayerMiddleInnerR(4.55 *mm);
  fIdealCoaxCrystal->SetExtraDeadLayerTopInnerR(4.55 *mm);
  fIdealCoaxCrystal->SetExtraDeadLayerBottomOuterR(3. *cm);
  fIdealCoaxCrystal->SetExtraDeadLayerMiddleOuterR(1.5 *cm);
  fIdealCoaxCrystal->SetExtraDeadLayerTopOuterR(4.55 *mm);
  fIdealCoaxCrystal->SetExtraDeadLayerConsidered(true);
  fIdealCoaxCrystal->CoreBubbleOn();
  fIdealCoaxCrystal->SetDeadLayerConsidered(true);
  fIdealCoaxCrystal->SetCutInHalf(false);
  fIdealCoaxCrystal->SetIsActive(true);
  fIdealCoaxCrystal->SetIsOneOfMany(false);
  fIdealCoaxCrystal->SetG4MaterialName("Germanium-Nat");
  fMgPlateThickness = 1.00 * mm;
  fCryostatHeight = 16.2 * cm;  
  fCryostatOuterRadius = 3.5 * cm;
  fCryostatOffset = - 4.0 * in;
  fCryostatThickness = 1.3 * mm;
  fCrystalOffset = 4.* mm;
  fMountCupWallThickness = .76 * mm;
  fIRShieldThickness = 0.06 * mm;
  fMountCupBaseThickness = 3.2 * mm;
  fCopperPlateThickness = 0.75 * in;
  fCopperPlateRadius = 1. * in;
  fInnerVolumeHeight = 16. * in; 
  fInnerVolumeRadius = 5.5 * in;
  fCopperLayerThickness = 0.116 *in;
  fLeadShieldThickness = 4.0 * in;
  fLeadShieldHeight
    = fInnerVolumeHeight + 2*fCopperLayerThickness
    + 2*fLeadShieldThickness +.01 * cm;
  fLeadShieldRadius 
    = fInnerVolumeRadius + fCopperLayerThickness
    + fLeadShieldThickness + .01 * cm;
  fOuterVolumeRadius = 70.0 * cm;
  fMountCupInnerRadius = fCrystalRadius + 0.02 * cm;
  fMountCupOuterRadius = fMountCupInnerRadius + fMountCupWallThickness;
  fIRShieldThickness = 0.06 * mm;
  fMountCupAssemblyHeight = 94 * mm;
  fMountCupWallHeight
    = fMountCupAssemblyHeight
    - fIRShieldThickness - fMountCupBaseThickness;
  fColdFingerLength = 4. * in;
  fColdFingerRadius = .625 * in;
  fAlCylinderHeight = 2.25 * in;
  fAlCylinderOffset = 1.0 * cm;
  fAlMountHeight = 3.25 * in;
  fCopperRodRadius = 0.375 * in;
  fCopperRodHeight = 5.5 * in;
  fCryostatCopperRodRadius = 0.375 * in;
  fCryostatCopperRodHeight = 6.4 * cm;
  fColdFingerCopperRodRadius = 0.375 * in;
  fColdFingerCopperRodLength = 4. * in;
  fTopOfCryostat = -fCryostatOffset + fMgPlateThickness + fAlCylinderOffset; 
}
//---------------------------------------------------------------------------//
