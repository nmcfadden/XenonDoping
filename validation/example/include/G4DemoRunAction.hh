/*
 * G4DemoRunAction.hh
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#ifndef G4DEMORUNACTION_HH_
#define G4DEMORUNACTION_HH_

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class G4DemoRunAction : public G4UserRunAction {
public:
	G4DemoRunAction();
	~G4DemoRunAction();
    G4Run* GenerateRun();
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run* aRun);
};

#endif /* G4DEMORUNACTION_HH_ */
