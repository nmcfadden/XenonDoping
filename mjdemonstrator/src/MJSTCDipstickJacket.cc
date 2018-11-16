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
//      
// CLASS IMPLEMENTATION:  MJSTCDipstickJacket.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
 * Part Origin: At the (x,y,z) center of the Jacket.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 11-14-2013, Updated visualization properties to make compatible with Ray Tracer, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDipstickJacket.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDipstickJacket::MJSTCDipstickJacket(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCDipstickJacketDwg", "SS303") //partName, serialNumber, drawingNumber, material
{;}

MJSTCDipstickJacket::MJSTCDipstickJacket(const MJSTCDipstickJacket & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDipstickJacket::~MJSTCDipstickJacket()
{;}

G4LogicalVolume* MJSTCDipstickJacket::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
 	G4Tubs* DipstickJacket = new G4Tubs("DipstickJacket", 24.892*mm, 25.4*mm, 121.717*mm, 0, 2*pi);
 	
    G4VisAttributes* SSVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    SSVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(DipstickJacket, material, logicalName);
    pVol->SetVisAttributes(SSVisAtt); 
    MGLog(debugging) << "Created STC Dipstick Jacket Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dipstick Jacket Logical" << endlog; 
  return pVol;
}  
