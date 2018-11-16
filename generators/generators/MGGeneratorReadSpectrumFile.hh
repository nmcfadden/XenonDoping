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
// 29 May 2008   L.Pandola    First implementation


#ifndef MGGeneratorReadSpectrumFile_h
#define MGGeneratorReadSpectrumFile_h 1

#include "globals.hh"
#include <vector>

class MGGeneratorReadSpectrumFile 
{
public:
  MGGeneratorReadSpectrumFile();
  ~MGGeneratorReadSpectrumFile();
  //Constructor, fileName is the name of the file with path
  void ChangeFileName(G4String newFile);
  G4double ShootEnergy();

private:
  void InitializeSpectrumFromFile();
  std::vector<G4double> fVectorSpectrumCumulative;
  std::vector<G4double> fVectorEnergy;
  G4bool fileInitialized;
  G4String fSourceFile;

};
#endif

