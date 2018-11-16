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
// CLASS IMPLEMENTATION:  MJCharCollimatorHousingAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Places four 8-32 screws and four 1/4-20 screws in the collimator housing
 * Assembly origin: center of external box (same as CollimatorHousing)
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
#include "mjdemonstrator/MJCharCollimatorHousingAssembly.hh"
#include "mjdemonstrator/MJCharCollimatorHousing.hh"
#include "mjdemonstrator/MJChar832Screw.hh"
#include "mjdemonstrator/MJChar1420Screw.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJCharCollimatorHousingAssembly::MJCharCollimatorHousingAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "CollimatorHousingAssembly")
{
  fCollimatorHousingPtr = new MJCharCollimatorHousing(fName+"_CollimatorHousing_001", "867-5309");
  f832Screw1Ptr = new MJChar832Screw(fName+"_832Screw_001", "867-5309", 2.0*25.4*mm);
  f832Screw2Ptr = new MJChar832Screw(fName+"_832Screw_002", "867-5309", 2.0*25.4*mm);
  f832Screw3Ptr = new MJChar832Screw(fName+"_832Screw_003", "867-5309", 2.0*25.4*mm);
  f832Screw4Ptr = new MJChar832Screw(fName+"_832Screw_004", "867-5309", 2.0*25.4*mm);
  f1420Screw1Ptr = new MJChar1420Screw(fName+"_1420Screw_001", "867-5309", (0.875)*25.4*mm);
  f1420Screw2Ptr = new MJChar1420Screw(fName+"_1420Screw_002", "867-5309", (0.875)*25.4*mm);
  f1420Screw3Ptr = new MJChar1420Screw(fName+"_1420Screw_003", "867-5309", (0.875)*25.4*mm);
  f1420Screw4Ptr = new MJChar1420Screw(fName+"_1420Screw_004", "867-5309", (0.875)*25.4*mm);
}


MJCharCollimatorHousingAssembly::MJCharCollimatorHousingAssembly(const MJCharCollimatorHousingAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCollimatorHousingPtr = new MJCharCollimatorHousing(fName+"_CollimatorHousing_001", "867-5309");
  f832Screw1Ptr = new MJChar832Screw(fName+"_832Screw_001", "867-5309", 2.0*25.4*mm);
  f832Screw2Ptr = new MJChar832Screw(fName+"_832Screw_002", "867-5309", 2.0*25.4*mm);
  f832Screw3Ptr = new MJChar832Screw(fName+"_832Screw_003", "867-5309", 2.0*25.4*mm);
  f832Screw4Ptr = new MJChar832Screw(fName+"_832Screw_004", "867-5309", 2.0*25.4*mm);
  f1420Screw1Ptr = new MJChar1420Screw(fName+"_1420Screw_001", "867-5309", (0.875)*25.4*mm);
  f1420Screw2Ptr = new MJChar1420Screw(fName+"_1420Screw_002", "867-5309", (0.875)*25.4*mm);
  f1420Screw3Ptr = new MJChar1420Screw(fName+"_1420Screw_003", "867-5309", (0.875)*25.4*mm);
  f1420Screw4Ptr = new MJChar1420Screw(fName+"_1420Screw_004", "867-5309", (0.875)*25.4*mm);
}

MJCharCollimatorHousingAssembly::~MJCharCollimatorHousingAssembly()
{
  delete fCollimatorHousingPtr;
  delete f832Screw1Ptr;
  delete f832Screw2Ptr;
  delete f832Screw3Ptr;
  delete f832Screw4Ptr;
  delete f1420Screw1Ptr;
  delete f1420Screw2Ptr;
  delete f1420Screw3Ptr;
  delete f1420Screw4Ptr;
}

void MJCharCollimatorHousingAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *collimatorHousinglocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *collimatorHousinglocalRot = new G4RotationMatrix();
  G4AffineTransform *collimatorHousingaffine = new G4AffineTransform(collimatorHousinglocalRot,*collimatorHousinglocalPos);
  *collimatorHousingaffine*=*assemAffine;  
  G4ThreeVector *collimatorHousingglobalPos = new G4ThreeVector(collimatorHousingaffine->NetTranslation());
  G4RotationMatrix *collimatorHousingglobalRot= new G4RotationMatrix(collimatorHousingaffine->NetRotation());
  fCollimatorHousingPtr->Place(collimatorHousingglobalPos, collimatorHousingglobalRot, motherLogical);
  delete collimatorHousinglocalPos;
  delete collimatorHousinglocalRot;
  delete collimatorHousingaffine;

  G4ThreeVector *screwlocalPos = new G4ThreeVector(1.2*25.4*mm, -0.905*25.4*mm, 0.765*25.4*mm);
  G4RotationMatrix *screwlocalRot = new G4RotationMatrix();
  screwlocalRot->rotateX(-pi/2.);
  G4AffineTransform *screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos1 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot1 = new G4RotationMatrix(screwaffine->NetRotation());
  f832Screw1Ptr->Place(screwglobalPos1, screwglobalRot1, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(1.2*25.4*mm, -0.905*25.4*mm, -1.015*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwlocalRot->rotateX(-pi/2.);
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos2 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot2 = new G4RotationMatrix(screwaffine->NetRotation());
  f832Screw2Ptr->Place(screwglobalPos2, screwglobalRot2, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(-1.2*25.4*mm, -0.905*25.4*mm, 0.765*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwlocalRot->rotateX(-pi/2.);
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos3 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot3 = new G4RotationMatrix(screwaffine->NetRotation());
  f832Screw3Ptr->Place(screwglobalPos3, screwglobalRot3, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(-1.2*25.4*mm, -0.905*25.4*mm, -1.015*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwlocalRot->rotateX(-pi/2.);
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos4 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot4 = new G4RotationMatrix(screwaffine->NetRotation());
  f832Screw4Ptr->Place(screwglobalPos4, screwglobalRot4, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(1.1875*25.4*mm, 0.5*25.4*mm, (0.25+3.53/2.0)*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos5 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot5 = new G4RotationMatrix(screwaffine->NetRotation());
  f1420Screw1Ptr->Place(screwglobalPos5, screwglobalRot5, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(1.1875*25.4*mm, -0.5*25.4*mm, (0.25+3.53/2.0)*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos6 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot6 = new G4RotationMatrix(screwaffine->NetRotation());
  f1420Screw2Ptr->Place(screwglobalPos6, screwglobalRot6, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(-1.1875*25.4*mm, 0.5*25.4*mm, (0.25+3.53/2.0)*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos7 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot7 = new G4RotationMatrix(screwaffine->NetRotation());
  f1420Screw3Ptr->Place(screwglobalPos7, screwglobalRot7, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

  screwlocalPos = new G4ThreeVector(-1.1875*25.4*mm, -0.5*25.4*mm, (0.25+3.53/2.0)*25.4*mm);
  screwlocalRot = new G4RotationMatrix();
  screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos8 = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot8 = new G4RotationMatrix(screwaffine->NetRotation());
  f1420Screw4Ptr->Place(screwglobalPos8, screwglobalRot8, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;

}





