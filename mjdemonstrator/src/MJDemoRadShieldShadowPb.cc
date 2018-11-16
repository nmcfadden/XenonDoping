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
// CLASS IMPLEMENTATION:  MJDemoRadShieldShadowPb.cc
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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 20, 2010
 * 
 * REVISION:
 * 
 * 09-20-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoRadShieldShadowPb.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldShadowPb::MJDemoRadShieldShadowPb(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldShadowPbDwg", "Lead-Ain")
{
  size_t shadowShieldPos = serialNumber.find("ShadowShield");
  if(shadowShieldPos!=std::string::npos) {
    size_t start = serialNumber.find_first_of("0123456789", shadowShieldPos);
    size_t length = serialNumber.find_first_not_of("0123456789", shadowShieldPos) - start;
    SetThickness(std::stoi(serialNumber.substr(start, length)));
  }
  else SetThickness(12);
}

MJDemoRadShieldShadowPb::MJDemoRadShieldShadowPb(const MJDemoRadShieldShadowPb & rhs) : 
  MJVDemoPart(rhs)
{
  G4String serialNumber = rhs.GetSerialNumber();
  size_t shadowShieldPos = serialNumber.find("ShadowShield");
  if(shadowShieldPos!=std::string::npos) {
    size_t start = serialNumber.find_first_of("0123456789", shadowShieldPos);
    size_t length = serialNumber.find_first_not_of("0123456789", shadowShieldPos) - start;
    SetThickness(std::stoi(serialNumber.substr(start, length)));
  }
  else SetThickness(12);
}

MJDemoRadShieldShadowPb::~MJDemoRadShieldShadowPb()
{;}

G4LogicalVolume* MJDemoRadShieldShadowPb::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    MGLog(debugging) << "Shadow shield thickness set to " << GetThickness() << " inches" << endlog;
    G4Box* shadowBox = new G4Box("shadowBox", GetThickness()*25.4*mm, 9.0*25.4*mm, 8.0*25.4*mm);    

    G4VisAttributes* blackVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)); // black
    blackVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(shadowBox, material, logicalName);
    pVol->SetVisAttributes(blackVisAtt); 
    MGLog(debugging) << "Created Lead Shadow Shield Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Lead Shadow Shield Logical" << endlog; 
  return pVol;
}  
