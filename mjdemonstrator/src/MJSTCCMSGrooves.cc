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
// CLASS IMPLEMENTATION:  MJSTCCMSGrooves.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from MJ83-01-030 rev0
 * Part Origin: The (x,y,z) middle of the grooves
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Tuesday January 21 2014
 * 
 * REVISION:
 * 01-21-2014, Created. Used to be a part of ColdPlateAssembly. I separated
 *             it to make it easier to rotate it through the messenger, J. MacMullin
  *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCCMSGrooves.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCCMSGrooves::MJSTCCMSGrooves(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCCMSGroovesDwg", "Aluminum") 
{;}

MJSTCCMSGrooves::MJSTCCMSGrooves(const MJSTCCMSGrooves & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCCMSGrooves::~MJSTCCMSGrooves()
{;}

G4LogicalVolume* MJSTCCMSGrooves::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* bulk = new G4Tubs("bulk", 50.927*mm, 57.15*mm, 15.875*mm, 0, 2*pi);
	G4Tubs* groove = new G4Tubs("groove", (57.15-3.81)*mm, 57.15*mm, 0.635*mm, 0, 2*pi);
    G4UnionSolid* setof2 = new G4UnionSolid("setof2", groove, groove,
    										0, G4ThreeVector(0*mm, 0*mm, -(2.0*1.27)*mm));
   	G4UnionSolid* setof4 = new G4UnionSolid("setof4", setof2, setof2,
    										0, G4ThreeVector(0*mm, 0*mm, -(4.0*1.27)*mm));
   	G4UnionSolid* setof8 = new G4UnionSolid("setof8", setof4, setof4,
    										0, G4ThreeVector(0*mm, 0*mm, -(8.0*1.27)*mm));
   	G4UnionSolid* setof12 = new G4UnionSolid("setof12", setof8, setof4,
    										0, G4ThreeVector(0*mm, 0*mm, -(16.0*1.27)*mm));
	G4SubtractionSolid *withGrooves = new G4SubtractionSolid("withGrooves", bulk, setof12,
											0, G4ThreeVector(0*mm, 0*mm, (15.875-0.635-1.27)*mm));

    G4VisAttributes* AlVisAtt = new G4VisAttributes(G4Colour(0.75, 0.75, 0.75)); // light gray
    AlVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(withGrooves, material, logicalName);
    pVol->SetVisAttributes(AlVisAtt); 
    MGLog(debugging) << "Created STC CMS Grooves Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC CMS Grooves Logical" << endlog; 
  return pVol;
}  
