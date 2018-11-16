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
// $Id: MGProcessesMessenger.cc,v 1.6 2009-01-17 15:14:12 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessesMessenger.cc
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
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Sep 14 15:03:59 PDT 2004
 * 
 * REVISION:
 *
 * 09-14-2004, Kareem
 *             Initial file creation and submission
 * 12-15-2004, Luciano
 *             Registered new realm "CosmicRays"
 * 02-14-2005, Luciano
 *             Added bool command to switch on/off optical processes
 * 02-16-2005, Luciano
 *             Added bool command to switch on/off lowE EM physics
 * 07-12-2005, Luciano     
 *             Added command to switch on/off QGSP_GN_HP_BIC_ISO alternative
 *             physics list for neutrons, protons and muons
 * 17 Apr 2006, M. Marino     
 *             Added commands to turn on QGSC, Bertini Cascade and 
 *             muonNucleus interaction.
 * 24 Mar 2014, L.Pandola
 *             Added command to switch between FTFP and QGSP (default)
 * 23-08-2017, Ralph Massarczyk     Added command to get limits
 * 25-06-2018, Ralph Massarczyk     Added different physics list options, when lowenergy flag enabled
 */

//---------------------------------------------------------------------------//

//  MG headers
#include "processes/MGProcessesMessenger.hh"      //Present MG Class Headers 
#include "processes/MGProcessesList.hh"
#include "processes/MGUIcmdStepLimit.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"


//  GEANT4 headers
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "globals.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ProcessManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4StepLimiter.hh"


//---------------------------------------------------------------------------//
MGProcessesMessenger::MGProcessesMessenger( MGProcessesList *list ) :
theList( list )
{
	MGProcessesDir = new G4UIdirectory( "/MG/processes/" );
	MGProcessesDir->SetGuidance( "UI commands to control the energy realm of the simulation" );
	
	//  /MG/processes/realm
	MGProcessesChoiceCommand = new G4UIcmdWithAString( "/MG/processes/realm", this );
	MGProcessesChoiceCommand->SetGuidance( "Select the simulation realm" );
	MGProcessesChoiceCommand->SetParameterName( "simRealm", false );
	MGProcessesChoiceCommand->SetCandidates( "BBdecay DarkMatter CosmicRays OpticalPhoton" );
	MGProcessesChoiceCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	// /MG/processes/optical
	opticalProcessesCmd = new G4UIcmdWithABool("/MG/processes/optical",this);
	opticalProcessesCmd->SetGuidance("Switch on/off optical processes");
	opticalProcessesCmd->SetGuidance("(default: false)");
	opticalProcessesCmd->AvailableForStates(G4State_PreInit);

  // /MG/processes/opticalOnly
  opticalOnlyCmd = new G4UIcmdWithABool("/MG/processes/opticalOnly",this);
  opticalOnlyCmd->SetGuidance("Only use optical processes");
  opticalOnlyCmd->SetGuidance("(default: false)");
  opticalOnlyCmd->AvailableForStates(G4State_PreInit);  

	// /MG/processes/lowenergy
	lowEnergyProcessesCmd = new G4UIcmdWithABool("/MG/processes/lowenergy",this);
	lowEnergyProcessesCmd->SetGuidance("Set on/off the LowE EM processes");
	lowEnergyProcessesCmd->SetGuidance("(default: true)");
	lowEnergyProcessesCmd->AvailableForStates(G4State_PreInit);

	// /MG/processes/lowenergy Options
	lowEnergyProcessesOptionCmd = new G4UIcmdWithAnInteger("/MG/processes/lowenergyOption",this);
	lowEnergyProcessesOptionCmd->SetGuidance("Switch between physics list options when lowenergy flag enabled");
	lowEnergyProcessesOptionCmd->SetGuidance("see https://geant4.web.cern.ch/node/1731#Ex");
	lowEnergyProcessesOptionCmd->SetGuidance("0 (default) Livermore");
	lowEnergyProcessesOptionCmd->SetGuidance("1           EmOption1");
	lowEnergyProcessesOptionCmd->SetGuidance("2           EmOption2");
	lowEnergyProcessesOptionCmd->SetGuidance("3           EmOption3");
	lowEnergyProcessesOptionCmd->SetGuidance("4           EmOption4");
	lowEnergyProcessesOptionCmd->SetGuidance("5           Penelope");
	lowEnergyProcessesOptionCmd->SetGuidance("6           LivermorePolarized");
	lowEnergyProcessesOptionCmd->AvailableForStates(G4State_PreInit);
	lowEnergyProcessesOptionCmd->SetDefaultValue(0);

        // /MG/process/useBertCascade
	fUseBertiniCascadeCmd = new G4UIcmdWithABool("/MG/processes/useBertCascade",this);
	fUseBertiniCascadeCmd->SetGuidance("Uses Bertini Cascade");
	fUseBertiniCascadeCmd->SetGuidance("For hadrons and mesons");
	fUseBertiniCascadeCmd->SetGuidance("(default: false)");
	fUseBertiniCascadeCmd->AvailableForStates(G4State_PreInit);       

	// /MG/process/useFTF
	fUseFTFCmd = new G4UIcmdWithABool("/MG/processes/useFTF",this);
	fUseFTFCmd->SetGuidance("Uses FTF for the high energy part instead of QGSP");
	fUseFTFCmd->SetGuidance("For hadrons and mesons");
	fUseFTFCmd->SetGuidance("(default: false)");
	fUseFTFCmd->SetGuidance("Works at the moment only with Bertini (not binary)");
	fUseFTFCmd->AvailableForStates(G4State_PreInit);       

	// /MG/process/useAllHP
        fUseAllHPCmd = new G4UIcmdWithABool("/MG/processes/useAllHP",this);
        fUseAllHPCmd->SetGuidance("Uses ParticleHP with QGSP & Binary Cascade");
        fUseAllHPCmd->SetGuidance("(default: false)");
        fUseAllHPCmd->AvailableForStates(G4State_PreInit);

        // /MG/process/useNoHadPhysics
	fUseNoHadPhysicsCmd = new G4UIcmdWithABool("/MG/processes/useNoHadPhysics",this);
	fUseNoHadPhysicsCmd->SetGuidance("Shuts off Hadronic Physics");
	fUseNoHadPhysicsCmd->SetGuidance("(default: false)");
	fUseNoHadPhysicsCmd->AvailableForStates(G4State_PreInit);

   // /MG/process/setStepLimit
   fStepLimitCommand = new MGUIcmdStepLimit("/MG/processes/setStepLimit", this);


   // /MG/process/setStepLimit
   fGetStepLimitCommand = new G4UIcmdWithoutParameter("/MG/processes/GetStepLimit", this);
   fGetStepLimitCommand->SetGuidance("Prints step limits for all particles");

   // /MG/process/setStepLimit
   fGetStepLimitCommand = new G4UIcmdWithoutParameter("/MG/processes/GetStepLimit", this);
 	 fGetStepLimitCommand->SetGuidance("Prints step limits for all particles");

   fAngCorrCommand = new G4UIcmdWithAnInteger("/MG/processes/useAngCorr", this);
   fAngCorrCommand->SetGuidance("Turn on angular correlations and optionally set max two J to something other than 20");
   fAngCorrCommand->SetGuidance("Must be called before /run/initialize");
   fAngCorrCommand->SetParameterName("maxTwoJ", true);
   fAngCorrCommand->SetDefaultValue(20);
}

//---------------------------------------------------------------------------//
//MGProcessesMessenger::MGProcessesMessenger(const MGProcessesMessenger & other)
//{
//
//}

//---------------------------------------------------------------------------//
MGProcessesMessenger::~MGProcessesMessenger()
{
	delete MGProcessesChoiceCommand;
	delete MGProcessesDir;
	delete opticalProcessesCmd;
  delete opticalOnlyCmd;
	delete lowEnergyProcessesCmd;
	delete lowEnergyProcessesOptionCmd;
	delete fUseBertiniCascadeCmd;
	delete fUseFTFCmd;
	delete fUseAllHPCmd;
  delete fUseNoHadPhysicsCmd;
  delete fStepLimitCommand;	
  delete fGetStepLimitCommand;
  delete fAngCorrCommand;
}

//---------------------------------------------------------------------------//
void MGProcessesMessenger::SetNewValue( G4UIcommand *command, G4String newValue )
{

	if( command == MGProcessesChoiceCommand )
		theList->SetRealm( newValue );
	else if (command == opticalProcessesCmd) 
	  theList->SetOpticalFlag(opticalProcessesCmd->GetNewBoolValue(newValue));
  else if (command == opticalOnlyCmd)
    theList->SetOpticalPhysicsOnly(opticalOnlyCmd->GetNewBoolValue(newValue));
	else if (command == lowEnergyProcessesCmd)
	  theList->SetLowEnergyFlag(lowEnergyProcessesCmd->GetNewBoolValue(newValue));
	else if (command == lowEnergyProcessesOptionCmd)
		theList->SetLowEnergyOption(lowEnergyProcessesOptionCmd->GetNewIntValue(newValue));
  else if (command == fUseBertiniCascadeCmd)
	  theList->SetUseBertiniCascadeFlag(fUseBertiniCascadeCmd->GetNewBoolValue(newValue));     
	else if (command == fUseFTFCmd)
	  theList->SetUseFTFFlag(fUseFTFCmd->GetNewBoolValue(newValue));

  else if (command == fUseAllHPCmd)
          theList->SetUseAllHPFlag(fUseAllHPCmd->GetNewBoolValue(newValue));
  else if( command == fUseNoHadPhysicsCmd)
          theList->SetUseNoHadPhysFlag(fUseNoHadPhysicsCmd->GetNewBoolValue(newValue));
  else if( command == fStepLimitCommand) {
          G4String particleName = fStepLimitCommand->GetParticleName(newValue);
          theList->LimitStepForParticle(particleName);
          MGLog(trace) << "Limit step for " << particleName << endlog;

          G4String volumeName = fStepLimitCommand->GetVolumeName(newValue);
          G4double maxStep = fStepLimitCommand->GetStepSize(newValue);
          MGManager::GetMGManager()->GetManagerDetectorConstruction()->SetMaxStepLimit(volumeName, maxStep);
        }
	else if (command == fGetStepLimitCommand) 
	  theList->GetStepLimits();
  else if( command == fAngCorrCommand) {
    theList->SetUseAngCorr(fAngCorrCommand->GetNewIntValue(newValue));
  }

}
