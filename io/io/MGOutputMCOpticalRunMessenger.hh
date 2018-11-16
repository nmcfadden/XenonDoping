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
 * AUTHOR: ASchubert
 * CONTACT: alexis3@u.washington.edu
 * FIRST SUBMISSION: Fri Jan 22 07:17:08 PST 2010
 * 
 * REVISION:
 *
 * 03-31-2010, Initial submission, Created based on G4Steps, A. Schubert
 * 07-27-2010, Removed decay chain stopping; this should be done with Geant4's 
 *             nucleus limits, A. Schubert
 * 09-28-2010, Added command to print surface areas, A. Schubert
 * 11-08-2010, Added MJDemonstrator active volume naming scheme
 * 12-01-2011, Added command to write all steps in events that deposit energy,
 *             A. Schubert
 * 03-16-2012, Added command to window importance-process tracks.  This should
 *             not be used together with time windowing -- A. Schubert
 */

#ifndef _MGOutputMCOpticalRunMessenger_HH
#define _MGOutputMCOpticalRunMessenger_HH

#include "globals.hh"
#include "G4UImessenger.hh"
#include "io/MGOutputMCOpticalRun.hh"


class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcommand;
class G4UIdirectory;

// Messenger for MGOutputMCOpticalRun 
class MGOutputMCOpticalRunMessenger : public G4UImessenger
{
public:

  MGOutputMCOpticalRunMessenger(MGOutputMCOpticalRun *g4steps);
  ~MGOutputMCOpticalRunMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);

private:

  // Instance of MGOutputMCOpticalRun to be controlled by this messenger
  MGOutputMCOpticalRun     *fMCOpticalRun;
  G4UIdirectory       *fDirectory;

  // /MG/io/MCOpticalRun/setRunID
  G4UIcmdWithAnInteger *fSetRunIDCmd;

  // /MG/io/MCOpticalRun/setNSteps
  G4UIcmdWithAnInteger *fSetNStepsCmd;

  // /MG/io/MCOpticalRun/setWriteAllSteps
  G4UIcmdWithABool   *fSetWriteAllStepsCmd;
  
  // /MG/io/MCOpticalRun/setWriteAllSensitiveSteps
  G4UIcmdWithABool  *fSetWriteAllSensitiveStepsCmd;

  // /MG/io/MCOpticalRun/setWriteAllStepsInEventsThatDepositEnergy
  G4UIcmdWithABool   *fSetWriteAllStepsInEventsThatDepositEnergyCmd;

  // /MG/io/MCOpticalRun/setPrintSurfaceAreas
  G4UIcmdWithABool   *fSetPrintSurfAreasCmd;

  // /MG/io/MCOpticalRun/killAlphas
  G4UIcmdWithABool   *fKillAlphasCmd;

  // /MG/io/MCOpticalRun/killBetas
  G4UIcmdWithABool   *fKillBetasCmd;

  // /MG/io/MCOpticalRun/killGammas
  G4UIcmdWithABool   *fKillGammasCmd;

  // /MG/io/MCOpticalRun/killGammas
  G4UIcmdWithABool   *fKillNeutronsCmd;

  // /MG/io/MCOpticalRun/stopNuclei
  G4UIcmdWithABool   *fStopNucleiCmd;
  
  // /MG/io/MCOpticalRun/KillAll
  G4UIcmdWithABool   *fKillAllCmd;

  // /MG/io/MCOpticalRun/useTimeWindow
  G4UIcmdWithABool *fSetUseTimeWindowCmd;

  // /MG/io/MCOpticalRun/setTimeWindow
  G4UIcmdWithADoubleAndUnit *fSetTimeWindowCmd;

  // /MG/io/MCOpticalRun/useImportanceSamplingWindow
  G4UIcmdWithABool *fUseImportanceSamplingWindowCmd;

  // /MG/io/MCOpticalRun/SetSensitiveIDLabelScheme [classic, labelID, askGeom, file, manual] (label, filename)
  G4UIcommand *fSetSensitiveIDLabelSchemeCmd;

  // /MG/io/MCOpticalRun/AddSensitiveVolnameID [volname] [ID]
  G4UIcommand *fAddSensitiveVolnameID;

};
#endif
