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
// $Id: MGGeneratorTUNLFELMessenger.cc,v 1.2 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorTUNLFELMessenger.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Aug  4 11:23:24 PDT 2004
 * 
 * REVISION:
 *
 * 08-04-2004, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorTUNLFEL.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorTUNLFELMessenger.hh"

//---------------------------------------------------------------------------//

#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

MGGeneratorTUNLFELMessenger::MGGeneratorTUNLFELMessenger(MGGeneratorTUNLFEL *generator) : fTUNLFELGenerator(generator)
{
  // /MG/generator/TUNLFEL
  fTUNLFELDirectory = new G4UIdirectory("/MG/generator/TUNLFEL/");
  fTUNLFELDirectory->SetGuidance("TUNL FEL beam generator control.");

  // MG/generator/TUNLFEL/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/generator/TUNLFEL/dump", this);
  fDumpCmd->SetGuidance("Dump parameters of generator to screen");

  // /MG/generator/TUNLFEL/energysigma
  fEnergySigmaCmd = new G4UIcmdWithADoubleAndUnit(
		    "/MG/generator/TUNLFEL/energysigma", this);
  fEnergySigmaCmd->SetGuidance("Mean energy for FEL beam");
  fEnergySigmaCmd->SetDefaultUnit("MeV");
  fEnergySigmaCmd->SetUnitCandidates("ev keV Mev GeV TeV");

  // /MG/generator/TUNLFEL/majorsigma
  fMajorSigmaCmd = new G4UIcmdWithADoubleAndUnit(
		   "/MG/generator/TUNLFEL/majorsigma", this);
  fMajorSigmaCmd->SetGuidance("Sigma of beam width along major axis.");
  fMajorSigmaCmd->SetDefaultUnit("cm");
  fMajorSigmaCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/TUNLFEL/meanenergy
  fMeanEnergyCmd = new G4UIcmdWithADoubleAndUnit(
		   "/MG/generator/TUNLFEL/meanenergy", this);
  fMeanEnergyCmd->SetGuidance("Mean energy of beam.");
  fMeanEnergyCmd->SetDefaultUnit("MeV");
  fMeanEnergyCmd->SetUnitCandidates("ev keV MeV GeV TeV");

  // /MG/generator/TUNLFEL/minorsigma
  fMinorSigmaCmd = new G4UIcmdWithADoubleAndUnit(
		   "/MG/generator/TUNLFEL/minorsigma", this);
  fMinorSigmaCmd->SetGuidance("Sigma of beam width along minor axis.");
  fMinorSigmaCmd->SetDefaultUnit("cm");
  fMinorSigmaCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/TUNLFEL/origin
  fOriginCmd = new  G4UIcmdWith3VectorAndUnit(
		   "/MG/generator/TUNLFEL/origin", this);
  fOriginCmd->SetGuidance("Origin in mother coordinate system of beam.");
  fOriginCmd->SetDefaultUnit("cm");
  fOriginCmd->SetUnitCandidates("mm cm m");

  // /MG/generator/TUNLFEL/rho
  fRhoCmd = new G4UIcmdWithADouble("/MG/generator/TUNLFEL/rho", this);
  fRhoCmd->SetGuidance(
      "Major axis angle of beam profile relative to x-axis in degrees");
}

//---------------------------------------------------------------------------//

MGGeneratorTUNLFELMessenger::MGGeneratorTUNLFELMessenger(const MGGeneratorTUNLFELMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorTUNLFELMessenger::~MGGeneratorTUNLFELMessenger()
{
  delete fRhoCmd;
  delete fOriginCmd;
  delete fMinorSigmaCmd;
  delete fMeanEnergyCmd;
  delete fMajorSigmaCmd;
  delete fEnergySigmaCmd;
  delete fDumpCmd;
  delete fTUNLFELDirectory;
}

//---------------------------------------------------------------------------//

G4String MGGeneratorTUNLFELMessenger::GetCurrentValue(G4UIcommand *cmd)
{
  G4String cv = " ";
  if(cmd == fDumpCmd)
    fTUNLFELGenerator->Dump();

  return cv;
}

//---------------------------------------------------------------------------//

void MGGeneratorTUNLFELMessenger::SetNewValue(G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fEnergySigmaCmd)
    fTUNLFELGenerator->SetEnergySigma(
		       fEnergySigmaCmd->GetNewDoubleValue(newValues));
  else if(cmd == fMajorSigmaCmd) 
    fTUNLFELGenerator->SetMajorSigma(
		       fMajorSigmaCmd->GetNewDoubleValue(newValues));
  else if(cmd == fMeanEnergyCmd) 
    fTUNLFELGenerator->SetMeanEnergy(
		       fMeanEnergyCmd->GetNewDoubleValue(newValues));
  else if(cmd == fMinorSigmaCmd)
    fTUNLFELGenerator->SetMinorSigma(
                       fMinorSigmaCmd->GetNewDoubleValue(newValues));
  else if(cmd == fOriginCmd)
    fTUNLFELGenerator->SetOrigin(fOriginCmd->GetNew3VectorValue(newValues));
  else if(cmd == fRhoCmd)
    fTUNLFELGenerator->SetRho(fRhoCmd->GetNewDoubleValue(newValues) * deg);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
