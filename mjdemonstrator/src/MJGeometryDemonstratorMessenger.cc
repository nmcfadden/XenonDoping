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
// $Id: MJGeometryDemonstratorMessenger.cc,v 1.4 2005/03/31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryDemonstratorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * If the cables are included, there are overlapping geometries. -Jacquie
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M. Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: Thursday May 5, 2011
 * 
 * REVISION:
 *
 * 09-01-2017, Add commands for poly shield, R. Massarczyk
 * 02-24-2016, Add commands for positioning the cryostats, T. Caldwell
 * 04-24-2014, Fixed overlapping geometries, J. MacMullin
 * 04-04-2011, Created, M. Kidd
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
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

#include "mjdemonstrator/MJGeometryDemonstrator.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJGeometryDemonstratorMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryDemonstratorMessenger::MJGeometryDemonstratorMessenger(
MJGeometryDemonstrator *demo) : fDemonstrator(demo)
{
  // /MG/demonstrator
  fDemonstratorDirectory = new G4UIdirectory("/MG/demonstrator/");

  //  /MG/demonstrator/cryostat
  fCryostatChoiceCmd = new G4UIcmdWithAString("/MG/demonstrator/cryostat", this);
  fCryostatChoiceCmd->SetGuidance("Pick which cryostat configuration you'd like to use.");
  fCryostatChoiceCmd->SetGuidance("This should correspond to a choice in the .JSON file.");
  fCryostatChoiceCmd->SetParameterName("theCryo", false);
  G4String candidates;
  candidates = "MJD";
  fCryostatChoiceCmd->SetCandidates(candidates);
  fCryostatChoiceCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cavernOn
  fCavernOnCmd = new G4UIcmdWithABool("/MG/demonstrator/cavernOn", this);
  fCavernOnCmd->SetGuidance("Set to true if you want to activate the surrounding rock cavern.");
  fCavernOnCmd->SetGuidance("Set to false if you want to have just the space around the demonstrator.");
  fCavernOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/muonVetoOn
  fMuonVetoOnCmd = new G4UIcmdWithABool("/MG/demonstrator/muonVetoOn", this);
  fMuonVetoOnCmd->SetGuidance("Set to true if you want to include the muon veto in the geometry.");
  fMuonVetoOnCmd->SetGuidance("Set to false if you want to not include the muon veto in the geometry.");
  fMuonVetoOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/radShieldOn
  fRadShieldOnCmd = new G4UIcmdWithABool("/MG/demonstrator/radShieldOn", this);
  fRadShieldOnCmd->SetGuidance("Set to true if you want to include the rad shield in the geometry.");
  fRadShieldOnCmd->SetGuidance("Set to false if you want to not include the rad shield in the geometry.");
  fRadShieldOnCmd->AvailableForStates(G4State_PreInit);
    

  // /MG/demonstrator/cal1On
  //fCal1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cal1On", this);
  //fCal1OnCmd->SetGuidance("Set to true if you want to include calibration assembly 1 in the geometry.");
  //fCal1OnCmd->SetGuidance("Set to false if you do not want to include calibration assembly 1 in the geometry..");
  //fCal1OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cryo1On
  //fCryo1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cryo1On", this);
  //fCryo1OnCmd->SetGuidance("Set to true if you want to include cryostat assembly 1 in the geometry.");
  //fCryo1OnCmd->SetGuidance("Set to false if you do not want to include cryostat assembly 1 in the geometry..");
  //fCryo1OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cryo1Pos
  fCryo1PosCmd =
    new G4UIcmdWith3VectorAndUnit("/MG/demonstrator/cryo1Pos", this);
  fCryo1PosCmd->SetGuidance("PosCmdition of the module 1 cryostat");
  fCryo1PosCmd->SetDefaultValue(fDemonstrator->GetCryo1Pos());
  fCryo1PosCmd->SetUnitCandidates("mm cm m");
  fCryo1PosCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/demonstrator/cryo1Rot
  fCryo1RotCmd =
    new G4UIcmdWithADoubleAndUnit("/MG/demonstrator/cryo1Rot", this);
  fCryo1RotCmd->SetGuidance("Rotation about z of the module 1 cryostat");
  fCryo1RotCmd->SetDefaultValue(fDemonstrator->GetCryo1Rot());
  fCryo1RotCmd->SetUnitCandidates("rad deg");
  fCryo1RotCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/thermosiphon1On
  //fThermosiphon1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/thermosiphon1On", this);
  //fThermosiphon1OnCmd->SetGuidance("Set to true if you want to include thermosiphon assembly 1 in the geometry.");
  //fThermosiphon1OnCmd->SetGuidance("Set to false if you do not want to include thermosiphon assembly 1 in the geometry..");
  //fThermosiphon1OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/thermalShield1On
  //fThermalShield1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/thermalShield1On", this);
  //fThermalShield1OnCmd->SetGuidance("Set to true if you want to include thermal shield assembly 1 in the geometry.");
  //fThermalShield1OnCmd->SetGuidance("Set to false if you do not want to include thermal shield assembly 1 in the geometry..");
  //fThermalShield1OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cables1On
  /*fCables1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cables1On", this);
  fCables1OnCmd->SetGuidance("Set to true if you want to include cables assembly 1 in the geometry.");
  fCables1OnCmd->SetGuidance("Set to false if you do not want to include cables assembly 1 in the geometry..");
  fCables1OnCmd->AvailableForStates(G4State_PreInit);*/

  // /MG/demonstrator/string1On
  //fString1OnCmd = new G4UIcmdWithABool("/MG/demonstrator/string1On", this);
  //fString1OnCmd->SetGuidance("Set to true if you want to include detector string assembly 1 in the geometry.");
  //fString1OnCmd->SetGuidance("Set to false if you do not want to include detector string assembly 1 in the geometry..");
  //fString1OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cal2On
  //fCal2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cal2On", this);
  //fCal2OnCmd->SetGuidance("Set to true if you want to include calibration assembly 2 in the geometry.");
  //fCal2OnCmd->SetGuidance("Set to false if you do not want to include calibration assembly 2 in the geometry..");
  //fCal2OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cryo2On
  //fCryo2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cryo2On", this);
  //fCryo2OnCmd->SetGuidance("Set to true if you want to include cryostat assembly 2 in the geometry.");
  //fCryo2OnCmd->SetGuidance("Set to false if you do not want to include cryostat assembly 2 in the geometry..");
  //fCryo2OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cryo2PosCmd
  fCryo2PosCmd =
    new G4UIcmdWith3VectorAndUnit("/MG/demonstrator/cryo2Pos", this);
  fCryo2PosCmd->SetGuidance("PosCmdition of the module 2 cryostat");
  fCryo2PosCmd->SetDefaultValue(fDemonstrator->GetCryo2Pos());
  fCryo2PosCmd->SetUnitCandidates("mm cm m");
  fCryo2PosCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cryo2Rot
  fCryo2RotCmd =
    new G4UIcmdWithADoubleAndUnit("/MG/demonstrator/cryo2Rot", this);
  fCryo2RotCmd->SetGuidance("Rotation about z of the module 2 cryostat");
  fCryo2RotCmd->SetDefaultValue(fDemonstrator->GetCryo2Rot());
  fCryo2RotCmd->SetUnitCandidates("rad deg");
  fCryo2RotCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/demonstrator/thermosiphon2On
  //fThermosiphon2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/thermosiphon2On", this);
  //fThermosiphon2OnCmd->SetGuidance("Set to true if you want to include thermosiphon assembly 2 in the geometry.");
  //fThermosiphon2OnCmd->SetGuidance("Set to false if you do not want to include thermosiphon assembly 2 in the geometry..");
  //fThermosiphon2OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/thermalShield2On
  //fThermalShield2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/thermalShield2On", this);
  //fThermalShield2OnCmd->SetGuidance("Set to true if you want to include thermal shield assembly 2 in the geometry.");
  //fThermalShield2OnCmd->SetGuidance("Set to false if you do not want to include thermal shield assembly 2 in the geometry..");
  //fThermalShield2OnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/cables2On
  /*fCables2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/cables2On", this);
  fCables2OnCmd->SetGuidance("Set to true if you want to include cables assembly 2 in the geometry.");
  fCables2OnCmd->SetGuidance("Set to false if you do not want to include cables assembly 2 in the geometry..");
  fCables2OnCmd->AvailableForStates(G4State_PreInit);*/

  // /MG/demonstrator/string2On
  //fString2OnCmd = new G4UIcmdWithABool("/MG/demonstrator/string2On", this);
  //fString2OnCmd->SetGuidance("Set to true if you want to include detector string assembly 2 in the geometry.");
  //fString2OnCmd->SetGuidance("Set to false if you do not want to include detector string assembly 2 in the geometry..");
  //fString2OnCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/demonstrator/InnerCopperOn
  fInnerCopperOnCmd = new G4UIcmdWithABool("/MG/demonstrator/innerCopperOn", this);
  fInnerCopperOnCmd->SetGuidance("Set to true if you want to include inner copper shield in the geometry.");
  fInnerCopperOnCmd->SetGuidance("Set to false if you want N2 instead of inner copper.");
  fInnerCopperOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/TSShieldOn
  fTSShieldOnCmd = new G4UIcmdWithABool("/MG/demonstrator/TSShieldOn", this);
  fTSShieldOnCmd->SetGuidance("Set to true if you want to include the thermosyphon and cube shielding in the geometry.");
  fTSShieldOnCmd->SetGuidance("Set to false if you want vacuum instead of the thermosyphon and cube shielding.");
  fTSShieldOnCmd->AvailableForStates(G4State_PreInit);


  // /MG/demonstrator/shadowShieldThickness
  fShadowShieldThicknessCmd = new G4UIcmdWithAnInteger("/MG/demonstrator/shadowShieldThickness", this);
  char guidance[999];
  sprintf(guidance, "Set the thickness of the Pb shadow shield. Default thickness is %d in. Must be integer number of inches", fDemonstrator->GetShadowShieldThickness());
  fShadowShieldThicknessCmd->SetGuidance(guidance);
  fShadowShieldThicknessCmd->SetDefaultValue(fDemonstrator->GetShadowShieldThickness());
  fShadowShieldThicknessCmd->AvailableForStates(G4State_PreInit);

  // /MG/demonstrator/setPolyShield
  fPolyShieldCmd = new G4UIcmdWithAString("/MG/demonstrator/setPolyShield", this);
  fPolyShieldCmd->SetGuidance("Pick the Polyshield in a DataSet");
  fPolyShieldCmd->SetGuidance("DS0, DS1, ... DS5a, DS5b, DS6");
  fPolyShieldCmd->SetGuidance("or pick single panels by using a decimal corresponding a 24-bit variable");
  fPolyShieldCmd->SetParameterName("thePoly", false);
  fPolyShieldCmd->AvailableForStates(G4State_PreInit);



}

//---------------------------------------------------------------------------//

MJGeometryDemonstratorMessenger::~MJGeometryDemonstratorMessenger()
{
  delete fCavernOnCmd;
  delete fMuonVetoOnCmd;
  delete fRadShieldOnCmd;
  //delete fCal1OnCmd;
  //delete fCryo1OnCmd;
  delete fCryo1PosCmd;
  delete fCryo1RotCmd;
  //delete fThermosiphon1OnCmd;
  //delete fThermalShield1OnCmd;
  //delete fCables1OnCmd;
  //delete fString1OnCmd;
  //delete fCal2OnCmd;
  //delete fCryo2OnCmd;
  delete fCryo2PosCmd;
  delete fCryo2RotCmd;
  //delete fThermosiphon2OnCmd;
  //delete fThermalShield2OnCmd;
  //delete fCables2OnCmd;
  //delete fString2OnCmd;
  delete fInnerCopperOnCmd;
  delete fTSShieldOnCmd;
  delete fShadowShieldThicknessCmd;
  delete fPolyShieldCmd;
  delete fDemonstratorDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryDemonstratorMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fCavernOnCmd)
    fDemonstrator->SetCavern(fCavernOnCmd->GetNewBoolValue(nV));
  else if (cmd == fCryostatChoiceCmd)
    fDemonstrator->SetCryostatChoice(nV);
  else if(cmd == fMuonVetoOnCmd)
    fDemonstrator->SetMuonVeto(fMuonVetoOnCmd->GetNewBoolValue(nV));
  else if(cmd == fRadShieldOnCmd)
    fDemonstrator->SetRadShield(fRadShieldOnCmd->GetNewBoolValue(nV));
  //else if(cmd == fCal1OnCmd)
  //  fDemonstrator->SetCal1(fCal1OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fThermosiphon1OnCmd)
  //  fDemonstrator->SetThermosiphon1(fThermosiphon1OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fThermalShield1OnCmd)
  //  fDemonstrator->SetThermalShield1(fThermalShield1OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fCables1OnCmd)
  //  fDemonstrator->SetCables1(fCables1OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fString1OnCmd)
  //  fDemonstrator->SetString1(fString1OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fCal2OnCmd)
  //  fDemonstrator->SetCal2(fCal2OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fThermosiphon2OnCmd)
  //  fDemonstrator->SetThermosiphon2(fThermosiphon2OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fThermalShield2OnCmd)
  //  fDemonstrator->SetThermalShield2(fThermalShield2OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fCables2OnCmd)
  //  fDemonstrator->SetCables2(fCables2OnCmd->GetNewBoolValue(nV));
  //else if(cmd == fString2OnCmd)
  //  fDemonstrator->SetString2(fString2OnCmd->GetNewBoolValue(nV));
  else if(cmd == fInnerCopperOnCmd)
    fDemonstrator->SetInnerCopper(fInnerCopperOnCmd->GetNewBoolValue(nV));
  else if(cmd == fTSShieldOnCmd)
    fDemonstrator->SetTSShield(fTSShieldOnCmd->GetNewBoolValue(nV));
  else if(cmd == fShadowShieldThicknessCmd) {
    fDemonstrator->SetShadowShieldThickness(fShadowShieldThicknessCmd->GetNewIntValue(nV));
    fShadowShieldThicknessCmd->SetDefaultValue(fDemonstrator->GetShadowShieldThickness());
  }
  else if(cmd == fCryo1PosCmd){
    fDemonstrator->SetCryo1Pos(fCryo1PosCmd->GetNew3VectorValue(nV));
    fCryo1PosCmd->SetDefaultValue(fDemonstrator->GetCryo1Pos());
  }
  else if(cmd == fCryo1RotCmd){
    fDemonstrator->SetCryo1Rot(fCryo1RotCmd->GetNewDoubleValue(nV));
    fCryo1RotCmd->SetDefaultValue(fDemonstrator->GetCryo1Rot());
  }
  else if(cmd == fCryo2PosCmd){
    fDemonstrator->SetCryo2Pos(fCryo2PosCmd->GetNew3VectorValue(nV));
    fCryo2PosCmd->SetDefaultValue(fDemonstrator->GetCryo2Pos());
  }
  else if(cmd == fCryo2RotCmd){
    fDemonstrator->SetCryo2Rot(fCryo2RotCmd->GetNewDoubleValue(nV));
    fCryo2RotCmd->SetDefaultValue(fDemonstrator->GetCryo2Rot());
  }
  else if (cmd == fPolyShieldCmd)
    fDemonstrator->SetPolyShield(nV);

}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
