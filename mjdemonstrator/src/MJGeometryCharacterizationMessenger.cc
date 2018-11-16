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
// $Id: MJGeometryCharacterizationMessenger.cc,v 1.4 2005/03/31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryCharacterizationMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Modified from MJGeometryDemonstratorMessenger
 *
 * Provides commands to choose source position (in cylindrical coordinates)
 * and to place the various parts.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Asher Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 * 
 * REVISION:
 *
 * 07-22-2013, Created, A. Wasserman
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield of the STC from messenger. J. MacMullin
 *			   For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

#include "mjdemonstrator/MJGeometryCharacterization.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJGeometryCharacterizationMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryCharacterizationMessenger::MJGeometryCharacterizationMessenger(
							       MJGeometryCharacterization *cal) : fCharacterization(cal)
{
  // /MG/characterization
  fCharacterizationDirectory = new G4UIdirectory("/MG/characterization/");

  // /MG/characterization/cavernOn
  fCavernOnCmd = new G4UIcmdWithABool("/MG/characterization/cavernOn", this);
  fCavernOnCmd->SetGuidance("Set to true if you want to activate the surrounding rock cavern.");
  fCavernOnCmd->SetGuidance("Set to false if you want to have just the space around the stand.");
  fCavernOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/standOn
  fStandOnCmd = new G4UIcmdWithABool("/MG/characterization/standOn", this);
  fStandOnCmd->SetGuidance("Set to true if you want to activate the characterization stand.");
  fStandOnCmd->SetGuidance("Set to false if you want to have just the space around the stand.");
  fStandOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/cryostatOn
  fCryostatOnCmd = new G4UIcmdWithABool("/MG/characterization/cryostatOn", this);
  fCryostatOnCmd->SetGuidance("Set to true if you want to activate the cryostat.");
  fCryostatOnCmd->SetGuidance("Set to false if you want to have just the space around the stand.");
  fCryostatOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/cryostat
  fCryostatChoiceCmd = new G4UIcmdWithAString("/MG/characterization/cryostat", this);
  fCryostatChoiceCmd->SetGuidance("Choose the type of cryostat to use.");
  // add to list of candidates if new geometries are needed
  fCryostatChoiceCmd->SetCandidates("STC PopTop");
  fCryostatChoiceCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/coldPlateRotationInDegrees
  fColdPlateRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/characterization/coldPlateRotationInDegrees", this);
  fColdPlateRotationInDegreesCmd->SetGuidance("Rotation of ColdPlate w.r.t. the string (in degrees) for the STC");
  fColdPlateRotationInDegreesCmd->SetGuidance("Possible values: 0.0, 120.0, or 240.0");
  fColdPlateRotationInDegreesCmd->SetDefaultValue(0.0);
  fColdPlateRotationInDegreesCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/characterization/CMSRotationInDegrees
  fCMSRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/characterization/CMSRotationInDegrees", this);
  fCMSRotationInDegreesCmd->SetGuidance("Rotation of CMS w.r.t. the slot in the ColdPlate (in degrees) for the STC");
  fCMSRotationInDegreesCmd->SetDefaultValue(0.0);
  fCMSRotationInDegreesCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/thermalRotationInDegrees
  fThermalRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/characterization/thermalRotationInDegrees", this);
  fThermalRotationInDegreesCmd->SetGuidance("Rotation of IR shield w.r.t. the string (in degrees) for the STC");
  fThermalRotationInDegreesCmd->SetGuidance("Possible values: 30.0*i for any integer i (e.g. 0.0, 30.0, 60.0 ... 300.0, 330.0)");
  fThermalRotationInDegreesCmd->SetDefaultValue(0.0);
  fThermalRotationInDegreesCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/stringOn
  fStringOnCmd = new G4UIcmdWithABool("/MG/characterization/stringOn", this);
  fStringOnCmd->SetGuidance("Set to true if you want to activate the detector string.");
  fStringOnCmd->SetGuidance("Set to false if you want to have just the space around the stand.");
  fStringOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/thinIR
  fThinIRCmd = new G4UIcmdWithABool("/MG/characterization/thinIR", this);
  fThinIRCmd->SetGuidance("Set to true if you want to activate the thin IR shield.");
  fThinIRCmd->SetGuidance("Set to false if you want the standard IR shield. (Default)");
  fThinIRCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/sourceAngle
  fSourceAngleCmd = new G4UIcmdWithADoubleAndUnit("/MG/characterization/sourceAngle", this);
  fSourceAngleCmd->SetGuidance("Set the angle to place the characterization source");
  fSourceAngleCmd->AvailableForStates(G4State_PreInit);
  fSourceAngleCmd->SetDefaultUnit("rad");
  fSourceAngleCmd->SetUnitCandidates("rad deg");

  // /MG/characterization/sourceRadius
  fSourceRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/characterization/sourceRadius", this);
  fSourceRadiusCmd->SetGuidance("Set the radius to place the characterization source");
  fSourceRadiusCmd->AvailableForStates(G4State_PreInit);
  fSourceRadiusCmd->SetDefaultUnit("cm");
  fSourceRadiusCmd->SetUnitCandidates("mm cm m");

  // /MG/characterization/sourceZ
  fSourceZCmd = new G4UIcmdWithADoubleAndUnit("/MG/characterization/sourceZ", this);
  fSourceZCmd->SetGuidance("Set the height to place the characterization source");
  fSourceZCmd->AvailableForStates(G4State_PreInit);
  fSourceZCmd->SetDefaultUnit("cm");
  fSourceZCmd->SetUnitCandidates("mm cm m");

  // /MG/characterization/printSourcePosition
  fPrintSourcePositionCmd = new G4UIcmdWithoutParameter("/MG/characterization/printSourcePosition", this);
  fPrintSourcePositionCmd->SetGuidance("Print a list of the cylindrical coordinates of the characterization source");
  fPrintSourcePositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // /MG/characterization/sourceMaterial
  fSourceMaterialCmd = new G4UIcmdWithAString("/MG/characterization/sourceMaterial", this);
  fSourceMaterialCmd->SetGuidance("Choose the material of the source.");
  // add to candidate list if new materials are added to the button source
  fSourceMaterialCmd->SetCandidates("Barium-133 Americium-241 Cobalt-60");
  fSourceMaterialCmd->AvailableForStates(G4State_PreInit);

  // /MG/characterization/stringName
  fStringNameCmd = new G4UIcmdWithAString("/MG/characterization/stringName", this);
  fStringNameCmd->SetGuidance("Choose a string from the demonstrator.");
  // add to candidate list if new materials are added to the button source
  fStringNameCmd->SetCandidates("C1S1 C1S2 C1S3 C1S4 C1S5 C1S6 C2S0 C2S1 C2S2 C2S3 C2S4 C2S5 C2S6 C2S7");
  fStringNameCmd->AvailableForStates(G4State_PreInit);
}

//---------------------------------------------------------------------------//

MJGeometryCharacterizationMessenger::~MJGeometryCharacterizationMessenger()
{
  delete fCavernOnCmd;
  delete fStandOnCmd;
  delete fCryostatOnCmd;
  delete fCryostatChoiceCmd;
  delete fColdPlateRotationInDegreesCmd;
  delete fCMSRotationInDegreesCmd;
  delete fThermalRotationInDegreesCmd;
  delete fStringOnCmd;
  delete fThinIRCmd;
  delete fCharacterizationDirectory;
  delete fSourceAngleCmd;
  delete fSourceRadiusCmd;
  delete fSourceZCmd;
  delete fPrintSourcePositionCmd;
  delete fSourceMaterialCmd;
  delete fStringNameCmd;
}

//---------------------------------------------------------------------------//

void MJGeometryCharacterizationMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fCavernOnCmd)
    fCharacterization->SetCavern(fCavernOnCmd->GetNewBoolValue(nV));
  if(cmd == fStandOnCmd)
    fCharacterization->SetStand(fStandOnCmd->GetNewBoolValue(nV));
  else if(cmd == fCryostatOnCmd)
    fCharacterization->SetCryostat(fCryostatOnCmd->GetNewBoolValue(nV));
  else if(cmd == fCryostatChoiceCmd)
    fCharacterization->SetCryostatChoice(nV);
  else if(cmd == fColdPlateRotationInDegreesCmd)
  	 fCharacterization->SetColdPlateAngleInDegrees(fColdPlateRotationInDegreesCmd->GetNewDoubleValue(nV));
  else if(cmd == fCMSRotationInDegreesCmd)
  	 fCharacterization->SetCMSAngleInDegrees(fCMSRotationInDegreesCmd->GetNewDoubleValue(nV));
  else if(cmd == fThermalRotationInDegreesCmd)
  	 fCharacterization->SetThermalAngleInDegrees(fThermalRotationInDegreesCmd->GetNewDoubleValue(nV));
  else if(cmd == fStringOnCmd)
    fCharacterization->SetString(fStringOnCmd->GetNewBoolValue(nV));
  else if(cmd == fThinIRCmd)
    fCharacterization->SetThinIR(fThinIRCmd->GetNewBoolValue(nV));
  else if(cmd == fSourceAngleCmd)
    fCharacterization->SetSourceAngle(fSourceAngleCmd->GetNewDoubleValue(nV));
  else if(cmd == fSourceRadiusCmd)
    fCharacterization->SetSourceRadius(fSourceRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fSourceZCmd)
    fCharacterization->SetSourceZ(fSourceZCmd->GetNewDoubleValue(nV));
  else if(cmd == fPrintSourcePositionCmd)
    fCharacterization->PrintSourcePosition();
  else if(cmd == fSourceMaterialCmd)
    fCharacterization->SetSourceMaterial(nV);
  else if(cmd == fStringNameCmd)
    fCharacterization->SetStringName(nV);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
