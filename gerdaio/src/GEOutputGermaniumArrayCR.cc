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
// $Id: GEOutputGermaniumArrayCR.cc,v 1.10 2009-06-16 14:23:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
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
 * AUTHOR:  Luciano Pandola
 * CONTACT:
 * FIRST SUBMISSION:
 * Nov-14-2007, Moved Segmentation into output class, Daniel
 * Apr-03-2008, D.Lenz, Added det_totnum,det_id,det_edep,hits_iddet, now it is easy to plot spectrum of each single crystal
 * July-30-2008, D.Lenz, Added SegmentationOffsetAngle for crystals in the segmentation scheme
 * Apr-26-2016, J. Hakenmueller, Adaption to BEGe pairs and phase2 according to GEOutputGermaniumArray.cc
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

#include <sstream>
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
#include "G4VProcess.hh"
#include "G4UImanager.hh"
#include "G4Ions.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4Neutron.hh"
#include "G4Triton.hh"
#include "G4Proton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Gamma.hh"


// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputGermaniumArrayCR.hh"    
#include "gerdaio/GETrajectory.hh"    


#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputGermaniumArrayCR::GEOutputGermaniumArrayCR(G4bool isMother, G4bool needt):
  MGOutputRoot(isMother),
  NeedTrajectoryInformation(needt),gNavigator(0)
{
  SetSchemaDefined(false);
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fFLAG_RequireHit = false; 
  fArrayCenter.setX(0.*cm);
  fArrayCenter.setY(0.*cm);
  fArrayCenter.setZ(19.5*cm);
  fRunInitialized = false;
}


GEOutputGermaniumArrayCR::~GEOutputGermaniumArrayCR()
{
  if(IsMother())
    CloseFile();
  //fout.close();
}

void GEOutputGermaniumArrayCR::DefineSchema()
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
    //nT->Branch("vertex_time",vertex_time,"vertex_time[vertex_totnum]/F");
    //nT->Branch("vertex_numparticle",vertex_numparticle,"vertex_numparticle[vertex_totnum]/I");

    // MC true primary particle information
    nT->Branch("mc_totnumparticles", &mc_totnumparticles, "mc_totnumparticles/I");
    nT->Branch("mc_iparticle",mc_iparticle,"mc_iparticle[mc_totnumparticles]/I");
    nT->Branch("mc_kineticEnergy",mc_kineticEnergy,"mc_kineticEnergy[mc_totnumparticles]/F");
    nT->Branch("mc_theta",mc_theta,"mc_theta[mc_totnumparticles]/F");
    nT->Branch("mc_phi",mc_phi,"mc_phi[mc_totnumparticles]/F");
    nT->Branch("mc_costheta",mc_costheta,"mc_costheta[mc_totnumparticles]/F");

    // how many Ge detectors have energy deposit
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

    // neutrons or protons found in the crystal (0=no, 1=yes)
    nT->Branch("neutronflag",&neutronflag,"neutronflag/I");

    // muon found in the crystal (0=no, 1 =yes)
    nT->Branch("muonflag",&muonflag,"muonflag/I");

    nT->Branch("inelasticflag",&inelasticflag,"inelastigflag/I");

    nT->Branch("energyratio",&energyratio,"energyratio/F");

  if (NeedTrajectoryInformation){
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
  }
    SetSchemaDefined(true);
  }
}

void GEOutputGermaniumArrayCR::BeginOfRunAction()
{
  if (!fRunInitialized)
    {
      if(IsMother()) OpenFile();
      DefineSchema();
      
      G4String file = GetFileName()+".txt";
      //G4cout << "Filename " << file << G4endl;
      //fout.open(file);
      
      //This avoids the file to be appended at the start of a new job
      G4String fluxFileName = GetFileName()+".flu";
      std::ofstream fluxfile(fluxFileName);
      fluxfile.close(); 
      
      G4String neuFileName = GetFileName()+".neu";
      std::ofstream neufile(neuFileName);
      neufile.close();
      
      G4String fileName = GetFileName()+".iso";
      std::ofstream foutIso(fileName);
      foutIso.close();

      this->FillDetectorTypeMatrix();

      MGLog(trace) << "Center of the crystal array: " << fArrayCenter/cm << " cm" << endlog;
      fRunInitialized = true;
    }
}


void GEOutputGermaniumArrayCR::BeginOfEventAction(const G4Event *event)
{
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
  // Save information about primary particle(s)
  G4PrimaryParticle *primaryParticle;
  G4PrimaryVertex   *primaryVertex;
  vertex_totnum = event->GetNumberOfPrimaryVertex();

  if(vertex_totnum > MAX_NVTX) {
    MGLog(trace) << vertex_totnum << "vertex generated. Currently"
                   << " only " << MAX_NVTX << " are supported. "
                   << " EventID : " << event->GetEventID() << endlog;
    vertex_totnum = MAX_NVTX;
  }


  //this is a test!!!!!!!!
  /*
    if (event->GetEventID() == 5235) {
     G4UImanager* UI = G4UImanager::GetUIpointer();
     UI->ApplyCommand("/tracking/verbose 1");
    }
  */

  mc_totnumparticles=0;

  neutronflag = 0; //this is managed by StackingAction
  muonflag = 0;
  inelasticflag = 0;

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
      G4double totalMomentum = primaryParticle->GetMomentum().mag();
      G4double theMass = primaryParticle->GetMass();
      mc_kineticEnergy[mc_totnumparticles] 
	= G4ToRoot((std::sqrt(totalMomentum*totalMomentum+theMass*theMass)-theMass)/GeV);
      mc_theta[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().theta())/deg);
      mc_phi[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().phi())/deg);
      mc_costheta[mc_totnumparticles]  = G4ToRoot((primaryParticle->GetMomentum().cosTheta()));
      MGLog(debugging) << totalMomentum/MeV << " " << theMass/MeV << " " << 
	mc_kineticEnergy[mc_totnumparticles]/MeV << endlog;
      mc_totnumparticles++;
    }
  }
  eventnumber = G4ToRoot(event->GetEventID());

}

void GEOutputGermaniumArrayCR::EndOfEventAction(const G4Event *evt)  
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;

//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");
  G4int waterCollID = SDman->GetCollectionID("WaterHC");
  G4int scintCollID = SDman->GetCollectionID("ScintHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* NCHC = 0; //nitrogen hits
  GEGeometrySDHitsCollection* WCHC = 0; //water hits
  GEGeometrySDHitsCollection* PSHC = 0; //scintillator hits


  if (HCE) {
      GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
      NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
      WCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(waterCollID));
      PSHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(scintCollID));
  }


  G4ThreeVector tempvector;
  
  hits_tote = 0.0;
  G4double hits_tote_lep = 0.0;
  G4double hits_tote_hadr = 0.0;

  for (G4int i=0; i<MAX_NGESEG; i++) {
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
      cout << hits_totnum << " hits generated. Currently"
	   << " only " << MAX_NHITS <<" are supported. "
	   << " EventID : " << evt->GetEventID() << endl;
      hits_totnum = MAX_NHITS;
    }
    for (G4int i=0; i<hits_totnum; i++) {
      //start new segmentation scheme
      MGGeometryDetector* theDet = 
	MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
      GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);      
      GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();


       
      G4int  fCrystalType  = -1;
      G4int  fOuterCounter      = -1;
      G4int  fInnerCounter      = -1;
      //G4int  fIsTheSecondInAPair = 0;
      G4int  fCopyNumberCounter = -1;
      G4bool fCrystalFound = false;

      G4int fCountSegments = -1;

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

      G4int NPhiSegments  = detPar->GetNPhiSegments();
      G4int NZSegments    = detPar->GetNZSegments();      

      if(NPhiSegments == 0) NPhiSegments = 1;
      if(NZSegments == 0) NZSegments = 1;
      G4double CrystalHeight = detPar->GetHeight() - detPar->GetDLTopThickness() - detPar->GetPassivationThickness();       
      
      G4ThreeVector localPosition = (*GCHC)[i]->GetLocalPos();
      G4double fPhi; 
      if( localPosition.phi() < 0){  fPhi = (2 * TMath::Pi() ) + localPosition.phi();}
      else {fPhi = localPosition.phi();}
      G4double fZ = localPosition.z() + 0.5 * CrystalHeight;
      if(fZ<0 && detPar->GetGeometry()!=4){
      	if (fZ<0 && detPar->GetGeometry()!=5){MGLog(warning)<<"fZ is smaller zero! THIS SHOULD NOT HAPPEN!!! fZ = "<<fZ<<endlog;}}
      G4int fSegID = -1;
      
      if(NZSegments == 1 && NPhiSegments == 1){fSegID = fCountSegments +1;}
      
      else{
	for(G4int j=0; j< NZSegments;j++){
	  for(G4int k=0; k< NPhiSegments;k++){
	    
	    if( ( fZ > CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * (j+1) ) && (fZ <= CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * j) ) {

	      //ok we might have an angular offset since crystal is not aligned with holder!!
	      double angularOffset =   theDetDB->GetSegmentationOffsetAngle();
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
		//case 3': if both are bigger than 2Pi subtract 2Pi
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
      
       //end new segmentation scheme
       
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
       if ((hits_trackpdg[i] == 11) || (hits_trackpdg[i] = -11) || 
	   (hits_trackpdg[i] == 13) || (hits_trackpdg[i] = -13) || 
	   (hits_trackpdg[i] == 22))
	 {
	   hits_tote_lep += (*GCHC)[i]->GetEdep() / MeV;
	 }
       else
	 {
	   hits_tote_hadr += (*GCHC)[i]->GetEdep() / MeV;
	 }

       // for individual detector spectra
       edep_allgedetreally[hits_iddet[i]] += hits_edep[i];
       numhits_allgedetreally[hits_iddet[i]]++;

       // for individual segment spectra
       edep_allgedet[hits_idseg[i]] += hits_edep[i];
       numhits_allgedet[hits_idseg[i]]++;
    }
  }

  seg_totnum=0;

  energyratio = -1.;
  if (hits_tote > 0)
    energyratio = G4ToRoot(hits_tote_lep/hits_tote);
  
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

  //Energy deposit in LNitrogen
  G4double nitrogen_edep = 0.0;
  if (NCHC)
    {
      for (G4int i=0; i<(G4int)NCHC->entries(); i++) 
	{
	  nitrogen_edep += (*NCHC)[i]->GetEdep();
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
 

  //*****************************************************************
  //Fill txt file //this is a test
  // if (seg_totnum > 0) {
  //    fout << eventnumber << " " << seg_totnum << " ";
  //    for (G4int jj=0;jj<MAX_NGESEG;jj++)
  //	{
  //	  fout << edep_allgedet[jj] << " ";
  //	}
  //   fout << scintillator_edep/MeV << " " << nitrogen_edep/MeV <<  " " << water_edep/MeV << endl;
  // }

      

  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;
  MGLog(debugging) << "Energy in water " << water_edep/GeV << " GeV" << endlog;
  MGLog(debugging) << "Energy in nitrogen " << nitrogen_edep/MeV << " MeV" << endlog;
  MGLog(debugging) << "Energy in plastic scintillator " << scintillator_edep/MeV << " MeV" << endlog;


//
// all trajectories
//
  if (NeedTrajectoryInformation){
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  trj_totnum  = 0;
  trjp_totnum = 0;
  G4String volnametemp;
  if (trajectoryContainer) trj_totnum = trajectoryContainer->entries();
  if (trj_totnum > MAX_NTRJ ) {
//      MGLog(warning) << trj_totnum << " trj generated. Currently"
      cout << trj_totnum << " trj generated. Currently"
                     << " only " << MAX_NTRJ <<" are supported. "
                     << " EventID : " << evt->GetEventID() << endl;
//                     << " EventID : " << evt->GetEventID() << endlog;
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
    trj_npoints[i]      =trj->GetPointEntries();
    trj_istart[i]       =trjp_totnum;
    trjp_totnum        +=trj_npoints[i];
    trj_iend[i]         =trjp_totnum-1;
    if (trjp_totnum > MAX_NTRJPOINT ) {
//      MGLog(warning) << trjp_totnum << " trjp generated. Currently"
      cout << trjp_totnum << " trjp generated. Currently"
                     << " only " << MAX_NTRJPOINT <<" are supported. "
                     << " EventID : " << evt->GetEventID() << endl;
//                     << " EventID : " << evt->GetEventID() << endlog;
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
      if (volnametemp.find("Ge")!=string::npos) {trjp_insidege[jj]=1;}
      if (j==0)                                 {trjp_insidege[jj]=1;}
//      G4cout<<" "<<volnametemp<<" "<<trjp_insidege[jj]<<G4endl;
     }
    }
  } 
  }
 
  if ((IsMother() && fFLAG_RequireHit == true && hits_tote>0) || 
      (IsMother() && fFLAG_RequireHit == false))
    FillTree(); 
  
  //  if (IsMother()) FillTree();
  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

}

void GEOutputGermaniumArrayCR::EndOfRunAction()
{
  /*
  if(IsMother())
    CloseFile();
  MGLog(trace) << "EndOfRunAction finished." << endlog;
  //fout.close();
  */
}

void GEOutputGermaniumArrayCR::RootSteppingAction(const G4Step *step)
{
  //This Stepping action kills long-lived nuclei (they do not decay)
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
	  //stable nuclei are unaffected
	}
    }
  
// Check for neutrons in Ge
  if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition())
    {
      G4double neuEnergy = step->GetTrack()->GetKineticEnergy();
      if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe")
	{
	  G4ThreeVector position = step->GetTrack()->GetPosition();
	  G4ThreeVector momentum = step->GetTrack()->GetMomentumDirection();
	  G4String neuFileName = GetFileName()+".neu";
	  std::ofstream neufile(neuFileName,std::ios::app);
	  neufile << step->GetTrack()->GetTrackID() << " " << 
	  step->GetTrack()->GetParentID() << " " <<neuEnergy/keV << " " << 
	  position.x()/mm << " " << position.y()/mm << " " << 
	  position.z()/mm << " " << momentum.x() << " " << momentum.y() << " " << 
	  momentum.z() << G4endl;
	  //step->GetTrack()->SetTrackStatus(fStopAndKill); //avoids double counting of neutrons
	  neufile.close();
	  neutronflag = 1;
	}
      if (neuEnergy > 19.999*MeV && neuEnergy < 20.001*MeV)
	{
	  step->GetTrack()->SetKineticEnergy(19.99*MeV);
	  MGLog(warning) << "Ugly thing: changed neutron energy from " << neuEnergy <<
	    " to 19.99 MeV" << endlog;
	  //this is due to the fact that 20-MeV neutrons may enter in an infinite loop
	}
    }
  if (step->GetTrack()->GetDefinition() == G4Proton::Definition())
    {
      if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe")
	{
	  neutronflag = 1;
	}
    }
  if (step->GetTrack()->GetDefinition() == G4MuonMinus::Definition() ||
      step->GetTrack()->GetDefinition() == G4MuonPlus::Definition())
      {
	if (step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe")
	  {
	    muonflag = 1;
	  }
      }
  if ((step->GetTrack()->GetMaterial()->GetName() == "EnrichedGe") &&
      (step->GetTrack()->GetDefinition() == G4Gamma::Definition()))
    {
      if (step->GetTrack()->GetParentID()>0) 
	{
	  G4String name = step->GetTrack()->GetCreatorProcess()->GetProcessName();
	  if (name == "nCapture" || name == "nInelastic")
	    {
	      inelasticflag = 1;
	    }
	}
    }
  //
  if (step->GetTrack()->GetCurrentStepNumber() > 100000)
    {
      step->GetTrack()->SetTrackStatus(fStopAndKill);
      MGLog(warning) << "Killed particle in infinite looping" << G4endl;
    }
  
  //Check for whatever passes through a spherical surface of 30 cm radius
  G4double sphereRadius = 30.0*cm;
  G4ThreeVector preStepPoint = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector postStepPoint = step->GetPostStepPoint()->GetPosition();
  //check if the sphere is crossed
  if ((preStepPoint-fArrayCenter).mag() >= sphereRadius 
      && (postStepPoint-fArrayCenter).mag() < sphereRadius)
    {
      G4double energy = step->GetTrack()->GetKineticEnergy();
      G4int ID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
      G4String fluxFileName = GetFileName()+".flu";
      std::ofstream fluxfile(fluxFileName,std::ios::app);
      fluxfile << ID << " " << energy/MeV << " " << preStepPoint.x()/cm << 
	" " << preStepPoint.y()/cm << " " << preStepPoint.z()/cm << G4endl;
      fluxfile.close();
    }

  //Kill particles too much below GERDA: Luciano hack
  //if (step->GetPreStepPoint()->GetPosition().z() < -5.5*m)
  //  step->GetTrack()->SetTrackStatus(fStopAndKill);


}

G4ClassificationOfNewTrack GEOutputGermaniumArrayCR::StackingAction(const G4Track *track)
{
  //if (!neutronflag) //if it is already "1" do nothing!
  //  {
  //    if (track->GetDefinition()->GetParticleName() == "neutron") neutronflag = 1; 
  //  }    

  
//1) Check for isotopes and fill a .txt file
  G4String particleType = track->GetDefinition()->GetParticleType();
  G4String volumeName = " ";
 
  if (particleType == "nucleus" && track->GetParentID()>0)
   {
     G4String processname = track->GetCreatorProcess()->GetProcessName();
     G4Ions* ion = (G4Ions*) track->GetDefinition();
     G4double lifetime = ion->GetPDGLifeTime();
     G4String particleName = track->GetDefinition()->GetParticleName();
     volumeName = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->
       LocateGlobalPointAndSetup(track->GetPosition())->GetName();
     G4int Z = ion->GetAtomicNumber();
     G4double excitationEnergy = ion->GetExcitationEnergy();
     if (lifetime > 1e18*second) lifetime = -1; //too long-lived
     if (lifetime > 0 || excitationEnergy > 0 || (Z > 30 && Z < 40))
       {
         G4int A = ion->GetAtomicMass();
         G4ThreeVector position = track->GetPosition(); 
	 G4double kinE = track->GetKineticEnergy();
	 G4String fileName = GetFileName()+".iso";
	 std::ofstream foutIso(fileName,std::ios::app);
         foutIso << A << " " << Z << " " << particleName << " " << kinE/keV << " ";
         foutIso << position.x()/m << " " << position.y()/m << " " << position.z()/m << " ";
         foutIso << volumeName << " " << processname << endl;
	 foutIso.close();
       }
   }
  if (track->GetDefinition() == G4Triton::TritonDefinition())
    {
      volumeName = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->
       LocateGlobalPointAndSetup(track->GetPosition())->GetName();
      G4String processname = track->GetCreatorProcess()->GetProcessName();
      G4ThreeVector position = track->GetPosition(); 
      G4double kinE = track->GetKineticEnergy();
      G4String fileName = GetFileName()+".iso";
      std::ofstream foutIso(fileName,std::ios::app);
      foutIso << 3 << " " << 1 << " " << "triton" << " " << kinE/keV << " ";
      foutIso << position.x()/m << " " << position.y()/m << " " << position.z()/m << " ";
      foutIso << volumeName << " " << processname << endl;
      foutIso.close();
    }

  return fUrgent;
}
//------------------------------------------------------------------------------------------
void GEOutputGermaniumArrayCR::FillDetectorTypeMatrix()
{
  MGGeometryDetector* theDet = 
    MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
  GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);
        
  
  GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();
  
 // read in geometries 
  for (G4int j = 0; j < MaxNrDetInOneString; j++)
    for (G4int i = 0; i < 19; i++)
        for (G4int k = 0; k < 2; k++)
      fArrayDetectorTypeMatrix[i][j][k] = theDetDB->GetCrystalType(i,j,k);

  //std::ifstream fInputFileMatrix; 


  //fInputFileMatrix.open(theDetDB -> GetCrystalMatrixFilename()); 

  // read in geometries 
  //for (G4int j = 0; j < MaxNrDetInOneString; j++)
    //for (G4int i = 0; i < 19; i++)
      //{
	//G4int type; 
	//fInputFileMatrix >> type; 
	//fArrayDetectorTypeMatrix[i][j][k] = type; 

      //}
  
  // close file 

  //fInputFileMatrix.close(); 

}
