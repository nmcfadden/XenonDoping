//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
 * $Id: MJGeometryWIPPnDetector.hh,v 1.1 2009-05-29 17:12:56 rojo Exp $
 *      
 * CLASS DECLARATION:  MJGeometryWIPPnDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry class for n type detector underground at WIPP.
 * New implementation to accommodate new shield design (May 2007)
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
 * AUTHOR: R.Johnson
 * CONTACT: pitpat@u.washington.edu
 * FIRST SUBMISSION: Wed July 19 12:00:00 PDT 2007
 * 
 * REVISION:
 * 
 * 07-19-2006, Created, R. Johnson
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYWIPPNDETECTOR_HH
#define _MJGEOMETRYWIPPNDETECTOR_HH

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class  MJGeometryWIPPnDetectorMessenger;
class  G4Material;
class  MGGeometryIdealCoaxCrystal;
class  G4VPhysicalVolume;
class  G4VisAttributes;

//---------------------------------------------------------------------------//

class MJGeometryWIPPnDetector : public MGGeometryDetector
{
public:

  MJGeometryWIPPnDetector(G4String serNum);

  ~MJGeometryWIPPnDetector();

  void ConstructDetector();
  void SetShieldConfiguration(G4String str);
  void InitializeSample();  
  bool UsesMJPSQLDatabase() { return true; }  
  
  void Initialize7CuBalls();
  void InitializeEformedBeaker();
  void InitializeCuSO4();
  void InitializeCTFG();
  void InitializeFoils();
  void InitializePbFoils();
  void InitializeButton();
  
  void SetDefaults();
  void SetColors();

  void SetUseSamples(G4bool val) { fUsingSamples = val; }
  void SetSample(G4String val) { fSample = val; }
  void SetButtonMat(G4String val) { fButtonMat=val; }
  void SetCryostatThickness(G4double val) { fCryostatThickness = val; }
  void SetCryostatCrystalDistance(G4double val) { fDistanceXtal2Cryostat = val; }

private:
  MGGeometryIdealCoaxCrystal *fIdealCoaxCrystal;

  G4String            fShieldConfiguration;
  
  G4LogicalVolume*    fCopperLogical;
  G4LogicalVolume*    fCavityLogical;
  G4LogicalVolume*    fCryostatLogical;
  G4LogicalVolume*    fXtalVacuumLogical;
  
  G4VPhysicalVolume*  fCopperPhysical;
  G4VPhysicalVolume*  fCavityPhysical;
  G4VPhysicalVolume*  fCryostatPhysical;
  G4VPhysicalVolume*  fXtalVacuumPhysical;
  G4VPhysicalVolume*  fPhysicalCrystal;

  // Messenger
  MJGeometryWIPPnDetectorMessenger *fMessenger;


//---------------------------------------------------------------------------//  

  // Dimensions that can be tweaked

  G4double            fCryostatThickness;
  G4double            fDistanceXtal2Cryostat;

//---------------------------------------------------------------------------//  
  // optional Cu sources:
//---------------------------------------------------------------------------//  

  G4bool              fUsingSamples;
  G4String            fSample;
  
  // Cu balls:
  G4LogicalVolume*    f7CuBallsLogical;
  G4VPhysicalVolume*  f7CuBallsPhysical;

  G4double            fCuBallRadius;

  // Eformed Beaker
  G4LogicalVolume*    fBeakerLogical;
  G4VPhysicalVolume*  fBeakerPhysical;
  
  G4double            fBeakerOuterRadius;
  G4double            fBeakerLength;

  // CuSO4
  G4LogicalVolume*    fCuSO4Logical;
  G4VPhysicalVolume*  fCuSO4Physical;

  // CTFG
  G4LogicalVolume*    fCTFGLogical;
  G4VPhysicalVolume*  fCTFGPhysical;

  // Foils
  G4LogicalVolume*    CdFoilLogical;
  G4VPhysicalVolume*  CdFoilPhysical;
  G4LogicalVolume*    NbFoilLogical;
  G4VPhysicalVolume*  NbFoilPhysical;
  G4LogicalVolume*    ZnFoilLogical;
  G4VPhysicalVolume*  ZnFoilPhysical;
  G4LogicalVolume*    CdSpotLogical;
  G4VPhysicalVolume*  CdSpotPhysical;
  G4LogicalVolume*    NbSpotLogical;
  G4VPhysicalVolume*  NbSpotPhysical;
  G4LogicalVolume*    ZnSpotLogical;
  G4VPhysicalVolume*  ZnSpotPhysical;

  //PbFoils
  G4LogicalVolume*    PbFoilLogical;
  G4VPhysicalVolume*  PbFoilPhysical1;
  G4VPhysicalVolume*  PbFoilPhysical2; 
  G4VPhysicalVolume*  PbFoilPhysical3; 
  G4VPhysicalVolume*  PbFoilPhysical4; 
  G4VPhysicalVolume*  PbFoilPhysical5;
  G4LogicalVolume*    PbSpotLogical;
  G4VPhysicalVolume*  PbSpotPhysical1;
  G4VPhysicalVolume*  PbSpotPhysical2; 
  G4VPhysicalVolume*  PbSpotPhysical3; 
  G4VPhysicalVolume*  PbSpotPhysical4; 
  G4VPhysicalVolume*  PbSpotPhysical5;

  //Button source

  G4LogicalVolume*   PlasticLogical;
  G4VPhysicalVolume*  PlasticPhysical;
  G4LogicalVolume*   SourceLogical;
  G4VPhysicalVolume*  SourcePhysical;
  
  G4String           fButtonMat;

};
#endif
