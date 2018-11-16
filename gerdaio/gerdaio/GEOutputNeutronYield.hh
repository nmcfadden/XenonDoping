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
 * CLASS DECLARATION:  GEOutputNeutronYield.hh
 *
 * DESCRIPTION:
 *
 *   Writes output for the jobs aiming to determine the neutron yield from high-energy
 *   muon interactions on an ASCII file. This class inherits directly from MGOutputASCII
 *
 * REVISION: MM-DD-YYYY
 *
 *   10-18-2006 Created, L. Pandola
 *   12-13-2006 Revised, Removed useless members and methods, L.Pandola
 */

#ifndef _GEOUTPUTNEUTRONYIELD_HH
#define _GEOUTPUTNEUTRONYIELD_HH 1

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "io/MGOutputASCII.hh"

//---------------------------------------------------------------------------//

class GEOutputNeutronYield : public MGOutputASCII
{
public:

  /// Default constructor
  GEOutputNeutronYield();

  /// Destructor
  ~GEOutputNeutronYield();

  void BeginOfEventAction(const G4Event*);
  void BeginOfRunAction();
  void EndOfEventAction(const G4Event*);
  void EndOfRunAction();
  void SteppingAction(const G4Step*, G4SteppingManager*);

  G4ClassificationOfNewTrack StackingAction(const G4Track*);

private:
  G4int oldParentID;
  G4int nRunNumber;
  G4double primaryEnergy;
  G4bool energyPrimaryRead;
};

#endif

