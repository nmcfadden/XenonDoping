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
// $Id: MGGeneratorBaryonDecay.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorBaryonDecay.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * This generator should help us to understand several decay modes that were 
 * investigated for the baryon decay search
 * deltaB = 3
 * case 1 3n->73Ge pi0 neutrino
 * Double_t particle_mass[3] = {72.9235*amu, 0.135, 0.000000000120};
 * case 2 2np->73Ga pi0 e+ 
 * Double_t particle_mass[3] = {72.9251*amu, 0.135, 0.000000511};
 * case 3 2pn->73Zn pi+ e+ 
 * Double_t particle_mass[3] = {72.9298*amu, 0.1395, 0.000000511};
 * case 4 3p->73Cu pi+ pi+ e+ 
 * Double_t particle_mass[4] = {72.9367*amu, 0.1395, 0.1395, 0.000000511};
 *
 * deltaB = 2
 * case 5 nn->74Ge pi0 pi0
 * Double_t particle_mass[3] = {73.9211*amu, 0.135, 0.135};
 * case 6 nn->74Ge pi+ pi-
 * Double_t particle_mass[3] = {73.9211*amu, 0.1395, 0.1395};
 * case 7 np->74Ga pi0 pi+
 * Double_t particle_mass[3] = {73.9269*amu, 0.135, 0.1395};
 * case 8 pp->74Zn pi+ pi+
 * Double_t particle_mass[3] = {73.9294*amu, 0.1395, 0.1395};

*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2018
 *
 * REVISION:
 *
 * 02-15-2018, Created, R. Massarczyk
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

#include "generators/MGGeneratorBaryonDecay.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorBaryonDecayMessenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorBaryonDecayMessenger::MGGeneratorBaryonDecayMessenger(MGGeneratorBaryonDecay *generator) : fBaryonDecayGenerator(generator)
{
  // /MG/generator/BaryonDecay
  fBaryonDecayDirectory = new G4UIdirectory("/MG/generator/BaryonDecay/");
  fBaryonDecayDirectory->SetGuidance("control double beta.");


  // /MG/generator/BaryonDecay/rho
  fDecayMode = new G4UIcmdWithAnInteger("/MG/generator/BaryonDecay/mode", this);
  fDecayMode->SetGuidance("all in 76Ge !");
  fDecayMode->SetGuidance("mode 1: nnn -> 73Ge pi0 antinu");
  fDecayMode->SetGuidance("mode 2: pnn -> 73Ga pi0 e+");
  fDecayMode->SetGuidance("mode 3: ppn -> 73Zn pi+ e+");
  fDecayMode->SetGuidance("mode 4: ppp -> 73Cu pi+ pi+ e+");
  fDecayMode->SetGuidance("mode 5: nn  -> 74Ge pi0 pi0");
  fDecayMode->SetGuidance("mode 6: nn  -> 74Ge pi+ pi-");
  fDecayMode->SetGuidance("mode 7: np  -> 74Ga pi+ pi0");
  fDecayMode->SetGuidance("mode 8: np  -> 74Zn pi+ pi+");
  fDecayMode->SetParameterName("case",false);
  fDecayMode->SetRange("case > 0 && case < 9");
}

//---------------------------------------------------------------------------//

MGGeneratorBaryonDecayMessenger::MGGeneratorBaryonDecayMessenger(const MGGeneratorBaryonDecayMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorBaryonDecayMessenger::~MGGeneratorBaryonDecayMessenger()
{
  delete fDecayMode;
  delete fBaryonDecayDirectory;
}



//---------------------------------------------------------------------------//

void MGGeneratorBaryonDecayMessenger::SetNewValue(G4UIcommand *cmd, G4String newValues)
{
  if(cmd == fDecayMode)
    fBaryonDecayGenerator->SetDecayMode(fDecayMode->GetNewIntValue(newValues));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
