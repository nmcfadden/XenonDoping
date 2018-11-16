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
// $Id: MJOutputMCEvent.cc,v 1.9 2007-02-21 10:19:33 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputMCEvent.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sun Jan 30 10:12:46 PST 2005
 * 
 * REVISION:
 *
 * 01-30-2005, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"

#include "TTree.h"

#include "io/MGLogger.hh"
#include "io/MGOutputG4Steps.hh"
#include "mjio/MJOutputDetectorEvent.hh"
#include "mjio/MJOutputMCEventMessenger.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputMCEvent.hh"

//---------------------------------------------------------------------------//

MJOutputMCEvent::MJOutputMCEvent(G4bool isMother):
  MGOutputRoot(isMother), fFillG4StepsTree(true)
{  
  fMessenger = new MJOutputMCEventMessenger(this);
  fDetectorEvent = new MJOutputDetectorEvent(this, false);
  fG4Steps = new MGOutputG4Steps(false);
  if(!fDetectorEvent || !fG4Steps) {
    MGLog(error) << "Memory allocation error!" << endlog;
    MGLog(fatal) << endlog;
  }
}

//---------------------------------------------------------------------------//

MJOutputMCEvent::~MJOutputMCEvent()
{
  delete fDetectorEvent;
  delete fG4Steps;
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::BeginOfEventAction(const G4Event *event)
{
  fG4Steps->BeginOfEventAction(event);
  fDetectorEvent->BeginOfEventAction(event);
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::BeginOfRunAction()
{
  DefineSchema();
  fG4Steps->SetUseTimeWindow(fUseTimeWindow);
  fG4Steps->SetTimeWindow(fTimeWindow);
  fDetectorEvent->SetUseTimeWindow(fUseTimeWindow);
  fDetectorEvent->SetTimeWindow(fTimeWindow); 
  fG4Steps->BeginOfRunAction();
  fDetectorEvent->BeginOfRunAction();
  if(IsMother()) OpenFile();
  fG4Steps->GetTree()->SetDirectory(GetRootFile());
  fDetectorEvent->GetSpecialTree()->SetMaxTreeSize(1000000000);
  fG4Steps->GetTree()->SetMaxTreeSize(1000000000);
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::DefineSchema()
{
  if(!SchemaDefined()) {
    // Create output Tree if it has not been assigned.
    if(!fTree) {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      fTree = new TTree("fTree", "MC Event Output tree");
      if(!fTree) {
        MGLog(error) << "Could not allocate memory for tree!" << endlog;
        MGLog(fatal) << endlog;
      }
    }
    SetSchemaDefined(true);
    MGLog(trace) << "G4Step generic output schema defined." << endlog; 
  } else 
    MGLog(warning) << "Schema already defined." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::EndOfEventAction(const G4Event *event)
{
  fG4Steps->EndOfEventAction(event);
  if(fFillG4StepsTree)
    fG4Steps->FillTree(); 
  fDetectorEvent->EndOfEventAction(event); // Will fill tree, if required.
  fOffsetTime = 0.0;
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::EndOfRunAction()
{
  fG4Steps->EndOfRunAction();
  fDetectorEvent->EndOfRunAction();
  if(IsMother()) {
    SetRootFile(fG4Steps->GetTree()->GetCurrentFile());
    CloseFile();
    SetSchemaDefined(false);
    fG4Steps->SetSchemaDefined(false);
  }
}

//---------------------------------------------------------------------------//

void MJOutputMCEvent::RootSteppingAction(const G4Step* step)
{
  fG4Steps->RootSteppingAction(step);
  fDetectorEvent->RootSteppingAction(step);
}

//---------------------------------------------------------------------------// 

void MJOutputMCEvent::WritePartialEvent(const G4Event* event)
{
  //MCEvent handles the time increment, so just update the offset time of
  //DetectorEvent and G4Steps
  fDetectorEvent->SetOffsetTime(fOffsetTime);
  fG4Steps->SetOffsetTime(fOffsetTime);
  //Because G4Steps->IsMother() is false, WritePartialEvent() doesn't actually
  //write the partial event, so MCEvent fills the tree manually.
  fDetectorEvent->WritePartialEvent(event);
  fG4Steps->WritePartialEvent(event);
  if(fFillG4StepsTree) fG4Steps->FillTree(); 
}

void MJOutputMCEvent::ResetPartialEvent(const G4Event* event)
{
  fDetectorEvent->ResetPartialEvent(event);
  fG4Steps->ResetPartialEvent(event);
}

//---------------------------------------------------------------------------//  
//---------------------------------------------------------------------------//
