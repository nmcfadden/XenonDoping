/*
 * ParticleTrack.h
 *
 *  Created on: May 9, 2011
 *      Author: fraenkle
 */

#ifndef PARTICLETRACK_H_
#define PARTICLETRACK_H_

#include <vector>
#include <TGraph.h>

class ParticleTrack {
public:
	ParticleTrack();
	virtual ~ParticleTrack();
	void AddNode(double x, double y, double z, double Ekin, double Edep);
	double GetNetDisplacement();
	double GetTrackLength();
	double GetEnergyDeposit();
	double GetEndPositionX();
	double GetEndPositionY();
	double GetEndPositionZ();
	double GetFirstInteractionX();
	double GetFirstInteractionY();
	double GetFirstInteractionZ();
	double GetStartPositionX();
	double GetStartPositionY();
	double GetStartPositionZ();
	double GetPositionX(int stepNumber);
	double GetPositionY(int stepNumber);
	double GetPositionZ(int stepNumber);
	double GetInitialEnergy();
	void Report();
	void PlotTrack();

private:
	std::vector<double*> node;
	TGraph *gr;
	TGraph *grsec;
};

#endif /* PARTICLETRACK_H_ */
