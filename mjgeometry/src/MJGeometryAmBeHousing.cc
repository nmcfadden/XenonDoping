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
// $Id: MJGeometryAmBeHousing.cc,v 1.1 2007-02-23 10:54:55 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryAmBeHousing.cc
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 17 13:09:40 PST 2006
 * 
 * REVISION:
 *
 * 02-23-2007, Initial submission, A. Schubert
 */
//---------------------------------------------------------------------------//
//


#include <sstream>
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"

// MJ headers
#include "io/MGLogger.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "mjgeometry/MJGeometryAmBeHousingMessenger.hh" 

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryAmBeHousing.hh"      //Present MJ Class Headers 

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryAmBeHousing::MJGeometryAmBeHousing():
fSteelHousing_log(NULL), fSteelCavity_log(NULL), 
fLeadHousing_log(NULL), fLeadCavity_log(NULL), 
fSteelHousing_phys(NULL), fSteelCavity_phys(NULL), 
fLeadCavity_phys(NULL), fHousingPosition(0,0,0)

{
  fMessenger = new MJGeometryAmBeHousingMessenger(this);

  // materials:
  
  G4int nComponents, nAtoms;
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String symbol;
  G4double density;

  G4Element* Fe = new G4Element("Iron"    , symbol="Fe", z=26., a=55.85*g/mole);
  G4Element* Ni = new G4Element("Nickel"  , symbol="Ni", z=28., a=58.69*g/mole);
  G4Element* Cr = new G4Element("Chromium", symbol="Cr", z=24., a=52.00*g/mole);

  G4Material* steel = new G4Material("steel", density=7.5*g/cm3, nComponents=3);
  steel->AddElement(Fe, nAtoms=7);
  steel->AddElement(Ni, nAtoms=2);
  // Nickel causes segmentation faults in data file 28_62Nickel, i
  // therefore we don' use it
  steel->AddElement(Cr, nAtoms=1);

  G4Material* lead = G4Material::GetMaterial("Lead-Mod");
 //************** cavity is at vacuum until nan resolved: 
  G4Material* air = G4Material::GetMaterial("Vacuum");  

  // dimensions:
  G4double steelHousingOuterRadius = 0.75/2*2.54*cm;
  G4double steelHousingHalfHeight = 0.8/2*2.54*cm;
  G4double steelHousingThickness = 0.1*cm;

  G4double leadHousingOuterRadius = 1.1/2*2.54*cm;
  G4double leadHousingHalfHeight = 2.6/2*2.54*cm;
  G4double leadHousingThickness = 0.2*cm;

  // positions:
  G4ThreeVector fSteelHousingPosition(0,0,0);
  G4ThreeVector fSteelCavityPosition(0,0,0);
  G4ThreeVector fLeadCavityPosition(0,0,0);

  //create housings:

  //outer lead housing:
  G4Tubs* leadHousing_cylinder = new G4Tubs("leadHousing_cylinder", 0*cm, 
    leadHousingOuterRadius, leadHousingHalfHeight, 0*deg, 360*deg);
  G4Tubs* leadCavity_cylinder = new G4Tubs("leadCavity_cylinder", 0*cm, 
    leadHousingOuterRadius - leadHousingThickness, 
    leadHousingHalfHeight - leadHousingThickness, 0*deg, 360*deg);
   
  G4Tubs* steelHousing_cylinder = new G4Tubs("steelHousing_cylinder", 0*cm, 
    steelHousingOuterRadius, steelHousingHalfHeight, 0*deg, 360*deg);
  G4Tubs* steelCavity_cylinder = new G4Tubs("steelCavity_cylinder", 
    0*cm, steelHousingOuterRadius - steelHousingThickness, 
    steelHousingHalfHeight - steelHousingThickness, 0*deg, 360*deg);

  fSteelHousing_log = 
    new G4LogicalVolume(steelHousing_cylinder, steel, "SteelHousing_log");
  fSteelCavity_log = 
    new G4LogicalVolume(steelCavity_cylinder, air, "SteelCavity_log");
  fLeadHousing_log = 
    new G4LogicalVolume(leadHousing_cylinder, lead, "LeadHousing_log");
  fLeadCavity_log = 
    new G4LogicalVolume(leadCavity_cylinder, air, "LeadCavity_log");
  fLeadCavity_phys = 
    new G4PVPlacement(0, fLeadCavityPosition, fLeadCavity_log,"leadCavity",
    fLeadHousing_log,false,0);
  fSteelHousing_phys = 
    new G4PVPlacement(0, fSteelHousingPosition,fSteelHousing_log,"steelHousing",
    fLeadCavity_log,false,0);
  fSteelCavity_phys = 
    new G4PVPlacement(0, fSteelCavityPosition, fSteelCavity_log,"steelCavity",
    fSteelHousing_log,false,0);

}

MJGeometryAmBeHousing::~MJGeometryAmBeHousing()
{
   
}
