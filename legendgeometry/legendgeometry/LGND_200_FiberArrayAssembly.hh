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
 * CLASS DECLARATION:  LGND_200_FiberArrayAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *The assembly consisting of a JSON-generated detector and its mounting HW.
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
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 *
 * REVISION:
 *
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_200_FIBERARRAYASSEMBLY_HH
#define _LGND_200_FIBERARRAYASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Part.hh"
#include <vector>
class LGND_FiberCore;
class LGND_FiberCladding;
class LGND_FiberWLSCoating;
class LGND_FiberSiPM;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_200_FiberArrayAssembly
{

public:
  LGND_200_FiberArrayAssembly(G4String fName,G4String serialNumber,G4double length,G4String shape);
  LGND_200_FiberArrayAssembly(const LGND_200_FiberArrayAssembly &);
  ~LGND_200_FiberArrayAssembly();

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);



private:
  void AllocateParts();
  void UnallocateParts();

  LGND_FiberCore* fFiberCore;
  LGND_FiberCladding* fFiberCladdingInner;
  LGND_FiberCladding* fFiberCladdingOuter;
  LGND_FiberWLSCoating *fFiberWLSCoating;
  LGND_FiberSiPM *fFiberSiPM;
  LGND_FiberSiPM *fFiberReflector;

  G4double fLength;
  G4String fName;
  G4String fShape;
};
//
#endif
