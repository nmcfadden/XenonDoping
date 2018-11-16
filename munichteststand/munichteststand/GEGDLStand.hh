
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
 * FIRST SUBMISSION: 28 Jun 2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 12-01-2008, added dead layer parameter, z position, Dusan & Marik
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEGDLSTAND_HH
#define _GEGDLSTAND_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"
#include "munichteststand/GEGDLStandMessenger.hh"

// --------------------------------------------------------------------------- 

class GEGDLStandMessenger;
class GEGDLStand : public MGGeometryDetector
{

private:
  
  G4double deadLayerThickness;
  G4double zShiftDetector;
  G4double crystalDiameter;
  G4double crystalHeight;
  G4double holeDiameter;
  G4double holeHeight;
  G4double grooveouterDiameter;
  G4double grooveThickness;
  G4double grooveinnerDiameter;
  G4double cutparameter;
  G4double cutposition;
  G4double cuttopparameter;
  G4double cuttopposition;

  GEGDLStandMessenger* fTheMessenger;

public:
  
  // default constructor

  GEGDLStand();

  // destructor

  ~GEGDLStand();

  // construction inherited from MJGeometryDetector

  void ConstructDetector();
  void SetDeadThickness(G4double dT) {deadLayerThickness=dT;};
  void SetShiftDetector(G4double dT) {zShiftDetector=dT;};
  void SetDetectorDiam(G4double dT) {crystalDiameter=dT;};
  void SetDetectorHeight(G4double dT) {crystalHeight=dT;};
  void SetHoleDiam(G4double dT) {holeDiameter=dT;};
  void SetDetHoleHeight(G4double dT) {holeHeight=dT;};
  void SetGrooveOutDiam(G4double dT) {grooveouterDiameter=dT;};
  void SetGrooveThickness(G4double dT) {grooveThickness=dT;};
  void SetGrooveInDiam(G4double dT) {grooveinnerDiameter=dT;};
  void SetCutParameter(G4double dT) {cutparameter=dT;};
  void SetCutPosition(G4double dT) {cutposition=dT;};
  void SetCuttopParameter(G4double dT) {cuttopparameter=dT;};
  void SetCuttopPosition(G4double dT) {cuttopposition=dT;};


}; 

// --------------------------------------------------------------------------- 

#endif
