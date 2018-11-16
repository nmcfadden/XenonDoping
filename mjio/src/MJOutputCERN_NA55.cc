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
// $Id: MJOutputCERN_NA55.cc,v 1.7 2007-02-23 15:27:18 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputCERN_NA55.cc
//
/**
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 02-2006: Created, M. Marino
 * 
 */
//#include <map>

#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4String.hh"
#include "G4Step.hh"
#include "G4Neutron.hh"
#include "G4Run.hh"
#include "G4VProcess.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

#include "mjgeometry/MJGeometryCERN_NA55.hh"
#include "mjio/MJOutputCERN_NA55.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "geometry/MGGeometryDetector.hh"

using namespace std;
using namespace CLHEP;

MJOutputCERN_NA55::MJOutputCERN_NA55(G4bool isMother): MGOutputRoot(isMother) 
{
  SetSchemaDefined(false);
  SetWaveformsSaved(false);
  Clear();
  fEventNumber = 0;
  fNumCrashParticles = 0;
  fFirstTime = true;
  fParEventSuccess = new TParameter<long>("SuccessfulEvents", 0);
  fParBeamDumpLength_cm = new TParameter<double>("BDLength_cm",0.);
  fParBeamDumpDensity = new TParameter<double>("BDDensity",0.);
  fParNumCrashParticles = new TParameter<long>("Number_194Tl", 0);
  fTheSphere = NULL;
  fTheDetector = NULL;
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::Clear()
{
  fTheProcessNamePos = 0;
  fNMultiplicity = 0;   
  memset(fCosTheta, 0, kMaxMultiplicity*sizeof(double));
  memset(fPhi, 0, kMaxMultiplicity*sizeof(double));
  memset(fRadius, 0, kMaxMultiplicity*sizeof(double));
  memset(fE_MeV, 0, kMaxMultiplicity*sizeof(double));
  memset(fXMomentumDir, 0, kMaxMultiplicity*sizeof(double));
  memset(fYMomentumDir, 0, kMaxMultiplicity*sizeof(double));
  memset(fZMomentumDir, 0, kMaxMultiplicity*sizeof(double));
  memset(fTime, 0, kMaxMultiplicity*sizeof(double));
  memset(fTrackID, 0, kMaxMultiplicity*sizeof(int));
  memset(fParentID, 0, kMaxMultiplicity*sizeof(int));
  memset(fTheProcessName, 0, 
    ((kMaxCharacterLength+1)*kMaxMultiplicity)*sizeof(char));
  //fTheProcessedTrackIDs.clear();
}

//---------------------------------------------------------
//---------------------------------------------------------
MJOutputCERN_NA55::~MJOutputCERN_NA55()
{
  delete fParEventSuccess;
  delete fParBeamDumpLength_cm;
  delete fParBeamDumpDensity;
  delete fParNumCrashParticles;
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::BeginOfRunAction()
{
  fEventNumber = 0;
  if(IsMother()) OpenFile();
  DefineSchema();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::DefineSchema()
{
  if(!SchemaDefined()) {
    if(fTree == NULL) {
      if(!IsMother()) {
	MGLog(warning) << "No tree assigned to child output class." << endl;
      }
      fTree = new TTree("CERN_NA55Tree", "CERN_NA55 Neutron Tree");
    }

    //fTree = new TTree("neutronEndTree","neutronEndTree");
    fTree->Branch("nMult", &fNMultiplicity, "nMult/I");
    //fTree->Branch("x_cm", &fX_cm, "x[nMult]/D");
    //fTree->Branch("y_cm", &fY_cm, "y[nMult]/D");
    //fTree->Branch("z_cm", &fZ_cm, "z[nMult]/D");
    fTree->Branch("cosTheta", &fCosTheta, "cosTheta[nMult]/D");
    fTree->Branch("phi", &fPhi, "phi[nMult]/D");
    fTree->Branch("radius", &fRadius, "radius[nMult]/D");
    fTree->Branch("E_MeV", &fE_MeV, "E[nMult]/D");
    fTree->Branch("x_mom", &fXMomentumDir, "xMom[nMult]/D");
    fTree->Branch("y_mom", &fYMomentumDir, "yMom[nMult]/D");
    fTree->Branch("z_mom", &fZMomentumDir, "zMom[nMult]/D");
    fTree->Branch("TOF", &fTime, "TOF[nMult]/D");
    fTree->Branch("EventNumber", &fEventNumber, "EventNum/I");
    fTree->Branch("TrackID", &fTrackID, "TrackID[nMult]/I");
    fTree->Branch("ParentID", &fParentID, "ParentID[nMult]/I");
    fTree->Branch("CreatorProcess", &fTheProcessName, "CreatorProcess/C");
    SetSchemaDefined(true);
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::BeginOfEventAction(const G4Event*)
{
  if(!SchemaDefined()) {
    MGLog(fatal) << "BeginOfEventAction(): Must define schema first! "
                 << "Was BeginOfRunAction() called?" << endl;
  }
  
  Clear();
  fEventNumber++;
  //if(IsMother()) FillTree();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::RootSteppingAction(const G4Step* step)
{
  if(fFirstTime) {
     // a lot of this is to minimize unnecessary function calls, 
     // during runtime
     MGGeometryDetector* theDet = 
       MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
     MJGeometryCERN_NA55* theCERN_NA55Det = dynamic_cast<MJGeometryCERN_NA55*>(theDet);
     if(theCERN_NA55Det) {
       fTheBDHalfZLength = theCERN_NA55Det->GetBeamDumpHalfZLength();
       fParBeamDumpLength_cm->SetVal(2*fTheBDHalfZLength/cm);
       fParBeamDumpDensity->SetVal(theCERN_NA55Det->GetMaterialDensity()*cm3);
       fParBeamDumpLength_cm->Write();
       fParBeamDumpDensity->Write();
     }
     else {
       MGLog(error) << "Dynamic Cast Failed!  Is the geometry set to CERN_NA55?" << endlog;
     }
     GetPhysicalVolumes();
     if(!fTheSphere || !fTheDetector) {
       MGLog(error) << "Addresses not obtained for physical volumes!" << endlog;
     }
     fTheCrashingParticle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(81,194,0.);
     //fTheCrashingParticle_two = G4ParticleTable::GetParticleTable()->GetIon(81,188,0.);
     if(!fTheCrashingParticle) MGLog(error) << "Tl 194 not created!" << endlog;
     //if(!fTheCrashingParticle_two) MGLog(error) << "Tl 188 not created!" << endlog;
     fFirstTime = false;
  }

  G4Track* theTrack = step->GetTrack();
  const G4VPhysicalVolume* thePreVol = 
    step->GetPreStepPoint()->GetPhysicalVolume();
  const G4VPhysicalVolume* thePostVol =
    step->GetPostStepPoint()->GetPhysicalVolume();

  if(theTrack->GetDefinition() == fTheCrashingParticle)//|| theTrack->GetDefinition() == fTheCrashingParticle_two)
  {
    KillTrack(theTrack);
    fNumCrashParticles++;
  }
  else if(thePreVol == fTheSphere && thePostVol == fTheDetector)
  {
    //if(theTrack->GetDefinition() == G4Neutron::NeutronDefinition())
    if(theTrack->GetDefinition() == G4Neutron::NeutronDefinition())
    {
      GetData(step);
      //GetCreatorProcesses(theTrack);

    }
    // MGLog(trace) << "Track exiting the sphere!" << endlog;
    KillTrack(theTrack);
  }
  else if(step->GetPostStepPoint()->GetKineticEnergy() <= 1.*MeV) KillTrack(theTrack);
  //else if(theTrack->GetDefinition() == G4Neutron::NeutronDefinition())
  //{
  //  GetCreatorProcesses(theTrack);
 // }
}

//---------------------------------------------------------
//---------------------------------------------------------

void MJOutputCERN_NA55::EndOfEventAction(const G4Event*)
{
  fParEventSuccess->SetVal(fEventNumber);
  fParNumCrashParticles->SetVal(fNumCrashParticles);
  if(IsMother() && fNMultiplicity > 0) FillTree();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::EndOfRunAction()
{
  if(IsMother()) CloseFile();
  //DumpProcessInfo();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::GetData(const G4Step* aStep)
{
  // getting data for root file
  if(fNMultiplicity < kMaxMultiplicity) {
    G4Track* theTrack = aStep->GetTrack();		
    G4ThreeVector thePosition = aStep->GetPostStepPoint()->GetPosition();
    // recentering the coords
    thePosition.setZ(thePosition.z() - fTheBDHalfZLength);
    fCosTheta[fNMultiplicity] = thePosition.cosTheta();
    fPhi[fNMultiplicity] = thePosition.phi();
    fRadius[fNMultiplicity] = thePosition.mag();
    fE_MeV[fNMultiplicity] = aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
    G4ThreeVector theMomentum = aStep->GetPostStepPoint()->GetMomentumDirection();
    fXMomentumDir[fNMultiplicity] = theMomentum.x();
    fYMomentumDir[fNMultiplicity] = theMomentum.y();   
    fZMomentumDir[fNMultiplicity] = theMomentum.z();	  
    fTime[fNMultiplicity] = theTrack->GetGlobalTime()/ns; 
    fTrackID[fNMultiplicity] = theTrack->GetTrackID();
    fParentID[fNMultiplicity] = theTrack->GetParentID();
    G4String theProcess = theTrack->GetCreatorProcess()->GetProcessName();
    if(theProcess.length() > kMaxCharacterLength + 2)
    {
      // we must truncate the process name
      MGLog(warning) << "Process Name has exceeded: " << kMaxCharacterLength << endlog;
      MGLog(warning) << "Truncating name..." << endlog;
      theProcess.remove(theProcess.length() - (kMaxCharacterLength + 2));
    }
    theProcess+="\\";
    strncpy(&(fTheProcessName[fTheProcessNamePos]), theProcess.data(), theProcess.length());
    fTheProcessNamePos += theProcess.size();// + 1;
    fNMultiplicity++;
  }else{
    MGLog(warning) << "Multiplicity has exceeded: " << kMaxMultiplicity << endlog;
    MGLog(warning) << "Data is being lost..." << endlog;
    MGLog(warning) << "Please reset kMaxMultiplicity" << endlog;
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::KillTrack(G4Track* aTrack)
{ 
  aTrack->SetTrackStatus(fStopAndKill);
  aTrack->SetBelowThresholdFlag();
  aTrack->SetGoodForTrackingFlag(false);
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::WriteFile()
{
  // writing out successful event number
  // and making sure file is still open
  if(GetRootFile()) {
    fParEventSuccess->Write(fParEventSuccess->GetName(), TObject::kOverwrite);
    fParNumCrashParticles->Write(fParNumCrashParticles->GetName(), TObject::kOverwrite);
  }
  // calling parent function
  MGOutputRoot::WriteFile();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::CloseFile()
{
  // writing out successful event number
  if(GetRootFile()) {
    fParEventSuccess->Write(fParEventSuccess->GetName(), TObject::kOverwrite);
  }
  // calling parent function
  MGOutputRoot::CloseFile();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::GetPhysicalVolumes()
{
  // this is very specific to the CERN_NA55 geometry. 
  G4PhysicalVolumeStore* thePhysVolStore = G4PhysicalVolumeStore::GetInstance();
  for(unsigned int i=0; i<(*thePhysVolStore).size(); i++) {
    if(fTheSphere && fTheDetector) break;
    if((*thePhysVolStore)[i]->GetName()=="Detector") {
      fTheDetector = (*thePhysVolStore)[i];
      MGLog(trace) << "Detector address obtained." << endlog; 
    }
    else if((*thePhysVolStore)[i]->GetName()=="expSpherePhys") {
      fTheSphere = (*thePhysVolStore)[i]; 
      MGLog(trace) << "Sphere address obtained." << endlog;
    }
  }
}


//---------------------------------------------------------
//---------------------------------------------------------
/*void MJOutputCERN_NA55::GetCreatorProcesses(G4Track* theTrack)
{

  int theTrackID = static_cast<int>(theTrack->GetTrackID());
  fTheProcTrackIDIter = find(fTheProcessedTrackIDs.begin(), fTheProcessedTrackIDs.end(),theTrackID);
  if(fTheProcTrackIDIter == fTheProcessedTrackIDs.end())
  {
    // we haven't seen this ID before so we need to insert it
    fTheProcessedTrackIDs.push_back(theTrackID);
    fTheNeutronProcMapIter = fTheNeutronProcessMap.find(theTrack->GetCreatorProcess()->GetProcessName());
    if(fTheNeutronProcMapIter==fTheNeutronProcessMap.end())
    {
      // we haven't seen this process yet
      fTheNeutronProcessMap[theTrack->GetCreatorProcess()->GetProcessName()] = 1;  
    }
    else
    {
      // we have seen this process.  iterating it the fastest way.
      (fTheNeutronProcMapIter->second)++;
    }

  }

}
//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputCERN_NA55::DumpProcessInfo()
{
  
  G4cout << "Dumping Process info in MJOutputCERN_NA55::DumpProcessInfo()" <<G4endl;
  for(map<G4String, int>::const_iterator i=fTheNeutronProcessMap.begin();i!=fTheNeutronProcessMap.end();i++)
  {
    G4cout<<"Process: " << i->first << " seen " << i->second << " times." <<G4endl;
    
  }

}*/
