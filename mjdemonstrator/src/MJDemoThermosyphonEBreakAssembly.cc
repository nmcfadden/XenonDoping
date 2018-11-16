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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonEBreakAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Center of sealing face of bottom flange
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Sept 28, 2015
 *
 * REVISION:
 *
 * 09-28-2015, Created, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonEBreakAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonFlange.hh"
#include "mjdemonstrator/MJDemoThermosyphonGlass.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonEBreakAssembly::MJDemoThermosyphonEBreakAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "LMFE")
{
  fBottomFlangePtr = new MJDemoThermosyphonFlange(fName+"_BottomFlange_001", "867-5309");
  fTopFlangePtr = new MJDemoThermosyphonFlange(fName+"_TopFlange_001", "867-5309");
  fGlassPtr = new MJDemoThermosyphonGlass(fName+"_Glass_001", "867-5309");
}

MJDemoThermosyphonEBreakAssembly::MJDemoThermosyphonEBreakAssembly(const MJDemoThermosyphonEBreakAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
    fBottomFlangePtr = new MJDemoThermosyphonFlange(fName+"_BottomFlange_001", "867-5309");
    fTopFlangePtr = new MJDemoThermosyphonFlange(fName+"_TopFlange_001", "867-5309");
    fGlassPtr = new MJDemoThermosyphonGlass(fName+"_Glass_001", "867-5309");
}

MJDemoThermosyphonEBreakAssembly::~MJDemoThermosyphonEBreakAssembly()
{
  delete fGlassPtr;
  delete fTopFlangePtr;
  delete fBottomFlangePtr;
}

void MJDemoThermosyphonEBreakAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
    
  G4ThreeVector *bottomflangelocalPos = new G4ThreeVector(0, 0, 1.07*25.4*mm);
  G4RotationMatrix *bottomflangelocalRot = new G4RotationMatrix();
  bottomflangelocalRot->rotateX(pi);
  G4AffineTransform *bottomflangeaffine = new G4AffineTransform(bottomflangelocalRot,*bottomflangelocalPos);
  *bottomflangeaffine*=*assemAffine;  
  G4ThreeVector *bottomflangeglobalPos = new G4ThreeVector(bottomflangeaffine->NetTranslation());
  G4RotationMatrix *bottomflangeglobalRot= new G4RotationMatrix(bottomflangeaffine->NetRotation());
  fBottomFlangePtr->Place(bottomflangeglobalPos, bottomflangeglobalRot, motherLogical);
  delete bottomflangelocalPos;
  delete bottomflangelocalRot;
  delete bottomflangeaffine;
  
  G4ThreeVector *topflangelocalPos = new G4ThreeVector(0, 0, 2.15*25.4*mm);
  G4RotationMatrix *topflangelocalRot = new G4RotationMatrix();
  G4AffineTransform *topflangeaffine = new G4AffineTransform(topflangelocalRot,*topflangelocalPos);
  *topflangeaffine*=*assemAffine;
  G4ThreeVector *topflangeglobalPos = new G4ThreeVector(topflangeaffine->NetTranslation());
  G4RotationMatrix *topflangeglobalRot= new G4RotationMatrix(topflangeaffine->NetRotation());
  fTopFlangePtr->Place(topflangeglobalPos, topflangeglobalRot, motherLogical);
  delete topflangelocalPos;
  delete topflangelocalRot;
  delete topflangeaffine;
  
  G4ThreeVector *glasslocalPos = new G4ThreeVector(0, 0, 1.61*25.4*mm);
  G4RotationMatrix *glasslocalRot = new G4RotationMatrix();
  G4AffineTransform *glassaffine = new G4AffineTransform(glasslocalRot,*glasslocalPos);
  *glassaffine*=*assemAffine;
  G4ThreeVector *glassglobalPos = new G4ThreeVector(glassaffine->NetTranslation());
  G4RotationMatrix *glassglobalRot= new G4RotationMatrix(glassaffine->NetRotation());
  fGlassPtr->Place(glassglobalPos, glassglobalRot, motherLogical);
  delete glasslocalPos;
  delete glasslocalRot;
  delete glassaffine;
    

  delete assemAffine;
}
