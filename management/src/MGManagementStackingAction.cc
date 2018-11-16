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
// $Id: MGManagementStackingAction.cc,v 1.4 2007-02-21 16:30:56 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementStackingAction.cc
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
 * AUTHOR: L. Pandola
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 12-15-2004, Created, R. Pandola
 */
//---------------------------------------------------------------------------//
//

#include "management/MGManagementEventAction.hh"

//---------------------------------------------------------------------------//

#include "management/MGManagementStackingAction.hh"

//---------------------------------------------------------------------------//

MGManagementStackingAction::MGManagementStackingAction(MGManagementEventAction 
						       *eventaction):
  fEventAction(eventaction)
{;}

//---------------------------------------------------------------------------//

MGManagementStackingAction::MGManagementStackingAction(const MGManagementStackingAction & other) : G4UserStackingAction(other)
{;}

//---------------------------------------------------------------------------//

MGManagementStackingAction::~MGManagementStackingAction()
{;}

//---------------------------------------------------------------------------//

G4ClassificationOfNewTrack MGManagementStackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  if (fEventAction->GetOutputManager())
    {
      return fEventAction->GetOutputManager()->StackingAction(aTrack);
    }
  else 
    {
      return fUrgent; 
    }
}

//---------------------------------------------------------------------------//
void MGManagementStackingAction::NewStage()
{
  if (fEventAction->GetOutputManager())
    {
      fEventAction->GetOutputManager()->NewStage();
    }
}

//---------------------------------------------------------------------------//

void MGManagementStackingAction::PrepareNewEvent()
{
  if (fEventAction->GetOutputManager())
    {
      fEventAction->GetOutputManager()->PrepareNewEvent();
    }
}

//---------------------------------------------------------------------------//
