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
//                                                          
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: @AUTHOR@
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 */
//---------------------------------------------------------------------------//
//

//#include <systemG4Stuff.hh>             //additional system Headers
//#include "globalsStuff.hh"              //additional Global Headers
//#include "G4Stuff.hh"		          //additional G4 Class Headers
//#include "MJStuff.hh"                   //additional MJ Headers

//---------------------------------------------------------------------------//

#include "@PACKAGE_NAME@/@CLASS_NAME@.hh"                  //Present MJ Class Headers 

//---------------------------------------------------------------------------//

@CLASS_NAME@::@CLASS_NAME@()
{
	// Must be G4 compliant here
	// http://geant4.web.cern.ch/geant4/collaboration/coding_guidelines.html
	// e.g. geant4/source/volume/src/G4Navigator.cc as an example
}
@CLASS_NAME@::~@CLASS_NAME@()
{
}
