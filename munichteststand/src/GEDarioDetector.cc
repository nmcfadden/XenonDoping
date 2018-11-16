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
// $Id: GEDarioDetector.cc,v 1.5 2009-05-18 16:08:53 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GEDarioDetector.cc
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
 * 03-07-2007, Modified a bit sample-geometries, Dusan
 * 14-05-2009, Changed expHall_log material to Air, Dusan
 *
 */
//---------------------------------------------------------------------------//
//

//#include "globals.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
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

#include "munichteststand/GEDarioDetector.hh"
#include "munichteststand/GEDarioDetectorMessenger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GEDarioDetector::GEDarioDetector() : MGGeometryDetector("")
{
  //set defaults
  sample_geom="sphere";
  //box height is also liquid level in the standard box
  box_height=1.1*cm;
  box_width=2.*cm;
  box_thickness=2.*cm;
  /*
  G4double marinellibox_height=17.6*cm;
  G4double marinellibox_width=18.5*cm;
  G4double marinellibox_thickness=17.5*cm;
  G4double marinellitube_length=11.1*cm;
  G4double marinellitube_outrad=4.95*cm;
  */
  tube_innrad=0*cm;
  tube_outrad=3.65*cm;
  tube_length=2.8*cm;
  tube_angle=twopi;
  sphere_rad=0.00001*cm;
  sample_mat_name="NitrogenGas";
  //G4cout << "\n\n" << sample_mat_name << "\n\n";
  s_x=0.*cm;
  s_y=0.*cm;
  s_z=1.6*cm;
  //crystal parameters
  d_z=0.7*cm;
  d_dead=0.2*cm;
  // f_size=1.;
  crystal_d=5.95*cm;
  crystal_l=6.1*cm;
  theMessenger = new GEDarioDetectorMessenger(this);
}

//---------------------------------------------------------------------------//

GEDarioDetector::~GEDarioDetector()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void GEDarioDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  G4Material* Brass = G4Material::GetMaterial("Brass");
  G4Material* ProportionalGas = G4Material::GetMaterial("ProportionalGas");
  G4Material* N2Gas = G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  G4Material* Pb = G4Material::GetMaterial("MetalLead");
  G4Material* Fe = G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  G4Material* Vespel = G4Material::GetMaterial("Vespel");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  //G4Material* PE = G4Material::GetMaterial("PE");
  G4Material* Teflon = G4Material::GetMaterial("Teflon");
  G4Material* Air = G4Material::GetMaterial("Air");

 // Visualization attributes
  G4VisAttributes * red= new G4VisAttributes( G4Colour(255/255. ,0/255. ,12/255. ));
  G4VisAttributes * Brasscolour = red;

  G4VisAttributes * Sample_Colour = new G4VisAttributes(  G4Colour(255/255. ,0/255. ,12/255. ));
  Sample_Colour -> SetForceSolid(true);

  G4VisAttributes * skyblue = new G4VisAttributes( G4Colour(175/255. ,164/255. ,255/255. ));
  G4VisAttributes * Vacuumcolour = skyblue;

  G4VisAttributes * orange= new G4VisAttributes( G4Colour(241/255. ,224/255. ,0/255. ));
  G4VisAttributes * ProportionalGascolour = orange;

  G4VisAttributes * darkviolet = new G4VisAttributes( G4Colour(64/255. ,0/255. ,64/255. ));
  G4VisAttributes * Pbcolour = darkviolet;

  G4VisAttributes * grass= new G4VisAttributes( G4Colour(0/255. ,239/255. ,0/255. ));
  G4VisAttributes * Cucolour = grass;

  G4VisAttributes * violet= new G4VisAttributes( G4Colour(224/255. ,0/255. ,255/255. ));
  G4VisAttributes * Gecolour = violet;

  G4VisAttributes * purple= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));
  G4VisAttributes * GeLicolour = purple;

  G4VisAttributes * redbrown = new G4VisAttributes( G4Colour(123/255. ,45/255. ,65/255. ));
  G4VisAttributes * Fecolour = redbrown;
  //Fecolour -> SetForceSolid(true);

  G4VisAttributes * blue = new G4VisAttributes( G4Colour(0/255. ,0/255. ,255/255. ));
  G4VisAttributes * N2Gascolour = blue;

  G4VisAttributes * grayva= new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));
  G4VisAttributes * Alcolour = grayva;
  // Alcolour -> SetForceSolid(true);

  G4VisAttributes * yellowgray= new G4VisAttributes( G4Colour(214/255. ,214/255. ,207/255. ));
  G4VisAttributes * Vespelcolour = yellowgray;

  G4VisAttributes * indigo= new G4VisAttributes( G4Colour(0/255. ,0/255. ,190/255. ));
  G4VisAttributes * Acryliccolour = indigo;



  //------------------------------------------------------ volumes
  //------------------------------ experimental hall (world volume) 
  G4double expHall_x = 1*m;
  G4double expHall_y = 1*m;
  G4double expHall_z = 1*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  G4LogicalVolume* experimentalHall_log = new 
    G4LogicalVolume(experimentalHall_box, Air, "expHall_log", 0, 0, 0);
  experimentalHall_log -> SetVisAttributes(G4VisAttributes::Invisible);

  
  // ----------------------------  veto plates
  // --- front & back plate

  G4Box* veto1_box
    = new G4Box("veto1_box", 42.5*cm, 37.5*cm, 2.5*cm);
  G4LogicalVolume* veto1_log
    = new G4LogicalVolume(veto1_box, Brass, "veto1_log", 0, 0, 0); 
  veto1_log -> SetVisAttributes(Brasscolour);
  //veto1_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(0.,0, -49.5*cm), veto1_log, 
			"veto10", experimentalHall_log, true, 0);
  new G4PVPlacement(0, G4ThreeVector(0.,0, 49.5*cm), veto1_log, 
			"veto11", experimentalHall_log, true, 1);


  G4Box* vetopg1_box
    = new G4Box("vetopg1_box", 42.*cm, 37.*cm, 2.*cm);
  G4LogicalVolume* vetopg1_log
    = new G4LogicalVolume(vetopg1_box, ProportionalGas, "vetopg1_log", 0, 0, 0);
  vetopg1_log -> SetVisAttributes(ProportionalGascolour);
  //vetopg1_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(), vetopg1_log, "vetopg1", veto1_log, false, 0);


  
  // --- left & right plate

  G4Box* veto2_box
    = new G4Box("veto2_box", 2.5*cm, 37.5*cm, 45.5*cm);
  G4LogicalVolume* veto2_log
    = new G4LogicalVolume(veto2_box, Brass, "veto2_log", 0, 0, 0); 
  veto2_log -> SetVisAttributes(Brasscolour);
  //veto2_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(-32.5*cm,0.,0.), veto2_log, 
			"veto20", experimentalHall_log, true, 0);
  new G4PVPlacement(0, G4ThreeVector(32.5*cm,0.,0.), veto2_log, 
			"veto21", experimentalHall_log, true, 1);
  G4Box* vetopg2_box
    = new G4Box("vetopg2_box", 2.*cm, 37.*cm, 45.*cm);
  G4LogicalVolume* vetopg2_log
    = new G4LogicalVolume(vetopg2_box, ProportionalGas, "vetopg2_log", 0, 0, 0);
  vetopg2_log -> SetVisAttributes(ProportionalGascolour);
  //vetopg2_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(), vetopg2_log, "vetopg2", veto2_log, false, 0);

  
  // --- top plate

  G4Box* veto3_box
    = new G4Box("veto3_box", 42.5*cm, 2.5*cm, 57.5*cm);
  G4LogicalVolume* veto3_log
    = new G4LogicalVolume(veto3_box, Brass, "veto3_log", 0, 0, 0); 
  veto3_log -> SetVisAttributes(Brasscolour);
  //veto3_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(0.,40.*cm, 0.), veto3_log, 
			"veto3", experimentalHall_log, false, 0);
  G4Box* vetopg3_box
    = new G4Box("vetopg3_box", 42.*cm, 2.*cm, 57.*cm);
  G4LogicalVolume* vetopg3_log
    = new G4LogicalVolume(vetopg3_box, ProportionalGas, "vetopg3_log", 0, 0, 0);
  vetopg3_log -> SetVisAttributes(ProportionalGascolour);
  //vetopg3_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(), vetopg3_log, "vetopg3", veto3_log, false, 0);
  
  
  // --- bottom plate
  
  G4Box* veto4_box
    = new G4Box("veto4_box", 27.5*cm, 2.5*cm, 29.5*cm);
  G4LogicalVolume* veto4_log
    = new G4LogicalVolume(veto4_box, Brass, "veto4_log", 0, 0, 0); 
  veto4_log -> SetVisAttributes(Brasscolour);
  //veto4_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(0., -35*cm, 12.*cm), veto4_log, 
			"veto4", experimentalHall_log, false, 0);
  G4Box* vetopg4_box
    = new G4Box("vetopg4_box", 27.*cm, 2.*cm, 29.*cm);
  G4LogicalVolume* vetopg4_log
    = new G4LogicalVolume(vetopg4_box, ProportionalGas, "vetopg4_log", 0, 0, 0);
  vetopg4_log -> SetVisAttributes(ProportionalGascolour);
  //vetopg4_log-> SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(), vetopg4_log, "vetopg4", veto4_log, false, 0);
  
  


  // ----------------------------------------- a lead box 
  // --- this box has been cut in an inner and outer volume, for setting
  // --- different cuts for secondaries in the volumes

  G4Box* leadshield_box
    = new G4Box("leadshield_box", 30.*cm, 30.*cm, 47.*cm);
  G4LogicalVolume* leadshield_log
    = new G4LogicalVolume(leadshield_box, Pb, "leadshield_log", 0, 0, 0);
  leadshield_log -> SetVisAttributes(Pbcolour);
  //outerleado_log -> SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,0.,10*cm));
  // this is a propagation length limit
  new G4PVPlacement(0, G4ThreeVector(), leadshield_log, "leadshield", experimentalHall_log, false, 0);

 // --------------------------------------------- an iron shielding
  
  G4Box* ironshield_box
    = new G4Box("ironshield_box", 15.15*cm, 15.15*cm, 32.6*cm);
  G4LogicalVolume* ironshield_log
    = new G4LogicalVolume(ironshield_box, Fe, "ironshield_log", 0, 0, 0);
  ironshield_log -> SetVisAttributes(Fecolour);
  new G4PVPlacement(0, G4ThreeVector(), ironshield_log, "ironshield", leadshield_log, false, 0);

  // ----------------------------------------- a copper shielding

  G4Box* coppershield_box
    = new G4Box("coppershield_box", 13.15*cm, 13.15*cm, 30.6*cm);
  G4LogicalVolume* coppershield_log
    = new G4LogicalVolume(coppershield_box, Cu, "coppershield_log", 0, 0, 0);
  coppershield_log -> SetVisAttributes(Cucolour);
  new G4PVPlacement(0, G4ThreeVector(), coppershield_log, "coppershield", 
			ironshield_log, false, 0);

 

  // ----------------------------------------- the nitrogene filled chamber

  G4Box* Nfilledchamber_box
    = new G4Box("Nfilledchamber_box", 10.15*cm, 10.15*cm, 27.6*cm);
  G4LogicalVolume* Nfilledchamber_log
    = new G4LogicalVolume(Nfilledchamber_box, N2Gas, "Nfilledchamber_log", 0, 0, 0);
  Nfilledchamber_log -> SetVisAttributes(N2Gascolour);
  new G4PVPlacement(0, G4ThreeVector(),
			Nfilledchamber_log, "Nfilledchamber",coppershield_log, false, 0);
  
 
  // --- Copper block at the end of chamber

  G4VSolid* copperendblock_box = new G4Box("copperendblock_box", 10.15*cm, 10.15*cm, 2.5*cm);
  G4VSolid* copperendblock_hole = new G4Tubs("copperendblock_hole", 0., 5.1*cm, 3*cm, 0, 2*M_PI);
  G4VSolid* copperendblock_shape = new G4SubtractionSolid("copperendblock_shape", copperendblock_box, copperendblock_hole, 0, G4ThreeVector(0., 0., 0.));

  G4LogicalVolume* copperendblock_log
    = new G4LogicalVolume(copperendblock_shape, Cu, "copperendblock_log", 0, 0, 0);
  copperendblock_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,-3.1*cm), copperendblock_log, "copperendblock", Nfilledchamber_log, false, 0);


  // ------------------------------------------------------------------------------
  // --- Housing
  // ------------------------------------------------------------------------------
  // --- window
  
  G4Tubs* detwindow_tubs = new G4Tubs("detwindow_tubs", 0, 3.95*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detwindow_log 
    = new G4LogicalVolume(detwindow_tubs, Al, "detwindow_log", 0, 0, 0);
  detwindow_log -> SetVisAttributes(Alcolour);
  //  G4RotationMatrix* detwindow_rot = new G4RotationMatrix;
  //  detwindow_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0 , G4ThreeVector(0., 0, 10.725*cm), 
			detwindow_log, 
			"detwindow",Nfilledchamber_log, false, 0);
  // old translation was 0., (10.725-1.100)*cm, 0.
  

  // --- detector housing
  
  G4Tubs* dethousing_tubs = new G4Tubs("dethousing_tubs", 3.75*cm, 3.95*cm, 6.45*cm, 0, 2*M_PI);

  G4LogicalVolume* dethousing_log 
    = new G4LogicalVolume(dethousing_tubs, Al, "dethousing_log", 0, 0, 0);
  dethousing_log -> SetVisAttributes(Alcolour);
  //G4RotationMatrix* dethousing_rot = new G4RotationMatrix;
  //dethousing_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 4.2*cm), 
			dethousing_log, "dethousing", Nfilledchamber_log, false, 0);
  
  // --- an conical ring

  G4Cons* detconering_cons = new G4Cons("detconering_cons", 
					3.95*cm, 4.90*cm, 3.95*cm, 4.05*cm, 0.95*cm, 0, 2*M_PI);

  G4LogicalVolume* detconering_log 
    = new G4LogicalVolume(detconering_cons, Al, "detconering_log", 0, 0, 0);
  detconering_log -> SetVisAttributes(Alcolour);
  //G4RotationMatrix* detconering_rot = new G4RotationMatrix;
  //detconering_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -1.15*cm), 
			detconering_log, "detconering", Nfilledchamber_log, false, 0);
                                                      //old position -0.1*cm

  // --- a fixing ring

  G4Tubs* detfixring_tubs = new G4Tubs("detfixring_tubs", 3.95*cm, 4.90*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detfixring_log 
    = new G4LogicalVolume(detfixring_tubs, Al, "detfixring_log", 0, 0, 0);
  detfixring_log -> SetVisAttributes(Alcolour);
  //G4RotationMatrix* detfixring_rot = new G4RotationMatrix;
  //detfixring_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0, G4ThreeVector(0.,0, -2.175*cm), 
			detfixring_log, "detfixring", Nfilledchamber_log, false, 0);
                                                      //old position -1.65*cm  old half-lenght 0.6*cm
 
 
   // --- a fixing ring

  G4Tubs* dettempendcap_tubs = new G4Tubs("dettempendcap_tubs", 0., 4.90*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* dettempendcap_log 
    = new G4LogicalVolume(dettempendcap_tubs, Al, "dettempendcap_log", 0, 0, 0);
  dettempendcap_log -> SetVisAttributes(Alcolour);
  //G4RotationMatrix* dettempendcap_rot = new G4RotationMatrix;
  //dettempendcap_rot -> rotateX(0.5*M_PI);
  new G4PVPlacement(0, G4ThreeVector(0.,0, -2.325*cm), 
			dettempendcap_log, "dettempendcap", Nfilledchamber_log, false, 0);
 
  // --- the vacuum inside the cover

  G4Tubs* detvacuum_tubs = new G4Tubs("detvacuum_tubs", 
					0., 3.75*cm, 6.45*cm, 0, 2*M_PI);

  G4LogicalVolume* detvacuum_log 
    = new G4LogicalVolume(detvacuum_tubs, Vacuum, "detvacuum_log", 0, 0, 0);
  detvacuum_log -> SetVisAttributes(Vacuumcolour);
  //G4RotationMatrix* detvacuum_rot = new G4RotationMatrix;
  //detvacuum_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 4.2*cm), 
			detvacuum_log, "detvacuum", Nfilledchamber_log, false, 0);

  // ------------------------------------------------------------->>>
  // ------------ Detector and holder - changeable position, size 
 // --- the mantle of the inner aluminium cap

G4Tubs* detinnercap_tubs = new G4Tubs("detinnercap_tubs", 
					crystal_d/2+0.025*cm, crystal_d/2+0.225*cm, 4.*cm, 0, 2*M_PI);

G4LogicalVolume* detinnercap_log 
    = new G4LogicalVolume(detinnercap_tubs, Al, "detinnercap_log", 0, 0, 0);
detinnercap_log -> SetVisAttributes(Alcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 2.45*cm-d_z), 
			detinnercap_log, "detinnercap", detvacuum_log, false, 0);
                                                                                  //position old:1.75*cm
    
 // --- the bottom plate of the inner aluminium cap
  
G4Tubs* detinnercapbottom_tubs = new G4Tubs("detinnercapbottom_tubs", 
					0.6*cm, crystal_d/2+0.225*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* detinnercapbottom_log 
    = new G4LogicalVolume(detinnercapbottom_tubs, Al, "detinnercapbottom_log", 0, 0, 0);
detinnercapbottom_log -> SetVisAttributes(Alcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., -1.6*cm-d_z), 
			detinnercapbottom_log, "detinnercapbottom", detvacuum_log, false, 0);
                                                                                  //position old:(2.70-5.0)*cm
 			
 // --- the vespel cap
  
G4Tubs* detvespelcap_tubs = new G4Tubs("detvespelcap_tubs", 
					0.*cm,  crystal_d/2+0.26*cm, 0.05*cm, 0, 2*M_PI);

G4LogicalVolume* detvespelcap_log 
    = new G4LogicalVolume(detvespelcap_tubs, Vespel, "detvespelcap_log", 0, 0, 0);
detinnercapbottom_log -> SetVisAttributes(Vespelcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 6.5*cm-d_z), 
			detvespelcap_log, "detvespelcap", detvacuum_log, false, 0);
                                                                                  //position old:(2.7+3.1)*cm
    
 

  // ---------------------------------------------------------------------
  // --- the crystal with dead layer
  // ---------------------------------------------------------------------
  
G4Tubs* detdeadlayer_tubs = new G4Tubs("detdeadlayer_tubs", 
					0., crystal_d/2, crystal_l/2, 0, 2*M_PI);

G4LogicalVolume* detdeadlayer_log 
    = new G4LogicalVolume(detdeadlayer_tubs, GeLi, "detdeadlayer_log", 0, 0, 0);
detdeadlayer_log -> SetVisAttributes(GeLicolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 6.45*cm-crystal_l/2-d_z), 
			detdeadlayer_log, "detdeadlayer", detvacuum_log, false, 0);
                                                                                  //position old:(0.875+0.075+1.75)*cm


  // --- crystal with 2 mm dead layer: -----------------------------------
  
G4Tubs* crystal_tubs = new G4Tubs("crystal_tubs", 0.,  crystal_d/2-d_dead,  crystal_l/2-d_dead, 0, 2*M_PI);

G4LogicalVolume* crystal_log 
    = new G4LogicalVolume(crystal_tubs, Ge, "crystal_log", 0, 0, 0);
crystal_log -> SetVisAttributes(Gecolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
			crystal_log, "crystal", detdeadlayer_log, false, 0);
  

G4Tubs* detholeincrystal_tubs = new G4Tubs("detholeincrystal_tubs", 0., 0.4*cm, crystal_l*0.295, 0, 2*M_PI);

G4LogicalVolume* detholeincrystal_log
  = new G4LogicalVolume(detholeincrystal_tubs, Vacuum, "detholeincrystal_log", 0, 0, 0);
detholeincrystal_log -> SetVisAttributes(Vacuumcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., -crystal_l*0.205+d_dead), 
			detholeincrystal_log, "detholeincrystal", crystal_log, false, 0);
                                                                                  //position old:(-1.125+0.075)*cm


G4Tubs* detholeindeadlayer_tubs = new G4Tubs("detholeindeadlayer_tubs", 0., 0.4*cm, d_dead/2, 0, 2*M_PI);

G4LogicalVolume* detholeindeadlayer_log 
    = new G4LogicalVolume(detholeindeadlayer_tubs, Vacuum, "detholeindeadlayer_log", 0, 0, 0);
detholeindeadlayer_log -> SetVisAttributes(Vacuumcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., -crystal_l/2+d_dead/2), 
			detholeindeadlayer_log, "detholeindeadlayer", detdeadlayer_log, false, 0);
                                                                                  //position old:(-3.025+0.075)*cm
 
  // --- a conical ring of dead material
  
G4Cons* detdeadcone_cons = new G4Cons("detdeadcone_cons", 
					crystal_d/2-0.0001*cm-d_dead, crystal_d/2-d_dead, crystal_d/2-0.4*cm-d_dead, crystal_d/2-d_dead, 0.2*cm, 0, 2*M_PI);
G4LogicalVolume* detdeadcone_log 
    = new G4LogicalVolume(detdeadcone_cons, GeLi, "detdeadcone_log", 0, 0, 0);
detdeadcone_log -> SetVisAttributes(GeLicolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., crystal_l/2-d_dead-0.2*cm), 
			detdeadcone_log, "detdeadcone", crystal_log, false, 0);
                                                                                  //position old:(2.725-0.075)*cm
  

  // ---------------------------------------------------------------------------------------
  // ---  The samples
  // ---------------------------------------------------------------------------------------
 

  // --- a plastic cap between detector and sample (part of the standard box)
  
  /*
  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 
					0.*cm, (3.81+0.2)*cm, 0.05*cm, 0, 2*M_PI);
  // die 0.2 muessen wieder weg spaeter!!! Nur fuer asymmetrische NPL-Probe!
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detinnercapbottom_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 10.85*cm), 
			detplasticcap_log, 
			"detplasticcap", Nfilledchamber_log, false, 0);
  */
 G4VSolid* sample_shape=0;
 if (G4Material::GetMaterial(sample_mat_name))
   sample_mat = G4Material::GetMaterial(sample_mat_name);
 else 
   {
     MGLog(error) << "The material called " << sample_mat_name << " does not exist" << endlog;
     MGLog(error) << "Please check and run MaGe again!" << endlog;
     G4Exception("GEDarioDetector::ConstructDetector()","err001",FatalException,"Exit MaGe");
   }
 
  /*
  G4VSolid* marinelli_body   = new G4Box("marinelli_body", marinellibox_width/2, marinellibox_height/2, marinellibox_thickness/2);
  G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, marinellitube_outrad, marinellitube_length/2, 0, 2*M_PI);
  G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-3.2*cm));
  
  
  G4VSolid* marinelli_body   = new G4Box("marinelli_body", 92.5, 88, 87.5);
  G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, 49.5, 55.5, 0, 2*M_PI);
  G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-3.2*cm));
   
  G4VSolid* marinelli_body   = new G4Box("marinelli_body", 97.5, 97.5, 119);
  G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, 45, 59.5, 0, 2*M_PI);
  G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-5.951*cm));
  */

 if (sample_geom=="box")
   {
      sample_shape = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);
        }
 else if (sample_geom=="tube")
    {
      sample_shape = new G4Tubs("sample_tube", 
				tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
    }
  else if (sample_geom=="sphere")
    {
      //---- for point-like sources

      sample_shape = new G4Sphere("sample_shpere",
				  0., sphere_rad, 0.,twopi, 0.,twopi);
    }
  else if (sample_geom=="sbox")
    {
  // --- standard cylindrical box (cover side facing detector) - 0.1 cm (!) from detector window

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.35*cm), detplasticcap_log, "detplasticcap",Nfilledchamber_log, false, 0);

  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap1_log 
    = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+12.275*cm), detplasticcap1_log, "detplasticcap1", Nfilledchamber_log, false, 0);

  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticshell_log 
    = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+10.8*cm), detplasticshell_log, "detplasticshell", Nfilledchamber_log, false, 0);

  // cylinder sample inside the standard box (default - filling the volume)
  sample_shape = new G4Tubs("sample_tube", 
				tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
    }
  else if (sample_geom=="twobox")
    {
      //---for point like sources, two boxes away from detector (strong sources)
        // --- standard cylindrical box (cover side facing detector) - 0.1cm from detector window

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 10.95*cm), detplasticcap_log, "detplasticcap", Nfilledchamber_log, false, 0);

  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap1_log 
    = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 13.875*cm), detplasticcap1_log, "detplasticcap1", Nfilledchamber_log, false, 0);
 
  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticshell_log 
    = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 12.4*cm), detplasticshell_log, "detplasticshell", Nfilledchamber_log, false, 0);

  // --- one more standard box between detector and sample  - for strong sources measurement
    
  new G4PVPlacement(0, G4ThreeVector(0., 0., 14.05*cm), 
			detplasticcap_log, "detplasticcap2", Nfilledchamber_log, false, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 15.5*cm), 
			detplasticshell_log, "detplasticshell2", Nfilledchamber_log, false, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 16.975*cm), 
			detplasticcap1_log, "detplasticcap3", Nfilledchamber_log, false, 0);

  sample_shape = new G4Sphere("sample_shpere", 0., sphere_rad, 0.,twopi, 0.,twopi);
    }
 else if (sample_geom=="liquid")
    {
  // --- standard cylindrical box (cover side facing detector) - 0.1cm from detector window _____with liquid sample_____

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.35*cm), detplasticcap_log, "detplasticcap", Nfilledchamber_log, false, 0);

  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap1_log 
    = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+12.275*cm), detplasticcap1_log, "detplasticcap1", Nfilledchamber_log, false, 0);

  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticshell_log 
    = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+10.8*cm), detplasticshell_log, "detplasticshell", Nfilledchamber_log, false, 0);

  // liquid filling the standard box up to level of "boxheight"
  
  G4VSolid* sample_body   = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
  G4VSolid* sample_hole   = new G4Box("sample_hole", tube_outrad+0.1*cm, tube_outrad, tube_length/2+0.1*cm);
  sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, G4ThreeVector(0., 2*tube_outrad-box_height,0.));
    }
 else if (sample_geom=="marinelli")
   {
     // "marinelli" shape (box with a hole)
      G4VSolid* sample_body = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);
      G4VSolid* sample_hole = new G4Tubs("sample_tube", 0., tube_outrad, tube_length, 0, tube_angle);
      sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, G4ThreeVector(0., tube_innrad, -box_thickness/2));
   }
 else if (sample_geom=="custom")
    {
      // here define any other sample geometry

      /* //The Iglidur sample on a Cu holder (6 sticks, only 1 simulated)
      sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);

      G4Tubs* CuStick_tubs = new G4Tubs("CuStick_tubs", 0., 0.5*cm, tube_length/2, 0, 2*M_PI);
      G4LogicalVolume* CuStick_log = new G4LogicalVolume(CuStick_tubs, Cu, "CuStick_log", 0, 0, 0);
       CuStick_log -> SetVisAttributes(Cucolour);
      new G4PVPlacement(0, G4ThreeVector(0., s_y+0.2*cm, s_z), CuStick_log, "CuStick", Nfilledchamber_log, false, 0);
      */
      /*
      // The NPL bottle with definable liquid level - tube_leght
G4Tubs* NPLbottle_tubs = new G4Tubs("NPLbottle_tubs", 0., 3.62*cm, 7*cm, 0, 2*M_PI);

G4LogicalVolume* NPLbottle_log = new G4LogicalVolume(NPLbottle_tubs, PE, "NPLbottle_log", 0, 0, 0);
NPLbottle_log -> SetVisAttributes(Acryliccolour);

//rotate 90deg up :
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);

//position : touching detector window; on bottom of the Chamber
new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0., -3.15*cm, 14.42*cm), NPLbottle_log, "NPLbottle", Nfilledchamber_log, false, 0);



//contents :
 G4Tubs* NPLbottleN2_tubs = new G4Tubs("NPLbottleN2_tubs", 0., 3.5*cm, 6.6*cm-tube_length/2, 0, 2*M_PI);    //filled with N2 of the chamber (?)

G4LogicalVolume* NPLbottleN2_log = new G4LogicalVolume(NPLbottleN2_tubs, N2Gas, "NPLbottleN2_log", 0, 0, 0);
NPLbottleN2_log -> SetVisAttributes(N2Gascolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2), NPLbottleN2_log, "NPLbottleN2", NPLbottle_log, false, 0);


//needs to change macro confine volume ---> NPLsample
G4Tubs* NPLsample_tubs = new G4Tubs("NPLsample_tubs", 0., 3.5*cm, tube_length/2, 0, twopi);

G4LogicalVolume* NPLsample_log = new G4LogicalVolume(NPLsample_tubs, sample_mat, "NPLsample_log", 0, 0, 0);
NPLsample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2-6.6*cm), NPLsample_log, "NPLsample", NPLbottle_log, false, 0);


//puts small ball away, to avoid errors
sample_shape = new G4Sphere("sample_shpere", 0., sphere_rad, 0.,twopi, 0.,twopi);
 s_z = 15.*cm;
      */

      // The NPL small teflon bottle with definable height from chamber bottom - s_z and liquid level - tube_leght
G4Tubs* NPLbottle_tubs = new G4Tubs("NPLbottle_tubs", 0., 2.225*cm, 4.5*cm, 0, 2*M_PI);

G4LogicalVolume* NPLbottle_log = new G4LogicalVolume(NPLbottle_tubs, Teflon, "NPLbottle_log", 0, 0, 0);
NPLbottle_log -> SetVisAttributes(Acryliccolour);

//rotate 90deg up :
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);

//position : touching detector window; on bottom of the Chamber
new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0., s_y-5.65*cm, 13.025*cm), NPLbottle_log, "NPLbottle", Nfilledchamber_log, false, 0);



//contents :
 G4Tubs* NPLbottleN2_tubs = new G4Tubs("NPLbottleN2_tubs", 0., 2.175*cm, 4.25*cm-tube_length/2, 0, 2*M_PI);    //filled with N2 of the chamber (?)

G4LogicalVolume* NPLbottleN2_log = new G4LogicalVolume(NPLbottleN2_tubs, N2Gas, "NPLbottleN2_log", 0, 0, 0);
NPLbottleN2_log -> SetVisAttributes(N2Gascolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2-0.05*cm), NPLbottleN2_log, "NPLbottleN2", NPLbottle_log, false, 0);


//needs to change macro confine volume ---> NPLsample
G4Tubs* NPLsample_tubs = new G4Tubs("NPLsample_tubs", 0.,  2.175*cm, tube_length/2, 0, twopi);

G4LogicalVolume* NPLsample_log = new G4LogicalVolume(NPLsample_tubs, sample_mat, "NPLsample_log", 0, 0, 0);
NPLsample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2-4.3*cm), NPLsample_log, "NPLsample", NPLbottle_log, false, 0);


//puts small ball away, to avoid errors
sample_shape = new G4Sphere("sample_shpere", 0., sphere_rad, 0.,twopi, 0.,twopi);
 s_z = 15.*cm;
    }
 else   
   {
      MGLog(error) << "The shape is not defined " << endlog;
      MGLog(fatal) << endlog;
      G4Exception("GEDarioDetector::ConstructDetector()","err002",FatalException,"Check the shape type");
    }
 //G4LogicalVolume* sample_log = new G4LogicalVolume(marinelli_sample, sample_mat, "sample_log", 0, 0, 0);
  G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
  sample_log -> SetVisAttributes(Sample_Colour);

  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+10.8*cm), 
			sample_log, "sample", Nfilledchamber_log, false, 0);




  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  
  G4SDManager* SDman   =  G4SDManager::GetSDMpointer();
  G4String CrystalSDname      = "/mydet/gerda/gecrystal";
  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);
  SDman->AddNewDetector(CrystalSD);
  crystal_log-> SetSensitiveDetector(CrystalSD);
  
  SetDetectorLogical(experimentalHall_log);
  SetDetectorName("ExperimentalHall");
 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
