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
// $Id: MGGeneratorBACoNSurfaceMuonsMessenger.cc,v 1.0 2009/5/17
//      
// CLASS IMPLEMENTATION:  MGGeneratorBACoNSurfaceMuonsMessenger.cc
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

#include "generators/MGGeneratorBACoNSurfaceMuons.hh"
#include "generators/MGGeneratorBACoNSurfaceMuonsMessenger.hh"

//
//---------------------------------------------------------------------------//

MGGeneratorBACoNSurfaceMuonsMessenger::MGGeneratorBACoNSurfaceMuonsMessenger(MGGeneratorBACoNSurfaceMuons *generator) : fLiquidMuonGenerator(generator)
{
	// /MG/generator/LiquidMuon
	fLiquidMuonDirectory = new G4UIdirectory("/MG/generator/LGNDLiquidMuon/");
	fLiquidMuonDirectory->SetGuidance("Set to generate optical photons @128 nm in argon inside cryostat");

  fLiquidMuonSetRadius= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidMuon/SetRadiusMax",this);
  fLiquidMuonSetRadius->SetGuidance("Define Max Radius to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidMuonSetRadius->SetDefaultUnit("cm");
  fLiquidMuonSetRadius->SetUnitCategory("Length");
  fLiquidMuonSetRadius->SetUnitCandidates("micron mm cm m km");

  fLiquidMuonSetRadiusMin= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidMuon/SetRadiusMin",this);
  fLiquidMuonSetRadiusMin->SetGuidance("Define Min Radius to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidMuonSetRadiusMin->SetDefaultUnit("cm");
  fLiquidMuonSetRadiusMin->SetUnitCategory("Length");
  fLiquidMuonSetRadiusMin->SetUnitCandidates("micron mm cm m km");


  fLiquidMuonSetHeight= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidMuon/SetHeight",this);
  fLiquidMuonSetHeight->SetGuidance("Define Max Height to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidMuonSetHeight->SetDefaultUnit("cm");
  fLiquidMuonSetHeight->SetUnitCategory("Length");
  fLiquidMuonSetHeight->SetUnitCandidates("micron mm cm m km");

  fLiquidMuonSetBinWidth= new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidMuon/SetBinWidth",this);
  fLiquidMuonSetBinWidth->SetGuidance("Define voxel size for optical maps");
  fLiquidMuonSetBinWidth->SetDefaultUnit("cm");
  fLiquidMuonSetBinWidth->SetUnitCategory("Length");
  fLiquidMuonSetBinWidth->SetUnitCandidates("micron mm cm m km");

  fLiquidMuonSetNParticles = new G4UIcmdWithADouble("/MG/generator/LGNDLiquidMuon/SetNParticles",this);
  fLiquidMuonSetNParticles->SetGuidance("Set number of particles to be generated per event");


  //example
  // /MG/generator/LGNDLiquidMuon/SetCenterVector 0.0 0.0 100.0 cm
  fLiquidMuonSetCenterVector= new G4UIcmdWith3VectorAndUnit("/MG/generator/LGNDLiquidMuon/SetCenterVector",this);
  fLiquidMuonSetCenterVector->SetGuidance("Set Center of generator volume inside a Cylidrical Croystat that has diameter = height");
  fLiquidMuonSetCenterVector->SetGuidance("Default value is (0,0,0) but arrays are not centered on (0,0,0)");
  fLiquidMuonSetCenterVector->SetDefaultUnit("cm");
  fLiquidMuonSetCenterVector->SetUnitCategory("Length");
  fLiquidMuonSetCenterVector->SetUnitCandidates("micron mm cm m km");

  fLiquidMuonSetParticle = new G4UIcmdWithAString("/MG/generator/LGNDLiquidMuon/SetParticle",this);
  fLiquidMuonSetParticle->SetGuidance("Define particle type that will be generated in LAr");

  fLiquidMuonSetEnergy = new G4UIcmdWithADoubleAndUnit("/MG/generator/LGNDLiquidMuon/SetEnergy",this);
  fLiquidMuonSetEnergy->SetGuidance("Define energy to Generate points inside a Cylidrical Croystat that has diameter = height");
  fLiquidMuonSetEnergy->SetDefaultUnit("MeV");
  fLiquidMuonSetEnergy->SetUnitCategory("Energy");
  fLiquidMuonSetEnergy->SetUnitCandidates("eV KeV MeV GeV TeV");
}

//---------------------------------------------------------------------------//

MGGeneratorBACoNSurfaceMuonsMessenger::MGGeneratorBACoNSurfaceMuonsMessenger(const MGGeneratorBACoNSurfaceMuonsMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorBACoNSurfaceMuonsMessenger::~MGGeneratorBACoNSurfaceMuonsMessenger()
{
	delete fLiquidMuonDirectory;
  delete fLiquidMuonSetRadius;
  delete fLiquidMuonSetRadiusMin;
  delete fLiquidMuonSetHeight;
  delete fLiquidMuonSetParticle;
  delete fLiquidMuonSetEnergy;
  delete fLiquidMuonSetCenterVector;
  delete fLiquidMuonSetBinWidth;
  delete fLiquidMuonSetNParticles;
}

//---------------------------------------------------------------------------//

void MGGeneratorBACoNSurfaceMuonsMessenger::SetNewValue(G4UIcommand *cmd, G4String str)
{
  if(cmd == fLiquidMuonSetRadius){
    fLiquidMuonGenerator->SetRadius(fLiquidMuonSetRadius->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidMuonSetRadiusMin){
    fLiquidMuonGenerator->SetRadiusMin(fLiquidMuonSetRadiusMin->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidMuonSetHeight){
    fLiquidMuonGenerator->SetHeight(fLiquidMuonSetHeight->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidMuonSetBinWidth){
    fLiquidMuonGenerator->SetBinWidth(fLiquidMuonSetBinWidth->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidMuonSetNParticles){
    fLiquidMuonGenerator->SetNParticles(fLiquidMuonSetNParticles->GetNewDoubleValue(str));
  }
  else if(cmd == fLiquidMuonSetCenterVector){
    fLiquidMuonGenerator->SetCenterVector(fLiquidMuonSetCenterVector->GetNew3VectorValue(str));
  }
  else if(cmd == fLiquidMuonSetParticle){
    fLiquidMuonGenerator->SetParticleType(str);
  }
  else if(cmd == fLiquidMuonSetEnergy){
    fLiquidMuonGenerator->SetParticleEnergy(fLiquidMuonSetEnergy->GetNewDoubleValue(str));
  }
}
