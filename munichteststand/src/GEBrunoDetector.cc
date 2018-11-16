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
// $Id: GEBrunoDetector.cc,v 1.7 2009-05-18 16:08:53 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GEBrunoDetector.cc
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
 * 03-06-2007, Changed to avoid problem with materials defined on-the-fly, Luciano
 * 03-07-2007, Added sample-geometry 'custom' and modified a bit other geometries, Dusan
 * 06-01-2007, Shifted the setup to place coordinate origin in the middle of front detector window, Dusan
 * 14-05-2009, Added one sample geometry, Marc
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
#include "G4UnionSolid.hh"
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

#include "munichteststand/GEBrunoDetector.hh"
#include "munichteststand/GEBrunoDetectorMessenger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GEBrunoDetector::GEBrunoDetector() : MGGeometryDetector("")
{
  //set defaults
  sample_geom="none";
  //box height is also liquid level in the standard box
  box_height=1.1*cm;
  //box width is also diference between standard box and sample positions
  box_width=0.;
  box_thickness=5.*cm;
  // default standard dose
  tube_innrad=0*cm;
  tube_outrad=3.65*cm;
  tube_length=2.8*cm;
  tube_angle=twopi;
  sample_mat_name="NitrogenGas";
  //G4cout << "\n\n" << sample_mat_name << "\n\n";
  s_x=0.*cm;
  s_y=0.*cm;
  s_z=0.*cm;
  theMessenger = new GEBrunoDetectorMessenger(this);
}

//---------------------------------------------------------------------------//

GEBrunoDetector::~GEBrunoDetector()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void GEBrunoDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  // G4Material* Brass = G4Material::GetMaterial("Brass");
  G4Material* ProportionalGas = G4Material::GetMaterial("ProportionalGas");
  G4Material* N2Gas = G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  G4Material* Pb = G4Material::GetMaterial("MetalLead");
  G4Material* Fe = G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  G4Material* Air = G4Material::GetMaterial("Air");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  //G4Material* Vespel = G4Material::GetMaterial("Vespel");
  G4Material* PVC = G4Material::GetMaterial("PVC");
  G4Material* MONEL = G4Material::GetMaterial("MONEL");
  G4Material* HD1000 = G4Material::GetMaterial("HD1000");
  //  G4Material* K2CO3 = G4Material::GetMaterial("K2CO3");
  //  G4Material* NPLsolution = G4Material::GetMaterial("NPLsolution");
  // G4Material* PE = G4Material::GetMaterial("PE");

 // Visualization attributes
  //G4VisAttributes * Brasscolour= new G4VisAttributes( G4Colour(226/255. ,163/255. ,29/255. ));  //orange-brown

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

  G4VisAttributes * Acryliccolour= new G4VisAttributes( G4Colour(231/255. ,217/255. ,240/255. ));  //light blue-grey
 
  //  G4VisAttributes * Testcolour = new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));
  //Testcolour -> SetForceSolid(true);
 
 //------------------------------------------------------ volumes
  //------------------------------ experimental hall (world volume) ------- 
  G4double expHall_x = 1*m;
  G4double expHall_y = 1*m;
  G4double expHall_z = 1*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  G4LogicalVolume* experimentalHall_log = new 
    G4LogicalVolume(experimentalHall_box, Air, "expHall_log", 0, 0, 0);
  experimentalHall_log -> SetVisAttributes(G4VisAttributes::Invisible);


  //  G4VPhysicalVolume* experimentalHall_phys
  //    = new G4PVPlacement(0, G4ThreeVector(), experimentalHall_log, "expHall",0, false, 0);


  //------ A new air volume containing the whole detector in order to allow shifting it w.r.t. the coordinate origin

G4Box* air_box = new G4Box("air_box", 0.9*m,0.9*m,0.9*m);

G4LogicalVolume* air_log = new G4LogicalVolume(air_box, Air, "air_log", 0, 0, 0);
air_log -> SetVisAttributes(G4VisAttributes::Invisible);

new G4PVPlacement(0, G4ThreeVector(0., 0., 0.6*cm), air_log, "air", experimentalHall_log, false, 0);  



  // ---------------------------------------------------------------------------------------
// ---------------- OUTER SHIELDING -----------------
  // ---------------------------------------------------------------------------------------


  // --- Front Door : ------------------------------------------------------------
  // --- Steel structure - sides                                            y position?  iron instead of steel  !!!

  G4VSolid* Steelside_body   = new G4Box("Steelside_body", 4.75*cm, 36.5*cm, 14*cm);
  G4VSolid* Steelside_hole   = new G4Box("Steelside_hole", 4.75*cm, 37*cm, 12.5*cm);
  G4VSolid* SteelDoorPlusx_solid = new G4SubtractionSolid("SteelDoorPlusx_solid", Steelside_body, Steelside_hole, 0, G4ThreeVector(1*cm,0.,0.));

  G4LogicalVolume* SteelDoorPlusx_log = new  G4LogicalVolume(SteelDoorPlusx_solid, Fe, "SteelDoorPlusx_log", 0, 0, 0);
  SteelDoorPlusx_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(41*cm, 0., 35.4*cm), SteelDoorPlusx_log, "SteelDoorPlusx", air_log, false, 0);

  G4VSolid* SteelDoorMinusx_solid = new G4SubtractionSolid("SteelDoorMinusx_solid", Steelside_body, Steelside_hole, 0, G4ThreeVector(-1*cm,0.,0.));

  G4LogicalVolume* SteelDoorMinusx_log = new  G4LogicalVolume(SteelDoorMinusx_solid, Fe, "SteelDoorMinusx_log", 0, 0, 0);
  SteelDoorMinusx_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(-41*cm, 0., 35.4*cm), SteelDoorMinusx_log, "SteelDoorMinusx", air_log, false, 0);

  // --- Steel structure - bottom                                                               iron instead of steel  !!!

  G4VSolid* Steelbottom_body   = new G4Box("Steelbottom_body", 45.75*cm, 9.5*cm, 14*cm);
  G4VSolid* Steelbottom_hole   = new G4Box("Steelbottom_hole", 46*cm, 8.5*cm, 12.5*cm);
  G4VSolid* SteelDoorbottom_solid = new G4SubtractionSolid("SteelDoorbottom_solid", Steelbottom_body, Steelbottom_hole, 0, G4ThreeVector(0.,0.,0.));

  G4LogicalVolume* SteelDoorbottom_log = new  G4LogicalVolume(SteelDoorbottom_solid, Fe, "SteelDoorbottom_log", 0, 0, 0);
  SteelDoorbottom_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(0., -46*cm, 35.4*cm), SteelDoorbottom_log, "SteelDoorbottom", air_log, false, 0);

  // --- Steel plate                                                               iron instead of steel  !!!
  G4Box* SteelDoorPlate_box  = new G4Box("SteelDoorPlate_box", 39.5*cm, 38*cm, 0.25*cm);

  G4LogicalVolume* SteelDoorPlate_log = new  G4LogicalVolume(SteelDoorPlate_box, Fe, "SteelDoorPlate_log", 0, 0, 0);
  SteelDoorPlate_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(0., -1.5*cm, 49.65*cm), SteelDoorPlate_log, "SteelDoorPlate", air_log, false, 0);

  // --- Pb block                                                          y position?
  G4Box* PbBlockDoor_box  = new G4Box("PbBlockDoor_box", 36.25*cm, 36.5*cm, 7.5*cm);

  G4LogicalVolume* PbBlockDoor_log = new  G4LogicalVolume(PbBlockDoor_box, Pb, "PbBlockDoor_log", 0, 0, 0);
  PbBlockDoor_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 28.9*cm), PbBlockDoor_log, "PbBlockDoor", air_log, false, 0);

   //     --- Copper ring -----------------------------------
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* CuRingDoor_tubs = new G4Tubs("CuRingDoor_tubs", 
				          20.*cm, 20.5*cm, 2.5*cm, 0, 2*M_PI);
  G4LogicalVolume* CuRingDoor_log 
    = new G4LogicalVolume(CuRingDoor_tubs, Cu, "CuRingDoor_log", 0, 0, 0);
  CuRingDoor_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -5*cm), CuRingDoor_log, "CuRingDoor", PbBlockDoor_log, false, 0);

  //     --- Muon veto door cap  -----------------------------------
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* vetoDoorCap_tubs = new G4Tubs("vetoDoorCap_tubs", 
				          0., 20.*cm, 2.5*cm, 0, 2*M_PI);
  G4LogicalVolume* vetoDoorCap_log 
    = new G4LogicalVolume(vetoDoorCap_tubs, Cu, "vetoDoorCap_log", 0, 0, 0);
  vetoDoorCap_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -5*cm), vetoDoorCap_log, "vetoDoorCap", PbBlockDoor_log, false, 0);

  //           --- proportional counting gas
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* vetoGasDoorCap_tubs = new G4Tubs("vetoGasDoorCap_tubs", 
				        0., 19.5*cm, 2*cm, 0, 2*M_PI);
  G4LogicalVolume* vetoGasDoorCap_log 
    = new G4LogicalVolume(vetoGasDoorCap_tubs, ProportionalGas, "vetoGasDoorCap_log", 0, 0, 0);
  vetoGasDoorCap_log -> SetVisAttributes(ProportionalGascolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vetoGasDoorCap_log, "vetoGasDoorCap", vetoDoorCap_log, false, 0);


  // --- Cu plate :                                                   hole different radius !!!!
  G4VSolid* CuPlateDoor_body   = new G4Box("CuPlateDoor_body", 39.5*cm, 38*cm, 0.25*cm);
  G4VSolid* CuPlateDoor_hole   = new G4Tubs("CuPlateDoor_hole", 0, 20*cm, 0.5*cm, 0, 2*M_PI);
  G4VSolid* CuPlateDoor_solid = new G4SubtractionSolid("CuPlateDoor_solid", CuPlateDoor_body, CuPlateDoor_hole, 0, G4ThreeVector(0.,1.5*cm,0.));
  
  // G4Box* CuPlateDoor_box  = new G4Box("CuPlateDoor_box", 39.5*cm, 38*cm, 0.25*cm);

  G4LogicalVolume* CuPlateDoor_log = new  G4LogicalVolume(CuPlateDoor_solid, Cu, "CuPlateDoor_log", 0, 0, 0);
  CuPlateDoor_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., -1.5*cm, 21.15*cm), CuPlateDoor_log, "CuPlateDoor", air_log, false, 0);


  //     --- Muon veto door cap wall
  //                                      inner r, outer r, height, angle0, angle1
//
//    G4Tubs* vetoDoorCapWall_tubs = new G4Tubs("vetoDoorCapWall_tubs", 
//  				          0., 20.*cm, 0.25*cm, 0, 2*M_PI);
//    G4LogicalVolume* vetoDoorCapWall_log 
//      = new G4LogicalVolume(vetoDoorCapWall_tubs, Brass, "vetoDoorCapWall_log", 0, 0, 0);
//    vetoDoorCapWall_log -> SetVisAttributes(Brasscolour);
//
//  G4VPhysicalVolume* vetoDoorCapWall_phys
//  = new G4PVPlacement(0, G4ThreeVector(0., 1.5*cm, 0.), vetoDoorCapWall_log, "vetoDoorCapWall", CuPlateDoor_log, false, 0);




  // --- Outer shielding ------------------------------------------
  // --- Pb top plate
  G4Box* PbTop_box  = new G4Box("PbTop_box", 35*cm, 5*cm, 29.9*cm);

  G4LogicalVolume* PbTop_log = new  G4LogicalVolume(PbTop_box, Pb, "PbTop_log", 0, 0, 0);
  PbTop_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(1*cm, 27.7*cm, -9*cm), PbTop_log, "PbTop", air_log, false, 0);

  // --- Steel top plate                                                            iron instead of steel  !!!
  G4Box* SteelPlate_box  = new G4Box("SteelPlate_box", 36.5*cm, 1*cm, 31.4*cm);

  G4LogicalVolume* SteelPlate_log = new  G4LogicalVolume(SteelPlate_box, Fe, "SteelPlate_log", 0, 0, 0);
  SteelPlate_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(2*cm, 21.7*cm, -10.5*cm), SteelPlate_log, "SteelPlate", air_log, false, 0);

    // --- Pb side walls :
  G4Box* PbWallSide_box  = new G4Box("PbWallSide_box", 5*cm, 20.7*cm, 30*cm);

  G4LogicalVolume* PbWallSide_log = new  G4LogicalVolume(PbWallSide_box, Pb, "PbWallSide_log", 0, 0, 0);
  PbWallSide_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(25.25*cm, 0, -9.1*cm), PbWallSide_log, "PbWallPlusx", air_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-25.25*cm, 0, -9.1*cm), PbWallSide_log, "PbWallMinusx", air_log, false, 0);

  // --- Cu plate :
  G4Box* CuPlate_box  = new G4Box("CuPlate_box", 20.25*cm, 0.15*cm, 20.9*cm);

  G4LogicalVolume* CuPlate_log = new  G4LogicalVolume(CuPlate_box, Cu, "CuPlate_log", 0, 0, 0);
  CuPlate_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., -20.55*cm, 0.), CuPlate_log, "CuPlate", air_log, false, 0);

  // --- Pb bottom plate
  G4Box* PbBottom_box  = new G4Box("PbBottom_box", 40*cm, 7.5*cm, 29.5*cm);

  G4LogicalVolume* PbBottom_log = new  G4LogicalVolume(PbBottom_box, Pb, "PbBottom_log", 0, 0, 0);
  PbBottom_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(0, -28.2*cm, -8.6*cm), PbBottom_log, "PbBottom", air_log, false, 0);




  // --- PVC support -----------------------------------------------------          (change color)
  // G4Box* PVC_box  = new G4Box("PVC_box", 20.25*cm, 7.565*cm, 20.9*cm);

  G4VSolid* support_body   = new G4Box("support_body", 20.25*cm, 7.415*cm, 20.9*cm);
  G4VSolid* support_hole   = new G4Tubs("support_hole", 0, 20.25*cm, 21*cm, 0, 2*M_PI);
  G4VSolid* PVC_solid = new G4SubtractionSolid("PVC_solid", support_body, support_hole, 0, G4ThreeVector(0., 12.985*cm,0.));

  G4LogicalVolume* PVC_log = new  G4LogicalVolume(PVC_solid, PVC, "PVC_log", 0, 0, 0);
  PVC_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0., -12.985*cm, 0.), PVC_log, "PVC", air_log, false, 0);




  // --- Muon veto ---------------------------------                            add glue?!?
  // --- outer housing
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* veto_tubs = new G4Tubs("veto_tubs", 
				        15*cm, 20.25*cm, 18.4*cm, 0, 2*M_PI);
  G4LogicalVolume* veto_log 
    = new G4LogicalVolume(veto_tubs, Cu, "veto_log", 0, 0, 0);
  veto_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 2.5*cm), veto_log, "veto", air_log, false, 0);

  //     --- proportional counting gas
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* vetoGas_tubs = new G4Tubs("vetoGas_tubs", 
				        15.5*cm, 19.75*cm, 17.9*cm, 0, 2*M_PI);
  G4LogicalVolume* vetoGas_log 
    = new G4LogicalVolume(vetoGas_tubs, ProportionalGas, "vetoGas_log", 0, 0, 0);
  vetoGas_log -> SetVisAttributes(ProportionalGascolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vetoGas_log, "vetoGas", veto_log, false, 0);


 // ---------------------------------------------------------------------------------------
  // ------------- INNER SHIELDING ------------------------------------
  // ---------------------------------------------------------------------------------------


  // --- steel cylinder:                                                               iron instead of steel  !!!
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* insteelshield_tubs = new G4Tubs("insteelshield_tubs", 
					7.75*cm, 15*cm, 18.4*cm, 0, 2*M_PI);
  G4LogicalVolume* insteelshield_log 
    = new G4LogicalVolume(insteelshield_tubs, Fe, "insteelshield_log", 0, 0, 0);
  insteelshield_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 2.5*cm), 
			insteelshield_log, "insteelshield", air_log, false, 0);
  
  // --- Pb cylinder:
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* inPbshield_tubs = new G4Tubs("inPbshield_tubs", 
					4.99*cm, 7.75*cm, 18.4*cm, 0, 2*M_PI);
  G4LogicalVolume* inPbshield_log 
    = new G4LogicalVolume(inPbshield_tubs, Pb, "inPbshield_log", 0, 0, 0);
  inPbshield_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 2.5*cm), 
			inPbshield_log, "inPbshield", air_log, false, 0);

  // --- Cu cylinder:                                                          
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* inCushield_tubs = new G4Tubs("inCushield_tubs", 
					4.5*cm, 4.99*cm, 15.62*cm, 0, 2*M_PI);
  G4LogicalVolume* inCushield_log 
    = new G4LogicalVolume(inCushield_tubs, Cu, "inCushield_log", 0, 0, 0);
  inCushield_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -0.28*cm), inCushield_log, "inCushield", air_log, false, 0);

  // ---- removable inner Pb cylinder (position - touching the Cu-layer) :
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* inPbCap_tubs = new G4Tubs("inPbCap_tubs", 0., 4.95*cm, 2.5*cm, 0, 2*M_PI);

  G4LogicalVolume* inPbCap_log 
    = new G4LogicalVolume(inPbCap_tubs, Pb, "inPbCap_log", 0, 0, 0);
  inPbCap_log -> SetVisAttributes(Pbcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 17.84*cm), 
			inPbCap_log, "inPbCap", air_log, false, 0);




  // ---------------------------------------------------------------------------------------
  // ----------------------------- BACK SHIELDING --------------------------------------------
  // ---------------------------------------------------------------------------------------



  // --- Muon veto back cap  -----------------------------------
  //                                    inner r, outer r, height, angle0, angle1
 
  G4Tubs* vetoBackCap_tubs = new G4Tubs("vetoBackCap_tubs", 
				          4.5*cm, 20.25*cm, 2.5*cm, 0, 2*M_PI);
  G4LogicalVolume* vetoBackCap_log
    = new G4LogicalVolume(vetoBackCap_tubs, Cu, "vetoBackCap_log", 0, 0, 0);
  vetoBackCap_log -> SetVisAttributes(Cucolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -18.4*cm), vetoBackCap_log, "vetoBackCap", air_log, false, 0);

  //      --- proportional counting gas
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* vetoGasBackCap_tubs = new G4Tubs("vetoGasBackCap_tubs", 
				        5*cm, 19.75*cm, 2*cm, 0, 2*M_PI);
  G4LogicalVolume* vetoGasBackCap_log 
    = new G4LogicalVolume(vetoGasBackCap_tubs, ProportionalGas, "vetoGasBackCap_log", 0, 0, 0);
  vetoGasBackCap_log -> SetVisAttributes(ProportionalGascolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vetoGasBackCap_log, "vetoGasBackCap", vetoBackCap_log, false, 0);


  // --- 15cm Pb block with 90mm hole                                               dimensions just a gues!!!!!!!!

  G4VSolid* PbBackWall_body   = new G4Box("PbBackWall_body", 20.25*cm, 20.7*cm, 7.5*cm);
  G4VSolid* PbBackWall_hole   = new G4Tubs("PbBackWall_hole", 0, 4.5*cm, 8*cm, 0, 2*M_PI);
  G4VSolid* PbBackWall_solid = new G4SubtractionSolid("PbBackWall_solid", PbBackWall_body, PbBackWall_hole, 0, G4ThreeVector(0., 0,0.));

  G4LogicalVolume* PbBackWall_log = new  G4LogicalVolume(PbBackWall_solid, Pb, "PbBackWall_log", 0, 0, 0);
  PbBackWall_log -> SetVisAttributes(Pbcolour);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -28.4*cm), PbBackWall_log, "PbBackWall", air_log, false, 0);

  
  
  // ---------------------------------------------------------------------------------------
// --------------------- SAMPLE CHAMBER -----------------------------
  // ---------------------------------------------------------------------------------------


  // --- Inner volume filled with nitrogen ---------------------------------
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* inNitrogen_tubs = new G4Tubs("inNitrogen_tubs", 
					0, 4.5*cm, 25.62*cm, 0, 2*M_PI);
  G4LogicalVolume* inNitrogen_log 
    = new G4LogicalVolume(inNitrogen_tubs, N2Gas, "inNitrogen_log", 0, 0, 0);
  inNitrogen_log -> SetVisAttributes(N2Gascolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -10.28*cm), inNitrogen_log, "inNitrogen", air_log, false, 0);
  
  //      --- removable inner steel cylinder (position - touching the Pb-cap, adjust for different measurements)                  iron instead of steel  !!!
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* insteelCap_tubs = new G4Tubs("insteelCap_tubs", 0., 4.15*cm, 2.25*cm, 0, 2*M_PI);

  G4LogicalVolume* insteelCap_log 
    = new G4LogicalVolume(insteelCap_tubs, Fe, "insteelCap_log", 0, 0, 0);
  insteelCap_log -> SetVisAttributes(Fecolour);

  new G4PVPlacement(0, G4ThreeVector(0., -0.35*cm, 23.37*cm), 
			insteelCap_log, "insteelCap", inNitrogen_log, false, 0);
  
  
  //      --- missing - acrylic cap !
  


  // ---------------------------------------------------------------------------------------
  //      ----------------------------- DETECTOR ---------------------------------
  // ---------------------------------------------------------------------------------------


  //      --- Monel detector housing:                            some dimensions (lenghts) estimated!!!! Pb around back part missing!!!!! (change colour)
  //                                                             inner r, outer r, height, angle0, angle1

  G4VSolid* MonelHousing_body   = new G4Tubs("MonelHousing_body", 0, 4*cm, 11*cm, 0, 2*M_PI);
  G4VSolid* MonelHousing_hole   = new G4Tubs("MonelHousing_hole", 3.9*cm, 4.1*cm, 5*cm, 0, 2*M_PI);
  G4VSolid* MonelHousing_solid = new G4SubtractionSolid("MonelHousing_solid", MonelHousing_body, MonelHousing_hole, 0, G4ThreeVector(0.,0.,8*cm));
  
  G4LogicalVolume* MonelHousing_log = new G4LogicalVolume(MonelHousing_solid, MONEL, "MonelHousing_log", 0, 0, 0);
  MonelHousing_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, -1.32*cm), MonelHousing_log, "MonelHousing", inNitrogen_log, false, 0);

  //           --- Vaccuum around detector:
  //                                    inner r, outer r, height, angle0, angle1

  G4Tubs* detVacuum_tubs = new G4Tubs("detVacuum_tubs", 
					0, 3.825*cm, 10.9625*cm, 0, 2*M_PI);
  G4LogicalVolume* detVacuum_log 
    = new G4LogicalVolume(detVacuum_tubs, Vacuum, "detVacuum_log", 0, 0, 0);
  detVacuum_log -> SetVisAttributes(Vacuumcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, -0.0375*cm), detVacuum_log, "detVacuum", MonelHousing_log, false, 0);

  //                --- Al detector holder - front cylindrical shell with 2 holes
  //                                                             inner r, outer r, height, angle0, angle1

  G4VSolid* detAlHolder_body   = new G4Tubs("detAlHolder_body",  2.85*cm, 2.95*cm, 3.3*cm, 0, 2*M_PI);
  G4VSolid* detAlHolder_hole   = new G4Tubs("detAlHolder_hole", 0, 0.5*cm, 3*cm, 0, 2*M_PI);
  G4RotationMatrix* halfPi_x_rot = new G4RotationMatrix;
  halfPi_x_rot -> rotateX(0.5*M_PI);

  G4VSolid* detAlHolder_solid = new G4SubtractionSolid("detAlHolder_solid", detAlHolder_body, detAlHolder_hole, halfPi_x_rot, G4ThreeVector(0.,0.,0.9*cm));

  G4LogicalVolume* detAlHolder_log 
    = new G4LogicalVolume(detAlHolder_solid, Al, "detAlHolder_log", 0, 0, 0);
  detAlHolder_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 6.6875*cm), detAlHolder_log, "detAlHolder", detVacuum_log, false, 0);

  //                --- Al detector holder - 2x ring
  //                                                             inner r, outer r, height, angle0, angle1
  G4Tubs* detAlHolderRing_tubs = new G4Tubs("detAlHolderRing_tubs",  2.95*cm, 3.16*cm, 0.35*cm, 0, 2*M_PI);

  G4LogicalVolume* detAlHolderRing_log 
    = new G4LogicalVolume(detAlHolderRing_tubs, Al, "detAlHolderRing_log", 0, 0, 0);
  detAlHolderRing_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 8.6375*cm), detAlHolderRing_log, "detAlHolderRing1", detVacuum_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0, 5.5375*cm), detAlHolderRing_log, "detAlHolderRing2", detVacuum_log, false, 0);

  //                --- Al detector holder - back cap
  //                                                             inner r, outer r, height, angle0, angle1
  G4Tubs* detAlHolderBack_tubs = new G4Tubs("detAlHolderBack_tubs",  0.8*cm, 2.95*cm, 0.3*cm, 0, 2*M_PI);

  G4LogicalVolume* detAlHolderBack_log 
    = new G4LogicalVolume(detAlHolderBack_tubs, Al, "detAlHolderBack_log", 0, 0, 0);
  detAlHolderBack_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 3.0875*cm), detAlHolderBack_log, "detAlHolderBack", detVacuum_log, false, 0);

  //                --- Al detector holder - hub
  //                                                             inner r, outer r, height, angle0, angle1
  G4VSolid* detAlHolderHub_body   = new G4Tubs("detAlHolderHub_body",  0, 0.8*cm, 2.8*cm, 0, 2*M_PI);
  G4VSolid* detAlHolderHub_hole1   = new G4Tubs("detAlHolderHub_hole1", 0, 0.5*cm, 2*cm, 0, 2*M_PI);
  G4VSolid* detAlHolderHub_hole2   = new G4Tubs("detAlHolderHub_hole2", 0, 0.5*cm, 2*cm, 0, 2*M_PI);
  G4VSolid* detAlHolderHub_solid0 = new G4SubtractionSolid("detAlHolderHub_solid0", detAlHolderHub_body, detAlHolderHub_hole1, 0, G4ThreeVector(0.,0.,1.7*cm));
  G4VSolid* detAlHolderHub_solid = new G4SubtractionSolid("detAlHolderHub_solid", detAlHolderHub_solid0, detAlHolderHub_hole2, 0, G4ThreeVector(0.,0.,-2.6*cm));

  G4LogicalVolume* detAlHolderHub_log 
    = new G4LogicalVolume(detAlHolderHub_solid, Al, "detAlHolderHub_log", 0, 0, 0);
  detAlHolderHub_log -> SetVisAttributes(Alcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 0.5875*cm), detAlHolderHub_log, "detAlHolderHub", detVacuum_log, false, 0);

  //                --- detector  inner housing                                                           (change color)
  //                                                             inner r, outer r, height, angle0, angle1
  G4VSolid* detInHousing_body   = new G4Tubs("detInHousing_body",  0, 2.825*cm, 3*cm, 0, 2*M_PI);
  G4VSolid* detInHousing_hole   = new G4Tubs("detInHousing_hole", 0, 2.775*cm, 3*cm, 0, 2*M_PI);
  G4VSolid* detInHousing_solid = new G4SubtractionSolid("detInHousing_solid", detInHousing_body, detInHousing_hole, 0, G4ThreeVector(0.,0.,-0.1*cm));

  G4LogicalVolume* detInHousing_log 
    = new G4LogicalVolume(detInHousing_solid, HD1000, "detInHousing_log", 0, 0, 0);
  detInHousing_log -> SetVisAttributes(Vespelcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0, 6.9875*cm), detInHousing_log, "detInHousing", detVacuum_log, false, 0);



  // ---------------------------------------------------------------------------------------
// ------------------------------------- CRYSTAL --------------------------------------
  // ---------------------------------------------------------------------------------------


  //                --- crystal (whole) : -------------------------------------------
  
  G4Tubs* crystal_tubs = new G4Tubs("crystal_tubs", 
					0., 2.75*cm, 2.6*cm, 0, 2*M_PI);
  G4LogicalVolume* crystal_log 
    = new G4LogicalVolume(crystal_tubs, Ge, "crystal_log", 0, 0, 0);
  crystal_log -> SetVisAttributes(Gecolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 7.2875*cm), crystal_log, "crystal", detVacuum_log, false, 0);


  
  //                     --- dead layer (sides)
  
  G4Tubs* deadLayer_tubs = new G4Tubs("deadLayer_tubs", 2.65*cm, 2.75*cm, 2.6*cm, 0, 2*M_PI);

  G4LogicalVolume* deadLayer_log 
    = new G4LogicalVolume(deadLayer_tubs, GeLi, "deadLayer_log", 0, 0, 0);
  deadLayer_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), deadLayer_log, "deadLayer", crystal_log, false, 0);
  
  //                     --- dead layer (front)
  
  G4Tubs* deadLayerFront_tubs = new G4Tubs("deadLayerFront_tubs", 0.0, 2.65*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* deadLayerFront_log 
    = new G4LogicalVolume(deadLayerFront_tubs, GeLi, "deadLayerFront_log", 0, 0, 0);
  deadLayerFront_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 2.55*cm), deadLayerFront_log, "deadLayerFront", crystal_log, false, 0);

  //                     --- dead layer (back)
  
  G4Tubs* deadLayerBack_tubs = new G4Tubs("deadLayerBack_tubs", 1.35*cm, 2.65*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* deadLayerBack_log 
    = new G4LogicalVolume(deadLayerBack_tubs, GeLi, "deadLayerBack_log", 0, 0, 0);
  deadLayerBack_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -2.55*cm), deadLayerBack_log, "deadLayerBack", crystal_log, false, 0);
  
  //                     --- dead layer around the hole                                             material - Ge+Boron !!!!!!
  
  G4Tubs* deadLayerHole_tubs = new G4Tubs("deadLayerHole_tubs", 0., 0.35003*cm, 1.40015*cm, 0, 2*M_PI);

  G4LogicalVolume* deadLayerHole_log 
    = new G4LogicalVolume(deadLayerHole_tubs, GeLi, "deadLayerHole_log", 0, 0, 0);
  deadLayerHole_log -> SetVisAttributes(GeLicolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -1.19985*cm), deadLayerHole_log, "deadLayerHole", crystal_log, false, 0);
  
  //                         --- Hole in crystal

  G4Tubs* detHole_tubs = new G4Tubs("detHole_tubs", 0., 0.35*cm, 1.4*cm, 0, 2*M_PI);

  G4LogicalVolume* detHole_log 
    = new G4LogicalVolume(detHole_tubs, Vacuum, "detHole_log", 0, 0, 0);
  detHole_log -> SetVisAttributes(Vacuumcolour);

  new G4PVPlacement(0, G4ThreeVector(0., 0., -0.00015*cm), detHole_log, "detHole", deadLayerHole_log, false, 0);
    
  
  // ---------------------------------------------------------------------------------------
  // ---  THE SAMPLES
  // ---------------------------------------------------------------------------------------
  
  G4VSolid* sample_shape=0;
  
  if (G4Material::GetMaterial(sample_mat_name))
    sample_mat = G4Material::GetMaterial(sample_mat_name);
  else 
    {
      MGLog(error) << "The material called " << sample_mat_name << " does not exist" << endlog;
      MGLog(error) << "Please check and run MaGe again!" << endlog;
      G4Exception("GEBrunoDetector::ConstructDetector()","err001",FatalException,"Exit MaGe");
    }



 if (sample_geom=="sbox")
    {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

  // --- standard cylindrical box (cover side facing detector) , z-position of the sample can be adjusted by box_width parameter

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap_log  = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+0.05*cm), detplasticcap_log, "detplasticcap", inNitrogen_log, false, 0);


  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+2.975*cm), detplasticcap1_log, "detplasticcap1", inNitrogen_log, false, 0);


  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+1.5*cm), detplasticshell_log, "detplasticshell", inNitrogen_log, false, 0);


  // cylinder sample inside the standard box (default - filling the volume)
  sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);  

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_width+9.68*cm+1.5*cm), sample_log, "sample", inNitrogen_log, false, 0);

    }
 else if (sample_geom=="liquid")
    {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

  // --- standard cylindrical box (cover side facing detector)_with liquid sample_

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+0.05*cm), detplasticcap_log, "detplasticcap", inNitrogen_log, false, 0);

  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap1_log 
    = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+2.975*cm), detplasticcap1_log, "detplasticcap1", inNitrogen_log, false, 0);

  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticshell_log 
    = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+1.5*cm), detplasticshell_log, "detplasticshell", inNitrogen_log, false, 0);

  // liquid filling the standard box up to level of "boxheight"
  
  G4VSolid* sample_body   = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);
  G4VSolid* sample_hole   = new G4Box("sample_hole", tube_outrad+0.1*cm, tube_outrad, tube_length/2+0.1*cm);
  sample_shape = new G4SubtractionSolid("sample_shape", sample_body, sample_hole, 0, G4ThreeVector(0., 2*tube_outrad-box_height,0.));

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+1.5*cm), sample_log, "sample", inNitrogen_log, false, 0);
    }
   else if (sample_geom=="none")
    {
      //---- for point-like sources
    }
    else if (sample_geom=="box")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

      sample_shape = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+box_thickness/2), sample_log, "sample", inNitrogen_log, false, 0);
        }
      else if (sample_geom=="tube")
   {
// Keep this geometry unchanged to avoid confusion with macro writing!
// Use the sample geometry "custom" is for any non-standard samples that require coding.

sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm+tube_length/2), sample_log, "sample", inNitrogen_log, false, 0);

// the position of the middle of the BACK SIDE of the sample relative to the center of the front of detector cap!
// e.g. at position 0 0 0, the sample is just in front of the cap, touching it
   }



 else if (sample_geom=="custom")
   {
// the sample geometry "custom" is for any non-standard samples that require coding.



// -------------------------------------- SAMPLE 0221-0223: Erbium sample (from MPIK-EBIT) --------------------------- //
 
 	//Small pieces of erbium placed directly at the end cap of the detector
	//Simulated volume: small cylindrical piece of erbium

 sample_shape = new G4Tubs("sample_shape", 0.0*cm, 0.3*cm, 0.075*cm, 0, 2*M_PI);
 G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
 sample_log -> SetVisAttributes(Sample_Colour);

 new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+0.1*cm), sample_log, "sample", inNitrogen_log, false, 0);  


   }
  else if (sample_geom=="twobox")
    {
      //---for point like sources, two boxes away from detector (strong sources)
        // --- standard cylindrical box (cover side facing detector) - at Monel Housing

  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap_log 
    = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 9.73*cm), detplasticcap_log, "detplasticcap", inNitrogen_log, false, 0);

  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticcap1_log 
    = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 12.655*cm), detplasticcap1_log, "detplasticcap1", inNitrogen_log, false, 0);

  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.4*cm, 0, 2*M_PI);
  G4LogicalVolume* detplasticshell_log 
    = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 11.18*cm), detplasticshell_log, "detplasticshell", inNitrogen_log, false, 0);

  // --- one more standard box between detector and sample  - for strong sources measurement
    
  new G4PVPlacement(0, G4ThreeVector(0., 0., 12.8*cm), 
			detplasticcap_log, "detplasticcap2", inNitrogen_log, false, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 14.25*cm), 
			detplasticshell_log, "detplasticshell2", inNitrogen_log, false, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 15.725*cm), 
			detplasticcap1_log, "detplasticcap3", inNitrogen_log, false, 0);

  // --- the housing of the point like source :

sample_shape = new G4Box("sample_box", box_width/2, box_height/2, box_thickness/2);

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm), sample_log, "sample", inNitrogen_log, false, 0);
    }
  


 /*********************** old geometry definitions *************************************
  
  // --- standard cylindrical box (cover side facing detector) , z-position of the sample can be adjusted by box_width parameter
  
  G4Tubs* detplasticcap_tubs = new G4Tubs("detplasticcap_tubs", 0.*cm, 3.81*cm, 0.05*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap_log  = new G4LogicalVolume(detplasticcap_tubs, Acrylic, "detplasticcap_log", 0, 0, 0);
  detplasticcap_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+0.05*cm), detplasticcap_log, "detplasticcap", inNitrogen_log, false, 0);


  G4Tubs* detplasticcap1_tubs = new G4Tubs("detplasticcap1_tubs", 0.*cm, 3.81*cm, 0.075*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticcap1_log  = new G4LogicalVolume(detplasticcap1_tubs, Acrylic, "detplasticcap1_log", 0, 0, 0);
  detplasticcap1_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+3.275*cm), detplasticcap1_log, "detplasticcap1", inNitrogen_log, false, 0);


  G4Tubs* detplasticshell_tubs = new G4Tubs("detplasticshell_tubs", 3.65*cm, 3.81*cm, 1.55*cm, 0, 2*M_PI);

  G4LogicalVolume* detplasticshell_log  = new G4LogicalVolume(detplasticshell_tubs, Acrylic, "detplasticshell_log", 0, 0, 0);
  detplasticshell_log -> SetVisAttributes(Acryliccolour);

  new G4PVPlacement(0, G4ThreeVector(0, 0, s_z+9.68*cm+1.65*cm), detplasticshell_log, "detplasticshell", inNitrogen_log, false, 0);


  // cylinder sample inside the standard box (default - filling the volume)
  sample_shape = new G4Tubs("sample_tube", tube_innrad, tube_outrad, tube_length/2, 0, tube_angle);  

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+box_width+9.68*cm+1.65*cm), sample_log, "sample", inNitrogen_log, false, 0);

    } 


 {
      // here define any other sample geometry - feel free to replace contents

      // The empty NPL bottle
G4Tubs* NPLbottle_tubs = new G4Tubs("NPLbottle_tubs", 3.5*cm, 3.62*cm, 6.25*cm, 0, 2*M_PI);

G4LogicalVolume* NPLbottle_log = new G4LogicalVolume(NPLbottle_tubs, PE, "NPLbottle_log", 0, 0, 0);
NPLbottle_log -> SetVisAttributes(Acryliccolour);

//position : 1.2mm from detector window to account for plastic wrap in between;
new G4PVPlacement(0, G4ThreeVector(0., 0., 17.05*cm), NPLbottle_log, "NPLbottle", inNitrogen_log, false, 0);


G4Tubs* NPLbottleTop_tubs = new G4Tubs("NPLbottleTop_tubs", 0., 3.62*cm, 0.25*cm, 0, 2*M_PI);

G4LogicalVolume* NPLbottleTop_log = new G4LogicalVolume(NPLbottleTop_tubs, PE, "NPLbottleTop_log", 0, 0, 0);
NPLbottleTop_log -> SetVisAttributes(Acryliccolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 23.55*cm), NPLbottleTop_log, "NPLbottleTop", inNitrogen_log, false, 0);

//bottom of the bottle
G4VSolid* bottom_sphere   = new G4Sphere("bottom_sphere", 3.875*cm, 3.975*cm, 0.,twopi, 0.,twopi);
G4VSolid* cut_box   = new G4Box("cut_box", 10.*cm, 10.*cm, 10.*cm);
G4VSolid* bottom_first = new G4SubtractionSolid("bottom_first", bottom_sphere, cut_box, 0, G4ThreeVector(0., 0.,8.358*cm));
G4VSolid* bottom_shape = new G4SubtractionSolid("bottom_shape", bottom_first, cut_box, 0, G4ThreeVector(0., 0.,-12.642*cm));

G4VSolid* bottom2_sphere   = new G4Sphere("bottom2_sphere", 7.25*cm, 7.4*cm, 0.,twopi, 0.,twopi);
G4VSolid* bottom2_shape = new G4SubtractionSolid("bottom2_shape", bottom2_sphere, cut_box, 0, G4ThreeVector(0., 0.,-3.2*cm));

G4VSolid* NPLbottom_shape = new G4UnionSolid("NPLbottom_shape", bottom_shape, bottom2_shape, 0, G4ThreeVector(0., 0.,-9.442*cm));


G4LogicalVolume* NPLbottleBottom_log = new G4LogicalVolume(NPLbottom_shape, PE, "NPLbottleBottom_log", 0, 0, 0);
NPLbottleTop_log -> SetVisAttributes(Acryliccolour);
//NPLbottleBottom_log -> SetVisAttributes(Testcolour);

new G4PVPlacement(0, G4ThreeVector(0., 0., 12.442*cm), NPLbottleBottom_log, "NPLbottleBottom", inNitrogen_log, false, 0);


//contents : a surface layer on the wall
G4VSolid* sample_tubs   = new G4Tubs("sample_tube", 3.49*cm, 3.5*cm, 6.25*cm, 0, 2*M_PI);

//a surface layer on bottom
G4VSolid* Sbottom_sphere   = new G4Sphere("Sbottom_sphere", 3.865*cm, 3.875*cm, 0.,twopi, 0.,twopi);
G4VSolid* Sbottom_first = new G4SubtractionSolid("Sbottom_first", Sbottom_sphere, cut_box, 0, G4ThreeVector(0., 0.,8.358*cm));
G4VSolid* Sbottom_shape = new G4SubtractionSolid("Sbottom_shape", Sbottom_first, cut_box, 0, G4ThreeVector(0., 0.,-12.5*cm));

G4VSolid* Sbottom2_sphere   = new G4Sphere("Sbottom2_sphere", 7.4*cm, 7.41*cm, 0.,twopi, 0.,twopi);
G4VSolid* Sbottom2_shape = new G4SubtractionSolid("Sbottom2_shape", Sbottom2_sphere, cut_box, 0, G4ThreeVector(0., 0.,-3.15*cm));

G4VSolid* sample_first = new G4UnionSolid("sample_first", sample_tubs, Sbottom_shape, 0, G4ThreeVector(0.,0.,-4.608*cm));
sample_shape = new G4UnionSolid("sample_shape", sample_first, Sbottom2_shape, 0, G4ThreeVector(0., 0.,-14.05*cm));

s_z = 7.37*cm;

G4LogicalVolume* sample_log = new G4LogicalVolume(sample_shape, sample_mat, "sample_log", 0, 0, 0);  
  sample_log -> SetVisAttributes(Sample_Colour);

new G4PVPlacement(0, G4ThreeVector(s_x, s_y, s_z+9.68*cm), sample_log, "sample", inNitrogen_log, false, 0);
    }
 */
     else
    {
      MGLog(error) << "The shape is not defined " << endlog;
      MGLog(fatal) << endlog;
      G4Exception("GEBrunoDetector::ConstructDetector()","err002",FatalException,"Check the shape type");
    }


  
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

