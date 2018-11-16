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
// $Id: MJGeometryCERN_NA55.cc,v 1.4 2007-01-25 06:15:44 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryCERN_NA55.cc
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
 * 02-2006: Created, M. Marino
 *
 */
//---------------------------------------------------------------------------//
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VisAttributes.hh"
#include <math.h>

#include "mjgeometry/MJGeometryCERN_NA55.hh"
#include "mjgeometry/MJGeometryCERN_NA55Messenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


MJGeometryCERN_NA55::MJGeometryCERN_NA55() : MGGeometryDetector("CERN_NA55")
{
  fWorldBox = NULL;

  fExpSphere = NULL;
  fExpLog = NULL;
  
  fTheBDTube = NULL;
  fTheBDLog = NULL;
  fTheBDPhys = NULL;
  
  fBeamDumpType = kGraphite; // default to graphite.
  fTheBeamDumpHalfZLength = 0;
  fTheMessenger = new MJGeometryCERN_NA55Messenger(this);
}

MJGeometryCERN_NA55::~MJGeometryCERN_NA55()
{
  while(!fPhysicalVolumeVec.size())
  {
     if(!fPhysicalVolumeVec.back()) delete fPhysicalVolumeVec.back();
     fPhysicalVolumeVec.pop_back(); 
  }
  while(!fLogicalVolumeVec.size())
  {
     if(!fLogicalVolumeVec.back()) delete fLogicalVolumeVec.back();
     fLogicalVolumeVec.pop_back();
  }
  while(!fSolidVec.size())
  {
     if(!fSolidVec.back()) delete fSolidVec.back();
     fSolidVec.pop_back();
  }
  delete fTheMessenger;
}

void MJGeometryCERN_NA55::ConstructDetector()
{

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


  // MATERIALS

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String symbol;
  G4double density;
  
  G4Material* air = G4Material::GetMaterial("Air");
  G4Material* theBeamDumpMaterial;
  G4Material* vacuum = G4Material::GetMaterial("Vacuum");
  
  G4double theBDDiameter, theBDLength;
  
  // Here, we set the material for the beam dump.
  switch (fBeamDumpType) {
    case kGraphite:
      theBeamDumpMaterial = new G4Material("Graphite",z=6., a=12.01*g/mole, 
        density=2.26*g/cm3);  
      theBDDiameter = 8.*cm;
      theBDLength = 75.*cm;
      break;
    case kCopper:
      theBeamDumpMaterial = new G4Material("Copper", z=29., a=63.55*g/mole,
        density=8.99*g/cm3);  
      theBDDiameter = 10.*cm;
      theBDLength = 25.*cm;
      break;
    case kLead:
      theBeamDumpMaterial = new G4Material("Lead", z=82., a=207.2*g/mole,
        density=11.35*g/cm3);  
      theBDDiameter = 20.*cm;
      theBDLength = 10.*cm;
      break;
    default:
      MGLog(warning) << "unrecognized type: " << fBeamDumpType  << endlog;
      fBeamDumpType = kGraphite;
      theBeamDumpMaterial = new G4Material("Graphite",z=6., a=12.01*g/mole, 
        density=2.26*g/cm3);  
      theBDDiameter = 8.*cm;
      theBDLength = 75.*cm;
      break;
  }

  fTheBeamDumpHalfZLength = theBDLength/2.;
  MGLog(routine) << "Using Beam Dump Material: " << theBeamDumpMaterial->GetName()  << endlog;
  
//-------------------------------------------------------------------------- 
// VOLUMES:
//-------------------------------------------------------------------------- 

  // Coordinate system: z: along beam direction
  //                    y: to right looking along beam axis
  //                    x: up looking along beam axis
  //                    origin: at front of aluminum beam dump
  
  // world volume: big box of vacuum
  G4double worldHalfX = 20.0*m, worldHalfY = 20.0*m, worldHalfZ = 20.0*m;
  fWorldBox = new G4Box("worldBox", worldHalfX, worldHalfY, worldHalfZ);
  theDetectorLogical = new G4LogicalVolume(fWorldBox, 
    vacuum, "theDetectorLogical");

  fSolidVec.push_back(fWorldBox);
  fLogicalVolumeVec.push_back(theDetectorLogical);
  
 
  // setting up the expSphere which is just a helper solid for placement.
  
  G4double expRadius = 2.20*m;
  fExpSphere = 
    new G4Sphere("expSphere",0., expRadius, 0., 360.*deg, 0., 180.*deg);
  fExpLog = new G4LogicalVolume(fExpSphere, air, "expLog");

  fSolidVec.push_back(fExpSphere);
  fLogicalVolumeVec.push_back(fExpLog);

  
  // setting up beam dump
  fTheBDTube = new G4Tubs("theBDTube", 0.,theBDDiameter/2.,theBDLength/2., 0., 360.0*deg); 
  fTheBDLog = new G4LogicalVolume(fTheBDTube, theBeamDumpMaterial, "theBDLog");
  fSolidVec.push_back(fTheBDTube);
  fLogicalVolumeVec.push_back(fTheBDLog);
 
  //__________________________________________________________________________
  // positioning things now...
  //__________________________________________________________________________

  fTheBDPhys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), fTheBDLog, 
    "theBDPhys", fExpLog, false, 0);
  fPhysicalVolumeVec.push_back(fTheBDPhys);

    // now things are in the fExpLog.  We need to place the fExpLog so that the beam dump
    // face corresponds to z = 0 in the global coordinates.

  fPhysicalVolumeVec.push_back(new G4PVPlacement(0, 
    G4ThreeVector(0., 0.,fTheBDTube->GetZHalfLength()),
    fExpLog, "expSpherePhys", theDetectorLogical, false, 0));
}

//__________________________________________________________________________
//__________________________________________________________________________
G4double MJGeometryCERN_NA55::GetMaterialDensity()
{
  // returns in units of atoms / volume
  if(fTheBDLog)
  {  
    G4Material* theMat = fTheBDLog->GetMaterial();
    return(theMat->GetTotNbOfAtomsPerVolume());
  }
  else return(0.);
}
