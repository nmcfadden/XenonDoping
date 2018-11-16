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
 * CLASS DECLARATION:  GEGeometryGerda.hh
 *
 * DESCRIPTION:
 *
 *   A class to settle the Gerda experiments.
 *   It inherit class MGGeometryDetector
 *
 * AUTHOR: Xiang  Liu
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-02-2004, added kapton cable for Ge crystal, Xiang
 *   01-19-2005, implemented new file structure, Kevin 
 *   03-23-2005, Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 *   06-04-2007, added MGTCrystalData, Kevin  
 *   09-04-2007, added G4Region member for SensitiveRegion, Luciano
 *   14-06-2011, added encapsulation part, Sabine
 *   26-03-12012, added mini-shrouds, Giovanni
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 */

#ifndef _GEGEOMETRYGERDA_HH
#define _GEGEOMETRYGERDA_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "geometry/MGGeometryDetector.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryDetector.hh"
#include "gerdageometry/GEGeometryShielding.hh"
#include "gerdageometry/GEGeometryCalibrationSourceArray.hh" 
#include "gerdageometry/GEGeometryCollimatedCalibrationSourceArray.hh" 
#include "gerdageometry/GEGeometryEncapsulation.hh" 
#include "gerdageometry/GEGeometryMiniShroud.hh"
#include "GEGeometryDetectorParameters.hh" 
#include "MGCrystalDataContainer.hh" 
#include "gerdageometry/GEGeometryLArInstrumentation.hh"

// --------------------------------------------------------------------------- 

class GEGeometryGerda : public MGGeometryDetector
{

public:

  /// Default constructor
  GEGeometryGerda();

  /// Destructor
  virtual ~GEGeometryGerda();

  /// Construction inherited from MJGeometryDetector
  void ConstructDetector();

  GEGeometryDetectorDB* GetDB(){return fGERDADetectorDB;}

private:

  GEGeometryDetectorDB*             fGERDADetectorDB; 
  GEGeometryDetector*               fGERDADetector; 
  GEGeometryShielding*              fGERDAShielding; 
  GEGeometryCalibrationSourceArray* fGERDACalibrationSourceArray; 
  GEGeometryCollimatedCalibrationSourceArray* fGERDACollimatedCalibrationSourceArray; 
  GEGeometryEncapsulation* fGERDAEncapsulation; 
  GEGeometryMiniShroud* fGERDAMiniShroud;
  GEGeometryLArInstrumentation*		fGERDALArInstrumentation;

  G4Region* fSensitiveRegion;

protected:

  virtual void FillCrystalDataVector(MGCrystalDataVector* crystaldatavector);

};

#endif

