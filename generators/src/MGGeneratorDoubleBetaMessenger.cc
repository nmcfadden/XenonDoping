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
// $Id: MGGeneratorDoubleBetaMessenger.cc,v 1.2 2007-02-21 09:31:33 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorDoubleBetaMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  This generator is an implementation of BB-decay event generation via the
 *  theoretical phase space factors and related single electron spectral
 *  calculations of J. Kotila and F. Iachello. The datafiles it uses are
 *  provided directly by Kotila & Iachello and can be downloaded here:
 *  http://nucleartheory.yale.edu/double-beta-decay-phase-space-factors
 *  Their work is published in the article "Phase-space Factors for Double-B
 *  Decay": J. Kotila and F. Iachello, Physical Review C 85, 034316 (2012) and 
 *  nucleartheory.yale.edu
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2016
 *
 * REVISION:
 *
 * 03-08-2016, Created, R. Massarczyk
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorDoubleBeta.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorDoubleBetaMessenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorDoubleBetaMessenger::MGGeneratorDoubleBetaMessenger(MGGeneratorDoubleBeta *generator) : fDoubleBetaGenerator(generator)
{
  // /MG/generator/DoubleBeta
  fDoubleBetaDirectory = new G4UIdirectory("/MG/generator/DoubleBeta/");
  fDoubleBetaDirectory->SetGuidance("control double beta.");


  // /MG/generator/DoubleBeta/rho
  fNeutrinoCmd = new G4UIcmdWithAnInteger("/MG/generator/DoubleBeta/neutrinos", this);
  fNeutrinoCmd->SetGuidance("Number of neutrinos in a decay (0 or 2)");
  fNeutrinoCmd->SetParameterName("number",false);
  fNeutrinoCmd->SetRange("number == 0 || number == 2");
}

//---------------------------------------------------------------------------//

MGGeneratorDoubleBetaMessenger::MGGeneratorDoubleBetaMessenger(const MGGeneratorDoubleBetaMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorDoubleBetaMessenger::~MGGeneratorDoubleBetaMessenger()
{
  delete fNeutrinoCmd;
  delete fDoubleBetaDirectory;
}



//---------------------------------------------------------------------------//

void MGGeneratorDoubleBetaMessenger::SetNewValue(G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fNeutrinoCmd)
    fDoubleBetaGenerator->SetNeutrino(fNeutrinoCmd->GetNewIntValue(newValues));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
