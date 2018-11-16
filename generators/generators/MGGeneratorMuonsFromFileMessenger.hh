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
// Messenger for the dangerous muon geneator 
//
// History:
// --------
// 15.12.05 Markus Knapp

#ifndef MGGeneratorMuonsFromFileMessenger_h
#define MGGeneratorMuonsFromFileMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorMuonsFromFile;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class MGGeneratorMuonsFromFileMessenger: public G4UImessenger {
  
public:
  MGGeneratorMuonsFromFileMessenger(MGGeneratorMuonsFromFile *fPtclGun);
  ~MGGeneratorMuonsFromFileMessenger();
  
  void SetNewValue(G4UIcommand *command, G4String newValues);
 
  
private:
  MGGeneratorMuonsFromFile *fParticleGun;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fileNameCmd;
  //G4UIcmdWithAnInteger* eventstoprocessCmd;
};

#endif
