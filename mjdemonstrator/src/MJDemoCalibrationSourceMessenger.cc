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
// $Id: MJDemoCalibrationSourceMessenger.cc,v 1.1 2016/02/24 17:30:00 tcald Exp$ 
//      
// CLASS IMPLEMENTATION:  MJDemoCalibrationSourceMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Messenger class for the calibration source
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

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJDemoCalibrationSourceMessenger.hh"
#include "io/MGLogger.hh"
#include "G4UImanager.hh"

//---------------------------------------------------------------------------//

MJDemoCalibrationSourceMessenger::MJDemoCalibrationSourceMessenger(G4String index)
{
  std::string dir = "/MG/mjdemocalsource" + index + "/";
  fCalSource = new MJDemoCalibrationSource("tmp_source", "tmp_source");
  
  // /MG/mjdemocalsource
  fCalSourceDir = new G4UIdirectory(G4String(dir));

  // /MG/mjdemocalsource/helixRadius
  fHelixRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "helixRadius"), this);
  fHelixRadiusCmd->SetGuidance("Radius of the calibration source helix.");
  fHelixRadiusCmd->SetDefaultValue(fCalSource->GetHelixRadius());
  fHelixRadiusCmd->SetDefaultUnit("cm");
  fHelixRadiusCmd->SetUnitCandidates("mm cm m");

  // /MG/mjdemocalsource/tubeInnerRadius
  fTubeInnerRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "tubeInnerRadius"), this);
  fTubeInnerRadiusCmd->SetGuidance("Inner radius of the calibration source.");
  fTubeInnerRadiusCmd->SetDefaultValue(fCalSource->GetTubeInnerRadius());
  fTubeInnerRadiusCmd->SetDefaultUnit("cm");
  fTubeInnerRadiusCmd->SetUnitCandidates("mm cm m");

  // /MG/mjdemocalsource/tubeOuterRadius
  fTubeOuterRadiusCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "tubeOuterRadius"), this);
  fTubeOuterRadiusCmd->SetGuidance("Outer radius of the calibration source.");
  fTubeOuterRadiusCmd->SetDefaultValue(fCalSource->GetTubeOuterRadius());
  fTubeOuterRadiusCmd->SetDefaultUnit("cm");
  fTubeOuterRadiusCmd->SetUnitCandidates("mm cm m");
  
  // /MG/mjdemocalsource/startAngle
  fStartAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "startAngle"), this);
  fStartAngleCmd->SetGuidance("Starting angle of the source.");
  fStartAngleCmd->SetDefaultValue(fCalSource->GetStartAngle());
  fStartAngleCmd->SetDefaultUnit("deg");
  fStartAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocalsource/totalAngle
  fTotalAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "totalAngle"), this);
  fTotalAngleCmd->SetGuidance("Angle subtended by the calibration source.");
  fTotalAngleCmd->SetDefaultValue(fCalSource->GetTotalAngle());
  fTotalAngleCmd->SetDefaultUnit("deg");
  fTotalAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocalsource/helixAngle
  fHelixAngleCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "helixAngle"), this);
  fHelixAngleCmd->SetGuidance("Incline angle of the calibration source helix.");
  fHelixAngleCmd->SetDefaultValue(fCalSource->GetHelixAngle());
  fHelixAngleCmd->SetDefaultUnit("deg");
  fHelixAngleCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocalsource/angleBuffer
  fAngleBufferCmd =
    new G4UIcmdWithADoubleAndUnit(G4String(dir + "angleBuffer"), this);
  fAngleBufferCmd->SetGuidance("Angular buffer for splicing torus sections.");
  fAngleBufferCmd->SetDefaultValue(fCalSource->GetAngleBuffer());
  fAngleBufferCmd->SetDefaultUnit("deg");
  fAngleBufferCmd->SetUnitCandidates("rad deg");
  
  // /MG/mjdemocalsource/nsegments
  fNsegmentsCmd =
    new G4UIcmdWithAnInteger(G4String(dir + "nsegments"), this);
  fNsegmentsCmd->SetGuidance("Number of torus sections used to "
			     "approximate helix.");
  fNsegmentsCmd->SetDefaultValue(fCalSource->GetNsegments());
}

//---------------------------------------------------------------------------//

MJDemoCalibrationSourceMessenger::~MJDemoCalibrationSourceMessenger()
{
  delete fHelixRadiusCmd;
  delete fTubeInnerRadiusCmd;
  delete fTubeOuterRadiusCmd;
  delete fStartAngleCmd;
  delete fTotalAngleCmd;
  delete fHelixAngleCmd;
  delete fAngleBufferCmd;
  delete fNsegmentsCmd;
  delete fCalSourceDir;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSourceMessenger::SetNewValue(G4UIcommand* cmd,
						   G4String nv)
{
  if(cmd == fHelixRadiusCmd){
    fCalSource->SetHelixRadius(fHelixRadiusCmd->GetNewDoubleValue(nv));
    fHelixRadiusCmd->SetDefaultValue(fCalSource->GetHelixRadius());
  }
  else if(cmd == fTubeInnerRadiusCmd){
    fCalSource->SetTubeInnerRadius(fTubeInnerRadiusCmd->GetNewDoubleValue(nv));
    fTubeInnerRadiusCmd->SetDefaultValue(fCalSource->GetTubeInnerRadius());
  }
  else if(cmd == fTubeOuterRadiusCmd){
    fCalSource->SetTubeOuterRadius(fTubeOuterRadiusCmd->GetNewDoubleValue(nv));
    fTubeOuterRadiusCmd->SetDefaultValue(fCalSource->GetTubeOuterRadius());
  }
  else if(cmd == fStartAngleCmd){
    fCalSource->SetStartAngle(fStartAngleCmd->GetNewDoubleValue(nv));
    fStartAngleCmd->SetDefaultValue(fCalSource->GetStartAngle());
  }
  else if(cmd == fTotalAngleCmd){
    fCalSource->SetTotalAngle(fTotalAngleCmd->GetNewDoubleValue(nv));
    fTotalAngleCmd->SetDefaultValue(fCalSource->GetTotalAngle());
  }
  else if(cmd == fHelixAngleCmd){
    fCalSource->SetHelixAngle(fHelixAngleCmd->GetNewDoubleValue(nv));
    fHelixAngleCmd->SetDefaultValue(fCalSource->GetHelixAngle());
  }
  else if(cmd == fAngleBufferCmd){
    fCalSource->SetAngleBuffer(fAngleBufferCmd->GetNewDoubleValue(nv));
    fAngleBufferCmd->SetDefaultValue(fCalSource->GetAngleBuffer());
  }
  else if(cmd == fNsegmentsCmd){
    fCalSource->SetNsegments(fNsegmentsCmd->GetNewIntValue(nv));
    fNsegmentsCmd->SetDefaultValue(fCalSource->GetNsegments());
  }
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSourceMessenger::SetValues(MJDemoCalibrationSource* calSource)
{
  MJVDemoPart* tmp = new MJVDemoPart();
  *tmp = *(MJVDemoPart*) calSource;  
  *calSource = *fCalSource;
  *(MJVDemoPart*) calSource = *tmp;
  delete tmp;
  delete fCalSource;
  fCalSource = calSource;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
