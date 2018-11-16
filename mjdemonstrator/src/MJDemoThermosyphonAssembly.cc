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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Co-axial with thermosyphon tube, front face of cold plate adapter
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
 * 10-01-2015, Added electrical break, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonInsertionSubAssembly.hh"
#include "mjdemonstrator/MJDemo51624Bolt.hh"
#include "mjdemonstrator/MJDemoSignalCableTakeupSubAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonEBreakAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonAssembly::MJDemoThermosyphonAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "ThermosyphonAssemdwg")
{
  fInsertPtr = new MJDemoThermosyphonInsertionSubAssembly(fName+"_ThermosyphonInsert_001", serialNumber);
  fBolt01Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_001", "867-5309", 57.15);
  fBolt02Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_002", "867-5309", 57.15);
  fBolt03Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_003", "867-5309", 57.15);
  fBolt04Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_004", "867-5309", 57.15);
  fBolt05Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_005", "867-5309", 57.15);
  fBolt06Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_006", "867-5309", 57.15);
  fTakeup01Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_001", "867-5309");
  fTakeup02Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_002", "867-5309");
  fTakeup03Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_003", "867-5309");
  fTakeup04Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_004", "867-5309");
  fTakeup05Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_005", "867-5309");
  fTakeup06Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_006", "867-5309");
  fTakeup07Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_007", "867-5309");
  fTakeup08Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_008", "867-5309");
  fEBreakPtr = new MJDemoThermosyphonEBreakAssembly(fName+"_EBreak_001", "867-5309");
}

MJDemoThermosyphonAssembly::MJDemoThermosyphonAssembly(const MJDemoThermosyphonAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fInsertPtr = new MJDemoThermosyphonInsertionSubAssembly(fName+"_ThermosyphonInsert_001", rhs.GetInsertionAssembly()->GetSerialNumber());
  fBolt01Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_001", "867-5309", 57.15);
  fBolt02Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_002", "867-5309", 57.15);
  fBolt03Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_003", "867-5309", 57.15);
  fBolt04Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_004", "867-5309", 57.15);
  fBolt05Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_005", "867-5309", 57.15);
  fBolt06Ptr = new MJDemo51624Bolt(fName+"_5/16-24x2.25Bolt_006", "867-5309", 57.15);
  fTakeup01Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_001", "867-5309");
  fTakeup02Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_002", "867-5309");
  fTakeup03Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_003", "867-5309");
  fTakeup04Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_004", "867-5309");
  fTakeup05Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_005", "867-5309");
  fTakeup06Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_006", "867-5309");
  fTakeup07Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_007", "867-5309");
  fTakeup08Ptr = new MJDemoSignalCableTakeupSubAssembly(fName+"_Takeup_008", "867-5309");
  fEBreakPtr = new MJDemoThermosyphonEBreakAssembly(fName+"_EBreak_001", "867-5309");
}

MJDemoThermosyphonAssembly::~MJDemoThermosyphonAssembly()
{
  delete fInsertPtr;
  delete fBolt01Ptr;
  delete fBolt02Ptr;
  delete fBolt03Ptr;
  delete fBolt04Ptr;
  delete fBolt05Ptr;
  delete fBolt06Ptr;
  delete fTakeup01Ptr;
  delete fTakeup02Ptr;
  delete fTakeup03Ptr;
  delete fTakeup04Ptr;
  delete fTakeup05Ptr;
  delete fTakeup06Ptr;
  delete fTakeup07Ptr;
  delete fTakeup08Ptr;
  delete fEBreakPtr;
}

void MJDemoThermosyphonAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *InsertlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *InsertlocalRot = new G4RotationMatrix();
  InsertlocalRot->rotateX(0);
  InsertlocalRot->rotateY(0);
  G4AffineTransform *Insertaffine = new G4AffineTransform(InsertlocalRot,*InsertlocalPos);
  *Insertaffine*=*assemAffine;  
  G4ThreeVector *InsertglobalPos = new G4ThreeVector(Insertaffine->NetTranslation());
  G4RotationMatrix *InsertglobalRot= new G4RotationMatrix(Insertaffine->NetRotation());
  fInsertPtr->Place(InsertglobalPos, InsertglobalRot, motherLogical);
  delete InsertlocalPos;
  delete InsertlocalRot;
  delete Insertaffine;

  G4ThreeVector *Bolt01localPos = new G4ThreeVector(-104.775*mm, 25.4*mm, 24.5*mm); //24.14
  G4RotationMatrix *Bolt01localRot = new G4RotationMatrix();
  Bolt01localRot->rotateX(0);
  Bolt01localRot->rotateY(0);
  G4AffineTransform *Bolt01affine = new G4AffineTransform(Bolt01localRot,*Bolt01localPos);
  *Bolt01affine*=*assemAffine;
  G4ThreeVector *Bolt01globalPos = new G4ThreeVector(Bolt01affine->NetTranslation());
  G4RotationMatrix *Bolt01globalRot= new G4RotationMatrix(Bolt01affine->NetRotation());
  fBolt01Ptr->Place(Bolt01globalPos, Bolt01globalRot, motherLogical);
  delete Bolt01localPos;
  delete Bolt01localRot;
  delete Bolt01affine;
    
  G4ThreeVector *Bolt02localPos = new G4ThreeVector(-79.375*mm, 25.4*mm, 24.5*mm);
  G4RotationMatrix *Bolt02localRot = new G4RotationMatrix();
  Bolt02localRot->rotateX(0);
  Bolt02localRot->rotateY(0);
  G4AffineTransform *Bolt02affine = new G4AffineTransform(Bolt02localRot,*Bolt02localPos);
  *Bolt02affine*=*assemAffine;
  G4ThreeVector *Bolt02globalPos = new G4ThreeVector(Bolt02affine->NetTranslation());
  G4RotationMatrix *Bolt02globalRot= new G4RotationMatrix(Bolt02affine->NetRotation());
  fBolt02Ptr->Place(Bolt02globalPos, Bolt02globalRot, motherLogical);
  delete Bolt02localPos;
  delete Bolt02localRot;
  delete Bolt02affine;
    
  G4ThreeVector *Bolt03localPos = new G4ThreeVector(-9.525*mm, 25.4*mm, 24.5*mm);
  G4RotationMatrix *Bolt03localRot = new G4RotationMatrix();
  Bolt03localRot->rotateX(0);
  Bolt03localRot->rotateY(0);
  G4AffineTransform *Bolt03affine = new G4AffineTransform(Bolt03localRot,*Bolt03localPos);
  *Bolt03affine*=*assemAffine;
  G4ThreeVector *Bolt03globalPos = new G4ThreeVector(Bolt03affine->NetTranslation());
  G4RotationMatrix *Bolt03globalRot= new G4RotationMatrix(Bolt03affine->NetRotation());
  fBolt03Ptr->Place(Bolt03globalPos, Bolt03globalRot, motherLogical);
  delete Bolt03localPos;
  delete Bolt03localRot;
  delete Bolt03affine;
    
  G4ThreeVector *Bolt04localPos = new G4ThreeVector(-104.775*mm, -25.4*mm, 24.5*mm);
  G4RotationMatrix *Bolt04localRot = new G4RotationMatrix();
  Bolt04localRot->rotateX(0);
  Bolt04localRot->rotateY(0);
  G4AffineTransform *Bolt04affine = new G4AffineTransform(Bolt04localRot,*Bolt04localPos);
  *Bolt04affine*=*assemAffine;
  G4ThreeVector *Bolt04globalPos = new G4ThreeVector(Bolt04affine->NetTranslation());
  G4RotationMatrix *Bolt04globalRot= new G4RotationMatrix(Bolt04affine->NetRotation());
  fBolt04Ptr->Place(Bolt04globalPos, Bolt04globalRot, motherLogical);
  delete Bolt04localPos;
  delete Bolt04localRot;
  delete Bolt04affine;
    
  G4ThreeVector *Bolt05localPos = new G4ThreeVector(-79.375*mm, -25.4*mm, 24.5*mm);
  G4RotationMatrix *Bolt05localRot = new G4RotationMatrix();
  Bolt05localRot->rotateX(0);
  Bolt05localRot->rotateY(0);
  G4AffineTransform *Bolt05affine = new G4AffineTransform(Bolt05localRot,*Bolt05localPos);
  *Bolt05affine*=*assemAffine;
  G4ThreeVector *Bolt05globalPos = new G4ThreeVector(Bolt05affine->NetTranslation());
  G4RotationMatrix *Bolt05globalRot= new G4RotationMatrix(Bolt05affine->NetRotation());
  fBolt05Ptr->Place(Bolt05globalPos, Bolt05globalRot, motherLogical);
  delete Bolt05localPos;
  delete Bolt05localRot;
  delete Bolt05affine;
    
  G4ThreeVector *Bolt06localPos = new G4ThreeVector(-9.525*mm, -25.4*mm, 24.5*mm);
  G4RotationMatrix *Bolt06localRot = new G4RotationMatrix();
  Bolt06localRot->rotateX(0);
  Bolt06localRot->rotateY(0);
  G4AffineTransform *Bolt06affine = new G4AffineTransform(Bolt06localRot,*Bolt06localPos);
  *Bolt06affine*=*assemAffine;
  G4ThreeVector *Bolt06globalPos = new G4ThreeVector(Bolt06affine->NetTranslation());
  G4RotationMatrix *Bolt06globalRot= new G4RotationMatrix(Bolt06affine->NetRotation());
  fBolt06Ptr->Place(Bolt06globalPos, Bolt06globalRot, motherLogical);
  delete Bolt06localPos;
  delete Bolt06localRot;
  delete Bolt06affine;
    
  G4ThreeVector *Takeup01localPos = new G4ThreeVector(-64.454*mm, 9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup01localRot = new G4RotationMatrix();
  Takeup01localRot->rotateZ(0);
  Takeup01localRot->rotateY(0);
  G4AffineTransform *Takeup01affine = new G4AffineTransform(Takeup01localRot,*Takeup01localPos);
  *Takeup01affine*=*assemAffine;
  G4ThreeVector *Takeup01globalPos = new G4ThreeVector(Takeup01affine->NetTranslation());
  G4RotationMatrix *Takeup01globalRot= new G4RotationMatrix(Takeup01affine->NetRotation());
  fTakeup01Ptr->Place(Takeup01globalPos, Takeup01globalRot, motherLogical);
  delete Takeup01localPos;
  delete Takeup01localRot;
  delete Takeup01affine;
    
  G4ThreeVector *Takeup02localPos = new G4ThreeVector(5.396*mm, 9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup02localRot = new G4RotationMatrix();
  Takeup02localRot->rotateZ(0);
  Takeup02localRot->rotateY(0);
  G4AffineTransform *Takeup02affine = new G4AffineTransform(Takeup02localRot,*Takeup02localPos);
  *Takeup02affine*=*assemAffine;
  G4ThreeVector *Takeup02globalPos = new G4ThreeVector(Takeup02affine->NetTranslation());
  G4RotationMatrix *Takeup02globalRot= new G4RotationMatrix(Takeup02affine->NetRotation());
  fTakeup02Ptr->Place(Takeup02globalPos, Takeup02globalRot, motherLogical);
  delete Takeup02localPos;
  delete Takeup02localRot;
  delete Takeup02affine;
    
  G4ThreeVector *Takeup03localPos = new G4ThreeVector(60.97*mm, 9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup03localRot = new G4RotationMatrix();
  Takeup03localRot->rotateX(0);
  Takeup03localRot->rotateY(0);
  G4AffineTransform *Takeup03affine = new G4AffineTransform(Takeup03localRot,*Takeup03localPos);
  *Takeup03affine*=*assemAffine;
  G4ThreeVector *Takeup03globalPos = new G4ThreeVector(Takeup03affine->NetTranslation());
  G4RotationMatrix *Takeup03globalRot= new G4RotationMatrix(Takeup03affine->NetRotation());
  fTakeup03Ptr->Place(Takeup03globalPos, Takeup03globalRot, motherLogical);
  delete Takeup03localPos;
  delete Takeup03localRot;
  delete Takeup03affine;
    
  G4ThreeVector *Takeup04localPos = new G4ThreeVector(116.545*mm, 9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup04localRot = new G4RotationMatrix();
  Takeup04localRot->rotateX(0);
  Takeup04localRot->rotateY(0);
  G4AffineTransform *Takeup04affine = new G4AffineTransform(Takeup04localRot,*Takeup04localPos);
  *Takeup04affine*=*assemAffine;
  G4ThreeVector *Takeup04globalPos = new G4ThreeVector(Takeup04affine->NetTranslation());
  G4RotationMatrix *Takeup04globalRot= new G4RotationMatrix(Takeup04affine->NetRotation());
  fTakeup04Ptr->Place(Takeup04globalPos, Takeup04globalRot, motherLogical);
  delete Takeup04localPos;
  delete Takeup04localRot;
  delete Takeup04affine;

  G4ThreeVector *Takeup05localPos = new G4ThreeVector(156.218*mm, -9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup05localRot = new G4RotationMatrix();
  Takeup05localRot->rotateZ(pi);
  Takeup05localRot->rotateY(0);
  G4AffineTransform *Takeup05affine = new G4AffineTransform(Takeup05localRot,*Takeup05localPos);
  *Takeup05affine*=*assemAffine;
  G4ThreeVector *Takeup05globalPos = new G4ThreeVector(Takeup05affine->NetTranslation());
  G4RotationMatrix *Takeup05globalRot= new G4RotationMatrix(Takeup05affine->NetRotation());
  fTakeup05Ptr->Place(Takeup05globalPos, Takeup05globalRot, motherLogical);
  delete Takeup05localPos;
  delete Takeup05localRot;
  delete Takeup05affine;
    
  G4ThreeVector *Takeup06localPos = new G4ThreeVector(100.643*mm, -9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup06localRot = new G4RotationMatrix();
  Takeup06localRot->rotateZ(pi);
  Takeup06localRot->rotateY(0);
  G4AffineTransform *Takeup06affine = new G4AffineTransform(Takeup06localRot,*Takeup06localPos);
  *Takeup06affine*=*assemAffine;
  G4ThreeVector *Takeup06globalPos = new G4ThreeVector(Takeup06affine->NetTranslation());
  G4RotationMatrix *Takeup06globalRot= new G4RotationMatrix(Takeup06affine->NetRotation());
  fTakeup06Ptr->Place(Takeup06globalPos, Takeup06globalRot, motherLogical);
  delete Takeup06localPos;
  delete Takeup06localRot;
  delete Takeup06affine;

  G4ThreeVector *Takeup07localPos = new G4ThreeVector(45.069*mm, -9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup07localRot = new G4RotationMatrix();
  Takeup07localRot->rotateZ(pi);
  Takeup07localRot->rotateY(0);
  G4AffineTransform *Takeup07affine = new G4AffineTransform(Takeup07localRot,*Takeup07localPos);
  *Takeup07affine*=*assemAffine;
  G4ThreeVector *Takeup07globalPos = new G4ThreeVector(Takeup07affine->NetTranslation());
  G4RotationMatrix *Takeup07globalRot= new G4RotationMatrix(Takeup07affine->NetRotation());
  fTakeup07Ptr->Place(Takeup07globalPos, Takeup07globalRot, motherLogical);
  delete Takeup07localPos;
  delete Takeup07localRot;
  delete Takeup07affine;

  G4ThreeVector *Takeup08localPos = new G4ThreeVector(-24.781*mm, -9.526*mm, 32.5*mm);
  G4RotationMatrix *Takeup08localRot = new G4RotationMatrix();
  Takeup08localRot->rotateZ(pi);
  Takeup08localRot->rotateY(0);
  G4AffineTransform *Takeup08affine = new G4AffineTransform(Takeup08localRot,*Takeup08localPos);
  *Takeup08affine*=*assemAffine;
  G4ThreeVector *Takeup08globalPos = new G4ThreeVector(Takeup08affine->NetTranslation());
  G4RotationMatrix *Takeup08globalRot= new G4RotationMatrix(Takeup08affine->NetRotation());
  fTakeup08Ptr->Place(Takeup08globalPos, Takeup08globalRot, motherLogical);
  delete Takeup08localPos;
  delete Takeup08localRot;
  delete Takeup08affine;

  G4ThreeVector *ebreaklocalPos = new G4ThreeVector(-1049.427*mm, 0, (22.352+1.07*25.405)*mm);
  G4RotationMatrix *ebreaklocalRot = new G4RotationMatrix();
  G4AffineTransform *ebreakaffine = new G4AffineTransform(ebreaklocalRot,*ebreaklocalPos);
  *ebreakaffine*=*assemAffine;
  G4ThreeVector *ebreakglobalPos = new G4ThreeVector(ebreakaffine->NetTranslation());
  G4RotationMatrix *ebreakglobalRot= new G4RotationMatrix(ebreakaffine->NetRotation());
  fEBreakPtr->Place(ebreakglobalPos, ebreakglobalRot, motherLogical);
  delete ebreaklocalPos;
  delete ebreaklocalRot;
  delete ebreakaffine;
    
  delete assemAffine;
}
