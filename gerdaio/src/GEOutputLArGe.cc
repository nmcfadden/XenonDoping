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
// $Id: GEOutputLArGe.cc,v 1.7 2007-11-14 15:53:12 lenz Exp $ 
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
 * AUTHOR:  Luciano
 * CONTACT:
 * FIRST SUBMISSION:
 * 
 * REVISION:
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
#include "G4StepStatus.hh"

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputLArGe.hh"    
#include "gerdaio/GETrajectory.hh"    
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputLArGe::GEOutputLArGe(G4bool isMother, G4bool needt):
  MGOutputRoot(isMother),
  NeedTrajectoryInformation(needt),gNavigator(0)
{
  SetSchemaDefined(false);
  successfulInitialization = false;
  InitializePMTEfficiency();
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}


GEOutputLArGe::~GEOutputLArGe()
{;}

void GEOutputLArGe::DefineSchema()
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

    timeHisto = new TH1D("h1","Time distribution",6000,-3000.,3000.);
    energyHisto = new TH1D("h2","Frequency",600,90.,690.);

    // MC true vertex information
    nT->Branch("eventnumber",&eventnumber,"eventnumber/I");
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
    nT->Branch("mc_id",mc_id,"mc_id[mc_totnumparticles]/I");

    // hits in sensitive detectors
    nT->Branch("hits_totnum",&hits_totnum,"hits_totnum/I");
    nT->Branch("hits_tote",&hits_tote,"hits_tote/F");
    nT->Branch("hits_edep",hits_edep,"hits_edep[hits_totnum]/F");
    nT->Branch("hits_xpos",hits_xpos,"hits_xpos[hits_totnum]/F");
    nT->Branch("hits_ypos",hits_ypos,"hits_ypos[hits_totnum]/F");
    nT->Branch("hits_zpos",hits_zpos,"hits_zpos[hits_totnum]/F");
    nT->Branch("hits_idseg",hits_idseg,"hits_idseg[hits_totnum]/I");

    // how many Ge detectors have energy deposit
    nT->Branch("seg_totnum",&seg_totnum,"seg_totnum/I"); 

    // hits and deposited energy of each germanium crystal
    nT->Branch("seg_id",seg_id,"seg_id[seg_totnum]/I");
    nT->Branch("seg_numhits",seg_numhits,"seg_numhits[seg_totnum]/I");
    nT->Branch("seg_edep",seg_edep,"seg_edep[seg_totnum]/F");

    // energy deposit in argon
    nT->Branch("ene_in_argon",&ene_in_argon,"ene_in_argon/F");

    // light information
    nT->Branch("nScintillation",&nScintillation,"nScintillation/I");
    nT->Branch("nCerenkov",&nCerenkov,"nCerenkov/I");
    nT->Branch("nHitPMT",&nHitPMT,"nHitPMT/I");
    nT->Branch("nDetected",&nDetected,"nDetected/I");

    SetSchemaDefined(true);
  }
}

void GEOutputLArGe::BeginOfRunAction()
{
  if(IsMother()) OpenFile();
  DefineSchema();
}


void GEOutputLArGe::BeginOfEventAction(const G4Event *event)
{
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
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
      mc_px[mc_totnumparticles] = 
        G4ToRoot(primaryParticle->GetPx() / MeV);
      mc_py[mc_totnumparticles] = 
        G4ToRoot(primaryParticle->GetPy() / MeV);
      mc_pz[mc_totnumparticles] = 
        G4ToRoot(primaryParticle->GetPz() / MeV);
      mc_ivertex[mc_totnumparticles]=i;
      mc_totnumparticles++;
    }
  }
  eventnumber = G4ToRoot(event->GetEventID());
  nCerenkov = 0;
  nScintillation = 0;
  nHitPMT = 0;
  nDetected = 0;

  firstTime = 0.0;
  timeHisto->Reset();
  //energyHisto->Reset();
}

void GEOutputLArGe::EndOfEventAction(const G4Event *evt)  
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;

//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");
  G4int n2CollID = SDman->GetCollectionID("NitrogenHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits
  GEGeometrySDHitsCollection* NCHC = 0; //argon hits


  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
    NCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(n2CollID));
  }


  G4ThreeVector tempvector;
  
  hits_tote = 0.0;
  for (G4int i=0; i<MAX_NGESEG; i++) {
    edep_allgedet[i]=0.0;
    numhits_allgedet[i]=0;
  }

  if (GCHC){
    hits_totnum = GCHC->entries();  
    for (G4int i=0; i<hits_totnum; i++) {

      //start of new segmentation implementation

      MGGeometryDetector* theDet = 
	MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
      GEGeometryGerda* theGerdaDet = dynamic_cast<GEGeometryGerda*>(theDet);      
      GEGeometryDetectorDB* theDetDB = theGerdaDet->GetDB();


       
      G4int  fCrystalType  = -1;
      G4int  fOuterCounter      = -1;
      G4int  fInnerCounter      = -1;
      G4int  fCopyNumberCounter = -1;
      G4bool fCrystalFound = false;

      G4int fCountSegments = -1;

      while(!fCrystalFound){
	fOuterCounter++;
	fInnerCounter = -1;
	while(!fCrystalFound && fInnerCounter < 4){
	  fInnerCounter++;
	  if(fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter] >= 0 ){
	    fCopyNumberCounter++;
	    if(fCopyNumberCounter == (*GCHC)[i]->GetCopynumber() ){
	      fCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter];
	      fCrystalFound = true;   
	    }
	    
	    if(!fCrystalFound){
	      G4int fTmpCrystalType = fArrayDetectorTypeMatrix[fOuterCounter][fInnerCounter];
	      GEGeometryDetectorParameters* detPar = theDetDB->GetDetectorParameters(fTmpCrystalType);
	      if( (detPar->GetNPhiSegments() == 0) && (detPar->GetNZSegments() == 0) ){fCountSegments += 1;}
	      else{fCountSegments+= (detPar->GetNPhiSegments() * detPar->GetNZSegments());}
	    }
	    
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
      if(fZ<0){G4cout<<"fZ is smaller zero! THIS SHOULD NOT HAPPEN!!! fZ = "<<fZ<<G4endl;}
      G4int fSegID = -1;


     
       for(G4int j=0; j< NZSegments;j++){
	 for(G4int k=0; k< NPhiSegments;k++){

	   if( ( fZ > CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * (j+1) ) && (fZ <= CrystalHeight - ((double)CrystalHeight/ (double)NZSegments) * j) ) {
	     if( (fPhi >=  (2*TMath::Pi()/(double)NPhiSegments) * k) && (fPhi < (2*TMath::Pi()/(double)NPhiSegments) * (k+1) )) {
	       fSegID = ((k  * NZSegments ) + j +1) + fCountSegments ;	       
	       
	       if(fSegID == -1){
		 G4cout <<"This Shouldn't happen"<< G4endl;
		 G4cout <<"fSegID = "<< fSegID <<G4endl;
	       }
	     }  
	   }
	 }
       }

      ///end of new segmentation implementation


      hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      hits_edep[i] = G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
      tempvector = (*GCHC)[i]->GetPos();
      hits_xpos[i] = G4ToRoot(tempvector.getX()/cm);  
      hits_ypos[i] = G4ToRoot(tempvector.getY()/cm);  
      hits_zpos[i] = G4ToRoot(tempvector.getZ()/cm);  
      //      hits_idseg[i] = (*GCHC)[i]->GetCopynumber();
      hits_idseg[i] = fSegID;
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

  //Energy deposit in LAr
  G4double argon_edep = 0.0;
  if (NCHC)
    {
      for (G4int i=0; i<(G4int)NCHC->entries(); i++) 
	{
	  argon_edep += (*NCHC)[i]->GetEdep();
	}
    }

  ene_in_argon = G4ToRoot(argon_edep / MeV);

       
  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;
  MGLog(debugging) << "Energy in argon " << argon_edep/MeV << " MeV" << endlog;

  MGLog(debugging) << "Photons in PMT " << nHitPMT << endlog;
  MGLog(debugging) << "Photo electrons: " << nDetected << endlog;
 
  if (IsMother()) FillTree();
//  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

}

void GEOutputLArGe::EndOfRunAction()
{
  timeHisto->Write();
  timeHisto->Delete();
  energyHisto->Write();
  energyHisto->Delete();
  if(IsMother()) CloseFile();
  MGLog(trace) << "EndOfRunAction finished." << endlog;
}


#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

void GEOutputLArGe::RootSteppingAction(const G4Step *step)
{
  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  //Find the boundary process only once
  static G4OpBoundaryProcess* boundary = NULL;
  if (!boundary)
    {
      G4ProcessManager* pm = step->GetTrack()->GetDefinition()->GetProcessManager();
      G4int nprocesses = pm->GetProcessListLength();
      G4ProcessVector* pv = pm->GetProcessList();
      for (G4int i=0;i<nprocesses;i++)
	{
	  if ((*pv)[i]->GetProcessName()=="OpBoundary")
	    {
	      boundary = (G4OpBoundaryProcess*) (*pv)[i];
	      break;
	    }
	}
    }
  
  if (step->GetTrack()->GetDefinition()->GetParticleName() == "opticalphoton")
    {
      //Here kill photons in "wrong" places
      if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
	{
	  boundaryStatus = boundary->GetStatus();
	  //MGLog(debugging) << "BS: " << boundaryStatus << endlog;
	  if (boundaryStatus == Detection)
	    {
	      nHitPMT++;
	      G4double energy = step->GetTrack()->GetKineticEnergy();
	      energyHisto->Fill((Double_t) ((LambdaE/energy)/nanometer));
	      G4double time = step->GetTrack()->GetGlobalTime(); 
	      if (!firstTime) firstTime = time;
	      //MGLog(debugging) << (time-firstTime)/nanosecond << " ns" << endlog;
	      timeHisto->Fill((Double_t) ((time-firstTime)/nanosecond));
	      if (G4UniformRand() < PMTQuantumEfficiency(energy)) 
		nDetected++;	     
	    }

	}
    }
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
}

G4ClassificationOfNewTrack GEOutputLArGe::StackingAction(const G4Track *track)
{ 
  //Count what process generated the optical photons
  if(track->GetDefinition()->GetParticleName() == "opticalphoton")
    {
      // particle is optical photon
      if(track->GetParentID()>0){
	// particle is secondary
	if(track->GetCreatorProcess()->GetProcessName()=="Scintillation")
	  nScintillation++;
	else if(track->GetCreatorProcess()->GetProcessName()=="Cerenkov")
	  nCerenkov++;
      }
    }
  return fUrgent;
}

G4double GEOutputLArGe::PMTQuantumEfficiency(G4double energy)
{

  G4int j=0;
  if (successfulInitialization)
    {    
      //Here the data are loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;
      
      if (targetf < frequency[0] || targetf > frequency[npar-1]) 
	return 0.0;
      
      for(j=0;j<npar-1;j++)
	{
	  if (frequency[j]<targetf && targetf <= frequency[j+1]) break;
	}
      G4double eff = (targetf-frequency[j])
	*(efficiency[j+1]-efficiency[j])/(frequency[j+1]-frequency[j]);
      eff += efficiency[j];
      return (eff/2.);
      //Notice: from Stefan I know that QE is 1/2 of the one of the table
    }
  else
    {
      return 0.2;
    }

}

void GEOutputLArGe::InitializePMTEfficiency()
{
  char* path = getenv("MGGENERATORDATA");
  G4String pathString;
  if (!path)
  {
      MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
      pathString = "generators/data";
  }
  else
  {
    pathString = path;
  }
  G4String pathFile = pathString + "/pmt_sensitivity.dat";
  std::ifstream file(pathFile);
  
  if (file.is_open())
    {
      G4double bb=0;
      G4double aa = 0;
      for (G4int i=0;i<npar;i++)
	{
	  file >> aa >> bb;
	  frequency[i] = aa*nanometer;
	  efficiency[i] = bb;
	}
      file.close();
      successfulInitialization = true;
      MGLog(routine) << "PMT sensitivity data successfully read from file" << endlog; 
    }
  else
    {
      MGLog(warning) << "Data file not found!" << endlog;
      successfulInitialization = false;
    }
}

//------------------------------------------------------------------------------------------
void GEOutputLArGe::FillDetectorTypeMatrix()
{
  MGGeometryDetector* theDet = 
    MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector(); 
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
