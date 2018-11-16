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
// CLASS IMPLEMENTATION:  MJDemoBottomThermalShieldAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Assembly origin:  COM of annulus.
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
 * 09-07-2014: Renamed MJDemoBottomThermalShieldAssembly
 * 04-24-2014, Updated so there aren't overlaps with the updated Coldplate geometry, J. MacMullin
 * 09-10-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoBottomThermalShieldAssembly.hh"
#include "mjdemonstrator/MJDemoThermalShieldCan.hh"
#include "mjdemonstrator/MJDemoThermalShieldAnnulus.hh"
#include "mjdemonstrator/MJDemoThermalShieldSupport.hh"
#include "mjdemonstrator/MJDemoThermalShieldWedge.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoBottomThermalShieldAssembly::MJDemoBottomThermalShieldAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "ThermalShield")
{
  fCanPtr = new MJDemoThermalShieldCan(fName+"_ThermalShieldCan_001", "867-5309");
  fAnnulusPtr = new MJDemoThermalShieldAnnulus(fName+"_ThermalShieldAnnulus_001", "867-5309");
  fSupport1Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_001", "867-5309");
  fSupport2Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_002", "867-5309");
  fSupport3Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_003", "867-5309");
  fCenterPin1Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_001", "867-5309");
  fCenterPin2Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_002", "867-5309");
  fCenterPin3Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_003", "867-5309");
  fWedge1Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_001", "867-5309");
  fWedge2Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_002", "867-5309");
  fWedge3Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_003", "867-5309");
  fScrew1Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_001", "867-5309", 0.625*25.4);
  fScrew2Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_002", "867-5309", 0.625*25.4);
  fScrew3Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_003", "867-5309", 0.625*25.4);
  fScrew4Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_004", "867-5309", 0.625*25.4);
  fScrew5Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_005", "867-5309", 0.625*25.4);
  fScrew6Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_006", "867-5309", 0.625*25.4);
  fScrew7Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_007", "867-5309", 0.625*25.4);
  fScrew8Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_008", "867-5309", 0.625*25.4);
  fSideScrew1Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 0.25*25.4);
  fSideScrew2Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 0.25*25.4);
  fSideScrew3Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 0.25*25.4);
  fSideScrew4Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_004", "867-5309", 0.25*25.4);
  fSideScrew5Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_005", "867-5309", 0.25*25.4);
  fSideScrew6Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_006", "867-5309", 0.25*25.4);
}

MJDemoBottomThermalShieldAssembly::MJDemoBottomThermalShieldAssembly(const MJDemoBottomThermalShieldAssembly & rhs) :
  MJVDemoAssembly(rhs)
{
  fCanPtr = new MJDemoThermalShieldCan(fName+"_ThermalShieldCan_001", "867-5309");
  fAnnulusPtr = new MJDemoThermalShieldAnnulus(fName+"_ThermalShieldAnnulus_001", "867-5309");
  fSupport1Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_001", "867-5309");
  fSupport2Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_002", "867-5309");
  fSupport3Ptr = new MJDemoThermalShieldSupport(fName+"_ThermalShieldSupport_003", "867-5309");
  fCenterPin1Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_001", "867-5309");
  fCenterPin2Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_002", "867-5309");
  fCenterPin3Ptr = new MJDemoThermalShieldSupport(fName+"_CenteringPin_003", "867-5309");
  fWedge1Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_001", "867-5309");
  fWedge2Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_002", "867-5309");
  fWedge3Ptr = new MJDemoThermalShieldWedge(fName+"_ThermalShieldWedge_003", "867-5309");
  fScrew1Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_001", "867-5309", 0.625*25.4);
  fScrew2Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_002", "867-5309", 0.625*25.4);
  fScrew3Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_003", "867-5309", 0.625*25.4);
  fScrew4Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_004", "867-5309", 0.625*25.4);
  fScrew5Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_005", "867-5309", 0.625*25.4);
  fScrew6Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_006", "867-5309", 0.625*25.4);
  fScrew7Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_007", "867-5309", 0.625*25.4);
  fScrew8Ptr = new MJDemo832Screw(fName+"_8_32x0.625Screw_008", "867-5309", 0.625*25.4);
  fSideScrew1Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 0.25*25.4);
  fSideScrew2Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 0.25*25.4);
  fSideScrew3Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 0.25*25.4);
  fSideScrew4Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_004", "867-5309", 0.25*25.4);
  fSideScrew5Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_005", "867-5309", 0.25*25.4);
  fSideScrew6Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_006", "867-5309", 0.25*25.4);
}

MJDemoBottomThermalShieldAssembly::~MJDemoBottomThermalShieldAssembly()
{
  delete fCanPtr;
  delete fAnnulusPtr;
  delete fSupport1Ptr;
  delete fSupport2Ptr;
  delete fSupport3Ptr;
  delete fCenterPin1Ptr;
  delete fCenterPin2Ptr;
  delete fCenterPin3Ptr;
  delete fWedge1Ptr;
  delete fWedge2Ptr;
  delete fWedge3Ptr;
  delete fScrew1Ptr;
  delete fScrew2Ptr;
  delete fScrew3Ptr;
  delete fScrew4Ptr;
  delete fScrew5Ptr;
  delete fScrew6Ptr;
  delete fScrew7Ptr;
  delete fScrew8Ptr;
  delete fSideScrew1Ptr;
  delete fSideScrew2Ptr;
  delete fSideScrew3Ptr;
  delete fSideScrew4Ptr;
  delete fSideScrew5Ptr;
  delete fSideScrew6Ptr;
}

void MJDemoBottomThermalShieldAssembly::Place(G4ThreeVector* assemPosition,
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4double eps = 0.001 * mm;
  G4ThreeVector *shieldCanlocalPos = new G4ThreeVector(0, 0, 0.2*25.4*mm-eps);
  G4RotationMatrix *shieldCanlocalRot = new G4RotationMatrix();
  shieldCanlocalRot->rotateZ(-45.83*deg);
  G4AffineTransform *shieldCanaffine = new G4AffineTransform(shieldCanlocalRot,*shieldCanlocalPos);
  *shieldCanaffine*=*assemAffine;
  G4ThreeVector *shieldCanglobalPos = new G4ThreeVector(shieldCanaffine->NetTranslation());
  G4RotationMatrix *shieldCanglobalRot= new G4RotationMatrix(shieldCanaffine->NetRotation());
  fCanPtr->Place(shieldCanglobalPos, shieldCanglobalRot, motherLogical);
  delete shieldCanlocalPos;
  delete shieldCanlocalRot;
  delete shieldCanaffine;

  G4ThreeVector *shieldAnnuluslocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldAnnuluslocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldAnnulusaffine = new G4AffineTransform(shieldAnnuluslocalRot,*shieldAnnuluslocalPos);
  *shieldAnnulusaffine*=*assemAffine;
  G4ThreeVector *shieldAnnulusglobalPos = new G4ThreeVector(shieldAnnulusaffine->NetTranslation());
  G4RotationMatrix *shieldAnnulusglobalRot= new G4RotationMatrix(shieldAnnulusaffine->NetRotation());
  fAnnulusPtr->Place(shieldAnnulusglobalPos, shieldAnnulusglobalRot, motherLogical);
  delete shieldAnnuluslocalPos;
  delete shieldAnnuluslocalRot;
  delete shieldAnnulusaffine;

  G4double supportZposition = 0.073*25.4*mm;
  G4ThreeVector *shieldSupport1localPos = new G4ThreeVector(-6.3202*25.4*mm, 0, supportZposition);
  G4RotationMatrix *shieldSupport1localRot = new G4RotationMatrix();
  G4AffineTransform *shieldSupport1affine = new G4AffineTransform(shieldSupport1localRot,*shieldSupport1localPos);
  *shieldSupport1affine*=*assemAffine;
  G4ThreeVector *shieldSupport1globalPos = new G4ThreeVector(shieldSupport1affine->NetTranslation());
  G4RotationMatrix *shieldSupport1globalRot= new G4RotationMatrix(shieldSupport1affine->NetRotation());
  fSupport1Ptr->Place(shieldSupport1globalPos, shieldSupport1globalRot, motherLogical);
  delete shieldSupport1localPos;
  delete shieldSupport1localRot;
  delete shieldSupport1affine;

  G4ThreeVector *shieldSupport2localPos = new G4ThreeVector(-6.3202*25.4*mm*cos(120*deg),
  												-6.3202*25.4*mm*sin(120*deg), supportZposition);
  G4RotationMatrix *shieldSupport2localRot = new G4RotationMatrix();
  shieldSupport2localRot->rotateZ(-120*deg);
  G4AffineTransform *shieldSupport2affine = new G4AffineTransform(shieldSupport2localRot,*shieldSupport2localPos);
  *shieldSupport2affine*=*assemAffine;
  G4ThreeVector *shieldSupport2globalPos = new G4ThreeVector(shieldSupport2affine->NetTranslation());
  G4RotationMatrix *shieldSupport2globalRot= new G4RotationMatrix(shieldSupport2affine->NetRotation());
  fSupport2Ptr->Place(shieldSupport2globalPos, shieldSupport2globalRot, motherLogical);
  delete shieldSupport2localPos;
  delete shieldSupport2localRot;
  delete shieldSupport2affine;

  G4ThreeVector *shieldSupport3localPos = new G4ThreeVector(-6.3202*25.4*mm*cos(240*deg),
  												-6.3202*25.4*mm*sin(240*deg), supportZposition);
  G4RotationMatrix *shieldSupport3localRot = new G4RotationMatrix();
  shieldSupport3localRot->rotateZ(-240*deg);
  G4AffineTransform *shieldSupport3affine = new G4AffineTransform(shieldSupport3localRot,*shieldSupport3localPos);
  *shieldSupport3affine*=*assemAffine;
  G4ThreeVector *shieldSupport3globalPos = new G4ThreeVector(shieldSupport3affine->NetTranslation());
  G4RotationMatrix *shieldSupport3globalRot= new G4RotationMatrix(shieldSupport3affine->NetRotation());
  fSupport3Ptr->Place(shieldSupport3globalPos, shieldSupport3globalRot, motherLogical);
  delete shieldSupport3localPos;
  delete shieldSupport3localRot;
  delete shieldSupport3affine;

  G4ThreeVector *centeringPin1localPos = new G4ThreeVector(6.25*25.4*mm*cos(20*deg),
  												6.25*25.4*mm*sin(20*deg), 0.425*25.4*mm);
  G4RotationMatrix *centeringPin1localRot = new G4RotationMatrix();
  centeringPin1localRot->rotateZ(-20*deg);
  centeringPin1localRot->rotateY(pi/2);
  G4AffineTransform *centeringPin1affine = new G4AffineTransform(centeringPin1localRot,*centeringPin1localPos);
  *centeringPin1affine*=*assemAffine;
  G4ThreeVector *centeringPin1globalPos = new G4ThreeVector(centeringPin1affine->NetTranslation());
  G4RotationMatrix *centeringPin1globalRot= new G4RotationMatrix(centeringPin1affine->NetRotation());
  fCenterPin1Ptr->Place(centeringPin1globalPos, centeringPin1globalRot, motherLogical);
  delete centeringPin1localPos;
  delete centeringPin1localRot;
  delete centeringPin1affine;

  G4ThreeVector *centeringPin2localPos = new G4ThreeVector(6.25*25.4*mm*cos(20*deg),
  												6.25*25.4*mm*sin(20*deg), 0.425*25.4*mm);
  G4RotationMatrix *centeringPin2localRot = new G4RotationMatrix();
  centeringPin2localRot->rotateZ(-20*deg);
  centeringPin2localRot->rotateY(pi/2);
  G4AffineTransform *centeringPin2affine = new G4AffineTransform(centeringPin2localRot,*centeringPin2localPos);
  *centeringPin2affine*=*assemAffine;
  G4ThreeVector *centeringPin2globalPos = new G4ThreeVector(centeringPin2affine->NetTranslation());
  G4RotationMatrix *centeringPin2globalRot= new G4RotationMatrix(centeringPin2affine->NetRotation());
  fCenterPin2Ptr->Place(centeringPin2globalPos, centeringPin2globalRot, motherLogical);
  delete centeringPin2localPos;
  delete centeringPin2localRot;
  delete centeringPin2affine;

  G4ThreeVector *centeringPin3localPos = new G4ThreeVector(6.25*25.4*mm*cos(190*deg),
  												6.25*25.4*mm*sin(190*deg), 0.425*25.4*mm);
  G4RotationMatrix *centeringPin3localRot = new G4RotationMatrix();
  centeringPin3localRot->rotateZ(-190*deg);
  centeringPin3localRot->rotateY(pi/2);
  G4AffineTransform *centeringPin3affine = new G4AffineTransform(centeringPin3localRot,*centeringPin3localPos);
  *centeringPin3affine*=*assemAffine;
  G4ThreeVector *centeringPin3globalPos = new G4ThreeVector(centeringPin3affine->NetTranslation());
  G4RotationMatrix *centeringPin3globalRot= new G4RotationMatrix(centeringPin3affine->NetRotation());
  fCenterPin3Ptr->Place(centeringPin3globalPos, centeringPin3globalRot, motherLogical);
  delete centeringPin3localPos;
  delete centeringPin3localRot;
  delete centeringPin3affine;

  G4double wedgeZposition = -0.178*25.4*mm;
  G4ThreeVector *shieldWedge1localPos = new G4ThreeVector(-6.3202*25.4*mm, 0, wedgeZposition);
  G4RotationMatrix *shieldWedge1localRot = new G4RotationMatrix();
  G4AffineTransform *shieldWedge1affine = new G4AffineTransform(shieldWedge1localRot,*shieldWedge1localPos);
  *shieldWedge1affine*=*assemAffine;
  G4ThreeVector *shieldWedge1globalPos = new G4ThreeVector(shieldWedge1affine->NetTranslation());
  G4RotationMatrix *shieldWedge1globalRot= new G4RotationMatrix(shieldWedge1affine->NetRotation());
  fWedge1Ptr->Place(shieldWedge1globalPos, shieldWedge1globalRot, motherLogical);
  delete shieldWedge1localPos;
  delete shieldWedge1localRot;
  delete shieldWedge1affine;

  G4ThreeVector *shieldWedge2localPos = new G4ThreeVector(-6.3202*25.4*mm*cos(120*deg),
  											-6.3202*25.4*mm*sin(120*deg), wedgeZposition);
  G4RotationMatrix *shieldWedge2localRot = new G4RotationMatrix();
  shieldWedge2localRot->rotateZ(-120*deg);
  G4AffineTransform *shieldWedge2affine = new G4AffineTransform(shieldWedge2localRot,*shieldWedge2localPos);
  *shieldWedge2affine*=*assemAffine;
  G4ThreeVector *shieldWedge2globalPos = new G4ThreeVector(shieldWedge2affine->NetTranslation());
  G4RotationMatrix *shieldWedge2globalRot= new G4RotationMatrix(shieldWedge2affine->NetRotation());
  fWedge2Ptr->Place(shieldWedge2globalPos, shieldWedge2globalRot, motherLogical);
  delete shieldWedge2localPos;
  delete shieldWedge2localRot;
  delete shieldWedge2affine;

  G4ThreeVector *shieldWedge3localPos = new G4ThreeVector(-6.3202*25.4*mm*cos(240*deg),
  											-6.3202*25.4*mm*sin(240*deg), wedgeZposition);
  G4RotationMatrix *shieldWedge3localRot = new G4RotationMatrix();
  shieldWedge3localRot->rotateZ(-240*deg);
  G4AffineTransform *shieldWedge3affine = new G4AffineTransform(shieldWedge3localRot,*shieldWedge3localPos);
  *shieldWedge3affine*=*assemAffine;
  G4ThreeVector *shieldWedge3globalPos = new G4ThreeVector(shieldWedge3affine->NetTranslation());
  G4RotationMatrix *shieldWedge3globalRot= new G4RotationMatrix(shieldWedge3affine->NetRotation());
  fWedge3Ptr->Place(shieldWedge3globalPos, shieldWedge3globalRot, motherLogical);
  delete shieldWedge3localPos;
  delete shieldWedge3localRot;
  delete shieldWedge3affine;

  G4double screwRad = 5.86*25.4*mm;
  G4double screwAng = 360.0/9.0*deg;
  G4double screwOffset = 40*deg;
  G4double screwZposition = 0.7*25.4*mm+eps;
  G4ThreeVector *screw1localPos = new G4ThreeVector(screwRad*cos(0*screwAng+screwOffset),
  													screwRad*sin(0*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw1localRot = new G4RotationMatrix();
  G4AffineTransform *screw1affine = new G4AffineTransform(screw1localRot,*screw1localPos);
  *screw1affine*=*assemAffine;
  G4ThreeVector *screw1globalPos = new G4ThreeVector(screw1affine->NetTranslation());
  G4RotationMatrix *screw1globalRot= new G4RotationMatrix(screw1affine->NetRotation());
  fScrew1Ptr->Place(screw1globalPos, screw1globalRot, motherLogical);
  delete screw1localPos;
  delete screw1localRot;
  delete screw1affine;

  G4ThreeVector *screw2localPos = new G4ThreeVector(screwRad*cos(1*screwAng+screwOffset),
  													screwRad*sin(1*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw2localRot = new G4RotationMatrix();
  G4AffineTransform *screw2affine = new G4AffineTransform(screw2localRot,*screw2localPos);
  *screw2affine*=*assemAffine;
  G4ThreeVector *screw2globalPos = new G4ThreeVector(screw2affine->NetTranslation());
  G4RotationMatrix *screw2globalRot= new G4RotationMatrix(screw2affine->NetRotation());
  fScrew2Ptr->Place(screw2globalPos, screw2globalRot, motherLogical);
  delete screw2localPos;
  delete screw2localRot;
  delete screw2affine;


  G4ThreeVector *screw3localPos = new G4ThreeVector(screwRad*cos(2*screwAng+screwOffset),
  													screwRad*sin(2*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw3localRot = new G4RotationMatrix();
  G4AffineTransform *screw3affine = new G4AffineTransform(screw3localRot,*screw3localPos);
  *screw3affine*=*assemAffine;
  G4ThreeVector *screw3globalPos = new G4ThreeVector(screw3affine->NetTranslation());
  G4RotationMatrix *screw3globalRot= new G4RotationMatrix(screw3affine->NetRotation());
  fScrew3Ptr->Place(screw3globalPos, screw3globalRot, motherLogical);
  delete screw3localPos;
  delete screw3localRot;
  delete screw3affine;


  G4ThreeVector *screw4localPos = new G4ThreeVector(screwRad*cos(3*screwAng+screwOffset),
  													screwRad*sin(3*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw4localRot = new G4RotationMatrix();
  G4AffineTransform *screw4affine = new G4AffineTransform(screw4localRot,*screw4localPos);
  *screw4affine*=*assemAffine;
  G4ThreeVector *screw4globalPos = new G4ThreeVector(screw4affine->NetTranslation());
  G4RotationMatrix *screw4globalRot= new G4RotationMatrix(screw4affine->NetRotation());
  fScrew4Ptr->Place(screw4globalPos, screw4globalRot, motherLogical);
  delete screw4localPos;
  delete screw4localRot;
  delete screw4affine;


  G4ThreeVector *screw5localPos = new G4ThreeVector(screwRad*cos(4*screwAng+screwOffset),
  													screwRad*sin(4*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw5localRot = new G4RotationMatrix();
  G4AffineTransform *screw5affine = new G4AffineTransform(screw5localRot,*screw5localPos);
  *screw5affine*=*assemAffine;
  G4ThreeVector *screw5globalPos = new G4ThreeVector(screw5affine->NetTranslation());
  G4RotationMatrix *screw5globalRot= new G4RotationMatrix(screw5affine->NetRotation());
  fScrew5Ptr->Place(screw5globalPos, screw5globalRot, motherLogical);
  delete screw5localPos;
  delete screw5localRot;
  delete screw5affine;


  G4ThreeVector *screw6localPos = new G4ThreeVector(screwRad*cos(5*screwAng+screwOffset),
  													screwRad*sin(5*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw6localRot = new G4RotationMatrix();
  G4AffineTransform *screw6affine = new G4AffineTransform(screw6localRot,*screw6localPos);
  *screw6affine*=*assemAffine;
  G4ThreeVector *screw6globalPos = new G4ThreeVector(screw6affine->NetTranslation());
  G4RotationMatrix *screw6globalRot= new G4RotationMatrix(screw6affine->NetRotation());
  fScrew6Ptr->Place(screw6globalPos, screw6globalRot, motherLogical);
  delete screw6localPos;
  delete screw6localRot;
  delete screw6affine;


  G4ThreeVector *screw7localPos = new G4ThreeVector(screwRad*cos(6*screwAng+screwOffset),
  													screwRad*sin(6*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw7localRot = new G4RotationMatrix();
  G4AffineTransform *screw7affine = new G4AffineTransform(screw7localRot,*screw7localPos);
  *screw7affine*=*assemAffine;
  G4ThreeVector *screw7globalPos = new G4ThreeVector(screw7affine->NetTranslation());
  G4RotationMatrix *screw7globalRot= new G4RotationMatrix(screw7affine->NetRotation());
  fScrew7Ptr->Place(screw7globalPos, screw7globalRot, motherLogical);
  delete screw7localPos;
  delete screw7localRot;
  delete screw7affine;


  G4ThreeVector *screw8localPos = new G4ThreeVector(screwRad*cos(7*screwAng+screwOffset),
  													screwRad*sin(7*screwAng+screwOffset),
  													screwZposition);
  G4RotationMatrix *screw8localRot = new G4RotationMatrix();
  G4AffineTransform *screw8affine = new G4AffineTransform(screw8localRot,*screw8localPos);
  *screw8affine*=*assemAffine;
  G4ThreeVector *screw8globalPos = new G4ThreeVector(screw8affine->NetTranslation());
  G4RotationMatrix *screw8globalRot= new G4RotationMatrix(screw8affine->NetRotation());
  fScrew8Ptr->Place(screw8globalPos, screw8globalRot, motherLogical);
  delete screw8localPos;
  delete screw8localRot;
  delete screw8affine;

  G4ThreeVector *sideScrew1localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+0)*deg),
  															6.141*25.4*mm*sin((45.83+0)*deg), 0);//6.14
  G4RotationMatrix *sideScrew1localRot = new G4RotationMatrix();
  sideScrew1localRot->rotateZ(-(45.83+0)*deg);
  sideScrew1localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew1affine = new G4AffineTransform(sideScrew1localRot,*sideScrew1localPos);
  *sideScrew1affine*=*assemAffine;
  G4ThreeVector *sideScrew1globalPos = new G4ThreeVector(sideScrew1affine->NetTranslation());
  G4RotationMatrix *sideScrew1globalRot= new G4RotationMatrix(sideScrew1affine->NetRotation());
  fSideScrew1Ptr->Place(sideScrew1globalPos, sideScrew1globalRot, motherLogical);
  delete sideScrew1localPos;
  delete sideScrew1localRot;
  delete sideScrew1affine;

  G4ThreeVector *sideScrew2localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+60)*deg),
  															6.141*25.4*mm*sin((45.83+60)*deg), 0);
  G4RotationMatrix *sideScrew2localRot = new G4RotationMatrix();
  sideScrew2localRot->rotateZ(-(45.83+60)*deg);
  sideScrew2localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew2affine = new G4AffineTransform(sideScrew2localRot,*sideScrew2localPos);
  *sideScrew2affine*=*assemAffine;
  G4ThreeVector *sideScrew2globalPos = new G4ThreeVector(sideScrew2affine->NetTranslation());
  G4RotationMatrix *sideScrew2globalRot= new G4RotationMatrix(sideScrew2affine->NetRotation());
  fSideScrew2Ptr->Place(sideScrew2globalPos, sideScrew2globalRot, motherLogical);
  delete sideScrew2localPos;
  delete sideScrew2localRot;
  delete sideScrew2affine;

  G4ThreeVector *sideScrew3localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+120)*deg),
  															6.141*25.4*mm*sin((45.83+120)*deg), 0);
  G4RotationMatrix *sideScrew3localRot = new G4RotationMatrix();
  sideScrew3localRot->rotateZ(-(45.83+120)*deg);
  sideScrew3localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew3affine = new G4AffineTransform(sideScrew3localRot,*sideScrew3localPos);
  *sideScrew3affine*=*assemAffine;
  G4ThreeVector *sideScrew3globalPos = new G4ThreeVector(sideScrew3affine->NetTranslation());
  G4RotationMatrix *sideScrew3globalRot= new G4RotationMatrix(sideScrew3affine->NetRotation());
  fSideScrew3Ptr->Place(sideScrew3globalPos, sideScrew3globalRot, motherLogical);
  delete sideScrew3localPos;
  delete sideScrew3localRot;
  delete sideScrew3affine;

  G4ThreeVector *sideScrew4localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+180)*deg),
  															6.141*25.4*mm*sin((45.83+180)*deg), 0);
  G4RotationMatrix *sideScrew4localRot = new G4RotationMatrix();
  sideScrew4localRot->rotateZ(-(45.83+180)*deg);
  sideScrew4localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew4affine = new G4AffineTransform(sideScrew4localRot,*sideScrew4localPos);
  *sideScrew4affine*=*assemAffine;
  G4ThreeVector *sideScrew4globalPos = new G4ThreeVector(sideScrew4affine->NetTranslation());
  G4RotationMatrix *sideScrew4globalRot= new G4RotationMatrix(sideScrew4affine->NetRotation());
  fSideScrew4Ptr->Place(sideScrew4globalPos, sideScrew4globalRot, motherLogical);
  delete sideScrew4localPos;
  delete sideScrew4localRot;
  delete sideScrew4affine;

  G4ThreeVector *sideScrew5localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+240)*deg),
  															6.141*25.4*mm*sin((45.83+240)*deg), 0);
  G4RotationMatrix *sideScrew5localRot = new G4RotationMatrix();
  sideScrew5localRot->rotateZ(-(45.83+240)*deg);
  sideScrew5localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew5affine = new G4AffineTransform(sideScrew5localRot,*sideScrew5localPos);
  *sideScrew5affine*=*assemAffine;
  G4ThreeVector *sideScrew5globalPos = new G4ThreeVector(sideScrew5affine->NetTranslation());
  G4RotationMatrix *sideScrew5globalRot= new G4RotationMatrix(sideScrew5affine->NetRotation());
  fSideScrew5Ptr->Place(sideScrew5globalPos, sideScrew5globalRot, motherLogical);
  delete sideScrew5localPos;
  delete sideScrew5localRot;
  delete sideScrew5affine;

  G4ThreeVector *sideScrew6localPos = new G4ThreeVector(6.141*25.4*mm*cos((45.83+300)*deg),
  															6.141*25.4*mm*sin((45.83+300)*deg), 0);
  G4RotationMatrix *sideScrew6localRot = new G4RotationMatrix();
  sideScrew6localRot->rotateZ(-(45.83+300)*deg);
  sideScrew6localRot->rotateY(-pi/2);
  G4AffineTransform *sideScrew6affine = new G4AffineTransform(sideScrew6localRot,*sideScrew6localPos);
  *sideScrew6affine*=*assemAffine;
  G4ThreeVector *sideScrew6globalPos = new G4ThreeVector(sideScrew6affine->NetTranslation());
  G4RotationMatrix *sideScrew6globalRot= new G4RotationMatrix(sideScrew6affine->NetRotation());
  fSideScrew6Ptr->Place(sideScrew6globalPos, sideScrew6globalRot, motherLogical);
  delete sideScrew6localPos;
  delete sideScrew6localRot;
  delete sideScrew6affine;

  delete assemAffine;
}
