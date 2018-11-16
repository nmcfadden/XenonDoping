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
 * $Id: LGND_200_BaselineMessenger.hh,v 1.2 2005/01/06 01:09:02 rhenning Exp $
 *      
 * CLASS DECLARATION:  LGND_200_CalibrationAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Calibration Assembly that handles sources
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 *
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: May 2018
 * 
 * REVISION:
 *
 * 05-2018, Created, R. Massarczyk
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_200_CALIBRATIONASSEMBLY_HH
#define _LGND_200_CALIBRATIONASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Assembly.hh"
#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;
class LGND_200_CalibrationTaAbsorber;
class LGND_200_CalibrationSourceInner;
class LGND_200_CalibrationSourceOuter;
class LGND_200_CalibrationCuAbsorber;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_200_CalibrationAssembly: public LGND_Assembly
{
  
public:
  LGND_200_CalibrationAssembly(G4String, G4String);
  LGND_200_CalibrationAssembly(const LGND_200_CalibrationAssembly &);
  ~LGND_200_CalibrationAssembly();

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
	     G4LogicalVolume* motherLogical);

	void SetSourceNumber(G4int SourceNumber) {fSourceNumber = SourceNumber;}
	void SetSourcePosition(G4double SourcePosition) {fSourcePosition->setZ(SourcePosition);}
	void SetAbsorber(G4bool abs) {fAbsorber = abs;}

  LGND_200_CalibrationSourceInner* GetGlobalSourcePtr(){return fLGND_200_CalibrationSourceInnerPtr;}
	

private:	
	G4int 						fSourceNumber;
	G4ThreeVector*		fSourcePosition;
	G4ThreeVector*		fSourcePositionGlobal;
	G4bool						fAbsorber;

	LGND_200_CalibrationTaAbsorber*  		fLGND_200_CalibrationTaAbsorberPtr;
	LGND_200_CalibrationSourceInner*  	fLGND_200_CalibrationSourceInnerPtr;
	LGND_200_CalibrationSourceOuter*  	fLGND_200_CalibrationSourceOuterPtr;
	LGND_200_CalibrationCuAbsorber*  		fLGND_200_CalibrationCuAbsorberPtr;
};
//
#endif
