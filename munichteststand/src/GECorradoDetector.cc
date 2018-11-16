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
//                                                          
// $Id: GECorradoDetector.cc,v 1.5 2009-05-18 16:08:53 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GECorradoDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Luciano Pandola
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 03-06-2007, Added in MaGe, Luciano
 * 03-07-2007, Changed to avoid problem with materials defined on-the-fly, Luciano
 * 06-01-2007, Implemented correct geometry of Corrado (originally by Werner), Dusan
 * 14-05-2009, Added more geometry options, added hole-size parameters, Dusan and Marc
 */
//---------------------------------------------------------------------------//
//

//#include "globals.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4EllipticalCone.hh" 
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"

#include "gerdageometry/GEGeometrySD.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "munichteststand/GECorradoDetector.hh"
#include "munichteststand/GECorradoDetectorMessenger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GECorradoDetector::GECorradoDetector() : MGGeometryDetector("")
{
  //set defaults
  sample_geom="none";
  //box height is also liquid level in the standard box
  box_height=1.1*cm;
  //box width is also diference between standard box and sample positions
  box_width=0.*cm;
  box_thickness=0.1*cm;
  // default: standard dose
  tube_innrad=0*cm;
  tube_outrad=3.65*cm;
  tube_length=2.8*cm;
  tube_angle=twopi;
  sample_mat_name="NitrogenGas";
  s_x=0.*cm;
  s_y=0.*cm;
  s_z=0.*cm;
  //dead layer: fitted values for 3mm Al window - final
  d_front=0.1683617694*cm;
  d_side=0.1581820697*cm;
  coll_x=0;
  coll_y=0;
  coll_z=7.62*cm;
  //hole: fitted values - final
  hole_radius=0.82414*cm;
  hole_length=4.12414*cm;
  theMessenger = new GECorradoDetectorMessenger(this);
}

//---------------------------------------------------------------------------//

GECorradoDetector::~GECorradoDetector()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void GECorradoDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  G4Material* Brass = G4Material::GetMaterial("Brass");
  G4Material* Bronze = G4Material::GetMaterial("Bronze");
  G4Material* ProportionalGas = G4Material::GetMaterial("ProportionalGas");
  G4Material* N2Gas = G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  G4Material* GeB = G4Material::GetMaterial("Germanium/Boron");
  G4Material* Pb = G4Material::GetMaterial("MetalLead");
  G4Material* Fe = G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  G4Material* Vespel = G4Material::GetMaterial("Vespel");
  G4Material* Teflon = G4Material::GetMaterial("Teflon");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  G4Material* PE = G4Material::GetMaterial("PE");
  //G4Material* PVC = G4Material::GetMaterial("PVC");
  


 // Visualization attributes
  G4VisAttributes * Brasscolour= new G4VisAttributes( G4Colour(226/255. ,163/255. ,29/255. ));  //orange-brown

  G4VisAttributes * Sample_Colour = new G4VisAttributes(  G4Colour(255/255. ,0/255. ,12/255. ));  //red
  Sample_Colour -> SetForceSolid(true);

  G4VisAttributes * Vacuumcolour = new G4VisAttributes( G4Colour(175/255. ,164/255. ,255/255. ));  //sky-blue

  G4VisAttributes * ProportionalGascolour= new G4VisAttributes( G4Colour(154/255. ,237/255. ,193/255. ));  //blue-green

  G4VisAttributes * Pbcolour = new G4VisAttributes( G4Colour(171/255. ,171/255. ,195/255. ));  //gray

  G4VisAttributes * Cucolour= new G4VisAttributes( G4Colour(255/255. ,71/255. ,33/255. ));  //red

  G4VisAttributes * Gecolour= new G4VisAttributes( G4Colour(133/255. ,133/255. ,133/255. ));  //dark grey
  //Gecolour -> SetForceSolid(true);

  G4VisAttributes * GeLicolour= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));  //almost black (->'dead'...)

  G4VisAttributes * Fecolour = new G4VisAttributes( G4Colour(175/255. ,157/255. ,189/255. ));  //redish-blue gray

  G4VisAttributes * N2Gascolour = new G4VisAttributes( G4Colour(150/255. ,150/255. ,255/255. ));   //light blue

  G4VisAttributes * Alcolour= new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));   //light gray

  G4VisAttributes * Vespelcolour= new G4VisAttributes( G4Colour(224/255. ,224/255. ,190/255. ));  //yellow-gray

  G4VisAttributes * Collimatorcolour= new G4VisAttributes( G4Colour(0/255. ,0/255. ,190/255. ));  //indigo
  Collimatorcolour -> SetForceSolid(true);

  G4VisAttributes * Bronzecolour = new G4VisAttributes(G4Colour(149/255., 175/255., 69/255.));   //dark yellowish-green
 
  G4VisAttributes * Tefloncolour= new G4VisAttributes(G4Colour(119/255.,101/255., 0.));  //brown

  G4VisAttributes* GeBcolour= new G4VisAttributes (G4Colour(140/255., 113/255., 139/255.));  //another kind of grey...


  G4VisAttributes * Acryliccolour= new G4VisAttributes( G4Colour(231/255. ,217/255. ,240/255. ));  //light blue-grey

        
 
  //------------------------------------------------------ volumes
  //-------------------------------1.  Experimental hall (world volume)
  G4double expHall_x = 2.0*m;
  G4double expHall_y = 2.0*m;
  G4double expHall_z = 2.0*m;
  G4Box* experimentalHall_box = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  
G4LogicalVolume* experimentalHall_log = new G4LogicalVolume(experimentalHall_box, N2Gas, "expHall_log", 0, 0, 0);
experimentalHall_log -> SetVisAttributes(G4VisAttributes::Invisible);

 


//-----------------2. MUON-VETOS and STEEL-STRUCTURE ON THE BOTTOM OF THE SHIELDING

// STATUS:         READY FOR FIRST TESTING




//-----------------Muon-Vetos

//---------------- Frontside

G4Box* MuonVetoFrontSide_box = new G4Box("MuonVetoFrontSide_box", 42.65*cm, 35.00*cm, 2.5*cm);

G4LogicalVolume* MuonVetoFrontSide_log = new G4LogicalVolume(MuonVetoFrontSide_box, Brass, "MuonVetoFrontSide_log", 0, 0, 0);
MuonVetoFrontSide_log -> SetVisAttributes(Brasscolour);

  new G4PVPlacement(0, G4ThreeVector(0., 10.00*cm, 46.0*cm-5.6*cm), 
		    MuonVetoFrontSide_log, "MuonVetoFrontSide", 
		    experimentalHall_log, false, 0);
//moved whole setup 5.6cm to place the front cap of detector at the coordinate origin

//---------------- Proportional Gas P10 inside the Frontside  

G4Box* P10FrontSide_box = new G4Box("P10FrontSide_box", 41.15*cm, 33.5*cm, 2.0*cm);

G4LogicalVolume* P10FrontSide_log = new G4LogicalVolume(P10FrontSide_box, ProportionalGas, "P10FrontSide_log", 0, 0, 0);
P10FrontSide_log -> SetVisAttributes(ProportionalGascolour);

 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		  P10FrontSide_log, "P10FrontSide", 
		   MuonVetoFrontSide_log, false, 0);


//----------------- Topside

G4Box* MuonVetoTopSide_box = new G4Box("MuonVetoTopSide_box", 42.5*cm, 2.5*cm, 57.0*cm);

G4LogicalVolume* MuonVetoTopSide_log = new G4LogicalVolume(MuonVetoTopSide_box, Brass, "MuonVetoTopSide_log", 0, 0, 0);
MuonVetoTopSide_log -> SetVisAttributes(Brasscolour);

 new G4PVPlacement(0, G4ThreeVector(0., 38.0*cm, -14.5*cm-5.6*cm), 
		   MuonVetoTopSide_log, "MuonVetoTopSide", 
		   experimentalHall_log, false, 0);
//moved whole setup 5.6cm to place the front cap of detector at the coordinate origin

//----------------- Proportional Gas P10 inside the Topside  

G4Box* P10TopSide_box = new G4Box("P10TopSide_box", 41.0*cm, 2.0*cm, 55.5*cm);

G4LogicalVolume* P10TopSide_log = new G4LogicalVolume(P10TopSide_box, ProportionalGas, "P10TopSide_log", 0, 0, 0);
P10TopSide_log -> SetVisAttributes(ProportionalGascolour);

new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), P10TopSide_log, 
		  "P10TopSide", MuonVetoTopSide_log, false, 0);


//------------------ Endside

G4Box* MuonVetoEndSide_box = new G4Box("MuonVetoEndSide_box", 35.0*cm, 42.65*cm, 2.5*cm);

G4LogicalVolume* MuonVetoEndSide_log = new G4LogicalVolume(MuonVetoEndSide_box, Brass, "MuonVetoEndSide_log", 0, 0, 0);
MuonVetoEndSide_log -> SetVisAttributes(Brasscolour);

new G4PVPlacement(0, G4ThreeVector( 0., -7.15*cm, -46.0*cm-5.6*cm), 
		  MuonVetoEndSide_log, "MuonVetoEndSide", 
		  experimentalHall_log, false, 0);
//moved whole setup 5.6cm to place the front cap of detector at the coordinate origin

//------------------ Proportional Gas P10 inside the Endside  

G4Box* P10EndSide_box = new G4Box("P10EndSide_box", 33.5*cm, 41.15*cm, 2.0*cm);

G4LogicalVolume* P10EndSide_log = new G4LogicalVolume(P10EndSide_box, ProportionalGas, "P10EndSide_log", 0, 0, 0);
P10EndSide_log -> SetVisAttributes(ProportionalGascolour);

new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), P10EndSide_log, 
		  "P10EndSide", MuonVetoEndSide_log, false, 0);


//------------------- Side A and side B (on the right and on the left seen from frontside) 

G4Box* MuonVetoSide_box = new G4Box("MuonVetoSide_box", 2.5*cm, 29.15*cm, 47.00*cm);

G4LogicalVolume* MuonVetoSide_log = new G4LogicalVolume(MuonVetoSide_box, Brass, "MuonVetoSide_log", 0, 0, 0);
MuonVetoSide_log -> SetVisAttributes(Brasscolour);

  new G4PVPlacement(0, 
		    G4ThreeVector(37.50*cm, 6.35*cm, -3.5*cm-5.6*cm),
		    MuonVetoSide_log, "MuonVetoSideA", 
		    experimentalHall_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-37.50*cm, 6.35*cm, -3.5*cm-5.6*cm), 
		    MuonVetoSide_log, "MuonVetoSideB", 
		    experimentalHall_log, false, 0);
//moved whole setup 5.6cm to place the front cap of detector at the coordinate origin


// ------------------ Proportional Gas P10 inside the side A and side B

G4Box* P10Side_box = new G4Box("P10Side_box", 2.0*cm, 27.65*cm, 45.5*cm);

G4LogicalVolume* P10Side_log = new G4LogicalVolume(P10Side_box, ProportionalGas, "P10Side_log", 0, 0, 0);
P10Side_log -> SetVisAttributes(ProportionalGascolour);

new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), P10Side_log, 
		  "P10SideA", MuonVetoSide_log, false, 0);
//G4VPhysicalVolume* P10SideB_phys;
//P10SideB_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), P10Side_log, "P10SideB", MuonVetoSide_log, false, 0);




//-----------------3. SHIELDS(WITHOUT MUONVETOS)-------------------------------------------------------------

// STATUS:         Fixed Cooling fingers,  Dusan
//                 Copper in both doors still wrong and needs fixing!



//----------------- Lead-Shield

G4Box* PbShield_box = new G4Box("PbShield_box", 33.5*cm, 35.5*cm, 43.5*cm);

G4LogicalVolume* PbShield_log = new G4LogicalVolume(PbShield_box, Pb, "PbShield_log", 0, 0, 0);
PbShield_log -> SetVisAttributes(Pbcolour);

new G4PVPlacement(0, G4ThreeVector(0.,0.,-5.6*cm), 
		  PbShield_log, "PbShield", 
		  experimentalHall_log, false, 0);
//moved whole setup 5.6cm to place the front cap of detector at the coordinate origin

//------------------ Coolingfinger CF filled with N2gas through the lead-shield


G4Tubs* CFLeadShield_tubs = new G4Tubs("CFLeadShield_tubs", 0, 2.1*cm, 10.15*cm, 0, 2*M_PI);
G4Tubs* CFLeadShieldGas_tubs = new G4Tubs("CFLeadShieldGas_tubs", 0, 1.95*cm, 10.15*cm, 0, 2*M_PI);

G4LogicalVolume* CFLeadShield_log = new G4LogicalVolume(CFLeadShield_tubs, Cu, "CFLeadShield_log", 0, 0, 0);
G4LogicalVolume* CFLeadShieldGas_log = new G4LogicalVolume(CFLeadShieldGas_tubs, N2Gas, "CFLeadShieldGas_log", 0, 0, 0);

CFLeadShield_log -> SetVisAttributes(Cucolour);
CFLeadShieldGas_log -> SetVisAttributes(N2Gascolour);

G4RotationMatrix* HalfPi_rot_x = new G4RotationMatrix; 
HalfPi_rot_x ->rotateX(1.5*M_PI); 

new G4PVPlacement(HalfPi_rot_x, G4ThreeVector(0., -25.35*cm, -20.5*cm), 
		  CFLeadShield_log, "CFLeadShield", 
		  PbShield_log, false, 0);
new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		  CFLeadShieldGas_log, "CFLeadShieldGas", 
		  CFLeadShield_log, false, 0);


//------------------- Part 6 of the coldness-transfer-cable

G4Tubs* ColdTransfer6_tubs = new G4Tubs("ColdTransfer6_tubs", 0., 0.5*cm, 10.15*cm, 0, 2*M_PI);

G4LogicalVolume* ColdTransfer6_log = new G4LogicalVolume(ColdTransfer6_tubs, Cu, "ColdTransfer5_log", 0, 0, 0);
ColdTransfer6_log -> SetVisAttributes(Cucolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		  ColdTransfer6_log, "ColdTransfer6", 
		  CFLeadShieldGas_log, false, 0);


//------------------- Copper-planes, on which the lead-blocks of the doors are fixed

G4Box* CuPlane_box = new G4Box("CuPlane_box", 15.0*cm, 15.0*cm, 0.1*cm);

G4LogicalVolume* CuPlane_log = new G4LogicalVolume(CuPlane_box, Cu, "CuPlane_log", 0, 0, 0);
CuPlane_log -> SetVisAttributes(Cucolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, 38.4*cm), 
		  CuPlane_log, "CuPlaneFrontdoor1", 
		  PbShield_log, false, 0);

new G4PVPlacement(0, G4ThreeVector(0, 0, 33.4*cm), 
		  CuPlane_log, "CuPlaneFrontdoor2", 
		  PbShield_log, false, 0);
//G4VPhysicalVolume* CuPlaneFrontdoor3_phys;
//CuPlaneFrontdoor3_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 30.4*cm), CuPlane_log, "CuPlaneFrontdoor3", PbShield_log, false, 0);

new G4PVPlacement(0, G4ThreeVector(0, 0, -38.4*cm), 
		  CuPlane_log, "CuPlaneEnddoor1", 
		  PbShield_log, false, 0);
new G4PVPlacement(0, G4ThreeVector(0, 0, -33.4*cm), 
		  CuPlane_log, "CuPlaneEnddoor2", 
		  PbShield_log, false, 0);
//G4VPhysicalVolume* CuPlaneEnddoor3_phys;
//CuPlaneEnddoor3_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, -30.4*cm), CuPlane_log, "CuPlaneEnddoor3", PbShield_log, false, 0);


//------------------- Main-Copper-Shield

G4Box* CuShield_box = new G4Box("CuShield_box", 15.0*cm, 15.0*cm, 30.0*cm);

G4LogicalVolume* CuShield_log = new G4LogicalVolume(CuShield_box, Cu, "CuShield_log", 0, 0, 0);
CuShield_log -> SetVisAttributes(Cucolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, 0), CuShield_log, 
		  "CuShield", PbShield_log, false, 0);


//-------------------- Coolingfinger CF filled with N2gas through the main copper shield

G4Tubs* CFCopperShield_tubs = new G4Tubs("CFCopperShield_tubs", 0, 1.95*cm, 2.5*cm, 0, 2*M_PI);

G4LogicalVolume* CFCopperShield_log = new G4LogicalVolume(CFCopperShield_tubs, N2Gas, "CFCopperShield_log", 0, 0, 0);
CFCopperShield_log -> SetVisAttributes(N2Gascolour);

/*G4VPhysicalVolume* CFCopperShield_phys;
  CFCopperShield_phys = */ new G4PVPlacement(HalfPi_rot_x, G4ThreeVector(0., -12.5*cm, -20.5*cm), CFCopperShield_log, "CFCopperShield", CuShield_log, false, 0);

//--------------------- Part 4 of the coldness-transfer-cable

G4Tubs* ColdTransfer4_tubs = new G4Tubs("ColdTransfer4_tubs", 0., 0.5*cm, 2.5*cm, 0, 2*M_PI);

G4LogicalVolume* ColdTransfer4_log = new G4LogicalVolume(ColdTransfer4_tubs, Cu, "ColdTransfer4_log", 0, 0, 0);
ColdTransfer4_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* ColdTransfer4_phys;
  ColdTransfer4_phys = */ new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), ColdTransfer4_log, "ColdTransfer4", CFCopperShield_log, false, 0);


//--------------------- Copper-sheet above the Copper-Shield

G4Box* CuSheetAbove_box = new G4Box("CuSheetAbove",  25*cm, 0.1*cm, 43.5*cm);

G4LogicalVolume* CuSheetAbove_log = new G4LogicalVolume(CuSheetAbove_box, Cu, "CuSheetAbove_log", 0, 0, 0);
CuSheetAbove_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CuSheetAbove_phys;
  CuSheetAbove_phys = */  
 new G4PVPlacement(0, G4ThreeVector(0, 15.1*cm, 0), 
		   CuSheetAbove_log, "CuSheetAbove", 
		   PbShield_log, false, 0);


//--------------------- Copper-sheet beneath the Copper-Shield

G4Box* CuSheetBeneath_box = new G4Box("CuSheetBeneath", 15.2*cm, 0.1*cm, 43.5*cm);

G4LogicalVolume* CuSheetBeneath_log = new G4LogicalVolume(CuSheetBeneath_box, Cu, "CuSheetBeneath_log", 0, 0, 0);
CuSheetBeneath_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CuSheetBeneath_phys;
  CuSheetBeneath_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -15.1*cm, 0), 
		   CuSheetBeneath_log, "CuSheetBeneath", 
		   PbShield_log, false, 0);


//--------------------- Coolingfinger CF filled with N2gas through the copper-sheet beneath the  main copper shield

G4Tubs* CFCopperSheet_tubs = new G4Tubs("CFCopperSheet_tubs", 0, 1.95*cm, 0.1*cm, 0, 2*M_PI);

G4LogicalVolume* CFCopperSheet_log = new G4LogicalVolume(CFCopperSheet_tubs, N2Gas, "CFCopperSheet_log", 0, 0, 0);
CFCopperSheet_log -> SetVisAttributes(N2Gascolour);

/* G4VPhysicalVolume* CFCopperSheet_phys;
   CFCopperSheet_phys = */ 
 new G4PVPlacement(HalfPi_rot_x, G4ThreeVector(0., 0., -20.5*cm), 
		   CFCopperSheet_log, "CFCopperSheet", 
		   CuSheetBeneath_log, false, 0);


//---------------------- Part 5 of the coldness-transfer-cable

G4Tubs* ColdTransfer5_tubs = new G4Tubs("ColdTransfer5_tubs", 0., 0.5*cm, 0.1*cm, 0, 2*M_PI);

G4LogicalVolume* ColdTransfer5_log = new G4LogicalVolume(ColdTransfer5_tubs, Cu, "ColdTransfer5_log", 0, 0, 0);
ColdTransfer5_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* ColdTransfer5_phys;
  ColdTransfer5_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   ColdTransfer5_log, "ColdTransfer5", CFCopperSheet_log, 
		   false, 0);


//---------------------- Copper-sheets on both left and right sides of the Copper-Shield

G4Box* CuSheetLateral_box = new G4Box("CuSheetLateral", 0.1*cm, 15.0*cm, 43.5*cm);

G4LogicalVolume* CuSheetLateral_log = new G4LogicalVolume(CuSheetLateral_box, Cu,  "CuSheetLateral_log", 0, 0, 0);
CuSheetLateral_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CuSheetLateralLeft_phys;
  CuSheetLateralLeft_phys = */ 
new G4PVPlacement(0, G4ThreeVector(15.1*cm, 0., 0.), 
		  CuSheetLateral_log, "CuSheetLateralLeft", 
		  PbShield_log, false, 0);
/*G4VPhysicalVolume* CuSheetLateralRight_phys;
  CuSheetLateralRight_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-15.1*cm, 0., 0.), 
		   CuSheetLateral_log, "CuSheetLateralRight", 
		   PbShield_log, false, 0);






//------------------------4.  STRUCTURE OF THE N2-CHAMBER---------------------------------------------------------------------

// STATUS:            Fixed Cooling fingers,  Dusan




//------------------- N2-Chamber

G4Box* N2Chamber_box = new G4Box("N2Chamber_box", 10.0*cm, 10.0*cm, 25.0*cm);

G4LogicalVolume* N2Chamber_log = new G4LogicalVolume(N2Chamber_box, N2Gas, "N2Chamber_log", 0, 0, 0);
N2Chamber_log -> SetVisAttributes(N2Gascolour);

/* G4VPhysicalVolume* N2Chamber_phys;
   N2Chamber_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), 
		   N2Chamber_log, "N2Chamber", CuShield_log, false, 0);


//------------------ Copperwall in the N2-chamber

G4VSolid* CopperWall_Full = new G4Box("CopperWall_Full", 10.00*cm, 9.75*cm, 2.50*cm);
G4VSolid* CopperWall_Hole = new G4Tubs("CopperWall_Hole", 0, 2.1*cm, 3.00*cm, 0, 2*M_PI);
G4VSolid* CopperWall_Solid = new G4SubtractionSolid("CopperWall_Solid", CopperWall_Full, CopperWall_Hole, 0, G4ThreeVector(0, 0.25*cm, 0));

G4LogicalVolume* CopperWall_log = new G4LogicalVolume(CopperWall_Solid, Cu, "CopperWall_log", 0, 0, 0);
CopperWall_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CopperWall_phys;
  CopperWall_phys =*/ 
 new G4PVPlacement(0, G4ThreeVector(0, -0.25*cm, -13.5*cm), 
		   CopperWall_log, "CopperWall", N2Chamber_log, false, 0);



//----------------- Additional Copper-pieces between the Copper-wall and the Enddoor within the N2-chamber 

// Part 1 and 2

G4Box* CopperPiece_box = new G4Box("CopperPiece_box", 0.86*cm, 10.0*cm, 4.5*cm);

G4LogicalVolume* CopperPiece_log = new G4LogicalVolume(CopperPiece_box, Cu, "CopperPiece_log", 0, 0, 0);
CopperPiece_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* Copperpiece1_phys;
  Copperpiece1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(9.14*cm,0.,-20.5*cm), 
		   CopperPiece_log, "CopperPiece1", N2Chamber_log, 
		   false, 0);
 /*G4VPhysicalVolume* Copperpiece2_phys;
   Copperpiece2_phys =*/ 
 new G4PVPlacement(0, G4ThreeVector(-9.14*cm,0.,-20.5*cm), 
		   CopperPiece_log, "CopperPiece2", N2Chamber_log, false, 0);


//---------------- Cooling-Finger in the N2-chamber - horizontal part

G4VSolid* CoolingFingerHorizontal_tubs = new G4Tubs("CoolingFingerHorizontal_tubs", 1.95*cm, 2.1*cm, 7.3*cm, 0, 2*M_PI);
G4VSolid* CoolingFingerHorizontalCutOff_trd = new G4Trd("CoolingFingerHorizontalCutOff", 3.0*cm, 3.0*cm, 6.7*cm, 0.5*cm, 3.1*cm);
G4VSolid* CoolingFingerHorizontal_Solid = new G4SubtractionSolid("CoolingFingerHorizontal_Solid", CoolingFingerHorizontal_tubs, CoolingFingerHorizontalCutOff_trd, 0, G4ThreeVector(0, -3.6*cm, -5.2*cm));

G4LogicalVolume* CoolingFingerHorizontal_log = new G4LogicalVolume(CoolingFingerHorizontal_Solid, Cu, "CoolingFingerHorizontal_log", 0, 0, 0);
CoolingFingerHorizontal_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CoolingFingerHorizontal_phys;
  CoolingFingerHorizontal_phys = */
 new G4PVPlacement(0, G4ThreeVector(0., 0., -15.3*cm), 
		   CoolingFingerHorizontal_log, 
		   "CoolingFingerHorizontal", N2Chamber_log, false, 0);


//----------------- Part 2 of the coldness-transfer-cable

G4VSolid* ColdTransfer2_tubs = new G4Tubs("ColdTransfer2_tubs", 0., 0.5*cm, 7.3*cm, 0, 2*M_PI);
G4VSolid* ColdTransfer2CutOff_trd = new G4Trd("ColdTransfer2CutOff", 3.0*cm, 3.0*cm, 6.7*cm, 0.5*cm, 3.1*cm); 
G4VSolid* ColdTransfer2_solid = new G4SubtractionSolid("ColdTransfer2_solid", ColdTransfer2_tubs, ColdTransfer2CutOff_trd, 0, G4ThreeVector(0, -3.6*cm, -5.2*cm));

G4LogicalVolume* ColdTransfer2_log = new G4LogicalVolume(ColdTransfer2_solid, Cu, "ColdTransfer2_log", 0, 0, 0);
ColdTransfer2_log ->  SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* ColdTransfer2_phys;
  ColdTransfer2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., -15.3*cm), 
		   ColdTransfer2_log, "ColdTransfer2", 
		   N2Chamber_log, false, 0);


//------------------ Cooling-Finger in the N2-chamber - vertical part

G4VSolid* CoolingFingerVertical_tubs = new G4Tubs("CoolingFingerHorizontal_tubs", 1.95*cm, 2.1*cm, 6.05*cm, 0, 2*M_PI);
G4VSolid* CoolingFingerVerticalCutOff_trd = new G4Trd("CoolingFingerHorizontalCutOff", 3.0*cm, 3.0*cm, 6.7*cm, 0.5*cm, 3.1*cm);
G4VSolid* CoolingFingerVertical_solid = new G4SubtractionSolid("CoolingFingerVertical_solid", CoolingFingerVertical_tubs, CoolingFingerVerticalCutOff_trd, 0, G4ThreeVector(0, 3.6*cm, -3.95*cm));

G4LogicalVolume* CoolingFingerVertical_log = new G4LogicalVolume(CoolingFingerVertical_solid, Cu, "CoolingFingerVertical_log", 0, 0, 0);
CoolingFingerVertical_log -> SetVisAttributes(Cucolour);

/*G4VPhysicalVolume* CoolingFingerVertical_phys;
  CoolingFingerVertical_phys = */ 
 new G4PVPlacement(HalfPi_rot_x, G4ThreeVector(0., -3.95*cm, -20.5*cm), 
		   CoolingFingerVertical_log, "CoolingFingerVertical", 
		   N2Chamber_log, false, 0);


//------------------- Part 3 of the coldness-transfer-cable

G4VSolid* ColdTransfer3_tubs = new G4Tubs("ColdTransfer3_tubs", 0., 0.5*cm, 6.05*cm, 0, 2*M_PI);
G4VSolid* ColdTransfer3CutOff_trd = new G4Trd("ColdTransfer3CutOff", 3.0*cm, 3.0*cm, 6.7*cm, 0.5*cm, 3.1*cm);
G4VSolid* ColdTransfer3_solid = new G4SubtractionSolid("ColdTransfer3_solid", ColdTransfer3_tubs, ColdTransfer3CutOff_trd, 0, G4ThreeVector(0, 3.6*cm, -3.95*cm));

G4LogicalVolume* ColdTransfer3_log = new G4LogicalVolume(ColdTransfer3_solid, Cu, "ColdTransfer3_log", 0, 0, 0);
ColdTransfer3_log ->  SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* ColdTransfer3_phys;
   ColdTransfer3_phys = */ 
 new G4PVPlacement(HalfPi_rot_x, G4ThreeVector(0., -3.95*cm, -20.5*cm), 
		   ColdTransfer3_log, "ColdTransfer3", N2Chamber_log, 
		   false, 0);







//------------------- 5. OUTER DETECTOR HOUSING-----------------------------------------------------------

// STATUS:            Fixed screws , Dusan




//------------------ Outer Detector Housing as fullfilled body -----

G4VSolid* DetOuterHousing_body = new G4Tubs("DetOuterHousing_body",  0., 4.6*cm, 6.8*cm, 0, 2*M_PI);
G4VSolid* DetOuterHousing_hole = new G4Tubs("DetOuterHousins_hole",  4.25*cm, 5.0*cm, 6.8*cm, 0, 2*M_PI);
G4VSolid* DetOuterHousing_solid = new G4SubtractionSolid("DetOuterHousing_solid", DetOuterHousing_body, DetOuterHousing_hole, 0, G4ThreeVector(0., 0., 2.5*cm));

G4LogicalVolume* DetOuterHousing_log = new G4LogicalVolume(DetOuterHousing_solid, Al, "DetOuterHousing_log", 0, 0, 0);
DetOuterHousing_log -> SetVisAttributes(Alcolour);

/* G4VPhysicalVolume* DetOuterHousing_phys;
   DetOuterHousing_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., -1.2*cm), 
		   DetOuterHousing_log, "DetOuterHousingAl", 
		   N2Chamber_log, false, 0);

//------ ******* additional vacuum volume for different winow thickness : 3mm ********

G4Tubs* CapGap_tubs = new G4Tubs("CapGap_tubs", 0., 4.0*cm, 0.025*cm, 0, 2*M_PI);

G4LogicalVolume* CapGap_log = new G4LogicalVolume(CapGap_tubs, Vacuum, "CapGap_log", 0, 0, 0);
CapGap_log -> SetVisAttributes(Vacuumcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 6.475*cm), CapGap_log, "CapGapVac", DetOuterHousing_log, false, 0);


//------------------ Vacuum inside the Outer Detector Housing   
            
G4Tubs* Vacuum_tubs = new G4Tubs("Vacuum_tubs", 0., 4.0*cm, 6.625*cm, 0, 2*M_PI);

G4LogicalVolume* Vacuum_log = new G4LogicalVolume(Vacuum_tubs, Vacuum, "Vacuum_log", 0, 0, 0);
Vacuum_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* Vacuum_phys;
   Vacuum_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., -0.175*cm), 
		   Vacuum_log, "InHousingVac", DetOuterHousing_log, 
		   false, 0);


//------------------ Closing Parts of the Outer Detector Housing

//------------------ Part1

G4Tubs* DetOuterClosePart1_tubs = new G4Tubs("DetOuterClosePart1_tubs", 2.1*cm, 4.0*cm, 0.475*cm, 0, 2*M_PI);

G4LogicalVolume* DetOuterClosePart1_log = new G4LogicalVolume(DetOuterClosePart1_tubs, Al, "DetOuterClosePart1_log", 0, 0, 0);
DetOuterClosePart1_log -> SetVisAttributes(Alcolour);

/*G4VPhysicalVolume* DetOuterClosePart1_phys;
  DetOuterClosePart1_phys = */
 new G4PVPlacement(0, G4ThreeVector(0., 0., -6.15*cm), 
		   DetOuterClosePart1_log, "DetOuterClosePart1Al", 
		   Vacuum_log, false, 0);

//------------------- Part2

G4Tubs* DetOuterClosePart2_tubs = new G4Tubs("DetOuterClosePart2",  3.65*cm, 4.0*cm, 0.775*cm, 0, 2*M_PI);

G4LogicalVolume* DetOuterClosePart2_log = new G4LogicalVolume(DetOuterClosePart2_tubs, Al, "DetOuterClosePart2_log", 0, 0, 0);
DetOuterClosePart2_log -> SetVisAttributes(Alcolour);

/*G4VPhysicalVolume* DetOuterClosePart2_phys;
  DetOuterClosePart2_phys = */
 new G4PVPlacement(0, G4ThreeVector(0., 0., -4.9*cm), 
		   DetOuterClosePart2_log, "DetOuterClosePart2Al", 
		   Vacuum_log, false, 0);

//------------------- Part3: Copper-ring: belonging to the cooling finger and laying in Part1

G4Tubs* CopperRing_tubs = new G4Tubs("CopperRing_tubs", 1.95*cm, 2.1*cm, 0.475*cm, 0, 2*M_PI);

G4LogicalVolume* CopperRing_log = new G4LogicalVolume(CopperRing_tubs, Cu, "CopperRing_log", 0, 0, 0);
CopperRing_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* CopperRing_phys;
   CopperRing_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., -6.15*cm), 
		   CopperRing_log, "CopperRing", Vacuum_log, 
		   false, 0);


//------------------- Old-Roman-ship-iron-screws on the backside of the Outer Detector Housing

//------------------- Part of screws inside the Housing

G4Tubs* FeScrewIn_tubs = new G4Tubs("FeScrewIn",  0, 0.125*cm, 1.0*cm, 0, 2*M_PI);

G4LogicalVolume* FeScrewIn_log = new G4LogicalVolume(FeScrewIn_tubs, Fe, "FeScrewIn_log", 0, 0, 0);
FeScrewIn_log -> SetVisAttributes(Fecolour);

/*G4VPhysicalVolume* FeScrew1In_phys;
  FeScrew1In_phys = */
 new G4PVPlacement(0, G4ThreeVector(4.125*cm, 0., -5.8*cm), 
		   FeScrewIn_log, "FeScrew1In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew2In_phys;
    FeScrew2In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.9168*cm, 2.9168*cm, -5.8*cm), 
		   FeScrewIn_log, "FeScrew2In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew3In_phys;
    FeScrew3In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 4.125*cm, -5.8*cm), 
		   FeScrewIn_log, "FeScrew3In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew4In_phys;
    FeScrew4In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.9168*cm, 2.9168*cm, -5.8*cm), 
		   FeScrewIn_log, "FeScrew4In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew5In_phys;
    FeScrew5In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(- 4.125*cm, 0., -5.8*cm), 
		   FeScrewIn_log, "FeScrew5In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew6In_phys;
    FeScrew6In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.9168*cm, -2.9168*cm, -5.8*cm), 
		   FeScrewIn_log, "FeScrew6In", DetOuterHousing_log, false, 0);
 /*G4VPhysicalVolume* FeScrew7In_phys;
   FeScrew7In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -4.125*cm, -5.8*cm), 
		   FeScrewIn_log, "FeScrew7In", DetOuterHousing_log, false, 0);
 /* G4VPhysicalVolume* FeScrew8In_phys;
    FeScrew8In_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.9168*cm, -2.9168*cm , -5.8*cm), 
		   FeScrewIn_log, "FeScrew8In", DetOuterHousing_log, false, 0);


//------------------- Part of screws outside the Housing

G4Tubs* FeScrewOut_tubs = new G4Tubs("FeScrewOut", 0., 0.375*cm, 0.3*cm, 0, 2*M_PI);

G4LogicalVolume* FeScrewOut_log = new G4LogicalVolume(FeScrewOut_tubs, Fe, "FeScrewOut_log", 0, 0, 0);
FeScrewOut_log -> SetVisAttributes(Fecolour);

/* G4VPhysicalVolume* FeScrew1Out_phys;
   FeScrew1Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(4.125*cm, 0., -8.3*cm), 
		   FeScrewOut_log, "FeScrew1Out", N2Chamber_log, false, 0);
 /* G4VPhysicalVolume* FeScrew2Out_phys;
    FeScrew2Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.9168*cm, 2.9168*cm, -8.3*cm), 
		   FeScrewOut_log, "FeScrew2Out", N2Chamber_log, false, 0);
 /* G4VPhysicalVolume* FeScrew3Out_phys;
    FeScrew3Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 4.125*cm, -8.3*cm), 
		   FeScrewOut_log, "FeScrew3Out", N2Chamber_log , false, 0);
 /* G4VPhysicalVolume* FeScrew4Out_phys;
    FeScrew4Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.9168*cm, 2.9168*cm, -8.3*cm), 
		   FeScrewOut_log, "FeScrew4Out", N2Chamber_log , false, 0);
 /* G4VPhysicalVolume* FeScrew5Out_phys;
    FeScrew5Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(- 4.125*cm, 0., -8.3*cm), 
		   FeScrewOut_log, "FeScrew5Out", N2Chamber_log , false, 0);

 /* G4VPhysicalVolume* FeScrew6Out_phys;
    FeScrew6Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.9168*cm, -2.9168*cm, -8.3*cm), 
		   FeScrewOut_log, "FeScrew6Out", N2Chamber_log , false, 0);
 /* G4VPhysicalVolume* FeScrew7Out_phys;
    FeScrew7Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -4.125*cm, -8.3*cm), 
		   FeScrewOut_log, "FeScrew7Out", N2Chamber_log , false, 0);
 /* G4VPhysicalVolume* FeScrew8Out_phys;
    FeScrew8Out_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.9168*cm, -2.9168*cm , -8.3*cm), 
		   FeScrewOut_log, "FeScrew8Out", N2Chamber_log, false, 0);





//------------------- 6. INNER PARTS BESIDE THE INNER DETECTOR HOUSING-------------------------------


// STATUS:            fixed cooling cable, overlapping volumes,  Dusan




//------------------- VespelFrontCover

G4Tubs* VespelFrontCover_tubs = new G4Tubs("VespelFrontCover_tubs", 0, 3.7*cm, 0.15*cm, 0, 2*M_PI);

G4LogicalVolume* VespelFrontCover_log = new G4LogicalVolume(VespelFrontCover_tubs, Vespel, "VespelFrontCover_log", 0, 0, 0);
VespelFrontCover_log -> SetVisAttributes(Vespelcolour);

/* G4VPhysicalVolume* VespelFrontCover_phys;
   VespelFrontCover_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 6.115*cm), 
		   VespelFrontCover_log, "VespelFrontCover", 
		   Vacuum_log, false, 0);


//------------------- VespelClosingPart

G4Tubs* VespelClosingPart_tubs = new G4Tubs("VespelClosingPart_tubs", 3.47*cm, 3.7*cm, 0.5*cm, 0, 2*M_PI);

G4LogicalVolume* VespelClosingPart_log = new G4LogicalVolume(VespelClosingPart_tubs, Vespel, "VespelClosingPart_log", 0, 0, 0);
VespelClosingPart_log -> SetVisAttributes(Vespelcolour);

/* G4VPhysicalVolume* VespelClosingPart_phys;
   VespelClosingPart_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 5.465*cm), 
		   VespelClosingPart_log, "VespelClosingPart", 
		   Vacuum_log, false, 0);


//------------------- Copper-screws pressing against the inner detector housing

G4Box* ScrewInnerHousing_box = new G4Box("ScrewInnerHousing_box", 0.2*cm, 0.2*cm, 0.2*cm);

G4LogicalVolume* ScrewInnerHousing_log = new G4LogicalVolume(ScrewInnerHousing_box, Cu, "ScrewInnerHousing_log", 0, 0, 0);
ScrewInnerHousing_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* ScrewInnerHousing1a_phys;
   ScrewInnerHousing1a_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -3.67*cm, 1.265*cm), 
		   ScrewInnerHousing_log, "ScrewInnerHousing1aCu", 
		   Vacuum_log, false, 0);
 /* G4VPhysicalVolume* ScrewInnerHousing1b_phys;
    ScrewInnerHousing1b_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -3.67*cm, 3.765*cm), 
		   ScrewInnerHousing_log, "ScrewInnerHousing1bCu", 
		   Vacuum_log, false, 0);
 /* G4VPhysicalVolume* ScrewInnerHousing2a_phys;
    ScrewInnerHousing2a_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(3.67*cm, 0, 1.265*cm), 
		   ScrewInnerHousing_log, "ScrewInnerHousing2aCu", 
		   Vacuum_log, false, 0);
 /* G4VPhysicalVolume* ScrewInnerHousing2b_phys;
    ScrewInnerHousing2b_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(3.67*cm, 0, 3.765*cm), 
		   ScrewInnerHousing_log, "ScrewInnerHousing2bCu", 
		   Vacuum_log, false, 0);


//-------------------- FET

G4VSolid* Fet_box = new G4Box("Fet_box", 1.6*cm, 0.75*cm, 0.8*cm);
G4VSolid* Fet_cavity = new G4Box("Fet_cavity", 1.4*cm, 0.55*cm, 0.6*cm);
G4VSolid* Fet_solid = new G4SubtractionSolid("Fet_solid", Fet_box, Fet_cavity, 0, G4ThreeVector(0., 0., 0.));

G4LogicalVolume* Fet_log = new G4LogicalVolume(Fet_solid, Cu, "Fet_log", 0, 0, 0 );
Fet_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* Fet_phys;
   Fet_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 2.75*cm, -2.135*cm), 
		   Fet_log, "FetCu", Vacuum_log, false, 0);


//-------------------- FET-holder

//-------------------- Part1

G4VSolid* FetHolderPart1_body = new G4Box("FetHolderPart1_body", 1.6*cm, 1.7*cm, 0.1*cm);
G4VSolid* FetHolderPart1_hole = new G4Tubs("FetHolderPart1_hole", 0., 1.25*cm , 0.2*cm, 0, 2*M_PI);
G4VSolid* FetHolderPart1_solid = new G4SubtractionSolid("FetHolderPart1_solid", FetHolderPart1_body, FetHolderPart1_hole, 0, G4ThreeVector(0., -0.1*cm, 0.));

G4LogicalVolume* FetHolderPart1_log = new G4LogicalVolume(FetHolderPart1_solid, Cu, "FetHolderPart1_log", 0, 0, 0);
FetHolderPart1_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* FetHolderPart1_phys;
   FetHolderPart1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0.1*cm, -2.835*cm), 
		   FetHolderPart1_log, "FetHolderPart1Cu", 
		   Vacuum_log, false, 0);

//------------------- Part2 

G4Box* FetHolderPart2_box = new G4Box("FetHolderPart2_body", 1.6*cm, 0.1*cm, 0.9*cm);

G4LogicalVolume* FetHolderPart2_log = new G4LogicalVolume(FetHolderPart2_box, Cu, "FetHolderPart2_log", 0, 0, 0);
FetHolderPart2_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* FetHolderPart2_phys;
   FetHolderPart2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 1.9*cm , -2.035*cm), 
		   FetHolderPart2_log, "FetHolderPart2Cu", 
		   Vacuum_log, false, 0);

//------------------- Part3

G4Box* FetHolderPart3_box = new G4Box("FetHolderPart3_body", 1.6*cm, 0.75*cm, 0.1*cm);

G4LogicalVolume* FetHolderPart3_log = new G4LogicalVolume(FetHolderPart3_box, Cu, "FetHolderPart3_log", 0, 0, 0);
FetHolderPart3_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* FetHolderPart3_phys;
   FetHolderPart3_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 2.75*cm , -1.235*cm), 
		   FetHolderPart3_log, "FetHolderPart3Cu", Vacuum_log, 
		   false, 0);


//------------------- Cooled big screw

G4Tubs* CooledBigScrew_tubs = new G4Tubs("CooledBigScrew_tubs", 0, 1.25*cm, 1.25*cm, 0, 2*M_PI);

G4LogicalVolume* CooledBigScrew_log = new G4LogicalVolume(CooledBigScrew_tubs, Al, "CooledBigScrew_log", 0, 0, 0);
CooledBigScrew_log ->  SetVisAttributes(Alcolour);

/* G4VPhysicalVolume* CooledBigScrew_phys;
   CooledBigScrew_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -3.665*cm), 
		   CooledBigScrew_log, "CooledBigScrewAl", 
		   Vacuum_log, false, 0);


//------------------- Cooled middle screw

G4Tubs* CooledMiddleScrew_tubs = new G4Tubs("CooledMiddleScrew_tubs", 0, 0.98*cm, 0.25*cm, 0, 2*M_PI);

G4LogicalVolume* CooledMiddleScrew_log = new G4LogicalVolume(CooledMiddleScrew_tubs, Cu, "CooledMiddleScrew_log", 0, 0, 0);
CooledMiddleScrew_log ->  SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* CooledMiddleScrew_phys;
   CooledMiddleScrew_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -0.21*cm, -5.165*cm), 
		   CooledMiddleScrew_log, "CooledMiddleScrewCu", 
		   Vacuum_log, false, 0);


//------------------- Cooled small screw

G4Tubs* CooledSmallScrew_tubs = new G4Tubs("CooledSmallScrew_tubs", 0, 0.62*cm, 0.25*cm, 0, 2*M_PI);

G4LogicalVolume* CooledSmallScrew_log = new G4LogicalVolume(CooledSmallScrew_tubs, Cu, "CooledSmallScrew_log", 0, 0, 0);
CooledSmallScrew_log ->  SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* CooledSmallScrew_phys;
   CooledSmallScrew_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -5.665*cm), 
		   CooledSmallScrew_log, "CooledSmallScrewCu", 
		   Vacuum_log, false, 0);


//------------------- Part 1 of the coldness-transfer-cable

G4Tubs* ColdTransfer1_tubs = new G4Tubs("ColdTransfer1_tubs", 0., 0.5*cm, 0.355*cm, 0, 2*M_PI);

G4LogicalVolume* ColdTransfer1_log = new G4LogicalVolume(ColdTransfer1_tubs, Cu, "ColdTransfer1_log", 0, 0, 0);
ColdTransfer1_log ->  SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* ColdTransfer1_phys;
   ColdTransfer1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -6.27*cm), 
		   ColdTransfer1_log, "ColdTransfer1Cu", 
		   Vacuum_log, false, 0);


//------------------- Spider (original from Canberra)

G4Tubs* Spider_tubs = new G4Tubs("Spider_tubs", 1.25*cm, 3.75*cm, 0.57*cm, 0, 2*M_PI);

G4LogicalVolume* Spider_log = new G4LogicalVolume(Spider_tubs, Teflon, "Spider_log", 0, 0, 0);
Spider_log -> SetVisAttributes(Tefloncolour);

/* G4VPhysicalVolume* Spider_phys;
   Spider_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -3.505*cm), 
		   Spider_log, "SpiderTeflon", Vacuum_log, false, 0);


//------------------- Alu-ring inside the spider

G4Tubs* SpiderRing_tubs = new G4Tubs("SpiderRing_tubs", 1.25*cm, 1.45*cm, 0.67*cm, 0, 2*M_PI);

G4LogicalVolume* SpiderRing_log = new G4LogicalVolume(SpiderRing_tubs, Al, "SpiderRing_log", 0, 0, 0);
SpiderRing_log -> SetVisAttributes(Alcolour);

/* G4VPhysicalVolume* SpiderRing_phys;
   SpiderRing_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   SpiderRing_log, "SpiderRingAl", 
		   Spider_log, false, 0);


//------------------- 3 screws holding the spider

G4Tubs* SpiderScrew_tubs = new G4Tubs("SpiderScrew_tubs", 0., 0.3*cm, 0.8*cm, 0, 2*M_PI);

G4LogicalVolume* SpiderScrew_log = new G4LogicalVolume(SpiderScrew_tubs, Cu, "SpiderScrew_log", 0, 0, 0);
SpiderScrew_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* SpiderScrew1_phys;
   SpiderScrew1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.73*cm, 1.575*cm, -4.875*cm), 
		   SpiderScrew_log, "SpiderScrew1Cu", Vacuum_log, false, 0);
 /* G4VPhysicalVolume* SpiderScrew2_phys;
    SpiderScrew2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.73*cm, 1.575*cm, -4.875*cm), 
		   SpiderScrew_log, "SpiderScrew2Cu", Vacuum_log, false, 0);
 /* G4VPhysicalVolume* SpiderScrew3_phys;
    SpiderScrew3_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -3.15*cm, -4.875*cm), 
		   SpiderScrew_log, "SpiderScrew3Cu", Vacuum_log, false, 0);


//------------------- Parts of the 3 Spider-screws inside the spider-tube 

G4Tubs* SpiderScrewIn_tubs = new G4Tubs("SpiderScrewIn_tubs", 0., 0.3*cm, 0.57*cm, 0, 2*M_PI);

G4LogicalVolume* SpiderScrewIn_log = new G4LogicalVolume(SpiderScrewIn_tubs, Cu, "SpiderScrewIn_log", 0, 0, 0);
SpiderScrewIn_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* SpiderScrewIn1_phys;
   SpiderScrewIn1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(2.73*cm, 1.575*cm, 0.), 
		   SpiderScrewIn_log, "SpiderScrewIn1Cu", Spider_log, 
		   false, 0);
 /* G4VPhysicalVolume* SpiderScrewIn2_phys;
    SpiderScrewIn2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(-2.73*cm, 1.575*cm, 0.), 
		   SpiderScrewIn_log, "SpiderScrewIn2Cu", Spider_log, 
		   false, 0);
 /* G4VPhysicalVolume* SpiderScrewIn3_phys;
    SpiderScrewIn3_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, -3.15*cm, 0.), 
		   SpiderScrewIn_log, "SpiderScrewIn3Cu", Spider_log, false, 0);






//------------------- 7. INNER DETECTOR HOUSING--------------------------------------------------

// STATUS:            Fixed overlapping parts, Dusan




//------------------- Inner detector housing as fullfilled body

G4VSolid* DetInnerHousing_body = new G4Tubs("DetInnerHousing_tubs", 0, 3.47*cm, 4.19*cm, 0, 2*M_PI);
G4VSolid* DetInnerHousing_hole1 = new G4Tubs("DetInnerHousing_hole1", 1.4*cm, 5.*cm, 1.80*cm, 0, 2*M_PI);
G4VSolid* DetInnerHousing_hole2 = new G4Tubs("DetInnerHousing_hole2", 2.435*cm, 5.0*cm, 2.3*cm, 0, 2*M_PI);
G4VSolid* DetInnerHousing_solid1 = new G4SubtractionSolid("DetInnerHousing_solid1", DetInnerHousing_body, DetInnerHousing_hole1, 0, G4ThreeVector(0, 0, -4.19*cm));
G4VSolid* DetInnerHousing_solid2 = new G4SubtractionSolid("DetInnerHousing_solid2", DetInnerHousing_solid1, DetInnerHousing_hole2, 0, G4ThreeVector(0, 0, -4.19*cm));

G4LogicalVolume* DetInnerHousing_log = new G4LogicalVolume(DetInnerHousing_solid2, Al, "DetInnerHousing_log", 0, 0, 0);
DetInnerHousing_log -> SetVisAttributes(Alcolour);

/* G4VPhysicalVolume* DetInnerHousing_phys;
   DetInnerHousing_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 1.775*cm), 
		   DetInnerHousing_log, "DetInnerHousingAl", 
		   Vacuum_log, false, 0);


//------------------- Teflon feedthrough (further parts in the detector housing)

//------------------- Part1

G4Tubs* TeflonFeedthroughPart1_tubs = new G4Tubs("TeflonFeedthroughPart1_tubs",   0, 0.505*cm, 0.7*cm, 0, 2*M_PI);

G4LogicalVolume* TeflonFeedthroughPart1_log = new G4LogicalVolume(TeflonFeedthroughPart1_tubs, Teflon, "TeflonFeedthroughPart1_log", 0, 0, 0);
TeflonFeedthroughPart1_log -> SetVisAttributes(Tefloncolour);

/* G4VPhysicalVolume* TeflonFeedthroughPart1_phys;
   TeflonFeedthroughPart1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -2.89*cm), 
		   TeflonFeedthroughPart1_log, 
		   "TeflonFeedthroughPart1", 
		   DetInnerHousing_log, false, 0);


//------------------- Vacuum inside Part1

G4Tubs* InsideFeedthroughPart1_tubs = new G4Tubs("InsideFeedthroughPart1_tubs", 0, 0.305*cm, 0.62*cm, 0, 2*M_PI);

G4LogicalVolume* InsideFeedthroughPart1_log = new G4LogicalVolume(InsideFeedthroughPart1_tubs, Vacuum, "InsideFeedthroughPart1_log", 0, 0, 0);
InsideFeedthroughPart1_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* InsideFeedthroughPart1_phys;
   InsideFeedthroughPart1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 0.08*cm), 
		   InsideFeedthroughPart1_log, 
		   "InsideFeedthroughPart2Vac", 
		   TeflonFeedthroughPart1_log, false, 0);


//------------------- Part 1 of contact rod belonging to the contact spring inside vacuum of Part 1 of the feedthrough

G4Tubs* ContactRodPart1_tubs = new G4Tubs("ContactRodPart1_tubs", 0, 0.15*cm, 0.62*cm, 0, 2*M_PI);

G4LogicalVolume* ContactRodPart1_log = new G4LogicalVolume(ContactRodPart1_tubs, Cu, "ContactRodPart1_log", 0, 0, 0);
ContactRodPart1_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* ContactRodPart1_phys;
   ContactRodPart1_phys = */
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   ContactRodPart1_log, "ContactRodPart1Cu", 
		   InsideFeedthroughPart1_log, false, 0);


//------------------- Part2

G4Tubs* TeflonFeedthroughPart2_tubs = new G4Tubs("TeflonFeedthroughPart2_tubs",  0, 2.235*cm, 0.25*cm, 0, 2*M_PI);

G4LogicalVolume* TeflonFeedthroughPart2_log = new G4LogicalVolume(TeflonFeedthroughPart2_tubs, Teflon, "TeflonFeedthroughPart2_log", 0, 0, 0);
TeflonFeedthroughPart2_log -> SetVisAttributes(Tefloncolour);

/* G4VPhysicalVolume* TeflonFeedthroughPart2_phys;
   TeflonFeedthroughPart2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -1.94*cm), 
		   TeflonFeedthroughPart2_log, 
		   "TeflonFeedthroughPart2", DetInnerHousing_log, 
		   false, 0);


//-------------------- Vacuum A inside Part2

G4Tubs* InsideFeedthroughPart2A_tubs = new G4Tubs("InsideFeedthroughPart2A_tubs",  0, 0.305*cm, 0.20*cm, 0, 2*M_PI);

G4LogicalVolume* InsideFeedthroughPart2A_log = new G4LogicalVolume(InsideFeedthroughPart2A_tubs, Vacuum, "InsideFeedthroughPart2A_log", 0, 0, 0);
InsideFeedthroughPart2A_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* InsideFeedthroughPart2A_phys;
   InsideFeedthroughPart2A_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -0.05*cm), 
		   InsideFeedthroughPart2A_log, 
		   "InsideFeedthroughPart2AVac", 
		   TeflonFeedthroughPart2_log, false, 0);


//-------------------- Part 2 of contact rod belonging to the contact spring

G4Tubs* ContactRodPart2_tubs = new G4Tubs("ContactRodPart2_tubs", 0, 0.15*cm, 0.20*cm, 0, 2*M_PI);

G4LogicalVolume* ContactRodPart2_log = new G4LogicalVolume(ContactRodPart2_tubs, Cu, "ContactRodPart2_log", 0, 0, 0);
ContactRodPart2_log -> SetVisAttributes(Cucolour);

/* G4VPhysicalVolume* ContactRodPart2_phys;
   ContactRodPart2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   ContactRodPart2_log, "ContactrodPart2Cu", 
		   InsideFeedthroughPart2A_log, false, 0);


//-------------------- Vacuum B inside Part2

G4Tubs* InsideFeedthroughPart2B_tubs = new G4Tubs("InsideFeedthroughPart2B_tubs", 0, 2.135*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* InsideFeedthroughPart2B_log = new G4LogicalVolume(InsideFeedthroughPart2B_tubs, Vacuum, "InsideFeedthroughPart2B_log", 0, 0, 0);
InsideFeedthroughPart2B_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* InsideFeedthroughPart2B_phys;
   InsideFeedthroughPart2B_phys = */ 
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0.20*cm), 
		    InsideFeedthroughPart2B_log, 
		    "InsideFeedthroughPart2BVac", 
		    TeflonFeedthroughPart2_log, false, 0);


//-------------------- Part 3 of the contact spring in vacuum B inside Part2

G4Cons* ContactSpringPart3_cons = new G4Cons("ContactSpringPart3_cons", 0.8*cm, 0.9*cm, 0.5*cm, 0.6*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* ContactSpringPart3_log = new G4LogicalVolume(ContactSpringPart3_cons, Bronze, "ContactSpringPart3", 0, 0, 0);
ContactSpringPart3_log -> SetVisAttributes(Bronzecolour);
 
/* G4VPhysicalVolume* ContactSpringPart3_phys;
   ContactSpringPart3_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0. ), 
		   ContactSpringPart3_log, 
		   "ContactSpringPart3Bronze", 
		   InsideFeedthroughPart2B_log, false, 0);






//-------------------- 8. TEFLON VESSEL AND CRYSTAL-----------------------------------------------

// STATUS:             Fixed Chineese hat position,vacuum cavity in Teflon cup, crystal groove,B-dead layer (300nm)  Dusan 



//-------------------- Teflon Crystal cup (fullfilled)

G4Tubs* TeflonCrystalCup_tubs = new G4Tubs("TeflonCrystalCup_tubs",  0., 3.27*cm, 2.94*cm, 0, 2*M_PI);

G4LogicalVolume* TeflonCrystalCup_log = new G4LogicalVolume(TeflonCrystalCup_tubs, Teflon, "TeflonCrystalCup_log", 0, 0, 0);
TeflonCrystalCup_log -> SetVisAttributes(Tefloncolour);

/* G4VPhysicalVolume* TeflonCrystalCup_phys;
   TeflonCrystalCup_phys = */ 
 new G4PVPlacement(0,G4ThreeVector(0.,0.,1.25*cm), 
		   TeflonCrystalCup_log, "TeflonCrystalCup", 
		   DetInnerHousing_log, false, 0);


//-------------------- Crystal: DI 646 from PGT

G4Tubs* Crystal_tubs = new G4Tubs("Crystal_tubs",  0., 3.22*cm,  2.89*cm, 0, 2*M_PI);

G4LogicalVolume* Crystal_log = new G4LogicalVolume(Crystal_tubs, Ge, "Crystal_log", 0, 0, 0);
Crystal_log -> SetVisAttributes(Gecolour);

/* G4VPhysicalVolume* Crystal_phys;
   Crystal_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 0.05*cm), Crystal_log, 
		   "CrystalGe", TeflonCrystalCup_log, false, 0);



//-------------------- Dead-layer around crystal

G4Tubs* DeadLayerAroundCrystal_tubs = new G4Tubs("DeadLayerAroundCrystal_tubs",  3.22*cm-d_side, 3.22*cm, 2.89*cm, 0, 2*M_PI);

G4LogicalVolume* DeadLayerAroundCrystal_log = new G4LogicalVolume(DeadLayerAroundCrystal_tubs, GeLi, "DeadLayerAroundCrystal_log", 0, 0, 0);
DeadLayerAroundCrystal_log -> SetVisAttributes(GeLicolour);

/* G4VPhysicalVolume* DeadLayerAroundCrystal_phys;
   DeadLayerAroundCrystal_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   DeadLayerAroundCrystal_log, 
		   "DeadLayerAroundCrystalGe(Li)", Crystal_log, false, 0);



//--------------------- Deadlayer-frontside

G4Tubs* DeadLayerFrontside_tubs = new G4Tubs("DeadLayerFrontside_tubs", 0., 3.22*cm-d_side,  d_front/2, 0, 2*M_PI);

G4LogicalVolume* DeadLayerFrontside_log = new G4LogicalVolume(DeadLayerFrontside_tubs, GeLi, "DeadLayerFrontside_log", 0, 0, 0);
DeadLayerFrontside_log -> SetVisAttributes(GeLicolour);

/* G4VPhysicalVolume* DeadLayerFrontside_phys;
   DeadLayerFrontside_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, 2.89*cm-d_front/2), 
		   DeadLayerFrontside_log, "DeadLayerFrontsideGe(Li)", 
		   Crystal_log, false, 0);




//--------------------- Deadlayer-endside  ***************************  groove now moves with R-hole !!!!!! dR=1.25mm, Sgroove=3.25mm **************************

//--------------------- Part 1 (GeLi-diffused part) (thickness made equal to side DL)

G4Tubs* DeadLayerEndside1_tubs = new G4Tubs("DeadLayerEndside1_tubs", hole_radius+0.5*cm, 3.22*cm-d_side,  d_side/2, 0, 2*M_PI);

G4LogicalVolume* DeadLayerEndside1_log = new G4LogicalVolume(DeadLayerEndside1_tubs, GeLi, "DeadLayerEndside1_log", 0, 0, 0);
DeadLayerEndside1_log -> SetVisAttributes(GeLicolour);

/* G4VPhysicalVolume* DeadLayerEndside1_phys;
   DeadLayerEndside1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, d_side/2-2.89*cm), 
		   DeadLayerEndside1_log, "DeadLayerEndside1Ge(Li)", 
		   Crystal_log, false, 0);

//--------------------- Part 2 (GeB-diffused part)

G4Tubs* DeadLayerEndside2_tubs = new G4Tubs("DeadLayerEndside2_tubs", hole_radius+0.00015*mm, hole_radius+0.175*cm,  0.00015*mm, 0, 2*M_PI);

G4LogicalVolume* DeadLayerEndside2_log = new G4LogicalVolume(DeadLayerEndside2_tubs, GeB, "DeadLayerEndside2_log", 0, 0, 0);
DeadLayerEndside2_log -> SetVisAttributes(GeBcolour);

/* G4VPhysicalVolume* DeadLayerEndside2_phys;
   DeadLayerEndside2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -2.889985*cm), 
		   DeadLayerEndside2_log, "DeadLayerEndside2Ge(B)", 
		   Crystal_log, false, 0);


//--------------------- Small Hole between Part 1 and 2 of the deadlayer-endside = crystal groove

G4Tubs* HoleBetweenPart1And2_tubs = new G4Tubs("HoleBetweenPart1And2_tubs", hole_radius+0.175*cm, hole_radius+0.5*cm,  0.2*cm, 0, 2*M_PI);

G4LogicalVolume* HoleBetweenPart1And2_log = new G4LogicalVolume(HoleBetweenPart1And2_tubs, Vacuum, "HoleBetweenPart1And2_log", 0, 0, 0);
HoleBetweenPart1And2_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* HoleBetweenPart1And2_phys;
   HoleBetweenPart1And2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., -2.69*cm), 
		   HoleBetweenPart1And2_log, "HoleBetweenPart1And2Vac", 
		   Crystal_log, false, 0);


//--------------------- Deadlayer around crystal-hole (fullfilled)

G4Tubs* DeadLayerAroundHole_tubs = new G4Tubs("DeadLayerAroundHole_tubs",  0., hole_radius+0.00015*mm, hole_length/2+0.00015*mm, 0, 2*M_PI);

G4LogicalVolume* DeadLayerAroundHole_log = new G4LogicalVolume(DeadLayerAroundHole_tubs, GeB, "DeadLayerAroundHole_log", 0, 0, 0);
DeadLayerAroundHole_log -> SetVisAttributes(GeBcolour);

/* G4VPhysicalVolume* DeadLayerAroundHole_phys;
   DeadLayerAroundHole_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, hole_length/2+0.00015*mm-2.89*cm),
		   DeadLayerAroundHole_log, "DeadLayerAroundHoleGe(B)", 
		   Crystal_log, false, 0);


//--------------------- Vacuum inside the crystal-hole

G4Tubs* VacuumInCrystalHole_tubs = new G4Tubs("VacuumInCrystalHole_tubs", 0., hole_radius, hole_length/2, 0, 2*M_PI);

G4LogicalVolume* VacuumInCrystalHole_log = new G4LogicalVolume(VacuumInCrystalHole_tubs, Vacuum, "VacuumInCrystalHole_log", 0, 0, 0);
VacuumInCrystalHole_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* VacuumInCrystalHole_phys;
   VacuumInCrystalHole_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0, 0, -0.00015*mm), 
		   VacuumInCrystalHole_log, "VacuumInCrystalHole", 
		   DeadLayerAroundHole_log, false, 0);


//--------------------- Part 1 of the contact-spring in the crystal-hole = chineese hat

G4Cons* ContactSpringPart1_cons = new G4Cons("ContactSpringPart1_cons", 0.4*cm, 0.5*cm, 0.01*cm, 0.1*cm, 0.25*cm, 0, 2*M_PI);

G4LogicalVolume* ContactSpringPart1_log = new G4LogicalVolume(ContactSpringPart1_cons, Bronze, "ContactSpringPart1", 0, 0, 0);
ContactSpringPart1_log -> SetVisAttributes(Bronzecolour);
 
/* G4VPhysicalVolume* ContactSpringPart1_phys;
   ContactSpringPart1_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*cm-hole_length/2), 
		   ContactSpringPart1_log, "ContactSpringPart1Bronze", 
		   VacuumInCrystalHole_log, false, 0);


//--------------------- Vacuum at cavitiy in the endside of the TeflonCrystalCup

G4Tubs* VacuumInTeflonCrystalCup_tubs = new G4Tubs("VacuumInTeflonCrystalCu_tubs", 0., 1.97*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* VacuumInTeflonCrystalCup_log = new G4LogicalVolume(VacuumInTeflonCrystalCup_tubs, Vacuum, "VacuumInTeflonCrystalCup_log", 0, 0, 0);
VacuumInTeflonCrystalCup_log -> SetVisAttributes(Vacuumcolour);

/* G4VPhysicalVolume* VacuumInTeflonCrystalCup_phys ;
   VacuumInTeflonCrystalCup_phys = */
 new G4PVPlacement(0, G4ThreeVector(0, 0, -2.89*cm), 
		   VacuumInTeflonCrystalCup_log, 
		   "VacuumInTeflonCrystalCup", TeflonCrystalCup_log, 
		   false, 0);


//--------------------- Part 2 of the contact spring in the cavity of the TeflonCrystalCup

G4Cons* ContactSpringPart2_cons = new G4Cons("ContactSpringPart2_cons", 0.5*cm, 0.6*cm, 0.4*cm, 0.5*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* ContactSpringPart2_log = new G4LogicalVolume(ContactSpringPart2_cons, Bronze, "ContactSpringPart2", 0, 0, 0);
ContactSpringPart2_log -> SetVisAttributes(Bronzecolour);
 
/* G4VPhysicalVolume* ContactSpringPart2_phys;
   ContactSpringPart2_phys = */ 
 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
		   ContactSpringPart2_log, "ContactSpringPart2Bronze", 
		   VacuumInTeflonCrystalCup_log , false, 0);



  // ---------------------------------------------------------------------------------------
  // ---  The samples
  // ---------------------------------------------------------------------------------------
 
G4VSolid* sample_shape=0;
  
if (G4Material::GetMaterial(sample_mat_name))
sample_mat = G4Material::GetMaterial(sample_mat_name);
 else 
   {
     MGLog(error) << "The material called " << sample_mat_name << " does not exist" << endlog;
     MGLog(error) << "Please check and run MaGe again!" << endlog;
     G4Exception("GECorradoDetector::ConstructDetector()","err001",FatalException,"Exit MaGe");
   }

if (sample_geom=="none")
  {
    //---- for point-like sources
  }
 else if (sample_geom=="box")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

sample_shape = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log",0 , 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it
new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_thickness/2+5.6*cm), sample_log, "sample", N2Chamber_log, false, 0);

   }


 else if (sample_geom=="tube")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+tube_length/2+5.6*cm), sample_log, "sample", N2Chamber_log, false, 0);
   }


 else if (sample_geom=="sbox")
    {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

      // --- standard cylindrical box
      // box cover (the thinner wall) is facing the detector!!!!!!!
      // z-position of the sample relative to the acrylic box can be adjusted by box_width parameter

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap_log  = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+0.05*cm), detplasticcap_log, "detplasticcap", N2Chamber_log, false, 0);


  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+2.975*cm), detplasticcap1_log, "detplasticcap1", N2Chamber_log, false, 0);


  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+1.5*cm), detplasticshell_log, "detplasticshell", N2Chamber_log, false, 0);


  // cylinder sample inside the standard box (default - filling the volume)
  sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);  

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_width+5.6*cm+1.5*cm), sample_log, "sample", N2Chamber_log, false, 0);

    }


 else if (sample_geom=="liquid")
    {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

      // --- standard cylindrical box _with liquid sample_
      //
      // box cover (the thinner wall) is facing the detector!!!!!!!

 
  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap_log  = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+0.05*cm), detplasticcap_log, "detplasticcap", N2Chamber_log, false, 0);


  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+2.975*cm), detplasticcap1_log, "detplasticcap1", N2Chamber_log, false, 0);


  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+1.5*cm), detplasticshell_log, "detplasticshell", N2Chamber_log, false, 0);


  // liquid filling the standard box up to level of "boxheight"
  
  G4VSolid* sample_body   = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
  G4VSolid* sample_hole   = new G4Box("sample_hole", tube_outrad+0.1*cm, tube_outrad, tube_length/2+0.1*cm);
  sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, G4ThreeVector(0., 2*tube_outrad-box_height,0.));

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+5.6*cm+1.5*cm), sample_log, "sample", N2Chamber_log, false, 0);
    }


 else if (sample_geom=="marinelli")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

     // "marinelli" shape (box with a hole)
G4VSolid* sample_body = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);
G4VSolid* sample_hole = new G4Tubs("sample_tube", 0., tube_outrad, tube_length, 0, tube_angle);
sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, G4ThreeVector(0., tube_innrad, -box_thickness/2));

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it
new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_thickness/2+5.6*cm), sample_log, "sample", N2Chamber_log, false, 0);
   }



 else if (sample_geom=="roundmarinelli")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

//  box_height = liquid level measured from center
//  box_width = (external) length of the front part
//  box_thickness = wall thickness (also the difference between front and belt radius)
//  tube_innrad = radius of internal hole
//  tube_outrad = external radius of the belt part
//  tube_length = length of the belt part ( = depth of internal hole)

     // true marinelli shape (tube with a hole), including PE (?) contaniner of thickness "boxthickness" and hole depth "boxwidth"
     //
G4VSolid* marinelli1_tube = new G4Tubs("marinelli1_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
G4VSolid* marinelli2_tube = new G4Tubs("marinelli2_tube", 0, tube_outrad+box_thickness, box_width/2, 0, tube_angle);
G4VSolid* marinelli_shape = new G4UnionSolid("marinelli_shape", marinelli1_tube, marinelli2_tube, 0, G4ThreeVector(0., 0.,tube_length/2+box_width/2));
//
G4LogicalVolume* marinelli_log = new G4LogicalVolume(marinelli_shape, PE, "marinelli_log", 0, 0, 0);  
marinelli_log -> SetVisAttributes(Acryliccolour);
//
// the position of the middle of the BACK SIDE of the sample container relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it
 new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z-tube_length/2+5.6*cm), marinelli_log, "MarinelliPEHousing", N2Chamber_log, false, 0);
 //
G4VSolid* sample1_tube = new G4Tubs("sample1_tube", tube_innrad+box_thickness, tube_outrad-box_thickness, tube_length/2, 0, tube_angle);
G4VSolid* sample2_tube = new G4Tubs("sample2_tube", 0, tube_outrad, box_width/2-box_thickness, 0, tube_angle);
G4VSolid* sample_upper = new G4Box("sample_upper", 10.*cm, 10.*cm, 20.*cm);
G4VSolid* sample_lower = new G4Box("sample_lower", 10.*cm, 10.*cm, 20.*cm);
//
G4VSolid* help1_shape = new G4UnionSolid("help1_shape", sample1_tube, sample2_tube, 0, G4ThreeVector(0., 0.,tube_length/2+box_width/2-box_thickness));

sample_shape = new G4SubtractionSolid("sample_shape", help1_shape, sample_upper, 0, G4ThreeVector(0., box_height+10.*cm,0.));
G4VSolid* N2blanket_shape= new G4SubtractionSolid("N2blanket_shape", help1_shape, sample_lower, 0, G4ThreeVector(0., box_height-10.*cm,0.));

G4LogicalVolume* N2blanket_log = new G4LogicalVolume(N2blanket_shape, N2Gas, "N2blanket_log", 0, 0, 0);  
N2blanket_log -> SetVisAttributes(N2Gascolour);
G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);
//
new G4PVPlacement(0, G4ThreeVector(0, 0, box_thickness), N2blanket_log, "N2blanket", marinelli_log, false, 0);
new G4PVPlacement(0, G4ThreeVector(0, 0, box_thickness), sample_log, "sample", marinelli_log, false, 0);
   }



 else if (sample_geom=="custom")
   {
      // here define any other sample geometry - feel free to replace contents


     /*
  // ------------ NPL-C: small cylindrical box (cover side facing detector) , z-position can be adjusted by box_width parameter --------------
     //--------------- double plastic wrap around the box (approximated by one disc)
G4Tubs* detplastic_tubs = new G4Tubs("detplastic_tubs", 0.*cm, 4.*cm, 0.0125*cm, 0, 2*M_PI);

G4LogicalVolume* detplastic_log  = new G4LogicalVolume(detplastic_tubs, PE, "detplastic_log", 0, 0, 0);
detplastic_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, box_width+5.6*cm+0.0125*cm), detplastic_log, "detplastic",  N2Chamber_log, false, 0);



G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 2.13*cm, 0.04*cm, 0, 2*M_PI);

G4LogicalVolume* detplasticcap_log  = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
detplasticcap_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, box_width+5.6*cm+0.065*cm), detplasticcap_log, "detplasticcap",  N2Chamber_log, false, 0);



G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 2.13*cm, 2.26*cm, 0.86*cm, 0, 2*M_PI);

G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
detplasticshell_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, box_width+5.6*cm+0.965*cm), detplasticshell_log, "detplasticshell",  N2Chamber_log, false, 0);



G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 2.26*cm, 0.055*cm, 0, 2*M_PI);

G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
detplasticcap1_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(0, G4ThreeVector(0, 0, box_width+5.6*cm+1.88*cm), detplasticcap1_log, "detplasticcap1",  N2Chamber_log, false, 0);


  // cylinder sample inside the standard box (filling the volume, no options)
sample_shape = new G4Tubs("sample_tube", 0, 2.13*cm, 0.86*cm, 0, tube_angle);  

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(0, 0, box_width+5.6*cm+0.965*cm), sample_log, "sample",  N2Chamber_log, false, 0);
     */

     //---------------------------------- 7 Flexible steel tubes (sample 205) -------------------------------

/*

G4Tubs* flexsteel_tubs = new G4Tubs("flexsteel_tubs", 1*cm, 1.35*cm, 5*cm, 0, 2*M_PI);
//sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);

 G4VSolid* flexsteel_tubs2 = new G4UnionSolid("flexsteel_tubs2", flexsteel_tubs, flexsteel_tubs, 0, G4ThreeVector(2.7*cm,0.,0.));
 G4VSolid* flexsteel_tubs3 = new G4UnionSolid("flexsteel_tubs3", flexsteel_tubs2, flexsteel_tubs, 0, G4ThreeVector(-2.7*cm,0.,0.));

 G4RotationMatrix* halfPi_z_rot = new G4RotationMatrix;
 halfPi_z_rot -> rotateZ(0.5*M_PI);

 G4VSolid* flexsteel_tubs5 = new G4UnionSolid("flexsteel_tubs5", flexsteel_tubs3, flexsteel_tubs2, halfPi_z_rot, G4ThreeVector(5.6*cm,5.4*cm,0.));
 G4VSolid* flexsteel_tubs7 = new G4UnionSolid("flexsteel_tubs7", flexsteel_tubs5, flexsteel_tubs2, halfPi_z_rot, G4ThreeVector(-5.6*cm,5.4*cm,0.));

 G4LogicalVolume* sample_log = new G4LogicalVolume(flexsteel_tubs7, sample_mat, "sample_log", 0, 0, 0);  
 sample_log -> SetVisAttributes(Sample_Colour);

 G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
 halfPi_x_rot -> rotateX(0.5*M_PI);

 new G4PVPlacement(halfPi_x_rot, G4ThreeVector(s_x, s_y, s_z+1.35*cm+5.6*cm), sample_log, "sample", N2Chamber_log, false, 0);
*/

  // ------------------------------------ SAMPLE 0220_1: Air Filter  --------------------------------//
  
	//Cylindrical box encapsulating the CELLAR Air sample for measurement 0220_1 in February 2009
   	//Box directly attached to the front of the detector endcap
   	//Geometry: Radius = 2.5 cm; Margin-Width = 0.025 cm;
  	//	      Height = 0.5 cm; Margin Top/Bottom = 0.025 cm each


/* 
  
  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 2.55*cm, 0.0125*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, PVC, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+0.0125*cm), detplasticcap1_log, "detplasticcap1", N2Chamber_log, false, 0);


  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 2.50*cm, 2.55*cm, 0.25*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, PVC, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+5.6*cm+0.025*cm+0.25*cm), detplasticshell_log, "detplasticshell", N2Chamber_log, false, 0);


  // cylinder sample inside the standard box (default - filling the volume)
  sample_shape = new G4Tubs("sample_tube", 0.0*cm, 2.5*cm, 0.25*cm, 0, 2*M_PI);  

  G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);


  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+5.6*cm+0.025*cm+0.25*cm), sample_log, "sample", N2Chamber_log, false, 0);
*/

/*

  // -------------------------------------- SAMPLE 0224/0225: Krypton tubes --------------------------- //
  
  G4Material* CovarGlass = G4Material::GetMaterial("covarglass");

  	//Peculiarly formed cobar glass tubes containing krypton gas sample
	//directly attached to the detector end cap
	//Measurement in March 2009

  //--- ROTATIONS ---//
  
  G4RotationMatrix* Pi_x_rot = new G4RotationMatrix;
  Pi_x_rot -> rotateX(1.0*M_PI);
  
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);
  
  G4RotationMatrix* halfPi_x_rot_2 = new G4RotationMatrix;
  halfPi_x_rot_2 -> rotateX(1.5*M_PI);
  

  //--- GLASS CASING (inside casing) ---//
  
  //Sample volume in top cone
  G4Cons* cone_top = new G4Cons("cone_top", .0*cm, .25*cm, .0*cm, .000001*cm, .600*cm, 0, 2.0*M_PI);

  //Sample volume in cylinder top 
  G4Tubs* cylinder_top = new G4Tubs("cylinder_top", 0.25*cm, 0.70*cm, 0.06*cm, 0, 2*M_PI);

  //Sample volume in cylinder 
  G4Tubs* cylinder = new G4Tubs("cylinder", 0.0*cm, 0.70*cm, 1.00*cm, 0, 2*M_PI);

  //Sample volume in bottom cone
  G4Cons* cone_bottom = new G4Cons("cone_bottom", .0*cm, .70*cm, .0*cm, .000001*cm, .500*cm, 0, 2*M_PI);

  //--- GROUP CASING ---//
  
  G4VSolid* help_shape_1 = new G4UnionSolid("help_shape_1", cylinder, cylinder_top, 0, G4ThreeVector(0., 0.,1.060*cm));
  G4VSolid* help_shape_2 = new G4UnionSolid("help_shape_2", help_shape_1, cone_top, 0, G4ThreeVector(0., 0.,1.720*cm));
  G4VSolid* casing_shape = new G4UnionSolid("casing_shape", help_shape_2, cone_bottom, Pi_x_rot, G4ThreeVector(0., 0.,-1.500*cm));

  //--- PLACE CASING as physical Volume ---//
  G4LogicalVolume* casing_log =  new G4LogicalVolume(casing_shape, CovarGlass, "casing_log", 0, 0, 0);
  casing_log -> SetVisAttributes(Acryliccolour);
  G4VPhysicalVolume* casing_phys;
  casing_phys = new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0, 0, s_z+5.6*cm+0.7*cm), casing_log, "glasscasing", N2Chamber_log, false, 0);

  //--- SAMPLE VOLUME (inside casing) ---//
  
  //Sample volume in top cone
  G4Cons* cone_top_sample = new G4Cons("cone_top_sample", 0.0*cm, 0.19*cm, 0.0*cm, 0.0000001*cm, 0.45*cm, 0, 2.0*M_PI);

  //Sample volume in cylinder top 
  G4Tubs* cylinder_top_sample = new G4Tubs("cylinder_top_sample", 0.0*cm, 0.20*cm, 0.06*cm, 0, 2*M_PI);

  //Sample volume in cylinder 
  G4Tubs* cylinder_sample = new G4Tubs("cylinder_sample", 0.0*cm, 0.58*cm, 1.00*cm, 0, 2*M_PI);

  //Sample volume in bottom cone
  G4Cons* cone_bottom_sample = new G4Cons("cone_bottom_sample", .0*cm, .58*cm, .0*cm, .000001*cm, .415*cm, 0, 2*M_PI);

  //--- GROUP SAMPLE PARTS ---//
  
  G4VSolid* help_shape_3 = new G4UnionSolid("help_shape_3", cylinder_sample, cylinder_top_sample, 0, G4ThreeVector(0., 0.,1.060*cm));
  G4VSolid* help_shape_4 = new G4UnionSolid("help_shape_4", help_shape_3, cone_top_sample, 0, G4ThreeVector(0., 0.,1.570*cm));
  G4VSolid* sample_shape = new G4UnionSolid("sample_shape", help_shape_4, cone_bottom_sample, Pi_x_rot, G4ThreeVector(0., 0.,-1.415*cm));

  //--- PLACE SAMPLE inside the glass casing ---//
  G4LogicalVolume* sample_log =  new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
  sample_log -> SetVisAttributes(Sample_Colour);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), sample_log, "sample", casing_log, false, 0);

*/
  //----------------------------------- SAMPLE 0225_1: SIMPLE CYLINDRICAL GEOMETRY (for efficiency test) -------------------------------------- //
  
/*  
  //--- ROTATIONS ---//
  
  G4RotationMatrix* Pi_x_rot = new G4RotationMatrix;
  Pi_x_rot -> rotateX(1.0*M_PI);
  
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);
  
  G4RotationMatrix* halfPi_x_rot_2 = new G4RotationMatrix;
  halfPi_x_rot_2 -> rotateX(1.5*M_PI);
  
  //Sample volume in cylinder top 
  G4Tubs* cylinder_top = new G4Tubs("cylinder_top", 0.0*cm, 0.70*cm, 0.06*cm, 0, 2*M_PI);

  //Sample volume in cylinder 
  G4Tubs* cylinder = new G4Tubs("cylinder", 0.0*cm, 0.70*cm, 1.15*cm, 0, 2*M_PI);
  
  //Sample volume in cylinder bottom
  G4Tubs* cylinder_bottom = new G4Tubs("cylinder_bottom", 0.0*cm, 0.70*cm, 0.06*cm, 0, 2*M_PI);
  
  G4VSolid* help_shape_1 = new G4UnionSolid("help_shape_1", cylinder, cylinder_top, 0, G4ThreeVector(0., 0.,1.210*cm));
  G4VSolid* casing_shape = new G4UnionSolid("casing_shape", help_shape_1, cylinder_bottom, 0, G4ThreeVector(0., 0.,-1.210*cm));
  
  //--- PLACE CASING as physical Volume ---//
  G4LogicalVolume* casing_log =  new G4LogicalVolume(casing_shape, CovarGlass, "casing_log", 0, 0, 0);
  casing_log -> SetVisAttributes(Acryliccolour);
  G4VPhysicalVolume* casing_phys;
  casing_phys = new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0, 0, s_z+5.6*cm+0.7*cm), casing_log, "glasscasing", N2Chamber_log, false, 0);

  //Sample volume in cylinder 
  G4Tubs* sample_shape = new G4Tubs("sample_shape", 0.0*cm, 0.58*cm, 1.15*cm, 0, 2*M_PI);
  
  //--- PLACE SAMPLE inside the glass casing ---//
  G4LogicalVolume* sample_log =  new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
  sample_log -> SetVisAttributes(Sample_Colour);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), sample_log, "sample", casing_log, false, 0);
 

 
 
 // -------------------------------------- SAMPLE 0221-0223: Erbium sample (from MPIK-EBIT) --------------------------- //
 
 	//Small pieces of erbium placed directly at the end cap of the detector
	//Simulated volume: small cylindrical piece of erbium

 G4Tubs* sample_shape = new G4Tubs("sample_shape", 0.0*cm, 0.3*cm, 0.075*cm, 0, 2*M_PI);
 G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
 sample_log -> SetVisAttributes(Sample_Colour);

 new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_width+5.6*cm+0.2*cm), sample_log, "sample", N2Chamber_log, false, 0);  



// --------------------------------------- SAMPLE 0230: Krypton in cylindrical glass vessel ------------------------- //

  G4Material* DuranGlass = G4Material::GetMaterial("duranglass");
  
  //--- GLASS CASING
  G4Tubs* cylinder = new G4Tubs("cylinder", 0.0*cm, 2.00*cm, 2.40*cm, 0, 2*M_PI);
  G4Cons* cone = new G4Cons("cone", .0*cm, 2.00*cm, .0*cm, 0.60*cm, 0.60*cm, 0, 2.0*M_PI);
  
  G4VSolid* casing_shape = new G4UnionSolid("casing_shape", cylinder, cone, 0, G4ThreeVector(0., 0.,3.00*cm));

  //--- PLACE CASING as physical Volume ---//
  G4LogicalVolume* casing_log =  new G4LogicalVolume(casing_shape, DuranGlass, "casing_log", 0, 0, 0);
  casing_log -> SetVisAttributes(Acryliccolour);
  G4VPhysicalVolume* casing_phys;
  casing_phys = new G4PVPlacement(0, G4ThreeVector(0, -6.30*cm, s_z+5.6*cm-3.00*cm), casing_log, "glasscasing", N2Chamber_log, false, 0);
  
  //--- SAMPLE VOLUME
  G4Tubs* cylinder_sample = new G4Tubs("cylinder_sample", 0.0*cm, 1.90*cm, 2.30*cm, 0, 2*M_PI);
  G4Cons* cone_sample = new G4Cons("cone_sample", .0*cm, 1.90*cm, .0*cm, 0.50*cm, 0.60*cm, 0, 2.0*M_PI);
  
  G4VSolid* sample_shape = new G4UnionSolid("sample_shape", cylinder_sample, cone_sample, 0, G4ThreeVector(0., 0.,2.90*cm));
  
  //--- PLACE SAMPLE inside the glass casing ---//
  G4LogicalVolume* sample_log =  new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
  sample_log -> SetVisAttributes(Sample_Colour);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), sample_log, "sample", casing_log, false, 0);
  
  //--- Copper underlay
  G4Box* underlay = new G4Box("underlay_shape", 1.5*cm, 0.85*cm, 7.50*cm);
  G4LogicalVolume* underlay_log =  new G4LogicalVolume(underlay, Cu, "underlay_log", 0, 0, 0);
  underlay_log -> SetVisAttributes(Cucolour);
  new G4PVPlacement(0, G4ThreeVector(0, -9.15*cm, s_z+5.6*cm), underlay_log, "underlay", N2Chamber_log, false, 0);
 
*/



// ----------------------------------- SAMPLE 0229:  Steal screw threads for DOUBLE CHOOZ --------------------- //


  G4RotationMatrix* Pi_y_rot = new G4RotationMatrix;
  Pi_y_rot -> rotateY(1.0*M_PI);
  
  G4RotationMatrix* halfPi_y_rot = new G4RotationMatrix;
  halfPi_y_rot -> rotateY(0.5*M_PI);
  
  //Sample Volume
  
  //Parts of single steel thread
  G4Tubs* cylinder_head = new G4Tubs("cylinder_head", 0.5*cm, 1.60*cm, 2.00*cm, 0, 2*M_PI);
  G4Tubs* cylinder_body = new G4Tubs("cylinder_body", 0.0*cm, 0.60*cm, 6.50*cm, 0, 2*M_PI);
  
  //Single steel thread
  G4VSolid* single_thread_1 = new G4UnionSolid("single_thread_1", cylinder_body, cylinder_head, 0, G4ThreeVector(0., 0.,8.50*cm));
  G4VSolid* single_thread_2 = new G4UnionSolid("single_thread_2", cylinder_body, cylinder_head, 0, G4ThreeVector(0., 0.,8.50*cm));
  
  //Double steel thread
  G4VSolid* double_thread = new G4UnionSolid("double_thread",single_thread_1,single_thread_2, Pi_y_rot, G4ThreeVector(2.40*cm, 0.,4.00*cm));
  
  //Quadruple steel thread
  G4VSolid* quad_thread = new G4UnionSolid("quad_thread",double_thread,double_thread, 0, G4ThreeVector(2*2.40*cm, 0,0));
  
  //Octuple steel thread
  G4VSolid* oct_thread = new G4UnionSolid("oct_thread",quad_thread,quad_thread, 0, G4ThreeVector(4*2.40*cm, 0,0));
  
  //Dodekanuple steel thread
  G4VSolid* twelve_thread = new G4UnionSolid("twelve_thread",oct_thread,quad_thread, halfPi_y_rot, G4ThreeVector(10.5*cm, 0, 13.0*cm));
  
  //PLACE twelve steel threads as sample volume
  G4LogicalVolume* twelve_thread_log =  new G4LogicalVolume(twelve_thread, Fe, "twelve_thread_log", 0, 0, 0);
  twelve_thread_log -> SetVisAttributes(Fecolour);
  new G4PVPlacement(0, G4ThreeVector(-8.4*cm, -8.4*cm, -3.50*cm), twelve_thread_log, "sample", N2Chamber_log, false, 0);
  
   }


 else if (sample_geom=="collimator")
   {
//----------------------- THE COLLIMATOR -----------------------------------------------

G4VSolid* Collimator_Body = new G4Box("Collimator_Body", 3.5*cm, 3.5*cm, 2.*cm);
G4VSolid* Collimator_Hole = new G4Tubs("Collimator_Hole", 0., 0.079*cm, 2.*cm, 0, 2*M_PI);
G4VSolid* Collimator = new G4SubtractionSolid("Collimator", Collimator_Body, Collimator_Hole, 0, G4ThreeVector(0, 0, 0));

G4LogicalVolume* Collimator_log = new G4LogicalVolume(Collimator, Pb, "Collimator_log", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(coll_x ,coll_y ,coll_z), Collimator_log, "CollimatorPb", N2Chamber_log, false, 0); 

Collimator_log -> SetVisAttributes(Collimatorcolour);
   } 



 else if (sample_geom=="NPL-GL")
   {
     //----------------------------------- NPL bottle with fancy shape :-) -------------------------------------
//rotate 90deg up :
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);

     //--------------- plastic wrap around the box (approximated by one shell)
G4Tubs* detplastic_tubs = new G4Tubs("detplastic_tubs", 3.62*cm, 3.6325*cm, 8*cm, 0, 2*M_PI);

G4LogicalVolume* detplastic_log  = new G4LogicalVolume(detplastic_tubs, PE, "detplastic_log", 0, 0, 0);
detplastic_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0, 0, box_width+5.6*cm+3.6325*cm), detplastic_log, "detplastic",  N2Chamber_log, false, 0);


      // --------------------------- The NPL bottle
G4Tubs* NPLbottle_tubs = new G4Tubs("NPLbottle_tubs", 0., 3.62*cm, 6.5*cm, 0, 2*M_PI);

//bottom of the bottle
G4VSolid* bottom_sphere   = new G4Sphere("bottom_sphere", 0., 3.975*cm, 0.,twopi, 0.,twopi);
G4VSolid* cut_box   = new G4Box("cut_box", 10.*cm, 10.*cm, 10.*cm);
G4VSolid* bottom_first = new G4SubtractionSolid("bottom_first", bottom_sphere, cut_box, 0, G4ThreeVector(0., 0.,8.358*cm));
G4VSolid* bottom_shape = new G4SubtractionSolid("bottom_shape", bottom_first, cut_box, 0, G4ThreeVector(0., 0.,-12.642*cm));

G4VSolid* bottom2_sphere   = new G4Sphere("bottom2_sphere", 0., 7.25*cm, 0.,twopi, 0.,twopi);
//G4VSolid* bottom2_shape = new G4SubtractionSolid("bottom2_shape", bottom2_sphere, cut_box, 0, G4ThreeVector(0., 0.,-3.2*cm));

G4VSolid* NPLbottom_shape = new G4SubtractionSolid("NPLbottom_shape", bottom_shape, bottom2_sphere, 0, G4ThreeVector(0., 0.,-9.442*cm));

G4VSolid* NPLbottle_shape = new G4UnionSolid("NPLbottle_shape", NPLbottle_tubs, NPLbottom_shape, 0, G4ThreeVector(0.,0.,-4.858*cm));

G4LogicalVolume* NPLbottle_log = new G4LogicalVolume(NPLbottle_shape, PE, "NPLbottle_log", 0, 0, 0);
NPLbottle_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0., 0.2*cm, box_width+5.6*cm+3.6325*cm), NPLbottle_log, "NPLbottle", N2Chamber_log, false, 0);


//contents : cylinder
G4VSolid* sample_tubs   = new G4Tubs("sample_tube", 0., tube_outrad,  box_height/2, 0, 2*M_PI);

//bottom
G4VSolid* Sbottom_sphere   = new G4Sphere("Sbottom_sphere", 0., 3.875*cm, 0.,twopi, 0.,twopi);
G4VSolid* Sbottom_first = new G4SubtractionSolid("Sbottom_first", Sbottom_sphere, cut_box, 0, G4ThreeVector(0., 0.,8.358*cm));
G4VSolid* Sbottom_shape = new G4SubtractionSolid("Sbottom_shape", Sbottom_first, cut_box, 0, G4ThreeVector(0., 0.,-12.5*cm));

G4VSolid* Sbottom2_sphere   = new G4Sphere("Sbottom2_sphere", 0., 7.4*cm, 0.,twopi, 0.,twopi);

G4VSolid* sample_first = new G4SubtractionSolid("sample_first", Sbottom_shape, Sbottom2_sphere, 0, G4ThreeVector(0., 0.,-9.442*cm));

sample_shape = new G4UnionSolid("sample_shape", sample_tubs, sample_first, 0, G4ThreeVector(0.,0.,6.5*cm-box_height/2-4.858*cm));


G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(0, 0,  box_height/2-6.5*cm), sample_log, "sample",  NPLbottle_log, false, 0);

   }
 else
   {
     MGLog(error) << "The shape is not defined " << endlog;
     MGLog(fatal) << endlog;
     G4Exception("GECorradoDetector::ConstructDetector()","err002",FatalException,"Check the shape type");
   }
 
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  
G4SDManager* SDman   =  G4SDManager::GetSDMpointer();
G4String CrystalSDname      = "/mydet/gerda/gecrystal";
GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);
SDman->AddNewDetector(CrystalSD);


Crystal_log-> SetSensitiveDetector(CrystalSD);

SetDetectorLogical(experimentalHall_log);
SetDetectorName("ExperimentalHall");
 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
