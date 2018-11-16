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
 * CLASS DECLARATION:  MGOutputAIDA.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * Abstract base class for AIDA output file. Defines basic operations on 
 * root files and trees. This class is inherited by classes that
 * define detector specific outputs.  
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//
// 11-08-2004 Created, but still not working (work in progress), Luciano
//
// --------------------------------------------------------------------------//

#ifndef _MGOUTPUTAIDA_HH
#define _MGOUTPUTAIDA_HH 1

//---------------------------------------------------------------------------//

//CLHEP
//#include <CLHEP/Units/SystemOfUnits.h>

// AIDA


// Geant 4
#include "globals.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Step.hh"

//base class
#include "io/MGVOutputManager.hh"

//---------------------------------------------------------------------------//

class MGOutputAIDA : public MGVOutputManager
{
public:

  //default constructor
  MGOutputAIDA();

  //copy constructor
  MGOutputAIDA(const MGOutputAIDA &);

  //destructor
  virtual ~MGOutputAIDA();

  void OpenFile(); // use fFileName for filename
  void CloseFile();

  /** Define Tree Schema (usually at beginning of run). 
   Detector Specific.
   Pure virtual method. MUST be defined in derived class.
  */
  virtual void DefineSchema() = 0;

  /** Action to perform at beginning and end of events and runs
   Detector specific.
   Only include actions that directly affect the AIDA tree.
   Other actions should go into the MGManagementEventAction class.
  */
  virtual void BeginOfEventAction(const G4Event*) {;}
  virtual void BeginOfRunAction() {;}
  virtual void EndOfEventAction(const G4Event*) {;}
  virtual void EndOfRunAction() {;}
  virtual void SteppingAction(const G4Step*, G4SteppingManager*){;}


protected:


private:
};

#endif
