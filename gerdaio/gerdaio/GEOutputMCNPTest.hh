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
 * CLASS DECLARATION:  GEOutputMCNPTest.hh
 *
 * DESCRIPTION:
 *
 *   A class to write output for the MCNPTest on an ASCII file. This class inherits directly
 *   from MGOutputASCII
 *
 * REVISION: MM-DD-YYYY
 *
 *   01-18-2006 Created, L. Pandola
 *   13-12-2006 Revised, removed useless members and methods, L. Pandola
 */

#ifndef _GEOUTPUTMCNPTEST_HH
#define _GEOUTPUTMCNPTEST_HH 1

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "io/MGOutputASCII.hh"

//---------------------------------------------------------------------------//

class GEOutputMCNPTest : public MGOutputASCII
{
public:

  /// Default constructor
  GEOutputMCNPTest();

  /// Destructor
  ~GEOutputMCNPTest();

  void BeginOfEventAction(const G4Event*);
  void BeginOfRunAction();
  void EndOfEventAction(const G4Event*);
  void EndOfRunAction();
  void SteppingAction(const G4Step*, G4SteppingManager*);

  G4ClassificationOfNewTrack StackingAction(const G4Track*);


private:
  G4double fNeutronPathLenght;

};

#endif

