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
 * CLASS DECLARATION:  GEGeometryPMTSD.hh
 *
 * DESCRIPTION:
 *
 * AUTHOR:
 *
 * REVISION: MM-DD-YYYY
 *
 */

#ifndef GEGeometryPMTSD_h
#define GEGeometryPMTSD_h 1

// ---------------------------------------------------------------------------

#include "GEGeometryPMTSDHit.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class GEGeometryPMTSD : public G4VSensitiveDetector
{

public:
  GEGeometryPMTSD(G4String name, G4int nCells, G4String colName);
  ~GEGeometryPMTSD();

  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  // -- NB: Overloaded version to be used with the LAr Instrumentation.
  // It is manually called from the process manager.
  G4bool ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* /*ROhist*/);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();

private:
  GEGeometryPMTSDHitsCollection *PMTCollection;
  int* CellID;
  int numberOfCells;
  int HCID;
  G4double TimeInit;

};

#endif

