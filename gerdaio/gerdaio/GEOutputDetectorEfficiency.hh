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
 * CLASS DECLARATION:  GEOutputDetectorEfficiency.hh
 *
 * DESCRIPTION:
 *
 *   Writes output for the DetectorEfficiency on an ASCII file. This class inherits directly
 *   from MGOutputASCII
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-12-2006 Created, L. Pandola
 *
 */

#ifndef _GEOUTPUTDETECTOREFFICIENCY_HH
#define _GEOUTPUTDETECTOREFFICIENCY_HH 1

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "io/MGOutputASCII.hh"

//---------------------------------------------------------------------------//

class GEOutputDetectorEfficiency : public MGOutputASCII
{
public:

  //default constructor
  GEOutputDetectorEfficiency();

  //destructor
  ~GEOutputDetectorEfficiency();

  void BeginOfEventAction(const G4Event*);
  void BeginOfRunAction();
  void EndOfEventAction(const G4Event*);
  void EndOfRunAction();
  void SteppingAction(const G4Step*, G4SteppingManager*);

  G4ClassificationOfNewTrack StackingAction(const G4Track*);

private:
  G4int nRunNumber;
  G4bool energyPrimaryRead;
  G4double primaryEnergy;
  G4int nEvents;
  G4int nEventsInteracting;
  G4int nEventsFullEnergy;

};

#endif

