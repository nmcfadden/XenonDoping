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
// CLASS IMPLEMENTATION:  GEOutputLArGeOpticalMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: N. Barros (forking from L. Pandola's GEOutputGermaniumArrayMessenger)
 * CONTACT: 
 * FIRST SUBMISSION: Fri May 18 2012
 * 
 * REVISION:
 *
 * 05-18-2012, Created, N. Barros - Fork from GEOutputGermaniumArrayMessenger to adapt from LArGe
 *
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

#include "gerdaio/GEOutputLArGeOptical.hh"
#include "io/MGLogger.hh"


//---------------------------------------------------------------------------//

#include "gerdaio/GEOutputLArGeOpticalMessenger.hh"

//---------------------------------------------------------------------------//

using namespace std;

GEOutputLArGeOpticalMessenger::GEOutputLArGeOpticalMessenger(GEOutputLArGeOptical *theOutput):
  fOutputScheme(theOutput)
{
  fDirectory = new G4UIdirectory("/MG/output/");
  fDirectory->SetGuidance("Output control for GEOutputLArGeOptical.");

  // /MG/output/killDaughter
  fKillDaugNuclei = new G4UIcmdWithABool("/MG/output/killDaughter",this);
  fKillDaugNuclei->SetGuidance("Set boolean flag to switch on/off the code ");
  fKillDaugNuclei->SetGuidance("which kills unstable nuclei produced by primary particles, ");
  fKillDaugNuclei->SetGuidance("preventing them to decay");
  fKillDaugNuclei->SetGuidance("(default = true)");

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


  fWriteNumberOfEvents = new G4UIcmdWithABool("/MG/output/WriteNumberOfEventsToFile",this);
  fWriteNumberOfEvents->SetGuidance("Writes the number of events processed in the run into the output file.");
  fWriteNumberOfEvents->SetGuidance("Works only if one run is processed.");
  fWriteNumberOfEvents->SetGuidance("(default = false)");
  fWriteNumberOfEvents->SetDefaultValue(false);


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
  fCmd_LArInstGeEnergyCutLow->SetGuidance("Defines the energy threshold on the crystal for events to have photon simulated as well.");
  fCmd_LArInstGeEnergyCutLow->SetGuidance("\"/MG/output/LArInst/ApplyLArInstGeEnergyCut\" needs to be switched on.");
  fCmd_LArInstGeEnergyCutLow->SetDefaultUnit("MeV");
  fCmd_LArInstGeEnergyCutLow->SetUnitCandidates("MeV keV eV");

  fCmd_LArInstGeEnergyCutHigh = new G4UIcmdWithADoubleAndUnit("/MG/output/LArInst/SetGeEnergyCutHigh", this);
  fCmd_LArInstGeEnergyCutHigh->SetGuidance("Defines the energy threshold on the crystal for events to be stored.");
  fCmd_LArInstGeEnergyCutHigh->SetGuidance("\"/MG/output/LArInst/ApplyLArInstGeEnergyCut\" needs to be switched on.");
  fCmd_LArInstGeEnergyCutHigh->SetDefaultUnit("MeV");
  fCmd_LArInstGeEnergyCutHigh->SetUnitCandidates("MeV keV eV");

  fCmd_LArInstKillWLS = new G4UIcmdWithABool("/MG/output/LArInst/KillWLSTracks",this);
  fCmd_LArInstKillWLS->SetGuidance("Set boolean flag to switch on/off the code ");
  fCmd_LArInstKillWLS->SetGuidance("which kills the WLS tracks right after the WLS process.");


  fCmd_EGeHitThreshold = new G4UIcmdWithADoubleAndUnit("/MG/output/SetMCGeEnergyCutLow", this);
  fCmd_EGeHitThreshold->SetGuidance("Defines the energy threshold on the crystal for events to be stored.");
  fCmd_EGeHitThreshold->SetDefaultUnit("MeV");
  fCmd_EGeHitThreshold->SetUnitCandidates("MeV keV eV");
}

//---------------------------------------------------------------------------//

GEOutputLArGeOpticalMessenger::~GEOutputLArGeOpticalMessenger()
{
  delete fKillDaugNuclei;
  delete fDirectory;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
  delete fSaveOnlyEventsWithEnergyDepositCmd;
  delete fKillNeutrinos;
  delete fReduceHitInfo1Cmd;

  delete fCmd_KillGammasOutsideCone;
  delete fCmd_KillGammasOutsideCone_ArrayCenter;
  delete fCmd_KillGammasOutsideCone_StartCutRadius;
  delete fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle;

  delete fWriteNumberOfEvents;

  delete fCmd_LArInstApplyGeEnergyCut;
  delete fCmd_LArInstGeEnergyCutLow;
  delete fCmd_LArInstGeEnergyCutHigh;
  delete fCmd_LArInstKillWLS;
  delete fCmd_EGeHitThreshold;
}

//---------------------------------------------------------------------------//

void GEOutputLArGeOpticalMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  // /MG/output/killDaughter
  if(command == fKillDaugNuclei) {
    fOutputScheme->SetKillDaughterFlag(fKillDaugNuclei->GetNewBoolValue(newValues));
    if (fKillDaugNuclei->GetNewBoolValue(newValues))
      MGLog(trace) << "Unstable daughter nuclei will be killed " << endlog;
    else
      MGLog(trace) << "Unstable daughter nuclei will NOT be killed " << endlog;
  }
  else if(command == fSetUseTimeWindowCmd)
    fOutputScheme->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(newValues));
  else if(command == fSetTimeWindowCmd)
    fOutputScheme->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(newValues));
  else if (command == fSaveOnlyEventsWithEnergyDepositCmd)
    fOutputScheme->RequireHit(
                    fSaveOnlyEventsWithEnergyDepositCmd->GetNewBoolValue(newValues));
  else if(command == fKillNeutrinos) {
    fOutputScheme->SetDoKillNeutrinos(fKillNeutrinos->GetNewBoolValue(newValues));
    if (fKillNeutrinos->GetNewBoolValue(newValues))
      MGLog(trace) << "Neutrinos will be killed " << endlog;
    else
      MGLog(trace) << "Neutrinos will NOT be killed " << endlog;
  }  else if(command == fReduceHitInfo1Cmd) {
    fOutputScheme->SetReduceHitInfo1(fReduceHitInfo1Cmd->GetNewBoolValue(newValues));
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
  else if (command == fCmd_LArInstGeEnergyCutLow) {
	  fOutputScheme->SetLArInstCrystalEnLow(fCmd_LArInstGeEnergyCutLow->GetNewDoubleValue(newValues));
	  MGLog(trace) << "Lower Energy cut on Ge crystals now set to " <<  fCmd_LArInstGeEnergyCutLow->GetNewDoubleValue(newValues) << " ." << endlog;
  }
  else if (command == fCmd_LArInstGeEnergyCutHigh) {
	  fOutputScheme->SetLArInstCrystalEnHigh(fCmd_LArInstGeEnergyCutHigh->GetNewDoubleValue(newValues));
	  MGLog(trace) << "Higher Energy cut on Ge crystals now set to " <<  fCmd_LArInstGeEnergyCutHigh->GetNewDoubleValue(newValues) << " ." << endlog;
  }
  else if (command == fCmd_LArInstKillWLS) {
	  fOutputScheme->SetLArInstKillWLSTracks(fCmd_LArInstKillWLS->GetNewBoolValue(newValues));
	  if (fCmd_LArInstKillWLS->GetNewBoolValue(newValues)) {
		  MGLog(trace) << "WLS tracks will be killed after WLS process." << endlog;
	  } else {
		  MGLog(trace) << "WLS tracks will NOT be killed after WLS process." << endlog;
	  }
  }
  else if (command == fCmd_EGeHitThreshold) {
	  fOutputScheme->SetCrystalEnLow(fCmd_EGeHitThreshold->GetNewDoubleValue(newValues));
	  MGLog(trace) << "Lower Energy threshold for hits in the Ge crystals now set to " <<  fCmd_EGeHitThreshold->GetNewDoubleValue(newValues) << " ." << endlog;
  }
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
