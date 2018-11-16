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
// CLASS IMPLEMENTATION:  LGND_200_CalibrationCuAbsorber.cc
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
#include "legendgeometry/LGND_200_CalibrationCuAbsorber.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_200_CalibrationCuAbsorber::LGND_200_CalibrationCuAbsorber(G4String partName, G4String serialNumber) :
  LGND_Part(partName, serialNumber, "LGND_200_CalibrationCuAbsorber", "Copper-EF")
{;}

LGND_200_CalibrationCuAbsorber::LGND_200_CalibrationCuAbsorber(const LGND_200_CalibrationCuAbsorber & rhs) :
  LGND_Part(rhs)
{;}

LGND_200_CalibrationCuAbsorber::~LGND_200_CalibrationCuAbsorber()
{;}

G4LogicalVolume* LGND_200_CalibrationCuAbsorber::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4LogicalVolume* pVol = storePtr->GetVolume("LGND_200_CalibrationCuAbsorber", false);

  if (pVol == NULL){
		G4Tubs* CalibrationCylinderCap = new G4Tubs("CalibrationCylinderCap",0,6.3*mm,5.6*mm,0,360*deg);
  	G4Tubs* CalibrationCylinderCap2 = new G4Tubs("CalibrationCylinderCap2",0,3.3*mm,4.1*mm,0,360*deg);
	  G4SubtractionSolid* CalibrationSubtractionSolid3 = new G4SubtractionSolid("CalibrationSubtractionSolid3",
																																              CalibrationCylinderCap,
																																              CalibrationCylinderCap2,
																																		          0,
																																              G4ThreeVector(0.0, 0.0, -1.5*mm));
    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.8,0.2,0,0.3)); 
    greyVisAtt->SetForceWireframe(false);
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(CalibrationSubtractionSolid3, material, "LGND_200_CalibrationCuAbsorber");
    pVol->SetVisAttributes(greyVisAtt);
    MGLog(debugging) << "Created LGND_200_CalibrationCuAbsorber" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing LGND_200_CalibrationCuAbsorber" << endlog;
  return pVol;
}
