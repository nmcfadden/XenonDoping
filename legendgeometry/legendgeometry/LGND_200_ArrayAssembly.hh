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
 * CLASS DECLARATION:  LGND_200_ArrayAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *The assembly consisting of 19 detector strings.
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

#ifndef _LGND_200_ARRAYASSEMBLY_HH
#define _LGND_200_ARRAYASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Assembly.hh"
#include "legendgeometry/LGND_Part.hh"
#include "legendgeometry/LGND_200_StringAssembly.hh"
#include "G4LogicalVolume.hh"

using namespace std;
//---------------------------------------------------------------------------//

class LGND_200_ArrayAssembly: public LGND_Assembly
{

public:
  LGND_200_ArrayAssembly(G4String, G4String);
  LGND_200_ArrayAssembly(const LGND_200_ArrayAssembly &);
  ~LGND_200_ArrayAssembly();


  LGND_200_StringAssembly* GetString1() {return fString1Ptr;}
  LGND_200_StringAssembly* GetString2() {return fString2Ptr;}
  LGND_200_StringAssembly* GetString3() {return fString3Ptr;}
  LGND_200_StringAssembly* GetString4() {return fString4Ptr;}
  LGND_200_StringAssembly* GetString5() {return fString5Ptr;}
  LGND_200_StringAssembly* GetString6() {return fString6Ptr;}
  LGND_200_StringAssembly* GetString7() {return fString7Ptr;}
  LGND_200_StringAssembly* GetString8() {return fString8Ptr;}
  LGND_200_StringAssembly* GetString9() {return fString9Ptr;}
  LGND_200_StringAssembly* GetString10() {return fString10Ptr;}
  LGND_200_StringAssembly* GetString11() {return fString11Ptr;}
  LGND_200_StringAssembly* GetString12() {return fString12Ptr;}
  LGND_200_StringAssembly* GetString13() {return fString13Ptr;}
  LGND_200_StringAssembly* GetString14() {return fString14Ptr;}
  LGND_200_StringAssembly* GetString15() {return fString15Ptr;}
  LGND_200_StringAssembly* GetString16() {return fString16Ptr;}
  LGND_200_StringAssembly* GetString17() {return fString17Ptr;}
  LGND_200_StringAssembly* GetString18() {return fString18Ptr;}
  LGND_200_StringAssembly* GetString19() {return fString19Ptr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);



private:
  size_t fNumStrings;
  LGND_200_StringAssembly*  fString1Ptr;
  LGND_200_StringAssembly*  fString2Ptr;
  LGND_200_StringAssembly*  fString3Ptr;
  LGND_200_StringAssembly*  fString4Ptr;
  LGND_200_StringAssembly*  fString5Ptr;
  LGND_200_StringAssembly*  fString6Ptr;
  LGND_200_StringAssembly*  fString7Ptr;
  LGND_200_StringAssembly*  fString8Ptr;
  LGND_200_StringAssembly*  fString9Ptr;
  LGND_200_StringAssembly*  fString10Ptr;
  LGND_200_StringAssembly*  fString11Ptr;
  LGND_200_StringAssembly*  fString12Ptr;
  LGND_200_StringAssembly*  fString13Ptr;
  LGND_200_StringAssembly*  fString14Ptr;
  LGND_200_StringAssembly*  fString15Ptr;
  LGND_200_StringAssembly*  fString16Ptr;
  LGND_200_StringAssembly*  fString17Ptr;
  LGND_200_StringAssembly*  fString18Ptr;
  LGND_200_StringAssembly*  fString19Ptr;

};
//
#endif
