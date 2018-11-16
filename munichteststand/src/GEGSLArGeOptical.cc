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
 * AUTHOR: Nuno Barros (forking Luciano Pandola's GEGSLArGe)
 * FIRST SUBMISSION: 05-18-2012
 *
 * REVISION:
 *
 * 05-19-2012, N. Barros , Cleaned some superfluous code.
 * 						   Implemented a new source geometry with thinner encapsulation for Co60.
 * 						   Adjusted optical properties to LArGe measurements (triplet lifetime, yield and PMT QE).
 *
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"

#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "geometry/MGGeometryDetector.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryPMTSD.hh"

#include "munichteststand/GEGSLArGeOptical.hh"
#include "munichteststand/GEGSLArGeOpticalMessenger.hh"
#include "io/MGLogger.hh"

// -- ROOT include
#include "TGraph.h"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEGSLArGeOptical::GEGSLArGeOptical() :
MGGeometryDetector(""),liquidLogical(0),fCrystalSegmentLogical(0),fCrystalDeadLayerLogical(0),
fBuildCalibSource(false),
fCalibSourceGeometry("ThWire"),
fSourceType("internal"), // defaults to some random position
fBuildOptics(false)
{
	fBuildCrystals = true;
	CrystalInitialization();
	fMessenger = new GEGSLArGeOpticalMessenger(this);
}

// --------------------------------------------------------------------------- 

GEGSLArGeOptical::~GEGSLArGeOptical()
{
	delete fMessenger;
}

// --------------------------------------------------------------------------- 

void GEGSLArGeOptical::ConstructDetector()
{

	// Get the material manager
	G4NistManager* nistManager = G4NistManager::Instance();


	// construct airbox
	G4double AirBoxX = 30.0*m;
	G4double AirBoxY = 30.0*m;
	G4double AirBoxZ = 40.0*m;

	G4Box* airBox = new G4Box("airBox",AirBoxX/2.,AirBoxY/2.,AirBoxZ/2.);
	G4LogicalVolume* airBoxLogical =
			new G4LogicalVolume(airBox,G4Material::GetMaterial("Air"),
					"airBoxLogical");

	//Now starts the shielding
	// 1) PE
	G4double totalheight = 3580*mm;
	G4double totaldistance = 2490*mm;
	G4double zplans[2]={-totalheight/2.,totalheight/2.};
	G4double innerdistance[2]={0.,0.};
	G4double outerdistance[2]={totaldistance/2.,totaldistance/2.};
	G4Polyhedra* pe_shielding_poly = new G4Polyhedra("pe_shielding_poly",
			0.,twopi,8,2,
			zplans,innerdistance,outerdistance);
	G4LogicalVolume* peShieldingLogical =
			new G4LogicalVolume(pe_shielding_poly,G4Material::GetMaterial("BoronPolyethylene"),
					"peShieldingLogical");
	/*G4VPhysicalVolume* peShieldingPhysical =*/ new G4PVPlacement(0,
			G4ThreeVector(0,0,0),peShieldingLogical,"PE_shield",airBoxLogical,false,0);

	// 2) Steel
	G4double PEThick = 20*cm;
	totalheight = totalheight - 2*PEThick;
	totaldistance = totaldistance - 2*PEThick;
	zplans[0]=-totalheight/2.;
	zplans[1]=totalheight/2.;
	outerdistance[0]=totaldistance/2.;
	outerdistance[1]=totaldistance/2.;
	G4Polyhedra* steel_shielding_poly = new G4Polyhedra("steel_shielding_poly",
			0.,twopi,8,2,
			zplans,innerdistance,outerdistance);
	G4LogicalVolume* steelShieldingLogical =
			new G4LogicalVolume(steel_shielding_poly,G4Material::GetMaterial("Steel"),
					"steelShieldingLogical");
	/*G4VPhysicalVolume* steelShieldingPhysical =*/ new G4PVPlacement(0,
			G4ThreeVector(0,0,0),steelShieldingLogical,"Steel_shield",peShieldingLogical,false,0);

	// 3) Lead
	G4double SteelThick = 23.0*cm;
	totalheight = totalheight - 2*SteelThick;
	totaldistance = totaldistance - 2*SteelThick;
	zplans[0]=-totalheight/2.;
	zplans[1]=totalheight/2.;
	outerdistance[0]=totaldistance/2.;
	outerdistance[1]=totaldistance/2.;
	G4Polyhedra* lead_shielding_poly = new G4Polyhedra("lead_shielding_poly",
			0.,twopi,8,2,
			zplans,innerdistance,outerdistance);
	G4LogicalVolume* leadShieldingLogical =
			new G4LogicalVolume(lead_shielding_poly,G4Material::GetMaterial("MetalLead"),
					"leadShieldingLogical");
	//G4VPhysicalVolume* leadShieldingPhysical =
			new G4PVPlacement(0,G4ThreeVector(0,0,0),leadShieldingLogical,"Lead_shield",
					steelShieldingLogical,false,0);
	// 4) Copper
	G4double LeadThick = 10.0*cm;
	totalheight = totalheight - 2*LeadThick;
	totaldistance = totaldistance - 2*LeadThick;
	zplans[0]=-totalheight/2.;
	zplans[1]=totalheight/2.;
	outerdistance[0]=totaldistance/2.;
	outerdistance[1]=totaldistance/2.;
	G4Polyhedra* copper_shielding_poly = new G4Polyhedra("copper_shielding_poly",
			0.,twopi,8,2,
			zplans,innerdistance,outerdistance);
	G4LogicalVolume* copperShieldingLogical =
			new G4LogicalVolume(copper_shielding_poly,G4Material::GetMaterial("MetalCopper"),
					"copperShieldingLogical");
	//G4VPhysicalVolume* copperShieldingPhysical =
			new G4PVPlacement(0,G4ThreeVector(0,0,0),copperShieldingLogical,"Copper_shield",
					leadShieldingLogical,false,0);


	G4double dewarThickness = 50.0*mm;
	G4double liquidRadius = 45.0*cm; //inner radius of the dewar
	G4double liquidHeight= 182.0*cm/2.; //inner half-height of the dewar

	// Reflective foil with WLS. Let's consider a total thickness of 2*mm = 1*mm of reflective foil + 1*mm of WLS
	G4double reflectorThickness = 2*mm;
	G4double reflectorRadius = liquidRadius;
	G4double reflectorHeight = liquidHeight;

	G4double dewarRadius = liquidRadius + dewarThickness;
	G4double dewarHeight = liquidHeight + dewarThickness;

	// Make some adjustments to the dimensions in case we want to run
	// optical photons in LArGe : accommodate the reflective foils
	if (fBuildOptics) {
		// Reflector radius should become what is now the liquid radius
		// and the liquid radius should then become
		// reflectorRadius-reflectorThickness
		liquidRadius = reflectorRadius - reflectorThickness;
		liquidHeight = reflectorHeight - reflectorThickness;
	}

	// 5) Vacuum hollow space between shielding and dewar
	//G4double innerShieldingRadius = 70.0*cm;
	G4double CopperThick = 7.7*cm;
	totalheight = totalheight - 2*CopperThick;
	totaldistance = totaldistance - 2*CopperThick;
	zplans[0]=-totalheight/2.;
	zplans[1]=totalheight/2.;
	outerdistance[0]=totaldistance/2.;
	outerdistance[1]=totaldistance/2.;
	G4Polyhedra* vacuum_shielding_poly = new G4Polyhedra("vacuum_shielding_poly",
			0.,twopi,8,2,
			zplans,innerdistance,outerdistance);
	G4LogicalVolume* vacuumShieldingLogical =
			new G4LogicalVolume(vacuum_shielding_poly,G4Material::GetMaterial("Vacuum"),
					"vacuumShieldingLogical");
	//G4VPhysicalVolume* vacuumShieldingPhysical =
			new G4PVPlacement(0,G4ThreeVector(0,0,0),vacuumShieldingLogical,"Vacuum_space",
					copperShieldingLogical,false,0);

	// Dewar
	G4Tubs* dewarTub =  new G4Tubs("dewar_tub",0.,dewarRadius,dewarHeight,
			0,twopi);
	G4LogicalVolume* dewarLogical =
			new G4LogicalVolume(dewarTub,G4Material::GetMaterial("MetalCopper"),
					"dewarLogical");
	//G4VPhysicalVolume* dewarPhysical =
			new G4PVPlacement(0,G4ThreeVector(0,0,0),dewarLogical,"Dewar",
					vacuumShieldingLogical,false,0);


	G4LogicalVolume* reflectorLogicalVM = NULL;
	G4LogicalVolume* reflectorLogicalWLS = NULL;

	// The positioning should be different based on whether we have the reflective foil or not.
	if (fBuildOptics) {
		// If we have optics, first build the reflective foil coating the dewar

		// Reflective foil covering the dewar. In fact we consider two:
		// 1. A second layer that is the reflector VM2000
		// 2. A first layer that is the WLS

		/// 1.) reflective foil (1*mm thick)
		// define the material: VM2000 (it's a polymer)
		// VM2000 (Polyethylen) //

		G4Element* C = new G4Element("Carbon", "C", 6 , 12.011*g/mole);
		G4Element* H = new G4Element("Hydrogen", "H", 1 , 1.008*g/mole);
		G4Material* VM2000 = new G4Material("VM2000", 0.9*g/cm3, 2);
		VM2000->AddElement(C, 2);
		VM2000->AddElement(H, 4);

		// TPB (approximate)
		G4Material* TPB = new G4Material("TPB", 1.0*g/cm3, 2, kStateSolid);
		TPB->AddElement(C, 22);
		TPB->AddElement(H, 28);


		G4Tubs* reflectorTub = new G4Tubs("reflectorTub",0., reflectorRadius,
						    reflectorHeight,0.,twopi);
		reflectorLogicalVM =
		    new G4LogicalVolume(reflectorTub,G4Material::GetMaterial("VM2000"),
					"reflectorLogicalVM");
		/*G4VPhysicalVolume* reflectorPhysicalVM = */new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),reflectorLogicalVM,
			 		      "reflector",dewarLogical,false,0);

		/// 2.) WLS coating (1*mm thick)
		/// -- The material is set to nylon, but it doesn't really matter
		/// -- probably using something like
		G4Tubs* wlsTub = new G4Tubs("wlsTub",0., reflectorRadius-reflectorThickness/2.0,
						    reflectorHeight-reflectorThickness/2.0,0.,twopi);
		reflectorLogicalWLS =
		    new G4LogicalVolume(wlsTub,G4Material::GetMaterial("TPB"),
					"reflectorLogicalWLS");
		/*G4VPhysicalVolume* reflectorPhysicalWLS = */new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),reflectorLogicalWLS,
			 		      "wls_coating",reflectorLogicalVM,false,0);



	}

	// Cryoliquid
	G4Tubs* liquidTub =  new G4Tubs("liquid_tub",0.,liquidRadius,liquidHeight,
			0,twopi);
	liquidLogical =
			new G4LogicalVolume(liquidTub,G4Material::GetMaterial("LiquidArgon"),
					"liquidLogical");
	if (fBuildOptics) {
	  new G4PVPlacement(0,G4ThreeVector(0,0,0),
			    liquidLogical,"LiquidArgon",
			    reflectorLogicalWLS,false,0);
	} else {
		new G4PVPlacement(0,G4ThreeVector(0,0,0),
				  liquidLogical,"LiquidArgon",
				  dewarLogical,false,0);

	}
	// small plate separating the PMTs-electronics from the cryoliquid

	G4double copperholeRadius = 14.0*cm;
	G4double copperplateHeight = 1*cm;
	G4double CopperPlate_zpos = liquidHeight-liquidHeight/4.;

	G4Tubs* CopperPlate = new G4Tubs("copper_plate", copperholeRadius, liquidRadius, copperplateHeight, 0, twopi);
	G4LogicalVolume* CopperPlateLogical = new G4LogicalVolume(CopperPlate, G4Material::GetMaterial("MetalCopper"), "CopperPlateLogical");


	new G4PVPlacement(0,G4ThreeVector(0, 0, CopperPlate_zpos), 
			  CopperPlateLogical, "copper_plate", 
			  liquidLogical, false,0);


	// PMT placement
	// Photomultiplier Cathodes

	char namephotocathode[50];
	char namepmtcoat[50];

	G4double fPMTStartingAngle = 0.0 * deg ;
	G4double fPMTFinalAngle    = 360.0 * deg ;

	G4double fPhotocathodeInnerRadius = 10.0 * cm ;
	G4double fPhotocathodeOuterRadius = 10.016 * cm ;
	G4double fPhotocathodeThetaStart = 90.0 * deg;
	G4double fPhotocathodeThetaEnd = 180.0 * deg;


	G4double xpos;
	G4double ypos;
	G4double zpos;

	G4double dPhi;
	G4double R_pos;

	G4int Num_PMTs = 0;

	G4Sphere* fPhotocathode = new G4Sphere("Photocathode",
			fPhotocathodeInnerRadius,
			fPhotocathodeOuterRadius,
			fPMTStartingAngle,
			fPMTFinalAngle,
			fPhotocathodeThetaStart,
			fPhotocathodeThetaEnd);

	G4LogicalVolume* fPhotocathodeLogical = new G4LogicalVolume(fPhotocathode,
			G4Material::GetMaterial("CathodeMetalAluminium"),
			"fPhotocathodeLogical");


	G4Sphere* fPmtWLSsolid = NULL;
	G4LogicalVolume* fPmtWLSlogical = NULL;

	if (fBuildOptics) {
		// Build the WLS coating of the cathodes
		fPmtWLSsolid = new G4Sphere("PmtWLScoat",
					fPhotocathodeOuterRadius,
					fPhotocathodeOuterRadius + 1*mm,
					fPMTStartingAngle,
					fPMTFinalAngle,
					fPhotocathodeThetaStart,
					fPhotocathodeThetaEnd);
		fPmtWLSlogical = new G4LogicalVolume(fPmtWLSsolid,
				G4Material::GetMaterial("TPB"),"PmtWLScoatLogical");
	}

	// 3 PMTs every 120 degrees at r/2

	dPhi = 120*deg;
	zpos = CopperPlate_zpos - copperplateHeight/2.;
	R_pos = 1/2.*liquidRadius;



	for (G4int i=0; i<3; i++)
	{

		sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs);
		xpos=R_pos*cos(dPhi*i);
		ypos=R_pos*sin(dPhi*i);

		new G4PVPlacement(0,
				  G4ThreeVector(xpos,ypos,zpos),
				  fPhotocathodeLogical,
				  namephotocathode,
				  liquidLogical,
				  false,
				  Num_PMTs);

		if (fBuildOptics) {
			sprintf(namepmtcoat,"PmtWLScoat_%d", Num_PMTs);
			new G4PVPlacement(0,
					  G4ThreeVector(xpos,ypos,zpos),
					  fPmtWLSlogical,
					  namepmtcoat,
					  liquidLogical,
					  false,
					  Num_PMTs);
		}

		Num_PMTs++;

	}


	// 6 PMTs every 40 degree with a gap after each two at ~3/4*r

	dPhi = 40*deg;
	zpos = CopperPlate_zpos - copperplateHeight/2.;
	R_pos = 3/4.*liquidRadius;

	for (G4int i=1; i<9; i++)
	{
		if(i==1 || i==2 || i==4 || i==5 || i==7 || i==8)
		{
			sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs);

			xpos=R_pos*cos(dPhi*i);
			ypos=R_pos*sin(dPhi*i);

			new G4PVPlacement(0,
					  G4ThreeVector(xpos,ypos,zpos),
					  fPhotocathodeLogical,
					  namephotocathode,
					  liquidLogical,
					  false,
					  Num_PMTs);

			if (fBuildOptics) {
				sprintf(namepmtcoat,"PmtWLScoat_%d", Num_PMTs);
				new G4PVPlacement(0,
						  G4ThreeVector(xpos,ypos,zpos),
						  fPmtWLSlogical,
						  namepmtcoat,
						  liquidLogical,
						  false,
						  Num_PMTs);
			}


			Num_PMTs++;

		}
	}



	// Black tape, covering the plastic scintillator sheets for the muon veto

	G4double tape_length = 200.4*cm;
	G4double tape_width  = 50.4*cm;
	G4double tape_height = 3.4*cm;

	G4Box* TapePlate1 = new G4Box("TapePlate1", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical1 = new G4LogicalVolume(TapePlate1, G4Material::GetMaterial("Nylon"), "TapePlateLogical1");
	
	G4Box* TapePlate2 = new G4Box("TapePlate2", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical2 = new G4LogicalVolume(TapePlate2, G4Material::GetMaterial("Nylon"), "TapePlateLogical2");

	G4Box* TapePlate3 = new G4Box("TapePlate3", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical3 = new G4LogicalVolume(TapePlate3, G4Material::GetMaterial("Nylon"), "TapePlateLogical3");

	G4Box* TapePlate4 = new G4Box("TapePlate4", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical4 = new G4LogicalVolume(TapePlate4, G4Material::GetMaterial("Nylon"), "TapePlateLogical4");	
	G4Box* TapePlate5 = new G4Box("TapePlate5", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical5 = new G4LogicalVolume(TapePlate5, G4Material::GetMaterial("Nylon"), "TapePlateLogical5");
	G4Box* TapePlate6 = new G4Box("TapePlate6", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical6 = new G4LogicalVolume(TapePlate6, G4Material::GetMaterial("Nylon"), "TapePlateLogical6");
	G4Box* TapePlate7 = new G4Box("TapePlate7", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical7 = new G4LogicalVolume(TapePlate7, G4Material::GetMaterial("Nylon"), "TapePlateLogical7");
	G4Box* TapePlate8 = new G4Box("TapePlate8", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical8 = new G4LogicalVolume(TapePlate8, G4Material::GetMaterial("Nylon"), "TapePlateLogical8");
	G4Box* TapePlate9 = new G4Box("TapePlate9", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical9 = new G4LogicalVolume(TapePlate9, G4Material::GetMaterial("Nylon"), "TapePlateLogical9");
	G4Box* TapePlate10 = new G4Box("TapePlate10", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical10 = new G4LogicalVolume(TapePlate10, G4Material::GetMaterial("Nylon"), "TapePlateLogical10");
	G4Box* TapePlate11 = new G4Box("TapePlate11", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical11 = new G4LogicalVolume(TapePlate11, G4Material::GetMaterial("Nylon"), "TapePlateLogical11");
	G4Box* TapePlate12 = new G4Box("TapePlate12", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical12 = new G4LogicalVolume(TapePlate12, G4Material::GetMaterial("Nylon"), "TapePlateLogical12");
	G4Box* TapePlate13 = new G4Box("TapePlate13", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical13 = new G4LogicalVolume(TapePlate13, G4Material::GetMaterial("Nylon"), "TapePlateLogical13");
	G4Box* TapePlate14 = new G4Box("TapePlate14", tape_length/2., tape_width/2., tape_height/2.);
	G4LogicalVolume* TapePlateLogical14 = new G4LogicalVolume(TapePlate14, G4Material::GetMaterial("Nylon"), "TapePlateLogical14");

	G4RotationMatrix rm_rot;
	rm_rot.rotateZ(90*deg);


	new G4PVPlacement(0,G4ThreeVector(50.2*cm, -25.2*cm, 2*m), 
			  TapePlateLogical1, "TapePlate1", airBoxLogical, false,0);
	new G4PVPlacement(0,G4ThreeVector(50.2*cm, 25.2*cm, 2*m), 
			  TapePlateLogical2, "TapePlate2", airBoxLogical, false,0);
	new G4PVPlacement(0,G4ThreeVector(50.2*cm, 75.6*cm, 2*m), 
			  TapePlateLogical3, "TapePlate3", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(50.2*cm, -75.6*cm, 2*m), 
			  TapePlateLogical4, "TapePlate4", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(50.2*cm, -126*cm, 2*m), 
			  TapePlateLogical5, "TapePlate5", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(50.2*cm, 126*cm, 2*m), 
			  TapePlateLogical6, "TapePlate6", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(25.2*cm, 0*cm, 2.034*m), 
			  TapePlateLogical7, "TapePlate7", airBoxLogical, false,0);
	new G4PVPlacement(0,G4ThreeVector(25.2*cm, 50.4*cm, 2.034*m), 
			  TapePlateLogical8, "TapePlate8", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(25.2*cm, -50.4*cm, 2.034*m), 
			  TapePlateLogical9, "TapePlate9", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(25.2*cm, -100.8*cm, 2.034*m), 
			  TapePlateLogical10, "TapePlate10", airBoxLogical, false, 0);
	new G4PVPlacement(0,G4ThreeVector(25.2*cm, 100.8*cm, 2.034*m), 
			  TapePlateLogical11, "TapePlate11", airBoxLogical, false, 0);

	new G4PVPlacement(G4Transform3D(rm_rot, 
					G4ThreeVector(-75.4*cm ,0, 2*m)),
			  TapePlateLogical12,"TapePlate12",airBoxLogical,false,0);
	new G4PVPlacement(G4Transform3D(rm_rot, 
					G4ThreeVector(-125.8*cm ,0, 2*m)),
			  TapePlateLogical13,"TapePlate13",airBoxLogical,false,0);
	new G4PVPlacement(G4Transform3D(rm_rot, 
					G4ThreeVector(-100.4*cm ,0, 2.034*m)),
			  TapePlateLogical14,"TapePlate14",airBoxLogical,false,0);

	// Plastic scintillator sheets for muon veto
	G4double scint_length = tape_length-0.4*cm;
	G4double scint_width  = tape_width-0.4*cm;
	G4double scint_height = tape_height-0.4*cm;

	G4Box* ScintPlate_hor = new G4Box("ScintPlate_hor",scint_length/2., scint_width/2., scint_height/2.);
	G4LogicalVolume* ScintPlate_horLogical = new G4LogicalVolume(ScintPlate_hor, G4Material::GetMaterial("PlasticScintillator"), "ScintPlate_horLogical");


	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical1,false,0);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical2,false,1);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical3,false,2);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical4,false,3);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical5,false,4);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical6,false,5);
	
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical7,false,6);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical8,false,7);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical9,false,8);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical10,false,9);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical11,false,10);
	
	new G4PVPlacement(0, G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical12,false,11);
	new G4PVPlacement(0, G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical13,false,12);
	new G4PVPlacement(0, G4ThreeVector(0,0,0),
			  ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical14,false,13);

	if (fBuildCrystals) BuildEncapsulation();
	// Here go the crystals
	if (fBuildCrystals) BuildCrystalsInside();

	//---------------------------------------------------------------------------------//
	// color and visualization attributes
	//---------------------------------------------------------------------------------//
	G4Colour  white   (1.0, 1.0, 1.0) ;
	G4Colour  grey    (0.5, 0.5, 0.5) ;
	G4Colour  lgrey   (.85, .85, .85) ;
	G4Colour  red     (1.0, 0.0, 0.0) ;
	G4Colour  blue    (0.0, 0.0, 1.0) ;
	G4Colour  cyan    (0.0, 1.0, 1.0) ;
	G4Colour  magenta (1.0, 0.0, 1.0) ;
	G4Colour  yellow  (1.0, 1.0, 0.0) ;
	G4Colour  orange  (.75, .55, 0.0) ;
	G4Colour  lblue   (0.0, 0.0, .75) ;
	G4Colour  lgreen  (0.0, .75, 0.0) ;
	G4Colour  green   (0.0, 1.0, 0.0) ;
	G4Colour  brown   (0.7, 0.4, 0.1) ;
	airBoxLogical->SetVisAttributes(G4VisAttributes::Invisible);
	//airBoxLogical->SetVisAttributes(new G4VisAttributes(orange));
	peShieldingLogical->SetVisAttributes(new G4VisAttributes(grey));
	steelShieldingLogical->SetVisAttributes(new G4VisAttributes(blue));
	leadShieldingLogical->SetVisAttributes(new G4VisAttributes(brown));
	copperShieldingLogical->SetVisAttributes(new G4VisAttributes(red));
	vacuumShieldingLogical->SetVisAttributes(new G4VisAttributes(red));
	dewarLogical->SetVisAttributes(new G4VisAttributes(lblue));
	liquidLogical->SetVisAttributes(new G4VisAttributes(lgrey));
	if (fBuildOptics) {
		reflectorLogicalVM->SetVisAttributes(new G4VisAttributes(lgreen));
	}
	if (fCrystalSegmentLogical)
	{
		fCrystalSegmentLogical->SetVisAttributes(new G4VisAttributes(red));
		fCrystalDeadLayerLogical->SetVisAttributes(new G4VisAttributes(magenta));
	}

	ScintPlate_horLogical->SetVisAttributes(new G4VisAttributes(green));


	//Sensitive detectors
	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	GEGeometrySD* CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
	GEGeometrySD* CryoSD = new GEGeometrySD("/mydet/gerda/N2buffer");
	GEGeometrySD* ScintSD = new GEGeometrySD("/mydet/gerda/scintPlate");

	// GEGeometrySD* PassivationSD  = new GEGeometrySD("/mydet/gerda/passivation");
	GEGeometrySD* DeadLayerSD    = new GEGeometrySD("/mydet/gerda/deadlayer");

	SDman -> AddNewDetector(CrystalSD);
	SDman -> AddNewDetector(CryoSD);
	SDman -> AddNewDetector(ScintSD);
	//  SDman -> AddNewDetector(PassivationSD);

	SDman -> AddNewDetector(DeadLayerSD);
	fCrystalDeadLayerLogical->SetSensitiveDetector(DeadLayerSD);


	if (fCrystalSegmentLogical)
	{
		fCrystalSegmentLogical -> SetSensitiveDetector(CrystalSD);
	} else {
		MGLog(error) << "| | | | Don't have a logical volume to register the crystal hits" << endlog;

	}
	liquidLogical->SetSensitiveDetector(CryoSD);
	ScintPlate_horLogical->SetSensitiveDetector(ScintSD);

	GEGeometryPMTSD* PMTSD = new GEGeometryPMTSD("Phcathode",Num_PMTs,"PhCathodeHC" );
	SDman->AddNewDetector(PMTSD);
	fPhotocathodeLogical->SetSensitiveDetector(PMTSD);


	SetDetectorLogical(airBoxLogical);


	// Build the source if the flag is specified.
	if (fBuildCalibSource) {
		// -- Source position is now user defined.

		// MGLog(trace) << "Crystal position : " << GetCrystalPosition(0,0) << endlog;

		fSourcePosition = GetCrystalPosition(0,0);
		if (fSourceType == "internal") {
		// shift the source by 8 cm along x (random)
		// Attach here a temporary variable to know where to place the external sources
		// The external source is placed approx 50 cm away from the center
			fSourcePosition.setY(GetCrystalPosition(0,0).getY() + 8 * cm);
		}
		else if (fSourceType == "external") {
			// shift the source by 50 cm along x (random)
			// Attach here a temporary variable to know where to place the external sources
			// The external source is placed approx 50 cm away from the center
			fSourcePosition.setY(GetCrystalPosition(0,0).getY() + 50 * cm);
		}

		MGLog(trace) << "Placing calibration source under position: " << fSourcePosition << endlog;

		// Now check the geometry of the source to be deployed
		if (fCalibSourceGeometry == "ThWire") {

		// The source is composed of different regions (see schematic)
		// An inner cylinder of Epoxy
		// Another inner cylinder of ceramic
		// Covered by a shell of stainless steel
			MGLog(routine) << "Building calibration source [ " << fCalibSourceGeometry
					<< " ]." << endlog;
			/*
	  MGLog(warning) << "=========== Starting material loop :" << endlog;
	  for (unsigned int im = 0; im < G4Material::GetNumberOfMaterials(); ++im ) {
		  MGLog(warning) << "Material : " << (*(G4Material::GetMaterialTable()))[im]->GetName() << endlog;
	  }
	  MGLog(warning) << "=========== Finished material loop." << endlog;
			 */
			G4Tubs* CalibSourceTubs = new G4Tubs("SourceCaseTubs",
					0.0,
					1.0 * mm,
					3.5 * mm,
					0.0 * deg,
					360 * deg);


			G4LogicalVolume* steelSourceLogical =
					new G4LogicalVolume(CalibSourceTubs,G4Material::GetMaterial("Steel"),
							"SourceCaseLogical");
			
			if (fSourceType == "internal") {
			  new G4PVPlacement(0,fSourcePosition,
						  steelSourceLogical,
						  "SourceCase",liquidLogical,false,0);
			} else if (fSourceType == "external") {
			  new G4PVPlacement(0,fSourcePosition,
					    steelSourceLogical,
					    "SourceCase",
					    vacuumShieldingLogical,false,0);
			} else {
				MGLog(error) << "Unknow source position spec: " << fSourceType << endlog;
			}

			// Now build the internal parts of the source:
			//      The 1mm ceramic Th source
			//       The epoxy layer
			G4Tubs* CeramicSourceTubs = new G4Tubs("CalibSourceTubs",
					0.0,
					0.5 * mm,
					0.5 * mm,
					0.0 * deg,
					360 * deg);

			G4Material* mat = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

			G4LogicalVolume* ceramicSourceLogical =
					new G4LogicalVolume(CeramicSourceTubs,mat,
							"CalibSourceLogical");

			/*G4VPhysicalVolume* ceramicShieldingPhysical =*/ new G4PVPlacement(0,
					G4ThreeVector(0,0,0) ,ceramicSourceLogical,"CalibSource",steelSourceLogical,false,0);

			// Add the epoxy layer to the botom of the source
			G4Tubs* epoxySourceTubs = new G4Tubs("SourceEpoxyTubs",
					0.0,
					0.5 * mm,
					1.5 * mm,
					0.0 * deg,
					360 * deg);


			G4LogicalVolume* epoxySourceLogical =
					new G4LogicalVolume(epoxySourceTubs,G4Material::GetMaterial("Epoxy_mod"),
							"SourceEpoxyLogical");

			/*G4VPhysicalVolume* epoxyShieldingPhysical =*/ new G4PVPlacement(0,
					G4ThreeVector(0,0,-0.5*mm - 1.5*mm) ,epoxySourceLogical,"EpoxySource",steelSourceLogical,false,0);
		}
		else if (fCalibSourceGeometry == "CoWire") {

			// Source similar to the Th one, but with thinner steel walls
			MGLog(routine) << "Building calibration source [ " << fCalibSourceGeometry
					<< " ]." << endlog;

				/*
		  MGLog(warning) << "=========== Starting material loop :" << endlog;
		  for (unsigned int im = 0; im < G4Material::GetNumberOfMaterials(); ++im ) {
			  MGLog(warning) << "Material : " << (*(G4Material::GetMaterialTable()))[im]->GetName() << endlog;
		  }
		  MGLog(warning) << "=========== Finished material loop." << endlog;
				 */
				G4Tubs* CalibSourceTubs = new G4Tubs("SourceCaseTubs",
						0.0,
						0.75 * mm,
						3.5 * mm,
						0.0 * deg,
						360 * deg);


				G4LogicalVolume* steelSourceLogical =
						new G4LogicalVolume(CalibSourceTubs,G4Material::GetMaterial("Steel"),
								"SourceCaseLogical");
				if (fSourceType == "internal") {
				  new G4PVPlacement(0,fSourcePosition,
						    steelSourceLogical,
						    "SourceCase",liquidLogical,false,0);
				} else if (fSourceType == "external") {
				  new G4PVPlacement(0,fSourcePosition,
						    steelSourceLogical,
						    "SourceCase",
						    vacuumShieldingLogical,false,0);
				} else {
				  MGLog(error) << "Unknow source position spec: " << fSourceType << endlog;
				}

				// Now build the internal parts of the source:
				//      The 1mm ceramic Th source
				//       The epoxy layer
				G4Tubs* CeramicSourceTubs = new G4Tubs("CalibSourceTubs",
						0.0,
						0.5 * mm,
						0.5 * mm,
						0.0 * deg,
						360 * deg);

				G4Material* mat = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

				G4LogicalVolume* ceramicSourceLogical =
						new G4LogicalVolume(CeramicSourceTubs,mat,
								"CalibSourceLogical");

				/*G4VPhysicalVolume* ceramicShieldingPhysical * =*/ new G4PVPlacement(0,
						G4ThreeVector(0,0,0) ,ceramicSourceLogical,"CalibSource",steelSourceLogical,false,0);

				// Add the epoxy layer to the botom of the source
				G4Tubs* epoxySourceTubs = new G4Tubs("SourceEpoxyTubs",
						0.0,
						0.5 * mm,
						1.5 * mm,
						0.0 * deg,
						360 * deg);


				G4LogicalVolume* epoxySourceLogical =
						new G4LogicalVolume(epoxySourceTubs,G4Material::GetMaterial("Epoxy_mod"),
								"SourceEpoxyLogical");

				/*G4VPhysicalVolume* epoxyShieldingPhysical =*/ new G4PVPlacement(0,
						G4ThreeVector(0,0,-0.5*mm - 1.5*mm) ,epoxySourceLogical,"EpoxySource",steelSourceLogical,false,0);
			} else {
			MGLog(error) << "Unknown source geometry [" << fCalibSourceGeometry << "]. This will most likely fail." << endlog;
		}

	}



    if (fBuildOptics) {
    	// --
    	// Build the optical properties
    	// --
    	RegisterArgonOpticalProperties();

    	// Read the TPB re-emission spectrum from file
    	InitializeTPBSpectrum();


   	  // All optical properties
   	  // Energy grid goes from UV (115 nm) to green (650 nm)
   	  G4int ji = 0;
   	  G4double ee=0;
   	  const G4int num = 500;
   	  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
   	  G4double PPSCHighE = LambdaE /(115*nanometer);
   	  G4double PPSCLowE = LambdaE /(650*nanometer); //598
   	  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
   	  G4double LAr_SCPP[num];
   	  for (ji = 0; ji < num; ji++)
   	    {
   	      ee=PPSCLowE+ ((G4double)ji) * dee;
   	      LAr_SCPP[ji]=ee;
   	    }


   	  //Reflector (VM2000)
   	  //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm
   	  //and ~15% below it (almost a step-function)

   	  G4double Reflectivity[num];
   	  G4double Efficiency[num];

   	  for (ji=0;ji < num; ji++)
   	    {
   	      if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
   	    	  Reflectivity[ji] = 0.98; //visible
   	      else
   	    	  Reflectivity[ji] = 0.15; //UV

   	      Efficiency[ji] = 0.0;
   	    }
   	  G4MaterialPropertiesTable *vmOpTable = new G4MaterialPropertiesTable();
   	  vmOpTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);

   	  // Follow method used in WArP:

   	  G4OpticalSurface* reflOptSurface = new G4OpticalSurface("VM_surface");
   	  /*G4LogicalSkinSurface* reflSurface =*/ new G4LogicalSkinSurface("VM_surface",reflectorLogicalVM,reflOptSurface);
   	  reflOptSurface->SetType(dielectric_dielectric);
   	  reflOptSurface->SetFinish(groundfrontpainted); // Only Lambertian reflection
   	  reflOptSurface->SetMaterialPropertiesTable(vmOpTable);
   	  // data from WArP
   	  WLSyield = 1.2;

   	  G4double WLS_absorption[num];
   	  G4double WLS_emission[num];
   	  G4double Refraction[num];
   	  G4MaterialPropertiesTable *tpbTable = new G4MaterialPropertiesTable();
   	  for (ji=0;ji < num; ji++)
   	    {
   	      Refraction[ji] = 1.65; //this is just a guess
   	      if (LAr_SCPP[ji] > 8*eV)
   	    	  // For the moment set it to always absorb UV photons
   	    	  WLS_absorption[ji] = 0.001*nm; //absorbs UV (always)
   	      else
   	    	  WLS_absorption[ji] = 1000.*m; //otherwise imperturbed

   	      WLS_emission[ji] = TPBEmissionSpectrum(LAr_SCPP[ji]);
//   	      MGLog(routine) << "VM2000:  WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Energy :" << LAr_SCPP[ji]/eV << " eV Emission : " <<
//   	      WLS_emission[ji] << " Absorption : " << WLS_absorption[ji]/m << " m."<< endlog;
   	    }
   	  tpbTable->AddProperty("RINDEX",LAr_SCPP,Refraction,num);
   	  tpbTable->AddProperty("WLSABSLENGTH",LAr_SCPP,WLS_absorption,num);
   	  tpbTable->AddProperty("WLSCOMPONENT",LAr_SCPP,WLS_emission,num);
   	  // From WArP
   	  tpbTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
   	  tpbTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSyield);
   	  G4Material::GetMaterial("TPB")->SetMaterialPropertiesTable(tpbTable);

   	  //Germanium - date taken from literature
   	  for (ji=0;ji < num; ji++)
   	    {
   	      Reflectivity[ji] = 0.60;
   	    }
   	  G4MaterialPropertiesTable *germOptTable = new G4MaterialPropertiesTable();
   	  germOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
   	  G4OpticalSurface* germOptSurface
   	    = new G4OpticalSurface("Germ surface",glisur,polished,dielectric_metal);
   	  germOptSurface->SetMaterialPropertiesTable(germOptTable);

   	  //Associates the OpticalSurface to the actual volumes
   	  new G4LogicalSkinSurface("germanium_surf",fCrystalDeadLayerLogical,germOptSurface);


   	  //PMT
   	  for (ji=0;ji < num; ji++)
   	    {
   		  // Zero reflectivity and 0.2 efficiency means that the
   		  // photons are all "absorbed and identified as hits" with a QE of 20%
   	      Reflectivity[ji] = 0.0;
   	      Efficiency[ji] = 0.2; // Quantum efficiency of the PMT
   	    }
   	  G4MaterialPropertiesTable *pmtOptTable = new G4MaterialPropertiesTable();
   	  pmtOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
   	  pmtOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);
   	  G4OpticalSurface* pmtOptSurface
   	    = new G4OpticalSurface("PMT surface",glisur,polished,dielectric_metal);
   	  pmtOptSurface->SetMaterialPropertiesTable(pmtOptTable);


   	  //Associates the OpticalSurface to the actual volumes
   	  new G4LogicalSkinSurface("PMT_surf",fPhotocathodeLogical,pmtOptSurface);

    }

	SetDetectorName("GSLArGeOptical");

}

// --------------------------------------------------------------------------- 
void GEGSLArGeOptical::BuildEncapsulation()
{
	MGLog(debugging) << "Starting building encapsulation" << endlog;
	// -------------------------------------------------
	// volume: encaps
	// -------------------------------------------------
	if( fFlagCrystalEncapsulation == true ){
		G4double x = 50.0 * mm;
		G4double z = -371.5 * mm;
		G4Tubs* EncapsInner1_PTFE = new G4Tubs("EncapsInner1_PTFE",
				0.0*mm,
				fCrystalOuterRadius,
				fCrystalHeight/2.,
				0.0*deg,
				360.0*deg);

		G4Tubs* EncapsInner2_PTFE = new G4Tubs("EncapsInner2_PTFE",
				0.0*mm,
				fCrystalOuterRadius+0.5*mm,
				fCrystalHeight/2.+0.5*mm,
				0.0*deg,
				360.0*deg);
		G4SubtractionSolid* EncapsInner_PTFE = new G4SubtractionSolid( "EncapsInner_PTFE",
				EncapsInner2_PTFE,
				EncapsInner1_PTFE );

		G4LogicalVolume* EncapsInner_PTFELogical = new G4LogicalVolume(EncapsInner_PTFE, G4Material::GetMaterial("Teflon"), "EncapsInner_PTFELogical");
		/*G4VPhysicalVolume* EncapsInner_PTFEPhysical =*/ new G4PVPlacement(0,G4ThreeVector(x,0.0,z),EncapsInner_PTFELogical,"EncapsInner_PTFEPhysical",liquidLogical,false,0);

		G4Tubs* Encaps2_Copper = new G4Tubs("Encaps2_Copper",
				0.0*mm,
				fCrystalOuterRadius+2.0*mm,
				fCrystalHeight/2.+2.0*mm,
				0.0*deg,
				360.0*deg);
		G4SubtractionSolid* Encaps_Copper = new G4SubtractionSolid( "Encaps_Copper",
				Encaps2_Copper,
				EncapsInner2_PTFE );

		G4LogicalVolume* Encaps_CopperLogical = new G4LogicalVolume(Encaps_Copper, G4Material::GetMaterial("MetalCopper"), "Encaps_CopperLogical");
		/*G4VPhysicalVolume* Encaps_CopperPhysical =*/ new G4PVPlacement(0,G4ThreeVector(x,0.0,z),Encaps_CopperLogical,"Encaps_CopperPhysical",liquidLogical,false,0);


		G4Tubs* EncapsOuter2_PTFE = new G4Tubs("EncapsOuter2_PTFE",
				0.0*mm,
				fCrystalOuterRadius+3.0*mm,
				fCrystalHeight/2.+3.0*mm,
				0.0*deg,
				360.0*deg);
		G4SubtractionSolid* EncapsOuter_PTFE = new G4SubtractionSolid( "EncapsOuter_PTFE",
				EncapsOuter2_PTFE,
				Encaps2_Copper );

		G4LogicalVolume* EncapsOuter_PTFELogical = new G4LogicalVolume(EncapsOuter_PTFE, G4Material::GetMaterial("Teflon"), "EncapsOuter_PTFELogical");
		/*G4VPhysicalVolume* EncapsOuter_PTFEPhysical =*/ new G4PVPlacement(0,G4ThreeVector(x,0.0,z),EncapsOuter_PTFELogical,"EncapsOuter_PTFEPhysical",liquidLogical,false,0);
	}else{
		MGLog(debugging) << "No encapsulation." << endlog;
	}
}
// --------------------------------------------------------------------------- 

void GEGSLArGeOptical::BuildCrystalsInside()
{
	MGLog(debugging) << "Starting building crystals" << endlog;
	// -------------------------------------------------
	// volume: crystal
	// -------------------------------------------------
	G4Material* enrGe = G4Material::GetMaterial("NaturalGe");

	if (fFlagCrystalTrueCoaxial == true)
	{
		MGLog(debugging) << "Building true coaxial detectors " << endlog;
		MGLog(debugging) << fPhiSegmentsPerCrystal << " segments in phi" << endlog;
		MGLog(debugging) << fZSegmentsPerCrystal << " segments in z" << endlog;
		// dead layer

		G4double TubsInnerRadius   = fCrystalInnerRadius;
		G4double TubsOuterRadius   = fCrystalOuterRadius;
		G4double TubsHeight        = 0.5 * fCrystalHeight;
		G4double TubsStartAngle    = 0.0 * deg;
		G4double TubsSpanningAngle = 360.0 * deg;

		G4Tubs* CrystalDeadLayerTubs = new G4Tubs("CrystalDeadLayerTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		TubsInnerRadius   = fCrystalInnerRadius + fCrystalDeadLayerInnerThickness;
		TubsOuterRadius   = fCrystalOuterRadius - fCrystalDeadLayerOuterThickness;
		TubsHeight        = 0.5 * fCrystalHeight - fCrystalDeadLayerTopThickness;
		TubsStartAngle    = 0.0 * deg;
		TubsSpanningAngle = 360.0 * deg;

		G4Tubs* CrystalDeadLayerHoleTubs = new G4Tubs("CrystalDeadLayerHoleTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		G4SubtractionSolid* CrystalDeadLayerSubtractionSolid =
				new G4SubtractionSolid("CrystalSubtractionSolid",
						CrystalDeadLayerTubs,
						CrystalDeadLayerHoleTubs,
						0,
						G4ThreeVector(0.0, 0.0, 0.0));

		fCrystalDeadLayerLogical = new  G4LogicalVolume(CrystalDeadLayerSubtractionSolid,
				enrGe,
				"CrystalDeadLayerLogical");

		// segments

		G4double dPhi;

		if ( fPhiSegmentsPerCrystal== 1)
			dPhi = 360.0 * deg;
		else
			dPhi =   (360.0 * deg - fPhiSegmentsPerCrystal * fSegmentGapAngle)
			/ fPhiSegmentsPerCrystal;

		TubsInnerRadius   = fCrystalInnerRadius + fCrystalDeadLayerInnerThickness;
		TubsOuterRadius   = fCrystalOuterRadius - fCrystalDeadLayerOuterThickness;
		TubsHeight        = 0.5 * fSegmentHeight;
		TubsStartAngle    = 0.0 * deg;
		TubsSpanningAngle = dPhi;

		G4Tubs* CrystalSegmentTubs = new G4Tubs("CrystalSegmentTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		fCrystalSegmentLogical = new G4LogicalVolume(CrystalSegmentTubs,
				enrGe,
				"CrystalSegmentLogical");
	}
	else
	{
		MGLog(debugging) << "Building non-true coaxial detectors " << endlog;
		// dead layer
		// 1. outer volume of dead layer
		G4double TubsInnerRadius   = 0.0 * mm;
		G4double TubsOuterRadius   = fCrystalOuterRadius;
		G4double TubsHeight        = 0.5 * fCrystalHeight;
		G4double TubsStartAngle    = 0.0 * deg;
		G4double TubsSpanningAngle = 360.0 * deg;

		G4Tubs* CrystalDeadLayerTubs = new G4Tubs("CrystalDeadLayerTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		TubsInnerRadius   = 0.0 * cm;
		TubsOuterRadius   = fCrystalInnerRadius;
		TubsHeight        = 0.5 * fCrystalHoleDepth;

		G4Tubs* CrystalDeadLayerHoleTubs = new G4Tubs("CrystalDeadLayerHoleTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		G4double z = - 0.5 * (fCrystalHeight - fCrystalHoleDepth);

		G4ThreeVector holetranslation;

		holetranslation.setX(0.0 * cm);
		holetranslation.setY(0.0 * cm);
		holetranslation.setZ(z);

		G4SubtractionSolid* CrystalDeadLayerOuterSubtractionSolid =
				new G4SubtractionSolid("DeadLayerOuterSubtractionSolid",
						CrystalDeadLayerTubs,
						CrystalDeadLayerHoleTubs,
						0,
						holetranslation);
		// 2. inner volume
		TubsInnerRadius   = 0.0 * mm;
		TubsOuterRadius   = fCrystalOuterRadius - fCrystalDeadLayerOuterThickness;
		TubsHeight        = 0.5 * ( fCrystalHeight
				- fCrystalDeadLayerOuterThickness
				- fCrystalDeadLayerInnerThickness );
		TubsStartAngle    = 0.0 * deg;
		TubsSpanningAngle = 360.0 * deg;
		G4Tubs* CrystalDeadLayer1Tubs = new G4Tubs("CrystalDeadLayer1Tubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);
		TubsInnerRadius   = 0.0 * cm;
		TubsOuterRadius   = fCrystalInnerRadius + fCrystalDeadLayerInnerThickness;
		TubsHeight        = 0.5 * fCrystalHoleDepth;
		G4Tubs* CrystalDeadLayer1HoleTubs = new G4Tubs("CrystalDeadLayer1HoleTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		z = - 0.5 * ( fCrystalHeight
				- fCrystalDeadLayerOuterThickness
				- fCrystalDeadLayerInnerThickness
				- fCrystalHoleDepth );
		holetranslation.setX(0.0 * cm);
		holetranslation.setY(0.0 * cm);
		holetranslation.setZ(z);
		G4SubtractionSolid* CrystalDeadLayerInnerSubtractionSolid =
				new G4SubtractionSolid("DeadLayerInnerSubtractionSolid",
						CrystalDeadLayer1Tubs,
						CrystalDeadLayer1HoleTubs,
						0,
						holetranslation);

		// 3. subtract the two subtraction solid

		z =  - 0.5 * ( fCrystalDeadLayerOuterThickness - fCrystalDeadLayerInnerThickness );
		holetranslation.setX(0.0 * cm);
		holetranslation.setY(0.0 * cm);
		holetranslation.setZ(z);
		G4SubtractionSolid* CrystalDeadLayerSubtractionSolid =
				new G4SubtractionSolid("DeadLayerSubtractionSolid",
						CrystalDeadLayerOuterSubtractionSolid,
						CrystalDeadLayerInnerSubtractionSolid,
						0,
						holetranslation);

		// 4. finally deadlayer logical
		fCrystalDeadLayerLogical = new  G4LogicalVolume(CrystalDeadLayerSubtractionSolid,
				enrGe,
				"CrystalDeadLayerLogical");

		// segments

		TubsInnerRadius   = 0.0 * mm;
		TubsOuterRadius   = fCrystalOuterRadius - fCrystalDeadLayerOuterThickness;
		TubsHeight        = 0.5 * (fCrystalHeight
				- fCrystalDeadLayerOuterThickness
				- fCrystalDeadLayerInnerThickness );
		TubsStartAngle    = 0.0 * deg;
		TubsSpanningAngle = 360.0 * deg;

		G4Tubs* CrystalSegmentTubs = new G4Tubs("CrystalSegmentTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		TubsInnerRadius   = 0.0 * cm;
		TubsOuterRadius   = fCrystalInnerRadius + fCrystalDeadLayerInnerThickness;
		TubsHeight        = 0.5 * fCrystalHoleDepth;

		G4Tubs* CrystalSegmentHoleTubs = new G4Tubs("CrystalSegmentHoleTubs",
				TubsInnerRadius,
				TubsOuterRadius,
				TubsHeight,
				TubsStartAngle,
				TubsSpanningAngle);

		z = - 0.5 * ( fCrystalHeight
				- fCrystalDeadLayerOuterThickness
				- fCrystalDeadLayerInnerThickness
				- fCrystalHoleDepth );
		holetranslation.setX(0.0 * cm);
		holetranslation.setY(0.0 * cm);
		holetranslation.setZ(z);

		G4SubtractionSolid* CrystalSegmentSubtractionSolid =
				new G4SubtractionSolid("CrystalSegmentSubtractionSolid",
						CrystalSegmentTubs ,
						CrystalSegmentHoleTubs,
						0,
						holetranslation);

		fCrystalSegmentLogical = new  G4LogicalVolume(CrystalSegmentSubtractionSolid,
				enrGe,
				"CrystalSegmentLogical");
	}


	for (G4int col = 0; col < fNumberOfColumns; col++)
	{
		for (G4int row = 0; row < fCrystalsPerColumn; row++)
		{
			fCrystalPosition  = GetCrystalPosition(col,row);
			MGLog(warning) << "Crystal: " << col << " " << row << endlog;
			MGLog(warning) << "Position: " << fCrystalPosition << endlog;
			// dead layer
			char namedeadlayer[50];
			G4int nCrystalNumber = col*fCrystalsPerColumn + row;
			sprintf(namedeadlayer,"CrystalDeadLayer_%d",nCrystalNumber);
			/*G4VPhysicalVolume* fCrystalDeadLayerPhysical =*/ new G4PVPlacement(0,
					fCrystalPosition,
					fCrystalDeadLayerLogical,
					namedeadlayer,
					liquidLogical,
					false,
					nCrystalNumber);
			for (G4int i = 0; i < fPhiSegmentsPerCrystal; i++)
				for (G4int j = 0; j < fZSegmentsPerCrystal; j++)
					ConstructCrystalSegment(i,j);
		}

	}

}

void GEGSLArGeOptical::CrystalInitialization()
{
	fFlagCrystalTrueCoaxial         = false;
	fFlagCrystalEncapsulation       = false;

	// NB: 24/01/2011 - Customize the source to make it more like a BEGe
	fCrystalInnerRadius             =  1.00 * mm; // was 06.0  * mm;
	fCrystalOuterRadius             = 40.5 * mm; // was 42.0  * mm;
	fCrystalHeight                  = 32 * mm; // was 84.0  * mm;
	fCrystalDeadLayerInnerThickness =  0.5 * mm;
	fCrystalDeadLayerOuterThickness =  0.5  * mm;
	fCrystalDeadLayerTopThickness   =  0.5  * mm;
	fCrystalHoleDepth               = 0.5 * mm; // was 46.0  * mm;

	fSegmentMode           = 0;
	if (fFlagCrystalTrueCoaxial)
	{
		fPhiSegmentsPerCrystal = 6;
		fZSegmentsPerCrystal   = 3;
	}
	else
	{
		fPhiSegmentsPerCrystal = 1;
		fZSegmentsPerCrystal   = 1;
	}
	fSegmentStartingAngle  = 0 * deg;
	fSegmentGapAngle       = 0 * deg;
	fSegmentGap            = 0.0 * mm;

	fNumberOfColumns   = 3;
	fColumnDistance    = 2.0 * cm;
	fArrayDepth        = 0.0 * cm;

	fCrystalsPerColumn = 1;
	fCrystalColumnGap  = 5.0 * cm;
	fColumnHeight      =   fCrystalsPerColumn * fCrystalHeight
			+ (fCrystalsPerColumn - 1) * fCrystalColumnGap;
	fSegmentHeight    =   (fCrystalHeight - (fZSegmentsPerCrystal - 1) * fSegmentGap
			- fCrystalDeadLayerOuterThickness
			- fCrystalDeadLayerInnerThickness)
			/ fZSegmentsPerCrystal;
}

G4double GEGSLArGeOptical::GetSegmentAngle(G4int seg, G4int mode)
{

	G4double angle;
	G4double segmentangle =   (360.0 * deg - fPhiSegmentsPerCrystal * fSegmentGapAngle)
                        		  / fPhiSegmentsPerCrystal;

	angle =   fSegmentStartingAngle + 0.5 * fSegmentGapAngle
			+ seg * (segmentangle + fSegmentGapAngle);

	if (mode == 1)
	{
	}
	else if (mode == 2)
	{
		//      if (fCrystalColumn == 6)
		angle = angle + 0.5 * (segmentangle + fSegmentGapAngle);
	}
	else if (mode == 3)
	{
		angle = angle + 0.5 * (segmentangle + fSegmentGapAngle);
		//if (fCrystalColumn == 6)
		angle = angle - 0.5 * (segmentangle + fSegmentGapAngle);
	}
	else if (mode == 4)
	{
		angle = angle + 0.5 * (segmentangle + fSegmentGapAngle);
	}

	return angle;

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGSLArGeOptical::GetSegmentPosition(G4int segrow)
{

	G4ThreeVector position = G4ThreeVector(0.0, 0.0, 0.0);
	G4double      z;
	//G4double      x;

	if (fZSegmentsPerCrystal == 1)
	{
		//z = 0.0 * mm;
		z = -371.5 * mm
				- 0.5 * ( fCrystalDeadLayerOuterThickness
						- fCrystalDeadLayerInnerThickness );
	}
	else
	{
		z =   0.5 * fCrystalHeight
				- fCrystalDeadLayerTopThickness
				- 0.5 * fSegmentHeight
				- segrow * (fSegmentHeight + fSegmentGap);
	}

	position.setZ(z);

	position = position + fCrystalPosition;

	return position;

}

// --------------------------------------------------------------------------- 

void GEGSLArGeOptical::ConstructCrystalSegment(G4int fPhiSegmentNumber,G4int fZSegmentNumber)
{

	char namesegment[30];
	G4int fSegmentNumber = fZSegmentsPerCrystal * fPhiSegmentNumber + fZSegmentNumber;
	sprintf(namesegment,"Segment_%d", fSegmentNumber);

	G4double         angle;
	G4RotationMatrix rm;
	G4ThreeVector    segmentposition;

	angle = GetSegmentAngle(fPhiSegmentNumber, fSegmentMode);

	rm.rotateZ(angle);
	if (fFlagCrystalTrueCoaxial) {
		segmentposition = GetSegmentPosition(fZSegmentNumber);
		new G4PVPlacement(G4Transform3D(rm, segmentposition),
				  fCrystalSegmentLogical,
				  namesegment,
				  liquidLogical,
				  false,
				  fSegmentNumber);
	}
	else {
		G4double x = 50. * mm;
		G4double z = -371.5 * mm;
		new G4PVPlacement(0,
				  G4ThreeVector(x,0.,z-0.5*(fCrystalDeadLayerOuterThickness-fCrystalDeadLayerInnerThickness)),
				  fCrystalSegmentLogical,
				  namesegment,
				  liquidLogical,
				  false,
				  fSegmentNumber);
	}

} 

G4ThreeVector GEGSLArGeOptical::GetCrystalPosition(G4int col, G4int row)
{  
	G4ThreeVector* positionarray = new G4ThreeVector[fNumberOfColumns];
	G4ThreeVector position;

	G4double x, y, z;
	G4double r;
	G4double dphi;
	G4int    ncol = fNumberOfColumns;
	G4int    nrow = fCrystalsPerColumn;
	G4int mode = 0;

	z =  -371.5 * mm
			+ (fCrystalHeight +  fCrystalColumnGap)* 0.5 *(nrow-1)
			+ fArrayDepth
			- row * (fCrystalHeight + fCrystalColumnGap);

	r = fColumnDistance + fCrystalOuterRadius;

	if (ncol == 1)
	{
		x = 5.0 * cm;
		y = 0.0 * cm;

		positionarray[0].setX(x);
		positionarray[0].setY(y);
		positionarray[0].setZ(z);
	}

	else if (ncol == 2)
	{
		for (G4int i = 0; i < ncol  ; i++)
		{
			x = (1 - 2*i) * (0.5 * fColumnDistance + fCrystalOuterRadius);
			y = 0.0;

			positionarray[i].setX(x);
			positionarray[i].setY(y);
			positionarray[i].setZ(z);
		}
	}

	else if (ncol == 3)
	{
		dphi = 2 * pi / 3 * rad;

		for (G4int i = 0; i < ncol; i++)
		{
			x = r * sin(dphi * i);
			y = r * cos(dphi * i);

			positionarray[i].setX(x);
			positionarray[i].setY(y);
			positionarray[i].setZ(z);
		}
	}

	else if (ncol == 4)
	{
		dphi = 2 * pi / 4 * rad;

		for (G4int i = 0; i < ncol; i++)
		{
			x = r * sin(dphi * i);
			y = r * cos(dphi * i);

			positionarray[i].setX(x);
			positionarray[i].setY(y);
			positionarray[i].setZ(z);
		}

	}

	else if (ncol == 5)
	{
		dphi = 2 * pi / 4 * rad;

		for (G4int i = 0; i < fNumberOfColumns-1; i++)
		{
			x = (r + fCrystalOuterRadius) * sin(dphi * i);
			y = (r + fCrystalOuterRadius) * cos(dphi * i);

			positionarray[i].setX(x);
			positionarray[i].setY(y);
			positionarray[i].setZ(z);
		}

		positionarray[4].setX(0.0 * cm);
		positionarray[4].setY(0.0 * cm);
		positionarray[4].setZ(z);
	}

	else if (ncol == 7)
	{

		if (mode == 0)
		{
			dphi = 2 * pi / 6 * rad;

			for (G4int i = 0; i < ncol-1; i++)
			{
				x = (r + fCrystalOuterRadius) * sin(dphi * i);
				y = (r + fCrystalOuterRadius) * cos(dphi * i);

				positionarray[i].setX(x);
				positionarray[i].setY(y);
				positionarray[i].setZ(z);
			}

			positionarray[6].setX(0.0 * cm);
			positionarray[6].setY(0.0 * cm);
			positionarray[6].setZ(z);

		}

	}

	else if (ncol == 9)
	{

		for (G4int i = 0; i < 3; i++)
			for (G4int j = 0; j < 3; j++)
			{
				x = (i - 1) * (2.0 * fCrystalOuterRadius + fColumnDistance);
				y = (j - 1) * (2.0 * fCrystalOuterRadius + fColumnDistance);

				positionarray[i*3 + j].setX(x);
				positionarray[i*3 + j].setY(y);
				positionarray[i*3 + j].setZ(z);
			}

	}

	else if (ncol == 10)
	{
		if (mode == 0)
		{
			G4double dx, dy;

			dphi = 2 * pi / 6 * rad;

			for (G4int i = 0; i < ncol-1; i++)
			{
				x = (r + fCrystalOuterRadius) * sin(dphi * i);
				y = (r + fCrystalOuterRadius) * cos(dphi * i);

				positionarray[i].setX(x);
				positionarray[i].setY(y);
				positionarray[i].setZ(z);
			}

			positionarray[6].setX(0.0 * cm);
			positionarray[6].setY(0.0 * cm);
			positionarray[6].setZ(z);

			dx = fabs(positionarray[1].getX() - positionarray[5].getX());
			dy = fabs(positionarray[4].getY() - positionarray[5].getY());

			positionarray[7].setX(positionarray[6].getX() - dx);
			positionarray[7].setY(positionarray[6].getY());
			positionarray[7].setZ(z);

			positionarray[8].setX(positionarray[1].getX());
			positionarray[8].setY(positionarray[1].getY() + dy);
			positionarray[8].setZ(z);

			positionarray[9].setX(positionarray[2].getX());
			positionarray[9].setY(positionarray[2].getY() - dy);
			positionarray[9].setZ(z);
		}
	}

	else
	{
		positionarray[0].setX(0.0);
		positionarray[0].setY(0.0);
		positionarray[0].setZ(0.0);
	}

	position = positionarray[col];
	MGLog(debugging) << col << " " << row << " " << position/cm << endlog;

	delete [] positionarray;
	return position;

}

void GEGSLArGeOptical::SetFlagCrystalTrueCoaxial(G4bool val)
{
	fFlagCrystalTrueCoaxial = val;
	if (fFlagCrystalTrueCoaxial)
	{
		fPhiSegmentsPerCrystal = 6;
		fZSegmentsPerCrystal   = 3;
	}
	else
	{
		fPhiSegmentsPerCrystal = 1;
		fZSegmentsPerCrystal   = 1;
	}
}


void GEGSLArGeOptical::SetFlagCrystalEncapsulation(G4bool val)
{
	fFlagCrystalEncapsulation = val;
}

//// -- Function to assign optical properties to the LArGe simulation.
//void GEGSLArGeOptical::RegisterOpticalProperties() {
//	// All the information added here and on the functions below was imported from MPIKLArGe class and is now used to simulate a more complete design of the LAr Instrumentation.
//	// First take care of the Argon, to avoid further problems.
//	RegisterArgonOpticalProperties();
//	InitializeTPBSpectrum();
//}


// -- Aux functions taken from MPIKLArGe
void GEGSLArGeOptical::RegisterArgonOpticalProperties()
{
  static const G4int NUMENTRIES = 69;
  const G4int num = 69;
  static const G4double temp = 88.5*kelvin;
  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;

  /**
   * Nominal values for pure argon
   */
  G4double scint_yield = 23.6*eV;  // Nominal energy to produce a photon (measured)
  G4double photon_yield = 1.0*MeV/scint_yield;
  G4double tau_s = 6.0*ns;
  G4double tau_l = 1590.0*ns;
  //G4double yield_ratio = 0.3; // For gammas and electrons


  // New value based on the triplet lifetime from Mark Heisel
  // Redefine the values to res-scale according to Mark's calculation
  photon_yield = 20565;
  tau_s = 5.95*ns;
  tau_l = 500*ns;

  MGLog(routine) << "LAr Optical parameters: " << endlog;
  MGLog(routine) << "     Scintillation yield : " << photon_yield << " ph/MeV" << endlog;
  MGLog(routine) << "     Singlet lifetime : " << tau_s/ns << " ns" << endlog;
  MGLog(routine) << "     Triplet lifetime : " << tau_l/ns << " ns" << endlog;

  G4int ji;
  G4double e;
  G4double ee;

  G4double PPCKOVHighE = LambdaE / (115*nanometer);
  G4double PPCKOVLowE = LambdaE / (650*nanometer);
  G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES-1)));

  // liquid argon (LAr)
  G4double LAr_PPCK[(NUMENTRIES)];
  G4double LAr_RIND[(NUMENTRIES)];
  G4double LAr_RAYL[(NUMENTRIES)];
  G4double LAr_ABSL[(NUMENTRIES)];
  for (ji = 0; ji < NUMENTRIES; ji++)
    {
      e = PPCKOVLowE + ((G4double)ji) * de;
      LAr_PPCK[ji] = e;
      LAr_RIND[ji] = LArRefIndex((LambdaE / e));
      LAr_RAYL[ji] = LArRayLength((LambdaE / e), temp);

      MGLog(routine) << " WL: " << (LambdaE/LAr_PPCK[ji])/nm<< " nm Energy: " << LAr_PPCK[ji]/eV << " eV; Refr: " <<
	  LAr_RIND[ji] << " ; Rayleigh l. " << LAr_RAYL[ji]/m << " m" << endlog;


      if (((LambdaE / e)/nm) < 200.0) {
    	  LAr_ABSL[ji] =60*cm;
      } else {
    	  LAr_ABSL[ji] = 1000*m;
      }
     }

  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(136*nanometer);
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCIN[num];
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
    {
      ee=PPSCLowE+ ((G4double)ji) * dee;
      LAr_SCPP[ji]=ee;
      LAr_SCIN[ji]=ArScintillationSpectrum((LambdaE/ee)/nanometer);
//       MGLog(debugging) << " WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Scint: " << LAr_SCPP[ji]/eV << " eV :: " << LAr_SCIN[ji] << endlog;
/*
      MGLog(routine) << " WL1: " << (LambdaE/ee)/nanometer << " WL: " << (LambdaE/LAr_PPCK[ji])/nm<< " En: " << LAr_PPCK[ji]/eV << " ;n: " <<
	  LAr_RIND[ji] << " ; Rayleigh  " << LAr_RAYL[ji]/m << " m; Scint " << LAr_SCIN[ji] << endlog;
*/
    }

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",        LAr_PPCK, LAr_RIND, NUMENTRIES);
  myMPT1->AddProperty("RAYLEIGH",      LAr_PPCK, LAr_RAYL, NUMENTRIES);
  myMPT1->AddProperty("ABSLENGTH",     LAr_PPCK, LAr_ABSL, NUMENTRIES);

  // Fast and slow components of the scintillation
  // They should both be the same
  if ( (LAr_SCPP[0] >= PPCKOVLowE) &&
       (LAr_SCPP[(sizeof(LAr_SCPP)/sizeof(G4double) - 1)] <= PPCKOVHighE) )
    {
      myMPT1->AddProperty("FASTCOMPONENT",LAr_SCPP,LAr_SCIN,num);
      myMPT1->AddProperty("SLOWCOMPONENT",LAr_SCPP,LAr_SCIN,num);
    }
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",photon_yield);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", tau_s);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",tau_l);
  // This is the value for electrons and gammas
  // For example, for nuclear recoils it should be 0.75
  // nominal value for electrons and gamas: 0.23
  // Value used was provided by Mark
  myMPT1->AddConstProperty("YIELDRATIO",0.3);

  // Scintillation yield
  // WArP data:
  /*
   * ScintillationYield (mean energy to produce a UV photon)
   * dependent on the nature of the impinging particles
   *
   * for flat top response particles the mean energy to produce a photon:  is 19.5 eV
   *  Y =  1/19.5 eV
   *
   * ScintillationYield dependent also on:
   *  - Field configuration
   *  - Quencher impurities
   *
   * @ zero E field:
   *  Y_e = 0.8 Y
   *  Y_alpha = 0.7 Y
   *  Y_recoils = 0.2-0.4
   *
   *  These scales should be added to the physics list
   *
   *
   * G4double scint_yield=1.0/(19.5*eV);
   * myMPT1->AddConstProperty("SCINTILLATIONYIELD",scint_yield);
   */




  /**
   * statistical yield fluctuation can be broadened or narrower
   * (impurities, fano factor):
   *
   * LAr Fano factor = 0.11 ( Doke et al, NIM 134 (1976)353 )
   *
   *
   */

  G4double fano = 0.11;// Doke et al, NIM 134 (1976)353
  myMPT1->AddConstProperty("RESOLUTIONSCALE",fano);

  G4Material::GetMaterial("LiquidArgon")->SetMaterialPropertiesTable(myMPT1);
}

//-------------------------------------------------------------------------><


G4double GEGSLArGeOptical::LArRefIndex(const G4double lambda)
{
  return ( sqrt(LArEpsilon(lambda)) ); // square root of dielectric constant
}

//-------------------------------------------------------------------------><


// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395
// Work from ICARUS simulation

G4double GEGSLArGeOptical::LArEpsilon(const G4double lambda)
{
  G4double epsilon;
  if (lambda < 110*nanometer) return 1.0e4; // lambda MUST be > 110.0 nm
  epsilon = lambda / micrometer; // switch to micrometers
  epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
  epsilon = 1.2055e-2 * ( 0.2075 / (91.012 - epsilon) +
                          0.0415 / (87.892 - epsilon) +
                          4.3330 / (214.02 - epsilon) );
  epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
  G4double LArRho = 1.396*g/cm3;
  G4double GArRho = 1.66e-03*g/cm3;
  epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
  if (epsilon < 0.0 || epsilon > 0.999999) return 4.0e6;
  epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
  return epsilon;
}



//-------------------------------------------------------------------------><
// Calculates the Rayleigh scattering length using equations given in
// G. M. Seidel at al., "Rayleigh scattering in rare-gas liquids",
// arXiv:hep-ex/0111054 v2 22 Apr 2002

G4double GEGSLArGeOptical::LArRayLength(const G4double lambda,const
				   G4double temp)
{
  G4double dyne = 1.0e-5*newton;
  static const G4double LArKT = 2.18e-10 * cm2/dyne; // LAr isothermal compressibility
  static const G4double k = 1.380658e-23 * joule/kelvin; // the Boltzmann constant
  G4double h;
  h = LArEpsilon(lambda);
  if (h < 1.00000001) h = 1.00000001; // just a precaution
  h = (h - 1.0) * (h + 2.0); // the "dielectric constant" dependance
  h *= h; // take the square
  h *= LArKT * temp * k; // compressibility * temp * Boltzmann constant
  h /= lambda * lambda * lambda * lambda; // (lambda)^4
  h *= 9.18704494231105429; // (2 * Pi / 3)^3
  if ( h < (1.0 / (10.0 * km)) ) h = 1.0 / (10.0 * km); // just a precaution
  if ( h > (1.0 / (0.1 * nanometer)) ) h = 1.0 / (0.1 * nanometer); // just a precaution
  return ( 1.0 / h );
}

//-------------------------------------------------------------------------><
//-------------------------------------------------------------------------><

G4double GEGSLArGeOptical::ArScintillationSpectrum(const G4double kk)
{
	// Imported from the munichteststand
	// which was originally imported from WArP
	// The spectrum is in fact slightly more complex with 2 low intensity secondary peaks.
	G4double waveL;
	waveL =exp(-0.5*((kk-128.0)/(2.929))*((kk-128.0)/(2.929)));
	return waveL;
}

//-------------------------------------------------------------------------><
//-------------------------------------------------------------------------><



void GEGSLArGeOptical::InitializeTPBSpectrum()
{
  char* path = getenv("MGGENERATORDATA");
  G4String pathString = ".";
  if (!path)    
    MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;      
  else
    pathString = G4String(path);

  G4String pathFile = pathString + "/VM2000_em_spec.dat";
  fTPBspec = new TGraph(pathFile.data());
  if (fTPBspec->GetN() > 0 ) {
	  successfulInitialization = true;
	  MGLog(routine) << "TPB re-emission spectrum ( " << fTPBspec->GetN()
			  << " points) successfully loaded from file." << endlog;
  } else {
	  successfulInitialization = false;
	  MGLog(warning) << "TPB re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
  }

}

//-------------------------------------------------------------------------><

// Performs a linear interpolation of the spectrum
G4double GEGSLArGeOptical::TPBEmissionSpectrum(G4double energy)
{
  if (successfulInitialization)
    {
      //Here the data are loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;

      G4double eff =fTPBspec->Eval(targetf/nm);
      if (eff < 0.2) eff = 0.2;
      return eff;
    }
  else
    {
      return 0.2;
    }
}
