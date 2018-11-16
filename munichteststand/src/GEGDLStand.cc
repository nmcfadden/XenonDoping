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
*
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 28 Jun 2007
 *
 * REVISION: 3 Jul 2007, added new volumes
 *           6 Jul 2007, corrected holder, added source tube
 *          20 Jul 2007, added dead layers of the crystal 
 *         27 Nov 2008, added outside vessel, groove, remove source tube (Marik)
 *         15 Dec 2008, cut edges of detectors, Dusan & marik
 */
// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
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
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Cons.hh"

#include "geometry/MGGeometryDetector.hh"
#include "gerdageometry/GEGeometrySD.hh"

#include "munichteststand/GEGDLStand.hh"
#include "munichteststand/GEGDLStandMessenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEGDLStand::GEGDLStand() : MGGeometryDetector("")
{
  // default values for Prototype 1
  deadLayerThickness = 0.85*mm; //850 micrometers
  zShiftDetector = 18.1*mm; 
  crystalDiameter = 74.5*mm;
  crystalHeight = 69.*mm;
  holeDiameter = 12.*mm;
  holeHeight = 60.*mm; 
  grooveouterDiameter = 30.5*mm;
  grooveThickness = 3.*mm;
  grooveinnerDiameter = 23.*mm;
  cutparameter = 22.54*mm;
  cutposition = -100.*mm;
  cuttopparameter = 8.56*mm;
  cuttopposition = 100.*mm;
  fTheMessenger =  new GEGDLStandMessenger(this);
}

// --------------------------------------------------------------------------- 

GEGDLStand::~GEGDLStand()
{
  delete fTheMessenger;
}

// --------------------------------------------------------------------------- 

void GEGDLStand::ConstructDetector()
{
  // construct airbox
  G4double AirBoxX = 4.0*m;
  G4double AirBoxY = 4.0*m;
  G4double AirBoxZ = 4.0*m;

  G4Box* airBox = new G4Box("airBox",AirBoxX/2.,AirBoxY/2.,AirBoxZ/2.);
  G4LogicalVolume* airBoxLogical = 
    new G4LogicalVolume(airBox,G4Material::GetMaterial("Vacuum"),
			"airBoxLogical");


  G4double innerradius=0.0*cm;
  G4double angle1 = 0.0*deg;
  G4double angle2 = twopi;
  G4double tolerance = 0*micrometer;

G4RotationMatrix* HalfPi_rot_x = new G4RotationMatrix; 
HalfPi_rot_x ->rotateX(0); 

  //*********************************************//
  // dewar
  //*********************************************//

  //Air around
  G4Tubs* Air_tub = new G4Tubs("Air_tub",275.*mm,500.*mm,
				 .468*m,angle1,angle2);

  G4LogicalVolume* Air_log = new G4LogicalVolume(Air_tub, G4Material::GetMaterial("Air"), "Air_log");
  
new G4PVPlacement(HalfPi_rot_x,G4ThreeVector(0,0,65*mm),Air_log,"Air",airBoxLogical,0,0,0);



  // Outer dewar vessel
  G4Tubs* dewarOuter_tub = new G4Tubs("dewarOuter_tub",272.*mm,275.*mm,
				 .468*m,angle1,angle2);

  G4LogicalVolume* dewarOuter_log = new G4LogicalVolume(dewarOuter_tub, G4Material::GetMaterial("Steel"), "dewarOuter_log");
  
new G4PVPlacement(HalfPi_rot_x,G4ThreeVector(0,0,65*mm),dewarOuter_log,"dewar",airBoxLogical,0,0,0);


  G4double wallThickness = 2.0*mm; //thiis is wall+vacuum!
  G4double heightDewarCylinder = 394.0*mm;
  G4double heightDewarTotal = 915.0*mm;
  G4double radiusDewar = 225.0*mm;
  G4double radiusNeck = 69.5*mm;

  G4Tubs* dewarCore = new G4Tubs("dewarCore_tub",innerradius,radiusDewar,
				 heightDewarCylinder/2.,angle1,angle2);
  //Lower part
  G4double thetaStart = pi-std::atan(radiusDewar/(heightDewarCylinder*0.5));
  G4double deltaTheta = pi - thetaStart;
  G4double sphereRadius = radiusDewar/std::sin(thetaStart);
  G4double phiStart = 0*deg;
  G4double deltaPhi = twopi;
  G4Sphere* dewarSphereDown = new G4Sphere("dewarSphere1_sph",innerradius,
					  sphereRadius,phiStart,deltaPhi,
					  thetaStart,deltaTheta);
  G4UnionSolid* dewar1 = new G4UnionSolid("dewar1",
					  dewarCore,
					  dewarSphereDown,
					  NULL,
					  G4ThreeVector(0,0,-tolerance));
  //Upper part
  G4double theta0 = std::asin(radiusNeck/radiusDewar);
  thetaStart = 0*deg;
  deltaTheta = std::atan(radiusDewar/(heightDewarCylinder*0.5));
  G4Sphere* dewarSphereUp = new G4Sphere("dewarSphere2_sph",innerradius,
					 sphereRadius,phiStart,deltaPhi,
					 thetaStart,deltaTheta);

  G4UnionSolid* dewar2 = new G4UnionSolid("dewar2",
					  dewar1,
					  dewarSphereUp,
					  NULL,
					  G4ThreeVector(0,0,tolerance));

  //Neck
  G4double dewarNeckHeight = heightDewarTotal-heightDewarCylinder-
    2.0*(sphereRadius-0.5*heightDewarCylinder)+sphereRadius*std::sin(theta0)*std::tan(theta0);
  G4Tubs* dewarNeck_tub = new G4Tubs("dewarNeck_tub",innerradius,radiusNeck,
				 dewarNeckHeight/2.,angle1,angle2);
  G4double zPosNeck = heightDewarCylinder/2.+(sphereRadius-0.5*heightDewarCylinder)-
    sphereRadius*std::sin(theta0)*std::tan(theta0)+dewarNeckHeight/2.;

  G4UnionSolid* dewar3 = new G4UnionSolid("dewar3",
					  dewar2,
					  dewarNeck_tub,
					  NULL,
					  G4ThreeVector(0,0,zPosNeck+tolerance));

  G4LogicalVolume* dewar_log = new G4LogicalVolume(dewar3,
						   G4Material::GetMaterial("Steel"),
						   "dewar_log");
  new G4PVPlacement(HalfPi_rot_x,G4ThreeVector(0,0,0),dewar_log,"dewar",
		    airBoxLogical,0,0,0);

  //*********************************************//
  // liquid argon
  //*********************************************//
  G4double radiusLAr = radiusDewar-wallThickness;
  G4double heightLArCylinder = heightDewarCylinder;
  G4Tubs* larCore = new G4Tubs("larCore_tub",innerradius,radiusLAr,
				 heightLArCylinder/2.,angle1,angle2);
  
  //Lower part
  thetaStart = pi-std::atan(radiusLAr/(heightLArCylinder*0.5));
  deltaTheta = pi - thetaStart;
  G4double sphereLArRadius = radiusLAr/std::sin(thetaStart);
  phiStart = 0*deg;
  deltaPhi = twopi;
  G4Sphere* larSphereDown = new G4Sphere("larSphere1_sph",innerradius,
					  sphereLArRadius,phiStart,deltaPhi,
					  thetaStart,deltaTheta);
  G4UnionSolid* lar1 = new G4UnionSolid("lar1",
					larCore,
					larSphereDown,
					NULL,
					G4ThreeVector(0,0,-tolerance));

  //Upper part
  G4double radiusNeckLAr = radiusNeck-wallThickness;
  G4double theta0lar = std::asin(radiusNeckLAr/radiusLAr);
  thetaStart = 0*deg;
  deltaTheta = std::atan(radiusLAr/(heightDewarCylinder*0.5));
  G4Sphere* larSphereUp = new G4Sphere("larSphere2_sph",innerradius,
					 sphereLArRadius,phiStart,deltaPhi,
					 thetaStart,deltaTheta);
  G4UnionSolid* lar2 = new G4UnionSolid("lar2",
					lar1,
					larSphereUp,
					NULL,
					G4ThreeVector(0,0,tolerance));

  //Neck
  G4double fillingLevelNeck = dewarNeckHeight/2.;
  G4Tubs* dewarNeckLAr_tub = new G4Tubs("dewarNeck_tub",innerradius,radiusNeckLAr,
				 fillingLevelNeck/2.,angle1,angle2);
  zPosNeck = heightDewarCylinder/2.+(sphereLArRadius-0.5*heightDewarCylinder)-
    sphereLArRadius*std::sin(theta0lar)*std::tan(theta0lar)+fillingLevelNeck/2.;
  G4UnionSolid* lar3 = new G4UnionSolid("lar3",
					lar2,
					dewarNeckLAr_tub,
					NULL,
					G4ThreeVector(0,0,zPosNeck+tolerance));
  G4LogicalVolume* lar_log = new G4LogicalVolume(lar3,
						 G4Material::GetMaterial("LiquidArgon"),
						 "lar_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),lar_log,"liquidArgon",
		    dewar_log,0,0,0);
  
  //**********************************************
  //gaseous argon on the top of the neck
  //**********************************************
  G4Tubs* dewarNeckGAr_tub = new G4Tubs("dewarNeckLar_tub",innerradius,radiusNeckLAr,
					(dewarNeckHeight-fillingLevelNeck-2.0*tolerance)/2.,angle1,
					angle2);
  G4LogicalVolume* gar_log = new G4LogicalVolume(dewarNeckGAr_tub,
						 G4Material::GetMaterial("GaseousArgon"),
						 "gar_log");
  G4double zPosNeck_gas = zPosNeck+dewarNeckHeight-fillingLevelNeck;
  new G4PVPlacement(0,G4ThreeVector(0,0,zPosNeck_gas),gar_log,"gaseousArgon",
		    dewar_log,0,0,0);
  //**********************************************
  //IR shielding (outer)
  //**********************************************
  G4double IRshieldingHeight = 323*mm;
  G4double IRshieldingDiameter = 128*mm;
  G4double IRshieldingThickness = 2.0*mm;
  G4Tubs* ir_tubs = new G4Tubs("IRshield_tub",innerradius,IRshieldingDiameter/2.,
			       IRshieldingHeight/2.,angle1,angle2);
  G4LogicalVolume* ir_log = new G4LogicalVolume(ir_tubs,
						G4Material::GetMaterial("MetalCopper"),
						"ir_log");
  G4double zPosIR = -heightDewarCylinder/2. + 108.3*mm;
  new G4PVPlacement(0,G4ThreeVector(0,0,zPosIR),ir_log,"IRshield",
		    lar_log,0,0,0);

  //**********************************************
  //Argon inside IR shielding 
  //**********************************************
  G4Tubs* innerlar_tubs = new G4Tubs("innerlar_tub",innerradius,
				     IRshieldingDiameter/2.-IRshieldingThickness,
				     IRshieldingHeight/2.-IRshieldingThickness,
				     angle1,angle2);
  G4LogicalVolume* innerlar_log = new G4LogicalVolume(innerlar_tubs,
						      G4Material::GetMaterial("LiquidArgon"),
						      "innerlar_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),innerlar_log,"InnerLAr",
		    ir_log,0,0,0);
  
  //**********************************************
  //The crystal (including dead layers and active volume)
  //**********************************************
  // G4double crystalDiameter = 79*mm;
  // G4double crystalHeight = 81*mm;
  // G4double holeDiameter = 13*mm;
  // G4double holeHeight = 71*mm;
  
  G4Cons* corner_cons = new G4Cons("corner_cons", cutparameter, 5.5*cm, 5.*cm, 5.5*cm, 2.*cm, 0, 2*M_PI);
  G4Cons* cornertop_cons = new G4Cons("cornertop_cons", 5*cm, 5.5*cm, cuttopparameter, 5.5*cm, 2.*cm, 0, 2*M_PI);

  /****** RG3 modification ********************

  G4Tubs* RG3part_sol = new G4Tubs("RG3part",37.45*mm,
  			crystalDiameter/2.,
    			20*mm,
  			angle1,angle2);
  */
  G4Tubs* crystal0_sol = new G4Tubs("crystal0",innerradius,
				crystalDiameter/2.,
				crystalHeight/2.,
				angle1,angle2);

  //  G4SubtractionSolid* crystal_sol = new G4SubtractionSolid("crystal_sol", crystal0_sol, RG3part_sol, 0, G4ThreeVector(0., 0., cutposition));

  G4SubtractionSolid* crystal1_sol = new G4SubtractionSolid("crystal1_sol", crystal0_sol, cornertop_cons, 0, G4ThreeVector(0., 0., cuttopposition));

  G4SubtractionSolid* crystal_sol = new G4SubtractionSolid("crystal_sol", crystal1_sol, corner_cons, 0, G4ThreeVector(0., 0., cutposition));

  G4LogicalVolume* crystal_log = new G4LogicalVolume(crystal_sol,
						     G4Material::GetMaterial("Germanium/Lithium"),
						     "crystal_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,zShiftDetector),crystal_log,"Crystal",
		    innerlar_log,0,0,0);

  G4Tubs* active0_sol = new G4Tubs("active0",innerradius,
				crystalDiameter/2.-deadLayerThickness,
				crystalHeight/2.-deadLayerThickness,
				angle1,angle2);

  /****** RG3 modification ********************

   G4Tubs* RG3partdead_sol = new G4Tubs("RG3partdead",37.45*mm-deadLayerThickness,
  			crystalDiameter/2.,
  			20*mm,
  			angle1,angle2);

   G4SubtractionSolid* active1_sol = new G4SubtractionSolid("active1_sol", active0_sol, RG3partdead_sol, 0, G4ThreeVector(0., 0., cutposition));

  */

  G4SubtractionSolid* active1_sol = new G4SubtractionSolid("active1_sol", active0_sol, corner_cons, 0, G4ThreeVector(0., 0., cutposition+1.4142*deadLayerThickness));

  G4SubtractionSolid* active2_sol = new G4SubtractionSolid("active2_sol", active1_sol, cornertop_cons, 0, G4ThreeVector(0., 0., cuttopposition-1.4142*deadLayerThickness));

  G4Tubs* nondead_sol = new G4Tubs("nondead",holeDiameter/2.,
				grooveinnerDiameter/2.,
				deadLayerThickness/2.,
				angle1,angle2);
  G4UnionSolid* active_sol = new G4UnionSolid("active_sol",
					active2_sol,
					nondead_sol,
					NULL,
					G4ThreeVector(0,0,-crystalHeight/2.+deadLayerThickness/2.));
  G4LogicalVolume* active_log = new G4LogicalVolume(active_sol,
						     G4Material::GetMaterial("NaturalGe"),
						     "active_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),active_log,"Active",
		    crystal_log,0,0,0);


 //**********************************************
  //The crystal groove
  //**********************************************

 G4Tubs* deadgroove_sol =  new G4Tubs("deadgroove_sol",grooveinnerDiameter/2.,
				grooveouterDiameter/2.,
				deadLayerThickness/2.,
				angle1,angle2);

 G4LogicalVolume* deadgroove_log = new G4LogicalVolume(deadgroove_sol,
							  G4Material::GetMaterial("LiquidArgon"),
							  "deadgroove_log");
  G4double zPosdeadgroove = -crystalHeight/2.+ deadLayerThickness/2.;
  new G4PVPlacement(0,G4ThreeVector(0,0,zPosdeadgroove),deadgroove_log,"deadgroove",
		    crystal_log,0,0,0);


 G4Tubs* groove_sol =  new G4Tubs("groove_sol",grooveinnerDiameter/2.,
				grooveouterDiameter/2.,
				grooveThickness/2.-deadLayerThickness/2.,
				angle1,angle2);

 G4LogicalVolume* groove_log = new G4LogicalVolume(groove_sol,
							  G4Material::GetMaterial("LiquidArgon"),
							  "groove_log");
  G4double zPosgroove = -crystalHeight/2.+ grooveThickness/2.+deadLayerThickness/2.;
  new G4PVPlacement(0,G4ThreeVector(0,0,zPosgroove),groove_log,"groove",
		    active_log,0,0,0);



 //**********************************************
  //For ANG 1 smaller height between groove and hole
  //**********************************************

  /*  G4Tubs* aroundHole_sol = new G4Tubs("aroundhole",holeDiameter/2.,
				      grooveinnerDiameter/2.,
				   1.5*mm,
				   angle1,angle2);
  G4LogicalVolume* aroundHole_log = new G4LogicalVolume(aroundHole_sol,
							 G4Material::GetMaterial("LiquidArgon"),
							 "aroundHole_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,-32.5*mm),aroundHole_log,"AroundHole",
		    crystal_log,0,0,0);
  */

  G4Tubs* crystalHole_sol = new G4Tubs("holecrystal",innerradius,
				   holeDiameter/2.,
				   deadLayerThickness/2.,
				   angle1,angle2);
  G4LogicalVolume* crystalHole_log = new G4LogicalVolume(crystalHole_sol,
							 G4Material::GetMaterial("LiquidArgon"),
							 "crystalHole_log");
  G4double zShiftCrystalHole = -crystalHeight/2.+deadLayerThickness/2.;
  new G4PVPlacement(0,G4ThreeVector(0,0,zShiftCrystalHole),crystalHole_log,"CrystalHole",
		    crystal_log,0,0,0);


  G4Tubs* activeHole_sol = new G4Tubs("holeactive",innerradius,
				   holeDiameter/2.,
				   holeHeight/2.-deadLayerThickness/2,
				   angle1,angle2);
  G4LogicalVolume* activeHole_log = new G4LogicalVolume(activeHole_sol,
							 G4Material::GetMaterial("LiquidArgon"),
							 "activeHole_log");
  G4double zShiftActiveHole = -crystalHeight/2.+holeHeight/2.+deadLayerThickness/2.;
  new G4PVPlacement(0,G4ThreeVector(0,0,zShiftActiveHole),activeHole_log,"ActiveHole",
		    active_log,0,0,0);




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
  G4Colour  dead    (187/255. ,28/255. ,0/255. );
  G4Colour  gecolour (133/255. ,133/255. ,133/255. );
  G4Colour  cucolour (255/255. ,71/255. ,33/255. );
  G4Colour  fecolour (175/255. ,157/255. ,189/255.);
  G4Colour  skyblue  (175/255. ,164/255. ,255/255. );
  G4Colour  garcolour (150/255. ,150/255. ,255/255. );
  G4VisAttributes* larAttributes = new G4VisAttributes(blue);
  /*G4VisAttributes* copperAttributes =*/ new G4VisAttributes(magenta);
  /*G4VisAttributes* deadlayerAttributes =*/ new G4VisAttributes(lgrey);
  airBoxLogical->SetVisAttributes(G4VisAttributes::Invisible);
  Air_log->SetVisAttributes(new G4VisAttributes(skyblue));
  dewarOuter_log->SetVisAttributes(new G4VisAttributes(fecolour));
  dewar_log->SetVisAttributes(new G4VisAttributes(fecolour));
  lar_log->SetVisAttributes(larAttributes);
  gar_log->SetVisAttributes(new G4VisAttributes(garcolour));
  ir_log->SetVisAttributes(new G4VisAttributes(cucolour));
  innerlar_log->SetVisAttributes(larAttributes);
  crystalHole_log->SetVisAttributes(larAttributes);
  activeHole_log->SetVisAttributes(larAttributes);
  groove_log->SetVisAttributes(larAttributes);
  deadgroove_log->SetVisAttributes(larAttributes);
  crystal_log->SetVisAttributes(new G4VisAttributes(dead));
  active_log->SetVisAttributes(new G4VisAttributes(gecolour));

  //Sensitive detectors
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  GEGeometrySD* CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
  //GEGeometrySD* CryoSD = new GEGeometrySD("/mydet/gerda/N2buffer");

  SDman -> AddNewDetector(CrystalSD);
  //SDman -> AddNewDetector(CryoSD);

  active_log -> SetSensitiveDetector(CrystalSD);
  //argonLogical->SetSensitiveDetector(CryoSD);

 
  SetDetectorLogical(airBoxLogical);

  SetDetectorName("GDLStand"); 

}

// --------------------------------------------------------------------------- 
 


