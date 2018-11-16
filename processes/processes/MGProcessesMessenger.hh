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
 * $Id: MGProcessesMessenger.hh,v 1.6 2009-01-17 15:14:12 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGProcessesMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *  This is the messenger class for the physics processes. It provides an
 *  interface to the GEANT4 command prompt to set the interaction realm of the
 *  simulation: BB events (i.e., tens to hundreds of keV) or DM events (i.e.,
 *  ones of keV).
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *  The default is for the BB realm, as most stuff we have to deal with is in
 *  those energy levels.
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Sep 14 15:03:59 PDT 2004
 * 
 * REVISION:
 * 
 * 09-14-2004, Kareem
 *			   Initial file sumbission
 * 02-14-2005, Luciano     Added command
 * 02-16-2005, Luciano     Added command
 * 07-12-2005, Luciano     Added command to switch on QGSP_GN_HP_BIC_ISO alternative
 * 17-04-2006, M. Marino  Added a number of commands to implement QGSC, Bertini
 *                         Cascade, and MuonNucleusInteraction
 *                         physics list for neutrons, protons and muons
 * 24-03-2014, Luciano     Added command
 * 23-08-2017, Ralph Massarczyk     Added command
 * 25-06-2018, Ralph Massarczyk     Added different physics list options, when lowenergy flag enabled
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSESMESSENGER_HH
#define _MGPROCESSESMESSENGER_HH

//---------------------------------------------------------------------------//

//  GEANT4 headers
#include "globals.hh"
#include "G4UImessenger.hh"

class MGProcessesList;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class MGUIcmdStepLimit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;

//---------------------------------------------------------------------------//

class MGProcessesMessenger : public G4UImessenger
{
public:

  //default constructor
  MGProcessesMessenger( MGProcessesList* );

  //copy constructor
  //MGProcessesMessenger(const MGProcessesMessenger &);

  //destructor
  ~MGProcessesMessenger();

  //public interface
  void SetNewValue( G4UIcommand*, G4String );

  //protected members
protected:


  //private  members
private:
  MGProcessesList *theList;
  
  G4UIdirectory *MGProcessesDir;
  G4UIcmdWithAString *MGProcessesChoiceCommand;
  G4UIcmdWithABool* opticalProcessesCmd;
  G4UIcmdWithABool* opticalOnlyCmd;
  G4UIcmdWithABool* lowEnergyProcessesCmd; 
	G4UIcmdWithAnInteger* lowEnergyProcessesOptionCmd; 

  G4UIcmdWithABool* fUseBertiniCascadeCmd;
  G4UIcmdWithABool* fUseFTFCmd;
  G4UIcmdWithABool* fUseAllHPCmd;
  G4UIcmdWithABool* fUseNoHadPhysicsCmd;
  G4UIcmdWithoutParameter* fGetStepLimitCommand;
  MGUIcmdStepLimit* fStepLimitCommand;
  G4UIcmdWithAnInteger* fAngCorrCommand;

};
#endif

