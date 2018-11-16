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
// $Id: GEOutputGermaniumArray.cc,v 1.34 2009-03-25 15:16:55 schubert Exp $
//
// CLASS IMPLEMENTATION:  GEOutputGermaniumArray.cc
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
 * AUTHOR:  Xiang Liu
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * REVISION:
 *
 * 11-30-2004, Luciano,  Added branches for energy deposit in nitrogen & water
 * 12-06-2004, C.Tomei,  Added branch for energy deposit in plastic scintillator
 * 12-15-2004, L.Pandola Added Stacking Action for flagging events containing neutrons
 *                       Added one further branch for the neutron flag
 * 01-28-2005, X. Liu, renamed ntuple variables
 * 03-31-2005, L.Pandola, Kill long-lived nuclei (they do not decay)
 * 05-23-2005, Added warning messages if the number of hits or
 *             trajectory points exceed the maximum number
 * 02-11-2005, X. Liu, added the process id for each trajectory point
initstep              0
LowEnRayleigh         1
LowEnPhotoElec        2
LowEnergyIoni         3
LowEnCompton          4
LowEnBrem             5
hLowEIoni             6
RadioactiveDecay      7
Transportation        8
unknown              -1
 * Feb-07-2005, X. Liu, added particle mass, lepton and baryon numbers
 *  05-09-2006, X. Liu, added more physics processes
msc                     9
eIoni                   10
eBrem                   11
annihil                 12
SynRad                  13
PositronNuclear         14
ElectroNuclear          15
LowEnConversion         16
PhotonInelastic         17
muIoni                  18
muBrems                 19
muPairProd              20
muMinusCaptureAtRest    21
hIoni                   22
Decay                   23
muNuclear               24
LElastic                25
AlphaInelastic          26
AntiNeutronInelastic    27
AntiNeutronAnnihilationAtRest   28
AntiProtonInelastic     29
AntiProtonAnnihilationAtRest    30
DeuteronInelastic       31
KaonPlusInelastic       32
KaonMinusInelastic      33
KaonMinusAbsorptionAtRest       34
KaonZeroLInelastic      35
KaonZeroSInelastic      36
NeutronInelastic        37
LCapture                38
PionPlusInelastic       39
PionMinusInelastic      40
PiMinusAbsorptionAtRest 41
ProtonInelastic         42
TritonInelastic         43
 *
 * Mar-07-2007, Moved the Tracking Action inside output class, so it is executed only when needed
 *              Luciano
 * Nov-13-2007, Moved Segmentation into output class, Daniel
 * Nov-28-2007, Add new boolean flag to control kill/not kill of unstable
 *              daughter nuclei, and messenger, Luciano
 * Apr-03-2008, D.Lenz, Added det_totnum,det_id,det_edep,hits_iddet, now it is easy to plot spectrum of
 *              each single crystal
 * Jun-03-2008, Replaced G4cout and cout with MGLog, Luciano
 * Jun-27-2008, Removed explicit using namespace std, Luciano
 * Jun-27-2008, Changed in order to allow for multiple runs in the same execution, Luciano
 * July-30-2008, D.Lenz, Added SegmentationOffsetAngle for crystals in the segmentation scheme
 * Jan-07-2009, j. schubert, "/MG/output/killGammasOutsideCone true" can be used to kill gamma tracks outside a certain cone
 *                              Example for usage:
 *                                 /MG/output/killGammasOutsideCone true
 *                                 /MG/output/GammaConeCut_ArrayCenter 0 0 19.5 cm
 *                                 /MG/output/GammaConeCut_StartCutRadius 80 cm
 *                                 /MG/output/GammaConeCut_MaxAllowedOpeningAngle 30 deg
 *                              If  a gamma is created at position vecX and has momentum vecP then it is killed in case of
 *                               - momentum points not into a cone directed towards the array center,
 *                                 i.e. angle[vecP,GammaConeCut_ArrayCenter-vecX] < MaxAllowedOpeningAngle
 *                               - _AND_ it has to be outside a certain spherical range arround 'GammaConeCut_ArrayCenter'
 *
 * Mar-18-2009, j. schubert, "/MG/output/WriteNumberOfEventsToFile true" can be used to write the number of events
 *              to the output ROOT output-file
 *
 * Mar-25-2009, j. schubert, if you used G4gun and generated events with primary particle
 *                           flying into a cone pointing towards some defined direction,
 *                           e.g.
 *                                /MG/generator/g4gun/centric_effect_on true
 *                                /MG/generator/g4gun/centric_effect_cone true
 *                                /MG/generator/g4gun/detector_center_position 0.0 0.0 19.5 cm
 *                                /MG/generator/g4gun/opening_angle 10 deg
 *                           then the output ROOT file will contain a number
 *                           storing the fraction of the 'solid angle used' wrt to the 'total solid angle'
 *
 *Sept-17-2011, JJ. added trjp_vol to the outpput tree. To estimate the veto efficiency the variable 'ene_in_nitrogen'
 *		is not enough. I want to know what is the energy deposit in an arbitrarily chosen volume near the HPGe
 *		detector. 'trjp_vol' is set to:
 *						0 default,
 *						1 if the trj point is in germanium
 *						2 if the trj point is in LAr
 *						3 if the trj point is in any of the LAr instrumentation sensitive detectors (N. Barros)
 *      					* add your volume here
 *
 *   04-11-2011, N. Barros, *   Added code to store LAr instrumentation hits, if available. Does nto conflict with other code.
 *   							For the moment each collected photon is stored separately.
 *   						*   Added additional volume definition when storing trajectory. If trj point in any of the instrumentation volumes
 *   							trjo_vol == 3. Useful for debugging purposes.
 *   						* 	Added Optical Processes to the list:
 *   							- OpRayleigh : 44
 *   							- OpAbsorption : 45
 *   						*   Implemented a stacking selection for optical photons if the LAr Instrumentation is active.
 *   							Optical Photons are left to be tracked at the end and only if there is a hit in the Germanium.
 *
 *   05-22-2012, N. Barros, * Corrected bug in the LAr Instrumentation hit collection.
 *   						* Disable storage of trajectories with photon tracking on (takes too much memory and storage)
 *   						* Corrected problem in instrumentation hit generation in ::RootSteppingAction
 *   						* All LAr Instrumentation hits are now stored into a single dedicated collection.
 *
 *   05-24-2012, N. Barros, * Eliminated some debug messages.
 *
 *   07-16-2012, N. Barros, * Removed abort() call when when maximum number of photons was reached.
 *   07-18-2012, N. Barros, * Added possibility for tracking optical photons with LAr Instrumentation even without hits in germanium.
 *   08-14-2012, N. Barros, * Added the hybrid design to the stepping action verifications.
 *   						* Removed the code to abort when the number of photons exceeded the hardcoded limit. Now it issues only a warning.
 *   09-07-2012, N. Barros, * Added a separate TTree to store the mapping between LArInst part number and solid name.
 *   						* Added an option to force TTree buffer and header flushing below 300 MB.
 *  09-12-2012, N. Barros,  * Added support for fast mode in the LAr Instrumentation MC. It kills the event after having registered hits in every single part cluster  of the instrumentation.
 *  09-24-2012, N. Barros,  * Updated volume name check for photon sin the hybrid design of LAr Instrumentation.
 *  10-25-2012, N. Barros,  * Removed photon energy from the data structure (only wavelength is necessary).
 *  						* Added three variables to store energy deposition in different regions (between LArInstr shroud and Rn shroud, within 8 cm of the string center and the rest of instrumented volume).
 *  						* Eliminated superfulous fast_mode code for LAr Instrumentation data structure.
 *  12-20-2012, B. Lehnert  * Registered LAr Instrumentation APD option
 *  04-19-2016, J. Hakenmueller * Adaption to phaseII with up to 7 detectors per string
 *				* fZ warning removed for type 5 detectors (as before for type4)
 *  20-05-2016, JJ. Improvements to speed up photon tracking:
 *	- possibility to kill all photon tracks after 1 (or N) hit in any light detector (suff. cond. for LAr-veto) runs 10x faster
 *	- fixed problem of checking for energy deposit in Ge detector multiple times within the same event
 *  25-04-2017, C. Wiesinger * Added CosmicRay output, moved it from txt files to TTree, added time of isotope creation
 *  20-11-2017, K. v. Sturm * Added possibility to save the macro commands used inside the output root file using a TObjString
 *  16-04-2018, K. v. Sturm * add fKillLifetimeLongLivedNuclei to set live-time above which long lived nuclei are killed
 */
//---------------------------------------------------------------------------//
//
// include files for ROOT
#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TParameter.h"
#include "TObjString.h"

#include <sstream>
#include <string>
#include <typeinfo>


// Include files for the G4 classes
#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4DynamicParticle.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VProcess.hh"
#include "G4UImanager.hh"
#include "G4Ions.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4TrackingManager.hh"
#include "G4Neutron.hh"
#include "G4Triton.hh"
#include "G4Proton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Gamma.hh"
#include "G4OpBoundaryProcess.hh"

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"


#include "G4RunManager.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementTrackingAction.hh"
#include "management/MGManagementEventAction.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdageometry/GEGeometryPMTSDHit.hh"
#include "gerdageometry/GEGeometryPMTSD.hh"
#include "gerdaio/GEOutputGermaniumArray.hh"
#include "gerdaio/GETrajectory.hh"

//#include "generators/MGVGenerator.hh"
#include "generators/MGGeneratorPrimary.hh"
#include "generators/MGGeneratorG4Gun.hh"

#include "management/MGManagerDetectorConstruction.hh"
#include "gerdageometry/GEGeometryLArInstSD.hh"
#include "gerdageometry/GEGeometryLArInstSDHit.hh"
using namespace std;

using namespace CLHEP;


//---------------------------------------------------------------------------//
GEOutputGermaniumArray::GEOutputGermaniumArray(G4bool isMother, G4bool needt, G4bool larInst):
      MGOutputRoot(isMother),
      NeedTrajectoryInformation(needt),StoreLArInstrumentationHits(larInst),gNavigator(0),
      fReduceHitInfo1(false),

      fDoKillGammasOutsideCone(0),
      fGammaConeCut_ArrayCenter(0,0,0),
      fGammaConeCut_StartCutRadius(-1.*cm),
      fGammaConeCut_MaxAllowedOpeningAngle(180.*deg),

      fWriteMacroToOutput(true),
      fWriteNumberOfEventsToOutput(true),
      fLArInstApplyGeEneCut(false),
      fLArInstApplyGeCoincidenceCut(false),
      fLArInstGeEneCutLow(0.0),
      fLArInstGeEneCutHigh(999.9),
      fTreeLArInstParts(0),
      fForceSave(false),
      fAutoSaveSize(10000000),

      fAddCRInformation(false),
      fSphereCenter(0,0,-10.*cm),
      fSphereRadius(30.0*cm)
{
  SetSchemaDefined(false);
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fFLAG_RequireHit = false;
  fFLAG_RequireCoincidence = false;
  fKillDaughterFlag = true;
  fMessenger = new GEOutputGermaniumArrayMessenger(this);
  fRunInitialized = false;
  fDoKillNeutrinos= false;
  fKillLifetimeLongLivedNuclei = 1.0*microsecond;

  fAlwaysTrackPhotonsLAr = false;
}


GEOutputGermaniumArray::~GEOutputGermaniumArray()
{
  if(IsMother()) CloseFile();
  //if (fout.is_open()) fout.close();
  MGLog(trace) << "Close output files." << endlog;
  if (fMessenger) delete fMessenger;
  MGLog(trace) << "GEOutputArray messenger deleted" << endlog;
}

void GEOutputGermaniumArray::DefineSchema()
{
  if(!SchemaDefined()){


    TTree *nT;
    if(fTree)
      nT = fTree;
    else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "Germanium Array Tree");
    }

    // MC true vertex information
    nT->Branch("eventnumber",&eventnumber,"eventnumber/I");
    nT->Branch("vertex_totnum",&vertex_totnum,"vertex_totnum/I");
    nT->Branch("vertex_xpos",vertex_xpos,"vertex_xpos[vertex_totnum]/F");
    nT->Branch("vertex_ypos",vertex_ypos,"vertex_ypos[vertex_totnum]/F");
    nT->Branch("vertex_zpos",vertex_zpos,"vertex_zpos[vertex_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("vertex_time",vertex_time,"vertex_time[vertex_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("vertex_numparticle",vertex_numparticle,"vertex_numparticle[vertex_totnum]/I");
    // MC true primary particle information
    nT->Branch("mc_totnumparticles", &mc_totnumparticles, "mc_totnumparticles/I");
    nT->Branch("mc_iparticle",mc_iparticle,"mc_iparticle[mc_totnumparticles]/I");
    if( !fReduceHitInfo1 ) nT->Branch("mc_ivertex",mc_ivertex,"mc_ivertex[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_px",mc_px,"mc_px[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_py",mc_py,"mc_py[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_pz",mc_pz,"mc_pz[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_pe",mc_pe,"mc_pe[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_ekin",mc_ekin,"mc_ekin[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 ) nT->Branch("mc_id",mc_id,"mc_id[mc_totnumparticles]/I");
    if( !fReduceHitInfo1 && fAddCRInformation) nT->Branch("mc_theta",mc_theta,"mc_theta[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 && fAddCRInformation) nT->Branch("mc_phi",mc_phi,"mc_phi[mc_totnumparticles]/F");
    if( !fReduceHitInfo1 && fAddCRInformation) nT->Branch("mc_costheta",mc_costheta,"mc_costheta[mc_totnumparticles]/F");

    // mother nuclei
    nT->Branch("nuclei_baryonnumber",&nuclei_baryonnumber,"nuclei_baryonnumber/F");
    nT->Branch("nuclei_charge",&nuclei_charge,"nuclei_charge/F");

    // hits in sensitive detectors
    nT->Branch("hits_totnum",&hits_totnum,"hits_totnum/I");
    nT->Branch("hits_tote",&hits_tote,"hits_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_edep",hits_edep,"hits_edep[hits_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_xpos",hits_xpos,"hits_xpos[hits_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_ypos",hits_ypos,"hits_ypos[hits_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_zpos",hits_zpos,"hits_zpos[hits_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_iddet",hits_iddet,"hits_iddet[hits_totnum]/I");
    if( !fReduceHitInfo1 ) nT->Branch("hits_idseg",hits_idseg,"hits_idseg[hits_totnum]/I");
    if( !fReduceHitInfo1 ) nT->Branch("hits_time", hits_time, "hits_time[hits_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_trackid",hits_trackid,"hits_trackid[hits_totnum]/I");
    if( !fReduceHitInfo1 ) nT->Branch("hits_trackpdg",hits_trackpdg,"hits_trackpdg[hits_totnum]/I");

    // hits in passivation
    nT->Branch("hits_passivation_totnum",&hits_passivation_totnum,"hits_passivation_totnum/I");
    nT->Branch("hits_passivation_tote",&hits_passivation_tote,"hits_passivation_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_edep",hits_passivation_edep,"hits_passivation_edep[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_xpos",hits_passivation_xpos,"hits_passivation_xpos[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_ypos",hits_passivation_ypos,"hits_passivation_ypos[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_zpos",hits_passivation_zpos,"hits_passivation_zpos[hits_passivation_totnum]/F");

    // hits in deadlayer
    nT->Branch("hits_deadlayer_totnum",&hits_deadlayer_totnum,"hits_deadlayer_totnum/I");
    nT->Branch("hits_deadlayer_tote",&hits_deadlayer_tote,"hits_deadlayer_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_edep",hits_deadlayer_edep,"hits_deadlayer_edep[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_xpos",hits_deadlayer_xpos,"hits_deadlayer_xpos[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_ypos",hits_deadlayer_ypos,"hits_deadlayer_ypos[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_zpos",hits_deadlayer_zpos,"hits_deadlayer_zpos[hits_deadlayer_totnum]/F");

    // hits in LAr
    nT->Branch("hits_LAr_totnum",&hits_LAr_totnum,"hits_LAr_totnum/I");
    nT->Branch("hits_LAr_tote",&hits_LAr_tote,"hits_LAr_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_LAr_edep",hits_LAr_edep,"hits_LAr_edep[hits_LAr_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_LAr_xpos",hits_LAr_xpos,"hits_LAr_xpos[hits_LAr_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_LAr_ypos",hits_LAr_ypos,"hits_LAr_ypos[hits_LAr_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_LAr_zpos",hits_LAr_zpos,"hits_LAr_zpos[hits_LAr_totnum]/F");


    // how many Ge segments and detectors have energy deposit
    nT->Branch("seg_totnum",&seg_totnum,"seg_totnum/I");
    nT->Branch("det_totnum",&det_totnum,"det_totnum/I");

    // hits and deposited energy of each segment
    nT->Branch("seg_id",seg_id,"seg_id[seg_totnum]/I");
    nT->Branch("seg_numhits",seg_numhits,"seg_numhits[seg_totnum]/I");
    nT->Branch("seg_edep",seg_edep,"seg_edep[seg_totnum]/F");

    // hits and deposited energy of each germanium crystal
    nT->Branch("det_id",det_id,"det_id[det_totnum]/I");
    nT->Branch("det_numhits",det_numhits,"det_numhits[det_totnum]/I");
    nT->Branch("det_edep",det_edep,"det_edep[det_totnum]/F");

    // energy deposit in water and nitrogen
    nT->Branch("ene_in_water",&ene_in_water,"ene_in_water/F");
    nT->Branch("ene_in_nitrogen",&ene_in_nitrogen,"ene_in_nitrogen/F");

    // energy deposit in plastic scintillator
    nT->Branch("ene_in_scint",&ene_in_scint,"ene_in_scint/F");

    if (StoreLArInstrumentationHits) {
      // Hits in the instrumentation.
      nT->Branch("inst_det_total_hits",&inst_det_total_hits,"inst_det_total_hits/I");            // total number of registered particles
      nT->Branch("inst_ph_wave",inst_ph_wave,"inst_ph_wave[inst_det_total_hits]/F");   // wavelength of registered photon
      nT->Branch("inst_part_num",inst_part_num,"inst_part_num[inst_det_total_hits]/I");   // number of hitted PMT
      nT->Branch("inst_ph_hits_xpos",inst_ph_hits_xpos,"inst_ph_hits_xpos[inst_det_total_hits]/F");   // x-pos of registered photon
      nT->Branch("inst_ph_hits_ypos",inst_ph_hits_ypos,"inst_ph_hits_ypos[inst_det_total_hits]/F");   // y-pos of registered photon
      nT->Branch("inst_ph_hits_zpos",inst_ph_hits_zpos,"inst_ph_hits_zpos[inst_det_total_hits]/F");   // z-pos of registered photon
      nT->Branch("inst_ph_hits_xdir",inst_ph_hits_xdir,"inst_ph_hits_xdir[inst_det_total_hits]/F");   // x-dirof registered photon
      nT->Branch("inst_ph_hits_ydir",inst_ph_hits_ydir,"inst_ph_hits_ydir[inst_det_total_hits]/F");   // y-dir of registered photon
      nT->Branch("inst_ph_hits_zdir",inst_ph_hits_zdir,"inst_ph_hits_zdir[inst_det_total_hits]/F");   // z-dir of registered photon
      nT->Branch("inst_edep_lar_ms",&inst_edep_lar_ms,"inst_edep_lar_ms/F");
      nT->Branch("inst_edep_lar_in",&inst_edep_lar_in,"inst_edep_lar_in/F");
      nT->Branch("inst_edep_lar_out",&inst_edep_lar_out,"inst_edep_lar_out/F");

      /// -- Create another tree to contain the mapping between the part number and the
      /// the name of the physical volume to which it corresponds. Store as a separate string to
      /// avoid too much impact in the file size
      fTreeLArInstParts = new TTree("LArInst_parts","Identification of the LAr Instrumentation parts.");
      fTreeLArInstParts->Branch("id",&fPart_id);
      fTreeLArInstParts->Branch("name",&fPart_name);
    }


    // neutrons found in the track (0=no, 1=yes)
    if( !fReduceHitInfo1 ) nT->Branch("neutronflag",&neutronflag,"neutronflag/I");
    if( !fReduceHitInfo1 && fAddCRInformation) nT->Branch("muonflag",&muonflag,"muonflag/I");
    if( !fReduceHitInfo1 && fAddCRInformation) nT->Branch("inelasticflag",&inelasticflag,"inelastigflag/I");

    if (NeedTrajectoryInformation){
      MGLog(warning) << "Trajectory information requested. Evolving the root schema" << endlog;
      // trajectories (including secondary)
      nT->Branch("trj_totnum",&trj_totnum,"trj_totnum/I");
      nT->Branch("trj_id",trj_id,"trj_id[trj_totnum]/I");
      nT->Branch("trj_pid",trj_pid,"trj_pid[trj_totnum]/I");
      nT->Branch("trj_pdgencode",trj_pdgencode,"trj_pdgencode[trj_totnum]/I");
      nT->Branch("trj_charge",trj_charge,"trj_charge[trj_totnum]/F");
      nT->Branch("trj_px",trj_px,"trj_px[trj_totnum]/F");
      nT->Branch("trj_py",trj_py,"trj_py[trj_totnum]/F");
      nT->Branch("trj_pz",trj_pz,"trj_pz[trj_totnum]/F");
      nT->Branch("trj_npoints",trj_npoints,"trj_npoints[trj_totnum]/I");
      nT->Branch("trj_istart",trj_istart,"trj_istart[trj_totnum]/I");
      nT->Branch("trj_iend",trj_iend,"trj_iend[trj_totnum]/I");
      nT->Branch("trj_mass",trj_mass,"trj_mass[trj_totnum]/F");
      nT->Branch("trj_leptonnumber",trj_leptonnumber,"trj_leptonnumber[trj_totnum]/I");
      nT->Branch("trj_baryonnumber",trj_baryonnumber,"trj_baryonnumber[trj_totnum]/I");
      // points of trajectories
      nT->Branch("trjp_totnum",&trjp_totnum,"trjp_totnum/I");
      nT->Branch("trjp_xpos",trjp_xpos,"trjp_xpos[trjp_totnum]/F");
      nT->Branch("trjp_ypos",trjp_ypos,"trjp_ypos[trjp_totnum]/F");
      nT->Branch("trjp_zpos",trjp_zpos,"trjp_zpos[trjp_totnum]/F");
      nT->Branch("trjp_de",trjp_de,"trjp_de[trjp_totnum]/F");
      nT->Branch("trjp_steplength",trjp_steplength,"trjp_steplength[trjp_totnum]/F");
      nT->Branch("trjp_insidege",trjp_insidege,"trjp_insidege[trjp_totnum]/I");
      nT->Branch("trjp_processid",trjp_processid,"trjp_processid[trjp_totnum]/I");
      nT->Branch("trjp_vol",trjp_vol,"trjp_vol[trjp_totnum]/I");
    }

    if(fAddCRInformation) {
      nT->Branch("neu_totnum",&neu_totnum);
      nT->Branch("neu_trackid",&neu_trackid);
      nT->Branch("neu_parentid",&neu_parentid);
      nT->Branch("neu_ekin",&neu_ekin);
      nT->Branch("neu_xpos",&neu_xpos);
      nT->Branch("neu_ypos",&neu_ypos);
      nT->Branch("neu_zpos",&neu_zpos);
      nT->Branch("neu_xdir",&neu_xdir);
      nT->Branch("neu_ydir",&neu_ydir);
      nT->Branch("neu_zdir",&neu_zdir);

      nT->Branch("iso_totnum",&iso_totnum);
      nT->Branch("iso_A",&iso_A);
      nT->Branch("iso_Z",&iso_Z);
      nT->Branch("iso_particlename",&iso_particlename);
      nT->Branch("iso_time",&iso_time);
      nT->Branch("iso_ekin",&iso_ekin);
      nT->Branch("iso_xpos",&iso_xpos);
      nT->Branch("iso_ypos",&iso_ypos);
      nT->Branch("iso_zpos",&iso_zpos);
      nT->Branch("iso_volumename",&iso_volumename);
      nT->Branch("iso_processname",&iso_processname);

      nT->Branch("flu_totnum",&flu_totnum);
      nT->Branch("flu_particleid",&flu_particleid);
      nT->Branch("flu_ekin",&flu_ekin);
      nT->Branch("flu_xpos",&flu_xpos);
      nT->Branch("flu_ypos",&flu_ypos);
      nT->Branch("flu_zpos",&flu_zpos);
    }

    /// Optional optimization for long running jobs generating large files.
    /// By default ROOT flushes the buffers and writes the header of the TTree
    /// to the file when the amount of data written to the file exceeds 300 MB.
    /// For long simulations with small amount of data per event this can cause
    /// a lot of events to be lost in case of forced job termination.
    /// By changing this value, the buffers and the header are flushed for different sizes


    if (fForceSave) {
      if (fTree) {
        fTree->SetAutoSave(fAutoSaveSize);
      }
    }

    SetSchemaDefined(true);
  }
}

void GEOutputGermaniumArray::BeginOfRunAction()
{
  if (!fRunInitialized)
  {
    if(IsMother()) OpenFile();
    DefineSchema();

    //G4String file = GetFileName()+".txt";
    //fout.open(file);

    this->FillDetectorTypeMatrix();
    fRunInitialized = true;
  }
}


void GEOutputGermaniumArray::BeginOfEventAction(const G4Event *event)
{
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
  this->FillMCToNtupleTrees(event);
  // initialize nuclei_baryonnumber and nuclei_charge
  nuclei_baryonnumber=0.0;
  nuclei_charge=0.0;
  fStage = 0; LArHit = 0;

  if(fAddCRInformation) {
     //reset CR output
     if(neu_totnum != 0) {
         neu_totnum = 0;
         neu_trackid.clear();
         neu_parentid.clear();
         neu_ekin.clear();
         neu_xpos.clear();
         neu_ypos.clear();
         neu_zpos.clear();
         neu_xdir.clear();
         neu_ydir.clear();
         neu_zdir.clear();
     }
     if(iso_totnum != 0) {
         iso_totnum = 0;
         iso_A.clear();
         iso_Z.clear();
         iso_particlename.clear();
         iso_time.clear();
         iso_ekin.clear();
         iso_xpos.clear();
         iso_ypos.clear();
         iso_zpos.clear();
         iso_volumename.clear();
         iso_processname.clear();
     }
     if(flu_totnum != 0) {
         flu_totnum = 0;
         flu_particleid.clear();
         flu_ekin.clear();
         flu_xpos.clear();
         flu_ypos.clear();
         flu_zpos.clear();
     }
  }
}

void GEOutputGermaniumArray::FillMCToNtupleTrees(const G4Event *event)
{
  // Save information about primary particle(s)
  G4PrimaryParticle *primaryParticle;
  G4PrimaryVertex   *primaryVertex;
  vertex_totnum = event->GetNumberOfPrimaryVertex();

  if(vertex_totnum > MAX_NVTX) {
    MGLog(warning) << vertex_totnum << "vertex generated. Currently"
        << " only " << MAX_NVTX << " are supported. "
        << " EventID : " << event->GetEventID() << endlog;
    vertex_totnum = MAX_NVTX;
  }

  mc_totnumparticles=0;

  neutronflag = 0; //this is managed by StackingAction

  for(G4int i = 0; i < vertex_totnum; i++) {
    primaryVertex = event->GetPrimaryVertex(i);
    if(!primaryVertex) {
      MGLog(error) << "Bad primary vertex pointer." << endlog;
      MGLog(fatal) << endlog;
    }
    vertex_xpos[i] = G4ToRoot(primaryVertex->GetX0()/cm);
    vertex_ypos[i] = G4ToRoot(primaryVertex->GetY0()/cm);
    vertex_zpos[i] = G4ToRoot(primaryVertex->GetZ0()/cm);
    vertex_time[i] = G4ToRoot(primaryVertex->GetT0()/ns);
    vertex_numparticle[i] = primaryVertex->GetNumberOfParticle();

    for (G4int j=0; j<vertex_numparticle[i]; j++) {
      primaryParticle = primaryVertex->GetPrimary(j);
      if(!primaryParticle) {
        MGLog(error) << "Bad primary particle pointer." << endlog;
        MGLog(fatal) << endlog;
      }

      mc_iparticle[mc_totnumparticles]=G4ToRoot(primaryParticle->GetPDGcode());

      // Momentum
      G4ThreeVector P = primaryParticle->GetMomentum()/MeV;
      mc_px[mc_totnumparticles] = P.x();
      mc_py[mc_totnumparticles] = P.y();
      mc_pz[mc_totnumparticles] = P.z();

      // Mass (not filled to the ntuples though)
      G4double M = primaryParticle->GetMass()/MeV;
      mc_pe[mc_totnumparticles] = sqrt(P.mag2()+M*M);

      // Now kinetic(!!) energy
      if (P.mag() < M/100)
        mc_ekin[mc_totnumparticles] = P.mag2()/(2*M);
      else
        mc_ekin[mc_totnumparticles] = sqrt(P.mag2()+M*M)-M;

      mc_ivertex[mc_totnumparticles]=i;

      if( fAddCRInformation ) {
        mc_theta[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().theta())/deg);
        mc_phi[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().phi())/deg);
        mc_costheta[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().cosTheta()));
      }

      mc_totnumparticles++;
    }
  }
  eventnumber = G4ToRoot(event->GetEventID());

}

void GEOutputGermaniumArray::EndOfEventAction(const G4Event *evt)
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;
  //--> NewStage is no more called because no track is now in waiting stack
  this->FillHitsToNtupleTrees(evt);
  if (NeedTrajectoryInformation){
    MGLog(warning) << "Filling a trajectory." << endlog;
    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();

  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    this->FillMotherNucleiToNtupleTrees(evt);

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

}

void GEOutputGermaniumArray::FillMotherNucleiToNtupleTrees(const G4Event *event)
{
  // get the mother nuclear baryon and mass number before the end of the event
  nuclei_baryonnumber=-1.0;
  nuclei_charge=0.0;
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  //Int_t ipoint_nuclei=(trajectoryContainer->entries())-1;
  Int_t ipoint_nuclei=0;
  if(trajectoryContainer!=0) {
    while (nuclei_baryonnumber<0) {
      GETrajectory* trj = (GETrajectory*) ((*trajectoryContainer)[ipoint_nuclei]);
      if (trj->GetBaryonNumber()>60) {
        nuclei_baryonnumber = float(trj->GetBaryonNumber());
        nuclei_charge       = trj->GetCharge();
      }
      //ipoint_nuclei-=1;
      ipoint_nuclei+=1;
      if (ipoint_nuclei>=(trajectoryContainer->entries()) && nuclei_baryonnumber<0)
        nuclei_baryonnumber=0.0;
    }
  }
  MGLog(debugging) << "got mother nuclei baryonnumber " << nuclei_baryonnumber
      << " charge "<<nuclei_charge<< endlog;
}

void GEOutputGermaniumArray::FillHitsToNtupleTrees(const G4Event *evt){
  //
  // hits in sensitive detectors
  //

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int passivationCollID = SDman->GetCollectionID("PassivationHC");
  G4int deadlayerCollID = SDman->GetCollectionID("DeadLayerHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");
  G4int waterCollID = SDman->GetCollectionID("WaterHC");
  G4int scintCollID = SDman->GetCollectionID("ScintHC");
  // Get the LArInstrumentation hit collection.
  G4int larinstCollID = -1;
  if (StoreLArInstrumentationHits) {
    larinstCollID = SDman->GetCollectionID("LArInstrumentationHC");
  }


  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* PCHC = 0; //passivation hits
  GEGeometrySDHitsCollection* DCHC = 0; //dead layer hits
  GEGeometrySDHitsCollection* NCHC = 0; //nitrogen hits
  GEGeometrySDHitsCollection* WCHC = 0; //water hits
  GEGeometrySDHitsCollection* PSHC = 0; //scintillator hits
  GEGeometryLArInstSDHitsCollection *LArIHC = 0; // LAr Instrumentation

  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    PCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(passivationCollID));
    DCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(deadlayerCollID));
    NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
    WCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(waterCollID));
    PSHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(scintCollID));
    if (larinstCollID >=0) {
      LArIHC = (GEGeometryLArInstSDHitsCollection*)(HCE->GetHC(larinstCollID));
    }
  }

  G4ThreeVector tempvector;

  hits_tote = 0.0;
  hits_passivation_tote = 0.0;
  hits_deadlayer_tote = 0.0;
  hits_LAr_tote = 0.0;

  for (G4int i=0; i<MAX_NGESEG; i++)
  {
    edep_allgedet[i]=0.0;
    numhits_allgedet[i]=0;
  }

  for (G4int i=0; i<MAX_NGEDET; i++)
  {
    edep_allgedetreally[i]=0.0;
    numhits_allgedetreally[i]=0;
  }


  if (GCHC){
    hits_totnum = GCHC->entries();
    if (hits_totnum>MAX_NHITS) {
      MGLog(warning) << hits_totnum << " hits generated. Currently"
          << " only " << MAX_NHITS <<" are supported. "
          << " EventID : " << evt->GetEventID() << endlog;
      hits_totnum = MAX_NHITS;
    }

    for (G4int i=0; i<hits_totnum; i++) {

      G4int fSegID = -1;
      G4int  fCrystalType  = -1;
      G4int  fOuterCounter      = -1;
      G4int  fInnerCounter      = -1;
      //G4int  fIsTheSecondInAPair = 0;
      G4int  fCopyNumberCounter = -1;
      G4bool fCrystalFound = false;

      G4int fCountSegments = -1;
      G4int NPhiSegments  = 0;
      G4int NZSegments    = 0;
      G4double CrystalHeight = -1;
      G4ThreeVector localPosition;
      G4double fPhi;
      G4double fZ = -1;


      //start new segmentation scheme
      MGGeometryDetector* theDet =
          MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector();

      GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);
      GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();




      //FIXME: Has to account for the fact that crystal type 3 is composed effectively of two crystals
      //       Upper array is placed first.. they take consecutive entries
      // We have to consider that each crystal of geometry 3 is in fact 2 crystals. SHould be implemented as a segmented detector
      while(!fCrystalFound){
        fOuterCounter++;
        fInnerCounter = -1;
        while(!fCrystalFound && fInnerCounter < MaxNrDetInOneString){
          fInnerCounter++;
       	  if(fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter][0] >= 0 ){
            fCopyNumberCounter++;
            if(fCopyNumberCounter == (*GCHC)[i]->GetCopynumber() ){
              fCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter][0];
              fCrystalFound = true;
            }
            if(theDetDB->IsAPair(fOuterCounter,fInnerCounter))
            {
            	fCopyNumberCounter++;
                if(fCopyNumberCounter == (*GCHC)[i]->GetCopynumber() ){
                  fCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter][1];
                  fCrystalFound = true;
                  //fIsTheSecondInAPair = true;
                }
            }

             // applies only for equal pair beges of type 3
            if(!fCrystalFound) {
              G4int fTmpCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter][0];
              GEGeometryDetectorParameters* detPar = theDetDB->GetDetectorParameters(fTmpCrystalType);
              // Special case for back-to-back BEGes
              // FIXME : Implement a more general treatment of this type
              if (detPar->GetGeometry() == 3 && fCopyNumberCounter != (*GCHC)[i]->GetCopynumber() ) {
                fCopyNumberCounter++;
                if(fCopyNumberCounter == (*GCHC)[i]->GetCopynumber() ){
                  fCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter][0];
                  fCrystalFound = true;
                }
              }
              if( (detPar->GetNPhiSegments() == 0) && (detPar->GetNZSegments() == 0) ){fCountSegments += 1;}
              else{fCountSegments+= (detPar->GetNPhiSegments() * detPar->GetNZSegments());}
            }// end of special case for type 3

          }
        }
      }

      GEGeometryDetectorParameters* detPar = theDetDB->GetDetectorParameters(fCrystalType) ;

      NPhiSegments  = detPar->GetNPhiSegments();
      NZSegments    = detPar->GetNZSegments();
      if(NPhiSegments == 0) NPhiSegments = 1;
      if(NZSegments == 0) NZSegments = 1;
      CrystalHeight = detPar->GetHeight() - detPar->GetDLTopThickness() - detPar->GetPassivationThickness();

      localPosition = (*GCHC)[i]->GetLocalPos();

      if( localPosition.phi() < 0){  fPhi = (2 * TMath::Pi() ) + localPosition.phi();}
      else {fPhi = localPosition.phi();}
      fZ = localPosition.z() + 0.5 * CrystalHeight;
      //fZ not useful for BEGes as they have no passivation layer, but the parameter is set to 10e3 in the geometry file, such that this calculation leads to wrong results
      if(fZ<0 && detPar->GetGeometry()!=4){
      	if (fZ<0 && detPar->GetGeometry()!=5){
        MGLog(warning)<<"fZ is smaller zero! THIS SHOULD NOT HAPPEN!!! fZ = "<<
        	fZ/nm<< " nm for det type " <<
        	detPar->GetGeometry() <<" " <<
        	detPar->GetHeight()/nm << "nm "<<
        	detPar->GetDLTopThickness()/nm << "nm "<<
        	detPar->GetPassivationThickness()/nm << "nm "<<
        	0.5 * CrystalHeight/nm << "nm "<<
        	fCrystalType <<
        	endlog;}}

      if(NZSegments == 1 && NPhiSegments == 1){fSegID = fCountSegments +1;}

      else{
        for(G4int j=0; j< NZSegments;j++){
          for(G4int k=0; k< NPhiSegments;k++){

            if( ( fZ > CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * (j+1) ) && (fZ <= CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * j) ) {

              //ok we might have an angular offset since crystal is not aligned with holder!!
              double angularOffset =  theDetDB->GetSegmentationOffsetAngle();
              double phiLowerBound = ((2*TMath::Pi()/(double)NPhiSegments) * k) + angularOffset;
              double phiUpperBound = ((2*TMath::Pi()/(double)NPhiSegments) * (k+1)) + angularOffset;

              if(phiLowerBound < 0){phiLowerBound = (2 * TMath::Pi() ) + phiLowerBound;}
              if(phiUpperBound < 0){phiUpperBound = (2 * TMath::Pi() ) + phiUpperBound;}

              //case 1: since angularOffset might be positive phiUpperBound is bigger than 2Pi and phiLowerBound is smaller than 2Pi.
              if(phiLowerBound <= 2*TMath::Pi() && phiUpperBound > 2*TMath::Pi() ){
                phiUpperBound = phiUpperBound - (2*TMath::Pi());
                if(fPhi >= phiLowerBound || fPhi < phiUpperBound){
                  fSegID = ((k  * NZSegments ) + j +1) + fCountSegments ;
                }
              }

              //case 2: since angularOffset might be negative phiUpperBound is bigger than 0 and phiLowerBound is smaller than 2Pi.
              else if(phiLowerBound <= 2*TMath::Pi() && phiUpperBound > 0 && phiUpperBound < phiLowerBound){
                if(fPhi >= phiLowerBound || fPhi < phiUpperBound){
                  fSegID = ((k  * NZSegments ) + j +1) + fCountSegments ;
                }
              }

              //case 3: everything normal
              else{
                //case 3': if both are bigger than 2Pi. subtract 2Pi
                if(phiUpperBound > (2*TMath::Pi()) && phiLowerBound > (2*TMath::Pi()) ){
                  phiUpperBound = phiUpperBound - (2*TMath::Pi());
                  phiLowerBound = phiLowerBound - (2*TMath::Pi());
                }
                if(fPhi >= phiLowerBound && fPhi < phiUpperBound) {
                  fSegID = ((k  * NZSegments ) + j +1) + fCountSegments ;
                }
              }
            }
          }
        }
      }

      if(fSegID == -1){
        MGLog(warning) <<"This Shouldn't happen"<< endlog;
        MGLog(warning) <<"fSegID = "<< fSegID <<endlog;
      }


      //end of new segmentation scheme

      hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      hits_edep[i] = G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      tempvector = (*GCHC)[i]->GetPos();
      hits_xpos[i] = G4ToRoot(tempvector.getX()/cm);
      hits_ypos[i] = G4ToRoot(tempvector.getY()/cm);
      hits_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
      hits_iddet[i] = (*GCHC)[i]->GetCopynumber();
      hits_idseg[i] = fSegID;
      hits_time[i]  = G4ToRoot((*GCHC)[i]->GetHittime()/ns);
      hits_trackid[i] = (*GCHC)[i]->GetTrackID();
      hits_trackpdg[i]= (*GCHC)[i]->GetTrackPDG();
      // for individual detector spectra
      edep_allgedetreally[hits_iddet[i]] += hits_edep[i];
      numhits_allgedetreally[hits_iddet[i]]++;
      // for individual segment spectra
      edep_allgedet[hits_idseg[i]] += hits_edep[i];
      numhits_allgedet[hits_idseg[i]]++;

    }
  }

  seg_totnum=0;
  for (G4int i=0; i<MAX_NGESEG; i++) {
    if (edep_allgedet[i]>0.0) {
      seg_id[seg_totnum]=i;
      seg_edep[seg_totnum]=edep_allgedet[i]; //Xiang original version
      seg_numhits[seg_totnum]=numhits_allgedet[i];
      seg_totnum++;
    }
  }

  //detector stuff
  det_totnum=0;
  for (G4int i=0; i<MAX_NGEDET; i++) {
    if (edep_allgedetreally[i]>0.0) {
      det_id[det_totnum]=i;
      det_edep[det_totnum]=edep_allgedetreally[i];
      det_numhits[det_totnum]=numhits_allgedetreally[i];
      det_totnum++;
    }
  }


  // hits in passivation
  if (PCHC)
  {
    hits_passivation_totnum = PCHC->entries();

    if (hits_passivation_totnum>MAX_NHITS)
    {
      MGLog(warning) << hits_passivation_totnum << " hits generated. Currently"
          << " only " << MAX_NHITS <<" are supported. "
          << " EventID : " << evt->GetEventID() << endlog;
      hits_passivation_totnum = MAX_NHITS;
    }

    for (G4int i=0; i<hits_passivation_totnum; i++)
    {
      hits_passivation_tote += G4ToRoot( (*PCHC)[i]->GetEdep() / MeV );
      hits_passivation_edep[i] = G4ToRoot( (*PCHC)[i]->GetEdep() / MeV );
      tempvector = (*PCHC)[i]->GetPos();
      hits_passivation_xpos[i] = G4ToRoot(tempvector.getX()/cm);
      hits_passivation_ypos[i] = G4ToRoot(tempvector.getY()/cm);
      hits_passivation_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
    }
  }

  // hits in dead layer


  if (DCHC)
  {

    hits_deadlayer_totnum = DCHC->entries();

    if (hits_deadlayer_totnum>MAX_NHITS)
    {
      MGLog(warning) << hits_deadlayer_totnum << " hits generated. Currently"
          << " only " << MAX_NHITS <<" are supported. "
          << " EventID : " << evt->GetEventID() << endlog;
      hits_deadlayer_totnum = MAX_NHITS;
    }

    for (G4int i=0; i<hits_deadlayer_totnum; i++)
    {
      hits_deadlayer_tote += G4ToRoot( (*DCHC)[i]->GetEdep() / MeV );
      hits_deadlayer_edep[i] = G4ToRoot( (*DCHC)[i]->GetEdep() / MeV );
      tempvector = (*DCHC)[i]->GetPos();
      hits_deadlayer_xpos[i] = G4ToRoot(tempvector.getX()/cm);
      hits_deadlayer_ypos[i] = G4ToRoot(tempvector.getY()/cm);
      hits_deadlayer_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
    }
  }


  //Energy deposit in LNitrogen
  // TODO: this is liquid argon since 20000 BC. change name eventually
  G4double nitrogen_edep = 0.0;
  if (NCHC)
  {

	// old variable
    for (G4int i=0; i<(G4int)NCHC->entries(); i++)
    {
      nitrogen_edep += (*NCHC)[i]->GetEdep();
    }

    // new variables
    hits_LAr_totnum = NCHC->entries();
    if (hits_LAr_totnum>MAX_NHITS)
    {
      MGLog(warning) << hits_LAr_totnum << " hits generated. Currently"
          << " only " << MAX_NHITS <<" are supported. "
          << " EventID : " << evt->GetEventID() << endlog;
      hits_LAr_totnum = MAX_NHITS;
    }

    for (G4int i=0; i<hits_LAr_totnum; i++)
    {
      hits_LAr_tote += G4ToRoot( (*NCHC)[i]->GetEdep() / MeV );
      hits_LAr_edep[i] = G4ToRoot( (*NCHC)[i]->GetEdep() / MeV );
      tempvector = (*NCHC)[i]->GetPos();
      hits_LAr_xpos[i] = G4ToRoot(tempvector.getX()/cm);
      hits_LAr_ypos[i] = G4ToRoot(tempvector.getY()/cm);
      hits_LAr_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
    }


  }

  //Energy deposit in Water
  G4double water_edep = 0.0;
  if (WCHC)
  {
    for (G4int i=0;i<(G4int)WCHC->entries(); i++)
    {
      water_edep += (*WCHC)[i]->GetEdep();
    }
  }

  //Energy deposit in Plastic Scintillator
  G4double scintillator_edep = 0.0;
  if (PSHC)
  {
    for (G4int i=0; i<(G4int)PSHC->entries(); i++)
    {
      scintillator_edep += (*PSHC)[i]->GetEdep();
    }
  }

  ene_in_water = G4ToRoot(water_edep / MeV);
  ene_in_nitrogen = G4ToRoot (nitrogen_edep / MeV);
  ene_in_scint = G4ToRoot (scintillator_edep / MeV);

  /// LAr Instrumentation hits

  if (LArIHC) {

    // Temp vector for the hit position and direction
    G4ThreeVector hit_tmpvector;
    // Total deposited energy:
    edep_in_LArInst = 0.0;
    // number of hits in all instrumentation devices
    inst_det_total_hits = 0;

    G4int ph_entries = LArIHC->entries();

    MGLog(debugging) << "Have " << ph_entries << " entries in LAr hit collection." << endlog;
    inst_det_total_hits += ph_entries;
    /// Make a verification that we do not exceed the maximum number of registered photons
    if (ph_entries > MAX_NPH) {
      MGLog(warning) << "Number of photon hits larger than hardcoded limit for MAX_NPH (" << MAX_NPH << "). Aborting." << endlog;
      ph_entries = MAX_NPH;
    }
    for (G4int ihit = 0; ihit < ph_entries; ++ihit) {
      hit_tmpvector = (*LArIHC)[ihit]->GetPos();
      inst_ph_hits_xpos[ihit] = G4ToRoot(hit_tmpvector.getX()/cm);
      inst_ph_hits_ypos[ihit] = G4ToRoot(hit_tmpvector.getY()/cm);
      inst_ph_hits_zpos[ihit] = G4ToRoot(hit_tmpvector.getZ()/cm);

      // identify which instrumentation part was the hit detected. See notes above about this enconding
      inst_part_num[ihit] = (*LArIHC)[ihit]->GetPart();
      if (fPartMap.find(inst_part_num[ihit]) == fPartMap.end()) {
        fPartMap[inst_part_num[ihit]] = (*LArIHC)[ihit]->GetPartName();
      }
      inst_ph_energy[ihit] =  G4ToRoot(((GEGeometryLArInstSDHit*)((*LArIHC)[ihit]))->GetEnergy() / MeV) ;
      inst_ph_wave[ihit] =  G4ToRoot (3.e8 *4.1356692e-15*1.e9 /(((GEGeometryLArInstSDHit*)((*LArIHC)[ihit]))->GetEnergy() / MeV*1.e6));
      // we also want to know the incoming direction.
      hit_tmpvector = (*LArIHC)[ihit]->GetParticleDirection();
      inst_ph_hits_xdir[ihit] = G4ToRoot(hit_tmpvector.getX());
      inst_ph_hits_ydir[ihit] = G4ToRoot(hit_tmpvector.getY());
      inst_ph_hits_zdir[ihit] = G4ToRoot(hit_tmpvector.getZ());
      edep_in_LArInst += inst_ph_energy[ihit];
    }

    inst_edep_lar_out = 0.0;
    inst_edep_lar_in = 0.0;
    inst_edep_lar_ms = 0.0;

    // Store the energy deposited in different regions of LAr
    // Use the LAr sensitive detector for that...if available!
    // And store only if there were actually hits on the instrumentation.
    if (NCHC)
     {
    	// Series of variables that should be constant for all events but only needed in this case.
    	// Therefore keep them static
        static GEGeometryGerda* theGerdaDet= NULL;
        static GEGeometryDetectorDB* theDetDB = NULL;
        static G4ThreeVector detCenter;
        static G4double larinst_z_top = 0.0;
        static G4double larinst_z_bottom = 0.0;
        static G4double ms_top_z = 0.0;
        static G4double ms_bottom_z = 0.0;
        static std::vector<G4ThreeVector> *ms_pos = NULL;
        static G4double ms_radius = 0.0;
        static G4double ms_height = 0.0;

        // Only get these once
        if (theDetDB == NULL ) {
          theGerdaDet = dynamic_cast<GEGeometryGerda*>(MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector());
          theDetDB = theGerdaDet->GetDB();
          //LArInst_design = theDetDB->GetLArInstrumentationDesign();
          ms_top_z = theDetDB->GetZUppermostCrystalSurface();
          ms_bottom_z = theDetDB->GetZLowermostCrystalSurface();
          detCenter = theDetDB->GetStringDetectorCenter(0);
          detCenter.setZ(0.5*(ms_top_z + ms_bottom_z));
          larinst_z_top = detCenter.z() + 0.5*2100.0*mm;
          larinst_z_bottom = detCenter.z() - 0.5*2100.0*mm;
          ms_pos = theDetDB->GetMiniShroudPosition();
          ms_radius = theDetDB->GetMiniShroudOuterRadius();
          ms_height = theDetDB->GetMiniShroudHeight();
        }

    	G4ThreeVector Edpos;
    	G4double edep_radius;
       for (G4int ie=0; ie<(G4int)NCHC->entries(); ++ie)
       {
    	   Edpos = (*NCHC)[ie]->GetPos();

    	   // If X,Y radius is larger than the 500 mm shroud then go into "out"
    	   edep_radius =sqrt(Edpos.x()*Edpos.x() + Edpos.y()*Edpos.y());

    	   // If it is outside 250 mm, or inside 250 mm but outside the LAr Instr height
    	   if (((edep_radius > 250*mm) && (edep_radius < 380*mm)) ||
    			   ((edep_radius < 250*mm) && ((Edpos.z() > larinst_z_top) || (Edpos.z() < larinst_z_bottom)))) {
    	     inst_edep_lar_out += G4ToRoot( (*NCHC)[ie]->GetEdep()/keV );
    	   } else if ((edep_radius < 250*mm) && (Edpos.z() < larinst_z_top) && (Edpos.z() > larinst_z_bottom)) {
    	     // we know that it is inside the instrumented region
    	     inst_edep_lar_in += G4ToRoot( (*NCHC)[ie]->GetEdep()/keV);

    		   // And finally the final check for the MS
    		   for (size_t istr = 0; istr < ms_pos->size(); ++istr) {
    			   G4double dist = sqrt(pow(Edpos.x() - ms_pos->at(istr).x(),2) + pow(Edpos.y() - ms_pos->at(istr).y(),2));
    			   if ((dist < ms_radius) && (Edpos.z() < (ms_pos->at(istr).z() + 0.5*ms_height)) && (Edpos.z() > (ms_pos->at(istr).z() - 0.5*ms_height))) {
    				   inst_edep_lar_ms += G4ToRoot( (*NCHC)[ie]->GetEdep()/keV);
    			   }
    		   }
    	   }
       }
     }
  }

  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;
  MGLog(debugging) << "Energy in water " << water_edep/GeV << " GeV" << endlog;
  MGLog(debugging) << "Energy in nitrogen " << nitrogen_edep/MeV << " MeV" << endlog;
  MGLog(debugging) << "Energy in plastic scintillator " << scintillator_edep/MeV << " MeV" << endlog;
  if (StoreLArInstrumentationHits) {
    MGLog(debugging) << "Hits in LAr Instrumentation : " << inst_det_total_hits << ". " << endlog;
    MGLog(debugging) << "Energy in crioLiquid: " << endlog;
    MGLog(debugging) << "           outside : " << inst_edep_lar_out << " keV" << endlog;
    MGLog(debugging) << "            inside : " << inst_edep_lar_in << " keV" << endlog;
    MGLog(debugging) << "                MS : " << inst_edep_lar_ms << " keV" << endlog;
  }
}


void GEOutputGermaniumArray::FillTrajectoriesToNtupleTrees(const G4Event *evt)
{
  //
  // all trajectories
  //

  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  trj_totnum  = 0;
  trjp_totnum = 0;
  G4String volnametemp;
  G4String processnametemp;
  if (trajectoryContainer) trj_totnum = trajectoryContainer->entries();
  if (trj_totnum > MAX_NTRJ )
  {
    MGLog(warning) << trj_totnum << " trj generated. Currently"
        << " only " << MAX_NTRJ <<" are supported. "
        << " EventID : " << evt->GetEventID() << endlog;
    trj_totnum = MAX_NTRJ;
  }

  for (G4int i=0; i<trj_totnum; i++) {
    GETrajectory* trj = (GETrajectory*)
             ((*(evt->GetTrajectoryContainer()))[i]);
    trj_id[i]           =trj->GetTrackID();
    trj_pid[i]          =trj->GetParentID();
    trj_pdgencode[i]    =trj->GetPDGEncoding();
    trj_charge[i]       =trj->GetCharge();
    trj_px[i]           =G4ToRoot( trj->GetInitialMomentum().x()/MeV );
    trj_py[i]           =G4ToRoot( trj->GetInitialMomentum().y()/MeV );
    trj_pz[i]           =G4ToRoot( trj->GetInitialMomentum().z()/MeV );
    trj_mass[i]         =G4ToRoot( trj->GetMass()/MeV);
    trj_leptonnumber[i] =trj->GetLeptonNumber();
    trj_baryonnumber[i] =trj->GetBaryonNumber();
    trj_npoints[i]      =trj->GetPointEntries();
    trj_istart[i]       =trjp_totnum;
    trjp_totnum        +=trj_npoints[i];
    trj_iend[i]         =trjp_totnum-1;
    if (trjp_totnum > MAX_NTRJPOINT ) {
      MGLog(warning) << trjp_totnum << " trjp generated. Currently"
          << " only " << MAX_NTRJPOINT <<" are supported. "
          << " EventID : " << evt->GetEventID() << endlog;
    }
    else {
      for (G4int j=0; j<trj_npoints[i]; j++) {
        GETrajectoryPoint* trjp = (GETrajectoryPoint*)(trj->GetPoint(j));
        G4int jj=j+trj_istart[i];
        trjp_xpos[jj]        =G4ToRoot( trjp->GetPosition().x()/cm );
        trjp_ypos[jj]        =G4ToRoot( trjp->GetPosition().y()/cm );
        trjp_zpos[jj]        =G4ToRoot( trjp->GetPosition().z()/cm );
        trjp_de[jj]          =G4ToRoot( trjp->GetEnergyLost()/MeV  );
        trjp_steplength[jj]  =G4ToRoot( trjp->GetStepLength()/cm   );
        volnametemp       =trjp->GetVolumeName();

        trjp_insidege[jj]=0;
        if (volnametemp.find("Crystal")!=string::npos) {trjp_insidege[jj]=1;}
        if (j==0)                                 {trjp_insidege[jj]=1;}

        // volume id for LAr veto
        trjp_vol[jj]=0;
        if (volnametemp.find("Crystal")!=string::npos){trjp_vol[jj]  = 1;}
        if (volnametemp == "CryoLiquidPhysical"){trjp_vol[jj]  = 2;}
        if (volnametemp.find("LArInstr") != std::string::npos){trjp_vol[jj] = 3;}
        // following is the new part for process identification
        processnametemp = trjp->GetProcessName();
        trjp_processid[jj]=-1;

        if      (processnametemp=="initstep" )     trjp_processid[jj]=0;
        else if (processnametemp=="LowEnRayleigh" ) trjp_processid[jj]=1;
        else if (processnametemp=="LowEnPhotoElec") trjp_processid[jj]=2;
        else if (processnametemp=="LowEnergyIoni")  trjp_processid[jj]=3;
        else if (processnametemp=="LowEnCompton")   trjp_processid[jj]=4;
        else if (processnametemp=="LowEnBrem")      trjp_processid[jj]=5;
        else if (processnametemp=="hLowEIoni")      trjp_processid[jj]=6;
        else if (processnametemp=="RadioactiveDecay") trjp_processid[jj]=7;
        else if (processnametemp=="Transportation") trjp_processid[jj]=8;
        else if (processnametemp=="msc")            trjp_processid[jj]=9;
        else if (processnametemp=="eIoni")          trjp_processid[jj]=10;
        else if (processnametemp=="eBrem")          trjp_processid[jj]=11;
        else if (processnametemp=="annihil")        trjp_processid[jj]=12;
        else if (processnametemp=="SynRad")         trjp_processid[jj]=13;
        else if (processnametemp=="PositronNuclear") trjp_processid[jj]=14;
        else if (processnametemp=="ElectroNuclear")  trjp_processid[jj]=15;
        else if (processnametemp=="LowEnConversion") trjp_processid[jj]=16;
        else if (processnametemp=="PhotonInelastic") trjp_processid[jj]=17;
        else if (processnametemp=="muIoni")          trjp_processid[jj]=18;
        else if (processnametemp=="muBrems")         trjp_processid[jj]=19;
        else if (processnametemp=="muPairProd")      trjp_processid[jj]=20;
        else if (processnametemp=="muMinusCaptureAtRest")  trjp_processid[jj]=21;
        else if (processnametemp=="hIoni")           trjp_processid[jj]=22;
        else if (processnametemp=="Decay")           trjp_processid[jj]=23;
        else if (processnametemp=="muNuclear")       trjp_processid[jj]=24;
        else if (processnametemp=="LElastic")        trjp_processid[jj]=25;
        else if (processnametemp=="AlphaInelastic")  trjp_processid[jj]=26;
        else if (processnametemp=="AntiNeutronInelastic")            trjp_processid[jj]=27;
        else if (processnametemp=="AntiNeutronAnnihilationAtRest")   trjp_processid[jj]=28;
        else if (processnametemp=="AntiProtonInelastic")             trjp_processid[jj]=29;
        else if (processnametemp=="AntiProtonAnnihilationAtRest")    trjp_processid[jj]=30;
        else if (processnametemp=="DeuteronInelastic")            trjp_processid[jj]=31;
        else if (processnametemp=="KaonPlusInelastic")            trjp_processid[jj]=32;
        else if (processnametemp=="KaonMinusInelastic")           trjp_processid[jj]=33;
        else if (processnametemp=="KaonMinusAbsorptionAtRest")    trjp_processid[jj]=34;
        else if (processnametemp=="KaonZeroLInelastic")           trjp_processid[jj]=35;
        else if (processnametemp=="KaonZeroSInelastic")           trjp_processid[jj]=36;
        else if (processnametemp=="NeutronInelastic")             trjp_processid[jj]=37;
        else if (processnametemp=="LCapture")                     trjp_processid[jj]=38;
        else if (processnametemp=="PionPlusInelastic")            trjp_processid[jj]=39;
        else if (processnametemp=="PionMinusInelastic")           trjp_processid[jj]=40;
        else if (processnametemp=="PiMinusAbsorptionAtRest")      trjp_processid[jj]=41;
        else if (processnametemp=="ProtonInelastic")              trjp_processid[jj]=42;
        else if (processnametemp=="TritonInelastic")              trjp_processid[jj]=43;
        else if (processnametemp=="OpRayleigh")					trjp_processid[jj]=44;
        else if (processnametemp=="OpAbsorption")					trjp_processid[jj]=45;
        else if (processnametemp=="OpWLS")						trjp_processid[jj]=46;
        else if (processnametemp=="Scintillation")				trjp_processid[jj]=47;
        else if (processnametemp=="OpBoundary")					trjp_processid[jj]=48;
      }
    }
  }
}

void GEOutputGermaniumArray::FillNtuple()
{

  if ((IsMother() && fFLAG_RequireHit == true && hits_tote>0.0) ||
      (IsMother() && fFLAG_RequireHit == false))
    if ((fFLAG_RequireCoincidence == true && det_totnum>1) ||
        (fFLAG_RequireCoincidence == false))
    FillTree();

}

void GEOutputGermaniumArray::WritePartialEvent(const G4Event* evt)
{
  this->FillHitsToNtupleTrees(evt);
  if (NeedTrajectoryInformation){
    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();
}


// void GEOutputGermaniumArray::PrepareNewEvent(const G4Event* /*evt*/)
// {
//   //if (NeedTrajectoryInformation){
//   //  this->FillTrajectoriesToNtupleTrees(evt);
//   //}
//   ;
// }

void GEOutputGermaniumArray::ResetPartialEvent(const G4Event* evt)
{
  // clear all hits collections
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int passivationCollID = SDman->GetCollectionID("PassivationHC");
  G4int deadlayerCollID = SDman->GetCollectionID("DeadLayerHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");
  G4int waterCollID = SDman->GetCollectionID("WaterHC");
  G4int scintCollID = SDman->GetCollectionID("ScintHC");
  G4int larinstCollID = 0;

  // Added by Nuno to take care of the LAr instrumentation hits
  // There can be more than one collection for LAr instrumentation
  if (StoreLArInstrumentationHits) {
    larinstCollID = SDman->GetCollectionID("LArInstrumentationHC");
  }

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* PCHC = 0; //passivation hits
  GEGeometrySDHitsCollection* DCHC = 0; //dead layer hits
  GEGeometrySDHitsCollection* NCHC = 0; //nitrogen hits
  GEGeometrySDHitsCollection* WCHC = 0; //water hits
  GEGeometrySDHitsCollection* PSHC = 0; //scintillator hits
  GEGeometryLArInstSDHitsCollection* LArIHC = 0;


  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    PCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(passivationCollID));
    DCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(deadlayerCollID));
    NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
    WCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(waterCollID));
    PSHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(scintCollID));
    if (StoreLArInstrumentationHits) {
      LArIHC = (GEGeometryLArInstSDHitsCollection*)(HCE->GetHC(larinstCollID));
    }
  }

  if (GCHC) {
    hits_totnum = GCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*GCHC)[i];
    }
    GCHC->GetVector()->clear();
  }

  if (PCHC) {
    hits_totnum = PCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*PCHC)[i];
    }
    PCHC->GetVector()->clear();
  }

  if (DCHC) {
    hits_totnum = DCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*DCHC)[i];
    }
    DCHC->GetVector()->clear();
  }

  if (NCHC) {
    hits_totnum = NCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*NCHC)[i];
    }
    NCHC->GetVector()->clear();
  }

  if (WCHC) {
    hits_totnum = WCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*WCHC)[i];
    }
    WCHC->GetVector()->clear();
  }

  if (PSHC) {
    hits_totnum = PSHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*PSHC)[i];
    }
    PSHC->GetVector()->clear();
  }

  if (LArIHC) {
    hits_totnum = LArIHC->entries();
    for (G4int j = 0; j < hits_totnum; ++j) {
      delete (*LArIHC)[j];
    }
    LArIHC->GetVector()->clear();
  }

  // get the mother nuclear baryon and mass number before clear all trajectories
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    this->FillMotherNucleiToNtupleTrees(evt);

  // clear all trajectories and trajectory points
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    evt->GetTrajectoryContainer()->clearAndDestroy();
}


void GEOutputGermaniumArray::EndOfRunAction()
{
  const G4RunManager* runMan = G4RunManager::GetRunManager();

  // save macro in a TObjectString FIX ME
  if( fWriteMacroToOutput )
  {
    G4UImanager* UI = G4UImanager::GetUIpointer();

    G4int nCommands = UI -> GetNumberOfHistory();
    TString allCommands;

    if( nCommands == UI -> GetMaxHistSize() )
      MGLog(trace) << "Warning: Number of saved commands is equal to maximum. Some may have been lost on the way: InputMacroCommands = " << nCommands << endlog;
    else
      MGLog(trace) << "Number of saved commands in output root file: InputMacroCommands = " << nCommands << endlog;

    for( int c = 0; c < nCommands; c++ )
    {
        G4String command = UI -> GetPreviousCommand(c);
        allCommands += command.data();
        allCommands += "\n";
    }

    TObjString * macro = new TObjString( allCommands );

    TDirectory* originalDir = gDirectory;
    TFile* f = GetRootFile();
    f->cd();
    macro->Write("InputMacroCommands");
    originalDir->cd();
  }

  // save number of primaries in TParameter
  if( fWriteNumberOfEventsToOutput ) {

    G4int nEvts = runMan->GetCurrentRun()->GetNumberOfEvent();

    TParameter<long> p("NumberOfPrimaries",nEvts);

    TDirectory* originalDir = gDirectory;
    TFile* f = GetRootFile();
    f->cd();
    p.Write();
    originalDir->cd();

    // std::cout << "########################################" << std::endl;
    // std::cout << "########################################" << std::endl;
    // std::cout << "########################################" << std::endl;
    // std::cout << "Number Of Events was :"
    //           <<G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent()
    //           <<std::endl;
  }



  MGVGenerator* MageGenerator = ((MGGeneratorPrimary*)(runMan->GetUserPrimaryGeneratorAction()))->GetMGGenerator();
  if( MageGenerator!=0 ) {
    if( MageGenerator->GetGeneratorName()=="G4Gun" ) {

      MGGeneratorG4Gun* genG4gun = (MGGeneratorG4Gun*)MageGenerator;
      if( genG4gun->GetCentricEffectFlag()     &&
          genG4gun->GetCentricEffectConeFlag()    ) {

        double angle = genG4gun->GetCentricConeAngle() ;
        double cosThetaMin = std::cos(angle);
        double fraction_wrt_TotalSolidAngle = (1.-cosThetaMin)/2.;

        // store fraction of the SolidAngle in TParameter
        TParameter<double> p( "Fraction_Of_SolidAngle_UsedInSimulation",
                              fraction_wrt_TotalSolidAngle );

        TDirectory* originalDir = gDirectory;
        TFile* f = GetRootFile();
        f->cd();
        p.Write();
        originalDir->cd();


      }
    }
  }

  /// If the LAr Instrumentation output is on, write the corresponding TTree to file
  if (StoreLArInstrumentationHits) {
    StoreLArInstPartMap();
  }

  MGLog(trace) << "EndOfRunAction finished." << endlog;
}

void GEOutputGermaniumArray::RootSteppingAction(const G4Step *step)
{

  //This Stepping action kills long-lived daughter nuclei (they do not decay)
  if (fKillDaughterFlag)
  {
    G4String particleType = step->GetTrack()->GetDefinition()->GetParticleType();

    if (particleType == "nucleus" && step->GetTrack()->GetParentID()>0)
    {
      G4double energy = step->GetTrack()->GetKineticEnergy();
      if (energy < 0.1*keV)
      {
        G4Ions* ion = (G4Ions*) step->GetTrack()->GetDefinition();
        G4double lifetime = ion->GetPDGLifeTime();
        G4double excitationEnergy = ion->GetExcitationEnergy();

        //stable and excited nuclei --> track them as usual
        if (lifetime < 0 || excitationEnergy > 0) return;
        //if (lifetime < 0 ) return;

        if (lifetime > fKillLifetimeLongLivedNuclei) //kill long-lived nuclei
        {
	  //Notice: the StepAction is not called after step#0. Secondaries
          //are generated if RadioactiveDecay takes place at step#1
          G4TrackStatus newstatus =
            (step->GetTrack()->GetCurrentStepNumber() > 1) ?
            fStopAndKill : fKillTrackAndSecondaries;
          MGLog(debugging) << "Killing " <<
            ion->GetParticleName() << " (lifetime = " << lifetime/s <<
            "s) " << endlog;
          step->GetTrack()->SetTrackStatus(newstatus);
        }
      }
    }
  }

  if( fAddCRInformation ) {
    // Check for neutrons in Ge
    if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition()) {
      G4double neuEnergy = step->GetTrack()->GetKineticEnergy();
      if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe") {
        G4ThreeVector position = step->GetTrack()->GetPosition();
        G4ThreeVector momentum = step->GetTrack()->GetMomentumDirection();
        neu_trackid.push_back(G4ToRoot(step->GetTrack()->GetTrackID()));
        neu_parentid.push_back(G4ToRoot(step->GetTrack()->GetParentID()));
        neu_ekin.push_back(G4ToRoot(neuEnergy/keV));
        neu_xpos.push_back(G4ToRoot(position.x()/mm));
        neu_ypos.push_back(G4ToRoot(position.y()/mm));
        neu_zpos.push_back(G4ToRoot(position.z()/mm));
        neu_xdir.push_back(G4ToRoot(momentum.x()));
        neu_ydir.push_back(G4ToRoot(momentum.y()));
        neu_zdir.push_back(G4ToRoot(momentum.z()));
        neu_totnum++;
        neutronflag = 1;
      }
      if (neuEnergy > 19.999*MeV && neuEnergy < 20.001*MeV) {
        step->GetTrack()->SetKineticEnergy(19.99*MeV);
        MGLog(warning) << "Ugly thing: changed neutron energy from " << neuEnergy <<
        " to 19.99 MeV" << endlog;
        //this is due to the fact that 20-MeV neutrons may enter in an infinite loop
      }
    }
    if (step->GetTrack()->GetDefinition() == G4Proton::Definition()) {
      if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe") {
        neutronflag = 1;
      }
    }
    if (step->GetTrack()->GetDefinition() == G4MuonMinus::Definition() ||
        step->GetTrack()->GetDefinition() == G4MuonPlus::Definition()) {
      if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe") {
        muonflag = 1;
      }
    }
    if ((step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe") &&
        (step->GetTrack()->GetDefinition() == G4Gamma::Definition())) {
      if (step->GetTrack()->GetParentID()>0) {
        G4String name = step->GetTrack()->GetCreatorProcess()->GetProcessName();
        if (name == "nCapture" || name == "nInelastic") {
          inelasticflag = 1;
        }
      }
    }
    //
    if (step->GetTrack()->GetCurrentStepNumber() > 100000) {
      step->GetTrack()->SetTrackStatus(fStopAndKill);
      MGLog(warning) << "Killed particle in infinite looping" << G4endl;
    }

    //Check for whatever passes through a spherical surface of 30 cm radius
    G4ThreeVector preStepPoint = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector postStepPoint = step->GetPostStepPoint()->GetPosition();
    //check if the sphere is crossed
    if ((preStepPoint-fSphereCenter).mag() >= fSphereRadius
        && (postStepPoint-fSphereCenter).mag() < fSphereRadius) {
      flu_particleid.push_back(G4ToRoot(step->GetTrack()->GetDefinition()->GetPDGEncoding()));
      flu_ekin.push_back(G4ToRoot(step->GetTrack()->GetKineticEnergy()/MeV));
      flu_xpos.push_back(G4ToRoot(preStepPoint.x()/cm));
      flu_ypos.push_back(G4ToRoot(preStepPoint.y()/cm));
      flu_zpos.push_back(G4ToRoot(preStepPoint.z()/cm));
      flu_totnum++;
    }
  }

  // This stepping action kills WLS photon tracks.
  // We must be running with LArInstrumentation and have the WLSKill flag turned on.
  if (StoreLArInstrumentationHits) {
    // Stuff for the LAr Instrumentation
    static GEGeometryGerda* theGerdaDet= NULL;
    static GEGeometryDetectorDB* theDetDB = NULL;

    static G4int LArInst_design = -1;

    // Only get these once
    if (LArInst_design == -1 ) {
      theGerdaDet = dynamic_cast<GEGeometryGerda*>(MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector());
      theDetDB = theGerdaDet->GetDB();
      LArInst_design = theDetDB->GetLArInstrumentationDesign();
    }

    //TODO APD add time out things
    if (LArInst_design == LArInstr_fibers || LArInst_design == LArInstr_PMT || LArInst_design == LArInstr_hybrid || LArInst_design == LArInstr_APD || LArInst_design == LArInstr_hybridAPD) {
      // Get the track
      G4Track* theTrack = step->GetTrack();

      if (theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

        // First check: timeout!
        // If this track has been running for more than 5 microseconds kill it.
        // First just dump something.
        if (theTrack->GetLocalTime() > 5*microsecond) {
          MGLog(warning) << "Killing late event due to timeout (global,local,proper) : "
              << theTrack->GetGlobalTime()/microsecond << " us "
              << theTrack->GetLocalTime()/microsecond << " us "
              << theTrack->GetProperTime()/microsecond << " us "
              <<endlog;

          // If we have an optical photon bouncing around for more than 5 microseconds just kill it.
          // It does not make sense to run any further as it will never reach the
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }

        const G4Step* theStep= step;

        G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
        G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();


        G4OpBoundaryProcessStatus boundaryStatus=Undefined;
        static G4OpBoundaryProcess* boundary=NULL;

        //find the boundary process only once
        if(!boundary){
          G4ProcessManager* pm
          = theStep->GetTrack()->GetDefinition()->GetProcessManager();
          G4int nprocesses = pm->GetProcessListLength();
          G4ProcessVector* pv = pm->GetProcessList();
          G4int i;
          for( i=0;i<nprocesses;i++){

            if((*pv)[i]->GetProcessName()=="OpBoundary"){
              boundary = (G4OpBoundaryProcess*)(*pv)[i];
              break;
            }
          }
        }

        // The step goes out of the world
        if(!thePostPV){
          return;
        }

        // Check that we are coming *into* the sensitive area.
        // If the post physical volume is not one of the sensitive areas then just leave and
        // continue tracking

        switch (LArInst_design){
          case LArInstr_fibers:
          {
            if(!((thePostPV->GetName()).contains("LArInstrFiberPhysical"))) return;
            break;
          }
          case LArInstr_PMT:
          {
            if(!((thePostPV->GetName()).contains("LArInstr_PMTGlassPhysical"))) return;
            break;
          }
          case LArInstr_APD:
          {
            //TODO APD deal with sensitive surfaces
            if(!((thePostPV->GetName()).contains("APDOptSurfPhysical"))) return;
            break;
          }
          case LArInstr_hybrid:
            if(!((thePostPV->GetName()).contains("LArInstr_PMTGlassPhysical")) &&
                !((thePostPV->GetName()).contains("LArInstr_FiberOuter_SensitiveEnd_Physical")) &&
                !((thePostPV->GetName()).contains("LArInstr_SensitiveEndSegment_Physical")) &&
                !((thePostPV->GetName()).contains("LArInstr_FiberInner_SensitiveEnd_Physical")))
              return;
            break;
          case LArInstr_hybridAPD:
            if(!((thePostPV->GetName()).contains("LArInstr_PMTGlassPhysical")) &&
                !((thePostPV->GetName()).contains("LArInstr_FiberOuter_SensitiveEnd_Physical")) &&
                !((thePostPV->GetName()).contains("LArInstr_FiberInner_SensitiveEnd_Physical")) &&
                !((thePostPV->GetName()).contains("APDOptSurfPhysical")))
              return;
            break;

        }
        boundaryStatus=boundary->GetStatus();

        //Check to see if the particle was actually at a boundary
        //Otherwise the boundary status may not be valid
        //Prior to Geant4.6.0-p1 this would not have been enough to check
        if(thePostPoint->GetStepStatus()==fGeomBoundary){
          switch(boundaryStatus){
            case Detection:
              //Note, this assumes that the volume causing detection
              //is the photocathode because it is the only one with
              //non-zero efficiency
            {
              //Triger sensitive detector manually since photon is
              //absorbed in the boundary but status was Detection
              G4SDManager* SDman = G4SDManager::GetSDMpointer();
              G4String sdName="LArInstrumentation";

              GEGeometryLArInstSD* larinstSD = (GEGeometryLArInstSD*)SDman->FindSensitiveDetector(sdName);
              if(larinstSD) {
	        LArHit++;
                larinstSD->ProcessHits_constStep(step,NULL);
              }
              break;
            }
            default:
              break;
          }

        }
      }
    }
  }
}

G4ClassificationOfNewTrack GEOutputGermaniumArray::StackingAction(const G4Track *track)
{
  if ( !fAddCRInformation && !neutronflag) //if it is already "1" do nothing!
  {
    if (track->GetDefinition()->GetParticleName() == "neutron") neutronflag = 1;

    //I do not count neutrons because of the "double counting" problem pointed out
    //by Tuebingen people. I get just a yes/no.
  }

  if( fDoKillGammasOutsideCone ) {
    if ( track->GetDefinition()->GetPDGEncoding() == 22) {  // if the track is a gamma
      const G4ThreeVector& gammaMom( track->GetMomentumDirection() );
      const G4ThreeVector& gammaPos( track->GetPosition()          );
      if(fGammaConeCut_StartCutRadius< (fGammaConeCut_ArrayCenter-gammaPos).mag() ) {
        G4double angle = gammaMom.angle(fGammaConeCut_ArrayCenter-gammaPos);
        if(angle>fGammaConeCut_MaxAllowedOpeningAngle) return fKill;
      }
    }
  }

  //LUCIANO HACK FROM HERE

  if (track->GetDefinition()->GetParticleType() == "nucleus" && track->GetParentID()>0)
  {
    G4double energy = track->GetKineticEnergy();
    if (energy < 0.01*keV)
    {
      G4Ions* ion = (G4Ions*) track->GetDefinition();
      G4double lifetime = ion->GetPDGLifeTime();
      //stable and excited nuclei --> track them as usual
      if (lifetime < 0) return fUrgent; //useless...it is at rest
      //if (lifetime < 0 || excitationEnergy > 0) return;
      if (lifetime > fKillLifetimeLongLivedNuclei) //kill long-lived nuclei
        return fKill;
      //stable nuclei are unaffected
    }
  }

  if (track->GetParentID() > 0)
  {
    //first daugthers
    G4String processName = track->GetCreatorProcess()->GetProcessName();
    if (track->GetDefinition()->GetParticleName() == "RadioactiveDecay" && track->GetDefinition()==G4Gamma::Definition())
    {
      G4double energy = track->GetDynamicParticle()->GetKineticEnergy();
      if (energy>137*keV && energy<141.*keV) //kill manually
      {
        //G4cout << "Killed gamma of " << energy/keV << " keV " << G4endl;
        return fKill;
      }
    }
  }
  //Luciano Hack, TO HERE

  // return fUrgent;
  // call StackingAction() defined in mother class instead of simply returning fUrgent.
  // 2008-04-02. Jing
  if( fDoKillNeutrinos ) {
    if ( abs( track->GetDefinition()->GetPDGEncoding() ) == 12 ||
        abs( track->GetDefinition()->GetPDGEncoding() ) == 14 ||
        abs( track->GetDefinition()->GetPDGEncoding() ) == 16      ) {
      // kill the neutrino
      // std::cout << " a Neutrino is killed" << std::endl;
      return fKill;
    }
  }

  // If we are dealing with optical photons from the LAr Instrumentation
  // move them to the waiting stack
  //FIXME: There is a loop hole if someone activates photons and does not use the appropriate output schema.
  // 	   Have to be careful when choosing the output schema.
  //JJ:kill photon tracks if  LArHit >= fMaxPhHit,  def. value fMaxPhHit == 0 means that all photons are tracked

  if ( StoreLArInstrumentationHits ) {
    if (track->GetDefinition()->GetParticleName() == "opticalphoton" ) {

      if(fMaxPhHit > 0 && LArHit >= fMaxPhHit){
      G4StackManager *stackManager = G4EventManager::GetEventManager()->GetStackManager();
      MGLog(debugging) << "tracks left in stack  " << stackManager->GetNTotalTrack() << endlog;
      stackManager->clear();
      //return fKill;
      }

      return fWaiting;
    }
  }

  if ( fAddCRInformation ) {
    G4String particleType = track->GetDefinition()->GetParticleType();
    //G4String volumeName = " ";

    if (particleType == "nucleus" && track->GetParentID()>0) {
      G4Ions* ion = (G4Ions*) track->GetDefinition();
      G4double lifetime = ion->GetPDGLifeTime();
      G4int Z = ion->GetAtomicNumber();

      if (lifetime > 1e18*second) lifetime = -1; //too long-lived
      if (lifetime > 0 || ion->GetExcitationEnergy() > 0 || (Z > 30 && Z < 40)) {
        G4ThreeVector position = track->GetPosition();
        iso_A.push_back(G4ToRoot(ion->GetAtomicMass()));
        iso_Z.push_back(G4ToRoot(Z));
        iso_particlename.push_back(track->GetDefinition()->GetParticleName());
        iso_time.push_back(track->GetGlobalTime()/ns);
        iso_ekin.push_back(G4ToRoot(track->GetKineticEnergy()/keV));
        iso_xpos.push_back(G4ToRoot(position.x()/m));
        iso_ypos.push_back(G4ToRoot(position.y()/m));
        iso_zpos.push_back(G4ToRoot(position.z()/m));
        iso_volumename.push_back(G4TransportationManager::GetTransportationManager()->
         GetNavigatorForTracking()->LocateGlobalPointAndSetup(track->GetPosition())->GetName());
        iso_processname.push_back(track->GetCreatorProcess()->GetProcessName());
        iso_totnum++;
     }
   }
   if (track->GetDefinition() == G4Triton::TritonDefinition()) {
     G4ThreeVector position = track->GetPosition();
     iso_A.push_back(3);
     iso_Z.push_back(1);
     iso_particlename.push_back("triton");
     iso_time.push_back(track->GetGlobalTime()/ns);
     iso_ekin.push_back(G4ToRoot(track->GetKineticEnergy()/keV));
     iso_xpos.push_back(G4ToRoot(position.x()/m));
     iso_ypos.push_back(G4ToRoot(position.y()/m));
     iso_zpos.push_back(G4ToRoot(position.z()/m));
     iso_volumename.push_back(G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking()->LocateGlobalPointAndSetup(track->GetPosition())->GetName());
     iso_processname.push_back(track->GetCreatorProcess()->GetProcessName());
     iso_totnum++;
   }
  }

  return MGOutputRoot::StackingAction(track);
}

//------------------------------------------------------------------------------------------
void GEOutputGermaniumArray::PreUserTrackingAction(const G4Track* aTrack)
{
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
  {
    MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager()->SetStoreTrajectory(true);
    MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager()->SetTrajectory(new GETrajectory(aTrack));
  }
}

//------------------------------------------------------------------------------------------
void GEOutputGermaniumArray::FillDetectorTypeMatrix()
{
  MGGeometryDetector* theDet =
      MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector();
  if (theDet->GetDetectorName() != G4String("GERDA")) {
    MGLog(warning) << "Detector is not GERDA. Bypassing matrix creation." << endlog;
    return;
  }
  GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);


  GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();

  // read in geometries
  for (G4int j = 0; j < MaxNrDetInOneString; j++)
    for (G4int i = 0; i < 19; i++)
        for (G4int k = 0; k < 2; k++)
      fArrayDetectorTypeMatrix[i][j][k] = theDetDB->GetCrystalType(i,j,k);


}
//------------------------------------------------------------------------------------------
void GEOutputGermaniumArray::NewStage(){

  // all urgent tracks have been processed.
  // In principle only optical photons are left.

  // Check if there is a hit in the Germanium. Otherwise kill the track.
  // Can be overridden with the TrackPhotonsAlways flag
  if (StoreLArInstrumentationHits && fStage == 0) {
    if (!fAlwaysTrackPhotonsLAr) {
      // If the photons have already been tracked (stage 1) return and do
      // default action
      MGLog(debugging) << "Checking for hits prior to tracking the photons." << endlog;
      // Check for hits in the germanium detectors
      G4SDManager * SDman = G4SDManager::GetSDMpointer();
      G4RunManager* runMan = G4RunManager::GetRunManager();
      G4int geCollID = SDman->GetCollectionID("GermaniumHC");

      G4int nhits = 0;

      // Check for hits in the germanium
      if(geCollID>=0)
      {
        const G4Event* currentEvent = runMan->GetCurrentEvent();
        G4HCofThisEvent* HCE = currentEvent->GetHCofThisEvent();
        if (HCE) {
          GEGeometrySDHitsCollection* GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
          if (GCHC) {
            nhits = GCHC->entries();
            if (nhits > 0) {
              // check if there is a hit in another detector than the one with the first hit
              bool isCoin = false;
              G4int firstDetID =(*GCHC)[0]->GetCopynumber();
              for (G4int i=1; i<nhits; i++) {
            	  if ((*GCHC)[i]->GetCopynumber() != firstDetID) isCoin = true;
              }

              if (!fLArInstApplyGeEneCut) {
                // There is no energy cut to be applied.
                // Just check for positive number of hits.
                //MGLog(debugging) << "Have hits. Tracking" << endlog;

            	if(fLArInstApplyGeCoincidenceCut && isCoin)
            	{
            	  // if coincidence is required and present
            	  fStage = 1;
            	  return;
            	}
            	else if (!fLArInstApplyGeCoincidenceCut)
            	{
            	  //if coincidence is not required
              	  fStage = 1;
              	  return;
            	}

              } else {

	        MGLog(debugging) << "Computing Energy deposit in Ge. Nhits =  " << nhits << endlog;
                // There is a energy cut to be applied.
                // Only return if the threshold is reached
                // for hits in at least one crystal
                int index = 0;
                double edep[MAX_NGEDET];
                for (index= 0; index < MAX_NGEDET; ++index) {
                  edep[index] = 0.0;
                }
                // Check if at least one detector complies, otherwise kill the event.
                for (index = 0; index< nhits; ++index) {
                  edep[(*GCHC)[index]->GetCopynumber()] += (*GCHC)[index]->GetEdep() / MeV;
		  MGLog(debugging) << "HPGe. hits in det: " << (*GCHC)[index]->GetCopynumber() << ",   energy = " << (*GCHC)[index]->GetEdep() / MeV << endlog;
                }
                for (index= 0; index < MAX_NGEDET; ++index) {
                  if ((edep[index] > fLArInstGeEneCutLow) && (edep[index] < fLArInstGeEneCutHigh)) {
                    MGLog(debugging) << "Have hits in ROI. Tracking" << endlog;
                  	if(fLArInstApplyGeCoincidenceCut && isCoin)
                  	{
                  	  // if coincidence is required and present
                  	  fStage = 1;
                  	  return;
                  	}
                  	else if (!fLArInstApplyGeCoincidenceCut)
                  	{
                  	  //if coincidence is not required
                  	  fStage = 1;
			  //G4StackManager * stackManager = G4EventManager::GetEventManager()->GetStackManager();
			  //MGLog(debugging) << "total Nb. of tracks in the stack:  " << stackManager->GetNTotalTrack()  << endlog;
                      return;
                  	}
                  }
                }
              }
            }
          }
        }
      }
      // if it reaches this point it is because there are no registered hits. Drop the track
      // Should either clear the stack manager
      // or call ReClassify
      MGLog(debugging) << "No hits in Ge det. - clear photon stack" << endlog;
      G4StackManager * stackManager = G4EventManager::GetEventManager()->GetStackManager();
      stackManager->clear();
      return;
    } else {
      // track photons independently of having hits in germanium or not.
      fStage = 1;
      return;
    }
  }

  // Call the original base method
  // which will reclassify the pending tracks, moving from the waiting stack to urgent.
  MGLog(debugging) << "Now tracking photons from the waiting stack" << endlog;
  G4StackManager * stackManager = G4EventManager::GetEventManager()->GetStackManager();
  MGLog(debugging) << "Nb. phot. tracks   " << stackManager->GetNTotalTrack() << endlog;
  MGVOutputManager::NewStage();
}
//------------------------------------------------------------------------------------------
void GEOutputGermaniumArray::PrepareNewEvent(const G4Event* /*evt*/)
{
  //if (NeedTrajectoryInformation){
  //  this->FillTrajectoriesToNtupleTrees(evt);
  //}
  // -- Stage 1 is photon-tracking stage.
  fStage = 0; LArHit=0;
}

void GEOutputGermaniumArray::StoreLArInstPartMap() {
  if (!fTreeLArInstParts) {
    MGLog(warning) << "LArInstParts TTree not existing. No LAr Instrumentation part map will be stored." << endlog;
    return;
  }
  // Convert the parts map to a TTree
  for (std::map<G4int,G4String>::iterator it = fPartMap.begin(); it !=fPartMap.end(); ++it) {
    fPart_id = G4ToRoot((*it).first);
    fPart_name = (*it).second;
    fTreeLArInstParts->Fill();
  }
  fTreeLArInstParts->Write();
}
