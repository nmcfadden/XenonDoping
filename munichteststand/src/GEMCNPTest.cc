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
 * FIRST SUBMISSION: 01-18-2006
 *
 * The goal of this geometry is to cross-check the neutron tracking done by 
 * MaGe-Geant4 with a MCNP application written by Dano Kollar at MPI-Munich
 * REVISION:
 * 07-Mar-2006 Modified to study the background from (alpha,n) neutrons from 
 *             a steel cryostat shielded with 10 cm of copper
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
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "geometry/MGGeometryDetector.hh"
//#include "geometry/MGGeometryDetectorConstruction.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "munichteststand/GEMCNPTest.hh"

#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMCNPTest::GEMCNPTest() : 
  MGGeometryDetector("")
{;}

// --------------------------------------------------------------------------- 

GEMCNPTest::~GEMCNPTest()
{;}

// --------------------------------------------------------------------------- 

void GEMCNPTest::ConstructDetector()
{
  // construct airsphere
  G4double innerRadius = 0.*cm;
  G4double AirRadius = 800*cm;
  

  G4Sphere* airSphere = new G4Sphere("airSphere",
				     innerRadius,AirRadius,
				     0.,pi,0.,twopi); 
  G4LogicalVolume* airSphereLogical =
    new G4LogicalVolume(airSphere,G4Material::GetMaterial("Air"),
                        "airSphereLogical");

  G4double SteelRadius = 500*cm;
  G4double WaterRadius = 490*cm;
  G4double CopperRadius = 167*cm;
  G4double VacuumRadius = 162*cm;
  //Original setup:
  //G4double CopperRadiusInner = 152.0*cm;
  //Modified setup to study steel cryostat: no copper
  G4double CopperRadiusInner = 150.001*cm;
  G4double NitrogenRadius = 150.0*cm;

  //1) Steel water tank
  G4Sphere* steelSphere = new G4Sphere("steelSphere",
				     innerRadius,SteelRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* steelSphereLogical =
    new G4LogicalVolume(steelSphere,G4Material::GetMaterial("Steel"),
                        "steelSphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),steelSphereLogical,"WaterTank",airSphereLogical,false,0);
  
  //2) Water buffer
  G4Sphere* waterSphere = new G4Sphere("waterSphere",
				     innerRadius,WaterRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* waterSphereLogical =
    new G4LogicalVolume(waterSphere,G4Material::GetMaterial("Water"),
                        "waterSphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),waterSphereLogical,"WaterBuffer",steelSphereLogical,false,0);

  //3) Copper wall #1
  G4Sphere* copper1Sphere = new G4Sphere("copper1Sphere",
				     innerRadius,CopperRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* copper1SphereLogical =
    new G4LogicalVolume(copper1Sphere,G4Material::GetMaterial("MetalCopper"),
                        "copper1SphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),copper1SphereLogical,"CopperWall1",waterSphereLogical,false,0);

  
  //4) Vacuum insulation
  G4Sphere* vacuumSphere = new G4Sphere("vacuumSphere",
				     innerRadius,VacuumRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* vacuumSphereLogical =
    new G4LogicalVolume(vacuumSphere,G4Material::GetMaterial("Vacuum"),
                        "vacuumSphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),vacuumSphereLogical,"Insulation",copper1SphereLogical,false,0);

  
   //5) Copper wall#2
  G4Sphere* copper2Sphere = new G4Sphere("copper2Sphere",
				     innerRadius,CopperRadiusInner,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* copper2SphereLogical =
    new G4LogicalVolume(copper2Sphere,G4Material::GetMaterial("MetalCopper"),
                        "copper2SphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),copper2SphereLogical,"CopperWall2",vacuumSphereLogical,false,0);

  //6) Nitrogen volume
  //G4Material* cryoliquid = G4Material::GetMaterial("LiquidNitrogen");
  G4Material* cryoliquid = G4Material::GetMaterial("LiquidArgon");
  MGLog(routine) << "The cryogenic liquid is " << cryoliquid->GetName() << endlog;
  G4Sphere* nitrogenSphere = new G4Sphere("nitrogenSphere",
				     innerRadius,NitrogenRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* nitrogenSphereLogical =
    new G4LogicalVolume(nitrogenSphere,cryoliquid,
                        "nitrogenSphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),nitrogenSphereLogical,"CryoliquidBuffer",copper2SphereLogical,false,0);

  G4double InnerNitrogenRadius = 25.0*cm;

  //7) Inner nitrogen (this is the part where one wants to have the flux)
  G4Sphere* nitrogenInnerSphere = new G4Sphere("nitrogenInnerSphere",
				     innerRadius,InnerNitrogenRadius,
				     0.,twopi,0.,twopi); 
  G4LogicalVolume* nitrogenInnerSphereLogical =
    new G4LogicalVolume(nitrogenInnerSphere,cryoliquid,
                        "nitrogenInnerSphereLogical");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),nitrogenInnerSphereLogical,"CrystalRegion",nitrogenSphereLogical,false,0);

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
  airSphereLogical->SetVisAttributes(G4VisAttributes::Invisible);
  steelSphereLogical->SetVisAttributes(new G4VisAttributes(grey));
  waterSphereLogical->SetVisAttributes(new G4VisAttributes(lblue));
  copper1SphereLogical->SetVisAttributes(new G4VisAttributes(red));
  vacuumSphereLogical->SetVisAttributes(new G4VisAttributes(white));
  copper2SphereLogical->SetVisAttributes(new G4VisAttributes(red));
  nitrogenSphereLogical->SetVisAttributes(new G4VisAttributes(lgrey));
  nitrogenInnerSphereLogical->SetVisAttributes(new G4VisAttributes(lgrey));

  /*  
      MGLog(trace) << "Starting SD" << endlog;
      //Sensitive detectors
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      GEGeometrySD* CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
      GEGeometrySD* CryoSD = new GEGeometrySD("/mydet/gerda/N2buffer");
      
      SDman -> AddNewDetector(CrystalSD);
      SDman -> AddNewDetector(CryoSD);
      
      if (fCrystalSegmentLogical)
        {
          fCrystalSegmentLogical -> SetSensitiveDetector(CrystalSD);
        }
      liquidLogical->SetSensitiveDetector(CryoSD);
      
      MGLog(trace) << "End of SD" << endlog;
  */

  SetDetectorLogical(airSphereLogical);

  SetDetectorName("MCNPTest"); 

}


