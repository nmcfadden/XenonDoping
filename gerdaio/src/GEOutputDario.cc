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
 * CLASS IMPLEMENTATION:
 *   GEOutputDario
 *
 * AUTHOR:  Luciano
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 *  03-06-2007  energyHisto is deleted by ->Delete(). No need to delete it again at the distructor
 *  04-02-2007  added information of energy in sensitive materials different from Ge
 *  06-15-2007  added a new histogram with the spectrum of gammas from decays
 *  07-12-2007  info about primary spectrum written also in the txt output
 *  07-20-2007  allows multiple runs within the same ROOT file
 *  05-15-2009  add a messenger
 *  06-16-2009  change format/information of the ASCII output file
 *  09-15-2009  log separately energies of sensitive detectors
 *  02-03-2010  added R90 information in the output file
 *              give local coordinates in the ASCII output file
 *  02-03-2010  initialize fUseLocalCoordinates to true
 *  15-03-2012  Energy stored in the ntuple, Krzysztof
 *
 */

//
// include files for ROOT
#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TObject.h"
#include "TNtuple.h"

#include <sstream>
#include <string>


// Include files for the G4 classes
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
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
#include "G4PhysicalVolumeStore.hh"
#include "G4VSensitiveDetector.hh"

// Classes needed for access to hit information.
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometrySDHit.hh"
#include "gerdaio/GEOutputDario.hh"    
#include "gerdaio/GETrajectory.hh"    
#include "gerdaio/GEOutputDarioMessenger.hh"

using namespace std;

using namespace CLHEP;

//---------------------------------------------------------------------------//
GEOutputDario::GEOutputDario():
  MGOutputRoot(true),gNavigator(0),energyHisto(0),primaryGammaHisto(0),
  primaryGammaHisto2(0),fHitOutputFile(0),energyInDetectors(0)
{
  numberOfPrimaries = 0;
  fMajority = 1; //write data if at least one detector is hit
  SetSchemaDefined(false);
  rootFileOpened = false;
  fWriteHitDetailFlag = false;
  fUseLocalCoordinates = true;
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fMessenger = new GEOutputDarioMessenger(this);
  GeDetectorNames.clear();
  R90 = 0;
}

//---------------------------------------------------------------------------//

GEOutputDario::~GEOutputDario()
{
  if (fMessenger) delete fMessenger;
  if (energyInDetectors) delete energyInDetectors;
  GeDetectorNames.clear();
}

//---------------------------------------------------------------------------//

void GEOutputDario::DefineSchema()
{
  if(!SchemaDefined()){
    TTree *nT;
    if(fTree)
       nT = fTree;
    else {
      if(!IsMother())
        MGLog(warning) << "No tree assigned to child output class." << endlog;
      nT = fTree = new TTree("fTree", "Germanium Detector Tree");
    }
    
    //Histograms
    energyHisto = new TH1D("h1","ADC spectrum",8192,0.,8191.);
    //8k channels, 1 bin = 1 keV

    primaryGammaHisto = new TH1D("h2","Primary gammas",8192,0.,8191.);
    //1 bin = 1 keV

    primaryGammaHisto2 = new TH1D("h3","Primary gammas",40000,-0.05,3999.95);
    //1 bin = 0.1 keV

    nT->Branch("eventnumber",&eventnumber,"eventnumber/I");
  
   //Prepare output branches
    energyInDetectors = new std::vector<Float_t>;
    for (size_t i=0;i<GeDetectorNames.size();i++)
      energyInDetectors->push_back(0.);

    for (size_t i=0;i<GeDetectorNames.size();i++)
      {
	G4String detName = "ene_" + GeDetectorNames[i];
	G4String detName2 = detName + "/F";
	nT->Branch(detName.c_str(),&((*energyInDetectors)[i]),detName2.c_str());
      }

    nT->Branch("R90",&R90,"R90/F");
    nT->Branch("primaryVertex",primVertex,"primVertex[3]/F");
    nT->Branch("adcEnergy",&tote,"adcEnergy/F");
    SetSchemaDefined(true);
  }

  //create a file
  if (fWriteHitDetailFlag)
    {
      if (!fHitOutputFile)
	fHitOutputFile = new std::ofstream();
      G4String theFile = GetFileName()+".hits";
      fHitOutputFile->open(theFile);
    }
}

//---------------------------------------------------------------------------//

void GEOutputDario::BeginOfRunAction()
{
  //The file opening is done only at the first run
  if (!rootFileOpened)
    {
      //Count the number of Ge Detectors in the geometry
      G4PhysicalVolumeStore* thePVStore = G4PhysicalVolumeStore::GetInstance();
      for(size_t i=0; i<thePVStore->size(); i++) 
	{
	  G4VSensitiveDetector* SensDet =  (*thePVStore)[i]->GetLogicalVolume()->GetSensitiveDetector();
	  if (SensDet)
	    {
	      GeDetectorNames.push_back((*thePVStore)[i]->GetName());
	    }
	}
      MGLog(trace) << "Found: " << GeDetectorNames.size() << " sensitive detectors" << endlog; 
      MGLog(trace) << "Opening root file" << endlog;
      if(IsMother()) OpenFile();
      //    OpenRootFile(GetFileName());
      DefineSchema();
      rootFileOpened = true;
    }
}

//---------------------------------------------------------------------------//

void GEOutputDario::BeginOfEventAction(const G4Event *event)
{
  numberOfPrimaries = 0;
  MGLog(debugging) << "Start of event # " << event->GetEventID() << endlog;
  eventnumber = G4ToRoot(event->GetEventID());
  //G4int vertexNumber = event->GetNumberOfPrimaryVertex();
  G4PrimaryVertex   *primaryVertex = event->GetPrimaryVertex(0);
  if(!primaryVertex) {
    MGLog(error) << "Bad primary vertex pointer." << endlog;
    MGLog(fatal) << endlog;
  }
  primVertex[0] = G4ToRoot(primaryVertex->GetX0()/cm);
  primVertex[1] = G4ToRoot(primaryVertex->GetY0()/cm);
  primVertex[2] = G4ToRoot(primaryVertex->GetZ0()/cm);
  return;
}

//---------------------------------------------------------------------------//

void GEOutputDario::EndOfEventAction(const G4Event *evt)  
{
  MGLog(debugging) << "End of event # " << evt->GetEventID() << endlog;

//
// hits in sensitive detectors 
//

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int geCollID = SDman->GetCollectionID("GermaniumHC");

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  GEGeometrySDHitsCollection* GCHC = 0; //germanium hits


  if (HCE) {
    GCHC = (GEGeometrySDHitsCollection*)(HCE->GetHC(geCollID));
  }


  G4ThreeVector tempvector;
  
  hits_tote = 0.0;
  ene_in_argon = 0.0;
  G4double firstHitTime = 0;
 
  if (GCHC){    
    hits_totnum = GCHC->entries();  
    if (hits_totnum > 0)
      firstHitTime = (*GCHC)[0]->GetHittime();
   

    std::vector<G4ThreeVector> *centerOfEnergy = new std::vector<G4ThreeVector>;

    for (size_t k=0;k<GeDetectorNames.size();k++)
      {
	(*energyInDetectors)[k] = 0.;
	centerOfEnergy->push_back(G4ThreeVector(0,0,0));
      }
   

    //Done, now process hits.
    for (G4int i=0; i<hits_totnum; i++) 
      {
	G4String detName = (*GCHC)[i]->GetPhysicalVolume()->GetName();
	for (size_t k=0;k<GeDetectorNames.size();k++)
	  {
	    if (GeDetectorNames[k] == detName)
	      {
		(*energyInDetectors)[k]  += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );		
		(*centerOfEnergy)[k] += ((*GCHC)[i]->GetEdep()/MeV)*(*GCHC)[i]->GetPos();
	      }
	  }
	G4String materialName = 
	  (*GCHC)[i]->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
	if (materialName == "NaturalGe" || materialName == "EnrichedGe")
	  hits_tote += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
	else
	  ene_in_argon += G4ToRoot( (*GCHC)[i]->GetEdep() / MeV );
	if ((*GCHC)[i]->GetHittime() < firstHitTime)
	  firstHitTime = (*GCHC)[i]->GetHittime();	 	
      }
  

    //
    for (size_t k=0;k<GeDetectorNames.size();k++)
      {
	if ((*energyInDetectors)[k] > 0)
	  (*centerOfEnergy)[k] /= (*energyInDetectors)[k];
	MGLog(debugging) << "Center of mass of detector: " << GeDetectorNames[k] << " " << 
	  (*centerOfEnergy)[k]/cm << " cm " << endlog;
      }

    G4double totEnergy = 0;
    G4double targetEnergy = 0;
    G4int kDet = -1;
    for (size_t k=0;k<GeDetectorNames.size();k++)
      {
	if (GeDetectorNames[k] == "physiDet")
	  {
	    targetEnergy = (*energyInDetectors)[k];
	    kDet = (G4int) k;
	  }
      }

    if (targetEnergy > 0 && kDet >= 0)
      {	
	//Now, loop again
	std::vector<G4double>* distancesOrdered = new std::vector<G4double>();
	std::map<G4double,G4int>* mylist = new std::map<G4double,G4int>;
	
	for (G4int i=0; i<hits_totnum; i++) 
	  {
	    if ((*GCHC)[i]->GetPhysicalVolume()->GetName() == "physiDet")
	      {
		G4double distance = ((*GCHC)[i]->GetPos()-(*centerOfEnergy)[kDet]).mag();
		//G4cout << i << " " << (*GCHC)[i]->GetPos()/cm << " " << (*GCHC)[i]->GetEdep()/MeV << 
		//  " " << (*centerOfEnergy)[kDet]/cm << " " << distance/cm << G4endl;
		mylist->insert(std::make_pair(distance,i));
		distancesOrdered->push_back(distance);
	      }	    
	  }
	//sort vector
	sort(distancesOrdered->begin(),distancesOrdered->end());
	
	
	size_t kk=0;
	//Now sum energies according to distance
	for (kk=0;kk<mylist->size();kk++)
	  {
	    G4double targetDistance = (*distancesOrdered)[kk];
	    G4int index = mylist->find(targetDistance)->second;
	    //G4cout << kk << " " << targetDistance << " " << index << " " << 
	    //  totEnergy << " " << targetEnergy << endl;
	    totEnergy += G4ToRoot( (*GCHC)[index]->GetEdep() / MeV );	
	    if (totEnergy >= 0.9*targetEnergy) 
	      break;
	  }
	R90 = G4ToRoot((*distancesOrdered)[kk]/cm); 
	MGLog(debugging) << "R90 = " << R90 << " cm" << endlog;
	

	delete distancesOrdered;
	delete mylist;
      }
    delete centerOfEnergy;
  }


  energyHisto->Fill((Double_t) (hits_tote/keV));
  tote =   G4ToRoot((Double_t) (hits_tote/keV));
  MGLog(debugging) << "Energy in germanium " << hits_tote/keV << " keV" << endlog;
  MGLog(debugging) << "Energy outside " << ene_in_argon/keV << " keV" << endlog;

  G4int majorityOfThisEvent = 0;
  for (size_t k=0;k<GeDetectorNames.size();k++)
    if ((*energyInDetectors)[k] > 10*eV) majorityOfThisEvent++;

  if (IsMother()) 
    {
      if (majorityOfThisEvent >= fMajority)
	FillTree();
    }
  MGLog(debugging) << "EndOfEventAction Finished." << endlog;

  if (fWriteHitDetailFlag)
    {
      G4int eveID =  evt->GetEventID();
      if (GCHC)
	{
	  if (hits_totnum)
	    {
	      (*fHitOutputFile) << eveID << " " << hits_totnum << " " << numberOfPrimaries << G4endl;
	      for (G4int i=0; i<hits_totnum; i++) 
		{
		  G4ThreeVector position(0,0,0);
		  if (fUseLocalCoordinates)
		    position = (*GCHC)[i]->GetLocalPos(); 
		  else
		    position = (*GCHC)[i]->GetPos(); 

		  G4double time = (*GCHC)[i]->GetHittime();
		  G4double edep = (*GCHC)[i]->GetEdep();
		  G4int particleID = (*GCHC)[i]->GetTrackPDG();
		  G4int trackID = (*GCHC)[i]->GetTrackID();
		  G4int trackParentID = (*GCHC)[i]->GetParentTrackID();
		  //Something to do here
		  if (!(fHitOutputFile->is_open()))
		    {
		      MGLog(error) << "Something wrong with the .hits output file" << endlog;
		    }
		  else
		    {
		      
		      (*fHitOutputFile) << position.x()/cm << " " << position.y()/cm << " " << 
			position.z()/cm << " " << edep/keV << " " << (time-firstHitTime)/ns << " " << 
			particleID << " " << 
			trackID << " " << 
			trackParentID << " " << 
			(*GCHC)[i]->GetPhysicalVolume()->GetName() << G4endl;		      
		    }
		}
	    }
	}
      
    }
}

//---------------------------------------------------------------------------//

void GEOutputDario::EndOfRunAction()
{
    MGLog(trace) << "EndOfRunAction finished." << endlog;
}

//---------------------------------------------------------------------------//

void GEOutputDario::PrepareFileForClosing()
{
  energyHisto->Write();
  if (primaryGammaHisto->GetEntries()) //write histogram only if it is not empty
    primaryGammaHisto->Write();

   if (primaryGammaHisto2->GetEntries()) //write histogram only if it is not empty
    primaryGammaHisto2->Write();

  //dump the information on a ASCII file
  G4String  fileName = GetFileName()+".dat";
  std::ofstream file(fileName);
  for (Int_t i=1;i<=energyHisto->GetNbinsX();i++)
    {
      file << i << " " << energyHisto->GetBinContent(i) << " " << 
	primaryGammaHisto->GetBinContent(i) << endl;
    }
  file.close();

  energyHisto->Delete();
  primaryGammaHisto->Delete();
  primaryGammaHisto2->Delete();

  //close the file
  if (fHitOutputFile)
    {
      fHitOutputFile->close();
      delete fHitOutputFile;
    }

}

//---------------------------------------------------------------------------//

void GEOutputDario::RootSteppingAction(const G4Step *step)
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

   //Check for gamma-rays produced by Compton scattering or annihilation
  
  /*
  if (fHitOutputFile)
    {
      G4String particleName = 
                step->GetTrack()->GetDefinition()->GetParticleName();
      G4String volumeName = step->GetTrack()->GetVolume()->GetName();
      
      if (particleName == "alpha" && volumeName == "Substrate")
	(*fHitOutputFile) << step->GetTrack()->GetKineticEnergy()/MeV << " " << 
	  step->GetStepLength()/cm << G4endl;
    }
  */

  /*
      G4VPhysicalVolume* detectorPre = step->GetPreStepPoint()->GetPhysicalVolume();
      G4String detPreName = "NULL";
      if (detectorPre) detPreName = detectorPre->GetName();
      
      G4VPhysicalVolume* detectorPost = step->GetPostStepPoint()->GetPhysicalVolume();
      G4String detPostName = "NULL";
      if (detectorPost) detPostName = detectorPost->GetName();
      
      if (detPreName == "physiDet" && detPostName != "physiDet")      
	{
	  G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	  G4int partID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
	  G4ThreeVector pos = step->GetTrack()->GetPosition();
	  G4ThreeVector dir = step->GetTrack()->GetMomentumDirection();
	  G4double kinE = step->GetTrack()->GetKineticEnergy();      
	  (*fHitOutputFile) << evID << " " << pos.x()/mm << " " << pos.y()/mm << " " << 
	    pos.z()/mm << " " << dir.x() << " " << dir.y() << " " << 
	    dir.z() << " " << dir.phi()/deg << " " << kinE/keV << " " << partID << endl;      
	}
    }
  */
}

G4ClassificationOfNewTrack GEOutputDario::StackingAction(const G4Track *track)
{
  //This code looks for gammas emitted by nuclear decays
  //and counts how many primary particles are emitted in the decay

  G4int parentID = track->GetParentID();
  G4String particleName =  track->GetDefinition()->GetParticleName();
  G4String particleType = track->GetDefinition()->GetParticleType();
  G4double energy = 0;
  if (parentID > 0)
    {
      //first daugthers
      G4String processName = track->GetCreatorProcess()->GetProcessName();
      if (processName == "RadioactiveDecay" && particleName == "gamma")
	energy = track->GetDynamicParticle()->GetKineticEnergy();
      if (processName == "RadioactiveDecay" && parentID == 1)
	numberOfPrimaries++;
    }
  else if (parentID == 0)
    {
      if (particleType != "nucleus")
	numberOfPrimaries++;
      if (particleName == "gamma")
	energy = track->GetDynamicParticle()->GetKineticEnergy();
    }
    
  if (energy > 0)
    {
      primaryGammaHisto->Fill((Double_t) (energy/keV));
      primaryGammaHisto2->Fill((Double_t) (energy/keV));
    } 

  return fUrgent;
}












