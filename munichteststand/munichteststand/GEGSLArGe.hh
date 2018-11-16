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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 05-13-2005
 *
 * 3 Jan 2006, Luciano, Fixed according to overcome the changes in the gerdageometry/ 
 *                      design. Not very elegant, but working
 */

// ---------------------------------------------------------------------------

#ifndef _GEGSLARGE_HH
#define _GEGSLARGE_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//The messenger for this detector has to be written
// --------------------------------------------------------------------------- 

class G4LogicalVolume;
class GEGSLArGeMessenger;

class GEGSLArGe : public MGGeometryDetector
{

public:
  
  // default constructor
  GEGSLArGe();

  // destructor
  ~GEGSLArGe();

  void ConstructDetector();

  //setters
  void SetBuildCrystals(G4bool val)
  {fBuildCrystals = val;}
  void SetFlagCrystalTrueCoaxial(G4bool);
  void SetFlagCrystalEncapsulation(G4bool);
  void SetCrystalInnerRadius(G4double val)
  {fCrystalInnerRadius = val;}
  void SetCrystalOuterRadius(G4double val)
  {fCrystalOuterRadius = val;}
  void SetCrystalHeight(G4double val)
  {fCrystalHeight = val;}
  void SetCrystalDeadLayerInnerThickness(G4double val)
  {fCrystalDeadLayerInnerThickness=val;}
  void SetCrystalDeadLayerOuterThickness(G4double val)
  {fCrystalDeadLayerOuterThickness = val;}
  void SetCrystalDeadLayerTopThickness(G4double val)
  {fCrystalDeadLayerTopThickness = val;}
  void SetCrystalHoleDepth(G4double val)
  {fCrystalHoleDepth= val;}
  void SetPhiSegmentsPerCrystal(G4int val)
  {fPhiSegmentsPerCrystal = val;}
  void SetZSegmentsPerCrystal(G4int val)
  {fZSegmentsPerCrystal = val;}
  void SetNumberOfColumns(G4int val)
  {fNumberOfColumns = val;}
  void SetColumnDistance(G4double val)
  {fColumnDistance = val;}
  void SetCrystalsPerColumn(G4int val)
  {fCrystalsPerColumn = val;}
  void SetCrystalColumnGap(G4double val)
  {fCrystalColumnGap = val;}

  G4bool GetFlagCrystalTrueCoaxial(){return fFlagCrystalTrueCoaxial;};
  G4bool GetFlagCrystalEncapsulation(){return fFlagCrystalEncapsulation;};

private:
  void BuildCrystalsInside();
  void BuildEncapsulation();
  void CrystalInitialization();
  G4double GetSegmentAngle(G4int, G4int);
  G4ThreeVector GetSegmentPosition(G4int);
  void ConstructCrystalSegment(G4int,G4int);
  G4ThreeVector GetCrystalPosition(G4int, G4int);

  G4ThreeVector fCrystalPosition;
  G4bool fBuildCrystals;
  G4LogicalVolume* liquidLogical;
  G4LogicalVolume* fCrystalSegmentLogical;
  G4LogicalVolume* fCrystalDeadLayerLogical;

  G4bool fFlagCrystalTrueCoaxial;
  G4bool fFlagCrystalEncapsulation;
  G4double fCrystalInnerRadius;
  G4double fCrystalOuterRadius;
  G4double fCrystalHeight;
  G4double fCrystalDeadLayerInnerThickness;
  G4double fCrystalDeadLayerOuterThickness;
  G4double fCrystalDeadLayerTopThickness;
  G4double fCrystalHoleDepth;
  
  G4int fSegmentMode;
  G4int fPhiSegmentsPerCrystal;
  G4int fZSegmentsPerCrystal;

  G4double fSegmentStartingAngle;
  G4double fSegmentGapAngle;
  G4double fSegmentGap;

  G4int fNumberOfColumns;
  G4double fColumnDistance;
  G4double fArrayDepth;

  G4int fCrystalsPerColumn; 
  G4double fCrystalColumnGap;
  G4double fColumnHeight;
  G4double fSegmentHeight;

  GEGSLArGeMessenger* fMessenger;
}; 

// --------------------------------------------------------------------------- 

#endif
