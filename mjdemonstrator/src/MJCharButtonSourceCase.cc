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
// CLASS IMPLEMENTATION:  MJCharButtonSourceCase.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Radioactive button source case
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
#include "G4SubtractionSolid.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJCharButtonSourceCase.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJCharButtonSourceCase::MJCharButtonSourceCase(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ButtonSourceCase", "Polyethylene")
{;}

MJCharButtonSourceCase::MJCharButtonSourceCase(const MJCharButtonSourceCase & rhs) : 
  MJVDemoPart(rhs)
{;}

MJCharButtonSourceCase::~MJCharButtonSourceCase()
{;}

G4LogicalVolume* MJCharButtonSourceCase::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Define the polyethylene case
    G4double radius = 0.5*25.4*mm;
    G4double hz = 0.11*25.4*mm;
    G4Tubs* cylinder = new G4Tubs("Cylinder",0,radius,hz,0,2*pi);

    // Create a hollow the size of the button source
    // needs to be updated as soon as specs on the actual source are procured
    G4Tubs* sourceHollow = new G4Tubs("Hollow", 0, 0.01*mm, 0.0017715*mm, 0, 2*pi);
    G4SubtractionSolid* buttonSourceCase = new G4SubtractionSolid("ButtonSourceCase", cylinder, sourceHollow);

    G4VisAttributes* orangeVisAtt = new G4VisAttributes(G4Colour(0.8,0.4,0.0)); // orange
    orangeVisAtt->SetForceWireframe( false );
    G4Material* material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(buttonSourceCase, material, logicalName);
    pVol->SetVisAttributes(orangeVisAtt);  
    MGLog(debugging) << "Created Button Source Case Logical" << endlog;

  }
  else  MGLog(debugging) << "Using pre-existing Button Source Case Logical" << endlog; 
  return pVol;
}  
