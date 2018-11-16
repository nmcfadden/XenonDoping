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
 * $Id: MJOutputMCEventMessenger.hh,v 1.2 2006-08-28 22:40:51 rojo Exp $
 *      
 * CLASS DECLARATION:  MJOutputMCEventMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger class for MJOutputMCEvent.
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
 * FIRST SUBMISSION: Wed Sep 14 10:25:02 PDT 2005
 * 
 * REVISION:
 * 
 * 09-14-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTMCEVENTMESSENGER_HH
#define _MJOUTPUTMCEVENTMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJOutputMCEvent;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;

class MJOutputMCEventMessenger : public G4UImessenger
{
public:

  MJOutputMCEventMessenger(MJOutputMCEvent *);

  virtual ~MJOutputMCEventMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nV);

private:
  G4UIdirectory       *fDirectory;
  MJOutputMCEvent     *fOutputMCEvent;

  // Commands
  G4UIcmdWithABool           *fSetG4StepsOutputCmd;
  G4UIcmdWithABool           *fSetUseTimeWindowCmd;
  G4UIcmdWithADoubleAndUnit  *fSetTimeWindowCmd;

};

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
#endif
