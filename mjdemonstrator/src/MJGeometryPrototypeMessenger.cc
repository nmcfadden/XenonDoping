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
//      
// CLASS IMPLEMENTATION:  MJGeometryPrototypeMessenger.cc
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
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday March 26, 2014
 * 
 * REVISION:
 * 04-24-2014, Added in the thermal shield assembly, J. MacMullin
 * 03-26-2014, Created, J. MacMullin
 *
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
#include "G4UIdirectory.hh"
#include "TMath.h"

#include "mjdemonstrator/MJGeometryPrototype.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJGeometryPrototypeMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryPrototypeMessenger::MJGeometryPrototypeMessenger(
MJGeometryPrototype *proto) : fPrototype(proto)
{
  // /MG/prototype
  fPrototypeDirectory = new G4UIdirectory("/MG/prototype/");
  
  // /MG/prototype/PCposition
  fPCpositionCmd = new G4UIcmdWithAString("/MG/prototype/PCposition",this);
  fPCpositionCmd->SetDefaultValue("atGlovebox");
  fPCpositionCmd->SetGuidance("'atGlovebox' or 'inShield' position:");
  G4String candidates0 = "atGlovebox inShield";
  fPCpositionCmd->SetCandidates(candidates0);
  fPCpositionCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/prototype/DRshellOn
  fDRshellOnCmd = new G4UIcmdWithABool("/MG/prototype/DRshellOn", this);
  fDRshellOnCmd->SetGuidance("Set to true if you want to separate the rock in the e-forming room from the rest of the cavern.");
  fDRshellOnCmd->SetGuidance("Set to false if you want to have the cavern be one item.");
  fDRshellOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/cavernOn
  fCavernOnCmd = new G4UIcmdWithABool("/MG/prototype/cavernOn", this);
  fCavernOnCmd->SetGuidance("Set to true if you want to activate the surrounding rock cavern.");
  fCavernOnCmd->SetGuidance("Set to false if you want to just have space around the prototype.");
  fCavernOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/DRwallsOn
  fDRwallsOnCmd = new G4UIcmdWithABool("/MG/prototype/DRwallsOn", this);
  fDRwallsOnCmd->SetGuidance("Set to true if you want to activate the EFR walls.");
  fDRwallsOnCmd->SetGuidance("Set to false if you do not want to include the EFR walls in the geometry.");
  fDRwallsOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/muonVetoOn
  fMuonVetoOnCmd = new G4UIcmdWithABool("/MG/prototype/muonVetoOn", this);
  fMuonVetoOnCmd->SetGuidance("Set to true if you want to include the muon veto in the geometry.");
  fMuonVetoOnCmd->SetGuidance("Set to false if you want to not include the muon veto in the geometry.");
  fMuonVetoOnCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/prototype/vetoConfig
  fVetoConfigCmd = new G4UIcmdWithAString("/MG/prototype/vetoConfig", this);
  fVetoConfigCmd->SetDefaultValue("111111");
  fVetoConfigCmd->SetGuidance("Use this command to enable only certain panels of the muon veto.");
  fVetoConfigCmd->SetGuidance("The syntax is: nnnnnn, where n must be 0 or 1.");
  fVetoConfigCmd->SetGuidance("The first digit tells whether the top veto panels should be enabled.");
  fVetoConfigCmd->SetGuidance("The second is for the bottom veto panels; the third for the panels on the north side;");
  fVetoConfigCmd->SetGuidance("the fourth for the panels on the south side; the fifth for the panels on the west side;");
  fVetoConfigCmd->SetGuidance("and the sixth for the panels on the east side. Ex: '/MG/prototype/vetoConfig 011000' ");
  fVetoConfigCmd->SetGuidance("would enable only the bottom panels and the panels on the north side.");
    int command_l = 6;
    int n_permuations = (int)TMath::Power(2.,command_l);
    int i_combo, ph; char ind_Cand[10];
  G4String candidates1("000000");
    for(int i=1; i<n_permuations; i++){
      i_combo = 0;
      ph = i;
      for(int j=(command_l-1); j>=0; j--){
        if(ph>=(int)TMath::Power(2.,j)){
          i_combo+=(int)TMath::Power(10.,j); 
          ph-=(int)TMath::Power(2.,j);
          }
        } 
      sprintf(ind_Cand," %06d",i_combo);
  candidates1.append(ind_Cand);
      }
  fVetoConfigCmd->SetCandidates(candidates1);
  fVetoConfigCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/prototype/radShieldOn
  fRadShieldOnCmd = new G4UIcmdWithABool("/MG/prototype/radShieldOn", this);
  fRadShieldOnCmd->SetGuidance("Set to true if you want to include the rad shield in the geometry.");
  fRadShieldOnCmd->SetGuidance("Set to false if you want to not include the rad shield in the geometry.");
  fRadShieldOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/radConfig
  fRadConfigCmd = new G4UIcmdWithAString("/MG/prototype/radConfig", this);
  fRadConfigCmd->SetDefaultValue("1111111");
  fRadConfigCmd->SetGuidance("Use this command to enable only certain components of the rad shield.");
  fRadConfigCmd->SetGuidance("The syntax is: nnnnnnn, where n must be 0 or 1.");
  fRadConfigCmd->SetGuidance("The first digit tells whether the inner Cu should be enabled.");
  fRadConfigCmd->SetGuidance("The second is for the outer Cu; the third for the Pb shield;");
  fRadConfigCmd->SetGuidance("the fourth for the Pb behind the vacuum cubes (to cover holes where crossarms pass through);");
  fRadConfigCmd->SetGuidance("the fifth for the poly shield (not including overfloor); the sixth for the overfloor's poly;");
  fRadConfigCmd->SetGuidance("and the seventh for the Rn box. Ex: '/MG/prototype/vetoConfig 0110001' ");
  fRadConfigCmd->SetGuidance("would enable the outer Cu, Pb shield and Rn box.");
    command_l = 7; 
    n_permuations = (int)TMath::Power(2.,command_l);
  G4String candidates2("0000000");
    for(int i=1; i<n_permuations; i++){
      i_combo = 0;
      ph = i;
      for(int j=(command_l-1); j>=0; j--){
        if(ph>=(int)TMath::Power(2.,j)){
          i_combo+=(int)TMath::Power(10.,j); 
          ph-=(int)TMath::Power(2.,j);
          }
        } 
      sprintf(ind_Cand," %07d",i_combo);
  candidates2.append(ind_Cand);
      }
  fRadConfigCmd->SetCandidates(candidates2);
  fRadConfigCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/monoCuPlateOn
  fMonoCuPlateOnCmd = new G4UIcmdWithABool("/MG/prototype/monoCuPlateOn", this);
  fMonoCuPlateOnCmd->SetGuidance("Set to true if you want to include the monolith Cu plate in the geometry.");
  fMonoCuPlateOnCmd->SetGuidance("Set to false if you want to not include the monolith Cu plate in the geometry.");
  fMonoCuPlateOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/calOn
  fCalOnCmd = new G4UIcmdWithABool("/MG/prototype/calOn", this);
  fCalOnCmd->SetGuidance("Set to true if you want to include the calibration system in the geometry.");
  fCalOnCmd->SetGuidance("Set to false if you want to not include the calibration system in the geometry.");
  fCalOnCmd->AvailableForStates(G4State_PreInit);

 // /MG/prototype/cryoOn
  fCryoOnCmd = new G4UIcmdWithABool("/MG/prototype/cryoOn", this);
  fCryoOnCmd->SetGuidance("Set to true if you want to include the cryostat assembly in the geometry.");
  fCryoOnCmd->SetGuidance("Set to false if you do not want to include the cryostat assembly in the geometry.");
  fCryoOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/thermosiphonOn
  fThermosiphonOnCmd = new G4UIcmdWithABool("/MG/prototype/thermosiphonOn", this);
  fThermosiphonOnCmd->SetGuidance("Set to true if you want to include the thermosiphon assembly in the geometry.");
  fThermosiphonOnCmd->SetGuidance("Set to false if you do not want to include the thermosiphon assembly in the geometry.");
  fThermosiphonOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/thermalShieldOn
  fThermalShieldOnCmd = new G4UIcmdWithABool("/MG/prototype/thermalShieldOn", this);
  fThermalShieldOnCmd->SetGuidance("Set to true if you want to include the thermal shield assembly in the geometry.");
  fThermalShieldOnCmd->SetGuidance("Set to false if you do not want to include the thermal shield assembly in the geometry.");
  fThermalShieldOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/cablesOn
  /*fCablesOnCmd = new G4UIcmdWithABool("/MG/prototype/cablesOn", this);
  fCablesOnCmd->SetGuidance("Set to true if you want to include the cables assembly in the geometry.");
  fCablesOnCmd->SetGuidance("Set to false if you do not want to include the cables assembly in the geometry.");
  fCablesOnCmd->AvailableForStates(G4State_PreInit);*/

  // /MG/prototype/stringOn
  fStringOnCmd = new G4UIcmdWithABool("/MG/prototype/stringOn", this);
  fStringOnCmd->SetGuidance("Set to true if you want to include the detector string assembly in the geometry.");
  fStringOnCmd->SetGuidance("Set to false if you do not want to include the detector string assembly in the geometry.");
  fStringOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/prototype/tempSensorOn
  fTempSensorOnCmd = new G4UIcmdWithABool("/MG/prototype/tempSensorOn", this);
  fTempSensorOnCmd->SetGuidance("Set to true if you want to include the temperature sensor in the geometry.");
  fTempSensorOnCmd->SetGuidance("Set to false if you do not want to include the temperature sensor in the geometry.");
  fTempSensorOnCmd->AvailableForStates(G4State_PreInit);

}

//---------------------------------------------------------------------------//

MJGeometryPrototypeMessenger::~MJGeometryPrototypeMessenger()
{
  delete fPCpositionCmd;
  delete fDRshellOnCmd;
  delete fCavernOnCmd;
  delete fDRwallsOnCmd;
  delete fMuonVetoOnCmd;
  delete fVetoConfigCmd;
  delete fRadShieldOnCmd;
  delete fRadConfigCmd;
  delete fMonoCuPlateOnCmd;
  delete fCalOnCmd;
  delete fCryoOnCmd;
  delete fThermosiphonOnCmd;
  delete fThermalShieldOnCmd;
  //delete fCablesOnCmd;
  delete fStringOnCmd;
  delete fTempSensorOnCmd;
  delete fPrototypeDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryPrototypeMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fPCpositionCmd) {
	G4String position = nV; 
	fPrototype->SetPCposition(position);
	}
  if(cmd == fDRshellOnCmd)
  	 fPrototype->SetDRshell(fDRshellOnCmd->GetNewBoolValue(nV));
  if(cmd == fCavernOnCmd)
     fPrototype->SetCavern(fCavernOnCmd->GetNewBoolValue(nV));
  if(cmd == fDRwallsOnCmd)
     fPrototype->SetDRwalls(fDRwallsOnCmd->GetNewBoolValue(nV));
  if(cmd == fMuonVetoOnCmd)
    fPrototype->SetMuonVeto(fMuonVetoOnCmd->GetNewBoolValue(nV));
  if(cmd == fVetoConfigCmd) {
	G4String vetoConfig = nV; 
	fPrototype->SetVetoConfig(vetoConfig);
	}
  if(cmd == fRadShieldOnCmd)
    fPrototype->SetRadShield(fRadShieldOnCmd->GetNewBoolValue(nV));
  if(cmd == fRadConfigCmd) {
	G4String radConfig = nV; 
	fPrototype->SetRadConfig(radConfig);
	}
  if(cmd == fMonoCuPlateOnCmd)
    fPrototype->SetMonoCuPlate(fMonoCuPlateOnCmd->GetNewBoolValue(nV));
  if(cmd == fCalOnCmd)
    fPrototype->SetCal(fCalOnCmd->GetNewBoolValue(nV));
  if(cmd == fCryoOnCmd)
    fPrototype->SetCryo(fCryoOnCmd->GetNewBoolValue(nV));
  if(cmd == fThermosiphonOnCmd)
    fPrototype->SetThermosiphon(fThermosiphonOnCmd->GetNewBoolValue(nV));
  if(cmd == fThermalShieldOnCmd)
    fPrototype->SetThermalShield(fThermalShieldOnCmd->GetNewBoolValue(nV));
  //if(cmd == fCablesOnCmd)
  //  fPrototype->SetCables(fCablesOnCmd->GetNewBoolValue(nV));
  if(cmd == fStringOnCmd)
    fPrototype->SetString(fStringOnCmd->GetNewBoolValue(nV));
  if(cmd == fTempSensorOnCmd)
    fPrototype->SetTempSensor(fTempSensorOnCmd->GetNewBoolValue(nV));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
