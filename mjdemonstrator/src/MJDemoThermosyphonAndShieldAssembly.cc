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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonAndShieldAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Assembly origin:  Center of top face of coldplate.
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: Sept 07, 2014
 *
 * REVISION:
 *
 * 09-07-2014, Created, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonAndShieldAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonAssembly.hh"
#include "mjdemonstrator/MJDemoBottomThermalShieldAssembly.hh"
#include "mjdemonstrator/MJDemoTopThermalShieldAssembly.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonAndShieldAssembly::MJDemoThermosyphonAndShieldAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "TS&Shielddwg")
{
  fTSPtr = new MJDemoThermosyphonAssembly(fName+"_ThermosyphonAssem_001", serialNumber);
  fBShieldPtr = new MJDemoBottomThermalShieldAssembly(fName+"_BottomShield_001", "867-5309");
  fTShieldPtr = new MJDemoTopThermalShieldAssembly(fName+"_TopShield_001", "867-5309");
}

MJDemoThermosyphonAndShieldAssembly::MJDemoThermosyphonAndShieldAssembly(const MJDemoThermosyphonAndShieldAssembly & rhs) :
  MJVDemoAssembly(rhs)
{
  fTSPtr = new MJDemoThermosyphonAssembly(fName+"_ThermosyphonAssem_001", rhs.GetThermosyphon()->GetSerialNumber());
  fBShieldPtr = new MJDemoBottomThermalShieldAssembly(fName+"_BottomShield_001", "867-5309");
  fTShieldPtr = new MJDemoTopThermalShieldAssembly(fName+"_TopShield_001", "867-5309");
}

MJDemoThermosyphonAndShieldAssembly::~MJDemoThermosyphonAndShieldAssembly()
{
  delete fTSPtr;
  delete fBShieldPtr;
  delete fTShieldPtr;

}

void MJDemoThermosyphonAndShieldAssembly::Place(G4ThreeVector* assemPosition,
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.01*mm;

  G4ThreeVector *TSlocalPos = new G4ThreeVector(-32.258*mm, 0, 21.59+eps);
  G4RotationMatrix *TSlocalRot = new G4RotationMatrix();
  TSlocalRot->rotateX(0);
  TSlocalRot->rotateY(0);
  G4AffineTransform *TSaffine = new G4AffineTransform(TSlocalRot,*TSlocalPos);
  *TSaffine*=*assemAffine;
  G4ThreeVector *TSglobalPos = new G4ThreeVector(TSaffine->NetTranslation());
  G4RotationMatrix *TSglobalRot= new G4RotationMatrix(TSaffine->NetRotation());
  fTSPtr->Place(TSglobalPos, TSglobalRot, motherLogical);
  delete TSlocalPos;
  delete TSlocalRot;
  delete TSaffine;

  G4ThreeVector *fBShieldlocalPos = new G4ThreeVector(0, 0, -16*mm);
  G4RotationMatrix *fBShieldlocalRot = new G4RotationMatrix();
  fBShieldlocalRot->rotateX(0);
  fBShieldlocalRot->rotateZ(pi);
  G4AffineTransform *fBShieldaffine = new G4AffineTransform(fBShieldlocalRot,*fBShieldlocalPos);
  *fBShieldaffine*=*assemAffine;
  G4ThreeVector *fBShieldglobalPos = new G4ThreeVector(fBShieldaffine->NetTranslation());
  G4RotationMatrix *fBShieldglobalRot= new G4RotationMatrix(fBShieldaffine->NetRotation());
  fBShieldPtr->Place(fBShieldglobalPos, fBShieldglobalRot, motherLogical);
  delete fBShieldlocalPos;
  delete fBShieldlocalRot;
  delete fBShieldaffine;

  G4ThreeVector *fTShieldlocalPos = new G4ThreeVector(0, 0, 1*mm+2*eps);
  G4RotationMatrix *fTShieldlocalRot = new G4RotationMatrix();
  fTShieldlocalRot->rotateX(0);
  fTShieldlocalRot->rotateY(0);
  G4AffineTransform *fTShieldaffine = new G4AffineTransform(fTShieldlocalRot,*fTShieldlocalPos);
  *fTShieldaffine*=*assemAffine;
  G4ThreeVector *fTShieldglobalPos = new G4ThreeVector(fTShieldaffine->NetTranslation());
  G4RotationMatrix *fTShieldglobalRot= new G4RotationMatrix(fTShieldaffine->NetRotation());
  fTShieldPtr->Place(fTShieldglobalPos, fTShieldglobalRot, motherLogical);
  delete fTShieldlocalPos;
  delete fTShieldlocalRot;
  delete fTShieldaffine;

  delete assemAffine;
}
