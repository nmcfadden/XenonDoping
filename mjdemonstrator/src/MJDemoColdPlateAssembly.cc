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
// CLASS IMPLEMENTATION:  MJDemoColdPlateAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  COM of cold plate
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@ornl.gov
 * FIRST SUBMISSION: Sept 10, 2014
 * 
 * REVISION:
 *
 * 10-24-2014, Created - M. Green
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
#include "mjdemonstrator/MJDemoColdPlateAssembly.hh"
#include "mjdemonstrator/MJDemoColdplateCryo1Symmetric.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoColdPlateAssembly::MJDemoColdPlateAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "CPlateAssem")
{
  fCPPtr = new MJDemoColdplateCryo1Symmetric(fName+"_ColdPlate_001", "867-5309");
}

MJDemoColdPlateAssembly::MJDemoColdPlateAssembly(const MJDemoColdPlateAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCPPtr = new MJDemoColdplateCryo1Symmetric(fName+"_ColdPlate_001", "867-5309");
}

MJDemoColdPlateAssembly::~MJDemoColdPlateAssembly()
{
  delete fCPPtr;
}

void MJDemoColdPlateAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *CPlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *CPlocalRot = new G4RotationMatrix();
  CPlocalRot->rotateX(pi);
  CPlocalRot->rotateZ(pi/2);
  G4AffineTransform *CPaffine = new G4AffineTransform(CPlocalRot,*CPlocalPos);
  *CPaffine*=*assemAffine;  
  G4ThreeVector *CPglobalPos = new G4ThreeVector(CPaffine->NetTranslation());
  G4RotationMatrix *CPglobalRot= new G4RotationMatrix(CPaffine->NetRotation());
  fCPPtr->Place(CPglobalPos, CPglobalRot, motherLogical);
  delete CPlocalPos;
  delete CPlocalRot;
  delete CPaffine;

  delete assemAffine;
}
