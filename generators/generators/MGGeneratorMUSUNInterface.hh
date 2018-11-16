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
// Implementation of the MUSUN primary generator for cosmic ray muons
//
// History:
// --------
// 27 Apr 2005   L.Pandola    First implementation
// 24 Jul 2008   L.Pandola    Changed format according to the new MUSUN output to 
//                            be used for the Gerda MCC2. 

#ifndef MGGeneratorMUSUNInterface_h
#define MGGeneratorMUSUNInterface_h 1

#include <fstream>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"

class G4PrimaryVertex;
class G4Event;
class MGGeneratorMUSUNInterfaceMessenger;

class MGGeneratorMUSUNInterface : public G4VPrimaryGenerator
{
public:
  MGGeneratorMUSUNInterface(G4String);
  ~MGGeneratorMUSUNInterface();
  //Constructor, fileName is the name of the file with path

  void GeneratePrimaryVertex(G4Event*);
  void OpenFile();
  void ChangeFileName(G4String);
  void SetZShift(G4double shift){fZShift = shift;};

private:
  G4double fZShift;
  G4String fFileName;
  std::ifstream fInputFile;
  MGGeneratorMUSUNInterfaceMessenger* fTheMessenger;
};
#endif
