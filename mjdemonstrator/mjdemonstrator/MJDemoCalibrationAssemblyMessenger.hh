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
 * $Id: MJDemoCalibrationAssemblyMessenger.hh,v 1.1 2016/02/24 17:30:00 tcald Exp $
 *      
 * CLASS DECLARATION:  MJDemoCalibrationAssemblyMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for the calibration assembly
 *
 *--------------------------------------------------------------------------//
 * AUTHOR: T. Caldwell
 * CONTACT: tcald@unc.edu
 * FIRST SUBMISSION: Feb 24, 2016
 * 
 * REVISION:
 * 
 * 02-24-2016, Created, T. Caldwell
 */
// --------------------------------------------------------------------------//

#ifndef _MGDEMOCALIBRATIONASSEMBLYMESSENGER_HH
#define _MGDEMOCALIBRATIONASSEMBLYMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "mjdemonstrator/MJDemoCalibrationAssembly.hh"
#include "mjdemonstrator/MJDemoCalibrationSourceMessenger.hh"
#include "mjdemonstrator/MJDemoCalibrationTrackMessenger.hh"

class MJDemoCalibrationAssembly;

class G4UIdirectory;
class G4UIcmdWithInt;
class G4UIcmdWithADoubleAndUnit;

//---------------------------------------------------------------------------//

class MJDemoCalibrationAssemblyMessenger : public G4UImessenger
{
public:

  MJDemoCalibrationAssemblyMessenger(G4String index);
  ~MJDemoCalibrationAssemblyMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nv);
  void SetValues(MJDemoCalibrationAssembly* calAssembly);

protected:

private:
  G4UIdirectory* fCalAssemblyDir;
  MJDemoCalibrationAssembly* fCalAssembly;
  MJDemoCalibrationSourceMessenger* fCalSourceMessenger;
  MJDemoCalibrationTrackMessenger* fCalTrackMessenger;
  G4UIcmdWithABool* fSourceOnCmd;
  G4UIcmdWithABool* fTrackOnCmd;
  G4UIcmdWith3VectorAndUnit* fSourceOffsetCmd;
  G4UIcmdWith3VectorAndUnit* fTrackOffsetCmd;
};
#endif
