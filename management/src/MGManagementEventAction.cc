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
// $Id: MGManagementEventAction.cc,v 1.6 2007-06-08 12:54:09 jliu Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementEventAction.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 12 14:59:40 PDT 2004
 * 
 * REVISION:
 *
 * 08-08-2004, Added Reporting Frequency support, R. Henning
 * 04-12-2004, Initialized, R. Henning
 * 11-08-2004, Replaced OutputRoot with VOutputManager, Luciano
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 01-27-2005, Reporting now includes real time since start of run, Kareem
 * 15-03-2007, Added time output for the MGLog
 * 06-08-2007, Added some codes handling pre-waveform output, Jing Liu
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"

#include "management/MGManagementEventActionMessenger.hh"
#include "io/MGLogger.hh"
#include "io/MGVOutputManager.hh"
#include "management/MGManager.hh"
#include "management/MGManagementRunAction.hh"

#include <ctime>

//---------------------------------------------------------------------------//

#include "management/MGManagementEventAction.hh"

//---------------------------------------------------------------------------//

MGManagementEventAction::MGManagementEventAction():
  fOutputManager(0), fOutputManagerPreWF(0), 
  fReportingFrequency(100000), fWriteOutFrequency(0), 
  fWriteOutFileDuringRun(false)
{
  fG4Messenger = new MGManagementEventActionMessenger(this);
}

//---------------------------------------------------------------------------//

MGManagementEventAction::MGManagementEventAction(const MGManagementEventAction & other) : G4UserEventAction(other)
{;}

//---------------------------------------------------------------------------//

MGManagementEventAction::~MGManagementEventAction()
{
  if(fOutputManager) {
    fOutputManager->CloseFile();
    delete fOutputManager;
  }
  if(fOutputManagerPreWF) {
    fOutputManagerPreWF->CloseFile();
    delete fOutputManagerPreWF;
  }
  delete fG4Messenger;
}

//---------------------------------------------------------------------------//

void MGManagementEventAction::BeginOfEventAction(const G4Event *event)
{
  if(event->GetEventID()%fReportingFrequency == 0)
    {
      MGManager* manager = MGManager::GetMGManager();
      time_t endTime = time( NULL );
      G4double timeDifference = difftime( endTime,
					  manager->GetMGRunAction()->GetStartTime()
					  );
      tm *z = localtime(&endTime);
      
      MGLog(routine) << " Processing Event # " << event->GetEventID() 
		     << " at " << timeDifference << " seconds (real time), at the moment it is: " << z->tm_mon+1 << "." 
		     << z->tm_mday << "." << z->tm_year+1900 << ", " << z->tm_hour << ":" << z->tm_min << ":" << z->tm_sec 
		     << endlog;
      
      
    }
  if(fOutputManager)
    fOutputManager->BeginOfEventAction(event);
  if(fOutputManagerPreWF)
    fOutputManagerPreWF->BeginOfEventAction(event);
}

//---------------------------------------------------------------------------//

void MGManagementEventAction::EndOfEventAction(const G4Event *event)
{
  if(fWriteOutFileDuringRun && fOutputManager) { 
    if(fWriteOutFrequency <= 0) fWriteOutFrequency = fReportingFrequency;
    if(event->GetEventID()%fWriteOutFrequency == 0) fOutputManager->WriteFile();
  }
  if(fOutputManager) 
    fOutputManager->EndOfEventAction(event);

  if(fWriteOutFileDuringRun && fOutputManagerPreWF) { 
    if(fWriteOutFrequency <= 0) fWriteOutFrequency = fReportingFrequency;
    if(event->GetEventID()%fWriteOutFrequency == 0) fOutputManagerPreWF->WriteFile();
  }
  if(fOutputManagerPreWF) 
    fOutputManagerPreWF->EndOfEventAction(event);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
