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
// CLASS IMPLEMENTATION:  MGOutputMCRunMessenger.cc
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

#include "io/MGOutputMCRunMessenger.hh"
#include "io/MGLogger.hh"
#include <sstream>
using namespace std;

//---------------------------------------------------------------------------//

MGOutputMCRunMessenger::MGOutputMCRunMessenger(MGOutputMCRun *mcRun):
  fMCRun(mcRun)
{
  fDirectory = new G4UIdirectory("/MG/io/MCRun/");
  fDirectory->SetGuidance("Parameters for MGOutputMCRun class.");

  // could use G4RunManager's runIDcounter as the runID, but runIDcounter is
  // incremented each additional time  run/beamOn is called

  // /MG/io/MCRun/setRunID
  fSetRunIDCmd = new G4UIcmdWithAnInteger("/MG/io/MCRun/setRunID", this);
  fSetRunIDCmd->SetGuidance("Set run ID number");

  // /MG/io/MCRun/setNSteps
  fSetNStepsCmd = new G4UIcmdWithAnInteger("/MG/io/MCRun/setNSteps", this);
  fSetNStepsCmd->SetGuidance("Set max number of steps");

  // /MG/io/MCRun/setWriteAllSteps
  fSetWriteAllStepsCmd = new G4UIcmdWithABool("/MG/io/MCRun/setWriteAllSteps", this);
  fSetWriteAllStepsCmd->SetGuidance("Set to true to write steps in all volumes (not just detectors)");
  fSetWriteAllStepsCmd->SetParameterName("flag", true);
  fSetWriteAllStepsCmd->SetDefaultValue(true);

  // /MG/io/MCRun/setWriteAllStepsInEventsThatDepositEnergy
  fSetWriteAllStepsInEventsThatDepositEnergyCmd = 
    new G4UIcmdWithABool("/MG/io/MCRun/setWriteAllStepsInEventsThatDepositEnergy", this);
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetGuidance(
    "Set to true to write steps in all volumes for events with non-zero energy deposits");
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetParameterName("flag", true);
  fSetWriteAllStepsInEventsThatDepositEnergyCmd->SetDefaultValue(true);

  // /MG/io/MCRun/setPrintSurfaceAreas
  fSetPrintSurfAreasCmd = new G4UIcmdWithABool("/MG/io/MCRun/setPrintSurfaceAreas", this);
  fSetPrintSurfAreasCmd->SetGuidance("Set to true to print surface areas of all physical volumes");
  fSetPrintSurfAreasCmd->SetParameterName("flag", true);
  fSetPrintSurfAreasCmd->SetDefaultValue(true);

  // /MG/io/MCRun/killAlphas
  fKillAlphasCmd = new G4UIcmdWithABool("/MG/io/MCRun/killAlphas", this);
  fKillAlphasCmd->SetGuidance("Set to true to kill alpha tracks outside sensitive region");
  fKillAlphasCmd->SetParameterName("flag", true);
  fKillAlphasCmd->SetDefaultValue(true);

  // /MG/io/MCRun/killBetas
  fKillBetasCmd = new G4UIcmdWithABool("/MG/io/MCRun/killBetas", this);
  fKillBetasCmd->SetGuidance("Set to true to kill beta tracks outside sensitive region");
  fKillBetasCmd->SetParameterName("flag", true);
  fKillBetasCmd->SetDefaultValue(true);

  // /MG/io/MCRun/killGammas
  fKillGammasCmd = new G4UIcmdWithABool("/MG/io/MCRun/killGammas", this);
  fKillGammasCmd->SetGuidance("Set to true to kill gamma tracks outside sensitive region");
  fKillGammasCmd->SetParameterName("flag", true);
  fKillGammasCmd->SetDefaultValue(true);

  // /MG/io/MCRun/killNeutrons
  fKillNeutronsCmd = new G4UIcmdWithABool("/MG/io/MCRun/killNeutrons", this);
  fKillNeutronsCmd->SetGuidance("Set to true to kill neutron tracks outside sensitive region");
  fKillNeutronsCmd->SetParameterName("flag", true);
  fKillNeutronsCmd->SetDefaultValue(true);

  // /MG/io/MCRun/stopNuclei
  fStopNucleiCmd = new G4UIcmdWithABool("/MG/io/MCRun/stopNuclei", this);
  fStopNucleiCmd->SetGuidance("Set to true to stop recoil nuclear tracks outside sensitive region");
  fStopNucleiCmd->SetParameterName("flag", true);
  fStopNucleiCmd->SetDefaultValue(true);

  // /MG/io/MCRun/useTimeWindow
  fSetUseTimeWindowCmd = new G4UIcmdWithABool("/MG/io/MCRun/useTimeWindow", this);
  fSetUseTimeWindowCmd->SetGuidance("Command to window events into time windows");
  fSetUseTimeWindowCmd->SetGuidance("DO NOT USE WITH IMPORTANCE SAMPLING WINDOWING");
  fSetUseTimeWindowCmd->SetParameterName("flag", true);
  fSetUseTimeWindowCmd->SetDefaultValue(true);

  // /MG/io/MCRun/setTimeWindow
  fSetTimeWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/io/MCRun/setTimeWindow", this);
  fSetTimeWindowCmd->SetGuidance("Set a time window");
  fSetTimeWindowCmd->SetDefaultUnit("second");
  fSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

  // /MG/io/MCRun/useImportanceSamplingWindow
  fUseImportanceSamplingWindowCmd = new G4UIcmdWithABool("/MG/io/MCRun/useImportanceSamplingWindow", this);
  fUseImportanceSamplingWindowCmd->SetGuidance("Command to window events during importance sampling");
  fSetUseTimeWindowCmd->SetGuidance("DO NOT USE WITH TIME WINDOWING");
  fUseImportanceSamplingWindowCmd->SetParameterName("flag", true);
  fUseImportanceSamplingWindowCmd->SetDefaultValue(true);

  // /MG/io/MCRun/SetSensitiveIDLabelScheme [classic, labelID, askGeom, file, manual] (label, filename)
  fSetSensitiveIDLabelSchemeCmd = new G4UIcommand("/MG/io/MCRun/SetSensitiveIDLabelScheme", this);
  G4UIparameter* scheme = new G4UIparameter("scheme", 's', false);
  scheme->SetParameterCandidates("classic labelID askGeom file manual");
  fSetSensitiveIDLabelSchemeCmd->SetParameter(scheme);
  fSetSensitiveIDLabelSchemeCmd->SetParameter(new G4UIparameter("arg", 's', true));
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("Choose the method used to label sensitive volumes and encode their IDs");
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("Usage: MG/io/MCRun/SetSensitiveIDLabelScheme [scheme] (arg)");
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
  fSetSensitiveIDLabelSchemeCmd->SetGuidance("manual: ONLY use volume names added manually via /MG/io/MCRun/AddSensitiveVolnameID");

  // /MG/io/MCRun/AddSensitiveVolnameID [volname] [id]
  fAddSensitiveVolnameID = new G4UIcommand("/MG/io/MCRun/AddSensitiveVolnameID", this);
  fAddSensitiveVolnameID->SetParameter(new G4UIparameter("volname", 's', false));
  fAddSensitiveVolnameID->SetParameter(new G4UIparameter("id", 'i', false));
  fAddSensitiveVolnameID->SetGuidance("Manually add a sensitive volume name / ID pair");
  fAddSensitiveVolnameID->SetGuidance("May be used in conjunction with other schemes, or alone by calling");
  fAddSensitiveVolnameID->SetGuidance("/MG/io/MCRun/SetSensitiveIDLabelScheme manual");
}

//---------------------------------------------------------------------------//

MGOutputMCRunMessenger::~MGOutputMCRunMessenger()
{
  delete fStopNucleiCmd;
  delete fKillAlphasCmd;
  delete fKillBetasCmd;
  delete fKillGammasCmd;
  delete fKillNeutronsCmd;
  delete fSetWriteAllStepsCmd;
  delete fSetWriteAllStepsInEventsThatDepositEnergyCmd;
  delete fSetPrintSurfAreasCmd;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
  delete fUseImportanceSamplingWindowCmd;
  delete fSetSensitiveIDLabelSchemeCmd;
  delete fAddSensitiveVolnameID;
  delete fDirectory;
}

//---------------------------------------------------------------------------//

void MGOutputMCRunMessenger::SetNewValue(G4UIcommand *command, 
					   G4String newValues)
{
  if(command == fSetRunIDCmd)
    fMCRun->SetRunID(fSetRunIDCmd->GetNewIntValue(newValues));
  else if(command == fSetNStepsCmd)
    fMCRun->SetNSteps(fSetNStepsCmd->GetNewIntValue(newValues));
  else if(command == fSetWriteAllStepsCmd)
    fMCRun->SetWriteAllSteps(fSetWriteAllStepsCmd->GetNewBoolValue(newValues));
  else if(command == fSetWriteAllStepsInEventsThatDepositEnergyCmd)
    fMCRun->SetWriteAllStepsInEventsThatDepositEnergy(
        fSetWriteAllStepsInEventsThatDepositEnergyCmd->GetNewBoolValue(newValues));
  else if(command == fSetPrintSurfAreasCmd)
    fMCRun->SetAreSurfaceAreasPrinted(fSetPrintSurfAreasCmd->GetNewBoolValue(newValues));
  else if(command == fKillAlphasCmd)
    fMCRun->SetKillAlphas(fKillAlphasCmd->GetNewBoolValue(newValues));
  else if(command == fKillBetasCmd)
    fMCRun->SetKillBetas(fKillBetasCmd->GetNewBoolValue(newValues));
  else if(command == fKillGammasCmd)
    fMCRun->SetKillGammas(fKillGammasCmd->GetNewBoolValue(newValues));
  else if(command == fKillNeutronsCmd)
    fMCRun->SetKillNeutrons(fKillNeutronsCmd->GetNewBoolValue(newValues));
  else if(command == fStopNucleiCmd)
    fMCRun->SetStopNuclei(fStopNucleiCmd->GetNewBoolValue(newValues));
  else if(command == fSetUseTimeWindowCmd)
    fMCRun->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetTimeWindowCmd)
    fMCRun->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(newValues));
  else if(command == fUseImportanceSamplingWindowCmd)
    fMCRun->SetUseImportanceSamplingWindow(fUseImportanceSamplingWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetSensitiveIDLabelSchemeCmd) {
    istringstream is(newValues);
    string scheme;
    is >> scheme;
    if(scheme == "classic") fMCRun->SetSensitiveIDLabelSchemeClassic();
    else if(scheme == "labelID") {
      if(!is.good()) {
        MGLog(error) << "/MG/io/MCRun/SetSensitiveIDLabelScheme labelID: missing label argument" << endlog;
        return;
      }
      string label;
      is >> label;
      fMCRun->SetSensitiveIDLabelSchemeLabelID(label);
    }
    else if(scheme == "askGeom") fMCRun->SetSensitiveIDLabelSchemeAskGeom();
    else if(scheme == "file") {
      if(!is.good()) {
        MGLog(error) << "/MG/io/MCRun/SetSensitiveIDLabelScheme file: missing filename argument" << endlog;
        return;
      }
      string filename;
      is >> filename;
      fMCRun->SetSensitiveIDLabelSchemeFile(filename);
    }
    else if(scheme == "manual") fMCRun->SetSensitiveIDLabelSchemeManual();
    else {
      MGLog(warning) << "Unknown sensitive ID labeling scheme " << newValues << endlog;
    }
  }
  else if(command == fAddSensitiveVolnameID) {
    istringstream is(newValues);
    string volname;
    int id;
    is >> volname >> id;
    fMCRun->GetMCRun()->SetSensitiveIDOfVolumeName(volname, id);
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

