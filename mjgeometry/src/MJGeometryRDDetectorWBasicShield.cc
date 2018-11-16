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
// $Id: MJGeometryRDDetectorWBasicShield.cc,v 1.3 2009-07-15 16:42:46 paddy Exp $
//
// CLASS IMPLEMENTATION:  MJGeometryRDDetectorWBasicShield.cc
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
 * FIRST SUBMISSION: Thu Dec 15 17:23:12 PST 2005
 *
 * REVISION:
 * 06-30-2009  Inserted plastic for cross arm in InnerCuShield, P. Finnerty
 * 03-08-2006, Fixed bug that causes offset between placement and logical names
 *             of shield layers. R. Henning.
 * 12-15-2005, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4UImanager.hh"
#include "TMath.h"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4Color.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryRDCryostat.hh"

//---------------------------------------------------------------------------//
#include "mjgeometry/MJGeometryRDDetectorWBasicShield.hh"
//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryRDDetectorWBasicShield::MJGeometryRDDetectorWBasicShield(G4String serNum) :
 MGGeometryDetector(serNum), fEnclosedDetector(0), fCFCavityLogical(0), fCFCavityPhysical(0),
  fCuCFLogical(0), fCuCFPhysical(0), fPlasticFootLogical(0), fPlasticFootPhysical(0)
  //Paddy made edits here, check the .hh file
{
 fShieldThickness.clear();
 fShieldMaterialName.clear();
 fShieldMaterial.clear();
 fShieldName.clear();

 // Instantiate enclosed detector.
 // Should be handled by messenger later.
 fEnclosedDetector = new MJGeometryRDCryostat("Cryostat");
}


//---------------------------------------------------------------------------//

MJGeometryRDDetectorWBasicShield::~MJGeometryRDDetectorWBasicShield()
{
 delete fEnclosedDetector;
}

//---------------------------------------------------------------------------//

void MJGeometryRDDetectorWBasicShield::ConstructDetector()
{
 SetDefaults();
 Initialize();
 fEnclosedDetector->ConstructDetector();
 G4Box           *shieldSolid = 0;
 G4LogicalVolume *shieldLogical = 0;
 G4LogicalVolume *innerShieldLogical = fEnclosedDetector->GetDetectorLogical();
 G4double shieldX = 0, shieldY = 0, shieldZ = 0;
 G4double prevShieldX = 0, prevShieldY = 0, prevShieldZ = 0;
 G4double shieldMass_kg;
 for(size_t i = 0; i < fShieldThickness.size(); i++) {
   shieldX += fShieldThickness[i].x();
   shieldY += fShieldThickness[i].y();
   shieldZ += fShieldThickness[i].z();
   shieldSolid = new G4Box(fShieldName[i] + "Solid", shieldX, shieldY, shieldZ);
   shieldLogical = new G4LogicalVolume(shieldSolid,  fShieldMaterial[i], fShieldName[i] + "Logical");
   if(i == 0) {
     new G4PVPlacement(0, fShieldDisplacement[i], innerShieldLogical, fEnclosedDetector->GetSerialNumber()+"Physical", shieldLogical, false, 0);
     MGLog(routine) << "Placed Shield " << fEnclosedDetector->GetSerialNumber() << endlog;
   }
   else {
     new G4PVPlacement(0, fShieldDisplacement[i], innerShieldLogical, fShieldName[i-1]+"Physical", shieldLogical, false, 0);
     shieldMass_kg = 8.0 * (shieldX * shieldY * shieldZ - prevShieldX * prevShieldY * prevShieldZ ) *
       fShieldMaterial[i]->GetDensity() / kg;
     MGLog(routine) << "Placed Shield " << fShieldName[i] << " consisting of " << fShieldMaterial[i]->GetName()
		    << " and mass " << shieldMass_kg << "kg." << endlog;
   }
   prevShieldX = shieldX; prevShieldY = shieldY; prevShieldZ = shieldZ;
   
   //Paddy's Edits
   // Deactivated by RH 3/17/2010 to accommodate DEMO design with 5 crystals.
   //   if(fShieldName[i]=="InnerCuShield"){
   if(false){
     //set the dimensions
     G4double eps = 0.0001*cm;                       //1 micron
     G4double cavityRadius = 3.1752*cm;              //1 micron bigger than the 4.1275 cm thick cavity needed
     G4double cavityThickness = 24.999*mm;           //2 microns thinner than the 5.0 cm thick wall 
     G4double transformX = 235.0*mm;
     G4double transformY = 0.0*mm;              
     G4double transformZ = 130.0*mm;            
     G4double rotateYaxis = 90.0*degree;
     G4double CuCF_IR = cavityRadius/2.0;
     G4double CuCF_OR = cavityRadius - eps;
     G4double plasticFoot_IR = 0.0*cm;
     G4double plasticFoot_OR = 11.9*mm;  // ?
     G4double plasticFoot_H  = 4.*mm;     // ?

     //get the materials
      G4Material *PEEK = G4Material::GetMaterial("PEEK");
      //G4Material *Titanium = G4Material::GetMaterial("Titanium");
      //G4Material *Air = G4Material::GetMaterial("Air");
      G4Material *CopperEF = G4Material::GetMaterial("Copper-EF");
      G4Material *Vacuum = G4Material::GetMaterial("Vacuum");

     //location and transformation of the tube cut outs
      G4Transform3D CFTransformer(G4RotationMatrix().rotateY(rotateYaxis), G4ThreeVector(transformX, transformY, transformZ));
      G4Transform3D NullTransformer(G4RotationMatrix().rotateY(0.*degree), G4ThreeVector(0., 0., 0.));
      G4Transform3D PlasticTransformer(G4RotationMatrix().rotateY(90.*degree), G4ThreeVector(24.0*mm, 0.0*mm, -10.0*mm));
     
     //Inserting the cavity
      G4Tubs *CFCavitySolid = new G4Tubs("CFCavitySolid", 0., cavityRadius + 2.0*mm, cavityThickness, 0.,2.0*TMath::Pi());
      fCFCavityLogical = new G4LogicalVolume(CFCavitySolid, Vacuum, "CFCavityLogical", 0, 0, 0, true);
      fCFCavityPhysical = new G4PVPlacement(CFTransformer, fCFCavityLogical, "CFCavityPhysical", shieldLogical, false, 0);
      G4VisAttributes* green = new G4VisAttributes(G4Color(0,1,0));
      fCFCavityLogical->SetVisAttributes(green);

     //Inserting the Cu cold finger
      G4Tubs *CuCFSolid = new G4Tubs("CuCFSolid", CuCF_IR, CuCF_OR, cavityThickness - eps, 0.,2.0*TMath::Pi());
      fCuCFLogical = new G4LogicalVolume(CuCFSolid, CopperEF, "CuCFLogical", 0, 0, 0, true);
      fCuCFPhysical = new G4PVPlacement(NullTransformer, fCuCFLogical, "CuCFPhysical", fCFCavityLogical, false, 0);
      G4VisAttributes* yellow = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
      fCuCFLogical->SetVisAttributes(yellow);

     //Inserting the plastic foot
      G4Tubs *PlasticFootSolid = new G4Tubs("PlasticFootSolid", plasticFoot_IR, plasticFoot_OR, plasticFoot_H, 0., 2.0*TMath::Pi());
      fPlasticFootLogical = new G4LogicalVolume(PlasticFootSolid, PEEK, "PlasticFootLogical", 0, 0, 0, true);
      fPlasticFootPhysical = new G4PVPlacement(PlasticTransformer, fPlasticFootLogical, "PlasticFootPhysical", fCFCavityLogical, false, 0);
      G4VisAttributes* red = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
      fPlasticFootLogical->SetVisAttributes(red);
   }
   

   innerShieldLogical = shieldLogical;
 }
 theDetectorLogical = shieldLogical;
}

//---------------------------------------------------------------------------//

void MJGeometryRDDetectorWBasicShield::SetDefaults()
{

  G4double height_2 = 0.0;

 // Place cryostat at origin
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0) * cm);

 // Internal Compartment
 fShieldThickness.push_back(G4ThreeVector(200, 200, 200) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Air");
 fShieldName.push_back("InternalVolume");
 height_2 += 200*mm;

 // Inner Copper Shield
 fShieldThickness.push_back(G4ThreeVector(50, 50, 50) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Copper-EF");
 fShieldName.push_back("InnerCuShield");
 height_2 += 50*mm;

 // Outer Copper Shield
 fShieldThickness.push_back(G4ThreeVector(50, 50, 50) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Copper-OFHC");
 fShieldName.push_back("OuterCuShield");
 height_2 += 50*mm;

 // Inner Lead Shield
 fShieldThickness.push_back(G4ThreeVector(100, 100, 100) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Lead-Ain");
 fShieldName.push_back("InnerPbShield");
 height_2 += 100*mm;

 // Outer Lead Shield
 fShieldThickness.push_back(G4ThreeVector(400, 400, 400) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Lead-Mod");
 fShieldName.push_back("OuterPbShield");
 height_2 += 400*mm;

 // Neutron Shield
 fShieldThickness.push_back(G4ThreeVector(400, 400, 400) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0) * mm);
 fShieldMaterialName.push_back("Borated-Poly-10%");
 fShieldName.push_back("NeutronShield");
 height_2 += 400*mm;

 // Muon Veto
 fShieldThickness.push_back(G4ThreeVector(50, 50, 50) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,-539) *mm);
 fShieldMaterialName.push_back("Scintillator-Pla");
 fShieldName.push_back("MuonVeto");
 height_2 += 50*mm;

 // Experimental Room
 fShieldThickness.push_back(G4ThreeVector(2940, 2940, 540) * mm);
 fShieldDisplacement.push_back(G4ThreeVector(0,0,0));
 fShieldMaterialName.push_back("Air");
 fShieldName.push_back("Room");
 height_2 += 540*mm;

 // Surrounding Rock
 fShieldThickness.push_back(G4ThreeVector(10, 10, 10) * meter);
 fShieldMaterialName.push_back("Rock");
 fShieldName.push_back("Rock");
 height_2 += 10*m;

}

//---------------------------------------------------------------------------//

void MJGeometryRDDetectorWBasicShield::Initialize()
{
 size_t i;

 if(fShieldMaterialName.size() != fShieldName.size() ||
    fShieldName.size() != fShieldDisplacement.size() ||
    fShieldDisplacement.size() != fShieldThickness.size()) {
   MGLog(error) << "Inconsistent size of shield parameter vectors." << endlog;
   MGLog(fatal) << endlog;
 }

    // Get Materials
 fShieldMaterial.clear();
 for(i = 0; i < fShieldMaterialName.size(); i++) {
   fShieldMaterial.push_back(G4Material::GetMaterial(fShieldMaterialName[i]));
   if(!fShieldMaterial[i]){
     MGLog(error) << "Material " << fShieldMaterialName[i] << " not found." << endlog;
     MGLog(fatal) << endlog;
   }
 }

}
