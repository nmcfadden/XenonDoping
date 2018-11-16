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
// $Id: MGManagementTrackingAction.hh,v 1.2 2007-03-07 10:21:51 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementTrackingAction.hh
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
 *  7-Mar-2007     Changed so that GETrajectory is called by the concrete output class 
 *                  and the code is executed only when required, Luciano
 */
// --------------------------------------------------------------//
#ifndef MGManagementTrackingAction_h
#define MGManagementTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"

class MGManagementEventAction;

class MGManagementTrackingAction : public G4UserTrackingAction
{
 public:
  MGManagementTrackingAction(MGManagementEventAction*);
  virtual ~MGManagementTrackingAction();
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
  G4TrackingManager* GetTrackingManager() {return fpTrackingManager;};

 private:
  MGManagementEventAction* fEventAction;
};

#endif

