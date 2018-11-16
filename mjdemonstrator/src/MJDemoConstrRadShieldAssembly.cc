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
/*
* 09-01-2017, moved overfloorpanels class to poly assembly, R. Massarczyk
*/


#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoConstrRadShieldAssembly.hh"
#include "mjdemonstrator/MJDemoRadShieldCuInner.hh"
#include "mjdemonstrator/MJDemoRadShieldCuOuter.hh"
#include "mjdemonstrator/MJDemoRadShieldPb.hh"
#include "mjdemonstrator/MJDemoRadShieldShadowPb.hh"
#include "mjdemonstrator/MJDemoConstrRadShieldN2Volume.hh"
#include "mjdemonstrator/MJDemoRadShieldRnBox.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoConstrRadShieldAssembly::MJDemoConstrRadShieldAssembly(G4String fName, G4String serialNumber, G4String constrConfig) :
  MJVDemoAssembly(fName, serialNumber, "RadShield"), fConstrConfig(constrConfig)
{
  fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "867-5309"); 
  fCuOuterPtr = new MJDemoRadShieldCuOuter(fName+"_RadShieldCuOuter_001", "867-5309");  
  fPbPtr = new MJDemoRadShieldPb(fName+"_RadShieldPb_001", "867-5309"); 
  fShadow1Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_001", "867-5309"); 
  fShadow2Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_002", "867-5309");  
  //fN2Ptr = new MJDemoRadShieldN2Volume(fName+"_RadShieldN2_001", "867-5309");   
  fRnBoxPtr = new MJDemoRadShieldRnBox(fName+"_RadShieldRnBox_001", "867-5309");   
  }

MJDemoConstrRadShieldAssembly::MJDemoConstrRadShieldAssembly(const MJDemoConstrRadShieldAssembly & rhs) : 
  MJVDemoAssembly(rhs), fConstrConfig(rhs.fConstrConfig)
{
  fCuInnerPtr = new MJDemoRadShieldCuInner(fName+"_RadShieldCuInner_001", "867-5309"); 
  fCuOuterPtr = new MJDemoRadShieldCuOuter(fName+"_RadShieldCuOuter_001", "867-5309");  
  fPbPtr = new MJDemoRadShieldPb(fName+"_RadShieldPb_001", "867-5309");
  fShadow1Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_001", "867-5309"); 
  fShadow2Ptr = new MJDemoRadShieldShadowPb(fName+"_RadShieldShadowPb_002", "867-5309");    
  //fN2Ptr = new MJDemoRadShieldN2Volume(fName+"_RadShieldN2_001", "867-5309"); 	
  fRnBoxPtr = new MJDemoRadShieldRnBox(fName+"_RadShieldRnBox_001", "867-5309");   
}

MJDemoConstrRadShieldAssembly::~MJDemoConstrRadShieldAssembly()
{
  delete fCuInnerPtr;
  delete fCuOuterPtr;
  delete fPbPtr;
  delete fShadow1Ptr;
  delete fShadow2Ptr;
  delete fN2Ptr;
  delete fRnBoxPtr;
}

void MJDemoConstrRadShieldAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4bool innerCu, outerCu, lead, PbShadow, poly, polyOF, RnBox;
  if(fConstrConfig.empty())
    innerCu = outerCu = lead = PbShadow = poly = polyOF = RnBox = true;  
  else{
    MGLog(routine)<<"You have chosen to construct the rad shield with only the given components:"<<endl; 
    innerCu = outerCu = lead = PbShadow = poly = polyOF = RnBox = false; 
    if(fConstrConfig[0]=='1') { innerCu = true; MGLog(routine)<<"inner Cu"<<endl; }
    if(fConstrConfig[1]=='1') { outerCu = true; MGLog(routine)<<"outer Cu"<<endl; }
    if(fConstrConfig[2]=='1') { lead = true; MGLog(routine)<<"lead"<<endl; }
    if(fConstrConfig[3]=='1') { PbShadow = true; MGLog(routine)<<"lead behind vacuum cubes (to cover holes where crossarms pass through)"<<endl; }
    if(fConstrConfig[6]=='1') { RnBox = true; MGLog(routine)<<"Rn box"<<endl; }
    }
  fN2Ptr = new MJDemoConstrRadShieldN2Volume(fName+"_ConstrRadShieldN2_001", "867-5309", innerCu); 

  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *shieldCuInnerlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldCuInnerlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldCuInneraffine = new G4AffineTransform(shieldCuInnerlocalRot,*shieldCuInnerlocalPos);
  *shieldCuInneraffine*=*assemAffine;  
  G4ThreeVector *shieldCuInnerglobalPos = new G4ThreeVector(shieldCuInneraffine->NetTranslation());
  G4RotationMatrix *shieldCuInnerglobalRot= new G4RotationMatrix(shieldCuInneraffine->NetRotation());
  if(innerCu) fCuInnerPtr->Place(shieldCuInnerglobalPos, shieldCuInnerglobalRot, motherLogical);
  delete shieldCuInnerlocalPos;
  delete shieldCuInnerlocalRot;
  delete shieldCuInneraffine;
  
  G4ThreeVector *shieldCuOuterlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldCuOuterlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldCuOuteraffine = new G4AffineTransform(shieldCuOuterlocalRot,*shieldCuOuterlocalPos);
  *shieldCuOuteraffine*=*assemAffine;  
  G4ThreeVector *shieldCuOuterglobalPos = new G4ThreeVector(shieldCuOuteraffine->NetTranslation());
  G4RotationMatrix *shieldCuOuterglobalRot= new G4RotationMatrix(shieldCuOuteraffine->NetRotation());
  if(outerCu) fCuOuterPtr->Place(shieldCuOuterglobalPos, shieldCuOuterglobalRot, motherLogical);
  delete shieldCuOuterlocalPos;
  delete shieldCuOuterlocalRot;
  delete shieldCuOuteraffine;
  
  G4ThreeVector *shieldPblocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldPblocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldPbaffine = new G4AffineTransform(shieldPblocalRot,*shieldPblocalPos);
  *shieldPbaffine*=*assemAffine;  
  G4ThreeVector *shieldPbglobalPos = new G4ThreeVector(shieldPbaffine->NetTranslation());
  G4RotationMatrix *shieldPbglobalRot= new G4RotationMatrix(shieldPbaffine->NetRotation());
  if(lead) fPbPtr->Place(shieldPbglobalPos, shieldPbglobalRot, motherLogical);
  delete shieldPblocalPos;
  delete shieldPblocalRot;
  delete shieldPbaffine;
  
  G4ThreeVector *shieldShadowPb1localPos = new G4ThreeVector(70.8427*25.4*mm, 0, 0);
  G4RotationMatrix *shieldShadowPb1localRot = new G4RotationMatrix();
  G4AffineTransform *shieldShadowPb1affine = new G4AffineTransform(shieldShadowPb1localRot,*shieldShadowPb1localPos);
  *shieldShadowPb1affine*=*assemAffine;  
  G4ThreeVector *shieldShadowPb1globalPos = new G4ThreeVector(shieldShadowPb1affine->NetTranslation());
  G4RotationMatrix *shieldShadowPb1globalRot= new G4RotationMatrix(shieldShadowPb1affine->NetRotation());
  if(PbShadow) fShadow1Ptr->Place(shieldShadowPb1globalPos, shieldShadowPb1globalRot, motherLogical);
  delete shieldShadowPb1localPos;
  delete shieldShadowPb1localRot; 
  delete shieldShadowPb1affine;
  
  G4ThreeVector *shieldShadowPb2localPos = new G4ThreeVector(-8.1417*25.4*mm, -59.6695*25.4*mm, 0);
  G4RotationMatrix *shieldShadowPb2localRot = new G4RotationMatrix();
  shieldShadowPb2localRot->rotateZ(-pi/2);
  G4AffineTransform *shieldShadowPb2affine = new G4AffineTransform(shieldShadowPb2localRot,*shieldShadowPb2localPos);
  *shieldShadowPb2affine*=*assemAffine;  
  G4ThreeVector *shieldShadowPb2globalPos = new G4ThreeVector(shieldShadowPb2affine->NetTranslation());
  G4RotationMatrix *shieldShadowPb2globalRot= new G4RotationMatrix(shieldShadowPb2affine->NetRotation());
  if(PbShadow) fShadow2Ptr->Place(shieldShadowPb2globalPos, shieldShadowPb2globalRot, motherLogical);
  delete shieldShadowPb2localPos;
  delete shieldShadowPb2localRot;
  delete shieldShadowPb2affine;

  G4ThreeVector *shieldN2localPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldN2localRot = new G4RotationMatrix();
  G4AffineTransform *shieldN2affine = new G4AffineTransform(shieldN2localRot,*shieldN2localPos);
  *shieldN2affine*=*assemAffine;  
  G4ThreeVector *shieldN2globalPos = new G4ThreeVector(shieldN2affine->NetTranslation());
  G4RotationMatrix *shieldN2globalRot= new G4RotationMatrix(shieldN2affine->NetRotation());
  fN2Ptr->Place(shieldN2globalPos, shieldN2globalRot, motherLogical);
  delete shieldN2localPos;
  delete shieldN2localRot;
  delete shieldN2affine;
  

	
  G4ThreeVector *shieldRnBoxlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *shieldRnBoxlocalRot = new G4RotationMatrix();
  G4AffineTransform *shieldRnBoxaffine = new G4AffineTransform(shieldRnBoxlocalRot,*shieldRnBoxlocalPos);
  *shieldRnBoxaffine*=*assemAffine;  
  G4ThreeVector *shieldRnBoxglobalPos = new G4ThreeVector(shieldRnBoxaffine->NetTranslation());
  G4RotationMatrix *shieldRnBoxglobalRot= new G4RotationMatrix(shieldRnBoxaffine->NetRotation());
  if(RnBox) fRnBoxPtr->Place(shieldRnBoxglobalPos, shieldRnBoxglobalRot, motherLogical);
  delete shieldRnBoxlocalPos;
  delete shieldRnBoxlocalRot;
  delete shieldRnBoxaffine;
  delete assemAffine;
}
