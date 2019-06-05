//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id: MGGeneratorLGNDLiquidArgonMessenger.cc,v 1.0 2009/5/17
//      
// CLASS IMPLEMENTATION:  MGGeneratorLGNDLiquidArgonMessenger.cc
//
//---------------------------------------------------------------------------//
/**
* SPECIAL NOTES:
* 
*
*/
// 
//---------------------------------------------------------------------------//
/**
* AUTHOR: Neil McFadden
* CONTACT: nmcfadde@unm.edu
* FIRST SUBMISSION:
* 
* REVISION:
*
* 
*/
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"

#include "generators/MGGeneratorLGNDLiquidArgon.hh"
#include "generators/MGGeneratorLGNDLiquidArgonMessenger.hh"

//
//---------------------------------------------------------------------------//

MGGeneratorLGNDLiquidArgonMessenger::MGGeneratorLGNDLiquidArgonMessenger(MGGeneratorLGNDLiquidArgon *generator) : fLiquidArgonGenerator(generator)
{
	// /MG/generator/LiquidArgon
	fLiquidArgonDirectory = new G4UIdirectory("/MG/generator/LGNDLiquidArgon/");
	fLiquidArgonDirectory->SetGuidance("Set to generate optical photons @128 nm in argon inside cryostat");

  fLiquidArgonSetRadius= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidArgon/SetRadiusMax",this);
  fLiquidArgonSetRadius->SetGuidance("Define Max Radius to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidArgonSetRadius->SetDefaultUnit("cm");
  fLiquidArgonSetRadius->SetUnitCategory("Length");
  fLiquidArgonSetRadius->SetUnitCandidates("micron mm cm m km");

  fLiquidArgonSetRadiusMin= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidArgon/SetRadiusMin",this);
  fLiquidArgonSetRadiusMin->SetGuidance("Define Min Radius to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidArgonSetRadiusMin->SetDefaultUnit("cm");
  fLiquidArgonSetRadiusMin->SetUnitCategory("Length");
  fLiquidArgonSetRadiusMin->SetUnitCandidates("micron mm cm m km");


  fLiquidArgonSetHeight= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidArgon/SetHeight",this);
  fLiquidArgonSetHeight->SetGuidance("Define Max Height to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidArgonSetHeight->SetDefaultUnit("cm");
  fLiquidArgonSetHeight->SetUnitCategory("Length");
  fLiquidArgonSetHeight->SetUnitCandidates("micron mm cm m km");

  fLiquidArgonSetBinWidth= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidArgon/SetBinWidth",this);
  fLiquidArgonSetBinWidth->SetGuidance("Define voxel size for optical maps");
  fLiquidArgonSetBinWidth->SetDefaultUnit("cm");
  fLiquidArgonSetBinWidth->SetUnitCategory("Length");
  fLiquidArgonSetBinWidth->SetUnitCandidates("micron mm cm m km");

  fLiquidArgonSetNParticles = new G4UIcmdWithADouble("/MG/generator/LGNDLiquidArgon/SetNParticles",this);
  fLiquidArgonSetNParticles->SetGuidance("Set number of particles to be generated per event");


  //example
  // /MG/generator/LGNDLiquidArgon/SetCenterVector 0.0 0.0 100.0 cm
  fLiquidArgonSetCenterVector= new G4UIcmdWith3VectorAndUnit("/MG/generator/LGNDLiquidArgon/SetCenterVector",this);
  fLiquidArgonSetCenterVector->SetGuidance("Set Center of generator volume inside a Cylidrical Croystat that has diameter = height");
  fLiquidArgonSetCenterVector->SetGuidance("Default value is (0,0,0) but arrays are not centered on (0,0,0)");
  fLiquidArgonSetCenterVector->SetDefaultUnit("cm");
  fLiquidArgonSetCenterVector->SetUnitCategory("Length");
  fLiquidArgonSetCenterVector->SetUnitCandidates("micron mm cm m km");

  fLiquidArgonSetParticle = new G4UIcmdWithAString("/MG/generator/LGNDLiquidArgon/SetParticle",this);
  fLiquidArgonSetParticle->SetGuidance("Define particle type that will be generated in LAr");

  fLiquidArgonSetEnergy = new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidArgon/SetEnergy",this);
  fLiquidArgonSetEnergy->SetGuidance("Define energy to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidArgonSetEnergy->SetDefaultUnit("MeV");
  fLiquidArgonSetEnergy->SetUnitCategory("Energy");
  fLiquidArgonSetEnergy->SetUnitCandidates("eV KeV MeV GeV TeV");
}

//---------------------------------------------------------------------------//

MGGeneratorLGNDLiquidArgonMessenger::MGGeneratorLGNDLiquidArgonMessenger(const MGGeneratorLGNDLiquidArgonMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorLGNDLiquidArgonMessenger::~MGGeneratorLGNDLiquidArgonMessenger()
{
	delete fLiquidArgonDirectory;
  delete fLiquidArgonSetRadius;
  delete fLiquidArgonSetRadiusMin;
  delete fLiquidArgonSetHeight;
  delete fLiquidArgonSetParticle;
  delete fLiquidArgonSetEnergy;
  delete fLiquidArgonSetCenterVector;
  delete fLiquidArgonSetBinWidth;
  delete fLiquidArgonSetNParticles;
}

//---------------------------------------------------------------------------//

void MGGeneratorLGNDLiquidArgonMessenger::SetNewValue(G4UIcommand *cmd, G4String str)
{
  if(cmd == fLiquidArgonSetRadius){
    fLiquidArgonGenerator->SetRadius(fLiquidArgonSetRadius->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidArgonSetRadiusMin){
    fLiquidArgonGenerator->SetRadiusMin(fLiquidArgonSetRadiusMin->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidArgonSetHeight){
    fLiquidArgonGenerator->SetHeight(fLiquidArgonSetHeight->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidArgonSetBinWidth){
    fLiquidArgonGenerator->SetBinWidth(fLiquidArgonSetBinWidth->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidArgonSetNParticles){
    fLiquidArgonGenerator->SetNParticles(fLiquidArgonSetNParticles->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidArgonSetCenterVector){
    fLiquidArgonGenerator->SetCenterVector(fLiquidArgonSetCenterVector->GetNew3VectorValue(str));
  }
  else if(cmd == fLiquidArgonSetParticle){
    fLiquidArgonGenerator->SetParticleType(str);
  }
  else if(cmd == fLiquidArgonSetEnergy){
    fLiquidArgonGenerator->SetParticleEnergy(fLiquidArgonSetEnergy->GetNewDoubleValue(str));
  }
}
