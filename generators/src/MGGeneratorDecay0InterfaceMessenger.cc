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
// 25 Oct 2004   L.Pandola    First implementation
// 06 Dec 2004   L.Pandola    Re-naming fest!
// 23 Dec 2005   L.Pandola    Further renaming to comply with MGGenerator
// 09 Jul 2010   L.Pandola    Added commands to kill e+/e-/gammas
// 14 Mar 2012   L.Pandola    Added commands to recycle the same file when over
//
//Geant4 includes 
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"

//MaGe includes
#include "generators/MGGeneratorDecay0Interface.hh"
#include "generators/MGGeneratorDecay0InterfaceMessenger.hh"

MGGeneratorDecay0InterfaceMessenger::MGGeneratorDecay0InterfaceMessenger(MGGeneratorDecay0Interface* ptr) : 
  fPointerToInterface(ptr)
{
  fDirectory = new G4UIdirectory("/MG/generator/decay0/");
  fDirectory->SetGuidance("Control of Decay0 event generator");

  fCommand = new G4UIcmdWithAString("/MG/generator/decay0/filename",this);
  fCommand->SetGuidance("Set the file name for decay0 events");

  fGammaKillCmd =  new G4UIcmdWithABool("/MG/generator/decay0/killGamma",this);
  fGammaKillCmd->SetGuidance("Ignore gammas emitted by decay0");
  fGammaKillCmd->SetGuidance("Default = false");

  fElectronKillCmd =  new G4UIcmdWithABool("/MG/generator/decay0/killElectron",this);
  fElectronKillCmd->SetGuidance("Ignore e- emitted by decay0");
  fElectronKillCmd->SetGuidance("Default = false");

  fPositronKillCmd =  new G4UIcmdWithABool("/MG/generator/decay0/killPositron",this);
  fPositronKillCmd->SetGuidance("Ignore e+ emitted by decay0");
  fPositronKillCmd->SetGuidance("Default = false");

  fRecycleFileCmd = new G4UIcmdWithABool("/MG/generator/decay0/recycleFile",this);
  fRecycleFileCmd->SetGuidance("Re-use the same file if it does not contain enough events");
  fRecycleFileCmd->SetGuidance("Default = false");  

}

MGGeneratorDecay0InterfaceMessenger::~MGGeneratorDecay0InterfaceMessenger()
{
  delete fDirectory;
  delete fCommand;
  delete fGammaKillCmd;
  delete fElectronKillCmd;
  delete fPositronKillCmd;
  delete fRecycleFileCmd;
}

void MGGeneratorDecay0InterfaceMessenger::SetNewValue(G4UIcommand* cmd,G4String str)
{
  if (cmd == fCommand)    
    fPointerToInterface->ChangeFileName(str);
  else if (cmd == fGammaKillCmd)
    fPointerToInterface->KillGammas(fGammaKillCmd->GetNewBoolValue(str)); 
  else if (cmd == fElectronKillCmd)
    fPointerToInterface->KillElectrons(fElectronKillCmd->GetNewBoolValue(str)); 
  else if (cmd == fPositronKillCmd)
    fPointerToInterface->KillPositrons(fPositronKillCmd->GetNewBoolValue(str)); 
  else if (cmd == fRecycleFileCmd)
    fPointerToInterface->RecycleFile(fRecycleFileCmd->GetNewBoolValue(str)); 
}
