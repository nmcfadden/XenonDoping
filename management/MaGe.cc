//---------------------------------------------------------------------------//
//Bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MaGe.cc,v 1.20 2009-08-31 09:14:40 jliu Exp $
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * main() For MaGe simulation program. Handles command line interface and
 * Geant 4 initialization.
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Reyco Henning
 * CONTACT: rhenning@lbl.gov
 * FIRST SUBMISSION: Wed Mar 17 13:50:47 PST 2004
 *
 * REVISION:
 *
 * 3-17-2004:  Created. R. Henning
 * 4-16-2004:  Added G4Manager and other G4 mandatory actions. R. Henning
 * 5-16-2004:  Added MJManagementVisualization. R. Henning
 * 7-06-2004:  Added MJManager, R. Henning
 * 11-08-2004: Added TrackingAction, Xiang
 * 12-07-2004, Modifed for Gerda Integration, R. Henning
 * 12-15-2004: Added Stacking Action, L. Pandola
 * 03-07-2007, Changed TrackingAction, so that GETrajectory is executed only when required,
 *             L. Pandola
 * 20-11-2017, Allow for up to 100 macro commands to be saved in G4UImanager::histVec K. v. Sturm 
 */
//---------------------------------------------------------------------------//
//

#include <string.h>

//#include "CLHEP/Random/Random.h"

#include "globals.hh"
#include "G4PhysListFactory.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4VisManager.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "G4CLHEP"
#endif

#include "generators/MGGeneratorPrimary.hh"
#include "management/MGManagementEventAction.hh"
#include "management/MGManagementRunAction.hh"
#include "management/MGManagementTrackingAction.hh"
#include "management/MGManagementSteppingAction.hh"
#include "management/MGManagementStackingAction.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "management/MGManager.hh"
#include "processes/MGProcessesList.hh"
#include "io/MGLogger.hh"
#include "MGDOUtils.hh"

// Functions called from main().
void PrintHeader();
void PrintUsage(void);

using namespace std;

//---------------------------------------------------------------------------//

int main(int argc, char** argv)
{
  PrintHeader();

  MGLog(trace) << "Creating G4 and MJ Runmanagers." << endlog;
  G4RunManager *runManager = new G4RunManager;
  MGManager    *mjMgmt = new MGManager;

  // Set mandatory Geant4 action classes:
  // ------------------------------------

  // Register detector geometry and materials.
  MGLog(trace) << "Creating and registering G4 geometry." << endlog;
  MGManagerDetectorConstruction *Simulateddetector =
    new MGManagerDetectorConstruction;
  runManager->SetUserInitialization(Simulateddetector);
  mjMgmt->SetUserInitialization(Simulateddetector);
	
  // Register Geant4 physics processes
  MGLog(trace) << "Registering G4 physics processes." << endlog;
  // Search command line for -p [physlistname]
  G4VUserPhysicsList* processes = NULL;
  for(int i=1; i<argc; i++) {
    if(string(argv[i]) == "-p") {
      if(i+1 == argc) {
        MGLog(error) << "must supply a physics list name following -p" << endlog;
        PrintUsage();
        return 1;
      }
      MGLog(trace) << "Using G4 physics list " << argv[i+1] << endlog;
      processes = (new G4PhysListFactory)->GetReferencePhysList(argv[i+1]);
      if(processes == NULL) return 1;
      for(int j=i+2; j<argc; j++) argv[j-2] = argv[j];
      argc -= 2;
      break;
    }
  }
	
  if(processes == NULL) processes = new MGProcessesList;
  runManager->SetUserInitialization(processes);
  mjMgmt->SetUserInitialization(processes);

  // Create and initialize visualization manager.
  MGLog(trace) << "Creating and initializing visualization manager." << endlog;
  G4VisManager* visManager = new G4VisExecutive();
  mjMgmt->SetUserInitialization(visManager);
  visManager->Initialize();

  // Register event generator.
  MGLog(trace) << "Registering G4 event generator." << endlog;
  MGGeneratorPrimary *generator = new MGGeneratorPrimary;
  runManager->SetUserAction(generator);
  mjMgmt->SetUserInitialization(generator);

  // Register run action. What to do at beginning and end of each run.
  MGLog(trace) << "Registering G4 run action." << endlog;
  MGManagementRunAction *runAction = new MGManagementRunAction;
  runManager->SetUserAction(runAction);
  mjMgmt->SetUserInitialization(runAction);

  // Register event action, ie. what to save/compute for each event.
  MGLog(trace) << "Registering G4 event action." << endlog;
  MGManagementEventAction *eventAction = new MGManagementEventAction;
  runManager->SetUserAction(eventAction);
  mjMgmt->SetUserInitialization(eventAction);

  //Register tracking action
  MGLog(trace) << "Registering G4 tracking action." << endlog;
  MGManagementTrackingAction *trackAction =
    new MGManagementTrackingAction(eventAction);
  runManager->SetUserAction(trackAction);
  mjMgmt->SetUserInitialization(trackAction);

  // Register stepping action, ie. what to save/compute for each step.
  MGLog(trace) << "Registering G4 stepping action." << endlog;
  MGManagementSteppingAction *stepAction =
    new MGManagementSteppingAction(eventAction);
  runManager->SetUserAction(stepAction);
  mjMgmt->SetUserInitialization(stepAction);

  // Register stacking action, ie. what to do for each new stacked particle.
  MGLog(trace) << "Registering G4 stacking action." << endlog;
  MGManagementStackingAction *stackAction =
    new MGManagementStackingAction(eventAction);
  runManager->SetUserAction(stackAction);
  mjMgmt->SetUserInitialization(stackAction);


  // Handle command line arguments and select run mode.
  // --------------------------------------------------

  bool abortAtExit = false;
  if(argc == 1){
    MGLog(routine) << "Entering interactive mode." << endlog;
    G4UIsession *session = new G4UIterminal(new G4UItcsh);
    session->SessionStart();
    delete session;
   }
	 else{
    if (argc > 2) {
      if(argc > 3 || strcmp(argv[2], "abort") != 0) {
        MGLog(error) << "Error processing command line. Too many arguments.\n";
        PrintUsage();
				exit(1);
      }
      abortAtExit = true;
    }
    if(strcmp(argv[1], "-h")) {
      G4cout << "Entering batch mode...\n";
      G4cout << "Executing script file from command line: " << argv[1] << '\n';
      G4UImanager* UI = G4UImanager::GetUIpointer();
      UI -> SetMaxHistSize( 100 );
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command + fileName);
     }else {
      PrintUsage();
    }
  }

	delete visManager;
  delete mjMgmt;
  delete runManager;

  if(abortAtExit) {
    MGLog(routine) << "Aborting at end of main() by user request..." << endlog;
    abort();
  }  

  return 0;
}

//---------------------------------------------------------------------------//

void PrintHeader(void)
{
  G4cout << "Majorana/Gerda Monte Carlo Simulation" << G4endl;
  G4cout << "------------------------------------------" << G4endl;
  G4cout << "MaGe Revision: " << MGLogger::GetMaGeRevision() << G4endl;
  G4cout << "MaGe SVN Tag: " << MGLogger::GetMaGeTag() << G4endl;
  G4cout << "MaGe Last Updated: " << MGLogger::GetMaGeDate() << G4endl;
  G4cout << "MGDO Revision: " << MGDOUtils::GetMGDORevision() << G4endl;
  G4cout << "MGDO SVN Tag: " << MGDOUtils::GetMGDOTag() << G4endl;
  G4cout << "MGDO Last Updated: " << MGDOUtils::GetMGDODate() << G4endl;
  G4cout << "CLHEP Version: " << PACKAGE_VERSION << G4endl;
}

//---------------------------------------------------------------------------//

void PrintUsage(void)
{
  G4cout << "Usage:" << G4endl;
  G4cout << "MaGe -h : Displays this message" << G4endl;
  G4cout << "MaGe <filename> : Executes script <filename>" << G4endl;
  G4cout << "MaGe : Executes MJ interactively" << G4endl;
  G4cout << "Optional argument: -p [G4PhysicsListName] : use G4 internal physics list of the corresponding name" << G4endl << G4endl;
}


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
