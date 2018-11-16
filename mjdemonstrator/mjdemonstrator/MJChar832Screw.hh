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
 * CLASS DECLARATION:  MJChar832Screw.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry code for a 8-32 stainless steel hex screw
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 * Modified from MJDemo832Screw for new material
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJCHAR832SCREW_HH
#define _MJCHAR832SCREW_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJChar832Screw: public MJVDemoPart
{
  
public:
  MJChar832Screw(G4String partName, G4String serialNumber, 
					G4double length);
  MJChar832Screw(const MJChar832Screw &);
  ~MJChar832Screw();

  G4LogicalVolume* ConstructPart();

  void SetLength(G4double length)		{fScrewLength = length*CLHEP::mm;}
  G4double GetLength()					{return fScrewLength;}  

private:
  G4double fScrewLength;
};
//
#endif
