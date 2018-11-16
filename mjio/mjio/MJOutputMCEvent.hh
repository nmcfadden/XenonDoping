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
 * $Id: MJOutputMCEvent.hh,v 1.5 2007-02-21 10:19:33 rojo Exp $
 *      
 * CLASS DECLARATION:  MJOutputMCEvent.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Main MC Event output class. Contains all MC data, as well as simulated
 * detector output.
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
 * FIRST SUBMISSION: Sun Jan 30 10:12:46 PST 2005
 * 
 * REVISION:
 * 
 * 01-30-2005, Created, R. Henning
 * 09-14-2005, Added messenger, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTMCEVENT_HH
#define _MJOUTPUTMCEVENT_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Step;

class MJOutputMCEventMessenger;
class MJOutputDetectorEvent;
class MGOutputG4Steps;

class MJOutputMCEvent : public MGOutputRoot
{
public:

  MJOutputMCEvent(G4bool isMother);
  ~MJOutputMCEvent();


  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step* step);
  void WritePartialEvent(const G4Event* event);
  void ResetPartialEvent(const G4Event* event);
  MGOutputG4Steps *GetG4Steps() { return fG4Steps; }
  void             SetG4StepsOutput(G4bool sta) { fFillG4StepsTree = sta; }
 
private:
  MJOutputDetectorEvent      *fDetectorEvent;
  MGOutputG4Steps            *fG4Steps;
  MJOutputMCEventMessenger   *fMessenger;
  G4bool                     fFillG4StepsTree;
};
#endif
