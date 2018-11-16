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
 * CLASS IMPLEMENTATION:  LGND_200_InstFiber.cc
 */
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 * FIRST SUBMISSION: 
 *
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//
#ifndef _LGND_200_InstFiber_HH
#define _LGND_200_InstFiber_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_200_InstFiber: public LGND_Part
{

public:
  LGND_200_InstFiber(G4String partName, G4String serialNumber);
  LGND_200_InstFiber(G4String partName, G4String serialNumber, G4String material);
  LGND_200_InstFiber(const LGND_200_InstFiber &);
  ~LGND_200_InstFiber();

  G4LogicalVolume* BuildLogicalVolume();

private:

};
//
#endif
