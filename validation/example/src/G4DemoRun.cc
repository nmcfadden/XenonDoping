/*
 * G4DemoRun.cc
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#include "../include/G4DemoRun.hh"
#include "G4SDManager.hh"

G4DemoRun::G4DemoRun() : nEvent(0) {

	G4SDManager* SDM = G4SDManager::GetSDMpointer();
	energyDepositID = SDM->GetCollectionID("mfdet/eDep");

}

G4DemoRun::~G4DemoRun() {
	// TODO Auto-generated destructor stub
}

void G4DemoRun::RecordEvent(const G4Event* evt) {

	nEvent++;
	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	eventEnergyDeposit = (G4THitsMap<G4double>*) (HCE->GetHC(energyDepositID));
	totalEnergyDeposit += *eventEnergyDeposit;

	std::map<G4int,G4double*>::iterator itr = eventEnergyDeposit->GetMap()->begin();
	G4double val = *(itr->second);
	allEnergyDeposit[nEvent-1] = val;
}

G4double G4DemoRun::GetEnergyDeposit(){
	G4double val;
	val = 0.0;
	return val;
}
