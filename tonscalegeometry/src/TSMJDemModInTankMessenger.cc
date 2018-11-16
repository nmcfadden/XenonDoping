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
// $Id: TSMJDemModInTankMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  TSMJDemModInTankMessenger.cc
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
 * 03-11-2011 R. Martin created
 * 06-13-2011 Added commands for variable sized modules, R.Martin
 * 03-14-2012 R. Martin added command to set height of world volume
 * 07-30-2012, Added commands to set string radial clearance and radial
 *             spacing between strings, K. Nguyen
 */
//---------------------------------------------------------------------------//
//
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"
#include "io/MGLogger.hh"
#include "tonscalegeometry/TSMJDemModInTank.hh" 

//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSMJDemModInTankMessenger.hh" 

//---------------------------------------------------------------------------//

TSMJDemModInTankMessenger::TSMJDemModInTankMessenger(TSMJDemModInTank *mjdemintank) : fMJDemInTank(mjdemintank)
{
  // /MG/geometry/TSMJDemModInTank
  fDirectory = new G4UIdirectory("/MG/geometry/TSMJDemModInTank/");
  
  fTankVerticalOffsetCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/TankVerticalOffset", this);
  fTankVerticalOffsetCmd->SetGuidance("Set vertical offset of tank from floor of lab space");
  fTankVerticalOffsetCmd->SetDefaultUnit("m");
  fTankVerticalOffsetCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalDCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/CrystalDiameter", this);
  fCrystalDCmd->SetGuidance("Set diameter of crystals, default in mm");
  fCrystalDCmd->SetDefaultUnit("mm");
  fCrystalDCmd->SetUnitCandidates("m cm mm micrometer");
  
  fCrystalHCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/CrystalHeight", this);
  fCrystalHCmd->SetGuidance("Set height of crystals, default in mm");
  fCrystalHCmd->SetDefaultUnit("mm");
  fCrystalHCmd->SetUnitCandidates("m cm mm micrometer");
  
  // FIXME: Added by K. Nguyen
  fStringRadialClearanceCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/StringRadialClearance", this);
  fStringRadialClearanceCmd->SetGuidance("Set clearance spacing for each string, default in mm");
  fStringRadialClearanceCmd->SetDefaultUnit("mm");
  fStringRadialClearanceCmd->SetUnitCandidates("m cm mm micrometer");

  // FIXME: Added by K. Nguyen
  fRadialSpacingBetweenStringsCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/RadialSpacingBetweenStrings", this);
  fRadialSpacingBetweenStringsCmd->SetGuidance("Set spacing between each string, default in mm");
  fRadialSpacingBetweenStringsCmd->SetDefaultUnit("mm");
  fRadialSpacingBetweenStringsCmd->SetUnitCandidates("m cm mm micrometer");

  fWorldHCmd= new G4UIcmdWithADoubleAndUnit("/MG/geometry/TSMJDemModInTank/WorldVolumeHeight", this);
  fWorldHCmd->SetGuidance("Set height of world volume, default in m");
  fWorldHCmd->SetDefaultUnit("m");
  fWorldHCmd->SetUnitCandidates("m cm mm micrometer km");
 
  fTankIsActiveCmd = new G4UIcmdWithABool("/MG/geometry/TSMJDemModInTank/TankIsActive", this);
  fTankIsActiveCmd->SetGuidance("Set if tank is active (can create large output files!), false by default");
  
  fModuleTypeCmd = new G4UIcmdWithAString("/MG/geometry/TSMJDemModInTank/ModuleType", this);
  // FIXME: Added by K. Nguyen
  fModuleTypeCmd->SetGuidance("Choose module type, (MJDemonstrator, 1Ring, 2Ring, 3Ring, 4Ring, 1HEXRing, 2HEXRing)"); 
  
  fCrystalAssemblyTypeCmd = new G4UIcmdWithAString("/MG/geometry/TSMJDemModInTank/CrystalAssemblyType", this);
  fCrystalAssemblyTypeCmd->SetGuidance("Choose crystal assembly type, (MJDemonstrator, TS)");
  
  fCrystalsPerStringCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTank/CrystalsPerString",this);
  fCrystalsPerStringCmd->SetGuidance("Number of detector units in 1 string");
  
  fnModulesCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTank/NumberOfModules",this);
  fnModulesCmd->SetGuidance("Number of modules (detector arrays) in the geometry");
}

// -----------------------------------------------------------------------

TSMJDemModInTankMessenger::~TSMJDemModInTankMessenger()
{
  delete fWorldHCmd;
  delete fModuleTypeCmd;
  delete fCrystalDCmd;
  delete fCrystalHCmd;
  delete fStringRadialClearanceCmd;       // FIXME: Added by K. Nguyen
  delete fRadialSpacingBetweenStringsCmd; // FIXME: Added by K. Nguyen
  delete fCrystalAssemblyTypeCmd;	
  delete fCrystalsPerStringCmd;
  delete fTankVerticalOffsetCmd;
  delete fTankIsActiveCmd;
  delete fnModulesCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void TSMJDemModInTankMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fTankVerticalOffsetCmd) fMJDemInTank->SetTankVerticalOffset(fTankVerticalOffsetCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalDCmd) fMJDemInTank->SetCrystalDiameter(fCrystalDCmd->GetNewDoubleValue(nV));
  else if(cmd == fCrystalHCmd) fMJDemInTank->SetCrystalHeight(fCrystalHCmd->GetNewDoubleValue(nV));
  else if(cmd == fWorldHCmd) fMJDemInTank->SetHeightOfWorldVolume(fWorldHCmd->GetNewDoubleValue(nV));
  else if(cmd == fTankIsActiveCmd) fMJDemInTank->TankIsSensitive(fTankIsActiveCmd->GetNewBoolValue(nV));
  else if(cmd == fModuleTypeCmd) fMJDemInTank->SetModuleType(nV);
  else if(cmd == fnModulesCmd) fMJDemInTank->SetnModules(fnModulesCmd->GetNewIntValue(nV));
  else if(cmd == fCrystalAssemblyTypeCmd) fMJDemInTank->SetCrystalAssemblyType(nV);
  else if(cmd == fCrystalsPerStringCmd) fMJDemInTank->SetnCrystalsPerString(fCrystalsPerStringCmd->GetNewIntValue(nV));
  else if(cmd == fStringRadialClearanceCmd){ // FIXME: Added by K. Nguyen
    fMJDemInTank->SetStringRadialClearance(fStringRadialClearanceCmd->GetNewDoubleValue(nV));
  }
  else if(cmd == fRadialSpacingBetweenStringsCmd){ // FIXME: Added by K. Nguyen
    fMJDemInTank->SetRadialSpacingBetweenStrings(fRadialSpacingBetweenStringsCmd->GetNewDoubleValue(nV));
  }
  else{}
}

//---------------------------------------------------------------------------//

