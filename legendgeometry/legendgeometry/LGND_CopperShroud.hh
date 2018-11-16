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
// CLASS IMPLEMENTATION:  LGND_CoppperShroud.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 *
 * REVISION:
 *
 * 8-06-2018, Created, N. McFadden
 */
//---------------------------------------------------------------------------//
//

#ifndef _LGND_COPPERSHROUD_HH
#define _LGND_COPPERSHROUD_HH

#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_CopperShroud: public LGND_Part
{

public:
  LGND_CopperShroud(G4String, G4String, G4double,G4String);
  LGND_CopperShroud(G4String, G4String, G4double);
  LGND_CopperShroud(const LGND_CopperShroud &);
  ~LGND_CopperShroud();

  G4LogicalVolume* BuildLogicalVolume();
  void SetLength(G4double length) {fLength = length*CLHEP::mm;}

private:
  G4double fLength;

};
//
#endif
