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

#include "generators/MGGeneratorCosmicRayMuonsMessenger.hh"
#include "generators/MGGeneratorCosmicRayMuons.hh"
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

MGGeneratorCosmicRayMuonsMessenger::MGGeneratorCosmicRayMuonsMessenger
(MGGeneratorCosmicRayMuons *fPtclGun) : fParticleGun(fPtclGun) 
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

  // depth of the Laboratory
  depthCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/depth",this);
  depthCmd->SetGuidance("Set the depth of the Laboratory");
  depthCmd->SetGuidance("expressed in water equivalent");
  depthCmd->SetGuidance("Default value: 3700 m (Gran Sasso depth)");
  depthCmd->SetGuidance("Depth>=1*km && Depth <=10*km");
  depthCmd->SetParameterName("Depth",true,true);
  depthCmd->SetDefaultUnit("cm");
  depthCmd->SetUnitCandidates("mm cm m km");
  //depthCmd->SetRange("Depth>=1*km && Depth <=10*km");

  //index of the muon power spectrum
  indexCmd = new 
    G4UIcmdWithADouble("/MG/generator/cosmicray/index",this);
  indexCmd->SetGuidance("Set the spectral index of muons");
  indexCmd->SetGuidance("2.0 --> Exotic sources ");
  indexCmd->SetGuidance("2.7 --> Prompt sources (e.g. charm decay)");
  indexCmd->SetGuidance("3.7 --> Standard spectrum ");
  indexCmd->SetGuidance("Default value: 3.7");
  indexCmd->SetParameterName("Index",true,true);
  indexCmd->SetRange("Index == 2.0 || Index == 2.7 || Index == 3.7"); 
  
  // lower edge of the energy spectrum
  energyLowCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/energyLow",this);
  energyLowCmd->SetGuidance("Set the lower edge of the energy spectrum");
  energyLowCmd->SetGuidance("(minimum 100 MeV, maximum 10 GeV)");
  energyLowCmd->SetGuidance("Default value: 1 GeV");
  energyLowCmd->SetGuidance("energyLow>=100*MeV && energyLow <=10*GeV");
  energyLowCmd->SetParameterName("energyLow",true,true);
  energyLowCmd->SetDefaultUnit("GeV");
  energyLowCmd->SetUnitCandidates("MeV TeV");
  //energyLowCmd->SetRange("energyLow>=100*MeV && energyLow <=10*GeV");

  // upper edge of the energy spectrum
  energyLowCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/generator/cosmicray/energyUp",this);
  energyLowCmd->SetGuidance("Set the upper edge of the energy spectrum");
  energyLowCmd->SetGuidance("(minimum 100 GeV, maximum 50 TeV)");
  energyLowCmd->SetGuidance("Default value: 10 TeV");
  energyLowCmd->SetGuidance("energyLow>=100*GeV && energyLow <=50*TeV");
  energyLowCmd->SetParameterName("energyLow",true,true);
  energyLowCmd->SetDefaultUnit("GeV");
  energyLowCmd->SetUnitCandidates("MeV TeV");
  //energyLowCmd->SetRange("energyLow>=100*GeV && energyLow <=50*TeV");

  // name of the file containing the angular spectrum
  fileNameCmd = new 
    G4UIcmdWithAString("/MG/generator/cosmicray/filename",this);
  fileNameCmd->SetGuidance("Name of the file containing the angular spectrum");
  fileNameCmd->SetGuidance("Format: costheta pdf");
  fileNameCmd->SetGuidance("(evenly-spaced in costheta)");
  fileNameCmd->SetGuidance("Default: zenithmacro.dat");
}


MGGeneratorCosmicRayMuonsMessenger::~MGGeneratorCosmicRayMuonsMessenger() 
{
  delete fDirectory;
  delete heightCmd;
  delete radiusCmd;
  delete depthCmd;
  delete indexCmd;
  delete energyLowCmd;
  delete energyUpCmd;
  delete fileNameCmd;
}

void MGGeneratorCosmicRayMuonsMessenger::SetNewValue(G4UIcommand *command, 
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
  else if (command == depthCmd) 
    {
      fParticleGun->SetRockDepth(depthCmd->GetNewDoubleValue(newValues));
    }
  else if (command == indexCmd)
    {
      fParticleGun->SetSpectralIndex(indexCmd->GetNewDoubleValue(newValues));
    }
   else if (command == energyLowCmd)
    {
      fParticleGun->SetEnergyInf(energyLowCmd->GetNewDoubleValue(newValues));
    }
  else if (command == energyUpCmd)
    {
      fParticleGun->SetEnergySup(energyUpCmd->GetNewDoubleValue(newValues));
    }
  else if (command == fileNameCmd)
    {
      fParticleGun->SetFileName(newValues);
      MGLog(trace) << "File of angular spectrum changed to " << 
	fParticleGun->GetFileName() << endlog;
      MGLog(trace) << "Be sure that the format is correct!" << endlog;
    }
}




