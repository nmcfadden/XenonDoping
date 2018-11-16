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
/**                                                            
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoHVForkClampPin.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the copper cable guide. Part origin: Center of the hex hole
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 04-23-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polycone.hh"
#include "G4DisplacedSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHVForkClampPin.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVForkClampPin::MJDemoHVForkClampPin(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-147", "Vespel")
{;}

MJDemoHVForkClampPin::MJDemoHVForkClampPin(const MJDemoHVForkClampPin & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVForkClampPin::~MJDemoHVForkClampPin()
{;}

G4LogicalVolume* MJDemoHVForkClampPin::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    //polycone body
    G4double zPlanes[] = {-0.07*25.4*mm, -0.0665*25.4*mm, 0, 0, 0.030*25.4*mm};
    G4double rInner[] = {0, 0, 0, 0, 0};
    G4double rOuter[] = {0.0305/2*25.4*mm, 0.0375/2*25.4*mm, 0.0375/2*25.4*mm, 0.050/2*25.4*mm, 0.050/2*25.4*mm};
    G4Polycone* pin = new G4Polycone("pin", 0, 2*pi, 5, zPlanes, rInner, rOuter);
    //Below makes the logical volume
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.545,0.27,0.075)); // saddlebrown
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(pin, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created HV ClampPin Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Inner Cable Guide Logical" << endlog; 
  return pVol;
}  
