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
// CLASS IMPLEMENTATION:  GEOutputHADESBEGeTestsMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: B. Lehnert
 * CONTACT: 
 * FIRST SUBMISSION: 21-21-2011
 * 
 * REVISION: MM-DD-YYYY
 *
 * 12-21-2009, Derived from GEOutputDarioMessenger.cc,  B.Lehnert
 *
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

#include "gerdaio/GEOutputHADESBEGeTests.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "gerdaio/GEOutputHADESBEGeTestsMessenger.hh"

//---------------------------------------------------------------------------//


GEOutputHADESBEGeTestsMessenger::GEOutputHADESBEGeTestsMessenger(GEOutputHADESBEGeTests *theOutput):
  fOutputScheme(theOutput)
{
  fDirectory = new G4UIdirectory("/MG/output/");
  fDirectory->SetGuidance("Output control for GEOutputHADESBEGeTests.");


  // /MG/output/writeHitCoordinates
  fWriteHitCoordinatesCmd = new G4UIcmdWithABool("/MG/output/WriteHitCoordinates",this);
  fWriteHitCoordinatesCmd->SetGuidance("Set the boolean flag to write hits coordinates ");
  fWriteHitCoordinatesCmd->SetGuidance("(default = false)");

  // /MG/output/writeDLInformation
  fWriteDLInformationCmd = new G4UIcmdWithABool("/MG/output/WriteDLInformation",this);
  fWriteDLInformationCmd->SetGuidance("Set the boolean flag to write distances to surfaces ");
  fWriteDLInformationCmd->SetGuidance("(default = false)");

  // /MG/output/writeDLInformation
  fWritePPInformationCmd = new G4UIcmdWithABool("/MG/output/WritePPInformation",this);
  fWritePPInformationCmd->SetGuidance("Set the boolean flag to write pair production flag (e+ in event)");
  fWritePPInformationCmd->SetGuidance("(default = false)");

  // /MG/output/writeDLInformation
  fWritePVPInformationCmd = new G4UIcmdWithABool("/MG/output/WritePVPInformation",this);
  fWritePVPInformationCmd->SetGuidance("Set the boolean flag to write primary vertex position flag");
  fWritePVPInformationCmd->SetGuidance("(default = false)");

}

//---------------------------------------------------------------------------//

GEOutputHADESBEGeTestsMessenger::~GEOutputHADESBEGeTestsMessenger()
{
  delete fWriteHitCoordinatesCmd;
  delete fWriteDLInformationCmd;
  delete fWritePPInformationCmd;
  delete fWritePVPInformationCmd;

}

//---------------------------------------------------------------------------//

void GEOutputHADESBEGeTestsMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  if (command == fWriteHitCoordinatesCmd)
    {
      fOutputScheme->SetWriteHitCoordinates(fWriteHitCoordinatesCmd->GetNewBoolValue(newValues));
      if (fOutputScheme->GetWriteHitCoordinates())
	  MGLog(trace) << "Do write hits coordinates" << endlog;
      else
	  MGLog(trace) << "Do not write hits coordinates" << endlog;
    }
  else if (command == fWriteDLInformationCmd)
    {
      fOutputScheme->SetWriteDLInformation(fWriteDLInformationCmd->GetNewBoolValue(newValues));
      if (fOutputScheme->GetWriteDLInformation())
	  MGLog(trace) << "Do write distance to surfaces" << endlog;
      else
	  MGLog(trace) << "Do not write distance to surfaces" << endlog;
    }
  else if (command == fWritePPInformationCmd)
    {
      fOutputScheme->SetWritePPInformation(fWritePPInformationCmd->GetNewBoolValue(newValues));
      if (fOutputScheme->GetWritePPInformation())
	  MGLog(trace) << "Do write pair production flag" << endlog;
      else
	  MGLog(trace) << "Do not write pair production flag" << endlog;
    }
  else if (command == fWritePVPInformationCmd)
    {
      fOutputScheme->SetWritePVPInformation(fWritePVPInformationCmd->GetNewBoolValue(newValues));
      if (fOutputScheme->GetWritePVPInformation())
	  MGLog(trace) << "Do write primary vertex positions" << endlog;
      else
	  MGLog(trace) << "Do not write primary vertex positions" << endlog;
    }
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
