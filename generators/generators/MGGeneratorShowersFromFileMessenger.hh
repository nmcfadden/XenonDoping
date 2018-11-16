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
// Messenger for the shower geneator
//
// History:
// --------
// xx.08.2015 R. Massarczyk

#ifndef MGGeneratorShowersFromFileMessenger_h
#define MGGeneratorShowersFromFileMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorShowersFromFile;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class MGGeneratorShowersFromFileMessenger: public G4UImessenger {

public:
  MGGeneratorShowersFromFileMessenger(MGGeneratorShowersFromFile *fPtclGun);
  ~MGGeneratorShowersFromFileMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);


private:
  MGGeneratorShowersFromFile *fParticleGun;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fileNameCmd;
  G4UIcmdWithABool* onlyMuonsCmd;
  //G4UIcmdWithAnInteger* eventstoprocessCmd;
};

#endif
