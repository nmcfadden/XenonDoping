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
// $Id: MJGeometryWIPPnDetector.cc,v 1.1 2009-05-29 17:12:56 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryWIPPnDetector.cc
//
//---------------------------------------------------------------------------//
/*
 * SPECIAL NOTES:
 * 
 *
*/
// 
//---------------------------------------------------------------------------//
/*
 * AUTHOR: R.Johnson
 * CONTACT: 
 * FIRST SUBMISSION: Wed Apr 12 14:01:32 PDT 2006
 * 
 * REVISION:
 *
 * 04-12-2006, Created, R. Johnson
 * 02-16-2007, Updated.  Fixed header, removed extraneous code, R. Johnson
 * 05-02-2011, Added Foils, PbFoils, and Button sources, N. Fields
 * 11-02-2011, Changed WIPPn crystal and cryostat dimensions based on a validation
 * 		against source data, V.E. Guiseppe
 */
//---------------------------------------------------------------------------//
//

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh" 
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4Color.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "mjgeometry/MJGeometryWIPPnDetectorMessenger.hh"
#include "mjgeometry/MJGeometryWIPPnDetector.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//

MJGeometryWIPPnDetector::MJGeometryWIPPnDetector(G4String serNum)
:  MGGeometryDetector(serNum), fCavityLogical(0), fCryostatLogical(0),
   fXtalVacuumLogical(0), fCavityPhysical(0), fCryostatPhysical(0),
   fXtalVacuumPhysical(0), fPhysicalCrystal(0), fCuSO4Logical(0), 
   fCuSO4Physical(0), fCTFGLogical(0), fCTFGPhysical(0),  
   CdFoilLogical(0), CdFoilPhysical(0), NbFoilLogical(0), 
   NbFoilPhysical(0), ZnFoilLogical(0), ZnFoilPhysical(0), 
   CdSpotLogical(0), CdSpotPhysical(0), NbSpotLogical(0), 
   NbSpotPhysical(0), ZnSpotLogical(0), ZnSpotPhysical(0), 
   PbFoilLogical(0), PbFoilPhysical1(0),  PbFoilPhysical2(0),  
   PbFoilPhysical3(0),  PbFoilPhysical4(0),  PbFoilPhysical5(0), 
   PbSpotLogical(0), PbSpotPhysical1(0), PbSpotPhysical2(0),  
   PbSpotPhysical3(0),  PbSpotPhysical4(0), PbSpotPhysical5(0), 
   PlasticLogical(0), PlasticPhysical(0), SourceLogical(0), 
   SourcePhysical(0)
{
  fMessenger = new MJGeometryWIPPnDetectorMessenger(this);
  fIdealCoaxCrystal = new MGGeometryIdealCoaxCrystal("WIPPnDetector", false);
  fIdealCoaxCrystal->SetDefaults();
  SetDefaults();
}

//---------------------------------------------------------------------------//

MJGeometryWIPPnDetector::~MJGeometryWIPPnDetector()
{
  delete fIdealCoaxCrystal;
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJGeometryWIPPnDetector::ConstructDetector()
{

  //---------------------------------------------------------------------------//
  // Materials Definitions
  //---------------------------------------------------------------------------//

  G4Material *CopperMat = G4Material::GetMaterial("Copper-EF");
  G4Material   *LeadMat = G4Material::GetMaterial("Lead-Ain");
  G4Material    *AirMat = G4Material::GetMaterial("Air");
  G4Material    *Vacuum = G4Material::GetMaterial("Vacuum");

  //---------------------------------------------------------------------------//  
  // 2 shield configurations.  The detector / cryostat doesn't change, but the 
  // shield does.  The crystal is placed within vacuum which is placed within
  // a cryostat.  The cryostat is placed within an air volume.  This is then 
  // placed either within the lead enclosure (lead) or copper (copper).
  

  // Dimensions
  //---------------------------------------------------------------------------//


  // sizes of volumes

  G4double CryostatLengthOuter = 8*inch;
  G4double CryostatLengthInner = CryostatLengthOuter - 2*fCryostatThickness;
  G4double CryostatRadOuter = 1.5*inch;
  G4double CryostatRadInner = 1.4875*inch;

  
    G4double CavitySizeX = 4*inch;
    G4double CavitySizeZ = 20*inch;    
    G4double LeadSizeX = 16*inch;
    G4double LeadSizeZ = 36*inch;	
    G4double CopperSizeX = 8*inch;
    G4double CopperSizeZ = 24*inch;
  if(fShieldConfiguration == "lead"){
    CavitySizeX = 6*inch;
    CavitySizeZ = 14*inch;
    LeadSizeX = 16*inch;
    LeadSizeZ = 30*inch;
  }
    
  
  //---------------------------------------------------------------------------//  
  // position placement vectors
  //---------------------------------------------------------------------------//

  // the crystal and cryostat
  G4ThreeVector XtalPlacement(0,0,(CryostatLengthInner-4.42*cm)/2 - fDistanceXtal2Cryostat);
  G4ThreeVector CryostatVacuumPlacement(0,0,(CryostatLengthOuter-CryostatLengthInner)/2);
  G4ThreeVector CopperPlacement(0,0,2*inch);  
  G4ThreeVector CryostatPlacement(0,0,0);  
  
  if(fShieldConfiguration == "lead"){
    CryostatPlacement.setZ(-1*inch);
  }
  


  //---------------------------------------------------------------------------//

  //---------------------------------------------------------------------------//  
  //the solids
  //---------------------------------------------------------------------------//  


  G4Box *LeadSolid = new G4Box("LeadSolid", 
                               LeadSizeX/2, 
                               LeadSizeX/2,
                               LeadSizeZ/2);

  G4Box *CavitySolid = new G4Box("CavitySolid",
                                 CavitySizeX/2, 
                                 CavitySizeX/2, 
                                 CavitySizeZ/2);
  
  G4Tubs *XtalVacuumSolid = new G4Tubs("XtalVacuumSolid", 0*cm, CryostatRadInner,
                            CryostatLengthInner/2, 0*deg, 360*deg);
			    
  G4Tubs *CryostatSolid = new G4Tubs("CryostatSolid", 0*cm, 
                            CryostatRadOuter, CryostatLengthOuter/2,
			    0*deg, 360*deg);		    
  
  G4Box *CopperSolid = new G4Box("CopperSolid",
      CopperSizeX/2, CopperSizeX/2, CopperSizeZ/2);
  
  //---------------------------------------------------------------------------//  

  //---------------------------------------------------------------------------//    
  //the logical volumes
  //---------------------------------------------------------------------------//
  
  theDetectorLogical = new G4LogicalVolume(LeadSolid, LeadMat, "theDetectorLogical",
                                     0,0,0);
                                     
  //if(fShieldConfiguration == "copper"){
  //}
  fCopperLogical= new G4LogicalVolume(CopperSolid, CopperMat,
      "CopperLogical", 0,0,0);

  fCavityLogical = new G4LogicalVolume(CavitySolid, AirMat,
    "CavityLogical", 0,0,0);

  fXtalVacuumLogical = new G4LogicalVolume(XtalVacuumSolid, Vacuum,
    "XtalVacuumLogical", 0,0,0);
      
  fCryostatLogical = new G4LogicalVolume(CryostatSolid, CopperMat,
    "CryostatLogical", 0,0,0);
    
  //---------------------------------------------------------------------------//  
    
  //---------------------------------------------------------------------------//    
  //the physical volumes
  //---------------------------------------------------------------------------//  

  if(fShieldConfiguration == "copper"){
    fCopperPhysical= new G4PVPlacement(0,CopperPlacement, fCopperLogical,
      "Copper", theDetectorLogical, false, 0);

    fCavityPhysical = new G4PVPlacement(0,G4ThreeVector(0,0,0),
      fCavityLogical, "Cavity",fCopperLogical, false, 0);
  } else {
    fCavityPhysical = new G4PVPlacement(0,G4ThreeVector(0,0,0),
      fCavityLogical, "Cavity",theDetectorLogical, false, 0);  
  }
  
  fCryostatPhysical = new G4PVPlacement(0,CryostatPlacement,
    fCryostatLogical, "Cryostat", fCavityLogical, false, 0);

  fXtalVacuumPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,0),
    fXtalVacuumLogical, "XtalVacuum", fCryostatLogical,false,0);
  

  fIdealCoaxCrystal->ConstructDetector();

  fPhysicalCrystal = new G4PVPlacement(0, XtalPlacement,
                                       fIdealCoaxCrystal->GetDetectorLogical(),
                                       "PhysicalCrystal", fXtalVacuumLogical,
                                       false, 0);

  //---------------------------------------------------------------------------//  
  //if we're putting samples in...
  if(fUsingSamples) InitializeSample();
  //---------------------------------------------------------------------------//  

  SetColors();

}

//---------------------------------------------------------------------------//

void MJGeometryWIPPnDetector::SetColors()
{
// Everybody likes a little color
//---------------------------------------------------------------------------//  
  
  //  G4VisAttributes* WhiteVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0)); 
  //  G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //  G4VisAttributes* RedVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  //  G4VisAttributes* CyanVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  //  G4VisAttributes* MagVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* GreyVisAtt = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
  
  fCavityLogical->SetVisAttributes(YellowVisAtt);
  fCryostatLogical->SetVisAttributes(YellowVisAtt);
  fXtalVacuumLogical->SetVisAttributes(GreyVisAtt);
}



//---------------------------------------------------------------------------//

void MJGeometryWIPPnDetector::InitializeSample()
{
  if(fSample == "CuBalls") {
    Initialize7CuBalls();
  }
  else if(fSample == "CuSO4") {
    InitializeCuSO4();
  } 
  else if(fSample == "Beaker") {
    InitializeEformedBeaker();
  }
  else if(fSample == "CTFG") {
    InitializeCTFG();
  }
  else if(fSample == "Foils"){
    InitializeFoils();
  }
  else if(fSample == "PbFoils"){
    InitializePbFoils();
  }
  else if(fSample == "Button"){
    InitializeButton();
  }
}

//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::Initialize7CuBalls()
{
/*  if(!fMultipleSamples)
  {
    cout << "Warning!  Multiple samples have been included.  Did " << endl;
    cout << "you mean to include more than one sample to simulate?" << endl;
  }
*/
  // First create one Cu ball
  G4Sphere *SingleCuBallSolid = new G4Sphere("SingleCuBallSolid",
                                             0, fCuBallRadius, 
                                             0, 360*deg,
                                             0, 180*deg);

  // Create the logical volume for all 7
  G4Material *CopperMat = G4Material::GetMaterial("Copper-EF");

  f7CuBallsLogical = new G4LogicalVolume(SingleCuBallSolid, CopperMat,
                                        "7CuBallsLogical", 0,0,0);
 
  // now to create the physical volumes.  first the center one.
 
  G4int i = 0;
  char CuBallName[15];
  sprintf( CuBallName, "CuBall_%d", i);
  G4double eps = 1e-4*cm; //just a small tolerance to keep the balls separate
  G4double BallPosZ = 3*inch + 3*eps + fCuBallRadius;
  for(i = 0; i<1; i++) {
    f7CuBallsPhysical = new G4PVPlacement(0, G4ThreeVector(0,0, BallPosZ),
                                      f7CuBallsLogical, CuBallName, 
                                      fCavityLogical, false, i);
  }

  // and then the 6 outer ones...
 
  G4double rho = 2*fCuBallRadius + eps;
  for(i = 1; i<7; i++) {
    G4double BallPosX = rho*cos((pi/3)*(i-1));
    G4double BallPosY = rho*sin((pi/3)*(i-1));
    sprintf( CuBallName, "CuBall_%d", i);
    f7CuBallsPhysical = new G4PVPlacement(0, 
                        G4ThreeVector(BallPosX,BallPosY,BallPosZ),
                        f7CuBallsLogical, CuBallName, 
                        fCavityLogical, false, i);
  } 
  // add some color...
  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0)); 
  GreenVisAtt->SetForceSolid(true);
  f7CuBallsLogical->SetVisAttributes(GreenVisAtt);

}

//---------------------------------------------------------------------------// 


void MJGeometryWIPPnDetector::InitializeEformedBeaker()
{
  // beaker:
  // mass:              658.3 g
  // wall thickness:    1.21, 0.95, 0.81, 0.86 mm (90 deg sep)
  // open diameter:     109.4, 108.0 mm
  // bottom diameter:   112.5, 112.6, 112.1, 112.3
  // height:            158.8 mm
  // density of Cu:     8.96 g/cm^3
  /*if(!fMultipleSamples)
  {
    cout << "Warning!  Multiple samples have been included.  Did " << endl;
    cout << "you mean to include more than one sample to simulate?" << endl;
  }
*/

  //remember!! the the z-position is not correct yet.  if you forget,
  //well...
  G4double BeakerLength = 15.88*cm;
  G4double BeakerThickness = 1.175*mm;
  G4double eps = 1e-4*cm;

  G4VSolid *BeakerSolid = new G4Tubs("BeakerSolid", 0*cm, fBeakerOuterRadius,
                                   BeakerLength/2, 0*deg, 360*deg);

  G4Tubs *BeakerCavitySolid = new G4Tubs("BeakerCavitySolid", 0*cm, 
                                         fBeakerOuterRadius-BeakerThickness,
                                         BeakerLength/2,
                                         0*deg, 360*deg);

  BeakerSolid = new G4SubtractionSolid("BeakerSolid",
                              BeakerSolid, BeakerCavitySolid, 0, 
                              G4ThreeVector(0,0,-BeakerThickness));

  G4Material *CopperMat = G4Material::GetMaterial("Copper-EF");

  fBeakerLogical = new G4LogicalVolume(BeakerSolid, CopperMat,
                                        "fBeakerLogical", 0,0,0);
 
  G4double BeakerZPos = 3*inch - BeakerLength/2 + BeakerThickness + 10*eps;
 
  fBeakerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0, BeakerZPos),
                                      fBeakerLogical, "Beaker", 
                                      fCavityLogical, false, 0);

  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0)); 
  GreenVisAtt->SetForceSolid(true);
  fBeakerLogical->SetVisAttributes(GreenVisAtt);
}

//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::InitializeCuSO4()
{
  /*if(!fMultipleSamples)
  {
    cout << "Warning!  Multiple samples have been included.  Did " << endl;
    cout << "you mean to include more than one sample to simulate?" << endl;
  }
*/
  //remember!! the the z-position is not correct yet.  if you forget,
  //well...
  G4double eps = 1e-4*cm;

  
  G4VSolid *CuSO4Solid = new G4Box("CuSO4Solid", 
                               2.5*inch, 
                               2.5*inch,
                               1.25*inch);
  
  G4Box *SubtractSolid = new G4Box("SubtractSolid", 
                               2.8*inch, 
                               1.55*inch,
                               1.5*inch);

  CuSO4Solid = new G4SubtractionSolid("CuSO4Solid",
                              CuSO4Solid, SubtractSolid, 0, 
                              G4ThreeVector(0,0,-1.25*inch));



  // Create the CuSO_4 material
  G4Element *Cu = new G4Element("Copper", "Cu", 29, 63.55*g/mole);
  G4Element *S = new G4Element("Sulfur", "S", 16, 32.07*g/mole);
  G4Element *O = new G4Element("Oxygen", "O", 8, 15.99*g/mole);
  G4Material *CuSO4Mat = new G4Material("CopperSulfate", 2.28*g/cm3, 3);
  CuSO4Mat->AddElement(Cu,1);
  CuSO4Mat->AddElement(S,1);
  CuSO4Mat->AddElement(O,4);

  fCuSO4Logical = new G4LogicalVolume(CuSO4Solid, CuSO4Mat,
                                        "fCuSO4Logical", 0,0,0);
 
  G4double CuSO4ZPos = 2.75*inch + eps;
 
  fCuSO4Physical = new G4PVPlacement(0, G4ThreeVector(0,0, CuSO4ZPos),
                                      fCuSO4Logical, "CuSO4", 
                                      fCavityLogical, false, 0);

  G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); 
  BlueVisAtt->SetForceSolid(true);
  fCuSO4Logical->SetVisAttributes(BlueVisAtt);
}



//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::InitializeCTFG()
{
  G4Cons *CTFGSolid = new G4Cons("CTFGSolid", 
                               0.0*inch, 2.0*inch,
                               0.0*inch, 2.5*inch,
                               1.1375*inch, 0, 2*pi);
  
  // Create the FG material
  G4Element *Si = new G4Element("Silicon", "Si", 14, 28.085*g/mole);
  G4Element *O = new G4Element("Oxygen", "O", 8, 15.99*g/mole);
  //G4Element *Ca = new G4Element("Calcium", "Ca", 20, 40.08*g/mole);
  //G4Element *C = new G4Element("Carbon", "C", 6, 12.01*g/mole);
  // whatever it's made of, the mass is 797.4 gm, and the volume is 595.34 cm^3,
  // density = 1.3394 g/cm^3
  G4Material *CTFGMat = new G4Material("SiO2", 1.34*g/cm3, 2);

  CTFGMat->AddElement(Si,1);
  CTFGMat->AddElement(O,2);

  fCTFGLogical = new G4LogicalVolume(CTFGSolid, CTFGMat,
                                        "fCTFGLogical", 0,0,0);
 
  G4double CTFGZPos = 3.0*inch + 2.275/2 *inch + 0.35*inch;
 
  fCTFGPhysical = new G4PVPlacement(0, G4ThreeVector(0,0, CTFGZPos),
                                      fCTFGLogical, "CTFG", 
                                      fCavityLogical, false, 0);

  G4VisAttributes* OrangeVisAtt = new G4VisAttributes(G4Colour(0.8,0.35,0.06)); 
  OrangeVisAtt->SetForceSolid(true);
  fCTFGLogical->SetVisAttributes(OrangeVisAtt);
}



//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::InitializeFoils()
{
  G4double FoilHeight = 2.0*mm;
  G4double FoilWidth = 50*mm; //foils are square

  //The irradiated spot was .75" in diameter
  G4double SpotDiameter = 0.75*inch;

  G4VSolid *Foil = new G4Box("Foil", FoilWidth/2, FoilWidth/2, FoilHeight/2);

  G4Tubs *Spot = new G4Tubs("Spot", 0.0*inch, SpotDiameter/2, FoilHeight/2, 
			    0.0*deg, 360.0*deg);

  Foil = new G4SubtractionSolid("Foil", Foil, Spot);


  //G4element *Cd = new G4element("Cadmium", "Cd", 48, 112.4*g/mole);
  //G4element *Nb = new G4element("Niobium", "Nb", 41, 92.9*g/mole);
  //G4element *Zn = new G4element("Zinc", "Zn", 30, 65.38*g/mole);
  //G4element *Zr = new G4element("Zirconium", "Zr", 40, 91.22*g/mole);

  G4Material* Cd = new G4Material("Cd", 48, 112.4*g/mole, 8.65*g/cm3);
  G4Material* Nb = new G4Material("Nb", 41, 92.9*g/mole, 8.57*g/cm3);  
  G4Material* Zn = new G4Material("Zn", 30, 65.38*g/mole, 7.14*g/cm3);
  //G4Material* Zr = new G4Material("Zr", 40, 91.22*g/mole, 6.52*g/cm3);

  CdFoilLogical = new G4LogicalVolume(Foil, Cd, "CdFoilLogical");
  NbFoilLogical = new G4LogicalVolume(Foil, Nb, "NbFoilLogical");
  ZnFoilLogical = new G4LogicalVolume(Foil, Zn, "ZnFoilLogical");

  CdFoilPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*FoilHeight), 
				     CdFoilLogical, "CdFoilPhysical", fCavityLogical, 0, 0);
  NbFoilPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+1.5*FoilHeight), 
				     NbFoilLogical, "NbFoilPhysical", fCavityLogical, 0, 0);
  ZnFoilPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+2.5*FoilHeight), 
				     ZnFoilLogical, "ZnFoilPhysical", fCavityLogical, 0, 0);


  CdSpotLogical = new G4LogicalVolume(Spot, Cd, "CdSpotLogical");
  NbSpotLogical = new G4LogicalVolume(Spot, Nb, "NbSpotLogical");
  ZnSpotLogical = new G4LogicalVolume(Spot, Zn, "ZnSpotLogical");

  CdSpotPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*FoilHeight), 
				     CdSpotLogical, "CdSpotPhysical", fCavityLogical, 0, 0);
  NbSpotPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+1.5*FoilHeight), 
				     NbSpotLogical, "NbSpotPhysical", fCavityLogical, 0, 0);
  ZnSpotPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+2.5*FoilHeight), 
				     ZnSpotLogical, "ZnSpotPhysical", fCavityLogical, 0, 0);



}

//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::InitializePbFoils()
{
  //There are five foils each 0.5 mm tall
  G4double FoilHeight = 0.5*mm;
  G4double FoilWidth = 50*mm; //foils are square

  //irradiated spot is .75" in diameter
  G4double SpotDiameter = 0.75*inch;

  G4VSolid *Foil = new G4Box("Foil", FoilWidth/2, FoilWidth/2, FoilHeight/2);

  G4Tubs *Spot = new G4Tubs("Spot", 0.0*inch, SpotDiameter/2, FoilHeight/2, 0.0*deg, 360.0*deg);

  Foil = new G4SubtractionSolid("Foil", Foil, Spot);

  G4Material* Pb = new G4Material("Pb", 82, 207.2*g/mole, 11.34*g/cm3);

  //Foil 1
  PbFoilLogical = new G4LogicalVolume(Foil, Pb, "PbFoilLogical");
  PbSpotLogical = new G4LogicalVolume(Spot, Pb, "PbSpotLogical");

  PbFoilPhysical1 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*FoilHeight), 
				     PbFoilLogical, "PbFoilPhysical1", fCavityLogical, 0, 0);

  PbSpotPhysical1 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*FoilHeight), 
				     PbSpotLogical, "PbSpotPhysical1", fCavityLogical, 0, 0);

  //Foil 2
  PbFoilPhysical2 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+1.5*FoilHeight), 
				     PbFoilLogical, "PbFoilPhysical2", fCavityLogical, 0, 0);

  PbSpotPhysical2 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+1.5*FoilHeight), 
				     PbSpotLogical, "PbSpotPhysical2", fCavityLogical, 0, 0);

  //Foil 3
  PbFoilPhysical3 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+2.5*FoilHeight), 
				     PbFoilLogical, "PbFoilPhysical3", fCavityLogical, 0, 0);

  PbSpotPhysical3 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+2.5*FoilHeight), 
				     PbSpotLogical, "PbSpotPhysical3", fCavityLogical, 0, 0);

  //Foil 4
  PbFoilPhysical4 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+3.5*FoilHeight), 
				     PbFoilLogical, "PbFoilPhysical4", fCavityLogical, 0, 0);

  PbSpotPhysical4 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+3.5*FoilHeight), 
				     PbSpotLogical, "PbSpotPhysical4", fCavityLogical, 0, 0);

  //Foil 5
  PbFoilPhysical5 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+4.5*FoilHeight), 
				     PbFoilLogical, "PbFoilPhysical5", fCavityLogical, 0, 0);

  PbSpotPhysical5 = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+4.5*FoilHeight), 
				     PbSpotLogical, "PbSpotPhysical5", fCavityLogical, 0, 0);
 


}

//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::InitializeButton()
{
  G4double DiscHeight = 0.25*inch;
  G4double DiscDiameter = 1.0*inch; 
  G4double SourceHeight = 0.0125*inch; //assuming a thin source (1/10 of plug seams thin)
  G4double SourceDiameter = 0.197*inch;
  G4double SourceDiscOffset = DiscHeight/2 - 0.109*inch - SourceHeight/2; //keeps the source capusle located for any active height assuming the window dimension (0.109") is right.
  G4double SourcePositionOffset = 0.0*inch; // this is distance from cryostat to source 9.84" = 25cm

  G4VSolid *Disc = new G4Tubs("Disc", 0*in, DiscDiameter/2, DiscHeight/2, 0*deg, 360*deg);
  G4Tubs *Source = new G4Tubs("Source", 0*in, SourceDiameter/2, SourceHeight/2, 0*deg, 360*deg); 

  G4VSolid *DiscSolid = new G4SubtractionSolid("DiscSolid", Disc, Source, 0, G4ThreeVector(0,0,SourceDiscOffset)); //WAS (0,0,0)

  //Assuming that the plastic disc surrounding a button source is made out of polyethylene (CH2)
  //probably not an AWFUL assumption

  G4Element* C = new G4Element("Carbon", "C", 6, 12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", 1, 1.01*g/mole);
  
  G4Material* plastic = new G4Material("plastic", 0.94*g/cm3, 2);
  plastic->AddElement(C,1);
  plastic->AddElement(H,2);

  G4Material* sourcemat = new G4Material("sourcemat", 11, 22.99*g/mole, 0.968*g/cm3);


  if(!strcmp(fButtonMat,"Sodium")){
    sourcemat = new G4Material("sourcemat", 11, 22.99*g/mole, 0.968*g/cm3);}
  else if (!strcmp(fButtonMat,"Manganese")){
    sourcemat = new G4Material("sourcemat", 25, 54.94*g/mole, 7.21*g/cm3);}
  else if (!strcmp(fButtonMat,"Cobalt")){
    sourcemat = new G4Material("sourcemat", 27, 58.93*g/mole, 8.90*g/cm3);}
  else if (!strcmp(fButtonMat,"Cesium")){
    sourcemat = new G4Material("sourcemat", 55, 132.91*g/mole, 1.93*g/cm3);}

  MGLog(routine)<<"The material of the source is " << fButtonMat << ".\n";
 
 

  PlasticLogical = new G4LogicalVolume(DiscSolid, plastic, "PlasticLogical");
  SourceLogical = new G4LogicalVolume(Source, sourcemat, "SourceLogical");

  PlasticPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*DiscHeight+SourcePositionOffset), 
				     PlasticLogical, "PlasticPhysical", fCavityLogical, 0, 0);
  SourcePhysical = new G4PVPlacement(0, G4ThreeVector(0,0,4.0*inch+0.5*DiscHeight+SourceDiscOffset+SourcePositionOffset),
                                     SourceLogical, "SourcePhysical", fCavityLogical, 0, 0);



}



//---------------------------------------------------------------------------// 

void MJGeometryWIPPnDetector::SetDefaults()
{
  fIdealCoaxCrystal->SetHeight(4.42*cm); 
  fIdealCoaxCrystal->SetRadius(2.55*cm);
  fIdealCoaxCrystal->SetDeadLayerConsidered(true);
  fIdealCoaxCrystal->SetCoreDeadLayerThickness(0.13*cm);  
  fIdealCoaxCrystal->SetCoreRadius(0.54*cm); 
  fIdealCoaxCrystal->SetCoreDepth(3.44*cm); 
  fIdealCoaxCrystal->SetCornerRadius(0.0*cm);
  fIdealCoaxCrystal->SetCutInHalf(false);
  fIdealCoaxCrystal->SetOuterDeadLayerThickness(0.3*micrometer);
  fIdealCoaxCrystal->SetG4MaterialName("Germanium-Nat");

  fCryostatThickness = 0.08*cm; 
  fDistanceXtal2Cryostat = 0.5*cm; 
  fShieldConfiguration = "copper";
  fUsingSamples = false;
  fSample = "CuBalls";
  fCuBallRadius = 1.90*cm;
  fBeakerOuterRadius = 5.435*cm;
  fBeakerLength = 15.88*cm;
  fButtonMat = "Sodium";
}


void MJGeometryWIPPnDetector::SetShieldConfiguration(G4String str)
{
  if(str == "lead" || str == "copper") {
    fShieldConfiguration = str; 
  } else {
    MGLog(error) << str << " is not a valid shield configuration for WIPPn." << endlog;
    MGLog(error) << "Setting WIPPn shield configuration to copper." << endlog;
    fShieldConfiguration = "copper";
  }
}

//---------------------------------------------------------------------------//  
