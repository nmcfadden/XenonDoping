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
// CLASS IMPLEMENTATION:  MJSTCCMSAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Assembly Origin: The middle of the width (z) of the "baseplate" (the horizontally-flat part of the CMS stand)
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Tuesday January 21 2014
 * 
 * REVISION:
 * 01-21-2014, Created. Used to be a part of ColdPlateAssembly. I separated
 *             it to make it easier to rotate it through the messenger, J. MacMullin
 *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
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
#include "mjdemonstrator/MJSTCCMSAssembly.hh"
#include "mjdemonstrator/MJSTCCMSStand.hh"
#include "mjdemonstrator/MJSTCCMSSpool.hh"
#include "mjdemonstrator/MJSTCCMSGrooves.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCCMSAssembly::MJSTCCMSAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "CMS")
{
  fStandPtr = new MJSTCCMSStand(fName+"_Stand_001", "867-5309");
  fSpoolPtr = new MJSTCCMSSpool(fName+"_Spool_001", "867-5309");
  fGroovesPtr = new MJSTCCMSGrooves(fName+"_Grooves_001", "867-5309");
}

MJSTCCMSAssembly::MJSTCCMSAssembly(const MJSTCCMSAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fStandPtr = new MJSTCCMSStand(fName+"_Stand_001", "867-5309");
  fSpoolPtr = new MJSTCCMSSpool(fName+"_Spool_001", "867-5309");
  fGroovesPtr = new MJSTCCMSGrooves(fName+"_Grooves_001", "867-5309");
}

MJSTCCMSAssembly::~MJSTCCMSAssembly()
{
  delete fStandPtr;
  delete fSpoolPtr;
  delete fGroovesPtr;
}

void MJSTCCMSAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  
  G4ThreeVector *standlocalPos = new G4ThreeVector(0*mm, 0*mm, 0*mm);
  G4RotationMatrix *standlocalRot = new G4RotationMatrix();
  standlocalRot->rotateZ(4.0*pi/3.0);
  G4AffineTransform *standaffine = new G4AffineTransform(standlocalRot,*standlocalPos);
  *standaffine*=*assemAffine;  
  G4ThreeVector *standglobalPos = new G4ThreeVector(standaffine->NetTranslation());
  G4RotationMatrix *standglobalRot= new G4RotationMatrix(standaffine->NetRotation());
  fStandPtr->Place(standglobalPos, standglobalRot, motherLogical);
  delete standlocalPos;
  delete standlocalRot;
  delete standaffine; 

  G4ThreeVector *spoollocalPos = new G4ThreeVector(0*mm, 0*mm, ((3.175+2.0*6.35)+39.37-6.35)*mm+eps);
  G4RotationMatrix *spoollocalRot = new G4RotationMatrix();
  spoollocalRot->rotateZ(pi/3.0);
  G4AffineTransform *spoolaffine = new G4AffineTransform(spoollocalRot,*spoollocalPos);
  *spoolaffine*=*assemAffine;  
  G4ThreeVector *spoolglobalPos = new G4ThreeVector(spoolaffine->NetTranslation());
  G4RotationMatrix *spoolglobalRot= new G4RotationMatrix(spoolaffine->NetRotation());
  fSpoolPtr->Place(spoolglobalPos, spoolglobalRot, motherLogical);
  delete spoollocalPos;
  delete spoollocalRot;
  delete spoolaffine; 
  
  G4ThreeVector *grooveslocalPos = new G4ThreeVector(0*mm, 0, (((3.175+2.0*6.35)+39.37-6.35)-15.875)*mm+2.*eps);
  G4RotationMatrix *grooveslocalRot = new G4RotationMatrix();
  G4AffineTransform *groovesaffine = new G4AffineTransform(grooveslocalRot,*grooveslocalPos);
  *groovesaffine*=*assemAffine;  
  G4ThreeVector *groovesglobalPos = new G4ThreeVector(groovesaffine->NetTranslation());
  G4RotationMatrix *groovesglobalRot= new G4RotationMatrix(groovesaffine->NetRotation());
  fGroovesPtr->Place(groovesglobalPos, groovesglobalRot, motherLogical);
  delete grooveslocalPos;
  delete grooveslocalRot;
  delete groovesaffine;
  
  delete assemAffine;  
}
