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
// CLASS IMPLEMENTATION:  LGND_DUSupportRod.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Part Origin:  Coaxial with rod, coincident with top.
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Dec 2, 2017
 *
 * REVISION:
 *
 * 12-02-2017, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_DUSupportRod.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_DUSupportRod::LGND_DUSupportRod(G4String partName, G4String serialNumber, G4double length) :
  LGND_Part(partName, serialNumber, "LGND_DUSupportRod", "Copper-EF"),
  fLength(length*mm)
{;}

LGND_DUSupportRod::LGND_DUSupportRod(const LGND_DUSupportRod & rhs) :
  LGND_Part(rhs)
{;}

LGND_DUSupportRod::~LGND_DUSupportRod()
{;}

G4LogicalVolume* LGND_DUSupportRod::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial+"x"+G4UIcommand::ConvertToString(fLength);

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* rod = new G4Tubs("rod", 0.0*mm, 1.5*mm, fLength/2.0*mm, 0, 2*pi);
    G4Box* tab = new G4Box("tab", 9.25*mm, 2.5*mm, 0.75*mm);
    G4UnionSolid* support = new G4UnionSolid("support", rod, tab, 0, G4ThreeVector(-6.75*mm, 0, (-fLength/2+8.5-0.75)*mm));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,0.5)); //copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(support, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created DU Support Rod Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Support Rod Logical" << endlog;
  return pVol;
}
