/*
 * G4DemoRunAction.cc
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#include "../include/G4DemoRunAction.hh"
#include "G4DemoRun.hh"
#include <fstream>
#include <iostream>

G4DemoRunAction::G4DemoRunAction() {
	// TODO Auto-generated constructor stub

}

G4DemoRunAction::~G4DemoRunAction() {
	// TODO Auto-generated destructor stub
}

G4Run* G4DemoRunAction::GenerateRun() {
	return (new G4DemoRun);
}

void G4DemoRunAction::EndOfRunAction(const G4Run* aRun){
	G4DemoRun* theRun = (G4DemoRun*)aRun;
	std::ofstream outputfile;
	//ifstream inputfile;

	outputfile.open("G4Demo.txt");

	for(int i=0;i<100000;i++){
		//G4cout << "FLOPUT: EndOfRunAction\t" << theRun->allEnergyDeposit[i] << G4endl;
		outputfile << theRun->allEnergyDeposit[i] << G4endl;
	}
	outputfile.close();
}

void G4DemoRunAction::BeginOfRunAction(const G4Run*){
	//G4DemoRun* theRun = (G4DemoRun*)aRun;
	//G4cout << "FLOPUT: It has started..." << G4endl;
}
