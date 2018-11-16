//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
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
 * CLASS DECLARATION:  GEGeometryLArInstSD.hh
 *
 * DESCRIPTION: Implements the sensitive detector for the LAr Instrumentation.
 *
 *
 * AUTHOR: N. Barros
 *
 * REVISION:
 *
 *    05-22-2012, N. Barros, Initial commit.
 *
 */

#ifndef GEGeometryLArInstSD_h
#define GEGeometryLArInstSD_h 1

// ---------------------------------------------------------------------------

#include "GEGeometryLArInstSDHit.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class GEGeometryLArInstSD : public G4VSensitiveDetector
{

public:
  GEGeometryLArInstSD(G4String name, G4String colName);
  ~GEGeometryLArInstSD();

  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  // -- NB: Overloaded version to be used with the LAr Instrumentation.
  // It is manually called from the process manager.
  G4bool ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* /*ROhist*/);
  // Registers the instances of sensitive detectors, so that one can fix the part codes
  void RegisterDetectorInstances(G4LogicalVolume *log_vol);

  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();

private:
  GEGeometryLArInstSDHitsCollection *fHitCollection;
  std::map<G4String,G4int> logicCodeParts; // maps the logical volume to how many copies it has
  std::map<G4String,G4int>::iterator it_logicCode;
  //G4int HCID;

  //G4int fNparts;
};

#endif

