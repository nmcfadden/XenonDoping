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
// History:
// --------
// 28 Dec 2005   L.Pandola    First implementation


#ifndef MGGeneratorSources4AInterface_h
#define MGGeneratorSources4AInterface_h 1

#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include <vector>
#include <fstream>
#include "Randomize.hh"
#include "G4ThreeVector.hh"

class G4PrimaryVertex;
class G4Event;
class MGGeneratorSources4AInterfaceMessenger;

class MGGeneratorSources4AInterface : public G4VPrimaryGenerator
{
public:
  MGGeneratorSources4AInterface();
  ~MGGeneratorSources4AInterface();
  //Constructor, fileName is the name of the file with path
  void GeneratePrimaryVertex(G4Event* evt);
  void ChangeFileName(G4String newFile);

private:
  void InitializeSources4A();
  std::vector<G4double> *SourcesSpectrum;
  std::vector<G4double> *SourcesEnergy;
  G4bool SourcesInitialized;
  G4String sourceFile;
  G4double ShootSources4A();

  MGGeneratorSources4AInterfaceMessenger* fTheMessenger;

};
#endif

