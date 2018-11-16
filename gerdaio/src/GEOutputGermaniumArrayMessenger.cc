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
//
// CLASS IMPLEMENTATION:  GEOutputGermaniumArrayMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: L. Pandola
 * CONTACT:
 * FIRST SUBMISSION: Wed Nov 28 2007
 *
 * REVISION:
 *
 * 11-28-2007, Created, L. Pandola
 * 2009-Januar-07  j. schubert  "/MG/output/killGammasOutsideCone true" can be used to kill gamma tracks outside a certain cone
 *                              Example for usage:
 *                                 /MG/output/killGammasOutsideCone true
 *                                 /MG/output/GammaConeCut_ArrayCenter 0 0 19.5 cm
 *                                 /MG/output/GammaConeCut_StartCutRadius 80 cm
 *                                 /MG/output/GammaConeCut_MaxAllowedOpeningAngle 30 deg
 *                              If  a gamma is created at position vecX and has momentum vecP then it is killed in case of
 *                               - momentum points not into a cone directed towards the array center,
 *                                 i.e. angle[vecP,GammaConeCut_ArrayCenter-vecX] < MaxAllowedOpeningAngle
 *                               - _AND_ it has to be outside a certain spherical range arround 'GammaConeCut_ArrayCenter'
 * Mar-18-2009, j. schubert, "/MG/output/WriteNumberOfEventsToFile true" can be used to write the number of events
 *                           to the output ROOT output-file
 * 07-18-2012, N. Barros, Added commands to control the LAr instrumentation part of the simulations.
 * 09-07-2012, N. Barros, Added two additional commands:
 * 								- Enable forcing TTree buffers and header to be written to file after storing a certain amount of data.
 * 								- Customize the size for the data to be stored before forcing the buffers to flush.
 *
 * 19.05.2016, JJ. Added KillPhotonAfterNHits command. Stops opt. photon tracking after N hits in any sens. vol.
 * 20-11-2017, K. v. Sturm Added macro command to save all macro commands used inside the root output file
 * 16-04-2018, K. v. Sturm "/MG/output/killLifetimeLongLivedNuclei" to set live-time above which long lived nuclei are killed
 */
//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"

#include "gerdaio/GEOutputGermaniumArray.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "gerdaio/GEOutputGermaniumArrayMessenger.hh"

//---------------------------------------------------------------------------//

using namespace std;

GEOutputGermaniumArrayMessenger::GEOutputGermaniumArrayMessenger(GEOutputGermaniumArray *theOutput):
  fOutputScheme(theOutput)
{
  fDirectory = new G4UIdirectory("/MG/output/");
  fDirectory->SetGuidance("Output control for GEOutputGermaniumArray.");

  // /MG/output/killDaughter
  fKillDaughterNuclei = new G4UIcmdWithABool("/MG/output/killDaughter",this);
  fKillDaughterNuclei->SetGuidance("Set boolean flag to switch on/off the code ");
  fKillDaughterNuclei->SetGuidance("which kills unstable nuclei produced by primary particles, ");
  fKillDaughterNuclei->SetGuidance("preventing them to decay");
  fKillDaughterNuclei->SetGuidance("(default = true)");

  // /MG/output/killLifetimeLongLivedNuclei
  fKillLifetimeLongLivedNuclei = new G4UIcmdWithADoubleAndUnit("/MG/output/killLifetimeLongLivedNuclei",this);
  fKillLifetimeLongLivedNuclei->SetGuidance("Kill long lived nuclei with live times greater than...");
  fKillLifetimeLongLivedNuclei->SetGuidance("Default is 1.0*microsecond");
  fKillLifetimeLongLivedNuclei->SetDefaultUnit("microsecond");
  fKillLifetimeLongLivedNuclei->SetUnitCandidates("second millisecond microsecond nanosecond");

  // /MG/output/useTimeWindow
  fSetUseTimeWindowCmd = new G4UIcmdWithABool("/MG/output/useTimeWindow", this);
  fSetUseTimeWindowCmd->SetGuidance("Command to window events into time windows");

  // /MG/output/setTimeWindow
  fSetTimeWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/output/setTimeWindow", this);
  fSetTimeWindowCmd->SetGuidance("Set a time window");
  fSetTimeWindowCmd->SetDefaultUnit("second");
  fSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

  // /MG/output/saveOnlyEventsWithEnergyDeposit
  fSaveOnlyEventsWithEnergyDepositCmd = new G4UIcmdWithABool("/MG/output/saveOnlyEventsWithEnergyDeposit",this);
  fSaveOnlyEventsWithEnergyDepositCmd->SetGuidance("save only events with energy deposit in detector");

  // /MG/output/saveOnlyEventsWithCoincidence
  fSaveOnlyEventsWithCoincidenceCmd = new G4UIcmdWithABool("/MG/output/saveOnlyEventsWithCoincidence",this);
  fSaveOnlyEventsWithCoincidenceCmd->SetGuidance("save only events with energy deposit in at least two detectors");

  // /MG/output/killDaughter
  fKillNeutrinos = new G4UIcmdWithABool("/MG/output/killNeutrinos",this);
  fKillNeutrinos->SetGuidance("Set boolean flag to switch on/off the code ");
  fKillNeutrinos->SetGuidance("which kills neutrinos, preventing them to be");
  fKillNeutrinos->SetGuidance("tracked and consume time.");
  fKillNeutrinos->SetGuidance("(default = false)");


  // /MG/output/reduceHitInfo1
  fReduceHitInfo1Cmd = new G4UIcmdWithABool("/MG/output/reduceHitInfo1",this);
  fReduceHitInfo1Cmd->SetGuidance("Set boolean flag to switch on/off the code ");
  fReduceHitInfo1Cmd->SetGuidance("which significantly reduces the output to the root file");
  fReduceHitInfo1Cmd->SetGuidance("(default = false)");

  // /MG/output/addCRInformation
  fAddCRInformationCmd = new G4UIcmdWithABool("/MG/output/addCRInformation",this);
  fAddCRInformationCmd->SetGuidance("Set boolean flag to switch on/off CR information in the output ");
  fAddCRInformationCmd->SetGuidance("(default = false)");

  // /MG/output/killGammasOutsideCone
  fCmd_KillGammasOutsideCone = new G4UIcmdWithABool("/MG/output/killGammasOutsideCone",this);
  fCmd_KillGammasOutsideCone->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_KillGammasOutsideCone->SetGuidance("which kills gammas with momentum vector laying inside");
  fCmd_KillGammasOutsideCone->SetGuidance("a cone pointing towards the array center.");
  fCmd_KillGammasOutsideCone->SetGuidance("(default = false)");
  fCmd_KillGammasOutsideCone->SetGuidance("Also pay attention to the following commands:");
  fCmd_KillGammasOutsideCone->SetGuidance("/MG/output/GammaConeCut_ArrayCenter");
  fCmd_KillGammasOutsideCone->SetGuidance("/MG/output/GammaConeCut_StartCutRadius");
  fCmd_KillGammasOutsideCone->SetGuidance("/MG/output/GammaConeCut_MaxAllowedOpeningAngle");

  // /MG/output/GammaConeCut_ArrayCenter
  fCmd_KillGammasOutsideCone_ArrayCenter = new G4UIcmdWith3VectorAndUnit("/MG/output/GammaConeCut_ArrayCenter",this);
  fCmd_KillGammasOutsideCone_ArrayCenter->SetGuidance("Defines the center position of a region.");
  fCmd_KillGammasOutsideCone_ArrayCenter->SetGuidance("Gammas not pointing to this position within some certain angle will be killed.");
  fCmd_KillGammasOutsideCone_ArrayCenter->SetGuidance("\"/MG/output/killGammasOutsideCone\" needs to be switched on.");
  fCmd_KillGammasOutsideCone_ArrayCenter->SetDefaultUnit("cm");
  fCmd_KillGammasOutsideCone_ArrayCenter->SetUnitCategory("Length");
  fCmd_KillGammasOutsideCone_ArrayCenter->SetUnitCandidates("microm mm cm m km");

  // /MG/output/GammaConeCut_StartCutRadius
  fCmd_KillGammasOutsideCone_StartCutRadius = new G4UIcmdWithADoubleAndUnit("/MG/output/GammaConeCut_StartCutRadius", this);
  fCmd_KillGammasOutsideCone_StartCutRadius->SetGuidance("Set a radius of a sphere arround \"GammaConeCut_ArrayCenter\".");
  fCmd_KillGammasOutsideCone_StartCutRadius->SetGuidance("No Gammas will be killed within this sphere.");
  fCmd_KillGammasOutsideCone_StartCutRadius->SetGuidance("\"/MG/output/killGammasOutsideCone\" needs to be switched on.");
  fCmd_KillGammasOutsideCone_StartCutRadius->SetDefaultUnit("cm");
  fCmd_KillGammasOutsideCone_StartCutRadius->SetUnitCandidates("microm mm cm m km");

  // /MG/output/GammaConeCut_MaxAllowedOpeningAngle
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle = new G4UIcmdWithADoubleAndUnit("/MG/output/GammaConeCut_MaxAllowedOpeningAngle", this);
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetGuidance("Defines opening angle of a cone pointing from the gamma position");
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetGuidance("to a position defined by \"/MG/output/GammaConeCut_ArrayCenter\"");
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetGuidance("Gammas with track outside of this cone will be killed.");
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetGuidance("\"/MG/output/killGammasOutsideCone\" needs to be switched on.");
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetDefaultUnit("rad");
  fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->SetUnitCandidates("rad deg");

  fCmd_WriteMacroToOutput = new G4UIcmdWithABool("/MG/output/WriteMacroToOutput", this);
  fCmd_WriteMacroToOutput -> SetGuidance("Write macro commands used inside a TObjString in the root output file");
  fCmd_WriteMacroToOutput -> SetDefaultValue(true);

  fWriteNumberOfEvents = new G4UIcmdWithABool("/MG/output/WriteNumberOfEventsToFile",this);
  fWriteNumberOfEvents->SetGuidance("Writes the number of events processed in the run into the output file.");
  fWriteNumberOfEvents->SetGuidance("Works only if one run is proccessed.");
  fWriteNumberOfEvents->SetGuidance("(default = true)");
  fWriteNumberOfEvents->SetDefaultValue(true);

  //G4bool             fDoKillGammasOutsideCone;
  //G4ThreeVector      fGammaConeCut_ArrayCenter;
  //G4double           fGammaConeCut_StartCutRadius;
  //G4double           fGammaConeCut_MaxAllowedOpeningAngle;

  fDirectory = new G4UIdirectory("/MG/output/LArInst/");
  fDirectory->SetGuidance("Output control for LAr Instrumentation hits in GEOutputGermaniumArray.");
  // -- NB: Apply more quality cuts: energy cut in the crystal.
  fCmd_LArInstApplyGeEnergyCut = new G4UIcmdWithABool("/MG/output/LArInst/ApplyLArInstGeEnergyCut",this);
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("which applies a pre-cut over event quality by droping events whose");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("energy in the crystal is below a certain threshold.");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("(default = false)");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("The thresholds are set using the following commands:");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("/MG/output/LArInst/SetGeEnergyCutLow (default 0)");
  fCmd_LArInstApplyGeEnergyCut->SetGuidance("/MG/output/LArInst/SetGeEnergyCutHigh (default 1GeV)");

  fCmd_LArInstGeEnergyCutLow = new G4UIcmdWithADoubleAndUnit("/MG/output/LArInst/SetGeEnergyCutLow", this);
  fCmd_LArInstGeEnergyCutLow->SetGuidance("Defines the energy threshold on the crystal for events to be stored.");
  fCmd_LArInstGeEnergyCutLow->SetGuidance("\"/MG/output/LArInst/ApplyLArInstGeEnergyCut\" needs to be switched on.");
  fCmd_LArInstGeEnergyCutLow->SetDefaultUnit("MeV");
  fCmd_LArInstGeEnergyCutLow->SetUnitCandidates("MeV keV eV");

  fCmd_LArInstGeEnergyCutHigh = new G4UIcmdWithADoubleAndUnit("/MG/output/LArInst/SetGeEnergyCutHigh", this);
  fCmd_LArInstGeEnergyCutHigh->SetGuidance("Defines the energy threshold on the crystal for events to be stored.");
  fCmd_LArInstGeEnergyCutHigh->SetGuidance("\"/MG/output/LArInst/ApplyLArInstGeEnergyCut\" needs to be switched on.");
  fCmd_LArInstGeEnergyCutHigh->SetDefaultUnit("MeV");
  fCmd_LArInstGeEnergyCutHigh->SetUnitCandidates("MeV keV eV");

  fCmd_LArInstApplyGeCoincidenceCut = new G4UIcmdWithABool("/MG/output/LArInst/ApplyLArInstGeCoincidenceCut",this);
  fCmd_LArInstApplyGeCoincidenceCut->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_LArInstApplyGeCoincidenceCut->SetGuidance("which applies a pre-cut over event quality by droping events which");
  fCmd_LArInstApplyGeCoincidenceCut->SetGuidance("trigger less than 2 detectors.");
  fCmd_LArInstApplyGeCoincidenceCut->SetGuidance("(default = false)");


  fCmd_LArInstKillPhotonTracks = new G4UIcmdWithAnInteger("/MG/output/LArInst/KillPhotonAfterNHits",this);
  fCmd_LArInstKillPhotonTracks->SetGuidance("Stop the simulation after N photons are detected in the LAr veto");
  fCmd_LArInstKillPhotonTracks->SetGuidance("1 hit is enough to veto the event");
  fCmd_LArInstKillPhotonTracks->SetGuidance("default value = 0, all photons are tracked until the end.");

  /*
  fCmd_LArInstKillWLS = new G4UIcmdWithABool("/MG/output/LArInst/KillWLSTracks",this);
  fCmd_LArInstKillWLS->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_LArInstKillWLS->SetGuidance("which kills the WLS tracks right after the WLS process.");
*/
  fCmd_AlwaysTrackLArPhotons = new G4UIcmdWithABool("/MG/output/LArInst/alwaysTrackPhotons",this);
  fCmd_AlwaysTrackLArPhotons->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_AlwaysTrackLArPhotons->SetGuidance("which checks for Ge hits prior to tracking the photons.");
  fCmd_AlwaysTrackLArPhotons->SetGuidance("WARNING: Increases the run time enormously.");

  fCmd_ForceSaveTree = new G4UIcmdWithABool("/MG/output/forceSaveTree",this);
  fCmd_ForceSaveTree->SetGuidance("Set whether TTree::SetAutoSave should be called ");
  fCmd_ForceSaveTree->SetGuidance(" to define a different size of memory for header and buffer flushing.");
  fCmd_ForceSaveTree->SetGuidance("Use with care.");

  fCmd_AutoSaveSize = new G4UIcmdWithAnInteger("/MG/output/AutoSaveSize",this);
  fCmd_AutoSaveSize->SetGuidance("Define the amount of memory (in bytes) that ");
  fCmd_AutoSaveSize->SetGuidance(" must be written to the TTree before the buffers and header ");
  fCmd_AutoSaveSize->SetGuidance("are flushed. A default of 10 MB is defined for the moment.");

}

//---------------------------------------------------------------------------//

GEOutputGermaniumArrayMessenger::~GEOutputGermaniumArrayMessenger()
{
  delete fKillDaughterNuclei;
  delete fKillLifetimeLongLivedNuclei;
  delete fDirectory;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
  delete fSaveOnlyEventsWithEnergyDepositCmd;
  delete fSaveOnlyEventsWithCoincidenceCmd;
  delete fKillNeutrinos;
  delete fReduceHitInfo1Cmd;
  delete fAddCRInformationCmd;

  delete fCmd_KillGammasOutsideCone;
  delete fCmd_KillGammasOutsideCone_ArrayCenter;
  delete fCmd_KillGammasOutsideCone_StartCutRadius;
  delete fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle;

  delete fCmd_WriteMacroToOutput;
  delete fWriteNumberOfEvents;

  delete fCmd_LArInstApplyGeEnergyCut;
  delete fCmd_LArInstApplyGeCoincidenceCut;

  delete fCmd_LArInstGeEnergyCutLow;
  delete fCmd_LArInstGeEnergyCutHigh;
  delete fCmd_LArInstKillPhotonTracks;
  //delete fCmd_LArInstKillWLS;
  delete fCmd_AlwaysTrackLArPhotons;
}

//---------------------------------------------------------------------------//

void GEOutputGermaniumArrayMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  // /MG/output/killDaughter
  if(command == fKillDaughterNuclei) {
    fOutputScheme->SetKillDaughterFlag(fKillDaughterNuclei->GetNewBoolValue(newValues));
    if (fKillDaughterNuclei->GetNewBoolValue(newValues))
      MGLog(trace) << "Unstable daughter nuclei will be killed " << endlog;
    else
      MGLog(trace) << "Unstable daughter nuclei will NOT be killed " << endlog;
  }
  else if(command == fKillLifetimeLongLivedNuclei) {
    fOutputScheme->SetKillLifetimeLongLivedNuclei(fKillLifetimeLongLivedNuclei->GetNewDoubleValue(newValues));
    MGLog(trace) << "Kill long-lived nuclei above a livetime of " <<
                    fOutputScheme->GetKillLifetimeLongLivedNuclei()/CLHEP::microsecond << "*microsecond" << endlog;
  }
  else if(command == fSetUseTimeWindowCmd)
    fOutputScheme->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetTimeWindowCmd)
    fOutputScheme->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(newValues));
  else if (command == fSaveOnlyEventsWithEnergyDepositCmd)
    fOutputScheme->RequireHit(
                    fSaveOnlyEventsWithEnergyDepositCmd->GetNewBoolValue(newValues));
  else if (command == fSaveOnlyEventsWithCoincidenceCmd)
    fOutputScheme->RequireCoincidence(
                    fSaveOnlyEventsWithCoincidenceCmd->GetNewBoolValue(newValues));
  else if(command == fKillNeutrinos) {
    fOutputScheme->SetDoKillNeutrinos(fKillNeutrinos->GetNewBoolValue(newValues));
    if (fKillNeutrinos->GetNewBoolValue(newValues))
      MGLog(trace) << "Neutrinos will be killed " << endlog;
    else
      MGLog(trace) << "Neutrinos will NOT be killed " << endlog;
  }
  else if(command == fReduceHitInfo1Cmd) {
    fOutputScheme->SetReduceHitInfo1(fReduceHitInfo1Cmd->GetNewBoolValue(newValues));
  }
  else if(command == fAddCRInformationCmd) {
    fOutputScheme->SetAddCRInformation(fAddCRInformationCmd->GetNewBoolValue(newValues));
  }
  else if(command == fCmd_KillGammasOutsideCone) {
    fOutputScheme->SetDoKillGammasOutsideCone(fCmd_KillGammasOutsideCone->GetNewBoolValue(newValues));
    if( fCmd_KillGammasOutsideCone->GetNewBoolValue(newValues) )
        MGLog(trace) << "Gammas inside certain cone will be killed " << endlog;
    else
        MGLog(trace) << "Gammas inside certain cone will NOT be killed " << endlog;
  }
  else if(command == fCmd_KillGammasOutsideCone_ArrayCenter) {
    if( fOutputScheme->GetDoKillGammasOutsideCone()==false ) {
        MGLog(warning) << "You need to call \"/MG/output/killGammasOutsideCone true\" to use the Gamma-Cone-Cut feature." << endlog;
        MGLog(warning) << "Otherwise \"/MG/output/GammaConeCut_ArrayCenter\" has no effect."<<endlog;
    }
    fOutputScheme->SetGammaConeCut_ArrayCenter(fCmd_KillGammasOutsideCone_ArrayCenter->GetNew3VectorValue(newValues));
    MGLog(trace) << "GammaConeCut_ArrayCenter was set to "<< fCmd_KillGammasOutsideCone_ArrayCenter->GetNew3VectorValue(newValues) << endlog;
  }
  else if(command == fCmd_KillGammasOutsideCone_StartCutRadius) {
    if( fOutputScheme->GetDoKillGammasOutsideCone()==false ) {
        MGLog(warning) << "You need to call \"/MG/output/killGammasOutsideCone true\" to use the Gamma-Cone-Cut feature." << endlog;
        MGLog(warning) << "Otherwise \"/MG/output/GammaConeCut_StartCutRadius\" has no effect."<<endlog;
    }
    fOutputScheme->SetGammaConeCut_StartCutRadius(fCmd_KillGammasOutsideCone_StartCutRadius->GetNewDoubleValue(newValues));
    MGLog(trace) << "GammaConeCut_StartCutRadius was set to "<< fCmd_KillGammasOutsideCone_StartCutRadius->GetNewDoubleValue(newValues) << endlog;
  }
  else if(command == fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle) {
    if( fOutputScheme->GetDoKillGammasOutsideCone()==false ) {
        MGLog(warning) << "You need to call \"/MG/output/killGammasOutsideCone true\" to use the Gamma-Cone-Cut feature." << endlog;
        MGLog(warning) << "Otherwise \"/MG/output/GammaConeCut_MaxAllowedOpeningAngle\" has no effect."<<endlog;
    }
    fOutputScheme->SetGammaConeCut_MaxAllowedOpeningAngle(fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->GetNewDoubleValue(newValues));
    MGLog(trace) << "GammaConeCut_MaxAllowedOpeningAngle was set to "<< fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle->GetNewDoubleValue(newValues) << endlog;
  }
  else if( command == fCmd_WriteMacroToOutput )
  {
    fOutputScheme->Set_WriteMacroToOutput( fCmd_WriteMacroToOutput->GetNewBoolValue(newValues) );
    if( fCmd_WriteMacroToOutput->GetNewBoolValue(newValues) )
      MGLog(trace) << "Macro commands will be written to output root file as TObjString. " << endlog;
    else
      MGLog(trace) << "Macro commands will NOT be written to output root file as TObjString. " << endlog;
  }
  else if(command == fWriteNumberOfEvents) {
    fOutputScheme->Set_WriteNumberOfEventsToOutput(fWriteNumberOfEvents->GetNewBoolValue(newValues));
    if (fWriteNumberOfEvents->GetNewBoolValue(newValues))
      MGLog(trace) << "Number of events will be written to output file. " << endlog;
    else
      MGLog(trace) << "Number of events will NOT be written to output file. " << endlog;
  }
  else if (command == fCmd_LArInstApplyGeEnergyCut) {
	  fOutputScheme->SetLArInstApplyCrystalEnergyCut(fCmd_LArInstApplyGeEnergyCut->GetNewBoolValue(newValues));
	  if (fCmd_LArInstApplyGeEnergyCut->GetNewBoolValue(newValues))
		  MGLog(trace) << "Energy cut on Ge crystals being applied." << endlog;
	   else
	      MGLog(trace) << "Energy cut on Ge crystals NOT being applied. " << endlog;
  }
  else if (command == fCmd_LArInstApplyGeCoincidenceCut) {
	  fOutputScheme->SetLArInstApplyCrystalCoincidenceCut(fCmd_LArInstApplyGeCoincidenceCut->GetNewBoolValue(newValues));
	  if (fCmd_LArInstApplyGeCoincidenceCut->GetNewBoolValue(newValues))
		  MGLog(trace) << "Ge coincidence cut on Ge crystals being applied." << endlog;
	   else
	      MGLog(trace) << "Ge coincidence cut on Ge crystals NOT being applied. " << endlog;
  }
  else if (command == fCmd_LArInstGeEnergyCutLow) {
	  fOutputScheme->SetLArInstCrystalEnLow(fCmd_LArInstGeEnergyCutLow->GetNewDoubleValue(newValues));
	  MGLog(trace) << "Lower Energy cut on Ge crystals now set to " <<  fCmd_LArInstGeEnergyCutLow->GetNewDoubleValue(newValues) << " ." << endlog;
  }
  else if (command == fCmd_LArInstGeEnergyCutHigh) {
	  fOutputScheme->SetLArInstCrystalEnHigh(fCmd_LArInstGeEnergyCutHigh->GetNewDoubleValue(newValues));
	  MGLog(trace) << "Higher Energy cut on Ge crystals now set to " <<  fCmd_LArInstGeEnergyCutHigh->GetNewDoubleValue(newValues) << " ." << endlog;
  }
  /*
  else if (command == fCmd_LArInstKillWLS) {
	  fOutputScheme->SetLArInstKillWLSTracks(fCmd_LArInstKillWLS->GetNewBoolValue(newValues));
	  if (fCmd_LArInstKillWLS->GetNewBoolValue(newValues)) {
		  MGLog(trace) << "WLS tracks will be killed after WLS process." << endlog;
	  } else {
		  MGLog(trace) << "WLS tracks will NOT be killed after WLS process." << endlog;
	  }
  }
  */
  else if (command == fCmd_LArInstKillPhotonTracks) {
	  fOutputScheme->SetStopAfterNHits(fCmd_LArInstKillPhotonTracks->GetNewIntValue(newValues));
	  if (fCmd_LArInstKillPhotonTracks->GetNewIntValue(newValues) > 0) {
		  MGLog(trace) << "Photon tracks will be killed after "<< fCmd_LArInstKillPhotonTracks->GetNewIntValue(newValues) << " hits." << endlog;
	  } else {
		  MGLog(trace) << "All photons are processed." << endlog;
	  }
  }
  else if (command == fCmd_AlwaysTrackLArPhotons) {
	  fOutputScheme->SetAlwaysTrackLArPhotons(fCmd_AlwaysTrackLArPhotons->GetNewBoolValue(newValues));
	  if (fCmd_AlwaysTrackLArPhotons->GetNewBoolValue(newValues)) {
		  MGLog(warning) << "Flag to always track photons in LAr set to ON." << endlog;
	  } else {
		  MGLog(trace) << "Flag to always track photons in LAr set to OFF." << endlog;
	  }
  }
  else if (command == fCmd_ForceSaveTree) {
	  fOutputScheme->SetForceSave(fCmd_ForceSaveTree->GetNewBoolValue(newValues));
	  if (fCmd_ForceSaveTree->GetNewBoolValue(newValues)) {
		  MGLog(warning) << "Forcing the buffers and header of the output TTree to be flushed with custom value." << endlog;
	  } else {
		  MGLog(trace) << "Disabling forced flushing TTree buffers and header." << endlog;
	  }
  }
  else if (command == fCmd_AutoSaveSize) {
	  fOutputScheme->SetAutoSaveSize(fCmd_AutoSaveSize->GetNewIntValue(newValues));
	  MGLog(warning) << "Setting autosave size to " << fOutputScheme->GetAutoSaveSize() << " bytes." << endlog;
  }
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
