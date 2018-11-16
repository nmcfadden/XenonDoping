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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  @CLASS_NAME@.hh
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
 * AUTHOR: @AUTHOR@
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 * 
 * mm-dd-yyyy, What is changed, Whoami
 */
// --------------------------------------------------------------------------//

#ifndef _@ONCE_NAME@_HH
#define _@ONCE_NAME@_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers

//---------------------------------------------------------------------------//

class @CLASS_NAME@ 
{
  //public members
public:
  //default constructor
  @CLASS_NAME@();

  //copy constructor
  @CLASS_NAME@(const @CLASS_NAME@ &);

  //destructor
  ~@CLASS_NAME@();


  //protected members
protected:


  //private  members
private:
};
//
#endif
