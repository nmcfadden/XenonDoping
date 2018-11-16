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
// CLASS IMPLEMENTATION:  MJCharStandAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * This is the characterization stand for the test cryostat.
 * Based off engineering drawings from Matthew Busch
 * Assembly calls parts as follows:
 *
 * MJCharStandAssembly |-> MJCharButtonSourceAssembly |-> MJCharButtonSource
 *                    |                             |-> MJCharButtonSourceCase
 *                    |-> MJCharCollimator
 *                    |
 *                    |-> MJCharCollimatorHousingAssembly |-> MJCharCollimatorHousing
 *                    |                                  |-> MJChar832Screw x 4
 *                    |                                  |-> MJChar1420Screw x 4
 *                    |-> MJCharCollimatorMtgPlate
 *
 * Assembly origin:  center of top (closer to MtgPlate) button source face
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
#include "mjdemonstrator/MJCharStandAssembly.hh"
#include "mjdemonstrator/MJCharButtonSourceAssembly.hh"
#include "mjdemonstrator/MJCharCollimator.hh"
#include "mjdemonstrator/MJCharCollimatorHousingAssembly.hh"
#include "mjdemonstrator/MJCharCollimatorMtgPlate.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJCharStandAssembly::MJCharStandAssembly(G4String fName, G4String serialNumber, G4String sourceMaterialName) :
  MJVDemoAssembly(fName, serialNumber, "CalibrationStand")
{
  fSourceMaterialName = sourceMaterialName;
  fButtonSourceAssemblyPtr = new MJCharButtonSourceAssembly(fName+"_ButtonSourceAssembly_001", "867-5309", fSourceMaterialName);
  fCollimatorPtr = new MJCharCollimator(fName+"_Collimator_001", "867-5309");
  fCollimatorHousingAssemblyPtr = new MJCharCollimatorHousingAssembly(fName+"_CollimatorHousingAssembly_001", "867-5309");
  fCollimatorMtgPlatePtr = new MJCharCollimatorMtgPlate(fName+"_CollimatorMtgPlate_001", "867-5309");
}


MJCharStandAssembly::MJCharStandAssembly(const MJCharStandAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fSourceMaterialName = rhs.GetSourceMaterialName();
  fButtonSourceAssemblyPtr = new MJCharButtonSourceAssembly(fName+"_ButtonSourceAssembly_001", "867-5309", fSourceMaterialName);
  fCollimatorPtr = new MJCharCollimator(fName+"_Collimator_001", "867-5309");
  fCollimatorHousingAssemblyPtr = new MJCharCollimatorHousingAssembly(fName+"_CollimatorHousingAssembly_001", "867-5309");
  fCollimatorMtgPlatePtr = new MJCharCollimatorMtgPlate(fName+"_CollimatorMtgPlate_001", "867-5309");
}

MJCharStandAssembly::~MJCharStandAssembly()
{
  delete fButtonSourceAssemblyPtr;
  delete fCollimatorPtr;
  delete fCollimatorHousingAssemblyPtr;
  delete fCollimatorMtgPlatePtr;
}

void MJCharStandAssembly::Place(G4ThreeVector* assemPosition, 
			       G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *buttonSourcelocalPos = new G4ThreeVector(0, 0, -0.11*25.4*mm);
  G4RotationMatrix *buttonSourcelocalRot = new G4RotationMatrix();
  buttonSourcelocalRot->rotateZ(pi);
  G4AffineTransform *buttonSourceaffine = new G4AffineTransform(buttonSourcelocalRot,*buttonSourcelocalPos);
  *buttonSourceaffine*=*assemAffine;  
  G4ThreeVector *buttonSourceglobalPos = new G4ThreeVector(buttonSourceaffine->NetTranslation());
  G4RotationMatrix *buttonSourceglobalRot= new G4RotationMatrix(buttonSourceaffine->NetRotation());
  fButtonSourceAssemblyPtr->Place(buttonSourceglobalPos, buttonSourceglobalRot, motherLogical);
  delete buttonSourcelocalPos;
  delete buttonSourcelocalRot;
  delete buttonSourceaffine;
  
  G4ThreeVector *collimatorlocalPos = new G4ThreeVector(0, 0, -1.7*25.4*mm);
  G4RotationMatrix *collimatorlocalRot = new G4RotationMatrix();
  collimatorlocalRot->rotateZ(pi);
  G4AffineTransform *collimatoraffine = new G4AffineTransform(collimatorlocalRot,*collimatorlocalPos);
  *collimatoraffine*=*assemAffine;  
  G4ThreeVector *collimatorglobalPos = new G4ThreeVector(collimatoraffine->NetTranslation());
  G4RotationMatrix *collimatorglobalRot= new G4RotationMatrix(collimatoraffine->NetRotation());
  fCollimatorPtr->Place(collimatorglobalPos, collimatorglobalRot, motherLogical);
  delete collimatorlocalPos;
  delete collimatorlocalRot;
  delete collimatoraffine;

  G4ThreeVector *collimatorHousinglocalPos = new G4ThreeVector(0, 0, -1.765*25.4*mm);
  G4RotationMatrix *collimatorHousinglocalRot = new G4RotationMatrix();
  collimatorHousinglocalRot->rotateZ(pi);
  G4AffineTransform *collimatorHousingaffine = new G4AffineTransform(collimatorHousinglocalRot,*collimatorHousinglocalPos);
  *collimatorHousingaffine*=*assemAffine;  
  G4ThreeVector *collimatorHousingglobalPos = new G4ThreeVector(collimatorHousingaffine->NetTranslation());
  G4RotationMatrix *collimatorHousingglobalRot= new G4RotationMatrix(collimatorHousingaffine->NetRotation());
  fCollimatorHousingAssemblyPtr->Place(collimatorHousingglobalPos, collimatorHousingglobalRot, motherLogical);
  delete collimatorHousinglocalPos;
  delete collimatorHousinglocalRot;
  delete collimatorHousingaffine;

  // The mounting plate origin is not coaxial with the collimator, must be moved over by 0.125 inches
  G4ThreeVector *collimatorMtgPlatelocalPos = new G4ThreeVector(0.125*25.4*mm, 0, 0.25*25.4*mm);
  G4RotationMatrix *collimatorMtgPlatelocalRot = new G4RotationMatrix();
  collimatorMtgPlatelocalRot->rotateZ(pi);
  G4AffineTransform *collimatorMtgPlateaffine = new G4AffineTransform(collimatorMtgPlatelocalRot,*collimatorMtgPlatelocalPos);
  *collimatorMtgPlateaffine*=*assemAffine;  
  G4ThreeVector *collimatorMtgPlateglobalPos = new G4ThreeVector(collimatorMtgPlateaffine->NetTranslation());
  G4RotationMatrix *collimatorMtgPlateglobalRot= new G4RotationMatrix(collimatorMtgPlateaffine->NetRotation());
  fCollimatorMtgPlatePtr->Place(collimatorMtgPlateglobalPos, collimatorMtgPlateglobalRot, motherLogical);
  delete collimatorMtgPlatelocalPos;
  delete collimatorMtgPlatelocalRot;
  delete collimatorMtgPlateaffine;

}





