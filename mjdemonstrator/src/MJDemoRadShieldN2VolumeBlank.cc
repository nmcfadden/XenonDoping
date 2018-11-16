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
// CLASS IMPLEMENTATION:  MJDemoRadShieldN2VolumeBlank.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: greenmp@ornl.gov
 * FIRST SUBMISSION: June 11, 2015
 * 
 * REVISION:
 * 
 * 06-11-2015, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoRadShieldN2VolumeBlank.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldN2VolumeBlank::MJDemoRadShieldN2VolumeBlank(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldN2BlankDwg", "Nitrogen-Gas")
{;}

MJDemoRadShieldN2VolumeBlank::MJDemoRadShieldN2VolumeBlank(const MJDemoRadShieldN2VolumeBlank & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoRadShieldN2VolumeBlank::~MJDemoRadShieldN2VolumeBlank()
{;}

G4LogicalVolume* MJDemoRadShieldN2VolumeBlank::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* nitrogen = new G4Box("nitrogen", 8.015*25.4*mm, 7.905*25.4*mm, 9.953*25.4*mm);
    
    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1, 1, 1)); // white
    whiteVisAtt->SetForceWireframe(true);
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(nitrogen, material, logicalName);
    pVol->SetVisAttributes(whiteVisAtt); 
    MGLog(debugging) << "Created Blank N2 Volume Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Blank N2 Volume Logical" << endlog;
  return pVol;
}  
