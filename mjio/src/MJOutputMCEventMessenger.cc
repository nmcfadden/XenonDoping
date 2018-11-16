//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MJOutputMCEventMessenger.cc,v 1.2 2006-08-28 22:40:51 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputMCEventMessenger.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Sep 14 10:25:02 PDT 2005
 * 
 * REVISION:
 *
 * 09-14-2005, Created, R. Henning

 */
//---------------------------------------------------------------------------//
//

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

#include "mjio/MJOutputMCEvent.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputMCEventMessenger.hh"

//---------------------------------------------------------------------------//

MJOutputMCEventMessenger::MJOutputMCEventMessenger(MJOutputMCEvent *cptr):
  fOutputMCEvent(cptr)
{
  // /MG/io/MCEvent
  fDirectory = new G4UIdirectory("/MG/io/MCEvent/");

 // /MG/io/MCEvent/setG4StepsOutput
  fSetG4StepsOutputCmd = new G4UIcmdWithABool("/MG/io/MCEvent/setG4StepsOutput",
					      this);
  fSetG4StepsOutputCmd->SetGuidance("Activates writing G4 steps to file.");

  // /MG/io/MCEvent/useTimeWindow
  fSetUseTimeWindowCmd = new G4UIcmdWithABool("/MG/io/MCEvent/useTimeWindow",
  this);
  fSetUseTimeWindowCmd->SetGuidance("Command to window events into time windows");

  // /MG/io/MCEvent/setTimeWindow
  fSetTimeWindowCmd = new
  G4UIcmdWithADoubleAndUnit("/MG/io/MCEvent/setTimeWindow", this);
  fSetTimeWindowCmd->SetGuidance("Set a time window");
  fSetTimeWindowCmd->SetDefaultUnit("second");
  fSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");
}

//---------------------------------------------------------------------------//

MJOutputMCEventMessenger::~MJOutputMCEventMessenger()
{
  delete fSetG4StepsOutputCmd;
  delete fDirectory;
  delete fSetUseTimeWindowCmd;
  delete fSetTimeWindowCmd;
}

//---------------------------------------------------------------------------//

void MJOutputMCEventMessenger::SetNewValue(G4UIcommand* cmd, G4String nV)
{
  if(cmd == fSetG4StepsOutputCmd) {
    fOutputMCEvent->SetG4StepsOutput(fSetG4StepsOutputCmd->GetNewBoolValue(nV));
  }

  if(cmd == fSetUseTimeWindowCmd){
    fOutputMCEvent->SetUseTimeWindow(fSetUseTimeWindowCmd->GetNewBoolValue(nV));
  }

  if(cmd == fSetTimeWindowCmd){
    fOutputMCEvent->SetTimeWindow(fSetTimeWindowCmd->GetNewDoubleValue(nV));
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
