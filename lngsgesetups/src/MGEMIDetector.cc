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
// $Id: MGEMIDetector.cc,v 1.1 2010-03-04 16:39:00 matthiasl Exp $ 
//      
// CLASS IMPLEMENTATION:  MGEMIDetectorMessenger.cc
//	to be used for GsOr, GePV and GePaolo (default values GsOr)
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthias Laubenstein
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * REVISION:
 *
 * 03-08-2010, Added in MaGe, Luciano
 *
 */
//---------------------------------------------------------------------------//

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
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

#include "lngsgesetups/MGEMIDetector.hh"
#include "lngsgesetups/MGEMIDetectorMessenger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MGEMIDetector::MGEMIDetector() : MGGeometryDetector("")
{
  //set defaults
  //box height is also liquid level in the standard box
  box_height=1.1*cm;
  box_width=2.*cm;
  box_thickness=2.*cm;

  smarinelli_depth1=16.4*cm;
  smarinelli_depth2=10.15*cm;
  smarinelli_depth3=7.8*cm;

  sphere_rad=0.00001*cm;

  s_x=0.*cm;
  s_y=0.*cm;
  s_z=0.*cm;

  //crystal parameters
  d_z=0.45*cm;
  d_dead=0.15*cm;
  // f_size=1.;
  hole_d=1.11*cm;
  hole_l=9.23*cm;
  crystal_d=7.19*cm;
  crystal_l=10.41*cm;
  theMessenger = new MGEMIDetectorMessenger(this);
}

//---------------------------------------------------------------------------//

MGEMIDetector::~MGEMIDetector()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void MGEMIDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  //  G4Material* Brass = G4Material::GetMaterial("Brass");
  //  G4Material* ProportionalGas = G4Material::GetMaterial("ProportionalGas");
  G4Material* N2Gas = G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  G4Material* Pb = G4Material::GetMaterial("MetalLead");
  //  G4Material* Fe = G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  G4Material* Polypropylene = G4Material::GetMaterial("Polypropylene");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  G4Material* PE = G4Material::GetMaterial("PE");
  G4Material* PS = G4Material::GetMaterial("PS");
  //  G4Material* Teflon = G4Material::GetMaterial("Teflon");
  G4Material* Air = G4Material::GetMaterial("Air");
  G4Material* PET = G4Material::GetMaterial("PET");
  //  G4Material* epoxy = G4Material::GetMaterial("Epoxy");
  //  G4Material* CU5 = G4Material::GetMaterial("CU5");
  //  G4Material* PPO = G4Material::GetMaterial("Polypropylene");
  //  G4Material* dmp = G4Material::GetMaterial("DMP");

  // Visualization attributes
  /*  G4VisAttributes * red= new G4VisAttributes( G4Colour(255/255. ,0/255. ,12/255. ));
      G4VisAttributes * Brasscolour = red;
  */
  G4VisAttributes * Sample_Colour = new G4VisAttributes(  G4Colour(255/255. ,0/255. ,12/255. ));
  Sample_Colour -> SetForceSolid(true);

  G4VisAttributes * skyblue = new G4VisAttributes( G4Colour(175/255. ,164/255. ,255/255. ));
  G4VisAttributes * Vacuumcolour = skyblue;

  G4VisAttributes * orange= new G4VisAttributes( G4Colour(241/255. ,224/255. ,0/255. ));
  G4VisAttributes * Acryliccolour = orange;

  G4VisAttributes * darkviolet = new G4VisAttributes( G4Colour(64/255. ,0/255. ,64/255. ));
  G4VisAttributes * Pbcolour = darkviolet;

  G4VisAttributes * grass= new G4VisAttributes( G4Colour(0/255. ,239/255. ,0/255. ));
  G4VisAttributes * Cucolour = grass;

  G4VisAttributes * violet= new G4VisAttributes( G4Colour(224/255. ,0/255. ,255/255. ));
  G4VisAttributes * Gecolour = violet;

  G4VisAttributes * purple= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));
  G4VisAttributes * GeLicolour = purple;

  G4VisAttributes * redbrown = new G4VisAttributes( G4Colour(123/255. ,45/255. ,65/255. ));
  G4VisAttributes * PPcolour = redbrown;

  G4VisAttributes * blue = new G4VisAttributes( G4Colour(0/255. ,0/255. ,255/255. ));
  G4VisAttributes * N2Gascolour = blue;

  G4VisAttributes * grayva = new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));
  G4VisAttributes * Alcolour = grayva;
  // Alcolour -> SetForceSolid(true);

  G4VisAttributes * yellowgray= new G4VisAttributes( G4Colour(214/255. ,214/255. ,207/255. ));
  G4VisAttributes * PEcolour = yellowgray;

  G4VisAttributes * indigo= new G4VisAttributes( G4Colour(0/255. ,0/255. ,190/255. ));
  G4VisAttributes * PETcolour = indigo;


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


  // ----------------------------------------- a lead box 
  // --- this box has been cut in an inner and outer volume, for setting
  // --- different cuts for secondaries in the volumes

  G4Box* leadshield_box
    = new G4Box("leadshield_box", 32.5*cm, 32.5*cm, 50.0*cm);
  G4LogicalVolume* leadshield_log
    = new G4LogicalVolume(leadshield_box, Pb, "leadshield_log", 0, 0, 0);
  leadshield_log -> SetVisAttributes(Pbcolour);
  new G4PVPlacement(0, G4ThreeVector(), leadshield_log, "leadshield", experimentalHall_log, false, 0);

  // ----------------------------------------- a copper shielding

  G4Box* coppershield_box
    = new G4Box("coppershield_box", 12.5*cm, 12.5*cm, 25.0*cm);
  G4LogicalVolume* coppershield_log
    = new G4LogicalVolume(coppershield_box, Cu, "coppershield_log", 0, 0, 0);
  coppershield_log -> SetVisAttributes(Cucolour);
  new G4PVPlacement(0, G4ThreeVector(), coppershield_log, "coppershield", 
		    leadshield_log, false, 0);

  // ----------------------------------------- the nitrogen filled chamber

  G4Box* Nfilledchamber_box
    = new G4Box("Nfilledchamber_box", 8.0*cm, 8.0*cm, 11.95*cm);
  G4LogicalVolume* Nfilledchamber_log
    = new G4LogicalVolume(Nfilledchamber_box, N2Gas, "Nfilledchamber_log", 0, 0, 0);
  Nfilledchamber_log -> SetVisAttributes(N2Gascolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 11.95),
		    Nfilledchamber_log, "Nfilledchamber",coppershield_log, false, 0);
  
  /*  // ----------------------------------------- the nitrogen filled chamber

  G4Tubs* Nfilledchamber_tubs
  = new G4Tubs("Nfilledchamber_tubs", 0., 5.*cm, 11.6*cm, 0, twopi);
  G4LogicalVolume* Nfilledchamber_log
  = new G4LogicalVolume(Nfilledchamber_tubs, N2Gas, "Nfilledchamber_log", 0, 0, 0);
  Nfilledchamber_log -> SetVisAttributes(N2Gascolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 11.6),
  Nfilledchamber_log, "Nfilledchamber",coppershield_log, false, 0);
  */  
  // ------------------------------------------------------------------------------
  // --- Cryostat
  // ------------------------------------------------------------------------------
  // --- endcap window (GeMi D 94.0 mm)
  
  G4Tubs* detwindow_tubs = new G4Tubs("detwindow_tubs", 0, 4.6*cm, 0.025*cm, 0, twopi);

  G4LogicalVolume* detwindow_log 
    = new G4LogicalVolume(detwindow_tubs, Cu, "detwindow_log", 0, 0, 0);
  detwindow_log -> SetVisAttributes(Cucolour);
  //  G4RotationMatrix* detwindow_rot = new G4RotationMatrix;
  //  detwindow_rot -> rotateX(0.5*M_PI);

  new G4PVPlacement(0 , G4ThreeVector(0., 0, 2.525*cm), 
		    detwindow_log, 
		    "detwindow",Nfilledchamber_log, false, 0);
  // reference point of top of endcap: 2.55 cm

  // --- detector endcap
  
  G4Tubs* dethousing_tubs = new G4Tubs("dethousing_tubs", 4.6*cm, 4.7*cm, 7.25*cm, 0, twopi);

  G4LogicalVolume* dethousing_log 
    = new G4LogicalVolume(dethousing_tubs, Cu, "dethousing_log", 0, 0, 0);
  dethousing_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, -4.7*cm), 
		    dethousing_log, "dethousing", Nfilledchamber_log, false, 0);
  
 
  // --- the vacuum inside the endcap

  G4VSolid* detvacuum_tubs = new G4Tubs("detvacuum_tubs", 
					0., 4.6*cm, 7.225*cm, 0, twopi);

  G4LogicalVolume* detvacuum_log 
    = new G4LogicalVolume(detvacuum_tubs, Vacuum, "detvacuum_log", 0, 0, 0);
  detvacuum_log -> SetVisAttributes(Vacuumcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -4.725*cm), 
		    detvacuum_log, "detvacuum", Nfilledchamber_log, false, 0);

  // --- the mylar (PET) cladding inside endcap, introduce new variable dethousingclad_tubs
  
  G4Tubs* dethousingclad_tubs = new G4Tubs("dethousingclad_tubs", 
					   4.575*cm, 4.6*cm, 7.025*cm, 0, twopi);

  G4LogicalVolume* dethousingclad_log 
    = new G4LogicalVolume(dethousingclad_tubs, PET, "dethousingclad_log", 0, 0, 0);
  dethousingclad_log -> SetVisAttributes(PETcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -0.2*cm), 
		    dethousingclad_log, "dethousingclad", detvacuum_log, false, 0);
 			
  // ------------------------------------------------------------->>>
  // ------------ Detector and holder - changeable position, size 
  // --- the mantle of the copper holder

  G4Tubs* detinnercap_tubs = new G4Tubs("detinnercap_tubs", 
					crystal_d/2+0.005*cm, crystal_d/2+0.055*cm, 6.31*cm, 0, twopi);

  G4LogicalVolume* detinnercap_log 
    = new G4LogicalVolume(detinnercap_tubs, Cu, "detinnercap_log", 0, 0, 0);
  detinnercap_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.915*cm-d_z), 
		    detinnercap_log, "detinnercap", detvacuum_log, false, 0);
    
  // --- the bottom of the copper holder
  
  G4Tubs* detinnercapbottom_tubs = new G4Tubs("detinnercapbottom_tubs", 
					      0.8*cm, crystal_d/2+0.055*cm, 0.16*cm, 0, 2*M_PI);

  G4LogicalVolume* detinnercapbottom_log 
    = new G4LogicalVolume(detinnercapbottom_tubs, Cu, "detinnercapbottom_log", 0, 0, 0);
  detinnercapbottom_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -5.555*cm-d_z), 
		    detinnercapbottom_log, "detinnercapbottom", detvacuum_log, false, 0);
 			
  // --- the rings of the copper holder, introduce new variable detinnercapring1_tubs
  
  G4Tubs* detinnercapring_tubs = new G4Tubs("detinnercapring_tubs", 
					    crystal_d/2+0.055*cm, crystal_d/2+0.255*cm, 0.3*cm, 0, twopi);

  G4LogicalVolume* detinnercapring_log 
    = new G4LogicalVolume(detinnercapring_tubs, Cu, "detinnercapring_log", 0, 0, 0);
  detinnercapring_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 5.3*cm-d_z), 
		    detinnercapring_log, "detinnercapring", detvacuum_log, false, 0);
 			
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.5*cm-d_z), 
		    detinnercapring_log, "detinnercapring", detvacuum_log, false, 1);
 			
  // --- the mylar (PET) insulation on the crystal, introduce new variable detinnercaptop_tubs
  
  G4Tubs* detinnercaptop_tubs = new G4Tubs("detinnercaptop_tubs", 
					   0., crystal_d/2+0.005*cm, 0.00065*cm, 0, twopi);

  G4LogicalVolume* detinnercaptop_log 
    = new G4LogicalVolume(detinnercaptop_tubs, PET, "detinnercaptop_log", 0, 0, 0);
  detinnercaptop_log -> SetVisAttributes(PETcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 7.22565*cm-d_z), 
		    detinnercaptop_log, "detinnercaptop", detvacuum_log, false, 0);
 			
  // --- Al foil as IR shield, introduce new variable detinnercapplate_tubs
  
  G4Tubs* detinnercapplate_tubs = new G4Tubs("detinnercapplate_tubs", 
					     0., crystal_d/2.+0.005*cm, 0.00125*cm, 0, twopi);

  G4LogicalVolume* detinnercapplate_log 
    = new G4LogicalVolume(detinnercapplate_tubs, Al, "detinnercapplate_log", 0, 0, 0);
  detinnercapplate_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 7.22755*cm-d_z), 
		    detinnercapplate_log, "detinnercapplate", detvacuum_log, false, 0);
 			
  // --- the PE retainer, introduce new variable detpecap_tubs, side

  G4Tubs* detpecap_tubs = new G4Tubs("detpecap_tubs", 
				     crystal_d/2+0.055*cm,  crystal_d/2+0.155*cm, 0.4*cm, 0, twopi);

  G4LogicalVolume* detpecap_log 
    = new G4LogicalVolume(detpecap_tubs, PE, "detpecap_log", 0, 0, 0);
  detpecap_log -> SetVisAttributes(PEcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 6.7788*cm-d_z), 
		    detpecap_log, "detpecap", detvacuum_log, false, 0);
    
  // --- the PE retainer, top, introduce new variable detpecaptop_tubs

  G4Tubs* detpecaptop_tubs = new G4Tubs("detpecaptop_tubs", 
					crystal_d/2.-0.145*cm,  crystal_d/2.+0.155*cm, 0.05*cm, 0, twopi);
  G4LogicalVolume* detpecaptop_log 
    = new G4LogicalVolume(detpecaptop_tubs, PE, "detpecaptop_log", 0, 0, 0);
  detpecaptop_log -> SetVisAttributes(PEcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 7.2788*cm-d_z), 
		    detpecaptop_log, "detpecaptop", detvacuum_log, false, 0);
  

  // ---------------------------------------------------------------------
  // --- the crystal with dead layer
  // ---------------------------------------------------------------------
  
  G4Tubs* detdeadlayer_tubs = new G4Tubs("detdeadlayer_tubs", 
					 0., crystal_d/2, crystal_l/2, 0, twopi);

  G4LogicalVolume* detdeadlayer_log 
    = new G4LogicalVolume(detdeadlayer_tubs, GeLi, "detdeadlayer_log", 0, 0, 0);
  detdeadlayer_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 7.225*cm-crystal_l/2-d_z), 
		    detdeadlayer_log, "detdeadlayer", detvacuum_log, false, 0);


  // --- crystal without dead layer: -----------------------------------
  
  G4Tubs* crystal_tubs = new G4Tubs("crystal_tubs", 0.,  crystal_d/2-d_dead, crystal_l/2-d_dead/2, 0, twopi);

  G4LogicalVolume* crystal_log 
    = new G4LogicalVolume(crystal_tubs, Ge, "crystal_log", 0, 0, 0);
  crystal_log -> SetVisAttributes(Gecolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -d_dead/2), 
		    crystal_log, "crystal", detdeadlayer_log, false, 0);
  

  // --- hole in crystal -----------------------------------
  
  G4Tubs* detholeincrystal_tubs = new G4Tubs("detholeincrystal_tubs", 0., hole_d/2, 
					     hole_l/2-hole_d/4, 0, twopi);
  G4Sphere* detholeincrystal_bullet = new G4Sphere("detholeincrystal_bullet", 0., hole_d/2, 0, 
						   twopi, 0, twopi/4);

  G4VSolid* dethole_shape=0;
  dethole_shape   = new G4UnionSolid("dethole_shape", detholeincrystal_tubs, 
				     detholeincrystal_bullet, 0, G4ThreeVector (0.,0.,hole_l/2-hole_d/4));
  G4LogicalVolume* dethole_log
    = new G4LogicalVolume(dethole_shape, Vacuum, "dethole_log", 0, 0, 0);
  dethole_log -> SetVisAttributes(Vacuumcolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, -(crystal_l/2-d_dead/2)+hole_l/2), dethole_log,
		    "dethole", crystal_log, false, 0);

  /* --- hole in dead layer -----------------------------------
  
  G4Tubs* detholeindeadlayer_tubs = new G4Tubs("detholeindeadlayer_tubs", 0., 0.55*cm, d_dead/2, 0, twopi);

  G4LogicalVolume* detholeindeadlayer_log 
  = new G4LogicalVolume(detholeindeadlayer_tubs, Vacuum, "detholeindeadlayer_log", 0, 0, 0);
  detholeindeadlayer_log -> SetVisAttributes(Vacuumcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -crystal_l/2+d_dead/2), 
  detholeindeadlayer_log, "detholeindeadlayer", detdeadlayer_log, false, 0);
  */ 

  // --- a conical ring of dead material (bulleting of crystal)
  
  //  G4double x = 0;
  G4double y = 0;

  y = hole_d/2*(sqrt(2.)-1)/cos(twopi/8);  

  G4Cons* detdeadcone_cons = new G4Cons("detdeadcone_cons", 
					crystal_d/2-0.0001*cm-d_dead, crystal_d/2-d_dead, 
					crystal_d/2-y-d_dead, crystal_d/2-d_dead, y/2, 0, twopi);
  G4LogicalVolume* detdeadcone_log 
    = new G4LogicalVolume(detdeadcone_cons, GeLi, "detdeadcone_log", 0, 0, 0);
  detdeadcone_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., crystal_l/2-d_dead-y/2), 
		    detdeadcone_log, "detdeadcone", crystal_log, false, 0);
  
  // ---------------------------------------------------------------------------------------
  // ---  The samples
  // ---------------------------------------------------------------------------------------
 

  // --- a plastic cap between detector and sample (part of the standard box)
  
  /*
    G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 
    0.*cm, (3.81+0.2)*cm, 0.05*cm, 0, twopi);
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
      G4Exception("MGEMIDetector::ConstructDetector()","err001",FatalException,"Exit MaGe");
    }
 
  /*
    G4VSolid* marinelli_body   = new G4Box("marinelli_body", 
    marinellibox_width/2, marinellibox_height/2, marinellibox_thickness/2);
    G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, marinellitube_outrad, 
    marinellitube_length/2, 0, 2*M_PI);
    G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", 
    marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-3.2*cm));
  
  
    G4VSolid* marinelli_body   = new G4Box("marinelli_body", 92.5, 88, 87.5);
    G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, 49.5, 55.5, 0, 2*M_PI);
    G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", 
    marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-3.2*cm));
   
    G4VSolid* marinelli_body   = new G4Box("marinelli_body", 97.5, 97.5, 119);
    G4VSolid* marinelli_hole   = new G4Tubs("marinelli_hole", 0, 45, 59.5, 0, 2*M_PI);
    G4VSolid* marinelli_sample = new G4SubtractionSolid("marinelli_sample", 
    marinelli_body, marinelli_hole, 0, G4ThreeVector (0.,0.,-5.951*cm));
  */

  //	GAMA small Marinelli (138G)
  if (sample_geom=="smarinelli")
    {

      // ----------------------------------------- Cu block in nitrogen filled chamber

      G4Box* Cublock_box
	= new G4Box("Cublock_box", 8.0*cm, 8.0*cm, 3.6*cm);

      // ----------------------------------------- drilled hole in Cu block in nitrogen filled chamber

      G4Tubs* drhole_tubs
	= new G4Tubs("drhole_tubs", 0., 5.0*cm, 3.6*cm, 0, twopi);

      // ----------------------------------------- combined geometry for Cu shield in nitrogen filled chamber

      G4VSolid* Cu_shield=0;
      Cu_shield   = new G4SubtractionSolid("Cu_shield", Cublock_box, 
					   drhole_tubs, 0, G4ThreeVector (0.,0.,0.));
      G4LogicalVolume* Cu_shield_log
	= new G4LogicalVolume(Cu_shield, Cu, "Cu_shield_log", 0, 0, 0);
      Cu_shield_log -> SetVisAttributes(Cucolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, -8.35*cm), Cu_shield_log,
			"Cu_shield", Nfilledchamber_log, false, 0);
  
      // bottom part with hole

      G4VSolid* marinellis1l_tubs = new G4Tubs("marinellis1l_tubs", 4.95*cm, 7.35*cm, 0.1*cm, 0, twopi);
      G4LogicalVolume* marinellis1l_tubs_log 
	= new G4LogicalVolume(marinellis1l_tubs, Polypropylene, "marinellis1l_tubs_log", 0, 0, 0);
      marinellis1l_tubs_log -> SetVisAttributes(PPcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, -4.65*cm), marinellis1l_tubs_log, 
			"marinellis1l_tubs", Nfilledchamber_log, false, 0);

      // lower part outside

      G4VSolid* marinellis1o_tubs = new G4Tubs("marinellis1o_tubs", 7.25*cm, 7.35*cm, 3.9*cm, 0, twopi);
      G4LogicalVolume* marinellis1o_tubs_log 
	= new G4LogicalVolume(marinellis1o_tubs, Polypropylene, "marinellis1o_tubs_log", 0, 0, 0);
      marinellis1o_tubs_log -> SetVisAttributes(PPcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, -0.65*cm), marinellis1o_tubs_log, "marinellis1o_tubs", 
			Nfilledchamber_log, false, 0);

      // upper part outside 

      G4VSolid* marinellis2o_cons = new G4Cons("marinellis2o_cons", 
					       7.3*cm, 7.4*cm, 7.675*cm, 7.775*cm, 4.3*cm, 0, twopi);
      G4LogicalVolume* marinellis2o_cons_log 
	= new G4LogicalVolume(marinellis2o_cons, Polypropylene, "marinellis2o_cons_log", 0, 0, 0);
      marinellis2o_cons_log -> SetVisAttributes(PPcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, 7.55*cm), marinellis2o_cons_log, "marinellis2o_cons", 
			Nfilledchamber_log, false, 0);

      // inner cone

      G4VSolid* marinellis1i_cons = new G4Cons("marinellis1i_cons", 
					       4.95*cm, 5.05*cm, 4.8*cm, 4.9*cm, 5.075*cm, 0, twopi);
      G4LogicalVolume* marinellis1i_cons_log 
	= new G4LogicalVolume(marinellis1i_cons, Polypropylene, "marinellis1i_cons_log", 0, 0, 0);
      marinellis1i_cons_log -> SetVisAttributes(PPcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, 0.525*cm), marinellis1i_cons_log, "marinellis1i_cons", 
			Nfilledchamber_log, false, 0);

      // lower volume 630.28 cm^3 (straight outside walls with Ri 7.15 cm), middle volume 
      //(conical inside AND outside) 215.49 cm^3, and upper volume (conical outside) 1097.14 cm^3. 

      // top lid of inner cone

      G4VSolid* marinellis2l_tubs = new G4Tubs("marinellis2l_tubs", 0*cm, 4.8*cm, 0.075*cm, 0, twopi);
      G4LogicalVolume* marinellis2l_tubs_log 
	= new G4LogicalVolume(marinellis2l_tubs, PE, "marinellis2l_tubs_log", 0, 0, 0);
      marinellis2l_tubs_log -> SetVisAttributes(PEcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, 5.525*cm), marinellis2l_tubs_log, "marinellis2l_tubs", 
			Nfilledchamber_log, false, 0);

      // top lid of whole Marinelli

      G4VSolid* marinellis3l_tubs = new G4Tubs("marinellis3l_tubs", 0*cm, 7.9*cm, 0.05*cm, 0, twopi);
      G4LogicalVolume* marinellis3l_tubs_log 
	= new G4LogicalVolume(marinellis3l_tubs, PE, "marinellis3l_tubs_log", 0, 0, 0);
      marinellis3l_tubs_log -> SetVisAttributes(PEcolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, 11.9*cm), marinellis3l_tubs_log, "marinellis3l_tubs", 
			Nfilledchamber_log, false, 0);

      // total (real): 1.94291 l

      //	sample inside GAMA small Marinelli

      G4double fillingheight = 0;
      G4double sampleheight = 0;
      sample_shape = 0;
      fillingheight = 16.4*cm-smarinelli_height;

      if ((fillingheight>=0*cm) && (fillingheight<=7.8*cm))
	{
	  sample_shape = new G4Cons("sample_shape", 
				    5.05*cm, 
				    7.25*cm,
				    5.05*cm-(0.15/10.15*fillingheight), 
				    7.25*cm, 
				    fillingheight/2., 0, twopi);
	  sampleheight=fillingheight/2.;
	  G4LogicalVolume* sample_log
	    = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
	  sample_log -> SetVisAttributes(Sample_Colour);
	  new G4PVPlacement(0, G4ThreeVector(0, 0, (-4.55*cm+sampleheight)), sample_log,
			    "sample", Nfilledchamber_log, false, 0);
	}
      else if ((fillingheight>7.8*cm) && (fillingheight<=10.15*cm))
	{
	  G4VSolid* smarinellisample1_cons = new G4Cons("smarinellisample1_cons", 
							5.05*cm, 
							7.25*cm, 
							5.05*cm-(0.15/10.15*7.8*cm), 
							7.25*cm, 
							3.9*cm, 0, twopi);

	  G4VSolid* smarinellisample2_cons = new G4Cons("smarinellisample2_cons", 
							5.05*cm-(0.15/10.15*7.8*cm), 
							7.3*cm, 
							5.05*cm-(0.15/10.15*fillingheight), 
							7.3*cm+((0.375/8.6)*(fillingheight-7.8*cm)), 
							fillingheight/2.-3.9*cm, 0, twopi);

	  sample_shape   = new G4UnionSolid("sample_shape", smarinellisample1_cons, 
					    smarinellisample2_cons, 0, G4ThreeVector (0.,0.,fillingheight/2.));
	  sampleheight=fillingheight/2.;
	  G4LogicalVolume* sample_log
	    = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
	  sample_log -> SetVisAttributes(Sample_Colour);
	  new G4PVPlacement(0, G4ThreeVector(0, 0, -4.55*cm+3.9*cm), sample_log,
			    "sample", Nfilledchamber_log, false, 0);
	}
      else if ((fillingheight>10.15*cm) && (fillingheight<=16.4*cm))
	{
	  G4VSolid* smarinellisample1_cons = new G4Cons("smarinellisample1_cons", 
							5.05*cm, 
							7.25*cm, 
							5.05*cm-(0.15/10.15*7.8*cm), 
							7.25*cm, 
							3.9*cm, 0, twopi);

	  G4VSolid* smarinellisample2_cons = new G4Cons("smarinellisample2_cons", 
							5.05*cm-(0.15/10.15*7.8*cm), 
							7.3*cm, 
							4.9*cm, 
							7.3*cm+((0.375/8.6)*2.35*cm), 
							1.175*cm, 0, twopi);

	  G4VSolid* temp_shape=0;
	  temp_shape   = new G4UnionSolid("temp_shape", smarinellisample1_cons, 
					  smarinellisample2_cons, 0, G4ThreeVector (0.,0.,3.9*cm+1.175*cm));

	  G4VSolid* smarinellisample3_cons = new G4Cons("smarinellisample3_cons", 
							0*cm, 
							7.3*cm+((0.375/8.6)*2.35*cm), 
							0*cm, 
							7.3*cm+((0.375/8.6)*(fillingheight-7.8*cm)), 
							(fillingheight-10.15*cm)/2., 0, twopi);

	  sample_shape   = new G4UnionSolid("sample_shape", temp_shape, smarinellisample3_cons, 
					    0, 
					    G4ThreeVector (0.,0.,3.9*cm+2.35*cm+(fillingheight-10.15*cm)/2.));
	  sampleheight=fillingheight/2.;

	  G4LogicalVolume* sample_log
       	    = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
	  sample_log -> SetVisAttributes(Sample_Colour);
	  new G4PVPlacement(0, 
			    G4ThreeVector(0, 0, (-4.55*cm-1.175*cm+fillingheight/2.-smarinelli_height/2.)), 
			    sample_log,
			    "sample", Nfilledchamber_log, false, 0);
	}
      else
	{
	  MGLog(error) << "The value for the empty space above the filling is not correct" << endlog;
	  G4Exception("MGEMIDetector::ConstructDetector()","err002",FatalException,"Check the value");
	  MGLog(fatal) << endlog;
	}
    }
  else if (sample_geom=="box")
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

      sample_shape = new G4Sphere("sample_sphere",
				  0., sphere_rad, 0.,twopi, 0.,twopi);
    }
  else if (sample_geom=="sbox")
    {
      // ----------------------------------------- Cu block in nitrogen filled chamber

      G4Box* Cublock_box = new G4Box("Cublock_box", 8.0*cm, 8.0*cm, 10.425*cm);

      // ----------------------------------------- drilled hole in Cu block in nitrogen filled chamber

      G4Tubs* drhole_tubs = new G4Tubs("drhole_tubs", 0., 5.0*cm, 10.425*cm, 0, twopi);

      // ----------------------------------------- combined geometry for Cu shield in nitrogen filled chamber

      G4VSolid* Cu_shield=0;
      Cu_shield   = new G4SubtractionSolid("Cu_shield", Cublock_box, 
					   drhole_tubs, 0, G4ThreeVector (0.,0.,0.));
      G4LogicalVolume* Cu_shield_log
	= new G4LogicalVolume(Cu_shield, Cu, "Cu_shield_log", 0, 0, 0);
      Cu_shield_log -> SetVisAttributes(Cucolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, -1.525*cm), Cu_shield_log,
			"Cu_shield", Nfilledchamber_log, false, 0);
  
      // --- sample holder is 0.69 (0.815) cm from detector window (small sample holder), 

      //  G4Tubs* sample_holder_tubs = new G4Tubs("sample_holder_tubs", 4.75*cm, 5.0*cm, 
      //2.5*cm, 0, twopi); small
      G4Tubs* sample_holder_tubs = new G4Tubs("sample_holder_tubs", 4.75*cm, 5.0*cm, 2.83*cm, 0, twopi);
      G4LogicalVolume* sample_holder_log 
	= new G4LogicalVolume(sample_holder_tubs, Cu, "sample_holder_log", 0, 0, 0);
      sample_holder_log -> SetVisAttributes(Cucolour);
      //  new G4PVPlacement(0, G4ThreeVector(0., 0., 5.865*cm), sample_holder_log, "sample_holder",
      //Nfilledchamber_log, false, 0); small
      new G4PVPlacement(0, G4ThreeVector(0., 0., 6.07*cm), sample_holder_log, "sample_holder",
			Nfilledchamber_log, false, 0);

      // --- Cu support ring of sample holder 0.28 (0.255) cm thickness (small sample holder)

      //  G4Tubs* sample_holder_ring_tubs = 
      //new G4Tubs("sample_holder_ring_tubs", 4.5*cm, 4.75*cm, 0.1275*cm, 0, twopi); small
      G4Tubs* sample_holder_ring_tubs = new G4Tubs("sample_holder_ring_tubs", 
						   4.45*cm, 4.75*cm, 0.14*cm, 0, twopi);
      G4LogicalVolume* sample_holder_ring_log 
	= new G4LogicalVolume(sample_holder_ring_tubs, Cu, "sample_holder_ring_log", 0, 0, 0);
      sample_holder_ring_log -> SetVisAttributes(Cucolour);
      //  new G4PVPlacement(0, G4ThreeVector(0., 0., 3.4925*cm), sample_holder_ring_log, 
      //"sample_holder_ring",Nfilledchamber_log, false, 0); small
      new G4PVPlacement(0, G4ThreeVector(0., 0., 3.38*cm), sample_holder_ring_log, 
			"sample_holder_ring",Nfilledchamber_log, false, 0);

      // --- support ring in sample holder is 0.97 (1.07) cm from detector window (small sample holder)

      G4Tubs* supportring_tubs = new 
	G4Tubs("supportring_tubs", di_sring/2., do_sring/2., h_sring/2., 0, twopi);
      G4LogicalVolume* supportring_log 
	= new G4LogicalVolume(supportring_tubs, Acrylic, "supportring_log", 0, 0, 0);
      supportring_log -> SetVisAttributes(Acryliccolour);
      //  new G4PVPlacement(0, G4ThreeVector(0., 0., 3.62*cm+h_sring/2.), 
      //supportring_log, "supportring",Nfilledchamber_log, false, 0); small
      new G4PVPlacement(0, G4ThreeVector(0., 0., 3.52*cm+h_sring/2.), supportring_log, 
			"supportring",Nfilledchamber_log, false, 0);

      // --- standard cylindrical box (bottom side facing detector)

      G4Tubs* detplasticbottom_tubs = new G4Tubs("detplasticbottom_tubs", 0.*cm, 
						 do_sbox/2., 0.05*cm, 0, twopi);
      G4LogicalVolume* detplasticbottom_log 
	= new G4LogicalVolume(detplasticbottom_tubs, PS, "detplasticbottom_log", 0, 0, 0);
      detplasticbottom_log -> SetVisAttributes(PETcolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+3.52*cm+h_sring+0.05*cm), 
			detplasticbottom_log, "detplasticbottom",Nfilledchamber_log, false, 0);

      G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, do_sbox/2., 0.05*cm, 0, twopi);
      G4LogicalVolume* detplasticcap_log 
	= new G4LogicalVolume(detplasticcap_tubs, PS, "detplasticcap_log", 0, 0, 0);
      detplasticcap_log -> SetVisAttributes(PETcolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+3.52*cm+h_sring+h_sbox+0.15*cm), 
			detplasticcap_log, "detplasticcap", Nfilledchamber_log, false, 0);

      G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", di_sbox/2., 
						do_sbox/2., h_sbox/2., 0,twopi);
      G4LogicalVolume* detplasticshell_log 
	= new G4LogicalVolume(detplasticshell_tubs, PS, "detplasticshell_log", 0, 0, 0);
      detplasticshell_log -> SetVisAttributes(PETcolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+3.52*cm+h_sring+h_sbox/2.+0.1*cm), 
			detplasticshell_log, "detplasticshell", Nfilledchamber_log, false, 0);

      // cylinder sample inside the standard box (default - filling the volume)

      sample_shape = new G4Tubs("sample_tube", 
				tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
      G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
      sample_log -> SetVisAttributes(Sample_Colour);

      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, 3.52*cm+h_sring+tube_length/2.+0.1*cm), 
			sample_log, "sample", Nfilledchamber_log, false, 0);

    }
  else if (sample_geom=="twobox")
    {
      //---for point like sources, two boxes away from detector (strong sources)
      // --- standard cylindrical box (cover side facing detector) - 0.1cm from detector window

      G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, twopi);
      G4LogicalVolume* detplasticcap_log 
	= new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
      detplasticcap_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(0., 0., 10.95*cm), detplasticcap_log, "detplasticcap", 
			Nfilledchamber_log, false, 0);

      G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, twopi);
      G4LogicalVolume* detplasticcap1_log 
	= new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
      detplasticcap1_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(0., 0., 13.875*cm), detplasticcap1_log, "detplasticcap1", 
			Nfilledchamber_log, false, 0);
 
      G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, twopi);
      G4LogicalVolume* detplasticshell_log 
	= new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
      detplasticshell_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(0., 0., 12.4*cm), detplasticshell_log, "detplasticshell", 
			Nfilledchamber_log, false, 0);

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
      // --- standard cylindrical box (cover side facing detector) - 0.1cm from detector 
      //window _____with liquid sample_____

      G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, twopi);
      G4LogicalVolume* detplasticcap_log 
	= new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
      detplasticcap_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.35*cm), detplasticcap_log, "detplasticcap", 
			Nfilledchamber_log, false, 0);

      G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, twopi);
      G4LogicalVolume* detplasticcap1_log 
	= new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
      detplasticcap1_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+12.275*cm), detplasticcap1_log, "detplasticcap1", 
			Nfilledchamber_log, false, 0);

      G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0,twopi);
      G4LogicalVolume* detplasticshell_log 
	= new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
      detplasticshell_log -> SetVisAttributes(Acryliccolour);
      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+10.8*cm), detplasticshell_log, "detplasticshell", 
			Nfilledchamber_log, false, 0);

      // liquid filling the standard box up to level of "boxheight"
  
      G4VSolid* sample_body   = new G4Tubs("sample_tube", tube_innrad, tube_outrad, 
					   tube_length/2, 0, tube_angle);
      G4VSolid* sample_hole   = new G4Box("sample_hole", tube_outrad+0.1*cm, tube_outrad, 
					  tube_length/2+0.1*cm);
      sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, 
					    G4ThreeVector(0., 2*tube_outrad-box_height,0.));
    }
  else if (sample_geom=="marinelli")
    {
      // "marinelli" shape (box with a hole)
      G4VSolid* sample_body = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);
      G4VSolid* sample_hole = new G4Tubs("sample_tube", 0., tube_outrad, tube_length, 0, tube_angle);
      sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, 
					    G4ThreeVector(0., tube_innrad, -box_thickness/2));
    }
  else if (sample_geom=="custom")
    {
      // here define any other sample geometry
      // ----------------------------------------- Cu block in nitrogen filled chamber

      G4Box* Cublock_box = new G4Box("Cublock_box", 8.0*cm, 8.0*cm, 10.425*cm);

      // ----------------------------------------- drilled hole in Cu block in nitrogen filled chamber

      G4Tubs* drhole_tubs = new G4Tubs("drhole_tubs", 0., 5.0*cm, 10.425*cm, 0, twopi);

      // ----------------------------------------- combined geometry for Cu shield in nitrogen filled chamber

      G4VSolid* Cu_shield=0;
      Cu_shield   = new G4SubtractionSolid("Cu_shield", Cublock_box, 
					   drhole_tubs, 0, G4ThreeVector (0.,0.,0.));
      G4LogicalVolume* Cu_shield_log
	= new G4LogicalVolume(Cu_shield, Cu, "Cu_shield_log", 0, 0, 0);
      Cu_shield_log -> SetVisAttributes(Cucolour);
      new G4PVPlacement(0, G4ThreeVector(0, 0, -1.525*cm), Cu_shield_log,
			"Cu_shield", Nfilledchamber_log, false, 0);
  
      // --- Cu tap 0.815 cm from detector window  

      G4Tubs* cu_tap_tubs = new G4Tubs("cu_tap_tubs", 0*cm, 5.0*cm, 2.5*cm, 0, twopi); 
      G4LogicalVolume* cu_tap_log 
	= new G4LogicalVolume(cu_tap_tubs, Cu, "cu_tap_log", 0, 0, 0);
      cu_tap_log -> SetVisAttributes(Cucolour);
      new G4PVPlacement(0, G4ThreeVector(0., 0., 5.865*cm), cu_tap_log, "cu_tap",
			Nfilledchamber_log, false, 0); 

      // ENEA, 3 Ni disks

      G4VSolid* temp1_shape = new G4Tubs("temp1_shape", 0., 0.9*cm, 0.05*cm, 0, twopi);

      G4VSolid* temp2_shape = new G4Tubs("temp2_shape", 0., 0.9*cm, 0.05*cm, 0, twopi);

      G4VSolid* temp3_shape=0;
      temp3_shape   = new G4UnionSolid("temp3_shape", temp1_shape, 
				       temp2_shape, 0, G4ThreeVector (1.8*cm,0.,0.));

      sample_shape   = new G4UnionSolid("sample_shape", temp3_shape, temp1_shape, 
					0, G4ThreeVector (0.9*cm,1.56*cm,0.));

      G4LogicalVolume* sample_log
	= new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
      sample_log -> SetVisAttributes(Sample_Colour);
      new G4PVPlacement(0, G4ThreeVector(-0.9*cm, -0.52*cm, 2.6*cm), sample_log,
			"sample", Nfilledchamber_log, false, 0);

      //ElectronicsJunctionSled
      //ElectronicsJunctionSledPins
      //ElectronicsJunctionSledSpacer
      //ElectronicsJunctionSledCables

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
      /*G4Tubs* NPLbottle_tubs = new G4Tubs("NPLbottle_tubs", 0., 2.225*cm, 4.5*cm, 0, twopi);

      G4LogicalVolume* NPLbottle_log = new G4LogicalVolume(NPLbottle_tubs, Teflon, "NPLbottle_log", 0, 0, 0);
      NPLbottle_log -> SetVisAttributes(Acryliccolour);

      //rotate 90deg up :
      G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
      halfPi_x_rot -> rotateX(90*deg);

      //position : touching detector window; on bottom of the Chamber
      new G4PVPlacement(halfPi_x_rot, G4ThreeVector(0., s_y-5.65*cm, 13.025*cm), NPLbottle_log, "NPLbottle", Nfilledchamber_log, false, 0);



      //contents :
      G4Tubs* NPLbottleN2_tubs = new G4Tubs("NPLbottleN2_tubs", 0., 2.175*cm, 4.25*cm-tube_length/2, 0, twopi);    //filled with N2 of the chamber (?)

      G4LogicalVolume* NPLbottleN2_log = new G4LogicalVolume(NPLbottleN2_tubs, N2Gas, "NPLbottleN2_log", 0, 0, 0);
      NPLbottleN2_log -> SetVisAttributes(N2Gascolour);

      new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2-0.05*cm), NPLbottleN2_log, "NPLbottleN2", NPLbottle_log, false, 0);


      //needs to change macro confine volume ---> NPLsample
      G4Tubs* NPLsample_tubs = new G4Tubs("NPLsample_tubs", 0.,  2.175*cm, tube_length/2, 0, twopi);

      G4LogicalVolume* NPLsample_log = new G4LogicalVolume(NPLsample_tubs, sample_mat, "NPLsample_log", 0, 0, 0);
      NPLsample_log -> SetVisAttributes(Sample_Colour);

      new G4PVPlacement(0, G4ThreeVector(0., 0., tube_length/2-4.3*cm), NPLsample_log, "NPLsample", NPLbottle_log, false, 0);


      //puts small ball away, to avoid errors
      sample_shape = new G4Sphere("sample_sphere", 0., sphere_rad, 0.,twopi, 0.,twopi);
      s_z = 15.*cm;
      */
    }
  else   
    {
      //      MGLog(error) << "The shape " << sample_geom << " is not defined " << endlog;
      //      MGLog(error) sample_geom << endlog;

      G4cout << "\n\n" << sample_geom << "\n\n";
      MGLog(fatal) << endlog;
      G4Exception("MGEMIDetector::ConstructDetector()","err003",FatalException,"Check the shape type");
    }
  //G4LogicalVolume* sample_log = new G4LogicalVolume(marinelli_sample, sample_mat, "sample_log", 0, 0, 0);
  /*  G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);
      sample_log -> SetVisAttributes(Sample_Colour);

      new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+10.8*cm), 
      sample_log, "sample", Nfilledchamber_log, false, 0);

  */


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
