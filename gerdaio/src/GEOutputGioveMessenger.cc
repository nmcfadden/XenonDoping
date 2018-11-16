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
// CLASS IMPLEMENTATION:  GEOutputGioveMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: A. Poon
 * CONTACT: 
 * FIRST SUBMISSION: June 24 2009
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

#include "gerdaio/GEOutputGiove.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "gerdaio/GEOutputGioveMessenger.hh"

//---------------------------------------------------------------------------//


GEOutputGioveMessenger::GEOutputGioveMessenger(GEOutputGiove *theOutput):
  fOutputScheme(theOutput)
{
  fDirectory = new G4UIdirectory("/MG/output/");
  fDirectory->SetGuidance("Output control for GEOutputGiove.");

  // /MG/output/writeHitDetails
  fWriteHitDetails = new  G4UIcmdWithABool("/MG/output/writeHitDetails",this);
  fWriteHitDetails->SetGuidance("Set boolean flag to switch on/off the code ");
  fWriteHitDetails->SetGuidance("which dumps on a file the details step-by-step");
  fWriteHitDetails->SetGuidance("(default = false");
}

//---------------------------------------------------------------------------//

GEOutputGioveMessenger::~GEOutputGioveMessenger()
{
  delete fWriteHitDetails;
}

//---------------------------------------------------------------------------//

void GEOutputGioveMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
  // /MG/output/killDaughter
  if(command == fWriteHitDetails)
    {
      fOutputScheme->SetWriteDetailsFlag(fWriteHitDetails->GetNewBoolValue(newValues));
    if (fWriteHitDetails->GetNewBoolValue(newValues))
      MGLog(trace) << "Step-by-step info will be written " << endlog;
    else
      MGLog(trace) << "Step-by-step info will NOT be written " << endlog;
  }
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
