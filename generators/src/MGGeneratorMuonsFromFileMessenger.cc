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
// 15.12.05 Markus Knapp

#include "generators/MGGeneratorMuonsFromFileMessenger.hh"
#include "generators/MGGeneratorMuonsFromFile.hh"
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

MGGeneratorMuonsFromFileMessenger::MGGeneratorMuonsFromFileMessenger(MGGeneratorMuonsFromFile *fPtclGun) : fParticleGun(fPtclGun) 
{
  fDirectory = new G4UIdirectory("/MG/generator/MuonsFromFile/");
  fDirectory->SetGuidance("Control of MuonsFromFile event generator");

  // name of the file containing the dangerous muons
  fileNameCmd = new G4UIcmdWithAString("/MG/generator/MuonsFromFile/filename", this);
  fileNameCmd->SetGuidance("Name of the file containing the dangerous muons");


}

MGGeneratorMuonsFromFileMessenger::~MGGeneratorMuonsFromFileMessenger() 
{
  delete fileNameCmd;
}

void MGGeneratorMuonsFromFileMessenger::SetNewValue(G4UIcommand *command, G4String newValues) 
{
  if (command == fileNameCmd)
    {
      fParticleGun->SetFileName(newValues);
    }

}




