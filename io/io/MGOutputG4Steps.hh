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
 * $Id: MGOutputG4Steps.hh,v 1.13 2009-06-23 17:05:06 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MGOutputG4Steps.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * ROOT output that write all G4 relevant G4 step data to tree. This can be 
 * used as a base class for a more detailed output.
 * This class is designed for speed and simplicity, but is not extremely
 * flexible. 
 * Units: cm, keV, keV/c, ns
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Dec 16 17:03:08 PST 2004
 * 
 * REVISION:
 *
 * 11-24-2009, Added fWasPreStepWritten, A. Schubert
 * 06-18-2009, Added fWriteTrackPreStep, A. Schubert
 * 03-18-2009, Added fWriteSensitiveStepsOnly, A. Schubert
 * 02-26-2007, Fixed KillGammas and KillNeutrons, A. Schubert
 * 02-22-2007, Added KillGammas and KillNeutrons, A. Schubert
 * 03-09-2005, Added fArraysAllocated, R. Henning
 * 02-08-2005, Added Getter for fT, R. Henning
 * 02-03-2005, Added Getter for fEventID, R. Henning
 * 12-16-2004, Created, R. Henning 
 */
// --------------------------------------------------------------------------//

#ifndef _MGOUTPUTG4STEPS_HH
#define _MGOUTPUTG4STEPS_HH

//---------------------------------------------------------------------------//

#include "TTree.h"

#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Step;
class G4UImessenger;
class MGOutputG4StepsData;

class MGOutputG4Steps : public MGOutputRoot
{
public:
  MGOutputG4Steps(G4bool isMother);

  ~MGOutputG4Steps();

  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  void DumpStep();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void KillAlphas(G4bool stat) { fKillAlphas = stat; }
  void KillBetas(G4bool stat) { fKillBetas = stat; }
  void KillGammas(G4bool stat) { fKillGammas = stat; }
  void KillNeutrons(G4bool stat) { fKillNeutrons = stat; }
  void RootSteppingAction(const G4Step* step);
  void AssignG4StepsDataValues(const G4Step* step);
  void OutputDebuggingInfo(const G4Step* step);
  void SetMaxArrayLength(G4int num) { fMaxArrayLength = num; }
  void SetWriteSensitiveHitsOnly(G4bool val) {fWriteSensitiveHitsOnly = val;}
  void SetWriteSensitiveStepsOnly(G4bool val) {fWriteSensitiveStepsOnly = val;}
  void SetWriteTrackPreStep(G4bool val) {fWriteTrackPreStep = val;}
  void StopNuclei(G4bool blah) { fStopNuclei = blah; }
  void WriteEvent();
  void WritePartialEvent(const G4Event* event);
  void ResetPartialEvent(const G4Event* event);
  G4int GetEventID() { return fEventID; }
  G4int GetNumberOfSteps() 
    { return fStepIndex > fMaxArrayLength-1 ? fMaxArrayLength : fStepIndex; }
    //necessary b.c. MJOutputDetectorEvent needs access to fTotalNumberOfSteps
    //before G4Steps calculates it.
  MGOutputG4StepsData *GetG4StepsData() { return fT; }
  G4bool GetWasPreStepWritten(){ return fWasPreStepWritten; }

private:

  MGOutputG4StepsData   *fT;         // Class that defines ROOT tree.

  G4int       fMaxArrayLength;       // Space allocated in arrays.
  G4int       fStepIndex;            // Current position in arrays.
  G4int       fAvgLengthProcessName; // The average number of characters in a 
                                     // process name, including '\0'
  Int_t       fMaxProcArrayLength;   // Max length of array that saves process.
  G4bool      fOverflowReported;     // Set to true if array overflow has been
                                     // reported. 
  G4int       fEventID;              // G4 event ID for current event.
  G4bool      fWriteSensitiveHitsOnly; // Write only events with energyloss in 
                                       // sensitive volumes.
  G4bool      fWriteSensitiveStepsOnly; // Write only steps with energyloss in 
                                       // sensitive volumes.
  G4bool      fWriteTrackPreStep;      // Write prestep info for the first 
                                       // step in each track
  G4bool      fRecordPreStep;          // Flog to record prestep of first step in a track
  G4bool      fKillAlphas;             // Kill all alpha tracks outside sensitive volume after
                                       // one step.
  G4bool      fKillBetas;              // Kill all beta tracks outside sensitive volume after
                                       // one step.
  G4bool      fKillGammas;             // Kill all gamma tracks outside sensitive volume after
                                       // one step.
  G4bool      fKillNeutrons;              // Kill all neutron tracks outside sensitive volume after
                                       // one step.
  G4bool      fStopNuclei;             // Stop nuclei outside ensitive volume, but don't kill them.
                                       // This allows them to decay.
  G4UImessenger *fMessenger; 

  G4bool      fWasPreStepWritten;       // Flag whether prestep info was written
                                        // for a step

};
#endif
