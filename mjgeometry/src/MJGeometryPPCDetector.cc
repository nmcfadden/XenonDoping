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
// $Id: MJGeometryPPCDetector.cc,v 1.1 2008-09-18 17:39:34 mitzi Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPPCCryostat.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M.Boswell
 * CONTACT: 
 * FIRST SUBMISSION: Wed. July 23, 2008
 * 
 * REVISION:
 *
 * 07/23/2008, Created, M. Boswell
 *
 */
//---------------------------------------------------------------------------//
//


#include "globals.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"



#include "mjgeometry/MJGeometryPPCDetectorMessenger.hh"
#include "mjgeometry/MJGeometryPPCDetector.hh"

using namespace CLHEP;
using namespace MaGe;



MJGeometryPPCDetector::MJGeometryPPCDetector(G4String serNum, G4bool messOff):
  MGGeometryIdealCoaxCrystal(serNum,messOff) 
{
  if (!messOff) 
    fG4Messenger = new MJGeometryPPCDetectorMessenger(this);
  else 
    fG4Messenger = 0;

}
MJGeometryPPCDetector::MJGeometryPPCDetector(const MJGeometryPPCDetector &rhs):
  MGGeometryIdealCoaxCrystal(rhs), fCrystalRadius(rhs.fCrystalRadius), 
  fCrystalHeight(rhs.fCrystalHeight), fG4Messenger(0)
{;}

MJGeometryPPCDetector::~MJGeometryPPCDetector()
{
  delete fG4Messenger;
} 

void MJGeometryPPCDetector::ConstructDetector()
{

  MGGeometryIdealCoaxCrystal::SetIsActive(true);
  MGLog(debugging) << "construct detector" << endlog;
  MGGeometryIdealCoaxCrystal::ConstructDetector();
  theDetectorLogical = MGGeometryIdealCoaxCrystal::GetDetectorLogical();

}
void MJGeometryPPCDetector::SetDefaults()
{
  
  try {
    fCrystalMaterial =  G4Material::GetMaterial("Germanium-Enr");      
    if (!fCrystalMaterial) throw "Germanium-Enr";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material."
		     << " Defining it Manually." << endlog;
    MGLog(error) << endlog;
  }


  MGGeometryIdealCoaxCrystal::SetDefaults();
  fCrystalHeight = 2.36*inch;
  fCrystalRadius = 2.56/2*inch;
  //->SetCrystalMaterial(CrystalMaterial);
  MGGeometryIdealCoaxCrystal::SetCoreBubble(false);
  MGGeometryIdealCoaxCrystal::SetCoreDepth(0.1*mm);
  MGGeometryIdealCoaxCrystal::SetCoreRadius(0.1*mm);
  MGGeometryIdealCoaxCrystal::SetHeight(fCrystalHeight);
  MGGeometryIdealCoaxCrystal::SetRadius(fCrystalRadius);
  MGGeometryIdealCoaxCrystal::SetDeadLayerConsidered(true);
  MGGeometryIdealCoaxCrystal::SetIsActive(true);


}
