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
// $Id: MJGeometryPbShield.cc,v 1.2 2007-08-17 21:42:40 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPbShield.cc
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

#include "mjgeometry/MJGeometryPbShield.hh"
#include "mjgeometry/MJGeometryPbShieldMessenger.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


MJGeometryPbShield::MJGeometryPbShield() : MGGeometryDetector("PbShield")
{
  /* Initializing variables.  These must be set by the messenger. */
  fOuterShieldDimension = 0.;
  fInnerShieldDimension = 0.;
  fNumberImpSampRegions = 0;
  fImpSampRatio = 1.;
  
  fTheMessenger = new MJGeometryPbShieldMessenger(this);
}

MJGeometryPbShield::~MJGeometryPbShield()
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

void MJGeometryPbShield::ConstructDetector()
{

  /* First check to see if everything is ok. */
  if (fOuterShieldDimension <= fInnerShieldDimension) {
    MGLog(error) << "Outer dimension of shield (" << fOuterShieldDimension
      << ") less than inner dimension of shield (" << fInnerShieldDimension
      << ")." << endlog;
    MGLog(fatal) << endlog;
  }
  if (fNumberImpSampRegions <= 0) {
    MGLog(error) << "Number of importance regions incorrect: " << 
      fNumberImpSampRegions << endlog;
    MGLog(fatal) << endlog;
  }
  if (fImpSampRatio <= 1.) {
    MGLog(error) << "Importance ratio incorrect: " << 
      fImpSampRatio << endlog;
    MGLog(fatal) << endlog;
  }
  
  /* Get through to here, everything should be good. */

//-------------------------------------------------------------------------- 
// VISUALIZATION:
//__________________________________________________________________________
  
  // define colors:
 
  // set wireframe:

//-------------------------------------------------------------------------- 
// MATERIALS
//-------------------------------------------------------------------------- 

  G4Material* vacuum = G4Material::GetMaterial("Vacuum");
  G4Material* lead = new G4Material("Lead", 82., 207.2*g/mole, 11.35*g/cm3);  
  
//-------------------------------------------------------------------------- 
// VOLUMES:
//-------------------------------------------------------------------------- 

  // world volume: big box of vacuum
  G4double worldHalfX = 20.0*m, worldHalfY = 20.0*m, worldHalfZ = 20.0*m;
  fWorldBox = new G4Box("worldBox", worldHalfX, worldHalfY, worldHalfZ);
  theDetectorLogical = new G4LogicalVolume(fWorldBox, 
    vacuum, "theDetectorLogical");

  fSolidVec.push_back(fWorldBox);
  fLogicalVolumeVec.push_back(theDetectorLogical);
  
  /************************************************/
  // setting up the Lead Shield 
  /************************************************/
  
  G4double halfOuterShield = fOuterShieldDimension/2.;
  G4double halfInnerShield = fInnerShieldDimension/2.;
  fSolidVec.push_back(
    new G4Box(GetShieldName(0), halfOuterShield, halfOuterShield, halfOuterShield));
  fLogicalVolumeVec.push_back(
    new G4LogicalVolume(fSolidVec[fSolidVec.size()-1],lead,GetShieldName(0)+"_log"));
  fPhysicalVolumeVec.push_back(
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), 
    fLogicalVolumeVec[fLogicalVolumeVec.size()-1], GetShieldName(0)+"_phys",
    theDetectorLogical, false, 0));

    
  G4double halfShield = 0.;
  for (G4int i=1;i<fNumberImpSampRegions;i++) {
    halfShield = 
      (fNumberImpSampRegions-i)*(halfOuterShield-halfInnerShield)/fNumberImpSampRegions
      + halfInnerShield;
    fSolidVec.push_back(
      new G4Box(GetShieldName(i), halfShield, halfShield, halfShield));
    fLogicalVolumeVec.push_back(
      new G4LogicalVolume(fSolidVec[fSolidVec.size()-1],lead,GetShieldName(i)+"_log"));
    fPhysicalVolumeVec.push_back(
      new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), 
      fLogicalVolumeVec[fLogicalVolumeVec.size()-1], GetShieldName(i)+"_phys",
      fLogicalVolumeVec[fLogicalVolumeVec.size()-2], false, 0));
  }
  
  /************************************************/
  /* Setting up the inside of the shield. */
  /************************************************/

  fSolidVec.push_back(
    new G4Box("insideShield", halfInnerShield, halfInnerShield, halfInnerShield));
  fLogicalVolumeVec.push_back(
    new G4LogicalVolume(fSolidVec[fSolidVec.size()-1],vacuum,"insideShield_log"));
  fPhysicalVolumeVec.push_back(
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), 
    fLogicalVolumeVec[fLogicalVolumeVec.size()-1], "insideShield_phys",
    fLogicalVolumeVec[fLogicalVolumeVec.size()-2], false, 0));


  /************************************************/
  /* Setting up importance values.  This works so that the values
     increase as one moves towards the inside of the shield. */
  /************************************************/

  G4double impValue = MGGeometryDetector::kMinImpValue;
  for (G4int i=0;i<fNumberImpSampRegions;i++) {
    impValue *= fImpSampRatio;
    SetImportanceValueForRegion(fPhysicalVolumeVec[i], impValue); 
  }
  SetImportanceValueForRegion(
    fPhysicalVolumeVec[fPhysicalVolumeVec.size()-1], impValue); 
 
}

//__________________________________________________________________________
//__________________________________________________________________________

G4String MJGeometryPbShield::GetShieldName(G4int i)
{
  // returns the name of the shield given an integer

  std::ostringstream os;
  os << "PbShield_";
  if (i<10) os << '0';
  os << i;
  G4String name(os.str()); 
  return name;
}
