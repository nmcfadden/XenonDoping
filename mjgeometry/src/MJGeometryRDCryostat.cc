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
// $Id: MJGeometryRDCryostat.cc,v 1.7 2009-04-23 18:32:24 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryRDCryostat.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Nov 16 16:25:21 PST 2005
 * 
 * REVISION:
 *
 * 03-19-2010, Simplified Geometry for DEMO, added 5 xtal strings, R. Henning.
 * 03-08-2006, Changed CryostatPhysical to InnerCryostatPhysical, R. Henning
 * 11-17-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>
#include <vector>

#include "TMath.h"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4VSolid.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"
#include "mjgeometry/MJGeometryRDCryostatMessenger.hh"
#include "mjgeometry/MJGeometryRDCrystalColumn.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryRDCryostat.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryRDCryostat::MJGeometryRDCryostat(): 
MGGeometryDetector(""), 
fTemplateCrystalColumn(0), fTemplateCrystal(0), fCryostatMaterial(0), 
fOpenSpaceMaterial(0), fCuVisAtt(0), fMotherVolumeVisAtt(0), fMessenger(0)
{
  fHolePositionsXY.clear();
}

//---------------------------------------------------------------------------//

MJGeometryRDCryostat::MJGeometryRDCryostat(G4String serNum): 
MGGeometryDetector(serNum), 
fCryostatMaterial(0), fOpenSpaceMaterial(0), fCuVisAtt(0), fMotherVolumeVisAtt(0)
{
  // Instantiate template here so that their parameters may be set via messenger.
  fMessenger = new MJGeometryRDCryostatMessenger(this);
  fTemplateCrystal = new MJGeometryIdealCoaxWithShield("Template", false);
  fTemplateCrystalColumn = new MJGeometryRDCrystalColumn("Template", false);
  //  fTemplateCrystalColumn->SetDefaults(); // remove me later
  fTemplateCrystalColumn->SetDefaultsDEMONSTRATOR(); // remove me later
  fTemplateCrystalColumn->SetTemplateCrystal(fTemplateCrystal);
  fHolePositionsXY.clear();
  SetDefaults(); // Remove later. Move to messenger. 
}

//---------------------------------------------------------------------------//

MJGeometryRDCryostat::MJGeometryRDCryostat(const MJGeometryRDCryostat &rhs): 
  MGGeometryDetector(rhs.serialNumber)
{
  // Add me at end.
}

//---------------------------------------------------------------------------//

MJGeometryRDCryostat::~MJGeometryRDCryostat()
{
  size_t sizeCC = fCrystalColumns.size();
  for(size_t i = 0; i < sizeCC; i++)
    delete fCrystalColumns[i];
  delete fMessenger;
  delete fTemplateCrystalColumn;
  delete fTemplateCrystal;
  delete fCuVisAtt;
  delete fMotherVolumeVisAtt;
}

//---------------------------------------------------------------------------//

void MJGeometryRDCryostat::ConstructDetector()
{
  Initialize();
  MGLog(routine) << "Constructing Majorana RD cryostat geometry..." << endlog;
  size_t    i;

  // Build inner part of cryostat
  G4Tubs *innerCryostatSolid = 
    new G4Tubs("innerCryostatSolid", 0, fCryostatInnerRadius, 
	       fInnerCryostatHeight_2 - fDEMOCryostatThickness, 0, 360.0*deg);
  G4LogicalVolume *innerCryostatLogical =
    new G4LogicalVolume(innerCryostatSolid, fOpenSpaceMaterial, "innerCryostatLogical");
  innerCryostatLogical->SetVisAttributes(fMotherVolumeVisAtt);
  MGLog(routine) << "Constructed inner cavity of cryostat with radius: "<< fCryostatInnerRadius/cm
		 << "cm and height: " << (fInnerCryostatHeight_2 - fDEMOCryostatThickness) * 2. /cm << "cm." << endlog;

  // Place stringer mount plate if required.
  if(!fBuildDEMONSTRATOR) {
    G4VSolid *stringerMountPlateSolid = new G4Tubs("stringerMountPlateSolid", 0.0*cm,
      fStringerMountPlateRadius, fStringerMountPlateThickness/2., 0.*deg, 360.*deg);
    G4LogicalVolume *stringerMountPlateLogical 
      = new G4LogicalVolume(stringerMountPlateSolid, fCryostatMaterial, "stringerMountPlateLogical");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -fInnerCryostatHeight_2 + fThermalShroudHeight + 
				     fStringerMountPlateThickness/2. + 0.3*fEps), 
		    stringerMountPlateLogical, "StringerMountPlate", 
		    innerCryostatLogical, false, 0);
  }

  // Place Crystal Columns.
  if(fCrystalColumnPositions.size() != fCrystalColumns.size()) {
    MGLog(error) << "Internal STL vector length consistency check failed!" << endlog;
    MGLog(fatal) << endlog;
  }
  for(i = 0; i < fNOfCrystalColumns; i++) {
    fCrystalColumns[i]->SetDetectorLogical(0);
    MGLog(routine) << "Constructing crystal column: " << fCrystalColumns[i]->GetSerialNumber() << endlog;
    fCrystalColumns[i]->ConstructDetector();
    new G4PVPlacement(0,fCrystalColumnPositions[i] +
		      (fBuildDEMONSTRATOR ? G4ThreeVector(0,0,-1.5*cm) : 
		      G4ThreeVector(0, 0, -fInnerCryostatHeight_2 + fThermalShroudHeight -
				    fCrystalColumns[i]->GetSupportRodLength()/2.)),
		      fCrystalColumns[i]->GetDetectorLogical(), 
		      G4String(fCrystalColumns[i]->GetSerialNumber()),
		      innerCryostatLogical, false, 0);
  }

  // Build cryostat volume and place inner cryostat with strings inside it.
  G4Tubs *cryostatSolid = new G4Tubs("cryostatSolid", 0, fCryostatOuterRadius, 
				     fCryostatTotalHeight/2., 0, 360.0*deg);
  theDetectorLogical = new G4LogicalVolume(cryostatSolid, fCryostatMaterial, "cryostatLogical");
  new G4PVPlacement(0, G4ThreeVector(0,0,(fCryostatFloorThickness-fCryostatLidThickness)/2.), 
		    innerCryostatLogical, "InnerCryostatPhysical", theDetectorLogical, false, 0);
  theDetectorLogical->SetVisAttributes(fMotherVolumeVisAtt);
  MGLog(routine) << "Constructed Cryostat with outer radius: " << fCryostatOuterRadius/cm 
		 << "cm  and height: " << fCryostatTotalHeight/cm << "cm." << endlog;
  G4double cryostatWallVolume_cm3 = TMath::Pi() * (fCryostatOuterRadius  * fCryostatOuterRadius * fCryostatTotalHeight
				- fCryostatInnerRadius * fCryostatInnerRadius * 2. * fInnerCryostatHeight_2) / (cm*cm*cm);

  MGLog(routine) << "The cryostat walls have a volume of " << cryostatWallVolume_cm3 << "cm^3." 
		 << " and a mass of " 
		 << cm*cm*cm * cryostatWallVolume_cm3 * fCryostatMaterial->GetDensity() / kg << "kg." << endlog;

  Dump();
}

//---------------------------------------------------------------------------//

void MJGeometryRDCryostat::Dump()
{
  G4cout << "Majorana RD Cryostat Parameters:" << G4endl;
  G4cout << "Dimensions are in cm." << G4endl;
  G4cout << "--------------------------------" << G4endl;
  G4cout << "fEps: " << fEps << G4endl;
  G4cout << "fThermalShroudThickness: " << fThermalShroudThickness/cm << G4endl;
  G4cout << "fCryostatInnerRadius: " << fCryostatInnerRadius/cm << G4endl;
  G4cout << "fCryostatOuterRadius: " << fCryostatOuterRadius/cm << G4endl;
  G4cout << "fCryostatTotalHeight: " << fCryostatTotalHeight/cm << G4endl;
  G4cout << "fCryostatFloorThickness: " << fCryostatFloorThickness/cm << G4endl;
  G4cout << "fCryostatLidThickness: " << fCryostatLidThickness/cm << G4endl;
  G4cout << "fCryostatMaterialName: " << fCryostatMaterialName << G4endl;
  G4cout << "fDEMOCryostatThickness: " << fDEMOCryostatThickness/cm << G4endl;
  G4cout << "fInnerCryostatHeight_2: " << fInnerCryostatHeight_2/cm << G4endl; 
  G4cout << "fStringerMountPlateRadius: " << fStringerMountPlateRadius/cm << G4endl;
  G4cout << "fStringerMountPlateThickness: " << fStringerMountPlateThickness/cm << G4endl;
  G4cout << "fCrystalColumnHoleRadius: " << fCrystalColumnHoleRadius/cm << G4endl;
  G4cout << "fHoleSeparationY01: " << fHoleSeparationY01/cm << G4endl;
  G4cout << "fHoleSeparationY02: " << fHoleSeparationY02/cm << G4endl;
  G4cout << "fHoleSeparationY03: " << fHoleSeparationY03/cm << G4endl;
  G4cout << "fHoleSeparationY04: " << fHoleSeparationY04/cm << G4endl;
  G4cout << "fHoleSeparationX01: " << fHoleSeparationX01/cm << G4endl;
  G4cout << "fHoleSeparationX02: " << fHoleSeparationX02/cm << G4endl;
  G4cout << "fThermalShroudHeight: " << fThermalShroudHeight/cm << G4endl;
  //  G4cout << "fHolePositionsXY: " <<  fHolePositionsXY << G4endl;
  G4cout << "fOpenSpaceMaterialName: " << fOpenSpaceMaterialName << G4endl; 
  G4cout << "--------------------------------" << G4endl;
}

//---------------------------------------------------------------------------//


void MJGeometryRDCryostat::Initialize()
{
  MGLog(routine) << "Initializing Majorana RD cryostat geometry..." << endlog;

  fMotherVolumeVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  fMotherVolumeVisAtt->SetVisibility(false);
  fMotherVolumeVisAtt->SetForceSolid(false);

  // Consistency checks.
  if(!(fCryostatMaterial = G4Material::GetMaterial(fCryostatMaterialName))) {
    MGLog(error) << "Cryostat material " << fCryostatMaterialName << " not found." << endlog;
    MGLog(fatal) << endlog;
  }
  if(!(fOpenSpaceMaterial = G4Material::GetMaterial(fOpenSpaceMaterialName))) {
    MGLog(error) << "Open space material " << fOpenSpaceMaterialName << " not found." << endlog;
    MGLog(fatal) << endlog;
  }

  // Construct templates and compute useful variables. 
  MGLog(routine) << "Constructing template crystal." << endlog;
  fTemplateCrystal->SetIsOneOfMany(false);
  fTemplateCrystal->ConstructDetector();
  fTemplateCrystal->Dump();

  MGLog(routine) << "Constructing template crystal column." << endlog;
  fTemplateCrystalColumn->SetOneOfMany(false);
  fTemplateCrystalColumn->SetDetectorLogical(0);
  fTemplateCrystalColumn->ConstructDetector();

  fNOfCrystalColumns = fHolePositionsXY.size();
  fAssemblyHeight = fCryostatTotalHeight;
  fAssemblyRadius = fCryostatOuterRadius;

   // Instantiate crystal columns and set equal to template.
  stringstream crystalColumnName;
  fCrystalColumns.clear();
  for(size_t i = 0; i < fNOfCrystalColumns; i++) {
    crystalColumnName.str("");
    crystalColumnName << "CrystalColumn" << i;
    fCrystalColumns.push_back(new MJGeometryRDCrystalColumn(*fTemplateCrystalColumn));
    fCrystalColumns[i]->SetOneOfMany(true);
    fCrystalColumns[i]->SetSerialNumber(crystalColumnName.str());
    fCrystalColumns[i]->SetStringerSupportThickness(fStringerMountPlateThickness);
    fCrystalColumns[i]->SetTemplateCrystalColumnSerNum(fTemplateCrystal->GetSerialNumber());
    fCrystalColumns[i]->SetColumnRotationZ(fCrystalColumnRotations[i]);
  }
}

//---------------------------------------------------------------------------//

void MJGeometryRDCryostat::SetDefaults()
{
  MGLog(routine) << "Setting defaults for Majorana RD cryostat geometry." << endlog;
  fBuildDEMONSTRATOR = false;
  fEps = 0.001 * mm;
  fThermalShroudThickness = 0.5 * mm;
  fDEMOCryostatThickness = 10.0 * mm;
  fCryostatInnerRadius = (400.0 * mm - fThermalShroudThickness)/2.;
  fCryostatOuterRadius = 410.0 / 2. * mm;
  fCryostatLidThickness = 6.4 * mm;
  fCryostatMaterialName = "Copper-EF";
  fCryostatTotalHeight = fCryostatLidThickness + 339.7 * mm;
  fCryostatFloorThickness = 10.0 * mm + fThermalShroudThickness;
  fStringerMountPlateRadius = 387.4/2.0 * mm;
  fStringerMountPlateThickness = 9.5 * mm;
  fCrystalColumnHoleRadius = 33.8 * mm;
  fHoleSeparationY01 = 76.2 * mm;
  fHoleSeparationY02 = 152.4 * mm;
  fHoleSeparationY03 = 228.6 * mm;
  fHoleSeparationY04 = 304.8 * mm;
  fHoleSeparationX01 = 120.7 * mm;
  fHoleSeparationX02 = 241.3 * mm;
  fThermalShroudHeight = 271.3 * mm;
  fOpenSpaceMaterialName = "Vacuum";
  
  // x,y locations of holes in stringer mount plate
  // Using different numbering scheme than in white paper. Count from inside out, since it is 
  // it is easier to add crystals on outside.
  fInnerCryostatHeight_2 =(fCryostatTotalHeight -fCryostatLidThickness-fCryostatFloorThickness)/2;
  //  G4double  CCoffsetZ = -fInnerCryostatHeight_2 + fThermalShroudHeight + 
  //fStringerMountPlateThickness + (16 - 285.1) * mm;
  G4double  CCoffsetZ = 0;
  G4double  holeDispY01 = fHoleSeparationY01 / 2.;
  G4double  holeDispY02 = fHoleSeparationY02 / 2.;
  G4double  holeDispY03 = fHoleSeparationY03 / 2.;
  G4double  holeDispY04 = fHoleSeparationY04 / 2.;
  G4double  holeDispX01 = fHoleSeparationX01 / 2.;
  G4double  holeDispX02 = fHoleSeparationX02 / 2.;
  fHolePositionsXY.clear();
  fCrystalColumnPositions.clear();

  // Default rotations for crystal columns.
  G4double rot1 = 30.0 * deg;
  G4double rot2 = 90.0 * deg;  

  // Center crystal.
  fHolePositionsXY.push_back(G4ThreeVector(0, 0, 0)); // C0
  fCrystalColumnPositions.push_back(G4ThreeVector(0, 0, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);

  // Inner ring.
  fHolePositionsXY.push_back(G4ThreeVector(0, holeDispY02, 0)); // C1              
  fCrystalColumnPositions.push_back(G4ThreeVector(0, holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, holeDispY01, 0)); // C2
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, -holeDispY01, 0)); // C3
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, -holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(0, -holeDispY02, 0)); // C4
  fCrystalColumnPositions.push_back(G4ThreeVector(0, -holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, -holeDispY01, 0)); // C5
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, -holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, holeDispY01 ,0)); // C6
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, holeDispY01, CCoffsetZ));
  fCrystalColumnRotations.push_back(rot2);
  
  // Outer ring.    
  fHolePositionsXY.push_back(G4ThreeVector(0, holeDispY04, 0)); // C7
  fCrystalColumnPositions.push_back(G4ThreeVector(0, holeDispY04, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, holeDispY03, 0)); // C8
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, holeDispY03,CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX02, holeDispY02, 0)); // C9
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX02, holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX02, 0, 0)); // C10
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX02, 0, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX02, -holeDispY02, 0)); // C11
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX02, -holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, -holeDispY03, 0)); // C12
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, -holeDispY03, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(0, -holeDispY04, 0)); // C13
  fCrystalColumnPositions.push_back(G4ThreeVector(0, -holeDispY04, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, -holeDispY03, 0)); // C14
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, -holeDispY03, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX02, -holeDispY02,0)); // C15
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX02, -holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX02, 0, 0)); // C16
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX02, 0, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX02, holeDispY02, 0)); // C17
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX02, holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, holeDispY03, 0)); // C18
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, holeDispY03, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
}

//---------------------------------------------------------------------------//

void MJGeometryRDCryostat::SetDefaultsDEMONSTRATOR()
{
  // Use simplified geoemtry for the DEMOSNTRATOR proxy geometry we are building/
  // Only use cryostat of uniform thickness. Remove thermal shroud and cold-plate.

  MGLog(routine) << "Setting defaults for Majorana DEMONSTRATOR cryostat geometry." << endlog;
  fBuildDEMONSTRATOR = true;
  fEps = 0.001 * mm;
  fCryostatInnerRadius = 151.1 * mm; // Taken from prototype parts dwg. MJ80-01-001
  fDEMOCryostatThickness = 10.0 * mm;
  fCryostatOuterRadius = fCryostatInnerRadius + 10.0 * mm;
  fCryostatMaterialName = "Copper-EF";
  fCryostatTotalHeight = 300. * mm;
  fCrystalColumnHoleRadius = 36.0 * mm;
  fHoleSeparationY01 = 80.0 * mm;
  fHoleSeparationY02 = 160.0 * mm;
  fHoleSeparationX01 = 2.0 * 69.3 * mm;
  fOpenSpaceMaterialName = "Vacuum";

  // These variables are irrelevant for DEMO, but must be set to 0 for computations 
  // done during construction.
  fThermalShroudThickness = 0.;
  fCryostatLidThickness = 0.;
  fCryostatFloorThickness = 0.;

  // x,y locations of holes in stringer mount plate
  // Using different numbering scheme than in white paper. Count from inside out, since it is 
  // it is easier to add crystals on outside.

  fInnerCryostatHeight_2 = fCryostatTotalHeight / 2.;

  G4double  CCoffsetZ = 0;
  G4double  holeDispY01 = fHoleSeparationY01 / 2.;
  G4double  holeDispY02 = fHoleSeparationY02 / 2.;
  G4double  holeDispX01 = fHoleSeparationX01 / 2.;
  fHolePositionsXY.clear();
  fCrystalColumnPositions.clear();

  // Default rotations for crystal columns.
  G4double rot1 = 30.0 * deg;
  G4double rot2 = 90.0 * deg;  

  // Center crystal.
  fHolePositionsXY.push_back(G4ThreeVector(0, 0, 0)); // C0
  fCrystalColumnPositions.push_back(G4ThreeVector(0, 0, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);

  // Inner ring.
  fHolePositionsXY.push_back(G4ThreeVector(0, holeDispY02, 0)); // C1              
  fCrystalColumnPositions.push_back(G4ThreeVector(0, holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, holeDispY01, 0)); // C2
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(holeDispX01, -holeDispY01, 0)); // C3
  fCrystalColumnPositions.push_back(G4ThreeVector(holeDispX01, -holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(0, -holeDispY02, 0)); // C4
  fCrystalColumnPositions.push_back(G4ThreeVector(0, -holeDispY02, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot1);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, -holeDispY01, 0)); // C5
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, -holeDispY01, CCoffsetZ));  
  fCrystalColumnRotations.push_back(rot2);
  fHolePositionsXY.push_back(G4ThreeVector(-holeDispX01, holeDispY01 ,0)); // C6
  fCrystalColumnPositions.push_back(G4ThreeVector(-holeDispX01, holeDispY01, CCoffsetZ));
  fCrystalColumnRotations.push_back(rot2);
  
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
