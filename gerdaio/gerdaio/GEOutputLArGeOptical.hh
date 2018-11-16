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
 * CLASS DECLARATION:  GEOutputLArGeOptical.hh
 *
 * DESCRIPTION:
 *
 *    A class to hold the variables for LArGe setup
 *    that are needed to be saved to ntuples
 *
 * AUTHOR:  Nuno Barros (forking from Xiang Liu code)
 *
 * REVISION: MM-DD-YYYY
 *
 *   04-11-2011, N. Barros, Forked from the original GEOutputGermaniumArray code.
 *   05-18-2012, N. Barros, Renamed some variables to clean-up the code.
 *
 */

#ifndef _GEOUTPUTLArGeOPTICAL_HH
#define _GEOUTPUTLArGeOPTICAL_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"
#include "io/MGLogger.hh"

#include "G4ClassificationOfNewTrack.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryGerda.hh"
#include "gerdaio/GEOutputLArGeOpticalMessenger.hh"

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


class GEOutputLArGeOptical : public MGOutputRoot
{
public:
  //default constructor
  GEOutputLArGeOptical(G4bool isMother, G4bool needt, G4bool larInst = false);

  //copy constructor
  GEOutputLArGeOptical(const GEOutputLArGeOptical &);

  //destructor
  ~GEOutputLArGeOptical();

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
  void WritePartialEvent(const G4Event*);
//{MGLog(routine)<<"now processing tracks in waiting stack"<<endlog;};
  void ResetPartialEvent(const G4Event*);
  void PrepareNewEvent(const G4Event*);

  void RequireHit(G4bool flag)
    { fFLAG_RequireHit = flag; }

  void FillDetectorTypeMatrix();

  void SetKillDaughterFlag(G4bool bb){fKillDaughterFlag = bb;}
  G4bool GetKillDaughterFlag(){return fKillDaughterFlag;}

  void SetDoKillNeutrinos(G4bool bb){fDoKillNeutrinos = bb;}
  G4bool GetDoKillNeutrinos(){return fDoKillNeutrinos;}

  void SetReduceHitInfo1(G4bool bb){fReduceHitInfo1 = bb;}
  G4bool GetReduceHitInfo1(){return fReduceHitInfo1;}

  // - These methods are intended to speed up the [photon tracking simulation.
  void SetLArInstApplyCrystalEnergyCut(G4bool bb){ fLArInstApplyGeEneCut = bb;}
  G4bool GetLArInstApplyCrystalEnergyCut(){ return fLArInstApplyGeEneCut;}

  void SetLArInstKillWLSTracks(G4bool bb){ fLArInstKillWLSTrackFlag = bb;}
  G4bool GetLArInstKillWLSTracks(){ return fLArInstKillWLSTrackFlag;}

  void SetLArInstCrystalEnLow(G4double en) { fLArInstGeEneCutLow = en;}
  G4double GetLArInstCrystalEnLow() { return fLArInstGeEneCutLow;}

  void SetLArInstCrystalEnHigh(G4double en) { fLArInstGeEneCutHigh = en;}
  G4double GetLArInstCrystalEnHigh() { return fLArInstGeEneCutHigh;}

  void SetCrystalEnLow(G4double en) { fEGeHitThreshold = en;}
  G4double GetCrystalEnLow() { return fEGeHitThreshold;}

  void SetDoKillGammasOutsideCone(const G4bool bb) {fDoKillGammasOutsideCone=bb;}
  G4bool GetDoKillGammasOutsideCone() const {return fDoKillGammasOutsideCone;}
  void SetGammaConeCut_ArrayCenter(const G4ThreeVector& vv) {fGammaConeCut_ArrayCenter=vv;}
  void SetGammaConeCut_StartCutRadius(const G4double dd) {fGammaConeCut_StartCutRadius=dd;}
  void SetGammaConeCut_MaxAllowedOpeningAngle(const G4double dd) {fGammaConeCut_MaxAllowedOpeningAngle=dd;}


  void Set_WriteNumberOfEventsToOutput(const G4bool bb) {fWriteNumberOfEventsToOutput=bb;}

  // new function for filling trees
  void FillMCToNtupleTrees(const G4Event *event);
  void FillHitsToNtupleTrees(const G4Event *event);
  void FillTrajectoriesToNtupleTrees(const G4Event *event);
  void FillMotherNucleiToNtupleTrees(const G4Event *event);
  void FillNtuple();

  // new function to reclassify tracks popped from waiting list
  void NewStage();
  // new method to get some internal variables in sync with the processing
  void PrepareNewEvent();


  //private  members
private:

  static const G4int MAX_NHITS=2000;
  static const G4int MAX_NGESEG=700;
  static const G4int MAX_NGEDET=100;
  static const G4int MAX_NTRJ=2000;
  static const G4int MAX_NTRJPOINT=5000;
  static const G4int MAX_NVTX=10;

  static const G4int MAX_NPH=100000000;

  G4bool NeedTrajectoryInformation;  ///< switch to turn on trajectory storage
  G4bool StoreLArInstrumentationHits;			///< switch to turn on LAr Instrumentation hit storage

  // variables to be saved to ntuples
  Int_t    eventnumber;

  // list of primary vertexes with their position and time
  //  and how many particles are generated at these primary vertexes
  Int_t    vertex_totnum;
  Float_t  vertex_xpos[MAX_NVTX];
  Float_t  vertex_ypos[MAX_NVTX];
  Float_t  vertex_zpos[MAX_NVTX];
  Float_t  vertex_time[MAX_NVTX];
  Int_t    vertex_numparticle[MAX_NVTX];  ///< Number of particles

  //energy deposit in nitrogen and water
  Float_t ene_in_water;
  Float_t ene_in_nitrogen;
  Float_t ene_in_scint;

  Int_t neutronflag; ///< neutron flag

  // require at least hit in one segment
  Int_t fFLAG_RequireHit;

  // list of primary particles with their momentum and starting positions
  Int_t    mc_totnumparticles;
  Int_t    mc_iparticle[MAX_NTRJ];   // maximum 100 particles
  Int_t    mc_ivertex[MAX_NTRJ];     // which primary vertex
  Float_t  mc_px[MAX_NTRJ];
  Float_t  mc_py[MAX_NTRJ];
  Float_t  mc_pz[MAX_NTRJ];
  Float_t  mc_pe[MAX_NTRJ];
  Float_t  mc_ekin[MAX_NTRJ];
  Int_t    mc_id[MAX_NTRJ];  // what kind of particle

  // mother nuclei charge and baryon number
  Float_t  nuclei_baryonnumber;
  Float_t  nuclei_charge;

  // hits
  Int_t    hits_totnum;
  Float_t  hits_tote;
  Float_t  hits_edep[MAX_NHITS];
  Float_t  hits_xpos[MAX_NHITS];
  Float_t  hits_ypos[MAX_NHITS];
  Float_t  hits_zpos[MAX_NHITS];
  Int_t    hits_iddet[MAX_NHITS];  // which Ge detector this hit is in
  Int_t    hits_idseg[MAX_NHITS];  // which Ge detector segment this hit is in
  Float_t  hits_time[MAX_NHITS];
  Int_t    hits_trackid[MAX_NHITS]; // which track (in trajectory group)
  Int_t    hits_trackpdg[MAX_NHITS]; // track identity

  /*
  // hits in passivation
  Int_t    hits_passivation_totnum;
  Float_t  hits_passivation_tote;
  Float_t  hits_passivation_edep[MAX_NHITS]; 
  Float_t  hits_passivation_xpos[MAX_NHITS]; 
  Float_t  hits_passivation_ypos[MAX_NHITS]; 
  Float_t  hits_passivation_zpos[MAX_NHITS]; 
*/
  // hits in deadlayer
  Int_t    hits_deadlayer_totnum;
  Float_t  hits_deadlayer_tote;
  Float_t  hits_deadlayer_edep[MAX_NHITS];
  Float_t  hits_deadlayer_xpos[MAX_NHITS];
  Float_t  hits_deadlayer_ypos[MAX_NHITS];
  Float_t  hits_deadlayer_zpos[MAX_NHITS];

  // ge detectors and segmentation
  Int_t   seg_totnum;
  Int_t   seg_id[MAX_NGESEG];
  Int_t   seg_numhits[MAX_NGESEG];
  Float_t seg_edep[MAX_NGESEG];

  Int_t   det_totnum;
  Int_t   det_id[MAX_NGEDET];
  Int_t   det_numhits[MAX_NGEDET];
  Float_t det_edep[MAX_NGEDET];


  // Hits in the LAr
  Int_t 	hits_argon_totnum;
  Double_t	hits_argon_tote;
  Double_t	hits_argon_edep[MAX_NTRJPOINT];
  Double_t	hits_argon_xpos[MAX_NTRJPOINT];
  Double_t	hits_argon_ypos[MAX_NTRJPOINT];
  Double_t	hits_argon_zpos[MAX_NTRJPOINT];
  Double_t	hits_argon_time[MAX_NTRJPOINT];


  // hits in the LAr Instrumentation
  //  Int_t inst_ph_totnum;      // total number of registered photons
  Int_t inst_det_total_hits; // total number of registered particles
  Float_t inst_det_total_edep; // total energy deposited by the particles
  Float_t inst_ph_hits_xpos[MAX_NPH];   // x-pos of registered photon
  Float_t inst_ph_hits_ypos[MAX_NPH];   // y-pos of registered photon
  Float_t inst_ph_hits_zpos[MAX_NPH];   // z-pos of registered photon
  Float_t inst_ph_energy[MAX_NPH];   // energy of registered photon
  Float_t inst_ph_wave[MAX_NPH];   // wavelength of registered photon
  Float_t inst_part_num[MAX_NPH];   // number of hitted PMT
  Float_t inst_ph_hits_xdir[MAX_NPH];   // x-dirof registered photon
  Float_t inst_ph_hits_ydir[MAX_NPH];   // y-dir of registered photon
  Float_t inst_ph_hits_zdir[MAX_NPH];   // z-dir of registered photon
  Float_t edep_in_LArInst;

  // following are temporary variables
  float edep_allgedetreally[MAX_NGEDET]; //really used for the detectors
  int   numhits_allgedetreally[MAX_NGEDET]; //really used for the detectors

  float edep_allgedet[MAX_NGESEG]; //used for the segments
  int   numhits_allgedet[MAX_NGESEG]; //used for the segments




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
  float trj_mass[MAX_NTRJ];
  int   trj_leptonnumber[MAX_NTRJ];
  int   trj_baryonnumber[MAX_NTRJ];

  // points of the trajectories
  int   trjp_totnum;
  float trjp_xpos[MAX_NTRJPOINT];
  float trjp_ypos[MAX_NTRJPOINT];
  float trjp_zpos[MAX_NTRJPOINT];
  float trjp_de[MAX_NTRJPOINT];
  float trjp_steplength[MAX_NTRJPOINT];
  int   trjp_insidege[MAX_NTRJPOINT];
  int   trjp_processid[MAX_NTRJPOINT];
  int   trjp_vol[MAX_NTRJPOINT];
  char trjp_vol_name[MAX_NTRJPOINT][255];

  G4int fArrayDetectorTypeMatrix[19][5]; 

  std::ofstream fout;

  G4Navigator* gNavigator;

  G4bool fKillDaughterFlag;
  G4bool fRunInitialized;
  G4bool fDoKillNeutrinos;

  G4bool fReduceHitInfo1;

  G4bool             fDoKillGammasOutsideCone;
  G4ThreeVector      fGammaConeCut_ArrayCenter;
  G4double           fGammaConeCut_StartCutRadius;
  G4double           fGammaConeCut_MaxAllowedOpeningAngle;

  G4bool             fWriteNumberOfEventsToOutput;

  GEOutputLArGeOpticalMessenger* fMessenger;

  // variable to keep track of the stage of the event processing
  G4int fStage;

  // NB - Added to control the application of crystal energy cuts
  /**
   * The reason is that events that hit on low energies will not be a background for Qbb. Therefore we can disregard the
   * induced background by these events.
   *
   */
  G4bool fLArInstApplyGeEneCut;
  G4double fLArInstGeEneCutLow;
  G4double fLArInstGeEneCutHigh;
  G4bool fLArInstKillWLSTrackFlag;
  G4double fEGeHitThreshold;


};

#endif

