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
// CLASS IMPLEMENTATION:  MJDemoHVCableAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Coincident with mount plate edge, coincident with crossarm axis
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 * 
 * REVISION:
 * 
 * 09-10-2010, Created, M. Green
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
#include "mjdemonstrator/MJDemoHVCableAssembly.hh"
#include "mjdemonstrator/MJDemoHVCableConductor.hh"
#include "mjdemonstrator/MJDemoHVCableInsulation.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVCableAssembly::MJDemoHVCableAssembly(G4String fName, G4String serialNumber, 
												G4double cableLength) :
  MJVDemoAssembly(fName, serialNumber, "HVCable"), fCableLength(cableLength)
{  
  fWirePtr = new MJDemoHVCableConductor(fName+"_HVWire_001", "867-5309", fCableLength);
  fInsulationPtr = new MJDemoHVCableInsulation(fName+"_HVInsulation_001", "867-5309", fCableLength);
}

MJDemoHVCableAssembly::MJDemoHVCableAssembly(const MJDemoHVCableAssembly & rhs) : 
  MJVDemoAssembly(rhs), fCableLength(rhs.fCableLength)
{
  fWirePtr = new MJDemoHVCableConductor(fName+"_HVWire_001", "867-5309", fCableLength);
  fInsulationPtr = new MJDemoHVCableInsulation(fName+"_HVInsulation_001", "867-5309", fCableLength);
}

MJDemoHVCableAssembly::~MJDemoHVCableAssembly()
{
  delete fWirePtr;
  delete fInsulationPtr;
}

void MJDemoHVCableAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *wirelocalPos = new G4ThreeVector(0, 0, -fCableLength/2);
  G4RotationMatrix *wirelocalRot = new G4RotationMatrix();
  G4AffineTransform *wireaffine = new G4AffineTransform(wirelocalRot,*wirelocalPos);
  *wireaffine*=*assemAffine;  
  G4ThreeVector *wireglobalPos = new G4ThreeVector(wireaffine->NetTranslation());
  G4RotationMatrix *wireglobalRot= new G4RotationMatrix(wireaffine->NetRotation());
  fWirePtr->Place(wireglobalPos, wireglobalRot, motherLogical);
  delete wirelocalPos;
  delete wirelocalRot;
  delete wireaffine;

  G4ThreeVector *insulationlocalPos = new G4ThreeVector(0, 0, -fCableLength/2);
  G4RotationMatrix *insulationlocalRot = new G4RotationMatrix();
  G4AffineTransform *insulationaffine = new G4AffineTransform(insulationlocalRot,*insulationlocalPos);
  *insulationaffine*=*assemAffine;  
  G4ThreeVector *insulationglobalPos = new G4ThreeVector(insulationaffine->NetTranslation());
  G4RotationMatrix *insulationglobalRot= new G4RotationMatrix(insulationaffine->NetRotation());
  fInsulationPtr->Place(insulationglobalPos, insulationglobalRot, motherLogical);
  delete insulationlocalPos;
  delete insulationlocalRot;
  delete insulationaffine;

  delete assemAffine;
}
