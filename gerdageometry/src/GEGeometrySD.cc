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
//                                                          
// $Id: GEGeometrySD.cc,v 1.10 2009-06-16 14:42:20 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 *
 * AUTHOR:  Xiang Liu
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * 30-11-2004, Luciano & Claudia, Registered SD for Water and Nitrogen
 * 01-12-2004, Luciano. Bug fixed
 * 09-12-2004, Claudia. Registered SD for plastic scintillator
 * 07-12-2006, Manuela, added Collimator as sensitive volume
 * 04-02-2007, Luciano, added PhysicalVolume in Hits
 * 02-11-2007, Daniel, , added Worldposition 
 * 06-16-2009, Luciano, added ParentID in Hits
 * 05-23-2012, Nuno,	Removed the LAr Instrumentation from this class. Now it has a dedicated SD class.
 */

//---------------------------------------------------------------------------//
//
#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "io/MGLogger.hh"

#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//---------------------------------------------------------------------------//

GEGeometrySD::GEGeometrySD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  //MGLog(debugging) << "Detector " << name << endlog;
  if (name == "/mydet/gerda/N2buffer") HCname = "NitrogenHC";
  else if (name == "/mydet/gerda/waterbuffer") HCname = "WaterHC";
  else if (name == "/mydet/gerda/gecrystal") HCname = "GermaniumHC";
  else if (name == "/mydet/gerda/passivation") HCname = "PassivationHC";
  else if (name == "/mydet/gerda/deadlayer") HCname = "DeadLayerHC";
  else if (name == "/mydet/gerda/collimatorsd") HCname = "CollimatorHC";
  else if (name == "/mydet/gerda/scintPlate") HCname = "ScintHC";
  collectionName.insert(HCname);
}

GEGeometrySD::~GEGeometrySD(){;}

void GEGeometrySD::Initialize(G4HCofThisEvent* HCE)
{ 
  for (G4int i=0;i<(G4int)collectionName.size();i++)
    {

      G4int HCID = -1;

      HitsCollection = new GEGeometrySDHitsCollection
	(SensitiveDetectorName,collectionName[i]);
      if(HCID<0)
	{ 
	  HCID = GetCollectionID(i); 
	}
      HCE->AddHitsCollection(HCID,HitsCollection);
      MGLog(debugging) << "Initialized hits collection: " << collectionName[i] << endlog;
    }
//   MGLog(debugging) << "Number of collections: " <<  HCE->GetNumberOfCollections() << 
//     endlog;
}


G4bool GEGeometrySD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

// Ignore energy deposited by optical photons
  if (aStep->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  GEGeometrySDHit* newHit = new GEGeometrySDHit();
  newHit->SetEdep( edep );

  G4ThreeVector  worldPos =  aStep->GetPostStepPoint()->GetPosition();
  newHit->SetPos( worldPos );
  newHit->SetLocalPos( aStep->GetPostStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(worldPos) );
  newHit->SetHittime( aStep->GetPreStepPoint()->GetGlobalTime() );
//  newHit->SetEkinetic( aStep->GetKineticEnergy() );
  newHit->SetSteplength( aStep->GetStepLength() );
//  newHit->SetTracklength( aStep->GetTrackLength() );

  // how to get volume name
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  newHit->SetVolumename( theTouchable->GetVolume()->GetName() );
  newHit->SetCopynumber( theTouchable->GetVolume()->GetCopyNo() );
  newHit->SetPhysicalVolume(aStep->GetTrack()->GetVolume());

  // get track id
  newHit->SetTrackID( aStep->GetTrack()->GetTrackID() );
  newHit->SetTrackPDG( aStep->GetTrack()->GetDefinition()->GetPDGEncoding() );
  newHit->SetParentTrackID(aStep->GetTrack()->GetParentID());

//  newHit->SetTrackPDG( aStep->GetTrack()->GetDefinition() );

  HitsCollection->insert( newHit );
  return true;
}

void GEGeometrySD::EndOfEvent(G4HCofThisEvent*)
{;}

