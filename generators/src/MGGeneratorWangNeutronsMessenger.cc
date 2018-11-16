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
/**                                                            
 * $Id: MGGeneratorWangNeutronsMessenger.cc,v 1.3 2007-02-21 09:31:33 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorWangNeutronsMessenger.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for Wang neutrons generator.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: K.Hudek
 * CONTACT: 
 * FIRST SUBMISSION: Fri Jun 17 14:41:25 PDT 2005
 * 
 * REVISION:
 * 
 * 10-17-2005, Added minneutronenergy messenger, R. Henning
 * 09-07-2005, First submission, R. Henning for K. Hudek.
 * 
 */
// --------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorWangNeutrons.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorWangNeutronsMessenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorWangNeutronsMessenger::MGGeneratorWangNeutronsMessenger(
				    MGGeneratorWangNeutrons *generator) 
: fWangNeutronsGenerator(generator)
{
  // /MG/generator/WangNeutrons
  fWangNeutronsDirectory = new G4UIdirectory("/MG/generator/wangneutrons/");
  fWangNeutronsDirectory->SetGuidance("Wang Neutrons beam generator control.");

  // MG/generator/WangNeutrons/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/generator/wangneutrons/dump", 
					 this);
  fDumpCmd->SetGuidance("Dump parameters of generator to screen");

  // /MG/generator/WangNeutrons/maxmuonenergy
  fMaxMuonEnergyCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/maxmuonenergy", this);
  fMaxMuonEnergyCmd->SetGuidance("Max incident muon energy for spallation neutrons.");
  fMaxMuonEnergyCmd->SetDefaultUnit("GeV");
  fMaxMuonEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV");
  
  // /MG/generator/WangNeutrons/minmuonenergy
  fMinMuonEnergyCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/minmuonenergy", this);
  fMinMuonEnergyCmd->SetGuidance("Min incident muon energy for spallation neutrons.");
  fMinMuonEnergyCmd->SetDefaultUnit("GeV");
  fMinMuonEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV");
  fMinMuonBool = false;
  
  // /MG/generator/WangNeutrons/neutronenergycutoff
  fNeutronEnergyCutoffCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/neutronenergycutoff", this);
  fNeutronEnergyCutoffCmd->SetGuidance("Upper cutoff neutron energy for calc's.");
  fNeutronEnergyCutoffCmd->SetDefaultUnit("GeV");
  fNeutronEnergyCutoffCmd->SetUnitCandidates("eV keV MeV GeV TeV");

  // /MG/generator/WangNeutrons/minneutronenergy
  fMinNeutronEnergyCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/minneutronenergy", this);
  fMinNeutronEnergyCmd->SetGuidance("Lower cutoff neutron energy for calc's.");
  fMinNeutronEnergyCmd->SetDefaultUnit("GeV");
  fMinNeutronEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV");

  // /MG/generator/WangNeutrons/origin
  fOriginCmd = new  G4UIcmdWith3VectorAndUnit(
		      "/MG/generator/wangneutrons/origin", this);
  fOriginCmd->SetGuidance("Origin in mother coordinate system of beam.");
  fOriginCmd->SetDefaultUnit("cm");
  fOriginCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/WangNeutrons/sections
  fSamplePointsCmd = new  G4UIcmdWithAnInteger(
		      "/MG/generator/wangneutrons/samplepoints", this);
  fSamplePointsCmd->SetGuidance("Number of sample points for transformation.");

  // /MG/generator/WangNeutrons/depth
  fDepthCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/depth", this);
  fDepthCmd->SetGuidance("Depth in m.w.e. of the detector.");
  fDepthCmd->SetDefaultUnit("m");
  fDepthCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/WangNeutrons/cavex
  fCaveXCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/cavex", this);
  fCaveXCmd->SetGuidance("X dimention of the cave.");
  fCaveXCmd->SetDefaultUnit("m");
  fCaveXCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/WangNeutrons/cavey
  fCaveYCmd = new  G4UIcmdWithADoubleAndUnit(
		      "/MG/generator/wangneutrons/cavey", this);
  fCaveYCmd->SetGuidance("Y dimention of the cave.");
  fCaveYCmd->SetDefaultUnit("m");
  fCaveYCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/WangNeutrons/histofile
  fHistoFileCmd = new  G4UIcmdWithAString(
		      "/MG/generator/wangneutrons/histofile", this);
  fHistoFileCmd->SetGuidance("Path of histogram file to be saved.");
}

//---------------------------------------------------------------------------//

MGGeneratorWangNeutronsMessenger::MGGeneratorWangNeutronsMessenger(
				    const MGGeneratorWangNeutronsMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorWangNeutronsMessenger::~MGGeneratorWangNeutronsMessenger()
{
  delete fDumpCmd;
  delete fMaxMuonEnergyCmd;
  delete fMinMuonEnergyCmd;
  delete fMinNeutronEnergyCmd;
  delete fNeutronEnergyCutoffCmd;
  delete fDepthCmd;
  delete fOriginCmd;
  delete fCaveXCmd;
  delete fCaveYCmd;
  delete fHistoFileCmd;
  delete fWangNeutronsDirectory;
}

//---------------------------------------------------------------------------//

G4String MGGeneratorWangNeutronsMessenger::GetCurrentValue(G4UIcommand *cmd)
{
  G4String cv = " ";
  if(cmd == fDumpCmd)
    fWangNeutronsGenerator->Dump();

  return cv;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutronsMessenger::SetNewValue(
					  G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fMaxMuonEnergyCmd)
    fWangNeutronsGenerator->SetMaxMuonEnergy(
			      fMaxMuonEnergyCmd->GetNewDoubleValue(newValues));
  else if(cmd == fMinMuonEnergyCmd) {
    fWangNeutronsGenerator->SetMinMuonEnergy(
			      fMinMuonEnergyCmd->GetNewDoubleValue(newValues));
    fMinMuonBool = true;
  }   
  else if(cmd == fMinNeutronEnergyCmd)
    fWangNeutronsGenerator->SetMinNeutronEnergy(
			    fMinNeutronEnergyCmd->GetNewDoubleValue(newValues));
  else if(cmd == fNeutronEnergyCutoffCmd)
    fWangNeutronsGenerator->SetNeutronEnergyCutoff(
			    fNeutronEnergyCutoffCmd->GetNewDoubleValue(newValues));
  else if(cmd == fOriginCmd)
    fWangNeutronsGenerator->SetOrigin(
			      fOriginCmd->GetNew3VectorValue(newValues));
  else if(cmd == fSamplePointsCmd)
    fWangNeutronsGenerator->SetSamplePoints(
			      fSamplePointsCmd->GetNewIntValue(newValues));
  else if(cmd == fDepthCmd)
    fWangNeutronsGenerator->SetDepth(
			      fDepthCmd->GetNewDoubleValue(newValues));
  else if(cmd == fCaveXCmd)
    fWangNeutronsGenerator->SetCaveX(
			      fCaveXCmd->GetNewDoubleValue(newValues));
  else if(cmd == fCaveYCmd)
    fWangNeutronsGenerator->SetCaveY(
			      fCaveYCmd->GetNewDoubleValue(newValues));
  else if(cmd == fHistoFileCmd)
    fWangNeutronsGenerator->SetHistoFile(newValues);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
