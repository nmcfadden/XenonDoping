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
 * $Id: MGOutputParticleValidation.cc,v 1.2 2009-01-01 00:43:50 rojo Exp $
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

#include "globals.hh"
#include "G4Event.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4GenericIon.hh"
#include "TTree.h"

#include "io/MGLogger.hh"
#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

#include "io/MGOutputParticleValidation.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MGOutputParticleValidation::MGOutputParticleValidation() :
  MGOutputRoot(true)
{
  SetSchemaDefined(false);
  fHasRadDecay = true;
}

//---------------------------------------------------------------------------//

MGOutputParticleValidation::~MGOutputParticleValidation()
{
}

//---------------------------------------------------------------------------//

void MGOutputParticleValidation::BeginOfRunAction()
{
  fEventNumber = 0;
  OpenFile();
  DefineSchema();
}

//---------------------------------------------------------------------------//

void MGOutputParticleValidation::DefineSchema()
{
  if(!SchemaDefined()) {
    if(fTree == NULL) {
      fTree = new TTree("ValidationTree", "Particle Validation Tree");
    }
    fTree->Branch("EventNumber", &fEventNumber, "EventNumber/I");
    fTree->Branch("TrackID", &fTrackID, "TrackID/I");
    fTree->Branch("ParticleID", &fParticleID, "ParticleID/I");
    fTree->Branch("ParticleKE_keV", &fParticleKE, "ParticleKE/F");
    fTree->Branch("X_cm", &fX, "X/F");
    fTree->Branch("Y_cm", &fY, "Y/F");
    fTree->Branch("Z_cm", &fZ, "Z/F");
    fTree->Branch("Px", &fPx, "Px/F");
    fTree->Branch("Py", &fPy, "Py/F");
    fTree->Branch("Pz", &fPz, "Pz/F");
    SetSchemaDefined(true);
    MGLog(debugging) << "Particle Validation Tree Schema Defined." << G4endl; 
  } 
}

//---------------------------------------------------------------------------//

void MGOutputParticleValidation::BeginOfEventAction(const G4Event*)
{
  fTrackID = 0;
  fStackNumber = 0;
  fParticleID = 0;
  fParticleKE = 0.0;
  fX = 0.0;
  fY = 0.0;
  fZ = 0.0;
  fPx = 0.0;
  fPy = 0.0;
  fPz = 0.0;
}

//---------------------------------------------------------------------------//

G4ClassificationOfNewTrack MGOutputParticleValidation::StackingAction(const G4Track* aTrack)
{
  PopulateBranches(aTrack);
  if(aTrack->GetDefinition()->GetParticleType() == "nucleus"){
    fParticleID = G4ToRoot(100000000 +
            aTrack->GetDefinition()->GetBaryonNumber() * 1000 +
            (G4int)(aTrack->GetDefinition()->GetPDGCharge() / eplus));
  } else {
    fParticleID =
    G4ToRoot(aTrack->GetDefinition()->GetPDGEncoding());
  }
  ( (G4Track*) (aTrack))->SetKineticEnergy(0.0*keV);
  FillTree();
  fTrackID++;
  return fUrgent;
}

//---------------------------------------------------------------------------// 

void MGOutputParticleValidation::PopulateBranches(const G4Track* aTrack)
{
  fParticleKE = G4ToRoot(aTrack->GetKineticEnergy()/keV);
  fPosition = aTrack->GetPosition() / cm;
  fDirection = aTrack->GetMomentumDirection();
  fX = G4ToRoot(fPosition.x());
  fY = G4ToRoot(fPosition.y());
  fZ = G4ToRoot(fPosition.z());
  fPx = fDirection.x();
  fPy = fDirection.y();
  fPz = fDirection.z();
}
//---------------------------------------------------------------------------//

void MGOutputParticleValidation::EndOfEventAction(const G4Event*)
{
  fEventNumber++;
}

//---------------------------------------------------------------------------//

void MGOutputParticleValidation::EndOfRunAction()
{
  if(IsMother()) {
    CloseFile();
    SetSchemaDefined(false);
  }
}

//---------------------------------------------------------------------------// 
void MGOutputParticleValidation::ResetPartialEvent(const G4Event*)
{
  fStackNumber++;
}

//---------------------------------------------------------------------------//
