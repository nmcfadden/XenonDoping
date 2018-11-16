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
 * CLASS DECLARATION:  MJDemoHVForkAssembly.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Assembly of fork and vespel pin.
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

#include "mjdemonstrator/MJDemoHVForkAssembly.hh"
#include "mjdemonstrator/MJDemoHVFork.hh"
#include "mjdemonstrator/MJDemoHVForkClampPin.hh"
//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVForkAssembly::MJDemoHVForkAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-11-019V")
{
  fHVForkPtr = new MJDemoHVFork(fName + "_HVFork_001", "867-5309");
  fClampPinPtr = new MJDemoHVForkClampPin(fName + "_HVForkClampPin_001", "867-5309");
}

MJDemoHVForkAssembly::MJDemoHVForkAssembly(const MJDemoHVForkAssembly & rhs) : 
  MJVDemoAssembly(rhs) {}

MJDemoHVForkAssembly::~MJDemoHVForkAssembly()
{
  delete fHVForkPtr;
  delete fClampPinPtr;
}

void MJDemoHVForkAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *hvForkLocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *hvForkLocalRot = new G4RotationMatrix();
  G4AffineTransform *hvForkAffine = new G4AffineTransform(hvForkLocalRot,*hvForkLocalPos);
  *hvForkAffine *= *assemAffine;  
  G4ThreeVector *hvForkGlobalPos = new G4ThreeVector(hvForkAffine->NetTranslation());
  G4RotationMatrix *hvForkGlobalRot= new G4RotationMatrix(hvForkAffine->NetRotation());
  fHVForkPtr->Place(hvForkGlobalPos, hvForkGlobalRot, motherLogical);
  delete hvForkLocalPos;
  delete hvForkLocalRot;
  delete hvForkAffine;

  G4ThreeVector *clampPinLocalPos = new G4ThreeVector(0.19875*25.4*mm, -0.1*25.4*mm, 0.0701/2*25.4*mm);
  G4RotationMatrix *clampPinLocalRot = new G4RotationMatrix();
  G4AffineTransform *clampPinAffine = new G4AffineTransform(clampPinLocalRot,*clampPinLocalPos);
  *clampPinAffine *= *assemAffine;  
  G4ThreeVector *clampPinGlobalPos = new G4ThreeVector(clampPinAffine->NetTranslation());
  G4RotationMatrix *clampPinGlobalRot= new G4RotationMatrix(clampPinAffine->NetRotation());
  fClampPinPtr->Place(clampPinGlobalPos, clampPinGlobalRot, motherLogical);
  delete clampPinLocalPos;
  delete clampPinLocalRot;
  delete clampPinAffine;
}
