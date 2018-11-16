// C++ headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <string>

// CERN ROOT headers
#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TLegend.h>

// MaGe Validation headers
#include "RadioactiveDecay.h"
#include "BetaSpectrum.h"
#include "SingleDecay.h"
#include "../VaLi/RComp.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc < 3) {
		cout << "Usage: DecayAnalysis <data file> <ROOT file>\n";
		return 0;
	}

    gROOT->SetBatch(true);

	cout << "DecayAnalysis number of arguments: " << argc << "\n";

	string filename(argv[1]);
	string datafile(argv[2]);
	string isotope = filename.substr(4,filename.length()-8);
	string compfile = "";
	string compfile_new = "";
	int relcomp = 0;

	if(argc == 4){
		relcomp = 1;
		compfile = argv[3];
	}

	if(argc == 5){
		relcomp = 1;
		compfile = argv[3];
		compfile_new = argv[4];
	}

	TCanvas* can;
	TApplication theApp("App", &argc, argv);

	TH1F *hisbeta, *hisneutrino, *hissecondary, *hisgamma, *hisalpha, *histime;
	ifstream inputfile;
	ofstream outputfile;

	double temp;
	istringstream conv;
	stringstream label;
	string s;

	vector<RadioactiveDecay*> alphadecays;
	vector<RadioactiveDecay*> betadecays;
	vector<RadioactiveDecay*> gammadecays;
	vector<RadioactiveDecay*> secondaryelectrons;
	BetaSpectrum *betaenergyspectrum = 0;


	// Relative Comparison
	RComp *rcomp = new RComp();
	RComp *rcomp_new = new RComp();

	// Adjusting ROOT style
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	string* ps;
	size_t endposition;

	inputfile.open(filename.c_str());
	if (inputfile.fail()) {
		cout << "ERROR! Can't open file \"" << filename << "\"\n";
	}

	int decayType;
	double decayEnergy, sigmaDecayEnergy, decayIntensity, sigmaDecayIntensity, betamax, halflifelit;

	betamax = 0;
	while (getline(inputfile, s)) {

		ps = new string[2];

		endposition = s.find(":XX", 0);
		if(s[0]=='#')
			continue;
		if(endposition==string::npos)
			continue;

		ps[0] = s.substr(0, endposition);
		ps[1] = s.substr(endposition + 3);

		conv.str(ps[1]);
		conv >> decayType;

		switch(decayType){
		case 0:
			conv >> decayEnergy;
			conv >> sigmaDecayEnergy;
			conv >> decayIntensity;
			conv >> sigmaDecayIntensity;
			gammadecays.push_back(new RadioactiveDecay(decayType, isotope, decayEnergy, sigmaDecayEnergy, decayIntensity, sigmaDecayIntensity, ps[0]));
			break;
		case 1:
			conv >> decayEnergy;
			conv >> sigmaDecayEnergy;
			conv >> decayIntensity;
			conv >> sigmaDecayIntensity;
			betadecays.push_back(new RadioactiveDecay(decayType, isotope, decayEnergy, sigmaDecayEnergy, decayIntensity, sigmaDecayIntensity, ps[0]));
			if(decayEnergy>betamax)
				betamax = decayEnergy;
			break;
		case 2:
			conv >> decayEnergy;
			conv >> sigmaDecayEnergy;
			conv >> decayIntensity;
			conv >> sigmaDecayIntensity;
			alphadecays.push_back(new RadioactiveDecay(decayType, isotope, decayEnergy, sigmaDecayEnergy, decayIntensity, sigmaDecayIntensity, ps[0]));
			break;
		case 3:
			conv >> decayEnergy;
			conv >> sigmaDecayEnergy;
			conv >> decayIntensity;
			conv >> sigmaDecayIntensity;
			secondaryelectrons.push_back(new RadioactiveDecay(decayType, isotope, decayEnergy, sigmaDecayEnergy, decayIntensity, sigmaDecayIntensity, ps[0]));
			break;
		case 100:
			conv >> decayEnergy;
			label.str("");
			label.clear();
			label << isotope << "  -  " << decayEnergy << " keV #beta-decay";
			s = label.str();
			betaenergyspectrum = new BetaSpectrum(s.c_str(),decayEnergy);
			while (getline(inputfile, s)) {
				if(s[0] == '#')
					break;
				conv.str("");
				conv.clear();
				conv.str(s);
				conv >> decayEnergy;
				conv >> decayIntensity;
				betaenergyspectrum->AddFermiFunctionValue(decayEnergy,decayIntensity);
			}
			break;
		case 101:
			conv >> halflifelit;
			break;
		}

		conv.str("");
		conv.clear();
	}
	inputfile.close();


	if( (gammadecays.size() + betadecays.size() + alphadecays.size() + secondaryelectrons.size()) == 0){
		cout << "No literature data available! DecayAnalyis terminated.\n";
		return 0;
	}

	cout << "From " << datafile << ":" << "\t";
	cout << "alpha: " << alphadecays.size() << "\t";
	cout << "beta: " << betadecays.size() << "\t";
	cout << "gamma: " << gammadecays.size() << "\t";
	cout << "second: " << secondaryelectrons.size() << "\n";


	can = new TCanvas("can","DecayAnalysis",1100,500);
	can->SetGrid();
	can->Connect("Closed()", "TApplication", &theApp, "Terminate()");

	// temporary change
	hisbeta = new TH1F("hisbeta","histogram",8000,0.0,8000.0);
	hisneutrino = new TH1F("hisneutrino","histogram",8000,0.0,8000.0);
	hissecondary = new TH1F("hissecondary","histogram",8000,0.0,8000.0);
	hisgamma = new TH1F("hisgamma","histogram",8000,0.0,8000.0);
	hisalpha = new TH1F("hisalpha","histogram",8000,0.0,8000.0);
	vector <double> timevector;

	inputfile.open(datafile.c_str());
	if(inputfile.fail()){
		cout << "DecayAnalysis:\tERROR! Can't open file " << datafile << " !\n";
		return 0;
	}
	inputfile.close();

	UInt_t decayID, decayID_old;
	Int_t PID;
	Double_t time, energy, betaDecayEnergy, betaEnergy;

	TFile *runfile = new TFile(datafile.c_str());
	TTree *decaytree = (TTree*)runfile->Get("decaytree");

	decaytree->SetBranchAddress("decayID",&decayID);
	decaytree->SetBranchAddress("PID",&PID);
	decaytree->SetBranchAddress("time",&time);
	decaytree->SetBranchAddress("energy",&energy);

	int index = 0;
	int NDecaysTotal = 0;
	int electronCounter = 0;
	decayID_old = 0;

	decaytree->GetEntry(index);
	energy *= 1000.0;
	double maxtime=0;
	int decaytype=0;
	do {
		SingleDecay *decay = new SingleDecay();
		while (1) {
			decay->AddParticle(PID, energy, time);
			index++;
			if (index >= decaytree->GetEntries())
				break;
			decaytree->GetEntry(index);


			energy *= 1000.0;
			if (decayID != decayID_old) {
				decayID_old = decayID;
				break;
			}
		}
			decaytype=decay->GetDecayType();
			//If the decay type is not a gamma emission, stores time in timevector		
			if(decaytype !=0){
				timevector.push_back(decay->GetDecayTime(decaytype)/10e8);
						}
			//Keeps track of the longest time so far, stores it as maxtime
			if(decay->GetDecayTime(decaytype)>maxtime && decaytype!=0){
				maxtime=decay->GetDecayTime(decaytype);
			}

		switch(decay->GetDecayType()){
		case 1:
			if(betadecays.size() != 0)
				hisbeta->Fill(decay->GetDecayEnergy(1));
			for(int j=0;j<betadecays.size();j++)
				betadecays[j]->AddDecay(decay->GetDecayEnergy(1));
			if(decay->GetDecayEnergy(1) > (betamax*0.99) && betaenergyspectrum != 0){
				betaenergyspectrum->AddBetaElectron(decay->electron[0]);
			}
			for(int j=1;j<decay->electron.size();j++){
				hissecondary->Fill(decay->electron[j]);
				for(int k=0;k<secondaryelectrons.size();k++){
					secondaryelectrons[k]->AddDecay(decay->electron[j]);
				}
			}
			break;
		case 2:
			if(betadecays.size() != 0)
				hisbeta->Fill(decay->GetDecayEnergy(2));
			for(int j=0;j<betadecays.size();j++)
				betadecays[j]->AddDecay(decay->GetDecayEnergy(2));
			if(decay->GetDecayEnergy(2) > (betamax*0.99) && betaenergyspectrum != 0){
				betaenergyspectrum->AddBetaElectron(decay->positron[0]);
			}
			for(int j=0;j<decay->electron.size();j++){
				hissecondary->Fill(decay->electron[j]);
				for(int k=0;k<secondaryelectrons.size();k++){
					secondaryelectrons[k]->AddDecay(decay->electron[j]);
				}
			}
			break;
		case 3:
			for(int j=0;j<decay->electron.size();j++){
				hissecondary->Fill(decay->electron[j]);
				for(int k=0;k<secondaryelectrons.size();k++){
					secondaryelectrons[k]->AddDecay(decay->electron[j]);
				}
			}
			break;
		case 4:
			hisalpha->Fill(decay->GetDecayEnergy(4));
			for(int j=0;j<alphadecays.size();j++){
				alphadecays[j]->AddDecay(decay->GetDecayEnergy(4));
			}
			for(int j=0;j<decay->electron.size();j++){
				hissecondary->Fill(decay->electron[j]);
				for(int k=0;k<secondaryelectrons.size();k++){
					secondaryelectrons[k]->AddDecay(decay->electron[j]);
				}
			}
			break;
		}

		for (int j = 0; j < decay->gamma.size(); j++) {
			hisgamma->Fill(decay->gamma[j]);
			for (int k = 0; k < gammadecays.size(); k++) {
				gammadecays[k]->AddDecay(decay->gamma[j]);
			}
		}

		delete decay;

	} while (index < decaytree->GetEntries());

	runfile->Close();

	NDecaysTotal = decayID + 1;
	cout << "number of simulated decays: " << NDecaysTotal << "\n";

	label.str("");
	label.clear();
	label << isotope << "parameter.txt";
	s = label.str();

	double NQualityEnergy = 0;
	double NQualityIntensity = 0;
	double Ntotal = betadecays.size() + alphadecays.size() + gammadecays.size() + secondaryelectrons.size();
	Ntotal *= 10.0;

	outputfile.open(s.c_str());
	outputfile.precision(5);

	if (relcomp == 1) {

		label.str("");
		label.clear();
		label << "dat/" << isotope << "_" << compfile << ".root";
		if (gSystem->AccessPathName(label.str().c_str(), kFileExists) == 0){
			rcomp->OpenCompFile(label.str(), "READ");
		}


		if (compfile_new.size() > 0) {
			label.str("");
			label.clear();
			label << "dat/" << isotope << "_" << compfile_new << ".root";
			rcomp_new->CreateNewCompFile(label.str());
			rcomp_new->OpenCompFile(label.str(), "UPDATE");
		}

	}


	for(int i=0;i<betadecays.size();i++){
		betadecays[i]->SetNumberOfTotalDecays(NDecaysTotal);
		outputfile << betadecays[i]->GetSimulatedDecayEnergyToken();
		outputfile << betadecays[i]->GetLiteratureDecayEnergyToken();
		outputfile << betadecays[i]->GetSimulatedDecayIntensityToken();
		outputfile << betadecays[i]->GetLiteratureDecayIntensityToken();
		outputfile << betadecays[i]->GetEnergyDifferenceToken();
		outputfile << betadecays[i]->GetIntensityDifferenceToken();

		if(relcomp){
			outputfile << betadecays[i]->GetToken() << "_energy_old:XX\t";
			outputfile << rcomp->GetEntryValue(betadecays[i]->GetToken() + "_energy:XX") << "\n";
			outputfile << betadecays[i]->GetToken() << "_energy_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(betadecays[i]->GetSimulatedDecayEnergy(), betadecays[i]->GetSimulatedDecayEnergySigma(), rcomp->GetEntryValue(betadecays[i]->GetToken() + "_energy:XX"), rcomp->GetEntrySigma(betadecays[i]->GetToken() + "_energy:XX"),1.0);
			outputfile << betadecays[i]->GetToken() << "_intensity_old:XX\t";
			outputfile << rcomp->GetEntryValue(betadecays[i]->GetToken() + "_intensity:XX") << " $\\pm$ ";
			outputfile << rcomp->GetEntrySigma(betadecays[i]->GetToken() + "_intensity:XX") << "\n";
			outputfile << betadecays[i]->GetToken() << "_intensity_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(betadecays[i]->GetSimulatedDecayIntensity(), betadecays[i]->GetSimulatedDecayIntensitySigma(), rcomp->GetEntryValue(betadecays[i]->GetToken() + "_intensity:XX"), rcomp->GetEntrySigma(betadecays[i]->GetToken() + "_intensity:XX"),10.0);

			rcomp_new->WriteEntry(betadecays[i]->GetToken() + "_energy:XX", betadecays[i]->GetSimulatedDecayEnergy(),betadecays[i]->GetSimulatedDecayEnergySigma());
			rcomp_new->WriteEntry(betadecays[i]->GetToken() + "_intensity:XX", betadecays[i]->GetSimulatedDecayIntensity(), betadecays[i]->GetSimulatedDecayIntensitySigma());
		}

		NQualityEnergy += rcomp->GetAgreementLevel(betadecays[i]->GetSimulatedDecayEnergy(), betadecays[i]->GetSimulatedDecayEnergySigma(), betadecays[i]->GetLiteratureDecayEnergy(), betadecays[i]->GetLiteratureDecayEnergySigma(), 1.0);
		NQualityIntensity += rcomp->GetAgreementLevel(betadecays[i]->GetSimulatedDecayIntensity(), betadecays[i]->GetSimulatedDecayIntensitySigma(), betadecays[i]->GetLiteratureDecayIntensity(), betadecays[i]->GetLiteratureDecayIntensitySigma() ,10.0);
	}

	for(int i=0;i<alphadecays.size();i++){
		alphadecays[i]->SetNumberOfTotalDecays(NDecaysTotal);
		outputfile << alphadecays[i]->GetSimulatedDecayEnergyToken();
		outputfile << alphadecays[i]->GetLiteratureDecayEnergyToken();
		outputfile << alphadecays[i]->GetSimulatedDecayIntensityToken();
		outputfile << alphadecays[i]->GetLiteratureDecayIntensityToken();
		outputfile << alphadecays[i]->GetEnergyDifferenceToken();
		outputfile << alphadecays[i]->GetIntensityDifferenceToken();

		if(relcomp){
			outputfile << alphadecays[i]->GetToken() << "_energy_old:XX\t";
			outputfile << rcomp->GetEntryValue(alphadecays[i]->GetToken() + "_energy:XX") << "\n";
			outputfile << alphadecays[i]->GetToken() << "_energy_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(alphadecays[i]->GetSimulatedDecayEnergy(), alphadecays[i]->GetSimulatedDecayEnergySigma(), rcomp->GetEntryValue(alphadecays[i]->GetToken() + "_energy:XX"), rcomp->GetEntrySigma(alphadecays[i]->GetToken() + "_energy:XX"),1.0);
			outputfile << alphadecays[i]->GetToken() << "_intensity_old:XX\t";
			outputfile << rcomp->GetEntryValue(alphadecays[i]->GetToken() + "_intensity:XX") << " $\\pm$ ";
			outputfile << rcomp->GetEntrySigma(alphadecays[i]->GetToken() + "_intensity:XX") << "\n";
			outputfile << alphadecays[i]->GetToken() << "_intensity_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(alphadecays[i]->GetSimulatedDecayIntensity(), alphadecays[i]->GetSimulatedDecayIntensitySigma(), rcomp->GetEntryValue(alphadecays[i]->GetToken() + "_intensity:XX"), rcomp->GetEntrySigma(alphadecays[i]->GetToken() + "_intensity:XX"),10.0);
			rcomp_new->WriteEntry(alphadecays[i]->GetToken() + "_energy:XX", alphadecays[i]->GetSimulatedDecayEnergy(),alphadecays[i]->GetSimulatedDecayEnergySigma());
			rcomp_new->WriteEntry(alphadecays[i]->GetToken() + "_intensity:XX", alphadecays[i]->GetSimulatedDecayIntensity(), alphadecays[i]->GetSimulatedDecayIntensitySigma());
		}

		NQualityEnergy += rcomp->GetAgreementLevel(alphadecays[i]->GetSimulatedDecayEnergy(), alphadecays[i]->GetSimulatedDecayEnergySigma(), alphadecays[i]->GetLiteratureDecayEnergy(), alphadecays[i]->GetLiteratureDecayEnergySigma(), 1.0);
		NQualityIntensity += rcomp->GetAgreementLevel(alphadecays[i]->GetSimulatedDecayIntensity(), alphadecays[i]->GetSimulatedDecayIntensitySigma(), alphadecays[i]->GetLiteratureDecayIntensity(), alphadecays[i]->GetLiteratureDecayIntensitySigma() ,10.0);

	}

	for(int i=0;i<gammadecays.size();i++){
		gammadecays[i]->SetNumberOfTotalDecays(NDecaysTotal);
		outputfile << gammadecays[i]->GetSimulatedDecayEnergyToken();
		outputfile << gammadecays[i]->GetLiteratureDecayEnergyToken();
		outputfile << gammadecays[i]->GetSimulatedDecayIntensityToken();
		outputfile << gammadecays[i]->GetLiteratureDecayIntensityToken();
		outputfile << gammadecays[i]->GetEnergyDifferenceToken();
		outputfile << gammadecays[i]->GetIntensityDifferenceToken();

		if(relcomp){
			outputfile << gammadecays[i]->GetToken() << "_energy_old:XX\t";
			outputfile << rcomp->GetEntryValue(gammadecays[i]->GetToken() + "_energy:XX") << "\n";
			outputfile << gammadecays[i]->GetToken() << "_energy_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(gammadecays[i]->GetSimulatedDecayEnergy(), gammadecays[i]->GetSimulatedDecayEnergySigma(), rcomp->GetEntryValue(gammadecays[i]->GetToken() + "_energy:XX"), rcomp->GetEntrySigma(gammadecays[i]->GetToken() + "_energy:XX"),1.0);
			outputfile << gammadecays[i]->GetToken() << "_intensity_old:XX\t";
			outputfile << rcomp->GetEntryValue(gammadecays[i]->GetToken() + "_intensity:XX") << " $\\pm$ ";
			outputfile << rcomp->GetEntrySigma(gammadecays[i]->GetToken() + "_intensity:XX") << "\n";
			outputfile << gammadecays[i]->GetToken() << "_intensity_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(gammadecays[i]->GetSimulatedDecayIntensity(), gammadecays[i]->GetSimulatedDecayIntensitySigma(), rcomp->GetEntryValue(gammadecays[i]->GetToken() + "_intensity:XX"), rcomp->GetEntrySigma(gammadecays[i]->GetToken() + "_intensity:XX"),10.0);
			rcomp_new->WriteEntry(gammadecays[i]->GetToken() + "_energy:XX", gammadecays[i]->GetSimulatedDecayEnergy(),gammadecays[i]->GetSimulatedDecayEnergySigma());
			rcomp_new->WriteEntry(gammadecays[i]->GetToken() + "_intensity:XX", gammadecays[i]->GetSimulatedDecayIntensity(), gammadecays[i]->GetSimulatedDecayIntensitySigma());
		}

		NQualityEnergy += rcomp->GetAgreementLevel(gammadecays[i]->GetSimulatedDecayEnergy(), gammadecays[i]->GetSimulatedDecayEnergySigma(), gammadecays[i]->GetLiteratureDecayEnergy(), gammadecays[i]->GetLiteratureDecayEnergySigma(), 1.0);
		NQualityIntensity += rcomp->GetAgreementLevel(gammadecays[i]->GetSimulatedDecayIntensity(), gammadecays[i]->GetSimulatedDecayIntensitySigma(), gammadecays[i]->GetLiteratureDecayIntensity(), gammadecays[i]->GetLiteratureDecayIntensitySigma() ,10.0);

		//gammadecays[i]->DrawHistogram();
	}

	for(int i=0;i<secondaryelectrons.size();i++){
		secondaryelectrons[i]->SetNumberOfTotalDecays(NDecaysTotal);
		outputfile << secondaryelectrons[i]->GetSimulatedDecayEnergyToken();
		outputfile << secondaryelectrons[i]->GetLiteratureDecayEnergyToken();
		outputfile << secondaryelectrons[i]->GetSimulatedDecayIntensityToken();
		outputfile << secondaryelectrons[i]->GetLiteratureDecayIntensityToken();
		outputfile << secondaryelectrons[i]->GetEnergyDifferenceToken();
		outputfile << secondaryelectrons[i]->GetIntensityDifferenceToken();

		if(relcomp){
			outputfile << secondaryelectrons[i]->GetToken() << "_energy_old:XX\t";
			outputfile << rcomp->GetEntryValue(secondaryelectrons[i]->GetToken() + "_energy:XX") << "\n";
			outputfile << secondaryelectrons[i]->GetToken() << "_energy_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(secondaryelectrons[i]->GetSimulatedDecayEnergy(), secondaryelectrons[i]->GetSimulatedDecayEnergySigma(), rcomp->GetEntryValue(secondaryelectrons[i]->GetToken() + "_energy:XX"), rcomp->GetEntrySigma(secondaryelectrons[i]->GetToken() + "_energy:XX"),1.0);
			outputfile << secondaryelectrons[i]->GetToken() << "_intensity_old:XX\t";
			outputfile << rcomp->GetEntryValue(secondaryelectrons[i]->GetToken() + "_intensity:XX") << " $\\pm$ ";
			outputfile << rcomp->GetEntrySigma(secondaryelectrons[i]->GetToken() + "_intensity:XX") << "\n";
			outputfile << secondaryelectrons[i]->GetToken() << "_intensity_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(secondaryelectrons[i]->GetSimulatedDecayIntensity(), secondaryelectrons[i]->GetSimulatedDecayIntensitySigma(), rcomp->GetEntryValue(secondaryelectrons[i]->GetToken() + "_intensity:XX"), rcomp->GetEntrySigma(secondaryelectrons[i]->GetToken() + "_intensity:XX"),10.0);
			rcomp_new->WriteEntry(secondaryelectrons[i]->GetToken() + "_energy:XX", secondaryelectrons[i]->GetSimulatedDecayEnergy(),secondaryelectrons[i]->GetSimulatedDecayEnergySigma());
			rcomp_new->WriteEntry(secondaryelectrons[i]->GetToken() + "_intensity:XX", secondaryelectrons[i]->GetSimulatedDecayIntensity(), secondaryelectrons[i]->GetSimulatedDecayIntensitySigma());
		}

		NQualityEnergy += rcomp->GetAgreementLevel(secondaryelectrons[i]->GetSimulatedDecayEnergy(), secondaryelectrons[i]->GetSimulatedDecayEnergySigma(), secondaryelectrons[i]->GetLiteratureDecayEnergy(), secondaryelectrons[i]->GetLiteratureDecayEnergySigma(), 1.0);
		NQualityIntensity += rcomp->GetAgreementLevel(secondaryelectrons[i]->GetSimulatedDecayIntensity(), secondaryelectrons[i]->GetSimulatedDecayIntensitySigma(), secondaryelectrons[i]->GetLiteratureDecayIntensity(), secondaryelectrons[i]->GetLiteratureDecayIntensitySigma() ,10.0);
	}

	outputfile << "XX:" << isotope << "_spectrum:XX\tRadioactiveDecays/pics/" << isotope << "_spectrum.pdf\n";
	if(betaenergyspectrum != 0)
		outputfile << "XX:" << isotope << "_betaspectrum:XX\tRadioactiveDecays/pics/" << isotope << "_betaspectrum.pdf\n";


	outputfile << "XX:" << isotope << "_NumberOfSimulatedDecays:XX\t" << NDecaysTotal << "\n";

	if(relcomp == 1){
		rcomp_new->CloseCompFile();
		rcomp->CloseCompFile();
	}

//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	TLegend *leg = new TLegend(0.8,0.99,0.99,0.81);

	hisbeta->SetLineColor(kBlue);
	hisbeta->SetXTitle("energy [keV]");
	hisbeta->SetYTitle("counts /1 keV");
	//hisbeta->SetAxisRange(1.0,NDecaysTotal);
	hisbeta->GetYaxis()->SetRangeUser(0.5,2*NDecaysTotal);
	hisbeta->SetTitle(s.c_str());
	hisbeta->SetLineWidth(2);
	leg->AddEntry(hisbeta,"#beta-decays","l");
	hisbeta->Draw();

	hissecondary->SetLineColor(kAzure+10);
	hissecondary->SetLineWidth(2);
	leg->AddEntry(hissecondary,"secondary electrons","l");
	hissecondary->Draw("SAME");

	hisalpha->SetLineColor(kRed);
	hisalpha->SetLineWidth(2);
	leg->AddEntry(hisalpha,"#alpha-decays","l");
	hisalpha->Draw("SAME");

	hisgamma->SetLineColor(kOrange-3);
	hisgamma->SetLineWidth(2);
	leg->AddEntry(hisgamma,"#gamma-transitions","l");
	hisgamma->Draw("SAME");

	leg->Draw();
	can->SetLogy();



	gSystem->ProcessEvents();
	string s2=isotope;
	label.str("");
	label.clear();
	label << "pics/" << isotope << "_spectrum.pdf";
	s = label.str();

	can->SaveAs(s.c_str());
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

//-----------------------------------------------------------------------------


//Scales the range of the time histogram to account for all the data
	histime = new TH1F("histime","Decay Time",200,0.0,maxtime/10e8);

	//Fill the time histogram with data from the time vector
	cout << maxtime << "\n";
	for (int i = 0; i < timevector.size(); i++) {
		histime->Fill(timevector[i]);
	}
	histime->SetXTitle("time [s]");
	histime->SetYTitle("counts");
	histime->SetLineColor(kRed);
	histime->SetLineWidth(2.0);
	//Plots and fits the time histogram, find half-life from fit parameter & ln(2)
	TCanvas *can3 = new TCanvas("can3", "DecayAnalysis", 1100, 500);
	can3->SetGrid();
	can3->SetLogy();
	histime->Draw();
	histime->Fit("expo","Q");
	TF1 *fit = histime->GetFunction("expo");
	double halflife = -.69314718056 / (fit->GetParameter(1));
	cout << "half life is: " << halflife << " seconds\n";
	cout << "or " << halflife / (3600 * 24) << " days\n";

	label.str("");
	label.clear();
	label << "pics/" << s2 << "_decaytime.pdf";
	s = label.str();
	can3->SaveAs(s.c_str());
	//Put half-life data into parameter file
	double difference = (halflife - halflifelit) / halflifelit;
	if (difference < 0) {
		difference = difference * -1;
	}
	string timequality;
	if (difference * 100 < 5) {
		timequality = "\\colorbox{OliveGreen}{\\color{White} ";
	} else if (difference * 100 <= 20) {
		timequality = "\\colorbox{BurntOrange}{\\color{White} ";
	} else {
		timequality = "\\colorbox{Red}{\\color{White} ";
	}
	outputfile << "XX:" << s2 << "_halflife:XX\t" << halflife << "\n";
	outputfile << "XX:" << s2 << "_halflife_literature:XX\t" << halflifelit << "\n";
	outputfile << "XX:" << s2 << "_halflife_difference:XX\t" << timequality << "\t" << 100 * difference << "}\n";
	outputfile << "XX:" << s2 << "_decaytime:XX\tRadioactiveDecays/pics/" << s2 << "_decaytime.pdf\n";

//-----------------------------------------------------------------------------
	outputfile.close();
//	rcomp_new->OpenCompFile(compfile_new,"READ");
//	rcomp_new->PrintContent();
//	rcomp_new->CloseCompFile();

	outputfile.open("../globalparameter.txt",fstream::app);

	outputfile << "XX:" << s2 << "_EnergyQuality:XX\t";
	NQualityEnergy = (Ntotal - NQualityEnergy)/Ntotal*100.0;
	if(NQualityEnergy > 95.0){
		outputfile << "\\colorbox{OliveGreen}{\\color{White} ";
	} else if(NQualityEnergy >= 80.0){
		outputfile << "\\colorbox{BurntOrange}{\\color{White} ";
	} else{
		outputfile << "\\colorbox{Red}{\\color{White} ";
	}
	outputfile.precision(3);
	outputfile << NQualityEnergy << "}\n";

	outputfile << "XX:" << s2 << "_IntensityQuality:XX\t";
	NQualityIntensity = (Ntotal - NQualityIntensity)/Ntotal*100.0;
	if(NQualityIntensity > 95.0){
		outputfile << "\\colorbox{OliveGreen}{\\color{White} ";
	} else if(NQualityIntensity >= 80.0){
		outputfile << "\\colorbox{BurntOrange}{\\color{White} ";
	} else{
		outputfile << "\\colorbox{Red}{\\color{White} ";
	}
	outputfile.precision(3);
	outputfile << NQualityIntensity << "}\n";
	outputfile.close();


/*
	TGraph *gr = new TGraph(1000);
	for(int i=0;i<1000;i++){
		gr->SetPoint(i,(double)i/100.0,betaenergyspectrum->FermiFunctionInterpolation((double(i)/100.0)));
	}
*/

	if(betaenergyspectrum != 0){
		TCanvas *can2 = new TCanvas("can2","DecayAnalysis",1100,500);
		can2->SetGrid();
		betaenergyspectrum->CreateKuriePlot();
		betaenergyspectrum->CreateBetaSpectrumFit();
		betaenergyspectrum->DrawSpectrum();
		label.str("");
		label.clear();
		label << "pics/" << isotope << "_betaspectrum.pdf";
		s = label.str();
		gSystem->ProcessEvents();
		can2->SaveAs(s.c_str());

		//betaenergyspectrum->DrawKuriePlot();
		//cout << betaenergyspectrum->GetFitE0() << " +/- " << betaenergyspectrum->GetFitE0Sigma() << "\n";
		//cout << betaenergyspectrum->GetFitConstant() << " +/- " << betaenergyspectrum->GetFitConstantSigma() << "\n";
	}

//*/
//	gr->Draw("AP");

	//theApp.Run();
	return 0;
}
