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
 * $Id: MJDemoCalibrationTrackMessenger.hh,v 1.1 2016/02/24 17:30:00 tcald Exp $
 *      
 * CLASS DECLARATION:  MJDemoCalibrationTrackMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for the calibration track
 *
 * --------------------------------------------------------------------------//
 * AUTHOR: T. Caldwell
 * CONTACT: tcald@unc.edu
 * FIRST SUBMISSION: Feb 24, 2016
 * 
 * REVISION:
 * 
 * 02-24-2016, Created, T. Caldwell
 */
// --------------------------------------------------------------------------//


#ifndef _MGDEMOCALIBRATIONTRACKMESSENGER_HH
#define _MGDEMOCALIBRATIONTRACKMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "mjdemonstrator/MJDemoCalibrationTrack.hh"

class MJDemoCalibrationTrack;
class G4UIdirectory;
class G4UIcmdWithInt;
class G4UIcmdWithADoubleAndUnit;

//---------------------------------------------------------------------------//

class MJDemoCalibrationTrackMessenger : public G4UImessenger
{
public:

  MJDemoCalibrationTrackMessenger(G4String index);
  ~MJDemoCalibrationTrackMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nv);
  void SetValues(MJDemoCalibrationTrack* calTrack);

protected:

private:
  G4UIdirectory* fCalTrackDir;
  MJDemoCalibrationTrack* fCalTrack;
  G4UIcmdWithADoubleAndUnit* fHelixRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeInnerRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fStartAngleCmd;
  G4UIcmdWithADoubleAndUnit* fTotalAngleCmd;
  G4UIcmdWithADoubleAndUnit* fHelixAngleCmd;
  G4UIcmdWithADoubleAndUnit* fAngleBufferCmd;
  G4UIcmdWithAnInteger* fNsegmentsCmd;
};
#endif
