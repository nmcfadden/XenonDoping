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
// CLASS IMPLEMENTATION:  MJDemoLMFEAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Center of substrate, at trace level
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
#include "mjdemonstrator/MJDemoLMFEAssembly.hh"
#include "mjdemonstrator/MJDemoLMFESubstrate.hh"
#include "mjdemonstrator/MJDemoLMFETraces.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoLMFEAssembly::MJDemoLMFEAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "LMFE")
{
  fSubstratePtr = new MJDemoLMFESubstrate(fName+"_LMFESubstrate_001", "867-5309");  
  fTracesPtr = new MJDemoLMFETraces(fName+"_LMFETraces_001", "867-5309");  
}

MJDemoLMFEAssembly::MJDemoLMFEAssembly(const MJDemoLMFEAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fSubstratePtr = new MJDemoLMFESubstrate(fName+"_LMFESubstrate_001", "867-5309");  
  fTracesPtr = new MJDemoLMFETraces(fName+"_LMFETraces_001", "867-5309");  
}

MJDemoLMFEAssembly::~MJDemoLMFEAssembly()
{
  delete fTracesPtr;
  delete fSubstratePtr;
}

void MJDemoLMFEAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *LMFESubstratelocalPos = new G4ThreeVector(8.81*mm, 0, -0.125*mm);
  G4RotationMatrix *LMFESubstratelocalRot = new G4RotationMatrix();
  G4AffineTransform *LMFESubstrateaffine = new G4AffineTransform(LMFESubstratelocalRot,*LMFESubstratelocalPos);
  *LMFESubstrateaffine*=*assemAffine;  
  G4ThreeVector *LMFESubstrateglobalPos = new G4ThreeVector(LMFESubstrateaffine->NetTranslation());
  G4RotationMatrix *LMFESubstrateglobalRot= new G4RotationMatrix(LMFESubstrateaffine->NetRotation());
  fSubstratePtr->Place(LMFESubstrateglobalPos, LMFESubstrateglobalRot, motherLogical);
  delete LMFESubstratelocalPos;
  delete LMFESubstratelocalRot;
  delete LMFESubstrateaffine;
  
  G4ThreeVector *LMFETraceslocalPos = new G4ThreeVector(0, 0, 1000*nm);//215nm
  G4RotationMatrix *LMFETraceslocalRot = new G4RotationMatrix();
  G4AffineTransform *LMFETracesaffine = new G4AffineTransform(LMFETraceslocalRot,*LMFETraceslocalPos);
  *LMFETracesaffine*=*assemAffine;  
  G4ThreeVector *LMFETracesglobalPos = new G4ThreeVector(LMFETracesaffine->NetTranslation());
  G4RotationMatrix *LMFETracesglobalRot= new G4RotationMatrix(LMFETracesaffine->NetRotation());
  fTracesPtr->Place(LMFETracesglobalPos, LMFETracesglobalRot, motherLogical);
  delete LMFETraceslocalPos;
  delete LMFETraceslocalRot;
  delete LMFETracesaffine;


  delete assemAffine;
}
