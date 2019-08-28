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

#include <string>
#include <fstream>
#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4GenericIon.hh"
#include "G4Navigator.hh"
#include "G4ParticleDefinition.hh"
#include "G4TransportationManager.hh"
#include "G4VProcess.hh"
#include "G4NucleusLimits.hh"
#include "G4RadioactiveDecay.hh"
#include "G4StepPoint.hh"
#include "G4IonTable.hh"
#include "G4OpBoundaryProcess.hh"
#include "TTree.h"

#include "io/MGLogger.hh"
#include "MGTMCEventHeader.hh"
#include "MGTMCEventSteps.hh"
#include "io/MGOutputMCOpticalRunMessenger.hh"
#include "io/MGOutputRoot.hh"

#include "io/MGOutputMCOpticalRun.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"

#include "MGTMCStepData.hh"

#include "TROOT.h"
//#include "CLHEP/Utility/defs.h"
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "G4CLHEP"
#endif 

using namespace std;

using namespace CLHEP;

const G4double MGOutputMCOpticalRun::LambdaE = twopi *1.973269602e-16 * m * GeV;

MGOutputMCOpticalRun::MGOutputMCOpticalRun(G4bool isMother) :
  MGOutputRoot(isMother),
  fMCOpticalRun(NULL),
  fMCEventHeader(NULL),
  fMCEventSteps(NULL),
  fMCEventPrimaries(NULL),
  fRunID(0),
  fNSteps(0),
  fRunDescription(""),
  fEventCounter(0),
  fPastTrackPrimaryID(-1),
  fPrimX(0),
  fPrimY(0),
  fPrimZ(0),
  fWriteFlag(false),
  fPrimaryWriteFlag(true),
  fWriteAllSteps(false),
  fWriteAllStepsInEventsThatDepositEnergy(false),
  fWriteAllSensitiveSteps(false),
  fAreSurfaceAreasPrinted(false),
  fWriteRandGenState(false),
  fKillAlphas(false), 
  fKillBetas(false), 
  fKillGammas(false), 
  fKillNeutrons(false),
  fKillAll(false),
  fStopNuclei(false)
{
  fMCEventHeader = new MGTMCEventHeader();

  // allocate event steps object:
  fMCEventSteps = new MGTMCEventSteps();
  //moved to BeginOfRunAction
  // initialize array of steps; reserve default number of steps:
  fMCEventSteps->InitializeArray(1e7);
  
  // allocate event primaries object:
  fMCEventPrimaries = new MGTMCEventSteps();
  // initialize array of primaries; reserve space for 1e6 primaries::
  fMCEventPrimaries->InitializeArray(1e7);

  fMessenger = new MGOutputMCOpticalRunMessenger(this);
  fMCOpticalRun = new MGTMCRun();
  fSensitiveIDOfPhysicalVol.clear();
  fSensitiveIDLabelScheme = kClassic;
}

MGOutputMCOpticalRun::~MGOutputMCOpticalRun()
{
  delete fMCEventHeader;
  delete fMCEventSteps;
  delete fMCEventPrimaries;
  delete fMessenger;
  delete fMCOpticalRun;
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::BeginOfEventAction(const G4Event *event)
{

  // G4Event has two random number status fields. At the start of the event,
  // GetRandomNumberStatus() returns the random generator status immediately
  // before generating primaries, but we can't trust this 100%.  The positioning
  // of SetRandomNumberStatus() for a standard event in
  // G4RunManager::ProcessOneEvent() combined with the frequent updating of the
  // random number status throughout event processing makes it seem somewhat in
  // danger of not always returning this value. For a HEPMC event or generation
  // of an event from stored tracks this is also an appropriate variable to use
  // and seems less susceptible to problems down the road.  The other option is
  // GetRandomNumberStatusForProcessing(), which returns the random generator
  // status immediately after primary event generation but before event
  // processing, and remains static throughout event processing. For now, I will
  // use the former despite the danger, since typically you would want to set
  // the random number status and generate an event of interest, so the
  // "ForProcessing" status is not so useful.  The other option is to use the
  // ForProcessing status and set the primary event by hand from the stored
  // primary event info.

  string randNumberStatus = (fWriteRandGenState) ? event->GetRandomNumberStatus() : "";

  int eventID = event->GetEventID();

  fMCEventHeader->SetEventHeader(eventID, randNumberStatus);

  fMCEventSteps->ClearSteps();
  fMCEventPrimaries->ClearSteps();
  fPrimaryWriteFlag = true;

  fMCEventSteps->SetEventID( eventID );
  fMCEventPrimaries->SetEventID( eventID );

  MGLog(debugging) << "Start of event " << eventID << endl; 
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::BeginOfRunAction()
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  runManager->StoreRandomNumberStatusToG4Event(3);

  int nEvents = runManager->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  // Root init.
  if(IsMother()) OpenFile();
  DefineSchema();
  if(MGLogger::GetSeverity() <= MGLogger::trace) fTree->Print();

  fMCOpticalRun->SetRunID( fRunID );
  fMCOpticalRun->SetNEvents( nEvents );
  fMCOpticalRun->SetDescription( fRunDescription );
  fMCOpticalRun->SetWriteAllSteps( fWriteAllSteps );
  fMCOpticalRun->SetWriteAllStepsInEventsThatDepositEnergy( fWriteAllStepsInEventsThatDepositEnergy );
  fMCOpticalRun->SetWriteRandGenState( fWriteRandGenState );
  fMCOpticalRun->SetKillAlphas( fKillAlphas );
  fMCOpticalRun->SetKillBetas( fKillBetas );
  fMCOpticalRun->SetKillGammas( fKillGammas );
  fMCOpticalRun->SetKillNeutrons( fKillNeutrons );
  fMCOpticalRun->SetStopNuclei( fStopNuclei );
  //Default value is 5000
  if(fNSteps)
    fMCEventSteps->InitializeArray(fNSteps);

  // record whether importance sampling is used -- search for importance process
  // pointer by looping over all particles
  MGLog(debugging) << "searching for importance process" << endlog;
  G4ParticleTable *theParticleTable = G4ParticleTable::GetParticleTable();                                                       
  for ( G4int iParticle = 0; iParticle < theParticleTable->entries(); iParticle++ ) {

    // get name and process list for this particl
    G4ParticleDefinition* particle = theParticleTable->GetParticle(iParticle);
    G4String particleName = particle->GetParticleName();                                 
    G4ProcessManager* pmanager = particle->GetProcessManager(); 
    G4ProcessVector* pvector = pmanager->GetProcessList();
    MGLog(debugging) << "particle = " << particleName << endlog;

    // loop over processes for this particle
    for ( G4int iProcess = 0; iProcess < pvector->size(); iProcess++) {

      // get process name
      G4VProcess* process = (*pvector)[iProcess];
      G4String processName = process->GetProcessName();
      MGLog(debugging) << "\tprocess = " << processName << endlog;

      // test whether process name is ImportanceProcess
      if ( processName == "ImportanceProcess" ) {

        MGLog(routine) 
          << "found importance process for particle=" 
          << particleName
          << endlog;

        fMCOpticalRun->SetUseImportanceSampling(true);
        fMCOpticalRun->SetBiasedParticleID( GetMaGeParticleID(particle) );

        break;  // exit process loop

      }  // end test of process name

      if ( fMCOpticalRun->GetUseImportanceSampling() ) break;  // exit particle loop

    } // end loop over process vector

  }  // end loop over particle table

  // record whether parallel worlds are used
  G4int nParaWorld = G4RunManagerKernel::GetRunManagerKernel()->GetNumberOfParallelWorld();
  bool useParallelWorld = (nParaWorld > 0) ? true : false;
  fMCOpticalRun->SetUseParallelWorld(useParallelWorld);

  // record whether tracks created by importance sampling are windowed into
  // different events
  fMCOpticalRun->SetUseImportanceProcessWindow(GetUseImportanceSamplingWindow());

  fMCOpticalRun->SetMGDORevision( MGDOUtils::GetMGDORevision() );
  fMCOpticalRun->SetMGDOTag( MGDOUtils::GetMGDOTag() );
  fMCOpticalRun->SetMaGeRevision( MGLogger::GetMaGeRevision() );
  fMCOpticalRun->SetMaGeTag( MGLogger::GetMaGeTag() );
  fMCOpticalRun->SetGeantVersion( runManager->GetVersionString() );
  fMCOpticalRun->SetCLHEPVersion(PACKAGE_VERSION);
  fMCOpticalRun->SetROOTVersion( gROOT->GetVersion() );

  // get valid range in A and Z for radioactive decay:

  const G4IonTable *theIonTable = G4ParticleTable::GetParticleTable()->GetIonTable();                                                       
  G4NucleusLimits nucleusLimits;
    
  // get nucleus limits:                                                                                              
  for( G4int i=0; i<theIonTable->Entries(); i++ ) {
    G4String particleName = theIonTable->GetParticle(i)->GetParticleName();                                 
    if (particleName == "GenericIon") {                        
      G4ProcessManager* pmanager = theIonTable->GetParticle(i)->GetProcessManager(); 
      G4ProcessVector* pvector = pmanager->GetProcessList();
      for( G4int j = 0; j < pvector->size(); j++) {
        G4VProcess* process = (*pvector)[j];
        G4String processName = process->GetProcessName();
        if( processName == "RadioactiveDecay" ) {
          nucleusLimits  = ((G4RadioactiveDecay*) process)->GetNucleusLimits();
          i = theIonTable->Entries();
          break;
        } // end process name if statement
      } // end loop over process vector
    } // end particle name if statement
  }  // end loop over ion table

  fMCOpticalRun->SetAmax( nucleusLimits.GetAMax() );
  fMCOpticalRun->SetAmin( nucleusLimits.GetAMin() );
  fMCOpticalRun->SetZmax( nucleusLimits.GetZMax() );
  fMCOpticalRun->SetZmin( nucleusLimits.GetZMin() );
 
  fMCOpticalRun->SetUseTimeWindow(fUseTimeWindow);
  fMCOpticalRun->SetTimeWindow( fTimeWindow );

  // Get all sensitive volumes known by the geometry.
  // Don't set override bit (only messenger calls can override)
  // Get all active crystals, or better yet, all MGCrystalData's
  // To be added later... for now, get sensitive vols from physical vol names
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  
  MGLog(routine) << "this geometry has " << volStore->size() << " volumes" << endlog;

  for(size_t i = 0; i < volStore->size(); i++ ) {
    G4VPhysicalVolume* physicalVolume = (*volStore)[i];
    string volName = physicalVolume->GetName();
    MGLog(routine) << "volume " << i << ": " <<  volName << ", " << physicalVolume << endlog;

    int sensVolID = fMCOpticalRun->GetSensitiveIDOfVolumeName(volName, false);

    if(sensVolID == 0) {
      if(fSensitiveIDLabelScheme == kClassic) {
        size_t found = volName.find("ctive");
   
        // if the volume contains "ctive" (as in Active or active), it is a
        // detector; construct an ID for it:
        if( found != string::npos ){
   
          // Assume the name is ActiveCrystalLCrystalColumnN, where L is 0-9, as in
          // MGOutputG4StepsData

          int crystalNo = 0;
          int columnNo = 0;
      
          // check whether volume name string contains "CrystalColumn" and is long enough:
          found = volName.find("CrystalColumn");
          if( ( found != string::npos ) && (volName.length() >= 28 ) ) {
            // get the crystal row number:
            crystalNo = (int) volName[13] - '0'; 

            // get the column number:
            if( volName[28] == '\0') {
              columnNo = (int) ( volName[27] - '0' );
            } else {
              columnNo = 10 * (int) ( volName[27] - '0' );
              columnNo += (int) ( volName[28] - '0' );
            }
          }

          // construct the ID -- if volume name wasn't long enough or didn't
          // contain CrystalColumn, sensVolID will just be 1000000
          sensVolID = 1000000 + columnNo*100 + crystalNo;

          MGLog(debugging) 
            << volName << " is crystal " << crystalNo << " in column " 
            << columnNo << " and has ID " << sensVolID << endlog;

        }
      }
      else if(fSensitiveIDLabelScheme == kLabelID) {
        if(volName.find(fSensitiveIDLabel.c_str()) != string::npos) {
          sensVolID = atoi(volName.substr(fSensitiveIDLabel.size()).c_str());
        }
      }
      else if(fSensitiveIDLabelScheme == kAskGeom) {
        MGGeometryDetector* mgd = MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector();
        sensVolID = mgd->GetSensitiveVolumeID(volName);
      }
      else if(fSensitiveIDLabelScheme == kFile || fSensitiveIDLabelScheme == kManual) {
        // These methods write the vol names in advance to the list in MCOpticalRun.
        // sensVolID is already set prior to entering the if clause; just need
        // this here so that the next error works correctly.
      }
      else {
        MGLog(error) << "Unknown sensitive volume labeling scheme " 
                     << fSensitiveIDLabelScheme << endlog;
      }
    }
    fSensitiveIDOfPhysicalVol[physicalVolume] = sensVolID;
    if(sensVolID != 0) {
      fMCOpticalRun->SetSensitiveIDOfVolumeName( volName, sensVolID );
      MGLog(routine) << "Added sensitive volume " << volName 
                     << " with ID " << sensVolID << endlog;
    }
  } // increment physical volume pointer

  // check the lists
  size_t nSensVols = 0;
  map<G4VPhysicalVolume*, int>::iterator it;
  for(it=fSensitiveIDOfPhysicalVol.begin(); it!=fSensitiveIDOfPhysicalVol.end(); it++) {
    if(it->second) nSensVols++;
  }
  if(fMCOpticalRun->GetNSensitiveVols() != nSensVols) {
    MGLog(warning) << "fMCOpticalRun has " << fMCOpticalRun->GetNSensitiveVols()
                   << " vols but fSensitiveIDOfPhysicalVol has " 
                   << nSensVols << " vols with ID != 0"
                   << " -- check your sensitive vol lists." << endlog
                   << "If fMCOpticalRun has more, it is likely that you specified "
                   << "either manually or by file a volname that doesn't exist."
                   << endlog;
  }

  if ( fAreSurfaceAreasPrinted ) {
    cout
      << endl
      << "--------------------------------------------------------------------"
      << endl
      << "starting surface area dump" << endl
      << "phys volume | surf area [cm^2]" << endl
      << "--------------------------------------------------------------------"
      << endl;
    for(size_t i = 0; i < volStore->size(); i++ ) {
      G4VPhysicalVolume* physicalVolume = (*volStore)[i];
      string volName = physicalVolume->GetName();
      double surfArea = physicalVolume->GetLogicalVolume()->GetSolid()->GetSurfaceArea()/cm/cm;
      cout << volName << " | " << surfArea << endl;
    }
    cout
      << "ending surface area dump" << endl
      << "--------------------------------------------------------------------"
      << endl << endl;
  }

  fMCOpticalRun->Print();
  fMCEventHeader->SetIsHeartbeatEvent(true);
  WriteEvent();
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::DefineSchema()
{
  MGLog(routine) << "defining MGOutputMCOpticalRun schema..." << endlog;
  if(!SchemaDefined()) {

    //defining Optical Maps
    TDirectory *topDir = gDirectory;
    opticalDir = GetRootFile()->mkdir("OpticalOutput");
    opticalDir->cd();
    //TODO need to make a messenger command for all these parameters...
    maxX = 300.;
    minX = -300.;
    maxY=300.;
    minY = -300;
    maxZ= 850;
    minZ = -850;
    minR = 0;
    maxR = 300.;
    gridSpacing = 5;//*mm
    QE=1;
    nbinsX = (maxX-minX)/gridSpacing;
    nbinsY = (maxY-minY)/gridSpacing;
    nbinsZ = (maxZ-minZ)/gridSpacing;
    nbinsR = (maxR-minR)/gridSpacing;
    hMap = new TH3D("OpticalMap","OpticalMap",nbinsX,minX,maxX,nbinsY,minY,maxY,nbinsZ,minZ,maxZ);
    hMapUnscaled = (TH3D*) hMap->Clone("OpticalMapUnScaled");
    hMapNorm = (TH3D*) hMap->Clone("OpticalMapNorm");
    h2DMapRZ = new TH2D("2DOpticalMapRZ","2DOpticalMapRZ",nbinsR,minR,maxR,nbinsZ,minZ,maxZ);
    h2DMapRZUnscaled = (TH2D*) h2DMapRZ->Clone("2DOpticalMapRZUnscaled");
    h2DMapRZNorm = (TH2D*) h2DMapRZ->Clone("2DOpticalMapRZNorm");  
    h2DMapXY = new TH2D("2DOpticalMapXY","2DOpticalMapXY",nbinsX,minX,maxX,nbinsY,minY,maxY);
    h2DMapXYUnscaled = (TH2D*) h2DMapXY->Clone("2DOpticalMapXYUnscaled");
    h2DMapXYNorm = (TH2D*) h2DMapXY->Clone("2DOpticalMapXYNorm");
    h2DMapYZ = new TH2D("2DOpticalMapYZ","2DOpticalMapYZ",nbinsY,minY,maxY,nbinsZ,minZ,maxZ);
    h2DMapYZUnscaled = (TH2D*) h2DMapYZ->Clone("2DOpticalMapYZUnscaled");
    h2DMapYZNorm = (TH2D*) h2DMapYZ->Clone("2DOpticalMapYZNorm"); 
    hWeight = new TH1D("rWeight","rWeight",h2DMapRZ->GetNbinsX(),minR,maxR);
    MGLog(routine) << " INITIALIZATION file has:"  << endl;
    topDir->cd();

    // Create output Tree if it has not been assigned.
    if(fTree == NULL) {
      if(!IsMother()) MGLog(warning) << "No tree assigned to child output class." << endlog;
      fTree = new TTree("fTree", "Generic step output Tree");
    }
    
    fTree->Branch("fMCOpticalRun", "MGTMCRun", &fMCOpticalRun, 32000, 3);
    fTree->Branch("eventHeader", "MGTMCEventHeader", &fMCEventHeader, 32000, 3);
    fTree->Branch("eventSteps", "MGTMCEventSteps", &fMCEventSteps, 32000, 3);
    fTree->Branch("eventPrimaries", "MGTMCEventSteps", &fMCEventPrimaries, 32000, 3);
    //fTree->Branch("eventTotal","EventTotal",&fNevents,32000,1);  

    SetSchemaDefined(true);
    MGLog(debugging) << "MGDO MGTMCRun output schema defined." << G4endl; 
  } else MGLog(warning) << "Schema already defined." << endlog;

}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::EndOfEventAction(const G4Event*)
{
  WriteEvent();
  fOffsetTime = 0.0;
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::EndOfRunAction()
{
  fMCEventHeader->SetIsHeartbeatEvent(true);
  fMCEventSteps->ClearSteps();
  fMCEventPrimaries->ClearSteps();

  const G4RunManager* runMan = G4RunManager::GetRunManager();
   fNevents = runMan->GetCurrentRun()->GetNumberOfEvent();
   // simply store the Numbers in an TNamed title
   // this is the cheapest way
   // credit to Bryan Zhu
   TNamed nevents("NumberOfEvents",to_string(fNevents));
   nevents.Write();
   TNamed nprimaries("NumberOfPrimaries",to_string(fEventCounter));
   nprimaries.Write();
  WriteEvent();
  MGLog(routine)<<fEventCounter<<" events written"<<endlog;

    // normalize maps 
  //weight 3Dhistogram
  G4int n3D = 0, nXY = 0, nYZ = 0, nRZ = 0;
  G4double totalPrimaries = GetTree()->GetBranch("eventPrimaries")->GetEntries();
  MGLog(routine)<< " totalPrimaries " << totalPrimaries << endlog;
  MGLog(routine)<< "MapX " <<hMap->GetNbinsX()<<" MapY "<<hMap->GetNbinsY()
    <<" MapZ "<<hMap->GetNbinsZ()<<" rBins "<<h2DMapRZUnscaled->GetNbinsX()<<endlog;
  for(int i = 0; i < hMap->GetNbinsX();i++){
    for(int j = 0; j < hMap->GetNbinsY();j++){
      for(int k = 0; k < hMap->GetNbinsZ();k++){
        G4double binNorm =  hMapNorm->GetBinContent(i+1,j+1,k+1);
        G4double binVal  = hMapUnscaled->GetBinContent(i+1,j+1,k+1)*QE;
        if(binVal == 0) n3D++;
        if(binNorm>0) hMap->SetBinContent(i+1,j+1,k+1,binVal/binNorm);
      }
    }
  }
  MGLog(routine)<<"finished XYZ Histogram"<<endlog;
  for(int i = 0; i < h2DMapXY->GetNbinsX();i++){
    for(int j = 0; j < h2DMapXY->GetNbinsY();j++){
      G4double binNorm = h2DMapXYNorm->GetBinContent(i+1,j+1);
      G4double binVal = h2DMapXYUnscaled->GetBinContent(i+1,j+1)*QE;
      if(binVal == 0) nXY++;
      if(binNorm>0) h2DMapXY->SetBinContent(i+1,j+1,binVal/binNorm);
    }
  }
  MGLog(routine)<<"finished XY Histogram"<<endlog;
  for(int i = 0; i < h2DMapYZ->GetNbinsX();i++){
    for(int j = 0; j < h2DMapYZ->GetNbinsY();j++){
      G4double binNorm = h2DMapYZNorm->GetBinContent(i+1,j+1);
      G4double binVal = h2DMapYZUnscaled->GetBinContent(i+1,j+1)*QE;
      if(binVal == 0) nYZ++;
      if(binNorm>0) h2DMapYZ->SetBinContent(i+1,j+1,binVal/binNorm);
    }
  }
  MGLog(routine)<<"finished YZ Histogram"<<endlog;
  for(int i = 0; i < h2DMapRZ->GetNbinsX();i++){
    for(int j = 0; j < h2DMapRZ->GetNbinsY();j++){
      G4double binNorm = h2DMapRZNorm->GetBinContent(i+1,j+1);
      G4double binVal = h2DMapRZUnscaled->GetBinContent(i+1,j+1)*QE;
      G4double r = gridSpacing*(i+1)+minR;
      G4double weight = ((r+gridSpacing)*(r+gridSpacing)-r*r)/(maxR*maxR-minR*minR);
      binVal /= weight;
      if(binVal==0) nRZ++; 
      hWeight->SetBinContent(i+1,weight);
      if(binNorm>0) h2DMapRZ->SetBinContent(i+1,j+1,binVal/binNorm);
    }
  }
  MGLog(routine)<<"finished RZ Histogram"<<endlog;
  MGLog(routine)<<"Zero bins found..."<<nXY<<" XY, "<<nYZ<<" YZ, "<<nRZ<<" RZ bins, "<<n3D <<", xyz bins"<<endlog;
  if(IsMother()) {
    GetRootFile()->ls();
    MGLog(routine) << " number of map entries  " <<  hMap->GetEntries() << " norm entries " << hMapNorm->GetEntries() << endlog;
    GetRootFile()->Write();
    CloseFile();
    SetSchemaDefined(false);
  }
}

//---------------------------------------------------------------------------//

void MGOutputMCOpticalRun::RootSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  //Large R cut
  if(sqrt(  (preStepPoint->GetPosition().x()*preStepPoint->GetPosition().x())+
            (preStepPoint->GetPosition().y()+preStepPoint->GetPosition().y())+
            (preStepPoint->GetPosition().z()+preStepPoint->GetPosition().z()) ) > 100.*cm){
    step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
  }
  G4VPhysicalVolume* physicalVolume =postStepPoint->GetPhysicalVolume();

  int sensVolID = fSensitiveIDOfPhysicalVol[physicalVolume];
  double eDep = step->GetTotalEnergyDeposit();
  //add Primary Information using prestep
  //A Unique primary is defined as:
  //---having a unique trackID
  //---having no Parent (poor primary)
  //---current step number is 1
  //if(fPastTrackPrimaryID != track->GetTrackID() && track->GetParentID() == 0 && track->GetCurrentStepNumber() == 1){
  if(track->GetParentID() == 0 && track->GetCurrentStepNumber() == 1){
    int pid = GetMaGeParticleID(track->GetDefinition());
    int trackID = track->GetTrackID();
    G4String physVolName = preStepPoint->GetPhysicalVolume()->GetName();
    int t = preStepPoint->GetGlobalTime();
    int copyNo = 0; //  kineticE = preStepPoint->GetKineticEnergy();
    double kineticE = preStepPoint->GetKineticEnergy();
    //Prestep point used for intial position of primary
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ThreeVector localPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(position);
    G4ThreeVector momentum = preStepPoint->GetMomentumDirection();
    fPrimX = position.x();fPrimY = position.y();fPrimZ = position.z();
    fMCEventPrimaries->AddStep(
        false, 
        pid, 
        trackID, 
        0,  // parent track id 
        "primaryParticle", // process name
        physVolName, 
        0, // copy no -- FIXME
        sensVolID,
        t, 
        fOffsetTime,
        0.0, // edep
        kineticE/MeV,
        0.0, // step length
        0.0, // totalTrackLength
        position.x(), position.y(), position.z(), 
        localPosition.x(), localPosition.y(), localPosition.z(), 
        momentum.x(), momentum.y(), momentum.z(),
        -1, // step number
        -1.0 // track weight
    );
    //count number of primaries per event
    fEventCounter++;
    
    //Count photons that were detected and store their intial position 
    G4double r = sqrt(fPrimX*fPrimX+fPrimY*fPrimY);
    hMapNorm->Fill(fPrimX,fPrimY,fPrimZ);
    h2DMapRZNorm->Fill(r,fPrimZ);
    h2DMapXYNorm->Fill(fPrimX,fPrimY);
    h2DMapYZNorm->Fill(fPrimY,fPrimZ);

    MGLog(debugging)<<physVolName<<", KE = "<<kineticE/keV<<"@ ("<<position.x()<<","<<position.y()
      <<","<<position.z()<<")...Step number "<<track->GetCurrentStepNumber()<<endlog;
  }
  if(fPastTrackPrimaryID != track->GetTrackID()){
    fPastTrackPrimaryID = track->GetTrackID();
  }
  const G4VProcess* creator = track->GetCreatorProcess();
  G4String creatorName;
  if(creator) creatorName = creator->GetProcessName();

  /*
  if(creatorName != "Scintillation") {
    if ( !fWriteAllSteps){
      if(fWriteAllStepsInEventsThatDepositEnergy && eDep == 0) return;
      else if(sensVolID == 0 && fWriteAllSensitiveSteps){
        return;
      }
      else if( (sensVolID  == 0 || eDep == 0) && !fWriteAllSensitiveSteps && !fWriteAllStepsInEventsThatDepositEnergy){
        return;
      }
    }
  }
  */
 
  int pid = GetMaGeParticleID(track->GetDefinition());
  int trackID = track->GetTrackID();
  int parentTrackID = track->GetParentID();
  int copyNo = 0; // FIXME
  double stepLength = step->GetStepLength();
  double totalTrackLength = track->GetTrackLength();
  int stepIndex = fMCEventSteps->GetNSteps();
  bool recordPreStep = false;
  double trackWeight = 0;//postStepPoint->GetWeight();
  //const G4VProcess* creator = track->GetCreatorProcess();
  //G4String creatorName;
  if(creator)
    creatorName = creator->GetProcessName();

  //Use track weight to pass optical creator processes to root output
  //note that creator processes are different than step processes...
  //step process is reflection, compton scatter, ect..
  //creator Process is Scintillation, radioactive decay, WLS
  //...
  //note that WLS can be both a creator and a step process because a 
  //new track is created through WLS and an old track is terminated through WLS
  if(creatorName == "Scintillation") {
    trackWeight = 1;
  }
  else if (creatorName == "OpWLS"){
    trackWeight = 2;
  }
  else if (creatorName == "Cerenkov"){
    trackWeight = 3;
  }
  //If you use the pre point you never find tracks that stop in the SiPM,
  G4String physVolName;
  if(postStepPoint->GetPhysicalVolume() != NULL)
    physVolName = postStepPoint->GetPhysicalVolume()->GetName();
  G4String prePhysVolName;
  if(preStepPoint->GetPhysicalVolume()  != NULL)
    prePhysVolName = preStepPoint->GetPhysicalVolume()->GetName();


  if( stepIndex == 0 ){ 
    recordPreStep = true;
  } else {

    const MGTMCStepData* previousStepData = fMCEventSteps->GetStep( stepIndex - 1);

    if ( previousStepData != NULL) {
      int prevTrackID = previousStepData->GetTrackID();
      if ( prevTrackID != trackID ) recordPreStep = true;
    } else MGLog(warning) << "previous step doesn't exist!" << endlog;

  } 

  double t = postStepPoint->GetGlobalTime();
  double kineticE = postStepPoint->GetKineticEnergy();

  const G4VProcess* processDefinedStep = postStepPoint->GetProcessDefinedStep();
  string procName = (processDefinedStep) ?  processDefinedStep->GetProcessName() : "";
  G4ThreeVector position = postStepPoint->GetPosition();
  G4ThreeVector momentum = postStepPoint->GetMomentum();
  int iStep = track->GetCurrentStepNumber();
  
  if(sensVolID == 0) {
    // Kill alpha or e- track  or stop nuclei if user requires it.
    if((fKillAlphas && pid == 100004002) ||
       (fKillBetas && pid == 11) ||
       (fKillGammas && pid == 22) ||
       (fKillNeutrons && pid == 2112)||
       (iStep >= fNSteps-1 )) {
      step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
      MGLog(debugging)<<"Killing track for max Nstep...iStep "<<iStep<<" fNSteps "<<fNSteps<<", last volume step was in is "<<physVolName<<endlog;
    }
    else if(fStopNuclei && pid > 100000000) step->GetTrack()->SetKineticEnergy(0.0);
  } 
  // Kill (anti)neutrinos regardless
  if(pid == -12 || pid == 12) step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);

  G4ThreeVector localPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->
  GetTopTransform().TransformPoint(position);
  if ( !fWriteAllSteps){
    if(fWriteAllStepsInEventsThatDepositEnergy && eDep == 0) return;
    else if(sensVolID == 0 && fWriteAllSensitiveSteps){
      return;
    }
    else if( (sensVolID  == 0 || eDep == 0) && !fWriteAllSensitiveSteps && !fWriteAllStepsInEventsThatDepositEnergy){
      return;
    }
  }

  fWriteFlag = true;
  fMCEventSteps->AddStep( 
    recordPreStep, 
    pid, 
    trackID, 
    parentTrackID, 
    procName, 
    physVolName, 
    copyNo,
    sensVolID,
    t, 
    fOffsetTime,
    eDep/MeV, 
    kineticE/MeV,
    stepLength,
    totalTrackLength,
    position.x(), position.y(), position.z(), 
    //localPosition.x(), localPosition.y(), localPosition.z(), 
    //changing local position to intial position of primary particle
    fPrimX,fPrimY,fPrimZ,
    momentum.x(), momentum.y(), momentum.z(),
    iStep,
    trackWeight
  );

  if(creatorName == "OpWLS" && eDep > 0){
    //Fill histograms is hit is found in SiPM
    G4double r = sqrt(fPrimX*fPrimX+fPrimY*fPrimY);
    hMapUnscaled->Fill(fPrimX,fPrimY,fPrimZ);
    h2DMapRZUnscaled->Fill(r,fPrimZ);
    h2DMapXYUnscaled->Fill(fPrimX,fPrimY);
    h2DMapYZUnscaled->Fill(fPrimY,fPrimZ);
  }

  if( (sensVolID > 0) && (eDep > 0) ){
    fMCEventHeader->AddEnergyToDetectorID( sensVolID, eDep);
    fMCEventHeader->AddEnergyToTotalEnergy( eDep );
  }

  
  //Kill all tracks after first step
  if(fKillAll &&iStep > 0){
    step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
  }
}

//---------------------------------------------------------------------------//


void MGOutputMCOpticalRun::WriteEvent()
{
  MGLog(debugging) << "Writing event " <<  fMCEventHeader->GetEventID() << ", " 
    << fMCEventSteps->GetNSteps() << " steps, " << endlog;
  if(!fWriteFlag) return;
  if(IsMother()){
    FillTree();
    fWriteFlag = false;
    //moved to where primary step is filled
    //fEventCounter++;
  }
}

int MGOutputMCOpticalRun::GetMaGeParticleID(G4ParticleDefinition* particle)
{
  // function kept for backwards compatibility (nuclei had PID == 0 before
  // G4.8.2)
  if(particle == NULL) return 0;
  else return particle->GetPDGEncoding();
}

void MGOutputMCOpticalRun::WriteFile()
{
  MGOutputRoot::WriteFile();
}


bool MGOutputMCOpticalRun::MaGeParticleIDIsGeNucleus(int pid)
{
  if(pid > 1000000000) {
    pid = abs(pid);
    pid -= 1000000000;
    G4int LL = pid/10000000;
    pid -= 10000000*LL;
    G4int Z = pid/10000;
    return Z == 32;
  }
  else return false;
}

//--------------------------------------------------------------------

int MGOutputMCOpticalRun::GetSensitiveIDofPhysicalVolume( G4VPhysicalVolume*
physicalVolume)
{
  // get sensitive volume ID from map for fast access:
  if ( fSensitiveIDOfPhysicalVol.find(physicalVolume) ==
  fSensitiveIDOfPhysicalVol.end() ) {
    MGLog(error) 
      << "physical volume " << physicalVolume 
      << " was not found in fSensitiveIDOfPhysicalVol" << endlog
      << "assigning sensVolID 0" << endlog;
    return 0;  
  }

  return fSensitiveIDOfPhysicalVol[physicalVolume];
}

void MGOutputMCOpticalRun::ReadSensIDFile(const char* filename)
{
  ifstream file(filename);
  if(!file.good()) {
    MGLog(error) << "Couldn't open sensitive vol ID file " << filename << endl;
    return;
  }

  char buffer[1024];
  int id;
  while(file.good()) {
    if(file.peek() == '#') {
      file.getline(buffer, 1023);
    }
    else {
      file >> buffer >> id;
      if(!file.good()) break;
      fMCOpticalRun->SetSensitiveIDOfVolumeName(buffer, id);
    }
  }

  file.close();
}
