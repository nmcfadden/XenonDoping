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
// 27 Apr 2005   L.Pandola    First implementation
// 24 Jul 2008   L.Pandola    Changed format according to the new MUSUN output to 
//                            be used for the Gerda MCC2.

//Geant4 includes 
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//MaGe includes
#include "generators/MGGeneratorMUSUNInterface.hh"
#include "generators/MGGeneratorMUSUNInterfaceMessenger.hh"

MGGeneratorMUSUNInterfaceMessenger::MGGeneratorMUSUNInterfaceMessenger(MGGeneratorMUSUNInterface* ptr) : 
  fPointerToInterface(ptr)
{
  fDirectory = new G4UIdirectory("/MG/generator/musun/");
  fDirectory->SetGuidance("Control of MUSUN event generator");

  fCommand = new G4UIcmdWithAString("/MG/generator/musun/filename",this);
  fCommand->SetGuidance("Set the file name for MUSUN events");
  
  heightCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/zshift",this);
  heightCmd->SetGuidance("Set the shift of the z coordinate with respect to the MUSUN value");
  heightCmd->SetGuidance("Default value: 0 m");
  heightCmd->SetParameterName("Height",true,true);
  heightCmd->SetDefaultUnit("cm");
  heightCmd->SetUnitCandidates("mm cm m km");

}

MGGeneratorMUSUNInterfaceMessenger::~MGGeneratorMUSUNInterfaceMessenger()
{
  delete fDirectory;
  delete fCommand;
  delete heightCmd;
}

void MGGeneratorMUSUNInterfaceMessenger::SetNewValue(G4UIcommand* cmd,G4String newValues)
{
  if (cmd == fCommand)
    {
      fPointerToInterface->ChangeFileName(newValues);
    }
  else if (cmd == heightCmd)
    {
      fPointerToInterface->SetZShift(heightCmd->GetNewDoubleValue(newValues));
    }
}
