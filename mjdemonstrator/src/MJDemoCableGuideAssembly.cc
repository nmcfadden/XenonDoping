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
/**                                                            
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoCableGuideAssembly.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Inner Cable guide with clip inserts
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Apr 27, 2015
 * 
 * REVISION:
 * 
 * 04-27-2015, Created, K. Vorren
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"
#include "G4UIcommand.hh"
//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJDemoCableGuideAssembly.hh"
#include "mjdemonstrator/MJDemoInnerCableGuide.hh"
#include "mjdemonstrator/MJDemoCableGuideInsert.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJDemoCableGuideAssembly::MJDemoCableGuideAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ81-01-008B")
{
  fInnerCableGuidePtr = new MJDemoInnerCableGuide(fName + "_InnerCableGuide_001", "867-5309");
  fGuideInsert1Ptr = new MJDemoCableGuideInsert(fName + "_CableGuideInsert_001", "867-5309");
  fGuideInsert2Ptr = new MJDemoCableGuideInsert(fName + "_CableGuideInsert_002", "867-5309");
}

MJDemoCableGuideAssembly::MJDemoCableGuideAssembly(const MJDemoCableGuideAssembly & rhs) : 
  MJVDemoAssembly(rhs) {}

MJDemoCableGuideAssembly::~MJDemoCableGuideAssembly()
{
  delete fInnerCableGuidePtr;
  delete fGuideInsert1Ptr;
  delete fGuideInsert2Ptr;
}

void MJDemoCableGuideAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *cableGuideLocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *cableGuideLocalRot = new G4RotationMatrix();
  G4AffineTransform *cableGuideAffine = new G4AffineTransform(cableGuideLocalRot,*cableGuideLocalPos);
  *cableGuideAffine *= *assemAffine;  
  G4ThreeVector *cableGuideGlobalPos = new G4ThreeVector(cableGuideAffine->NetTranslation());
  G4RotationMatrix *cableGuideGlobalRot= new G4RotationMatrix(cableGuideAffine->NetRotation());
  fInnerCableGuidePtr->Place(cableGuideGlobalPos, cableGuideGlobalRot, motherLogical);
  delete cableGuideLocalPos;
  delete cableGuideLocalRot;
  delete cableGuideAffine;

  G4ThreeVector *guideInsert1localPos = new G4ThreeVector(-0.458*25.4*mm, -0.0025*25.4*mm, 0.11*25.4*mm);
  G4RotationMatrix *guideInsert1localRot = new G4RotationMatrix();
  guideInsert1localRot->rotateX(-pi/2);
  guideInsert1localRot->rotateY(197*deg);
  G4AffineTransform *guideInsert1affine = new G4AffineTransform(guideInsert1localRot,*guideInsert1localPos);
  *guideInsert1affine *= *assemAffine;  
  G4ThreeVector *guideInsert1globalPos = new G4ThreeVector(guideInsert1affine->NetTranslation());
  G4RotationMatrix *guideInsert1globalRot= new G4RotationMatrix(guideInsert1affine->NetRotation());
  fGuideInsert1Ptr->Place(guideInsert1globalPos, guideInsert1globalRot, motherLogical);
  delete guideInsert1localPos;
  delete guideInsert1localRot;
  delete guideInsert1affine;

  G4ThreeVector *guideInsert2localPos = new G4ThreeVector(0.458*25.4*mm, -0.0025*25.4*mm, 0.11*25.4*mm);
  G4RotationMatrix *guideInsert2localRot = new G4RotationMatrix();
  guideInsert2localRot->rotateX(-pi/2);
  guideInsert2localRot->rotateY(-17*deg);
  G4AffineTransform *guideInsert2affine = new G4AffineTransform(guideInsert2localRot,*guideInsert2localPos);
  *guideInsert2affine *= *assemAffine;  
  G4ThreeVector *guideInsert2globalPos = new G4ThreeVector(guideInsert2affine->NetTranslation());
  G4RotationMatrix *guideInsert2globalRot= new G4RotationMatrix(guideInsert2affine->NetRotation());
  fGuideInsert2Ptr->Place(guideInsert2globalPos, guideInsert2globalRot, motherLogical);
  delete guideInsert2localPos;
  delete guideInsert2localRot;
  delete guideInsert2affine;
}
