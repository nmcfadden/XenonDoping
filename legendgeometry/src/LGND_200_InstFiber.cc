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
// CLASS IMPLEMENTATION:  LGND_200_InstFiber.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 * FIRST SUBMISSION: 
 *
 * REVISION:
 *
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
#include "legendgeometry/LGND_200_InstFiber.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

LGND_200_InstFiber::LGND_200_InstFiber(G4String partName, G4String serialNumber, G4String material) :
  LGND_Part(partName, serialNumber,"Gerda_Fiber",material)
{;}
LGND_200_InstFiber::LGND_200_InstFiber(const LGND_200_InstFiber & rhs):
  LGND_Part(rhs)
{;}

LGND_200_InstFiber::~LGND_200_InstFiber()
{;}

G4LogicalVolume* LGND_200_InstFiber::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  
  G4double fiber_length    = 1.90*m; //TODO what is the real length?
  //Taken from GEGeometryLArINstHybrid.cc
  G4double fiber_thickness = 1.0*mm;
  G4double fiber_cladding_thickness = 0.01*fiber_thickness; // it's 2% of total...so each side has 1%
  //construct fiber
  if(pVol == NULL && this->GetMaterial() == "PolystyreneFiber"){

    G4Tubs * fiber = new G4Tubs("fiber",0,fiber_thickness,fiber_length,0,2*pi);

    G4VisAttributes* fiberAttributes = new G4VisAttributes(G4Colour(0,1,0));
    fiberAttributes->SetForceWireframe(true);
    G4Material *material;
    if(!this->GetMaterial().isNull())
      material = G4Material::GetMaterial(this->GetMaterial());
    else{
      MGLog(debugging) << "WARNING! no material set for InstrFiber" <<endlog;
      return NULL;
    }
    pVol = new G4LogicalVolume(fiber,material,logicalName);
    pVol->SetVisAttributes(fiberAttributes);
    MGLog(debugging) << "Created Gerda Fiber Logical" << endlog;

  }
  //construct fiber cladding
  else if (pVol == NULL && this->GetMaterial() == "PMMA"){
    G4Tubs* fiber_cladding = new G4Tubs("fiber_cladding",fiber_thickness,fiber_thickness+fiber_cladding_thickness,fiber_length,0,2*pi);
    G4VisAttributes* fiberAttributes = new G4VisAttributes(G4Colour(0,1,0));
    fiberAttributes->SetForceWireframe(true);
    G4Material *material;
    if(!this->GetMaterial().isNull())
      material = G4Material::GetMaterial(this->GetMaterial());
    else{
      MGLog(debugging) << "WARNING! no material set for InstrFiber" <<endlog;
      return NULL;
    }
    pVol = new G4LogicalVolume(fiber_cladding,material,logicalName);
    pVol->SetVisAttributes(fiberAttributes);
    MGLog(debugging) << "Created Gerda Fiber Logical" << endlog;
  }
  else if(pVol == NULL && this->GetMaterial() == "Silicon"){
    G4Tubs* fiber_reflector = new G4Tubs("fiber_reflector",0,fiber_thickness+fiber_cladding_thickness,1.0*mm,0,2*pi);
    //Someone needs to tell Geant that they spelled color wrong...
    G4VisAttributes* fiberAttributes = new G4VisAttributes(G4Colour(1,0,0));
    fiberAttributes->SetForceWireframe(true);
    G4Material *material;
    if(!this->GetMaterial().isNull())
      material = G4Material::GetMaterial(this->GetMaterial());
    else{
      MGLog(debugging) << "WARNING! no material set for InstrFiber" <<endlog;
      return NULL;
    }
    pVol = new G4LogicalVolume(fiber_reflector,material,logicalName);
    pVol->SetVisAttributes(fiberAttributes);
  }

  return pVol;
}
