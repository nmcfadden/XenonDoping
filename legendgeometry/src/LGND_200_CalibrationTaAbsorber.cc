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
// CLASS IMPLEMENTATION:  LGND_200_CalibrationTaAbsorber.cc
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
 * AUTHOR: Ralph Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: Jun, 2018
 *
 * REVISION:
 *
 * 06-2018, Created, Ralph
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_CalibrationTaAbsorber.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_200_CalibrationTaAbsorber::LGND_200_CalibrationTaAbsorber(G4String partName, G4String serialNumber) :
  LGND_Part(partName, serialNumber, "LGND_200_CalibrationTaAbsorber", "MetalTantalum")
{;}

LGND_200_CalibrationTaAbsorber::LGND_200_CalibrationTaAbsorber(const LGND_200_CalibrationTaAbsorber & rhs) :
  LGND_Part(rhs)
{;}

LGND_200_CalibrationTaAbsorber::~LGND_200_CalibrationTaAbsorber()
{;}

G4LogicalVolume* LGND_200_CalibrationTaAbsorber::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4LogicalVolume* pVol = storePtr->GetVolume("LGND_200_CalibrationTaAbsorber", false);

  if (pVol == NULL){
    G4Tubs* CalibrationCylinderTubs = new G4Tubs("CalibrationCylinderTubs",0,17.5*mm,30*mm,0,360*deg);
	  G4Tubs* CalibrationCollimatorTubs = new G4Tubs("CalibrationCollimatorTubs",0,3.2*mm,7.95*mm,0,360*deg);
	  G4SubtractionSolid* CalibrationSubtractionSolid1 = new G4SubtractionSolid("CalibrationSubtractionSolid1",
																															              CalibrationCylinderTubs ,
																															              CalibrationCollimatorTubs,
																															              0,
																															              G4ThreeVector(0.0, 0.0, 30.95*mm));

    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.6,0.6,0.4,0.3)); 
    greyVisAtt->SetForceWireframe(false);
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(CalibrationSubtractionSolid1, material, "LGND_200_CalibrationTaAbsorber");
    pVol->SetVisAttributes(greyVisAtt);
    MGLog(debugging) << "Created LGND_200_CalibrationTaAbsorber" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing LGND_200_CalibrationTaAbsorber" << endlog;
  return pVol;
}
