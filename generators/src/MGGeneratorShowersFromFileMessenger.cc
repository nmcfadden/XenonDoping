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
// Messenger for the dangerous muon geneator
//
// History:
// --------
// xx.08.2015 R. Massarczyk


#include "generators/MGGeneratorShowersFromFileMessenger.hh"
#include "generators/MGGeneratorShowersFromFile.hh"
#include "io/MGLogger.hh"

#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "G4Tokenizer.hh"

MGGeneratorShowersFromFileMessenger::MGGeneratorShowersFromFileMessenger(MGGeneratorShowersFromFile *fPtclGun) : fParticleGun(fPtclGun)
{
  fDirectory = new G4UIdirectory("/MG/generator/ShowersFromFile/");
  fDirectory->SetGuidance("Control of ShowersFromFile event generator");

  // name of the file
  fileNameCmd = new G4UIcmdWithAString("/MG/generator/ShowersFromFile/filename", this);
  fileNameCmd->SetGuidance("Name of the file containing the shower information");

  // only muons
  onlyMuonsCmd = new G4UIcmdWithABool("/MG/generator/ShowersFromFile/onlyMuons", this);
  onlyMuonsCmd->SetGuidance("simulate only muons");
  onlyMuonsCmd->SetParameterName("onlyMuons",true);
  onlyMuonsCmd->SetDefaultValue(false);
}


MGGeneratorShowersFromFileMessenger::~MGGeneratorShowersFromFileMessenger()
{
  delete fileNameCmd;
  delete onlyMuonsCmd;
}

void MGGeneratorShowersFromFileMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  if (command == fileNameCmd){
    fParticleGun->SetFileName(newValues);
  }
  else if (command == onlyMuonsCmd){
	fParticleGun->OnlyMuons(onlyMuonsCmd->GetNewBoolValue(newValues));
  }
}




