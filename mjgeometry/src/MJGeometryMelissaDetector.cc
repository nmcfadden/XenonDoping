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
// $Id: MJGeometryMelissaDetector.cc,v 1.8 2009/02/23 15:50:55 kmacon Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryMelissaDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: P. Finnerty, K. Macon
 * CONTACT: Kevin Macon mokevin@habanero.physics.unc.edu
 * FIRST SUBMISSION: 
 * 
 * REVISION: K. Macon 08-22-2008 Added Macro Command Support 
 *                                 for selecting sample volume.
 *           K. Macon 09-23-2008 Added Correct Cyrostat Height 
 *                                 and Sample Position
 *           K. Macon 09-25-2008 Added Calibration Source Geometry
 *           K. Macon 10-12-2008 Fixed Sample Placement Vectors
 *           K. Macon 02-22-2009 Added 1" OFHC Copper Shield to geometry
 *           S. MacMullin 07-2009 Modified crystal and cryostat geomtries for characterization. 
 */
//---------------------------------------------------------------------------//
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
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
#include "mjgeometry/MJGeometryMelissaDetectorMessenger.hh"
#include "mjgeometry/MJGeometryMelissaDetector.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

using namespace CLHEP;
using namespace MaGe;

//--------------------------------------------------------------------------//
MJGeometryMelissaDetector::MJGeometryMelissaDetector(G4String serNum) :
  MGGeometryDetector(serNum), 
  
  fLeadShieldLogical(0), fLeadCylinderLogical(0), fInnerVolumeLogical(0),
  fCopperShieldLogical(0), fCryostatLogical(0), fCryostatCavityLogical(0),
  fCryostatPuckLogical(0), fLeadShieldPhysical(0), fLeadCylinderPhysical(0),
  fInnerVolumePhysical(0), fCopperShieldPhysical(0), fCryostatPhysical(0),
  fCryostatCavityPhysical(0), fCryostatPuckPhysical(0), 
  
  
  fTeflonLogical(0),fTeflonPhysical(0), fSteelLogical(0), fSteelPhysical(0),
  fRockBucketLogical(0),fRockBucketPhysical(0), fRockBagLogical(0),
  fRockBagPhysical(0), fSuperInsulationLogical(0), fSuperInsulationPhysical(0),
  fAlRingsLogical(0), fAlRingsPhysical(0), fCalibrationSourceLogical(0),fCalibrationSourcePhysical(0),fPMTLogical(0),fPMTPhysical(0),
  fZeoliteLogical(0),fZeolitePhysical(0),fFoamInsulationLogical(0), fFoamInsulationPhysical(0),
  fMarinelliLogical(0), fMarinelliPhysical(0),
  fSamplePlacement(0)
{
  fMessenger = new MJGeometryMelissaDetectorMessenger(this);
  fIdealCoaxCrystal = new MGGeometryIdealCoaxCrystal("Melissa",false);
  fIdealCoaxCrystal->SetDefaults();
  SetDefaults();
}
//---------------------------------------------------------------------------//
MJGeometryMelissaDetector::~MJGeometryMelissaDetector()
{
delete fIdealCoaxCrystal;
delete fMessenger;
}
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::ConstructDetector()
{
  G4Material *Aluminum = G4Material::GetMaterial("Aluminum");
  G4Material *AirMat   = G4Material::GetMaterial("Air");
  G4Material *LeadMat  = G4Material::GetMaterial("Lead-Mod");
  G4Material *Vacuum   = G4Material::GetMaterial("Vacuum");
  G4Material *CopperMat = G4Material::GetMaterial("Copper-OFHC");

  
  if(!( Aluminum || AirMat || LeadMat || Vacuum)) {
    MGLog(error) << "NULL G4Material pointer!" << endlog;
    MGLog(fatal) << endlog;
  }
  //-------------------------------------------------------------------------//
  //SOLIDS
  //-------------------------------------------------------------------------//
  G4Sphere *outerVolumeSolid
    = new G4Sphere("outerVolumeSolid", 0.,  fOuterVolumeRadius, 
		   0.*degree, 360.0*degree, 0.0*degree, 180.0*degree);
  G4Box *leadShieldSolid
    = new G4Box("leadShieldSolid", fLeadShieldWidth/2., 
		fLeadShieldWidth/2., fLeadShieldHeight/2.);
  G4Box *innerVolumeSolid
    = new G4Box("innerVolumeSolid", fInnerVolumeWidth/2.,
		fInnerVolumeWidth/2., fInnerVolumeHeight/2.);
  G4Box *copperBoxSolid
    = new G4Box("copperBoxSolid", fInnerVolumeWidth/2 - .01*cm,
                fInnerVolumeWidth/2.-.01*cm, fInnerVolumeHeight/2-.01*cm);
  G4Box *copperInnerSolid
    = new G4Box("copperInnerSolid", fInnerVolumeWidth/2 - .01*cm
		-fCopperShieldThickness, fInnerVolumeWidth/2-.01*cm
                -fCopperShieldThickness, fInnerVolumeHeight/2-.01*cm
		-fCopperShieldThickness);
  G4Tubs *copperHoleSolid
    = new G4Tubs("copperHoleSolid", 0, fCopperHoleRadius, 
		 fCopperShieldThickness, 0, 2.0 * TMath::Pi());
  G4UnionSolid *copperCutSolid
    = new G4UnionSolid("Inner+HoleMoved", copperInnerSolid, copperHoleSolid,
		       0, G4ThreeVector(0,0,-fInnerVolumeWidth/2));
  G4SubtractionSolid *copperShieldSolid
    = new G4SubtractionSolid("copper_Box-cut_", copperBoxSolid,copperCutSolid);
  G4Tubs *cryostatSolid
    = new G4Tubs("cryostatSolid", 0, fCryostatOuterRadius,
		 fCryostatHeight/2., 0, 2.0 * TMath::Pi());
  G4Tubs *cryostatCavitySolid
    = new G4Tubs("cryostatCavitySolid", 0,
		 fCryostatOuterRadius - fCryostatThickness, 
		 fCryostatHeight/2. - fAlPlateThickness,
		 0., 2.0*TMath::Pi());
  G4Tubs *cryostatPuckSolid
    = new G4Tubs("cryostatPuckSolid", 0, fCrystalRadius, 
		 fCryostatPuckThickness/2, 0., 2.0*TMath::Pi());
  G4Tubs *leadCylinderSolid
    = new G4Tubs("leadCylinderSolid", 0, fCryostatOuterRadius,
		 fLeadCylinderHeight/2.-0.001, 0., 2.0*TMath::Pi());
  //-------------------------------------------------------------------------//
  //LOGICALS
  //-------------------------------------------------------------------------//
  theDetectorLogical
    = new G4LogicalVolume(outerVolumeSolid, AirMat, 
			  "outerVolumeLogical", 0, 0, 0, true);
  fLeadShieldLogical
    = new G4LogicalVolume(leadShieldSolid, LeadMat, 
			  "leadShieldLogical", 0, 0, 0, true);
  fInnerVolumeLogical
    = new G4LogicalVolume(innerVolumeSolid, AirMat, 
			  "innerVolumeLogical", 0, 0, 0, true);
  fCopperShieldLogical
    = new G4LogicalVolume(copperShieldSolid, CopperMat,
			  "innerVolumeLogical", 0, 0, 0, true);
  fCryostatLogical
    = new G4LogicalVolume(cryostatSolid, Aluminum,
			 "cryostatLogical", 0, 0, 0, true);
  fCryostatCavityLogical
    = new G4LogicalVolume(cryostatCavitySolid, Vacuum, 
			  "cryostatCavityLogical", 0, 0, 0, true);
  fCryostatPuckLogical
    = new G4LogicalVolume(cryostatPuckSolid, Aluminum,
			  "cryostatPuckLogical", 0, 0, 0, true);
  fLeadCylinderLogical
    = new G4LogicalVolume(leadCylinderSolid, LeadMat, 
 			  "leadCylinderLogical", 0, 0, 0, true);
  //-------------------------------------------------------------------------//
  //PLACEMENTS
  //-------------------------------------------------------------------------//
  G4double xtalZ 
    = -1*(fCrystalOffset + fCrystalHeight/2. 
	  - fCryostatHeight/2. + fCryostatThickness);
  G4ThreeVector xtalPlacement(0,0, xtalZ);
  G4ThreeVector cryostatPlacement(0, 0, fCryostatOffset - fCryostatHeight/2.
				  - fInnerVolumeHeight/2.);
  G4ThreeVector cryostatPuckPlacement(0, 0, xtalZ - fCrystalHeight/2
				      - 1.0*mm - fCryostatPuckThickness/2);
  G4ThreeVector leadCylinderPlacement(0,0, fCryostatOffset -fInnerVolumeHeight/2.
				      - fLeadCylinderHeight/2. 
				      - fCryostatHeight - .001);
  fLeadShieldPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fLeadShieldLogical, 
			"leadShieldPhysical", theDetectorLogical, false, 0);
  fInnerVolumePhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fInnerVolumeLogical, 
			"innerVolumePhysical", fLeadShieldLogical, false, 0);
  fCopperShieldPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fCopperShieldLogical,
			"copperShieldPhysical", fInnerVolumeLogical, false, 0);
  fCryostatPhysical
    = new G4PVPlacement(0, cryostatPlacement, fCryostatLogical, 
			"cryostatPhysical", fInnerVolumeLogical, false, 0);
  fCryostatCavityPhysical
    = new G4PVPlacement(0, G4ThreeVector(0,0,0), fCryostatCavityLogical,
		    "cryostatCavityPhysical", fCryostatLogical, false, 0);
  fIdealCoaxCrystal->ConstructDetector();
  fCrystalPhysical
    = new G4PVPlacement(0, xtalPlacement, 
			fIdealCoaxCrystal->GetDetectorLogical(), 
		       "crystalPhysical", fCryostatCavityLogical, false, 0);
  fIdealCoaxCrystal->GetDetectorLogical()->SetVisAttributes(G4Color(1.0,0,0,.8));
  fCryostatPuckPhysical
    = new G4PVPlacement(0, cryostatPuckPlacement, fCryostatPuckLogical, 
			"cryostatPuckPhysical",fCryostatCavityLogical, 
			false, 0);
  fLeadCylinderPhysical
    = new G4PVPlacement(0, leadCylinderPlacement, fLeadCylinderLogical,
			"leadCylinderPhysical", fInnerVolumeLogical, 
			false, 0);
  if(fUsingSamples) InitializeSample();
//---------------------------------------------------------------------------//
  G4VisAttributes* yellowva = new G4VisAttributes(G4Color(1.0,1.0,0.0,0.7));
  //G4VisAttributes* green = new G4VisAttributes(G4Color(0.0,1,0.0,.3));
  //G4VisAttributes* blue = new G4VisAttributes(G4Color(0.,0.,1.0)); 
  //G4VisAttributes* red = new G4VisAttributes(G4Color(1.0,0.0,0.0));
  //G4VisAttributes* cyan = new G4VisAttributes(G4Color(0.0,1.0,1.0));
  //G4VisAttributes* magenta = new G4VisAttributes(G4Color(1.0,0.0,1.0));
  G4VisAttributes* grey = new G4VisAttributes(G4Color(0.6,0.6,0.6,.4)); 
  G4VisAttributes* darkgrey = new G4VisAttributes(G4Color(0.9,0.9,0.9,.2));
  G4VisAttributes* darkestgrey = new G4VisAttributes(G4Color(1,1,1));
  G4VisAttributes* copper = new G4VisAttributes(G4Color(1, .625, .277170913, .3));

  theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fLeadShieldLogical->SetVisAttributes(darkgrey); 
  fLeadCylinderLogical->SetVisAttributes(yellowva);
  fInnerVolumeLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fCopperShieldLogical->SetVisAttributes(copper);
  fCryostatLogical->SetVisAttributes(grey);
  fCryostatCavityLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fCryostatPuckLogical->SetVisAttributes(darkestgrey);
}
//---------------------------------------------------------------------------//

void MJGeometryMelissaDetector::InitializeSample(){
  if(fSample == "Teflon"){InitializeTeflon();}
  else if(fSample == "Steel"){InitializeSteel();}
  else if(fSample == "RockBucket"){InitializeRockBucket();}
  else if(fSample == "RockBag"){InitializeRockBag();}
  else if(fSample == "SuperInsulation"){InitializeSuperInsulation();}
  else if(fSample == "AlRings"){InitializeAlRings();}
  else if(fSample == "CalibrationSource"){InitializeCalibrationSource();}
  else if(fSample == "PMT"){InitializePMT();}
  else if(fSample == "Zeolite"){InitializeZeolite();}
  else if(fSample == "FoamInsulation"){InitializeFoamInsulation();}
  else if(fSample == "Marinelli"){InitializeMarinelli();}
}
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeTeflon(){
  fSampleHeight = 3.0 * cm;
  fSampleWidth = 3.0 * cm;
  fSampleLength = 3.0 * cm;
  SetPosition(0, 0, fSampleHeight/2.+ 0.001*cm + fCryostatOffset
	      - fInnerVolumeHeight/2.);
  G4Box *TeflonSolid
    = new G4Box("Teflon", fSampleLength/2.,
		fSampleWidth/2., fSampleHeight/2.);
  fTeflonLogical
    = new G4LogicalVolume(TeflonSolid, G4Material::GetMaterial("Teflon"),
			  "Teflon", 0, 0, 0, true);
  fTeflonPhysical
    = new G4PVPlacement(0, fSamplePlacement, fTeflonLogical,"Teflon",
			fInnerVolumeLogical, false, 0);
  }
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeSteel(){
  fSampleHeight = .4 * cm;
  fSampleWidth = 3.0 * cm;
  fSampleLength = 10. * cm;
  SetPosition(0, 0, fCryostatOffset -fInnerVolumeHeight/2. 
	      + fSampleHeight/2.+ 0.001*cm);
  G4Box *SteelSolid
    = new G4Box("Steel", fSampleLength/2., 
		fSampleWidth/2., fSampleHeight/2.);
  fSteelLogical
    = new G4LogicalVolume(SteelSolid, G4Material::GetMaterial("SS303"),
			  "Steel", 0, 0, 0, true);
  fSteelPhysical
    = new G4PVPlacement(0, fSamplePlacement, fSteelLogical, 
			"Steel", fInnerVolumeLogical, false, 0);

  fSteelLogical->SetVisAttributes(G4Color(0.0,0.0,1.0));
  }
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeRockBucket(){
  G4Material *crushedRock
    = new G4Material("crushed rock", 1.474 * g/cm3, 2);
  G4Element* elO
    = new G4Element("Oxygen" ,"O" , 8., 16*g/mole);
  G4Element* elSi
    = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  crushedRock->AddElement(elSi, 1);
  crushedRock->AddElement(elO, 2);
  fSampleHeight = 5.0 * cm;
  fSampleWidth = 6.6 * cm;
  SetPosition(G4ThreeVector(0, 0,fCryostatOffset -fInnerVolumeHeight/2. 
			    + fSampleHeight/2.+ 0.001*cm));
  if(fPlacementOption=="InCorner"){
  SetPosition(G4ThreeVector(-fInnerVolumeWidth/2+fSampleWidth+fCopperShieldThickness,
			    -fInnerVolumeWidth/2+fSampleWidth+fCopperShieldThickness,
			    -fInnerVolumeHeight/2+fSampleHeight/2+fCopperShieldThickness));
  }
  G4Cons *rockBucketSolid
    = new G4Cons("RockBucket", 0., 5.0 * cm, 0. ,6.5 * cm, 
		 fSampleHeight/2, 0., 2.0 * TMath::Pi());
  fRockBucketLogical
    = new G4LogicalVolume(rockBucketSolid, crushedRock,
			  "RockBucket", 0, 0, 0, true);
  fRockBucketPhysical
    = new G4PVPlacement(0, fSamplePlacement, fRockBucketLogical, 
			"RockBucket", fInnerVolumeLogical, false, 0);
  fRockBucketLogical->SetVisAttributes(G4Color(0.0,0.0,1.0,.6));
 }
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeRockBag(){
  G4Material *crushedRock
    = new G4Material("crushed rock", 1.474 * g/cm3, 2);
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
  SetPosition(G4ThreeVector(0, 0,fCryostatOffset -fInnerVolumeHeight/2. 
			    + fSampleHeight/2.+ 0.001*cm));
  G4Ellipsoid *RockBagSolid
    = new G4Ellipsoid("RockBag", fSampleWidth/2., fSampleHeight/2.,
	      fSampleLength*3/4.,-fSampleLength/2., fSampleLength/2.);
  fRockBagLogical
    = new G4LogicalVolume(RockBagSolid, crushedRock,"RockBag", 0, 0, 0, true);
  fRockBagPhysical
    = new G4PVPlacement(xRot, fSamplePlacement, fRockBagLogical, 
			"RockBag", fInnerVolumeLogical, false, 0);
  fRockBagLogical->SetVisAttributes(G4Color(0.0,0.0,1.0));
  }
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeSuperInsulation()
{
  G4Element* elO
    = new G4Element("Oxygen", "O", 8., 16*g/mole);
  G4Element* elSi
    = new G4Element("Silicon", "Si", 14., 28.09*g/mole);

  G4double density;
  G4Material* Aerog = new G4Material("Aerogel", density = 0.257 * g/cm3, 2);
  Aerog->AddElement(elSi, 1);
  Aerog->AddElement(elO, 2);

  fSampleHeight = .75 * in;
  fSampleWidth = 12 * in;
  fSampleLength = 12 * in;
  SetPosition(G4ThreeVector(0, 0, fCryostatOffset - fInnerVolumeHeight/2. 
			    + fSampleHeight/2.+ 0.001*cm));
  G4Box *superInsulationSolid
    = new G4Box("SuperInsulation", fSampleLength/2., 
		fSampleWidth/2., fSampleHeight/2.);
  fSuperInsulationLogical
    = new G4LogicalVolume(superInsulationSolid, Aerog,
			  "SuperInsulation", 0, 0, 0, true);
  fSuperInsulationPhysical
    = new G4PVPlacement(0, fSamplePlacement, fSuperInsulationLogical, 
			"SuperInsulation", fInnerVolumeLogical, 
			false, 0);
  fSuperInsulationLogical->SetVisAttributes(G4Color(0.0,0.0,1.0));

  G4double SupInsVol = superInsulationSolid->GetCubicVolume();

  MGLog(trace) << "Super Insulation Volume: " << SupInsVol/cm3 
	       << " cm3" << endlog;
  MGLog(trace) << "Super Insulation Mass: " <<  (SupInsVol * density)/g
	       << " g" << endlog;
  }
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeAlRings(){
  fSampleHeight = .5 * in;
  G4double SampleInnerRadius = 7.5/2 * in;
  G4double SampleOuterRadius = 10.0/2 * in;
  SetPosition(0, 0,fCryostatOffset -fInnerVolumeHeight/2. 
	      + fSampleHeight/2.+ 0.001*cm-1.5*in);

  G4Tubs *AlRingsSolid
    = new G4Tubs("AlRings", SampleInnerRadius, SampleOuterRadius,
		 fSampleHeight/2., 0, 2.0 * TMath::Pi());
  fAlRingsLogical
    = new G4LogicalVolume(AlRingsSolid, G4Material::GetMaterial("Aluminum"),
			  "AlRings", 0, 0, 0, true);
  fAlRingsLogical->SetVisAttributes(green);
  fAlRingsPhysical
    = new G4PVPlacement(0, fSamplePlacement, fAlRingsLogical,"AlRings",
			fInnerVolumeLogical, false, 0);
}

//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeCalibrationSource(){
  fSampleHeight = 0.01*mm;
  G4double SampleRadius = 1.5*mm;
  if(fPlacementOption=="InCorner"){
  SetPosition(fInnerVolumeWidth/2-SampleRadius-0.01*mm-.5*in-fCopperShieldThickness,
	      fInnerVolumeWidth/2-SampleRadius-0.01*mm-.5*in-fCopperShieldThickness,
	      fSampleHeight/2-fInnerVolumeHeight/2+.001*mm-fCopperShieldThickness);
  }
  else if(fPlacementOption=="OnTop"){
  SetPosition(0, 0, fCryostatOffset -fInnerVolumeHeight/2. 
	      + fSampleHeight/2.+ 0.001*cm);
  }
  else{
  SetPosition(2*in,
	      fInnerVolumeWidth/2-SampleRadius-0.01*mm+fCopperShieldThickness,
	      fSampleHeight/2-fInnerVolumeHeight/2+.001*mm+6*in); 
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
			"CalibrationSource",fInnerVolumeLogical, false,0);
}
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializePMT(){
  G4double fBasesRadius = 4.6*cm;
  G4double fBasesHeight = 2.25*mm;
  G4double fResistorsWidth = 4.0*cm;
  G4double fResistorsLength = 5.0*cm; 
  G4double fResistorsHeight = 2.25*mm;
  G4double fCapacitorsWidth = 3.4*cm;
  G4double fCapacitorsLength = 4.1*cm;
  G4double fCapacitorsHeight = 5.0*mm;


 SetPosition(0., 0., fCryostatOffset -fInnerVolumeHeight/2. 
	      + fBasesHeight/2.+ 0.01*cm);

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
void MJGeometryMelissaDetector::InitializeZeolite(){
  fSampleHeight = 1.5 * cm;
  G4double SampleInnerRadius = 8.4/2 * cm;
  G4double SampleOuterRadius = 14.1/2 * cm;
  SetPosition(0, 0, fCryostatOffset - fInnerVolumeHeight/2 + fSampleHeight/2 - 7.2 * cm);

  G4Tubs *ZeoliteSolid
    = new G4Tubs("Zeolite", SampleInnerRadius, SampleOuterRadius,
		 fSampleHeight/2., 0, 2.0 * TMath::Pi());
  fZeoliteLogical
    = new G4LogicalVolume(ZeoliteSolid, G4Material::GetMaterial("Teflon"),
			  "Zeolite", 0, 0, 0, true);
  fZeoliteLogical->SetVisAttributes(green);
  fZeolitePhysical
    = new G4PVPlacement(0, fSamplePlacement, fZeoliteLogical,"Zeolite",
			fInnerVolumeLogical, false, 0);
}
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeFoamInsulation(){
  G4Element* elH
    = new G4Element("Hydrogen", "H", 1., 1*g/mole);
  G4Element* elC
    = new G4Element("Carbon", "C", 6., 12*g/mole);
  
  G4double density;
  G4Material* Foam = new G4Material("Foam", density = 0.07 * g/cm3, 2);
  Foam->AddElement(elH, 4);
  Foam->AddElement(elC, 2);

  G4double fFoamTopHeight = 5. * cm;
  G4double fFoamTopRadius = 17./2.* cm;
  G4double fFoamMiddleHeight = 1.5 * cm;
  G4double fFoamMiddleOuterRadius =14.1/2. * cm;
  G4double fFoamMiddleInnerRadius = 8.4/2.* cm;
 
SetPosition(0., 0., fCryostatOffset -fInnerVolumeHeight/2. 
	      + fFoamTopHeight/2.+ 0.01*cm);

G4Tubs *FoamTopSolid
  = new G4Tubs("FoamTop", 0, fFoamTopRadius, 
	       fFoamTopHeight/2.,0,2.0 *TMath::Pi());

G4Tubs  *FoamMiddleSolid
  = new G4Tubs("FoamMiddle",fFoamMiddleInnerRadius,fFoamMiddleOuterRadius,
	       fFoamMiddleHeight/2.,0,2.0 *TMath::Pi());

G4Tubs  *FoamBottomSolid
  = new G4Tubs("FoamBottom",fFoamMiddleInnerRadius,fFoamMiddleOuterRadius,
	       fFoamMiddleHeight/2.,0,2.0 *TMath::Pi());

G4UnionSolid *FoamSolid1
  = new G4UnionSolid("Foam1", FoamTopSolid,FoamMiddleSolid,0,G4ThreeVector(0,0,-fFoamMiddleHeight));

 G4UnionSolid *FoamSolid
   = new G4UnionSolid("Foam", FoamSolid1, FoamBottomSolid,0,G4ThreeVector(0,0,-7.2 * cm - fFoamTopHeight/2.));

fFoamInsulationLogical
    = new G4LogicalVolume(FoamSolid, Foam,
			  "FoamInsulation",0,0,0,true);

fFoamInsulationLogical->SetVisAttributes(green);

fFoamInsulationPhysical
    = new G4PVPlacement(0, fSamplePlacement, fFoamInsulationLogical,
			"FoamInsulation",fInnerVolumeLogical, false,0);
}
//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::InitializeMarinelli(){
	G4double fBottomHeight = 2.8 * in;
	G4double fTopHeight = 2.2 * in;
	G4double SampleInnerRadius = 8.4/2 * cm;
	G4double SampleOuterRadius = 14.1/2 * cm;
	SetPosition(0, 0, fCryostatOffset -fInnerVolumeHeight/2. 
	      - fBottomHeight/2.+ 0.01*cm);

	G4Tubs *MarinelliSolidBottom
		= new G4Tubs("MarinelliBottom", SampleInnerRadius, SampleOuterRadius,
			fBottomHeight/2., 0, 2.0*TMath::Pi());

	G4Tubs *MarinelliSolidTop
		= new G4Tubs("MarinelliTop", 0, SampleOuterRadius,
			fTopHeight/2., 0, 2.0*TMath::Pi());
	
	G4UnionSolid *MarinelliSolid
 		 = new G4UnionSolid("Marinelli", MarinelliSolidBottom,MarinelliSolidTop,0,G4ThreeVector(0,0,fBottomHeight/2. + fTopHeight/2.));

	fMarinelliLogical
		= new G4LogicalVolume(MarinelliSolid, G4Material::GetMaterial("Teflon"),
			"Marinelli", 0,0,0,true);
	
	fMarinelliLogical->SetVisAttributes(cyan);
	
	fMarinelliPhysical
		= new G4PVPlacement(0, fSamplePlacement, fMarinelliLogical, "Marinelli",
			fInnerVolumeLogical, false, 0);
}

//---------------------------------------------------------------------------//
void MJGeometryMelissaDetector::SetDefaults()
{ 
  fInnerVolumeHeight = 9. * 5.08 * cm; 
  fInnerVolumeWidth  = 8. * 5.08 * cm;
  fCopperShieldThickness = 1. * in;
  fCopperHoleRadius = 2. * in;
  fLeadShieldHeight = 15.0 * 5.08 * cm;      // 15 lead bricks high
  fLeadShieldWidth = 14.0 * 5.08 * cm;       // 14 lead bricks wide
  fLeadCylinderHeight = 2.52 * in;
  fOuterVolumeRadius = 70.0 * cm;
  fAlPlateThickness = 0.89 * mm;
  fCryostatHeight = 13.6 * cm;  
  fCryostatOuterRadius = 4.1 * cm;
  fCryostatOffset = 7.875 * in;            
  fCryostatThickness = 4.2 * mm;
  fCrystalOffset = 5. * mm;
  fCrystalHeight = 6.45*cm;
  fCrystalRadius = 3.25*cm;
  fCryostatPuckThickness = 1.0*cm;
  
  fIdealCoaxCrystal->SetHeight(fCrystalHeight);
  fIdealCoaxCrystal->SetRadius(fCrystalRadius);
  fIdealCoaxCrystal->SetCoreRadius(3.75 * mm); 
  fIdealCoaxCrystal->SetCoreDepth(5.0 * cm);
  fIdealCoaxCrystal->SetCornerRadius(6. * mm); // * Guesstimate
  fIdealCoaxCrystal->SetOuterDeadLayerThickness(1.06 * mm);
  fIdealCoaxCrystal->SetCoreDeadLayerThickness(0.3 * micrometer);
  fIdealCoaxCrystal->CoreBubbleOn();
  fIdealCoaxCrystal->SetDeadLayerConsidered(true);
  fIdealCoaxCrystal->SetCutInHalf(false);
  fIdealCoaxCrystal->SetIsActive(true);
  fIdealCoaxCrystal->SetIsOneOfMany(false);
  fIdealCoaxCrystal->SetG4MaterialName("Germanium-Nat");
}
//---------------------------------------------------------------------------//
