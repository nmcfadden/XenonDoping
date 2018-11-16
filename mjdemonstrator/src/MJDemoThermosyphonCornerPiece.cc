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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonCornerPiece.cc
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
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: June 31, 2014
 * 
 * REVISION:
 * 
 * 31-07-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonCornerPiece.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonCornerPiece::MJDemoThermosyphonCornerPiece(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-331-R002", "Copper-EF")
{;}

MJDemoThermosyphonCornerPiece::MJDemoThermosyphonCornerPiece(const MJDemoThermosyphonCornerPiece & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonCornerPiece::~MJDemoThermosyphonCornerPiece()
{;}

G4LogicalVolume* MJDemoThermosyphonCornerPiece::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    
    G4Box* body = new G4Box("body", 0.75*25.4*mm, 0.75*25.4*mm, 0.75*25.4*mm);
    G4Tubs* sidetube = new G4Tubs("sidetube", 0, 1.341/2*25.4*mm, 1.36*25.4*mm, 0, 2*pi);
    G4Tubs* toptube = new G4Tubs("toptube", 0, 1.341/2*25.4*mm, 0.13*25.4*mm, 0, 2*pi);
    
    G4RotationMatrix* rotation = new G4RotationMatrix();
    G4UnionSolid* partUnion = new G4UnionSolid("box and tube", body, toptube, rotation, G4ThreeVector(0, 0, 0.75*25.4*mm));

    rotation->rotateY(pi/2);
    partUnion = new G4UnionSolid("box and 2 tubes", partUnion, sidetube, rotation, G4ThreeVector(0.75*25.4*mm, 0, 0));
                                               
    G4Tubs* centerbore1 = new G4Tubs("centerbore1", 0, 1.00/2*25.4*mm, 2.613*25.4*mm, 0, 2*pi);
    G4Tubs* centerbore2 = new G4Tubs("centerbore2", 0, 1.00/2*25.4*mm, 1.375*25.4*mm, 0, 2*pi);
      
    G4SubtractionSolid* partSub = new G4SubtractionSolid("withcbore1", partUnion, centerbore1, rotation, G4ThreeVector(2.113*25.4*mm, 0, 0));
    rotation->rotateY(-pi/2);
    partSub = new G4SubtractionSolid("withcbore1&2", partSub, centerbore2, rotation, G4ThreeVector(0, 0, 0.88*25.4*mm));
      
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Thermosyphon Corner Piece Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Corner Piece Logical" << endlog;
  return pVol;
}  
