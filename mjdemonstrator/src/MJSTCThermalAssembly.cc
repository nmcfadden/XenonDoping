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
// CLASS IMPLEMENTATION:  MJSTCThermalAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Assembly Origin: The top of the IR Shield
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 11-14-2013, Created, J. MacMullin
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
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
#include "mjdemonstrator/MJSTCThermalAssembly.hh"
#include "mjdemonstrator/MJSTCThermalIRShield.hh"
#include "mjdemonstrator/MJSTCThermalCuFoil.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCThermalAssembly::MJSTCThermalAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Thermal")
{
  fIRShieldPtr = new MJSTCThermalIRShield(fName+"_IRShield_001", "867-5309");
  fCuFoilPtr = new MJSTCThermalCuFoil(fName+"_CuFoil_001", "867-5309");
}

MJSTCThermalAssembly::MJSTCThermalAssembly(const MJSTCThermalAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fIRShieldPtr = new MJSTCThermalIRShield(fName+"_IRShield_001", "867-5309");
  fCuFoilPtr = new MJSTCThermalCuFoil(fName+"_CuFoil_001", "867-5309");
}

MJSTCThermalAssembly::~MJSTCThermalAssembly()
{
  delete fIRShieldPtr;
  delete fCuFoilPtr;
}

void MJSTCThermalAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *irshieldlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *irshieldlocalRot = new G4RotationMatrix();
  G4AffineTransform *irshieldaffine = new G4AffineTransform(irshieldlocalRot,*irshieldlocalPos);
  *irshieldaffine*=*assemAffine;  
  G4ThreeVector *irshieldglobalPos = new G4ThreeVector(irshieldaffine->NetTranslation());
  G4RotationMatrix *irshieldglobalRot= new G4RotationMatrix(irshieldaffine->NetRotation());
  fIRShieldPtr->Place(irshieldglobalPos, irshieldglobalRot, motherLogical);
  delete irshieldlocalPos;
  delete irshieldlocalRot;
  delete irshieldaffine;

  G4ThreeVector *cufoillocalPos = new G4ThreeVector(0, 0, -((295.275-15.875)/2.+15.875)*mm-eps);
  G4RotationMatrix *cufoillocalRot = new G4RotationMatrix();
  cufoillocalRot->rotateZ(pi/3);
  G4AffineTransform *cufoilaffine = new G4AffineTransform(cufoillocalRot,*cufoillocalPos);
  *cufoilaffine*=*assemAffine;  
  G4ThreeVector *cufoilglobalPos = new G4ThreeVector(cufoilaffine->NetTranslation());
  G4RotationMatrix *cufoilglobalRot= new G4RotationMatrix(cufoilaffine->NetRotation());
  fCuFoilPtr->Place(cufoilglobalPos, cufoilglobalRot, motherLogical);
  delete cufoillocalPos;
  delete cufoillocalRot;
  delete cufoilaffine;  
    
  delete assemAffine;  
}
