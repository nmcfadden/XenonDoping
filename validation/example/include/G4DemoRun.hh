/*
 * G4DemoRun.hh
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#ifndef G4DEMORUN_HH_
#define G4DEMORUN_HH_

#include "G4Event.hh"
#include "globals.hh"
#include "G4Run.hh"
#include "G4THitsMap.hh"

class G4DemoRun : public G4Run{
public:
	G4DemoRun();
	~G4DemoRun();
	void RecordEvent(const G4Event* evt);
	G4double GetEnergyDeposit();
	G4double allEnergyDeposit[100000];
private:
	G4int energyDepositID;
	G4int nEvent;
	G4THitsMap<G4double> totalEnergyDeposit;
	G4THitsMap<G4double>* eventEnergyDeposit;
};

#endif /* G4DEMORUN_HH_ */
