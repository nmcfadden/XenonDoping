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
// CLASS IMPLEMENTATION:  MJDemoStudDoubleThreadedHCTakeupStacking.cc
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
 * AUTHOR: Max Hays
 * CONTACT: maxhays8@gmail.com
 * FIRST SUBMISSION: June 6, 2014
 * 
 * REVISION:
 * 
 * 06-06-2010, Created, M. Hays
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoStudDoubleThreadedHCTakeupStacking.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoStudDoubleThreadedHCTakeupStacking::MJDemoStudDoubleThreadedHCTakeupStacking(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "drawing number", "Copper-EF")
{;}

MJDemoStudDoubleThreadedHCTakeupStacking::MJDemoStudDoubleThreadedHCTakeupStacking(const MJDemoStudDoubleThreadedHCTakeupStacking & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoStudDoubleThreadedHCTakeupStacking::~MJDemoStudDoubleThreadedHCTakeupStacking()
{;}

G4LogicalVolume* MJDemoStudDoubleThreadedHCTakeupStacking::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* cylinder1 = new G4Tubs("Cylinder 1", 0.0*mm, 4.826/2*mm, 39.878/2*mm, 0, 2*M_PI);
    G4Tubs* cylinder2 = new G4Tubs("Cylinder 2", 0.0*mm, 6.35/2*mm, 27.178/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4UnionSolid* unionSolid;

    unionSolid = new G4UnionSolid("Union Solid", cylinder1, cylinder2, rotN, G4ThreeVector(0, 0, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(unionSolid, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Stud Double Threaded HC Take-up Stacking Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Stud Double Threaded HC Take-up Stacking Logical" << endlog; 
  return pVol;
}  
