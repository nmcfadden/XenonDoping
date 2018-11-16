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
 * $Id: GEHADESBEGeTests.hh,v 1.3 2011-11-29 Bjoern $
 *      
 * CLASS DECLARATION:  GEHADESBEGe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the BEGe detectors tests in HADES 
 *
 * REVISION:
 *
 *
 * 29-11-2011, Derived from GELNGSBEGeDetector class and implemented as new class in MaGe, Bjoern
 * 16-01-2012, Added different sources, Bjoern
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GEHADESBEGETESTDETECTOR_HH
#define _GEHADESBEGETESTDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4NistManager.hh" 

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;
class GEHADESBEGeTestsMessenger;

class GEHADESBEGeTests : public MGGeometryDetector
{

private:
  

public:

  //default constructor
  GEHADESBEGeTests();

  //destructor
  ~GEHADESBEGeTests();

  //public interface
  void ConstructDetector();

  void SetCryostatWindowThickness(G4double val){ CryostatWindowThickness = val;};
  G4double GetCryostatWindowThickness(){return CryostatWindowThickness;};
  //
  void SetCryostatWallThickness(G4double val){ CryostatWallThickness = val;};
  G4double GetCryostatWallThickness(){return CryostatWallThickness;};
  //
  void SetCryostatDiameter(G4double val){ CryostatDiameter = val;};
  G4double GetCryostatDiameter(){return CryostatDiameter;};
  //
  void SetCryostatHeight(G4double val){ CryostatHeight = val;};
  G4double GetCryostatHeight(){return CryostatHeight;};

  //
  void SetXtalDiameter(G4double val){ XtalDiameter = val;};
  G4double GetXtalDiameter(){return XtalDiameter;};
  //
  void SetXtalHeight(G4double val){ XtalHeight = val;};
  G4double GetXtalHeight(){return XtalHeight;};
  //
  void SetXtalDistanceToWindow(G4double val){ XtalDistanceToWindow = val;};
  G4double GetXtalDistanceToWindow(){return XtalDistanceToWindow;};
  //
  void SetXtalDitchInnerRadius(G4double val){ XtalDitchInnerRadius = val;};
  G4double GetXtalDitchInnerRadius(){return XtalDitchInnerRadius;};
  //
  void SetXtalDitchOuterRadius(G4double val){ XtalDitchOuterRadius = val;};
  G4double GetXtalDitchOuterRadius(){return XtalDitchOuterRadius;};
  //
  void SetXtalDitchDepth(G4double val){ XtalDitchDepth = val;};
  G4double GetXtalDitchDepth(){return XtalDitchDepth;};
  //
  void SetXtalDitchOnBottom(G4bool val){ XtalDitchOnBottom = val;};
  G4double GetXtalDitchOnBottom(){return XtalDitchOnBottom;};
  //
  void SetXtalCornerDiameter(G4double val){ XtalCornerDiameter = val;};
  G4double GetXtalCornerDiameter(){return XtalCornerDiameter;};
  //
  void SetXtalCornerHeight(G4double val){ XtalCornerHeight = val;};
  G4double GetXtalCornerHeight(){return XtalCornerHeight;};
  //
  void SetXtalCornerOnBottom(G4bool val){ XtalCornerOnBottom = val;};
  G4double GetXtalCornerOnBottom(){return XtalCornerOnBottom;};

  //
  void SetSpecialDetectorType(G4String type){fSpecialDetectorType = type;};
  G4String GetSpecialDetectorType(){return fSpecialDetectorType;};
  //
  void SetSourceType(G4String method){fSourceType = method;};
  G4String GetSourceType(){return fSourceType;};
  //
  void SetSourceDistance(G4double val){ SourceDistance = val;};
  G4double GetSourceDistance(){return SourceDistance;};
  //
  void SetActivateEnrBEGeCryostatHolders(G4bool val){ ActivateEnrBEGeCryostatHolders = val;};
  G4double GetActivateEnrBEGeCryostatHolders(){return ActivateEnrBEGeCryostatHolders;};

  //
  void SetActivateDepBEGeCryostatHolders(G4bool val){ ActivateDepBEGeCryostatHolders = val;};
  G4double GetActivateDepBEGeCryostatHolders(){return ActivateDepBEGeCryostatHolders;};
  //
  void SetHADESLeadCastleType(G4int val){ HADESLeadCastleType = val;};
  G4int GetHADESLeadCastleType(){return HADESLeadCastleType;};

  // BRADY ENVIRONMENT WITH COLLIMATOR
  //
  void SetBRADYEnv(G4bool val){ fUseBRADYEnv = val;};
  G4double GetBRADYEnv(){return fUseBRADYEnv;};

  void SetCollimator(G4int num){ fUseCollimator = num;};
  G4int GetCollimator(){return fUseCollimator;};

  void SetCollimatorDistance(G4double val){ fCollimatorDistance = val;};
  G4double GetCollimatorDistance(){return fCollimatorDistance;};

  void SetCollimatorPosition(G4double val){ fCollimatorPosition = val;};
  G4double GetCollimatorPosition(){return fCollimatorPosition;};

  void SetCryostatFillMaterial(G4String val){ fCryostatFillMaterial = val;};
  G4String GetCryostatFillMateria(){return fCryostatFillMaterial;};


  //functions for DL post processing
  
  G4float GetDistToUpperSurf(G4ThreeVector vec);
  G4float GetDistToLowerSurf(G4ThreeVector vec);
  G4float GetDistToSideSurf(G4ThreeVector vec);
  G4float GetDistToCornerSurf(G4ThreeVector vec);
  G4float GetDistToDitchSurf(G4ThreeVector vec);
    


private:
  GEHADESBEGeTestsMessenger* fMessenger;

  G4double CryostatWindowThickness;
  G4double CryostatWallThickness;
  G4double CryostatDiameter;
  G4double CryostatHeight;
 
  G4double XtalDiameter;
  G4double XtalHeight;
  G4double XtalDistanceToWindow;
  G4double XtalDitchInnerRadius;
  G4double XtalDitchOuterRadius;
  G4double XtalDitchDepth;
  G4bool XtalDitchOnBottom;
  G4double XtalCornerDiameter;
  G4double XtalCornerHeight;
  G4bool XtalCornerOnBottom;

  G4String fSpecialDetectorType;
  G4String fSourceType;
  G4double SourceDistance;

  G4bool ActivateEnrBEGeCryostatHolders;
  G4bool ActivateDepBEGeCryostatHolders;

  G4int HADESLeadCastleType;

  G4bool fUseBRADYEnv;
  G4int fUseCollimator;
  G4double fCollimatorDistance;
  G4double fCollimatorPosition;

  G4String fCryostatFillMaterial;
  
};
#endif
