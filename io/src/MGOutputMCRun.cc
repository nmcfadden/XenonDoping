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
#include "TTree.h"

#include "io/MGLogger.hh"
#include "MGTMCEventHeader.hh"
#include "MGTMCEventSteps.hh"
#include "io/MGOutputMCRunMessenger.hh"
#include "io/MGOutputRoot.hh"

#include "io/MGOutputMCRun.hh"
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

MGOutputMCRun::MGOutputMCRun(G4bool isMother) :
  MGOutputRoot(isMother),
  fMCRun(NULL),
  fMCEventHeader(NULL),
  fMCEventSteps(NULL),
  fMCEventPrimaries(NULL),
  fRunID(0),
  fNSteps(0),
  fRunDescription(""),
  fWriteAllSteps(false),
  fWriteAllStepsInEventsThatDepositEnergy(false),
  fAreSurfaceAreasPrinted(false),
  fWriteRandGenState(false),
  fKillAlphas(false), 
  fKillBetas(false), 
  fKillGammas(false), 
  fKillNeutrons(false), 
  fStopNuclei(false)
{
  fMCEventHeader = new MGTMCEventHeader();

  // allocate event steps object:
  fMCEventSteps = new MGTMCEventSteps();
  // initialize array of steps; reserve default number of steps:
  fMCEventSteps->InitializeArray();
  
  // allocate event primaries object:
  fMCEventPrimaries = new MGTMCEventSteps();
  // initialize array of primaries; reserve space for 10 primaries::
  fMCEventPrimaries->InitializeArray(10);

  fMessenger = new MGOutputMCRunMessenger(this);
  fMCRun = new MGTMCRun();
  fSensitiveIDOfPhysicalVol.clear();
  fSensitiveIDLabelScheme = kClassic;
}

MGOutputMCRun::~MGOutputMCRun()
{
  delete fMCEventHeader;
  delete fMCEventSteps;
  delete fMCEventPrimaries;
  delete fMessenger;
  delete fMCRun;
}

//---------------------------------------------------------------------------//

void MGOutputMCRun::BeginOfEventAction(const G4Event *event)
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

  fMCEventSteps->SetEventID( eventID );
  fMCEventPrimaries->SetEventID( eventID );

  MGLog(debugging) << "Start of event " << eventID << endl; 


  // Store primary information
  int iTrackCounter = 1;

  G4Navigator* navigator =
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  G4TouchableHandle touchable;

  for(int iVertex=0; iVertex < event->GetNumberOfPrimaryVertex(); iVertex++) {

    G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex(iVertex);
    G4ThreeVector position = primaryVertex->GetPosition();
    double t = primaryVertex->GetT0();

    for(int iParticle=0; iParticle < primaryVertex->GetNumberOfParticle(); iParticle++) {

      G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(iParticle);

      // FIXME: need to also check particle table and then check for
      // daughters if PID = 0; see event/src/G4PrimaryTransformer.cc
      int particleID = GetMaGeParticleID(primaryParticle->GetG4code());

      if(primaryParticle->GetDaughter() != NULL) {
        MGLog(warning) << "Primary particle had daughters: "
        << "track IDs of primary particles will be screwed up!" << endlog;
      }
      int trackID = iTrackCounter;
      iTrackCounter++;

      G4ThreeVector momentum = primaryParticle->GetMomentum();

      navigator->LocateGlobalPointAndUpdateTouchableHandle( position, momentum,
      touchable, false); 

      G4ThreeVector localPosition;
      string physVolName;
      int sensVolID = 0;

      // touchable is undefined in the world volume -- check status to avoid
      // error 
      if( !touchable ) {  
        localPosition = position;
        physVolName = "";
      } else {
        localPosition = touchable->GetHistory()->GetTopTransform().TransformPoint(position);
        G4VPhysicalVolume* physicalVolume = touchable->GetVolume();
        physVolName = physicalVolume->GetName();
        sensVolID = GetSensitiveIDofPhysicalVolume( physicalVolume );
      }

      double mass = primaryParticle->GetMass();
      double kineticE = sqrt(momentum.mag2() + mass*mass) - mass;

      fMCEventPrimaries->AddStep(
                 false, 
                 particleID, 
                 trackID, 
                 0,  // parent track id 
                 "primaryParticle", // process name
                 physVolName, 
                 0, // copy no -- FIXME
                 sensVolID,
                 t, 
                 fOffsetTime,
                 0.0, // edep
                 kineticE,
                 0.0, // step length
                 0.0, // totalTrackLength
                 position.x(), position.y(), position.z(), 
                 localPosition.x(), localPosition.y(), localPosition.z(), 
                 momentum.x(), momentum.y(), momentum.z(),
                 -1, // step number
                 -1.0 // track weight
      );
    }
  }
  navigator->LocateGlobalPointAndSetup(G4ThreeVector(0,0,0), 0, false);
}

//---------------------------------------------------------------------------//

void MGOutputMCRun::BeginOfRunAction()
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  runManager->StoreRandomNumberStatusToG4Event(3);

  int nEvents = runManager->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  // Root init.
  if(IsMother()) OpenFile();
  DefineSchema();
  if(MGLogger::GetSeverity() <= MGLogger::trace) fTree->Print();

  fMCRun->SetRunID( fRunID );
  fMCRun->SetNEvents( nEvents );
  fMCRun->SetDescription( fRunDescription );
  fMCRun->SetWriteAllSteps( fWriteAllSteps );
  fMCRun->SetWriteAllStepsInEventsThatDepositEnergy( fWriteAllStepsInEventsThatDepositEnergy );
  fMCRun->SetWriteRandGenState( fWriteRandGenState );
  fMCRun->SetKillAlphas( fKillAlphas );
  fMCRun->SetKillBetas( fKillBetas );
  fMCRun->SetKillGammas( fKillGammas );
  fMCRun->SetKillNeutrons( fKillNeutrons );
  fMCRun->SetStopNuclei( fStopNuclei );
  //Default value is 50000
  if(fNSteps)
    fMCEventSteps->InitializeArray(fNSteps);

  // record whether importance sampling is used -- search for importance process
  // pointer by looping over all particles
  MGLog(debugging) << "searching for importance process" << endlog;
  G4ParticleTable *theParticleTable = G4ParticleTable::GetParticleTable();                                                       
  for ( G4int iParticle = 0; iParticle < theParticleTable->entries(); iParticle++ ) {

    // get name and process list for this particle
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

        fMCRun->SetUseImportanceSampling(true);
        fMCRun->SetBiasedParticleID( GetMaGeParticleID(particle) );

        break;  // exit process loop

      }  // end test of process name

      if ( fMCRun->GetUseImportanceSampling() ) break;  // exit particle loop

    } // end loop over process vector

  }  // end loop over particle table

  // record whether parallel worlds are used
  G4int nParaWorld = G4RunManagerKernel::GetRunManagerKernel()->GetNumberOfParallelWorld();
  bool useParallelWorld = (nParaWorld > 0) ? true : false;
  fMCRun->SetUseParallelWorld(useParallelWorld);

  // record whether tracks created by importance sampling are windowed into
  // different events
  fMCRun->SetUseImportanceProcessWindow(GetUseImportanceSamplingWindow());

  fMCRun->SetMGDORevision( MGDOUtils::GetMGDORevision() );
  fMCRun->SetMGDOTag( MGDOUtils::GetMGDOTag() );
  fMCRun->SetMaGeRevision( MGLogger::GetMaGeRevision() );
  fMCRun->SetMaGeTag( MGLogger::GetMaGeTag() );
  fMCRun->SetGeantVersion( runManager->GetVersionString() );
  fMCRun->SetCLHEPVersion(PACKAGE_VERSION);
  fMCRun->SetROOTVersion( gROOT->GetVersion() );

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

  fMCRun->SetAmax( nucleusLimits.GetAMax() );
  fMCRun->SetAmin( nucleusLimits.GetAMin() );
  fMCRun->SetZmax( nucleusLimits.GetZMax() );
  fMCRun->SetZmin( nucleusLimits.GetZMin() );
 
  fMCRun->SetUseTimeWindow(fUseTimeWindow);
  fMCRun->SetTimeWindow( fTimeWindow );

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

    int sensVolID = fMCRun->GetSensitiveIDOfVolumeName(volName, false);

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
        // These methods write the vol names in advance to the list in MCRun.
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
      fMCRun->SetSensitiveIDOfVolumeName( volName, sensVolID );
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
  if(fMCRun->GetNSensitiveVols() != nSensVols) {
    MGLog(warning) << "fMCRun has " << fMCRun->GetNSensitiveVols()
                   << " vols but fSensitiveIDOfPhysicalVol has " 
                   << nSensVols << " vols with ID != 0"
                   << " -- check your sensitive vol lists." << endlog
                   << "If fMCRun has more, it is likely that you specified "
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

  fMCRun->Print();
  fMCEventHeader->SetIsHeartbeatEvent(true);
  WriteEvent();
}

//---------------------------------------------------------------------------//

void MGOutputMCRun::DefineSchema()
{
  MGLog(routine) << "defining MGOutputMCRun schema..." << endlog;
  if(!SchemaDefined()) {
    // Create output Tree if it has not been assigned.
    if(fTree == NULL) {
      if(!IsMother()) MGLog(warning) << "No tree assigned to child output class." << endlog;
      fTree = new TTree("fTree", "Generic step output Tree");
    }
    
    fTree->Branch("fMCRun", "MGTMCRun", &fMCRun, 32000, 3);
    fTree->Branch("eventHeader", "MGTMCEventHeader", &fMCEventHeader, 32000, 3);
    fTree->Branch("eventSteps", "MGTMCEventSteps", &fMCEventSteps, 32000, 3);
    fTree->Branch("eventPrimaries", "MGTMCEventSteps", &fMCEventPrimaries, 32000, 3);

    SetSchemaDefined(true);
    MGLog(debugging) << "MGDO MGTMCRun output schema defined." << G4endl; 
  } else MGLog(warning) << "Schema already defined." << endlog;

}

//---------------------------------------------------------------------------//

void MGOutputMCRun::EndOfEventAction(const G4Event*)
{
  WriteEvent();
  fOffsetTime = 0.0;
}

//---------------------------------------------------------------------------//

void MGOutputMCRun::EndOfRunAction()
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

  WriteEvent();
  if(IsMother()) {
    CloseFile();
    SetSchemaDefined(false);
  }
}

//---------------------------------------------------------------------------//

void MGOutputMCRun::RootSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4VPhysicalVolume* physicalVolume = track->GetVolume();

  int sensVolID = fSensitiveIDOfPhysicalVol[physicalVolume];
  double eDep = step->GetTotalEnergyDeposit();
  if ( !fWriteAllSteps){
    if(fWriteAllStepsInEventsThatDepositEnergy && eDep == 0) return;
    else if(sensVolID  == 0 || eDep == 0) return;
  }

 
  int pid = GetMaGeParticleID(track->GetDefinition());
  int trackID = track->GetTrackID();
  int parentTrackID = track->GetParentID();
  string physVolName = physicalVolume->GetName();
  int copyNo = 0; // FIXME
  double stepLength = step->GetStepLength();
  double totalTrackLength = track->GetTrackLength();
  int stepIndex = fMCEventSteps->GetNSteps();

  bool recordPreStep = false;

  if( stepIndex == 0 ){ 
    recordPreStep = true;
  } else {

    const MGTMCStepData* previousStepData = fMCEventSteps->GetStep( stepIndex - 1);

    if ( previousStepData != NULL) {
      int prevTrackID = previousStepData->GetTrackID();
      if ( prevTrackID != trackID ) recordPreStep = true;
    } else MGLog(warning) << "previous step doesn't exist!" << endlog;

  } 

  if( recordPreStep ) {

    G4StepPoint* stepPoint = step->GetPreStepPoint();

    double t = stepPoint->GetGlobalTime();
    double kineticE = stepPoint->GetKineticEnergy();

    const G4VProcess* creatorProcess = track->GetCreatorProcess();

    string procName = (creatorProcess) ?  creatorProcess->GetProcessName() : "";

    G4ThreeVector position = stepPoint->GetPosition();
            
    G4ThreeVector momentum = stepPoint->GetMomentum();

    G4ThreeVector localPosition = stepPoint->GetTouchableHandle()->
    GetHistory()->GetTopTransform().TransformPoint(position);
    int iStep = track->GetCurrentStepNumber()-1;
    double trackWeight = stepPoint->GetWeight();

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
      0.0, 
      kineticE,
      stepLength,
      totalTrackLength,
      position.x(), position.y(), position.z(), 
      localPosition.x(), localPosition.y(), localPosition.z(), 
      momentum.x(), momentum.y(), momentum.z(),
      iStep,
      trackWeight
    );

    recordPreStep = false;
  }


  G4StepPoint* stepPoint = step->GetPostStepPoint();

  //-------------------------------------------------------------------
  //This has been commented out because it can generate a segfault when
  //setWriteAllSteps is enabled and the track is in the World volume.
  //N. McFadden please fix or verify that this line is not necessary.
  //-------------------------------------------------------------------
  //for Optical Photon detection -N. McFadden
  //physVolName = stepPoint->GetPhysicalVolume()->GetName();

  double t = stepPoint->GetGlobalTime();
  double kineticE = stepPoint->GetKineticEnergy();
  double trackWeight = stepPoint->GetWeight();

  const G4VProcess* processDefinedStep = stepPoint->GetProcessDefinedStep();
  string procName = (processDefinedStep) ?  processDefinedStep->GetProcessName() : "";
  G4ThreeVector position = track->GetPosition();
  G4ThreeVector momentum = track->GetMomentum();
  int iStep = track->GetCurrentStepNumber();

  // 22 0ct 2010 -- changed to make local coords consistent with physicalVolume,
  // A. Schubert
  G4ThreeVector localPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->
  GetTopTransform().TransformPoint(position);


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
    eDep, 
    kineticE,
    stepLength,
    totalTrackLength,
    position.x(), position.y(), position.z(), 
    localPosition.x(), localPosition.y(), localPosition.z(), 
    momentum.x(), momentum.y(), momentum.z(),
    iStep,
    trackWeight
  );

  if(sensVolID == 0) {
    // Kill alpha or e- track  or stop nuclei if user requires it.
    if((fKillAlphas && pid == 100004002) ||
       (fKillBetas && pid == 11) ||
       (fKillGammas && pid == 22) ||
       (fKillNeutrons && pid == 2112)) {
      step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
    }
    else if(fStopNuclei && pid > 100000000) step->GetTrack()->SetKineticEnergy(0.0);
  }

  if( (sensVolID > 0) && (eDep > 0) ){
    fMCEventHeader->AddEnergyToDetectorID( sensVolID, eDep);
    fMCEventHeader->AddEnergyToTotalEnergy( eDep );
  }

  // Kill (anti)neutrinos regardless
  if(pid == -12 || pid == 12) step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
}

//---------------------------------------------------------------------------//


void MGOutputMCRun::WriteEvent()
{
  MGLog(debugging) << "Writing event " <<  fMCEventHeader->GetEventID() << ", " 
    << fMCEventSteps->GetNSteps() << " steps, " << endlog;
  if ( !fWriteAllSteps && (fMCEventHeader->GetTotalEnergy() == 0) && !fMCEventHeader->GetIsHeartbeatEvent() ) return;
  if(IsMother()) FillTree();
}

int MGOutputMCRun::GetMaGeParticleID(G4ParticleDefinition* particle)
{
  // function kept for backwards compatibility (nuclei had PID == 0 before
  // G4.8.2)
  if(particle == NULL) return 0;
  else return particle->GetPDGEncoding();
}

void MGOutputMCRun::WriteFile()
{
  MGOutputRoot::WriteFile();
}


bool MGOutputMCRun::MaGeParticleIDIsGeNucleus(int pid)
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

int MGOutputMCRun::GetSensitiveIDofPhysicalVolume( G4VPhysicalVolume*
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

void MGOutputMCRun::ReadSensIDFile(const char* filename)
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
      fMCRun->SetSensitiveIDOfVolumeName(buffer, id);
    }
  }

  file.close();
}

