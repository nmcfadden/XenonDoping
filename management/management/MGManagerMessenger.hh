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
 * $Id: MGManagerMessenger.hh,v 1.5 2007-08-17 21:41:44 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGManagerMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger class for manager. Handles all "management" realted message and
 * messages that do not fit into specifc categories.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Jul  6 17:37:42 PDT 2004
 * 
 * REVISION:
 * 
 * 07-06-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGERMESSENGER_HH
#define _MGMANAGERMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"
#include "management/MGManager.hh"

//---------------------------------------------------------------------------//

class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcommand;
class G4UIdirectory;

//---------------------------------------------------------------------------//

class MGManagerMessenger: public G4UImessenger 
{
public:

  //default constructor
  MGManagerMessenger(MGManager *manager);

  //copy constructor
  MGManagerMessenger(const MGManagerMessenger &);

  //destructor
  ~MGManagerMessenger();

  //public interface
  G4String GetCurrentValue(G4UIcommand *command);
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:


  //private  members
private:
  //MGManager            *fManager;        // /MG/manager
  G4UIdirectory        *fDirectory;

  // Commands
  G4UIcmdWithAString      *fMGLogCmd;             // /MG/manager/mglog
  G4UIcmdWithAString      *fUseRandomEngine;      // /MG/manager/useRandomEngine
  G4UIcmdWithAnInteger    *fHEPRandomSeedCmd;     // /MG/manager/heprandomseed
  G4UIcmdWithAnInteger    *fUseInternalSeedCmd;   // /MG/manager/useInternalSeed
  G4UIcmdWithoutParameter *fSeedWithDevRandomCmd; // /MG/manager/seedWithDevRandom
  G4UIcmdWithoutParameter *fSeedWithUUIDCmd;      // /MG/manager/seedWithUUID
};
#endif
