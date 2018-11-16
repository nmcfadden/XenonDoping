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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 05-13-2005
 *
 * REVISION:
 * 3 Jan 2006, Luciano, Fixed according to overcome the changes in the gerdageometry/ 
 *                      design. Not very elegant, but working
 *
 * 10 July 2007, Markus, included plastic pannels and photomultiplier cathodes (approximately
 *                       according to the scheme I got)
 *
 * 17 June 2011, Giovanni, added encapsulation
 * 
 * 20 June 2011, Luciano, Changed Dewar material from SS to Cu
 *                        Updated dimensions
 *
 * 21 June 2011, Giovanni, Updated dimensions
 *
 * 04 Juli 2011, Giovanni, Updated Crystal dimensions and position
 *
 * 02 August 2011, Giovanni, Updated Crystal vertical position
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

#include "geometry/MGGeometryDetector.hh"
//#include "geometry/MGGeometryDetectorConstruction.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryPMTSD.hh"
//#include "gerdageometry/GEGeometryDetectorArray.hh"
//#include "gerdageometry/GEGeometryDetectorDB.hh"

#include "munichteststand/GEGSLArGe.hh"
#include "munichteststand/GEGSLArGeMessenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEGSLArGe::GEGSLArGe() : 
  MGGeometryDetector(""),liquidLogical(0),fCrystalSegmentLogical(0),fCrystalDeadLayerLogical(0)
{
  fBuildCrystals = true;
  CrystalInitialization();
  fMessenger = new GEGSLArGeMessenger(this);
}

// --------------------------------------------------------------------------- 

GEGSLArGe::~GEGSLArGe()
{
  delete fMessenger;
}

// --------------------------------------------------------------------------- 

void GEGSLArGe::ConstructDetector()
{
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
  G4double dewarRadius = liquidRadius + dewarThickness;
  G4double dewarHeight = liquidHeight + dewarThickness; 

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


  // Cryoliquid 
  G4Tubs* liquidTub =  new G4Tubs("liquid_tub",0.,liquidRadius,liquidHeight,
				  0,twopi);
  liquidLogical = 
    new G4LogicalVolume(liquidTub,G4Material::GetMaterial("LiquidArgon"),
			"liquidLogical");
   //G4VPhysicalVolume* liquidPhysical = 
     new G4PVPlacement(0,G4ThreeVector(0,0,0),liquidLogical,"Argon",
 		      dewarLogical,false,0);


   // small plate seperating the PMTs-electronics from the cryoliquid

   G4double copperholeRadius = 14.0*cm;
   G4double copperplateHeight = 1*cm;
   G4double CopperPlate_zpos = liquidHeight-liquidHeight/4.;

   G4Tubs* CopperPlate = new G4Tubs("copper_plate", copperholeRadius, liquidRadius, copperplateHeight, 0, twopi);
   G4LogicalVolume* CopperPlateLogical = new G4LogicalVolume(CopperPlate, G4Material::GetMaterial("MetalCopper"), "CopperPlateLogical");
   new G4PVPlacement(0,G4ThreeVector(0, 0, CopperPlate_zpos), 
		     CopperPlateLogical, "copper_plate", liquidLogical, 
		     false,0);


   // Photomultiplier Cathodes

   char namephotocathode[50];

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
							       "fPhotocathodeLogical");; 

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

  
    new G4PVPlacement(0,G4ThreeVector(50.2*cm, -25.2*cm, 2*m), TapePlateLogical1, "TapePlate1", airBoxLogical, false,0);
    new G4PVPlacement(0,G4ThreeVector(50.2*cm, 25.2*cm, 2*m), TapePlateLogical2, "TapePlate2", airBoxLogical, false,0);
    new G4PVPlacement(0,G4ThreeVector(50.2*cm, 75.6*cm, 2*m), TapePlateLogical3, "TapePlate3", airBoxLogical, false, 0);	
     new G4PVPlacement(0,G4ThreeVector(50.2*cm, -75.6*cm, 2*m), TapePlateLogical4, "TapePlate4", airBoxLogical, false, 0);
    new G4PVPlacement(0,G4ThreeVector(50.2*cm, -126*cm, 2*m), TapePlateLogical5, "TapePlate5", airBoxLogical, false, 0);	
     new G4PVPlacement(0,G4ThreeVector(50.2*cm, 126*cm, 2*m), TapePlateLogical6, "TapePlate6", airBoxLogical, false, 0);   

     new G4PVPlacement(0,G4ThreeVector(25.2*cm, 0*cm, 2.034*m), TapePlateLogical7, "TapePlate7", airBoxLogical, false,0);
     new G4PVPlacement(0,G4ThreeVector(25.2*cm, 50.4*cm, 2.034*m), TapePlateLogical8, "TapePlate8", airBoxLogical, false, 0);	
    new G4PVPlacement(0,G4ThreeVector(25.2*cm, -50.4*cm, 2.034*m), TapePlateLogical9, "TapePlate9", airBoxLogical, false, 0);
     new G4PVPlacement(0,G4ThreeVector(25.2*cm, -100.8*cm, 2.034*m), TapePlateLogical10, "TapePlate10", airBoxLogical, false, 0);	
     new G4PVPlacement(0,G4ThreeVector(25.2*cm, 100.8*cm, 2.034*m), TapePlateLogical11, "TapePlate11", airBoxLogical, false, 0);  

    new G4PVPlacement(G4Transform3D(rm_rot, G4ThreeVector(-75.4*cm ,0, 2*m)),TapePlateLogical12,"TapePlate12",airBoxLogical,false,0);
    new G4PVPlacement(G4Transform3D(rm_rot, G4ThreeVector(-125.8*cm ,0, 2*m)),TapePlateLogical13,"TapePlate13",airBoxLogical,false,0);
    new G4PVPlacement(G4Transform3D(rm_rot, G4ThreeVector(-100.4*cm ,0, 2.034*m)),TapePlateLogical14,"TapePlate14",airBoxLogical,false,0);

   // Plastic scintillator sheets for muon veto
  G4double scint_length = tape_length-0.4*cm;
  G4double scint_width  = tape_width-0.4*cm;
  G4double scint_height = tape_height-0.4*cm;

  G4Box* ScintPlate_hor = new G4Box("ScintPlate_hor",scint_length/2., scint_width/2., scint_height/2.);
  G4LogicalVolume* ScintPlate_horLogical = new G4LogicalVolume(ScintPlate_hor, G4Material::GetMaterial("PlasticScintillator"), "ScintPlate_horLogical");
  
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical1,false,0);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical2,false,1);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical3,false,2);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical4,false,3);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical5,false,4);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical6,false,5);

  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical7,false,6);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical8,false,7);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical9,false,8);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical10,false,9);
  new G4PVPlacement(0,G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical11,false,10);
  
  new G4PVPlacement(0, G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical12,false,11);
  new G4PVPlacement(0, G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical13,false,12);
  new G4PVPlacement(0, G4ThreeVector(0,0,0),ScintPlate_horLogical,"ScintPlate_hor",TapePlateLogical14,false,13);

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

  SDman -> AddNewDetector(CrystalSD);
  SDman -> AddNewDetector(CryoSD);
  SDman -> AddNewDetector(ScintSD);

  if (fCrystalSegmentLogical)
    {
      fCrystalSegmentLogical -> SetSensitiveDetector(CrystalSD);
    }
  //liquidLogical->SetSensitiveDetector(CryoSD);
  liquidLogical->SetSensitiveDetector(CrystalSD);
  ScintPlate_horLogical->SetSensitiveDetector(ScintSD);

  GEGeometryPMTSD* PMTSD = new GEGeometryPMTSD("Phcathode",Num_PMTs,"PhCathodeHC" );    
  SDman->AddNewDetector(PMTSD); 
  fPhotocathodeLogical->SetSensitiveDetector(PMTSD);
  
    
  SetDetectorLogical(airBoxLogical);

  SetDetectorName("GSLArGe"); 





}

// --------------------------------------------------------------------------- 
void GEGSLArGe::BuildEncapsulation()
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

void GEGSLArGe::BuildCrystalsInside()
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
	  //MGLog(debugging) << "Crystal: " << col << " " << row << endlog;
	  //MGLog(debugging) << "Position: " << fCrystalPosition << endlog;
	  // dead layer
	  char namedeadlayer[50];
	  G4int nCrystalNumber = col*fCrystalsPerColumn + row;
	  sprintf(namedeadlayer,"CrystalDeadLayer_%d",nCrystalNumber);
 	  /*G4VPhysicalVolume* fCrystalDeadLayerPhysical = */new G4PVPlacement(0,
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

//   theDatabase->SetCrystalDeadLayerLogical(fCrystalDeadLayerLogical);

}

void GEGSLArGe::CrystalInitialization()
{
  fFlagCrystalTrueCoaxial         = false; 
  fFlagCrystalEncapsulation       = false;
  fCrystalInnerRadius             =  6.0  * mm; 
  fCrystalOuterRadius             = 42.0  * mm; 
  fCrystalHeight                  = 84.0  * mm; 
  fCrystalDeadLayerInnerThickness =  0.01 * mm; 
  fCrystalDeadLayerOuterThickness =  2.0  * mm; 
  fCrystalDeadLayerTopThickness   =  2.0  * mm; 
  fCrystalHoleDepth               = 46.0  * mm;
  
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

G4double GEGSLArGe::GetSegmentAngle(G4int seg, G4int mode) 
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

G4ThreeVector GEGSLArGe::GetSegmentPosition(G4int segrow)
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

void GEGSLArGe::ConstructCrystalSegment(G4int fPhiSegmentNumber,G4int fZSegmentNumber)
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

G4ThreeVector GEGSLArGe::GetCrystalPosition(G4int col, G4int row) 
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
  //MGLog(debugging) << fColumnHeight/cm << " " << fCrystalHeight << " " << 
  //  fArrayDepth/cm << " " << z/cm <<endlog;

  r = fColumnDistance + fCrystalOuterRadius; 

  //MGLog(debugging) << ncol << endlog;

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

void GEGSLArGe::SetFlagCrystalTrueCoaxial(G4bool val)
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


void GEGSLArGe::SetFlagCrystalEncapsulation(G4bool val)
{
  fFlagCrystalEncapsulation = val;
}
