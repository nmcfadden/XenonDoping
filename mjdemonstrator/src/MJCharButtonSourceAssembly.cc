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
// CLASS IMPLEMENTATION:  MJCharButtonSourceAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Assembly origin: center of both discs (same as ButtonSource)
 *
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJCharButtonSourceAssembly.hh"
#include "mjdemonstrator/MJCharButtonSource.hh"
#include "mjdemonstrator/MJCharButtonSourceCase.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJCharButtonSourceAssembly::MJCharButtonSourceAssembly(G4String fName, G4String serialNumber, G4String sourceMaterialName) :
  MJVDemoAssembly(fName, serialNumber, "ButtonSourceAssembly")
{
  fSourceMaterialName = sourceMaterialName;
  fButtonSourcePtr = new MJCharButtonSource(fName+"_ButtonSource_001", "867-5309");
  fButtonSourcePtr->SetPartMaterial(fSourceMaterialName);
  fButtonSourceCasePtr = new MJCharButtonSourceCase(fName+"_ButtonSourceCase_001", "867-5309");
}


MJCharButtonSourceAssembly::MJCharButtonSourceAssembly(const MJCharButtonSourceAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fSourceMaterialName = rhs.GetSourceMaterialName();
  fButtonSourcePtr = new MJCharButtonSource(fName+"_ButtonSource_001", "867-5309");
  fButtonSourcePtr->SetPartMaterial(fSourceMaterialName);
  fButtonSourceCasePtr = new MJCharButtonSourceCase(fName+"_ButtonSourceCase_001", "867-5309");
}

MJCharButtonSourceAssembly::~MJCharButtonSourceAssembly()
{
  delete fButtonSourcePtr;
  delete fButtonSourceCasePtr;
}

void MJCharButtonSourceAssembly::Place(G4ThreeVector* assemPosition, 
				      G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  
  // source is placed in case, no translation necessary
  G4ThreeVector *buttonSourcelocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *buttonSourcelocalRot = new G4RotationMatrix();
  G4AffineTransform *buttonSourceaffine = new G4AffineTransform(buttonSourcelocalRot,*buttonSourcelocalPos);
  *buttonSourceaffine*=*assemAffine;  
  G4ThreeVector *buttonSourceglobalPos = new G4ThreeVector(buttonSourceaffine->NetTranslation());
  G4RotationMatrix *buttonSourceglobalRot= new G4RotationMatrix(buttonSourceaffine->NetRotation());
  fButtonSourcePtr->Place(buttonSourceglobalPos, buttonSourceglobalRot, motherLogical);
  delete buttonSourcelocalPos;
  delete buttonSourcelocalRot;
  delete buttonSourceaffine;

  G4ThreeVector *buttonSourceCaselocalPos = new G4ThreeVector(0,0,0);
  G4RotationMatrix *buttonSourceCaselocalRot = new G4RotationMatrix();
  G4AffineTransform *buttonSourceCaseaffine = new G4AffineTransform(buttonSourceCaselocalRot,*buttonSourceCaselocalPos);
  *buttonSourceCaseaffine*=*assemAffine;  
  G4ThreeVector *buttonSourceCaseglobalPos = new G4ThreeVector(buttonSourceCaseaffine->NetTranslation());
  G4RotationMatrix *buttonSourceCaseglobalRot = new G4RotationMatrix(buttonSourceCaseaffine->NetRotation());
  fButtonSourceCasePtr->Place(buttonSourceCaseglobalPos, buttonSourceCaseglobalRot, motherLogical);
  delete buttonSourceCaselocalPos;
  delete buttonSourceCaselocalRot;
  delete buttonSourceCaseaffine;
}





