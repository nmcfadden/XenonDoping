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
	attenuationLiterature = 0;
	hisEndZ = 0;
}

MaterialInteraction::MaterialInteraction(std::string t, std::string m, double e, double a){

	token = t;
	materialName = m;
	energy = e;
	attenuationLiterature = a;
	hisEndZ = 0;
}

std::string MaterialInteraction::GetMaterialName(){

	return materialName;
}

void MaterialInteraction::SetMaterialName(std::string name){

	materialName = name;
}

std::string MaterialInteraction::GetToken(){

	return token;
}

void MaterialInteraction::SetToken(std::string name){

	token = name;
}

double MaterialInteraction::GetAttenuationLiterature(){

	return attenuationLiterature;
}

std::string MaterialInteraction::GetAttenuationLiteratureToken(){

	std::stringstream label;

	label.str("");
	label.clear();
	label.precision(4);
	label << token.substr(0,token.size()-3) << "_attenuation_literature:XX\t" << GetAttenuationLiterature();

	return label.str();
}

void MaterialInteraction::SetAttenuationLiterature(double a){

	attenuationLiterature = a;
}

double MaterialInteraction::GetEnergy(){

	if(energy == 0){
		if(tracks.size() > 0)
			energy = tracks[0]->GetInitialEnergy();
	}

	return energy;
}

std::string MaterialInteraction::GetEnergyToken(){

	std::stringstream label;

	label.str("");
	label.clear();
	label << token.substr(0,token.size()-3) << "_energy:XX\t" << GetEnergy();

	return label.str();
}

void MaterialInteraction::SetEnergy(double e){

	energy = e;
}

void MaterialInteraction::AddInteractionNode(int eventID,double x, double y, double z, double Ekin,double Edep){

	if(tracks.size() <= eventID){
		ParticleTrack *newtrack = new ParticleTrack();
		tracks.push_back(newtrack);
	}

	tracks[eventID]->AddNode(x,y,z,Ekin,Edep);
}

void MaterialInteraction::TrackReport(int eventID){

	if(eventID < tracks.size()){
		tracks[eventID]->Report();
		std::cout << "Track Length: " << tracks[eventID]->GetTrackLength() << "\n";
	}
}

void MaterialInteraction::TrackPlot(int eventID){

	if(eventID < tracks.size())
		tracks[eventID]->PlotTrack();
}

void MaterialInteraction::CreateHistogramEndZ(){

	double maxZ = 0.0;
	for(int i=0;i<tracks.size();i++){
		if(tracks[i]->GetTrackLength() > maxZ)
			maxZ = tracks[i]->GetTrackLength();
	}

	if(hisEndZ){
		delete hisEndZ;
		std::cout << "WARNING from MaterialInteraction: Replacing internal histogram!\n";
	}

	std::stringstream label;
	label.str("");
	label.clear();
	label << "hisEndZ_" << GetEnergy() << "MeV";

	hisEndZ = new TH1F(label.str().c_str(),"track length",1000,0.0,maxZ * 1.1);

	for(int i=0;i<tracks.size();i++){
		hisEndZ->Fill(tracks[i]->GetTrackLength());
	}
}

void MaterialInteraction::PlotHistogramEndZ(){

	if(hisEndZ==0)
		CreateHistogramEndZ();

	hisEndZ->SetLineWidth(2);
	hisEndZ->SetLineColor(kRed);
	hisEndZ->SetXTitle("penetration depth [mm]");

	hisEndZ->Draw();
}

double MaterialInteraction::GetAttenuationSimulation(){

	if(hisEndZ==0)
		CreateHistogramEndZ();

	TF1 *expfit = new TF1("expfit","expo",0.0,1000.0);
	hisEndZ->Fit(expfit,"Q");

	return -10.0 * expfit->GetParameter(1);
}

std::string MaterialInteraction::GetAttenuationSimulationToken(){

	std::stringstream label;

	label.str("");
	label.clear();
	label.precision(4);
	label << token.substr(0,token.size()-3) << "_attenuation_simulation:XX\t" << GetAttenuationSimulation() << " $\\pm$ ";
	label.precision(2);
	label << GetAttenuationSimulationSigma();

	return label.str();
}

double MaterialInteraction::GetAttenuationSimulationSigma(){

	if(hisEndZ==0)
		CreateHistogramEndZ();

	TF1 *expfit = new TF1("expfit","expo",0.0,1000.0);
	hisEndZ->Fit(expfit,"Q");

	return 10.0 * expfit->GetParError(1);
}

std::string MaterialInteraction::GetAttenuationDifferenceToken(){

	std::stringstream label;
	RComp *rcomp = new RComp();

	label.str("");
	label.clear();
	label << token.substr(0,token.size()-3) << "_attenuation_difference:XX\t";
	label << rcomp->GetAgreementColourString(GetAverageTotalRange(),GetAttenuationLiterature()*0.005,GetAttenuationLiterature(),0.0,10.0);

	delete rcomp;
	return label.str();
}

double MaterialInteraction::GetAverageTotalRange(){
	double range = 0.0;
	for(int i=0;i<tracks.size();i++){
		range += tracks[i]->GetTrackLength();
	}

	range /= (double)(tracks.size());

	return range;
}

std::string MaterialInteraction::GetAverageTotalRangeToken(){

	std::stringstream label;

	label.str("");
	label.clear();
	label << token.substr(0,token.size()-3) << "_attenuation_simulation:XX\t";
	label << GetAverageTotalRange();

	return label.str();
}

MaterialInteraction::~MaterialInteraction() {

	for(int i=0;i<tracks.size();i++)
		delete tracks[i];

	tracks.clear();
//	if(hisEndZ)
//		delete hisEndZ;
}
