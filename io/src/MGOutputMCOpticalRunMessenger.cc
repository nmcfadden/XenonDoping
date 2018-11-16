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
// $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
//      
// CLASS IMPLEMENTATION:  MGOutputMCOpticalRunMessenger.cc
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
 * AUTHOR: ASchubert
 * CONTACT: alexis3@u.washington.edu
 * FIRST SUBMISSION: Fri Jan 22 07:17:08 PST 2010
 * 
 * REVISION:
 * 
 * 03-31-2010, Initial submission, A. Schubert
 * 07-27-2010, Removed decay chain stopping; this should be done with Geant4's 
 *             nucleus limits, A. Schubert
 * 09-28-2010, Added command to print surface areas, A. Schubert
 * 11-08-2010, Added MJDemonstrator Active Volume naming scheme
 */
//---------------------------------------------------------------------------//
//

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIdirectory.hh"

//---------------------------------------------------------------------------//

#include "io/MGOutputMCOpticalRunMessenger.hh"
#include "io/MGLogger.hh"
#include <sstream>
using namespace std;

//---------------------------------------------------------------------------//

MGOutputMCOpticalRunMessenger::MGOutputMCOpticalRunMessenger(MGOutputMCOpticalRun *mcRun):
  fMCOpticalRun(mcRun)
{
  fDirectory = new G4UIdirectory("/MG/io/MCOpticalRun/");
  fDirectory->SetGuidance("Parameters for MGOutputMCOpticalRun class.");

  // could use G4RunManager's runIDcounter as the runID, but runIDcounter is
  // incremented each additional time  run/beamOn is called

  // /MG/io/MCOpticalRun/setRunID
  fSetRunIDCmd = new G4UIcmdWithAnInteger("/MG/io/MCOpticalRun/setRunID", this);
  fSetRunIDCmd->SetGuidance("Set run ID number");

  // /MG/io/MCOpticalRun/setNSteps
  fSetNStepsCmd = new G4UIcmdWithAnInteger("/MG/io/MCOpticalRun/setNSteps", this);
  fSetNStepsCmd->SetGuidance("Set max number of steps");

  // /MG/io/MCOpticalRun/setWriteAllSteps
  fSetWriteAllStepsCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/setWriteAllSteps", this);
  fSetWriteAllStepsCmd->SetGuidance("Set to true to write steps in all volumes (not just detectors)");
  fSetWriteAllStepsCmd->SetParameterName("flag", true);
  fSetWriteAllStepsCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/setWriteAllStepsInEventsThatDepositEnergy
  fSetWriteAllStepsInEventsThatDepositEnergyCmd = 
    new G4UIcmdWithABool("/MG/io/MCOpticalRun/setWriteAllStepsInEventsThatDepositEnergy", this);
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetGuidance(
    "Set to true to write steps in all volumes for events with non-zero energy deposits");
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetParameterName("flag", true);
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetDefaultValue(true);

    // /MG/io/MCOpticalRun/setWriteAllSensitiveSteps
  fSetWriteAllSensitiveStepsCmd = 
    new G4UIcmdWithABool("/MG/io/MCOpticalRun/setWriteAllSensitiveSteps", this);
  fSetWriteAllSensitiveStepsCmd->SetGuidance(
    "Set to true to write steps in all volumes for events with non-zero energy deposits");
  fSetWriteAllSensitiveStepsCmd->SetParameterName("flag", true);
  fSetWriteAllSensitiveStepsCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/setPrintSurfaceAreas
  fSetPrintSurfAreasCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/setPrintSurfaceAreas", this);
  fSetPrintSurfAreasCmd->SetGuidance("Set to true to print surface areas of all physical volumes");
  fSetPrintSurfAreasCmd->SetParameterName("flag", true);
  fSetPrintSurfAreasCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/killAlphas
  fKillAlphasCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/killAlphas", this);
  fKillAlphasCmd->SetGuidance("Set to true to kill alpha tracks outside sensitive region");
  fKillAlphasCmd->SetParameterName("flag", true);
  fKillAlphasCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/killBetas
  fKillBetasCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/killBetas", this);
  fKillBetasCmd->SetGuidance("Set to true to kill beta tracks outside sensitive region");
  fKillBetasCmd->SetParameterName("flag", true);
  fKillBetasCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/killGammas
  fKillGammasCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/killGammas", this);
  fKillGammasCmd->SetGuidance("Set to true to kill gamma tracks outside sensitive region");
  fKillGammasCmd->SetParameterName("flag", true);
  fKillGammasCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/killNeutrons
  fKillNeutronsCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/killNeutrons", this);
  fKillNeutronsCmd->SetGuidance("Set to true to kill neutron tracks outside sensitive region");
  fKillNeutronsCmd->SetParameterName("flag", true);
  fKillNeutronsCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/stopNuclei
  fStopNucleiCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/stopNuclei", this);
  fStopNucleiCmd->SetGuidance("Set to true to stop recoil nuclear tracks outside sensitive region");
  fStopNucleiCmd->SetParameterName("flag", true);
  fStopNucleiCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/KillAll
  fKillAllCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/KillAll", this);
  fKillAllCmd->SetGuidance("Set to true to KILL ALL ... tracks after their first step");
  fKillAllCmd->SetParameterName("flag", true);
  fKillAllCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/useTimeWindow
  fSetUseTimeWindowCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/useTimeWindow", this);
  fSetUseTimeWindowCmd->SetGuidance("Command to window events into time windows");
  fSetUseTimeWindowCmd->SetGuidance("DO NOT USE WITH IMPORTANCE SAMPLING WINDOWING");
  fSetUseTimeWindowCmd->SetParameterName("flag", true);
  fSetUseTimeWindowCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/setTimeWindow
  fSetTimeWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/io/MCOpticalRun/setTimeWindow", this);
  fSetTimeWindowCmd->SetGuidance("Set a time window");
  fSetTimeWindowCmd->SetDefaultUnit("second");
  fSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

  // /MG/io/MCOpticalRun/useImportanceSamplingWindow
  fUseImportanceSamplingWindowCmd = new G4UIcmdWithABool("/MG/io/MCOpticalRun/useImportanceSamplingWindow", this);
  fUseImportanceSamplingWindowCmd->SetGuidance("Command to window events during importance sampling");
  fSetUseTimeWindowCmd->SetGuidance("DO NOT USE WITH TIME WINDOWING");
  fUseImportanceSamplingWindowCmd->SetParameterName("flag", true);
  fUseImportanceSamplingWindowCmd->SetDefaultValue(true);

  // /MG/io/MCOpticalRun/SetSensitiveIDLabelScheme [classic, labelID, askGeom, file, manual] (label, filename)
  fSetSensitiveIDLabelSchemeCmd = new G4UIcommand("/MG/io/MCOpticalRun/SetSensitiveIDLabelScheme", this);
  G4UIparameter* scheme = new G4UIparameter("scheme", 's', false);
  scheme->SetParameterCandidates("classic labelID askGeom file manual");
  fSetSensitiveIDLabelSchemeCmd->SetParameter(scheme);
  fSetSensitiveIDLabelSchemeCmd->SetParameter(new G4UIparameter("arg", 's', true));
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("Choose the method used to label sensitive volumes and encode their IDs");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("Usage: MG/io/MCOpticalRun/SetSensitiveIDLabelScheme [scheme] (arg)");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("Available schemes:");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("classic: used historically by MJ. Default, but deprecated.");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("labelID: look for volumes named [label][ID].");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("  Required argument: label");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("askGeom: ask the geometry class. ");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("file: specify a file containing a list of volume name - ID pairs");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("  Required argument: filename");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("  Example file:");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("    # comments starting with \"#\" less than 1024 chars in length are allowed");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("    volumeName1  1022");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("    volumeName2  2343");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("    ...");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("manual: ONLY use volume names added manually via /MG/io/MCOpticalRun/AddSensitiveVolnameID");

  // /MG/io/MCOpticalRun/AddSensitiveVolnameID [volname] [id]
  fAddSensitiveVolnameID = new G4UIcommand("/MG/io/MCOpticalRun/AddSensitiveVolnameID", this);
  fAddSensitiveVolnameID->SetParameter(new G4UIparameter("volname", 's', false));
  fAddSensitiveVolnameID->SetParameter(new G4UIparameter("id", 'i', false));
  fAddSensitiveVolnameID->SetGuidance("Manually add a sensitive volume name / ID pair");
  fAddSensitiveVolnameID->SetGuidance("May be used in conjunction with other schemes, or alone by calling");
  fAddSensitiveVolnameID->SetGuidance("/MG/io/MCOpticalRun/SetSensitiveIDLabelScheme manual");
}

//---------------------------------------------------------------------------//

MGOutputMCOpticalRunMessenger::~MGOutputMCOpticalRunMessenger()
{
  delete fStopNucleiCmd;
  delete fKillAllCmd;
  delete fKillAlphasCmd;
  delete fKillBetasCmd;
  delete fKillGammasCmd;
  delete fKillNeutronsCmd;
  delete fSetWriteAllStepsCmd;
  delete fSetWriteAllStepsInEventsThatDepositEnergyCmd;
  delete fSetWriteAllSensitiveStepsCmd;
  delete fSetPrintSurfAreasCmd;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
  delete fUseImportanceSamplingWindowCmd;
  delete fSetSensitiveIDLabelSchemeCmd;
  delete fAddSensitiveVolnameID;
  delete fDirectory;
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRunMessenger::SetNewValue(G4UIcommand *command, 
					   G4String newValues)
{
  if(command == fSetRunIDCmd)
    fMCOpticalRun->SetRunID(fSetRunIDCmd->GetNewIntValue(newValues));
  else if(command == fSetNStepsCmd)
    fMCOpticalRun->SetNSteps(fSetNStepsCmd->GetNewIntValue(newValues));
  else if(command == fSetWriteAllStepsCmd)
    fMCOpticalRun->SetWriteAllSteps(fSetWriteAllStepsCmd->GetNewBoolValue(newValues));
  else if(command == fSetWriteAllStepsInEventsThatDepositEnergyCmd)
    fMCOpticalRun->SetWriteAllStepsInEventsThatDepositEnergy(
        fSetWriteAllStepsInEventsThatDepositEnergyCmd->GetNewBoolValue(newValues));
  else if(command == fSetWriteAllSensitiveStepsCmd)
      fMCOpticalRun->SetWriteAllSensitiveSteps(
          fSetWriteAllSensitiveStepsCmd->GetNewBoolValue(newValues));
  else if(command == fSetPrintSurfAreasCmd)
    fMCOpticalRun->SetAreSurfaceAreasPrinted(fSetPrintSurfAreasCmd->GetNewBoolValue(newValues));
  else if(command == fKillAlphasCmd)
    fMCOpticalRun->SetKillAlphas(fKillAlphasCmd->GetNewBoolValue(newValues));
  else if(command == fKillBetasCmd)
    fMCOpticalRun->SetKillBetas(fKillBetasCmd->GetNewBoolValue(newValues));
  else if(command == fKillGammasCmd)
    fMCOpticalRun->SetKillGammas(fKillGammasCmd->GetNewBoolValue(newValues));
  else if(command == fKillNeutronsCmd)
    fMCOpticalRun->SetKillNeutrons(fKillNeutronsCmd->GetNewBoolValue(newValues));
  else if(command == fStopNucleiCmd)
    fMCOpticalRun->SetStopNuclei(fStopNucleiCmd->GetNewBoolValue(newValues));
  else if(command == fKillAllCmd)
    fMCOpticalRun->SetKillAll(fKillAllCmd->GetNewBoolValue(newValues));
  else if(command == fSetUseTimeWindowCmd)
    fMCOpticalRun->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetTimeWindowCmd)
    fMCOpticalRun->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(newValues));
  else if(command == fUseImportanceSamplingWindowCmd)
    fMCOpticalRun->SetUseImportanceSamplingWindow(fUseImportanceSamplingWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetSensitiveIDLabelSchemeCmd) {
    istringstream is(newValues);
    string scheme;
    is >> scheme;
    if(scheme == "classic") fMCOpticalRun->SetSensitiveIDLabelSchemeClassic();
    else if(scheme == "labelID") {
      if(!is.good()) {
        MGLog(error) << "/MG/io/MCOpticalRun/SetSensitiveIDLabelScheme labelID: missing label argument" << endlog;
        return;
      }
      string label;
      is >> label;
      fMCOpticalRun->SetSensitiveIDLabelSchemeLabelID(label);
    }
    else if(scheme == "askGeom") fMCOpticalRun->SetSensitiveIDLabelSchemeAskGeom();
    else if(scheme == "file") {
      if(!is.good()) {
        MGLog(error) << "/MG/io/MCOpticalRun/SetSensitiveIDLabelScheme file: missing filename argument" << endlog;
        return;
      }
      string filename;
      is >> filename;
      fMCOpticalRun->SetSensitiveIDLabelSchemeFile(filename);
    }
    else if(scheme == "manual") fMCOpticalRun->SetSensitiveIDLabelSchemeManual();
    else {
      MGLog(warning) << "Unknown sensitive ID labeling scheme " << newValues << endlog;
    }
  }
  else if(command == fAddSensitiveVolnameID) {
    istringstream is(newValues);
    string volname;
    int id;
    is >> volname >> id;
    fMCOpticalRun->GetMCRun()->SetSensitiveIDOfVolumeName(volname, id);
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

