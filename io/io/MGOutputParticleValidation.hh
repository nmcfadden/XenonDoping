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
 * $Id: MGOutputParticleValidation.hh,v 1.3 2009-01-01 00:43:50 rojo Exp $
 *      
 * CLASS DECLARATION:  MGOutputParticleValidation.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Records name and energy of particle as it's created, then sets KE to 0
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
 * AUTHOR: R.Johnson
 * CONTACT: 
 * FIRST SUBMISSION: Tue Feb 05 10:00:00 PST 2008
 * 
 * REVISION:
 *
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGOUTPUTPARTICLEVALIDATION_HH
#define _MGOUTPUTPARTICLEVALIDATION_HH

//---------------------------------------------------------------------------//

#include "TTree.h"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Step;
class G4UImessenger;
class MGOutputParticleValidationData;

class MGOutputParticleValidation : public MGOutputRoot
{
public:
  MGOutputParticleValidation();

  virtual ~MGOutputParticleValidation();

  void BeginOfRunAction();
  void DefineSchema();
  void BeginOfEventAction(const G4Event *event);
  void RootSteppingAction(const G4Step* /*step*/){}
  virtual G4ClassificationOfNewTrack StackingAction(const G4Track* aTrack);
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  
  void PopulateBranches(const G4Track* aTrack);
  void WritePartialEvent(const G4Event*){;}
  void ResetPartialEvent(const G4Event*);

private:
  Int_t         fTrackID;
  G4int         fEventNumber;
  G4int         fStackNumber;  // only used if using TimeWindowing
  Int_t         fParticleID;
  Float_t       fParticleKE;
  G4ThreeVector fPosition;
  G4ThreeVector fDirection;
  Float_t       fX;
  Float_t       fY;
  Float_t       fZ;
  Float_t       fPx;
  Float_t       fPy;
  Float_t       fPz;

};
#endif
