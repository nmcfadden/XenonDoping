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
// Messenger for the cosmic ray muon geneator 
//
// History:
// --------
// 28 Oct 2004   L.Pandola    First implementation (not working)
// 09 Dec 2004   L.Pandola    Working version
// 22 Dec 2004   L.Pandola    Changed defaults
// 08 Feb 2005   L.Pandola    Added commands
// 16 May 2011   A. Schubert  Commented out SetRange comments, "*" character
//                            gave an error
// 07 Dec 2011   A. Schubert  Added depth unit candidates 

#include "generators/MGGeneratorCosmicSurfMuonsMessenger.hh"
#include "generators/MGGeneratorCosmicSurfMuons.hh"
#include "io/MGLogger.hh"

#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>               
#include "G4Tokenizer.hh"

MGGeneratorCosmicSurfMuonsMessenger::MGGeneratorCosmicSurfMuonsMessenger
(MGGeneratorCosmicSurfMuons *fPtclGun) : fParticleGun(fPtclGun) 
{
  fDirectory = new G4UIdirectory("/MG/generator/cosmicray/");
  fDirectory->SetGuidance("Control of cosmic rays event generator");

  // height of source
  heightCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/height",this);
  heightCmd->SetGuidance("Set the z position of the muon shower");
  heightCmd->SetGuidance("Default value: 8.1 m");
  heightCmd->SetParameterName("Height",true,true);
  heightCmd->SetDefaultUnit("cm");
  heightCmd->SetUnitCandidates("mm cm m km");

  // radius of source
  radiusCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/radius",this);
  radiusCmd->SetGuidance("Set the radius of the muon shower");
  radiusCmd->SetGuidance("Default value: 8.0 m");
  radiusCmd->SetParameterName("Radius",true,true);
  radiusCmd->SetDefaultUnit("cm");
  radiusCmd->SetUnitCandidates("mm cm m km");

 }


MGGeneratorCosmicSurfMuonsMessenger::~MGGeneratorCosmicSurfMuonsMessenger() 
{
  delete heightCmd;
  delete radiusCmd;
}

void MGGeneratorCosmicSurfMuonsMessenger::SetNewValue(G4UIcommand *command, 
    G4String newValues) 
{
  if (command == heightCmd)
  {
    fParticleGun->SetHalfZ(heightCmd->GetNewDoubleValue(newValues));
  }
  else if (command == radiusCmd) 
  {
    fParticleGun->SetRadius(radiusCmd->GetNewDoubleValue(newValues));
  }
}




