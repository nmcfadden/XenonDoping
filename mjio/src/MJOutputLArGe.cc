//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                         MAJORANA Simulation                               //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA Collaboration. It is based on Geant4, an intellectual       //
//      property of the RD44 GEANT4 collaboration.                           //
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
// $Id: MJOutputLArGe.cc,v 1.9 2004/11/19  
//      
// CLASS IMPLEMENTATION:  MJOutputLArGe.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * widely inspired by MJOutputLANLClover (thanks Reyco !)
 * 4 segments: E1 = x,y > 0 ; E2 = x > 0, y < 0 ; 
 *             E3 = x < 0, y > 0 ; E4 = x,y < 0  
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: dimarcom
 * CONTACT: 
 * FIRST SUBMISSION: Friday November 19th 2004
 * 
 *
 */
//---------------------------------------------------------------------------//
//

//#include <CLHEP/Random/RandGauss.h>
//#include <CLHEP/Units/PhysicalConstants.h>
#include "Randomize.hh"

#include "globals.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Material.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Vector3D.hh"

#include "io/MGLogger.hh"
#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputLArGe.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

MJOutputLArGe::MJOutputLArGe(G4String, G4bool isMother):
  MGOutputRoot(isMother), fTreePointer(0)
{
  SetSchemaDefined(false);
  SetWaveformsSaved(false);
}

//---------------------------------------------------------------------------//

MJOutputLArGe::~MJOutputLArGe()
{
  delete fTreePointer;
}

//--------------------------------------------------------------------------//

void MJOutputLArGe::BeginOfEventAction(const G4Event *event)
{
  MJOutputLArGeNoPS   *tP = fTreePointer;

  fE1_MC = fE2_MC = fE3_MC = fE4_MC = fLAr_MC = fGe_MC = fNoCal_MC = 0;

  tP->EventNumber = G4ToRoot(event->GetEventID());
}

//---------------------------------------------------------------------------//

void MJOutputLArGe::BeginOfRunAction()
{
  // Get sensitive material.
  fSensitiveMaterialIndex = 
    G4Material::GetMaterial("Germanium-Nat")->GetIndex();

  fScintillatingMaterialIndex = 
    G4Material::GetMaterial("LAr")->GetIndex();

  // Root init.
  if(IsMother()) OpenFile();
  DefineSchema();

}

//--------------------------------------------------------------------------//

void MJOutputLArGe::DefineSchema()
{

  if(!SchemaDefined()) {
    TTree *nT;

    // Create output Tree if it has not been assigned.
    if(fTree)
      nT = fTree;
    else {
      if(!IsMother())
	MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "LANL Clover Tree");
    }

    // Create Tree schema from object.
    MJOutputLArGeNoPS   *tP;
    tP = fTreePointer = new MJOutputLArGeNoPS;

    // MC Header info.
    nT->Branch("EventNumber", &tP->EventNumber, "EventNumber/i");//Event Number
    nT->Branch("NumberOfParticles", &tP->NumberOfParticles,
	       "NumberOfParticles/I");
    // Num. of particles generated at beginning of event.
    nT->Branch("Particle", tP->Particle, "Particle[NumberOfParticles]/I");
    // Enumerated particle type (PDG code). 
    nT->Branch("MomentumX", tP->MomentumX, "MomentumX[NumberOfParticles]/F");
    nT->Branch("MomentumY", tP->MomentumY, "MomentumY[NumberOfParticles]/F");
    nT->Branch("MomentumZ", tP->MomentumZ, "MomentumZ[NumberOfParticles]/F");
    // Initial momentum vector of each particle (keV/c).
    nT->Branch("PositionX", tP->PositionX, "PositionX[NumberOfParticles]/F");
    nT->Branch("PositionY", tP->PositionY, "PositionY[NumberOfParticles]/F");
    nT->Branch("PositionZ", tP->PositionZ, "PositionZ[NumberOfParticles]/F");
    // Initial position of each particle (cm).

    nT->Branch("E1_MC", &tP->E1_MC, "E1_MC/F"); // Energy deposit (MC truth)
    nT->Branch("E2_MC", &tP->E2_MC, "E2_MC/F"); // in keV
    nT->Branch("E3_MC", &tP->E3_MC, "E3_MC/F"); 
    nT->Branch("E4_MC", &tP->E4_MC, "E4_MC/F"); 
    nT->Branch("LAr_MC", &tP->LAr_MC, "LAr_MC/F"); 
    nT->Branch("Ge_MC", &tP->Ge_MC, "Ge_MC/F"); 
    nT->Branch("NoCal_MC", &tP->NoCal_MC, "NoCal_MC/F"); 

    SetSchemaDefined(true);
  }
}


//---------------------------------------------------------------------------//

void MJOutputLArGe::EndOfEventAction(const G4Event *event)
{
  // Save information about primary particle(s)
  G4PrimaryParticle *primaryParticle;
  G4PrimaryVertex   *primaryVertex;
  MJOutputLArGeNoPS  *tP = fTreePointer;
  G4int numberOfParticles = event->GetNumberOfPrimaryVertex();

  if(numberOfParticles > MAX_N_PARTICLES) {
    MGLog(warning) << numberOfParticles << "particles generated. Currently"
	           << " only " << MAX_N_PARTICLES << " are supported. "
	           << " EventID : " << event->GetEventID() << endlog;
    numberOfParticles = MAX_N_PARTICLES;
  }

  for(G4int i = 0; i < numberOfParticles; i++) {
    primaryVertex = event->GetPrimaryVertex(i);
    primaryParticle = primaryVertex->GetPrimary();
    tP->Particle[i] = G4ToRoot(primaryParticle->GetPDGcode());

    if(tP->Particle[i] == 11) {
    tP->MomentumX[i] = 
      G4ToRoot(primaryParticle->GetPx() / keV);
    tP->MomentumY[i] = 
      G4ToRoot(primaryParticle->GetPy() / keV);
    tP->MomentumZ[i] = 
      G4ToRoot(primaryParticle->GetPz() / keV);
    tP->PositionX[i] = sqrt(tP->MomentumX[i] * tP->MomentumX[i] + tP->MomentumY[i] * tP->MomentumY[i] + tP->MomentumZ[i] * tP->MomentumZ[i]) ;
    tP->PositionY[i] = G4ToRoot(primaryVertex->GetY0()/cm);
    tP->PositionZ[i] = G4ToRoot(primaryVertex->GetZ0()/cm);
    }
  }

  tP->NumberOfParticles = numberOfParticles;

  // Convert and save accumulated energies in keV.
  
  tP->E1_MC = (Float_t) G4ToRoot(fE1_MC / keV);
  tP->E2_MC = (Float_t) G4ToRoot(fE2_MC / keV);
  tP->E3_MC = (Float_t) G4ToRoot(fE3_MC / keV);
  tP->E4_MC = (Float_t) G4ToRoot(fE4_MC / keV);
  tP->LAr_MC = (Float_t) G4ToRoot(fLAr_MC / keV);
  tP->Ge_MC = (Float_t) G4ToRoot(fGe_MC / keV);
  tP->NoCal_MC = (Float_t) G4ToRoot(fNoCal_MC / keV);
  
  if(IsMother())
    FillTree();
}

//---------------------------------------------------------------------------//

void MJOutputLArGe::EndOfRunAction()
{
  if(IsMother()) CloseFile();
}

//---------------------------------------------------------------------------//

void MJOutputLArGe::RootSteppingAction(const G4Step* step)
{
  G4Track *track = step->GetTrack();
  G4Material *material = track->GetMaterial();

  // Determine in which segment the step occurred. 
  if(material->GetIndex() == fSensitiveMaterialIndex){
    G4ThreeVector position = track->GetPosition();
    G4double x = position.x();
    G4double deltaE = step->GetTotalEnergyDeposit();
    if(x > 0.0) {
      if(position.y() > 0.0){
	fE1_MC += deltaE;
      } 
      else {
	fE2_MC += deltaE;
      } 
    } 
    else {
      if(position.y() > 0.0){
	fE3_MC += deltaE;
      } 
      else {
	fE4_MC += deltaE;
      } 
    } 
    fGe_MC += deltaE; // in the whole crystal
    fNoCal_MC += deltaE; // in the whole crystal
  } 
  //  G4cout << fNoCal_MC << G4endl; // output almost always 0

  // depot d'energie dans LAr
  if(material->GetIndex() == fScintillatingMaterialIndex){
    G4double deltaE = step->GetTotalEnergyDeposit();
    fLAr_MC += deltaE;
  } 

} 


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
