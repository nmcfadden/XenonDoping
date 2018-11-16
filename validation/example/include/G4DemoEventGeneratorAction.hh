/*
 * G4DemoEventGeneratorAction.hh
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#ifndef G4DEMOEVENTGENERATORACTION_HH_
#define G4DEMOEVENTGENERATORACTION_HH_

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class G4DemoEventGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	G4DemoEventGeneratorAction();
	~G4DemoEventGeneratorAction();
	void GeneratePrimaries(G4Event* anEvent);
private:
	G4ParticleGun* particleGun;
};

#endif /* G4DEMOEVENTGENERATORACTION_HH_ */
