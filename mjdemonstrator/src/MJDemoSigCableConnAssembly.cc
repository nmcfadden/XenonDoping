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
// CLASS IMPLEMENTATION:  MJDemoSigCableConnAssembly.cc
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
 * FIRST SUBMISSION: Jan 14, 2011
 * 
 * REVISION:
 * 
 * 01-14-2011, Created, M. Green
 *
 * Rev 2: 01-10-2013, W. Xu, wxu@lanl.gov. Use  
 *
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
#include "mjdemonstrator/MJDemoSigCableConnAssembly.hh"
#include "mjdemonstrator/MJDemoSigCableConnSilica.hh"
#include "mjdemonstrator/MJDemoSigCableConnClamp.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSigCableConnAssembly::MJDemoSigCableConnAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "SigCableConn")
{
  fClampPtr = new MJDemoSigCableConnClamp(fName+"_Clamp_001", "867-5309");
  fSilicaPtr = new MJDemoSigCableConnSilica(fName+"_Silica_001", "867-5309"); 
}

MJDemoSigCableConnAssembly::MJDemoSigCableConnAssembly(const MJDemoSigCableConnAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fClampPtr = new MJDemoSigCableConnClamp(fName+"_Clamp_001", "867-5309");
  fSilicaPtr = new MJDemoSigCableConnSilica(fName+"_Silica_001", "867-5309");   
}

MJDemoSigCableConnAssembly::~MJDemoSigCableConnAssembly()
{
  delete fClampPtr;
  delete fSilicaPtr;
}

void MJDemoSigCableConnAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *clamplocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *clamplocalRot = new G4RotationMatrix();
  G4AffineTransform *clampaffine = new G4AffineTransform(clamplocalRot,*clamplocalPos);
  *clampaffine*=*assemAffine;  
  G4ThreeVector *clampglobalPos = new G4ThreeVector(clampaffine->NetTranslation());
  G4RotationMatrix *clampglobalRot= new G4RotationMatrix(clampaffine->NetRotation());
  fClampPtr->Place(clampglobalPos, clampglobalRot, motherLogical);
  delete clamplocalPos;
  delete clamplocalRot;
  delete clampaffine;
  
  G4ThreeVector *silicalocalPos = new G4ThreeVector(0, -0.3467*25.4*mm, 0.126*25.4*mm);
  G4RotationMatrix *silicalocalRot = new G4RotationMatrix();
  G4AffineTransform *silicaaffine = new G4AffineTransform(silicalocalRot,*silicalocalPos);
  *silicaaffine*=*assemAffine;  
  G4ThreeVector *silicaglobalPos = new G4ThreeVector(silicaaffine->NetTranslation());
  G4RotationMatrix *silicaglobalRot= new G4RotationMatrix(silicaaffine->NetRotation());
  fSilicaPtr->Place(silicaglobalPos, silicaglobalRot, motherLogical);
  delete silicalocalPos;
  delete silicalocalRot;
  delete silicaaffine;



  delete assemAffine;
}
