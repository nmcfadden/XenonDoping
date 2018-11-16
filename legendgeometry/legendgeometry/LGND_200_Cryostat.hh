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
 * CLASS DECLARATION:  LGND_200_Cryostat.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *Geometry code for the Gerda Cryostat
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 *
 * REVISION:
 *
 * 07-22-2017, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_200_Cryostat_HH
#define _LGND_200_Cryostat_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_200_Cryostat: public LGND_Part
{

public:
  LGND_200_Cryostat(G4String partName, G4String serialNumber);
  LGND_200_Cryostat(const LGND_200_Cryostat &);
  ~LGND_200_Cryostat();

  G4LogicalVolume* BuildLogicalVolume();
  
  inline G4double GetOuterRadius(){return fOuterRadius;}
  inline G4double GetOuterHeight(){return fOuterHeight;}
  inline G4double GetThickness  (){return fThickness;}
  inline G4double GetInnerRadius(){return fInnerRadius;}
  inline G4double GetInnerHeight(){return fInnerHeight;}

private:
  G4double fOuterRadius;
  G4double fOuterHeight;
  G4double fThickness;
  G4double fInnerRadius;
  G4double fInnerHeight;
};
//
#endif
