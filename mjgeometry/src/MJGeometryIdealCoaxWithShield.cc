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
// $Id: MJGeometryIdealCoaxWithShield.cc,v 1.7 2005-12-15 23:38:35 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryIdealCoaxWithShield.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Jan  6 11:34:32 PST 2005
 * 
 * REVISION:
 *
 * 01-06-2005, Created, R. Henning
 * 02-14-2005, Added geometry for coldplate and endcap, R. Henning
 * 11-21-2005, Added inherited MGGeometryIdealCoaxCrystal::SetDefaults 
 *             to SetDefaults()
 * 11-22-2005, Minor improvements. Set theDetectorLogical ==
 *             MGGeometryIdealCoaxCrystal::GetDetector() for no shield case. 
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShieldMessenger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryIdealCoaxWithShield::MJGeometryIdealCoaxWithShield(G4String serNum,
							     G4bool messOff):
  MGGeometryIdealCoaxCrystal(serNum, messOff), fColdplateVisAtt(0), 
  fCanVisAtt(0)
{
  if(!messOff)
    fG4Messenger = new MJGeometryIdealCoaxWithShieldMessenger(this);
  else 
    fG4Messenger = 0;
}

//---------------------------------------------------------------------------//

MJGeometryIdealCoaxWithShield::MJGeometryIdealCoaxWithShield(const MJGeometryIdealCoaxWithShield &rhs): 
  MGGeometryIdealCoaxCrystal(rhs),
  fShieldOn(rhs.fShieldOn),
  fAssemblyRadius(rhs.fAssemblyRadius),
  fAssemblyHeight(rhs.fAssemblyHeight),
  fEndcapThickness(rhs.fEndcapThickness),
  fColdplateThickness(rhs.fColdplateThickness),
  fEndcapMaterialName(rhs.fEndcapMaterialName),
  fColdplateMaterialName(rhs.fColdplateMaterialName),
  fColdplateVisAtt(0), fCanVisAtt(0), fG4Messenger(0)
{;}

//---------------------------------------------------------------------------//

MJGeometryIdealCoaxWithShield::~MJGeometryIdealCoaxWithShield()
{
  if(!fIsOneOfMany) {
    delete fCanVisAtt;
    delete fColdplateVisAtt;
  }
  delete fG4Messenger;
}

//---------------------------------------------------------------------------//

void MJGeometryIdealCoaxWithShield::ConstructDetector()
{
  MGGeometryIdealCoaxCrystal::ConstructDetector();

  if(fShieldOn) {
    
    // Grab crystal logical, since it will be placed inside the shield.
    // Shield logical becomes theDetectorLogical.
    G4LogicalVolume *crystalLogical = MGGeometryIdealCoaxCrystal::GetDetectorLogical();
    G4double eps = 0.00001 * cm; // Small perturbation for geometry

    // Get materials.
    G4Material *capMaterial = G4Material::GetMaterial(fEndcapMaterialName);
    if(!capMaterial) {
      MGLog(error) << "Material \'" << fEndcapMaterialName 
		   << "\' does not exist."<< endlog; 
      MGLog(fatal) << endlog;
    }
    G4Material *coldplateMaterial =
      G4Material::GetMaterial(fColdplateMaterialName);
    if(!coldplateMaterial) {
      MGLog(warning) << "Coldplate material does not exist or not defined. " 
		     << "Setting equal to endcap material" << endlog;
      coldplateMaterial = capMaterial;
      fColdplateMaterialName = fEndcapMaterialName;
    }
    G4Material *cavityMaterial = G4Material::GetMaterial("Vacuum");
    if(!cavityMaterial) {
      MGLog(error) << "Material \'Vacuum\' does not exist." << endlog;
      MGLog(fatal) << endlog;
    }
   

    // Define and check height for crystal cavity 
    G4double cavityHeight = fAssemblyHeight - 2.0 * fEndcapThickness - eps - 
      fColdplateThickness; // Height of cavity that crystal goes in.
    if(cavityHeight < fHeight) {
      MGLog(error)<< "Cavity height smaller than crystal! Resizing assembly "
		  << "height." << endlog;
      fAssemblyHeight += fHeight - cavityHeight + eps;
      cavityHeight = fAssemblyHeight - 2.0 * fEndcapThickness - eps - 
      fColdplateThickness;
    }


    // Define and check radius for crystal cavity.
    G4double cavityRadius = fAssemblyRadius - fEndcapThickness;
    if(cavityRadius < fRadius) {
      MGLog(error) << "Cavity radius smaller than crystal! Resizing assembly"
		   << " radius." << endlog;
      fAssemblyRadius = fRadius + eps + fEndcapThickness;
      cavityRadius = fRadius + eps;
    }
      

    // Build tube containing entire assembly + crystal.
    G4Tubs *assemblyTube = new G4Tubs("assemblyTube", 0*cm, fAssemblyRadius,
				      fAssemblyHeight/2., 0*deg, 360*deg);
    theDetectorLogical = new G4LogicalVolume(assemblyTube, 
					     capMaterial, "assemblyLogical" + GetSerialNumber(),
					     0, 0, 0, true);


    // Build cavity for crystal
    G4Tubs *cavitySolid = new G4Tubs("cavitySolid", 0*cm, cavityRadius, 
				     cavityHeight / 2., 0*deg, 360*deg);
    G4LogicalVolume *cavityLogical = new G4LogicalVolume(cavitySolid, 
							 cavityMaterial,
							 "cavityLogical" + GetSerialNumber(),
							 0,0,0,true);


    // Insert cavity into shield assembly.
    new G4PVPlacement(0, G4ThreeVector(0.,0.,(-cavityHeight + fHeight)/2. + eps),
				  crystalLogical, "crystalInCavity" + GetSerialNumber(),
				  cavityLogical, false, 0);


    // Insert cavity w/ crystal into shield assembly.
    new G4PVPlacement(0, G4ThreeVector(0.,0.,-fAssemblyHeight/2. +
				       fEndcapThickness + fColdplateThickness + eps + 
				       cavityHeight / 2.),
		        cavityLogical, "cavityInShield" + GetSerialNumber(),
			theDetectorLogical, false, 0);


    // If coldplate material is different than endcap, build and add it. 
    if(coldplateMaterial != capMaterial) {
      G4Tubs *coldplateSolid = new G4Tubs("coldplateSolid", 0*cm, cavityRadius,
					  fColdplateThickness / 2., 0*deg,
					  360*deg);
      G4LogicalVolume *coldplateLogical = 
	new G4LogicalVolume(coldplateSolid, coldplateMaterial, 
			    "coldplateLogical" + GetSerialNumber(), 0, 0, 0, true);
      fColdplateVisAtt = new G4VisAttributes(G4Color(0,0,1,1));
      fColdplateVisAtt->SetForceSolid(true);
      coldplateLogical->SetVisAttributes(fColdplateVisAtt);
      new G4PVPlacement(0, G4ThreeVector(0, 0,(-fAssemblyHeight + fColdplateThickness)/2.), 
			  coldplateLogical, "coldplateInShield" + GetSerialNumber(), 
			  theDetectorLogical, false, 0);
      
    }
    fCanVisAtt = new G4VisAttributes(G4Color(0.8,0.8,0.8,1));
    fCanVisAtt->SetForceWireframe(true);
    //  fCanVisAtt->SetForceAuxEdgeVisible(true);
    theDetectorLogical->SetVisAttributes(fCanVisAtt);
  } else { // Shield not on, return crystal only.
    fAssemblyHeight = fHeight;
    fAssemblyRadius = fRadius;
    theDetectorLogical = MGGeometryIdealCoaxCrystal::GetDetectorLogical();
  }
}

//---------------------------------------------------------------------------//

void MJGeometryIdealCoaxWithShield::Dump()
{
  G4cout << G4endl << "MJGeometryIdealCoaxWithShield Dump." << G4endl;
  G4cout << "-------------------------------------------------------"<<G4endl; 
  G4cout << "(Units are in cm.)" << G4endl;
  G4cout << "fShieldOn: " << fShieldOn << G4endl;
  G4cout << "fAssemblyRadius: " << fAssemblyRadius / cm << G4endl;
  G4cout << "fAssemblyHeight: " << fAssemblyHeight / cm<< G4endl;
  G4cout << "fEndcapThickness: " << fEndcapThickness / cm << G4endl;
  G4cout << "fColdplateThickness: " << fColdplateThickness / cm << G4endl;
  G4cout << "fColdplateMaterialName: " << fColdplateMaterialName << G4endl;
  G4cout << "fEndcapMaterialName: " << fEndcapMaterialName << G4endl;
  G4cout << "-------------------------------------------------------"<<G4endl; 
  MGGeometryIdealCoaxCrystal::Dump();
}

//---------------------------------------------------------------------------//

void MJGeometryIdealCoaxWithShield::SetDefaults()
{
  fShieldOn = false;
  fAssemblyHeight = fHeight;
  fAssemblyRadius = fRadius;
  fColdplateMaterialName = fEndcapMaterialName = "Copper-OFHC";
  MGGeometryIdealCoaxCrystal::SetDefaults();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
