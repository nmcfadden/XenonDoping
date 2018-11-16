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
 * $Id: 
 *      
 * CLASS DECLARATION:  MJGeometryCERN_NA55Messenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *  Messenger for CERN NA55 simulation geometry.
 *
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
 * AUTHOR: mgmarino 
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 
 *  */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYCERN_NA55MESSENGER_HH
#define _MJGEOMETRYCERN_NA55MESSENGER_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometryCERN_NA55;
class G4UIdirectory;
class G4UIcmdWithAString;

//---------------------------------------------------------------------------//

class MJGeometryCERN_NA55Messenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryCERN_NA55Messenger(MJGeometryCERN_NA55*);

  //destructor
  ~MJGeometryCERN_NA55Messenger();

   void SetNewValue(G4UIcommand*, G4String);

  //private  members
protected:
  G4UIdirectory* fCERN_NA55Directory;
  MJGeometryCERN_NA55* fCERN_NA55; 
  G4UIcmdWithAString* fSetBeamDumpTypeCmd;

};
#endif
