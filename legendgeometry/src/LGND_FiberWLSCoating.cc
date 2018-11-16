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
// CLASS IMPLEMENTATION:  LGND_MiniShroud.cc
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
 * AUTHOR: Neil McFadden
 * CONTACT: ncmfadde@unm.edu
 *
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_FiberWLSCoating.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

LGND_FiberWLSCoating::LGND_FiberWLSCoating(G4String partName, G4String serialNumber,G4double length) :
  LGND_Part(partName, serialNumber, "LGND_FiberWLSCoating", "TPB"),
  fLength(length*mm)
{;}

LGND_FiberWLSCoating::LGND_FiberWLSCoating(G4String partName, G4String serialNumber,G4double length,G4String shape) :
  LGND_Part(partName, serialNumber, "LGND_FiberWLSCoating", "TPB"),
  fLength(length*mm),
  fShape(shape)
{ 
  //When part is declared, logical volume is created, that way there is immediate access to internal part values
  BuildLogicalVolume();
}

LGND_FiberWLSCoating::LGND_FiberWLSCoating(G4String partName, G4String serialNumber) :
  LGND_Part(partName, serialNumber, "LGND_FiberWLSCoating", "TPB")
{;}

LGND_FiberWLSCoating::LGND_FiberWLSCoating(const LGND_FiberWLSCoating & rhs) :
  LGND_Part(rhs)
{;}

LGND_FiberWLSCoating::~LGND_FiberWLSCoating()
{;}

G4LogicalVolume* LGND_FiberWLSCoating::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial+"x"+G4UIcommand::ConvertToString(fLength);
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  G4VSolid* wlsSolid;
  G4double length_inner = .5*mm;
  G4double length_outer = .5*mm + 600.*nm;
  fRadius = length_outer-length_inner;

  if(pVol == NULL){
    //A Box is a cuboid of given half lengths
    //centred on the origin with sides parallel to the x/y/z axes.
    if(fShape == "square"){
      G4VSolid* innerSolid = new G4Box("innerSolid",length_inner,length_inner,fLength);
      G4VSolid* outerSolid = new G4Box("OuterSolid",length_outer,length_outer,fLength);
      wlsSolid = new G4SubtractionSolid("wlsSolid",outerSolid,innerSolid);
    }
    else if(fShape == "circle"){
      wlsSolid = new G4Tubs("wlsSolid",length_inner,length_outer,fLength,0,2*pi);      
    }
    else
      MGLog(error) << "Incorrect shape for fiber core : "<<fShape<<" not valid! \n use square or cirlce"<<endlog;

    pVol = new G4LogicalVolume(wlsSolid,G4Material::GetMaterial(this->GetMaterial()),logicalName); 
  }


  return pVol;
}
