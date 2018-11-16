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
// $Id: MGGeneratorLGND200CalibrationMessenger.cc,v 1.2 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorLGND200CalibrationMessenger.cc
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
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * June-2018, Created, Ralph
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorLGND200Calibration.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorLGND200CalibrationMessenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorLGND200CalibrationMessenger::MGGeneratorLGND200CalibrationMessenger(
MGGeneratorLGND200Calibration *generator) : fLGND200CalibrationGenerator(generator)
{
  // /MG/generator/LGND200Calibration
  fLGND200CalibrationDirectory = new G4UIdirectory("/MG/generator/LGND200Calibration/");
  fLGND200CalibrationDirectory->SetGuidance("LGND200 Calibration track generator control.");

  // MG/generator/LGND200Calibration/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/generator/LGND200Calibration/dump", this);
  fDumpCmd->SetGuidance("Dump parameters of generator to screen");

  // /MG/generator/LGND200Calibration/setZ
  fZCmd = new G4UIcmdWithAnInteger("/MG/generator/LGND200Calibration/setZ", this);
  fZCmd->SetGuidance("The Z value of the ion");
  
  // /MG/generator/LGND200Calibration/setA
  fACmd = new G4UIcmdWithAnInteger("/MG/generator/LGND200Calibration/setA", this);
  fACmd->SetGuidance("The A value of the ion");
  
}

//---------------------------------------------------------------------------//

MGGeneratorLGND200CalibrationMessenger::MGGeneratorLGND200CalibrationMessenger(const MGGeneratorLGND200CalibrationMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorLGND200CalibrationMessenger::~MGGeneratorLGND200CalibrationMessenger()
{
  delete fACmd;
  delete fZCmd;
  delete fDumpCmd;
  delete fLGND200CalibrationDirectory;
}


//---------------------------------------------------------------------------//

void MGGeneratorLGND200CalibrationMessenger::SetNewValue(G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fZCmd)
    fLGND200CalibrationGenerator->SetIonZ(fZCmd->GetNewIntValue(newValues));
  else if(cmd == fACmd)
    fLGND200CalibrationGenerator->SetIonA(fACmd->GetNewIntValue(newValues));
	else if(cmd = fDumpCmd)
		fLGND200CalibrationGenerator->Dump();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
