/*
 * SingleDecay.h
 *
 *  Created on: Mar 30, 2011
 *      Author: fraenkle
 */

#ifndef SINGLEDECAY_H_
#define SINGLEDECAY_H_

#include <vector>
using namespace std;
//! Single Decay
/*!
 * This class contains methods to determine the kind of a radioactive decay from the emitted particles
 */
class SingleDecay {
public:
	SingleDecay();
	virtual ~SingleDecay();
	//! Add particle
	/*!
	 * Adds a particle of given type and energy
	 * \param id particle id number (-2 = anti-neutrino, -1 = positron, 1 = electron, 2 = neutrino, 3 = gamma, 4 = alpha, 5 = nucleus)
	 * \param energy energy of the particle in
	 */
	void AddParticle(int id, double energy, double time);
	//! Get decay type
	/*!
	 * Determines the type of radioactive decay from the emitted particles
	 * \return decay type (1 = beta-minus, 2 = beta-plus, 3 = electron capture, 4 = alpha)
	 */
	int GetDecayType();
	//! Get decay energy
	/*!
	 * Calculates and returns the decay energy of the given decay type
	 * \param type type of radioactive decay (1 = beta-minus, 2 = beta-plus, 3 = electron capture, 4 = alpha)
	 * \return decay energy in keV
	 */
	double GetDecayEnergy(int type);
	/*Gets Decay Time
	
	double GetDecayTime(int type);*/
	//! Report
	/*!
	 * Prints the number of particles of each type and the energy of each particle
	 */
	double GetDecayTime(int type);
	void Report();
	


	//! internal vector to store the energies of emitted electrons
	std::vector<double> electron;
	//! internal vector to store the energies of emitted positrons
	std::vector<double> positron;
	//! internal vector to store the energies of emitted electronneutrinos
	std::vector<double> electronneutrino;
	//! internal vector to store the energies of emitted electronantineutrinos
	std::vector<double> electronantineutrino;
	//! internal vector to store the energies of emitted gammas
	std::vector<double> gamma;
	//! internal vector to store the energies of emitted alpha particles
	std::vector<double> alpha;
	//! internal vector to store the energies of recoil nuclei
	std::vector<double>	nucleus;
	//Internal vectors to store the times that particles are created
	std::vector<double> electrontime;
	std::vector<double> alphatime;
	std::vector<double> electronneutrinotime;
	std::vector<double> positrontime;

};

#endif /* SINGLEDECAY_H_ */
