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
//                                                          
// $Id: MJOutputShielding.cc,v 1.5 2007-02-23 15:27:18 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputShielding.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * This output class is intended for use with shielding studies. A volume is
 * specified via G4Messengers, and when a particle enters that volume the 
 * particle's identity and energy are recorded, then the track is
 * fStopAndKilled.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Dec 27 09:14:19 PST 2005
 * 
 * REVISION:
 *
 * 12-27-2005, Initial submission, Kareem
 */
//---------------------------------------------------------------------------//
//

#include "io/MGLogger.hh"
#include "io/MGOutputRoot.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "globals.hh"

#include "mjio/MJOutputShieldingMessenger.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputShielding.hh"      //Present MJ Class Headers 

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJOutputShielding::MJOutputShielding( G4bool isMother ) : MGOutputRoot( isMother ), fTreePointer( 0 )
{
  SetSchemaDefined( false );
  fMessenger = new MJOutputShieldingMessenger( this );
}

MJOutputShielding::~MJOutputShielding()
{
  delete fTreePointer;
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJOutputShielding::BeginOfRunAction()
{
  // Root init.
  if(IsMother()) OpenFile();
  DefineSchema();
}

//--------------------------------------------------------------------------//

void MJOutputShielding::DefineSchema()
{

  if(!SchemaDefined()) {
    TTree *nT;

    // Create output Tree if it has not been assigned.
    if(fTree)
      nT = fTree;
    else {
      if(!IsMother())
      MGLog(warning) << "No tree assigned to child output class." << endlog;
    Char_t title[150];
    sprintf( title, "Shielding studies, volume of interest is %s", fVolumeName );
      nT = fTree = new TTree("fTree", title );
    }

    // Create Tree schema from object.
    MJOutputShieldingData *tP;
    tP = fTreePointer = new MJOutputShieldingData;

    // MC Header info.
    nT->Branch("EventID", &tP->EventID, "EventID/I");
    nT->Branch("Particle", &tP->Particle[0], "Particle/C");
    nT->Branch("Energy",&tP->Energy,"Energy/D");

    SetSchemaDefined(true);
  }
}

//--------------------------------------------------------------------------//

void MJOutputShielding::BeginOfEventAction(const G4Event *event)
{
  MJOutputShieldingData   *tP = fTreePointer;

  tP->EventID = 0;
  tP->Energy = 0.0;

  tP->EventID = G4ToRoot(event->GetEventID());
}

//---------------------------------------------------------------------------//

void MJOutputShielding::RootSteppingAction(const G4Step* step)
{
  G4Track *track = step->GetTrack();
  if( track->GetNextVolume() )
    if( track->GetNextVolume()->GetName() == fVolumeName ) {
      strncpy( fTreePointer->Particle, track->GetDefinition()->GetParticleName(), 
        track->GetDefinition()->GetParticleName().length());
      fTreePointer->Energy = track->GetDynamicParticle()->GetKineticEnergy()/keV;

      if( IsMother() )
        FillTree();
    
      track->SetTrackStatus( fStopAndKill );
    }
}
//---------------------------------------------------------------------------//

void MJOutputShielding::EndOfEventAction( const G4Event*)
{

}

//---------------------------------------------------------------------------//

void MJOutputShielding::EndOfRunAction()
{
  if(IsMother()) {
    CloseFile();
    SetSchemaDefined( false );
  }    
}

//---------------------------------------------------------------------------//
