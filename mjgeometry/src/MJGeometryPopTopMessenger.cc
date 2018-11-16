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
// $Id: MJGeometryPopTopMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPopTop.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
  * AUTHOR: R.Martin
 * CONTACT: 
 * FIRST SUBMISSION: 11 Mar 2011
 * 
 * REVISION:
 * 
 * 03-04-2013 R. Martin created
 */
//---------------------------------------------------------------------------//
//
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryPopTopMessenger.hh"
#include "mjgeometry/MJGeometryPopTop.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//



//---------------------------------------------------------------------------//

MJGeometryPopTopMessenger::MJGeometryPopTopMessenger(MJGeometryPopTop *popTop) : fPopTop(popTop)
{
  // /MG/geometry/MJGeometryPopTop
  fDirectory = new G4UIdirectory("/MG/geometry/MJGeometryPopTop/");
  
  fSourcePositionCmd = new G4UIcmdWith3VectorAndUnit("/MG/geometry/MJGeometryPopTop/SourcePosition", this);
  fSourcePositionCmd->SetGuidance("Give position of source relative to top of cryostat");
  fSourcePositionCmd->SetDefaultUnit("cm");
  fSourcePositionCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalOffsetCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalOffset", this);
  fCrystalOffsetCmd->SetGuidance("Give offset from crystal from the vacuum shroud");
  fCrystalOffsetCmd->SetDefaultUnit("mm");
  fCrystalOffsetCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCryostatTypeCmd= new G4UIcmdWithAnInteger("/MG/geometry/MJGeometryPopTop/CryostatType", this);
  fCryostatTypeCmd->SetGuidance("Give type of IR cup in cryostat, 1=like PONaMa1, 2=like PONaMa2, 3=like P42538B");
  
  fCrystalGeomFileCmd= new G4UIcmdWithAString("/MG/geometry/MJGeometryPopTop/LoadCrystalFromFile", this);
  fCrystalGeomFileCmd->SetGuidance("File from which to load crystal geometry");
  
  fCrystalDimpleNomRadiusCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalDimpleNominalBoreRadius", this);	
  fCrystalDimpleNomRadiusCmd->SetGuidance("Give radius at the bottom of the bore hole for the dimple (Quantity E on the Ortec spec sheet)");
  fCrystalDimpleNomRadiusCmd->SetDefaultUnit("mm");
  fCrystalDimpleNomRadiusCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalDimpleDepthCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalDimpleDepth", this);
  fCrystalDimpleDepthCmd->SetGuidance("Give depth of the bore hole for the dimple (Quantity D on ORTEC spec sheet)");
  fCrystalDimpleDepthCmd->SetDefaultUnit("mm");
  fCrystalDimpleDepthCmd->SetUnitCandidates("m cm mm micrometer");
  
  
  fCrystalDimpleDiameterCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalDimpleDiameter", this);
  fCrystalDimpleDiameterCmd->SetGuidance("Give diameter of the crystal dimple (Quantity C)");
  fCrystalDimpleDiameterCmd->SetDefaultUnit("mm");
  fCrystalDimpleDiameterCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalDiameterCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalDiameter", this);
  fCrystalDiameterCmd->SetGuidance("Give diameter of the crystal");
  fCrystalDiameterCmd->SetDefaultUnit("mm");
  fCrystalDiameterCmd->SetUnitCandidates("m cm mm micrometer");

  fCrystalLengthCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalLength", this);
  fCrystalLengthCmd->SetGuidance("Give length of the crystal");
  fCrystalLengthCmd->SetDefaultUnit("mm");
  fCrystalLengthCmd->SetUnitCandidates("m cm mm micrometer");

  fCrystalDimpleRadialOffsetCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTop/CrystalDimpleRadialOffset", this);
  fCrystalDimpleRadialOffsetCmd->SetGuidance("Give radial offset of the crystal dimple, in the x-direction ");
  fCrystalDimpleRadialOffsetCmd->SetDefaultUnit("mm");
  fCrystalDimpleRadialOffsetCmd->SetUnitCandidates("m cm mm micrometer");

  fPlaceSourceHolderCmd = new G4UIcmdWithABool("/MG/geometry/MJGeometryPopTop/UseSourceHolder", this);
  fPlaceSourceHolderCmd->SetGuidance("Choose to use the teflon 25cm source holder");
}

// -----------------------------------------------------------------------

MJGeometryPopTopMessenger::~MJGeometryPopTopMessenger()
{
  delete fSourcePositionCmd;
  delete fCrystalOffsetCmd;
  delete fCryostatTypeCmd;
  delete fCrystalGeomFileCmd;
  delete fCrystalDimpleNomRadiusCmd;
  delete fCrystalDimpleDepthCmd;
  delete fCrystalDimpleDiameterCmd;
  delete fPlaceSourceHolderCmd;
  delete fCrystalDimpleRadialOffsetCmd;
  delete fCrystalDiameterCmd;
  delete fCrystalLengthCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void MJGeometryPopTopMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fSourcePositionCmd) fPopTop->PlaceSource(fSourcePositionCmd->GetNew3VectorValue(nV));
  else if(cmd == fCrystalOffsetCmd) fPopTop->SetCrystalOffset(fCrystalOffsetCmd->GetNewDoubleValue(nV));
  else if(cmd == fCryostatTypeCmd) fPopTop->SetCryostatType(fCryostatTypeCmd->GetNewIntValue(nV));
  else if(cmd == fCrystalGeomFileCmd) fPopTop->LoadCrystalGeomFromFile(nV);	
  else if(cmd == fCrystalDimpleNomRadiusCmd) fPopTop->SetCrystalDimpleNominalRadius(fCrystalDimpleNomRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleDepthCmd) fPopTop->SetCrystalDimpleHoleDepth(fCrystalDimpleDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleDiameterCmd) fPopTop->SetCrystalDimpleHoleDiameter(fCrystalDimpleDiameterCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleRadialOffsetCmd) fPopTop->SetCrystalDimpleRadialOffset(fCrystalDimpleRadialOffsetCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDiameterCmd) fPopTop->SetCrystalDiameter(fCrystalDiameterCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalLengthCmd) fPopTop->SetCrystalLength(fCrystalLengthCmd->GetNewDoubleValue(nV));
  else if(cmd == fPlaceSourceHolderCmd) fPopTop->PlaceSourceHolder(fPlaceSourceHolderCmd->GetNewBoolValue(nV));
  //else if(cmd == ) fPopTop->(nV);
  else{}
}

//---------------------------------------------------------------------------//

