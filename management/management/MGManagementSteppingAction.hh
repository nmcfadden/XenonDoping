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
 * $Id: MGManagementSteppingAction.hh,v 1.1 2004-12-08 07:55:57 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGManagementSteppingAction.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Main stepping action class.
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu May 27 11:12:25 PDT 2004
 * 
 * REVISION:
 * 
 * 05-27-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGEMENTSTEPPINGACTION_HH
#define _MGMANAGEMENTSTEPPINGACTION_HH

//---------------------------------------------------------------------------//

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"

//---------------------------------------------------------------------------//

class MGManagementEventAction;

class MGManagementSteppingAction : public G4UserSteppingAction
{
public:

  //default constructor
  MGManagementSteppingAction(MGManagementEventAction *eventaction);

  //copy constructor
  MGManagementSteppingAction(const MGManagementSteppingAction &);

  //destructor
  ~MGManagementSteppingAction();

  //public interface

  void UserSteppingAction(const G4Step *step);

  //protected members
protected:


  //private  members
private:
  MGManagementEventAction  *fEventAction;

};
#endif
