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
// CLASS IMPLEMENTATION:  MJDemoContactPin.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * BeGe contact pin
 * Part Origin: crystal contact point
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoContactPin.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoContactPin::MJDemoContactPin(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-009", "Copper-EF"),
  fDiameter(0.04*25.4*mm), fLength(0.349*25.4*mm)
{;}

MJDemoContactPin::MJDemoContactPin(const MJDemoContactPin & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoContactPin::~MJDemoContactPin()
{;}

G4LogicalVolume* MJDemoContactPin::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0, 0.297*25.4*mm, 0.297*25.4*mm, 0.351*25.4*mm};
    G4double rConeInner[] = {0, 0, 0, 0};
    G4double rConeOuter[] = {0.02*25.4*mm, 0.02*25.4*mm, 0.034*25.4*mm, 0.034*25.4*mm};
    G4Polycone* contactPin = new G4Polycone("Pin", 0, 2*pi, 4, zConePlanes, rConeInner, rConeOuter);

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(contactPin, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Contact Pin Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Contact Pin Logical" << endlog; 
  return pVol;
}  
