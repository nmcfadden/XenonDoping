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
// $Id: TSMJDemModInTankMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  TSMJDemModInTankMessenger.cc
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
  * AUTHOR: R.Martin
 * CONTACT: 
 * FIRST SUBMISSION: 12 Mar 2012
 * 
 * REVISION:
 * 
 * 03-12-2012 R. Martin created
 *
 */
//---------------------------------------------------------------------------//
//
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"
#include "io/MGLogger.hh"
#include "tonscalegeometry/TSMJDemModInTankParallelWorld.hh" 

//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSMJDemModInTankParallelWorldMessenger.hh" 

//---------------------------------------------------------------------------//

TSMJDemModInTankParallelWorldMessenger::TSMJDemModInTankParallelWorldMessenger(TSMJDemModInTankParallelWorld *mjdemintankparallel) :
  fMJDemModInTankParallelWorld(mjdemintankparallel)
{
  fDirectory = new G4UIdirectory("/MG/geometry/TSMJDemModInTankParallel/");
    
  fnRockWorldsCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTankPrallel/nRockParallelVolumes",this);
  fnRockWorldsCmd->SetGuidance("Number of parallel world volumes in the rock");

  fnShotcreteWorldsCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTankPrallel/nShotcreteParallelVolumes",this);
  fnShotcreteWorldsCmd->SetGuidance("Number of parallel world volumes in the Shotcrete");

  fnLabSpaceWorldsCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTankPrallel/nLabSpaceParallelVolumes",this);
  fnLabSpaceWorldsCmd->SetGuidance("Number of parallel world volumes in the LabSpace");

  fnTankWorldsCmd = new G4UIcmdWithAnInteger("/MG/geometry/TSMJDemModInTankPrallel/nTankParallelVolumes",this);
  fnTankWorldsCmd->SetGuidance("Number of parallel world volumes in the Tank");
}

// -----------------------------------------------------------------------

TSMJDemModInTankParallelWorldMessenger::~TSMJDemModInTankParallelWorldMessenger()
{
  delete fnRockWorldsCmd;
  delete fnShotcreteWorldsCmd;
  delete fnLabSpaceWorldsCmd;
  delete fnTankWorldsCmd;
  delete fDirectory;
}

// -----------------------------------------------------------------------

void TSMJDemModInTankParallelWorldMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(fMJDemModInTankParallelWorld){
    if(cmd == fnRockWorldsCmd)fMJDemModInTankParallelWorld->SetnRockWorlds(fnRockWorldsCmd->GetNewIntValue(nV));
    else if(cmd == fnShotcreteWorldsCmd )fMJDemModInTankParallelWorld->SetnShotcreteWorlds(fnShotcreteWorldsCmd->GetNewIntValue(nV));
    else if(cmd == fnLabSpaceWorldsCmd )fMJDemModInTankParallelWorld->SetnLabSpaceWorlds(fnLabSpaceWorldsCmd->GetNewIntValue(nV));
    else if(cmd == fnTankWorldsCmd )fMJDemModInTankParallelWorld->SetnTankWorlds(fnTankWorldsCmd->GetNewIntValue(nV));
    else{};
  }
}

// -----------------------------------------------------------------------

