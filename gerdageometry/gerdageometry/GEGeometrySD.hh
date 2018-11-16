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
 * CLASS DECLARATION:  GEGeometrySD.hh
 *
 * DESCRIPTION:
 *
 *   A class for sensitive detector.
 *   A user is supposed NOT to inherit this class
 *   but to use this class to define his sensitive detector
 *
 * AUTHOR:  Xiang Liu
 *
 * REVISION: MM-DD-YYYY
 *
 */

#ifndef _GEGEOMETRYSD_HH
#define _GEGEOMETRYSD_HH

//---------------------------------------------------------------------------

#include "G4VSensitiveDetector.hh"
#include "GEGeometrySDHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

//---------------------------------------------------------------------------

class  GEGeometrySD : public G4VSensitiveDetector
{
public:
  /// Default constructor
  GEGeometrySD(G4String name);

  //copy constructor

  /// Destructor
  ~GEGeometrySD();

  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);


private:

      GEGeometrySDHitsCollection *HitsCollection;
};

#endif

