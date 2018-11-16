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
/**                                                            
 * $Id: MGManagementStackingAction.hh,v 1.1 2004-12-15 15:27:13 pandola Exp $
 *      
 * CLASS DECLARATION:  MGManagementStackingAction.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Main stacking action class.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: L. Pandola
 * 
 * REVISION:
 * 
 * 12-15-2004, Created, L. Pandola
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGEMENTSTACKINGACTION_HH
#define _MGMANAGEMENTSTACKINGACTION_HH

//---------------------------------------------------------------------------//

#include "G4Track.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
//---------------------------------------------------------------------------//

class MGManagementEventAction;

class MGManagementStackingAction : public G4UserStackingAction
{
public:

  //default constructor
  MGManagementStackingAction(MGManagementEventAction *eventaction);

  //copy constructor
  MGManagementStackingAction(const MGManagementStackingAction &);

  //destructor
  ~MGManagementStackingAction();

  //public interface
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  void NewStage();
  void PrepareNewEvent();

  //protected members
protected:


  //private  members
private:
  MGManagementEventAction  *fEventAction;

};
#endif
