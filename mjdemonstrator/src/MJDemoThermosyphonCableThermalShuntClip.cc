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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonCableThermalShuntClip.cc
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
 * FIRST SUBMISSION: June 30, 2014
 * 
 * REVISION:
 * 
 * 30-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntClip.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonCableThermalShuntClip::MJDemoThermosyphonCableThermalShuntClip(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-145-R000", "Copper-EF")
{;}

MJDemoThermosyphonCableThermalShuntClip::MJDemoThermosyphonCableThermalShuntClip(const MJDemoThermosyphonCableThermalShuntClip & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonCableThermalShuntClip::~MJDemoThermosyphonCableThermalShuntClip()
{;}

G4LogicalVolume* MJDemoThermosyphonCableThermalShuntClip::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 49.784/2*mm, 52.832/2*mm, 5.08/2*mm, (90-45.27/2)*deg, 45.27*deg);
    G4Tubs* mainCyl2 = new G4Tubs("Main Cyl 2", 24.122*mm, 52.832/2*mm, 1.27/2*mm, (90-23.72)*deg, 23.72*2*deg);
    G4Box* addPrism1 = new G4Box("Added Prism 1", 9.457/2*mm, 7.5/2*mm, 1.27/2*mm);
    G4Box* addPrism2 = new G4Box("Added Prism 2", 2.507/2*mm, 2.507/2*mm, 1.27/2*mm);
    G4Box* hub = new G4Box("Hub", 0.001*mm, 0.001*mm, 0.001*mm);
    G4Box* subPrism1 = new G4Box("Sub Prism 1", 100/2*mm, 100/2*mm, 100/2*mm);
    G4Tubs* addCyl1 = new G4Tubs("Add Cyl 1", 0, 6.604/2*mm, 1.27/2*mm, 0, 2*M_PI);
    G4Tubs* addCyl2 = new G4Tubs("Add Cyl 2", 0, 5.08/2*mm, 1.27/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl1 = new G4Tubs("Sub Cyl 1", 0, 4.496/2*mm, 2/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rot = new G4RotationMatrix;
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;
    G4UnionSolid* pieceUnion1;

    pieceUnion1 = new G4UnionSolid("Piece Union 1", hub, addPrism2, rotN, G4ThreeVector(-2.507/2*mm, 2.507/2*mm, 0));

    partUnion = new G4UnionSolid("Part Union", mainCyl2, mainCyl, rotN, G4ThreeVector(0, 0, -(1.27-5.08)/2*mm));
    partUnion = new G4UnionSolid("Part Union", partUnion, addPrism1, rotN, G4ThreeVector(0, (17.272+7.5/2)*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl1, rotN, G4ThreeVector(4.728*mm, 20.574*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl1, rotN, G4ThreeVector(-4.728*mm, 20.574*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl2, rotN, G4ThreeVector(-7.04*mm, 21.808*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, addCyl2, rotN, G4ThreeVector(7.04*mm, 21.808*mm, 0));
    rot->rotateZ(45*deg);
    partUnion = new G4UnionSolid("Part Union", partUnion, pieceUnion1, rot, G4ThreeVector(7.063*mm, 18.239*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, pieceUnion1, rot, G4ThreeVector(-7.063*mm, 18.239*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partUnion, subPrism1, rotN, G4ThreeVector((9.58+100/2)*mm, 0, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subPrism1, rotN, G4ThreeVector(-(9.58+100/2)*mm, 0, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl1, rotN, G4ThreeVector(-4.742*mm, 20.542*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, subCyl1, rotN, G4ThreeVector(4.742*mm, 20.542*mm, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Thermosyphon Cable Thermal Shunt Clip Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Cable Thermal Shunt Clip Logical" << endlog; 
  return pVol;
}
