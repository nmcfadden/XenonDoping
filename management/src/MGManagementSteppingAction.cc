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
// $Id: MGManagementSteppingAction.cc,v 1.3 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementSteppingAction.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu May 27 11:12:25 PDT 2004
 * 
 * REVISION:
 *
 * 05-27-2004, Created, R. Henning
 * 11-08-2004, Replaced OutputRoot with VOutputManager methods, Luciano
 * 11-24-2004, Added check for the existence of the fOutputManager
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "management/MGManagementEventAction.hh"

//---------------------------------------------------------------------------//

#include "management/MGManagementSteppingAction.hh"

//---------------------------------------------------------------------------//

MGManagementSteppingAction::MGManagementSteppingAction(MGManagementEventAction 
						       *eventaction):
  fEventAction(eventaction)
{;}

//---------------------------------------------------------------------------//

MGManagementSteppingAction::MGManagementSteppingAction(const MGManagementSteppingAction & other) : G4UserSteppingAction(other) 
{;}

//---------------------------------------------------------------------------//

MGManagementSteppingAction::~MGManagementSteppingAction()
{;}

//---------------------------------------------------------------------------//

void MGManagementSteppingAction::UserSteppingAction(const G4Step *step)
{
  if (fEventAction->GetOutputManager())
    fEventAction->GetOutputManager()->SteppingAction(step, fpSteppingManager);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
