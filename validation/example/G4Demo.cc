#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4DemoDetector.hh"
#include "G4DemoPhysics.hh"
#include "G4DemoEventGeneratorAction.hh"
#include "G4DemoRun.hh"
#include "G4DemoRunAction.hh"

int main(int argc, char** argv) {

	// Construct the default run manager
	G4RunManager* runManager = new G4RunManager;

	// set mandatory initialization classes
	G4VUserDetectorConstruction* detector = new G4DemoDetector;
	runManager->SetUserInitialization(detector);
	G4VUserPhysicsList* physics = new G4DemoPhysics;
	runManager->SetUserInitialization(physics);

	// set mandatory user action class
	G4VUserPrimaryGeneratorAction* eventgenerator = new G4DemoEventGeneratorAction;
	runManager->SetUserAction(eventgenerator);


	// add user defined action (get energy deposit)
	G4UserRunAction* runaction = new G4DemoRunAction;
	runManager->SetUserAction(runaction);

	// Initialize G4 kernel
	runManager->Initialize();

	// Get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();
	UI->ApplyCommand("/run/verbose 1");
	UI->ApplyCommand("/event/verbose 0");
	UI->ApplyCommand("/tracking/verbose 0");

	runManager->BeamOn(100000);

	delete runManager;

	return 0;
}
