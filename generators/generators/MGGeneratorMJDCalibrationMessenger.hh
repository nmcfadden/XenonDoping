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
 * $Id: MGGeneratorMJDCalibrationMessenger.hh,v 1.1 2014-14-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorMJDCalibrationMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for MJD Calibration generator.
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
 * AUTHOR: K. Vorren
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 02-24-2016, Removed most commands from messenger since they are now
 *             read directly from the geometry, T. Caldwell
 * 07-14-2014, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORMJDCALMESSENGER_HH
#define _MGGENERATORMJDCALMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class MGGeneratorMJDCalibration;

class MGGeneratorMJDCalibrationMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorMJDCalibrationMessenger(MGGeneratorMJDCalibration *generator);

  //copy constructor
  MGGeneratorMJDCalibrationMessenger(const MGGeneratorMJDCalibrationMessenger &);

  //destructor
  ~MGGeneratorMJDCalibrationMessenger();

  //public interface

  G4String GetCurrentValue(G4UIcommand *cmd);
  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:


  //private  members

private:
  MGGeneratorMJDCalibration *fMJDCalibrationGenerator;

  G4UIdirectory *fMJDCalibrationDirectory;

  G4UIcmdWithoutParameter *fDumpCmd;
  G4UIcmdWithAnInteger *fACmd;
  G4UIcmdWithAnInteger *fZCmd;
  G4UIcmdWithAString *fSourcePosCmd;
};
#endif
