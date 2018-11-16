/*
 * BetaSpectrum.h
 *
 *  Created on: Feb 10, 2011
 *      Author: fraenkle
 */

#include <string>
#include <vector>
#include <TH1F.h>
#include <TGraph.h>
#include <TF1.h>

#ifndef BETASPECTRUM_H_
#define BETASPECTRUM_H_

//! Beta Spectrum
/*!
 * This class contains methods for analysing a beta spectrum
 */
class BetaSpectrum {
public:
	//! Constructor
	/*!
	 * Constructs a BetaSpectrum object with given isotope name and endpoint energy
	 *\param name name of the isotope
	 *\param E0 endpoint energy of the beta decay
	 */
	BetaSpectrum(std::string name, double E0);
	//! Add beta-decay electron to spectrum
	/*!
	 * Adds a beta-decay electron with a given energy to the internal histogram
	 * \param energy energy of the beta-decay electron in keV
	 */
	void AddBetaElectron(double energy);
	//! Add Fermi function value
	/*!
	 * Adds the value of the Fermi function to an internal array
	 * \param eta reduced momentum
	 * \param ffv Fermi function value for eta
	 */
	void AddFermiFunctionValue(double eta, double ffv);
	//! Calcutlate normalized energy epsilon
	/*!
	 * Calculates and returns the normalized energy for a given energy value in keV
	 * \param energy energy value in keV
	 * \return normalized energy
	 */
	double CalculateNormalizedEnergy(double energy);
	//! Calculate energy in keV
	/*!
	 * Calculates and returns the energy in keV for a given normalized energy value
	 * \param epsilon normalized energy value
	 * \return energy in keV
	 */
	double CalculatekeVEnergy(double epsilon);
	//! Calculate normalized momentum eta
	/*!
	 * Calculates and returns the normalized momentum for a given normalized energy value
	 * \param epsilon normalized energy value
	 * \return normalized momentum
	 */
	double CalculateNormalizedMomentum(double epsilon);
	//! Fermi function interpolation
	/*!
	 * Performs a linear interpolation of the Fermi function for a given normalized momentum and returns the Fermi function value
	 * \param eta normalized momentum
	 * \return Fermi function value for eta
	 */
	double FermiFunctionInterpolation(double eta);
	//! Get E0
	/*!
	 * Returns the endpoint energy of the beta-decay
	 * \return endpoint energy in keV
	 */
	double GetE0();
	//! Get E0 from fit
	/*!
	 * Returns the endpoint energy of the beta-decay obtained from a fit to the beta-spectrum
	 * \return fitted endpoint energy in keV
	 */
	double GetFitE0();
	//! Get E0 fit uncertainty
	/*!
	 * Returns the uncertainty on the the endpoint energy from a fit to the beta-spectrum
	 * \return fitted endpoint energy uncertainty in keV
	 */
	double GetFitE0Sigma();
	//! Get fit constant
	/*!
	 * The fit to the beta spectrum is done as a linear fit in a Kurie Plot, returns the slope of the fit function
	 * \return slope of linear fit function
	 */
	double GetFitConstant();
	//! Get fit constant uncertainty
	/*!
	 * The fit to the beta spectrum is done as a linear fit in a Kurie Plot, returns the uncertainty on the slope of the fit function
	 * \return uncertainty on the slope of the fit function
	 */
	double GetFitConstantSigma();
	//! Get Epsilon0
	/*!
	 * Returns the normalized endpoint energy of the beta decay
	 * \return normalized endpoint energy
	 */
	double GetEpsilon0();
	//! Create Kurie plot
	/*!
	 * Creates a Kurie plot of the beta spectrum and stores it in an internal TGraph
	 */
	void CreateKuriePlot();
	//! Create beta spectrum fit
	/*!
	 * Reconstructs a beta spectrum from the fit values of the Kurie plot and stores the spectrum in an internal histogram
	 */
	void CreateBetaSpectrumFit();
	//! Draw Kurie plot
	/*!
	 * Draws Kurie plot of the beta spectrum
	 */
	void DrawKuriePlot();
	//! Draw beta spectrum
	/*!
	 * Draws the beta spectrum and the beta spectrum reconstructed from a fit of the Kurie plot
	 */
	void DrawSpectrum();
	virtual ~BetaSpectrum();

private:
	//! internal histogram for storage of beta spectrum
	TH1F *hisbeta;
	//! internal histogram for storage of fitted beta spectrum
	TH1F *hisbetafit;
	//! internal graph for storage of Kurie plot
	TGraph *grKuriePlot;
	//! internal linear function used to fit Kurie plot
	TF1 *kurieFunction;
	//! literature value of endpoint energy in keV
	double literatureE0;
	//! literature value of endpoint energy (normalized energy)
	double literatureEpsilon0;
	//! beta spectrum endpoint energy obtained from fit to Kurie plot
	double simulatedE0;
	//! electron mass in keV
	static constexpr double m_e = 510.99891;
	//! vector for storage of eta value of tabulated Fermi function
	std::vector<double> normalizedMomentum;
	//! vector for storage of Fermi value of tabulated Fermi function
	std::vector<double> fermiValue;
};

#endif /* BETASPECTRUM_H_ */
