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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-26-2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 09-04-2007, Added definition of SensitiveRegion (not done anymore in the 
 *             base class), Luciano
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh" 
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"


#include "gerdageometry/GEGeometrySD.hh"

#include "munichteststand/GEHadesScintillator.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEHadesScintillator::GEHadesScintillator() : MGGeometryDetector("")
{

  // set pointer to NULL 

  physical_gedetector1 = 0; 
  physical_gedetector2 = 0; 
  physical_scintillator1 = 0; 
  physical_scintillator2 = 0; 
  physical_steelplate1 = 0; 
  physical_steelplate2 = 0; 
  physical_steelplate3 = 0; 
  physical_steelplate4 = 0; 
  physical_leadplate = 0; 
  physical_source = 0; 
  physical_muondisk = 0; 
  sensitiveRegion = 0;
}

// --------------------------------------------------------------------------- 

GEHadesScintillator::~GEHadesScintillator()
{

  // delete physical volumes 

  if (!physical_gedetector1) 
    delete physical_gedetector1; 

  if (!physical_gedetector2)
    delete physical_gedetector2; 

  if (!physical_scintillator1)
    delete physical_scintillator1; 

  if (!physical_scintillator2)
    delete physical_scintillator2; 

  if (!physical_steelplate1)
    delete physical_steelplate1; 

  if (!physical_steelplate2)
    delete physical_steelplate2; 

  if (!physical_steelplate3)
    delete physical_steelplate3; 

  if (!physical_steelplate4)
    delete physical_steelplate4; 

  if (!physical_leadplate)
    delete physical_leadplate; 

  if (!physical_muondisk)
    delete physical_muondisk; 

  if (!physical_source)
    delete physical_source; 

  if (sensitiveRegion)
    delete sensitiveRegion;

} 

// --------------------------------------------------------------------------- 

void GEHadesScintillator::ConstructDetector()
{

  // -------------------------------------------------------------------------
  // sensitive region  
  // -------------------------------------------------------------------------
  sensitiveRegion = new G4Region("SensitiveRegion");
  MGLog(trace) << "Sensitive Region registered" << endlog;

  // -------------------------------------------------------------------------
  // materials 
  // -------------------------------------------------------------------------

  G4Material* air        = G4Material::GetMaterial("Air");
  G4Material* plastic    = G4Material::GetMaterial("PlasticScintillator");
  G4Material* germanium  = G4Material::GetMaterial("NaturalGe"); 

  // -------------------------------------------------------------------------
  // attributes 
  // -------------------------------------------------------------------------
  
  G4Colour red (1.0,  0.0,  0.0);
  G4Colour blue(0.0,  0.0,  0.7);
  G4Colour grey(0.7,  0.7,  0.7); 

  G4VisAttributes* visatt_scintillator = new G4VisAttributes(blue); 
  visatt_scintillator -> SetVisibility(true); 
  visatt_scintillator -> SetForceSolid(true); 

  G4VisAttributes* visatt_gedetector = new G4VisAttributes(red); 
  visatt_gedetector -> SetVisibility(true); 
  visatt_gedetector -> SetForceSolid(true); 

  G4VisAttributes* visatt_steelplate = new G4VisAttributes(red); 
  visatt_steelplate -> SetVisibility(true); 
  visatt_steelplate -> SetForceSolid(true); 

  G4VisAttributes* visatt_leadplate = new G4VisAttributes(grey); 
  visatt_leadplate -> SetVisibility(true); 
  visatt_leadplate -> SetForceSolid(true); 

  G4VisAttributes* visatt_source = new G4VisAttributes(red); 
  visatt_source -> SetVisibility(true); 
  visatt_source -> SetForceSolid(true); 

  // -------------------------------------------------------------------------
  // sizes and measures 
  // -------------------------------------------------------------------------

  G4double gedetector1_height = 5.0 * cm; 
  G4double gedetector2_height = 5.0 * cm; 
  G4double gedetector1_distancetosource = 2.0 * cm; 
  G4double gedetector2_distancetosource = 1.0 * cm; 

  G4double scintillator_height = 2.54 * cm; 
  G4double scintillator_length = 80.0 * cm; 
  G4double scintillator_distancetosource = 50.0 * cm; 

  G4double steelplate_height = 2.0 * mm; 
  G4double steelplate_length = scintillator_length; 

  G4double leadplate_height = 6.0 * mm; 
  G4double leadplate_length = steelplate_length; 

  G4double gap = 1.0 * cm; 

  // -------------------------------------------------------------------------
  // logical and physical volumes 
  // -------------------------------------------------------------------------

  // the experimental hall 

  G4Box* box_hall = new G4Box("box_hall", 0.5 * 10.0 * m, 0.5 * 10.0 * m, 0.5 * 10.0 * m); 

  G4LogicalVolume* logical_hall = new G4LogicalVolume(box_hall, air, "logical_hall"); 
  logical_hall -> SetVisAttributes(G4VisAttributes::Invisible); 

  // the muon disk 

  // the germanium detector 1 

  G4Tubs* tubs_gedetector1 = new G4Tubs("tubs_gedetector1", 
					0.5 * cm, 
					2.5 * cm, 
					0.5 * gedetector1_height, 
					0.0 * deg, 
					360.0 * deg); 

  G4LogicalVolume* logical_gedetector1 = new G4LogicalVolume(tubs_gedetector1, germanium, "logical_gedetector1"); 
  logical_gedetector1 -> SetVisAttributes(visatt_gedetector); 

  sensitiveRegion -> AddRootLogicalVolume(logical_gedetector1);

  physical_gedetector1 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.5 * gedetector1_height + gedetector1_distancetosource), 
					   logical_gedetector1, 
					   "gedetector1", 
					   logical_hall, 
					   false, 
					   0); 

  // the germanium detector 2

  G4Tubs* tubs_gedetector2 = new G4Tubs("tubs_gedetector2", 
					0.5 * cm, 
					2.5 * cm, 
					0.5 * gedetector2_height, 
					0.0 * deg, 
					360.0 * deg); 

  G4LogicalVolume* logical_gedetector2 = new G4LogicalVolume(tubs_gedetector2, germanium, "logical_gedetector2"); 
  logical_gedetector2 -> SetVisAttributes(visatt_gedetector); 

  sensitiveRegion -> AddRootLogicalVolume(logical_gedetector1);

  physical_gedetector2 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -(0.5 * gedetector2_height + gedetector2_distancetosource)), 
					   logical_gedetector2, 
					   "gedetector2", 
					   logical_hall, 
					   false, 
					   1); 

  // the source

  G4Tubs* tubs_source = new G4Tubs("tubs_source", 
				   0.0 * cm, 
				   0.5 * cm, 
				   0.5 * 0.2 * cm, 
				   0.0 * deg, 
				   360.0 * deg); 

  G4LogicalVolume* logical_source = new G4LogicalVolume(tubs_source, plastic, "logical_source"); 

  logical_source -> SetVisAttributes(visatt_source); 
  
  physical_source = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), 
				      logical_source, 
				      "source", 
				      logical_hall, 
				      false, 
				      0); 

  // the lead plate 

  G4Box* box_leadplate = new G4Box("box_leadplate", 
				   0.5 * leadplate_length, 
				   0.5 * leadplate_length, 
				   0.5 * leadplate_height); 
  
  G4LogicalVolume* logical_leadplate = new G4LogicalVolume(box_leadplate, 
							   plastic, 
							   "logical_leadplate"); 
  logical_leadplate -> SetVisAttributes(visatt_leadplate); 
  
  physical_leadplate = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource), 
					 logical_leadplate, 
					  "leadplate", 
					 logical_hall, 
					 false, 
					 0); 

  // the steel plates 

  G4Box* box_steelplate = new G4Box("box_steelplate", 
				   0.5 * steelplate_length, 
				   0.5 * steelplate_length, 
				   0.5 * steelplate_height); 
  
  G4LogicalVolume* logical_steelplate = new G4LogicalVolume(box_steelplate, 
							   plastic, 
							   "logical_steelplate"); 
  logical_steelplate -> SetVisAttributes(visatt_steelplate); 
  
  physical_steelplate1 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource +
							    0.5 * leadplate_height + gap + 1.5 * steelplate_height + scintillator_height), 
					   logical_steelplate, 
					   "steelplate1", 
					   logical_hall, 
					   false, 
					   0);

  physical_steelplate2 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource +
							    0.5 * (leadplate_height + steelplate_height)), 
					   logical_steelplate, 
					   "steelplate2", 
					   logical_hall, 
					   false, 
					   1);
  
  physical_steelplate3 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource +
							    - 0.5 * (leadplate_height + steelplate_height)), 
					   logical_steelplate, 
					   "steelplate3", 
					   logical_hall, 
					   false, 
					   2);
  
  physical_steelplate4 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource +
							    - (0.5 * leadplate_height + gap + 1.5 * steelplate_height + scintillator_height)), 
					   logical_steelplate, 
					   "steelplate4", 
					   logical_hall, 
					   false, 
					   3);
  
  
  // the scintillator plates 
  
  G4Box* box_scintillator = new G4Box("box_scintillator", 
				      0.5 * scintillator_length, 
				      0.5 * scintillator_length, 
				      0.5 * scintillator_height); 

  G4LogicalVolume* logical_scintillator = new G4LogicalVolume(box_scintillator, 
							      plastic, 
							      "logical_scintillator"); 
  sensitiveRegion -> AddRootLogicalVolume(logical_scintillator);

  logical_scintillator -> SetVisAttributes(visatt_scintillator); 
  
  physical_scintillator1 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource + 
							      0.5 * (scintillator_height + leadplate_height) + steelplate_height + gap), 
					     logical_scintillator, 
					     "PS3", 
					     logical_hall, 
					     false, 
					     0); 

  physical_scintillator1 = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, scintillator_distancetosource + 
							      - 0.5 * (scintillator_height + leadplate_height) - steelplate_height - gap), 
					     logical_scintillator, 
					     "PS4", 
					     logical_hall, 
					     false, 
					     1); 

  // -------------------------------------------------------------------------
  // sensitive region 
  // -------------------------------------------------------------------------

  // get SD manager pointer 

  G4SDManager* SDmanager = G4SDManager::GetSDMpointer();

  // create sensitive detectors 

  GEGeometrySD* SDgedetector   = new GEGeometrySD("/mydet/gerda/gecrystal"); 
  GEGeometrySD* SDscintillator = new GEGeometrySD("/mydet/gerda/scintPlate"); 

  // add sensitive detectors to SD manager 

  SDmanager -> AddNewDetector(SDgedetector); 
  SDmanager -> AddNewDetector(SDscintillator); 

  // set sensitive detectors 

  logical_gedetector1 -> SetSensitiveDetector(SDgedetector); 
  logical_gedetector2 -> SetSensitiveDetector(SDgedetector); 
  logical_scintillator -> SetSensitiveDetector(SDscintillator); 

  // set detector mother volume 

  this -> SetDetectorLogical(logical_hall); 

  // set detector name 

  this -> SetDetectorName("HadesScintillator"); 

} 

// --------------------------------------------------------------------------- 

