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
// Messenger for neutron generator in the tunnel
//
// History:
// --------
// 28 Dec 2005   L.Pandola    First implementation 

#ifndef MGGeneratorSources4AInterfaceMessenger_H
#define MGGeneratorSources4AInterfaceMessenger_H 1

class MGGeneratorSources4AInterface;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorSources4AInterfaceMessenger : public G4UImessenger
{
public:

  MGGeneratorSources4AInterfaceMessenger(MGGeneratorSources4AInterface*);
  ~MGGeneratorSources4AInterfaceMessenger();
  void SetNewValue(G4UIcommand*,G4String);

private:
  MGGeneratorSources4AInterface* fPointerToInterface;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fCommand;
};

#endif

