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
// CLASS IMPLEMENTATION:  MJDemoSigConnPlugAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Coaxial with Z-axis, in center of wide face.
 *                  Flat is parallel to XZ-Plane
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 04, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Pins and Solder blobs are now represented by a single part
 *             rather than 8, I. Guinn
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
#include "mjdemonstrator/MJDemoSigConnPlugAssembly.hh"
#include "mjdemonstrator/MJDemoSigConnHousing.hh"
#include "mjdemonstrator/MJDemoSigConnPins.hh"
#include "mjdemonstrator/MJDemoSigConnSolder.hh"
#include "mjdemonstrator/MJDemoSigConnStrainRelief.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSigConnPlugAssembly::MJDemoSigConnPlugAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "SignalPlug")
{
  fHousingPtr = new MJDemoSigConnHousing(fName+"_Housing_001", "867-5309");
  fStrRelPtr = new MJDemoSigConnStrainRelief(fName+"_StrainRelief_001", "867-5309");
  fPinsPtr = new MJDemoSigConnPins(fName+"_pin_001", "867-5309");
  fSolderPtr = new MJDemoSigConnSolder(fName+"_solder_001", "867-5309");
}

MJDemoSigConnPlugAssembly::MJDemoSigConnPlugAssembly(const MJDemoSigConnPlugAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fHousingPtr = new MJDemoSigConnHousing(fName+"_Housing_001", "867-5309");
  fStrRelPtr = new MJDemoSigConnStrainRelief(fName+"_StrainRelief_001", "867-5309");
  fPinsPtr = new MJDemoSigConnPins(fName+"_pins_001", "867-5309");
  fSolderPtr = new MJDemoSigConnSolder(fName+"_solder_001", "867-5309");
}

MJDemoSigConnPlugAssembly::~MJDemoSigConnPlugAssembly()
{
  delete fHousingPtr;
  delete fStrRelPtr;
  delete fPinsPtr;
  delete fSolderPtr;
}

void MJDemoSigConnPlugAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  //Make sure all surfaces are separated by at least tolerance. This avoids any boundary crossing problems. This means that the pins will be fully inside of the vespel and the solder will not be in contact with the pins.
  G4double tolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation, *assemPosition);

  G4ThreeVector *pinLocalPos = new G4ThreeVector(0, 0, MJDemoSigConnPins::GetLength()/2);
  G4RotationMatrix *pinLocalRot = new G4RotationMatrix();
  G4AffineTransform *pinAffine = new G4AffineTransform(pinLocalRot, *pinLocalPos);
  *pinAffine *= *assemAffine;    
  G4ThreeVector *pinGlobalPos = new G4ThreeVector(pinAffine->NetTranslation());
  G4RotationMatrix *pinGlobalRot = new G4RotationMatrix(pinAffine->NetRotation());
  fPinsPtr->Place(pinGlobalPos, pinGlobalRot, motherLogical);
  delete pinAffine;
  delete pinLocalRot;
  delete pinLocalPos;
  
  
  G4ThreeVector *solderLocalPos = new G4ThreeVector(0, 0, MJDemoSigConnPins::GetLength() + MJDemoSigConnSolder::GetLength()/2 + tolerance);
  G4RotationMatrix *solderLocalRot = new G4RotationMatrix();
  G4AffineTransform *solderAffine = new G4AffineTransform(solderLocalRot, *solderLocalPos);
  *solderAffine *= *assemAffine;    
  G4ThreeVector *solderGlobalPos = new G4ThreeVector(solderAffine->NetTranslation());
  G4RotationMatrix *solderGlobalRot = new G4RotationMatrix(solderAffine->NetRotation());
  fSolderPtr->Place(solderGlobalPos, solderGlobalRot, motherLogical);
  delete solderAffine;
  delete solderLocalRot;
  delete solderLocalPos;
  
  G4ThreeVector *strRelLocalPos = new G4ThreeVector(0, 0, MJDemoSigConnPins::GetLength() + MJDemoSigConnSolder::GetLength() + MJDemoSigConnStrainRelief::GetLength()/2 + 3*tolerance);
  G4RotationMatrix *strRelLocalRot = new G4RotationMatrix();
  G4AffineTransform *strRelAffine = new G4AffineTransform(strRelLocalRot, *strRelLocalPos);
  *strRelAffine *= *assemAffine;
  G4ThreeVector *strRelGlobalPos = new G4ThreeVector(strRelAffine->NetTranslation());
  G4RotationMatrix *strRelGlobalRot = new G4RotationMatrix(strRelAffine->NetRotation());
  fStrRelPtr->Place(strRelGlobalPos, strRelGlobalRot, motherLogical);
  delete strRelLocalPos;
  delete strRelLocalRot;
  delete strRelAffine;
  
  G4ThreeVector *housingLocalPos = new G4ThreeVector();
  G4RotationMatrix *housingLocalRot = new G4RotationMatrix();
  G4AffineTransform *housingAffine = new G4AffineTransform(housingLocalRot, *housingLocalPos);
  *housingAffine *= *assemAffine;
  G4ThreeVector *housingGlobalPos = new G4ThreeVector(housingAffine->NetTranslation());
  G4RotationMatrix *housingGlobalRot = new G4RotationMatrix(housingAffine->NetRotation());
  fHousingPtr->Place(housingGlobalPos, housingGlobalRot, motherLogical);
  delete housingLocalPos;
  delete housingLocalRot;
  delete housingAffine;
  
  delete assemAffine;
}
