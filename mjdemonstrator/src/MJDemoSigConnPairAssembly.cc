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
// CLASS IMPLEMENTATION:  MJDemoSigConnPairAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Centered in between both plugs
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 20, 2014
 * 
 * REVISION:
 * 
 * 06-20-2014, Created, I. Guinn
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
#include "mjdemonstrator/MJDemoSigConnPairAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSigConnPairAssembly::MJDemoSigConnPairAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "SignalPlug")
{
  fPlug1 = new MJDemoSigConnPlugAssembly(fName+"_Plug_001", "867-5309");
  fPlug2 = new MJDemoSigConnPlugAssembly(fName+"_Plug_002", "867-5309");
}

MJDemoSigConnPairAssembly::MJDemoSigConnPairAssembly(const MJDemoSigConnPairAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fPlug1 = new MJDemoSigConnPlugAssembly(fName+"_Plug_001", "867-5309");
  fPlug2 = new MJDemoSigConnPlugAssembly(fName+"_Plug_002", "867-5309");
}

MJDemoSigConnPairAssembly::~MJDemoSigConnPairAssembly()
{
  delete fPlug1;
  delete fPlug2;
}

void MJDemoSigConnPairAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double tolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation, *assemPosition);

  G4ThreeVector *plug1LocalPos = new G4ThreeVector(0, 0, tolerance);
  G4RotationMatrix *plug1LocalRot = new G4RotationMatrix();
  G4AffineTransform *plug1Affine = new G4AffineTransform(plug1LocalRot, *plug1LocalPos);
  *plug1Affine *= *assemAffine;
  G4ThreeVector *plug1GlobalPos = new G4ThreeVector(plug1Affine->NetTranslation());
  G4RotationMatrix *plug1GlobalRot = new G4RotationMatrix(plug1Affine->NetRotation());
  fPlug1->Place(plug1GlobalPos, plug1GlobalRot, motherLogical);
  delete plug1LocalPos;
  delete plug1LocalRot;
  delete plug1Affine;

  G4ThreeVector *plug2LocalPos = new G4ThreeVector(0, 0, -tolerance);
  G4RotationMatrix *plug2LocalRot = new G4RotationMatrix(CLHEP::HepRotationY(180*deg));
  G4AffineTransform *plug2Affine = new G4AffineTransform(plug2LocalRot, *plug2LocalPos);
  *plug2Affine *= *assemAffine;
  G4ThreeVector *plug2GlobalPos = new G4ThreeVector(plug2Affine->NetTranslation());
  G4RotationMatrix *plug2GlobalRot = new G4RotationMatrix(plug2Affine->NetRotation());
  fPlug2->Place(plug2GlobalPos, plug2GlobalRot, motherLogical);
  delete plug2LocalPos;
  delete plug2LocalRot;
  delete plug2Affine;

  delete assemAffine;
}
