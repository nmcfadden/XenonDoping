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
// $Id: MGGeneratorPNNLMessenger.cc,v 1.3 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorPNNLMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Fri Jul  9 10:41:06 PDT 2004
 * 
 * REVISION:
 *
 * 07-09-2004, Created and included D. Jordan's code, R. Henning
 *
 * added a few lines to allow (or not) betas to be generated 
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"

#include "generators/MGGeneratorPNNL.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorPNNLMessenger.hh"  

//---------------------------------------------------------------------------//

#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

MGGeneratorPNNLMessenger::MGGeneratorPNNLMessenger(MGGeneratorPNNL *generator):
  fPNNLGenerator(generator)
{
  // /MG/generator/PNNL/
  fPNNLDirectory = new G4UIdirectory("/MG/generator/PNNL/");
  fPNNLDirectory->SetGuidance(
    "PNNL radioactive decay chain event generator control.");

  // /MG/generator/PNNL/init
  fPNNLInitializeCmd = 
    new G4UIcmdWithAString("/MG/generator/PNNL/init",this);
  fPNNLInitializeCmd->SetGuidance(
    "Initialize PNNL radioactive source generator with decay chain filename");
  fPNNLInitializeCmd->SetParameterName("DecayChainFile",true);
  fPNNLInitializeCmd->SetDefaultValue("DecayChain.dat");

  // /MG/generator/PNNL/reportingfrequency
  fPNNLReportingFrequencyCmd = new
    G4UIcmdWithAnInteger("/MG/generator/PNNL/reportingfrequency",this);
  fPNNLReportingFrequencyCmd->SetGuidance(
    "Set number of events to wait before reporting generator status.");

  // /MG/generator/PNNL/generatebeta
  fPNNLGenerateBetaCmd =
    new G4UIcmdWithABool("/MG/generator/PNNL/generatebeta",this);
  fPNNLGenerateBetaCmd->SetGuidance(
				    "Set the generator to produce (true) or not (false) betas");
  fPNNLGenerateBetaCmd->SetParameterName("GenerateBeta",true);
  fPNNLGenerateBetaCmd->SetDefaultValue(false);
 
  // /MG/generator/PNNL/setsourceage
  fPNNLSourceAgeCmd = 
    new G4UIcmdWithADouble("/MG/generator/PNNL/setsourceage",this);
  fPNNLSourceAgeCmd->SetGuidance(
    "Set the age of the radioactive source in years");
  fPNNLSourceAgeCmd->SetParameterName("SourceAge",true);
  fPNNLSourceAgeCmd->SetDefaultValue(0.0);
  fPNNLSourceAgeCmd->SetRange("SourceAge>=0.0");

  // /MG/generator/PNNL/position
  fPNNLPositionCmd=new G4UIcmdWith3Vector("/MG/generator/PNNL/position", this);
  fPNNLPositionCmd->SetGuidance(
     "Set Position in mother coordinates of pointsource.");
  fPNNLPositionCmd->SetDefaultValue(G4ThreeVector(0.0, 0.0, 0.0));
}

//---------------------------------------------------------------------------//

MGGeneratorPNNLMessenger::MGGeneratorPNNLMessenger(const MGGeneratorPNNLMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorPNNLMessenger::~MGGeneratorPNNLMessenger()
{
  delete fPNNLInitializeCmd; 
  delete fPNNLReportingFrequencyCmd;
  delete fPNNLGenerateBetaCmd;
  delete fPNNLSourceAgeCmd;
  delete fPNNLDirectory;
}

//---------------------------------------------------------------------------//

void MGGeneratorPNNLMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  if(cmd == fPNNLInitializeCmd) {
    fPNNLGenerator->SetPNNLDecayChainFile(newValue);
  } else if (cmd == fPNNLReportingFrequencyCmd) {
    G4int freq = fPNNLReportingFrequencyCmd->GetNewIntValue(newValue);
    fPNNLGenerator->SetReportingFrequency(freq);
  } else if (cmd == fPNNLGenerateBetaCmd) {
    G4bool generate_beta = fPNNLGenerateBetaCmd->GetNewBoolValue(newValue);
    fPNNLGenerator->SetPNNLGenerateBeta(generate_beta);
  } else if (cmd == fPNNLSourceAgeCmd) {
    G4double source_age = fPNNLSourceAgeCmd->GetNewDoubleValue(newValue); 
    fPNNLGenerator->SetPNNLSourceAge(source_age);
  } else if (cmd == fPNNLPositionCmd) {
    fPNNLGenerator->SetPosition(
      fPNNLPositionCmd->GetNew3VectorValue(newValue) * cm);
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
