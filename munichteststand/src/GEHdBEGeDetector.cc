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
// $Id: GEHdBEGeDetector.cc,v 1.1 2009-05-14 21:19:12 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  GEHdBEGeDetector.cc
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

#include "munichteststand/GEHdBEGeDetector.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GEHdBEGeDetector::GEHdBEGeDetector() : MGGeometryDetector("")
{;}

//---------------------------------------------------------------------------//

GEHdBEGeDetector::~GEHdBEGeDetector()
{;}

//---------------------------------------------------------------------------//

void GEHdBEGeDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  /*G4Material* Vacuum =*/ G4Material::GetMaterial("Vacuum");
  /*G4Material* Brass =*/ G4Material::GetMaterial("Brass");
  /*G4Material* ProportionalGas =*/ G4Material::GetMaterial("ProportionalGas");
  /*G4Material* N2Gas =*/ G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  /*G4Material* Pb =*/ G4Material::GetMaterial("MetalLead");
  /*G4Material* Fe =*/ G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  /*G4Material* Vespel =*/ G4Material::GetMaterial("Vespel");
  /*G4Material* Acrylic =*/ G4Material::GetMaterial("Acrylic");
  /*G4Material* PE =*/ G4Material::GetMaterial("PE");
  /*G4Material* Teflon =*/ G4Material::GetMaterial("Teflon");
  G4Material* Air = G4Material::GetMaterial("Air");

  /*G4Material* Quartz =*/ G4Material::GetMaterial("Quartz");

  
 // Visualization attributes
  //G4VisAttributes * Brasscolour= new G4VisAttributes( G4Colour(226/255. ,163/255. ,29/255. ));  //orange-brown

  //G4VisAttributes * Sample_Colour = new G4VisAttributes(  G4Colour(255/255. ,0/255. ,12/255. ));  //red
  //Sample_Colour -> SetForceSolid(true);

  //G4VisAttributes * Vacuumcolour = new G4VisAttributes( G4Colour(175/255. ,164/255. ,255/255. ));  //sky-blue

  /*G4VisAttributes * ProportionalGascolour=*/ new G4VisAttributes( G4Colour(154/255. ,237/255. ,193/255. ));  //blue-green

  /*G4VisAttributes * Pbcolour =*/ new G4VisAttributes( G4Colour(171/255. ,171/255. ,195/255. ));  //gray

  G4VisAttributes * Cucolour= new G4VisAttributes( G4Colour(255/255. ,71/255. ,33/255. ));  //red

  G4VisAttributes * Gecolour= new G4VisAttributes( G4Colour(133/255. ,133/255. ,133/255. ));  //dark grey
  //Gecolour -> SetForceSolid(true);

  G4VisAttributes * GeLicolour= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));  //almost black ( -> 'dead'...)

  //G4VisAttributes * Fecolour = new G4VisAttributes( G4Colour(175/255. ,157/255. ,189/255. ));  //redish-blue gray

  /*G4VisAttributes * N2Gascolour =*/ new G4VisAttributes( G4Colour(150/255. ,150/255. ,255/255. ));   //light blue

  G4VisAttributes * Alcolour= new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));   //light gray

  //G4VisAttributes * Vespelcolour= new G4VisAttributes( G4Colour(224/255. ,224/255. ,190/255. ));  //yellow-gray

  /*G4VisAttributes * Acryliccolour=*/ new G4VisAttributes( G4Colour(231/255. ,217/255. ,240/255. ));  //light blue-grey
 
  

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


  /*

  //------------ collimator with stepped hole ----------------------

  G4VSolid*solidKolim = new G4Box("solidKolim", 20*cm, 20*cm, 10*cm);        //collimator

  //G4VSolid*hole_cons = new G4Cons("hole_cons", 0, 5*mm, 0, 1.5*mm, 10*cm, 0, 2*M_PI);

  G4Tubs* Cilindr1 = new G4Tubs("Cilindr1", 0, 1.5*mm, 21*cm, 0, 360.*deg);    //hole 

  G4VSolid*collim_shape1 = new G4SubtractionSolid("collim_shape1", solidKolim, Cilindr1, 0, G4ThreeVector(0., 0., -10*cm));

  G4Tubs* Cilindr2 = new G4Tubs("Cilindr2", 0, 2.*mm, 15*cm, 0, 360.*deg);    //hole 

  G4VSolid*collim_shape2 = new G4SubtractionSolid("collim_shape2", collim_shape1, Cilindr2, 0, G4ThreeVector(0., 0., -10*cm));

  G4Tubs* Cilindr3 = new G4Tubs("Cilindr3", 0, 2.5*mm, 10*cm, 0, 360.*deg);    //hole 

  G4VSolid*collim_shape3 = new G4SubtractionSolid("collim_shape3", collim_shape2, Cilindr3, 0, G4ThreeVector(0., 0., -10*cm));

  G4Tubs* Cilindr4 = new G4Tubs("Cilindr4", 0, 3*mm, 5*cm, 0, 360.*deg);    //hole 

  G4VSolid*collim_shape4 = new G4SubtractionSolid("collim_shape4", collim_shape3, Cilindr4, 0, G4ThreeVector(0., 0., -10*cm));

  G4LogicalVolume *logiclKolim = new G4LogicalVolume(collim_shape4, Pb, "logiclKolim");
  logiclKolim -> SetVisAttributes(Pbcolour);
   G4PVPlacement *physilKolim = new G4PVPlacement(0, G4ThreeVector(0,0,11*cm), logiclKolim,
					       "physilKolim", experimentalHall_log, false, 0);
  */

   /*           lead shield

  G4Box *solidShield = new G4Box("solidShield", 6*cm, 6*cm, 2.5*cm);        //Pb 
  G4LogicalVolume *logiclShield = new G4LogicalVolume(solidShield, Pb, "logiclShield");
  logiclShield -> SetVisAttributes(Pbcolour);
  G4PVPlacement *physilShield = new G4PVPlacement(0, G4ThreeVector(0,0,7.5*cm), logiclShield,
					       "physilShield", experimentalHall_log, false, 0);

   */
   /*             source cover

  G4Tubs* Sourcecover = new G4Tubs("Sourcecover", 0, 1*cm, 1.575*mm, 0, 360.*deg);    //Al cover
  G4LogicalVolume *logicSourcecover = new G4LogicalVolume(Sourcecover, Acrylic, "logicSourcecover");
  logicSourcecover -> SetVisAttributes(Acryliccolour);
  G4PVPlacement *physiSourcecover = new G4PVPlacement(0, G4ThreeVector(0,0,591.425*mm), logicSourcecover,
					       "physiSourcecover", experimentalHall_log, false, 0);	
  
   */

  // ----------- detector housing ----------------------------------

					   
  G4Tubs* Alwall = new G4Tubs("Alwall", 49.3*mm, 50.8*mm, 41.5*mm, 0, 360.*deg);    //Al wall
  G4LogicalVolume *logicAlwall = new G4LogicalVolume(Alwall, Al, "logicAlwall");
  logicAlwall -> SetVisAttributes(Alcolour);
  /*G4PVPlacement *physilAlwall =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-45*mm), logicAlwall,
					       "physilAlwall", experimentalHall_log, false, 0);	

  G4Tubs* Alcover = new G4Tubs("Alcover", 0, 50.8*mm, 0.75*mm, 0, 360.*deg);    //Al cover
  G4LogicalVolume *logicAlcover = new G4LogicalVolume(Alcover, Al, "logicAlcover");
  logicAlcover -> SetVisAttributes(Alcolour);
  /*G4PVPlacement *physilAlcover =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-2.75*mm), logicAlcover,
					       "physilAlcover", experimentalHall_log, false, 0);	
						   

  G4Tubs* Sh1 = new G4Tubs("Sh1", 40.75*mm, 42.25*mm, 0.75*mm, 0, 360.*deg);    //Cu shield 1
  G4LogicalVolume *logiclSh1 = new G4LogicalVolume(Sh1, Cu, "logiclSh1");
  logiclSh1 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh1 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-10.5*mm), logiclSh1,
					       "physilSh1", experimentalHall_log, false, 0);
						   
  G4Tubs* Sh2 = new G4Tubs("Sh2", 40.75*mm, 44.25*mm, 4.25*mm, 0, 360.*deg);    //Cu shield 2
  G4LogicalVolume *logiclSh2 = new G4LogicalVolume(Sh2, Cu, "logiclSh2");
  logiclSh2 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh2 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-15.5*mm), logiclSh2,
					       "physilSh2", experimentalHall_log, false, 0);					   
						   
  G4Tubs* Sh3 = new G4Tubs("Sh3", 40.75*mm, 42.25*mm, 1.5*mm, 0, 360.*deg);    //Cu shield 3
  G4LogicalVolume *logiclSh3 = new G4LogicalVolume(Sh3, Cu, "logiclSh3");
  logiclSh3 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh3 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-21.25*mm), logiclSh3,
					       "physilSh3", experimentalHall_log, false, 0);
						   
  G4Tubs* Sh4 = new G4Tubs("Sh4", 40.75*mm, 44.25*mm, 3.5*mm, 0, 360.*deg);    //Cu shield 4
  G4LogicalVolume *logiclSh4 = new G4LogicalVolume(Sh4, Cu, "logiclSh4");
  logiclSh4 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh4 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-26.25*mm), logiclSh4,
					       "physilSh4", experimentalHall_log, false, 0);		

  G4Tubs* Sh5 = new G4Tubs("Sh5", 40.75*mm, 42.25*mm, 18.5*mm, 0, 360.*deg);    //Cu shield 5
  G4LogicalVolume *logiclSh5 = new G4LogicalVolume(Sh5, Cu, "logiclSh5");
  logiclSh5 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh5 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-48.25*mm), logiclSh5,
					       "physilSh5", experimentalHall_log, false, 0);	
						   
  G4Tubs* Sh6 = new G4Tubs("Sh6", 0, 41.5*mm, 5*mm, 0, 360.*deg);    //Cu shield 6
  G4LogicalVolume *logiclSh6 = new G4LogicalVolume(Sh6, Cu, "logiclSh6");
  logiclSh6 -> SetVisAttributes(Cucolour);
  /*G4PVPlacement *physilSh6 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-71.75*mm), logiclSh6,
					       "physilSh6", experimentalHall_log, false, 0);



  // ------------------- detector with dead layer -----------------------

  double dead_layer = 0.42*mm;

  G4Tubs* solidDet = new G4Tubs("solidDet", 0*mm, 40.5*mm, 16*mm, 0, 360.*deg);   // detector
  G4LogicalVolume *logicDet = new G4LogicalVolume(solidDet, Ge, "logicDet");
  logicDet -> SetVisAttributes(Gecolour);
  /*G4PVPlacement *physiDet =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-24.25*mm), logicDet,
					       "physiDet", experimentalHall_log, false, 0);	

  G4Tubs* SolidDeadLayer = new G4Tubs("SolidDeadLayer", 0*mm, 40.5*mm-dead_layer, dead_layer/2, 0, 360.*deg);   // dead layer up
  G4LogicalVolume *logicDeadLayer = new G4LogicalVolume(SolidDeadLayer, GeLi, "logicDeadLayer");
  logicDeadLayer -> SetVisAttributes(GeLicolour);
  /*G4PVPlacement *physiDeadLayer =*/ new G4PVPlacement(0, G4ThreeVector(0,0,16*mm-dead_layer/2), logicDeadLayer,
					       "physiDeadLayer", logicDet, false, 0);	
	
  G4Tubs* TubDeadLayer = new G4Tubs("TubDeadLayer", 40.5*mm-dead_layer, 40.5*mm, 16*mm, 0, 360.*deg);   // dead layer  around
  G4LogicalVolume *logicTubDeadLayer = new G4LogicalVolume(TubDeadLayer, GeLi, "logicTubDeadLayer");
  logicTubDeadLayer -> SetVisAttributes(GeLicolour);
  /*G4PVPlacement *physiTubDeadLayer =*/ new G4PVPlacement(0, G4ThreeVector(0,0,0), logicTubDeadLayer,
					       "physiTubDeadLayer", logicDet, false, 0);						   
		
  double dead_layer2 = 0.51*mm; // (corresponding to 156.8 cm^2 active volume)
				   
  G4Tubs* SolidDeadLayer2 = new G4Tubs("SolidDeadLayer2", 0*mm, 40.5*mm-dead_layer, dead_layer2/2, 0, 360.*deg);   // dead layer down
  G4LogicalVolume *logicDeadLayer2 = new G4LogicalVolume(SolidDeadLayer2, GeLi, "logicDeadLayer2");
  logicDeadLayer2 -> SetVisAttributes(GeLicolour);
  /*G4PVPlacement *physiDeadLayer2 =*/ new G4PVPlacement(0, G4ThreeVector(0,0,-16*mm+dead_layer2/2), logicDeadLayer2,
					       "physiDeadLayer2", logicDet, false, 0);	
  
  
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  
  G4SDManager* SDman   =  G4SDManager::GetSDMpointer();
  G4String CrystalSDname      = "/mydet/gerda/gecrystal";
  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);
  SDman->AddNewDetector(CrystalSD);

  // BEGe
  logicDet->SetSensitiveDetector(CrystalSD);
    
  SetDetectorLogical(experimentalHall_log);
  SetDetectorName("ExperimentalHall");
 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
