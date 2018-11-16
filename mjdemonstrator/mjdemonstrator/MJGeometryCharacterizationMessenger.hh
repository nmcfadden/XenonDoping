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
 * $Id: MJGeometryCharacterizationMessenger.hh,v 1.2 2005/01/06 01:09:02 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryCharacterizationMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Messenger class for the characterization stand geometry. 
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 * Modified from MJGeometryDemonstratorMessenger.hh
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 * 
 * REVISION:
 * 07-22-2013, Created, A. Wasserman 
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield of the STC from messenger. J. MacMullin
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYCHARACTERIZATIONMESSENGER_HH
#define _MGGEOMETRYCHARACTERIZATIONMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometryCharacterization;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;


//---------------------------------------------------------------------------//

class MJGeometryCharacterizationMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryCharacterizationMessenger(MJGeometryCharacterization*);

  //destructor
  ~MJGeometryCharacterizationMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fCharacterizationDirectory;
  MJGeometryCharacterization      *fCharacterization; 
  G4UIcmdWithABool            *fCavernOnCmd;
  G4UIcmdWithABool            *fStandOnCmd;
  G4UIcmdWithABool            *fThinIRCmd;
  G4UIcmdWithABool            *fCryostatOnCmd;
  G4UIcmdWithAString* fCryostatChoiceCmd;
  G4UIcmdWithADouble		  *fColdPlateRotationInDegreesCmd;
  G4UIcmdWithADouble		  *fCMSRotationInDegreesCmd;
  G4UIcmdWithADouble		  *fThermalRotationInDegreesCmd;
  G4UIcmdWithABool            *fStringOnCmd;
  G4UIcmdWithADoubleAndUnit* fSourceAngleCmd;
  G4UIcmdWithADoubleAndUnit* fSourceRadiusCmd; 
  G4UIcmdWithADoubleAndUnit* fSourceZCmd;
  G4UIcmdWithoutParameter* fPrintSourcePositionCmd;
  G4UIcmdWithAString* fSourceMaterialCmd;
  G4UIcmdWithAString* fStringNameCmd;
};
#endif
