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
// $Id: BACON_BaselineMessenger.cc,v 1.4 2005/03/31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  BACON_BaselineMessenger.cc
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
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: May 2018
 * 
 * REVISION:
 *
 * 05-2018, Created, R. Massarczyk
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIdirectory.hh"

#include "bacongeometry/BACON_Baseline.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "bacongeometry/BACON_BaselineMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


BACON_BaselineMessenger::BACON_BaselineMessenger(
BACON_Baseline *bacon) : fBACON(bacon)
{
  //BACoN 200
  fBACONDirectory = new G4UIdirectory("/MG/BACON/");

  fLiquidCmd = new G4UIcmdWithAString("/MG/BACON/SetLiquid", this);
  fLiquidCmd->SetGuidance("This is just a test until we have more commands");
  fLiquidCmd->AvailableForStates(G4State_PreInit);
}

//---------------------------------------------------------------------------//

BACON_BaselineMessenger::~BACON_BaselineMessenger()
{

	delete fLiquidCmd;
}

//---------------------------------------------------------------------------//

void BACON_BaselineMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
	if (cmd == fLiquidCmd) fBACON->SetLiquid(nV);

	return;
}


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
