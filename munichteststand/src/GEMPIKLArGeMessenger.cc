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

/**
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 05-03-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"

#include "munichteststand/GEMPIKLArGeMessenger.hh"
#include "munichteststand/GEMPIKLArGe.hh" 
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 

GEMPIKLArGeMessenger::GEMPIKLArGeMessenger(GEMPIKLArGe *teststand) : 
  fDetector(teststand) 
{

  // directories 
  
  fMPIKLArGeDirectory =  new G4UIdirectory("/MG/geometry/large/"); 
  fMPIKLArGeDirectory->SetGuidance("Tune teststand parameters for LArGe test stand"); 

  
  fWLSYield =  new G4UIcmdWithADouble("/MG/geometry/large/WLS_yield", this); 
  fWLSYield->SetGuidance("Yield for wavelenght shifting in UV by VM2000");
  fWLSYield->SetGuidance("Default value: 0.6");

  fCrystalInCmd = new G4UIcmdWithABool("/MG/geometry/large/crystal",this);
  fCrystalInCmd->SetGuidance("If set to false the crystal is not inserted ");
  fCrystalInCmd->SetGuidance("in the dewar");

}

// --------------------------------------------------------------------------- 

GEMPIKLArGeMessenger::~GEMPIKLArGeMessenger()
{

  delete fMPIKLArGeDirectory; 
  delete fWLSYield;
  delete fCrystalInCmd;
}

// --------------------------------------------------------------------------- 

void GEMPIKLArGeMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{

  // directory: /MG/geometry/teststand/

  if (cmd == fWLSYield)
    {
      G4double h = fWLSYield->GetNewDoubleValue(newvalue);
      if (h < 0 || h > 1) 
	{
	  MGLog(warning) << "Efficiency cannot be negative or larger than 100%" << endlog;
	}
      else
	{
	  fDetector->SetWLSYield(h);
	  MGLog(trace) << "WLS efficiency set to " << h << endlog;
	}
    }
  else if (cmd == fCrystalInCmd)
    {
      fDetector->SetCrystalIn(fCrystalInCmd->GetNewBoolValue(newvalue));
    }
}

// --------------------------------------------------------------------------- 
