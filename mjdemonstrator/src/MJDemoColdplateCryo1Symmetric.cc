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
// CLASS IMPLEMENTATION:  MJDemoColdplateCryo1Symmetric.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM of cold plate
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
#include "mjdemonstrator/MJDemoColdplateCryo1Symmetric.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoColdplateCryo1Symmetric::MJDemoColdplateCryo1Symmetric(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-169", "Copper-EF")
{;}

MJDemoColdplateCryo1Symmetric::MJDemoColdplateCryo1Symmetric(const MJDemoColdplateCryo1Symmetric & rhs) :
  MJVDemoPart(rhs)
{;}

MJDemoColdplateCryo1Symmetric::~MJDemoColdplateCryo1Symmetric()
{;}

G4LogicalVolume* MJDemoColdplateCryo1Symmetric::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.1;

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 0, 317.5/2*mm, 10.16/2*mm, 0, 2*M_PI);
    G4Tubs* screwHole1a = new G4Tubs("Screw Hole 1 Part a", 0, 9.906/2*mm, 10/2*mm, 0, 2*M_PI);
    G4Tubs* screwHole1b = new G4Tubs("Screw Hole 1 Part b", 0, 5.105/2*mm, 20/2*mm, 0, 2*M_PI);
    G4Tubs* screwHole2 = new G4Tubs("Screw Hole 2", 0, 6.909/2*mm, 20/2*mm, 0, 2*M_PI);
    G4Tubs* screwHole3 = new G4Tubs("Screw Hole 3", 0, 5.5/2*mm, 20/2*mm, 0, 2*M_PI);//4.496
    G4Tubs* screwHole4 = new G4Tubs("Screw Hole 4", 0, 3.302/2*mm, 3.175/2*mm+epsilon, 0, 2*M_PI);
    G4Box* holePrism = new G4Box("Hole Prism", 28.448/2*mm, 13.106/2*mm, 10.16/2*mm+epsilon);
    G4Tubs* holeCyl = new G4Tubs("Hole Cyl", 0, 13.106/2*mm, 10.16/2*mm+epsilon, 0, 2*M_PI);
    G4Box* piecePrism1 = new G4Box("Piece Prism 1", 15.718/2*mm, 5.457/2*mm, 10.16/2*mm);
    G4Box* piecePrism2 = new G4Box("Piece Prism 2", 1.509/2*mm, 3.871/2*mm, 10.16/2*mm);
    G4Tubs* pieceCyl = new G4Tubs("Piece Cyl", 0, 3.175/2*mm, 10.16/2*mm+epsilon, 0, 2*M_PI);
    G4Box* pieceHub = new G4Box("Piece Hub", 0.1*mm, 0.1*mm, 0.1*mm);
    G4Box* subPrism = new G4Box("Sub Prism", 56.568/2*mm, 20*mm, 20*mm);

    G4RotationMatrix* rotN = new G4RotationMatrix;
    G4RotationMatrix* rot = new G4RotationMatrix;
    G4UnionSolid* partUnion;
    G4SubtractionSolid* partSub;
    G4UnionSolid* screwHole1 = new G4UnionSolid("Screw Hole 1", screwHole1a, screwHole1b, rotN, G4ThreeVector(0, 0, 0));
    G4UnionSolid* hole = new G4UnionSolid("Hole", holeCyl, holePrism, rotN, G4ThreeVector(-28.488/2*mm, 0, 0));
    hole = new G4UnionSolid("Hole", hole, holeCyl, rotN, G4ThreeVector(-28.488*mm, 0, 0));
    G4SubtractionSolid* pieceSub = new G4SubtractionSolid("Piece", piecePrism1, piecePrism2, rotN, G4ThreeVector((15.718-1.509)/2*mm, (5.457-3.871)/2*mm));
    pieceSub = new G4SubtractionSolid("Piece", pieceSub, piecePrism2, rotN, G4ThreeVector(-(15.718-1.509)/2*mm, (5.457-3.871)/2*mm));
    pieceSub = new G4SubtractionSolid("Piece", pieceSub, pieceCyl, rotN, G4ThreeVector(-7.938*mm, -(5.457-3.175)/2*mm, 0));
    pieceSub = new G4SubtractionSolid("Piece", pieceSub, pieceCyl, rotN, G4ThreeVector(7.938*mm, -(5.457-3.175)/2*mm, 0));
    G4UnionSolid* pieceUnion = new G4UnionSolid("Piece", pieceHub, pieceSub, rotN, G4ThreeVector(0, (5.457+317.5)/2*mm, 0));
    pieceSub = new G4SubtractionSolid("Piece", pieceUnion, screwHole3, rotN, G4ThreeVector(0, 6.320*25.4*mm, 0));

    partSub = new G4SubtractionSolid("Part", mainCyl, screwHole1, rotN, G4ThreeVector(0, 19.05*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(16.498*mm, -9.525*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-16.498*mm, -9.525*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(34.873*mm, -67.672*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-34.873*mm, -67.672*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(41.169*mm, -100.061*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-41.169*mm, -100.061*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(66.071*mm, -78.414*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-66.071*mm, -78.414*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(0*mm, 113.03*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(44.063*mm, 63.288*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-44.063*mm, 63.288*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(32.778*mm, 94.294*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-32.778*mm, 94.294*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(65.272*mm, 88.564*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-65.272*mm, 88.564*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(104.648*mm, 50.546*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-104.648*mm, 50.546*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(80.149*mm, 12.245*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-80.149*mm, 12.245*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(112.643*mm, 6.515*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-112.643*mm, 6.515*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(91.434*mm, -18.716*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole1, rotN, G4ThreeVector(-91.434*mm, -18.716*mm, (10.16-2.794)/2*mm));

    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(25.4*mm, -41.783*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(-25.4*mm, -41.783*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(25.4*mm, -111.633*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(-25.4*mm, -111.633*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(25.4*mm, -137.033*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole2, rotN, G4ThreeVector(-25.4*mm, -137.033*mm, (10.16-2.794)/2*mm));

    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(51.737*mm, 139.563*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(-51.737*mm, 139.563*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(129.342*mm, 73.656*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(-129.342*mm, 73.656*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(146.427*mm, -26.716*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(-146.427*mm, -26.716*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(94.997*mm, -114.587*mm, (10.16-2.794)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole3, rotN, G4ThreeVector(-94.997*mm, -114.587*mm, (10.16-2.794)/2*mm));

    partSub = new G4SubtractionSolid("Part", partSub, hole, rotN, G4ThreeVector(14.224*mm, 46.711*mm, 0));
    rot->rotateZ(67.*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(97.682*mm, 85.244*mm, 0));
    rot->rotateZ(-67.*deg);
    rot->rotateZ(-67.*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(-86.566*mm, 111.431*mm, 0));
    rot->rotateZ(67.*deg);
    rot->rotateZ(78.34*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(141.632*mm, 2.272*mm, 0));
    rot->rotateZ(-78.34*deg);
    rot->rotateZ(-78.34*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(-135.882*mm, 30.133*mm, 0));
    rot->rotateZ(78.34*deg);
    rot->rotateZ(80*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(96.888*mm, -87.706*mm, 0));
    rot->rotateZ(-80*deg);
    rot->rotateZ(-80*deg);
    partSub = new G4SubtractionSolid("Part", partSub, hole, rot, G4ThreeVector(-91.948*mm, -59.69*mm, 0));
    rot->rotateZ(80*deg);

    partUnion = new G4UnionSolid("Part", partSub, pieceSub, rotN, G4ThreeVector(0, 0, 0));
    rot->rotateZ(120*deg);
    partUnion = new G4UnionSolid("Part", partUnion, pieceSub, rot, G4ThreeVector(0, 0, 0));
    rot->rotateZ(120*deg);
    partUnion = new G4UnionSolid("Part", partUnion, pieceSub, rot, G4ThreeVector(0, 0, 0));

    partSub = new G4SubtractionSolid("Part", partUnion, subPrism, rotN, G4ThreeVector(0, -(156.21+20)*mm, 0));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole4, rotN, G4ThreeVector(139.025*mm, -80.266*mm, (10.16-3.175)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole4, rotN, G4ThreeVector(-139.025*mm, -80.266*mm, (10.16-3.175)/2*mm));
    partSub = new G4SubtractionSolid("Part", partSub, screwHole4, rotN, G4ThreeVector(0, 160.352*mm, (10.16-3.175)/2*mm));

    G4Tubs* pinHole = new G4Tubs("pinHole", 0, 0.07*25.4*mm, 0.25*25.4*mm, 0, 2*pi);

    G4RotationMatrix* pinRotation1 = new G4RotationMatrix();
    pinRotation1->rotateZ(-80*deg);
    pinRotation1->rotateY(pi/2);
    partSub = new G4SubtractionSolid("Part", partSub, pinHole, pinRotation1,
                      G4ThreeVector(6.25*25.4*mm*cos(80*deg), 6.25*25.4*mm*sin(80*deg), 0));

    G4RotationMatrix* pinRotation2 = new G4RotationMatrix();
    pinRotation2->rotateZ(70*deg);
    pinRotation2->rotateY(pi/2);
    partSub = new G4SubtractionSolid("Part", partSub, pinHole, pinRotation2,
                      G4ThreeVector(6.25*25.4*mm*cos(-70*deg), 6.25*25.4*mm*sin(-70*deg), 0));

    G4RotationMatrix* pinRotation3 = new G4RotationMatrix();
    pinRotation3->rotateZ(110*deg);
    pinRotation3->rotateY(pi/2);
    partSub = new G4SubtractionSolid("Part", partSub, pinHole, pinRotation3,
                      G4ThreeVector(6.25*25.4*mm*cos(-110*deg), 6.25*25.4*mm*sin(-110*deg), 0));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0));
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Coldplate Cryo 1 Symmetric Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Coldplate Cryo 1 Symmetric Logical" << endlog;
  return pVol;
}
