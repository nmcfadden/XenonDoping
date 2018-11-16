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
// $Id: MJGeometryPopTopXYscanMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPopTopXYscan.cc
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
 * AUTHOR: R.Martin, W. Xu
 * CONTACT: 
 * FIRST SUBMISSION: 11 Mar 2011
 * 
 * REVISION:
 * 05-08-2013 W. Xu modified to xy scan with collimator 
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
#include "mjgeometry/MJGeometryPopTopXYscanMessenger.hh"
#include "mjgeometry/MJGeometryPopTopXYscan.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//



//---------------------------------------------------------------------------//

MJGeometryPopTopXYscanMessenger::MJGeometryPopTopXYscanMessenger(MJGeometryPopTopXYscan *popTopxyscan) : fPopTopxyscan(popTopxyscan)
{
  // /MG/geometry/MJGeometryPopTopXYscan
  fDirectory = new G4UIdirectory("/MG/geometry/MJGeometryPopTopXYscan/");
  
  fSourcePositionCmd = new G4UIcmdWith3VectorAndUnit("/MG/geometry/MJGeometryPopTopXYscan/SourcePosition", this);
  fSourcePositionCmd->SetGuidance("Give position of source relative to top of cryostat");
  fSourcePositionCmd->SetDefaultUnit("cm");
  fSourcePositionCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalOffsetCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalOffset", this);
  fCrystalOffsetCmd->SetGuidance("Give offset from crystal from the vacuum shroud");
  fCrystalOffsetCmd->SetDefaultUnit("mm");
  fCrystalOffsetCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCryostatTypeCmd= new G4UIcmdWithAnInteger("/MG/geometry/MJGeometryPopTopXYscan/CryostatType", this);
  fCryostatTypeCmd->SetGuidance("Give type of IR cup in cryostat, 1=like PONaMa1, 2=like PONaMa2, 3=like P42538B");
  
  fCrystalGeomFileCmd= new G4UIcmdWithAString("/MG/geometry/MJGeometryPopTopXYscan/LoadCrystalFromFile", this);
  fCrystalGeomFileCmd->SetGuidance("File from which to load crystal geometry");
  
  fCrystalDimpleNomRadiusCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalDimpleNominalBoreRadius", this);	
  fCrystalDimpleNomRadiusCmd->SetGuidance("Give radius at the bottom of the bore hole for the dimple (Quantity E on the Ortec spec sheet)");
  fCrystalDimpleNomRadiusCmd->SetDefaultUnit("mm");
  fCrystalDimpleNomRadiusCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalDimpleDepthCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalDimpleDepth", this);
  fCrystalDimpleDepthCmd->SetGuidance("Give depth of the bore hole for the dimple (Quantity D on ORTEC spec sheet)");
  fCrystalDimpleDepthCmd->SetDefaultUnit("mm");
  fCrystalDimpleDepthCmd->SetUnitCandidates("m cm mm micrometer");
  
  
  fCrystalDimpleDiameterCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalDimpleDiameter", this);
  fCrystalDimpleDiameterCmd->SetGuidance("Give diameter of the crystal dimple (Quantity C)");
  fCrystalDimpleDiameterCmd->SetDefaultUnit("mm");
  fCrystalDimpleDiameterCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalDiameterCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalDiameter", this);
  fCrystalDiameterCmd->SetGuidance("Give diameter of the crystal");
  fCrystalDiameterCmd->SetDefaultUnit("mm");
  fCrystalDiameterCmd->SetUnitCandidates("m cm mm micrometer");

  fCrystalLengthCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalLength", this);
  fCrystalLengthCmd->SetGuidance("Give length of the crystal");
  fCrystalLengthCmd->SetDefaultUnit("mm");
  fCrystalLengthCmd->SetUnitCandidates("m cm mm micrometer");

  fCrystalDimpleRadialOffsetCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CrystalDimpleRadialOffset", this);
  fCrystalDimpleRadialOffsetCmd->SetGuidance("Give radial offset of the crystal dimple, in the x-direction ");
  fCrystalDimpleRadialOffsetCmd->SetDefaultUnit("mm");
  fCrystalDimpleRadialOffsetCmd->SetUnitCandidates("m cm mm micrometer");

  fCollimatorGapCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/MJGeometryPopTopXYscan/CollimatorGap", this);
  fCollimatorGapCmd->SetGuidance("adjust the collimator Gap between the bottom of the collimator and the top of the cryostat");
  fCollimatorGapCmd->SetDefaultUnit("mm");
  fCollimatorGapCmd->SetUnitCandidates("m cm mm micrometer");

  fPlaceCollimatorCmd = new G4UIcmdWithABool("/MG/geometry/MJGeometryPopTopXYscan/UseCollimator", this);
  fPlaceCollimatorCmd->SetGuidance("Choose to use the lead 5cm Collimator");

    
  fPlaceSourceHolderCmd = new G4UIcmdWithABool("/MG/geometry/MJGeometryPopTopXYscan/UseSourceHolder", this);
  fPlaceSourceHolderCmd->SetGuidance("Choose to use the teflon 25cm source holder");
}

// -----------------------------------------------------------------------

MJGeometryPopTopXYscanMessenger::~MJGeometryPopTopXYscanMessenger()
{
  delete fSourcePositionCmd;
  delete fCrystalOffsetCmd;
  delete fCryostatTypeCmd;
  delete fCrystalGeomFileCmd;
  delete fCrystalDimpleNomRadiusCmd;
  delete fCrystalDimpleDepthCmd;
  delete fCrystalDimpleDiameterCmd;
  delete fCrystalDimpleRadialOffsetCmd;
  delete fCrystalDiameterCmd;
  delete fCrystalLengthCmd;
  delete fPlaceCollimatorCmd;
  delete fPlaceSourceHolderCmd;
  delete fCollimatorGapCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void MJGeometryPopTopXYscanMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fSourcePositionCmd) fPopTopxyscan->PlaceSource(fSourcePositionCmd->GetNew3VectorValue(nV));
  else if(cmd == fCrystalOffsetCmd) fPopTopxyscan->SetCrystalOffset(fCrystalOffsetCmd->GetNewDoubleValue(nV));
  else if(cmd == fCryostatTypeCmd) fPopTopxyscan->SetCryostatType(fCryostatTypeCmd->GetNewIntValue(nV));
  else if(cmd == fCrystalGeomFileCmd) fPopTopxyscan->LoadCrystalGeomFromFile(nV);	
  else if(cmd == fCrystalDimpleNomRadiusCmd) fPopTopxyscan->SetCrystalDimpleNominalRadius(fCrystalDimpleNomRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleDepthCmd) fPopTopxyscan->SetCrystalDimpleHoleDepth(fCrystalDimpleDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleDiameterCmd) fPopTopxyscan->SetCrystalDimpleHoleDiameter(fCrystalDimpleDiameterCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDimpleRadialOffsetCmd) fPopTopxyscan->SetCrystalDimpleRadialOffset(fCrystalDimpleRadialOffsetCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDiameterCmd) fPopTopxyscan->SetCrystalDiameter(fCrystalDiameterCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalLengthCmd) fPopTopxyscan->SetCrystalLength(fCrystalLengthCmd->GetNewDoubleValue(nV));
  else if(cmd == fPlaceCollimatorCmd) fPopTopxyscan->PlaceCollimator(fPlaceCollimatorCmd->GetNewBoolValue(nV));
  else if(cmd == fCollimatorGapCmd) fPopTopxyscan->SetCollimatorGap(fCollimatorGapCmd->GetNewDoubleValue(nV));
  else if(cmd == fPlaceSourceHolderCmd) fPopTopxyscan->PlaceSourceHolder(fPlaceSourceHolderCmd->GetNewBoolValue(nV));
  //else if(cmd == ) fPopTop->(nV);
  else{}
}

//---------------------------------------------------------------------------//

