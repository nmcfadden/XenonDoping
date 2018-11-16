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
// CLASS IMPLEMENTATION:  MJDemoCableAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  COM of cable, at one end
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
#include "mjdemonstrator/MJDemoCableAssembly.hh"
#include "mjdemonstrator/MJDemoCableConductor.hh"
#include "mjdemonstrator/MJDemoCableInsulation.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableAssembly::MJDemoCableAssembly(G4String fName, G4String serialNumber, 
												G4double cableLength) :
  MJVDemoAssembly(fName, serialNumber, "Cable"), fCableLength(cableLength)
{  
  fWire1Ptr = new MJDemoCableConductor(fName+"_Wire_001", "867-5309", fCableLength);
  fWire2Ptr = new MJDemoCableConductor(fName+"_Wire_002", "867-5309", fCableLength);
  fWire3Ptr = new MJDemoCableConductor(fName+"_Wire_003", "867-5309", fCableLength);
  fWire4Ptr = new MJDemoCableConductor(fName+"_Wire_004", "867-5309", fCableLength);
  fInsulationPtr = new MJDemoCableInsulation(fName+"_Insulation_001", "867-5309", fCableLength);
}

MJDemoCableAssembly::MJDemoCableAssembly(const MJDemoCableAssembly & rhs) : 
  MJVDemoAssembly(rhs), fCableLength(rhs.fCableLength)
{
  fWire1Ptr = new MJDemoCableConductor(fName+"_Wire_001", "867-5309", fCableLength);
  fWire2Ptr = new MJDemoCableConductor(fName+"_Wire_002", "867-5309", fCableLength);
  fWire3Ptr = new MJDemoCableConductor(fName+"_Wire_003", "867-5309", fCableLength);
  fWire4Ptr = new MJDemoCableConductor(fName+"_Wire_004", "867-5309", fCableLength);
  fInsulationPtr = new MJDemoCableInsulation(fName+"_Insulation_001", "867-5309", fCableLength);
}

MJDemoCableAssembly::~MJDemoCableAssembly()
{
  delete fWire1Ptr;
  delete fWire2Ptr;
  delete fWire3Ptr;
  delete fWire4Ptr;
  delete fInsulationPtr;
}

void MJDemoCableAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *wire1localPos = new G4ThreeVector(-0.009*25.4*mm, 0, -fCableLength/2);
  G4RotationMatrix *wire1localRot = new G4RotationMatrix();
  G4AffineTransform *wire1affine = new G4AffineTransform(wire1localRot,*wire1localPos);
  *wire1affine*=*assemAffine;  
  G4ThreeVector *wire1globalPos = new G4ThreeVector(wire1affine->NetTranslation());
  G4RotationMatrix *wire1globalRot= new G4RotationMatrix(wire1affine->NetRotation());
  fWire1Ptr->Place(wire1globalPos, wire1globalRot, motherLogical);
  delete wire1localPos;
  delete wire1localRot;
  delete wire1affine;

  G4ThreeVector *wire2localPos = new G4ThreeVector(-0.003*25.4*mm, 0, -fCableLength/2);
  G4RotationMatrix *wire2localRot = new G4RotationMatrix();
  G4AffineTransform *wire2affine = new G4AffineTransform(wire2localRot,*wire2localPos);
  *wire2affine*=*assemAffine;  
  G4ThreeVector *wire2globalPos = new G4ThreeVector(wire2affine->NetTranslation());
  G4RotationMatrix *wire2globalRot= new G4RotationMatrix(wire2affine->NetRotation());
  fWire2Ptr->Place(wire2globalPos, wire2globalRot, motherLogical);
  delete wire2localPos;
  delete wire2localRot;
  delete wire2affine;

  G4ThreeVector *wire3localPos = new G4ThreeVector(0.003*25.4*mm, 0, -fCableLength/2);
  G4RotationMatrix *wire3localRot = new G4RotationMatrix();
  G4AffineTransform *wire3affine = new G4AffineTransform(wire3localRot,*wire3localPos);
  *wire3affine*=*assemAffine;  
  G4ThreeVector *wire3globalPos = new G4ThreeVector(wire3affine->NetTranslation());
  G4RotationMatrix *wire3globalRot= new G4RotationMatrix(wire3affine->NetRotation());
  fWire3Ptr->Place(wire3globalPos, wire3globalRot, motherLogical);
  delete wire3localPos;
  delete wire3localRot;
  delete wire3affine;

  G4ThreeVector *wire4localPos = new G4ThreeVector(0.009*25.4*mm, 0, -fCableLength/2);
  G4RotationMatrix *wire4localRot = new G4RotationMatrix();
  G4AffineTransform *wire4affine = new G4AffineTransform(wire4localRot,*wire4localPos);
  *wire4affine*=*assemAffine;  
  G4ThreeVector *wire4globalPos = new G4ThreeVector(wire4affine->NetTranslation());
  G4RotationMatrix *wire4globalRot= new G4RotationMatrix(wire4affine->NetRotation());
  fWire4Ptr->Place(wire4globalPos, wire4globalRot, motherLogical);
  delete wire4localPos;
  delete wire4localRot;
  delete wire4affine;

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
