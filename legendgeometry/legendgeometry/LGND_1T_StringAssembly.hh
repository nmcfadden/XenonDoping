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
 * CLASS DECLARATION:  LGND_1T_StringAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *The assembly consisting of a string of 77mm crystal assemblies.
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
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Jul 22, 2017
 *
 * REVISION:
 *
 * 07-22-2017, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_1T_STRINGASSEMBLY_HH
#define _LGND_1T_STRINGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Assembly.hh"
#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;
class LGND_1T_DetUnitAssembly;
class LGND_MiniShroud;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_1T_StringAssembly: public LGND_Assembly
{

public:
  LGND_1T_StringAssembly(G4String, G4String, size_t crystalNum = 30);
  LGND_1T_StringAssembly(const LGND_1T_StringAssembly &);
  ~LGND_1T_StringAssembly();

  size_t GetNumberOfCrystals() {return fCrystalNum;}
  void SetNumberOfCrystals(size_t crystalNum);

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);


private:
  size_t fCrystalNum;
  LGND_1T_DetUnitAssembly** fDetUnitPtr;
  LGND_MiniShroud* fMiniShroudPtr;

};
//
#endif
