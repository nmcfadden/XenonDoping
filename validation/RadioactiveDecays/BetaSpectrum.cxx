/*
 * BetaSpectrum.cxx
 *
 *  Created on: Feb 10, 2011
 *      Author: fraenkle
 */

#include "BetaSpectrum.h"
#include <math.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>


BetaSpectrum::BetaSpectrum(std::string name, double E0) {

	literatureE0 = E0;
	literatureEpsilon0 = CalculateNormalizedEnergy(E0);
	kurieFunction = new TF1("KurieFunction","pol1",1.0,literatureEpsilon0 * 1.1);
	hisbeta = new TH1F(name.c_str(),name.c_str(),(int)(E0*1.1/10.0),0,(int)(E0*1.1/10.0)*10);
	hisbetafit = new TH1F("hisbetafit","hisbetafit",(int)(E0*1.1/10.0)*10,0,(int)(E0*1.1/10.0)*10);
	grKuriePlot = 0;
}

void BetaSpectrum::AddBetaElectron(double energy){

	hisbeta->Fill(energy);
}

void BetaSpectrum::AddFermiFunctionValue(double eta, double ffv){

	normalizedMomentum.push_back(eta);
	fermiValue.push_back(ffv);
}

double BetaSpectrum::CalculateNormalizedEnergy(double energy){

	return (energy + m_e) / m_e;
}

double BetaSpectrum::CalculatekeVEnergy(double epsilon){

	return epsilon * m_e - m_e;
}

double BetaSpectrum::CalculateNormalizedMomentum(double epsilon){

	return sqrt( (epsilon*epsilon) - 1);
}

double BetaSpectrum::FermiFunctionInterpolation(double eta){

	double ffi, m, b;
	int index = -1;

	for(int i=0;i<normalizedMomentum.size()-1;i++){
		if(eta >= normalizedMomentum[i] && eta < normalizedMomentum[i+1]){
			index = i;
			break;
		}
	}

	m = (fermiValue[index+1]-fermiValue[index]) / (normalizedMomentum[index+1]-normalizedMomentum[index]);
	b = fermiValue[index] - m * normalizedMomentum[index];

	ffi = m * eta + b;
	return ffi;
}

void BetaSpectrum::DrawSpectrum(){

	TLegend *leg = new TLegend(0.75,0.99,0.99,0.9);

	hisbeta->SetXTitle("energy [keV]");
	hisbeta->SetLineColor(kBlue);
	hisbeta->SetLineWidth(1.5);
	leg->AddEntry(hisbeta,"simulated #beta-spectrum","lp");
	hisbeta->Draw("E1");

	hisbetafit->SetLineColor(kGray + 3);
	hisbetafit->SetLineWidth(0.5);
	leg->AddEntry(hisbetafit,"fit","l");
	hisbetafit->Draw("SAME");

	leg->Draw();
}

void BetaSpectrum::CreateKuriePlot(){

	double eta, epsilon;
	double x, y, Neta;

	grKuriePlot = new TGraph(hisbeta->GetNbinsX()-1);
	for(int i=1;i<hisbeta->GetNbinsX();i++){
		epsilon = CalculateNormalizedEnergy(i * 10 - 5);
		eta = CalculateNormalizedMomentum(epsilon);
		Neta = hisbeta->GetBinContent(i) * sqrt(epsilon * epsilon - 1)/epsilon;
		x = epsilon;
		y = sqrt(Neta/(eta * epsilon * FermiFunctionInterpolation(eta)));
		grKuriePlot->SetPoint(i-1,x,y);
	}
}

void BetaSpectrum::CreateBetaSpectrumFit(){

	double K = GetFitConstant();
	double epsilon0 = CalculateNormalizedEnergy(GetFitE0());
	double E, N, epsilon, eta;

	for(int i=1;i<hisbetafit->GetNbinsX();i++){
		E = i - 0.5;
		epsilon = CalculateNormalizedEnergy(E);
		if(epsilon>epsilon0)
			break;
		eta = CalculateNormalizedMomentum(epsilon);
		N = K * eta * epsilon * FermiFunctionInterpolation(eta) * (epsilon0 - epsilon) * (epsilon0 - epsilon);
		N *= epsilon/sqrt(epsilon*epsilon -1);

		hisbetafit->Fill(E,N);
	}

}

void BetaSpectrum::DrawKuriePlot(){

	if(grKuriePlot == 0)
		CreateKuriePlot();
	grKuriePlot->SetTitle("Kurie Plot");
	grKuriePlot->GetXaxis()->SetTitle("#varepsilon");
	grKuriePlot->GetYaxis()->SetTitle("#sqrt{#frac{N(#eta)}{#eta #varepsilon G(#eta,Z)}}");

	TCanvas *can = new TCanvas("KPlot","Kurie Plot",1100,500);
	can->SetGrid();
	grKuriePlot->Draw("AP");
}

double BetaSpectrum::GetFitE0(){

	if(grKuriePlot == 0)
		CreateKuriePlot();
	grKuriePlot->Fit("KurieFunction","QR");

	return CalculatekeVEnergy(-1.0 * kurieFunction->GetParameter(0)/kurieFunction->GetParameter(1));
}

double BetaSpectrum::GetFitE0Sigma(){

	if(grKuriePlot == 0)
		CreateKuriePlot();
	grKuriePlot->Fit("KurieFunction","QR");

	double p0 = kurieFunction->GetParameter(0);
	double p0s = kurieFunction->GetParError(0);
	double p1 = kurieFunction->GetParameter(1);
	double p1s = kurieFunction->GetParError(1);

	double E0sigma = 0;

	E0sigma = sqrt( (1.0/(p1*p1) * p0s * p0s) + ((p0/(p1*p1))*(p0/(p1*p1)) * p1s * p1s));
	return CalculatekeVEnergy(1+E0sigma);
}

double BetaSpectrum::GetFitConstant(){

	if(grKuriePlot == 0)
		CreateKuriePlot();
	grKuriePlot->Fit("KurieFunction","QR");

	return kurieFunction->GetParameter(1) * kurieFunction->GetParameter(1);
}

double BetaSpectrum::GetFitConstantSigma(){

	if(grKuriePlot == 0)
		CreateKuriePlot();
	grKuriePlot->Fit("KurieFunction","QR");

	return 2 * kurieFunction->GetParError(1) * kurieFunction->GetParameter(1);
}

double BetaSpectrum::GetE0(){

	return literatureE0;
}

double BetaSpectrum::GetEpsilon0(){

	return literatureEpsilon0;
}

BetaSpectrum::~BetaSpectrum() {

	delete hisbeta;
	delete hisbetafit;
	delete kurieFunction;
	if(grKuriePlot != 0)
		delete grKuriePlot;
}
