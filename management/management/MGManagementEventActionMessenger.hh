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
 * $Id: MGManagementEventActionMessenger.hh,v 1.5 2008-04-08 15:15:46 jliu Exp $
 *      
 * CLASS DECLARATION:  MGManagementEventActionMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geant 4 messenger class for MGManagementEventAction
 * MG/management/eventaction
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 12 16:26:42 PDT 2004
 * 
 * REVISION:
 * 
 * 04-12-2004, Created, R. Henning
 * 04-07-2004, Added additional commands, R. Henning
 * 07-12-2004, Removed initialization commands, moved to BeginOfRunAction
 *             of MJManagementRunAction, R. Henning
 * 11-08-2004, The name of a few G4UIcommand were changed (no change in the 
 *             messenger commands)
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 06-08-2007, Added two commands for pre-waveform, Jing Liu
 * 26-11-2007, Added commands for class GEOutputCrystal, Jing Liu
 * 08-04-2008, Added TimeWindow commands for GEOutputCrystal, Jing Liu
 * 02-11-2011, Added TimeWindow commands for GEOutputTestStand, A. Volynets
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGEMENTEVENTACTIONMESSENGER_HH
#define _MGMANAGEMENTEVENTACTIONMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"
#include "management/MGManagementEventAction.hh"

//---------------------------------------------------------------------------//

class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithABool;
//---------------------------------------------------------------------------//

class MGManagementEventActionMessenger: public G4UImessenger 
{
public:

  //default constructor
  MGManagementEventActionMessenger(MGManagementEventAction *eventaction);

  //copy constructor
  MGManagementEventActionMessenger(const MGManagementEventActionMessenger &);

  //destructor
  ~MGManagementEventActionMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand *command);

  //protected members
protected:


  //private  members
private:

  /** Event action object to be controlled by user interface commands. */
  MGManagementEventAction *fEventAction;

  G4UIdirectory           *fEventDirectory;
  // /MG/management/eventaction

  G4UIdirectory           *fCrystalDirectory;
  // /MG/eventaction/Crystal

  G4UIdirectory           *fGerdaTestStandDirectory;
  // /MG/eventaction/GerdaTestStand

  // Commands
  // --------

  // /MG/eventaction/getoutputschema
   G4UIcmdWithoutParameter *fGetOutputSchemaCmd;

  // /MG/eventaction/rootfilename
  G4UIcmdWithAString      *fSetFileNameCmd;

  // /MG/eventaction/rootschema
  G4UIcmdWithAString      *fSetSchemaCmd;

  // /MG/eventaction/reportingfrequency
  G4UIcmdWithAnInteger    *fSetReportingFrequencyCmd;
  
  // /MG/eventaction/writeOutFrequency
  G4UIcmdWithAnInteger    *fSetWriteOutFrequencyCmd;

  // /MG/eventaction/writeOutFileDuringRun
  G4UIcmdWithABool        *fSetWriteOutFileDuringRunCmd;

  // /MG/eventaction/PreWaveformFormat
  G4UIcmdWithAString         *fSetPreWaveformFormatCmd;

  // /MG/eventaction/PreWaveformFileName
   G4UIcmdWithAString        *fSetPreWaveformFileNameCmd;


  // /MG/eventaction/GerdaTestStand/useTimeWindow
   G4UIcmdWithABool          *fGerdaTestStandUseTimeWindowCmd;

  // /MG/eventaction/GerdaTestStand/setTimeWindow
   G4UIcmdWithADoubleAndUnit *fGerdaTestStandSetTimeWindowCmd;


  // /MG/eventaction/Crystal/save
   G4UIcmdWithAString        *fCrystalSaveCmd;

  // /MG/eventaction/Crystal/useTimeWindow
   G4UIcmdWithABool          *fCrystalUseTimeWindowCmd;

  // /MG/eventaction/Crystal/setTimeWindow
   G4UIcmdWithADoubleAndUnit *fCrystalSetTimeWindowCmd;
};
#endif
