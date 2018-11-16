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
// CLASS IMPLEMENTATION:  MJDemoRadShieldN2Volume.cc
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
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoRadShieldN2Volume.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldN2Volume::MJDemoRadShieldN2Volume(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ShieldN2VolumeDwg", "Nitrogen-Gas")
{;}

MJDemoRadShieldN2Volume::MJDemoRadShieldN2Volume(const MJDemoRadShieldN2Volume & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoRadShieldN2Volume::~MJDemoRadShieldN2Volume()
{;}

G4LogicalVolume* MJDemoRadShieldN2Volume::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 16.030*25.4*mm, 7.905*25.4*mm, 9.953*25.4*mm);
    
    G4Tubs* cryostatTube = new G4Tubs("cryostatTube", 0, 1.75*25.4*mm, 10.0*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateY(pi/2);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cryostatTube, rotation1,
    								G4ThreeVector(15.0*25.4*mm, 0, 4.4265*25.4*mm));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateZ(pi/2);
    rotation2->rotateY(pi/2); 
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cryostatTube, rotation2, 
    								G4ThreeVector(-8.1417*25.4*mm, -5*25.4*mm, 4.4265*25.4*mm));
 
 
    G4double zConePlanes[] = {-13.68*25.4*mm, -3.9*25.4*mm, -3.9*25.4*mm, -2.17*25.4*mm,
    							-2.17*25.4*mm, 1.99*25.4*mm, 1.99*25.4*mm, 3.7*25.4*mm,
    							3.7*25.4*mm, 4.76*25.4*mm};
    G4double rConeInner[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    G4double rConeOuter[] = {7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm,
    							7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm,
    							7.39*25.4*mm, 7.39*25.4*mm};
    G4Polycone* cryostat = new G4Polycone("cryostat", 0, 2*pi, 10, zConePlanes, rConeInner, rConeOuter);

    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cryostat, 0,
    								G4ThreeVector(8.1417*25.4*mm, 0, 4.4265*25.4*mm));

    G4SubtractionSolid* nitrogen = new G4SubtractionSolid("nitrogen", body4, cryostat, 0,
    								G4ThreeVector(-8.1417*25.4*mm, 0, 4.4265*25.4*mm));
    
    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1, 1, 1)); // white
    whiteVisAtt->SetForceWireframe(true);
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(nitrogen, material, logicalName);
    pVol->SetVisAttributes(whiteVisAtt); 
    MGLog(debugging) << "Created N2 Volume Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing N2 Volume Logical" << endlog; 
  return pVol;
}  
