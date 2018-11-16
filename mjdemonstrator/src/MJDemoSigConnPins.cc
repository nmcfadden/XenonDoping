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
// CLASS IMPLEMENTATION:  MJDemoSigConnPins.cc
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
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 4, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Now includes all 8 pins instead of a single pin, I. Guinn
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSigConnPins.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;
using namespace MaGe;


MJDemoSigConnPins::MJDemoSigConnPins(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "SigConnPins", "Brass")
{;}

MJDemoSigConnPins::MJDemoSigConnPins(const MJDemoSigConnPins & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSigConnPins::~MJDemoSigConnPins()
{;}

G4LogicalVolume* MJDemoSigConnPins::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* pin = new G4Tubs("sig_pin", 0, GetDiam()/2., GetLength()/2., 0, 2.*pi);
    G4VSolid* pins = new G4DisplacedSolid("1sig_pin", pin, G4RotateZ3D(22.5*deg)*G4TranslateX3D(GetRho()));
    for(int i=1; i<8; i++) {
      pins = new G4UnionSolid((char)('1'+i)+string("sig_pins"), pins, pin, G4RotateZ3D((22.5+45*i)*deg)*G4TranslateX3D(GetRho()));
    }
    
    G4VisAttributes* visAtt = new G4VisAttributes(yellow);
    visAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(pins, material, logicalName);
    pVol->SetVisAttributes(visAtt); 
    MGLog(debugging) << "Created SigConnPins Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing SigConnPins Logical" << endlog; 
  return pVol;
}
