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
// Messenger for the neutron generator
//
// History:
// --------
// 25 Jan 2005   L.Pandola    First implementation 
// 26 Jan 2005   L.Pandola    Added command to switch fission/cosmogenic

#include "generators/MGGeneratorNeutronsAtGSMessenger.hh"
#include "generators/MGGeneratorNeutronsAtGS.hh"
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

MGGeneratorNeutronsAtGSMessenger::MGGeneratorNeutronsAtGSMessenger
(MGGeneratorNeutronsAtGS *fPtclGun) : fParticleGun(fPtclGun) 
{
  fDirectory = new G4UIdirectory("/MG/generator/neutrons/");
  fDirectory->SetGuidance("Control of neutron event generator");

  // height of source
  heightCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/neutrons/height",this);
  heightCmd->SetGuidance("Set the z position of the neutron shower");
  heightCmd->SetGuidance("Default value: 8.0 m");
  heightCmd->SetParameterName("Height",true,true);
  heightCmd->SetDefaultUnit("cm");
  heightCmd->SetUnitCandidates("mm cm m km");

  // radius of source
  radiusCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/neutrons/radius",this);
  radiusCmd->SetGuidance("Set the radius of the neutron shower");
  radiusCmd->SetGuidance("Default value: 8.0 m");
  radiusCmd->SetParameterName("Radius",true,true);
  radiusCmd->SetDefaultUnit("cm");
  radiusCmd->SetUnitCandidates("mm cm m km");

  //position of the source (roof, walls, floor)
  directionCmd = new  G4UIcmdWithAString("/MG/generator/neutrons/direction",this);
  directionCmd->SetGuidance("Set the position of the neutron shower");
  directionCmd->SetGuidance("(roof, walls, floor)");
  directionCmd->SetCandidates("roof walls floor");

  //fission or cosmogenic
  fissionCmd = new G4UIcmdWithABool("/MG/generator/neutrons/fission",this);
  fissionCmd->SetGuidance("true: fission and (alpha,n) neutrons generated");
  fissionCmd->SetGuidance("false: cosmogenic neutrons");
  fissionCmd->SetGuidance("Default: false");
}


MGGeneratorNeutronsAtGSMessenger::~MGGeneratorNeutronsAtGSMessenger() 
{
  delete fDirectory;
  delete heightCmd;
  delete radiusCmd;
  delete directionCmd;
  delete fissionCmd;
}

void MGGeneratorNeutronsAtGSMessenger::SetNewValue(G4UIcommand *command, 
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
  else if (command == directionCmd)
    {
      fParticleGun->SetMuonOrigin(newValues);
    }
  else if (command == fissionCmd)
    {
      fParticleGun->SetFissionFlag(fissionCmd->GetNewBoolValue(newValues));
    }
}




