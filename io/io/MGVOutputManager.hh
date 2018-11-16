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
 *      
 * DESCRIPTION: 
 *
 * Abstract base class for generic analysis. This class is inherited by 
 * classes that define detector specific and implementation 
 * specific outputs. This is a virtual class. 
 *
 * REVISION:
 * 
 * MM-DD-YYYY
 * 11-05-2004, Created, L. Pandola
 * 12-15-2004, Added stacking action, L. Pandola
 * 02-15-2005 Now one can re-classify the track in stacking, L. Pandola
 * 02-20-2007 Added Time Window capabilities
 * 03-10-2007 Added methods to use TrackingAction in derived classes, L. Pandola
 * 04-08-2008 Added detailed debugging info for TimeWindow functions. Jing Liu
 * 01-27-2012 Refactored to allow future updates, A. Schubert
 * 03-16-2012 Implemented importance sampling windowing.  Importance sampling
 *            windowing and time windowing should *NOT* be used together. 
 *            -- A. Schubert
 *
 */

#ifndef _MGVOUTPUTMANAGER_HH
#define _MGVOUTPUTMANAGER_HH 1

#include "globals.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ClassificationOfNewTrack.hh"

/// Base class for output management
class MGVOutputManager
{
 public:
  MGVOutputManager();
  virtual ~MGVOutputManager();
  
  /** Action to perform at beginning and end of events and runs
      Detector specific.
      Only include actions that directly affect the Root tree.
      Other actions should go into the MGManagementEventAction class.
  */
  virtual void BeginOfEventAction(const G4Event*) {}
  virtual void BeginOfRunAction() {}
  virtual void EndOfEventAction(const G4Event*) {}
  virtual void EndOfRunAction() {}
  virtual void SteppingAction(const G4Step*, G4SteppingManager*) {}
  
  /// Whether track is time windowed; sets track global time to 0 if true.
  virtual G4bool IsTrackTimeWindowed(const G4Track* aTrack);

  /// Whether track is windowed for importance sampling
  virtual G4bool IsTrackImportanceSamplingWindowed(const G4Track* aTrack);

  /// Whether track is first in a new stage
  virtual G4bool IsTrackFirstInNewStage();

  /// Track classification, called by G4UserStackingAction::ClassifyNewTrack()
  virtual G4ClassificationOfNewTrack StackingAction(const G4Track* aTrack);

  /** 
  * When urgent stack is empty, all tracks are moved from waiting to urgent
  * and this method is called. 
  */
  /// Called after tracks are moved from waiting to urgent stack
  virtual void NewStage();

  virtual void PrepareNewEvent(const G4Event* = NULL) {;}
  virtual void WritePartialEvent(const G4Event*); 
  virtual void ResetPartialEvent(const G4Event*) {;} 
  virtual void PreUserTrackingAction(const G4Track*){;};
  virtual void PostUserTrackingAction(const G4Track*){;};

  //Define detector specific tree schema
  //MUST defined in the derived class
  virtual void DefineSchema() = 0;
  virtual void OpenFile() = 0;
  virtual void CloseFile() = 0;
  // allow for one to write out files in the midst of a run 
  // (to save data being trashed by aborts)
  // By default, does nothing.
  virtual void WriteFile(){}
  
  //Setters and getters
  G4String GetFileName() {return fFileName;}
  G4bool SchemaDefined() {return fSchemaDefined;}
  void SetFileName(G4String name){fFileName = name;}
  void SetSchemaDefined(G4bool sta) {fSchemaDefined = sta;}
  void SetWaveformsSaved(G4bool saved) {fWaveformsSaved = saved;}
  G4bool WaveformsSaved() {return fWaveformsSaved;}
  
  void SetUseTimeWindow(G4bool val) {fUseTimeWindow = val;}
  G4bool GetUseTimeWindow() {return fUseTimeWindow;}
  void SetTimeWindow(G4double val) {fTimeWindow = val;}
  G4double GetTimeWindow() {return fTimeWindow;}
  void SetOffsetTime(G4double val) {fOffsetTime = val;}
  G4double GetOffsetTime() {return fOffsetTime;}

  void SetUseImportanceSamplingWindow(G4bool val) { fUseImportanceSamplingWindow = val; }
  G4bool GetUseImportanceSamplingWindow() { return fUseImportanceSamplingWindow; }
  
 protected:
  static G4String fFileName;
  
 private:
  G4bool      fSchemaDefined; //true if DefineSchema() has been run
  
  G4bool      fWaveformsSaved; //is waveform simulation data being saved?
  
 protected:
  // Time Windowing data members
  G4bool      fUseTimeWindow; //if true will enable time windowing
  G4double    fTimeWindow;    //Time Window used in Windowing.
  G4double    fOffsetTime; //Holds the cumulative deleted time for a track 
  G4double    fTempOffsetTime; //Holds the most recent deleted time for a track 
  G4bool      fHasRadDecay;   //bool to see if RadioactiveDK is valid process
  G4VProcess* fRadDecayProcPointer; //pointer to Radioactive Decay Process

  // Importance sampling windowing data members
  G4bool      fInNewStage;      // whether this is a new stage
  G4bool      fOnFirstTrack;    // whether this is the first track
  G4bool      fUseImportanceSamplingWindow; // whether to use importance sampling windowing
};

#endif
