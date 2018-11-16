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
 * CLASS DECLARATION:  MJDemoSigConnPins.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry code for 8 signal connector pins arranged as in the signal plugs
 * Single pin is modeled as a solid cylinder
 * Center is located in the middle of the 8 pins, in the center along the length of the pins
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
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 4, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Now includes all 8 pins instead of a single pin, I. Guinn
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGCONNPIN_HH
#define _MJDEMOSIGCONNPIN_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"
#include "geometry/MGGeometryGlobals.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSigConnPins: public MJVDemoPart
{
  
public:
  MJDemoSigConnPins(G4String, G4String);
  MJDemoSigConnPins(const MJDemoSigConnPins &);
  ~MJDemoSigConnPins();

  G4LogicalVolume* ConstructPart();
  static inline G4double GetLength() {return 0.25*MaGe::in;}
  static inline G4double GetDiam() {return 0.02*MaGe::in;}
  static inline G4double GetRho() {return 0.065*MaGe::in;}
};
//
#endif
