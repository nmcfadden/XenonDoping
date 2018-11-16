/*
 * MaterialInteraction.cxx
 *
 *  Created on: Jun 9, 2011
 *      Author: fraenkle
 */

#include <iostream>
#include <sstream>
#include "TCanvas.h"
#include "TPad.h"
#include "TF1.h"
#include "MaterialInteraction.h"
#include "../VaLi/RComp.h"

MaterialInteraction::MaterialInteraction() {

	token = "";
	materialName = "";
	energy = 0;
	xSectionLiterature = 0;
	hisEndZ = 0;
}

MaterialInteraction::MaterialInteraction(std::string t, std::string m, double e, double x) {

	token = t;
	materialName = m;
	energy = e;
	xSectionLiterature = x;
	hisEndZ = 0;
}

std::string MaterialInteraction::GetMaterialName() {

	return materialName;
}

void MaterialInteraction::SetMaterialName(std::string name) {

	materialName = name;
}

std::string MaterialInteraction::GetToken() {

	return token;
}

void MaterialInteraction::SetToken(std::string name) {

	token = name;
}

double MaterialInteraction::GetXsectionLiterature() {

	return xSectionLiterature;
}

std::string MaterialInteraction::GetXsectionLiteratureToken() {

	std::stringstream label;

	label.str("");
	label.clear();
	label.precision(4);
	label << token.substr(0, token.size() - 3) << "_literature:XX\t" << GetXsectionLiterature();

	return label.str();
}

void MaterialInteraction::SetXsectionLiterature(double x) {

	xSectionLiterature = x;
}

double MaterialInteraction::GetEnergy() {

	if (energy == 0) {
		if (tracks.size() > 0)
			energy = tracks[0]->GetInitialEnergy();
	}

	return energy;
}

std::string MaterialInteraction::GetEnergyToken() {

	std::stringstream label;

	label.str("");
	label.clear();
	label << token.substr(0, token.size() - 3) << "_energy:XX\t" << GetEnergy();

	return label.str();
}

void MaterialInteraction::SetEnergy(double e) {

	energy = e;
}

void MaterialInteraction::AddInteractionNode(int eventID, double x, double y, double z, double Ekin, double Edep) {

	if (tracks.size() <= eventID) {
		ParticleTrack *newtrack = new ParticleTrack();
		tracks.push_back(newtrack);
	}

	x *= 10.0;
	y *= 10.0;
	z *= 10.0;

	tracks[eventID]->AddNode(x, y, z, Ekin, Edep);
}

void MaterialInteraction::TrackReport(int eventID) {

	if (eventID < tracks.size()) {
		std::cout
				<< "___________________________________________________________\n";
		tracks[eventID]->Report();
		std::cout
				<< "___________________________________________________________\n\n";
	}
}

void MaterialInteraction::TrackPlot(int eventID) {

	if (eventID < tracks.size())
		tracks[eventID]->PlotTrack();
}

void MaterialInteraction::CreateHistogramEndZ() {

	double maxZ = 0.0;
	for (int i = 0; i < tracks.size(); i++) {
		if (tracks[i]->GetFirstInteractionZ() - tracks[i]->GetStartPositionZ() > maxZ)
			maxZ = tracks[i]->GetFirstInteractionZ() - tracks[i]->GetStartPositionZ();
	}

	if (hisEndZ) {
		delete hisEndZ;
		std::cout << "WARNING from MaterialInteraction: Replacing internal histogram!\n";
	}

	hisEndZ = new TH1F(token.c_str(), "end point z direction", 1000, 0.0, maxZ * 1.1);

	for (int i = 0; i < tracks.size(); i++) {
		hisEndZ->Fill(tracks[i]->GetFirstInteractionZ() - tracks[i]->GetStartPositionZ());
	}

	int sum;
	for (int i = 0; i < hisEndZ->GetNbinsX(); i++) {
		sum = 0;
		for (int j = i; j < hisEndZ->GetNbinsX(); j++) {
			sum += hisEndZ->GetBinContent(j);
		}
		hisEndZ->SetBinContent(i, sum);
	}
}

void MaterialInteraction::PlotHistogramEndZ() {

	if (hisEndZ == 0)
		CreateHistogramEndZ();

	hisEndZ->SetLineWidth(2);
	hisEndZ->SetLineColor(kRed);
	hisEndZ->SetXTitle("penetration depth [mm]");

	hisEndZ->Draw();
}

double MaterialInteraction::GetMeanFreePath() {

	if (hisEndZ == 0)
		CreateHistogramEndZ();

	TF1 *expfit = new TF1("expfit", "expo", 0.0, 1000.0);
	hisEndZ->Fit(expfit, "Q");

	return -0.1 / expfit->GetParameter(1);

}

int MaterialInteraction::GetNumberOfTracks() {

	return tracks.size();
}

double MaterialInteraction::GetXsectionSimulation(double density, double weight, int N) {

	double xsection = N_a * density / weight;
	xsection = 1 / (xsection * GetMeanFreePath()) * 1e24;
	xsection = xsection * ((double) (GetNumberOfTracks()) / (double) (N));

	return xsection;
}

std::string MaterialInteraction::GetXsectionSimulationToken(double density, double weight, int N) {

	std::stringstream label;

	label.str("");
	label.clear();
	label.precision(4);
	label << token.substr(0, token.size() - 3) << "_simulation:XX\t" << GetXsectionSimulation(density, weight, N);

	return label.str();
}

std::string MaterialInteraction::GetXsectionDifferenceToken(double density, double weight, int N) {

	std::stringstream label;
	RComp *rcomp = new RComp();

	label.str("");
	label.clear();
	label << token.substr(0, token.size() - 3) << "_difference:XX\t";
	label << rcomp->GetAgreementColourString(GetXsectionSimulation(density, weight, N), GetXsectionSimulation(density, weight, N) * 0.1, GetXsectionLiterature(), 0.0, 10.0);

	delete rcomp;
	return label.str();
}

double MaterialInteraction::GetFirstScatterAngle(int eventID) {

	double a[3] = { 0 };
	double b[3] = { 0 };
	double theta = 0.0;

	if (eventID < tracks.size()) {
		a[0] = tracks[eventID]->GetPositionX(1);
		a[1] = tracks[eventID]->GetPositionY(1);
		a[2] = tracks[eventID]->GetPositionZ(1);

		b[0] = tracks[eventID]->GetPositionX(2) - a[0];
		b[1] = tracks[eventID]->GetPositionY(2) - a[1];
		b[2] = tracks[eventID]->GetPositionZ(2) - a[2];

		theta = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / (sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) * sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]));
		theta = acos(theta);
	}

	return theta;
}

MaterialInteraction::~MaterialInteraction() {

	for (int i = 0; i < tracks.size(); i++)
		delete tracks[i];

	tracks.clear();
}
