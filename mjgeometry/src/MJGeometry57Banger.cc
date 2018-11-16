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
// $Id: MJGeometry57Banger.cc,v 1.7 2007-02-19 21:35:02 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometry57Banger.cc
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
 * FIRST SUBMISSION: Thu Jan  6 12:47:55 PST 2005
 * 
 * REVISION:
 *
 * 01-06-2005, Created, R. Henning
 * 02-10-2005, Added list of crystal names. R. Henning 
 * 02-01-2010, Removed reference to Platter support that was never used. R Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>

#include "TMath.h"

#include "globals.hh"
#include "G4ios.hh"
#include "G4Box.hh"
#include "G4Color.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Polyhedra.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometry57BangerFuelRodSupport.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometry57Banger.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJGeometry57Banger::MJGeometry57Banger(G4String serNum) :
  MGGeometryDetector(serNum), fPhysicalCrystals(0), fCavityLogical(0), 
  fHexagonVisAtt(0), fShieldVisAtt(0), 
  fCrystalLocations(0), fSupportStructure(0)
{
  fTemplateCrystal = new MJGeometryIdealCoaxWithShield("Template", false);
  SetDefaults();
}

//---------------------------------------------------------------------------//

MJGeometry57Banger::~MJGeometry57Banger()
{
  fCrystalNames.clear();
  fCrystalLocations.clear();
  delete []fPhysicalCrystals;
  delete fHexagonVisAtt;
  delete fShieldVisAtt;
  delete fTemplateCrystal;
  delete fSupportStructure;
}

//---------------------------------------------------------------------------//

void MJGeometry57Banger::ConstructDetector()
{
  // Before this method is invoked, the fTemplateCrystal needs to be defined
  // via messenger or otherwise. 
  fTemplateCrystal->ConstructDetector();
  fCrystalSpacingXY = 2.0 * fTemplateCrystal->GetAssemblyRadius() 
    + fCrystalSeparationXY;
  fCrystalSpacingZ = fTemplateCrystal->GetAssemblyHeight() 
    + fCrystalSeparationZ;
  
  // Build containing hexagon.
  //--------------------------

   // Useful CLHEP functions and local constants.
  G4double eps = 0.00001 * cm;  // Small fudge factor to extend hexagon by.
  G4double (*myCos)(G4double) = TMath::Cos;
  G4double cos30 = myCos(30.0 * deg);

  if(fCrystalSpacingXY < 2.0 * fTemplateCrystal->GetAssemblyRadius()) {
    MGLog(warning) << "Crystal assembly radius too large!\nfCrystalSpacing =" 
	           << fCrystalSpacingXY << "\nfAssemblyRadius = "
	           << fTemplateCrystal->GetAssemblyRadius() << '\n'
	           << "Setting fCrystalSpacing to 2,1*fAssemblyRadius."
	           << endlog;
    fCrystalSpacingXY = 2.1 * fTemplateCrystal->GetAssemblyRadius();
  }

  if(fCrystalSpacingZ < fTemplateCrystal->GetAssemblyHeight()) {
    MGLog(warning) << "Crystal assembly height to large!\n"
	           << "Setting fCrystalSpacingZ to 1.1*fAssemblyHeight."
	           << endlog;
    fCrystalSpacingZ = 2.1 * fTemplateCrystal->GetAssemblyHeight();
  }

  G4double c30csp = cos30 * fCrystalSpacingXY;
  G4double s30csp = 0.5 * fCrystalSpacingXY;

  fEdgeRadius = 2.0 * c30csp + fTemplateCrystal->GetAssemblyRadius() + 2*eps
    + fHexagonEnvelope;
  G4double rInner[2] = {0.0, 0.0};
  G4double rOuter[2] = {fEdgeRadius, fEdgeRadius};
  fHeight = 2.0*fCrystalSpacingZ + fTemplateCrystal->GetAssemblyHeight() + 
    + 1.5 * cm;
  G4double z[2] = {-fHeight/2.0, fHeight/2.0};
  G4int numSides, numz;
  G4Polyhedra *hexagonSolid = new G4Polyhedra("hexagon",
					 0.0 * deg,
					 360.0 * deg,
					 numSides = 6,
					 numz = 2,
					 z, rInner, rOuter);
  fHexagonVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0, 0.0));
  fHexagonVisAtt->SetVisibility(true);
  fHexagonVisAtt->SetForceWireframe(true); 

  // Build shield, if required.
  // Simple shield for now, will add more functionality later.
  // Simple shield is 50cm of outer lead and 0.5 cm of inner Cu.
  // A 4 cm high "attic" is added on top of the upper Cu shield.
  // This is where electronics would be housed and serves as a source.
  // Plan would be to create special shield class and allow user to select
  // different shielding configurations.
  if(fShieldOn) {
    fShieldVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.0));
    fShieldVisAtt->SetVisibility(true);
    fShieldVisAtt->SetForceWireframe(true); 
    G4double cavityRadius = fEdgeRadius / myCos(22.5 * deg) + eps;
    G4Tubs *outerLeadShieldSolid = new G4Tubs("outerLeadShieldSolid", 
					0., cavityRadius + 20.0*cm, 
					fHeight / 2.0 + 20.0*cm, 
					0*deg, 360*deg);
    MGLog(routine) << "Lead Shield: height : " << fHeight/2.0/cm + 20.0
		   << " outer radius: " << cavityRadius/cm + 0.5+20.0<<endlog; 
    theDetectorLogical = new G4LogicalVolume(outerLeadShieldSolid, 
			  G4Material::GetMaterial("Lead-Ain"),
			  "outerShieldLogical");
    theDetectorLogical->SetVisAttributes(fShieldVisAtt);
    MGLog(routine) << "Copper cavity: height : " << fHeight/2.0/cm + 0.5
		   << " outer radius: " << cavityRadius/cm + 0.5 << endlog; 
    G4Tubs *innerCuShieldSolid = new G4Tubs("innerCuShieldSolid", 0., 
				      cavityRadius + 0.5 * cm, 
				      fHeight/2.0 + 0.5 * cm, 
				      0*deg, 360*deg);
    G4LogicalVolume* innerCuShieldLogical = new 
      G4LogicalVolume(innerCuShieldSolid, G4Material::GetMaterial("Copper-EF"),
		      "innerShieldLogical");
    innerCuShieldLogical->SetVisAttributes(fShieldVisAtt);
    new G4PVPlacement(0, G4ThreeVector(0,0,0), innerCuShieldLogical, 
		      "innerShieldPhysical", theDetectorLogical, false, 0);
    G4Tubs *cavitySolid = new G4Tubs("cavitySolid", 0., cavityRadius, 
				     fHeight/2. + eps, 0*deg, 360*deg);
    fCavityLogical = new
      G4LogicalVolume(cavitySolid, G4Material::GetMaterial("Vacuum"), 
		      "cavityLogical");
    fCavityLogical->SetVisAttributes(fShieldVisAtt);
    new G4PVPlacement(0, G4ThreeVector(0,0,0), fCavityLogical, 
                      "cavityPhysical", innerCuShieldLogical, false, 0);
    G4Tubs *atticSolid = new G4Tubs("atticSolid", 0., cavityRadius, 
				    2.0 * cm, 0*deg, 360 * deg);
    G4LogicalVolume *atticLogical = new
      G4LogicalVolume(atticSolid, G4Material::GetMaterial("Air"), 
		      "atticLogical");
    atticLogical->SetVisAttributes(fShieldVisAtt);
    new G4PVPlacement(0, G4ThreeVector(0,0,fHeight/2.+2.5*cm+eps),
                      atticLogical, "atticPhysical", theDetectorLogical, false, 0);
  } else {
    theDetectorLogical = new G4LogicalVolume(hexagonSolid, 
					   G4Material::GetMaterial("Vacuum"), 
					   "hexagonLogical");
    fCavityLogical = theDetectorLogical;
    theDetectorLogical->SetVisAttributes(fHexagonVisAtt);
  }

  // Add crystals.
  // -------------

  fTotalNumberOfCrystals = fNumberOfCrystalsPerLayer * fNumberOfLayers;
  fPhysicalCrystals = new G4PVPlacement*[fTotalNumberOfCrystals];
  if(!fPhysicalCrystals) {
    MGLog(error) << "Could not allocate memory for physical crystals!"<<endlog;
    MGLog(fatal) << endlog;
  }

  // Define coordinates for 57 Banger. 
  // Use Dave Jordan's "outside-in" numbering scheme from white paper.
  G4double xCoord[] = {

    // Outer layer
    -fCrystalSpacingXY, 0.0, fCrystalSpacingXY,
    1.5 * fCrystalSpacingXY, 2.0 * fCrystalSpacingXY, 
    1.5 * fCrystalSpacingXY, fCrystalSpacingXY, 0.0,
    -fCrystalSpacingXY, -1.5 * fCrystalSpacingXY, 
    -2.0 * fCrystalSpacingXY, -1.5 * fCrystalSpacingXY, 

    // Inner layer
    -s30csp, s30csp, fCrystalSpacingXY, s30csp, -s30csp, -fCrystalSpacingXY, 
    
    // Center
    0.0};

  G4double yCoord[] = {

    // Outer Layer
    -2.0 * c30csp, -2.0 * c30csp, -2.0 * c30csp, 
    -c30csp, 0.0, c30csp, 2.0 * c30csp, 
    2.0 * c30csp, 2.0 * c30csp, c30csp, 0.0, -c30csp,

    // Inner Layer
    -c30csp, -c30csp, 0.0, c30csp, c30csp, 0.0,

    // Center
    0.0};

  G4double zCoord[] = { -fCrystalSpacingZ, 0.0, fCrystalSpacingZ };

  // Place and name crystals in hexagon.
  stringstream nameStream, layerStream;
  fCrystalNames.clear();
  fCrystalLocations.clear();
  MJGeometryIdealCoaxWithShield *crystalMJ;
  for(G4int nLayer = 0, nCrystal = 0; nLayer < fNumberOfLayers; nLayer++){
    layerStream.str("");
    layerStream << "L" << nLayer;
    for(G4int i = 0; i < fNumberOfCrystalsPerLayer; i++) {
      nameStream.str("");
      nameStream << layerStream.str() << "C" << i;
      fCrystalNames.push_back(nameStream.str());
      crystalMJ = new MJGeometryIdealCoaxWithShield(*fTemplateCrystal);
      crystalMJ->SetSerialNumber(nameStream.str());
      crystalMJ->ConstructDetector();
      fCrystalLocations.push_back(
	 G4ThreeVector(xCoord[i], yCoord[i] , zCoord[nLayer]));
      MGLog(routine) << xCoord[i]/cm << ", " << yCoord[i] << ", " << zCoord[nLayer] 
	     << endlog;
      fPhysicalCrystals[nCrystal] = 
	new G4PVPlacement(0,
			  fCrystalLocations[nCrystal],
			  crystalMJ->GetDetectorLogical(),
			  nameStream.str(),
			  fCavityLogical,
			  false,
			  nCrystal);
      nCrystal++;
      delete crystalMJ;
      crystalMJ = 0;
    } 
  }

  ConstructSupportStructure();
}

//---------------------------------------------------------------------------//

void MJGeometry57Banger::ConstructSupportStructure()
{
  if(fSupportStructureName == "FuelRod01") {
    MGLog(routine) << "FuelRod01 supports constructed" << endlog;
    fSupportStructure = new MJGeometry57BangerFuelRodSupport(this,"Fuelrod01");
    fSupportStructure->Construct();
  } else {
    MGLog(routine) << "No supports constructed." << endlog;
    fSupportStructure = 0;
  }
}

//---------------------------------------------------------------------------//

void MJGeometry57Banger::SetDefaults()
{
  // Default values. Meant to be adjusted.
  // Note that these may not be consistent with crystal geometry. 
  // Consistency will be checked in DetectorConstruction().
  
  fShieldOn = false;
  fHexagonEnvelope = 2.0 * cm;
  fNumberOfLayers = 3;
  fNumberOfCrystalsPerLayer = 19;
  fTotalNumberOfCrystals = fNumberOfCrystalsPerLayer * fNumberOfLayers;
  fCrystalSeparationXY = 1.0 * cm;
  fCrystalSeparationZ = 1.2 * cm;
  fSupportStructureName = "FuelRod01"; // Set w/ mesenger later.
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
  
