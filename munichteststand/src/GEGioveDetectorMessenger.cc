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
/**
 * REVISION:
 *
 * 03-06-2007, Alan Poon  First version
 *
 */
// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEGioveDetectorMessenger.hh"
#include "munichteststand/GEGioveDetector.hh" 

// --------------------------------------------------------------------------- 

GEGioveDetectorMessenger::GEGioveDetectorMessenger(GEGioveDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/giove/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for Giove detector"); 

fShieldingConfigCmd = new G4UIcmdWithAnInteger("/MG/geometry/giove/shieldingconfig", this);
fShieldingConfigCmd -> SetGuidance("Set the configuration model number");
fShieldingConfigCmd -> SetParameterName("ShieldingConfig", true);
fShieldingConfigCmd -> SetDefaultValue(0);

fNumberOfLayersCmd = new G4UIcmdWithAnInteger("/MG/geometry/giove/numberoflayers", this);
fNumberOfLayersCmd -> SetGuidance("Set the number of the shielding layers");
fNumberOfLayersCmd -> SetParameterName("NumberOfLayers", true);
fNumberOfLayersCmd -> SetDefaultValue(10);
		
fBoxHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/giove/boxheight", this);
fBoxHeightCmd->SetGuidance("Set the height of the innermost box");
fBoxHeightCmd->SetParameterName("BoxHeight", true);
fBoxHeightCmd->SetDefaultValue(25.0); 
fBoxHeightCmd-> SetDefaultUnit("cm");
  
fBoxWidthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/giove/boxwidth", this);
fBoxWidthCmd -> SetGuidance("Set the width of the innermost sample box");
fBoxWidthCmd -> SetParameterName("BoxWidth", true); 
fBoxWidthCmd -> SetDefaultValue(25.0); 
fBoxWidthCmd -> SetDefaultUnit("cm"); 

fLayerThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/giove/layerthickness", this);
fLayerThicknessCmd -> SetGuidance("Set the thickness of the shielding layers");
fLayerThicknessCmd -> SetParameterName("LayerThickness", true); 
fLayerThicknessCmd -> SetDefaultValue(5.0); 
fLayerThicknessCmd -> SetDefaultUnit("cm"); 
	
}

// --------------------------------------------------------------------------- 

GEGioveDetectorMessenger::~GEGioveDetectorMessenger()
{

  delete fDetectorDir;
  delete fShieldingConfigCmd;
  delete fNumberOfLayersCmd;
  delete fBoxHeightCmd;
  delete fBoxWidthCmd;
  delete fLayerThicknessCmd;
  
}

// --------------------------------------------------------------------------- 

void GEGioveDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
  if (cmd == fShieldingConfigCmd)
    {
      fDetectorPointer->SetShieldingConfig(fShieldingConfigCmd->GetNewIntValue(newvalue));
    }
  else if (cmd == fNumberOfLayersCmd)
    {
      fDetectorPointer->SetNumberOfLayers(fNumberOfLayersCmd->GetNewIntValue(newvalue));
    }
  else if (cmd == fBoxHeightCmd)
    {
      fDetectorPointer->SetBoxHeight(fBoxHeightCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fBoxWidthCmd)
    {
      fDetectorPointer->SetBoxWidth(fBoxWidthCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fLayerThicknessCmd)
    {
       fDetectorPointer->SetLayerThickness(fLayerThicknessCmd->GetNewDoubleValue(newvalue));
    }
}

// --------------------------------------------------------------------------- 
