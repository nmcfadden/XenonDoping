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
 * AUTHOR: Kevin
 * CONTACT:
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 * 03-02-2005 Xiang renamed the variables
 * 10-16-2006 Xiang, if the number of hits, of trj or of trjp 
 *                 exceeds the limit, it will be set to the limit.
 *
 * 12-07-2006 Manuela, added collimator as sensitive volume
 * 10-11-2010 Alex, 1) included phi and r of hits to the root file,
                    2) previous definition of hits_idseg[i] did not work
 * 01-24-2011 Alex, fixed a bug in hits_r
 * 02-17-2011 Alex, added possibility to use time window for decay chains.
                    The structure was changed back to be similar with GEOutputGermaniumArray.
                    Trajectories information consistency (in comparison with the
                    previous version) has to be checked still!!!
 */

// include files for ROOT
#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TNtuple.h"

#include <string>

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
#include "G4TrackingManager.hh"

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementTrackingAction.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputTestStand.hh"
#include "gerdaio/GETrajectory.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputTestStand::GEOutputTestStand(G4bool isMother):MGOutputRoot(isMother)
{
  SetSchemaDefined(false);
  fFLAG_Save_Energy = true;
  fFLAG_Save_Hits   = true;
  fFLAG_Save_Trajectories = false;
  fFLAG_Coincidence = false;
  fFLAG_SiegfriedCoincidence = false;
}


GEOutputTestStand::~GEOutputTestStand()
{;}

void GEOutputTestStand::DefineSchema()
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

    if (fFLAG_Save_Energy) {
        nT->Branch("eventnumber",&eventnumber,"eventnumber/I");
        nT->Branch("hits_tote",&hits_tote,"hits_tote/F");
        nT->Branch("seg_totnum",&seg_totnum,"seg_totnum/I");
        nT->Branch("seg_edep",seg_edep,"seg_edep[seg_totnum]/F");
    }

    if (fFLAG_Save_Hits) {

    // MC true vertex information
    nT->Branch("vertex_totnum",&vertex_totnum,"vertex_totnum/I");
    nT->Branch("vertex_xpos",vertex_xpos,"vertex_xpos[vertex_totnum]/F");
    nT->Branch("vertex_ypos",vertex_ypos,"vertex_ypos[vertex_totnum]/F");
    nT->Branch("vertex_zpos",vertex_zpos,"vertex_zpos[vertex_totnum]/F");
    nT->Branch("vertex_time",vertex_time,"vertex_time[vertex_totnum]/F");
    nT->Branch("vertex_numparticle",vertex_numparticle,"vertex_numparticle[vertex_totnum]/I");
    // MC true primary particle information
    nT->Branch("mc_totnumparticles", &mc_totnumparticles, "mc_totnumparticles/I");
    nT->Branch("mc_iparticle",mc_iparticle,"mc_iparticle[mc_totnumparticles]/I");
    nT->Branch("mc_ivertex",mc_ivertex,"mc_ivertex[mc_totnumparticles]/F"); 
    nT->Branch("mc_px",mc_px,"mc_px[mc_totnumparticles]/F");
    nT->Branch("mc_py",mc_py,"mc_py[mc_totnumparticles]/F");
    nT->Branch("mc_pz",mc_pz,"mc_pz[mc_totnumparticles]/F");
    nT->Branch("mc_pe",mc_pe,"mc_pe[mc_totnumparticles]/F");
    nT->Branch("mc_ekin",mc_ekin,"mc_ekin[mc_totnumparticles]/F");
    nT->Branch("mc_id",mc_id,"mc_id[mc_totnumparticles]/I");

    // hits in sensitive detectors
    nT->Branch("hits_totnum",&hits_totnum,"hits_totnum/I");
    nT->Branch("hits_edep",hits_edep,"hits_edep[hits_totnum]/F");
    nT->Branch("hits_xpos",hits_xpos,"hits_xpos[hits_totnum]/F");
    nT->Branch("hits_ypos",hits_ypos,"hits_ypos[hits_totnum]/F");
    nT->Branch("hits_zpos",hits_zpos,"hits_zpos[hits_totnum]/F");
    nT->Branch("hits_phi",hits_phi,"hits_phi[hits_totnum]/F");
    nT->Branch("hits_r",hits_r,"hits_r[hits_totnum]/F");
    nT->Branch("hits_idseg",hits_idseg,"hits_idseg[hits_totnum]/I");
    nT->Branch("hits_time", hits_time, "hits_time[hits_totnum]/F");
    nT->Branch("hits_trackid",hits_trackid,"hits_trackid[hits_totnum]/I");
    nT->Branch("hits_trackpdg",hits_trackpdg,"hits_trackpdg[hits_totnum]/I");

    // how many Ge detectors have energy deposit

    // hits and deposited energy of each germanium crystal
    nT->Branch("seg_id",seg_id,"seg_id[seg_totnum]/I");
    nT->Branch("seg_numhits",seg_numhits,"seg_numhits[seg_totnum]/I");

    // neutrons found in the track (0=no, 1=yes)
    nT->Branch("neutronflag",&neutronflag,"neutronflag/I");

    //energy deposit in collimator
    nT->Branch("ene_in_collimator",&ene_in_collimator,"ene_in_collimator/F");
    }
    if (fFLAG_Save_Trajectories){
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
    // points of trajectories
    nT->Branch("trjp_totnum",&trjp_totnum,"trjp_totnum/I");
    nT->Branch("trjp_xpos",trjp_xpos,"trjp_xpos[trjp_totnum]/F");
    nT->Branch("trjp_ypos",trjp_ypos,"trjp_ypos[trjp_totnum]/F");
    nT->Branch("trjp_zpos",trjp_zpos,"trjp_zpos[trjp_totnum]/F");
    nT->Branch("trjp_de",trjp_de,"trjp_de[trjp_totnum]/F");
    nT->Branch("trjp_steplength",trjp_steplength,"trjp_steplength[trjp_totnum]/F");
    nT->Branch("trjp_insidege",trjp_insidege,"insitrjp_de[trjp_totnum]/I");
    nT->Branch("trjp_processid",trjp_processid,"trjp_processid[trjp_totnum]/I");
    }
    SetSchemaDefined(true);
  }
}

void GEOutputTestStand::BeginOfRunAction()
{
  if(IsMother()) OpenFile();
  DefineSchema();
}


void GEOutputTestStand::BeginOfEventAction(const G4Event *event)
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
//  MGLog(debugging)<< "Saving " << numvertex<<" vertex(es)." << endlog;

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

void GEOutputTestStand::EndOfEventAction(const G4Event *evt)  
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;
  //--> NewStage is no more called because no track is now in waiting stack
  this->FillHitsToNtupleTrees(evt);
  if (fFLAG_Save_Trajectories){
    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();

//Alex::   if (fFLAG_Save_Trajectories || MGVOutputManager::GetUseTimeWindow())
//    this->FillMotherNucleiToNtupleTrees(evt);

  MGLog(debugging) << "EndOfEventAction Finished." << endlog;


}
void GEOutputTestStand::FillHitsToNtupleTrees(const G4Event *evt)
{
//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int collCollID = SDman->GetCollectionID("CollimatorHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* CCHC = 0; //collimator hits

  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    CCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(collCollID));

  }


  G4ThreeVector tempvector;

  hits_tote = 0.0;
  for (G4int i=0; i<MAX_NGESEG; i++) {
    edep_allgedet[i]=0.0;
    numhits_allgedet[i]=0;
  }

  G4double H = 7.0 * cm;// GetHeight();
  G4double Pi = 3.14159265358979323846 * rad;
                     // For segments 1-18:
  G4int    lID = -1; // Layer ID: 0 for bottom (seg.  1 -  6)
                     //           1 for middle (seg.  7 - 12)
                     //           2 for top    (seg. 13 - 18)

  G4int    cID = -1; // Column ID: 1 for phi=[ 0,     pi/3)
                     //            2 for phi=[ pi/3,  2pi/3)
                     //            3 for phi=[ 2pi/3, pi)
                     //            4 for phi=[-pi,   -2pi/3)
                     //            5 for phi=[-2pi/3,-pi/3)
                     //            6 for phi=[-pi/3,  0)

  if (GCHC) {
    hits_totnum = GCHC->entries();
    if (hits_totnum>MAX_NHITS) {
       MGLog(warning) << " Event "<<eventnumber
                      << " number of hits "<<hits_totnum
                      << " exceeds limit "<<MAX_NHITS<<endl;
       hits_totnum=MAX_NHITS;
    }
    for (G4int i=0; i<hits_totnum; i++) {
      hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      hits_edep[i] = G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      tempvector = (*GCHC)[i]->GetPos();
      hits_xpos[i] = G4ToRoot(tempvector.getX()/cm);
      hits_ypos[i] = G4ToRoot(tempvector.getY()/cm);
      hits_zpos[i] = G4ToRoot(tempvector.getZ()/cm);
      hits_phi [i] = G4ToRoot(tempvector.getPhi()/rad);
      hits_r   [i] = G4ToRoot(tempvector.getRho()/cm);
      hits_time[i] = G4ToRoot((*GCHC)[i]->GetHittime()/ns);
//      cout<<hits_time[i]<<"  "<<fOffsetTime<<endl;
      hits_trackid[i] = (*GCHC)[i]->GetTrackID();
      hits_trackpdg[i]= (*GCHC)[i]->GetTrackPDG();

      // Get layer ID of the hit: difvide by the segment height
      lID = int((hits_zpos[i]*cm+H/2.0)/(H/3.0));
      if (lID>2) lID=2; // To be safe, should not happen
      if (lID<0) lID=0; // To be safe, should not happen

      // Get column ID of the hit: divide by 60 deg.
      cID = int((hits_phi[i]*rad+2*Pi)/(Pi/3.))+1;// +2Pi -> make the angle positive and rotate the scheme as defined above (in cID)
                                                  // +1   -> starting from 1st segment, not from 0th
      while (cID>6) cID-=6;
      while (cID<1) cID+=6;

      hits_idseg[i] = lID * 6 + cID;
      edep_allgedet[hits_idseg[i]] += hits_edep[i];
      numhits_allgedet[hits_idseg[i]]++;


/*
      // save info of delayed radioactive decay
      if (hitProcess[Nhits]==31 && fUseTimeWindow && particle->GetPDGLifeTime()>fTimeWindow) {
        MotherNuclearX[Nnuclei] = hitWorldX[Nhits];
        MotherNuclearY[Nnuclei] = hitWorldY[Nhits];
        MotherNuclearZ[Nnuclei] = hitWorldZ[Nhits];

        MotherNuclearMass[Nnuclei] = G4float(hitBaryonNumber[Nhits]);
        MotherNuclearCharge[Nnuclei] = hitCharge[Nhits];
        MotherNuclearPDG[Nnuclei] = hitPDG[Nhits];

        Nnuclei++;
        if (Nnuclei>5) {
          MGLog(warning)<<"\n\t More than 6 radioactive nuclei created in event "
               <<eventID<<".\n\t This is not supported currently."
               <<"\n\t Information of some of the nuclei will be lost."<<endlog;
          Nnuclei = 5;
        }
      }
*/
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

  //Energy deposit in Collimator
  G4double collimator_edep = 0.0;
  if (CCHC) {
    for (G4int i=0;i<(G4int)CCHC->entries(); i++) {
      collimator_edep += (*CCHC)[i]->GetEdep();
    }
  }

  ene_in_collimator = G4ToRoot(collimator_edep / MeV);

}


void GEOutputTestStand::FillTrajectoriesToNtupleTrees(const G4Event *evt)
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
  if (trj_totnum>MAX_NTRJ) {
       MGLog(warning) << " Event "<<eventnumber
                      << " number of trj "<<trj_totnum
                      << " exceeds limit "<<MAX_NTRJ<<endl;
       trj_totnum=MAX_NTRJ;
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
    trj_npoints[i]      =trj->GetPointEntries();
    trj_istart[i]       =trjp_totnum;
    trjp_totnum        +=trj_npoints[i];
    trj_iend[i]         =trjp_totnum-1;
    if (trjp_totnum>=MAX_NTRJPOINT) {
       MGLog(warning) << " Event "<<eventnumber
                      << " number of trjps "<<trjp_totnum
                      << " exceeds limit "<<MAX_NTRJPOINT<<endl;
       trjp_totnum=MAX_NTRJPOINT;
    }
    if (trjp_totnum==MAX_NTRJPOINT) continue;
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
      if (volnametemp.find("Ge")!=string::npos) {trjp_insidege[jj]=1;}
      if (j==0)                                 {trjp_insidege[jj]=1;}
//      G4cout<<" "<<volnametemp<<" "<<trjp_insidege[jj]<<G4endl;
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
    }
  }
}

void GEOutputTestStand::EndOfRunAction()
{
  if(IsMother()) CloseFile();
  MGLog(trace) << "EndOfRunAction finished." << endlog;
}

void GEOutputTestStand::FillNtuple()
{

//---> a test
/*
  Int_t itest_ismother=0;
  if (IsMother()) itest_ismother=1;

  Int_t itest_coincidenceflag=0;
  if (fFLAG_Coincidence) itest_coincidenceflag=1;

  Int_t itest_siegfriedcoincidenceflag=0;
  if (fFLAG_SiegfriedCoincidence) itest_siegfriedcoincidenceflag=1;

  Int_t itest_siegfriedregecoincidenceflag=0;
  if (OneSiegfriedCoincidenceEvent) itest_siegfriedregecoincidenceflag=1;

  Int_t itest_siegfriedenergyflag=0;
  if (OneSiegfriedEnergyEvent) itest_siegfriedenergyflag=1;

  Int_t itest_regeenergyflag=0;
  if (OneREGeEnergyEvent) itest_regeenergyflag=1;

  if (test_icase>0) {
  cout<<  eventnumber                       <<" "
      << test_icase                         <<" "
      << itest_ismother                     <<" "
      << itest_coincidenceflag              <<" "
      << itest_siegfriedcoincidenceflag     <<" "
      << itest_siegfriedregecoincidenceflag <<" "
      << itest_siegfriedenergyflag          <<" "
      << itest_regeenergyflag               <<" "
      <<endl;
  }
*/

// following calculates the SiegfriedCoincidenceFlag conditions
  G4bool OneSiegfriedCoincidenceEvent = false;
  G4bool OneSiegfriedEnergyEvent = false;
  G4bool OneREGeEnergyEvent = false;
  if (fFLAG_SiegfriedCoincidence) {
    for (Int_t i=0; i<seg_totnum; i++) {
      if (seg_id[i]<18)  OneSiegfriedEnergyEvent = true;
      if (seg_id[i]>=18) OneREGeEnergyEvent = true;
    }
    if (OneSiegfriedEnergyEvent && OneREGeEnergyEvent)
      OneSiegfriedCoincidenceEvent = true;
  }

  //Int_t test_icase=0;
  if (IsMother() && seg_totnum > 0) {
    // first case no coincidence required
    if      (!fFLAG_Coincidence && !fFLAG_SiegfriedCoincidence ) {
         FillTree();
         //test_icase=1;
    }

    // second case only rege coincidence required
    else if ( fFLAG_Coincidence && !fFLAG_SiegfriedCoincidence ) {
       if ( seg_totnum>=2 ) {
         FillTree();
         //test_icase=2;
       }
    }

    // third case siegfried coincidence
    else if (!fFLAG_Coincidence &&  fFLAG_SiegfriedCoincidence ) {
       if ( OneSiegfriedCoincidenceEvent ) {
         FillTree();
         //test_icase=3;
       }
    }
  }
}


void GEOutputTestStand::WritePartialEvent(const G4Event* evt)
{
  this->FillHitsToNtupleTrees(evt);
  if (fFLAG_Save_Trajectories){
    this->FillTrajectoriesToNtupleTrees(evt);
  }
  this->FillNtuple();
}

void GEOutputTestStand::ResetPartialEvent(const G4Event* evt)
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int collCollID = SDman->GetCollectionID("CollimatorHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* CCHC = 0; //collimator hits

  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    CCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(collCollID));
  }

  if (GCHC) {
    hits_totnum = GCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*GCHC)[i];
    }
    GCHC->GetVector()->clear();
  }
  if (CCHC) {
    hits_totnum = CCHC->entries();
    for (Int_t i=0; i<hits_totnum; i++) {
      delete (*CCHC)[i];
    }
    CCHC->GetVector()->clear();
  }

  // clear all trajectories and trajectory points
  if ( fFLAG_Save_Trajectories || MGVOutputManager::GetUseTimeWindow())
    evt->GetTrajectoryContainer()->clearAndDestroy();

}

void GEOutputTestStand::RootSteppingAction(const G4Step * /*step*/)
{;}

G4ClassificationOfNewTrack GEOutputTestStand::StackingAction(const G4Track *track)
{
  if (!neutronflag) //if it is already "1" do nothing!
    {
      if (track->GetDefinition()->GetParticleName() == "neutron") 
	{
	  neutronflag = 1; 
	  //MGLog(debugging) << "Neutron found" << endlog;
	}
      //I do not count neutrons because of the "double counting" problem pointed out 
      //by Tuebingen people. I get just a yes/no.
    }
  
//  return fUrgent;
  return MGOutputRoot::StackingAction(track);
}

void GEOutputTestStand::PreUserTrackingAction(const G4Track* aTrack)
{
  if (fFLAG_Save_Trajectories || MGVOutputManager::GetUseTimeWindow())
    {
      //One has to get the tracking manager from Tracking Action, and then give the commands 
      //SetStoreTrajectory and SetTrajectory
      MGManager::GetMGManager()->GetMGTrackingAction()->GetTrackingManager()->SetStoreTrajectory(true);
      MGManager::GetMGManager()->GetMGTrackingAction()->
	GetTrackingManager()->SetTrajectory(new GETrajectory(aTrack));
    }
}
