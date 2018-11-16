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
 * $Id: 
 *      
 * CLASS DECLARATION:  MJGeometryPbShieldMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *  Messenger for CERN NA55 simulation geometry.
 *
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
 * AUTHOR: mgmarino 
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 
 *  */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYPbShieldMESSENGER_HH
#define _MJGEOMETRYPbShieldMESSENGER_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometryPbShield;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

//---------------------------------------------------------------------------//

class MJGeometryPbShieldMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryPbShieldMessenger(MJGeometryPbShield*);

  //destructor
  ~MJGeometryPbShieldMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  //private  members
protected:
  G4UIdirectory* fPbShieldDirectory;
  MJGeometryPbShield* fPbShield; 
  G4UIcmdWithADoubleAndUnit* fSetOuterShieldDim;
  G4UIcmdWithADoubleAndUnit* fSetInnerShieldDim;
  G4UIcmdWithADouble* fSetImpSampRatio;
  G4UIcmdWithAnInteger* fSetNumberImpSampRegions;

};
#endif
