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
// $Id: MGProcessesList.cc,v 1.22 2009-01-17 15:14:12 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessesList.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * This file is the physics list for Majorana and GERDA. It incorporates low-energy physics
 * applicable to a low-background environment, as well as hadronic interactions
 * resulting from cosmic ray spallation.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 19 14:42:36 PDT 2004
 * 
 * REVISION:
 *
 * 04-23-2004, Kareem
 *             Initial file submission
 * 09-15-2004, Kareem
 *             Added an energy realm messenger to set the energy thresholds to
 *               run double-beta simulations (energy cutoff of < 100 keV) or
 *               dark matter (energy cutoff of < 1 keV).
 * 12-15-2004, Luciano
 *             Added G4MuNuclearInteraction (gives a crash in the destructor)
 *             Added a new realm "CosmicRays" with more relaxed cuts to speed-up
 *              the simulation of CR events
 * 12-22-2004  Luciano
 *             Introduced cut-per-region 
 *             Relaxed cuts everywhere else than crystals
 * 01-11-2004  Luciano
 *             Added isotope production from neutrons
 * 02-14-2005  Luciano
 *             Added method for definition of Cerenkov emission
 * 02-16-2005  Luciano
 *             Added flag for switching between Std/LowE electromagnetic
 * 02-24-2005  Kevin
 *             Use G4MuonNucleus() instead of G4MuNuclearInteraction() (crashes at exit)
 * 02-26-2005  Luciano
 *             Correct stupid bug (in the latest changes hadronic interactions of mu- 
 *             were lost)
 * 03-11-2005  Reyco
 *             Set the default cuts for gammas, e- and e+ to 0.1mm each.
 * 04-12-2005  Luciano
 *             Registered extra processes for optical photons (Rayleigh, WLS)
 * 07-12-2005  Luciano
 *             Registered new methods for QGSP_GN_HP_BIC_ISO physics list
 *             optimized by Michael Bauer 
 *              (to be used as alternative to the "default" list)
 * 07-12-2005  Luciano
 *             BBdecay cuts are restored at default (inconsistency between constructor and SetRealm)
 *             Added annihilation processes for anti-p and anti-nu
 *             Changed PhotoNuclear process according to G4 physics lists.
 *             Added ElasticScattering for ions
 *             Pion and kaons: TheoFS model from 12 GeV instead of 15 GeV
 * 07-13-2005  Luciano
 *             Added a boolean flag to control the alternative physics list
 * 07-29-2005  Luciano
 *             Fixed bug reported by Jason concerning QGSP_GN_HP_BIC_ISO physics list
 * 02-22-2007  Luciano
 *             Removed the #ifdef USE_G8_0 directive that was used as temporary bug fix
 *             for Geant4 bug-report 846. The bug has been fixed from Geant 8.1 on. 
 * 09-04-2007  Luciano
 *             Slight change in the code for setting cuts of the SensitiveRegion
 * 10-03-2007  Luciano
 *             Renamed neutron processes (nCapture and nFission)
 * 04-14-2008  Luciano
 *             Correct the default physics list for neutrons in order to avoid energy range of 
 *             different models (LEP and QGSP) fully overlapping 
 * 04-15-2008  Luciano
 *             Correct the physics list for pi+/- when fUseQGSCFlag is true and Bertini cascade 
 *             is used (the region between 9.5 and 9.9 GeV was covered by three models), and this 
 *             caused a crash. No problem when Binary cascade was used or fUseQGSCFlag=false
 * 04-16-2008  Luciano      
 *             Added method and private members to dump physics list
 * 01-16-2009  Markus
 *             Corrected Cerenkov Process
 * 01-26-2010, Luciano
 *             Migration of physics list according to Geant4 9.3
 * 08-04-2010, A. Schubert
 *             Assigned lifetime to triton
 * 18 Nov 2011 A. Schubert
 *             Added parallel world scoring process
 * 23 Apr 2013 N. Barros
 *             Migrated Physics list to be compatible with GEANT4 9.6.1
 * 13 Aug 2016 J. Detwiler
 *             Add option to turn on Charged Particle HP / TENDL. 
 * 23 Aug 2017 R. Massarczyk added function to show set step limits
 * 24 Aug 2017 R. Massarczyk
 *             Added getStep limits to check that limits are set properly
 * 05 Apr 2018 R. Massarczyk corrected Tritium Decay
 * 25-06-2018, Ralph Massarczyk     Added different physics list options, when lowenergy flag enabled
 *             
 */
//---------------------------------------------------------------------------//
//

//---------------------------------------------------------------------------//

//  MG headers
#include "processes/MGProcessesList.hh"      //Present MG Class Headers 
#include "processes/MGProcessesMessenger.hh"
#include "io/MGLogger.hh"

//  GEANT4 headers
#include "globals.hh"
#include "G4Version.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4DataQuestionaire.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4ios.hh"
#include <iomanip>  
#include <sstream>

#include "G4UserLimits.hh"
#include "G4StepLimiter.hh"
#include "G4UImanager.hh"

using namespace CLHEP;
using namespace std;

#ifndef theParticleIterator
#define theParticleIterator GetParticleIterator()
#endif

#ifndef theParticleIterator
#define theParticleIterator GetParticleIterator()
#endif

// Constructor /////////////////////////////////////////////////////////////
MGProcessesList::MGProcessesList() : G4VModularPhysicsList(), 
				     fEMConstructor(0)
{

  processesMessenger = new MGProcessesMessenger( this );

  //  The default values for the energy thresholds are tuned to 100 keV
  //  in natural germanium (i.e., the BBdecay realm)
  defaultCutValue     = 0.1*mm;
  cutForGamma = 0.1*mm;
  cutForElectron = 0.1*mm;
  cutForPositron = 0.1*mm;
  cutForGammaSensitive = 0.1*mm;
  cutForElectronSensitive = 0.1*mm;
  cutForPositronSensitive = 0.1*mm;
  cutForProton = 0.;
  cutForAlpha = 0.;
  cutForGenericIon = 0.;
  SetCuts();
  
  VerboseLevel = 1;
  OpVerbLevel = 0;

  G4DataQuestionaire it(photon, neutron, radioactive);

  constructOptical = false;
  useLowE = true;
	fuseLowEoption = 0;
  fUseBertiniCascade = true; // using Bertini cascade instead of Binary
  fUseFTFFlag = false;
  fUseNoHadPhysFlag = false;
  fUseAllHPFlag = false;
  fAngCorrMaxTwoJ = 0;
  fUseOpticalPhysicsOnly = false;
  // optimized DM physics list
  
  fPhysicsListHadrons = " ";
  
  SetVerboseLevel(VerboseLevel);
	
}

// Destructor //////////////////////////////////////////////////////////////
MGProcessesList::~MGProcessesList()
{
  delete processesMessenger;
}

// Construct Particles /////////////////////////////////////////////////////
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
void MGProcessesList::ConstructParticle() 
{
  
  G4BosonConstructor pBoConstructor;
  pBoConstructor.ConstructParticle();
  
  G4LeptonConstructor pLConstructor;
  pLConstructor.ConstructParticle();
  
  G4MesonConstructor pMConstructor;
  pMConstructor.ConstructParticle();
  
  G4BaryonConstructor pBaConstructor;
  pBaConstructor.ConstructParticle();
  
  G4IonConstructor pIConstructor;
  pIConstructor.ConstructParticle();
  
  G4ShortLivedConstructor pSConstructor;
  pSConstructor.ConstructParticle();
}

// Construct Processes //////////////////////////////////////////////////////
#include "G4RunManagerKernel.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"

#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"


#include "G4IonTable.hh"
#include "G4RadioactiveDecay.hh"

void MGProcessesList::ConstructProcess() 
{

  AddTransportation();
  
  // parallel worlds must be added after G4Transportation
  G4int nParaWorld = G4RunManagerKernel::GetRunManagerKernel()->GetNumberOfParallelWorld();
  if (nParaWorld > 0) {
    AddParallelWorldScoring();
  }
  if(fUseOpticalPhysicsOnly){
    MGLog(trace) << "Constucting optical processes"<<endlog;
    ConstructOp();
    MGLog(trace) << "Constucting cerenkov processes"<<endlog;
    ConstructCerenkov();
    return;
  }
  //EM Physics
  if (useLowE) 	    
    {
			switch (fuseLowEoption){
				//from https://geant4.web.cern.ch/node/1731
				case 1:
					fEMConstructor = new G4EmStandardPhysics_option1();
		      MGLog(trace) << "Using EmPhysics Option 1" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				case 2:
					fEMConstructor = new G4EmStandardPhysics_option2();
		      MGLog(trace) << "Using EmPhysics Option 2" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				case 3:
					fEMConstructor = new G4EmStandardPhysics_option3();
		      MGLog(trace) << "Using EmPhysics Option 3" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				case 4:
					fEMConstructor = new G4EmStandardPhysics_option4();
		      MGLog(trace) << "Using EmPhysics Option 4" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				case 5:
					fEMConstructor = new G4EmPenelopePhysics();
		      MGLog(trace) << "Using Penelope Physics" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				case 6:
					fEMConstructor = new G4EmLivermorePolarizedPhysics();
		      MGLog(trace) << "Using Livermore-Polarized Physics" << endlog;	  
		      MGLog(trace) << "see https://geant4.web.cern.ch/node/1731" << endlog;	  
					break;
				default:			
		      MGLog(trace) << "Using Livermore/LowEnergy electromagnetic physics" << endlog;	  
	  	    fEMConstructor = new G4EmLivermorePhysics();
					break;
			}
    }
  else
    {
      MGLog(trace) << "Using Standard electromagnetic physics" << endlog;
      fEMConstructor = new G4EmStandardPhysics();
    }
  fEMConstructor->ConstructProcess();
  
  //Includes synchrotron radiation, gamma-nuclear, muon-nuclear and 
  //e+/e- nuclear interactions
  G4EmExtraPhysics* EMExtraPhysics = new G4EmExtraPhysics();
  G4String choice = "on";
  EMExtraPhysics->Synch(choice);
  EMExtraPhysics->GammaNuclear(choice);
  EMExtraPhysics->MuonNuclear(choice);
  EMExtraPhysics->ConstructProcess();
 
  //TODO getting caught here when running?!?!?
  if (constructOptical) {
    MGLog(trace) << "Constucting optical processes"<<endlog;
    ConstructOp();
    MGLog(trace) << "Constucting cerenkov processes"<<endlog;
    ConstructCerenkov();
  }
  else 
    MGLog(routine) << "Processes for Optical Photons are inactivated" << endlog;

  MGLog(trace) << "finished optical contstruction physics"<<endlog;
  //Add decays
  G4VPhysicsConstructor* decayPhysics = new G4DecayPhysics(VerboseLevel);
  decayPhysics->ConstructProcess();
  G4VPhysicsConstructor* radDecayPhysics = new G4RadioactiveDecayPhysics(VerboseLevel);
  radDecayPhysics->ConstructProcess();
  MGLog(trace) << "finished decays processes construction"<<endlog;
  const G4IonTable *theIonTable = 
    G4ParticleTable::GetParticleTable()->GetIonTable();
  MGLog(trace) << "entries in ion table "<< theIonTable->Entries() <<endlog;
  //Assign manually triton decay
  for (G4int i=0; i<theIonTable->Entries(); i++) 
  {
    G4ParticleDefinition* particle = theIonTable->GetParticle(i);     
		// assign Tritium (3H) life time given by NuDat 2.5 - A. Schubert 21 July 2010:
		// follow http://hypernews.slac.stanford.edu/HyperNews/geant4/get/hadronprocess/1538/1.html

    if (particle == G4Triton::Definition())
    {
      MGLog(trace)<<" found trition particle "<< endlog;
      particle->SetPDGLifeTime(12.32*log(2.0)*365*24*3600*second);
      particle->SetPDGStable(false);
			G4ProcessManager* pmanager = particle->GetProcessManager();
			// Remove G4Decay process, which requires a registered decay table
			G4VProcess* decay = 0;
			G4ProcessVector* pvec = pmanager->GetAtRestProcessVector();
			for (G4int j=0; j<pvec->size() && decay==0; j++){
			 	if ((*pvec)[j]->GetProcessName() == "Decay") decay = (*pvec)[j];
			}
			if (decay) pmanager->RemoveProcess(decay);
 		  // Attach RDM, which is a rest-discrete process
      pmanager->SetVerboseLevel(VerboseLevel);
      pmanager->AddProcess(new G4RadioactiveDecay(),1000,-1,1000);
    }
  }
  MGLog(trace) << "finsihed triton decay processes construction"<<endlog;

  if(!fUseNoHadPhysFlag)  
  {
    // Hadron Elastic scattering
    MGLog(trace) << "constructing Elastic Physics "<<endlog;
    G4VPhysicsConstructor* hElasticPhysics = new G4HadronElasticPhysicsHP(VerboseLevel);
    hElasticPhysics->ConstructProcess();
    
    //Hadron Inelastic scattering
    G4VPhysicsConstructor* hPhysics = 0;
    if(fUseAllHPFlag) {
      hPhysics = new G4HadronPhysicsQGSP_BIC_AllHP(VerboseLevel);
      fPhysicsListHadrons = "QGSP_BIC_AllHP";
    }
    else if (fUseFTFFlag) {
      if (!fUseBertiniCascade) {
        MGLog(warning) << "You requested a FTFP physics list with binary cascade" << endlog;
        MGLog(warning) << "This is not available at the moment. Use FTFP with Bertini" << endlog;
      }
      hPhysics = new G4HadronPhysicsFTFP_BERT_HP(VerboseLevel);
      fPhysicsListHadrons = "FTFP_BERT_HP";
    }
    else {
      if (fUseBertiniCascade) {
        hPhysics = new G4HadronPhysicsQGSP_BERT_HP(VerboseLevel);
        fPhysicsListHadrons = "QGSP_BERT_HP";
      }
      else {
        hPhysics = new G4HadronPhysicsQGSP_BIC_HP(VerboseLevel);
        fPhysicsListHadrons = "QGSP_BIC_HP";
      }
    }
    MGLog(trace) << "hadron processes construction"<<endlog;
    hPhysics->ConstructProcess();
    
    MGLog(trace) << "stopping processes construction"<<endlog;
    G4VPhysicsConstructor* stoppingPhysics = new G4StoppingPhysics();
    stoppingPhysics->ConstructProcess();

    MGLog(trace) << "ion processes construction"<<endlog;
    G4VPhysicsConstructor* ionPhysics = new G4IonPhysics();
    ionPhysics->ConstructProcess();
  }
  MGLog(trace) << "finsihed hadron processes construction"<<endlog;
  // Turn on angular correlations in gamma decays if requested (requires G4.10.4
  // or later; will just emit a warning that these commands don't exist if a
  // user tries to turn this feature on in previous G4 versions)
  if(fAngCorrMaxTwoJ > 0) {
    G4UImanager::GetUIpointer()->ApplyCommand("/process/deex/correlatedGamma true");
    ostringstream os;
    os << "/process/deex/maxTwoJ " << fAngCorrMaxTwoJ;
    G4UImanager::GetUIpointer()->ApplyCommand(os.str().c_str());
  }
  
  DumpPhysicsList();
}

// Transportation ///////////////////////////////////////////////////////////
#include "processes/MGProcessesMaxTimeCuts.hh"
#include "processes/MGProcessesMinEkineCuts.hh"
void MGProcessesList::AddTransportation() 
{
	
  G4VUserPhysicsList::AddTransportation();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    // step limits
    if(fLimitSteps[particleName]) {
      pmanager->AddProcess(new G4StepLimiter, -1, -1, 3);
      MGLog(trace) << "Steps will be limited for " << particleName << endlog;
    }
  }
}

// Parallel World Scoring ////////////////////////////////////////////////////
#include "G4ParallelWorldScoringProcess.hh"

void MGProcessesList::AddParallelWorldScoring() 
{
  G4ParallelWorldScoringProcess* theParallelWorldScoringProcess 
    = new G4ParallelWorldScoringProcess("ParallelWorldScoringProc");
  theParallelWorldScoringProcess->SetParallelWorld("ParallelSamplingWorld");
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      if (!particle->IsShortLived())
	{
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  pmanager->AddProcess(theParallelWorldScoringProcess);
	  pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxAtRest);
	  pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxAlongStep, 1);
	  pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxPostStep);
	}
    }
}



// Optical Processes ////////////////////////////////////////////////////////
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpRayleigh.hh"
#include "G4OpWLS.hh"
void MGProcessesList::ConstructOp() {
	
 	// default scintillation process
	G4Scintillation* theScintProcessDef = new G4Scintillation("Scintillation");
	theScintProcessDef->SetTrackSecondariesFirst(true);
	theScintProcessDef->SetScintillationYieldFactor(1.0);
	theScintProcessDef->SetScintillationExcitationRatio(0.0);
	theScintProcessDef->SetVerboseLevel(OpVerbLevel);
	
	// scintillation process for alpha:
	G4Scintillation* theScintProcessAlpha = new G4Scintillation("Scintillation");
	theScintProcessAlpha->SetTrackSecondariesFirst(true);
	theScintProcessAlpha->SetScintillationYieldFactor(1.1);
	theScintProcessAlpha->SetScintillationExcitationRatio(1.0);
	theScintProcessAlpha->SetVerboseLevel(OpVerbLevel);
	
	// scintillation process for heavy nuclei
	G4Scintillation* theScintProcessNuc = new G4Scintillation("Scintillation");
	theScintProcessNuc->SetTrackSecondariesFirst(true);
	theScintProcessNuc->SetScintillationYieldFactor(0.2);
	theScintProcessNuc->SetScintillationExcitationRatio(1.0);
	theScintProcessNuc->SetVerboseLevel(OpVerbLevel);
	
	// optical processes
	G4OpAbsorption* theAbsorptionProcess = new G4OpAbsorption();
	G4OpBoundaryProcess* theBoundaryProcess = new G4OpBoundaryProcess();
	G4OpRayleigh* theRayleighScatteringProcess = new G4OpRayleigh();
	G4OpWLS* theWLSProcess = new G4OpWLS();
	
  theAbsorptionProcess->SetVerboseLevel(OpVerbLevel);
	theBoundaryProcess->SetVerboseLevel(OpVerbLevel);
	theWLSProcess->SetVerboseLevel(OpVerbLevel);
       
#if G4VERSION_NUMBER < 960
	// Old code (geant4 9.4/9.5) - The surface model must now be specified on a surface basis.
	// Cannot be specified in the physics list.
	G4OpticalSurfaceModel themodel = unified;
	theBoundaryProcess->SetModel(themodel);
#endif
	
	theParticleIterator->reset();
	while( (*theParticleIterator)() ) {
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
	 	if (theScintProcessDef->IsApplicable(*particle)) {
			if(particle->GetParticleName() == "GenericIon") {
				pmanager->AddProcess(theScintProcessNuc); // AtRestDiscrete
				pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxPostStep);
			} else if(particle->GetParticleName() == "alpha") {
				pmanager->AddProcess(theScintProcessAlpha);
				pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxPostStep);
			} else {
				pmanager->AddProcess(theScintProcessDef);
				pmanager->SetProcessOrderingToLast(theScintProcessDef,idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessDef,idxPostStep);
			}	  
		}
      
		if (particleName == "opticalphoton") {
			pmanager->AddDiscreteProcess(theAbsorptionProcess);
			pmanager->AddDiscreteProcess(theBoundaryProcess);
			pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
			pmanager->AddDiscreteProcess(theWLSProcess);
		}
	}
}


// Hadronic processes ////////////////////////////////////////////////////////



//---------------------------------------------------------------------------//

void MGProcessesList::SetCuts()
{
  //special for low energy physics
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV,100.*GeV);

  SetCutValue( cutForGamma, "gamma" );
  SetCutValue( cutForElectron, "e-" );
  SetCutValue( cutForPositron, "e+" );
  SetCutValue( cutForProton, "proton" );
  SetCutValue( cutForAlpha, "alpha" );
  SetCutValue( cutForGenericIon, "GenericIon" );
  
  if (G4RegionStore::GetInstance())
    {
      if (G4RegionStore::GetInstance()->size() > 1) 
	{
	  //Set different cuts for the sensitive region
	  G4Region* region = G4RegionStore::GetInstance()->GetRegion("SensitiveRegion");  
	  if (region) 
	    {
	      MGLog(trace) << "Register cuts for SensitiveRegion " << endlog;
	      G4ProductionCuts* cuts = region->GetProductionCuts(); 
	      if (!cuts) //zero pointer --> cuts not defined yet
		cuts = new G4ProductionCuts;
	      cuts->SetProductionCut(cutForGammaSensitive,"gamma");
	      cuts->SetProductionCut(cutForElectronSensitive,"e-");
	      cuts->SetProductionCut(cutForPositronSensitive,"e+");
	      cuts->SetProductionCut(defaultCutValue,"proton");
	      cuts->SetProductionCut(defaultCutValue,"alpha");
	      cuts->SetProductionCut(defaultCutValue,"GenericIon");
	      region->SetProductionCuts(cuts);
	    }
	}
    }
  
  G4int nParticles = 5;
  MGLog(trace) << "Production cuts set" << endlog;
  DumpCutValuesTable( nParticles );
}

//---------------------------------------------------------------------------//
void MGProcessesList::SetRealm( G4String realm )
{
	if( realm == "BBdecay" ) 
	  {
	    //  Changed by R. Henning, 2/16/2005
            // 
	    MGLog(routine) << "Realm set to BBdecay" << endlog;
	    cutForGamma = 0.1*mm;
	    cutForElectron = 0.1*mm;
	    cutForPositron = 0.1*mm;
	    cutForProton = defaultCutValue;
	    cutForAlpha = defaultCutValue;
	    cutForGenericIon = defaultCutValue;
	    cutForGammaSensitive = cutForGamma;
	    cutForElectronSensitive = cutForElectron;
	    cutForPositronSensitive = cutForPositron;
	    SetCuts();
	  } else if( realm == "DarkMatter" ) 
	    {
	      MGLog(routine) << "Realm set to DarkMatter" << endlog;
	      //  These values are tuned to ~1 keV for gamma, e+, e- in
	      //  natural germanium.
	      cutForGamma = 0.005*mm;
	      cutForElectron = 0.0005*mm;
	      cutForPositron = 0.0005*mm;
	      cutForProton = defaultCutValue;
	      cutForAlpha = defaultCutValue;
	      cutForGenericIon = defaultCutValue;
	      cutForGammaSensitive = cutForGamma;
	      cutForElectronSensitive = cutForElectron;
	      cutForPositronSensitive = cutForPositron;
	      SetCuts();
	    } else if( realm == "CosmicRays" ) 
	      {
		//
		MGLog(routine) << "Realm set to CosmicRays (cut-per-region)" << endlog;
		cutForGamma = 5*cm;
		cutForElectron = 1*cm;
		cutForPositron = 1*cm;
		cutForProton = 5*mm;
		cutForAlpha = 5*mm;
		cutForGenericIon = 5*mm;
		cutForGammaSensitive = 30*mm;
		cutForElectronSensitive = 0.04*mm;
		cutForPositronSensitive = 0.04*mm;
		SetCuts();	
	      } else
		MGLog( error ) << "Error: invalid energy cut realm \"" << realm
			       << "\"." << G4endl
			       << "Must use either \"BBdecay\" or \"DarkMatter\"." << G4endl;
}

//---------------------------------------------------------------------------//
#include "G4Cerenkov.hh"

void MGProcessesList::ConstructCerenkov()
{
  G4Cerenkov *theCerenkovProcess = new G4Cerenkov();
  G4ProcessManager * pManager = 0;

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    pManager = particle->GetProcessManager();
    if(theCerenkovProcess->IsApplicable(*particle)){
      pManager->AddProcess(theCerenkovProcess);
      pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);

    }
  }
}


//---------------------------------------------------------------------------//

void MGProcessesList::DumpPhysicsList()
{
  G4cout << "====================================================================" << G4endl;
  G4cout << "                      MaGe physics list                             " << G4endl;
  G4cout << "====================================================================" << G4endl;
  if (useLowE)
    G4cout << "Electromagnetic physics: Livermore/LowEnergy " << G4endl;
  else
    G4cout << "Electromagnetic physics: Standard " << G4endl;
  G4cout << "====================================================================" << G4endl;

  if (constructOptical)
    G4cout << "Physics for optical photons registered" << G4endl;
  else
    G4cout << "No processes activated for optical photons" << G4endl;
  G4cout << "====================================================================" << G4endl;

  if (fUseNoHadPhysFlag)
    {
      G4cout << "No processes activated for hadrons" << G4endl;
      G4cout << "====================================================================" << G4endl;
      return;
    }
  G4cout << fPhysicsListHadrons << G4endl;
  G4cout << "====================================================================" << G4endl;
  return;
}

//---------------------------------------------------------------------------//

void MGProcessesList::GetStepLimits(){
	DumpPhysicsList();
  G4cout << "========================Show limits ================================" << G4endl;
  G4cout << " gamma " << GetCutValue("gamma") << "keV" << G4endl;
  G4cout << " e-    " << GetCutValue("e-") << "keV" << G4endl;
  G4cout << " e+    " << GetCutValue("e+") << "keV" << G4endl;
  G4cout << " p     " << GetCutValue("proton") << "keV" << G4endl;
  G4cout << " alpha " << GetCutValue("alpha") <<"keV" << G4endl;
  G4cout << " Ion   " << GetCutValue("GenericIon") << "keV" << G4endl;
  G4cout << "====================================================================" << G4endl;

}
