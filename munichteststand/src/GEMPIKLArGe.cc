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
*
* note: (0 0 0) corresponds to the middle of the glass dewar - correct ?
*
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 04-07-2005
 *
 * REVISION:
 *
 * 05-03-2005, Luciano, Added Messenger
 * 25-05-2005, Marie, modified values of parameters
 * 13-06-2005, Marie, simplified Gran Sasso tank, for talk to Majorana coll meeting
 *                    ... will use GEGSLArGe.cc geometry from now on
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
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4MaterialPropertiesTable.hh"

#include "geometry/MGGeometryDetector.hh"
//#include "geometry/MGGeometryDetectorConstruction.hh"
#include "gerdageometry/GEGeometrySD.hh"

//#include "gerdageometry/GEGeometryDetector.hh"
//#include "gerdageometry/GEGeometryDetectorDB.hh"

#include "munichteststand/GEMPIKLArGe.hh"
#include "munichteststand/GEMPIKLArGeMessenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMPIKLArGe::GEMPIKLArGe() : MGGeometryDetector("")
{
  successfulInitialization = false;
  WLSyield = 0.6;
  InitializeVM2000Spectrum();
  isCrystalIn = true;
  fTheMessenger =  new GEMPIKLArGeMessenger(this);
}

// --------------------------------------------------------------------------- 

GEMPIKLArGe::~GEMPIKLArGe()
{
  delete fTheMessenger;
}

// --------------------------------------------------------------------------- 

void GEMPIKLArGe::ConstructDetector()
{

  RegisterArgonOpticalProperties();

  // construct airbox
  G4double AirBoxX = 4.0*m;
  G4double AirBoxY = 4.0*m;
  G4double AirBoxZ = 4.0*m;

  G4Box* airBox = new G4Box("airBox",AirBoxX/2.,AirBoxY/2.,AirBoxZ/2.);
  G4LogicalVolume* airBoxLogical = 
    new G4LogicalVolume(airBox,G4Material::GetMaterial("Air"),
			"airBoxLogical");

  // the stainless-steel tank is 110 cm high, 
  // but the glass dewar - which contains LAr - is only 76 cm high
  // i think the st-st tank needs to be implemented only for external sources
  // so for now, i just put the dimensions for the glass dewar instead of the st-st tank 
  /* "old" version
  G4double dewarH = 110*cm;
  G4double dewarR = 35.0*cm; // but isn't 35 cm the diameter !? */
  G4double dewarH = 76.0*cm;
  G4double dewarR = 15.0*cm;
  /* Gran Sasso simplified geometry 
  G4double dewarH = 200.0*cm;
  G4double dewarR = 45.0*cm; */

  G4Tubs* dewarTub = new G4Tubs("dewarTub",0.,dewarR,dewarH/2.,0.,twopi);
  G4LogicalVolume* dewarLogical = 
    new G4LogicalVolume(dewarTub,G4Material::GetMaterial("Steel"),
			"dewarLogical");
/*   G4VPhysicalVolume* dewarPhys = */
    new G4PVPlacement(0,G4ThreeVector(),dewarLogical,
		      "dewar",airBoxLogical,false,0);

  //Liquid argon
  G4double dewarThickness = 5*mm;
  G4Tubs* argonTub = new G4Tubs("argonTub",0.,dewarR-dewarThickness,
				(dewarH/2.)-dewarThickness,0.,twopi);
  G4LogicalVolume* argonLogical = 
    new G4LogicalVolume(argonTub,G4Material::GetMaterial("LiquidArgon"),
			"argonLogical");
/*   G4VPhysicalVolume* argonPhys = */
    new G4PVPlacement(0,G4ThreeVector(),argonLogical,
		      "argon",dewarLogical,false,0);

  // source cap - did not simulate the whole tube, but just a cylinder around the source
  G4double capH = 3.0*mm;
  G4double capR = 1.5*mm;
  G4double capPosZ = -10.7*cm;
  //  G4double capPosZ = -40.71*cm; // for Gran Sasso simplified geometry

  G4Tubs *sourceCap = new G4Tubs("sourceCap",0.,capR, capH/2.,0,twopi);
  G4LogicalVolume* capLogical = 
    new G4LogicalVolume(sourceCap,G4Material::GetMaterial("Steel"),"capLogical");
/*   G4VPhysicalVolume* capPhys = */
     new G4PVPlacement(0,G4ThreeVector(0.,0.,capPosZ),capLogical, 
 		      "cap",argonLogical,false,0);

  //reflector tube
  G4double reflectorH = 43*cm;
  G4double reflectorR = 10*cm;
  /*  // Gran Sasso simplified geometry, assuming it is covered with reflector + WLS
      G4double reflectorH = 200*cm; 
      G4double reflectorR = 45*cm;  */
  G4double reflectorThick = 0.5*mm;
  
  G4Tubs* reflectorTub = new G4Tubs("reflectorTub",reflectorR-reflectorThick,
				    reflectorR,reflectorH/2.,0.,twopi);
  G4LogicalVolume* reflectorLogical = 
    new G4LogicalVolume(reflectorTub,G4Material::GetMaterial("Nylon"),
			"reflectorLogical");
  // distance between bottom of dewar and bottom layer of reflector
  G4double distDewarReflector = 3.0*cm;
  G4double reflPosZ =  - (dewarH/2.) + reflectorH/2. + distDewarReflector;

/*   G4VPhysicalVolume* reflectorPhys = */
     new G4PVPlacement(0,G4ThreeVector(0.,0.,reflPosZ),reflectorLogical,
 		      "reflector",argonLogical,false,0);


  //This is a test --> Prima WLS, poi riflessione
  
  G4Tubs* reflectorTubB = new G4Tubs("reflectorTubB",reflectorR,
				    reflectorR+reflectorThick,reflectorH/2.,0.,twopi);
  G4LogicalVolume* reflectorLogicalB = 
    new G4LogicalVolume(reflectorTubB,G4Material::GetMaterial("Nylon"),
			"reflectorLogicalB");
/*   G4VPhysicalVolume* reflectorPhysB = */
    /*    new G4PVPlacement(0,G4ThreeVector(0.,0.,reflPosZ),reflectorLogicalB,
	  "reflectorB",argonLogical,false,0); */
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0),reflectorLogicalB,
		      "reflectorB",argonLogical,false,0);


  G4Tubs* reflectorBaseTub =  new G4Tubs("reflectorBaseTub",0,
				    reflectorR,reflectorThick/2.,0.,twopi);
  G4LogicalVolume* reflectorBaseLogical = 
    new G4LogicalVolume(reflectorBaseTub,G4Material::GetMaterial("Nylon"),
			"reflectorBaseLogical");
  
  G4double reflBPosZ = - (dewarH/2.) + dewarThickness + reflectorThick/2.;
/*   G4VPhysicalVolume* reflectorBasePhys = */
     new G4PVPlacement(0,G4ThreeVector(0.,0.,reflBPosZ),reflectorBaseLogical,
 		      "reflectorBase",argonLogical,false,0);

  //secondo strato: riflettore vero e proprio

  G4Tubs* reflectorBaseTubB =  new G4Tubs("reflectorBaseTubB",0,
				    reflectorR,reflectorThick/2.,0.,twopi);
  G4LogicalVolume* reflectorBaseLogicalB = 
    new G4LogicalVolume(reflectorBaseTubB,G4Material::GetMaterial("Nylon"),
			"reflectorBaseLogicalB");
  
  reflBPosZ = reflBPosZ - reflectorThick;
/*   G4VPhysicalVolume* reflectorBasePhysB = */
     new G4PVPlacement(0,G4ThreeVector(0.,0.,reflBPosZ),reflectorBaseLogicalB,
 		      "reflectorBaseB",argonLogical,false,0);

  //--- crystal & dead layer

  /* 168 g crystal
  G4double crystalR = 2.06*cm;
  G4double crystalH = 2.36*cm;
  G4double deadlayerT = 0.75*mm;
  */

  /* 2 kg crystal   */
  G4double crystalR = 3.925*cm;
  G4double crystalH = 9.35*cm;
  G4double deadlayerT = 0.75*mm;

  G4Tubs* deadLayerTub = new G4Tubs("deadLayerTub",0.,crystalR,
				crystalH/2.,0.,twopi);

  G4LogicalVolume* deadLayerLogical = 
    new G4LogicalVolume(deadLayerTub,G4Material::GetMaterial("NaturalGe"),
			"deadLayerLogical");

  G4Tubs* crystalTub = new G4Tubs("crystalTub",0.,crystalR-deadlayerT,
				(crystalH-deadlayerT)/2.,0.,twopi);
  G4LogicalVolume* crystalLogical = 
    new G4LogicalVolume(crystalTub,G4Material::GetMaterial("NaturalGe"),
			"crystalLogical");

  // distance from bottom of crystal to bottom reflector foil
  //  G4double distCrystalBottom = 14.0*cm + crystalH/2; // 168g crystal
//  G4double distCrystalReflector = 10.0*cm; // 2 kg crystal

  G4double cryPosX = 0.0;
  G4double cryPosY = 0.0;
  //  G4double cryPosZ = - dewarH/2. + crystalH/2. + distDewarReflector + distCrystalReflector;
  G4double cryPosZ = - dewarH/2. + crystalH/2. + 45.0*cm;
  MGLog(trace) << "dewarH/2 = " << dewarH/2 << " ; crystalH/2 = " << crystalH/2 
	       << " ; cryPosZ = " << cryPosZ << endlog;

  if (!isCrystalIn)
    { 
      cryPosZ = dewarH/2. + 15*cm;
      //move the crystal out! No problem with i/o and sensitive
    }

  // the 168 g crystal was lying on its side, but the 2 kg crystal is standing up
  G4RotationMatrix* rotM = new G4RotationMatrix();
  rotM->rotateX(90*deg);

  if (!isCrystalIn)
    {
      /*      deadLayerPhys = new G4PVPlacement(rotM,G4ThreeVector(cryPosX,cryPosY,cryPosZ),
					deadLayerLogical,
					"deadlayer",airBoxLogical,false,0); */
      new G4PVPlacement(0,G4ThreeVector(cryPosX,cryPosY,cryPosZ),
			deadLayerLogical,
			"deadlayer",airBoxLogical,false,0);
    }
  else
    {
      /*      deadLayerPhys = 
	new G4PVPlacement(rotM,G4ThreeVector(cryPosX,cryPosY,cryPosZ),
			  deadLayerLogical,
			  "deadlayer",argonLogical,false,0); */      
      new G4PVPlacement(0,G4ThreeVector(cryPosX,cryPosY,cryPosZ),
			deadLayerLogical,
			"deadlayer",argonLogical,false,0);
    }

  G4double crystalS = (deadlayerT/2.)-1.0*micrometer;
  G4ThreeVector posizione(cryPosX,cryPosY,crystalS);

  MGLog(trace) << "*** crystal center *** " 
	       << G4ThreeVector(cryPosX,cryPosY,cryPosZ)/cm << endlog;

  if (isCrystalIn) {/*G4VPhysicalVolume* crystalPhys = */
		     new G4PVPlacement(0,posizione,
				       crystalLogical,
				       "crystal",deadLayerLogical,false,0);
  }
  

  // PMT
  G4double PMTradius = 8*cm;
  G4Sphere* PMTSphere = new G4Sphere("PMTSphere",0.,PMTradius,
				     pi,pi,0.,twopi);
  G4LogicalVolume* PMTLogical = 
    new G4LogicalVolume(PMTSphere,G4Material::GetMaterial("Quartz"),
			"PMTLogical");
  rotM->rotateX(pi);
  G4double PMTPosX = 0;
  G4double PMTPosY = 0;
  G4double PMTPosZ = - dewarH/2 + reflectorH + distDewarReflector + PMTradius/2.;
/*   G4VPhysicalVolume* PMTPhys = */
    new G4PVPlacement(rotM,G4ThreeVector(PMTPosX,PMTPosY,PMTPosZ),
		      PMTLogical,
		      "PMT",argonLogical,false,0);


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
  airBoxLogical->SetVisAttributes(G4VisAttributes::Invisible);
  dewarLogical->SetVisAttributes(new G4VisAttributes(grey));
  argonLogical->SetVisAttributes(new G4VisAttributes(blue));
  capLogical->SetVisAttributes(new G4VisAttributes(cyan));
  crystalLogical->SetVisAttributes(new G4VisAttributes(brown));
  PMTLogical->SetVisAttributes(new G4VisAttributes(red));


  //Sensitive detectors
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  GEGeometrySD* CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
  GEGeometrySD* CryoSD = new GEGeometrySD("/mydet/gerda/N2buffer");

  SDman -> AddNewDetector(CrystalSD);
  SDman -> AddNewDetector(CryoSD);

  crystalLogical -> SetSensitiveDetector(CrystalSD);
  argonLogical->SetSensitiveDetector(CryoSD);

  // All optical properties
  // Energy grid goes from UV (115 nm) to green (650 nm) 
  G4int ji = 0;
  G4double ee=0;
  const G4int num = 500;
  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(650*nanometer); //598
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
    {
      ee=PPSCLowE+ ((G4double)ji) * dee;
      LAr_SCPP[ji]=ee;
    }

  //Reflector (VM2000)
  //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm 
  //and ~15% below it (almost a step-function)
  
  //Stefan says the QE for WLS is about 60%. 
  //Since the width of the reflector layer is 1 mm (in THIS simulation)
  //one should have e(-1mm/ll) = 0.40 --> ll = 1.1 mm
  // 40% --> 1.9 mm
  // 50% --> 1.5 mm
  // 60% --> 1.1 mm
  // 80% --> 0.6 mm
  

  G4double Reflectivity[num];
  G4double Efficiency[num];
  G4double WLS_absorption[num];
  G4double WLS_emission[num];
  for (ji=0;ji < num; ji++)
    {
      if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
	Reflectivity[ji] = 0.98; //visible
      else
	Reflectivity[ji] = 0.15; //UV
      
      Efficiency[ji] = 0.0;
      if (LAr_SCPP[ji] > 5*eV)
	WLS_absorption[ji] = CalculateWLSmfp(WLSyield); //absorbs UV (sometimes...)
      else
	WLS_absorption[ji] = 10.*m; //otherwise imperturbed
      WLS_emission[ji] = VM2000EmissionSpectrum(LAr_SCPP[ji]);
      //MGLog(debugging) << "VM2000: " << LAr_SCPP[ji]/eV << " " <<
      //WLS_emission[ji] << " " << WLS_absorption[ji]/m << endlog;
    }
  G4MaterialPropertiesTable *reflOptTable = new G4MaterialPropertiesTable();
  reflOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
  reflOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);  

  G4OpticalSurface* reflOptSurface 
    = new G4OpticalSurface("Refl surface",glisur,polished,dielectric_metal);
  reflOptSurface->SetMaterialPropertiesTable(reflOptTable);
  


  new G4LogicalSkinSurface("reflector_surf",reflectorLogicalB,reflOptSurface);

  //new G4LogicalSkinSurface("reflector_surf",reflectorLogical,reflOptSurface);
  //new G4LogicalBorderSurface("reflector_surf",argonPhys,reflectorPhys,reflOptSurface);

  G4double Refraction[num];
  //G4double Rayl[num];
  G4double AbsorptionL[num];
  G4MaterialPropertiesTable *nylonTable = new G4MaterialPropertiesTable();
  for (ji=0;ji < num; ji++)
    {
      Refraction[ji] = 1.15; //this is just a guess
      //Rayl[ji] = 10*m;
      AbsorptionL[ji] = 50*m;
    }
  nylonTable->AddProperty("RINDEX",LAr_SCPP,Refraction,num);
  //nylonTable->AddProperty("RAYLEIGH",LAr_SCPP,Rayl,num);
  nylonTable->AddProperty("ABSLENGTH",LAr_SCPP,AbsorptionL,num);
  nylonTable->AddProperty("WLSABSLENGTH",LAr_SCPP,WLS_absorption,num);
  nylonTable->AddProperty("WLSCOMPONENT",LAr_SCPP,WLS_emission,num);
  nylonTable->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
  G4Material::GetMaterial("Nylon")->SetMaterialPropertiesTable(nylonTable);


  
  //Reflector Base
  new G4LogicalSkinSurface("reflector2_surf",reflectorBaseLogicalB,reflOptSurface);
  //new G4LogicalBorderSurface("reflector2_surf",argonPhys,reflectorBasePhys,refl2OptSurface);

  //Germanium
  for (ji=0;ji < num; ji++)
    {
      Reflectivity[ji] = 0.40;
      Efficiency[ji] = 0.;
    }
  G4MaterialPropertiesTable *germOptTable = new G4MaterialPropertiesTable();
  germOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
  germOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);
  G4OpticalSurface* germOptSurface 
    = new G4OpticalSurface("Germ surface",glisur,polished,dielectric_metal);
  germOptSurface->SetMaterialPropertiesTable(germOptTable);

  //Associates the OpticalSurface to the actual volumes
  new G4LogicalSkinSurface("germanium_surf",deadLayerLogical,germOptSurface);
  //new G4LogicalBorderSurface("germanium_surf",deadLayerPhys,argonPhys,germOptSurface);
  

  
 //Dewar
  for (ji=0;ji < num; ji++)
    {
      Reflectivity[ji] = 0.3;//test
      Efficiency[ji] = 0.0;
    }
  G4MaterialPropertiesTable *dewarOptTable = new G4MaterialPropertiesTable();
  dewarOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
  dewarOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);
  G4OpticalSurface* dewarOptSurface 
    = new G4OpticalSurface("Dewar surface",glisur,polished,dielectric_metal);
  dewarOptSurface->SetMaterialPropertiesTable(dewarOptTable);

  //Associates the OpticalSurface to the actual volumes
  new G4LogicalSkinSurface("dewar_surf",dewarLogical,dewarOptSurface);
  //new G4LogicalBorderSurface("dewar_surf",argonPhys,dewarPhys,dewarOptSurface);


  //PMT
  for (ji=0;ji < num; ji++)
    {
      Reflectivity[ji] = 0.0;
      Efficiency[ji] = 1.0; //the convolution with QE is done later
    }
  G4MaterialPropertiesTable *pmtOptTable = new G4MaterialPropertiesTable();
  pmtOptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
  pmtOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);
  G4OpticalSurface* pmtOptSurface 
    = new G4OpticalSurface("PMT surface",glisur,polished,dielectric_metal);
  pmtOptSurface->SetMaterialPropertiesTable(pmtOptTable);

  //Associates the OpticalSurface to the actual volumes
  new G4LogicalSkinSurface("PMT_surf",PMTLogical,pmtOptSurface);
  //new G4LogicalBorderSurface("PMT_surf",argonPhys,PMTPhys,pmtOptSurface);

  if(MGLogger::GetSeverity() <= MGLogger::debugging)
    {
      G4LogicalBorderSurface::DumpInfo();
      G4LogicalSkinSurface::DumpInfo();
    }
  SetDetectorLogical(airBoxLogical);

  SetDetectorName("MPIKLArGe"); 

}

// --------------------------------------------------------------------------- 
 

void GEMPIKLArGe::RegisterArgonOpticalProperties()
{
  static const G4int NUMENTRIES = 69;
  const G4int num = 22;
  static const G4double temp = 88.5*kelvin;
  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
  G4double scint_yield = 40000./MeV; //sono 40000
  MGLog(routine) << "Scintillation yield set to " << scint_yield << " ph/MeV" << endlog;


  G4int ji;
  G4double e;
  G4double ee;

  G4double PPCKOVHighE = LambdaE / (115*nanometer);
  G4double PPCKOVLowE = LambdaE / (650*nanometer); 
  G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES-1)));
  G4double LArAbsLength = 3*m; //just a number. ICARUS says it is negligible over 
  // the 17m*3.5m*3.8m T300 volume

  // liquid argon (LAr)  
  G4double LAr_PPCK[(NUMENTRIES)];
  G4double LAr_RIND[(NUMENTRIES)];
  G4double LAr_RAYL[(NUMENTRIES)];
  G4double LAr_ABSL[(NUMENTRIES)];
  for (ji = 0; ji < NUMENTRIES; ji++)
    {
      e = PPCKOVLowE + ((G4double)ji) * de;
      LAr_PPCK[ji] = e;
      LAr_RIND[ji] = LArRefIndex((LambdaE / e));
      LAr_RAYL[ji] = LArRayLength((LambdaE / e), temp);
      MGLog(debugging) << "Energy: " << LAr_PPCK[ji]/eV << " eV; Refr: " << 
	LAr_RIND[ji] << " ; Rayleigh l. " << LAr_RAYL[ji]/m << " m" << endlog;
      LAr_ABSL[ji] = LArAbsLength;
    }

  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(136*nanometer);
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCIN[num];
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
    {
      ee=PPSCLowE+ ((G4double)ji) * dee;
      LAr_SCPP[ji]=ee;
      LAr_SCIN[ji]=ArScintillationSpectrum((LambdaE/ee)/nanometer);
      MGLog(debugging) << "Scint: " << LAr_SCPP[ji]/eV << " " << LAr_SCIN[ji] << endlog;
    }

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",        LAr_PPCK, LAr_RIND, NUMENTRIES);
  myMPT1->AddProperty("RAYLEIGH",      LAr_PPCK, LAr_RAYL, NUMENTRIES);
  myMPT1->AddProperty("ABSLENGTH",     LAr_PPCK, LAr_ABSL, NUMENTRIES);
  if ( (LAr_SCPP[0] >= PPCKOVLowE) &&
       (LAr_SCPP[(sizeof(LAr_SCPP)/sizeof(G4double) - 1)] <= PPCKOVHighE) )
    {
      myMPT1->AddProperty("FASTCOMPONENT",LAr_SCPP,LAr_SCIN,num);
      myMPT1->AddProperty("SLOWCOMPONENT",LAr_SCPP,LAr_SCIN,num);
    }
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",scint_yield);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 6.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",1590.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.23);
  
  G4Material::GetMaterial("LiquidArgon")->SetMaterialPropertiesTable(myMPT1);
}

G4double GEMPIKLArGe::LArRefIndex(const G4double lambda)
{
  return ( sqrt(LArEpsilon(lambda)) ); // square root of dielectric constant
}

// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395

G4double GEMPIKLArGe::LArEpsilon(const G4double lambda)
{
  G4double epsilon;
  if (lambda < 110*nanometer) return 1.0e4; // lambda MUST be > 110.0 nm
  epsilon = lambda / micrometer; // switch to micrometers
  epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
  epsilon = 1.2055e-2 * ( 0.2075 / (91.012 - epsilon) +
                          0.0415 / (87.892 - epsilon) +
                          4.3330 / (214.02 - epsilon) );
  epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
  G4double LArRho = 1.396*g/cm3;
  G4double GArRho = 1.66e-03*g/cm3;
  epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
  if (epsilon < 0.0 || epsilon > 0.999999) return 4.0e6;
  epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
  return epsilon;
}

//-------------------------------------------------------------------------><
// Calculates the Rayleigh scattering length using equations given in
// G. M. Seidel at al., "Rayleigh scattering in rare-gas liquids",
// arXiv:hep-ex/0111054 v2 22 Apr 2002

G4double GEMPIKLArGe::LArRayLength(const G4double lambda,const 
				   G4double temp)
{
  G4double dyne = 1.0e-5*newton;
  static const G4double LArKT = 2.18e-10 * cm2/dyne; // LAr isothermal compressibility
  static const G4double k = 1.380658e-23 * joule/kelvin; // the Boltzmann constant
  G4double h;
  h = LArEpsilon(lambda);
  if (h < 1.00000001) h = 1.00000001; // just a precaution
  h = (h - 1.0) * (h + 2.0); // the "dielectric constant" dependance
  h *= h; // take the square
  h *= LArKT * temp * k; // compressibility * temp * Boltzmann constant
  h /= lambda * lambda * lambda * lambda; // (lambda)^4
  h *= 9.18704494231105429; // (2 * Pi / 3)^3
  if ( h < (1.0 / (10.0 * km)) ) h = 1.0 / (10.0 * km); // just a precaution
  if ( h > (1.0 / (0.1 * nanometer)) ) h = 1.0 / (0.1 * nanometer); // just a precaution
  return ( 1.0 / h );}

G4double GEMPIKLArGe::ArScintillationSpectrum(const G4double kk)
{
  G4double waveL;
  waveL =exp(-0.5*((kk-128.0)/(2.929))*((kk-128.0)/(2.929)));
  return waveL;
}

void GEMPIKLArGe::InitializeVM2000Spectrum()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
    {
      MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
      //path = "/d1/neutrino/luciano/MaGe/generators/data";
    }
  G4String pathString(path);
  G4String pathFile = pathString + "/VM2000_em.dat";
  std::ifstream file(pathFile);
  if (file.is_open())
    {
      G4double aa,bb,cc,dd,ee,ff,gg,hh;      
      for (G4int i=0;i<npoints;i++)
        {
          file >> aa >> bb >> cc >> dd >> ee >> ff >> gg >> hh;
          frequencyV[i] = gg*nanometer;
          efficiencyV[i] = hh;
        }
      file.close();
      successfulInitialization = true;
      MGLog(routine) << "VM2000 data successfully read from file" << endlog;
    }
  else
    {
      MGLog(warning) << "Data file not found!" << endlog;
      successfulInitialization = false;
    }
}


G4double GEMPIKLArGe::VM2000EmissionSpectrum(G4double energy)
{
  G4int j=0;
  if (successfulInitialization)
    {
      //Here the data are loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;

      if (targetf < frequencyV[0] || targetf > frequencyV[npoints-1])
        return 0.0;

      for(j=0;j<npoints-1;j++)
        {
          if (frequencyV[j]<targetf && targetf <= frequencyV[j+1]) break;
        }
      G4double eff = (targetf-frequencyV[j])
        *(efficiencyV[j+1]-efficiencyV[j])/(frequencyV[j+1]-frequencyV[j]);
      eff += efficiencyV[j];
      return eff;
    }
  else
    {
      return 0.2;
    }
}
G4double GEMPIKLArGe::CalculateWLSmfp(G4double yield)
{
  //e(-1mm/ll) = 0.40 --> ll = 1.1 mm
  G4double totalPath = 1*mm; //hardcoded, for now
  if (yield < 0. || yield > 1.) 
    {
      MGLog(warning) << "Yield cannot be larger than 100%!" << endlog;
      MGLog(warning) << "Yield scaled to 100%" << endlog;
    }
  if (yield == 0) return 10.*m;
  if (yield == 1) return 0.01*mm;
  G4double help = std::log(1.-yield);
  G4double mfp = -1*totalPath/help;
  //MGLog(debugging) << "WLS mean free path = " << mfp/mm << " mm" << endlog;
  return mfp;
}
