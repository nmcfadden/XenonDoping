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
// $Id: MJDemoCalibrationAssemblyMessenger.cc,v 1.1 2016/02/24 17:30:00 tcald Exp$ 
//      
// CLASS IMPLEMENTATION:  MJDemoCalibrationAssemblyMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Messenger class for the calibration assembly
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Tom Caldwell
 * CONTACT: tcald@unc.edu
 * FIRST SUBMISSION: Feb 24, 2016
 * 
 * REVISION:
 * 
 * 02-24-2016, Created, T. Caldwell
 */
//---------------------------------------------------------------------------//
//

#include "mjdemonstrator/MJDemoCalibrationAssemblyMessenger.hh"
#include "io/MGLogger.hh"
#include "G4UImanager.hh"

//---------------------------------------------------------------------------//

MJDemoCalibrationAssemblyMessenger::MJDemoCalibrationAssemblyMessenger(G4String index)
{
  std::string dir = "/MG/mjdemocalassembly" + index + "/";
  fCalAssembly = new MJDemoCalibrationAssembly("tmp_cal", "tmp_cal");
  fCalSourceMessenger = new MJDemoCalibrationSourceMessenger(index);
  fCalTrackMessenger = new MJDemoCalibrationTrackMessenger(index);
  
  // /MG/mjdemocalassembly
  fCalAssemblyDir = new G4UIdirectory(G4String(dir));

  // /MG/mjdemocalassembly/sourceOn
  fSourceOnCmd = new G4UIcmdWithABool(G4String(dir + "sourceOn"), this);
  fSourceOnCmd->SetGuidance("Calibration source eneable.");
  fSourceOnCmd->SetDefaultValue(fCalAssembly->GetSourceOn());

  // /MG/mjdemocalassembly/trackOn
  fTrackOnCmd = new G4UIcmdWithABool(G4String(dir + "trackOn"), this);
  fTrackOnCmd->SetGuidance("Calibration track enable.");
  fTrackOnCmd->SetDefaultValue(fCalAssembly->GetTrackOn());

  // /MG/mjdemocalassembly/sourceOffset
  fSourceOffsetCmd =
    new G4UIcmdWith3VectorAndUnit(G4String(dir + "sourceOffset"), this);
  fSourceOffsetCmd->SetGuidance("Offset of the calibration source.");
  fSourceOffsetCmd->SetDefaultValue(fCalAssembly->GetSourceOffset());
  fSourceOffsetCmd->SetDefaultUnit("cm");
  fSourceOffsetCmd->SetUnitCandidates("mm cm m");

  // /MG/mjdemocalassembly/trackOffset
  fTrackOffsetCmd =
    new G4UIcmdWith3VectorAndUnit(G4String(dir + "trackOffset"), this);
  fTrackOffsetCmd->SetGuidance("Offset of the calibration track.");
  fTrackOffsetCmd->SetDefaultValue(fCalAssembly->GetTrackOffset());
  fTrackOffsetCmd->SetDefaultUnit("cm");
  fTrackOffsetCmd->SetUnitCandidates("mm cm m");
}

//---------------------------------------------------------------------------//

MJDemoCalibrationAssemblyMessenger::~MJDemoCalibrationAssemblyMessenger()
{
  delete fCalSourceMessenger;
  delete fCalTrackMessenger;
  delete fSourceOnCmd;
  delete fTrackOnCmd;
  delete fSourceOffsetCmd;
  delete fTrackOffsetCmd;
  delete fCalAssemblyDir;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationAssemblyMessenger::SetNewValue(G4UIcommand* cmd,
						     G4String nv)
{
  if(cmd == fSourceOnCmd){
    fCalAssembly->SetSourceOn(fSourceOnCmd->GetNewBoolValue(nv));
    fSourceOnCmd->SetDefaultValue(fCalAssembly->GetSourceOn());
  }
  else if(cmd == fTrackOnCmd){
    fCalAssembly->SetTrackOn(fTrackOnCmd->GetNewBoolValue(nv));
    fTrackOnCmd->SetDefaultValue(fCalAssembly->GetTrackOn());
  }
  else if(cmd == fSourceOffsetCmd){
    fCalAssembly->SetSourceOffset(fSourceOffsetCmd->GetNew3VectorValue(nv));
    fSourceOffsetCmd->SetDefaultValue(fCalAssembly->GetSourceOffset());
  }
  else if(cmd == fTrackOffsetCmd){
    fCalAssembly->SetTrackOffset(fTrackOffsetCmd->GetNew3VectorValue(nv));
    fTrackOffsetCmd->SetDefaultValue(fCalAssembly->GetTrackOffset());
  }
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationAssemblyMessenger::SetValues(MJDemoCalibrationAssembly* calAssembly)
{
  fCalSourceMessenger->SetValues(calAssembly->GetSource());
  fCalTrackMessenger->SetValues(calAssembly->GetTrack());
  MJVDemoAssembly* tmp = new MJVDemoAssembly();
  *tmp = *(MJVDemoAssembly*) calAssembly;
  calAssembly->SetTrackOn(fCalAssembly->GetTrackOn());
  calAssembly->SetSourceOn(fCalAssembly->GetSourceOn());
  calAssembly->SetTrackOffset(fCalAssembly->GetTrackOffset());
  calAssembly->SetSourceOffset(fCalAssembly->GetSourceOffset());
    *(MJVDemoAssembly*) calAssembly = *tmp;
  delete tmp;
  delete fCalAssembly;
  fCalAssembly = calAssembly;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
