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
// $Id: MGOutputGeneralSurfaceSampler.cc,v 1.4 2008-05-19 20:09:46 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputGeneralSurfaceSampler.cc
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
 * AUTHOR: R. Johnson
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISIONS:
 * 11/22/2011 Include a messenger that requires the surface to be ONLY between
 *   two of the selected volumes. Bjoern Lehnert
 * 01/14/2019 Include a messenger that allows exclusion of surfaces between
 *   volumes of the same material. Allows for a small gap between surfaces that
 *   can be set. The impetus for this is that in the MJ geometry, the N2 volume
 *   is modeled as two separate volumes, even though it is in fact a single
 *   volume. Micah Buuck
*/

#include "G4PhysicalVolumeStore.hh"
#include "G4Geantino.hh"

#include "io/MGLogger.hh"
#include "io/MGOutputGeneralSurfaceSampler.hh" 
#include "io/MGOutputGeneralSurfaceSamplerMessenger.hh"

using namespace std;

MGOutputGeneralSurfaceSampler::MGOutputGeneralSurfaceSampler() : MGOutputRoot(true)
{
  SetSchemaDefined(false);
  fMessenger = new MGOutputGeneralSurfaceSamplerMessenger( this );
  fNmax = 0;
  fOnlyBetweenSelectedVolumes = false;
  fIgnoreIdenticalMaterials = false;
  fStepSizeToIgnore = 0;
  fSavedLastStep = false;
  fLastMaterial = nullptr;
  fLastVolume = nullptr;
  fWorldVolume = nullptr;
  fDefaultVolume = nullptr;
  fDefaultVolumeName = "Detector";
}

MGOutputGeneralSurfaceSampler::~MGOutputGeneralSurfaceSampler()
{
  delete fMessenger;
}

void MGOutputGeneralSurfaceSampler::BeginOfRunAction()
{
  OpenFile();
  DefineSchema();
  MGLog(routine) << "GSS will sample surfaces of the following volumes: " << endlog;
  G4String volumes = "";
  map<const G4VPhysicalVolume*, G4bool>::iterator iPVM;
  for(iPVM = fInterestingVolMap.begin(); iPVM != fInterestingVolMap.end(); iPVM++) {
    volumes += iPVM->first->GetName();
    volumes += " ";
  }
  MGLog(routine) << volumes << endlog;

  //Find default volume
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  for(size_t n = 0; n < volStore->size(); n++) {
    if(MatchesWildcard((*volStore)[n]->GetName(), fDefaultVolumeName)) {
      fDefaultVolume = (*volStore)[n];
    }
  }

}

void MGOutputGeneralSurfaceSampler::DefineSchema()
{
  if(!SchemaDefined()) {
    if(fTree == NULL) {
      fTree = new TTree("GSSTree", "General Surface Sampler Coordinate Tree");
    }
    fTree->Branch("x_cm", &fX_cm, "x/D");
    fTree->Branch("y_cm", &fY_cm, "y/D");
    fTree->Branch("z_cm", &fZ_cm, "z/D");
    SetSchemaDefined(true);
  }
}

void MGOutputGeneralSurfaceSampler::BeginOfEventAction(const G4Event *event)
{
  if(!SchemaDefined()){
    MGLog(error) << "BeginOfEventAction(): Must define schema first! "
                 << "Was BeginOfRunActdion() called?" << endlog;
  }

  if(event->GetPrimaryVertex()->GetPrimary()->GetG4code() != G4Geantino::Geantino()){
    MGLog(error) << "Particle is not a geantino!  This i/o scheme " <<
                "requires the use of geantinos." << endlog;
    MGLog(fatal) << endlog;
  }
  fCoordVector.clear();
  fPostVolumeVector.clear();
  fPreVolumeVector.clear();
  fX_cm = 0.0;
  fY_cm = 0.0;
  fZ_cm = 0.0;
}

void MGOutputGeneralSurfaceSampler::RootSteppingAction(const G4Step* step)
{
  G4VPhysicalVolume* preStepVolume = step->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume* postStepVolume = step->GetPostStepPoint()->GetPhysicalVolume();
  
  //booleans to see if the pre and post volumes are interesting
  bool preStepVolCheck = fInterestingVolMap[preStepVolume];
  bool postStepVolCheck = fInterestingVolMap[postStepVolume];

  //If fIgnoreIdenticalMaterials is set, then ignore surfaces between parts of
  //idential material. Because the parts all have small gaps between them, we
  //have to keep track of the last step point.
  if( fIgnoreIdenticalMaterials && preStepVolume!=nullptr
      && postStepVolume!=nullptr && preStepVolume->GetMotherLogical()!=nullptr
      && postStepVolume->GetMotherLogical()!=nullptr ) {
    G4Material* preStepMaterial = step->GetPreStepPoint()->GetMaterial();
    G4ThreeVector preStepPosition = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector postStepPosition = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector stepVector = postStepPosition - preStepPosition;
    G4ThreeVector preStepNormal = preStepVolume->GetLogicalVolume()->GetSolid()->SurfaceNormal(preStepPosition);
    G4ThreeVector postStepNormal = postStepVolume->GetLogicalVolume()->GetSolid()->SurfaceNormal(postStepPosition);
    double stepDistance = (abs(stepVector.dot(preStepNormal))+abs(stepVector.dot(postStepNormal)))/2.0;

    if(postStepVolume==fDefaultVolume) {
      fLastMaterial = preStepMaterial;
      fLastVolume = preStepVolume;
    }
    G4Material* postStepMaterial = step->GetPostStepPoint()->GetMaterial();
  
    if(stepDistance<fStepSizeToIgnore) {
      if(preStepMaterial==postStepMaterial) return;
      if(preStepVolume==fDefaultVolume && fLastMaterial!=nullptr && fLastMaterial==postStepMaterial && fLastVolume!=postStepVolume) {
        if(fSavedLastStep) {
          fCoordVector.pop_back();
          fPostVolumeVector.pop_back();
          fPreVolumeVector.pop_back();
        }
        return;
      }
    }
  }

  //Now to see whether any of the volumes are interesting, and if so, that the
  //interesting volume is not a mother of the boundary volume
  if(!preStepVolCheck && !postStepVolCheck){
    fSavedLastStep = false;
    return; // neither volume at the boundary is interesting
  } 
  else if(preStepVolCheck && postStepVolCheck){
    //definitely an interesting volume to sample
    fCoordVector.push_back( step->GetPostStepPoint()->GetPosition() );
    fPostVolumeVector.push_back( step->GetPostStepPoint()->GetPhysicalVolume());
    fPreVolumeVector.push_back( step->GetPreStepPoint()->GetPhysicalVolume());
    fSavedLastStep = true;
  } 
  else if(!fOnlyBetweenSelectedVolumes){ //process not if two interesting volumes are required 
    // only one of the volumes is interesting
    //booleans to see whether one is the mother volume of the other
    G4bool postIsMotherOfpre = (preStepVolume->GetMotherLogical() == postStepVolume->GetLogicalVolume());
    G4bool preIsMotherOfpost = (postStepVolume->GetMotherLogical() == preStepVolume->GetLogicalVolume());
    if(preStepVolCheck && !preIsMotherOfpost){
      //then the prestep volume is interesting and not a mother.  golden! 
      fCoordVector.push_back( step->GetPostStepPoint()->GetPosition() );
      fPostVolumeVector.push_back( step->GetPostStepPoint()->GetPhysicalVolume());
      fPreVolumeVector.push_back( step->GetPreStepPoint()->GetPhysicalVolume());
      fSavedLastStep = true;
    } else if(postStepVolCheck && !postIsMotherOfpre) {
      //then the poststepvolume is interesting and not a mother.  golden!
      fCoordVector.push_back( step->GetPostStepPoint()->GetPosition() );
      fPostVolumeVector.push_back( step->GetPostStepPoint()->GetPhysicalVolume());
      fPreVolumeVector.push_back( step->GetPreStepPoint()->GetPhysicalVolume());
      fSavedLastStep = true;
    }
    else fSavedLastStep = false;
  }
}

void MGOutputGeneralSurfaceSampler::EndOfEventAction( const G4Event*)
{
  // at the end of an event, there's a array/vector/whatever of g4ThreeVectors
  // that are on the surface of the interesting volumes.  now to pick one, or
  // toss the bunch.  First, generate a random integer between 1 and N (N being
  // the maximum possible number of intersections).  If this random number is
  // greater than N, this event is for crap, so just end the event and onto the
  // next.  if the number is lessthan or =, then choose one of those bad boys at
  // random, and then write that coordinate to a text file.  fin!

  if(fNmax == 0) fNmax = fInterestingVolMap.size()*2;
 
  if(int(fCoordVector.size()) > fNmax) {
    MGLog(error) << "The number of intersections (" << fCoordVector.size()
                 << ") was larger than fNmax!\n"
                 << "The position disitribution will NOT be uniform!\n"
                 << "You need to increase fNmax via a call to /MG/io/gss/setMaxIntersections [nMax]" << endlog;
  }
  if(fCoordVector.size() % 2 == 1) {
    MGLog(warning) << "Got an odd number of intersections -- check your geometry and bounding volume." << endlog;
    for(int k = 0; k< int(fPreVolumeVector.size()); k++){
      MGLog(warning) << "PreStepVolume " << k << " is " << (fPreVolumeVector[k])->GetName() << endlog;
      MGLog(warning) << "PostStepVolume " << k << " is " << (fPostVolumeVector[k])->GetName() << endlog;
      MGLog(warning) << "Coordinates are (x,y,z) " << (fCoordVector[k]).x() <<
      " " << (fCoordVector[k]).y() << " " << (fCoordVector[k]).z() << endlog;
    }  
  }

  G4int r = G4int(ceil(fNmax * G4UniformRand())); // thus, r is a random integer between 1,N
  if(r <= int(fCoordVector.size())){
    G4int randindex = G4int(floor(fCoordVector.size() * G4UniformRand())); // random index
    fX_cm = (fCoordVector[randindex]).getX();
    fY_cm = (fCoordVector[randindex]).getY();
    fZ_cm = (fCoordVector[randindex]).getZ();
    if(IsMother()) FillTree();
  }
}

void MGOutputGeneralSurfaceSampler::EndOfRunAction()
{
  if(IsMother()) CloseFile();
}

void MGOutputGeneralSurfaceSampler::AddVolume(G4String aVolName)
{
  //ok, so the G4PhysicalVolumeetore stores pointers to all the physical
  //volumes.  the messenger sends a string to this function, so i have to match
  //it with the physical volume store.  For each element in the store, i'll do a
  //->GetName() and match it with the str from the messenger.  If there's a
  //match, then I'll add that pointer to the physicalvolumemap
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  bool found = false;
  for(size_t n = 0; n < volStore->size(); n++) {
    if(MatchesWildcard((*volStore)[n]->GetName(), aVolName)) {
      fInterestingVolMap[(*volStore)[n]] = true;
      found = true;
    }
  }
  if(!found) DumpNotFound(aVolName);
}

void MGOutputGeneralSurfaceSampler::RemoveVolume(G4String aVolName)
{
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  bool found = false;
  for(size_t m = 0; m < volStore->size(); m++) {
    if(MatchesWildcard((*volStore)[m]->GetName(), aVolName)) {
      fInterestingVolMap.erase((*volStore)[m]);
      found = true;
    }
  }
  if(!found) DumpNotFound(aVolName);
}


void MGOutputGeneralSurfaceSampler::SetOnlyBetweenSelectedVolumes(G4bool aBool)
{
    fOnlyBetweenSelectedVolumes = aBool;
}


void MGOutputGeneralSurfaceSampler::DumpNotFound(G4String aVolName)
{
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  MGLog(warning) << "Volume " << aVolName << " not found in PhysicalVolumeStore!\n"
                 << "Available volumes:" << endlog;
  G4String volumes = "";
  for(size_t i=0; i<volStore->size(); i++) {
    volumes += (*volStore)[i]->GetName();
    volumes += " ";
  }
  MGLog(warning) << volumes << endlog;
  
}

bool MGOutputGeneralSurfaceSampler::MatchesWildcard(G4String str, G4String wc)
{
  // Test if str matches a unix-style wildcard pattern containing ? and *.
  // The wildcard [] is currently not implemented. Note that ALL ? and *
  // in wc are interpreted as wildcards and therefore should not be
  // expected to be exactly matched. For example, if str = a*b, it will 
  // match wc = a?b.
  size_t wcPos = wc.find('*');
  if(wcPos != G4String::npos) {
    // match up until the *
    if(!MatchesWildcard(str.substr(0, wcPos), wc.substr(0, wcPos))) {
      return false;
    }
  
    // if * is the last thing in wc and the pre-G4Strings matched, then the
    // strings match
    if(wcPos == wc.size()-1) return true; 

    // for each occurence in str of the next character c after * in wc, see
    // if the substr starting from c in str matches the subst following *
    // in wc
    G4String wcpost = wc.substr(wcPos+1);
    while(1) {
      size_t wcPostPos = str.find(wcpost.substr(0,1));
      if(wcPostPos == G4String::npos) return false;
      G4String strpost = str.substr(wcPostPos);
      if(MatchesWildcard(strpost, wcpost)) return true;
      str = strpost.substr(1);
    }
  }

  // If there are no *'s, then the lengths of the strings should match
  if(str.size() != wc.size()) return false;

  // now handle ?, it's easy.
  wcPos = wc.find('?');
  if(wcPos != G4String::npos) {
    return MatchesWildcard(str.substr(0, wcPos).c_str(), wc.substr(0, wcPos).c_str()) &&
           MatchesWildcard(str.substr(wcPos+1).c_str(), wc.substr(wcPos+1).c_str());
  }

  return str == wc;
}

