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
// $Id: MJOutputSolidBlock.cc,v 1.5 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputSolidBlock.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 14:25:50 PDT 2004
 * 
 * REVISION:
 *
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 06-18-2005, Eliminate strstream usage in favor of ROOT's Form(). Jason
 *
 */
//---------------------------------------------------------------------------//
//

#include <iostream>

#include "TH1D.h"
#include "TTree.h"

#include "globals.hh"
#include "G4DynamicParticle.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputSolidBlock.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJOutputSolidBlock::MJOutputSolidBlock(G4bool isMother):
  MGOutputRoot(isMother), fPrimarye(0),
  feTrackDistanceHisto(0), feEdepHisto(0), 
  feVertexDistance(0), feNStepsHisto(0),
  feEdepStep(0), feEAtEnd(0), feKEnergyHisto(0), Edep(0)
{
  SetSchemaDefined(false);
}

//---------------------------------------------------------------------------//

MJOutputSolidBlock::~MJOutputSolidBlock()
{
  delete [] Edep;
}

//---------------------------------------------------------------------------//

void MJOutputSolidBlock::BeginOfEventAction(const G4Event *event)
{
    // Save information about primary particle(s)
  G4PrimaryParticle *primaryParticle;
  G4PrimaryVertex   *primaryVertex;
  NumberOfParticles = event->GetNumberOfPrimaryVertex();

  if(NumberOfParticles > fMaxNumParticles) {
    MGLog(warning) << NumberOfParticles << "particles generated. Currently"
                   << " only " << fMaxNumParticles << " are supported. "
                   << " EventID : " << event->GetEventID() << endlog;
    NumberOfParticles = fMaxNumParticles;
  }
  MGLog(debugging)<< "Saving " << NumberOfParticles<<" particle(s)." << endlog;


  G4int i;
  for(i = 0; i < NumberOfParticles; i++) {
    primaryVertex = event->GetPrimaryVertex(i);
    if(!primaryVertex) {
      MGLog(error) << "Bad primary vertex pointer." << endlog;
      MGLog(fatal) << endlog;
    }
    primaryParticle = primaryVertex->GetPrimary();
    if(!primaryParticle) {
      MGLog(error) << "Bad primary particle pointer." << endlog;
      MGLog(fatal) << endlog;
    }
    Particle[i] = G4ToRoot(primaryParticle->GetPDGcode());
    MomentumX[i] = 
      G4ToRoot(primaryParticle->GetPx() / keV);
    MomentumY[i] = 
      G4ToRoot(primaryParticle->GetPy() / keV);
    MomentumZ[i] = 
      G4ToRoot(primaryParticle->GetPz() / keV);
    PositionX[i] = G4ToRoot(primaryVertex->GetX0()/cm);
    PositionY[i] = G4ToRoot(primaryVertex->GetY0()/cm);
    PositionZ[i] = G4ToRoot(primaryVertex->GetZ0()/cm);
  }

  primaryVertex = event->GetPrimaryVertex(0);
  if(!primaryVertex) {
    MGLog(error) << "Bad primary vertex pointer." << endlog;
    MGLog(fatal) << endlog;
  }
  primaryParticle = primaryVertex->GetPrimary();
  if(!primaryParticle) {
    MGLog(error) << "Bad primary particle pointer." << endlog;
    MGLog(fatal) << endlog;
  }

  if(Edep) 
    for(i = 0; i < fEdepArraySize; i++)
       Edep[i] = 0.0;
  else {
    MGLog(error) << "Output Array not allocated!" << endlog;
    MGLog(fatal) << endlog;
  }

  EdepOutside = 0.0;
  EventNumber = G4ToRoot(event->GetEventID());
  fTrackLength = 0.0;
  feNSteps = 0;
  fPrimarye = 0;
  fPrimaryDemiseRecorded = false;
  MGLog(debugging) << "BeginOfEventAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputSolidBlock::BeginOfRunAction()
{
  if(IsMother())
    OpenFile();

  feTrackDistanceHisto = new TH1D("h1","Primary e- Track Distance",200,0.,.25);
  feEdepHisto = new TH1D("h2","Energy Deposit from Vertex", 200, 0., 0.5);
  feVertexDistance = new TH1D("h3","Distance from Vertex", 200, 0., 2.0);
  feNStepsHisto = new TH1D("h4","Number of Steps", 400, -0.5, 399.5);
  feEdepStep = new TH1D("h5","Edep/step", 200, 0.0, 100.0);
  feEAtEnd = new TH1D("h6","E at End", 100, 0.0, 10.0);
  feG4Distance = new TH1D("h7","G4 len", 500, 0., 5.0);
  feAccTrackLen = new TH1D("h8","Acc Len", 500, 0., 5.0);
  feKEnergyHisto = new TH1D("h9","Kinetic Energy", 1000, 0., 1.0);
  fEDistance = new TH1D("h10","Distance vs energy loss", 1000, 0., 2.0);

  if(!(feTrackDistanceHisto && feEdepHisto && feVertexDistance && 
       feNStepsHisto && feEdepStep &&  feEAtEnd && feG4Distance && 
       feAccTrackLen)) {
    MGLog(error) << "Could not allocate Histograms! Out of Memory?" << endlog;
    MGLog(fatal) << endlog;
  }

  dX = dY = dZ = 0.001 * cm;
  nX = nY = 100;
  nZ = 100;
  OriginX = OriginY = -0.05 * cm;
  OriginZ = -0.002 * cm;
  fOrigin = G4ThreeVector(0,0,0);
  fMaxNumParticles = 2;
  fEdepArraySize = nX * nZ;

  Particle = new Int_t[fMaxNumParticles];
  MomentumX = new Float_t[fMaxNumParticles];
  MomentumY = new Float_t[fMaxNumParticles];
  MomentumZ = new Float_t[fMaxNumParticles];
  PositionX = new Float_t[fMaxNumParticles];
  PositionY = new Float_t[fMaxNumParticles];
  PositionZ = new Float_t[fMaxNumParticles];
  if(!(Particle && MomentumX && MomentumY && MomentumZ &&
       PositionX && PositionY && PositionZ)) {
    MGLog(error) << "Could not allocate arrays. Out of Memory?" << endlog;
    MGLog(fatal) << endlog;
  }

  DefineSchema();
  MGLog(trace) << "BeginOfRunAction() Finished." << endlog;
}

//---------------------------------------------------------------------------//

G4bool MJOutputSolidBlock::ConvertCoordinateToGridPoint(
   G4ThreeVector pos, G4int *gridPoint)
{
  // Origin of gridpoint coord system is at (0,0,0)
  gridPoint[0] = (G4int)(((Float_t)pos.x() - OriginX) / dX);
  gridPoint[1] = (G4int)(((Float_t)pos.y() - OriginY) / dY);
  gridPoint[2] = (G4int)(((Float_t)pos.z() - OriginZ) / dZ);

  // Return true if point is inside limits.
  return gridPoint[0] >= 0 && gridPoint[1] >= 0 && gridPoint[2] >= 0 &&
    gridPoint[0] < nX && gridPoint[1] < nY && gridPoint[2] < nZ;
}
//---------------------------------------------------------------------------//

void MJOutputSolidBlock::DefineSchema()
{
  if(!SchemaDefined()){
    TTree *nT;
    if(fTree)
       nT = fTree;
    else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "LANL Clover Tree");
    }

    // MC Header info.
    nT->Branch("EventNumber", &EventNumber, "EventNumber/i");//Event Number
    nT->Branch("NumberOfParticles", &NumberOfParticles,"NumberOfParticles/I");
    // Num. of particles generated at beginning of event.
    nT->Branch("Particle", Particle, "Particle[NumberOfParticles]/I");
    // Enumerated particle type (PDG code). 
    nT->Branch("MomentumX", MomentumX, "MomentumX[NumberOfParticles]/F");
    nT->Branch("MomentumY", MomentumY, "MomentumY[NumberOfParticles]/F");
    nT->Branch("MomentumZ", MomentumZ, "MomentumZ[NumberOfParticles]/F");
    // Initial momentum vector of each particle (keV/c).
    nT->Branch("PositionX", PositionX, "PositionX[NumberOfParticles]/F");
    nT->Branch("PositionY", PositionY, "PositionY[NumberOfParticles]/F");
    nT->Branch("PositionZ", PositionZ, "PositionZ[NumberOfParticles]/F");
    // Initial position of each particle (cm).

    // Number of points in grid in each direction.
    nT->Branch("nX", &nX, "nX/I");
    nT->Branch("nY", &nY, "nY/I");
    nT->Branch("nZ", &nZ, "nZ/I");
 
   // Size (cm) of each step.
    nT->Branch("dX", &dX, "dX/F");
    nT->Branch("dY", &dY, "dY/F");
    nT->Branch("dZ", &dZ, "dZ/F");

   // Size (cm) of each step.
    nT->Branch("OriginX", &OriginX, "OriginX/F");
    nT->Branch("OriginY", &OriginY, "OriginY/F");
    nT->Branch("OriginZ", &OriginZ, "OriginZ/F");

   // Energy deposit grid. 
    Edep = new Float_t[nX*nZ];
    if(!Edep) {
      MGLog(error) << "Could not allocate grid array!" << endlog;
      MGLog(fatal) << endlog;
    }
    MGLog(routine) << "Allocated output array of size " << nX*nZ << endlog;

    // Figure how large a array to create in the branch. 
    // Multidimensional arrays in Root is a pain in the ass. 
    // Use a flat array instead.
    nT->Branch("Edep", Edep, Form("Edep[%d]/F", nX*nZ));
    MGLog(trace) << "Created Branch Edep: " << Form("Edep[%d]/F", nX*nZ) << endlog;

    // Energy deposit outside of Grid.
    nT->Branch("EdepOutside", &EdepOutside, "EdepOutside/F");

    SetSchemaDefined(true);
  }
}


//---------------------------------------------------------------------------//

void MJOutputSolidBlock::EndOfEventAction(const G4Event*)
{
  G4int   i;
  for(i = 0; i < fEdepArraySize; i++)
    Edep[i] /= keV;

  EdepOutside /= keV;
  
  feNStepsHisto->Fill((Double_t)(G4ToRoot(feNSteps)));
  feAccTrackLen->Fill(G4ToRoot(fTrackLength / mm));

  if(IsMother())
    FillTree();

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputSolidBlock::EndOfRunAction()
{
  if(IsMother()) CloseFile();
  MGLog(trace) << "EndOfRunAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void MJOutputSolidBlock::RootSteppingAction(const G4Step *step)
{
  G4Track *track = step->GetTrack();
  G4ThreeVector position = track->GetPosition();
  G4int gridPoint[3];
  if(ConvertCoordinateToGridPoint(position, gridPoint)) 
    Edep[gridPoint[0] * nZ + gridPoint[2]] += step->GetTotalEnergyDeposit();
  else
    EdepOutside += step->GetTotalEnergyDeposit();
  G4ThreeVector displacement = position - fOrigin;
  G4double distancemm = displacement.mag() / mm;
  fEDistance->Fill(G4ToRoot(distancemm), 
		   G4ToRoot(step->GetTotalEnergyDeposit() / keV));

  // Is this the primary e-?
  if(!fPrimarye) 
    fPrimarye = track->GetDynamicParticle();
  if(track->GetDynamicParticle() == fPrimarye){
    if(track->GetTrackStatus() != fAlive && !fPrimaryDemiseRecorded) {
      feEAtEnd->Fill(G4ToRoot(fOldKineticEnergy / keV));
      feVertexDistance->Fill(G4ToRoot(distancemm));
      feG4Distance->Fill(G4ToRoot(track->GetTrackLength() / mm));
      fPrimaryDemiseRecorded = true;
    } else 
      fOldKineticEnergy = track->GetKineticEnergy();
    feNSteps++;
    fTrackLength += step->GetStepLength();
    feTrackDistanceHisto->Fill(G4ToRoot(step->GetStepLength() / mm));
    feEdepStep->Fill(G4ToRoot(step->GetTotalEnergyDeposit() / keV));
    feKEnergyHisto->Fill(G4ToRoot(track->GetKineticEnergy() / keV));
  } else {
    feEdepHisto->Fill(G4ToRoot(step->GetTotalEnergyDeposit() / keV));
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
