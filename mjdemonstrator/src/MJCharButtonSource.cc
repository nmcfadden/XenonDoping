//---------------------------------------------------------------------------//
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
//                                                          
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJCharButtonSource.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Radioactive button source
 * Part Origin: Center of cylinder
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJCharButtonSource.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJCharButtonSource::MJCharButtonSource(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ButtonSource", "sourceMaterial")
{;}

MJCharButtonSource::MJCharButtonSource(const MJCharButtonSource & rhs) : 
  MJVDemoPart(rhs)
{;}

MJCharButtonSource::~MJCharButtonSource()
{;}

G4LogicalVolume* MJCharButtonSource::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Define the source material
    G4String sourceMaterialName = this->GetMaterial();
    G4String symbol;
    G4double z, a, density;
    if(sourceMaterialName == "Barium-133")
      {
	symbol = "Ba";
	z = 56.;
	a = 133.*g/mole;
	density = 3.51*g/cm3;
      }
    else if(sourceMaterialName == "Cobalt-60")
      {
	symbol = "Co";
	z = 27.;
	a = 60.*g/mole;
	density = 8.9*g/cm3;
      }
    else if(sourceMaterialName == "Americium-241")
      {
	symbol = "Am";
	z = 95.;
	a = 241.*g/mole;
	density = 12.0*g/cm3;
      }
    else
      {
	MGLog(debugging) << "No corresponding source material found for " << sourceMaterialName << endlog;
	MGLog(debugging) << "Using Barium-133 as default" << endlog;
	symbol = "Ba";
	z = 56.;
	a = 133.*g/mole;
	density = 3.51*g/cm3;
      }
    G4Element* sourceElement = new G4Element(sourceMaterialName, symbol, z, a);
    G4Material* sourceMaterial = new G4Material(sourceMaterialName, density, 1);
    sourceMaterial->AddElement(sourceElement, 1);

    // Define the source geometry
    // Matches the "hole" in ButtonSourceCase, needs to be updated with correct specs
    // Currently is created for the amount of material necessary to generate 1uCi of activity in Ba-133
    G4double radius = 0.01*mm;
    G4double hz = 0.0017715*mm; // half height
    G4Tubs* buttonSource = new G4Tubs("ButtonSource",0,radius,hz,0,2*pi);

    G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    redVisAtt->SetForceWireframe( false );
    // material overridden to place custom material
    //G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(buttonSource, sourceMaterial, logicalName);
    pVol->SetVisAttributes(redVisAtt);  
    MGLog(debugging) << "Created Button Source Logical" << endlog;

  }
  else  MGLog(debugging) << "Using pre-existing Button Source Logical" << endlog; 
  return pVol;
}  
