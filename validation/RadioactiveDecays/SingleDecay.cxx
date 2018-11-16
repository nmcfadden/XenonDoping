/*
 * SingleDecay.cxx
 *
 *  Created on: Mar 30, 2011
 *      Author: fraenkle
 */

#include "SingleDecay.h"
#include <iostream>
#include <vector>
using namespace std;

SingleDecay::SingleDecay() {
	// TODO Auto-generated constructor stub

}

void SingleDecay::AddParticle(int id, double energy, double time){

	switch(id){
	case -2:
		electronantineutrino.push_back(energy);

		break;
	case -1:
		positron.push_back(energy);
		positrontime.push_back(time);
		break;
	case 1:
		electron.push_back(energy);
		electrontime.push_back(time);

		break;
	case 2:
		electronneutrino.push_back(energy);
		electronneutrinotime.push_back(time);
		break;
	case 3:
		gamma.push_back(energy);

		break;
	case 4:
		alpha.push_back(energy);
		alphatime.push_back(time);
		break;
	case 5:
		nucleus.push_back(energy);

		break;
	}
}

int SingleDecay::GetDecayType(){

	if(electronneutrino.size() == 0 && electronantineutrino.size() > 0)
		return 1;	// beta-minus decay
	if(positron.size() > 0 && electronneutrino.size() > 0)
		return 2;	// beta-plus decay
	if(positron.size() == 0 && electronneutrino.size() > 0)
		return 3;	// electron capture
	if(alpha.size() > 0)
		return 4;	// alpha decay

	return 0;
}

double SingleDecay::GetDecayEnergy(int type){

	double decayenergy = 0;

	switch(type){
	case 1:
		decayenergy = electron[0] + electronantineutrino[0];
		break;
	case 2:
		decayenergy = positron[0] + electronneutrino[0];
		break;
	case 3:
		decayenergy = electronneutrino[0];
		break;
	case 4:
		decayenergy = alpha[0];
		break;
	}

	return decayenergy;
}

double SingleDecay::GetDecayTime(int type){
	double decaytime;
	decaytime=5000;
	double mintime;
	
	switch(type){
//Beta minus decay: returns earliest electron (the primary one)
	case 1:
		mintime=electrontime[0];
		for(int i=0;i<electrontime.size();i++){
			if(electrontime[i]<mintime){ mintime=electrontime[i];}
		}
		decaytime=mintime;

		break;
//Beta plus decay: returns earliest positron
	case 2:
		mintime=positrontime[0];
		for(int i=0;i<positrontime.size();i++){
			if(positrontime[i]<mintime){ mintime=positrontime[i];}
		}
		decaytime=mintime;

		break;
//Electron capture: returns earliest electron neutrino
	case 3:
		mintime=electronneutrinotime[0];
		for(int i=0;i<electronneutrinotime.size();i++){
			if(electronneutrinotime[i]<mintime){ mintime=electronneutrinotime[i];}
		}
		decaytime=mintime;

		break;
//Alpha decay: returns earliest alpha particle
	case 4:
		mintime=alphatime[0];
		for(int i=0;i<alphatime.size();i++){
			if(alphatime[i]<mintime){ mintime=alphatime[i];}
		}
		decaytime=mintime;

		break;
	}
	return decaytime;
}

void SingleDecay::Report(){

	std::cout << "\n==========================================\n";
	std::cout << "electrons:\t" << electron.size() << "\n";
	for(int i=0;i<electron.size();i++)
		std::cout << electron[i] << " keV\n";
	std::cout << "positrons:\t" << positron.size() << "\n";
	for(int i=0;i<positron.size();i++)
		std::cout << positron[i] << " keV\n";
	std::cout << "neutrinos:\t" << electronneutrino.size() << "\n";
	for(int i=0;i<electronneutrino.size();i++)
		std::cout << electronneutrino[i] << " keV\n";
	std::cout << "anti-neutrinos:\t" << electronantineutrino.size() << "\n";
	for(int i=0;i<electronantineutrino.size();i++)
		std::cout << electronantineutrino[i] << " keV\n";
	std::cout << "gammas:\t" << gamma.size() << "\n";
	for(int i=0;i<gamma.size();i++)
		std::cout << gamma[i] << " keV\n";
	std::cout << "alphas:\t" << alpha.size() << "\n";
	for(int i=0;i<alpha.size();i++)
		std::cout << alpha[i] << " keV\n";
	std::cout << "nuclei:\t" << nucleus.size() << "\n";
	for(int i=0;i<nucleus.size();i++)
		std::cout << nucleus[i] << " keV\n";

}

SingleDecay::~SingleDecay() {
	// TODO Auto-generated destructor stub
}
