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
// $Id: MJOutputSLACBD.cc,v 1.11 2007-08-17 00:52:35 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputSLACBD.cc
//
/**
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 08-2005: Created, M. Marino
 * 08-22-2005: Ported to MaGe, J. Detwiler
 * 
 */

//#include <string.h>

#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4String.hh"
#include "G4Step.hh"
#include "G4Neutron.hh"
#include "G4Run.hh"
#include "G4VProcess.hh"

#include "TMath.h"

#include "mjgeometry/MJGeometrySLACBD.hh"
#include "mjio/MJOutputSLACBD.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "geometry/MGGeometryDetector.hh"

using namespace std;

using namespace CLHEP;

MJOutputSLACBD::MJOutputSLACBD(G4bool isMother): MGOutputRoot(isMother) 
{
  SetSchemaDefined(false);
  SetWaveformsSaved(false);
  Clear();
  //fPreStepIndex = 0;
  fEventNumber = 0;
  fFirstTime = true;
  fIsForwardMoving = true;
  fNumSlabs = 0;
  fSlabNumberDataLimit = 0;
  fYBlackHolePos = -46.0*cm; // Setting limit 15 cm into steel wall 
//  for(int i=0;i<kNumProcesses;i++) fPreStepProcesses[i] = NULL;
  fParNumSlabs = new TParameter<long>("NumberOfSlabs", 0);
  fParEventSuccess = new TParameter<long>("SuccessfulEvents", 0);
}

void MJOutputSLACBD::Clear()
{
    fX_cm=0.0;
    fY_cm=0.0;
    fZ_cm=0.0;
    fE_MeV=0.0;
    fXMomentumDir=0.0;
    fYMomentumDir=0.0;
    fZMomentumDir=0.0; 
    fSlabNumber = 0;
    fTime = 0.0;
    fParticleWeight = 0.0;
    fTrackID = 0;
    fParentID = 0;
}

MJOutputSLACBD::~MJOutputSLACBD()
{
  delete fParNumSlabs;
  delete fParEventSuccess;
}

void MJOutputSLACBD::BeginOfRunAction()
{
  if(IsMother()) OpenFile();
  DefineSchema();
}

void MJOutputSLACBD::DefineSchema()
{
  if(!SchemaDefined()) {
    if(fTree == NULL) {
      if(!IsMother()) {
        MGLog(warning) << "No tree assigned to child output class." << endl;
      }
      fTree = new TTree("SLACBDTree", "SLACBD Neutron Tree");
    }

    // fTree = new TTree("neutronEndTree","neutronEndTree");
    fTree->Branch("x_cm", &fX_cm, "x/D");
    fTree->Branch("y_cm", &fY_cm, "y/D");
    fTree->Branch("z_cm", &fZ_cm, "z/D");
    fTree->Branch("E_MeV", &fE_MeV, "E/D");
    fTree->Branch("x_mom", &fXMomentumDir, "xMom/D");
    fTree->Branch("y_mom", &fYMomentumDir, "yMom/D");
    fTree->Branch("z_mom", &fZMomentumDir, "zMom/D");
    fTree->Branch("SlabNumber", &fSlabNumber, "SlabNum/I");
    fTree->Branch("TOF", &fTime, "TOF/D");
    fTree->Branch("ParticleWeight", &fParticleWeight, "Weight/D");
    fTree->Branch("EventNumber", &fEventNumber, "EventNum/I");
    fTree->Branch("TrackID", &fTrackID, "TrackID/I");
    fTree->Branch("ParentID", &fParentID, "ParentID/I");
    fTree->Branch("Forward", &fIsForwardMoving, "Forward/O");
    SetSchemaDefined(true);
  }
}

void MJOutputSLACBD::BeginOfEventAction(const G4Event*)
{
  if(!SchemaDefined()) {
    MGLog(fatal) << "BeginOfEventAction(): Must define schema first! "
                 << "Was BeginOfRunAction() called?" << endl;
  }
  
  Clear();
  fEventNumber++;
  //if(IsMother()) FillTree();
}

void MJOutputSLACBD::RootSteppingAction(const G4Step* step)
{
  if(fFirstTime) {
     // a lot of this is to minimize unnecessary function calls, 
     // during runtime
     MGGeometryDetector* theDet = 
       MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
     MJGeometrySLACBD* theSLACBDDet = dynamic_cast<MJGeometrySLACBD*>(theDet);
     if(theSLACBDDet) {
       theSLACBDDet->FillPhysVolMap(fPhysVolMap);
     }
     else {
       MGLog(error) << "Dynamic Cast Failed!  Is the geometry set to SLACBD?" << endlog;
     }
     fNumSlabs = fPhysVolMap.size() - 1;
     fTheEndOfPhysVolMap = fPhysVolMap.end();
     fParNumSlabs->SetVal(fNumSlabs);
     fParNumSlabs->Write();
     // The following correction factor is taken after running G4 and comparing
     // to data.  It corresponds to 0.133553/ft.
    // FillCorrectionMap(G4Material::GetMaterial("concrete"), 0.4854207/m);
     fFirstTime = false;
  }

  G4Track* theTrack = step->GetTrack();
  // need to set up a black hole "area" by hand.
  // I.e. we need to kill all particles going to the left (on beam axis)
  // of a certain point. 
  G4double yPos = theTrack->GetPosition().y();
  if(yPos <= fYBlackHolePos) KillTrack(theTrack);
   
  if(theTrack->GetDefinition() == G4Neutron::NeutronDefinition())
  {
    G4double theEnergy = theTrack->GetKineticEnergy();
    
    // kill neutrons below .9 MeV 
    if(theEnergy < 0.8 * MeV)
    {
      KillTrack(theTrack);
    }
    /*G4VProcess* theProcess = theTrack->GetCreatorProcess();
    G4int theTrackID = theTrack->GetTrackID();
    fNProcIt = fNeutronProcessMap.find(theProcess);
    if(fNProcIt != fNeutronProcessMap.end())
    {
      
    }
    G4cout << "******* a neutron *********" << G4endl
           << "created by " << theTrack->GetCreatorProcess()->GetProcessName() << G4endl
           << "track id: " << theTrack->GetTrackID() << G4endl;
    */
    //ReweightTheTrack(theTrack);
    G4VPhysicalVolume* aPreVol = step->GetPreStepPoint()->GetPhysicalVolume();
    G4VPhysicalVolume* aPostVol = step->GetPostStepPoint()->GetPhysicalVolume();
    if(aPreVol!=aPostVol) {  
      fThePostVolIt = fPhysVolMap.find(aPostVol);
      fThePreVolIt = fPhysVolMap.find(aPreVol);
      if(fThePostVolIt != fTheEndOfPhysVolMap) {
        // particle enters one shield part
        // making sure not leaving detector
        if(fThePreVolIt == fTheEndOfPhysVolMap 
          && fThePostVolIt->second == 0
          && fSlabNumberDataLimit <= 0) 
        {
          // case that enters 1st slab from the left
          fIsForwardMoving = true;
          GetData(step, aPostVol);
        } 
        else 
        {
          fIsForwardMoving = (fThePreVolIt!=fTheEndOfPhysVolMap 
            && ((fThePostVolIt->second - fThePreVolIt->second == 1 
            && fThePreVolIt->second >= fSlabNumberDataLimit) 
            || (fThePostVolIt->second == -1 
            && fThePreVolIt->second == fNumSlabs - 1))); 
          // case that it goes from left to right
          GetData(step, aPostVol);
        }
      }
    }
  }
}


void MJOutputSLACBD::EndOfEventAction(const G4Event*)
{
  fParEventSuccess->SetVal(fEventNumber);
}

void MJOutputSLACBD::EndOfRunAction()
{
  if(IsMother()) CloseFile();
}

void MJOutputSLACBD::GetData(const G4Step* aStep, const G4VPhysicalVolume* aVol)
{
  // getting data for root file
  G4Track* theTrack = aStep->GetTrack();
  G4ThreeVector thePosition = aStep->GetPostStepPoint()->GetPosition();
  fX_cm =  thePosition.x()/cm;
  fY_cm = thePosition.y()/cm;
  fZ_cm = thePosition.z()/cm;  
  fE_MeV = aStep->GetPostStepPoint()->GetKineticEnergy()/MeV;
  G4ThreeVector theMomentum = aStep->GetPostStepPoint()->GetMomentumDirection();
  fXMomentumDir = theMomentum.x();
  fYMomentumDir = theMomentum.y();   
  fZMomentumDir = theMomentum.z();  
  fSlabNumber = fPhysVolMap[aVol];
  fTime = theTrack->GetGlobalTime()/ns; 
  fParticleWeight = theTrack->GetWeight();
  fTrackID = theTrack->GetTrackID();
  fParentID = theTrack->GetParentID();
  if(IsMother()) FillTree();
}

void MJOutputSLACBD::KillTrack(G4Track* aTrack)
{ 
  aTrack->SetTrackStatus(fStopAndKill);
  aTrack->SetBelowThresholdFlag();
  aTrack->SetGoodForTrackingFlag(false);
}

void MJOutputSLACBD::ReweightTheTrack(G4Track* theTrack)
{
  // This function corrects the weight of a particle according
  // to the true value (which must be input).  This is to correct for incorrect attenuation.
  static G4double stepLength, correctFactor;
  static G4ThreeVector theThreeVector;
  fMaterialCorrectIt = fMaterialCorrectionMap.find(theTrack->GetStep()->GetPreStepPoint()->GetMaterial());
  if(fMaterialCorrectIt == fMaterialCorrectionMap.end()) return;
  correctFactor = fMaterialCorrectIt->second;
  theThreeVector = theTrack->GetStep()->GetPostStepPoint()->GetPosition(); 
  theThreeVector -= theTrack->GetStep()->GetPreStepPoint()->GetPosition(); 
  stepLength = theThreeVector.mag();

  // We set the post step point weight since this is the weight that gets propagated through the rest
  // of the Track.
  theTrack->GetStep()->GetPostStepPoint()->SetWeight(
    theTrack->GetStep()->GetPostStepPoint()->GetWeight()*TMath::Exp(stepLength*correctFactor));
}

void MJOutputSLACBD::FillCorrectionMap(const G4Material* theMat, G4double aCFactor)
{
  fMaterialCorrectionMap[theMat] = aCFactor;
}

void MJOutputSLACBD::WriteFile()
{
  // writing out successful event number
  // and making sure file is still open
  if(GetRootFile()) {
    fParEventSuccess->Write(fParEventSuccess->GetName(), TObject::kOverwrite);
  }
  // calling parent function
  MGOutputRoot::WriteFile();
}

void MJOutputSLACBD::CloseFile()
{
  // writing out successful event number
  if(GetRootFile()) {
    fParEventSuccess->Write(fParEventSuccess->GetName(), TObject::kOverwrite);
  }
  // calling parent function
  MGOutputRoot::CloseFile();
}
