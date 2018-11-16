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
// $Id: GETrajectory.cc,v 1.3 2006-07-31 12:46:46 jliu Exp $
//
//
// ---------------------------------------------------------------
//
// GETrajectory.cc
//   May 10th, 2010, Added protection against NULL pointer from 
//                   GetProcessDefinedStep(), Luciano
//
// ---------------------------------------------------------------


#include "gerdaio/GETrajectory.hh"
#include "gerdaio/GETrajectoryPoint.hh"

#include "G4ParticleTable.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include <sstream>
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"

using namespace CLHEP;

// ---------------------------------------------------------------

G4Allocator<GETrajectory> GETrajectoryAllocator;

// ---------------------------------------------------------------

GETrajectory::GETrajectory()
:  positionRecord(0), fTrackID(0), fParentID(0),
   PDGEncoding( 0 ), PDGCharge(0.0), ParticleName(""),
   initialMomentum( G4ThreeVector() ),
   ParticleMass(0.0), ParticleLeptonNumber(0), ParticleBaryonNumber(0) 
{;}

// ---------------------------------------------------------------

GETrajectory::GETrajectory(const G4Track* aTrack)
{
   G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   ParticleMass = fpParticleDefinition->GetPDGMass();
   ParticleLeptonNumber = fpParticleDefinition->GetLeptonNumber();
   ParticleBaryonNumber = fpParticleDefinition->GetBaryonNumber();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   initialMomentum = aTrack->GetMomentum();
   positionRecord = new GETrajectoryPointContainer();
   // Following is for the first trajectory point
/*
   if (aTrack->GetVolume()!=0) {
     positionRecord->push_back(new GETrajectoryPoint
      (aTrack->GetPosition(),aTrack->GetVolume()->GetName()));
   }
   else {
     positionRecord->push_back(new GETrajectoryPoint
      (aTrack->GetPosition(), "tempvolume"));
   }
*/
/*
  PROBLEM to be solved: how to get volume name from aTrack
*/
   positionRecord->push_back(new GETrajectoryPoint(aTrack->GetPosition()));

}

// ---------------------------------------------------------------

GETrajectory::GETrajectory(GETrajectory & right):G4VTrajectory()
{
  ParticleName = right.ParticleName;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  initialMomentum = right.initialMomentum;
  positionRecord = new GETrajectoryPointContainer();
  ParticleMass = right.ParticleMass;
  ParticleLeptonNumber = right.ParticleLeptonNumber;
  ParticleBaryonNumber = right.ParticleBaryonNumber;

  for(size_t i=0;i<right.positionRecord->size();i++)
  {
    GETrajectoryPoint* rightPoint = (GETrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new GETrajectoryPoint(*rightPoint));
  }
}

// ---------------------------------------------------------------

GETrajectory::~GETrajectory()
{
  //  positionRecord->clearAndDestroy();
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

// ---------------------------------------------------------------

void GETrajectory::ShowTrajectory(std::ostream& os) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::ShowTrajectory(os);
  // ... or override with your own code here.
}

// ---------------------------------------------------------------

/*
// no need to overload if you are just calling the default implementation. Also
// - causes a compiler error in G4.10.
void GETrajectory::DrawTrajectory(G4int i_mode) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::DrawTrajectory(i_mode);
  // ... or override with your own code here.
}
*/

// ---------------------------------------------------------------

const std::map<G4String,G4AttDef>* GETrajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("GETrajectory",isNew);
  if (isNew) {

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");

    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");

    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");

    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","","G4double");

    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");

    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom, "Momentum of track at start of trajectory",
			      "Physics","","G4ThreeVector");

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP,"No. of points","Physics","","G4int");

  }
  return store;
}

// ---------------------------------------------------------------

std::vector<G4AttValue>* GETrajectory::CreateAttValues() const
{
  std::ostringstream ss;

  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  ss.seekp(std::ios::beg);
  ss << fTrackID << std::ends;
  values->push_back(G4AttValue("ID",ss.str(),""));

  ss.seekp(std::ios::beg);
  ss << fParentID << std::ends;
  values->push_back(G4AttValue("PID",ss.str(),""));

  values->push_back(G4AttValue("PN",ParticleName,""));

  ss.seekp(std::ios::beg);
  ss << PDGCharge << std::ends;
  values->push_back(G4AttValue("Ch",ss.str(),""));

  ss.seekp(std::ios::beg);
  ss << PDGEncoding << std::ends;
  values->push_back(G4AttValue("PDG",ss.str(),""));

  ss.seekp(std::ios::beg);
  ss << G4BestUnit(initialMomentum,"Energy") << std::ends;
  values->push_back(G4AttValue("IMom",ss.str(),""));

  ss.seekp(std::ios::beg);
  ss << GetPointEntries() << std::ends;
  values->push_back(G4AttValue("NTP",ss.str(),""));

  return values;
}

// ---------------------------------------------------------------

void GETrajectory::AppendStep(const G4Step* aStep)
{
/*
   positionRecord->push_back( new GETrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
*/
    GETrajectoryPoint* oneTrajectoryPoint = new GETrajectoryPoint();
    oneTrajectoryPoint->SetPosition(aStep->GetPostStepPoint()->GetPosition());
    oneTrajectoryPoint->SetEnergyLost(aStep->GetTotalEnergyDeposit());
    oneTrajectoryPoint->SetStepLength(aStep->GetStepLength());

    oneTrajectoryPoint->SetTrackLength(aStep->GetTrack()->GetTrackLength());

    G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
    if (theTouchable->GetVolume()!=0){
      oneTrajectoryPoint->SetVolumeName( theTouchable->GetVolume()->GetName() );
    }
    else { 
      oneTrajectoryPoint->SetVolumeName("outofworld"); 
    }

    //Protection against NULL process pointer
    if (aStep->GetPostStepPoint()->GetProcessDefinedStep())
      oneTrajectoryPoint->SetProcessName( aStep->GetPostStepPoint()
					  ->GetProcessDefinedStep()->GetProcessName() );
    else
      oneTrajectoryPoint->SetProcessName("noProcess");

    positionRecord->push_back(oneTrajectoryPoint);
}
  
// ---------------------------------------------------------------

G4ParticleDefinition* GETrajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

// ---------------------------------------------------------------

void GETrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  GETrajectory* seco = (GETrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  { 
    positionRecord->push_back((*(seco->positionRecord))[i]);
    //    positionRecord->push_back(seco->positionRecord->removeAt(1));
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}

// ---------------------------------------------------------------
