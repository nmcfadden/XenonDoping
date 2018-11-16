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
// CLASS IMPLEMENTATION:  MJDemoSignalCableTakeupSubAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin: Coaxial with screw 1, coincident with bottom face of takeup
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
#include "mjdemonstrator/MJDemoSignalCableTakeupSubAssembly.hh"
#include "mjdemonstrator/MJDemoSignalCableTakeupLowBulkhead.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSignalCableTakeupSubAssembly::MJDemoSignalCableTakeupSubAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "SignalCableTakeupdwg")
{
  fTakeupPtr = new MJDemoSignalCableTakeupLowBulkhead(fName+"_Takeup_001", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);

}

MJDemoSignalCableTakeupSubAssembly::MJDemoSignalCableTakeupSubAssembly(const MJDemoSignalCableTakeupSubAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fTakeupPtr = new MJDemoSignalCableTakeupLowBulkhead(fName+"_Takeup_001", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);

}

MJDemoSignalCableTakeupSubAssembly::~MJDemoSignalCableTakeupSubAssembly()
{
    delete fTakeupPtr;
    delete fScrew01Ptr;
    delete fScrew02Ptr;

}

void MJDemoSignalCableTakeupSubAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.01*mm;

  G4ThreeVector *TakeuplocalPos = new G4ThreeVector(22.225*mm, 3.1735*mm, 3.175/2*mm);
  G4RotationMatrix *TakeuplocalRot = new G4RotationMatrix();
  TakeuplocalRot->rotateX(-pi/2);
  TakeuplocalRot->rotateY(0);
  G4AffineTransform *Takeupaffine = new G4AffineTransform(TakeuplocalRot,*TakeuplocalPos);
  *Takeupaffine*=*assemAffine;  
  G4ThreeVector *TakeupglobalPos = new G4ThreeVector(Takeupaffine->NetTranslation());
  G4RotationMatrix *TakeupglobalRot= new G4RotationMatrix(Takeupaffine->NetRotation());
  fTakeupPtr->Place(TakeupglobalPos, TakeupglobalRot, motherLogical);
  delete TakeuplocalPos;
  delete TakeuplocalRot;
  delete Takeupaffine;

  G4ThreeVector *Screw01localPos = new G4ThreeVector(0, 0, 3.175*mm+eps);
  G4RotationMatrix *Screw01localRot = new G4RotationMatrix();
  Screw01localRot->rotateZ(0);
  Screw01localRot->rotateX(0);
  G4AffineTransform *Screw01affine = new G4AffineTransform(Screw01localRot,*Screw01localPos);
  *Screw01affine*=*assemAffine;
  G4ThreeVector *Screw01globalPos = new G4ThreeVector(Screw01affine->NetTranslation());
  G4RotationMatrix *Screw01globalRot= new G4RotationMatrix(Screw01affine->NetRotation());
  fScrew01Ptr->Place(Screw01globalPos, Screw01globalRot, motherLogical);
  delete Screw01localPos;
  delete Screw01localRot;
  delete Screw01affine;
    
  G4ThreeVector *Screw02localPos = new G4ThreeVector(44.45*mm, 6.347*mm, 3.175*mm+eps);
  G4RotationMatrix *Screw02localRot = new G4RotationMatrix();
  Screw02localRot->rotateZ(0);
  Screw02localRot->rotateX(0);
  G4AffineTransform *Screw02affine = new G4AffineTransform(Screw02localRot,*Screw02localPos);
  *Screw02affine*=*assemAffine;
  G4ThreeVector *Screw02globalPos = new G4ThreeVector(Screw02affine->NetTranslation());
  G4RotationMatrix *Screw02globalRot= new G4RotationMatrix(Screw02affine->NetRotation());
  fScrew02Ptr->Place(Screw02globalPos, Screw02globalRot, motherLogical);
  delete Screw02localPos;
  delete Screw02localRot;
  delete Screw02affine;
 

  delete assemAffine;
}
