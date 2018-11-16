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
 * CLASS DECLARATION:  LGND_1T_DetUnitAssembly.hh
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
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Jul 22, 2017
 *
 * REVISION:
 *
 * 07-22-2017, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_1T_DETUNITASSEMBLY_HH
#define _LGND_1T_DETUNITASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Assembly.hh"
#include "legendgeometry/LGND_Part.hh"
#include <vector>
class LGND_Detector;
class LGND_SiPlate;
class LGND_HVRing90;
class LGND_LMFE;
class LGND_CrystalInsulator;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_1T_DetUnitAssembly: public LGND_Assembly
{

public:
  LGND_1T_DetUnitAssembly(
	G4String fName,
	G4String serialNumber
  );
  LGND_1T_DetUnitAssembly(const LGND_1T_DetUnitAssembly &);
  ~LGND_1T_DetUnitAssembly();

  G4double GetDUheight(){return fDUheight;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  void SetCrystalDiameter(G4double diameter)	{fCrystalDiameter = diameter*CLHEP::mm;}
  void SetCrystalHeight(G4double height)		{fCrystalHeight = height*CLHEP::mm;}
  void SetPostRadius(G4double radius)			{fPostRadius = radius*CLHEP::mm;}
  G4double GetCrystalDiameter() 				{return fCrystalDiameter;}
  G4double GetCrystalHeight()					{return fCrystalHeight;}
  G4double GetPostRadius()						{return fPostRadius;}

  LGND_Detector* GetCrystal() {return fDetectorPtr;}

private:
  G4double fCrystalDiameter;
  G4double fCrystalHeight;
  G4double fPostRadius;
  G4double fDUheight;
  std::vector<G4String> fSpacerType;

  void AllocateParts();
  void UnallocateParts();

  LGND_Detector* fDetectorPtr;
  LGND_SiPlate* fSiPlatePtr;
  LGND_HVRing90* fHVRingPtr;
  LGND_LMFE* fLMFEPtr;
  LGND_CrystalInsulator* fInsulator1Ptr;
  LGND_CrystalInsulator* fInsulator2Ptr;
  LGND_CrystalInsulator* fInsulator3Ptr;
};
//
#endif
