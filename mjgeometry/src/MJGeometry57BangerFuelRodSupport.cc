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
// $Id: MJGeometry57BangerFuelRodSupport.cc,v 1.3 2007-02-19 21:35:02 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometry57BangerFuelRodSupport.cc
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
 * AUTHOR: R.
 * CONTACT: Henning
 * FIRST SUBMISSION: Mon Mar  7 15:18:41 PST 2005
 * 
 * REVISION:
 *
 * 03-07-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <math.h>
#include <sstream>

//#include "CLHEP/GenericFunctions/Cos.hh"
//#include "CLHEP/GenericFunctions/GenericFunctions.hh"
// Removing CLHEP in favor of TMath.
#include "TMath.h"

#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometry57Banger.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometry57BangerFuelRodSupport.hh" 

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometry57BangerFuelRodSupport::MJGeometry57BangerFuelRodSupport(
  MGGeometryDetector *mother, G4String name) :
  MJVGeometrySupportStructure(mother, name),
  fTubeMaterial(0), fTubeVisAtt(0), 
  fSupportRingMaterial(0), fSupportRingVisAtt(0)
{
  SetDefaults();
}

//---------------------------------------------------------------------------//

MJGeometry57BangerFuelRodSupport::~MJGeometry57BangerFuelRodSupport()
{
  delete fTubeVisAtt;
  delete fSupportRingVisAtt;
}

//---------------------------------------------------------------------------//

void MJGeometry57BangerFuelRodSupport::Construct()
{

  // Useful local functions, pointers, constants, and variables.
  // -----------------------------------------------------------

  MJGeometry57Banger *motherBanger = (MJGeometry57Banger*) GetMotherDetector();
  G4LogicalVolume *motherLogical = motherBanger->GetCavityLogical();
  G4double assyRadius=motherBanger->GetTemplateCrystal()->GetAssemblyRadius();
  G4double assyHeight=motherBanger->GetTemplateCrystal()->GetAssemblyHeight();
  G4double eps = 0.00001 * cm; 
  G4double cos30 = TMath::Cos(30.0 * deg);
  G4int i;
  G4ThreeVector crystalPosition;
  G4ThreeVector displacement;
  std::stringstream nameStream;

  // Construct support rings and set vis. attributes.
  // ------------------------------------------------

  if(!(fSupportRingMaterial = 
       G4Material::GetMaterial(fSupportRingMaterialName))) {
    MGLog(error) << "Support ring material " << fSupportRingMaterialName 
		 << " not found." << endlog;
    MGLog(fatal) << endlog;
  }
  G4Tubs *supportRingSolid = new G4Tubs("tubeSolid", fSupportRingInnerRadius, 
			       fSupportRingOuterRadius, 
			       fSupportRingThickness/2., 0*deg, 360*deg);
  G4LogicalVolume *supportRingLogical = 
    new G4LogicalVolume(supportRingSolid, fSupportRingMaterial, 
			"supportRingLogical", 0,0,0, true);
  fSupportRingVisAtt = new G4VisAttributes(G4Colour(0.1, 0.9, 0.0));
  fSupportRingVisAtt->SetVisibility(true);
  fSupportRingVisAtt->SetForceSolid(true); 
  supportRingLogical->SetVisAttributes(fSupportRingVisAtt);


  // Position Support Rings
  // ----------------------
  for(i = 0; i < 57; i++) {
    nameStream.str("");
    nameStream << "supportRing" << i;
    crystalPosition = motherBanger->GetCrystalLocation(i);
    displacement.set(0., 0., -(assyHeight + fSupportRingThickness)/2. - eps);
    new G4PVPlacement(0, crystalPosition + displacement, supportRingLogical,
                      nameStream.str(), motherLogical, false, i);
  }
				     

  // Construct tubes and set visualization attributes
  // ------------------------------------------------

  if(!(fTubeMaterial = G4Material::GetMaterial(fTubeMaterialName))) {
    MGLog(error) << "Tube material " << fTubeMaterialName << " not found."
		 << endlog;
    MGLog(fatal) << endlog;
  }
  MGLog(routine) << "Fuelrod length : " << fTubeLength 
		 << " outer radius: " << fTubeOuterRadius 
		 << " inner radius: " << fTubeInnerRadius << endlog;
  G4Tubs *tubeSolid = new G4Tubs("tubeSolid", fTubeInnerRadius, 
				 fTubeOuterRadius, fTubeLength/2., 
				 0*deg, 360*deg);
  G4LogicalVolume *tubeLogical = 
    new G4LogicalVolume(tubeSolid, fTubeMaterial, "tubeLogical", 0,0,0, true);
  fTubeVisAtt = new G4VisAttributes(G4Colour(0.8, 0.2, 0.0));
  fTubeVisAtt->SetVisibility(true);
  fTubeVisAtt->SetForceSolid(true); 
  tubeLogical->SetVisAttributes(fTubeVisAtt);


  // Position tubes
  // --------------

  // Grab x,y coordinates of "fuel rods"
  G4ThreeVector rodPos[19];
  i = 0;
  for(G4int nFound = 0; i < 57 && nFound < 19; i++){
    crystalPosition = motherBanger->GetCrystalLocation(i);
    if(crystalPosition.z() > 
       motherBanger->GetTemplateCrystal()->GetAssemblyHeight()/2.) {
      rodPos[nFound] = crystalPosition;
      rodPos[nFound++].setZ(0.0);
    }
  }

  // Place three tubes for each "fuel rod"
  G4double distFromCore = assyRadius+fTubeCrystalSeparation + fTubeOuterRadius;
  for(i = 0; i < 19; i++) {
    if(fabs(rodPos[i].y()) > 3. * assyRadius * cos30 || 
       fabs(rodPos[i].y()) < assyRadius * cos30) {
      displacement.set(cos30, 0.5, 0.0);
      displacement *= distFromCore;
    } else { 
      displacement.set(0.0, 1.0, 0.0);
      displacement *= distFromCore;
    }
    nameStream.str("");
    nameStream << "tube" << motherBanger->GetCrystalName(i) << "a";
    new G4PVPlacement(0, rodPos[i] + displacement, tubeLogical,
                      nameStream.str(), motherLogical, false, i);
    displacement.rotateZ(120.0 * deg);
    nameStream.str("");
    nameStream << "tube" << motherBanger->GetCrystalName(i) << "b";
    new G4PVPlacement(0, rodPos[i] + displacement, tubeLogical,
                      nameStream.str(), motherLogical, false, i);
    displacement.rotateZ(120.0 * deg);
    nameStream.str("");
    nameStream << "tube" << motherBanger->GetCrystalName(i) << "c";
    new G4PVPlacement(0, rodPos[i] + displacement, tubeLogical,
                      nameStream.str(), motherLogical, false, i);
  }
}

//---------------------------------------------------------------------------//

void MJGeometry57BangerFuelRodSupport::SetDefaults()
{
  // Specs. from e-mail from Harry, 3/7/2005
  MJGeometry57Banger *motherBanger = (MJGeometry57Banger*) GetMotherDetector();

  G4double supportRingWidth = 0.3 * cm;
  fSupportRingOuterRadius = 
    motherBanger->GetTemplateCrystal()->GetAssemblyRadius();
  fSupportRingInnerRadius = fSupportRingOuterRadius - supportRingWidth;
  fSupportRingThickness = supportRingWidth;
  fSupportRingMaterialName = "Polyethylene";
  fTubeOuterRadius = 0.25 * cm;
  fTubeInnerRadius = 0.175 * cm; // Gives 1g / cm of tube density.
  fTubeMaterialName = "Copper-EF";
  fTubeLength = motherBanger->GetCavityHeight() - 0.1*cm;
  fTubeCrystalSeparation = 0.4 * cm; 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
