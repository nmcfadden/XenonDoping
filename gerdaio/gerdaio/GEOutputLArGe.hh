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
 * CLASS DECLARATION:  GEOutputLArGe.hh
 *
 * DESCRIPTION:
 *
 *   A class to hold the variables for germanium array
 *   that are needed to be saved to ntuples. It differs from GEOutputGermaniumArrayOptical 
 *   because it contains some extra information (which needs CPU to be calculated!) 
 *   related to water Cerenkov and to optical photons
 *
 * AUTHOR:  Luciano
 *
 * CONTACT:
 *
 * REVISION: MM-DD-YYYY
 *
 *
 */

#ifndef _GEOUTPUTLARGE_HH
#define _GEOUTPUTLARGE_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"

#include "G4ClassificationOfNewTrack.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryGerda.hh"
//---------------------------------------------------------------------------//
// ROOT declarations
class TFile;
class TH1D;
class TTree;

//G4 classes
class G4Run;
class G4Event;
class G4DynamicParticle;
class G4Navigator;

//---------------------------------------------------------------------------//


class GEOutputLArGe : public MGOutputRoot
{
public:
  //default constructor
  GEOutputLArGe(G4bool isMother, G4bool needt);

  //copy constructor
  GEOutputLArGe(const GEOutputLArGe &);

  //destructor
  ~GEOutputLArGe();

  // public functions to replace those in MGOutputRoot
  void SetTrajectoryInformation(G4bool needt)
         {NeedTrajectoryInformation=needt;} 
  void DefineSchema();
  void BeginOfRunAction();                        // define tree here
  void BeginOfEventAction(const G4Event *event);  // copy primary particles
  void EndOfEventAction(const G4Event *event);    // hits from sensitive D
  void EndOfRunAction();                          // close tree
  void RootSteppingAction(const G4Step *step);
  G4ClassificationOfNewTrack StackingAction(const G4Track *track);

  void FillDetectorTypeMatrix();

  //private  members
private:

  static const G4int MAX_NHITS=600;
  static const G4int MAX_NGESEG=400; 
  static const G4int MAX_NTRJ=100;
  static const G4int MAX_NTRJPOINT=1000;
  static const G4int MAX_NVTX=10;

  // switch to turn on trajectory storage
  G4bool NeedTrajectoryInformation;
  
  // variables to be saved to ntuples
  Int_t    eventnumber;

  // list of primary vertexes with their position and time
  // and how many particles are generated at these primary vertexes
  Int_t    vertex_totnum;
  Float_t  vertex_xpos[MAX_NVTX];
  Float_t  vertex_ypos[MAX_NVTX];
  Float_t  vertex_zpos[MAX_NVTX];
  Float_t  vertex_time[MAX_NVTX];
  Int_t    vertex_numparticle[MAX_NVTX];  // how many particles here

  //energy deposit in argon 
  Float_t ene_in_argon;

  // list of primary particles with their momentum and starting positions
  Int_t    mc_totnumparticles;
  Int_t    mc_iparticle[MAX_NTRJ];     // maximum 100 particles
  Int_t    mc_ivertex[MAX_NTRJ];  // which primary vertex
  Float_t  mc_px[MAX_NTRJ];
  Float_t  mc_py[MAX_NTRJ];
  Float_t  mc_pz[MAX_NTRJ];
  Float_t  mc_pe[MAX_NTRJ];
  Int_t    mc_id[MAX_NTRJ];  // what kind of particle

  // hits
  Int_t    hits_totnum;
  Float_t  hits_tote;
  Float_t  hits_edep[MAX_NHITS]; 
  Float_t  hits_xpos[MAX_NHITS]; 
  Float_t  hits_ypos[MAX_NHITS]; 
  Float_t  hits_zpos[MAX_NHITS]; 
  Int_t    hits_idseg[MAX_NHITS];  // which Ge detector segment this hit is in

  // ge detectors and segmentation
  Int_t   seg_totnum;
  Int_t   seg_id[MAX_NGESEG];
  Int_t   seg_numhits[MAX_NGESEG];
  Float_t seg_edep[MAX_NGESEG];

  // following are temporary variables
  float edep_allgedet[MAX_NGESEG];
  int   numhits_allgedet[MAX_NGESEG];

  Int_t nCerenkov;
  Int_t nScintillation;
  Int_t nHitPMT;
  Int_t nDetected;

  G4double firstTime;

  static const G4int npar = 401;
  G4double frequency[npar];
  G4double efficiency[npar];
  G4bool successfulInitialization;
  TH1D* energyHisto;
  TH1D* timeHisto;


  G4Navigator* gNavigator;

  void InitializePMTEfficiency();
  G4double PMTQuantumEfficiency(G4double);

  G4int fArrayDetectorTypeMatrix[19][5];

};

#endif

