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
 * $Id: MGOutputG4StepsMessenger.hh,v 1.6 2009-06-23 17:05:06 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MGOutputG4StepsMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for MGOutputG4Steps class.
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
 * FIRST SUBMISSION: Wed Dec 28 11:02:05 PST 2005
 * 
 * REVISION:
 *
 * 03-18-2009, Added setWriteSensitiveStepsOnly command, A. Schubert
 * 02-22-2007, Added killAlpha and killBeta commands, A. Schubert 
 * 08-15-2006, Added stopNuclei, killAlpha and killBeta commands. R. Henning
 * 12-28-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGOUTPUTG4STEPSMESSENGER_HH
#define _MGOUTPUTG4STEPSMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"
#include "io/MGOutputG4Steps.hh"

//---------------------------------------------------------------------------//

class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;
class G4UIdirectory;

class MGOutputG4StepsMessenger : public G4UImessenger
{
public:

  MGOutputG4StepsMessenger(MGOutputG4Steps *g4steps);
  ~MGOutputG4StepsMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);

private:

  // Instance of MGOutputG4Steps to be controlled by this messenger
  MGOutputG4Steps     *fG4Steps;
  G4UIdirectory       *fDirectory;

  // /MG/io/G4Steps/setWriteSensitiveOnly
  G4UIcmdWithABool   *fSetWriteSensitiveOnlyCmd;

  // /MG/io/G4Steps/setWriteSensitiveStepsOnly
  G4UIcmdWithABool   *fSetWriteSensitiveStepsOnlyCmd;

  // /MG/io/G4Steps/setWriteSensitiveStepsOnly
  G4UIcmdWithABool   *fSetWriteTrackPreStepCmd;

  // /MG/io/G4Steps/killAlphas
  G4UIcmdWithABool   *fKillAlphasCmd;

  // /MG/io/G4Steps/killBetas
  G4UIcmdWithABool   *fKillBetasCmd;

  // /MG/io/G4Steps/killGammas
  G4UIcmdWithABool   *fKillGammasCmd;

  // /MG/io/G4Steps/killGammas
  G4UIcmdWithABool   *fKillNeutronsCmd;

  // /MG/io/G4Steps/stopNuclei
  G4UIcmdWithABool   *fStopNucleiCmd;

  G4UIcmdWithABool           *fSetUseTimeWindowCmd;
  G4UIcmdWithADoubleAndUnit  *fSetTimeWindowCmd;

};
#endif
