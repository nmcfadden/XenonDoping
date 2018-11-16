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
// CLASS IMPLEMENTATION:  MJDemoCryostatRailAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  
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
#include "mjdemonstrator/MJDemoCryostatRailAssembly.hh"
#include "mjdemonstrator/MJDemoCryostatCenterRail.hh"
#include "mjdemonstrator/MJDemoCryostatLidRail.hh"
#include "mjdemonstrator/MJDemoCryostatBolt.hh"
#include "mjdemonstrator/MJDemoCryostatNut.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatRailAssembly::MJDemoCryostatRailAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "CryostatRails")
{
  fCenterRail1Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_001", "867-5309");
  fCenterRail2Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_002", "867-5309");
  fCenterRail3Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_003", "867-5309");
  fCenterRail4Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_004", "867-5309");
  fLidRail1Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_001", "867-5309");
  fLidRail2Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_002", "867-5309");  
  fLidRail3Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_003", "867-5309");
  fLidRail4Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_004", "867-5309");
  fBolt1Ptr = new MJDemoCryostatBolt(fName+"_Bolt_001", "867-5309");
  fBolt2Ptr = new MJDemoCryostatBolt(fName+"_Bolt_002", "867-5309");  
  fBolt3Ptr = new MJDemoCryostatBolt(fName+"_Bolt_003", "867-5309");  
  fBolt4Ptr = new MJDemoCryostatBolt(fName+"_Bolt_004", "867-5309");  
  fBolt5Ptr = new MJDemoCryostatBolt(fName+"_Bolt_005", "867-5309");  
  fBolt6Ptr = new MJDemoCryostatBolt(fName+"_Bolt_006", "867-5309");  
  fBolt7Ptr = new MJDemoCryostatBolt(fName+"_Bolt_007", "867-5309");  
  fBolt8Ptr = new MJDemoCryostatBolt(fName+"_Bolt_008", "867-5309");  
  fBolt9Ptr = new MJDemoCryostatBolt(fName+"_Bolt_009", "867-5309");  
  fBolt10Ptr = new MJDemoCryostatBolt(fName+"_Bolt_010", "867-5309");  
  fBolt11Ptr = new MJDemoCryostatBolt(fName+"_Bolt_011", "867-5309");  
  fBolt12Ptr = new MJDemoCryostatBolt(fName+"_Bolt_012", "867-5309");  
  fNut1Ptr = new MJDemoCryostatNut(fName+"_Nut_001", "867-5309");
  fNut2Ptr = new MJDemoCryostatNut(fName+"_Nut_002", "867-5309");
  fNut3Ptr = new MJDemoCryostatNut(fName+"_Nut_003", "867-5309");
  fNut4Ptr = new MJDemoCryostatNut(fName+"_Nut_004", "867-5309");
  fNut5Ptr = new MJDemoCryostatNut(fName+"_Nut_005", "867-5309");
  fNut6Ptr = new MJDemoCryostatNut(fName+"_Nut_006", "867-5309");
  fNut7Ptr = new MJDemoCryostatNut(fName+"_Nut_007", "867-5309");
  fNut8Ptr = new MJDemoCryostatNut(fName+"_Nut_008", "867-5309");
  fNut9Ptr = new MJDemoCryostatNut(fName+"_Nut_009", "867-5309");
  fNut10Ptr = new MJDemoCryostatNut(fName+"_Nut_010", "867-5309");
  fNut11Ptr = new MJDemoCryostatNut(fName+"_Nut_011", "867-5309");
  fNut12Ptr = new MJDemoCryostatNut(fName+"_Nut_012", "867-5309");
}

MJDemoCryostatRailAssembly::MJDemoCryostatRailAssembly(const MJDemoCryostatRailAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCenterRail1Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_001", "867-5309");
  fCenterRail2Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_002", "867-5309");
  fCenterRail3Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_003", "867-5309");
  fCenterRail4Ptr = new MJDemoCryostatCenterRail(fName+"_CenterRail_004", "867-5309");
  fLidRail1Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_001", "867-5309");
  fLidRail2Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_002", "867-5309");  
  fLidRail3Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_003", "867-5309");
  fLidRail4Ptr = new MJDemoCryostatLidRail(fName+"_LidRail_004", "867-5309");
  fBolt1Ptr = new MJDemoCryostatBolt(fName+"_Bolt_001", "867-5309");
  fBolt2Ptr = new MJDemoCryostatBolt(fName+"_Bolt_002", "867-5309");  
  fBolt3Ptr = new MJDemoCryostatBolt(fName+"_Bolt_003", "867-5309");  
  fBolt4Ptr = new MJDemoCryostatBolt(fName+"_Bolt_004", "867-5309");  
  fBolt5Ptr = new MJDemoCryostatBolt(fName+"_Bolt_005", "867-5309");  
  fBolt6Ptr = new MJDemoCryostatBolt(fName+"_Bolt_006", "867-5309");  
  fBolt7Ptr = new MJDemoCryostatBolt(fName+"_Bolt_007", "867-5309");  
  fBolt8Ptr = new MJDemoCryostatBolt(fName+"_Bolt_008", "867-5309");  
  fBolt9Ptr = new MJDemoCryostatBolt(fName+"_Bolt_009", "867-5309");  
  fBolt10Ptr = new MJDemoCryostatBolt(fName+"_Bolt_010", "867-5309");  
  fBolt11Ptr = new MJDemoCryostatBolt(fName+"_Bolt_011", "867-5309");  
  fBolt12Ptr = new MJDemoCryostatBolt(fName+"_Bolt_012", "867-5309");  
  fNut1Ptr = new MJDemoCryostatNut(fName+"_Nut_001", "867-5309");
  fNut2Ptr = new MJDemoCryostatNut(fName+"_Nut_002", "867-5309");
  fNut3Ptr = new MJDemoCryostatNut(fName+"_Nut_003", "867-5309");
  fNut4Ptr = new MJDemoCryostatNut(fName+"_Nut_004", "867-5309");
  fNut5Ptr = new MJDemoCryostatNut(fName+"_Nut_005", "867-5309");
  fNut6Ptr = new MJDemoCryostatNut(fName+"_Nut_006", "867-5309");
  fNut7Ptr = new MJDemoCryostatNut(fName+"_Nut_007", "867-5309");
  fNut8Ptr = new MJDemoCryostatNut(fName+"_Nut_008", "867-5309");
  fNut9Ptr = new MJDemoCryostatNut(fName+"_Nut_009", "867-5309");
  fNut10Ptr = new MJDemoCryostatNut(fName+"_Nut_010", "867-5309");
  fNut11Ptr = new MJDemoCryostatNut(fName+"_Nut_011", "867-5309");
  fNut12Ptr = new MJDemoCryostatNut(fName+"_Nut_012", "867-5309");
}

MJDemoCryostatRailAssembly::~MJDemoCryostatRailAssembly()
{
  delete fCenterRail1Ptr;
  delete fCenterRail2Ptr;
  delete fCenterRail3Ptr;
  delete fCenterRail4Ptr;
  delete fLidRail1Ptr;
  delete fLidRail2Ptr;
  delete fLidRail3Ptr;
  delete fLidRail4Ptr;
  delete fBolt1Ptr;
  delete fBolt2Ptr;  
  delete fBolt3Ptr;  
  delete fBolt4Ptr;  
  delete fBolt5Ptr;  
  delete fBolt6Ptr;  
  delete fBolt7Ptr;  
  delete fBolt8Ptr;  
  delete fBolt9Ptr;  
  delete fBolt10Ptr;  
  delete fBolt11Ptr;  
  delete fBolt12Ptr;  
  delete fNut1Ptr;
  delete fNut2Ptr;  
  delete fNut3Ptr;  
  delete fNut4Ptr;  
  delete fNut5Ptr;  
  delete fNut6Ptr;  
  delete fNut7Ptr;  
  delete fNut8Ptr;  
  delete fNut9Ptr;  
  delete fNut10Ptr;  
  delete fNut11Ptr;  
  delete fNut12Ptr;  
}

void MJDemoCryostatRailAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition); 
  
  G4ThreeVector *CenterRail1localPos = new G4ThreeVector(0, 0, 0*25.4*mm);
  G4RotationMatrix *CenterRail1localRot = new G4RotationMatrix();
  G4AffineTransform *CenterRail1affine = new G4AffineTransform(CenterRail1localRot,*CenterRail1localPos);
  *CenterRail1affine*=*assemAffine;  
  G4ThreeVector *CenterRail1globalPos = new G4ThreeVector(CenterRail1affine->NetTranslation());
  G4RotationMatrix *CenterRail1globalRot= new G4RotationMatrix(CenterRail1affine->NetRotation());
  fCenterRail1Ptr->Place(CenterRail1globalPos, CenterRail1globalRot, motherLogical);
  delete CenterRail1localPos;
  delete CenterRail1localRot;
  delete CenterRail1affine;  

  G4ThreeVector *CenterRail2localPos = new G4ThreeVector(0, 0, 0*25.4*mm);
  G4RotationMatrix *CenterRail2localRot = new G4RotationMatrix();
  CenterRail2localRot->rotateZ(-pi/2);
  G4AffineTransform *CenterRail2affine = new G4AffineTransform(CenterRail2localRot,*CenterRail2localPos);
  *CenterRail2affine*=*assemAffine;  
  G4ThreeVector *CenterRail2globalPos = new G4ThreeVector(CenterRail2affine->NetTranslation());
  G4RotationMatrix *CenterRail2globalRot= new G4RotationMatrix(CenterRail2affine->NetRotation());
  fCenterRail2Ptr->Place(CenterRail2globalPos, CenterRail2globalRot, motherLogical);
  delete CenterRail2localPos;
  delete CenterRail2localRot;
  delete CenterRail2affine;
  
  G4ThreeVector *CenterRail3localPos = new G4ThreeVector(0, 0, 0*25.4*mm);
  G4RotationMatrix *CenterRail3localRot = new G4RotationMatrix();
  CenterRail3localRot->rotateZ(-pi);  
  G4AffineTransform *CenterRail3affine = new G4AffineTransform(CenterRail3localRot,*CenterRail3localPos);
  *CenterRail3affine*=*assemAffine;  
  G4ThreeVector *CenterRail3globalPos = new G4ThreeVector(CenterRail3affine->NetTranslation());
  G4RotationMatrix *CenterRail3globalRot= new G4RotationMatrix(CenterRail3affine->NetRotation());
  fCenterRail3Ptr->Place(CenterRail3globalPos, CenterRail3globalRot, motherLogical);
  delete CenterRail3localPos;
  delete CenterRail3localRot;
  delete CenterRail3affine;
  
  G4ThreeVector *CenterRail4localPos = new G4ThreeVector(0, 0, 0*25.4*mm);
  G4RotationMatrix *CenterRail4localRot = new G4RotationMatrix();
  CenterRail4localRot->rotateZ(-3*pi/2);  
  G4AffineTransform *CenterRail4affine = new G4AffineTransform(CenterRail4localRot,*CenterRail4localPos);
  *CenterRail4affine*=*assemAffine;  
  G4ThreeVector *CenterRail4globalPos = new G4ThreeVector(CenterRail4affine->NetTranslation());
  G4RotationMatrix *CenterRail4globalRot= new G4RotationMatrix(CenterRail4affine->NetRotation());
  fCenterRail4Ptr->Place(CenterRail4globalPos, CenterRail4globalRot, motherLogical);
  delete CenterRail4localPos;
  delete CenterRail4localRot;
  delete CenterRail4affine;
  
  G4ThreeVector *LidRail1localPos = new G4ThreeVector(0, 0, 1.325*25.4*mm); //1.2248
  G4RotationMatrix *LidRail1localRot = new G4RotationMatrix();
  G4AffineTransform *LidRail1affine = new G4AffineTransform(LidRail1localRot,*LidRail1localPos);
  *LidRail1affine*=*assemAffine;  
  G4ThreeVector *LidRail1globalPos = new G4ThreeVector(LidRail1affine->NetTranslation());
  G4RotationMatrix *LidRail1globalRot= new G4RotationMatrix(LidRail1affine->NetRotation());
  fLidRail1Ptr->Place(LidRail1globalPos, LidRail1globalRot, motherLogical);
  delete LidRail1localPos;
  delete LidRail1localRot;
  delete LidRail1affine;   

  G4ThreeVector *LidRail2localPos = new G4ThreeVector(0, 0, 1.325*25.4*mm);
  G4RotationMatrix *LidRail2localRot = new G4RotationMatrix();
  LidRail2localRot->rotateZ(-pi/2);
  G4AffineTransform *LidRail2affine = new G4AffineTransform(LidRail2localRot,*LidRail2localPos);
  *LidRail2affine*=*assemAffine;  
  G4ThreeVector *LidRail2globalPos = new G4ThreeVector(LidRail2affine->NetTranslation());
  G4RotationMatrix *LidRail2globalRot= new G4RotationMatrix(LidRail2affine->NetRotation());
  fLidRail2Ptr->Place(LidRail2globalPos, LidRail2globalRot, motherLogical);
  delete LidRail2localPos;
  delete LidRail2localRot;
  delete LidRail2affine;   

  G4ThreeVector *LidRail3localPos = new G4ThreeVector(0, 0, 1.325*25.4*mm);
  G4RotationMatrix *LidRail3localRot = new G4RotationMatrix();
  LidRail3localRot->rotateZ(-pi);
  G4AffineTransform *LidRail3affine = new G4AffineTransform(LidRail3localRot,*LidRail3localPos);
  *LidRail3affine*=*assemAffine;  
  G4ThreeVector *LidRail3globalPos = new G4ThreeVector(LidRail3affine->NetTranslation());
  G4RotationMatrix *LidRail3globalRot= new G4RotationMatrix(LidRail3affine->NetRotation());
  fLidRail3Ptr->Place(LidRail3globalPos, LidRail3globalRot, motherLogical);
  delete LidRail3localPos;
  delete LidRail3localRot;
  delete LidRail3affine;   

  G4ThreeVector *LidRail4localPos = new G4ThreeVector(0, 0, 1.325*25.4*mm);
  G4RotationMatrix *LidRail4localRot = new G4RotationMatrix();
  LidRail4localRot->rotateZ(-3*pi/2);
  G4AffineTransform *LidRail4affine = new G4AffineTransform(LidRail4localRot,*LidRail4localPos);
  *LidRail4affine*=*assemAffine;  
  G4ThreeVector *LidRail4globalPos = new G4ThreeVector(LidRail4affine->NetTranslation());
  G4RotationMatrix *LidRail4globalRot= new G4RotationMatrix(LidRail4affine->NetRotation());
  fLidRail4Ptr->Place(LidRail4globalPos, LidRail4globalRot, motherLogical);
  delete LidRail4localPos;
  delete LidRail4localRot;
  delete LidRail4affine;   


  G4double boltRad = 7.1266*25.4*mm;
  G4double boltangle = 360.0/12.0*deg;
  G4ThreeVector *Bolt1localPos = new G4ThreeVector(boltRad*cos(0.5*boltangle), 
  													boltRad*sin(0.5*boltangle), 0);
  G4RotationMatrix *Bolt1localRot = new G4RotationMatrix();
  Bolt1localRot->rotateZ(-0.5*boltangle);
  G4AffineTransform *Bolt1affine = new G4AffineTransform(Bolt1localRot,*Bolt1localPos);
  *Bolt1affine*=*assemAffine;  
  G4ThreeVector *Bolt1globalPos = new G4ThreeVector(Bolt1affine->NetTranslation());
  G4RotationMatrix *Bolt1globalRot= new G4RotationMatrix(Bolt1affine->NetRotation());
  fBolt1Ptr->Place(Bolt1globalPos, Bolt1globalRot, motherLogical);
  delete Bolt1localPos;
  delete Bolt1localRot;
  delete Bolt1affine;   
  
  G4ThreeVector *Bolt2localPos = new G4ThreeVector(boltRad*cos(1.5*boltangle), 
  													boltRad*sin(1.5*boltangle), 0);
  G4RotationMatrix *Bolt2localRot = new G4RotationMatrix();
  Bolt2localRot->rotateZ(-1.5*boltangle);
  G4AffineTransform *Bolt2affine = new G4AffineTransform(Bolt2localRot,*Bolt2localPos);
  *Bolt2affine*=*assemAffine;  
  G4ThreeVector *Bolt2globalPos = new G4ThreeVector(Bolt2affine->NetTranslation());
  G4RotationMatrix *Bolt2globalRot= new G4RotationMatrix(Bolt2affine->NetRotation());
  fBolt2Ptr->Place(Bolt2globalPos, Bolt2globalRot, motherLogical);
  delete Bolt2localPos;
  delete Bolt2localRot;
  delete Bolt2affine;    
  
  G4ThreeVector *Bolt3localPos = new G4ThreeVector(boltRad*cos(2.5*boltangle), 
  													boltRad*sin(2.5*boltangle), 0);
  G4RotationMatrix *Bolt3localRot = new G4RotationMatrix();
  Bolt3localRot->rotateZ(-2.5*boltangle);
  G4AffineTransform *Bolt3affine = new G4AffineTransform(Bolt3localRot,*Bolt3localPos);
  *Bolt3affine*=*assemAffine;  
  G4ThreeVector *Bolt3globalPos = new G4ThreeVector(Bolt3affine->NetTranslation());
  G4RotationMatrix *Bolt3globalRot= new G4RotationMatrix(Bolt3affine->NetRotation());
  fBolt3Ptr->Place(Bolt3globalPos, Bolt3globalRot, motherLogical);
  delete Bolt3localPos;
  delete Bolt3localRot;
  delete Bolt3affine;    
  
  G4ThreeVector *Bolt4localPos = new G4ThreeVector(boltRad*cos(3.5*boltangle), 
  													boltRad*sin(3.5*boltangle), 0);
  G4RotationMatrix *Bolt4localRot = new G4RotationMatrix();
  Bolt4localRot->rotateZ(-3.5*boltangle);
  G4AffineTransform *Bolt4affine = new G4AffineTransform(Bolt4localRot,*Bolt4localPos);
  *Bolt4affine*=*assemAffine;  
  G4ThreeVector *Bolt4globalPos = new G4ThreeVector(Bolt4affine->NetTranslation());
  G4RotationMatrix *Bolt4globalRot= new G4RotationMatrix(Bolt4affine->NetRotation());
  fBolt4Ptr->Place(Bolt4globalPos, Bolt4globalRot, motherLogical);
  delete Bolt4localPos;
  delete Bolt4localRot;
  delete Bolt4affine;    
  
  G4ThreeVector *Bolt5localPos = new G4ThreeVector(boltRad*cos(4.5*boltangle), 
  													boltRad*sin(4.5*boltangle), 0);
  G4RotationMatrix *Bolt5localRot = new G4RotationMatrix();
  Bolt5localRot->rotateZ(-4.5*boltangle);
  G4AffineTransform *Bolt5affine = new G4AffineTransform(Bolt5localRot,*Bolt5localPos);
  *Bolt5affine*=*assemAffine;  
  G4ThreeVector *Bolt5globalPos = new G4ThreeVector(Bolt5affine->NetTranslation());
  G4RotationMatrix *Bolt5globalRot= new G4RotationMatrix(Bolt5affine->NetRotation());
  fBolt5Ptr->Place(Bolt5globalPos, Bolt5globalRot, motherLogical);
  delete Bolt5localPos;
  delete Bolt5localRot;
  delete Bolt5affine;    
  
  G4ThreeVector *Bolt6localPos = new G4ThreeVector(boltRad*cos(5.5*boltangle), 
  													boltRad*sin(5.5*boltangle), 0);
  G4RotationMatrix *Bolt6localRot = new G4RotationMatrix();
  Bolt6localRot->rotateZ(-5.5*boltangle);
  G4AffineTransform *Bolt6affine = new G4AffineTransform(Bolt6localRot,*Bolt6localPos);
  *Bolt6affine*=*assemAffine;  
  G4ThreeVector *Bolt6globalPos = new G4ThreeVector(Bolt6affine->NetTranslation());
  G4RotationMatrix *Bolt6globalRot= new G4RotationMatrix(Bolt6affine->NetRotation());
  fBolt6Ptr->Place(Bolt6globalPos, Bolt6globalRot, motherLogical);
  delete Bolt6localPos;
  delete Bolt6localRot;
  delete Bolt6affine;   
  
  G4ThreeVector *Bolt7localPos = new G4ThreeVector(boltRad*cos(6.5*boltangle), 
  													boltRad*sin(6.5*boltangle), 0);
  G4RotationMatrix *Bolt7localRot = new G4RotationMatrix();
  Bolt7localRot->rotateZ(-6.5*boltangle);
  G4AffineTransform *Bolt7affine = new G4AffineTransform(Bolt7localRot,*Bolt7localPos);
  *Bolt7affine*=*assemAffine;  
  G4ThreeVector *Bolt7globalPos = new G4ThreeVector(Bolt7affine->NetTranslation());
  G4RotationMatrix *Bolt7globalRot= new G4RotationMatrix(Bolt7affine->NetRotation());
  fBolt7Ptr->Place(Bolt7globalPos, Bolt7globalRot, motherLogical);
  delete Bolt7localPos;
  delete Bolt7localRot;
  delete Bolt7affine;     
  
  G4ThreeVector *Bolt8localPos = new G4ThreeVector(boltRad*cos(7.5*boltangle), 
  													boltRad*sin(7.5*boltangle), 0);
  G4RotationMatrix *Bolt8localRot = new G4RotationMatrix();
  Bolt8localRot->rotateZ(-7.5*boltangle);
  G4AffineTransform *Bolt8affine = new G4AffineTransform(Bolt8localRot,*Bolt8localPos);
  *Bolt8affine*=*assemAffine;  
  G4ThreeVector *Bolt8globalPos = new G4ThreeVector(Bolt8affine->NetTranslation());
  G4RotationMatrix *Bolt8globalRot= new G4RotationMatrix(Bolt8affine->NetRotation());
  fBolt8Ptr->Place(Bolt8globalPos, Bolt8globalRot, motherLogical);
  delete Bolt8localPos;
  delete Bolt8localRot;
  delete Bolt8affine;   
  
  G4ThreeVector *Bolt9localPos = new G4ThreeVector(boltRad*cos(8.5*boltangle), 
  													boltRad*sin(8.5*boltangle), 0);
  G4RotationMatrix *Bolt9localRot = new G4RotationMatrix();
  Bolt9localRot->rotateZ(-8.5*boltangle);
  G4AffineTransform *Bolt9affine = new G4AffineTransform(Bolt9localRot,*Bolt9localPos);
  *Bolt9affine*=*assemAffine;  
  G4ThreeVector *Bolt9globalPos = new G4ThreeVector(Bolt9affine->NetTranslation());
  G4RotationMatrix *Bolt9globalRot= new G4RotationMatrix(Bolt9affine->NetRotation());
  fBolt9Ptr->Place(Bolt9globalPos, Bolt9globalRot, motherLogical);
  delete Bolt9localPos;
  delete Bolt9localRot;
  delete Bolt9affine;   
  
  G4ThreeVector *Bolt10localPos = new G4ThreeVector(boltRad*cos(9.5*boltangle), 
  													boltRad*sin(9.5*boltangle), 0);
  G4RotationMatrix *Bolt10localRot = new G4RotationMatrix();
  Bolt10localRot->rotateZ(-9.5*boltangle);
  G4AffineTransform *Bolt10affine = new G4AffineTransform(Bolt10localRot,*Bolt10localPos);
  *Bolt10affine*=*assemAffine;  
  G4ThreeVector *Bolt10globalPos = new G4ThreeVector(Bolt10affine->NetTranslation());
  G4RotationMatrix *Bolt10globalRot= new G4RotationMatrix(Bolt10affine->NetRotation());
  fBolt10Ptr->Place(Bolt10globalPos, Bolt10globalRot, motherLogical);
  delete Bolt10localPos;
  delete Bolt10localRot;
  delete Bolt10affine;   
   
  G4ThreeVector *Bolt11localPos = new G4ThreeVector(boltRad*cos(10.5*boltangle), 
  													boltRad*sin(10.5*boltangle), 0);
  G4RotationMatrix *Bolt11localRot = new G4RotationMatrix();
  Bolt11localRot->rotateZ(-10.5*boltangle);
  G4AffineTransform *Bolt11affine = new G4AffineTransform(Bolt11localRot,*Bolt11localPos);
  *Bolt11affine*=*assemAffine;  
  G4ThreeVector *Bolt11globalPos = new G4ThreeVector(Bolt11affine->NetTranslation());
  G4RotationMatrix *Bolt11globalRot= new G4RotationMatrix(Bolt11affine->NetRotation());
  fBolt11Ptr->Place(Bolt11globalPos, Bolt11globalRot, motherLogical);
  delete Bolt11localPos;
  delete Bolt11localRot;
  delete Bolt11affine;     
  
  G4ThreeVector *Bolt12localPos = new G4ThreeVector(boltRad*cos(11.5*boltangle), 
  													boltRad*sin(11.5*boltangle), 0);
  G4RotationMatrix *Bolt12localRot = new G4RotationMatrix();
  Bolt12localRot->rotateZ(-11.5*boltangle);
  G4AffineTransform *Bolt12affine = new G4AffineTransform(Bolt12localRot,*Bolt12localPos);
  *Bolt12affine*=*assemAffine;  
  G4ThreeVector *Bolt12globalPos = new G4ThreeVector(Bolt12affine->NetTranslation());
  G4RotationMatrix *Bolt12globalRot= new G4RotationMatrix(Bolt12affine->NetRotation());
  fBolt12Ptr->Place(Bolt12globalPos, Bolt12globalRot, motherLogical);
  delete Bolt12localPos;
  delete Bolt12localRot;
  delete Bolt12affine;     

  G4ThreeVector *Nut1localPos = new G4ThreeVector(boltRad*cos(0.5*boltangle), 
  													boltRad*sin(0.5*boltangle), 1.3258*25.4*mm);//1.2248
  G4RotationMatrix *Nut1localRot = new G4RotationMatrix();
  Nut1localRot->rotateX(pi);
  Nut1localRot->rotateZ(-0.5*boltangle);
  G4AffineTransform *Nut1affine = new G4AffineTransform(Nut1localRot,*Nut1localPos);
  *Nut1affine*=*assemAffine;  
  G4ThreeVector *Nut1globalPos = new G4ThreeVector(Nut1affine->NetTranslation());
  G4RotationMatrix *Nut1globalRot= new G4RotationMatrix(Nut1affine->NetRotation());
  fNut1Ptr->Place(Nut1globalPos, Nut1globalRot, motherLogical);
  delete Nut1localPos;
  delete Nut1localRot;
  delete Nut1affine;   
  
  G4ThreeVector *Nut2localPos = new G4ThreeVector(boltRad*cos(1.5*boltangle), 
  													boltRad*sin(1.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut2localRot = new G4RotationMatrix();
  Nut2localRot->rotateX(pi);
  Nut2localRot->rotateZ(-1.5*boltangle);
  G4AffineTransform *Nut2affine = new G4AffineTransform(Nut2localRot,*Nut2localPos);
  *Nut2affine*=*assemAffine;  
  G4ThreeVector *Nut2globalPos = new G4ThreeVector(Nut2affine->NetTranslation());
  G4RotationMatrix *Nut2globalRot= new G4RotationMatrix(Nut2affine->NetRotation());
  fNut2Ptr->Place(Nut2globalPos, Nut2globalRot, motherLogical);
  delete Nut2localPos;
  delete Nut2localRot;
  delete Nut2affine;    
  
  G4ThreeVector *Nut3localPos = new G4ThreeVector(boltRad*cos(2.5*boltangle), 
  													boltRad*sin(2.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut3localRot = new G4RotationMatrix();
  Nut3localRot->rotateX(pi);
  Nut3localRot->rotateZ(-2.5*boltangle);
  G4AffineTransform *Nut3affine = new G4AffineTransform(Nut3localRot,*Nut3localPos);
  *Nut3affine*=*assemAffine;  
  G4ThreeVector *Nut3globalPos = new G4ThreeVector(Nut3affine->NetTranslation());
  G4RotationMatrix *Nut3globalRot= new G4RotationMatrix(Nut3affine->NetRotation());
  fNut3Ptr->Place(Nut3globalPos, Nut3globalRot, motherLogical);
  delete Nut3localPos;
  delete Nut3localRot;
  delete Nut3affine;    
  
  G4ThreeVector *Nut4localPos = new G4ThreeVector(boltRad*cos(3.5*boltangle), 
  													boltRad*sin(3.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut4localRot = new G4RotationMatrix();
  Nut4localRot->rotateX(pi);
  Nut4localRot->rotateZ(-3.5*boltangle);
  G4AffineTransform *Nut4affine = new G4AffineTransform(Nut4localRot,*Nut4localPos);
  *Nut4affine*=*assemAffine;  
  G4ThreeVector *Nut4globalPos = new G4ThreeVector(Nut4affine->NetTranslation());
  G4RotationMatrix *Nut4globalRot= new G4RotationMatrix(Nut4affine->NetRotation());
  fNut4Ptr->Place(Nut4globalPos, Nut4globalRot, motherLogical);
  delete Nut4localPos;
  delete Nut4localRot;
  delete Nut4affine;    
  
  G4ThreeVector *Nut5localPos = new G4ThreeVector(boltRad*cos(4.5*boltangle), 
  													boltRad*sin(4.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut5localRot = new G4RotationMatrix();
  Nut5localRot->rotateX(pi);
  Nut5localRot->rotateZ(-4.5*boltangle);
  G4AffineTransform *Nut5affine = new G4AffineTransform(Nut5localRot,*Nut5localPos);
  *Nut5affine*=*assemAffine;  
  G4ThreeVector *Nut5globalPos = new G4ThreeVector(Nut5affine->NetTranslation());
  G4RotationMatrix *Nut5globalRot= new G4RotationMatrix(Nut5affine->NetRotation());
  fNut5Ptr->Place(Nut5globalPos, Nut5globalRot, motherLogical);
  delete Nut5localPos;
  delete Nut5localRot;
  delete Nut5affine;    
  
  G4ThreeVector *Nut6localPos = new G4ThreeVector(boltRad*cos(5.5*boltangle), 
  													boltRad*sin(5.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut6localRot = new G4RotationMatrix();
  Nut6localRot->rotateX(pi);
  Nut6localRot->rotateZ(-5.5*boltangle);
  G4AffineTransform *Nut6affine = new G4AffineTransform(Nut6localRot,*Nut6localPos);
  *Nut6affine*=*assemAffine;  
  G4ThreeVector *Nut6globalPos = new G4ThreeVector(Nut6affine->NetTranslation());
  G4RotationMatrix *Nut6globalRot= new G4RotationMatrix(Nut6affine->NetRotation());
  fNut6Ptr->Place(Nut6globalPos, Nut6globalRot, motherLogical);
  delete Nut6localPos;
  delete Nut6localRot;
  delete Nut6affine;   
  
  G4ThreeVector *Nut7localPos = new G4ThreeVector(boltRad*cos(6.5*boltangle), 
  													boltRad*sin(6.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut7localRot = new G4RotationMatrix();
  Nut7localRot->rotateX(pi);
  Nut7localRot->rotateZ(-6.5*boltangle);
  G4AffineTransform *Nut7affine = new G4AffineTransform(Nut7localRot,*Nut7localPos);
  *Nut7affine*=*assemAffine;  
  G4ThreeVector *Nut7globalPos = new G4ThreeVector(Nut7affine->NetTranslation());
  G4RotationMatrix *Nut7globalRot= new G4RotationMatrix(Nut7affine->NetRotation());
  fNut7Ptr->Place(Nut7globalPos, Nut7globalRot, motherLogical);
  delete Nut7localPos;
  delete Nut7localRot;
  delete Nut7affine;     
  
  G4ThreeVector *Nut8localPos = new G4ThreeVector(boltRad*cos(7.5*boltangle), 
  													boltRad*sin(7.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut8localRot = new G4RotationMatrix();
  Nut8localRot->rotateX(pi);
  Nut8localRot->rotateZ(-7.5*boltangle);
  G4AffineTransform *Nut8affine = new G4AffineTransform(Nut8localRot,*Nut8localPos);
  *Nut8affine*=*assemAffine;  
  G4ThreeVector *Nut8globalPos = new G4ThreeVector(Nut8affine->NetTranslation());
  G4RotationMatrix *Nut8globalRot= new G4RotationMatrix(Nut8affine->NetRotation());
  fNut8Ptr->Place(Nut8globalPos, Nut8globalRot, motherLogical);
  delete Nut8localPos;
  delete Nut8localRot;
  delete Nut8affine;   
  
  G4ThreeVector *Nut9localPos = new G4ThreeVector(boltRad*cos(8.5*boltangle), 
  													boltRad*sin(8.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut9localRot = new G4RotationMatrix();
  Nut9localRot->rotateX(pi);
  Nut9localRot->rotateZ(-8.5*boltangle);
  G4AffineTransform *Nut9affine = new G4AffineTransform(Nut9localRot,*Nut9localPos);
  *Nut9affine*=*assemAffine;  
  G4ThreeVector *Nut9globalPos = new G4ThreeVector(Nut9affine->NetTranslation());
  G4RotationMatrix *Nut9globalRot= new G4RotationMatrix(Nut9affine->NetRotation());
  fNut9Ptr->Place(Nut9globalPos, Nut9globalRot, motherLogical);
  delete Nut9localPos;
  delete Nut9localRot;
  delete Nut9affine;   
  
  G4ThreeVector *Nut10localPos = new G4ThreeVector(boltRad*cos(9.5*boltangle), 
  													boltRad*sin(9.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut10localRot = new G4RotationMatrix();
  Nut10localRot->rotateX(pi);
  Nut10localRot->rotateZ(-9.5*boltangle);
  G4AffineTransform *Nut10affine = new G4AffineTransform(Nut10localRot,*Nut10localPos);
  *Nut10affine*=*assemAffine;  
  G4ThreeVector *Nut10globalPos = new G4ThreeVector(Nut10affine->NetTranslation());
  G4RotationMatrix *Nut10globalRot= new G4RotationMatrix(Nut10affine->NetRotation());
  fNut10Ptr->Place(Nut10globalPos, Nut10globalRot, motherLogical);
  delete Nut10localPos;
  delete Nut10localRot;
  delete Nut10affine;   
   
  G4ThreeVector *Nut11localPos = new G4ThreeVector(boltRad*cos(10.5*boltangle), 
  													boltRad*sin(10.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut11localRot = new G4RotationMatrix();
  Nut11localRot->rotateX(pi);
  Nut11localRot->rotateZ(-10.5*boltangle);
  G4AffineTransform *Nut11affine = new G4AffineTransform(Nut11localRot,*Nut11localPos);
  *Nut11affine*=*assemAffine;  
  G4ThreeVector *Nut11globalPos = new G4ThreeVector(Nut11affine->NetTranslation());
  G4RotationMatrix *Nut11globalRot= new G4RotationMatrix(Nut11affine->NetRotation());
  fNut11Ptr->Place(Nut11globalPos, Nut11globalRot, motherLogical);
  delete Nut11localPos;
  delete Nut11localRot;
  delete Nut11affine;     
  
  G4ThreeVector *Nut12localPos = new G4ThreeVector(boltRad*cos(11.5*boltangle), 
  													boltRad*sin(11.5*boltangle), 1.3258*25.4*mm);
  G4RotationMatrix *Nut12localRot = new G4RotationMatrix();
  Nut12localRot->rotateX(pi);
  Nut12localRot->rotateZ(-11.5*boltangle);
  G4AffineTransform *Nut12affine = new G4AffineTransform(Nut12localRot,*Nut12localPos);
  *Nut12affine*=*assemAffine;  
  G4ThreeVector *Nut12globalPos = new G4ThreeVector(Nut12affine->NetTranslation());
  G4RotationMatrix *Nut12globalRot= new G4RotationMatrix(Nut12affine->NetRotation());
  fNut12Ptr->Place(Nut12globalPos, Nut12globalRot, motherLogical);
  delete Nut12localPos;
  delete Nut12localRot;
  delete Nut12affine;     
  
  delete assemAffine;
}
