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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonSupportThickHalfFemale.cc
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
 * FIRST SUBMISSION: June 23, 2014
 * 
 * REVISION:
 * 
 * 23-06-2010, Created, M. Hays
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
#include "mjdemonstrator/MJDemoThermosyphonSupportThickHalfFemale.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonSupportThickHalfFemale::MJDemoThermosyphonSupportThickHalfFemale(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-139-R002", "Vespel")
{;}

MJDemoThermosyphonSupportThickHalfFemale::MJDemoThermosyphonSupportThickHalfFemale(const MJDemoThermosyphonSupportThickHalfFemale & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonSupportThickHalfFemale::~MJDemoThermosyphonSupportThickHalfFemale()
{;}

G4LogicalVolume* MJDemoThermosyphonSupportThickHalfFemale::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 33.274/2*mm, 70.18/2*mm, 6.35/2*mm, 0, M_PI);
    G4Tubs* hole1 = new G4Tubs("Hole 1", 0, 12.954/2*mm, 10/2*mm, 0, 2*M_PI);
    G4Tubs* hole2 = new G4Tubs("Hole 2", 55.982/2*mm, 63.906/2*mm, 10/2*mm, 120*deg, 40*deg);
    G4Tubs* hole3 = new G4Tubs("Hole 3", 55.976/2*mm, 63.912/2*mm, 10/2*mm, 43*deg, 32*deg);
    G4Tubs* hole4 = new G4Tubs("Hole 4", 37.44/2*mm, 45.364/2*mm, 10/2*mm, 46*deg, 32*deg);
    G4Tubs* hole5 = new G4Tubs("Hole 5", 37.44/2*mm, 45.364/2*mm, 10/2*mm, 123*deg, 46*deg);
    G4Tubs* holeEnd = new G4Tubs("Cylindrical Hole Ends", 0, 3.692/2*mm, 10/2*mm, 0, 2*M_PI);
    G4Tubs* holeBand1 = new G4Tubs("Hole Band 1", 35.306/2*mm, 38.1/2*mm, 10/2*mm, -29.1*deg, 60*deg);
    G4Tubs* holeBand2 = new G4Tubs("Hole Band 2", 35.306/2*mm, 38.1/2*mm, 10/2*mm, -36.1*deg, 60*deg);
    G4Tubs* screwHole = new G4Tubs("Screw Hole", 0, 3.65/2*mm, 10/2*mm, 0, 2*M_PI);
    G4Box* piecePrism1 = new G4Box("Piece Prism 1", 9.113/2*mm, 6.05/2*mm, 6.35/2*mm);
    G4Box* piecePrism2 = new G4Box("Piece Prism 2", 3.175/4*mm, (6.05-3.175/2)/2*mm+epsilon, 6.35/2*mm+epsilon);
    G4Tubs* pieceCyl = new G4Tubs("Piece Cyl", 0, 3.175/2*mm, 6.35/2*mm+epsilon, 0, 2*M_PI);
    G4Box* tab = new G4Box("tab", 11.303/2*mm, 3.175/2*mm, (2.794+3.175)/2*mm);
    G4Tubs* tubeclearance = new G4Tubs("tubeclearance", 0, 34.1/2*mm, 20*mm, 0, 2*M_PI);
      

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rot = new G4RotationMatrix;
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;

    G4SubtractionSolid* pieceSub = new G4SubtractionSolid("Piece Sub", piecePrism1, piecePrism2, rotN, G4ThreeVector((9.113-3.175/2)/2*mm, 6.05/2*mm-(6.05-3.175/2)/2*mm+epsilon, 0));
    pieceSub = new G4SubtractionSolid("Piece Sub", pieceSub, piecePrism2, rotN, G4ThreeVector(-(9.113-3.175/2)/2*mm, 6.05/2*mm-(6.05-3.175/2)/2*mm+epsilon, 0));
    pieceSub = new G4SubtractionSolid("Piece Sub", pieceSub, pieceCyl, rotN, G4ThreeVector((9.113)/2*mm, -6.05/2*mm+3.175/2, 0));
    pieceSub = new G4SubtractionSolid("Piece Sub", pieceSub, pieceCyl, rotN, G4ThreeVector(-(9.113)/2*mm, -6.05/2*mm+3.175/2, 0));

    partSub = new G4SubtractionSolid("Part", mainCyl, hole1, rotN, G4ThreeVector(-4.053*mm, 25.589*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, hole1, rotN, G4ThreeVector(23.481*mm, 10.949*mm, 0));    
    partSub = new G4SubtractionSolid("Part", partSub, hole2, rotN, G4ThreeVector(0, 0, 0));
    partSub = new G4SubtractionSolid("Part", partSub, hole3, rotN, G4ThreeVector(0, 0, 0));
    partSub = new G4SubtractionSolid("Part", partSub, hole4, rotN, G4ThreeVector(0, 0, 0));
    partSub = new G4SubtractionSolid("Part", partSub, hole5, rotN, G4ThreeVector(0, 0, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(14.38*mm, 14.891*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(21.92*mm, 20.441*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(4.304*mm, 20.249*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(7.757*mm, 28.951*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(-11.275*mm, 17.361*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(-14.986*mm, 25.957*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(-20.321*mm, 3.95*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeEnd, rotN, G4ThreeVector(-28.164*mm, 10.251*mm, 0));    
    partSub = new G4SubtractionSolid("Part", partSub, holeBand1, rotN, G4ThreeVector(-15.039*mm, 31.704*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, holeBand2, rotN, G4ThreeVector(13.322*mm, 18.358*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole, rotN, G4ThreeVector(-31.61*mm, 5.007*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole, rotN, G4ThreeVector(32.004*mm, 5.007*mm, 0));
      
    rot->rotateZ(90*deg);
    rot->rotateZ(-61*deg);
    partUnion = new G4UnionSolid("Part", partSub, pieceSub, rot, G4ThreeVector(((70.18/2+6.05/2)*mm-3*epsilon)*cos(61*deg), ((70.18/2+6.05/2)*mm-3*epsilon)*sin(61*deg), 0));
    rot->rotateZ(-78*deg);
    partUnion = new G4UnionSolid("Part", partUnion, pieceSub, rot, G4ThreeVector(((70.18/2+6.05/2)*mm-3*epsilon)*cos(139*deg), ((70.18/2+6.05/2)*mm-3*epsilon)*sin(139*deg), 0));
      
    partUnion = new G4UnionSolid("Part", partUnion, tab, rotN, G4ThreeVector((16.637+11.303/2)*mm, 3.175/2*mm, (2.794+3.175)/2*mm));
    
    partSub = new G4SubtractionSolid("Part", partUnion, tubeclearance, rotN, G4ThreeVector(0,0,0));
      
    G4VisAttributes* brownVisAtt = new G4VisAttributes(G4Colour(0.63, 0.32, 0.18)); // brown
    brownVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(brownVisAtt);
    MGLog(debugging) << "Created Thermosyphon Support Thick Female Half With Slotted Cable Holes Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Support Thick Female Half With Slotted Cable Holes Logical" << endlog; 
  return pVol;
}  
