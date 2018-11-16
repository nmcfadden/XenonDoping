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
// CLASS IMPLEMENTATION:  MJDemoCableGuideSnapClipThermosyphonShort.cc
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
 * FIRST SUBMISSION: June 17, 2014
 * 
 * REVISION:
 * 
 * 17-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoCableGuideSnapClipThermosyphonShort.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableGuideSnapClipThermosyphonShort::MJDemoCableGuideSnapClipThermosyphonShort(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-168-R000", "Vespel")
{;}

MJDemoCableGuideSnapClipThermosyphonShort::MJDemoCableGuideSnapClipThermosyphonShort(const MJDemoCableGuideSnapClipThermosyphonShort & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCableGuideSnapClipThermosyphonShort::~MJDemoCableGuideSnapClipThermosyphonShort()
{;}

G4LogicalVolume* MJDemoCableGuideSnapClipThermosyphonShort::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 66.294/2*mm, 73.406/2*mm, 1.27/2*mm, (90-64/2)*deg, 64*deg);
    G4Tubs* mainCyl2a = new G4Tubs("Main Cyl 2a", 70.358/2*mm, 73.406/2*mm, 6.655/2*mm, (90-64/2)*deg, 64*deg);
    G4Box* addPrism1 = new G4Box("Added Prism 1", 9.457/2*mm, 6/2*mm, 1.27/2*mm);
    G4Tubs* addCyl1 = new G4Tubs("Add Cyl", 0, 6.604/2*mm, 1.27/2*mm, 0, 2*M_PI);
    G4Box* pieceHub = new G4Box("Piece Hub", 0.0001*mm, 0.0001*mm, 0.0001*mm);
    G4Box* piecePrism = new G4Box("Piece Prism", 4/2*mm, 6/2*mm, 1.27/2*mm);
    G4Box* piecePrism2 = new G4Box("Piece Prism 2", 2.9/4*mm, 4/2*mm, 1.27/2*mm);
    G4Tubs* subCyl = new G4Tubs("Sub Cyl", 0, 3.15/2*mm, 1.27/2*mm+epsilon, 0, 2*M_PI);
    G4Tubs* subCyl2 = new G4Tubs("Sub Cyl 2", 0, 4.915/2*mm, 6*mm, 0, 2*M_PI);

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rot = new G4RotationMatrix;
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;

    G4UnionSolid* pieceUnion1 = new G4UnionSolid("Piece Union 1", pieceHub, piecePrism, rotN, G4ThreeVector(-4/2, 6/2*mm, 0));
    pieceUnion1 = new G4UnionSolid("Piece Union 1", pieceUnion1, piecePrism2, rotN, G4ThreeVector(2.9/4, (3.327+2)*mm, 0));
    G4UnionSolid* pieceUnion2 = new G4UnionSolid("Piece Union 2", pieceHub, piecePrism, rotN, G4ThreeVector(4/2, 6/2*mm, 0));
    pieceUnion2 = new G4UnionSolid("Piece Union 2", pieceUnion2, piecePrism2, rotN, G4ThreeVector(-2.9/4, (3.327+2)*mm, 0));


    partUnion = new G4UnionSolid("Part Union", mainCyl, mainCyl2a, rotN, G4ThreeVector(0, 0, (1.27+6.655)/2*mm));

    partUnion = new G4UnionSolid("Part Union", partUnion, addPrism1, rotN, G4ThreeVector(0, (27.559+3)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl1, rotN, G4ThreeVector(4.728*mm, 30.861*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl1, rotN, G4ThreeVector(-4.728*mm, 30.861*mm, 0));
    rot->rotateZ(45*deg);
    partUnion = new G4UnionSolid("Part Union", partUnion, pieceUnion1, rot, G4ThreeVector(7.063*mm, 28.526*mm, 0));
    rot->rotateZ(-90*deg);
    partUnion = new G4UnionSolid("Part Union", partUnion, pieceUnion2, rot, G4ThreeVector(-7.063*mm, 28.526*mm, 0));
    partSub = new G4SubtractionSolid("Part Union", partUnion, subCyl, rotN, G4ThreeVector(10.529*mm, 29.765*mm, 0));
    partSub = new G4SubtractionSolid("Part Union", partSub, subCyl, rotN, G4ThreeVector(-10.529*mm, 29.765*mm, 0));

    partSub = new G4SubtractionSolid("Part Union", partSub, subCyl2, rotN, G4ThreeVector(5.007*mm, 31.61*mm, 0));
    partSub = new G4SubtractionSolid("Part Union", partSub, subCyl2, rotN, G4ThreeVector(-5.007*mm, 31.61*mm, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.63, 0.32, 0.18)); // brown
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Cable Guide Snap Clip Thermosyphon Short Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cable Guide Snap Clip Thermosyphon Short Logical" << endlog; 
  return pVol;
}  
