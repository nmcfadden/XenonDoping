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
// $Id: MGGeneratorMJDCalibrationMessenger.cc,v 1.2 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorMJDCalibrationMessenger.cc
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
 * AUTHOR: K. Vorren
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-24-2016, Removed most commands from messenger since they are now
 *             read directly from the geometry, T. Caldwell
 * 07-14-2014, Created, K. Vorren
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorMJDCalibration.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorMJDCalibrationMessenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorMJDCalibrationMessenger::MGGeneratorMJDCalibrationMessenger(MGGeneratorMJDCalibration *generator) : fMJDCalibrationGenerator(generator)
{
  // /MG/generator/MJDCalibration
  fMJDCalibrationDirectory = new G4UIdirectory("/MG/generator/MJDCalibration/");
  fMJDCalibrationDirectory->SetGuidance("MJD Calibration track generator control.");

  // MG/generator/MJDCalibration/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/generator/MJDCalibration/dump", this);
  fDumpCmd->SetGuidance("Dump parameters of generator to screen");

  // /MG/generator/MJDCalibration/setZ
  fZCmd = new G4UIcmdWithAnInteger("/MG/generator/MJDCalibration/setZ", this);
  fZCmd->SetGuidance("The Z value of the ion");
  generator->SetIonZ(90);
  fZCmd->SetDefaultValue(generator->GetIonZ());
  
  // /MG/generator/MJDCalibration/setA
  fACmd = new G4UIcmdWithAnInteger("/MG/generator/MJDCalibration/setA", this);
  fACmd->SetGuidance("The A value of the ion");
  generator->SetIonA(228);
  fACmd->SetDefaultValue(generator->GetIonZ());
  
  // /MG/generator/MJDCalibration/setSourcePos
  fSourcePosCmd =
    new G4UIcmdWithAString("/MG/generator/MJDCalibration/setSourcePos", this);
  fSourcePosCmd->SetGuidance("West or East cryostat position.");
  fSourcePosCmd->SetCandidates("W E");
  fSourcePosCmd->SetDefaultValue("W");
  fMJDCalibrationGenerator->SetSourcePos("W");
}

//---------------------------------------------------------------------------//

MGGeneratorMJDCalibrationMessenger::MGGeneratorMJDCalibrationMessenger(const MGGeneratorMJDCalibrationMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorMJDCalibrationMessenger::~MGGeneratorMJDCalibrationMessenger()
{
  delete fACmd;
  delete fZCmd;
  delete fDumpCmd;
  delete fMJDCalibrationDirectory;
}

//---------------------------------------------------------------------------//

G4String MGGeneratorMJDCalibrationMessenger::GetCurrentValue(G4UIcommand *cmd)
{
  G4String cv = " ";
  if(cmd == fDumpCmd)
    fMJDCalibrationGenerator->Dump();

  return cv;
}

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibrationMessenger::SetNewValue(G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fZCmd)
    fMJDCalibrationGenerator->SetIonZ(fZCmd->GetNewIntValue(newValues));
  else if(cmd == fACmd)
    fMJDCalibrationGenerator->SetIonA(fACmd->GetNewIntValue(newValues));
  else if(cmd == fSourcePosCmd)
    fMJDCalibrationGenerator->SetSourcePos(newValues);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
