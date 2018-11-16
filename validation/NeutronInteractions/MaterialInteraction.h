/*
 * MaterialInteraction.h
 *
 *  Created on: Jun 9, 2011
 *      Author: fraenkle
 */

#ifndef MATERIALINTERACTION_H_
#define MATERIALINTERACTION_H_

#include <string>
#include <vector>
#include <math.h>
#include "../VaLi/ParticleTrack.h"
#include "TH1F.h"

const double N_a = 6.02214e23;

class MaterialInteraction {
public:
	MaterialInteraction();
	MaterialInteraction(std::string t, std::string m, double e, double x);
	std::string GetToken();
	std::string GetMaterialName();
	double GetEnergy();
	std::string GetEnergyToken();
	double GetXsectionLiterature();
	std::string GetXsectionLiteratureToken();
	void SetToken(std::string name);
	void SetMaterialName(std::string name);
	void SetEnergy(double e);
	void SetXsectionLiterature(double x);
	void AddInteractionNode(int eventID, double x, double y, double z, double Ekin, double Edep);
	void TrackReport(int eventID);
	void TrackPlot(int eventID);
	void CreateHistogramEndZ();
	void PlotHistogramEndZ();
	double GetMeanFreePath();
	double GetMeanFreePathSigma();
	int GetNumberOfTracks();
	double GetXsectionSimulation(double density, double weight, int N);
	std::string GetXsectionSimulationToken(double density, double weight, int N);
	std::string GetXsectionDifferenceToken(double density, double weight, int N);
	double GetFirstScatterAngle(int eventID);
	virtual ~MaterialInteraction();

private:
	std::vector<ParticleTrack*> tracks;
	std::string token;
	std::string materialName;
	double energy;
	double xSectionLiterature;

	TH1F* hisEndZ;
};

#endif /* MATERIALINTERACTION_H_ */
