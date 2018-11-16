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
// CLASS IMPLEMENTATION:  MJDemoRadShieldAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  COM of Shielding Box.
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
 * 09-01-2017, Move Poly things to PolyAssembly
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
#include "mjdemonstrator/MJDemoRadShieldAssembly.hh"
#include "mjdemonstrator/MJDemoRadShieldCuInner.hh"
#include "mjdemonstrator/MJDemoRadShieldCuOuter.hh"
#include "mjdemonstrator/MJDemoRadShieldPbInner.hh"
#include "mjdemonstrator/MJDemoRadShieldPbOuter.hh"
#include "mjdemonstrator/MJDemoRadShieldShadowPb.hh"
#include "mjdemonstrator/MJDemoRadShieldN2VolumeBlank.hh"
#include "mjdemonstrator/MJDemoRadShieldN2VolumeEast.hh"
#include "mjdemonstrator/MJDemoRadShieldN2VolumeWest.hh"
#include "mjdemonstrator/MJDemoRadShieldRnBox.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoRadShieldAssembly::MJDemoRadShieldAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "RadShield")
{
  if (fSerialNumber.contains("IC")) fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "Copper-EF");
  else fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "Nitrogen-Gas");
  fCuOuterPtr = new MJDemoRadShieldCuOuter(fName+"_RadShieldCuOuter_001", "867-5309");  
  fPbInnerPtr = new MJDemoRadShieldPbInner(fName+"_RadShieldPbInner_001", "867-5309"); 
  fPbOuterPtr = new MJDemoRadShieldPbOuter(fName+"_RadShieldPbOuter_001", "867-5309"); 
  if (fSerialNumber.contains("ShadowShield")) {
    fShadow1Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_001", serialNumber); 
    fShadow2Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_002", serialNumber);  
  }
  else {
    fShadow1Ptr = nullptr;
    fShadow2Ptr = nullptr;
  }
  fN2BlankEPtr = new MJDemoRadShieldN2VolumeBlank(fName+"_RadShieldN2Blank_001", "867-5309");
  fN2BlankWPtr = new MJDemoRadShieldN2VolumeBlank(fName+"_RadShieldN2Blank_002", "867-5309");
  fN2EPtr = new MJDemoRadShieldN2VolumeEast(fName+"_RadShieldN2East_001", "867-5309");
  fN2WPtr = new MJDemoRadShieldN2VolumeWest(fName+"_RadShieldN2West_001", "867-5309");
  fRnBoxPtr = new MJDemoRadShieldRnBox(fName+"_RadShieldRnBox_001", "867-5309");   
  }

MJDemoRadShieldAssembly::MJDemoRadShieldAssembly(const MJDemoRadShieldAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  if (fSerialNumber.contains("IC")) fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "Copper-EF");
  else fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "Nitrogen-Gas");
  fCuOuterPtr = new MJDemoRadShieldCuOuter(fName+"_RadShieldCuOuter_001", "867-5309");  
  fPbInnerPtr = new MJDemoRadShieldPbInner(fName+"_RadShieldPbInner_001", "867-5309");
  fPbOuterPtr = new MJDemoRadShieldPbOuter(fName+"_RadShieldPbOuter_001", "867-5309");
  if (fSerialNumber.contains("ShadowShield")) {
    fShadow1Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_001", rhs.GetShadow1()->GetSerialNumber()); 
    fShadow2Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_002", rhs.GetShadow2()->GetSerialNumber());
  }
  else {
    fShadow1Ptr = nullptr;
    fShadow2Ptr = nullptr;
  }
  fN2BlankEPtr = new MJDemoRadShieldN2VolumeBlank(fName+"_RadShieldN2Blank_001", "867-5309");
  fN2BlankWPtr = new MJDemoRadShieldN2VolumeBlank(fName+"_RadShieldN2Blank_002", "867-5309");
  fN2EPtr = new MJDemoRadShieldN2VolumeEast(fName+"_RadShieldN2East_001", "867-5309");
  fN2WPtr = new MJDemoRadShieldN2VolumeWest(fName+"_RadShieldN2West_001", "867-5309");	 
  fRnBoxPtr = new MJDemoRadShieldRnBox(fName+"_RadShieldRnBox_001", "867-5309");   
}

MJDemoRadShieldAssembly::~MJDemoRadShieldAssembly()
{
  delete fCuInnerPtr;
  delete fCuOuterPtr;
  delete fPbInnerPtr;
  delete fPbOuterPtr;
  if(fShadow1Ptr!=nullptr) delete fShadow1Ptr;
  if(fShadow2Ptr!=nullptr) delete fShadow2Ptr;
  delete fN2BlankEPtr;
  delete fN2BlankWPtr;
  delete fN2EPtr;
  delete fN2WPtr;
  delete fRnBoxPtr;
}

void MJDemoRadShieldAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *shieldCuInnerlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldCuInnerlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldCuInneraffine = new G4AffineTransform(shieldCuInnerlocalRot,*shieldCuInnerlocalPos);
  *shieldCuInneraffine*=*assemAffine;  
  G4ThreeVector *shieldCuInnerglobalPos = new G4ThreeVector(shieldCuInneraffine->NetTranslation());
  G4RotationMatrix *shieldCuInnerglobalRot= new G4RotationMatrix(shieldCuInneraffine->NetRotation());
  fCuInnerPtr->Place(shieldCuInnerglobalPos, shieldCuInnerglobalRot, motherLogical);
  delete shieldCuInnerlocalPos;
  delete shieldCuInnerlocalRot;
  delete shieldCuInneraffine;
  
  G4ThreeVector *shieldCuOuterlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldCuOuterlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldCuOuteraffine = new G4AffineTransform(shieldCuOuterlocalRot,*shieldCuOuterlocalPos);
  *shieldCuOuteraffine*=*assemAffine;  
  G4ThreeVector *shieldCuOuterglobalPos = new G4ThreeVector(shieldCuOuteraffine->NetTranslation());
  G4RotationMatrix *shieldCuOuterglobalRot= new G4RotationMatrix(shieldCuOuteraffine->NetRotation());
  fCuOuterPtr->Place(shieldCuOuterglobalPos, shieldCuOuterglobalRot, motherLogical);
  delete shieldCuOuterlocalPos;
  delete shieldCuOuterlocalRot;
  delete shieldCuOuteraffine;

  //Inner and outer partitions of Pb shield have same center and rotation
  G4ThreeVector *shieldPblocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldPblocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldPbaffine = new G4AffineTransform(shieldPblocalRot,*shieldPblocalPos);
  *shieldPbaffine*=*assemAffine;  
  G4ThreeVector *shieldPbglobalPos = new G4ThreeVector(shieldPbaffine->NetTranslation());
  G4RotationMatrix *shieldPbglobalRot= new G4RotationMatrix(shieldPbaffine->NetRotation());
  fPbInnerPtr->Place(shieldPbglobalPos, shieldPbglobalRot, motherLogical);
  fPbOuterPtr->Place(shieldPbglobalPos, shieldPbglobalRot, motherLogical);
  delete shieldPblocalPos;
  delete shieldPblocalRot;
  delete shieldPbaffine;
  
  G4ThreeVector *shieldShadowPb1localPos = new G4ThreeVector(70.8427*25.4*mm, 0, 0);
  G4RotationMatrix *shieldShadowPb1localRot = new G4RotationMatrix();
  G4AffineTransform *shieldShadowPb1affine = new G4AffineTransform(shieldShadowPb1localRot,*shieldShadowPb1localPos);
  *shieldShadowPb1affine*=*assemAffine;  
  G4ThreeVector *shieldShadowPb1globalPos = new G4ThreeVector(shieldShadowPb1affine->NetTranslation());
  G4RotationMatrix *shieldShadowPb1globalRot = new G4RotationMatrix(shieldShadowPb1affine->NetRotation());
  if(fShadow1Ptr!=nullptr) fShadow1Ptr->Place(shieldShadowPb1globalPos, shieldShadowPb1globalRot, motherLogical);
  delete shieldShadowPb1localPos;
  delete shieldShadowPb1localRot; 
  delete shieldShadowPb1affine;
  
  G4ThreeVector *shieldShadowPb2localPos = new G4ThreeVector(-8.1417*25.4*mm, -59.6695*25.4*mm, 0);
  G4RotationMatrix *shieldShadowPb2localRot = new G4RotationMatrix();
  shieldShadowPb2localRot->rotateZ(-pi/2);
  G4AffineTransform *shieldShadowPb2affine = new G4AffineTransform(shieldShadowPb2localRot,*shieldShadowPb2localPos);
  *shieldShadowPb2affine*=*assemAffine;  
  G4ThreeVector *shieldShadowPb2globalPos = new G4ThreeVector(shieldShadowPb2affine->NetTranslation());
  G4RotationMatrix *shieldShadowPb2globalRot = new G4RotationMatrix(shieldShadowPb2affine->NetRotation());
  if(fShadow2Ptr!=nullptr) fShadow2Ptr->Place(shieldShadowPb2globalPos, shieldShadowPb2globalRot, motherLogical);
  delete shieldShadowPb2localPos;
  delete shieldShadowPb2localRot;
  delete shieldShadowPb2affine;

  G4ThreeVector *shieldN2ElocalPos = new G4ThreeVector(8.015*25.4*mm, 0, 0);
  G4RotationMatrix *shieldN2ElocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldN2Eaffine = new G4AffineTransform(shieldN2ElocalRot,*shieldN2ElocalPos);
  *shieldN2Eaffine*=*assemAffine;
  G4ThreeVector *shieldN2EglobalPos = new G4ThreeVector(shieldN2Eaffine->NetTranslation());
  G4RotationMatrix *shieldN2EglobalRot= new G4RotationMatrix(shieldN2Eaffine->NetRotation());
  if (fSerialNumber.contains("E")) fN2EPtr->Place(shieldN2EglobalPos, shieldN2EglobalRot, motherLogical);
  else fN2BlankEPtr->Place(shieldN2EglobalPos, shieldN2EglobalRot, motherLogical);
  delete shieldN2ElocalPos;
  delete shieldN2ElocalRot;
  delete shieldN2Eaffine;
  
  G4ThreeVector *shieldN2WlocalPos = new G4ThreeVector(-8.015*25.4*mm, 0, 0);
  G4RotationMatrix *shieldN2WlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldN2Waffine = new G4AffineTransform(shieldN2WlocalRot,*shieldN2WlocalPos);
  *shieldN2Waffine*=*assemAffine;
  G4ThreeVector *shieldN2WglobalPos = new G4ThreeVector(shieldN2Waffine->NetTranslation());
  G4RotationMatrix *shieldN2WglobalRot= new G4RotationMatrix(shieldN2Waffine->NetRotation());
  MGLog(debugging) << "RadShield SN: " << fSerialNumber << endlog;
  if (fSerialNumber.contains("W")) fN2WPtr->Place(shieldN2WglobalPos, shieldN2WglobalRot, motherLogical);
  else fN2BlankWPtr->Place(shieldN2WglobalPos, shieldN2WglobalRot, motherLogical);
  delete shieldN2WlocalPos;
  delete shieldN2WlocalRot;
  delete shieldN2Waffine;
 
	
  G4ThreeVector *shieldRnBoxlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldRnBoxlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldRnBoxaffine = new G4AffineTransform(shieldRnBoxlocalRot, *shieldRnBoxlocalPos);
  *shieldRnBoxaffine*=*assemAffine;
  G4ThreeVector *shieldRnBoxglobalPos = new G4ThreeVector(shieldRnBoxaffine->NetTranslation());
  G4RotationMatrix *shieldRnBoxglobalRot= new G4RotationMatrix(shieldRnBoxaffine->NetRotation());
  fRnBoxPtr->Place(shieldRnBoxglobalPos, shieldRnBoxglobalRot, motherLogical);
  delete shieldRnBoxlocalPos;
  delete shieldRnBoxlocalRot;
  delete shieldRnBoxaffine;
  delete assemAffine;
}
