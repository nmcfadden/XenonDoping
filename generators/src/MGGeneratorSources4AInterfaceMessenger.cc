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
// Messenger for the Sources4A neutron generator
//
// History:
// --------
// 25 Jan 2005   L.Pandola    First implementation
 
#include "generators/MGGeneratorSources4AInterface.hh"
#include "generators/MGGeneratorSources4AInterfaceMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"

MGGeneratorSources4AInterfaceMessenger::MGGeneratorSources4AInterfaceMessenger
(MGGeneratorSources4AInterface* ptr) : 
  fPointerToInterface(ptr)
{
  fDirectory = new G4UIdirectory("/MG/generator/sources4a/");
  fDirectory->SetGuidance("Control of Sources4A event generator");

  fCommand = new G4UIcmdWithAString("/MG/generator/sources4a/filename",this);
  fCommand->SetGuidance("Set the file name for Sources4A events");
}

MGGeneratorSources4AInterfaceMessenger::~MGGeneratorSources4AInterfaceMessenger()
{
  delete fDirectory;
  delete fCommand;
}

void MGGeneratorSources4AInterfaceMessenger::SetNewValue(G4UIcommand* cmd,G4String str)
{
  if (cmd == fCommand)
    {
      fPointerToInterface->ChangeFileName(str);
    }
}
