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
 * CLASS DECLARATION: GEOutputTestStand.hh
 *
 * DESCRIPTION:
 *
 *   A class to hold the variables for test stand output
 *   that are needed to be saved to ntuples
 *
 * AUTHOR:  Kevin Kroeninger
 *
 * REVISION:  MM-DD-YYYY
 *   02-03-2005, Xiang, renamed the variables
 *   06-01-2005, Kevin, added coincidence flag 
 *   11-02-2005, X. Liu, added process id for each trajectory-point
 *         initstep              0
 *         LowEnRayleigh         1
 *         LowEnPhotoElec        2
 *         LowEnergyIoni         3
 *         LowEnCompton          4
 *         LowEnBrem             5
 *         hLowEIoni             6
 *         RadioactiveDecay      7
 *         Transportation        8
 *         unknown              -1
 *    03-10-2006, Xiang, added coincidence flag for munich siegfried teststand
 *    07-12-2006, Manuela, added energy deposition in collimator
 *    04-30-2007, Xiang, clean up the code by adding 5 new flags
 *                        flag_save_energy  (true by default)
 *                        flag_save_hits    (need hits information mostly)
 *                        flag_save_trjactory 
 *                           (only for complete event and special study)
 *                        flag_require_coincidence (for 2-rege setup)
 *                        flag_require_sigfried_coincidence 
 *                           (for siegfried coincidence with ReGe)
 *    10-11-2010, Alex, updates on tree variables to be written, added phi and r of hits
 *    02-17-2011, Alex, added possibility to use time window for decay chains
 */

#ifndef _GEOUTPUTTESTSTAND_HH
#define _GEOUTPUTTESTSTAND_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "io/MGOutputRoot.hh"
#include "G4ClassificationOfNewTrack.hh"

//---------------------------------------------------------------------------//
// ROOT declarations
class TFile;
class Th1D;
class TTree;

//G4 classes
class G4Run;
class G4Event;
class G4DynamicParticle;

//---------------------------------------------------------------------------//

class GEOutputTestStand : public MGOutputRoot
{
public:
  /// Default constructor
  GEOutputTestStand(G4bool isMother);
  //GEOutputTestStand(G4bool isMother, G4bool needt, G4bool energyonly);

  /// Copy constructor
  GEOutputTestStand(const GEOutputTestStand &);

  /// Destructor
  ~GEOutputTestStand();

  // public functions to replace those in MGOutputRoot
  void SetTrajectoryInformation(G4bool needt)
         {fFLAG_Save_Trajectories=needt;} 
  void DefineSchema();
  void BeginOfRunAction();                        // define tree here
  void BeginOfEventAction(const G4Event *event);  // copy primary particles
  void EndOfEventAction(const G4Event *event);    // hits from sensitive D
  void EndOfRunAction();                          // close tree
  void WritePartialEvent(const G4Event* event);
  void ResetPartialEvent(const G4Event* event);
  void RootSteppingAction(const G4Step *step);
  G4ClassificationOfNewTrack StackingAction(const G4Track *track);
  void PreUserTrackingAction(const G4Track* track);

  void SetSaveEnergyFlag(G4bool flag)
  { fFLAG_Save_Energy = flag; }
  G4bool GetSaveEnergyFlag()
  { return fFLAG_Save_Energy; }

  void SetSaveHitsFlag(G4bool flag) 
  { fFLAG_Save_Hits = flag; }
  G4bool GetSaveHitsFlag()
  { return fFLAG_Save_Hits; }

  void SetSaveTrajectoriesFlag(G4bool flag) 
  { fFLAG_Save_Trajectories = flag; }
  G4bool GetSaveTrajectoriesFlag()
  { return fFLAG_Save_Trajectories; }

  void SetCoincidenceFlag(G4bool flag)
  { fFLAG_Coincidence = flag; }
  G4bool GetCoincidenceFlag()
  { return fFLAG_Coincidence; }

  void SetSiegfriedCoincidenceFlag(G4bool flag)
  { fFLAG_SiegfriedCoincidence = flag; }
  G4bool GetSiegfriedCoincidenceFlag()
  { return fFLAG_SiegfriedCoincidence; }

  // new function for filling trees
  void FillHitsToNtupleTrees(const G4Event *event);
  void FillTrajectoriesToNtupleTrees(const G4Event *event);
  void FillNtuple();

  //private  members

private:

  static const G4int MAX_NHITS=2000;
  static const G4int MAX_NGESEG=400; 
  static const G4int MAX_NTRJ=2000;
  static const G4int MAX_NTRJPOINT=5000;
  static const G4int MAX_NVTX=10;

  
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

  //neutron flag
  Int_t neutronflag;

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
  Float_t  hits_phi [MAX_NHITS];
  Float_t  hits_r   [MAX_NHITS];
  Int_t    hits_idseg[MAX_NHITS];  // which Ge detector segment this hit is in
  Float_t  hits_time[MAX_NHITS];
  Int_t    hits_trackid[MAX_NHITS]; // which track (in trajectory group)
  Int_t    hits_trackpdg[MAX_NHITS]; // track identity

  // ge detectors and segmentation
  Int_t   seg_totnum;
  Int_t   seg_id[MAX_NGESEG];
  Int_t   seg_numhits[MAX_NGESEG];
  Float_t seg_edep[MAX_NGESEG];

  // following are temporary variables
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
  int   trjp_processid[MAX_NTRJPOINT];

  //energy deposit in collimator
  Float_t ene_in_collimator;

  G4bool fFLAG_Save_Energy;
  G4bool fFLAG_Save_Hits;
  G4bool fFLAG_Save_Trajectories;
  G4bool fFLAG_Coincidence;
  G4bool fFLAG_SiegfriedCoincidence;

};

#endif

