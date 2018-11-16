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
// CLASS IMPLEMENTATION:  MJDemoThermalShieldSupport.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 *
 * REVISION:
 *
 * 09-10-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermalShieldSupport.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermalShieldSupport::MJDemoThermalShieldSupport(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldSupportDWG", "Vespel")
{;}

MJDemoThermalShieldSupport::MJDemoThermalShieldSupport(const MJDemoThermalShieldSupport & rhs) :
  MJVDemoPart(rhs)
{;}

MJDemoThermalShieldSupport::~MJDemoThermalShieldSupport()
{;}

G4LogicalVolume* MJDemoThermalShieldSupport::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* support = new G4Tubs("support", 0, 0.0625*25.4*mm, 0.23*25.4*mm, 0, 2*pi);//0.25

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(support, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt);
    MGLog(debugging) << "Created Thermal Shield Support Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermal Shield Support Logical" << endlog;
  return pVol;
}
