//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
 * $Id: MJOutputDetectorEvent.hh,v 1.9 2009-02-25 01:32:36 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MJOutputDetectorEvent.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Data class for ROOT that contains all detector data for a single event.
 * IMPORTANT NOTE: This class has to be used with the MGOutpuG4Steps class
 * and has to be defined after it, since it requires a pointer to the
 * associated MGOutputG4Steps class.
 * This class also creates its own output tree, since there may be multiple 
 * detector events for a single Geant 4 events, ie. long decay chains, 
 * pile-up. There is a leaf in the tree equal to the MC event number that 
 * created this event (fMCEventNum). 
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Sat Jan 29 16:53:57 PST 2005
 * 
 * REVISION:
 * 
 * 01-29-2005, Created, R. Henning
 * 08-16-2005, Added EdepWithPulseHeightDefect, R. Henning
 * 02-20-2009, fixed nuclearMass per Chao Zhang's recommendation, A. Schubert 
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTDETECTOREVENT_HH
#define _MJOUTPUTDETECTOREVENT_HH

//---------------------------------------------------------------------------//

#include <list>
#include <math.h>
#include <string>

#include "globals.hh"

#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Navigator;
class G4Step;

class TFile;
class TTree;

class MJOutputDetectorEventData;
class MJOutputMCEvent;

class MJOutputDetectorEvent : public MGOutputRoot
{
public:

  MJOutputDetectorEvent(MJOutputMCEvent *ptr, G4bool isMother);
  ~MJOutputDetectorEvent();

  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  G4double EdepWithPulseHeightDefect(G4double endMom2, G4double edep, 
				     G4int particleID)
  {
  // Account for "inefficiency" of conversion of Ge nuclear recoil energy into
  // ionization (e-hole pairs). This inefficiency is also know as quenching
  // or pulse-height defect.
  // Parameterization from NIM A 546 (2005) 553
  // ONLY VALID FOR Ge!
  //

  G4double nuclearMass = 
    ((G4double)(particleID%10000)/10. - 0.08) * 0.931e6; // updated 20 Feb 2009
 
 
  return 0.21*1.099*pow(0.5*(edep + sqrt(endMom2)/nuclearMass),0.099)*edep;
  } 

  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step* step);
  void WritePartialEvent(const G4Event* event);
  void ResetPartialEvent(const G4Event* event);
  void WriteEvent();
  TTree *GetSpecialTree() { return fSpecialTree; } 

private:

  /**
   * Estimated time to acquire an event. 
   */
  G4double                          fDAQTime; 

  std::list<G4double>               fSeedTimes;
  std::list<std::string>                 fVolumeNames;
  TFile                             *fSpecialFile;
  TTree                             *fSpecialTree;
  MJOutputMCEvent                   *fMCEvent;
  MJOutputDetectorEventData         *fDetectorEventData;
  UInt_t                            fDetectorEventNum;
  G4Navigator                       *fNavigator;
};
#endif
