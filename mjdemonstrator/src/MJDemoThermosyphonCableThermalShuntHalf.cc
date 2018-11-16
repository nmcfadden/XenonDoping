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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonCableThermalShuntHalf.cc
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
 * FIRST SUBMISSION: June 27, 2014
 * 
 * REVISION:
 * 
 * 27-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntHalf.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonCableThermalShuntHalf::MJDemoThermosyphonCableThermalShuntHalf(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-146-R002", "Copper-EF")
{;}

MJDemoThermosyphonCableThermalShuntHalf::MJDemoThermosyphonCableThermalShuntHalf(const MJDemoThermosyphonCableThermalShuntHalf & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonCableThermalShuntHalf::~MJDemoThermosyphonCableThermalShuntHalf()
{;}

G4LogicalVolume* MJDemoThermosyphonCableThermalShuntHalf::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* cyl1 = new G4Tubs("Cyl 1", 37.287/2*mm, 44.45/2*mm, 6.604/2*mm, 0, M_PI);
    G4Tubs* cyl2 = new G4Tubs("Cyl 2", 33.274/2*mm, 38/2*mm, 6.604/2*mm, 0, 26.893*deg);
    G4Tubs* cyl3 = new G4Tubs("Cyl 3", 17.997*mm, 39/2*mm, 6.604/2*mm, 15*deg, (31.02-15)*deg);
    G4Tubs* cyl4 = new G4Tubs("Cyl 4", 40/2*mm, 23.588*mm, 6.604/2*mm, 15*deg, (24.1-15)*deg);
    G4Tubs* cyl5 = new G4Tubs("Cyl 5", 41/2*mm, 49.784/2*mm, 6.604/2*mm, 0, 17.918*deg);
    G4Tubs* cyl6 = new G4Tubs("Cyl 6", 33.274/2*mm, 38/2*mm, 6.604/2*mm, (180-18.893)*deg, 18.893*deg);
    G4Tubs* cyl7 = new G4Tubs("Cyl 7", 17.997*mm, 39/2*mm, 6.604/2*mm, (180-23.02)*deg, (23.02-5)*deg);
    G4Tubs* cyl8 = new G4Tubs("Cyl 8", 40/2*mm, 49.784/2*mm, 6.604/2*mm, (180-17.918)*deg, 17.918*deg);
    G4Tubs* cyl9 = new G4Tubs("Cyl 9", 40/2*mm, 23.461*mm, 6.604/2*mm, (180-24.01)*deg, (24.01-5)*deg);
    G4Tubs* cyl10 = new G4Tubs("Cyl 10", 17.26*mm, 18*mm, 6.604/2*mm, (180-21)*deg, (21-5)*deg);
    G4Tubs* cyl11 = new G4Tubs("Cyl 11", 17.26*mm, 18*mm, 6.604/2*mm, 0, 29*deg);
    G4Tubs* cyl12 = new G4Tubs("Cyl 12", 0, 34.442/2*mm, 2.794/2*mm, 0, 90*deg);

    G4Box* prism1 = new G4Box("Prsm 1", 2/2*mm, 6.5/2*mm, 2.794/2*mm);

    G4Tubs* sideCyl1 = new G4Tubs("Side Cyl 1", 0, 2.54/2*mm, 6.604/2*mm, 0, 2*M_PI);
    G4Tubs* sideCyl2 = new G4Tubs("Side Cyl 2", 0, 1.27/2*mm, 6.604/2*mm, 0, 2*M_PI);
    G4Tubs* sideCyl3 = new G4Tubs("Side Cyl 3", 0, 3.65/2*mm, 20/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rotY = new G4RotationMatrix; //7.336, 2.794
    G4RotationMatrix* rot = new G4RotationMatrix;
    rotY->rotateY(90.*deg);
    rot->rotateZ(-45.*deg);
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;

    partUnion = new G4UnionSolid("Part Union", cyl1, cyl2, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl3, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl4, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl5, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl10, rotN, G4ThreeVector(0, 0, 0));

    partUnion = new G4UnionSolid("Part Union", partUnion, cyl6, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl7, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl8, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl9, rotN, G4ThreeVector(0, 0, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, cyl11, rotN, G4ThreeVector(0, 0, 0));

    partUnion = new G4UnionSolid("Part Union", partUnion, sideCyl1, rotN, G4ThreeVector(22.476*mm, 7.267*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, sideCyl2, rotN, G4ThreeVector(15.404*mm, 7.813*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, sideCyl2, rotN, G4ThreeVector(-16.341*mm, 5.593*mm, 0));
    partUnion = new G4UnionSolid("Part Union", partUnion, sideCyl1, rotN, G4ThreeVector(-22.476*mm, 7.267*mm, 0));

    partSub = new G4SubtractionSolid("Part Sub", partUnion, sideCyl1, rotN, G4ThreeVector(-21.447*mm, 9.593*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, sideCyl1, rotN, G4ThreeVector(21.447*mm, 9.593*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, sideCyl2, rotN, G4ThreeVector(15.433*mm, 9.281*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, sideCyl2, rotN, G4ThreeVector(-16.574*mm, 7.043*mm, 0));

    partSub = new G4SubtractionSolid("Part Sub", partSub, sideCyl3, rotN, G4ThreeVector(-20.542*mm, 4.742*mm, 0));
    partSub = new G4SubtractionSolid("Part Sub", partSub, sideCyl3, rotN, G4ThreeVector(20.542*mm, 4.742*mm, 0));

    partSub = new G4SubtractionSolid("Part Sub", partSub, prism1, rotN, G4ThreeVector(-(16.523-2/2)*mm, 6.5/2*mm, -(6.604-2.794)/2*mm));

    partSub = new G4SubtractionSolid("Part Sub", partSub, cyl12, rotN, G4ThreeVector(0, 0, -(6.604-2.794)/2*mm));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // copper
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Thermosyphon Cable Thermal Shunt Half Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Cable Thermal Shunt Half Logical" << endlog; 
  return pVol;
}  
