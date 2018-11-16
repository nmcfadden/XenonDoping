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
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu// //---------------------------------------------------------------------------//
/**                                                            
 * $Id: MGGeometryGlobals.hh,v 1.2 2005-02-18 21:18:36 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGGeometryGlobals.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * Header file to contain variables, definitions, and accompanying headers that
 * are common to many MJGeometry classes.
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
 * AUTHOR: Kareem
 * CONTACT: 
 * FIRST SUBMISSION: Thu Jun 24 12:57:48 PDT 2004
 * 
 * REVISION:
 * 
 * 06-24-2004, Kareem
 *             Initial file submission (very simple 50cm lead shield)
 * 07-02-2004, R. Henning, 
 *             Converted all G4Color objects to "static" to allow
 *             file to be included in many different files without having
 *             multiple definitions of the same global objects.
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYGLOBALS_HH
#define _MGGEOMETRYGLOBALS_HH

//---------------------------------------------------------------------------//

//  GEANT4 headers
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
//---------------------------------------------------------------------------//

namespace MaGe {

//  Build the colors to be used in the visual attributes
static G4Color white(1.0, 1.0, 1.0);		// white
static G4Color ltgray(0.25, 0.25, 0.25);   // light gray
static G4Color mdgray(0.5, 0.5, 0.5);		// gray
static G4Color dkgray(0.75, 0.75, 0.75);   // dark gray
static G4Color black(0.0, 0.0, 0.0);		// black
static G4Color red(1.0, 0.0, 0.0);			// red
static G4Color green(0.0, 1.0, 0.0);		// green
static G4Color blue(0.0, 0.0, 1.0);		// blue
static G4Color cyan(0.0, 1.0, 1.0);		// cyan
static G4Color magenta(1.0, 0.0, 1.0);		// magenta 
static G4Color yellow(1.0, 1.0, 0.0);		// yellow

// definition of the inch, see 
// http://www.unc.edu/~rowlett/units/dictF.html
static const G4double inch = 2.54*CLHEP::cm; 
static const G4double inches = inch; 
static const G4double in = inch; 
static const G4double foot = 12.0*inch;
static const G4double feet = foot;
static const G4double ft = foot;

}

#endif
