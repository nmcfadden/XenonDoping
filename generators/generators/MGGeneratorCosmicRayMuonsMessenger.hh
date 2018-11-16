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
// Messenger for cosmic ray muon generator 
// (with the correct spectrum) in the tunnel. It is work in progress
//
// History:
// --------
// 28 Oct 2004   L.Pandola    First implementation (not working)
// 09 Dec 2004   L.Pandola    First working version
// 08 Feb 2005   L.Pandola    Added commands 

#ifndef MGGeneratorCosmicRayMuonsMessenger_h
#define MGGeneratorCosmicRayMuonsMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MGGeneratorCosmicRayMuons;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class MGGeneratorCosmicRayMuonsMessenger: public G4UImessenger {
  
public:
  MGGeneratorCosmicRayMuonsMessenger(MGGeneratorCosmicRayMuons *fPtclGun);
  ~MGGeneratorCosmicRayMuonsMessenger();
  
  void SetNewValue(G4UIcommand *command, G4String newValues);
 
  
private:
  MGGeneratorCosmicRayMuons *fParticleGun;
  G4UIdirectory* fDirectory;
  G4UIcmdWithADoubleAndUnit* radiusCmd;
  G4UIcmdWithADoubleAndUnit* heightCmd;
  G4UIcmdWithADoubleAndUnit* depthCmd;
  G4UIcmdWithADoubleAndUnit* energyLowCmd;
  G4UIcmdWithADoubleAndUnit* energyUpCmd;
  G4UIcmdWithAString* fileNameCmd;
  G4UIcmdWithADouble* indexCmd;
};

#endif
