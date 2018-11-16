/*
 * RadioactiveDecay.h
 *
 *  Created on: Feb 7, 2011
 *      Author: fraenkle
 */

#include <string>
#include <TH1F.h>

#ifndef RADIOACTIVEDECAY_H_
#define RADIOACTIVEDECAY_H_

//! Radioactive Decay
/*!
 * This class contains methods to analyse radioactive decays
 */
class RadioactiveDecay {
public:
	//! Constructor
	/*!
	 * Constructs a RadioactiveDecay object for a specific type of decay and decay energy
	 * \param type type of decay (0 = gamma, 1 = beta, 2 = alpha, 3 = secondary electron)
	 * \param name name of the isotope, for example Bi214, Co60,...
	 * \param energy literature decay energy in keV
	 * \param energysigma uncertainty on literature decay energy in keV
	 * \param intensity intensity (or branching ratio) of the decay channel in %
	 * \param intensitysigma uncertainty on intensity in %
	 * \param token name of the associated token
	 */
	RadioactiveDecay(int type, std::string name, double energy, double energysigma, double intensity, double intensitysigma, std::string token);
	virtual ~RadioactiveDecay();
	//! Get decay type
	/*!
	 * Returns the type of the decay as an integer value (0 = gamma, 1 = beta, 2 = alpha, 3 = secondary electron)
	 * \return decay type
	 */
	int GetDecayType();
	//! Get isotope name
	/*!
	 * Returns the name of the isotope specified for this decay
	 * \return name of isotope as string
	 */
	std::string GetIsotopeName();
	//! Get token
	/*!
	 * Returns the token associated with this decay
	 * \return token name as string
	 */
	std::string GetToken();
	//! Add decay
	/*!
	 * Adds an individual decay of given energy to the internal histogram
	 * \param energy energy of the decay in keV
	 */
	void AddDecay(double energy);
	//! Draw histogram
	/*!
	 * Draws the internal histogram within +/- 1% of the literature decay value
	 */
	void DrawHistogram();
	//! Get number of decays
	/*!
	 * Returns the number of decays for this radioactive decay
	 * \return number of decays
	 */
	int GetNumberOfDecays();
	//! Get total number of decays
	/*!
	 * Returns the number of all decays of the isotope
	 * \return number of all decays
	 */
	int GetNumberOfTotalDecays();
	//! Set total number of decays
	/*!
	 * Sets the number of all decays of the isotope
	 * \param number number of all decays
	 */
	void SetNumberOfTotalDecays(int number);
	//! Get literature decay energy
	/*!
	 * Returns the literature decay energy
	 * \return literature decay energy in keV
	 */
	double GetLiteratureDecayEnergy();
	//! Get literature decay energy uncertainty
	/*!
	 * Returns the literature decay energy uncertainty
	 * \return literature decay energy uncertainty in keV
	 */
	double GetLiteratureDecayEnergySigma();
	//! Get literature decay energy token
	/*!
	 * Returns a token for the literature decay energy and the actual value
	 * \return literature decay energy token as string
	 */
	std::string GetLiteratureDecayEnergyToken();
	//! Get literature decay intensity
	/*!
	 * Returns the literature value for the decay intensity (branching ratio)
	 * \return literature decay intensity in %
	 */
	double GetLiteratureDecayIntensity();
	//! Get literature decay intensity uncertainty
	/*!
	 * Returns the literature uncertainty for the decay intensity (branching ratio)
	 * \return literature decay uncertainty in %
	 */
	double GetLiteratureDecayIntensitySigma();
	//! Get literature decay intensity token
	/*!
	 * Returns a token for the literature decay intensity and the actual value
	 * \return literature decay intensity token as string
	 */
	std::string GetLiteratureDecayIntensityToken();
	//! Get simulated decay energy
	/*!
	 * Returns the simulated energy of the decay
	 * \return simulated energy in keV
	 */
	double GetSimulatedDecayEnergy();
	//! Get simulated decay energy uncertainty
	/*!
	 * Not implemented so far, there is no uncertainty of a simulation parameter
	 * \return 0.0 keV
	 */
	double GetSimulatedDecayEnergySigma();
	//! Get simulated decay energy token
	/*!
	 * Returns a token for the simulated decay energy and the actual value
	 * \return simulated decay energy token as string
	 */
	std::string GetSimulatedDecayEnergyToken();
	//! Get simulated decay intensity
	/*!
	 * Returns the simulated decay intensity (branching ratio)
	 * \return simulated decay intensity in %
	 */
	double GetSimulatedDecayIntensity();
	//! Get simulated decay intensity uncertainty
	/*!
	 * Returns the simulated uncertainty for the decay intensity (branching ratio), for small event numbers (<10) a binomial estimator is used
	 * \return simulated uncertainty for decay intensity in %
	 */
	double GetSimulatedDecayIntensitySigma();
	//! Get simulated decay intensity token
	/*!
	 * Returns a token for the simulated decay intensity and the actual value
	 * \return simulated decay intensity token as string
	 */
	std::string GetSimulatedDecayIntensityToken();
	//! Get energy difference token
	/*!
	 * Returns a token for the difference between literature value and simulated value and the actual difference
	 * \return energy difference token as string
	 */
	std::string GetEnergyDifferenceToken();
	//! Get intensity difference token
	/*!
	 * Returns a token for the difference between literature value and simulated value and the actual difference
	 * \return intensity difference token as string
	 */
	std::string GetIntensityDifferenceToken();


private:
	//! decay type (0 = gamma, 1 = beta, 2 = alpha, 3 = secondary electron)
	int decayType;
	//! number of decays of specific decay channel
	int numberOfDecays;
	//! number of decays for isotope
	int numberOfTotalDecays;
	//! name of the isotope
	std::string isotopeName;
	//! associated token
	std::string tokenName;
	//! literature value for the decay energy
	double literatureDecayEnergy;
	//! uncertainty of the literature decay energy
	double literatureDecayEnergySigma;
	//! literature decay intensity (branching ratio)
	double literatureDecayIntensity;
	//! uncertainty of the literature decay intensity
	double literatureDecayIntensitySigma;
	//! simulated decay energy
	double simulatedDecayEnergy;
	//! uncertainty of the simulated decay energy
	double simulatedDecayEnergySigma;
	//! simulated decay intensity (branching ratio)
	double simulatedDecayIntensity;
	//! uncertainty of the simulated intensity
	double simulatedDecayIntensitySigma;
	//! internal histogram of specific decay channel (histogram range +/- 1% of the literature energy value)
	TH1F *his;
};

#endif /* RADIOACTIVEDECAY_H_ */
