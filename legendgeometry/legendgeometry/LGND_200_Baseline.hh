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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *
 * CLASS DECLARATION:  LGND_200_Baseline.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *LEGeND 200kg baseline array geometry builder
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
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: May  25 2017
 *
 * REVISION:
 *
 * 05-25-2017, Created, M. Green
 * June 2018, Added CalibrationStuff
 */
//---------------------------------------------------------------------------//
//


#ifndef _LGND_200_BASELINE_HH
#define _LGND_200_BASELINE_HH


//---------------------------------------------------------------------------//
#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"



//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

class LGND_200_BaselineMessenger;

using namespace std;

class LGND_200_Baseline: public MGGeometryDetector
{
public:
  LGND_200_Baseline(G4String);
  virtual ~LGND_200_Baseline();

  G4String GetMaterialsList() { return "LGND"; }
  void ConstructDetector();

  virtual int GetSensitiveVolumeID(const std::string& volName);

	void SetTest(G4String);

	void 		SetCalibration(G4bool CalibrationOn) {fCalibrationOn = CalibrationOn;}
	void 		SetCalibrationSourceNumber(G4int SourceNumber) {fCalibrationSourceNumber = SourceNumber;}
	void 		SetCalibrationSourcePosition(G4double SourcePosition) {fCalibrationSourcePosition = SourcePosition;}
	void 		SetCalibrationAbsorber(G4bool CalibrationAbs) {fCalibrationAbs = CalibrationAbs;}
  void    SetFiberShape(G4String shape) {fFiberShape = shape;}
	G4bool	IsCalibrationOn() { return fCalibrationOn; }

private:

	LGND_200_BaselineMessenger* fMessenger;

	G4bool		fCalibrationOn;
	G4double 	fCalibrationSourceNumber;
	G4double  fCalibrationSourcePosition;
	G4bool		fCalibrationAbs;
  G4String  fFiberShape;


};
#endif
