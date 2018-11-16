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
// Implementation of the Decay0 primary generator
//
// History:
// --------
// 25 Oct 2004   L.Pandola    First implementation
// 04 Nov 2004   L.Pandola    Accomodated in the MJ structure
// 06 Dec 2004   L.Pandola    Renaming fest!
// 23 Dec 2005   L.Pandola    Further renaming to comply with MGGenerator
// 09 Jul 2010   L.Pandola    Add variables to generate/kill 
//                            particles selectively
// 14 Mar 2012   L.Pandola    Decide whether to recycle the file (when over)
//                            using a boolean flag.
//

#ifndef MGGeneratorDecay0Interface_h
#define MGGeneratorDecay0Interface_h 1

#include <fstream>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"

class G4PrimaryVertex;
class G4Event;
class MGGeneratorDecay0InterfaceMessenger;

class MGGeneratorDecay0Interface : public G4VPrimaryGenerator
{
public:
  MGGeneratorDecay0Interface(G4String fileName);
  ~MGGeneratorDecay0Interface();
  //Constructor, fileName is the name of the file with path

  void GeneratePrimaryVertex(G4Event* evt);
  void OpenFile();
  void ChangeFileName(G4String newFile);

  void KillElectrons(G4bool val){fKillElectrons = val;};
  void KillPositrons(G4bool val){fKillPositrons = val;};
  void KillGammas(G4bool val){fKillGammas = val;};
  
  void RecycleFile(G4bool val){fRecycleFile = val;};

private:
  G4String ConvertCodeToName(G4int code);
  G4String fFileName;
  std::ifstream fInputFile;
  MGGeneratorDecay0InterfaceMessenger* fTheMessenger;

  G4bool fKillElectrons;
  G4bool fKillPositrons;
  G4bool fKillGammas;

  G4bool fRecycleFile;

};
#endif
