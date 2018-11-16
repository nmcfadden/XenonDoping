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
// CLASS IMPLEMENTATION:  MJDemoDummyAssem.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  crystal COM
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
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
#include "mjdemonstrator/MJDemo70mmCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoPuckBeGe.hh"
#include "mjdemonstrator/MJDemoHVRing70.hh"
#include "mjdemonstrator/MJDemoHVNut.hh"
#include "mjdemonstrator/MJDemoHollowHexRod.hh"
#include "mjdemonstrator/MJDemoCrystalInsulator.hh"
#include "mjdemonstrator/MJDemoCableGuide.hh"
#include "mjdemonstrator/MJDemoCrystalMountingPlate.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo70mmCrystalAssembly::MJDemo70mmCrystalAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-02-100")
{;}

MJDemo70mmCrystalAssembly::MJDemo70mmCrystalAssembly(const MJDemo70mmCrystalAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{;}

MJDemo70mmCrystalAssembly::~MJDemo70mmCrystalAssembly()
{;}

void MJDemo70mmCrystalAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  MJDemoPuckBeGe *crystal1 = new MJDemoPuckBeGe(fName+"_"+fSerialNumber+"_crystal",
                                                          "001", 70, 30);  
  G4ThreeVector *crystal1localPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *crystal1localRot = new G4RotationMatrix(0, pi, 0);
  G4AffineTransform *crystal1affine = new G4AffineTransform(crystal1localRot,*crystal1localPos);
  *crystal1affine*=*assemAffine;  
  G4ThreeVector *crystal1globalPos = new G4ThreeVector(crystal1affine->NetTranslation());
  G4RotationMatrix *crystal1globalRot= new G4RotationMatrix(crystal1affine->NetRotation());
  crystal1->Place(crystal1globalPos, crystal1globalRot, motherLogical);
  delete crystal1localPos;
  delete crystal1localRot;
  delete crystal1affine;
  
  
  MJDemoHVRing70 *HVRing1 = new MJDemoHVRing70(fName+"_"+fSerialNumber+"_HVRing", "001");  
  G4double HVRingZposition = -10.174*mm;
  G4ThreeVector *HVRing1localPos = new G4ThreeVector(0, 0, HVRingZposition);
  G4RotationMatrix *HVRing1localRot = new G4RotationMatrix();
  G4AffineTransform *HVRing1affine = new G4AffineTransform(HVRing1localRot,*HVRing1localPos);
  *HVRing1affine*=*assemAffine;  
  G4ThreeVector *HVRing1globalPos = new G4ThreeVector(HVRing1affine->NetTranslation());
  G4RotationMatrix *HVRing1globalRot= new G4RotationMatrix(HVRing1affine->NetRotation());
  HVRing1->Place(HVRing1globalPos, HVRing1globalRot, motherLogical);
  delete HVRing1localPos;
  delete HVRing1localRot;
  delete HVRing1affine;
  
  
  MJDemoHVNut *HVNut1 = new MJDemoHVNut(fName+"_"+fSerialNumber+"_HVNut", "001");  
  G4double HVNutZposition = HVRingZposition - .06*25.4*mm;
  G4ThreeVector *HVNut1localPos = new G4ThreeVector(1.7*25.4*mm, 0, HVNutZposition);
  G4RotationMatrix *HVNut1localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut1affine = new G4AffineTransform(HVNut1localRot,*HVNut1localPos);
  *HVNut1affine*=*assemAffine;  
  G4ThreeVector *HVNut1globalPos = new G4ThreeVector(HVNut1affine->NetTranslation());
  G4RotationMatrix *HVNut1globalRot= new G4RotationMatrix(HVNut1affine->NetRotation());
  HVNut1->Place(HVNut1globalPos, HVNut1globalRot, motherLogical);
  delete HVNut1localPos;
  delete HVNut1localRot;
  delete HVNut1affine;
  
  MJDemoHVNut *HVNut2 = new MJDemoHVNut(fName+"_"+fSerialNumber+"_HVNut", "002");  
  G4ThreeVector *HVNut2localPos = new G4ThreeVector(
  					1.7*cos(120*deg)*25.4*mm, 1.7*sin(120*deg)*25.4*mm, HVNutZposition);
  G4RotationMatrix *HVNut2localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut2affine = new G4AffineTransform(HVNut2localRot,*HVNut2localPos);
  *HVNut2affine*=*assemAffine;  
  G4ThreeVector *HVNut2globalPos = new G4ThreeVector(HVNut2affine->NetTranslation());
  G4RotationMatrix *HVNut2globalRot= new G4RotationMatrix(HVNut2affine->NetRotation());
  HVNut2->Place(HVNut2globalPos, HVNut2globalRot, motherLogical);
  delete HVNut2localPos;
  delete HVNut2localRot;
  delete HVNut2affine;
  
  MJDemoHVNut *HVNut3 = new MJDemoHVNut(fName+"_"+fSerialNumber+"_HVNut", "003");  
  G4ThreeVector *HVNut3localPos = new G4ThreeVector(
  					1.7*cos(120*deg)*25.4*mm, -1.7*sin(120*deg)*25.4*mm, HVNutZposition);
  G4RotationMatrix *HVNut3localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut3affine = new G4AffineTransform(HVNut3localRot,*HVNut3localPos);
  *HVNut3affine*=*assemAffine;  
  G4ThreeVector *HVNut3globalPos = new G4ThreeVector(HVNut3affine->NetTranslation());
  G4RotationMatrix *HVNut3globalRot= new G4RotationMatrix(HVNut3affine->NetRotation());
  HVNut3->Place(HVNut3globalPos, HVNut3globalRot, motherLogical);
  delete HVNut3localPos;
  delete HVNut3localRot;
  delete HVNut3affine;
  
 
  MJDemoHollowHexRod *HollowHexRod1 = new MJDemoHollowHexRod(fName+"_"+fSerialNumber+
                                                              "_HollowHexRod", "001");  
  G4double HollowHexRodZposition = HVRingZposition + 38.556*mm;
  G4ThreeVector *HollowHexRod1localPos = new G4ThreeVector(1.7*25.4*mm, 0, HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod1localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod1affine = new G4AffineTransform(HollowHexRod1localRot, 
                                                                 *HollowHexRod1localPos);
  *HollowHexRod1affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod1globalPos = new G4ThreeVector(HollowHexRod1affine->NetTranslation());
  G4RotationMatrix *HollowHexRod1globalRot= new G4RotationMatrix(
                                                               HollowHexRod1affine->NetRotation());
  HollowHexRod1->Place(HollowHexRod1globalPos, HollowHexRod1globalRot, motherLogical);
  delete HollowHexRod1localPos;
  delete HollowHexRod1localRot;
  delete HollowHexRod1affine;
  
  MJDemoHollowHexRod *HollowHexRod2 = new MJDemoHollowHexRod(fName+"_"+fSerialNumber+
                                                              "_HollowHexRod", "002");  
  G4ThreeVector *HollowHexRod2localPos = new G4ThreeVector(
                       	1.7*cos(120*deg)*25.4*mm, 1.7*sin(120*deg)*25.4*mm, HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod2localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod2affine = new G4AffineTransform(HollowHexRod2localRot,
                                                                 *HollowHexRod2localPos);
  *HollowHexRod2affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod2globalPos = new G4ThreeVector(HollowHexRod2affine->NetTranslation());
  G4RotationMatrix *HollowHexRod2globalRot= new G4RotationMatrix(
                                                               HollowHexRod2affine->NetRotation());
  HollowHexRod2->Place(HollowHexRod2globalPos, HollowHexRod2globalRot, motherLogical);
  delete HollowHexRod2localPos;
  delete HollowHexRod2localRot;
  delete HollowHexRod2affine;
  
  MJDemoHollowHexRod *HollowHexRod3 = new MJDemoHollowHexRod(fName+"_"+fSerialNumber+
                                                              "_HollowHexRod", "003");  
  G4ThreeVector *HollowHexRod3localPos = new G4ThreeVector(
  						1.7*cos(120*deg)*25.4*mm, -1.7*sin(120*deg)*25.4*mm, HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod3localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod3affine = new G4AffineTransform(HollowHexRod3localRot,
                                                                 *HollowHexRod3localPos);
  *HollowHexRod3affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod3globalPos = new G4ThreeVector(HollowHexRod3affine->NetTranslation());
  G4RotationMatrix *HollowHexRod3globalRot= new G4RotationMatrix(
                                                               HollowHexRod3affine->NetRotation());
  HollowHexRod3->Place(HollowHexRod3globalPos, HollowHexRod3globalRot, motherLogical);
  delete HollowHexRod3localPos;
  delete HollowHexRod3localRot;
  delete HollowHexRod3affine;
 
 
  MJDemoCrystalInsulator *CrystalInsulator1 = new MJDemoCrystalInsulator(                                            
                                               fName+"_"+fSerialNumber+"_CrystalInsulator", "001");  
  G4double CrystalInsulatorZposition = 15*mm;
  G4ThreeVector *CrystalInsulator1localPos = new G4ThreeVector(1.35*25.4*mm, 0, 
  															   CrystalInsulatorZposition);
  G4RotationMatrix *CrystalInsulator1localRot = new G4RotationMatrix();
  G4AffineTransform *CrystalInsulator1affine = new G4AffineTransform(CrystalInsulator1localRot,
                                                                     *CrystalInsulator1localPos);
  *CrystalInsulator1affine*=*assemAffine;  
  G4ThreeVector *CrystalInsulator1globalPos = new G4ThreeVector(
                                                      CrystalInsulator1affine->NetTranslation());
  G4RotationMatrix *CrystalInsulator1globalRot= new G4RotationMatrix(
                                                      CrystalInsulator1affine->NetRotation());
  CrystalInsulator1->Place(CrystalInsulator1globalPos, CrystalInsulator1globalRot, motherLogical);
  delete CrystalInsulator1localPos;
  delete CrystalInsulator1localRot;
  delete CrystalInsulator1affine;
  
  MJDemoCrystalInsulator *CrystalInsulator2 = new MJDemoCrystalInsulator(
                                               fName+"_"+fSerialNumber+"_CrystalInsulator", "002");  
  G4ThreeVector *CrystalInsulator2localPos = new G4ThreeVector(-0.675*25.4*mm, 1.169*25.4*mm, 
                                                                CrystalInsulatorZposition);
  G4RotationMatrix *CrystalInsulator2localRot = new G4RotationMatrix();
  CrystalInsulator2localRot->rotateZ(-120*deg);
  G4AffineTransform *CrystalInsulator2affine = new G4AffineTransform(CrystalInsulator2localRot,
                                                                     *CrystalInsulator2localPos);
  *CrystalInsulator2affine*=*assemAffine;  
  G4ThreeVector *CrystalInsulator2globalPos = new G4ThreeVector(
                                                      CrystalInsulator2affine->NetTranslation());
  G4RotationMatrix *CrystalInsulator2globalRot= new G4RotationMatrix(
                                                      CrystalInsulator2affine->NetRotation());
  CrystalInsulator2->Place(CrystalInsulator2globalPos, CrystalInsulator2globalRot, motherLogical);
  delete CrystalInsulator2localPos;
  delete CrystalInsulator2localRot;
  delete CrystalInsulator2affine;
  
  MJDemoCrystalInsulator *CrystalInsulator3 = new MJDemoCrystalInsulator(
                                               fName+"_"+fSerialNumber+"_CrystalInsulator", "003");  
  G4ThreeVector *CrystalInsulator3localPos = new G4ThreeVector(-0.675*25.4*mm, -1.169*25.4*mm, 
                                                                CrystalInsulatorZposition);
  G4RotationMatrix *CrystalInsulator3localRot = new G4RotationMatrix();
  CrystalInsulator3localRot->rotateZ(-240*deg);
  G4AffineTransform *CrystalInsulator3affine = new G4AffineTransform(CrystalInsulator3localRot,
                                                                     *CrystalInsulator3localPos);
  *CrystalInsulator3affine*=*assemAffine;  
  G4ThreeVector *CrystalInsulator3globalPos = new G4ThreeVector(
                                                      CrystalInsulator3affine->NetTranslation());
  G4RotationMatrix *CrystalInsulator3globalRot= new G4RotationMatrix(
                                                      CrystalInsulator3affine->NetRotation());
  CrystalInsulator3->Place(CrystalInsulator3globalPos, CrystalInsulator3globalRot, motherLogical);
  delete CrystalInsulator3localPos;
  delete CrystalInsulator3localRot;
  delete CrystalInsulator3affine;
  

  MJDemoCableGuide *CableGuide1 = new MJDemoCableGuide(fName+"_"+fSerialNumber+"_CableGuide", 
                                                       "001");  
  G4double CableGuideZposition = 19.213*mm;
  G4ThreeVector *CableGuide1localPos = new G4ThreeVector(1.7*25.4*mm, 0.2*25.4*mm, 
                                                         CableGuideZposition);
  G4RotationMatrix *CableGuide1localRot = new G4RotationMatrix();
  CableGuide1localRot->rotateY(pi);
  G4AffineTransform *CableGuide1affine = new G4AffineTransform(CableGuide1localRot,
                                                                     *CableGuide1localPos);
  *CableGuide1affine*=*assemAffine;  
  G4ThreeVector *CableGuide1globalPos = new G4ThreeVector(CableGuide1affine->NetTranslation());
  G4RotationMatrix *CableGuide1globalRot= new G4RotationMatrix(CableGuide1affine->NetRotation());
  CableGuide1->Place(CableGuide1globalPos, CableGuide1globalRot, motherLogical);
  delete CableGuide1localPos;
  delete CableGuide1localRot;
  delete CableGuide1affine;
  
  MJDemoCableGuide *CableGuide2 = new MJDemoCableGuide(fName+"_"+fSerialNumber+"_CableGuide", 
                                                       "002");  
  G4ThreeVector *CableGuide2localPos = new G4ThreeVector(-1.023*25.4*mm, 1.372*25.4*mm, 
                                                                CableGuideZposition);
  G4RotationMatrix *CableGuide2localRot = new G4RotationMatrix();
  CableGuide2localRot->rotateY(pi);
  CableGuide2localRot->rotateZ(120*deg);
  G4AffineTransform *CableGuide2affine = new G4AffineTransform(CableGuide2localRot,
                                                                     *CableGuide2localPos);
  *CableGuide2affine*=*assemAffine;  
  G4ThreeVector *CableGuide2globalPos = new G4ThreeVector(CableGuide2affine->NetTranslation());
  G4RotationMatrix *CableGuide2globalRot= new G4RotationMatrix(CableGuide2affine->NetRotation());
  CableGuide2->Place(CableGuide2globalPos, CableGuide2globalRot, motherLogical);
  delete CableGuide2localPos;
  delete CableGuide2localRot;
  delete CableGuide2affine;
  
  MJDemoCableGuide *CableGuide3 = new MJDemoCableGuide(fName+"_"+fSerialNumber+"_CableGuide", 
                                                       "003");  
  G4ThreeVector *CableGuide3localPos = new G4ThreeVector(-0.677*25.4*mm, -1.572*25.4*mm, 
                                                                CableGuideZposition);
  G4RotationMatrix *CableGuide3localRot = new G4RotationMatrix();
  CableGuide3localRot->rotateY(pi);
  CableGuide3localRot->rotateZ(240*deg);
  G4AffineTransform *CableGuide3affine = new G4AffineTransform(CableGuide3localRot,
                                                                     *CableGuide3localPos);
  *CableGuide3affine*=*assemAffine;  
  G4ThreeVector *CableGuide3globalPos = new G4ThreeVector(CableGuide3affine->NetTranslation());
  G4RotationMatrix *CableGuide3globalRot= new G4RotationMatrix(CableGuide3affine->NetRotation());
  CableGuide3->Place(CableGuide3globalPos, CableGuide3globalRot, motherLogical);
  delete CableGuide3localPos;
  delete CableGuide3localRot;
  delete CableGuide3affine;

  MJDemoCrystalMountingPlate *CrystalMountingPlate = new MJDemoCrystalMountingPlate(
 										fName+"_"+fSerialNumber+"_CrystalMountingPlate", "001");  
  G4ThreeVector *CrystalMountingPlatelocalPos = new G4ThreeVector(0, 0, ((0.125*25.4)+20)*mm);
  G4RotationMatrix *CrystalMountingPlatelocalRot = new G4RotationMatrix();
  G4AffineTransform *CrystalMountingPlateaffine = new G4AffineTransform(
  									CrystalMountingPlatelocalRot,*CrystalMountingPlatelocalPos);
  *CrystalMountingPlateaffine*=*assemAffine;  
  G4ThreeVector *CrystalMountingPlateglobalPos = new G4ThreeVector(
  													CrystalMountingPlateaffine->NetTranslation());
  G4RotationMatrix *CrystalMountingPlateglobalRot= new G4RotationMatrix(
  													CrystalMountingPlateaffine->NetRotation());
  CrystalMountingPlate->Place(CrystalMountingPlateglobalPos, 
  							  CrystalMountingPlateglobalRot, motherLogical);
  delete CrystalMountingPlatelocalPos;
  delete CrystalMountingPlatelocalRot;
  delete CrystalMountingPlateaffine;
  delete assemAffine;
}


