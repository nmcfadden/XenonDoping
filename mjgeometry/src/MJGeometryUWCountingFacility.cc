//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MJGeometryUWCountingFacility.cc,v 1.14 2008-09-23 16:41:43 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryUWCountingFacility.cc
//
//---------------------------------------------------------------------------//
/**
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Mon Oct 15 20:01:34 PDT 2007
 * 
 * REVISION:
 *
 * 10-17-2007, Initial submission, A. Schubert
 * 05-22-2008, switched y-coords of dets 1 and 2, added gravel sample, 
 *             added unique dimensions for 1 and 2, A. Schubert 
*/
//---------------------------------------------------------------------------//
//
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
//#include "G4BREPSolidPCone.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"

// MJ headers
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryUWOrtecGMX.hh"
#include <math.h>

using namespace std;

using namespace CLHEP;


//#include <systemG4Stuff.hh>             //additional system Headers
//#include "globalsStuff.hh"              //additional Global Headers
//#include "G4Stuff.hh"		          //additional G4 Class Headers
//#include "MJStuff.hh"                   //additional MJ Headers

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryUWCountingFacility.hh"      //Present MJ Class Headers 

//---------------------------------------------------------------------------//

MJGeometryUWCountingFacility::MJGeometryUWCountingFacility(G4String serNum):
  MGGeometryDetector(serNum),
  fShieldCavity_log(NULL), 
  fShieldWall_log(NULL), 
  fCopperPlate_log(NULL),
  //fSample_log(NULL), 
  //fBrick_log(NULL), 
  //fPlate_log(NULL), 
  fShieldCavity_phys(NULL),
  fShieldWall_phys(NULL),
  fDetectorOne_phys(NULL),
  fDetectorTwo_phys(NULL),
  fCopperPlate_phys(NULL)
  //fSample_phys(NULL), 
  //fBrick_phys(NULL) 
  //fPlate_phys(NULL) 
{
}

//MJGeometryUWCountingFacility::MJGeometryUWCountingFacility(const MJGeometryUWCountingFacility & other)
//{
//}

MJGeometryUWCountingFacility::~MJGeometryUWCountingFacility()
{
/*
  delete fShieldCavity_log; 
  delete fShieldWall_log; 
  delete fCopperPlate_log;
  delete fSample_log; 
  delete fBrick_log;
  delete fPlate_log; 
  delete fShieldCavity_phys;
  delete fShieldWall_phys;
  delete fDetectorOne_phys;
  delete fDetectorTwo_phys;
  delete fCopperPlate_phys;
  delete fSample_phys; 
  delete fBrick_phys;
  delete fPlate_phys; 
*/
}

void MJGeometryUWCountingFacility::ConstructDetector()
{
  //-------------------------------------------------------------------------//
  // MATERIALS:
  //-------------------------------------------------------------------------//

  G4Material *shieldMaterial = G4Material::GetMaterial("Lead-Mod");
  G4Material *shieldCavityMaterial = G4Material::GetMaterial("Air");
  G4Material *copper = G4Material::GetMaterial("Copper-OFHC");

  if(!(shieldCavityMaterial && copper &&  shieldMaterial )) {
    MGLog(error) << "NULL G4Material pointer!" << endlog;
    MGLog(fatal) << endlog;
  }

  //-------------------------------------------------------------------------//
  // DIMENSIONS 
  //-------------------------------------------------------------------------//


  G4double copperPlateHalfThickness = 0.5/2*2.54*cm;

  G4double tolerance = 0.000001*cm;

  // lead shield:
  G4double shieldHalfX = 44*2.54/2*cm; 
  G4double shieldHalfY = 28*2.54/2*cm; 
  G4double shieldHalfZ = 22*2.54/2*cm + copperPlateHalfThickness; 
  G4double shieldThickness = 6*2.54*cm;

  // wall inside lead shield:
  G4double shieldWallHalfX = 4*2.54/2*cm; // 4" wall
  G4double shieldWallHalfY = shieldHalfY - shieldThickness - tolerance;
  G4double shieldWallHalfZ = shieldHalfZ - shieldThickness - tolerance;
  G4double shieldWallHoleRadius = 3.65*2.54/2*cm; // 3.65" dia.

  // copper plate supporting roof of shield:
  G4double copperPlateHalfX = shieldHalfX - shieldThickness - 0.1*mm;
  G4double copperPlateHalfY = shieldHalfY - shieldThickness - 0.1*mm;

  // air space in shield:
  G4double shieldCavityHalfX = shieldHalfX-shieldThickness; 
  G4double shieldCavityHalfY = shieldHalfY-shieldThickness; 
  G4double shieldCavityHalfZ = shieldHalfZ-shieldThickness;

  //-------------------------------------------------------------------------//
  // POSITIONS:
  //-------------------------------------------------------------------------//

  G4double shieldWallToPbX = 11.25*2.54*cm;

  G4double ortecEdgeToPbX = shieldWallToPbX - 3.5*2.54*cm;
  G4double ortecOriginToPbX = 5.25/2*2.54*cm + ortecEdgeToPbX;

  G4double detectorX = 
    shieldCavityHalfX - ortecOriginToPbX;
  //G4double detectorY = shieldCavityHalfY - 3.75*2.54*cm;
  G4double detectorY = 4.975*2.54*cm;  // moved + 0.725" in y ags 06 May 2008
  //G4double detectorZ = -shieldCavityHalfZ + 3.25*2.54*cm;
  G4double detectorZ = -shieldCavityHalfZ + 3.225*2.54*cm;  // moved - 0.025" in z ags 06 May 2008

  cout << "face of Al, in cm:  x: " << (shieldCavityHalfX - ortecEdgeToPbX)/cm << ", y: " << detectorY/cm << ", z: " 
    << detectorZ/cm << endl;

  G4double shieldWallX = shieldCavityHalfX - 11.25*2.54*cm - shieldWallHalfX;

  G4double shieldWallHoleY = detectorY;
  G4double shieldWallHoleZ = detectorZ;

  G4ThreeVector shieldCavity_pos(0,0,0);
  G4ThreeVector shieldWall_pos(shieldWallX,0,0);
  G4ThreeVector copperPlate_pos(0,0,shieldCavityHalfZ-copperPlateHalfThickness-0.1*mm);
  G4ThreeVector detectorOne_pos(detectorX,-detectorY,detectorZ);
  G4ThreeVector detectorTwo_pos(detectorX,detectorY,detectorZ);
  G4ThreeVector shieldWallHoleOne_pos(0,shieldWallHoleY,shieldWallHoleZ);
  G4ThreeVector shieldWallHoleTwo_pos(0,-shieldWallHoleY,shieldWallHoleZ);


  //-------------------------------------------------------------------------//
  // PLACEMENT:
  //-------------------------------------------------------------------------//
  
  // lead shield (theDetectorLogical):  
  G4Box* shield_box = new G4Box("shield_box",shieldHalfX,shieldHalfY,
    shieldHalfZ);
  theDetectorLogical = new G4LogicalVolume(shield_box, shieldMaterial,
    "theDetectorLogical");

  // copper plate supporting roof of shield:
  G4Box* copperPlate_box = new G4Box("copperPlate_box",copperPlateHalfX,
    copperPlateHalfY,copperPlateHalfThickness);
  fCopperPlate_log = new G4LogicalVolume(copperPlate_box, 
    copper,"fCopperPlate_log",0,0,0);
  fCopperPlate_phys = new G4PVPlacement(0,copperPlate_pos, 
    fCopperPlate_log,"copperPlate",theDetectorLogical,false,0);

  // air space in lead shield:
  G4Box* shieldCavity_box = new G4Box("shieldCavity_box",shieldCavityHalfX,
    shieldCavityHalfY,shieldCavityHalfZ);
  fShieldCavity_log = new G4LogicalVolume(shieldCavity_box, 
    shieldCavityMaterial,"fShieldCavity_log",0,0,0);
  fShieldCavity_phys = new G4PVPlacement(0,shieldCavity_pos, 
    fShieldCavity_log,"shieldCavity",theDetectorLogical,false,0);

  // construct Pb wall from subtraction solid:
  // a temporary box in the shape of the wall:
  G4Box* shieldWallBlank = new G4Box("shieldWallBlank",shieldWallHalfX,
    shieldWallHalfY,shieldWallHalfZ);
  // a cylinder in the shape of the hole:
  G4Tubs* shieldWallHole = new G4Tubs("shieldWallHole",0,shieldWallHoleRadius,
    shieldWallHalfX + 1.0*cm, 0*deg,360*deg);
  G4RotationMatrix* holeRotation = new G4RotationMatrix;
  holeRotation->rotateY(90*deg);

  // subtract one hole from the shield wall:
  G4SubtractionSolid* shieldWallTemp_box = new G4SubtractionSolid("shieldWallTemp_box",
    shieldWallBlank,shieldWallHole,holeRotation,shieldWallHoleOne_pos);
  // subtract another hole from the shield wall:
  G4SubtractionSolid* shieldWall_box = new G4SubtractionSolid("shieldWall_box",
    shieldWallTemp_box,shieldWallHole,holeRotation,shieldWallHoleTwo_pos);

  fShieldWall_log = new G4LogicalVolume(shieldWall_box, 
    shieldMaterial,"fShieldWall_log",0,0,0);
  fShieldWall_phys = new G4PVPlacement(0,shieldWall_pos, 
    fShieldWall_log,"shieldWall",fShieldCavity_log,false,0);

  // Ortec detectors inside lead shield:
  MJGeometryUWOrtecGMX* detectorOne;
  MJGeometryUWOrtecGMX* detectorTwo;
  detectorOne = new MJGeometryUWOrtecGMX("detectorOne"); 
  detectorTwo = new MJGeometryUWOrtecGMX("detectorTwo"); 

  // per Ortec, 1998, s/n N40037:
  detectorOne->SetCrystalLength(70.7*mm);
  detectorOne->SetCrystalDiameter(64.8*mm);

  // per Ortec, 1999, s/n N30479A:
  detectorTwo->SetCrystalLength(64.3*mm);
  detectorTwo->SetCrystalDiameter(68.0*mm);

  detectorOne->ConstructDetector();
  detectorTwo->ConstructDetector();
  G4RotationMatrix* detectorRotation = new G4RotationMatrix;
  detectorRotation->rotateY(270*deg);
  fDetectorOne_phys = new G4PVPlacement(detectorRotation,detectorOne_pos,
  detectorOne->GetDetectorLogical(),"detectorOne",fShieldCavity_log,false,0);
  fDetectorTwo_phys = new G4PVPlacement(detectorRotation,detectorTwo_pos,
  detectorTwo->GetDetectorLogical(),"detectorTwo",fShieldCavity_log,false,0);

 //--------------------------------------------------------------------------
 // VISUALIZATION:
 //__________________________________________________________________________

   // define colors:
   //G4VisAttributes* RedVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
   G4VisAttributes* OrangeVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
   //G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0));
   //G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
   //G4VisAttributes* PurpleVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
   //G4VisAttributes* WhiteVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
   G4VisAttributes* GreyVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
   G4VisAttributes* DarkGreyVisAtt= new G4VisAttributes(G4Colour(0.1,0.1,0.1));

   // set colors of objects in simulation:
   theDetectorLogical->SetVisAttributes(DarkGreyVisAtt);
   fShieldCavity_log->SetVisAttributes(DarkGreyVisAtt);
   fShieldWall_log->SetVisAttributes(DarkGreyVisAtt);
   detectorOne->GetDetectorLogical()->SetVisAttributes(GreyVisAtt);
   detectorTwo->GetDetectorLogical()->SetVisAttributes(GreyVisAtt);
   fCopperPlate_log->SetVisAttributes(OrangeVisAtt);
   //fShieldCavity_log->SetVisAttributes (G4VisAttributes::Invisible);  // make the cavity invisible

  //-------------------------------------------------------------------------//
  // SAMPLE:
  //-------------------------------------------------------------------------//
  
  /* 
  For a given sample material and mass, generate a cube of sample material 
  inside the lead shield, sitting on the bottom of the cavity in the lead 
  in the z-direction, centered between the detectors in the y-direction, and
  5 mm from the edge of the detectors in the +x-direction. 
  */

/*
  G4Material *sampleMaterial = G4Material::GetMaterial("Teflon");
  G4double sampleMass = 100.0; // grams
  G4double sampleDensity = sampleMaterial->GetDensity()/(g/cm3);
  G4double sampleHalfLength = pow(sampleMass/sampleDensity,1/3.0)/2*cm;
  MGLog(routine) << "Sample density (g/cm^3): " << sampleDensity << endlog;  
  MGLog(routine) << "Sample cube length (cm): " << sampleHalfLength*2/cm << endlog;  
  MGLog(routine) << "Sample mass (g): " << pow(sampleHalfLength*2/cm,3)*sampleDensity << endlog;  

 // brick to support small sample:
  G4double brickHalfX = 8*2.54/2*cm;
  G4double brickHalfY = 4*2.54/2*cm;
  G4double brickHalfZ = 2*2.54/2*cm;

  G4double brickX = shieldCavityHalfX - ortecEdgeToPbX + sampleHalfLength + 5.0*mm;
  G4double brickY = 0.0*cm;
  G4double brickZ = - shieldCavityHalfZ + brickHalfZ + 0.1*mm;
  G4ThreeVector brick_pos(brickX,brickY,brickZ);
  G4Box* brick_box = new G4Box("brick_box",brickHalfX,brickHalfY,brickHalfZ);
  fBrick_log = new G4LogicalVolume(brick_box, 
    shieldMaterial,"fBrick_log",0,0,0);
  fBrick_phys = new G4PVPlacement(0,brick_pos, 
    fBrick_log,"brick",fShieldCavity_log,false,0);

  // crystal length is 70.9
  G4double sampleX = shieldCavityHalfX - ortecEdgeToPbX - 74.9/2*mm;
  G4double sampleY = 0.0*cm;
  // place sample 0.5 mm from the floor:
  G4double sampleZ = brickZ + brickHalfZ+ sampleHalfLength + 0.1*mm;
  //G4double sampleZ = brickZ - brickHalfZ + sampleHalfLength + 0.1*mm;
  G4ThreeVector sample_pos(sampleX,sampleY,sampleZ);
 // cube sample:
  G4Box* sample_box = new G4Box("sample_box",sampleHalfLength,sampleHalfLength,
    sampleHalfLength);
   fBrick_log->SetVisAttributes(DarkGreyVisAtt);
*/

  //-------------------------------------------------------------------------//
  // Generic sample properties:
  //-------------------------------------------------------------------------//
  // material:
  /*
  G4int nComponents, nAtoms;
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String symbol;
  G4double density  = 0.0; 

  G4double fracMass = 0.0;
  G4Element* H  = new G4Element("Hydrogen"  , symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"    , symbol="C" , z= 6., a=12.01*g/mole);
  G4Element* Mn = new G4Element("Manganese" , symbol="Mn", z=25., a=54.94*g/mole);
  G4Element* Cr = new G4Element("Chromium"  , symbol="Cr", z=24., a=52.00*g/mole);
  G4Element* Ni = new G4Element("Nickel"    , symbol="Ni", z=28., a=58.69*g/mole);
  G4Element* P  = new G4Element("Phosphorus", symbol="P",  z=15., a=30.97*g/mole);
  G4Element* S  = new G4Element("Sulfur"    , symbol="S",  z=16., a=32.07*g/mole);
  G4Element* Fe = new G4Element("Iron"      , symbol="Fe", z=26., a=55.85*g/mole);

  G4Element* Si = new G4Element("Silicon"   , symbol="Si", z=14., a=28.09*g/mole);
  G4Element* O  = new G4Element("Oxygen"    , symbol="O" , z= 8., a=16.00*g/mole);
  G4Element* Al = new G4Element("Aluminum"  , symbol="Al", z=13., a=26.98*g/mole);
  G4Element* K  = new G4Element("Potassium" , symbol="K ", z=19., a=39.10*g/mole);
  G4Element* Na = new G4Element("Sodium"    , symbol="Na", z=11., a=22.99*g/mole);
  G4Element* Ca = new G4Element("Calcium"   , symbol="Ca", z=20., a=40.08*g/mole);
  */


  //-------------------------------------------------------------------------//
  // Rexolite sample:
  //-------------------------------------------------------------------------//
/*
  G4Material *sampleMaterial = new G4Material("rexolite",density =1.05*g/cm3, nComponents=2);
  sampleMaterial->AddElement(H , nAtoms=1);
  sampleMaterial->AddElement(C , nAtoms=1);

  // sample geometry
  G4double sampleRadius = 4.0*2.54/2.0*cm;
  G4double sampleHalfLength = 6.0*2.54/2.0*cm;
  G4Tubs* sample_box = new G4Tubs("sample_box",0,sampleRadius,
    sampleHalfLength,0.0*deg,360.0*deg);

  // sample position: (crystal length is 70.9 mm)
  G4double sampleX = shieldCavityHalfX - ortecEdgeToPbX - sampleRadius;
  G4double sampleY = 0.0*cm;
  // place sample 0.1 mm from the floor:
  G4double sampleZ = -shieldCavityHalfZ + sampleHalfLength + 0.1*mm;
*/
  //-------------------------------------------------------------------------//
  // Magnet epoxy for Katrin:
  //-------------------------------------------------------------------------//

/*
  G4Material *sampleMaterial = new G4Material("Plastic", density = 2.4*g/cm3, 
    nComponents = 2);
  sampleMaterial->AddElement(C, nAtoms = 9);
  sampleMaterial->AddElement(H, nAtoms = 10);

  // sample geometry
  G4double sampleMinRadius[2];
  G4double sampleMaxRadius[2];
  G4double sampleHalfLength[2];
  
  sampleMinRadius[0] = 0.0*cm;
  sampleMinRadius[1] = 0.0*cm;
  sampleMaxRadius[0] = 2.0*2.54/2.0*cm;
  sampleMaxRadius[1] = 3.0*2.54/2.0*cm;
  sampleHalfLength[0] = 0.0*cm;
  sampleHalfLength[1] = 2.0*2.54*cm;

  G4BREPSolidPCone* sample_box = new G4BREPSolidPCone("sample_box",
		    0*deg,
		    360*deg,		   
		    2, // sections
		    0.0,		   
		    sampleHalfLength,
		    sampleMinRadius,
		    sampleMaxRadius);

  // sample position: (crystal length is 70.9 mm)
  G4double sampleX = shieldCavityHalfX - ortecEdgeToPbX - sampleMaxRadius[1];
  G4double sampleY = 0.0*cm;
  // place sample 0.1 mm from the floor:
  G4double sampleZ = -shieldCavityHalfZ + sampleHalfLength[0] + 0.1*mm;
*/

  //-------------------------------------------------------------------------//
  // welds for Katrin:
  //-------------------------------------------------------------------------//


  // Materials:
  //-------------------------------------------------------------------------//
/*
  // data from eFunda:
  G4Material *stainless304L = new G4Material("stainless304L", 
    density = 8.0*g/cm3, nComponents = 8);
  stainless304L->AddElement(C,  fracMass = 0.03/100.0);
  stainless304L->AddElement(Mn, fracMass = 2.00/100.0);
  stainless304L->AddElement(Si, fracMass = 1.00/100.0);
  stainless304L->AddElement(Cr, fracMass = 19.0/100.0);
  stainless304L->AddElement(Ni, fracMass = 10.0/100.0);
  stainless304L->AddElement(P,  fracMass = 0.045/100.0);
  stainless304L->AddElement(S,  fracMass = 0.03/100.0);
  stainless304L->AddElement(Fe, fracMass = 67.895/100.0);

  // data from eFunda:
  G4Material *stainless308L = new G4Material("stainless308L", 
    density = 7.865*g/cm3, nComponents = 8);
  stainless308L->AddElement(C,  fracMass = 0.03/100.0);
  stainless308L->AddElement(Mn, fracMass = 2.00/100.0);
  stainless308L->AddElement(Si, fracMass = 1.00/100.0);
  stainless308L->AddElement(Cr, fracMass = 20.0/100.0);
  stainless308L->AddElement(Ni, fracMass = 11.0/100.0);
  stainless308L->AddElement(P,  fracMass = 0.045/100.0);
  stainless308L->AddElement(S,  fracMass = 0.03/100.0);
  stainless308L->AddElement(Fe, fracMass = 65.895/100.0);

  //-------------------------------------------------------------------------//
  // 10-inch weld sample:
  //-------------------------------------------------------------------------//

  G4Material *sampleMaterial = stainless308L; 

  // sample geometry
  G4double plateHalfThickness = 0.12*2.54/2*cm;
  G4double plateHalfLength = 3.47*2.54/2*cm;
  G4double plateHalfWidth = 0.95*2.54/2*cm;

  G4double beadHalfWidth = sqrt(2.)*plateHalfThickness;

  
  // generate some temporary boxes used to generate subraction solids:
  G4double box2HalfWidth = beadHalfWidth + 1*cm;
  // a box from which the welded steel plate will be created:
  G4Box* box0 = new G4Box("box0",plateHalfLength,plateHalfWidth,plateHalfThickness);
  // a box from which the weld bead will be created:
  G4Box* box1 = new G4Box("box1",plateHalfLength,beadHalfWidth,beadHalfWidth);
  // a box to subtract from the steel plate and the weld bead:
  G4Box* box2 = new G4Box("box2",plateHalfLength+1*cm,box2HalfWidth,box2HalfWidth);

  // rotate box2 so its can be subtracted from the other boxes:
  G4RotationMatrix* box2Rotation = new G4RotationMatrix;
  box2Rotation->rotateX(45*deg);

  // create some vectors to offset box2 to do the cutting:
  G4ThreeVector beadZTrans(0,box2HalfWidth/sqrt(2.),box2HalfWidth/sqrt(2.) );
  G4ThreeVector plateZTrans(0,0,sqrt(2.)*box2HalfWidth - plateHalfThickness - tolerance);

  // create a steel plate with a notch cut out: 
  G4SubtractionSolid* plate_box = 
    new G4SubtractionSolid("plate_box",box0,box2,box2Rotation, plateZTrans);

  // create the weld bead in the shape of a prism:
  G4SubtractionSolid* sample_box = 
    new G4SubtractionSolid("sample_box",box1,box2,box2Rotation,beadZTrans); 

  // steel plate position: (crystal length is 70.9 mm)
  // edge of plate was approx 1/8 inch from Pb wall, due to double-bagging:
  G4double plateX = shieldWallX + plateHalfLength + 0.125*2.54*cm;  
  G4double plateY = -detectorY;
  // place sample 0.1 mm from the floor:
  G4double plateZ = -shieldCavityHalfZ + plateHalfThickness + 0.1*mm;
  G4ThreeVector plate_pos(plateX,plateY,plateZ);
  fPlate_log = new G4LogicalVolume(plate_box, 
    sampleMaterial,"fPlate_log",0,0,0);
  fPlate_phys = new G4PVPlacement(0,plate_pos, 
    fPlate_log,"plate",fShieldCavity_log,false,0);
  fPlate_log->SetVisAttributes(RedVisAtt);

  // weld bead position:
  G4double sampleX = plateX;
  G4double sampleY = plateY;
  // place sample 0.1 mm from the floor:
  G4double sampleZ = plateZ + plateHalfThickness;
  
  G4RotationMatrix* sampleRotation = new G4RotationMatrix;
  sampleRotation->rotateX(-45*deg);
*/
/*
  //-------------------------------------------------------------------------//
  // Majorana standard gravel sample:
  //-------------------------------------------------------------------------//

  G4Material *sampleMaterial = new G4Material("gravel", density = 1.33*g/cm3, 
    nComponents = 6);
  sampleMaterial->AddElement(Si, nAtoms = 117);
  sampleMaterial->AddElement(O,  nAtoms = 312);
  sampleMaterial->AddElement(Al, nAtoms = 39);
  sampleMaterial->AddElement(K,  nAtoms = 13);
  sampleMaterial->AddElement(Na, nAtoms = 13);
  sampleMaterial->AddElement(Ca, nAtoms = 13);

  // sample geometry
  G4double sampleMinRadius[2];
  G4double sampleMaxRadius[2];
  G4double sampleHalfLength[2];
  
  sampleMinRadius[0] = 0.0*cm;
  sampleMinRadius[1] = 0.0*cm;
  sampleMaxRadius[0] = 4.2*2.54/2.0*cm;
  sampleMaxRadius[1] = 2.41*2.54*cm;
  sampleHalfLength[0] = 0.0*cm;
  sampleHalfLength[1] = 2.3*2.54*cm;

  G4BREPSolidPCone* sample_box = new G4BREPSolidPCone("sample_box",
		    0*deg,
		    360*deg,		   
		    2, // sections
		    0.0,		   
		    sampleHalfLength,
		    sampleMinRadius,
		    sampleMaxRadius);

  // sample position: (crystal length is 70.9 mm)
  //G4double sampleX = shieldCavityHalfX - ortecEdgeToPbX;
  //G4double sampleY = 0.0*cm;
  // place sample 0.1 mm from the floor:
  //G4double sampleZ = -shieldCavityHalfZ + sampleHalfLength[0] + 0.1*mm;

  // sample placement; same for most samples:
  G4ThreeVector sample_pos(sampleX,sampleY,sampleZ);
  fSample_log = new G4LogicalVolume(sample_box, 
    sampleMaterial,"fSample_log",0,0,0);
  //fSample_phys = new G4PVPlacement(sampleRotation,sample_pos, 
  fSample_phys = new G4PVPlacement(0,sample_pos, 
    fSample_log,"sample",fShieldCavity_log,false,0);
  fSample_log->SetVisAttributes(GreenVisAtt);
*/

}

