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
#include "../VaLi/ParticleTrack.h"
#include "TH1F.h"

class MaterialInteraction {
public:
	MaterialInteraction();
	MaterialInteraction(std::string t, std::string m, double e, double a);
	std::string GetToken();
	std::string GetMaterialName();
	double GetEnergy();
	std::string GetEnergyToken();
	double GetAttenuationLiterature();
	std::string GetAttenuationLiteratureToken();
	void SetToken(std::string name);
	void SetMaterialName(std::string name);
	void SetEnergy(double e);
	void SetAttenuationLiterature(double a);
	void AddInteractionNode(int eventID, double x, double y, double z, double Ekin, double Edep);
	void TrackReport(int eventID);
	void TrackPlot(int eventID);
	void CreateHistogramEndZ();
	void PlotHistogramEndZ();
	double GetAttenuationSimulation();
	std::string GetAttenuationSimulationToken();
	double GetAttenuationSimulationSigma();
	std::string GetAttenuationDifferenceToken();
	double GetAverageTotalRange();
	std::string GetAverageTotalRangeToken();
	virtual ~MaterialInteraction();

private:
	std::vector<ParticleTrack*> tracks;
	std::string token;
	std::string materialName;
	double energy;
	double attenuationLiterature;

	TH1F* hisEndZ;
};

#endif /* MATERIALINTERACTION_H_ */
