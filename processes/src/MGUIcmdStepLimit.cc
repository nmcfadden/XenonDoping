//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: MGUIcmdStepLimit.cc,v 1.1 2009-01-17 15:26:59 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#include "processes/MGUIcmdStepLimit.hh"
#include "G4Tokenizer.hh"
#include "G4UnitsTable.hh"
#include <sstream>

MGUIcmdStepLimit::MGUIcmdStepLimit(const char* aCommandPath, G4UImessenger* aMessenger)
:G4UIcommand(aCommandPath, aMessenger)
{
  G4UIparameter * particleParam = new G4UIparameter('s');
  SetParameter(particleParam);
  particleParam->SetParameterName("particle name");
  G4UIparameter * volumeParam = new G4UIparameter('s');
  SetParameter(volumeParam);
  volumeParam->SetParameterName("physical volume name");
  G4UIparameter * stepValueParam = new G4UIparameter('d');
  SetParameter(stepValueParam);
  stepValueParam->SetParameterName("step value");
  G4UIparameter * stepUnitParam = new G4UIparameter('s');
  SetParameter(stepUnitParam);
  stepUnitParam->SetParameterName("step unit");

  AvailableForStates(G4State_PreInit);

  SetGuidance("Set step limit for [particle] in [physical volume name].");
}

G4double MGUIcmdStepLimit::GetStepSize(const char* paramString)
{
  char particle[30];
  char volume[30];
  G4double value;
  char units[30];

  std::istringstream is(paramString);
  is >> particle >> volume >> value >> units;

  return (value*ValueOf(G4String(units)));
}

G4String MGUIcmdStepLimit::GetParticleName(const char* paramString)
{
  char particle[30];
  char volume[30];
  G4double value;
  char units[30];

  std::istringstream is(paramString);
  is >> particle >> volume >> value >> units;

  return G4String(particle);
}

G4String MGUIcmdStepLimit::GetVolumeName(const char* paramString)
{
  char particle[30];
  char volume[30];
  G4double value;
  char units[30];

  std::istringstream is(paramString);
  is >> particle >> volume >> value >> units;

  return G4String(volume);
}

