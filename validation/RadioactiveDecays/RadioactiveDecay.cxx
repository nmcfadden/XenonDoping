/*
 * RadioactiveDecay.cxx
 *
 *  Created on: Feb 7, 2011
 *      Author: fraenkle
 */

#include "RadioactiveDecay.h"
#include "../VaLi/RComp.h"
#include <math.h>
#include <sstream>
#include <iostream>
#include <TCanvas.h>
#include <TLatex.h>

RadioactiveDecay::RadioactiveDecay(int type, std::string name, double energy,
		double energysigma, double intensity, double intensitysigma,
		std::string token) {

	decayType = type;
	isotopeName = name;
	literatureDecayEnergy = energy;
	literatureDecayEnergySigma = energysigma;
	literatureDecayIntensity = intensity;
	literatureDecayIntensitySigma = intensitysigma;
	tokenName = token;

	his = new TH1F(tokenName.c_str(), isotopeName.c_str(), 1000,
			literatureDecayEnergy - literatureDecayEnergy * 0.01,
			literatureDecayEnergy + literatureDecayEnergy * 0.01);
	numberOfDecays = 0;
	numberOfTotalDecays = 0;

}

void RadioactiveDecay::AddDecay(double energy) {

	his->Fill(energy);
	numberOfTotalDecays++;
}

void RadioactiveDecay::DrawHistogram(){

	std::string s;
	std::stringstream label;
	TLatex *latexstring;

	label.str("");
	label.clear();
	label << isotopeName << " - " << literatureDecayEnergy;
	s = label.str();

	TCanvas *can = new TCanvas(s.c_str());
	can->SetGrid();

	his->SetTitle(s.c_str());
	his->SetXTitle("energy [keV]");
	his->SetFillColor(kBlack);
	his->Draw();

	label.str("");
	label.clear();
	label << "detected energy: " << GetSimulatedDecayEnergy() << " keV   literature energy: " << GetLiteratureDecayEnergy() << " keV";
	s = label.str();
	latexstring = new TLatex(0.3, 0.965,s.c_str());
	latexstring->SetNDC();
	latexstring->SetTextSize(0.035);
	latexstring->SetTextColor(kBlue);
	latexstring->Draw();

	label.str("");
	label.clear();
	label << "detected events: " << GetNumberOfDecays() << "   expected events: " << GetLiteratureDecayIntensity() * GetNumberOfTotalDecays() / 100.0;
	s = label.str();
	latexstring = new TLatex(0.3, 0.925,s.c_str());
	latexstring->SetNDC();
	latexstring->SetTextSize(0.035);
	latexstring->SetTextColor(kBlue);
	latexstring->Draw();

}

int RadioactiveDecay::GetDecayType() {

	return decayType;
}

std::string RadioactiveDecay::GetIsotopeName() {

	return isotopeName;
}

std::string RadioactiveDecay::GetToken(){

	return tokenName;
}

double RadioactiveDecay::GetLiteratureDecayEnergy() {

	return literatureDecayEnergy;
}

double RadioactiveDecay::GetLiteratureDecayEnergySigma() {

	return literatureDecayEnergySigma;
}

std::string RadioactiveDecay::GetLiteratureDecayEnergyToken(){

	std::string s;
	std::stringstream label;

	s = tokenName.substr(0, tokenName.length());
	label.str("");
	label.clear();
	label << s << "_energy_literature:XX\t";
	label << literatureDecayEnergy << " $\\pm$ " << literatureDecayEnergySigma << "\n";

	s = label.str();
	return s;
}

double RadioactiveDecay::GetLiteratureDecayIntensity() {

	return literatureDecayIntensity;
}

double RadioactiveDecay::GetLiteratureDecayIntensitySigma() {

	return literatureDecayIntensitySigma;
}

std::string RadioactiveDecay::GetLiteratureDecayIntensityToken() {

	std::string s;
	std::stringstream label;

	s = tokenName.substr(0, tokenName.length());
	label.str("");
	label.clear();
	label << s << "_intensity_literature:XX\t";
	label << literatureDecayIntensity << " $\\pm$ " << literatureDecayIntensitySigma << "\n";

	s = label.str();
	return s;
}

int RadioactiveDecay::GetNumberOfDecays() {

	int decaynumber = 0;

	int dmin = 0;
	std::vector<int> peakposition;

	for (int i = 1; i < his->GetNbinsX() - 1; i++) {
		if (his->GetBinContent(i) > 0)
			peakposition.push_back(i);
	}
	if (peakposition.size() == 0)
		return 0;

	for (int i = 0; i < peakposition.size(); i++) {
		if (fabs(peakposition[dmin] - his->GetNbinsX() / 2) > fabs(peakposition[i] - his->GetNbinsX() / 2))
			dmin = i;
	}

	for (int i = peakposition[dmin] - 2; i <= peakposition[dmin] + 2; i++) {
		if (i < 1)
			continue;
		if (i >= his->GetNbinsX() - 1)
			continue;
		decaynumber += his->GetBinContent(i);
	}

	return decaynumber;
}

int RadioactiveDecay::GetNumberOfTotalDecays() {

	return numberOfTotalDecays;
}

void RadioactiveDecay::SetNumberOfTotalDecays(int number){

	numberOfTotalDecays = number;
}

double RadioactiveDecay::GetSimulatedDecayEnergy() {

	double decayenergy = 0.0;
	int decaynumber = 0;

	int dmin = 0;
	std::vector<int> peakposition;

	for (int i = 1; i < his->GetNbinsX() - 1; i++) {
		if (his->GetBinContent(i) > 0)
			peakposition.push_back(i);
	}
	if (peakposition.size() == 0)
		return 0;

	for (int i = 0; i < peakposition.size(); i++) {
		if (fabs(peakposition[dmin] - his->GetNbinsX() / 2) > fabs(peakposition[i] - his->GetNbinsX() / 2))
			dmin = i;
	}

	decayenergy =  GetLiteratureDecayEnergy() + (peakposition[dmin] - his->GetNbinsX()/2)*GetLiteratureDecayEnergy()*0.02/1000.0;

	return decayenergy;
}

double RadioactiveDecay::GetSimulatedDecayEnergySigma() {

	// not implemented so far
	return 0.0;
}

std::string RadioactiveDecay::GetSimulatedDecayEnergyToken() {

	std::string s;
	std::stringstream label;

	s = tokenName.substr(0, tokenName.length() - 0);
	label.str("");
	label.clear();
	label.precision(5);
	label << s << "_energy:XX\t";
	label << GetSimulatedDecayEnergy() << "\n";

	s = label.str();
	return s;
}

double RadioactiveDecay::GetSimulatedDecayIntensity() {

	double decayintensity = 0.0;
	decayintensity = GetNumberOfDecays() / (double)(numberOfTotalDecays) * 100.0;
	return decayintensity;
}

double RadioactiveDecay::GetSimulatedDecayIntensitySigma() {

	double decayintensitysigma = 0.0;

	if(GetNumberOfDecays() < 10){	// use binomial estimators
		double Nsim = GetNumberOfDecays();
		double Ntot = numberOfTotalDecays;
		decayintensitysigma = (Nsim+1)*(Nsim+2)/((Ntot+2)*(Ntot+3)) - (Nsim+1)*(Nsim+1)/((Ntot+2)*(Ntot+2));
		decayintensitysigma *= 100.0;
	}
	else{
		decayintensitysigma = sqrt(GetNumberOfDecays()) / (double)(numberOfTotalDecays) * 100.0;
	}
	return decayintensitysigma;
}

std::string RadioactiveDecay::GetSimulatedDecayIntensityToken() {

	std::string s;
	std::stringstream label;

	s = tokenName.substr(0, tokenName.length() - 0);
	label.str("");
	label.clear();
	label << s << "_intensity:XX\t";
	label.precision(3);
	label << GetSimulatedDecayIntensity() << " $\\pm$ ";
	label.precision(2);
	label << GetSimulatedDecayIntensitySigma() << "\n";

	s = label.str();
	return s;
}

std::string RadioactiveDecay::GetEnergyDifferenceToken() {

	std::string s;
	std::stringstream label;
	RComp *rcomp = new RComp();

	s = tokenName.substr(0, tokenName.length() - 0);
	label.str("");
	label.clear();
	label << s << "_energy_diff:XX\t";

	if (GetNumberOfDecays() == 0 && (double)(GetNumberOfTotalDecays()) * literatureDecayIntensity / 100.0 < 2.0) {
		label << "\\colorbox{Gray}{\\color{White} low }\n";
	}
	else {
		label << rcomp->GetAgreementColourString(GetSimulatedDecayEnergy(),GetSimulatedDecayEnergySigma(),literatureDecayEnergy, literatureDecayEnergySigma,1.0);
	}
	s = label.str();
	delete rcomp;
	return s;
}

std::string RadioactiveDecay::GetIntensityDifferenceToken() {

	std::string s;
	std::stringstream label;
	RComp *rcomp = new RComp();

	s = tokenName.substr(0, tokenName.length() - 0);
	label.str("");
	label.clear();
	label << s << "_intensity_diff:XX\t";

	if (GetNumberOfDecays() == 0 && (double)(GetNumberOfTotalDecays()) * literatureDecayIntensity / 100.0 < 2.0) {
		label << "\\colorbox{Gray}{\\color{White} low }\n";
	} else {
		label << rcomp->GetAgreementColourString(GetSimulatedDecayIntensity(),GetSimulatedDecayIntensitySigma(),literatureDecayIntensity, literatureDecayIntensitySigma,10.0);
	}

	s = label.str();
	delete rcomp;
	return s;
}

RadioactiveDecay::~RadioactiveDecay() {
	// TODO Auto-generated destructor stub
	delete his;
}
