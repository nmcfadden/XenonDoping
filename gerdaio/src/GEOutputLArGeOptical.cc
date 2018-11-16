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
//      
// CLASS IMPLEMENTATION:  GEOutputLArGeOptical.cc
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
 * AUTHOR:  Nuno Barros
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * 05-18-2012, Nuno, Created a fork of the original GEOutputGermaniumArray to
 * store information using the GEGSLArGeOptical geometry with optical tracking.
 * As this implies a considerable amount of changes a new class was added.
 * 
 * REVISION:
 *
 * 05-19-2012, N. Barros, Cleaned up some lingering debugging messages from the code.
 * 18-07-2012, N. Barros, Changed call to G4Exception to be compatible with geant4 9.5
 *
 *
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
#include "TVector2.h"

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
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdageometry/GEGeometryPMTSDHit.hh"
#include "gerdageometry/GEGeometryPMTSD.hh"
#include "gerdaio/GEOutputLArGeOptical.hh"
#include "gerdaio/GETrajectory.hh"    

//#include "generators/MGVGenerator.hh"
#include "generators/MGGeneratorPrimary.hh"
#include "generators/MGGeneratorG4Gun.hh"

#include "management/MGManagerDetectorConstruction.hh"
#include "gerdageometry/GEGeometryPMTSDHit.hh"
//#include "geometry/MGGeometryDetector.hh"
#include "munichteststand/GEGSLArGeOptical.hh"
using namespace std;

using namespace CLHEP;


//---------------------------------------------------------------------------//
GEOutputLArGeOptical::GEOutputLArGeOptical(G4bool isMother, G4bool needt, G4bool larInst):
  MGOutputRoot(isMother),
  NeedTrajectoryInformation(needt),StoreLArInstrumentationHits(larInst),gNavigator(0),
  fReduceHitInfo1(false),

  fDoKillGammasOutsideCone(0),
  fGammaConeCut_ArrayCenter(0,0,0),
  fGammaConeCut_StartCutRadius(-1.*cm),
  fGammaConeCut_MaxAllowedOpeningAngle(180.*deg),
  fWriteNumberOfEventsToOutput(false),
  fLArInstApplyGeEneCut(false),
  fLArInstGeEneCutLow(0.0),
  fLArInstGeEneCutHigh(999.9),
  fLArInstKillWLSTrackFlag(true), // enable by default
  fEGeHitThreshold(0.0)
{
  SetSchemaDefined(false);
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fFLAG_RequireHit = false; 
  fKillDaughterFlag = true;
  fMessenger = new GEOutputLArGeOpticalMessenger(this);
  fRunInitialized = false;
  fDoKillNeutrinos= false;
  edep_in_LArInst = 0.0;

}


GEOutputLArGeOptical::~GEOutputLArGeOptical()
{
  if(IsMother()) CloseFile();
  //if (fout.is_open()) fout.close();
  MGLog(trace) << "Close output files." << endlog;
  if (fMessenger) delete fMessenger;
  MGLog(trace) << "GEOutputArray messenger deleted" << endlog;
}

void GEOutputLArGeOptical::DefineSchema()
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

    /**
     *
    // hits in passivation 
    nT->Branch("hits_passivation_totnum",&hits_passivation_totnum,"hits_passivation_totnum/I");
    nT->Branch("hits_passivation_tote",&hits_passivation_tote,"hits_passivation_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_edep",hits_passivation_edep,"hits_passivation_edep[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_xpos",hits_passivation_xpos,"hits_passivation_xpos[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_ypos",hits_passivation_ypos,"hits_passivation_ypos[hits_passivation_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_passivation_zpos",hits_passivation_zpos,"hits_passivation_zpos[hits_passivation_totnum]/F");
     */
    // hits in deadlayer 
    nT->Branch("hits_deadlayer_totnum",&hits_deadlayer_totnum,"hits_deadlayer_totnum/I");
    nT->Branch("hits_deadlayer_tote",&hits_deadlayer_tote,"hits_deadlayer_tote/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_edep",hits_deadlayer_edep,"hits_deadlayer_edep[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_xpos",hits_deadlayer_xpos,"hits_deadlayer_xpos[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_ypos",hits_deadlayer_ypos,"hits_deadlayer_ypos[hits_deadlayer_totnum]/F");
    if( !fReduceHitInfo1 ) nT->Branch("hits_deadlayer_zpos",hits_deadlayer_zpos,"hits_deadlayer_zpos[hits_deadlayer_totnum]/F");


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
    //nT->Branch("ene_in_water",&ene_in_water,"ene_in_water/F");
    nT->Branch("hits_argon_totnum",&hits_argon_totnum,"hits_argon_totnum/I");
    nT->Branch("hits_argon_tote",&hits_argon_tote,"hits_argon_tote/D");
    if( !fReduceHitInfo1 ) nT->Branch("hits_argon_edep",hits_argon_edep,"hits_argon_edep[hits_argon_totnum]/D");
    if( !fReduceHitInfo1 ) nT->Branch("hits_argon_xpos",hits_argon_xpos,"hits_argon_xpos[hits_argon_totnum]/D");
    if( !fReduceHitInfo1 ) nT->Branch("hits_argon_ypos",hits_argon_ypos,"hits_argon_ypos[hits_argon_totnum]/D");
    if( !fReduceHitInfo1 ) nT->Branch("hits_argon_zpos",hits_argon_zpos,"hits_argon_zpos[hits_argon_totnum]/D");
    if( !fReduceHitInfo1 ) nT->Branch("hits_argon_time", hits_argon_time, "hits_argon_time[hits_argon_totnum]/D");

    //nT->Branch("ene_in_argon",&ene_in_nitrogen,"ene_in_argon/F");

    // energy deposit in plastic scintillator
    nT->Branch("ene_in_scint",&ene_in_scint,"ene_in_scint/F");


    	// Hits in the instrumentation.
    	// Information stored as in the PMTs of the Cherenkov veto
    	nT->Branch("inst_det_total_hits",&inst_det_total_hits,"inst_det_total_hits/I");            // total number of registered particles
    	nT->Branch("inst_det_total_edep",&inst_det_total_edep,"inst_det_total_edep/F");            // total number of registered particles
    	nT->Branch("inst_ph_hits_xpos",inst_ph_hits_xpos,"inst_ph_hits_xpos[inst_det_total_hits]/F");   // x-pos of registered photon
    	nT->Branch("inst_ph_hits_ypos",inst_ph_hits_ypos,"inst_ph_hits_ypos[inst_det_total_hits]/F");   // y-pos of registered photon
    	nT->Branch("inst_ph_hits_zpos",inst_ph_hits_zpos,"inst_ph_hits_zpos[inst_det_total_hits]/F");   // z-pos of registered photon
    	nT->Branch("inst_ph_energy",inst_ph_energy,"inst_ph_energy[inst_det_total_hits]/F");   // energy of registered photon
    	nT->Branch("inst_ph_wave",inst_ph_wave,"inst_ph_wave[inst_det_total_hits]/F");   // wavelength of registered photon
    	nT->Branch("inst_part_num",inst_part_num,"inst_part_num[inst_det_total_hits]/I");   // number of hitted PMT
    	nT->Branch("inst_ph_hits_xdir",inst_ph_hits_xdir,"inst_ph_hits_xdir[inst_det_total_hits]/F");   // x-dirof registered photon
    	nT->Branch("inst_ph_hits_ydir",inst_ph_hits_ydir,"inst_ph_hits_ydir[inst_det_total_hits]/F");   // y-dir of registered photon
    	nT->Branch("inst_ph_hits_zdir",inst_ph_hits_zdir,"inst_ph_hits_zdir[inst_det_total_hits]/F");   // z-dir of registered photon


    // neutrons found in the track (0=no, 1=yes)
    if( !fReduceHitInfo1 ) nT->Branch("neutronflag",&neutronflag,"neutronflag/I");

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
    nT->Branch("trjp_vol_name",trjp_vol_name,"trjp_vol_name[trjp_totnum]/C");
    nT->Branch("trjp_vol",trjp_vol,"trjp_vol[trjp_totnum]/I");
  }
    SetSchemaDefined(true);
  }
}

void GEOutputLArGeOptical::BeginOfRunAction()
{
  if (!fRunInitialized)
    {
      if(IsMother()) OpenFile();
      DefineSchema();
  
      fRunInitialized = true;
    }


}


void GEOutputLArGeOptical::BeginOfEventAction(const G4Event *event)
{
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
  this->FillMCToNtupleTrees(event);
  // initialize nuclei_baryonnumber and nuclei_charge
  nuclei_baryonnumber=0.0;
  nuclei_charge=0.0;
  fStage = 0;
  // Force the header of the root file to be updated
  // to salvage some data in case of job failure.
  // NOTE: Do not set the reporting frequency too high. This is an expensive operation.
  // On the other hand it allows to avoid the frustration of waiting for 10h and get nothing because the
  // header was not written to file. The reporting frequency should be set to a sensitive value that would allow a compromise between
  // I/O overhead and recovery of the files.
  // NOTE: (2) The larger the output file, the longer this operation takes.
  if ((event->GetEventID()%MGManager::GetMGManager()->GetMGEventAction()->GetReportingFrequency()) == 0) {
	  if (fTree) {
		  fTree->AutoSave();
	  }
  }
}

void GEOutputLArGeOptical::FillMCToNtupleTrees(const G4Event *event)
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
      mc_totnumparticles++;
    }
  }
  eventnumber = G4ToRoot(event->GetEventID());

}

void GEOutputLArGeOptical::EndOfEventAction(const G4Event *evt)
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;
  //--> NewStage is no more called because no track is now in waiting stack
  this->FillHitsToNtupleTrees(evt);
  if (NeedTrajectoryInformation){
      //MGLog(warning) << "OOOOOOO Filling a trajectory." << endlog;

    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();

  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    this->FillMotherNucleiToNtupleTrees(evt);

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

}

void GEOutputLArGeOptical::FillMotherNucleiToNtupleTrees(const G4Event *event)
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

void GEOutputLArGeOptical::FillHitsToNtupleTrees(const G4Event *evt){
//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int deadlayerCollID = SDman->GetCollectionID("DeadLayerHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");
  G4int scintCollID = SDman->GetCollectionID("ScintHC");
  // Get also the photocathode hits.
  G4int pmtCollID = SDman->GetCollectionID("PhCathodeHC");


  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* DCHC = 0; //dead layer hits
  GEGeometrySDHitsCollection* NCHC = 0; //nitrogen hits
  GEGeometrySDHitsCollection* PSHC = 0; //scintillator hits
  GEGeometryPMTSDHitsCollection *pmtHC = 0; // photocathode hits

  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    DCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(deadlayerCollID));
    NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
    PSHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(scintCollID));
    pmtHC = (GEGeometryPMTSDHitsCollection*)(HCE->GetHC(pmtCollID));
  }

  G4ThreeVector tempvector;
  
  hits_tote = 0.0;
  hits_deadlayer_tote = 0.0; 

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
      MGLog(warning) << hits_totnum << " hits generated in the Germanium. Currently"
                     << " only " << MAX_NHITS <<" are supported. "
                     << " EventID : " << evt->GetEventID() << endlog;
      hits_totnum = MAX_NHITS;
    }

    for (G4int i=0; i<hits_totnum; i++) {
      
	G4int fSegID = -1;
	
	G4int fCountSegments = -1;
	G4int NPhiSegments  = 0;
	G4int NZSegments    = 0;
	//G4double CrystalHeight = -1;
	G4ThreeVector localPosition;	


      //start new segmentation scheme
      MGGeometryDetector* theDet = 
    		  MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector();

      if (theDet->GetDetectorName() == G4String("GSLArGeOptical")) {
	        
	fCountSegments = 1;
	   NPhiSegments  = 1;
	   NZSegments    = 1;
	   //CrystalHeight = 32.0 * mm;
	   localPosition = (*GCHC)[i]->GetLocalPos();
	   /*
	     if( localPosition.phi() < 0){  fPhi = (2 * TMath::Pi() ) + localPosition.phi();}
	     else {fPhi = localPosition.phi();}
	   */
	   //fZ = localPosition.z() + 0.5 * CrystalHeight;

	   if(NZSegments == 1 && NPhiSegments == 1){fSegID = fCountSegments +1;}
	  
	   // NULLIFY the pointers to collections that we know beforehand that they do not exist
	   DCHC = 0;

      } else {
	  MGLog(fatal) << "Something went wrong with detector name : " << theDet->GetDetectorName() << endlog;
	  G4Exception("GEOutputLArGeOptical::FillHitsToNtupleTrees","IdMismatch",FatalErrorInArgument,G4String("Wrong detector name : " + theDet->GetDetectorName()).data());
      }


      // increment to the stored total energy in the Crystal
      hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      hits_edep[i] = G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      tempvector = (*GCHC)[i]->GetPos();
      // in centimeters
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

  // hits in dead layer 
  
  hits_deadlayer_totnum = 0;
  hits_deadlayer_tote = 0.0;
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


  //Energy deposit in LArgon
  hits_argon_tote = 0.0;
  G4double nitrogen_edep = 0.0;
  if (NCHC)
    {
	  int stored_hits = 0;
	  hits_argon_totnum = NCHC->entries();

	  // Discard hits by photons.

      if (hits_argon_totnum>MAX_NTRJPOINT)
	{
	  MGLog(warning) << hits_argon_totnum << " hits generated. Currently"
	       << " only " << MAX_NTRJPOINT <<" are supported. "
	       << " EventID : " << evt->GetEventID() << endlog;
	}
      for (G4int i=0; i<hits_argon_totnum; i++)
	{
    	  if (stored_hits >= MAX_NTRJPOINT) {
    		  MGLog(warning) << "Reached maximum of hits. Ignoring the next ones." << endlog;
    		  continue;
    	  }
    	  // Check if the hit was caused by an optical photon.
    	  // If so, ignore. Optical photons have a pdf code of 0 (duh)
    	  if ((*NCHC)[i]->GetTrackPDG() == 0) {
    		  continue;
    	  }
    	  stored_hits++;

	  if (!fReduceHitInfo1 ) {
		  hits_argon_edep[i] = G4ToRoot( (*NCHC)[i]->GetEdep() / MeV );
		  tempvector = (*NCHC)[i]->GetPos();
		  hits_argon_xpos[i] = G4ToRoot(tempvector.getX()/cm);
		  hits_argon_ypos[i] = G4ToRoot(tempvector.getY()/cm);
		  hits_argon_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
		  hits_argon_time[i] = G4ToRoot((*NCHC)[i]->GetHittime()/ns);
	  }
	  hits_argon_tote += G4ToRoot( (*NCHC)[i]->GetEdep() / MeV );

	}
    }
  	 nitrogen_edep = hits_argon_tote;

   //Energy deposit in Water
  G4double water_edep = 0.0;
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

  // Energy deposit in LAr Instrumentation
  /// Process the instrumentation hits

  if (pmtHC) {
	  inst_det_total_hits = 0;

	  G4ThreeVector hit_tmpvector;

	  // Total deposited energy
	  edep_in_LArInst = 0.0;

	  inst_det_total_hits = 0;
	  inst_det_total_edep = 0.0;

	  inst_det_total_hits = pmtHC->entries();

	  for (G4int ihit = 0; ihit<inst_det_total_hits; ++ihit) {
		  hit_tmpvector = (*pmtHC)[ihit]->GetPos();
		  inst_ph_hits_xpos[ihit] = G4ToRoot(hit_tmpvector.getX()/cm);
		  inst_ph_hits_ypos[ihit] = G4ToRoot(hit_tmpvector.getY()/cm);
		  inst_ph_hits_zpos[ihit] = G4ToRoot(hit_tmpvector.getZ()/cm);
		  // identify which instrumentation part was the hit detected. See notes above about this enconding
		  inst_part_num[ihit] = (*pmtHC)[ihit]->GetNdet();
		  inst_ph_energy[ihit] =  G4ToRoot((*pmtHC)[ihit]->GetTotEnergy() / MeV) ;
		  inst_ph_wave[ihit] =  G4ToRoot (3.e8 *4.1356692e-15*1.e9 /((*pmtHC)[ihit]->GetTotEnergy() / MeV*1.e6));
		  // we also would like to know the momentum direction.
		  // Not overly important but nice to have
		  hit_tmpvector = (*pmtHC)[ihit]->GetParticleDirection();
		  inst_ph_hits_xdir[ihit] = G4ToRoot(hit_tmpvector.getX());
		  inst_ph_hits_ydir[ihit] = G4ToRoot(hit_tmpvector.getY());
		  inst_ph_hits_zdir[ihit] = G4ToRoot(hit_tmpvector.getZ());
		  edep_in_LArInst += inst_ph_energy[ihit];

		  inst_det_total_edep += inst_ph_energy[ihit]/MeV;
	  }
  }





  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;
  MGLog(debugging) << "Energy in water " << water_edep/GeV << " GeV" << endlog;
  MGLog(debugging) << "Energy in nitrogen " << nitrogen_edep/MeV << " MeV" << endlog;
  MGLog(debugging) << "Energy in plastic scintillator " << scintillator_edep/MeV << " MeV" << endlog;
  MGLog(debugging) << "Energy in LAr Instrumentation " << edep_in_LArInst/MeV << " MeV" << endlog;

}


void GEOutputLArGeOptical::FillTrajectoriesToNtupleTrees(const G4Event *evt)
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
      if (j==0) {
    	  sprintf(trjp_vol_name[jj],"Source");
      } else {
      // Name of the Volume:
      strcpy(trjp_vol_name[jj],trjp->GetVolumeName().c_str());
      }

// volume id for LAr veto
      trjp_vol[jj]=0;
      if (volnametemp.find("Crystal")!=string::npos){trjp_vol[jj]  = 1;}
      if (volnametemp.find("LiquidArgon")){trjp_vol[jj]  = 2;}
      if (volnametemp.find("CalibSource") != std::string::npos){trjp_vol[jj] = 4;}
      if (volnametemp.find("CrystalDeadLayer")!=string::npos){trjp_vol[jj]  = 5;}
      if (volnametemp.find("Segment")!=string::npos){trjp_vol[jj]  = 10;}
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

void GEOutputLArGeOptical::FillNtuple()
{

  if ((IsMother() && fFLAG_RequireHit == true && hits_tote>0.0) || 
      (IsMother() && fFLAG_RequireHit == false))
    if (hits_tote > fEGeHitThreshold) {
	  FillTree();
    }
  
}

void GEOutputLArGeOptical::WritePartialEvent(const G4Event* evt)
{
  this->FillHitsToNtupleTrees(evt);
  if (NeedTrajectoryInformation){
    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();
}

void GEOutputLArGeOptical::PrepareNewEvent(const G4Event* /*evt*/)
{
  ;
}

void GEOutputLArGeOptical::ResetPartialEvent(const G4Event* evt)
{
// clear all hits collections
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int deadlayerCollID = SDman->GetCollectionID("DeadLayerHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");
  G4int scintCollID = SDman->GetCollectionID("ScintHC");
  // Get also the photocathode hits.
   G4int pmtCollID = SDman->GetCollectionID("PhCathodeHC");



  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* DCHC = 0; //dead layer hits
  GEGeometrySDHitsCollection* NCHC = 0; //nitrogen hits
  //GEGeometrySDHitsCollection* WCHC = 0; //water hits
  GEGeometrySDHitsCollection* PSHC = 0; //scintillator hits
  GEGeometryPMTSDHitsCollection *pmtHC = 0; // photocathode hits

  std::vector<GEGeometryPMTSDHitsCollection*> LArIHC;


  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    DCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(deadlayerCollID));
    NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
    PSHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(scintCollID));
    pmtHC = (GEGeometryPMTSDHitsCollection*)(HCE->GetHC(pmtCollID));


  }

  if (GCHC) {
    hits_totnum = GCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*GCHC)[i];
    }
    GCHC->GetVector()->clear();
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
  if (PSHC) {
    hits_totnum = PSHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*PSHC)[i];
    }
    PSHC->GetVector()->clear();
  }

  if (pmtHC) {
	  hits_totnum = pmtHC->entries();
	  for (int i = 0; i < hits_totnum; ++i) {
		  delete (*pmtHC)[i];
	  }
	  pmtHC->GetVector()->clear();
  }

// get the mother nuclear baryon and mass number before clear all trajectories
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    this->FillMotherNucleiToNtupleTrees(evt);

// clear all trajectories and trajectory points
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    evt->GetTrajectoryContainer()->clearAndDestroy();

}


void GEOutputLArGeOptical::EndOfRunAction()
{

    const G4RunManager* runMan = G4RunManager::GetRunManager(); 

    if( fWriteNumberOfEventsToOutput ) {

        G4int nEvts = runMan->GetCurrentRun()->GetNumberOfEvent();

        // simply store the Numbers in an TVector2,
        // this is the cheapest way
        TVector2 v(nEvts,nEvts);

        TDirectory* originalDir = gDirectory;
        TFile* f = GetRootFile();
        f->cd();
        v.Write("NumberOfEvents");
        originalDir->cd();

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

                // simply store the fraction of the SolidAngle in an TVector2,
                // this is the cheapest way
                TVector2 v(fraction_wrt_TotalSolidAngle,
                           fraction_wrt_TotalSolidAngle);

                TDirectory* originalDir = gDirectory;
                TFile* f = GetRootFile();
                f->cd();
                v.Write("Fraction_Of_SolidAngle_UsedInSimulation");
                originalDir->cd();
                
                
            }
        }
    }
    
    MGLog(trace) << "EndOfRunAction finished." << endlog;
}

void GEOutputLArGeOptical::RootSteppingAction(const G4Step *step)
{
	//MGLog(debugging) << "In GEOutputLArGeArray::RootSteppingAction " << endlog;
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
	      
	      if (lifetime > 1.0*microsecond) //kill long-lived nuclei
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


  // Check and register the possible hit in the photo cathode
  G4Track* theTrack = step->GetTrack();
  // It cannot be a primary track (optical photons are always secondaries)
  if(theTrack->GetParentID()==0){
	    //This is a primary track. Do nothing
	  return;
  }
  if (theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

	  const G4Step* theStep= step;
	  // Get the previous and posterior points
	  // -- For the moment this is not used....but it will.
	  //G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	  //G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

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

	  // Check that we are coming *into* the photocathode.
	  // If the post physical volume is not the photocathode
	  // just get out and continue your work.

	  if(!((thePostPV->GetName()).contains("PMTphotocathode"))) return;

	    boundaryStatus=boundary->GetStatus();

	    //Check to see if the particle was actually at a boundary
	    //Otherwise the boundary status may not be valid
	    //Prior to Geant4.6.0-p1 this would not have been enough to check
	    if(thePostPoint->GetStepStatus()==fGeomBoundary){
	      switch(boundaryStatus){
	      case Detection: //Note, this assumes that the volume causing detection
	    	  //is the photocathode because it is the only one with
	    	  //non-zero efficiency
		{
		  //Triger sensitive detector manually since photon is
		  //absorbed but status was Detection
		  G4SDManager* SDman = G4SDManager::GetSDMpointer();
		  G4String sdName="Phcathode";
		  GEGeometryPMTSD* pmtSD = (GEGeometryPMTSD*)SDman->FindSensitiveDetector(sdName);
		  if(pmtSD)
		    pmtSD->ProcessHits_constStep(step,NULL);
		  break;
		}
	      case Absorption:
	      case FresnelReflection:
	      case TotalInternalReflection:
	      case SpikeReflection:
	      default:
		break;
	      }
	  }
  }

}

G4ClassificationOfNewTrack GEOutputLArGeOptical::StackingAction(const G4Track *track)
{
  if (!neutronflag) //if it is already "1" do nothing!
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
          if (lifetime > 1.0*microsecond) //kill long-lived nuclei            
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
    		  MGLog(warning) << "Killed gamma of " << energy/keV << " keV " << G4endl;
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
  if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) {
	  return fWaiting;
  }

  return MGOutputRoot::StackingAction(track);
 }

//------------------------------------------------------------------------------------------
void GEOutputLArGeOptical::PreUserTrackingAction(const G4Track* aTrack)
{
  if (NeedTrajectoryInformation || MGVOutputManager::GetUseTimeWindow())
    {
      //One has to get the tracking manager from Tracking Action, and then give the commands
      //SetStoreTrajectory and SetTrajectory
      MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager()->SetStoreTrajectory(true);
      MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager()->SetTrajectory(new GETrajectory(aTrack));
    }
}

//------------------------------------------------------------------------------------------
void GEOutputLArGeOptical::FillDetectorTypeMatrix()
{
  MGGeometryDetector* theDet = 
    MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
  if (theDet->GetDetectorName() != G4String("GERDA")) {
      MGLog(warning) << "Detector is not GERDA. Bypassing matrix creation." << endlog;
      return;
  }
  GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);
        
  
  GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();


  std::ifstream fInputFileMatrix; 


  fInputFileMatrix.open(theDetDB -> GetCrystalMatrixFilename()); 

  // read in geometries 
  for (G4int j = 0; j < 5; j++)
    for (G4int i = 0; i < 19; i++)
      {
	G4int type; 
	fInputFileMatrix >> type; 
	fArrayDetectorTypeMatrix[i][j] = type; 

      }
  
  // close file 

  fInputFileMatrix.close(); 

}
//------------------------------------------------------------------------------------------
void GEOutputLArGeOptical::NewStage(){

	// all urgent tracks have been processed.
	// In principle only optical photons are left.

	// Check if there is a hit in the Germanium. Otherwise kill the track.
	// If the photons have already been tracked (stage 1) return and do
	// default action
	// Check for hits in either the passivation layer or the Germanium layer
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
						if (!fLArInstApplyGeEneCut) {
							return;
						}
						else {
							// There is a energy cut to be applied.
							// Only return if the threshold is reached
							// for hist in at least one crystal
							int index = 0;
							double edep = 0;
							// Check if at least one detector complies, otherwise kill the event.
							for (index = 0; index< nhits; ++index) {
								edep += (*GCHC)[index]->GetEdep() / MeV;
							}
							if ((edep > fLArInstGeEneCutLow) && (edep < fLArInstGeEneCutHigh)) {

									fStage = 1;
									return;
							}
						}
					}
				}
			}
		}
		// if it reaches this point it is because there are no registered hits. Drop the track
		// Should either clear the stack manager
		// or call ReClassify
		G4StackManager * stackManager = G4EventManager::GetEventManager()->GetStackManager();
		stackManager->clear();

	// Call the original base method
	MGVOutputManager::NewStage();
}
//------------------------------------------------------------------------------------------
void GEOutputLArGeOptical::PrepareNewEvent()
{
	fStage = 0;
}
