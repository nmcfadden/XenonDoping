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
// CLASS IMPLEMENTATION:  MJSTCDewarAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from DWG No X3314 from Kadel Engineering
 * Many dimensions weren't in the drawing and they were approximated for this geometry.
 * Some small details are not included.
 *
 * Assembly Origin: Top of the dewar
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION:
 * 10-17-2013, Created, J. MacMullin
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
#include "mjdemonstrator/MJSTCDewarAssembly.hh"
#include "mjdemonstrator/MJSTCDewarIntExt.hh"
#include "mjdemonstrator/MJSTCDewarInsulation.hh"
#include "mjdemonstrator/MJSTCDewarLN.hh"
#include "mjdemonstrator/MJSTCDewarActChar.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJSTCDewarAssembly::MJSTCDewarAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Dewar")
{
  fIntExtPtr = new MJSTCDewarIntExt(fName+"_IntExt_001", "867-5309");
  fInsulationPtr = new MJSTCDewarInsulation(fName+"_Insulation_001", "867-5309");
  fLNPtr = new MJSTCDewarLN(fName+"_LN_001", "867-5309");
  fActCharPtr = new MJSTCDewarActChar(fName+"_ActChar_001", "867-5309");
}

MJSTCDewarAssembly::MJSTCDewarAssembly(const MJSTCDewarAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fIntExtPtr = new MJSTCDewarIntExt(fName+"_IntExt_001", "867-5309");
  fInsulationPtr = new MJSTCDewarInsulation(fName+"_Insulation_001", "867-5309");
  fLNPtr = new MJSTCDewarLN(fName+"_LN_001", "867-5309");
  fActCharPtr = new MJSTCDewarActChar(fName+"_ActChar_001", "867-5309");
}

MJSTCDewarAssembly::~MJSTCDewarAssembly()
{
  delete fIntExtPtr;
  delete fInsulationPtr;
  delete fLNPtr;
  delete fActCharPtr;
}

void MJSTCDewarAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *intextlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *intextlocalRot = new G4RotationMatrix();
  G4AffineTransform *intextaffine = new G4AffineTransform(intextlocalRot,*intextlocalPos);
  *intextaffine*=*assemAffine;  
  G4ThreeVector *intextglobalPos = new G4ThreeVector(intextaffine->NetTranslation());
  G4RotationMatrix *intextglobalRot= new G4RotationMatrix(intextaffine->NetRotation());
  fIntExtPtr->Place(intextglobalPos, intextglobalRot, motherLogical);
  delete intextlocalPos;
  delete intextlocalRot;
  delete intextaffine;

  G4ThreeVector *insulationlocalPos = new G4ThreeVector(0, 0, -(((574.802-2.0*11.43)/2.0)+11.43)*mm);
  G4RotationMatrix *insulationlocalRot = new G4RotationMatrix();
  G4AffineTransform *insulationaffine = new G4AffineTransform(insulationlocalRot,*insulationlocalPos);
  *insulationaffine*=*assemAffine;  
  G4ThreeVector *insulationglobalPos = new G4ThreeVector(insulationaffine->NetTranslation());
  G4RotationMatrix *insulationglobalRot= new G4RotationMatrix(insulationaffine->NetRotation());
  fInsulationPtr->Place(insulationglobalPos, insulationglobalRot, motherLogical);
  delete insulationlocalPos;
  delete insulationlocalRot;
  delete insulationaffine;

  G4ThreeVector *lnlocalPos = new G4ThreeVector(0, 0, (-11.43+0.03)*mm);
  G4RotationMatrix *lnlocalRot = new G4RotationMatrix();
  G4AffineTransform *lnaffine = new G4AffineTransform(lnlocalRot,*lnlocalPos);
  *lnaffine*=*assemAffine;  
  G4ThreeVector *lnglobalPos = new G4ThreeVector(lnaffine->NetTranslation());
  G4RotationMatrix *lnglobalRot= new G4RotationMatrix(lnaffine->NetRotation());
  fLNPtr->Place(lnglobalPos, lnglobalRot, motherLogical);
  delete lnlocalPos;
  delete lnlocalRot;
  delete lnaffine;

  G4ThreeVector *actcharlocalPos = new G4ThreeVector(0, 0, -(11.43+520.7-49.0682)*mm);
  G4RotationMatrix *actcharlocalRot = new G4RotationMatrix();
  G4AffineTransform *actcharaffine = new G4AffineTransform(actcharlocalRot,*actcharlocalPos);
  *actcharaffine*=*assemAffine;  
  G4ThreeVector *actcharglobalPos = new G4ThreeVector(actcharaffine->NetTranslation());
  G4RotationMatrix *actcharglobalRot= new G4RotationMatrix(actcharaffine->NetRotation());
  fActCharPtr->Place(actcharglobalPos, actcharglobalRot, motherLogical);
  delete actcharlocalPos;
  delete actcharlocalRot;
  delete actcharaffine;    
    
  delete assemAffine;  
}
