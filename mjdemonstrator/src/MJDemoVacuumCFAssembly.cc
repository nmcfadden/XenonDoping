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
// CLASS IMPLEMENTATION:  MJDemoVacuumCFAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  COM of the vacuum cube
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Sept 28, 2015
 *
 * REVISION:
 *
 * 09-28-2015, Created, M. Green
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
#include "mjdemonstrator/MJDemoVacuumCFAssembly.hh"
#include "mjdemonstrator/MJDemo8CFCube.hh"
#include "mjdemonstrator/MJDemo8CFFlange.hh"
#include "mjdemonstrator/MJDemo8CFWeldedBellows.hh"
#include "mjdemonstrator/MJDemo8CFFlangeShield.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

MJDemoVacuumCFAssembly::MJDemoVacuumCFAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "LMFE")
{
  fCubePtr = new MJDemo8CFCube(fName+"_8CFCube_001", "867-5309");
  fBellowsPtr = new MJDemo8CFWeldedBellows(fName+"_WeldedBellows_001", "867-5309");
  fBackFlangePtr = new MJDemo8CFFlange(fName+"_BackFlange_001", "867-5309");
  fTopFlangePtr = new MJDemo8CFFlange(fName+"_TopFlange_001", "867-5309");
  fBottomFlangePtr = new MJDemo8CFFlange(fName+"_BottomFlange_001", "867-5309");
  fLeftFlangePtr = new MJDemo8CFFlange(fName+"_LeftFlange_002", "867-5309");
  fRightFlangePtr = new MJDemo8CFFlange(fName+"_RightFlange_001", "867-5309");
  if(fSerialNumber.contains("TSShield")) {
    fFlangeShieldPtr = new MJDemo8CFFlangeShield(fName+"_FlangeShield_001", "Copper-EF");
  }
  else {
    fFlangeShieldPtr = new MJDemo8CFFlangeShield(fName+"_FlangeShield_001", "Vacuum");
  }
}

MJDemoVacuumCFAssembly::MJDemoVacuumCFAssembly(const MJDemoVacuumCFAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
    fCubePtr = new MJDemo8CFCube(fName+"_8CFCube_001", "867-5309");
    fBellowsPtr = new MJDemo8CFWeldedBellows(fName+"_WeldedBellows_001", "867-5309");
    fBackFlangePtr = new MJDemo8CFFlange(fName+"_BackFlange_001", "867-5309");
    fTopFlangePtr = new MJDemo8CFFlange(fName+"_TopFlange_001", "867-5309");
    fBottomFlangePtr = new MJDemo8CFFlange(fName+"_BottomFlange_001", "867-5309");
    fLeftFlangePtr = new MJDemo8CFFlange(fName+"_LeftFlange_002", "867-5309");
    fRightFlangePtr = new MJDemo8CFFlange(fName+"_RightFlange_001", "867-5309");
    if(fSerialNumber.contains("TSShield")) {
      fFlangeShieldPtr = new MJDemo8CFFlangeShield(fName+"_FlangeShield_001", "Copper-EF");
    }
    else {
      fFlangeShieldPtr = new MJDemo8CFFlangeShield(fName+"_FlangeShield_001", "Vacuum");
    }
}    

MJDemoVacuumCFAssembly::~MJDemoVacuumCFAssembly()
{
  delete fCubePtr;
  delete fBellowsPtr;
  delete fBackFlangePtr;
  delete fTopFlangePtr;
  delete fLeftFlangePtr;
  delete fRightFlangePtr;
  delete fBottomFlangePtr;
  delete fFlangeShieldPtr;
}

void MJDemoVacuumCFAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
    
  G4ThreeVector *cubelocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *cubelocalRot = new G4RotationMatrix();
  G4AffineTransform *cubeaffine = new G4AffineTransform(cubelocalRot,*cubelocalPos);
  *cubeaffine*=*assemAffine;  
  G4ThreeVector *cubeglobalPos = new G4ThreeVector(cubeaffine->NetTranslation());
  G4RotationMatrix *cubeglobalRot= new G4RotationMatrix(cubeaffine->NetRotation());
  fCubePtr->Place(cubeglobalPos, cubeglobalRot, motherLogical);
  delete cubelocalPos;
  delete cubelocalRot;
  delete cubeaffine;
  
  G4ThreeVector *bellowslocalPos = new G4ThreeVector(4.05*25.4*mm, 0, 0);
  G4RotationMatrix *bellowslocalRot = new G4RotationMatrix();
  bellowslocalRot->rotateY(-pi/2);
  G4AffineTransform *bellowsaffine = new G4AffineTransform(bellowslocalRot,*bellowslocalPos);
  *bellowsaffine*=*assemAffine;
  G4ThreeVector *bellowsglobalPos = new G4ThreeVector(bellowsaffine->NetTranslation());
  G4RotationMatrix *bellowsglobalRot= new G4RotationMatrix(bellowsaffine->NetRotation());
  fBellowsPtr->Place(bellowsglobalPos, bellowsglobalRot, motherLogical);
  delete bellowslocalPos;
  delete bellowslocalRot;
  delete bellowsaffine;
  
  G4ThreeVector *backflangelocalPos = new G4ThreeVector(-4.445*25.4*mm, 0, 0);
  G4RotationMatrix *backflangelocalRot = new G4RotationMatrix();
  backflangelocalRot->rotateY(-pi/2);
  G4AffineTransform *backflangeaffine = new G4AffineTransform(backflangelocalRot,*backflangelocalPos);
  *backflangeaffine*=*assemAffine;
  G4ThreeVector *backflangeglobalPos = new G4ThreeVector(backflangeaffine->NetTranslation());
  G4RotationMatrix *backflangeglobalRot= new G4RotationMatrix(backflangeaffine->NetRotation());
  fBackFlangePtr->Place(backflangeglobalPos, backflangeglobalRot, motherLogical);
  delete backflangelocalPos;
  delete backflangelocalRot;
  delete backflangeaffine;
    
  G4ThreeVector *bottomflangelocalPos = new G4ThreeVector(0, 0, -4.445*25.4*mm);
  G4RotationMatrix *bottomflangelocalRot = new G4RotationMatrix();
  bottomflangelocalRot->rotateY(0);
  G4AffineTransform *bottomflangeaffine = new G4AffineTransform(bottomflangelocalRot,*bottomflangelocalPos);
  *bottomflangeaffine*=*assemAffine;
  G4ThreeVector *bottomflangeglobalPos = new G4ThreeVector(bottomflangeaffine->NetTranslation());
  G4RotationMatrix *bottomflangeglobalRot= new G4RotationMatrix(bottomflangeaffine->NetRotation());
  fBottomFlangePtr->Place(bottomflangeglobalPos, bottomflangeglobalRot, motherLogical);
  delete bottomflangelocalPos;
  delete bottomflangelocalRot;
  delete bottomflangeaffine;

  G4ThreeVector *topflangelocalPos = new G4ThreeVector(0, 0, 4.445*25.4*mm);
  G4RotationMatrix *topflangelocalRot = new G4RotationMatrix();
  topflangelocalRot->rotateY(pi);
  G4AffineTransform *topflangeaffine = new G4AffineTransform(topflangelocalRot,*topflangelocalPos);
  *topflangeaffine*=*assemAffine;
  new G4ThreeVector(topflangeaffine->NetTranslation());
  new G4RotationMatrix(topflangeaffine->NetRotation());
  //fTopFlangePtr->Place(topflangeglobalPos, topflangeglobalRot, motherLogical);
  delete topflangelocalPos;
  delete topflangelocalRot;
  delete topflangeaffine;

  G4ThreeVector *leftflangelocalPos = new G4ThreeVector(0, 4.445*25.4*mm, 0);
  G4RotationMatrix *leftflangelocalRot = new G4RotationMatrix();
  leftflangelocalRot->rotateX(pi/2);
  G4AffineTransform *leftflangeaffine = new G4AffineTransform(leftflangelocalRot,*leftflangelocalPos);
  *leftflangeaffine*=*assemAffine;
  G4ThreeVector *leftflangeglobalPos = new G4ThreeVector(leftflangeaffine->NetTranslation());
  G4RotationMatrix *leftflangeglobalRot= new G4RotationMatrix(leftflangeaffine->NetRotation());
  fLeftFlangePtr->Place(leftflangeglobalPos, leftflangeglobalRot, motherLogical);
  delete leftflangelocalPos;
  delete leftflangelocalRot;
  delete leftflangeaffine;

  G4ThreeVector *rightflangelocalPos = new G4ThreeVector(0, -4.445*25.4*mm, 0);
  G4RotationMatrix *rightflangelocalRot = new G4RotationMatrix();
  rightflangelocalRot->rotateX(-pi/2);
  G4AffineTransform *rightflangeaffine = new G4AffineTransform(rightflangelocalRot,*rightflangelocalPos);
  *rightflangeaffine*=*assemAffine;
  G4ThreeVector *rightflangeglobalPos = new G4ThreeVector(rightflangeaffine->NetTranslation());
  G4RotationMatrix *rightflangeglobalRot= new G4RotationMatrix(rightflangeaffine->NetRotation());
  fRightFlangePtr->Place(rightflangeglobalPos, rightflangeglobalRot, motherLogical);
  delete rightflangelocalPos;
  delete rightflangelocalRot;
  delete rightflangeaffine;

  G4ThreeVector *flangeshieldlocalPos = new G4ThreeVector(-3.599*25.4*mm, 0, 0);
  G4RotationMatrix *flangeshieldlocalRot = new G4RotationMatrix();
  flangeshieldlocalRot->rotateY(-pi/2);
  G4AffineTransform *flangeshieldaffine = new G4AffineTransform(flangeshieldlocalRot,*flangeshieldlocalPos);
  *flangeshieldaffine*=*assemAffine;
  G4ThreeVector *flangeshieldglobalPos = new G4ThreeVector(flangeshieldaffine->NetTranslation());
  G4RotationMatrix *flangeshieldglobalRot= new G4RotationMatrix(flangeshieldaffine->NetRotation());
  fFlangeShieldPtr->Place(flangeshieldglobalPos, flangeshieldglobalRot, motherLogical);
  delete flangeshieldlocalPos;
  delete flangeshieldlocalRot;
  delete flangeshieldaffine;
  
  delete assemAffine;
}
