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
 * CLASS DECLARATION:  MJGeometrySLACBDMessenger.hh
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

#ifndef _MJGEOMETRYSLACBDMESSENGER_HH
#define _MJGEOMETRYSLACBDMESSENGER_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometrySLACBD;
class G4UIdirectory;
class G4UIcmdWithAString;

//---------------------------------------------------------------------------//

class MJGeometrySLACBDMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometrySLACBDMessenger(MJGeometrySLACBD*);

  //copy constructor
  //MJGeometrySLACBDMessenger(const MJGeometrySLACBDMessenger &);

  //destructor
  ~MJGeometrySLACBDMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  //public interface

  //protected members
protected:


  //private  members
private:
  G4UIdirectory *fSLACBDDirectory;
  MJGeometrySLACBD *fSLACBD; 
  G4UIcmdWithAString *fShieldThicknessCmd;

};
#endif
