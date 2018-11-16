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
// $Id: MJOutputPbShield.cc,v 1.3 2008-02-26 18:26:20 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputPbShield.cc
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

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Alpha.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4PhysicalVolumeStore.hh"

#include "mjio/MJOutputPbShield.hh"
#include "io/MGLogger.hh"

using namespace std;

using namespace CLHEP;

MJOutputPbShield::MJOutputPbShield(G4bool isMother): MGOutputRoot(isMother) 
{
  SetSchemaDefined(false);
  SetWaveformsSaved(false);
  fNMultiplicity = 1;
  Clear();
  fEventNumber = 0;
  fFirstTime = true;
  fParEventSuccess = new TParameter<long>("SuccessfulEvents", 0);
  fTheInsideOfTheShield = NULL;
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::Clear()
{
  if (fNMultiplicity > 0) {
    memset(fX, 0, kMaxMultiplicity*sizeof(double));
    memset(fY, 0, kMaxMultiplicity*sizeof(double));
    memset(fZ, 0, kMaxMultiplicity*sizeof(double));
    memset(fE_MeV, 0, kMaxMultiplicity*sizeof(double));
    memset(fXMomentumDir, 0, kMaxMultiplicity*sizeof(double));
    memset(fYMomentumDir, 0, kMaxMultiplicity*sizeof(double));
    memset(fZMomentumDir, 0, kMaxMultiplicity*sizeof(double));
    memset(fWeight, 0, kMaxMultiplicity*sizeof(double));
    memset(fTrackID, 0, kMaxMultiplicity*sizeof(int));
    memset(fParentID, 0, kMaxMultiplicity*sizeof(int));
  }
  fNMultiplicity = 0;   
}

//---------------------------------------------------------
//---------------------------------------------------------
MJOutputPbShield::~MJOutputPbShield()
{
  delete fParEventSuccess;
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::BeginOfRunAction()
{
  fEventNumber = 0;
  if(IsMother()) OpenFile();
  DefineSchema();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::DefineSchema()
{
  if(!SchemaDefined()) {
    if(fTree == NULL) {
      if(!IsMother()) {
	MGLog(warning) << "No tree assigned to child output class." << endl;
      }
      fTree = new TTree("PbShieldTree", "PbShield Gamma Tree");
    }

    fTree->Branch("nMult", &fNMultiplicity, "nMult/I");
    fTree->Branch("x_cm", &fX, "x[nMult]/D");
    fTree->Branch("y_cm", &fY, "y[nMult]/D");
    fTree->Branch("z_cm", &fZ, "z[nMult]/D");
    fTree->Branch("E_MeV", &fE_MeV, "E[nMult]/D");
    fTree->Branch("x_mom", &fXMomentumDir, "xMom[nMult]/D");
    fTree->Branch("y_mom", &fYMomentumDir, "yMom[nMult]/D");
    fTree->Branch("z_mom", &fZMomentumDir, "zMom[nMult]/D");
    fTree->Branch("EventNumber", &fEventNumber, "EventNum/I");
    fTree->Branch("TrackID", &fTrackID, "TrackID[nMult]/I");
    fTree->Branch("ParentID", &fParentID, "ParentID[nMult]/I");
    fTree->Branch("weight", &fWeight, "weight[nMult]/D");
    SetSchemaDefined(true);
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::BeginOfEventAction(const G4Event* event)
{
  if(!SchemaDefined()) {
    MGLog(fatal) << "BeginOfEventAction(): Must define schema first! "
                 << "Was BeginOfRunAction() called?" << endl;
  }
  Clear();
  fEventNumber++;
  if (fNMultiplicity < kMaxMultiplicity) {
    G4PrimaryVertex* theVert = event->GetPrimaryVertex();		
    G4PrimaryParticle* thePart = theVert->GetPrimary();		
    if (thePart->GetG4code() != G4Gamma::GammaDefinition()) {
      MGLog(error) << "Initial particle not a gamma?!" << endl;
    }
    G4ThreeVector thePosition = theVert->GetPosition();
    fX[fNMultiplicity] = thePosition.x()/cm;
    fY[fNMultiplicity] = thePosition.y()/cm;
    fZ[fNMultiplicity] = thePosition.z()/cm;
    G4ThreeVector theMomentum = thePart->GetMomentum();
    fE_MeV[fNMultiplicity] = theMomentum.mag()/MeV;
    fXMomentumDir[fNMultiplicity] = theMomentum.x();
    fYMomentumDir[fNMultiplicity] = theMomentum.y();   
    fZMomentumDir[fNMultiplicity] = theMomentum.z();	  
    fNMultiplicity++;
  } else {
    MGLog(warning) << "Multiplicity has exceeded: " << kMaxMultiplicity << endlog;
    MGLog(warning) << "Data is being lost..." << endlog;
    MGLog(warning) << "Please reset kMaxMultiplicity" << endlog;
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::RootSteppingAction(const G4Step* step)
{
  if(fFirstTime) {
     // a lot of this is to minimize unnecessary function calls, 
     // during runtime
     GetPhysicalVolumes();
     if (!fTheInsideOfTheShield || !fTheOuterEdge) {
       MGLog(error) << "Address not obtained for physical volumes!" << endlog;
       MGLog(fatal) << endlog;
     }
     fFirstTime = false;
  }

  G4Track* theTrack = step->GetTrack();
  //const G4VPhysicalVolume* thePostVol =
  //  step->GetPostStepPoint()->GetPhysicalVolume();

  KillTrack(theTrack);
  /*
  if (thePostVol == fTheOuterEdge) {
    *//* Kill everything outside the outer edge of the rock. */
   /* KillTrack(theTrack);
  } else if (particle == G4Alpha::AlphaDefinition() 
      || particle == G4NeutrinoE::NeutrinoEDefinition()
      || particle == G4AntiNeutrinoE::AntiNeutrinoEDefinition()) {
    *//* Kill alphas, anti-neutrinos and neutrinos. */
   /* KillTrack(theTrack);
  } else if (particle->GetPDGEncoding() > 100000000) {
    *//* Stop nuclei. */
    /*theTrack->SetKineticEnergy(0.0);
  } //else if (thePostVol == fTheInsideOfTheShield) {
    if(particle == G4Gamma::GammaDefinition())
    {
      GetData(step);
    }
    KillTrack(theTrack);
  //}*/
}

//---------------------------------------------------------
//---------------------------------------------------------

void MJOutputPbShield::EndOfEventAction(const G4Event*)
{
  fParEventSuccess->SetVal(fEventNumber);
  if(IsMother() && fNMultiplicity > 0) FillTree();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::EndOfRunAction()
{
  if(IsMother()) CloseFile();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::GetData(const G4Step* aStep)
{
  // getting data for root file
  if (fNMultiplicity < kMaxMultiplicity) {
    G4Track* theTrack = aStep->GetTrack();		
    G4ThreeVector thePosition = aStep->GetPostStepPoint()->GetPosition();
    fX[fNMultiplicity] = thePosition.x()/cm;
    fY[fNMultiplicity] = thePosition.y()/cm;
    fZ[fNMultiplicity] = thePosition.z()/cm;
    fE_MeV[fNMultiplicity] = aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
    G4ThreeVector theMomentum = aStep->GetPostStepPoint()->GetMomentumDirection();
    fXMomentumDir[fNMultiplicity] = theMomentum.x();
    fYMomentumDir[fNMultiplicity] = theMomentum.y();   
    fZMomentumDir[fNMultiplicity] = theMomentum.z();	  
    fWeight[fNMultiplicity] = theTrack->GetWeight();
    fTrackID[fNMultiplicity] = theTrack->GetTrackID();
    fParentID[fNMultiplicity] = theTrack->GetParentID();
    fNMultiplicity++;
  } else {
    MGLog(warning) << "Multiplicity has exceeded: " << kMaxMultiplicity << endlog;
    MGLog(warning) << "Data is being lost..." << endlog;
    MGLog(warning) << "Please reset kMaxMultiplicity" << endlog;
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::KillTrack(G4Track* aTrack)
{ 
  aTrack->SetTrackStatus(fStopAndKill);
  aTrack->SetBelowThresholdFlag();
  aTrack->SetGoodForTrackingFlag(false);
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::WriteFile()
{
  // writing out successful event number
  // and making sure file is still open
  if(GetRootFile()) {
    fParEventSuccess->Write(fParEventSuccess->GetName(), TObject::kOverwrite);
  }
  // calling parent function
  MGOutputRoot::WriteFile();
}

//---------------------------------------------------------
//---------------------------------------------------------
void MJOutputPbShield::CloseFile()
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
void MJOutputPbShield::GetPhysicalVolumes()
{
  // this is very specific to the PbShield geometry. 
  G4PhysicalVolumeStore* thePhysVolStore = G4PhysicalVolumeStore::GetInstance();
  for(unsigned int i=0; i<(*thePhysVolStore).size(); i++) {
    if(fTheInsideOfTheShield && fTheOuterEdge) break;
    if((*thePhysVolStore)[i]->GetName()=="insideShield_phys") {
    //if((*thePhysVolStore)[i]->GetName()=="PbShield_00_phys") {
      fTheInsideOfTheShield = (*thePhysVolStore)[i];
      MGLog(trace) << "Inside of the shield address obtained." << endlog; 
    } else if((*thePhysVolStore)[i]->GetName()=="rockRoom_00_phys") {
      fTheOuterEdge = (*thePhysVolStore)[i];
      MGLog(trace) << "Outer edge of the rock address obtained." << endlog; 
    }
  }
}

