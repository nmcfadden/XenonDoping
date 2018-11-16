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
// $Id: MJGeometrySLACBD.cc,v 1.7 2007-01-25 06:15:44 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometrySLACBD.cc
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
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 08-2005: Created, M. Marino
 * 08-22-2005: Ported to MaGe, J. Detwiler
 *
 */
//---------------------------------------------------------------------------//
#include <sstream>
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4IStore.hh"
#include "G4VIStore.hh"
#include "G4UserLimits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VisAttributes.hh"

#include "mjgeometry/MJGeometrySLACBD.hh"
#include "mjgeometry/MJGeometrySLACBDMessenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


MJGeometrySLACBD::MJGeometrySLACBD() : MGGeometryDetector("SLACBD")
{
  fWorldBox = NULL;

  fConcBlockBox = NULL;
  fConcBlockLog = NULL;
  fConcBlockPhys = NULL;

  fAirRoomBox = NULL;
  fAirRoomLog = NULL;
  fAirRoomPhys = NULL;

  fSteelSideBox = NULL;
  fSteelSideRightLog = NULL;
  fSteelSideLeftLog = NULL;
  fLeftSteelPhys = NULL;
  fRightSteelPhys = NULL;
  fSteelTopBox = NULL;
  fSteelTopLog = NULL;
  fSteelTopPhys = NULL;
  fSteelSideShieldBox = NULL;
  fSteelSideShieldLog = NULL;

  fAirSideBox = NULL;
  fAirSideLog = NULL;
  fAirSidePhys = NULL;

  fAlumBDTube = NULL;
  fAlumBDLog = NULL;
  fAlumBDPhys = NULL;

  fSteelBDTube = NULL;
  fSteelBDLog = NULL;
  fSteelBDPhys = NULL;

  fSteelAwningBox = NULL;
  fSteelAwningLog = NULL;
  fSteelAwningPhys = NULL;

  fImpSampShieldBox = NULL;
  fImpSampShieldLog = NULL;
  fImpSampShieldPhys = NULL;
  fConcShieldThickness = kNoShield;

  fTheMessenger = new MJGeometrySLACBDMessenger(this);
}

MJGeometrySLACBD::~MJGeometrySLACBD()
{
  delete fTheMessenger;
}

void MJGeometrySLACBD::ConstructDetector()
{

  // MATERIALS

  G4int nComponents, nAtoms;
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String symbol;
  G4double density, fractionMass;

  G4Element* O  = new G4Element("Oxygen"   , symbol="O" , z=8. , a=16.00*g/mole);
  G4Element* Si = new G4Element("Silicon"  , symbol="Si", z=14., a=28.06*g/mole);
  G4Element* Ca = new G4Element("Calcium"  , symbol="Ca", z=20., a=40.08*g/mole);
  G4Element* Al = new G4Element("Aluminum" , symbol="Al", z=13., a=26.98*g/mole);
  G4Element* C  = new G4Element("Carbon"   , symbol="C" , z= 6., a=12.01*g/mole);
  G4Element* Fe = new G4Element("Iron"     , symbol="Fe", z=26., a=55.85*g/mole);
  G4Element* Na = new G4Element("Sodium"   , symbol="Na", z=11., a=22.99*g/mole);
  G4Element* K  = new G4Element("Potassium", symbol="K" , z=19., a=39.10*g/mole);
  G4Element* H  = new G4Element("Hydrogen" , symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* Mg = new G4Element("Magnesium", symbol="Mg", z=12., a=24.31*g/mole);
  G4Element* Ni = new G4Element("Nickel"   , symbol="Ni", z=28., a=58.69*g/mole);
  G4Element* Cr = new G4Element("Chromium" , symbol="Cr", z=24., a=52.00*g/mole);

  G4Material* concrete = new G4Material("concrete", density=2.35*g/cm3 , nComponents=10);
  concrete->AddElement(O , fractionMass=50.0*perCent);
  concrete->AddElement(Si, fractionMass=20.0*perCent);
  concrete->AddElement(Ca, fractionMass=19.5*perCent);
  concrete->AddElement(Al, fractionMass= 3.0*perCent);
  concrete->AddElement(C , fractionMass= 3.0*perCent);
  concrete->AddElement(Fe, fractionMass= 1.4*perCent);
  concrete->AddElement(Na, fractionMass= 1.0*perCent);
  concrete->AddElement(K , fractionMass= 1.0*perCent);
  concrete->AddElement(H , fractionMass= 0.6*perCent);
  concrete->AddElement(Mg, fractionMass= 0.5*perCent);

  G4Material* steel = new G4Material("steel", density=7.5*g/cm3, nComponents=3);
  steel->AddElement(Fe, nAtoms=7);
  steel->AddElement(Ni, nAtoms=2);
  // Nickel causes segmentation faults in data file 28_62Nickel, therefore we don' use it
  steel->AddElement(Cr, nAtoms=1);

  G4Material* aluminum = new G4Material("aluminum", density=2.700*g/cm3, nComponents=1);
  aluminum->AddElement(Al,nAtoms=1);

  G4Material* air = G4Material::GetMaterial("Air");

//-------------------------------------------------------------------------- 
// VISUALIZATION:
//__________________________________________________________________________
  
  // define colors:
  G4VisAttributes* RedVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0));
  G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* PurpleVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* WhiteVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* BlackVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
 
  // set wireframe:
  RedVisAtt->SetForceSolid(true);
  GreenVisAtt->SetForceSolid(true);
  BlueVisAtt->SetForceSolid(true);
  PurpleVisAtt->SetForceSolid(true);
  WhiteVisAtt->SetForceWireframe(true);
  BlackVisAtt->SetForceWireframe(true);

//-------------------------------------------------------------------------- 
// VOLUMES:
//-------------------------------------------------------------------------- 

  // the natural unit for some of the volumes is feet. The G4 guys must not
  // like feet, because they don't define it.

  G4double ft = 30.48*cm;

  // Coordinate system: z: along beam direction
  //                    y: to right looking along beam axis
  //                    x: up looking along beam axis
  //                    origin: at front of aluminum beam dump
  
  // world volume: big box of air
  G4double worldHalfX = 50.0*ft, worldHalfY = 50.0*ft, worldHalfZ = 50.0*ft;
  fWorldBox = new G4Box("worldBox", worldHalfX, worldHalfY, worldHalfZ);
  theDetectorLogical = new G4LogicalVolume(fWorldBox, 
    G4Material::GetMaterial("Vacuum"), "theDetectorLogical");

  fSolidVec.push_back(fWorldBox);
  fLogicalVolumeVec.push_back(theDetectorLogical);
  
  //theDetectorLogical->SetVisAttributes(BlueVisAtt);
   
  // concrete block; will be placed in theDetectorLogical later. From here
  // until its placement, we work in the reference frame of the concrete block
  // 
  // the "air" daughter volume will not be concentric in order to allow for 
  // multiple concrete side shielding depths.

  // concrete shield thickness -> 3 different schemes
  // 	kNoShield: Minimal shielding (6 ft) mainly for debugging
  // 	kThinShield:  9 ft
  // 	kMedShield:   11 ft
  // 	kThickShield: 13 ft
  
  G4double concShieldHalfY = 0.;
  fNumImpSampRegions = 12;   
  switch (fConcShieldThickness) {
    case kNoShield:
      concShieldHalfY = 0.;
      break;
    case kVeryThinShield:
      concShieldHalfY = 0.5*ft / 2.0;
      fNumImpSampRegions=3;
      break;
    case kThinShield:
      concShieldHalfY = 3.0*ft / 2.0;
      fNumImpSampRegions=18;
      break;
    case kMedShield:
      concShieldHalfY = 5.0*ft / 2.0;
      fNumImpSampRegions=22;
      break;
    case kThickShield:
      concShieldHalfY = 7.0*ft / 2.0;
      fNumImpSampRegions=26;
      break;
    default:
      MGLog(warning) << "unrecognized shield thickness " << fConcShieldThickness << ", using no shield" << endlog;
      fConcShieldThickness = kNoShield;
      concShieldHalfY = 0.;
      break;
  }
  MGLog(routine) << "Concrete shield thickness: " << concShieldHalfY/ft << " ft" << endlog;
  
  // setting up concrete block using shield thickness
  
  G4double concBlockHalfX = 20.6*ft/2.0;
  G4double concBlockHalfY = 26.0*ft/2.0 + concShieldHalfY;
  G4double concBlockHalfZ = 20.0*ft/2.0;
  fConcBlockBox = new G4Box("concBlockBox", concBlockHalfX, concBlockHalfY, concBlockHalfZ);
  fConcBlockLog = new G4LogicalVolume(fConcBlockBox, concrete, "concBlockLog");
  fSolidVec.push_back(fConcBlockBox);
  fLogicalVolumeVec.push_back(fConcBlockLog);
 
//__________________________________________________________________________
// Importance Sampling "shield" to see neutrons once they exit the concrete 
// shield.
//__________________________________________________________________________

  fImpSampShieldBox = new G4Box("impSampShieldBox", concBlockHalfX + 12.0*cm,
    concBlockHalfY + 12.0*cm, concBlockHalfZ + 12.0*cm);
  fImpSampShieldLog = new G4LogicalVolume(fImpSampShieldBox, 
    G4Material::GetMaterial("Vacuum"), "impSampShieldLog");
  fSolidVec.push_back(fImpSampShieldBox);
  fLogicalVolumeVec.push_back(fImpSampShieldLog);
  
//__________________________________________________________________________
// air room in concrete block; allow 6 ft concrete on all sides
//__________________________________________________________________________

  G4double airRoomHalfX = 8.6*ft/2.0, airRoomHalfY = 14.0*ft/2.0, airRoomHalfZ = 8.0*ft/2.0;
  fAirRoomBox = new G4Box("airRoomBox", airRoomHalfX, airRoomHalfY, airRoomHalfZ);
  fAirRoomLog = new G4LogicalVolume(fAirRoomBox, air, "airRoomLog");
  fAirRoomPhys = new G4PVPlacement(0, G4ThreeVector(0.,-concShieldHalfY,0.),
     fAirRoomLog, "airRoomPhys", fConcBlockLog, false, 0);

  fSolidVec.push_back(fAirRoomBox);
  fLogicalVolumeVec.push_back(fAirRoomLog);
  fPhysicalVolumeVec.push_back(fAirRoomPhys);
  
//__________________________________________________________________________
// we set up a series of concrete daughter volumes to the right (y direction)
// of the air room in order to facilitate importance sampling.
//__________________________________________________________________________
  
  G4double concSideShieldHalfX = fConcBlockBox->GetXHalfLength();
  G4double concSideShieldHalfY = (fConcBlockBox->GetYHalfLength() - 
     fAirRoomBox->GetYHalfLength() + concShieldHalfY)/(2.0*fNumImpSampRegions);
  G4double concSideShieldHalfZ = fConcBlockBox->GetZHalfLength();

  fConcSideShieldBox = new G4Box("concSideShieldBox", concSideShieldHalfX,
     concSideShieldHalfY, concSideShieldHalfZ);
  fConcSideShieldLog = new G4LogicalVolume(fConcSideShieldBox, concrete, 
     "concSideShieldLog");
  fSolidVec.push_back(fConcSideShieldBox);
  fLogicalVolumeVec.push_back(fConcSideShieldLog);
  
    
//__________________________________________________________________________
// steel shield:  one box for the top
// two log vols for the side (for importance sampling)
//__________________________________________________________________________

  G4double ceilingAirGap = 15.0*cm;
  G4double steelSideHalfX = airRoomHalfX - ceilingAirGap/2.0,
           steelSideHalfY = 2.75*ft/2.0, steelSideHalfZ = airRoomHalfZ;
  fSteelSideBox = new G4Box("steelSideBox", steelSideHalfX, steelSideHalfY, steelSideHalfZ);
  fSteelSideRightLog = new G4LogicalVolume(fSteelSideBox, steel, "steelSideRightLog");
  fSteelSideLeftLog = new G4LogicalVolume(fSteelSideBox, steel, "steelSideLeftLog");

  fSolidVec.push_back(fSteelSideBox);
  fLogicalVolumeVec.push_back(fSteelSideLeftLog);
  fLogicalVolumeVec.push_back(fSteelSideRightLog);
  
  G4double steelTopHalfX = 59.0*cm/2.0, steelTopHalfY = 133.08*cm/2.0, steelTopHalfZ = steelSideHalfZ;
  fSteelTopBox = new G4Box("steelTopBox", steelTopHalfX, steelTopHalfY, steelTopHalfZ);
  fSteelTopLog = new G4LogicalVolume(fSteelTopBox, steel, "steelTopLog");
  
//__________________________________________________________________________
// Setting up a set of volumes to the right to facilitate imp samp in steel shield
//__________________________________________________________________________
 
  fNumImpSampSteelRegions = 5;
    //static_cast<G4int>(fNumImpSampRegions*(steelSideHalfY)); 
  fSteelSideShieldBox = new G4Box("steelShieldSideBox", steelSideHalfX, 
    steelSideHalfY/fNumImpSampSteelRegions, steelSideHalfZ);
  fSteelSideShieldLog = new G4LogicalVolume(fSteelSideShieldBox, steel, 
    "steelSideShieldLog");
  fSolidVec.push_back(fSteelSideShieldBox);
  fLogicalVolumeVec.push_back(fSteelSideShieldLog);
  G4double steelSideShieldPosY;
  G4VPhysicalVolume* tempSteelShield;
  G4String steelShieldName; 
  G4double* steelShieldImp = new G4double[fNumImpSampSteelRegions]; 
  for(int i=0;i<fNumImpSampSteelRegions;i++) {
    steelShieldImp[i] = 3*std::pow(2.,i); 
  }
  for(G4int i=0;i<fNumImpSampSteelRegions;i++) {
     // setting up daughter volumes from the outside in, but numbering them 
     // from inside to the outside.  
     steelSideShieldPosY = 
       steelSideHalfY - (steelSideHalfY/fNumImpSampSteelRegions)*(1 + 2*i);
     steelShieldName = 
       "Steel_"+GetShieldName(fNumImpSampSteelRegions - 1 - i);     
     tempSteelShield = new G4PVPlacement(0, 
        G4ThreeVector(0., steelSideShieldPosY,0.), 
        fSteelSideShieldLog, steelShieldName, fSteelSideRightLog, false, 0);  
     SetImportanceValueForRegion(tempSteelShield, 
	steelShieldImp[fNumImpSampSteelRegions - 1 - i]); 
     fPhysicalVolumeVec.push_back(tempSteelShield);
  } 
  // place steel shields in air room
  
  G4ThreeVector leftSteelPos(-ceilingAirGap/2.0, -(steelTopHalfY + steelSideHalfY), 0);
  fLeftSteelPhys = new G4PVPlacement(0, leftSteelPos, fSteelSideLeftLog, "leftSteelPhys", fAirRoomLog, false, 0);
  
  G4ThreeVector rightSteelPos(-ceilingAirGap/2.0, steelTopHalfY + steelSideHalfY, 0);
  fRightSteelPhys = new G4PVPlacement(0, rightSteelPos, fSteelSideRightLog, 
    "rightSteelPhys", fAirRoomLog, false, 0);
  
  G4ThreeVector steelTopPos(airRoomHalfX - steelTopHalfX - ceilingAirGap, 0, 0);
  fSteelTopPhys = new G4PVPlacement(0, steelTopPos, fSteelTopLog, "steelTopLog", fAirRoomLog, false, 0);

  fPhysicalVolumeVec.push_back(fLeftSteelPhys);
  fPhysicalVolumeVec.push_back(fRightSteelPhys); 
  fSolidVec.push_back(fSteelTopBox);
  fLogicalVolumeVec.push_back(fSteelTopLog);
  fPhysicalVolumeVec.push_back(fSteelTopPhys);
  
  //fSteelSideRightLog->SetVisAttributes(RedVisAtt);
  
  
  G4String shieldName;
  G4double concSidePosY;
  
  // setting importance values for shield portions
  
  G4double* shieldImp = new G4double[fNumImpSampRegions];
  for(int i=0;i<fNumImpSampRegions;i++) shieldImp[i] =  
    steelShieldImp[fNumImpSampSteelRegions - 1]*std::pow(1.7, i + 1);
 
  G4VPhysicalVolume* tempShield;
  for(G4int i = 0;i<fNumImpSampRegions;i++)
  {
     // setting up daughter volumes from the outside in, but numbering them 
     // from inside to the outside.  

     concSidePosY = concBlockHalfY - concSideShieldHalfY - 2.*i*concSideShieldHalfY;
     shieldName = GetShieldName(fNumImpSampRegions - 1 - i);     
     tempShield = new G4PVPlacement(0, G4ThreeVector(0., concSidePosY,0.), 
        fConcSideShieldLog, shieldName, fConcBlockLog, false, 0);  
     SetImportanceValueForRegion(tempShield, shieldImp[fNumImpSampRegions - 1 - i]); 
     fPhysicalVolumeVec.push_back(tempShield);
  } 
//__________________________________________________________________________
// air box to the right of steel shield (in order to facilitate imp samp)
//__________________________________________________________________________

  G4double airSideHalfX = airRoomHalfX,
     airSideHalfY = (airRoomHalfY - 2*steelSideHalfY - steelTopHalfY)/2.0,
     airSideHalfZ = airRoomHalfZ;
  
  G4ThreeVector airSidePos(0, airSideHalfY + steelTopHalfY + 2*steelSideHalfY, 0); 
  fAirSideBox = new G4Box("airSideBox", airSideHalfX, airSideHalfY , airSideHalfZ);
  fAirSideLog = new G4LogicalVolume(fAirSideBox, air, "airSideLog");

  fAirSidePhys = new G4PVPlacement(0, airSidePos, fAirSideLog, "airSidePhys",
     fAirRoomLog, false, 0);
 
  fSolidVec.push_back(fAirSideBox);
  fLogicalVolumeVec.push_back(fAirSideLog);
  fPhysicalVolumeVec.push_back(fAirSidePhys);
  //fAirSideLog->SetVisAttributes(GreenVisAtt);
  
//__________________________________________________________________________
// aluminum beam dump
//__________________________________________________________________________

  G4double alumBDRadius = 19.0*cm, alumBDHalfHeight = 145.0*cm/2.0;
  fAlumBDTube = new G4Tubs("alumBDTube", 0.0*cm, alumBDRadius, alumBDHalfHeight, 0.0*deg, 360.0*deg);
  fAlumBDLog = new G4LogicalVolume(fAlumBDTube, aluminum, "alumBDLog");

  // put aluminum beam dump in air room, with its axis 37.12 cm above the
  // concrete floor, with 12 cm space between its outer edge and the left steel
  // wall, and centered in room along z-direction

  G4double alumBDPosX = -airRoomHalfX + 37.12*cm,
           alumBDPosY = leftSteelPos.y() + steelSideHalfY + 12.0*cm + alumBDRadius,
           alumBDPosZ = 0.*m;
  G4ThreeVector alumBDPos(alumBDPosX, alumBDPosY, alumBDPosZ);
  fAlumBDPhys = new G4PVPlacement(0, alumBDPos, fAlumBDLog, "alumBDPhys", fAirRoomLog, false,0);

  fSolidVec.push_back(fAlumBDTube);
  fLogicalVolumeVec.push_back(fAlumBDLog);
  fPhysicalVolumeVec.push_back(fAlumBDPhys);
  
//__________________________________________________________________________
// steel beam dump, placed right behind aluminum beam dump
//__________________________________________________________________________

  G4double steelBDRadius = alumBDRadius, steelBDHalfHeight = 18.0*cm/2.0;
  fSteelBDTube = new G4Tubs("steelBDTube", 0.0*cm, steelBDRadius, steelBDHalfHeight, 0.0*deg, 360.0*deg);
  fSteelBDLog = new G4LogicalVolume(fSteelBDTube, steel, "steelBDLog");

  G4double steelBDPosX = alumBDPosX, steelBDPosY = alumBDPosY,
           steelBDPosZ = alumBDPosZ + alumBDHalfHeight + steelBDHalfHeight;
  G4ThreeVector steelBDPos(steelBDPosX, steelBDPosY, steelBDPosZ);
  fSteelBDPhys = new G4PVPlacement(0, steelBDPos, fSteelBDLog, "steelBDPhys", fAirRoomLog, false,0);

  fSolidVec.push_back(fSteelBDTube);
  fLogicalVolumeVec.push_back(fSteelBDLog);
  fPhysicalVolumeVec.push_back(fSteelBDPhys);
  
//__________________________________________________________________________
// steel awning directly above beam dump
//__________________________________________________________________________

  G4double steelAwningHalfX = 15.24*cm/2.0, steelAwningHalfY = 62.0*cm/2.0,
           steelAwningHalfZ = alumBDHalfHeight + steelBDHalfHeight;
  fSteelAwningBox = new G4Box("steelAwningBox", steelAwningHalfX, steelAwningHalfY, steelAwningHalfZ);
  fSteelAwningLog = new G4LogicalVolume(fSteelAwningBox, steel, "steelAwningLog");
  
  G4double steelAwningPosX = alumBDPosX + alumBDRadius + steelAwningHalfX,
           steelAwningPosY = alumBDPosY, steelAwningPosZ = steelBDHalfHeight;
  G4ThreeVector steelAwningPos(steelAwningPosX, steelAwningPosY, steelAwningPosZ);
  fSteelAwningPhys = new G4PVPlacement(0, steelAwningPos, fSteelAwningLog, "steelAwningPhys", fAirRoomLog, false, 0);

  fSolidVec.push_back(fSteelAwningBox);
  fLogicalVolumeVec.push_back(fSteelAwningLog);
  fPhysicalVolumeVec.push_back(fSteelAwningPhys);
  
//__________________________________________________________________________
// position concrete block in world volume so that aluminum beam dump
// face center is at origin. We will now work in world volume coordinates.
//__________________________________________________________________________

  G4double concBlockPosX = -alumBDPosX;
  G4double concBlockPosY = -alumBDPosY + concShieldHalfY;
  G4double concBlockPosZ = -alumBDPosZ + alumBDHalfHeight;
  G4ThreeVector concBlockPos(concBlockPosX, concBlockPosY, concBlockPosZ);
  fConcBlockPhys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), fConcBlockLog, 
    "concBlockPhys", fImpSampShieldLog, false, 0);
  fImpSampShieldPhys = new G4PVPlacement(0, concBlockPos, fImpSampShieldLog,
    "impSampShieldPhys", theDetectorLogical, false, 0);

  fPhysicalVolumeVec.push_back(fConcBlockPhys);
  fPhysicalVolumeVec.push_back(fImpSampShieldPhys);
  
//__________________________________________________________________________
// IMPORTANCE VALUES:
// Setting the Importance values for the rest of the world.
//__________________________________________________________________________

  SetImportanceValueForRegion(fAirRoomPhys); 
  SetImportanceValueForRegion(fLeftSteelPhys);  
  SetImportanceValueForRegion(fRightSteelPhys); 
  SetImportanceValueForRegion(fSteelTopPhys);
  SetImportanceValueForRegion(fAirSidePhys, 
    steelShieldImp[fNumImpSampSteelRegions - 1]); 
  SetImportanceValueForRegion(fAlumBDPhys);
  SetImportanceValueForRegion(fSteelBDPhys);
  SetImportanceValueForRegion(fSteelAwningPhys);
  SetImportanceValueForRegion(fConcBlockPhys);
  SetImportanceValueForRegion(fImpSampShieldPhys);
   
// setting up colors for every logical volume (black)
  for(unsigned int i = 0; i<fLogicalVolumeVec.size();i++) {
    fLogicalVolumeVec[i]->SetVisAttributes(BlackVisAtt);
  }
  fConcSideShieldLog->SetVisAttributes(G4VisAttributes::GetInvisible()); 
  fSteelSideShieldLog->SetVisAttributes(G4VisAttributes::GetInvisible());  
  fImpSampShieldLog->SetVisAttributes(G4VisAttributes::GetInvisible());
  fAlumBDLog->SetVisAttributes(BlueVisAtt);
  fSteelAwningLog->SetVisAttributes(GreenVisAtt);
  fSteelSideRightLog->SetVisAttributes(RedVisAtt);
  fSteelSideLeftLog->SetVisAttributes(RedVisAtt);
  fSteelTopLog->SetVisAttributes(RedVisAtt);
  fSteelBDLog->SetVisAttributes(PurpleVisAtt);

  delete [] steelShieldImp;
  delete [] shieldImp;
}

//__________________________________________________________________________
//__________________________________________________________________________

G4String MJGeometrySLACBD::GetShieldName(G4int i)
{
  // returns the name of the shield given an integer

  std::ostringstream os;
  os << "ShieldPart_";
  if (i<10) os << '0';
  os << i;
  G4String name(os.str()); 
  return name;
}   

//__________________________________________________________________________
//__________________________________________________________________________

void MJGeometrySLACBD::FillPhysVolMap(map<const G4VPhysicalVolume*, int> &theMap)
{
  // Fills a map with the partition shield physical volumes (along with slab number).
  // This is used in MJOutputSLACBD as a fast method for determining when a particle
  //   leaves one shield partition and progresses to another.
	
  if(theMap.size() == 0) {
    G4PhysicalVolumeStore* thePhysVolStore = G4PhysicalVolumeStore::GetInstance();
    for(unsigned int i=0; i<(*thePhysVolStore).size(); i++) {
      if(theMap.size() == (unsigned int)(fNumImpSampRegions) + 1) break;
      if((*thePhysVolStore)[i]->GetName()=="impSampShieldPhys") {
        map<const G4VPhysicalVolume* , int> :: const_iterator anIt;
        anIt = theMap.find((*thePhysVolStore)[i]);
        if(anIt == theMap.end()) {
	  theMap[(*thePhysVolStore)[i]] = -1; //Shield gets -1
	  MGLog(trace) << (*thePhysVolStore)[i]->GetName() << " added to map." << endlog;
	}
      }else{
        for(int j=0;j<fNumImpSampRegions;j++) {
	  if((*thePhysVolStore)[i]->GetName() == GetShieldName(j)) {
            map<const G4VPhysicalVolume* , int> :: const_iterator anotherIt;
	    anotherIt = theMap.find((*thePhysVolStore)[i]);
	    if(anotherIt == theMap.end()) {
	      theMap[(*thePhysVolStore)[i]] = j;
	      MGLog(trace) << (*thePhysVolStore)[i]->GetName() << " added to map." << endlog;
	      break;
	    }
	  } 
	}
      }
    }
  }
}


