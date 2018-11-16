//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJSTCColdPlateAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Assembly Origin: The middle of the width (z) of the Coldplate
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION:
 * 10-17-2013, Created, J. MacMullin
 * 11-14-2013, Added adapter nuts for attaching to adapter plate, J. MacMullin
 * 01-21-2014, Added ability to rotate ColdPlate and CMS from messenger. J. MacMullin
 *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCColdPlateAssembly.hh"
#include "mjdemonstrator/MJSTCColdPlateBody.hh"
#include "mjdemonstrator/MJDemo1032Nut.hh"
#include "mjdemonstrator/MJSTCColdPlateShuntStrap.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCColdPlateAssembly::MJSTCColdPlateAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "ColdPlate")
{
  fBodyPtr = new MJSTCColdPlateBody(fName+"_Body_001", "867-5309");
  fAdapterNut01Ptr = new MJDemo1032Nut(fName+"_AdapterNut01_001", "867-5309");
  fAdapterNut02Ptr = new MJDemo1032Nut(fName+"_AdapterNut01_002", "867-5309");
  fAdapterNut03Ptr = new MJDemo1032Nut(fName+"_AdapterNut01_003", "867-5309");
  fShuntStrap01Ptr = new MJSTCColdPlateShuntStrap(fName+"_ShuntStrap_001", "867-5309");
  fShuntStrap02Ptr = new MJSTCColdPlateShuntStrap(fName+"_ShuntStrap_002", "867-5309");
}

MJSTCColdPlateAssembly::MJSTCColdPlateAssembly(const MJSTCColdPlateAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fBodyPtr = new MJSTCColdPlateBody(fName+"_Body_001", "867-5309");
  fAdapterNut01Ptr = new MJDemo1032Nut(fName+"_AdapterNut_001", "867-5309");
  fAdapterNut02Ptr = new MJDemo1032Nut(fName+"_AdapterNut_002", "867-5309");
  fAdapterNut03Ptr = new MJDemo1032Nut(fName+"_AdapterNut_003", "867-5309");
  fShuntStrap01Ptr = new MJSTCColdPlateShuntStrap(fName+"_ShuntStrap_001", "867-5309");
  fShuntStrap02Ptr = new MJSTCColdPlateShuntStrap(fName+"_ShuntStrap_002", "867-5309");
}

MJSTCColdPlateAssembly::~MJSTCColdPlateAssembly()
{
  delete fBodyPtr;
  delete fAdapterNut01Ptr;
  delete fAdapterNut02Ptr;
  delete fAdapterNut03Ptr;
  delete fShuntStrap01Ptr;
  delete fShuntStrap02Ptr;
}

void MJSTCColdPlateAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *bodylocalPos = new G4ThreeVector(0*mm, 0*mm, 0*mm);
  G4RotationMatrix *bodylocalRot = new G4RotationMatrix();
  G4AffineTransform *bodyaffine = new G4AffineTransform(bodylocalRot,*bodylocalPos);
  *bodyaffine*=*assemAffine;  
  G4ThreeVector *bodyglobalPos = new G4ThreeVector(bodyaffine->NetTranslation());
  G4RotationMatrix *bodyglobalRot= new G4RotationMatrix(bodyaffine->NetRotation());
  fBodyPtr->Place(bodyglobalPos, bodyglobalRot, motherLogical);
  delete bodylocalPos;
  delete bodylocalRot;
  delete bodyaffine;
  
  G4ThreeVector *adapterNut01localPos = new G4ThreeVector(19.05*cos(90*deg)*mm, 19.05*sin(90*deg)*mm, 3.175*mm+eps);
  G4RotationMatrix *adapterNut01localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut01affine = new G4AffineTransform(adapterNut01localRot,*adapterNut01localPos);
  *adapterNut01affine*=*assemAffine;  
  G4ThreeVector *adapterNut01globalPos = new G4ThreeVector(adapterNut01affine->NetTranslation());
  G4RotationMatrix *adapterNut01globalRot= new G4RotationMatrix(adapterNut01affine->NetRotation());
  fAdapterNut01Ptr->Place(adapterNut01globalPos, adapterNut01globalRot, motherLogical);
  delete adapterNut01localPos;
  delete adapterNut01localRot;
  delete adapterNut01affine;

  G4ThreeVector *adapterNut02localPos = new G4ThreeVector(19.05*cos(210*deg)*mm, 19.05*sin(210*deg)*mm, 3.175*mm+eps);
  G4RotationMatrix *adapterNut02localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut02affine = new G4AffineTransform(adapterNut02localRot,*adapterNut02localPos);
  *adapterNut02affine*=*assemAffine;  
  G4ThreeVector *adapterNut02globalPos = new G4ThreeVector(adapterNut02affine->NetTranslation());
  G4RotationMatrix *adapterNut02globalRot= new G4RotationMatrix(adapterNut02affine->NetRotation());
  fAdapterNut02Ptr->Place(adapterNut02globalPos, adapterNut02globalRot, motherLogical);
  delete adapterNut02localPos;
  delete adapterNut02localRot;
  delete adapterNut02affine;
  
  G4ThreeVector *adapterNut03localPos = new G4ThreeVector(19.05*cos(330*deg)*mm, 19.05*sin(330*deg)*mm, 3.175*mm+eps);
  G4RotationMatrix *adapterNut03localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut03affine = new G4AffineTransform(adapterNut03localRot,*adapterNut03localPos);
  *adapterNut03affine*=*assemAffine;  
  G4ThreeVector *adapterNut03globalPos = new G4ThreeVector(adapterNut03affine->NetTranslation());
  G4RotationMatrix *adapterNut03globalRot= new G4RotationMatrix(adapterNut03affine->NetRotation());
  fAdapterNut03Ptr->Place(adapterNut03globalPos, adapterNut03globalRot, motherLogical);
  delete adapterNut03localPos;
  delete adapterNut03localRot;
  delete adapterNut03affine;
  
  //I don't know how high the shunt straps are raised off of the cold plate, so the 2 mm of the z position is a guess.
  //To have the correct z position I would need the size of the bolts that are used to secure the shunt straps to the cold plate.
  G4ThreeVector *shuntstrap01localPos = new G4ThreeVector((33.731+(3.302-3.175)*sin(pi/12.0)+(9.525-3.302)*cos(pi/12.0))*mm, -(9.0424+(3.302-3.175)*cos(pi/12.0)+(9.525-3.302)*sin(pi/12.0))*mm, (3.175+3.175+2.0)*mm);
  G4RotationMatrix *shuntstrap01localRot = new G4RotationMatrix();
  shuntstrap01localRot->rotateZ(pi/12.0);
  G4AffineTransform *shuntstrap01affine = new G4AffineTransform(shuntstrap01localRot,*shuntstrap01localPos);
  *shuntstrap01affine*=*assemAffine;  
  G4ThreeVector *shuntstrap01globalPos = new G4ThreeVector(shuntstrap01affine->NetTranslation());
  G4RotationMatrix *shuntstrap01globalRot= new G4RotationMatrix(shuntstrap01affine->NetRotation());
  fShuntStrap01Ptr->Place(shuntstrap01globalPos, shuntstrap01globalRot, motherLogical);
  delete shuntstrap01localPos;
  delete shuntstrap01localRot;
  delete shuntstrap01affine;
 
  G4ThreeVector *shuntstrap02localPos = new G4ThreeVector((9.0424+(3.302-3.175)*cos(pi/12.0)+(9.525-3.302)*sin(pi/12.0))*mm, (33.7312+(3.302-3.175)*sin(pi/12.0)+(9.525-3.302)*cos(pi/12.0))*mm, (3.175+3.175+2.0)*mm);
  G4RotationMatrix *shuntstrap02localRot = new G4RotationMatrix();
  shuntstrap02localRot->rotateZ(19.0*pi/12.0);
  G4AffineTransform *shuntstrap02affine = new G4AffineTransform(shuntstrap02localRot,*shuntstrap02localPos);
  *shuntstrap02affine*=*assemAffine;  
  G4ThreeVector *shuntstrap02globalPos = new G4ThreeVector(shuntstrap02affine->NetTranslation());
  G4RotationMatrix *shuntstrap02globalRot= new G4RotationMatrix(shuntstrap02affine->NetRotation());
  fShuntStrap02Ptr->Place(shuntstrap02globalPos, shuntstrap02globalRot, motherLogical);
  delete shuntstrap02localPos;
  delete shuntstrap02localRot;
  delete shuntstrap02affine;
  
  delete assemAffine;  
}
