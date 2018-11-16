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
// CLASS IMPLEMENTATION:  MJDemo8CFCube.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM of box
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Sept 28, 2015
 *
 * REVISION:
 *
 * 09-28-2015, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemo8CFCube.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo8CFCube::MJDemo8CFCube(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "8CFCubeDwg", "SS303")
{;}

MJDemo8CFCube::MJDemo8CFCube(const MJDemo8CFCube & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemo8CFCube::~MJDemo8CFCube()
{;}

G4LogicalVolume* MJDemo8CFCube::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* tube = new G4Tubs("tube", 0, 3.0*25.4*mm, 5.0*25.4*mm, 0, 2*pi);
    G4Box* outerbox = new G4Box("outerbox", 4.0*25.4*mm, 4.0*25.4*mm, 4.0*25.4*mm);
    G4Box* innerbox = new G4Box("innerbox", 3.0*25.4*mm, 3.0*25.4*mm, 3.0*25.4*mm);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    G4SubtractionSolid* box = new G4SubtractionSolid("box", outerbox, innerbox, rotation, G4ThreeVector(0,0,0));
    G4SubtractionSolid* box2 = new G4SubtractionSolid("box2", box, tube, rotation, G4ThreeVector(0,0,0));
    rotation->rotateY(pi/2);
    G4SubtractionSolid* box3 = new G4SubtractionSolid("box3", box2, tube, rotation, G4ThreeVector(0,0,0));
    rotation->rotateX(pi/2);
    G4SubtractionSolid* cube = new G4SubtractionSolid("cube", box3, tube, rotation, G4ThreeVector(0,0,0));

      
    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0, 0, 0.8)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(cube, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(debugging) << "Created 8CF Cube Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 8CF Cube Logical" << endlog;
  return pVol;
}  
