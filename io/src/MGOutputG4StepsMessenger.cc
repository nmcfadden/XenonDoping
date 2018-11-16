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
// $Id: MGOutputG4StepsMessenger.cc,v 1.7 2009-06-23 17:05:06 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputG4StepsMessenger.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Dec 28 11:02:05 PST 2005
 * 
 * REVISION:
 * 
 * 03-18-2009, setWriteSensitiveStepsOnly command, A. Schubert
 * 02-22-2007, Added killGamma and killNeutron commands, A. Schubert
 * 08-15-2006, Added stopNuclei, killAlpha and killBeta commands. R. Henning
 * 12-28-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

//---------------------------------------------------------------------------//

#include "io/MGOutputG4StepsMessenger.hh"

//---------------------------------------------------------------------------//

MGOutputG4StepsMessenger::MGOutputG4StepsMessenger(MGOutputG4Steps *g4Steps):
  fG4Steps(g4Steps)
{
  fDirectory = new G4UIdirectory("/MG/io/G4Steps/");
  fDirectory->SetGuidance("Parameters for MGOutputG4Steps class.");

  // /MG/io/G4Steps/setWriteSensitiveOnly
  fSetWriteSensitiveOnlyCmd = new G4UIcmdWithABool("/MG/io/G4Steps/setWriteSensitiveOnly", this);
  fSetWriteSensitiveOnlyCmd->
    SetGuidance("Set to true to only write hits to sensitive volumes (ones with \"active\" in their placement names)");

  // /MG/io/G4Steps/setWriteSensitiveStepsOnly
  fSetWriteSensitiveStepsOnlyCmd = new G4UIcmdWithABool("/MG/io/G4Steps/setWriteSensitiveStepsOnly", this);
  fSetWriteSensitiveStepsOnlyCmd->
    SetGuidance("Set to true to only write steps in the active volume with hits to sensitive volumes (ones with \"active\" in their placement names)");

  // /MG/io/G4Steps/setWriteSensitiveStepsOnly
  fSetWriteTrackPreStepCmd = new
  G4UIcmdWithABool("/MG/io/G4Steps/setWriteTrackPreStep", this);
  fSetWriteSensitiveStepsOnlyCmd->
    SetGuidance("Set to true to write prestep info for the first step in each track");

  // /MG/io/G4Steps/killAlphas
  fKillAlphasCmd = new G4UIcmdWithABool("/MG/io/G4Steps/killAlphas", this);
  fKillAlphasCmd->SetGuidance("Set to true to kill alpha tracks outside sensitive region");

  // /MG/io/G4Steps/killBetas
  fKillBetasCmd = new G4UIcmdWithABool("/MG/io/G4Steps/killBetas", this);
  fKillBetasCmd->SetGuidance("Set to true to kill beta tracks outside sensitive region");

  // /MG/io/G4Steps/killGammas
  fKillGammasCmd = new G4UIcmdWithABool("/MG/io/G4Steps/killGammas", this);
  fKillGammasCmd->SetGuidance("Set to true to kill gamma tracks outside sensitive region");

  // /MG/io/G4Steps/killNeutrons
  fKillNeutronsCmd = new G4UIcmdWithABool("/MG/io/G4Steps/killNeutrons", this);
  fKillNeutronsCmd->SetGuidance("Set to true to kill neutron tracks outside sensitive region");

  // /MG/io/G4Steps/stopNuclei
  fStopNucleiCmd = new G4UIcmdWithABool("/MG/io/G4Steps/stopNuclei", this);
  fStopNucleiCmd->SetGuidance("Set to true to stop recoil nuclear tracks outside sensitive region");

  // /MG/io/G4Steps/useTimeWindow
  fSetUseTimeWindowCmd = new G4UIcmdWithABool("/MG/io/G4Steps/useTimeWindow",
  this);
  fSetUseTimeWindowCmd->SetGuidance("Command to window events into time windows");

  // /MG/io/G4Steps/setTimeWindow
  fSetTimeWindowCmd = new
  G4UIcmdWithADoubleAndUnit("/MG/io/G4Steps/setTimeWindow", this);
  fSetTimeWindowCmd->SetGuidance("Set a time window");
  fSetTimeWindowCmd->SetDefaultUnit("second");
  fSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

}

//---------------------------------------------------------------------------//

MGOutputG4StepsMessenger::~MGOutputG4StepsMessenger()
{
  delete fStopNucleiCmd;
  delete fKillAlphasCmd;
  delete fKillBetasCmd;
  delete fKillGammasCmd;
  delete fKillNeutronsCmd;
  delete fSetWriteSensitiveOnlyCmd;
  delete fSetWriteSensitiveStepsOnlyCmd;
  delete fSetWriteTrackPreStepCmd;
  delete fDirectory;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
}

//---------------------------------------------------------------------------//

void MGOutputG4StepsMessenger::SetNewValue(G4UIcommand *command, 
					   G4String newValues)
{
  if(command == fSetWriteSensitiveOnlyCmd)
    fG4Steps->SetWriteSensitiveHitsOnly(fSetWriteSensitiveOnlyCmd->GetNewBoolValue(newValues));
  else if(command == fSetWriteSensitiveStepsOnlyCmd)
    fG4Steps->SetWriteSensitiveStepsOnly(fSetWriteSensitiveStepsOnlyCmd->GetNewBoolValue(newValues));
  else if(command == fSetWriteTrackPreStepCmd)
    fG4Steps->SetWriteTrackPreStep(fSetWriteTrackPreStepCmd->GetNewBoolValue(newValues));
  else if(command == fKillAlphasCmd)
    fG4Steps->KillAlphas(fKillAlphasCmd->GetNewBoolValue(newValues));
  else if(command == fKillBetasCmd)
    fG4Steps->KillBetas(fKillBetasCmd->GetNewBoolValue(newValues));
  else if(command == fKillGammasCmd)
    fG4Steps->KillGammas(fKillGammasCmd->GetNewBoolValue(newValues));
  else if(command == fKillNeutronsCmd)
    fG4Steps->KillNeutrons(fKillNeutronsCmd->GetNewBoolValue(newValues));
  else if(command == fStopNucleiCmd)
    fG4Steps->StopNuclei(fStopNucleiCmd->GetNewBoolValue(newValues));
  else if(command == fSetUseTimeWindowCmd)
    fG4Steps->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetTimeWindowCmd)
    fG4Steps->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(newValues));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
