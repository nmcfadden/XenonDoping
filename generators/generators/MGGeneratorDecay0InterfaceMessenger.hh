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
// Messenger for Decay0 generator
//
// History:
// --------
// 25 Oct 2004   L.Pandola    First implementation
// 06 Dec 2004   L.Pandola    Re-naming fest!
// 23 Dec 2005   L.Pandola    Further renaming to comply with MGGenerator
// 09 Jul 2010   L.Pandola    Added commands to kill e+/e-/gammas
// 14 Mar 2012   L.Pandola    Added commands to recycle the same file when over
//
#ifndef MGGeneratorDecay0InterfaceMessenger_H
#define MGGeneratorDecay0InterfaceMessenger_H 1

class MGGeneratorDecay0Interface;
class G4UIdirectory;
class G4UIcmdWithAString; 
class G4UIcommand;
class G4UIcmdWithABool;

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorDecay0InterfaceMessenger : public G4UImessenger
{
public:

  MGGeneratorDecay0InterfaceMessenger(MGGeneratorDecay0Interface*);
  ~MGGeneratorDecay0InterfaceMessenger();
  void SetNewValue(G4UIcommand*,G4String);

private:
  MGGeneratorDecay0Interface* fPointerToInterface;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fCommand;
  G4UIcmdWithABool* fGammaKillCmd;
  G4UIcmdWithABool* fElectronKillCmd;
  G4UIcmdWithABool* fPositronKillCmd;

  G4UIcmdWithABool* fRecycleFileCmd;
};

#endif

