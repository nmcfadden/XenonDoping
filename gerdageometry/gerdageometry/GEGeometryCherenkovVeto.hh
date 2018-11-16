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
 * CLASS DECLARATION:  GEGeometryCherenkovVeto.hh
 *
 * DESCRIPTION:
 *
 *   A class to hold all the Cherenkov veto materials
 *
 * AUTHOR: Markus Knapp
 *
 * REVISION: MM-DD-YYYY
 *
 *  12-05-2007 Changes due to the changes of the cryostat
 */

#ifndef _GECHERENKOVVETO_HH
#define _GECHERENKOVVETO_HH

//---------------------------------------------------------------------------//
//      Geant4 headers
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"

// MG headers
//#include "geometry/MGGeometryDetector.hh"
#include "GEGeometryDetectorDB.hh"
//---------------------------------------------------------------------------//

class GEGeometryCherenkovVeto
{
  //public members
public:


  /// Default constructor
  GEGeometryCherenkovVeto(GEGeometryDetectorDB* theGeometryDB);

  //copy constructor

  /// Destructor
  ~GEGeometryCherenkovVeto();

  /// Construction
  void ConstructCherenkovVeto();


  //protected members
protected:


  //private  members
private:
  //GEGeometryDetectorDB* theGeometryDB;

  G4double VM2000EmissionSpectrum(G4double);
  void InitializeVM2000Spectrum();
  
  G4double fShieldingWaterTankHeight; 
  G4double fShieldingWaterHeight; 
  G4double fShieldingWaterRadius; 
  G4double fShieldingAirBufferHeight; 
  //G4double fShieldingInnerFootHeight; 
  //G4double fShieldingOuterFootHeight; 
  G4double fShieldingCopperShieldingLargeHeight; 
  G4double fShieldingCopperShieldingSmallHeight; 
  G4double fPhotocathodeInnerRadius;
  G4double fPhotocathodeOuterRadius;
  G4double fPMTInnerRadius;                     
  G4double fPMTOuterRadius; 
  G4double fPMTSteelConeThickness;
  G4double fPMTSteelConeHeight;
  G4double fPMTSteelConeUpperRmin;
  G4double fPMTSteelConeUpperRmax;
  G4double fPMTSteelConeLowerRmin;
  G4double fPMTSteelConeLowerRmax;
  G4double fPMTSteelBottomHeight;
  G4double fReflectionFoilThickness;
  G4double fShieldingFootHeight;
  G4double fShieldingFootIR;
  G4double fPMTCableTrayThickness;
  G4double fPMTHorizontalCableTrayWidth;
  G4double fPMTVerticalCableTrayHeight;

  G4LogicalVolume* fWaterLogical; 
  G4LogicalVolume* fPhotocathodeLogical;
  G4LogicalVolume* fPMTSteelConeLogical;
  G4LogicalVolume* fPMTSteelBottomLogical;
  G4LogicalVolume* fReflectionFoilWaterTankTubeLogical;
  G4LogicalVolume* fReflectionFoilWaterTankBottomLogical;
  G4LogicalVolume* fReflectionFoilCryoFootLogical;
  G4LogicalVolume* fReflectionFoilCryoFootTopLogical;
  G4LogicalVolume* fReflectionFoilCryoFootBottomLogical;
  G4LogicalVolume* fVerticalPMTCableTrayLogical;
  G4LogicalVolume* fHorizontalPMTCableTrayLogical;

  G4VPhysicalVolume* fPhotocathodePhysical;
  G4VPhysicalVolume* fPMTSteelConePhysical; 
  G4VPhysicalVolume* fPMTSteelBottomPhysical; 
  G4VPhysicalVolume* fReflectionFoilWaterTankTubePhysical;
  G4VPhysicalVolume* fReflectionFoilWaterTankBottomPhysical;
  G4VPhysicalVolume* fReflectionFoilCryoFootPhysical;
  G4VPhysicalVolume* fReflectionFoilCryoFootTopPhysical;
  G4VPhysicalVolume* fReflectionFoilCryoFootBottomPhysical;
  G4VPhysicalVolume* fHorizontalPMTCableTrayPhysical;
  G4VPhysicalVolume* fVerticalPMTCableTrayPhysical;

  G4double WLSyield;
  G4double CalculateWLSmfp(G4double);
  static const G4int npoints = 250;
  G4double frequencyV[npoints];
  G4double efficiencyV[npoints];
  G4bool successfulInitialization;

  G4int dist_number;

};

#endif

