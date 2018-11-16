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
// History:
// --------
// 13 Sep 2005  X. Liu       First implementation
// 25 Jan 2006 L. Pandola   Added commands
// 11 Apr 2006 L. Pandola   Cone-sampling corrected
// 11 Nov 2006 L. Pandola   Added commands for "centric-sampling" with a cone
// 29 May 2008 L. Pandola   Added commands to sample energy from a file spectrum

//Geant4 includes 
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4ParticleGun.hh"

//MaGe includes
#include "generators/MGGeneratorG4Gun.hh"
#include "generators/MGGeneratorG4GunMessenger.hh"
#include "io/MGLogger.hh"

MGGeneratorG4GunMessenger::MGGeneratorG4GunMessenger(MGGeneratorG4Gun* ptr) :
  fPointerToMGG4Gun(ptr)
{
  fDirectory = new G4UIdirectory("/MG/generator/g4gun/");
  fDirectory->SetGuidance("Control of MG G4Gun event generator");

  fTurnOnCommand = new G4UIcmdWithABool
                       ("/MG/generator/g4gun/centric_effect_on",this);
  fTurnOnCommand->SetParameterName("FlagCentricEffect",true);
  fTurnOnCommand->SetDefaultValue(false);
  fTurnOnCommand->SetGuidance("Turn on or off centric effect");

  fCenterPositionCmd = new G4UIcmdWith3VectorAndUnit
                           ("/MG/generator/g4gun/detector_center_position",this);
  fCenterPositionCmd->SetGuidance("Define detector array center position");
  fCenterPositionCmd->SetDefaultUnit("cm");
  fCenterPositionCmd->SetUnitCategory("Length");
  fCenterPositionCmd->SetUnitCandidates("microm mm cm m km");

  fCenterPositionSmearCmd = new G4UIcmdWithADoubleAndUnit
                                ("/MG/generator/g4gun/detector_position_smear",this);
  fCenterPositionSmearCmd->SetGuidance("Define detector center position smearing");
  fCenterPositionSmearCmd->SetDefaultUnit("cm");
  fCenterPositionSmearCmd->SetUnitCategory("Length");
  fCenterPositionSmearCmd->SetUnitCandidates("microm mm cm m km");

  fTurnOnCentricConeCmd = new G4UIcmdWithABool
                       ("/MG/generator/g4gun/centric_effect_cone",this);
  fTurnOnCentricConeCmd->SetDefaultValue(false);
  fTurnOnCentricConeCmd->SetGuidance("Generate primary particles in the target direction");
  fTurnOnCentricConeCmd->SetGuidance("within a cone of given opening angle");

  fOpeningAngleCmd= new G4UIcmdWithADoubleAndUnit
    ("/MG/generator/g4gun/opening_angle",this);
  fOpeningAngleCmd->SetGuidance("Set the opening angle of the cone for the centric effect");
  fOpeningAngleCmd->SetGuidance("Default is 0 (= particles shot directly on the target)");
  fOpeningAngleCmd->SetDefaultUnit("rad");
  fOpeningAngleCmd->SetUnitCandidates("rad deg");
  // ---------------------------------------------------------------------------//
  fTurnConeCommand = new G4UIcmdWithABool
    ("/MG/generator/g4gun/cone_on",this);
  fTurnConeCommand->SetParameterName("ConeEffect",true);
  fTurnConeCommand->SetDefaultValue(false);
  fTurnConeCommand->SetGuidance("Turn on/off the sampling of the direction over a cone");
  fTurnConeCommand->SetGuidance("default is false (= fixed direction)");


  fDeltaThetaCmd= new G4UIcmdWithADoubleAndUnit
    ("/MG/generator/g4gun/thetaDelta",this);
  fDeltaThetaCmd->SetGuidance("Define the delta-theta for the ");
  fDeltaThetaCmd->SetGuidance("isotropic sampling of the direction over a cone");
  fDeltaThetaCmd->SetGuidance("(default is pi)");
  fDeltaThetaCmd->SetDefaultUnit("rad");
  fDeltaThetaCmd->SetUnitCandidates("rad deg");
  
  fConeAxisDirectionCmd = new G4UIcmdWith3Vector
    ("/MG/generator/g4gun/coneDirection",this);
  fConeAxisDirectionCmd->SetGuidance("Set the direction of the cone axis");
  fConeAxisDirectionCmd->SetGuidance(" - no need to be normalized - ");
  fConeAxisDirectionCmd->SetGuidance("(default is the z-axis)");

  // ---------------------------------------------------------------------------//
  fReadFromFileCmd = new G4UIcmdWithABool
    ("/MG/generator/g4gun/spectrum_from_file",this);
  fReadFromFileCmd->SetDefaultValue(false);
  fReadFromFileCmd->SetGuidance("Turn on/off the sampling of energy from a histogram read from file");
  fReadFromFileCmd->SetGuidance("default is false (= fixed energy)");
  fReadFromFileCmd->SetGuidance("File to be read can be set with /MG/generator/g4gun/spectrum_filename");

  fSetFileNameCmd = new G4UIcmdWithAString
    ("/MG/generator/g4gun/spectrum_filename",this);
  fSetFileNameCmd->SetGuidance("Set file name for sampling energy spectrum from a file-based histogram");
}

//---------------------------------------------------------------------------//

MGGeneratorG4GunMessenger::MGGeneratorG4GunMessenger(const MGGeneratorG4GunMessenger & other) : G4UImessenger(other)
{;}

MGGeneratorG4GunMessenger::~MGGeneratorG4GunMessenger()
{
  delete fDirectory;
  delete fTurnOnCommand;
  delete fCenterPositionCmd;
  delete fCenterPositionSmearCmd;
  delete fTurnConeCommand;
  delete fDeltaThetaCmd;
  delete fConeAxisDirectionCmd;
  delete fTurnOnCentricConeCmd;
  delete fOpeningAngleCmd;
  delete fReadFromFileCmd;
  delete fSetFileNameCmd;
}

//---------------------------------------------------------------------------//

void MGGeneratorG4GunMessenger::SetNewValue(G4UIcommand* cmd,G4String str)
{
  if (cmd == fTurnOnCommand) 
    {
      fPointerToMGG4Gun->SetCentricEffectFlag(fTurnOnCommand->GetNewBoolValue(str)); 
    }
  else if (cmd == fCenterPositionCmd)
    {
      fPointerToMGG4Gun->SetCenterPosition(fCenterPositionCmd->GetNew3VectorValue(str));
    }
  else if (cmd == fCenterPositionSmearCmd)
    {
      fPointerToMGG4Gun->SetCenterPositionSmear(fCenterPositionSmearCmd->GetNewDoubleValue(str));
    }
  else if (cmd == fTurnConeCommand)
    {
      fPointerToMGG4Gun->SetIsotropicConeFlag(fTurnConeCommand->GetNewBoolValue(str));
      MGLog(routine) << "Direction sampling from a cone" << endlog;
    }
  else if (cmd == fDeltaThetaCmd)
    {
      fPointerToMGG4Gun->SetConeDeltaTheta(fDeltaThetaCmd->GetNewDoubleValue(str));
      if (!(fPointerToMGG4Gun->GetIsotropicConeFlag()))
	{
	  MGLog(routine) << "The command will have effect only if you give " << 
	    " /MG/generator/g4gun/cone_on true" << endlog;
	  }
    }
  else if (cmd == fConeAxisDirectionCmd)
    {
      fPointerToMGG4Gun->SetConeAxisDirection(fConeAxisDirectionCmd->GetNew3VectorValue(str));
      if (!(fPointerToMGG4Gun->GetIsotropicConeFlag()))
	{
	  MGLog(routine) << "The command will have effect only if you give " << 
	    " /MG/generator/g4gun/cone_on true" << endlog;
	  }
    }
  else if (cmd == fTurnOnCentricConeCmd)
    {
      fPointerToMGG4Gun->SetCentricEffectConeFlag(fTurnOnCentricConeCmd->GetNewBoolValue(str));
    }
  else if (cmd == fOpeningAngleCmd)
    {
      fPointerToMGG4Gun->SetCentricConeAngle(fOpeningAngleCmd->GetNewDoubleValue(str));
    }
  else if (cmd == fReadFromFileCmd)
    {
      fPointerToMGG4Gun->SetReadSpectrumFromFile(fReadFromFileCmd->GetNewBoolValue(str));
    }
  else if (cmd == fSetFileNameCmd)
    {
      fPointerToMGG4Gun->SetFileNameFromFile(str);
    }
}

//---------------------------------------------------------------------------//
