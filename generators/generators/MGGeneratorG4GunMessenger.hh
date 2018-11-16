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
// Messenger for MGG4 generator
//
// History:
// --------
// 13 Sep 2005 X. Liu       First implementation
// 25 Jan 2006 L. Pandola   Added commands
// 11 Apr 2006 L. Pandola   Removed useless command
// 11 Nov 2006 L. Pandola   Added commands 
// 29 May 2008 L. Pandola   Added commands to sample energy from a file spectrum

#ifndef MGGeneratorG4GunMessenger_H
#define MGGeneratorG4GunMessenger_H 1

class MGGeneratorG4Gun;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithAString;
class G4UIcommand;
class G4ParticleGun;

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorG4GunMessenger : public G4UImessenger
{
public:

  // default constructor
  MGGeneratorG4GunMessenger(MGGeneratorG4Gun*);

  // copy constructor
  MGGeneratorG4GunMessenger(const MGGeneratorG4GunMessenger &);

  // destructor
  ~MGGeneratorG4GunMessenger();

  // public interface
  void SetNewValue(G4UIcommand* cmd,G4String str);

private:
  MGGeneratorG4Gun* fPointerToMGG4Gun;
  G4UIdirectory* fDirectory;
  G4UIcmdWithABool* fTurnOnCommand;
  G4UIcmdWith3VectorAndUnit* fCenterPositionCmd;
  G4UIcmdWithADoubleAndUnit* fCenterPositionSmearCmd;
  G4UIcmdWithABool* fTurnOnCentricConeCmd;
  G4UIcmdWithADoubleAndUnit* fOpeningAngleCmd;

  G4UIcmdWithABool* fTurnConeCommand;
  G4UIcmdWithADoubleAndUnit* fDeltaThetaCmd;
  G4UIcmdWith3Vector* fConeAxisDirectionCmd;

  G4UIcmdWithABool* fReadFromFileCmd;
  G4UIcmdWithAString* fSetFileNameCmd;
};

#endif

