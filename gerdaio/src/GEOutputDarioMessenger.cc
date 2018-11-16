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
// CLASS IMPLEMENTATION:  GEOutputDarioMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: L. Pandola
 * CONTACT: 
 * FIRST SUBMISSION: May 14 2009
 * 
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//

#include "globals.hh"        
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"   
#include "G4UIcmdWithoutParameter.hh"   
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"

#include "gerdaio/GEOutputDario.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "gerdaio/GEOutputDarioMessenger.hh"

//---------------------------------------------------------------------------//


GEOutputDarioMessenger::GEOutputDarioMessenger(GEOutputDario *theOutput):
  fOutputScheme(theOutput)
{
  fDirectory = new G4UIdirectory("/MG/output/");
  fDirectory->SetGuidance("Output control for GEOutputDario.");

  // /MG/output/writeHitDetails
  fWriteHitDetails = new  G4UIcmdWithABool("/MG/output/writeHitDetails",this);
  fWriteHitDetails->SetGuidance("Set boolean flag to switch on/off the code ");
  fWriteHitDetails->SetGuidance("which dumps on a file the details step-by-step");
  fWriteHitDetails->SetGuidance("(default = false");

  // /MG/output/setMajority
  fSetMajorityCmd =  new G4UIcmdWithAnInteger("/MG/output/setMajority",this);
  fSetMajorityCmd->SetGuidance("Set the minimum number of sensitive detector to be hit ");
  fSetMajorityCmd->SetGuidance("before writing an event in the output file");
  fSetMajorityCmd->SetGuidance("(default = 1)");

  // /MG/output/useLocalCoordinates
  fWriteLocalCoordinatesCmd = new G4UIcmdWithABool("/MG/output/useLocalCoordinates",this);
  fWriteLocalCoordinatesCmd->SetGuidance("Set the boolean flag to write the hits coordinates ");
  fWriteLocalCoordinatesCmd->SetGuidance("in the ASCII file in local (true) or global (false) ");
  fWriteLocalCoordinatesCmd->SetGuidance("reference frame");
  fWriteLocalCoordinatesCmd->SetGuidance("(default = true --> local coordinates)");
  
}

//---------------------------------------------------------------------------//

GEOutputDarioMessenger::~GEOutputDarioMessenger()
{
  delete fWriteHitDetails;
  delete fSetMajorityCmd;
  delete fWriteLocalCoordinatesCmd;
}

//---------------------------------------------------------------------------//

void GEOutputDarioMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  if(command == fWriteHitDetails)
    {
      fOutputScheme->SetWriteDetailsFlag(fWriteHitDetails->GetNewBoolValue(newValues));
    if (fWriteHitDetails->GetNewBoolValue(newValues))
      MGLog(trace) << "Step-by-step info will be written " << endlog;
    else
      MGLog(trace) << "Step-by-step info will NOT be written " << endlog;
    }
  else if (command == fSetMajorityCmd)
    {
      fOutputScheme->SetMajority(fSetMajorityCmd->GetNewIntValue(newValues));
      MGLog(trace) << "Majority level set to: " << fOutputScheme->GetMajority() << endlog;
    }
  if (command == fWriteLocalCoordinatesCmd)
    {
      fOutputScheme->UseLocalCoordinates(fWriteLocalCoordinatesCmd->GetNewBoolValue(newValues));
      if (fWriteLocalCoordinatesCmd->GetNewBoolValue(newValues))
	MGLog(trace) << "Use local coordinate system in the ASCII output file " << endlog;
      else
	MGLog(trace) << "Use global coordinate system in the ASCII output file " << endlog;
    }
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
