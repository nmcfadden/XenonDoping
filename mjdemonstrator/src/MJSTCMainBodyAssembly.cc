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
// CLASS IMPLEMENTATION:  MJSTCMainBodyAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
 * Did not include vacuum hardware (nuts and bolts) in geometry.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. MacMullin
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
#include "mjdemonstrator/MJSTCMainBodyAssembly.hh"
#include "mjdemonstrator/MJSTCMainBodyCan.hh"
#include "mjdemonstrator/MJSTCMainBody8Flange6Bore.hh"
#include "mjdemonstrator/MJSTCMainBodyHoop.hh"
#include "mjdemonstrator/MJSTCMainBodyBlankFlange.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCMainBodyAssembly::MJSTCMainBodyAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MainBody")
{
  fCanPtr = new MJSTCMainBodyCan(fName+"_Can_001", "867-5309");
  fBottomFlangePtr = new MJSTCMainBody8Flange6Bore(fName+"_BottomFlange_001", "867-5309");
  fHoopPtr = new MJSTCMainBodyHoop(fName+"_Hoop_001", "867-5309");
  fTopFlangePtr = new MJSTCMainBody8Flange6Bore(fName+"_TopFlange_001", "867-5309");
  fBlankFlangePtr = new MJSTCMainBodyBlankFlange(fName+"_BlankFlange_001", "867-5309");
}

MJSTCMainBodyAssembly::MJSTCMainBodyAssembly(const MJSTCMainBodyAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCanPtr = new MJSTCMainBodyCan(fName+"_Can_001", "867-5309");
  fBottomFlangePtr = new MJSTCMainBody8Flange6Bore(fName+"_BottomFlange_001", "867-5309");
  fHoopPtr = new MJSTCMainBodyHoop(fName+"_Hoop_001", "867-5309");
  fTopFlangePtr = new MJSTCMainBody8Flange6Bore(fName+"_TopFlange_001", "867-5309");
  fBlankFlangePtr = new MJSTCMainBodyBlankFlange(fName+"_BlankFlange_001", "867-5309");
}

MJSTCMainBodyAssembly::~MJSTCMainBodyAssembly()
{
  delete fCanPtr;
  delete fBottomFlangePtr;
  delete fHoopPtr;
  delete fTopFlangePtr;
  delete fBlankFlangePtr;
}

void MJSTCMainBodyAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *canlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *canlocalRot = new G4RotationMatrix();
  G4AffineTransform *canaffine = new G4AffineTransform(canlocalRot,*canlocalPos);
  *canaffine*=*assemAffine;  
  G4ThreeVector *canglobalPos = new G4ThreeVector(canaffine->NetTranslation());
  G4RotationMatrix *canglobalRot= new G4RotationMatrix(canaffine->NetRotation());
  fCanPtr->Place(canglobalPos, canglobalRot, motherLogical);
  delete canlocalPos;
  delete canlocalRot;
  delete canaffine;
  
  G4ThreeVector *bottomflangelocalPos = new G4ThreeVector(0, 0, 22.352*mm+eps);
  G4RotationMatrix *bottomflangelocalRot = new G4RotationMatrix();
  G4AffineTransform *bottomflangeaffine = new G4AffineTransform(bottomflangelocalRot,*bottomflangelocalPos);
  *bottomflangeaffine*=*assemAffine;  
  G4ThreeVector *bottomflangeglobalPos = new G4ThreeVector(bottomflangeaffine->NetTranslation());
  G4RotationMatrix *bottomflangeglobalRot= new G4RotationMatrix(bottomflangeaffine->NetRotation());
  fBottomFlangePtr->Place(bottomflangeglobalPos, bottomflangeglobalRot, motherLogical);
  delete bottomflangelocalPos;
  delete bottomflangelocalRot;
  delete bottomflangeaffine;
 
  G4ThreeVector *hooplocalPos = new G4ThreeVector(0, 0, (22.352+50.8-9.652)*mm+2.*eps); 
  G4RotationMatrix *hooplocalRot = new G4RotationMatrix();
  G4AffineTransform *hoopaffine = new G4AffineTransform(hooplocalRot,*hooplocalPos);
  *hoopaffine*=*assemAffine;  
  G4ThreeVector *hoopglobalPos = new G4ThreeVector(hoopaffine->NetTranslation());
  G4RotationMatrix *hoopglobalRot= new G4RotationMatrix(hoopaffine->NetRotation());
  fHoopPtr->Place(hoopglobalPos, hoopglobalRot, motherLogical);
  delete hooplocalPos;
  delete hooplocalRot;
  delete hoopaffine;   
  
  G4ThreeVector *topflangelocalPos = new G4ThreeVector(0, 0, (22.352+50.8-9.652+50.8-9.652)*mm+3.*eps);
  G4RotationMatrix *topflangelocalRot = new G4RotationMatrix();
  topflangelocalRot->rotateX(pi);
  G4AffineTransform *topflangeaffine = new G4AffineTransform(topflangelocalRot,*topflangelocalPos);
  *topflangeaffine*=*assemAffine;  
  G4ThreeVector *topflangeglobalPos = new G4ThreeVector(topflangeaffine->NetTranslation());
  G4RotationMatrix *topflangeglobalRot= new G4RotationMatrix(topflangeaffine->NetRotation());
  fTopFlangePtr->Place(topflangeglobalPos, topflangeglobalRot, motherLogical);
  delete topflangelocalPos;
  delete topflangelocalRot;
  delete topflangeaffine;

  G4ThreeVector *blankflangelocalPos = new G4ThreeVector(0, 0, (22.352+50.8-9.652+50.8-9.652+22.352)*mm+4.*eps); 
  G4RotationMatrix *blankflangelocalRot = new G4RotationMatrix();
  blankflangelocalRot->rotateX(pi);
  G4AffineTransform *blankflangeaffine = new G4AffineTransform(blankflangelocalRot,*blankflangelocalPos);
  *blankflangeaffine*=*assemAffine;  
  G4ThreeVector *blankflangeglobalPos = new G4ThreeVector(blankflangeaffine->NetTranslation());
  G4RotationMatrix *blankflangeglobalRot= new G4RotationMatrix(blankflangeaffine->NetRotation());
  fBlankFlangePtr->Place(blankflangeglobalPos, blankflangeglobalRot, motherLogical);
  delete blankflangelocalPos;
  delete blankflangelocalRot;
  delete blankflangeaffine;

  delete assemAffine;  
}
