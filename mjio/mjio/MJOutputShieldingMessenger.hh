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
 * $Id: MJOutputShieldingMessenger.hh,v 1.1 2005-12-28 04:48:58 kareem Exp $
 *      
 * CLASS DECLARATION:  MJOutputShieldingMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger class for interacting with the shielding output class. Used to
 * set the name of the volume of interest.
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
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Dec 27 15:30:06 PST 2005
 * 
 * REVISION:
 * 
 * 12-27-2005, Initial submission, Kareem
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTSHIELDINGMESSENGER_HH
#define _MJOUTPUTSHIELDINGMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MJOutputShielding;
class G4UIdirectory;
class G4UIcmdWithAString;

//---------------------------------------------------------------------------//

class MJOutputShieldingMessenger : public G4UImessenger
{
public:

  //default constructor
  MJOutputShieldingMessenger( MJOutputShielding* );

  //destructor
  ~MJOutputShieldingMessenger();

  //public interface
  void SetNewValue( G4UIcommand*, G4String );

  //protected members
protected:

  //private  members
private:
  G4UIdirectory			*fOutputShieldingDirectory;
  MJOutputShielding		*fOutputShielding; 
  G4UIcmdWithAString	*fSetVolumeNameCmd;

};
#endif
