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
 * CLASS DECLARATION:  GEOutputGermaniumArrayOptical.hh
 *
 * DESCRIPTION:
 *
 *    A class to hold the variables for germanium array
 *    that are needed to be saved to ntuples. It differs from GEOutputGermaniumArrayOptical 
 *    because it contains some extra information (which needs CPU to be calculated!) 
 *    related to water Cerenkov and to optical photons
 *
 * AUTHOR:  Luciano
 *
 * REVISION: MM-DD-YYYY
 *
 *   02-28-2005, X. Liu, renamed ntuple variables
 *   04-03-2008, D. Lenz, Added det_totnum,det_id,det_edep,hits_iddet, now it is easy to plot spectrum of each single crystal *
 *   04-19-2016, J. Hakenmueller, * Adaption to phaseII with up to 7 slots per string
 */

#ifndef _GEOUTPUTGERMANIUMARRAYOPTICAL_HH
#define _GEOUTPUTGERMANIUMARRAYOPTICAL_HH

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
class Th1D;
class TTree;

//G4 classes
class G4Run;
class G4Event;
class G4DynamicParticle;
class G4Navigator;

//---------------------------------------------------------------------------//


class GEOutputGermaniumArrayOptical : public MGOutputRoot
{
public:
  /// Default constructor
  GEOutputGermaniumArrayOptical(G4bool isMother, G4bool needt);

  /// Copy constructor
  GEOutputGermaniumArrayOptical(const GEOutputGermaniumArrayOptical &);

  /// Destructor
  ~GEOutputGermaniumArrayOptical();

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
  void PreUserTrackingAction(const G4Track* track);

  void FillDetectorTypeMatrix();

  //private  members
private:

  static const G4int MAX_NHITS=2000;
  static const G4int MAX_NGESEG=700;
  static const G4int MAX_NGEDET=100;
  static const G4int MAX_NTRJ=2000;
  static const G4int MAX_NTRJPOINT=5000;
  static const G4int MAX_NVTX=10;

  // included by Markus for Cherenkov
  static const G4int MAX_NPH=10000000;


  TTree *nT; ///< root tree


  G4bool NeedTrajectoryInformation;  ///< switch to turn on trajectory storage

  // variables to be saved to ntuples
  Int_t    eventnumber;

  // list of primary vertexes with their position and time
  //  and how many particles are generated at these primary vertexes
  Int_t    vertex_totnum;
  Float_t  vertex_xpos[MAX_NVTX];
  Float_t  vertex_ypos[MAX_NVTX];
  Float_t  vertex_zpos[MAX_NVTX];
  Float_t  vertex_time[MAX_NVTX];
  Int_t    vertex_numparticle[MAX_NVTX];  // how many particles here

  //energy deposit in nitrogen and water
  Float_t ene_in_water;
  Float_t ene_in_nitrogen;
  Float_t ene_in_scint;

  //neutron flag
  Int_t neutronflag;
  Int_t nCerenkov; 
  Int_t muInWaterFlag;

  // list of primary particles with their momentum and starting positions
  Int_t    mc_totnumparticles;
  Int_t    mc_iparticle[MAX_NTRJ];     // maximum 100 particles
  Int_t    mc_ivertex[MAX_NTRJ];  // which primary vertex
  Float_t  mc_px[MAX_NTRJ];
  Float_t  mc_py[MAX_NTRJ];
  Float_t  mc_pz[MAX_NTRJ];
  Float_t  mc_pe[MAX_NTRJ];
  Float_t  mc_ekin[MAX_NTRJ];
  Int_t    mc_id[MAX_NTRJ];  // what kind of particle

  // hits
  Int_t    hits_totnum;
  Float_t  hits_tote;
  Float_t  hits_edep[MAX_NHITS]; 
  Float_t  hits_xpos[MAX_NHITS]; 
  Float_t  hits_ypos[MAX_NHITS]; 
  Float_t  hits_zpos[MAX_NHITS]; 
  Int_t    hits_idseg[MAX_NHITS];  // which Ge detector segment this hit is in
  Int_t    hits_iddet[MAX_NHITS];  // which Ge detector this hit is in

  // ge detectors and segmentation
  Int_t   seg_totnum;
  Int_t   seg_id[MAX_NGESEG];
  Int_t   seg_numhits[MAX_NGESEG];
  Float_t seg_edep[MAX_NGESEG];

  Int_t   det_totnum;
  Int_t   det_id[MAX_NGEDET];
  Int_t   det_numhits[MAX_NGEDET];
  Float_t det_edep[MAX_NGEDET];


  // following are temporary variables
  float edep_allgedetreally[MAX_NGEDET]; //really used for the detectors
  int   numhits_allgedetreally[MAX_NGEDET]; //really used for the detectors

  float edep_allgedet[MAX_NGESEG];
  int   numhits_allgedet[MAX_NGESEG];

  // trajectories (including secondary) and their points
  int   trj_totnum;
  int   trj_id[MAX_NTRJ];
  int   trj_pid[MAX_NTRJ];
  int   trj_pdgencode[MAX_NTRJ];
  float trj_charge[MAX_NTRJ];
  float trj_px[MAX_NTRJ];
  float trj_py[MAX_NTRJ];
  float trj_pz[MAX_NTRJ];
  int   trj_npoints[MAX_NTRJ];
  int   trj_istart[MAX_NTRJ];
  int   trj_iend[MAX_NTRJ];

  // points of the trajectories
  int   trjp_totnum;
  float trjp_xpos[MAX_NTRJPOINT];
  float trjp_ypos[MAX_NTRJPOINT];
  float trjp_zpos[MAX_NTRJPOINT];
  float trjp_de[MAX_NTRJPOINT];
  float trjp_steplength[MAX_NTRJPOINT];
  int   trjp_insidege[MAX_NTRJPOINT];

  // Cherenkov Veto
  Int_t ph_totnum;
  Int_t PMT_total_hits;
  Int_t Muon_number;
  Float_t ph_hits_xpos[MAX_NPH];
  Float_t ph_hits_ypos[MAX_NPH];
  Float_t ph_hits_zpos[MAX_NPH];
  Float_t ph_energy[MAX_NPH]; 
  Float_t ph_wave[MAX_NPH];
  Int_t ph_det_flag[MAX_NPH];
  Float_t ph_edep[MAX_NPH];
  Int_t PMT_NUM[MAX_NPH];

  G4int MaxNrDetInOneString=7;//for Phase2, has been 5 for Phase1
  G4int fArrayDetectorTypeMatrix[19][7][2]; 

  std::ofstream fout;

  G4Navigator* gNavigator;

};

#endif

