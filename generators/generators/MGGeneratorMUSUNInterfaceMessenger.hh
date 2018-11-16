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
// Messenger for MUSUN cosmic ray generator
//
// History:
// --------
// 27 Apr 2005   L.Pandola    First implementation
// 24 Jul 2008   L.Pandola    Changed format according to the new MUSUN output to 
//                            be used for the Gerda MCC2. 

#ifndef MGGeneratorMUSUNInterfaceMessenger_H
#define MGGeneratorMUSUNInterfaceMessenger_H 1

class MGGeneratorMUSUNInterface;
class G4UIdirectory;
class G4UIcmdWithAString; 
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorMUSUNInterfaceMessenger : public G4UImessenger
{
public:

  MGGeneratorMUSUNInterfaceMessenger(MGGeneratorMUSUNInterface*);
  ~MGGeneratorMUSUNInterfaceMessenger();
  void SetNewValue(G4UIcommand*,G4String);

private:
  MGGeneratorMUSUNInterface* fPointerToInterface;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fCommand;
  G4UIcmdWithADoubleAndUnit* heightCmd;
};

#endif

