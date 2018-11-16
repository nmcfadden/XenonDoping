/*
 * G4DemoPhysics.hh
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#ifndef G4DEMOPHYSICS_HH_
#define G4DEMOPHYSICS_HH_

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4DemoPhysics : public G4VUserPhysicsList{
public:
	G4DemoPhysics();
	~G4DemoPhysics();
protected:
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();
	void ConstructBosons();
	void ConstructLeptons();
	void ConstructMesons();
	void ConstructBaryons();
	void ConstructGeneral();
	void ConstructEM();
	void AddStepMax();
};

#endif /* G4DEMOPHYSICS_HH_ */
