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
// CLASS IMPLEMENTATION:  MJGeometrySTCMessenger.cc
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
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 10-17-2013, Updated Coldplate and added Dewar. J. MacMullin
 * 11-14-2013, 1. Made the STC an assembly so it could place in the CharStand.
 *			   2. Added a string into the STC. J. MacMullin
 * 01-21-2014, 1. Created a "shell" w/in the rock cavern in the e-forming room for 
 *				  simulating the rock around the STC. 
 *			   2. Added in walls of the e-forming room. 
 *             3. Added ability to rotate ColdPlate, CMS, and IR shield from messenger. J. MacMullin
 *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

#include "mjdemonstrator/MJGeometrySTC.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJGeometrySTCMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometrySTCMessenger::MJGeometrySTCMessenger(MJGeometrySTC *stc) : fSTC(stc) 
{
  // /MG/stc
  fSTCDirectory = new G4UIdirectory("/MG/stc/");

  // /MG/stc/EFROn
  fEFROnCmd = new G4UIcmdWithABool("/MG/stc/EFROn", this);
  fEFROnCmd->SetGuidance("Set to true if you want to separate the rock in the e-forming room from the rest of the cavern.");
  fEFROnCmd->SetGuidance("Set to false if you want to have the cavern be one item.");
  fEFROnCmd->AvailableForStates(G4State_PreInit);

  // /MG/stc/cavernOn
  fCavernOnCmd = new G4UIcmdWithABool("/MG/stc/cavernOn", this);
  fCavernOnCmd->SetGuidance("Set to true if you want to activate the surrounding rock cavern.");
  fCavernOnCmd->SetGuidance("Set to false if you want to just have space around the STC.");
  fCavernOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/stc/EFRwallsOn
  fEFRwallsOnCmd = new G4UIcmdWithABool("/MG/stc/EFRwallsOn", this);
  fEFRwallsOnCmd->SetGuidance("Set to true if you want to activate the EFR walls.");
  fEFRwallsOnCmd->SetGuidance("Set to false if you do not want to include the EFR walls in the geometry.");
  fEFRwallsOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/stc/stringTestCryoOn
  fStringTestCryoOnCmd = new G4UIcmdWithABool("/MG/stc/stringTestCryoOn", this);
  fStringTestCryoOnCmd->SetGuidance("Set to true if you want to include the STC assembly in the geometry.");
  fStringTestCryoOnCmd->SetGuidance("Set to false if you do not want to include the STC assembly in the geometry.");
  fStringTestCryoOnCmd->AvailableForStates(G4State_PreInit); 
  
  // /MG/stc/coldPlateRotationInDegrees
  fColdPlateRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/stc/coldPlateRotationInDegrees", this);
  fColdPlateRotationInDegreesCmd->SetGuidance("Rotation of ColdPlate w.r.t. the string (in degrees)");
  fColdPlateRotationInDegreesCmd->SetGuidance("Possible values: 0.0, 120.0, or 240.0");
  fColdPlateRotationInDegreesCmd->SetDefaultValue(0.0);
  fColdPlateRotationInDegreesCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/stc/CMSRotationInDegrees
  fCMSRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/stc/CMSRotationInDegrees", this);
  fCMSRotationInDegreesCmd->SetGuidance("Rotation of CMS w.r.t. the string (in degrees)");
  fCMSRotationInDegreesCmd->SetDefaultValue(0.0);
  fCMSRotationInDegreesCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/stc/thermalRotationInDegrees
  fThermalRotationInDegreesCmd = new G4UIcmdWithADouble("/MG/stc/thermalRotationInDegrees", this);
  fThermalRotationInDegreesCmd->SetGuidance("Rotation of IR shield w.r.t. the string (in degrees)");
  fThermalRotationInDegreesCmd->SetGuidance("Possible values: 30.0*i for any integer i (e.g. 0.0, 30.0, 60.0 ... 300.0, 330.0)");
  fThermalRotationInDegreesCmd->SetDefaultValue(0.0);
  fThermalRotationInDegreesCmd->AvailableForStates(G4State_PreInit);
 
  // /MG/stc/stringOn
  fStringOnCmd = new G4UIcmdWithABool("/MG/stc/stringOn", this);
  fStringOnCmd->SetGuidance("Set to true if you want to include the string assembly in the geometry.");
  fStringOnCmd->SetGuidance("Set to false if you do not want to include the string assembly in the geometry.");
  fStringOnCmd->AvailableForStates(G4State_PreInit);
}

//---------------------------------------------------------------------------//

MJGeometrySTCMessenger::~MJGeometrySTCMessenger()
{
  delete fEFROnCmd;
  delete fCavernOnCmd;
  delete fEFRwallsOnCmd;
  delete fStringTestCryoOnCmd;
  delete fColdPlateRotationInDegreesCmd;
  delete fCMSRotationInDegreesCmd;
  delete fThermalRotationInDegreesCmd;
  delete fStringOnCmd;
  delete fSTCDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometrySTCMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fEFROnCmd)
  	 fSTC->SetEFR(fEFROnCmd->GetNewBoolValue(nV));
  if(cmd == fCavernOnCmd)
     fSTC->SetCavern(fCavernOnCmd->GetNewBoolValue(nV));
  if(cmd == fEFRwallsOnCmd)
     fSTC->SetEFRwalls(fEFRwallsOnCmd->GetNewBoolValue(nV));
  if(cmd == fStringTestCryoOnCmd)
  	 fSTC->SetStringTestCryo(fStringTestCryoOnCmd->GetNewBoolValue(nV));
  if(cmd == fColdPlateRotationInDegreesCmd)
  	 fSTC->SetColdPlateAngleInDegrees(fColdPlateRotationInDegreesCmd->GetNewDoubleValue(nV));
  if(cmd == fCMSRotationInDegreesCmd)
  	 fSTC->SetCMSAngleInDegrees(fCMSRotationInDegreesCmd->GetNewDoubleValue(nV));
  if(cmd == fThermalRotationInDegreesCmd)
  	 fSTC->SetThermalAngleInDegrees(fThermalRotationInDegreesCmd->GetNewDoubleValue(nV));
  if(cmd == fStringOnCmd)
     fSTC->SetString(fStringOnCmd->GetNewBoolValue(nV));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
