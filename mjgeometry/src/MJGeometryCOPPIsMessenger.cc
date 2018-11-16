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
//      
// CLASS IMPLEMENTATION:  MJGeometryMelissaDetectorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  Modeled after MJGeometryWIPPnNewShieldMessenger.cc
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Mary Kidd
 * CONTACT: 
 * FIRST SUBMISSION:  June 26, 2012
 * 
 * REVISION:
 *
 * 
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "mjgeometry/MJGeometryCOPPIs.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryCOPPIsMessenger.hh"  

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryCOPPIsMessenger::MJGeometryCOPPIsMessenger(MJGeometryCOPPIs* Pauli):
fCOPPIsDetector( Pauli )
{
  fCOPPIsDir = new G4UIdirectory("/MG/geometry/COPPIs/");
 
  // turn the use of OPPIs 3 and 4 on and off
  fUseOPPI3Cmd = 
    new G4UIcmdWithABool("/MG/geometry/COPPIs/useOPPI3",this);
  fUseOPPI3Cmd->SetGuidance("Command to place OPPI3");

  fUseOPPI4Cmd = 
    new G4UIcmdWithABool("/MG/geometry/COPPIs/useOPPI4",this);
  fUseOPPI4Cmd->SetGuidance("Command to place OPPI4");

  //choose distance between detectors

  fSetDetSepCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setDetSep", this);
  fSetDetSepCmd->SetGuidance("Set separation of OPPI3 and OPPI4");
  fSetDetSepCmd->SetDefaultUnit("cm");
  fSetDetSepCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetDetSepCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //choose OPPI3 top dead layer

  fSetOPPI3TDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setOPPI3TDL", this);
  fSetOPPI3TDLCmd->SetGuidance("Set top dead layer of OPPI3");
  fSetOPPI3TDLCmd->SetDefaultUnit("mm");
  fSetOPPI3TDLCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetOPPI3TDLCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //choose OPPI4 top dead layer

  fSetOPPI4TDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setOPPI4TDL", this);
  fSetOPPI4TDLCmd->SetGuidance("Set top dead layer of OPPI4");
  fSetOPPI4TDLCmd->SetDefaultUnit("mm");
  fSetOPPI4TDLCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetOPPI4TDLCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //choose OPPI3 side dead layer

  fSetOPPI3SDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setOPPI3SDL", this);
  fSetOPPI3SDLCmd->SetGuidance("Set excess side dead layer of OPPI3");
  fSetOPPI3SDLCmd->SetDefaultUnit("mm");
  fSetOPPI3SDLCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetOPPI3SDLCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //choose OPPI4 side dead layer

  fSetOPPI4SDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setOPPI4SDL", this);
  fSetOPPI4SDLCmd->SetGuidance("Set excess side dead layer of OPPI4");
  fSetOPPI4SDLCmd->SetDefaultUnit("mm");
  fSetOPPI4SDLCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetOPPI4SDLCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // choose whether to include lead cylinder
  fUsePbCylCmd = new G4UIcmdWithABool(
    "/MG/geometry/COPPIs/usePbCyl",this);
  fUsePbCylCmd->SetGuidance("Command to use lead cylinder");

  fSetPbLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/COPPIs/setPbL", this);
  fSetPbLCmd->SetGuidance("Set separation of OPPI3 and OPPI4");
  fSetPbLCmd->SetDefaultUnit("cm");
  fSetPbLCmd->SetUnitCandidates(" m cm mm micrometer ");
  fSetPbLCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // choose which foils to include
  fUseZnCmd = new G4UIcmdWithABool(
    "/MG/geometry/COPPIs/useZn",this);
  fUseZnCmd->SetGuidance("Command to use Zn foil");

  fUseZrCmd = new G4UIcmdWithABool(
    "/MG/geometry/COPPIs/useZr",this);
  fUseZrCmd->SetGuidance("Command to use Zr foil");

  fUseNbCmd = new G4UIcmdWithABool(
    "/MG/geometry/COPPIs/useNb",this);
  fUseNbCmd->SetGuidance("Command to use Nb foil");

  fUseCdCmd = new G4UIcmdWithABool(
    "/MG/geometry/COPPIs/useCd",this);
  fUseCdCmd->SetGuidance("Command to use Cd foil");


}

MJGeometryCOPPIsMessenger::~MJGeometryCOPPIsMessenger()
{
  delete fCOPPIsDir;
  delete fUseOPPI3Cmd;
  delete fUseOPPI4Cmd;
  delete fSetDetSepCmd;
  delete fSetPbLCmd;
  delete fUsePbCylCmd;
  delete fUseZnCmd;
  delete fUseZrCmd;
  delete fUseNbCmd;
  delete fUseCdCmd;
}

void MJGeometryCOPPIsMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  if (command == fUseOPPI3Cmd) 
  {
    fCOPPIsDetector->UseOPPI3(fUseOPPI3Cmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using OPPI3" << endlog; 
  }
  else if (command == fUseOPPI4Cmd)
  {
    fCOPPIsDetector->UseOPPI4(fUseOPPI4Cmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using OPPI4"<<endlog;
  }
  else if (command == fSetDetSepCmd)
  {
   fCOPPIsDetector->SetDetSep(fSetDetSepCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using detector separation of"<<newValues<<"." << endlog;
    }
  else if (command == fSetOPPI3TDLCmd)
  {
   fCOPPIsDetector->SetOPPI3TDL(fSetOPPI3TDLCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using OPPI3 top dead layer of"<<newValues<<"." << endlog;
    }
  else if (command == fSetOPPI4TDLCmd)
  {
   fCOPPIsDetector->SetOPPI4TDL(fSetOPPI4TDLCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using OPPI4 top dead layer of"<<newValues<<"." << endlog;
    }
  else if (command == fSetOPPI3SDLCmd)
  {
   fCOPPIsDetector->SetOPPI3SDL(fSetOPPI3SDLCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using OPPI3 excess side dead layer of"<<newValues<<"." << endlog;
    }
  else if (command == fSetOPPI4SDLCmd)
  {
   fCOPPIsDetector->SetOPPI4SDL(fSetOPPI4SDLCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using OPPI4 excess side dead layer of"<<newValues<<"." << endlog;
    }
 else if (command == fUsePbCylCmd)
  {
    fCOPPIsDetector->UsePbCyl(fUsePbCylCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Pb Cylinder"<<endlog;
  }
  else if (command == fSetPbLCmd)
  {
   fCOPPIsDetector->SetPbL(fSetPbLCmd->GetNewDoubleValue(newValues));
    MGLog(trace) << "Using lead length of"<<newValues<<"." << endlog;
    }  
 else if (command == fUseZnCmd)
  {
    fCOPPIsDetector->UseZn(fUseZnCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Zn foil"<<endlog;
  }
 else if (command == fUseZrCmd)
  {
    fCOPPIsDetector->UseZr(fUseZrCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Zr foil"<<endlog;
  }
 else if (command == fUseNbCmd)
  {
    fCOPPIsDetector->UseNb(fUseNbCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Nb foil"<<endlog;
  }
 else if (command == fUseCdCmd)
  {
    fCOPPIsDetector->UseCd(fUseCdCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Cd foil"<<endlog;
  }

}
