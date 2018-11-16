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
 * CLASS DECLARATION:  GEOutputGermaniumArray.hh
 *
 * DESCRIPTION:
 *
 *    A class to hold the variables for germanium array
 *    that are needed to be saved to ntuples
 *
 * AUTHOR:  Xiang Liu
 *
 * REVISION: MM-DD-YYYY
 *
 *   11-30-2004, Luciano  Added branches for energy deposit in nitrogen and water
 *   12-06-2004, C.Tomei  Added branch for energy deposit in plastic scintillator
 *   12-06-2004, Luciano  Re-naming
 *   12-15-2004, L.Pandola Added Stacking Action for flagging events containing neutrons
 *                         Added one further branch for the neutron flag
 *   12-22-2004, L.Pandola Replaced #define with static const
 *   01-12-2005, L.Pandola Added gNavigator as private member
 *   02-28-2005, X. Liu, renamed ntuple variables
 *   03-13-2005, X. Liu, added track id and hit time information for each hit
 *   05-23-2005, X. Liu, increased the hits trajectory-points array size
 *   02-11-2005, X. Liu, added process id for each trajectory-point
 *                          - initstep              0
 *                          - LowEnRayleigh         1
 *                          - LowEnPhotoElec        2
 *                          - LowEnergyIoni         3
 *                          - LowEnCompton          4
 *                          - LowEnBrem             5
 *                          - hLowEIoni             6
 *                          - RadioactiveDecay      7
 *                          - Transportation        8
 *                          - unknown              -1
 *
 *   02-07-2005, X. Liu, added particle mass, lepton and baryon numbers
 *   04-26-2006, X. Liu, increased variable array size
 *   03-07-2007, L. Pandola, further increase of the array size
 *                            moved GETrajectory code INTO specific output schemes
 *   1-28-2007, L. Pandola, Add new boolean flag to control kill/not kill of unstable
 *                            daughter nuclei, and messenger
 *   04-03-2008, D. Lenz, Added det_totnum,det_id,det_edep,hits_iddet, now it is easy to plot spectrum of each single crystal
 *   04-07-2008, X. Liu, use time stamps to separate radioactive decay events
 *   04-11-2008, X. Liu, added flag switch for saving only events with energy deposits
 *
 *   06-27-2008, L. Pandola, Changed in order to allow for multiple runs in the same execution (fRunInitialized)
 *   01-07-2009, j. schubert, "/MG/output/killGammasOutsideCone true" can be used to kill gamma tracks outside a certain cone
 *                                Example for usage:
 *                                   /MG/output/killGammasOutsideCone true
 *                                   /MG/output/GammaConeCut_ArrayCenter 0 0 19.5 cm
 *                                   /MG/output/GammaConeCut_StartCutRadius 80 cm
 *                                   /MG/output/GammaConeCut_MaxAllowedOpeningAngle 30 deg
 *                                If  a gamma is created at position vecX and has momentum vecP then it is killed in case of
 *                                 - momentum points not into a cone directed towards the array center,
 *                                   i.e. angle[vecP,GammaConeCut_ArrayCenter-vecX] < MaxAllowedOpeningAngle
 *                                 - _AND_ it has to be outside a certain spherical range arround 'GammaConeCut_ArrayCenter'
 *   03-18-2009, j. schubert, "/MG/output/WriteNumberOfEventsToFile true" can be used to write the number of events
 *                             to the output ROOT output-file
 *   11-04-2011, N. Barros, Added optional branches to store LAr Instrumentation hit information. Does not conflict with previous code.
 *   07-18-2012, N. Barros, Added new flag to track optical photons whether there is hit in Germanium or not.
 *   09-07-2012, N. Barros, Added new variables to control the new options added to the messenger.
 *   						The new options allow to customize the buffer size for TTree::AutoSave and to map the LAr Instrumentation parts to the part number.
 *   10-25-2012, N. Barros, Added new variables to keep track of energy deposited in LAr in different regions
 *   04-19-2016, J. Hakenmueller * Adaption to phaseII with up to 7 detectors per string
 *   25-04-2017, C. Wiesinger * Added CosmicRay output, moved it from txt files to TTree, added time of isotope creation
 *   20-11-2017, K. v. Sturm * Added possibility to save the macro commands used inside the output root file using a TObjString,
 *                            switch on and off using fWriteMacroToOutput
 *   16-04-2018, K. v. Sturm * add fKillLifetimeLongLivedNuclei to set live-time above which long lived nuclei are killed
 */

#ifndef _GEOUTPUTGERMANIUMARRAY_HH
#define _GEOUTPUTGERMANIUMARRAY_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"
#include "io/MGLogger.hh"

#include "G4ClassificationOfNewTrack.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryGerda.hh"
#include "gerdaio/GEOutputGermaniumArrayMessenger.hh"

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


class GEOutputGermaniumArray : public MGOutputRoot
{
public:
  //default constructor
  GEOutputGermaniumArray(G4bool isMother, G4bool needt, G4bool larInst = false);

  //copy constructor
  GEOutputGermaniumArray(const GEOutputGermaniumArray &);

  //destructor
  ~GEOutputGermaniumArray();

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

  void RequireCoincidence(G4bool flag)
    { fFLAG_RequireCoincidence = flag; }

  void FillDetectorTypeMatrix();

  void SetKillDaughterFlag(G4bool bb){fKillDaughterFlag = bb;}
  G4bool GetKillDaughterFlag(){return fKillDaughterFlag;}

  void SetKillLifetimeLongLivedNuclei(G4double ll){ fKillLifetimeLongLivedNuclei = ll;}
  G4double GetKillLifetimeLongLivedNuclei(){return fKillLifetimeLongLivedNuclei;}

  void SetDoKillNeutrinos(G4bool bb){fDoKillNeutrinos = bb;}
  G4bool GetDoKillNeutrinos(){return fDoKillNeutrinos;}

  void SetReduceHitInfo1(G4bool bb){fReduceHitInfo1 = bb;}
  G4bool GetReduceHitInfo1(){return fReduceHitInfo1;}

  void SetAddCRInformation(G4bool bb){fAddCRInformation = bb;}
  G4bool GetAddCRInformation(){return fAddCRInformation;}

  // -- NB: Added to apply hit energy cuts
  // - These methods are intended to speed up the [photon tracking simulation.
  void SetLArInstApplyCrystalEnergyCut(G4bool bb){ fLArInstApplyGeEneCut = bb;}
  G4bool GetLArInstApplyCrystalEnergyCut(){ return fLArInstApplyGeEneCut;}

  void SetLArInstApplyCrystalCoincidenceCut(G4bool bb){ fLArInstApplyGeCoincidenceCut = bb;}
  G4bool GetLArInstApplyCrystalCoincidenceCut(){ return fLArInstApplyGeCoincidenceCut;}


  /*
  void SetLArInstKillWLSTracks(G4bool bb){ fLArInstKillWLSTrackFlag = bb;}
  G4bool GetLArInstKillWLSTracks(){ return fLArInstKillWLSTrackFlag;}
   */
  void SetLArInstCrystalEnLow(G4double en) { fLArInstGeEneCutLow = en;}
  G4double GetLArInstCrystalEnLow() { return fLArInstGeEneCutLow;}

  void SetLArInstCrystalEnHigh(G4double en) { fLArInstGeEneCutHigh = en;}
  G4double GetLArInstCrystalEnHigh() { return fLArInstGeEneCutHigh;}

  void SetAlwaysTrackLArPhotons(G4bool status) { fAlwaysTrackPhotonsLAr = status;}
  G4bool GetAlwaysTrackLArPhotons() { return fAlwaysTrackPhotonsLAr;}

  void SetStopAfterNHits(G4int ph_hits) { fMaxPhHit = ph_hits;}
  G4int GetStopAfterNHits() { return fMaxPhHit;}


  void SetDoKillGammasOutsideCone(const G4bool bb) {fDoKillGammasOutsideCone=bb;}
  G4bool GetDoKillGammasOutsideCone() const {return fDoKillGammasOutsideCone;}
  void SetGammaConeCut_ArrayCenter(const G4ThreeVector& vv) {fGammaConeCut_ArrayCenter=vv;}
  void SetGammaConeCut_StartCutRadius(const G4double dd) {fGammaConeCut_StartCutRadius=dd;}
  void SetGammaConeCut_MaxAllowedOpeningAngle(const G4double dd) {fGammaConeCut_MaxAllowedOpeningAngle=dd;}


  void Set_WriteMacroToOutput(const G4bool bb) {fWriteMacroToOutput=bb;}

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
  //void PrepareNewEvent();
  void SetForceSave(G4bool status) {fForceSave =status; };
  G4bool GetForceSave() {return fForceSave; };

  void SetAutoSaveSize(G4long size /*in bytes*/) {fAutoSaveSize = size;};
  G4long GetAutoSaveSize() {return fAutoSaveSize;};

  //private  members
private:

  void StoreLArInstPartMap();

  static const G4int MAX_NHITS=5000;
  static const G4int MAX_NGESEG=700;
  static const G4int MAX_NGEDET=100;
  static const G4int MAX_NTRJ=2000;
  static const G4int MAX_NTRJPOINT=5000;
  static const G4int MAX_NVTX=10;

  static const G4int MAX_NPH=5000;

  // kill photon tracks if LArHit > something
  G4int LArHit;

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
  // require edep in at least two crystals
  Int_t fFLAG_RequireCoincidence;

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

  // hits in passivation
  Int_t    hits_passivation_totnum;
  Float_t  hits_passivation_tote;
  Float_t  hits_passivation_edep[MAX_NHITS];
  Float_t  hits_passivation_xpos[MAX_NHITS];
  Float_t  hits_passivation_ypos[MAX_NHITS];
  Float_t  hits_passivation_zpos[MAX_NHITS];

  // hits in deadlayer
  Int_t    hits_deadlayer_totnum;
  Float_t  hits_deadlayer_tote;
  Float_t  hits_deadlayer_edep[MAX_NHITS];
  Float_t  hits_deadlayer_xpos[MAX_NHITS];
  Float_t  hits_deadlayer_ypos[MAX_NHITS];
  Float_t  hits_deadlayer_zpos[MAX_NHITS];

  // hits in LAr
  Int_t    hits_LAr_totnum;
  Float_t  hits_LAr_tote;
  Float_t  hits_LAr_edep[MAX_NHITS];
  Float_t  hits_LAr_xpos[MAX_NHITS];
  Float_t  hits_LAr_ypos[MAX_NHITS];
  Float_t  hits_LAr_zpos[MAX_NHITS];



  // ge detectors and segmentation
  Int_t   seg_totnum;
  Int_t   seg_id[MAX_NGESEG];
  Int_t   seg_numhits[MAX_NGESEG];
  Float_t seg_edep[MAX_NGESEG];

  Int_t   det_totnum;
  Int_t   det_id[MAX_NGEDET];
  Int_t   det_numhits[MAX_NGEDET];
  Float_t det_edep[MAX_NGEDET];


  // hits in the LAr Instrumentation
  //  Int_t inst_ph_totnum;      // total number of registered photons
  Int_t inst_det_total_hits; // total number of registered particles
  Float_t inst_ph_hits_xpos[MAX_NPH];   // x-pos of registered photon
  Float_t inst_ph_hits_ypos[MAX_NPH];   // y-pos of registered photon
  Float_t inst_ph_hits_zpos[MAX_NPH];   // z-pos of registered photon
  Float_t inst_ph_energy[MAX_NPH];   // energy of registered photon
  Float_t inst_ph_wave[MAX_NPH];   // wavelength of registered photon
  Int_t inst_part_num[MAX_NPH];   // number of hitted PMT
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

  G4int MaxNrDetInOneString=7;//for Phase2, has been 5 for Phase1
  G4int fArrayDetectorTypeMatrix[19][7][2];

  std::ofstream fout;

  G4Navigator* gNavigator;

  G4bool fKillDaughterFlag;
  G4bool fRunInitialized;
  G4bool fDoKillNeutrinos;
  G4double fKillLifetimeLongLivedNuclei; // kill long-lived nuclei with livetime longer than <---

  G4bool fReduceHitInfo1;

  G4bool             fDoKillGammasOutsideCone;
  G4ThreeVector      fGammaConeCut_ArrayCenter;
  G4double           fGammaConeCut_StartCutRadius;
  G4double           fGammaConeCut_MaxAllowedOpeningAngle;

  G4bool             fWriteMacroToOutput;
  G4bool             fWriteNumberOfEventsToOutput;

  GEOutputGermaniumArrayMessenger* fMessenger;

  // variable to keep track of the stage of the event processing
  G4int fStage;

  /**
   * Control the application of crystal energy cuts
   *
   * The reason is that events that hit on low energies will not be a backgroun for Qbb. Therefore we can disregard the
   * induced background by these events.
   *
   */
  G4bool fLArInstApplyGeEneCut;
  G4bool fLArInstApplyGeCoincidenceCut;
  G4double fLArInstGeEneCutLow;
  G4double fLArInstGeEneCutHigh;
  /** Force optical photon tracking even if there are no hits in the germanium
   *
   */
  G4bool fAlwaysTrackPhotonsLAr;

  //Stop tracking photons after enough hits
  G4int fMaxPhHit=0;

  /**Variables to keep track of the LAr instrumentation parts
   *
   * Depending on the event, the ID of each part of the LAr instrumentation can change.
   * Therefore, the name of the volume is stored in a separate TTree to avoid waste of storage.
   * */
  TTree*	fTreeLArInstParts; /// Tree to hold the mapping of the LAr Instrumentation part to the code number
  std::map<G4int,G4String> fPartMap;
  Int_t fPart_id;
  TString fPart_name;

  /**
   * Variables to support the ability for force the ROOT file to be saved more frequently.
   * Usefull for very slow simulations that might go over any enforced timeout.
   * If the job somehow is killed, this guarantees thatat least some data was stored.
   *
   * WARNING: Use this feature with care, since it flushes the tree, so for large trees this operation becomes increasingly slow.
   */
  G4bool fForceSave;
  G4long fAutoSaveSize;

  /**
   * Variables to keep track where the energy was deposited in the LAr.
   */
  Float_t inst_edep_lar_ms; ///< energy deposited inside what would be the MS
  Float_t inst_edep_lar_in; ///< energy deposited inside the instrumented region
  Float_t inst_edep_lar_out; ///< energy deposited between instrumented region and Rn shroud

  /**
   * CosmicRay output -> coherent with old GermaniumArrayCR output, but in output tree.
   */
  G4bool        fAddCRInformation;
  G4ThreeVector fSphereCenter;
  G4double      fSphereRadius;

  Float_t  mc_theta[MAX_NTRJ];
  Float_t  mc_phi[MAX_NTRJ];
  Float_t  mc_costheta[MAX_NTRJ];
  Int_t    muonflag;
  Int_t    inelasticflag;

  Int_t neu_totnum;
  vector<Int_t> neu_trackid;
  vector<Int_t> neu_parentid;
  vector<Float_t> neu_ekin;
  vector<Float_t> neu_xpos;
  vector<Float_t> neu_ypos;
  vector<Float_t> neu_zpos;
  vector<Float_t> neu_xdir;
  vector<Float_t> neu_ydir;
  vector<Float_t> neu_zdir;

  Int_t iso_totnum;
  vector<Int_t>   iso_A;
  vector<Int_t>   iso_Z;
  vector<TString> iso_particlename;
  vector<Float_t> iso_time;
  vector<Float_t> iso_ekin;
  vector<Float_t> iso_xpos;
  vector<Float_t> iso_ypos;
  vector<Float_t> iso_zpos;
  vector<TString> iso_volumename;
  vector<TString> iso_processname;

  Int_t flu_totnum;
  vector<Int_t>   flu_particleid;
  vector<Float_t> flu_ekin;
  vector<Float_t> flu_xpos;
  vector<Float_t> flu_ypos;
  vector<Float_t> flu_zpos;
};

#endif
