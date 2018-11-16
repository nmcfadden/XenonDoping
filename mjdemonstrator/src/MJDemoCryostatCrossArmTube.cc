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
// CLASS IMPLEMENTATION:  MJDemoCryostatCrossArmTube.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  
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
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatCrossArmTube.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatCrossArmTube::MJDemoCryostatCrossArmTube(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatCrossArmTubeDwg", "Copper-EF")
{;}

MJDemoCryostatCrossArmTube::MJDemoCryostatCrossArmTube(const MJDemoCryostatCrossArmTube & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCryostatCrossArmTube::~MJDemoCryostatCrossArmTube()
{;}

G4LogicalVolume* MJDemoCryostatCrossArmTube::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0.05*25.4*mm, 0.075*25.4*mm, 0.075*25.4*mm, 26.68*25.4*mm, 26.68*25.4*mm,
    							27.13*25.4*mm};
    G4double rConeInner[] = {1.625*25.4*mm, 1.625*25.4*mm, 1.625*25.4*mm, 1.625*25.4*mm, 
    							1.625*25.4*mm, 1.625*25.4*mm};
    G4double rConeOuter[] = {1.75*25.4*mm, 1.75*25.4*mm, 1.749*25.4*mm, 1.749*25.4*mm, //1.75
    							3.0*25.4*mm, 3.0*25.4*mm};
    G4Polycone* tube = new G4Polycone("Tube", 0, 2*pi, 6, zConePlanes, rConeInner, rConeOuter);


    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(tube, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Cryostat Cross Arm Tube Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Cross Arm Tube Logical" << endlog; 
  return pVol;
}  
