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
// CLASS IMPLEMENTATION:  MJDemoTopThermalShieldAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Center Top of Cold Plate.
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
 * 09-22-2014: Created - M. Green
 *
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
#include "mjdemonstrator/MJDemoTopThermalShieldAssembly.hh"
#include "mjdemonstrator/MJDemoThermalShieldCanTop.hh"
#include "mjdemonstrator/MJDemoIRShieldTopBaffle.hh"
#include "mjdemonstrator/MJDemoCableGuardNarrowIRShield.hh"
#include "mjdemonstrator/MJDemoTopIRShieldSupport.hh"
#include "mjdemonstrator/MJDemoTopIRShieldSupportFarEnd.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoTopThermalShieldAssembly::MJDemoTopThermalShieldAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "ThermalShield")
{
  fCanPtr = new MJDemoThermalShieldCanTop(fName+"_ThermalShieldCan_001", "867-5309");
  fBaffle1Ptr = new MJDemoIRShieldTopBaffle(fName+"_Baffle_001", "867-5309");
  fBaffle2Ptr = new MJDemoIRShieldTopBaffle(fName+"_Baffle_002", "867-5309");
  fCableGuardPtr = new MJDemoCableGuardNarrowIRShield(fName+"_CableGuard_001", "867-5309");
  fNearSupportPtr = new MJDemoTopIRShieldSupport(fName+"_NearSupport_001", "867-5309");
  fFarSupportPtr = new MJDemoTopIRShieldSupportFarEnd(fName+"_FarSupport_001", "867-5309");
  fBaffleScrew1Ptr = new  MJDemo832Screw(fName+"_8_32x0.50Screw_001", "867-5309", 12.7);
  fBaffleScrew2Ptr = new  MJDemo832Screw(fName+"_8_32x0.50Screw_002", "867-5309", 12.7);
  fNearScrew1Ptr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fNearScrew2Ptr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fFarScrewPtr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 6.35);
    
}

MJDemoTopThermalShieldAssembly::MJDemoTopThermalShieldAssembly(const MJDemoTopThermalShieldAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCanPtr = new MJDemoThermalShieldCanTop(fName+"_ThermalShieldCan_001", "867-5309");
  fBaffle1Ptr = new MJDemoIRShieldTopBaffle(fName+"_Baffle_001", "867-5309");
  fBaffle2Ptr = new MJDemoIRShieldTopBaffle(fName+"_Baffle_002", "867-5309");
  fCableGuardPtr = new MJDemoCableGuardNarrowIRShield(fName+"_CableGuard_001", "867-5309");
  fNearSupportPtr = new MJDemoTopIRShieldSupport(fName+"_NearSupport_001", "867-5309");
  fFarSupportPtr = new MJDemoTopIRShieldSupportFarEnd(fName+"_FarSupport_001", "867-5309");
  fBaffleScrew1Ptr = new  MJDemo832Screw(fName+"_8_32x0.50Screw_001", "867-5309", 12.7);
  fBaffleScrew2Ptr = new  MJDemo832Screw(fName+"_8_32x0.50Screw_002", "867-5309", 12.7);
  fNearScrew1Ptr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fNearScrew2Ptr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fFarScrewPtr = new  MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 6.35);
}

MJDemoTopThermalShieldAssembly::~MJDemoTopThermalShieldAssembly()
{
  delete fCanPtr;
  delete fBaffle1Ptr;
  delete fBaffle2Ptr;
  delete fCableGuardPtr;
  delete fNearSupportPtr;
  delete fFarSupportPtr;
  delete fBaffleScrew1Ptr;
  delete fBaffleScrew2Ptr;
  delete fNearScrew1Ptr;
  delete fNearScrew2Ptr;
  delete fFarScrewPtr;
}

void MJDemoTopThermalShieldAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4double eps = 0.001 * mm;
  G4ThreeVector *shieldCanlocalPos = new G4ThreeVector(0, 0, 1*mm);
  G4RotationMatrix *shieldCanlocalRot = new G4RotationMatrix();
  shieldCanlocalRot->rotateX(pi);
  G4AffineTransform *shieldCanaffine = new G4AffineTransform(shieldCanlocalRot,*shieldCanlocalPos);
  *shieldCanaffine*=*assemAffine;  
  G4ThreeVector *shieldCanglobalPos = new G4ThreeVector(shieldCanaffine->NetTranslation());
  G4RotationMatrix *shieldCanglobalRot= new G4RotationMatrix(shieldCanaffine->NetRotation());
  fCanPtr->Place(shieldCanglobalPos, shieldCanglobalRot, motherLogical);
  delete shieldCanlocalPos;
  delete shieldCanlocalRot;
  delete shieldCanaffine;

  G4ThreeVector *Baffle1localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, (6.35+76.217/2)*mm+eps, 95.25/2*mm);
  G4RotationMatrix *Baffle1localRot = new G4RotationMatrix();
  Baffle1localRot->rotateZ(-pi/2);
  G4AffineTransform *Baffle1affine = new G4AffineTransform(Baffle1localRot,*Baffle1localPos);
  *Baffle1affine*=*assemAffine;
  G4ThreeVector *Baffle1globalPos = new G4ThreeVector(Baffle1affine->NetTranslation());
  G4RotationMatrix *Baffle1globalRot= new G4RotationMatrix(Baffle1affine->NetRotation());
  fBaffle1Ptr->Place(Baffle1globalPos, Baffle1globalRot, motherLogical);
  delete Baffle1localPos;
  delete Baffle1localRot;
  delete Baffle1affine;
    
  G4ThreeVector *Baffle2localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, -(6.35+76.217/2)*mm-eps, 95.25/2*mm);
  G4RotationMatrix *Baffle2localRot = new G4RotationMatrix();
  Baffle2localRot->rotateZ(pi/2);
  G4AffineTransform *Baffle2affine = new G4AffineTransform(Baffle2localRot,*Baffle2localPos);
  *Baffle2affine*=*assemAffine;
  G4ThreeVector *Baffle2globalPos = new G4ThreeVector(Baffle2affine->NetTranslation());
  G4RotationMatrix *Baffle2globalRot= new G4RotationMatrix(Baffle2affine->NetRotation());
  fBaffle2Ptr->Place(Baffle2globalPos, Baffle2globalRot, motherLogical);
  delete Baffle2localPos;
  delete Baffle2localRot;
  delete Baffle2affine;
    
  G4ThreeVector *CableGuardlocalPos = new G4ThreeVector(-(144.5)*mm, 0, 25.44*mm-eps); //145.125
  G4RotationMatrix *CableGuardlocalRot = new G4RotationMatrix();
  CableGuardlocalRot->rotateZ(pi/2);
  CableGuardlocalRot->rotateX(-pi/2);
  G4AffineTransform *CableGuardaffine = new G4AffineTransform(CableGuardlocalRot,*CableGuardlocalPos);
  *CableGuardaffine*=*assemAffine;
  G4ThreeVector *CableGuardglobalPos = new G4ThreeVector(CableGuardaffine->NetTranslation());
  G4RotationMatrix *CableGuardglobalRot= new G4RotationMatrix(CableGuardaffine->NetRotation());
  fCableGuardPtr->Place(CableGuardglobalPos, CableGuardglobalRot, motherLogical);
  delete CableGuardlocalPos;
  delete CableGuardlocalRot;
  delete CableGuardaffine;
    
  G4ThreeVector *NearSupportlocalPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, 0, (72.813/2+47.625)*mm);
  G4RotationMatrix *NearSupportlocalRot = new G4RotationMatrix();
  NearSupportlocalRot->rotateZ(pi/2);
  G4AffineTransform *NearSupportaffine = new G4AffineTransform(NearSupportlocalRot,*NearSupportlocalPos);
  *NearSupportaffine*=*assemAffine;
  G4ThreeVector *NearSupportglobalPos = new G4ThreeVector(NearSupportaffine->NetTranslation());
  G4RotationMatrix *NearSupportglobalRot= new G4RotationMatrix(NearSupportaffine->NetRotation());
  fNearSupportPtr->Place(NearSupportglobalPos, NearSupportglobalRot, motherLogical);
  delete NearSupportlocalPos;
  delete NearSupportlocalRot;
  delete NearSupportaffine;
    
  G4ThreeVector *FarSupportlocalPos = new G4ThreeVector((109.855+28.575/2)*mm, 0, (121.313/2-1)*mm);
  G4RotationMatrix *FarSupportlocalRot = new G4RotationMatrix();
  FarSupportlocalRot->rotateZ(pi);
  G4AffineTransform *FarSupportaffine = new G4AffineTransform(FarSupportlocalRot,*FarSupportlocalPos);
  *FarSupportaffine*=*assemAffine;
  G4ThreeVector *FarSupportglobalPos = new G4ThreeVector(FarSupportaffine->NetTranslation());
  G4RotationMatrix *FarSupportglobalRot= new G4RotationMatrix(FarSupportaffine->NetRotation());
  fFarSupportPtr->Place(FarSupportglobalPos, FarSupportglobalRot, motherLogical);
  delete FarSupportlocalPos;
  delete FarSupportlocalRot;
  delete FarSupportaffine;
    
  G4ThreeVector *BaffleScrew1localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, 12.8*mm, (72.813/2+47.625+36.513-35.8105)*mm);
  G4RotationMatrix *BaffleScrew1localRot = new G4RotationMatrix();
  BaffleScrew1localRot->rotateX(pi/2);
  G4AffineTransform *BaffleScrew1affine = new G4AffineTransform(BaffleScrew1localRot,*BaffleScrew1localPos);
  *BaffleScrew1affine*=*assemAffine;
  G4ThreeVector *BaffleScrew1globalPos = new G4ThreeVector(BaffleScrew1affine->NetTranslation());
  G4RotationMatrix *BaffleScrew1globalRot= new G4RotationMatrix(BaffleScrew1affine->NetRotation());
  fBaffleScrew1Ptr->Place(BaffleScrew1globalPos, BaffleScrew1globalRot, motherLogical);
  delete BaffleScrew1localPos;
  delete BaffleScrew1localRot;
  delete BaffleScrew1affine;
   
  G4ThreeVector *BaffleScrew2localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, -12.8*mm, (72.813/2+47.625+36.513-35.8105+6.35)*mm);
  G4RotationMatrix *BaffleScrew2localRot = new G4RotationMatrix();
  BaffleScrew2localRot->rotateX(-pi/2);
  G4AffineTransform *BaffleScrew2affine = new G4AffineTransform(BaffleScrew2localRot,*BaffleScrew2localPos);
  *BaffleScrew2affine*=*assemAffine;
  G4ThreeVector *BaffleScrew2globalPos = new G4ThreeVector(BaffleScrew2affine->NetTranslation());
  G4RotationMatrix *BaffleScrew2globalRot= new G4RotationMatrix(BaffleScrew2affine->NetRotation());
  fBaffleScrew2Ptr->Place(BaffleScrew2globalPos, BaffleScrew2globalRot, motherLogical);
  delete BaffleScrew2localPos;
  delete BaffleScrew2localRot;
  delete BaffleScrew2affine;
    
  G4ThreeVector *NearScrew1localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, 15.875*mm, 122*mm);
  G4RotationMatrix *NearScrew1localRot = new G4RotationMatrix();
  NearScrew1localRot->rotateY(0); //5.362*deg
  G4AffineTransform *NearScrew1affine = new G4AffineTransform(NearScrew1localRot,*NearScrew1localPos);
  *NearScrew1affine*=*assemAffine;
  G4ThreeVector *NearScrew1globalPos = new G4ThreeVector(NearScrew1affine->NetTranslation());
  G4RotationMatrix *NearScrew1globalRot= new G4RotationMatrix(NearScrew1affine->NetRotation());
  fNearScrew1Ptr->Place(NearScrew1globalPos, NearScrew1globalRot, motherLogical);
  delete NearScrew1localPos;
  delete NearScrew1localRot;
  delete NearScrew1affine;
    
  G4ThreeVector *NearScrew2localPos = new G4ThreeVector(-(6.35/2+89.535+32.258)*mm, -15.875*mm, 122*mm);
  G4RotationMatrix *NearScrew2localRot = new G4RotationMatrix();
  NearScrew2localRot->rotateY(0); //5.362*deg
  G4AffineTransform *NearScrew2affine = new G4AffineTransform(NearScrew2localRot,*NearScrew2localPos);
  *NearScrew2affine*=*assemAffine;
  G4ThreeVector *NearScrew2globalPos = new G4ThreeVector(NearScrew2affine->NetTranslation());
  G4RotationMatrix *NearScrew2globalRot= new G4RotationMatrix(NearScrew2affine->NetRotation());
  fNearScrew2Ptr->Place(NearScrew2globalPos, NearScrew2globalRot, motherLogical);
  delete NearScrew2localPos;
  delete NearScrew2localRot;
  delete NearScrew2affine;
  
  G4ThreeVector *FarScrewlocalPos = new G4ThreeVector((109.855+5.058)*mm, 0, 122*mm);
  G4RotationMatrix *FarScrewlocalRot = new G4RotationMatrix();
  FarScrewlocalRot->rotateY(0); //-5.326*deg
  G4AffineTransform *FarScrewaffine = new G4AffineTransform(FarScrewlocalRot,*FarScrewlocalPos);
  *FarScrewaffine*=*assemAffine;
  G4ThreeVector *FarScrewglobalPos = new G4ThreeVector(FarScrewaffine->NetTranslation());
  G4RotationMatrix *FarScrewglobalRot= new G4RotationMatrix(FarScrewaffine->NetRotation());
  fFarScrewPtr->Place(FarScrewglobalPos, FarScrewglobalRot, motherLogical);
  delete FarScrewlocalPos;
  delete FarScrewlocalRot;
  delete FarScrewaffine;
    
    
  delete assemAffine;  
}
