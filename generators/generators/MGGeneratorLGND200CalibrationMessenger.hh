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
 * $Id: MGGeneratorLGND200CalibrationMessenger.hh,v 1.1 2014-14-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorLGND200CalibrationMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for LGND200 Calibration generator.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * June-2018, Created, Ralph
 *
 */
//---------------------------------------------------------------------------//
//

#ifndef _MGGENERATORLGND200CALMESSENGER_HH
#define _MGGENERATORLGND200CALMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class MGGeneratorLGND200Calibration;
      
class MGGeneratorLGND200CalibrationMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorLGND200CalibrationMessenger(MGGeneratorLGND200Calibration* );

  //copy constructor
  MGGeneratorLGND200CalibrationMessenger(const MGGeneratorLGND200CalibrationMessenger &);

  //destructor
  ~MGGeneratorLGND200CalibrationMessenger();

  //public interface

//  G4String GetCurrentValue(G4UIcommand *cmd);
  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:


  //private  members

private:
  MGGeneratorLGND200Calibration *fLGND200CalibrationGenerator;

  G4UIdirectory *fLGND200CalibrationDirectory;

  G4UIcmdWithoutParameter *fDumpCmd;
  G4UIcmdWithAnInteger *fACmd;
  G4UIcmdWithAnInteger *fZCmd;

};
#endif
