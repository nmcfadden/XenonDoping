//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
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
// $Id: MJGeometryLArGe.cc,v 1.7 2007-02-21 08:11:52 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryLArGe.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: dimarcom
 * CONTACT: 
 * FIRST SUBMISSION: Tue Nov  9 10:38:35 PST 2004
 * 
 * REVISION:
 *
 * 11-09-2004, created, dimarcom
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 01-06-2004, added dead layer and groove, dimarcom
 *
 */
//---------------------------------------------------------------------------//

// from Geant4 example N01
#include "G4Material.hh"                  
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"     
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"

//  MJ headers
#include "mjgeometry/MJGeometryLArGe.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//

MJGeometryLArGe::MJGeometryLArGe(G4String serNum):
  MGGeometryDetector(serNum), // LArGe not in database, will that work ?
  /*groove_log(0),*/ deadlayer_log(0), crystal_log(0), cryostat_log(0), 
  /*groove_phys(0),*/ deadlayer_phys(0), crystal_phys(0)//, cryostat_phys(0)
{;}

MJGeometryLArGe::~MJGeometryLArGe()
{;}
//---------------------------------------------------------------------------//

void MJGeometryLArGe::ConstructDetector()
{
  //------------------------------ materials

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name;

  G4Material* LAr = G4Material::GetMaterial("LAr");
  if (!LAr) {
    density = 1.4*g/cm3;
    a = 39.95*g/mole;
    LAr = new G4Material(name="LAr",z=18.,a,density);
    G4cout << "Material: " << LAr->GetName() << " created" << endl;
  }

  /*  density = 0.002*g/cm3;
      a = 39.95*g/mole;
      G4Material* ArGas = new G4Material(name="ArGas",z=18.,a,density); */

  G4Material *crystalMaterial = G4Material::GetMaterial("Germanium-Nat");
 
// ------------------------------------ dimensions

  G4double cryostat_radius = 10.0 * cm;
  G4double cryostat_height = 30.0 * cm;
//  G4double cryostat_radius = 100.0 * cm;
//  G4double cryostat_height = 300.0 * cm;
  G4cout << "cryostat_radius " << cryostat_radius / cm << " cm" << endl;
  G4cout << "cryostat_height " << cryostat_height / cm << " cm" << endl;

  G4double dead_layer = 0.3 *cm;    // PNNL LArGe
  G4cout << "dead layer " << dead_layer / cm << " cm" << endl;

  G4double groove_radius = 1 *cm;
  G4double groove_depth = 3 *cm;

  //  G4double crystal_radius = 2.06 *cm;    // 168g crystal
  //  G4double crystal_height = 2.36 *cm;    // 168g crystal
  //  G4double crystal_radius = 3.90 *cm;    // 2kg crystal
  //  G4double crystal_height = 7.81 *cm;    // 2kg crystal
  G4double crystal_radius = 2.6 *cm;    // PNNL LArGe
  G4double crystal_height = 5.3 *cm;    // PNNL LArGe
  G4cout << "crystal_radius " << crystal_radius / cm << " cm" << endl;
  G4cout << "crystal_height " << crystal_height / cm << " cm" << endl;

// ------------------------------------ cryostat

  G4Tubs *cryostat_cyl = new G4Tubs( "cryostat_cyl", 0, 
				     cryostat_radius, 
				     cryostat_height/2,
				     0*deg, 360*deg);
    theDetectorLogical = new G4LogicalVolume(cryostat_cyl,LAr,"theDetectorLogical"); 
//  cryostat_log = new G4LogicalVolume(cryostat_cyl,LAr,
//				     "cryostat_log"); 

 
  /* // no placement because equivalent to the World Volume (? correct ?)
     G4double cryostat_x = 0;
     G4double cryostat_y = 0;
     G4double cryostat_z = 0;

     cryostat_phys = new G4PVPlacement( 0, 
     G4ThreeVector(cryostat_x*cm,cryostat_y*cm,cryostat_z*cm), 
     cryostat_log, "cryostat_phys", 
     theDetectorLogical, false, 0 ); */

  // Visualization attrbutes for cryostat.
  G4VisAttributes *cryostatVisAtt = new G4VisAttributes(blue);
  cryostatVisAtt->SetVisibility( true );
  cryostatVisAtt->SetForceSolid( false );
//errore qui!!!
  theDetectorLogical->SetVisAttributes(cryostatVisAtt); 
//  cryostat_log->SetVisAttributes(cryostatVisAtt);
   
  // ------------------------------------ dead layer
  
  G4Tubs *deadlayer_cyl = new G4Tubs( "deadlayer_cyl",
				      crystal_radius - dead_layer, 
				      crystal_radius,
				      crystal_height/2, 
				      0*deg, 360*deg );

  G4double deadlayer_x = 0;
  G4double deadlayer_y = 0;
  G4double deadlayer_z = 0;
  deadlayer_log = new G4LogicalVolume(deadlayer_cyl, crystalMaterial,
				      "deadlayer_log");

  deadlayer_phys = new G4PVPlacement( 0, 
	    G4ThreeVector(deadlayer_x*cm, deadlayer_y*cm, deadlayer_z*cm), 
	    deadlayer_log, "GeDeadlayer", cryostat_log, false, 0 );
  SetDetectorName("dead"); //the default is "Detector"

  // Visualization attributes for the dead layer.
  G4VisAttributes *deadlayerVisAtt = new G4VisAttributes(red);
  deadlayerVisAtt->SetVisibility( true );
  deadlayerVisAtt->SetForceSolid( false );
  deadlayer_log->SetVisAttributes(deadlayerVisAtt);


  // ------------------------------------ groove
  
  G4Tubs *groove_cyl = new G4Tubs( "groove_cyl", 0*cm, 
				   groove_radius,
				   groove_depth/2 *cm, 
				   0*deg, 360*deg );
  G4ThreeVector groove_pos(0,0,crystal_height / 2 - groove_depth);

  // ------------------------------------ crystal
  
  G4Tubs *crystal_cyl = new G4Tubs( "crystal_cyl", 0*cm, 
				    crystal_radius - dead_layer, 
				    crystal_height/2 *cm, 
				    0*deg, 360*deg );

  G4SubtractionSolid *crystal_solid = new G4SubtractionSolid("crystal_solid", crystal_cyl, groove_cyl,0,groove_pos);

  crystal_log = new G4LogicalVolume(crystal_solid, crystalMaterial,
				    "crystal_log");

  G4double crystal_x = 0;
  G4double crystal_y = 0;
  G4double crystal_z = 0;
  crystal_phys = new G4PVPlacement( 0, 
	    G4ThreeVector(crystal_x*cm, crystal_y*cm, crystal_z*cm), 
	    crystal_log, "GeCrystal_phys", cryostat_log, false, 0 );

  SetDetectorName("LArGe"); //the default is "Detector"

  // Visualization attributes for crystal.
  G4VisAttributes *crystalVisAtt = new G4VisAttributes(red);
  crystalVisAtt->SetVisibility( true );
  crystalVisAtt->SetForceSolid( false );
  crystal_log->SetVisAttributes(crystalVisAtt);
  //  MGLog(routine) << "cylindrical crystal created out of: " << fMaterialName << endlog;


  /*
  //---------------- sensitive detector definition --------------------//
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String geCrystalSDname = "/mydet/gerda/gecrystal";
  GEGeometrySD * geCrystalSD = new GEGeometrySD(geCrystalSDname);
  SDman->AddNewDetector(geCrystalSD);
  theGermaniumCrystalActiveLogical->SetSensitiveDetector(geCrystalSD);
  */
   cout << "sono qui" << endl ;

} // end of constructor
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//




