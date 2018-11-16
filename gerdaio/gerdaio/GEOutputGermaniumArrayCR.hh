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
 * CLASS DECLARATION:  GEOutputGermaniumArrayCR.hh
 *
 * DESCRIPTION:
 *
 * REVISION: MM-DD-YYYY
 *
 *   04-03-2008, D. Lenz, Added det_totnum, det_id, det_edep, hits_iddet, now it is easy
 *                         to plot spectrum of each single crystal
 *   04-19-2016, J. Hakenmueller, * Adaption to phaseII with up to 7 slots per string
 *				
 */

#ifndef _GEOUTPUTGERMANIUMARRAYCR_HH
#define _GEOUTPUTGERMANIUMARRAYCR_HH

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


class GEOutputGermaniumArrayCR : public MGOutputRoot
{
public:
  //default constructor
  GEOutputGermaniumArrayCR(G4bool isMother, G4bool needt);

  //copy constructor
  GEOutputGermaniumArrayCR(const GEOutputGermaniumArrayCR &);

  //destructor
  ~GEOutputGermaniumArrayCR();

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

  void RequireHit(G4bool flag)
    { fFLAG_RequireHit = flag; }

  void FillDetectorTypeMatrix();


  //private  members
private:

  static const G4int MAX_NHITS=1000;
  static const G4int MAX_NGESEG=400; 
  static const G4int MAX_NGEDET=100; 
  static const G4int MAX_NTRJ=400;
  static const G4int MAX_NTRJPOINT=1500;
  static const G4int MAX_NVTX=10;

  G4bool NeedTrajectoryInformation; ///< switch to turn on trajectory storage

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
  Int_t muonflag;
  Int_t inelasticflag;

  // require at least hit in one segment 
  Int_t fFLAG_RequireHit; 

  // list of primary particles with their momentum and starting positions
  Int_t    mc_totnumparticles;
  Int_t    mc_iparticle[MAX_NTRJ];     // maximum 100 particles
  Float_t  mc_kineticEnergy[MAX_NTRJ];
  Float_t  mc_theta[MAX_NTRJ];
  Float_t  mc_phi[MAX_NTRJ];
  Float_t  mc_costheta[MAX_NTRJ];

  // hits
  Int_t    hits_totnum;
  Float_t  hits_tote;
  Float_t energyratio;
  Float_t  hits_edep[MAX_NHITS]; 
  Float_t  hits_xpos[MAX_NHITS]; 
  Float_t  hits_ypos[MAX_NHITS]; 
  Float_t  hits_zpos[MAX_NHITS]; 
  Int_t    hits_iddet[MAX_NHITS];  // which Ge detector this hit is in
  Int_t    hits_idseg[MAX_NHITS];  // which Ge detector segment this hit is in
  Float_t  hits_time[MAX_NHITS];
  Int_t    hits_trackid[MAX_NHITS]; // which track (in trajectory group)
  Int_t    hits_trackpdg[MAX_NHITS]; // track identity
 
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
  
  G4int MaxNrDetInOneString=7;//for Phase2, has been 5 for Phase1
  G4int fArrayDetectorTypeMatrix[19][7][2]; 

  std::ofstream fout;

  G4ThreeVector fArrayCenter;

  G4Navigator* gNavigator;

  G4bool fRunInitialized;

};

#endif

