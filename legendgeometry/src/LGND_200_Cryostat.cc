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
// CLASS IMPLEMENTATION:  LGND_200_Cryostat.cc
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
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_Cryostat.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_200_Cryostat::LGND_200_Cryostat(G4String partName, G4String serialNumber) :
  LGND_Part(partName, serialNumber, "Gerda_Cryostat", "Copper-EF")
{
  fOuterRadius = 2.0*m;
  fOuterHeight = 2.0*m;
  fThickness = 0.05*m;
  fInnerRadius = fOuterRadius - fThickness;
  fInnerHeight = fOuterHeight - fThickness;
}

LGND_200_Cryostat::LGND_200_Cryostat(const LGND_200_Cryostat & rhs) :
  LGND_Part(rhs)
{;}

LGND_200_Cryostat::~LGND_200_Cryostat()
{;}

G4LogicalVolume* LGND_200_Cryostat::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* tank = new G4Tubs("tank",0, fOuterRadius, fOuterHeight, 0, 2*pi);
    G4Tubs* cavity = new G4Tubs("cavity", 0, fInnerRadius, fInnerHeight, 0, 2*pi);
    G4SubtractionSolid* cryostat = new G4SubtractionSolid("cryostat", tank, cavity);

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,0.5)); //copper color
    copperVisAtt->SetForceWireframe( true );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(cryostat, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Gerda Cryostat Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Gerda Cryostat Logical" << endlog;
  return pVol;
}
