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
// $Id: MJDemoCalibrationTrackMessenger.cc,v 1.1 2016/02/24 17:30:00 tcald Exp$ 
//      
// CLASS IMPLEMENTATION:  MJDemoCalibrationTrackMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Messenger class for the calibration track
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

#include "mjdemonstrator/MJDemoCalibrationTrackMessenger.hh"
#include "io/MGLogger.hh"
#include "G4UImanager.hh"

//---------------------------------------------------------------------------//

MJDemoCalibrationTrackMessenger::MJDemoCalibrationTrackMessenger(G4String index)
{
  std::string dir = "/MG/mjdemocaltrack" + index + "/";
  fCalTrack = new MJDemoCalibrationTrack("tmp_track", "tmp_track");
  
  // /MG/mjdemocaltrack
  fCalTrackDir = new G4UIdirectory(G4String(dir));

  // /MG/mjdemocaltrack/helixRadius
  fHelixRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "helixRadius"), this);
  fHelixRadiusCmd->SetGuidance("Radius of the calibration track helix.");
  fHelixRadiusCmd->SetDefaultValue(fCalTrack->GetHelixRadius());
  fHelixRadiusCmd->SetDefaultUnit("cm");
  fHelixRadiusCmd->SetUnitCandidates("mm cm m");

  // /MG/mjdemocaltrack/tubeInnerRadius
  fTubeInnerRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "tubeInnerRadius"), this);
  fTubeInnerRadiusCmd->SetGuidance("Inner radius of the calibration track.");
  fTubeInnerRadiusCmd->SetDefaultValue(fCalTrack->GetTubeInnerRadius());
  fTubeInnerRadiusCmd->SetDefaultUnit("cm");
  fTubeInnerRadiusCmd->SetUnitCandidates("mm cm m");

  // /MG/mjdemocaltrack/tubeOuterRadius
  fTubeOuterRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "tubeOuterRadius"), this);
  fTubeOuterRadiusCmd->SetGuidance("Outer radius of the calibration track.");
  fTubeOuterRadiusCmd->SetDefaultValue(fCalTrack->GetTubeOuterRadius());
  fTubeOuterRadiusCmd->SetDefaultUnit("cm");
  fTubeOuterRadiusCmd->SetUnitCandidates("mm cm m");
  
  // /MG/mjdemocaltrack/startAngle
  fStartAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "startAngle"), this);
  fStartAngleCmd->SetGuidance("Starting angle of the track.");
  fStartAngleCmd->SetDefaultValue(fCalTrack->GetStartAngle());
  fStartAngleCmd->SetDefaultUnit("deg");
  fStartAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocaltrack/totalAngle
  fTotalAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "totalAngle"), this);
  fTotalAngleCmd->SetGuidance("Angle subtended by the calibration track.");
  fTotalAngleCmd->SetDefaultValue(fCalTrack->GetTotalAngle());
  fTotalAngleCmd->SetDefaultUnit("deg");
  fTotalAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocaltrack/helixAngle
  fHelixAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "helixAngle"), this);
  fHelixAngleCmd->SetGuidance("Incline angle of the calibration track helix.");
  fHelixAngleCmd->SetDefaultValue(fCalTrack->GetHelixAngle());
  fHelixAngleCmd->SetDefaultUnit("deg");
  fHelixAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocaltrack/angleBuffer
  fAngleBufferCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "angleBuffer"), this);
  fAngleBufferCmd->SetGuidance("Angular buffer for splicing torus sections.");
  fAngleBufferCmd->SetDefaultValue(fCalTrack->GetAngleBuffer());
  fAngleBufferCmd->SetDefaultUnit("deg");
  fAngleBufferCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocaltrack/nsegments
  fNsegmentsCmd =
    new G4UIcmdWithAnInteger(G4String(dir + "nsegments"), this);
  fNsegmentsCmd->SetGuidance("Number of torus sections used to "
			     "approximate helix.");
  fNsegmentsCmd->SetDefaultValue(fCalTrack->GetNsegments());
}

//---------------------------------------------------------------------------//

MJDemoCalibrationTrackMessenger::~MJDemoCalibrationTrackMessenger()
{
  delete fHelixRadiusCmd;
  delete fTubeInnerRadiusCmd;
  delete fTubeOuterRadiusCmd;
  delete fStartAngleCmd;
  delete fTotalAngleCmd;
  delete fHelixAngleCmd;
  delete fAngleBufferCmd;
  delete fNsegmentsCmd;
  delete fCalTrackDir;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationTrackMessenger::SetNewValue(G4UIcommand* cmd,
						   G4String nv)
{
  if(cmd == fHelixRadiusCmd){
    fCalTrack->SetHelixRadius(fHelixRadiusCmd->GetNewDoubleValue(nv));
    fHelixRadiusCmd->SetDefaultValue(fCalTrack->GetHelixRadius());
  }
  else if(cmd == fTubeInnerRadiusCmd){
    fCalTrack->SetTubeInnerRadius(fTubeInnerRadiusCmd->GetNewDoubleValue(nv));
    fTubeInnerRadiusCmd->SetDefaultValue(fCalTrack->GetTubeInnerRadius());
  }
  else if(cmd == fTubeOuterRadiusCmd){
    fCalTrack->SetTubeOuterRadius(fTubeOuterRadiusCmd->GetNewDoubleValue(nv));
    fTubeOuterRadiusCmd->SetDefaultValue(fCalTrack->GetTubeOuterRadius());
  }
  else if(cmd == fStartAngleCmd){
    fCalTrack->SetStartAngle(fStartAngleCmd->GetNewDoubleValue(nv));
    fStartAngleCmd->SetDefaultValue(fCalTrack->GetStartAngle());
  }
  else if(cmd == fTotalAngleCmd){
    fCalTrack->SetTotalAngle(fTotalAngleCmd->GetNewDoubleValue(nv));
    fTotalAngleCmd->SetDefaultValue(fCalTrack->GetTotalAngle());
  }
  else if(cmd == fHelixAngleCmd){
    fCalTrack->SetHelixAngle(fHelixAngleCmd->GetNewDoubleValue(nv));
    fHelixAngleCmd->SetDefaultValue(fCalTrack->GetHelixAngle());
  }
  else if(cmd == fAngleBufferCmd){
    fCalTrack->SetAngleBuffer(fAngleBufferCmd->GetNewDoubleValue(nv));
    fAngleBufferCmd->SetDefaultValue(fCalTrack->GetAngleBuffer());
  }
  else if(cmd == fNsegmentsCmd){
    fCalTrack->SetNsegments(fNsegmentsCmd->GetNewIntValue(nv));
    fNsegmentsCmd->SetDefaultValue(fCalTrack->GetNsegments());
  }
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationTrackMessenger::SetValues(MJDemoCalibrationTrack* calTrack)
{
  MJVDemoPart* tmp = new MJVDemoPart();
  *tmp = *(MJVDemoPart*) calTrack;  
  *calTrack = *fCalTrack;
  *(MJVDemoPart*) calTrack = *tmp;
  delete tmp;
  delete fCalTrack;
  fCalTrack = calTrack;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
