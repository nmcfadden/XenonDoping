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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonCableThermalShuntAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Co-axial with thermosyphon tube, center of cylinder
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 *
 * REVISION:
 *
 * 07-30-2014, Created, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntHalf.hh"
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntClip.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonCableThermalShuntAssembly::MJDemoThermosyphonCableThermalShuntAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-11-142-R001")
{
  fFemaleHalfPtr = new MJDemoThermosyphonCableThermalShuntHalf(fName+"_FemaleHalf_001", "867-5309");
  fMaleHalfPtr = new MJDemoThermosyphonCableThermalShuntHalf(fName+"_MaleHalf_001", "867-5309");
  fClip01Ptr = new MJDemoThermosyphonCableThermalShuntClip(fName+"_Clip_001", "867-5309");
  fClip02Ptr = new MJDemoThermosyphonCableThermalShuntClip(fName+"_Clip_002", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fScrew03Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 6.35);
  fScrew04Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_004", "867-5309", 6.35);
}

MJDemoThermosyphonCableThermalShuntAssembly::MJDemoThermosyphonCableThermalShuntAssembly(const MJDemoThermosyphonCableThermalShuntAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fFemaleHalfPtr = new MJDemoThermosyphonCableThermalShuntHalf(fName+"_FemaleHalf_001", "867-5309");
  fMaleHalfPtr = new MJDemoThermosyphonCableThermalShuntHalf(fName+"_MaleHalf_001", "867-5309");
  fClip01Ptr = new MJDemoThermosyphonCableThermalShuntClip(fName+"_Clip_001", "867-5309");
  fClip02Ptr = new MJDemoThermosyphonCableThermalShuntClip(fName+"_Clip_002", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fScrew03Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 6.35);
  fScrew04Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_004", "867-5309", 6.35);
}

MJDemoThermosyphonCableThermalShuntAssembly::~MJDemoThermosyphonCableThermalShuntAssembly()
{
  delete fFemaleHalfPtr;
  delete fMaleHalfPtr;
  delete fClip01Ptr;
  delete fClip02Ptr;
  delete fScrew01Ptr;
  delete fScrew02Ptr;
  delete fScrew03Ptr;
  delete fScrew04Ptr;
}

void MJDemoThermosyphonCableThermalShuntAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.001*mm;

  G4ThreeVector *FemaleHalflocalPos = new G4ThreeVector(0, eps, 0);
  G4RotationMatrix *FemaleHalflocalRot = new G4RotationMatrix();
  FemaleHalflocalRot->rotateZ(0*deg);
  G4AffineTransform *FemaleHalfaffine = new G4AffineTransform(FemaleHalflocalRot,*FemaleHalflocalPos);
  *FemaleHalfaffine*=*assemAffine;  
  G4ThreeVector *FemaleHalfglobalPos = new G4ThreeVector(FemaleHalfaffine->NetTranslation());
  G4RotationMatrix *FemaleHalfglobalRot= new G4RotationMatrix(FemaleHalfaffine->NetRotation());
  fFemaleHalfPtr->Place(FemaleHalfglobalPos, FemaleHalfglobalRot, motherLogical);
  delete FemaleHalflocalPos;
  delete FemaleHalflocalRot;
  delete FemaleHalfaffine;
    
  G4ThreeVector *MaleHalflocalPos = new G4ThreeVector(0, -eps, 0);
  G4RotationMatrix *MaleHalflocalRot = new G4RotationMatrix();
  MaleHalflocalRot->rotateZ(180*deg);
  G4AffineTransform *MaleHalfaffine = new G4AffineTransform(MaleHalflocalRot,*MaleHalflocalPos);
  *MaleHalfaffine*=*assemAffine;
  G4ThreeVector *MaleHalfglobalPos = new G4ThreeVector(MaleHalfaffine->NetTranslation());
  G4RotationMatrix *MaleHalfglobalRot= new G4RotationMatrix(MaleHalfaffine->NetRotation());
  fMaleHalfPtr->Place(MaleHalfglobalPos, MaleHalfglobalRot, motherLogical);
  delete MaleHalflocalPos;
  delete MaleHalflocalRot;
  delete MaleHalfaffine;
    
  G4ThreeVector *Clip01localPos = new G4ThreeVector(eps, 0, -3.94*mm);
  G4RotationMatrix *Clip01localRot = new G4RotationMatrix();
  Clip01localRot->rotateZ(90*deg);
  G4AffineTransform *Clip01affine = new G4AffineTransform(Clip01localRot,*Clip01localPos);
  *Clip01affine*=*assemAffine;
  G4ThreeVector *Clip01globalPos = new G4ThreeVector(Clip01affine->NetTranslation());
  G4RotationMatrix *Clip01globalRot= new G4RotationMatrix(Clip01affine->NetRotation());
  fClip01Ptr->Place(Clip01globalPos, Clip01globalRot, motherLogical);
  delete Clip01localPos;
  delete Clip01localRot;
  delete Clip01affine;
  
  G4ThreeVector *Clip02localPos = new G4ThreeVector(-eps, 0, -3.94*mm);
  G4RotationMatrix *Clip02localRot = new G4RotationMatrix();
  Clip02localRot->rotateZ(-90*deg);
  G4AffineTransform *Clip02affine = new G4AffineTransform(Clip02localRot,*Clip02localPos);
  *Clip02affine*=*assemAffine;
  G4ThreeVector *Clip02globalPos = new G4ThreeVector(Clip02affine->NetTranslation());
  G4RotationMatrix *Clip02globalRot= new G4RotationMatrix(Clip02affine->NetRotation());
  fClip02Ptr->Place(Clip02globalPos, Clip02globalRot, motherLogical);
  delete Clip02localPos;
  delete Clip02localRot;
  delete Clip02affine;

  G4ThreeVector *Screw01localPos = new G4ThreeVector(20.542*mm, 4.742*mm+eps, -4.7*mm);
  G4RotationMatrix *Screw01localRot = new G4RotationMatrix();
  Screw01localRot->rotateX(180*deg);
  G4AffineTransform *Screw01affine = new G4AffineTransform(Screw01localRot,*Screw01localPos);
  *Screw01affine*=*assemAffine;
  G4ThreeVector *Screw01globalPos = new G4ThreeVector(Screw01affine->NetTranslation());
  G4RotationMatrix *Screw01globalRot= new G4RotationMatrix(Screw01affine->NetRotation());
  fScrew01Ptr->Place(Screw01globalPos, Screw01globalRot, motherLogical);
  delete Screw01localPos;
  delete Screw01localRot;
  delete Screw01affine;

  G4ThreeVector *Screw02localPos = new G4ThreeVector(20.542*mm, -4.742*mm-eps, -4.7*mm);
  G4RotationMatrix *Screw02localRot = new G4RotationMatrix();
  Screw02localRot->rotateX(180*deg);
  G4AffineTransform *Screw02affine = new G4AffineTransform(Screw02localRot,*Screw02localPos);
  *Screw02affine*=*assemAffine;
  G4ThreeVector *Screw02globalPos = new G4ThreeVector(Screw02affine->NetTranslation());
  G4RotationMatrix *Screw02globalRot= new G4RotationMatrix(Screw02affine->NetRotation());
  fScrew02Ptr->Place(Screw02globalPos, Screw02globalRot, motherLogical);
  delete Screw02localPos;
  delete Screw02localRot;
  delete Screw02affine;
    
  G4ThreeVector *Screw03localPos = new G4ThreeVector(-20.542*mm, 4.742*mm+eps, -4.7*mm);
  G4RotationMatrix *Screw03localRot = new G4RotationMatrix();
  Screw03localRot->rotateX(180*deg);
  G4AffineTransform *Screw03affine = new G4AffineTransform(Screw03localRot,*Screw03localPos);
  *Screw03affine*=*assemAffine;
  G4ThreeVector *Screw03globalPos = new G4ThreeVector(Screw03affine->NetTranslation());
  G4RotationMatrix *Screw03globalRot= new G4RotationMatrix(Screw03affine->NetRotation());
  fScrew03Ptr->Place(Screw03globalPos, Screw03globalRot, motherLogical);
  delete Screw03localPos;
  delete Screw03localRot;
  delete Screw03affine;
    
  G4ThreeVector *Screw04localPos = new G4ThreeVector(-20.542*mm, -4.742*mm-eps, -4.7*mm);
  G4RotationMatrix *Screw04localRot = new G4RotationMatrix();
  Screw04localRot->rotateX(180*deg);
  G4AffineTransform *Screw04affine = new G4AffineTransform(Screw04localRot,*Screw04localPos);
  *Screw04affine*=*assemAffine;
  G4ThreeVector *Screw04globalPos = new G4ThreeVector(Screw04affine->NetTranslation());
  G4RotationMatrix *Screw04globalRot= new G4RotationMatrix(Screw04affine->NetRotation());
  fScrew04Ptr->Place(Screw04globalPos, Screw04globalRot, motherLogical);
  delete Screw04localPos;
  delete Screw04localRot;
  delete Screw04affine;
    
  delete assemAffine;
}
