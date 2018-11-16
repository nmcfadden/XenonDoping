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
 * $Id: MGManagementEventAction.hh,v 1.5 2007-06-08 12:52:31 jliu Exp $
 *      
 * CLASS DECLARATION:  MJManagementEventAction.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Defines Geant 4 event action for MaGe simulation. 
 * Constructor determines detector being simulated via Geant 4 messenger:
 * (MGManagemementEventActionMessenger), creates output class and assigns
 * it to fOutputRoot pointer.
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
 * FIRST SUBMISSION: Mon Apr 12 14:59:40 PDT 2004
 * 
 * REVISION:
 * 
 * 04-12-2004: Initialized, R. Henning
 * 11-08-2004: Replaced OutputRoot with VOutputManager, Luciano
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 01-11-2005; Added getter for ReportingFrequency, Luciano
 * 06-08-2007, Added some members handling pre-waveform output, Jing Liu
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJMANAGEMENTEVENTACTION_HH
#define _MJMANAGEMENTEVENTACTION_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4Event.hh"
#include "G4UserEventAction.hh"

#include "io/MGVOutputManager.hh" 

//---------------------------------------------------------------------------//

class MGManagementEventActionMessenger;

class MGManagementEventAction :
  public G4UserEventAction
{
public:

  //default constructor
  MGManagementEventAction();

  //copy constructor
  MGManagementEventAction(const MGManagementEventAction &);

  //destructor
  ~MGManagementEventAction();

  // Geant 4 methods.
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  /**
   * Set/Get output pointer and name
   */
  void         SetOutputManager(MGVOutputManager *outr) {fOutputManager=outr;}
  void         SetOutputManagerPreWF(MGVOutputManager *outr) {fOutputManagerPreWF=outr;}
  MGVOutputManager* GetOutputManager() {return fOutputManager;}
  MGVOutputManager* GetOutputManagerPreWF() {return fOutputManagerPreWF;}
  const MGVOutputManager* GetConstOutputManager() const {return fOutputManager;}
  void         SetOutputName(const G4String name) {fOutputName = name;}
  G4String     GetOutputName() {return fOutputName;}
  void         SetReportingFrequency(G4int freq) { fReportingFrequency = freq;}
  G4int        GetReportingFrequency(){return fReportingFrequency;}
  void         SetWriteOutFileDuringRun(G4bool theBool) {fWriteOutFileDuringRun = theBool;}
  void         SetWriteOutFrequency(G4int theFreq) {fWriteOutFrequency = theFreq;}
  G4int        GetWriteOutFrequency() {return fWriteOutFrequency;}

  //protected members
protected:


  //private  members
private:

  /**
   * Messenger class for user interface.
   */
  MGManagementEventActionMessenger  *fG4Messenger;

  /**
   * Pointer to the output class. Set via User interface
   */
  MGVOutputManager* fOutputManager;
  MGVOutputManager* fOutputManagerPreWF;

  /**
   * Name of Root output schema (as selected by user)
   */
  G4String     fOutputName;

  G4int        fReportingFrequency;  // Dump event # to output every 
                                     // fReportingFrequency events.
  // set up possibility to write out file during a run (after a number of events)
  G4int fWriteOutFrequency;
  G4bool fWriteOutFileDuringRun;
};
#endif
