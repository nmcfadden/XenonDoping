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
// $Id: MGManagementTrackingAction.cc,v 1.2 2007-03-07 10:22:47 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementTrackingAction.cc
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
 *  AUTHOR:
 *     Xiang Liu  <xliu@mppmu.mpg.de>
 *     Bernhard Schwingenheuer <B.Schwingenheuer@mpi-hd.mpg.de>
 *
 * FIRST SUBMISSION: 08-10-2004 
 * 
 * REVISION: 
 *   7-Mar-2007     Changed so that GETrajectory is called by the concrete output class 
 *                  and the code is executed only when required, Luciano
 *
 */
// --------------------------------------------------------------//
#include "management/MGManagementTrackingAction.hh"
#include "management/MGManagementEventAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"


MGManagementTrackingAction::MGManagementTrackingAction(MGManagementEventAction 
						       *eventaction):
  fEventAction(eventaction)
{;}

MGManagementTrackingAction::~MGManagementTrackingAction()
{;}

void MGManagementTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  if (fEventAction->GetOutputManager())
    {
      fEventAction->GetOutputManager()->PreUserTrackingAction(aTrack);
    }  
}

void MGManagementTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  if (fEventAction->GetOutputManager())
    {
      fEventAction->GetOutputManager()->PostUserTrackingAction(aTrack);
    }
}
