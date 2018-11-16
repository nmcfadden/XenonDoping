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
 * $Id: LGND_200_BaselineMessenger.hh,v 1.2 2005/01/06 01:09:02 rhenning Exp $
 *      
 * CLASS DECLARATION:  LGND_200_BaselineMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Messenger class for the legend200 geometry. 
 * Define dimensions using G4 macros.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 *
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: May 2018
 * 
 * REVISION:
 *
 * 05-2018, Created, R. Massarczyk
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_200_BASELINEMESSENGER_HH
#define _LGND_200_BASELINEMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class LGND_200_Baseline;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;

//---------------------------------------------------------------------------//

class LGND_200_BaselineMessenger : public G4UImessenger
{
public:

  //default constructor
  LGND_200_BaselineMessenger(LGND_200_Baseline*);

  //destructor
  ~LGND_200_BaselineMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory         *fLegend200Directory;

  LGND_200_Baseline     *fLegend200; 
	G4UIcmdWithAString    *fTestCmd;

	G4UIcmdWithABool      					*fCalibrationCmd;
	G4UIcmdWithAnInteger     				*fCalibrationSetSourceNumberCmd;
	G4UIcmdWithADoubleAndUnit				*fCalibrationSetSourcePositionCmd;
	G4UIcmdWithABool      					*fCalibrationSetAbsorberCmd;
  G4UIcmdWithAString              *fFiberSetShape;

};
#endif
