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
 * $Id: MGManagementRunAction.hh,v 1.3 2005-01-27 20:56:24 kareem Exp $
 *      
 * CLASS DECLARATION:  MGManagementRunAction.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Executes all the BeginOfRunAction() and EndOfRunAction() methods of 
 * management classes.
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
 * FIRST SUBMISSION: Mon Jul 12 11:49:43 PDT 2004
 * 
 * REVISION:
 * 
 * 07-12-2004, Created, R. Henning
 * 07-19-2004, Modified to support singlet MJManager accessed via static 
 *             method, R. Henning
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 01-27-2005, Added variable to keep track of whether the randomization seed
 *             is automatically set in the macro file, Kareem
 * 01-27-2005, Tracks the time a run started for the purposes of benchmarking,
 *             Kareem
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGEMENTRUNACTION_HH
#define _MGMANAGEMENTRUNACTION_HH

//---------------------------------------------------------------------------//

class G4Run;

class MGManager;

//---------------------------------------------------------------------------//

#include "globals.hh"
#include <ctime>

//---------------------------------------------------------------------------//

#include "G4UserRunAction.hh"

//---------------------------------------------------------------------------//

class MGManagementRunAction : public G4UserRunAction
{
public:

  //default constructor
  MGManagementRunAction();

  //copy constructor
  MGManagementRunAction(const MGManagementRunAction &);

  //destructor
  ~MGManagementRunAction();

  //public interface
  void BeginOfRunAction(const G4Run *run);
  void EndOfRunAction(const G4Run *run);
  
  void SetControlledRandomization() { fControlledRandomization = true; }
  G4bool GetControlledRandomization() { return fControlledRandomization; }
  time_t GetStartTime() { return fStartTime; }

  //protected members
protected:


  //private  members
private:
  G4bool fControlledRandomization;
  time_t fStartTime;

};
#endif
