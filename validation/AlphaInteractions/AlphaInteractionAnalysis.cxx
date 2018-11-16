// C++ headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

// CERN ROOT headers
#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TLegend.h>
//#include <TLegend.h>

// MaGe Validation headers
#include "../VaLi/ParticleTrack.h"
#include "MaterialInteraction.h"
#include "../VaLi/RComp.h"
#include "../../io/io/MGOutputG4StepsData.hh"

using namespace std;

int main(int argc, char* argv[]) {

	string compfile = "";
	string compfile_new = "";
	int relcomp = 0;

	if (argc < 2) {
		std::cout << "Usage: AlphaInteractionAnalysis <element name> \n";
		return 0;
	}

	if(argc == 3){
		relcomp = 1;
		compfile = argv[2];
	}

	if(argc == 4){
		relcomp = 1;
		compfile = argv[2];
		compfile_new = argv[3];
	}

	string elementName(argv[1]);

	TCanvas* can;
	TApplication theApp("App", &argc, argv);
	TFile *runfile;

	stringstream label;
	ifstream inputfile;
	ofstream outputfile;

	// Relative Comparison
	RComp *rcomp = new RComp();
	RComp *rcomp_new = new RComp();

	// Adjusting ROOT style
	gROOT->SetBatch(true);
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	UInt_t runID, eventID, trackID, parentID;
	Int_t PID;
	Double_t positionX, positionY, positionZ, energyKinetic, energyDeposit;

	can = new TCanvas("can","InteractionAnalysis",1100,500);
	can->SetGrid();
	can->SetLogy();
	can->Connect("Closed()", "TApplication", &theApp, "Terminate()");

	string s;
	string* ps;

	vector<double> energy_literature;
	vector<double> attenuation_literature;
	vector<double> energy_simulation;
	vector<double> attenuation_simulation;
	vector<string> token;
	double density,temp;
	int dataType;
	size_t endposition;
	istringstream conv;

	string filename = "dat/" + elementName + ".dat";
	inputfile.open(filename.c_str());
	if (inputfile.fail()) {
		cout << "ERROR! Can't open file \"" << filename << "\"\n";
	}

	while (getline(inputfile, s)) {

		ps = new string[2];

		endposition = s.find(":XX", 0);
		if(s[0]=='#')
			continue;
		if(endposition==string::npos)
			continue;

		ps[0] = s.substr(0, endposition + 3);
		ps[1] = s.substr(endposition + 3);

		conv.str(ps[1]);
		conv >> dataType;

		switch(dataType){
		case 10:
			conv >> density;
			cout << "density:\t" << density << "\n";
			break;
		case 13:
			conv >> temp;
			energy_literature.push_back(temp);
			conv >> temp;
			attenuation_literature.push_back(temp / density);
			token.push_back(ps[0]);
			break;
		}
		conv.str("");
		conv.clear();
	}
	inputfile.close();

	s = elementName + "parameter.txt";
	cout << "generating file " << s << "\n";
	outputfile.open(s.c_str());

	inputfile.open("dat/alphaEnergies.txt");
	if (inputfile.fail()) {
		cout << "ERROR! File alphaEnergies.txt is missing. AlphaInteractionAnalysis terminated\n";
		return 0;
	}

	while (getline(inputfile, s)) {

		conv.str(s);
		conv >> temp;
		conv.str("");
		conv.clear();
		energy_simulation.push_back(temp);

		filename = "dat/" + elementName + "_" + s + "MeV.root";
		cout << filename << "\n";
		runfile = new TFile(filename.c_str());
		if(runfile->IsZombie()){
			cout << "trouble!\n";
			return 0;
		}

		MaterialInteraction *newInteraction = new MaterialInteraction();
		newInteraction->SetToken("XX:" + elementName + "_CSDA_alpha" + s + "MeV:XX");
		newInteraction->SetEnergy(temp);
		for(int i=0;i<energy_literature.size();i++){
			if(temp == energy_literature[i]){
				newInteraction->SetAttenuationLiterature(attenuation_literature[i]);
				break;
			}
		}

		MGOutputG4StepsData *dat = new MGOutputG4StepsData();
		TTree *tree = (TTree*) runfile->Get("fTree");
		TBranch *branch = tree->GetBranch("RawMC");

		branch->SetAddress(&dat);

		cout << "number of entries: " << tree->GetEntries() << "\n";

		for(int i=0;i<tree->GetEntries();i++){
			tree->GetEntry(i);
			newInteraction->AddInteractionNode(i, 0.0, 0.0, 0.0, temp, 0.0);
			for(int j=0;j<dat->fTotalNumberOfSteps;j++){
				if(dat->fTrackID[j]==1){
					//cout << dat->fX[j] << "\t" << dat->fY[j] << "\t" << dat->fZ[j] << "\n";
					newInteraction->AddInteractionNode(i,dat->fX[j], dat->fY[j], dat->fZ[j],0.0,dat->fEdep[j]);
				}
			}
		}

		outputfile << newInteraction->GetEnergyToken() << "\n";
		outputfile << newInteraction->GetAttenuationLiteratureToken() << "\n";
		outputfile << newInteraction->GetAverageTotalRangeToken() << "\n";
		outputfile << newInteraction->GetAttenuationDifferenceToken();

		if(newInteraction->GetEnergy() == 2.0){
			newInteraction->PlotHistogramEndZ();

			std::stringstream label;
			label.str("");
			label.clear();
			label << "pics/" << elementName << "_alpha" << newInteraction->GetEnergy() << "MeV.pdf";
			can->SaveAs(label.str().c_str());
			outputfile << "XX:" << elementName << "_alpha" << newInteraction->GetEnergy() << "MeV_plot:XX\t";
			outputfile << "AlphaInteractions/" << label.str() << "\n";
			gSystem->ProcessEvents();
		}

		attenuation_simulation.push_back(newInteraction->GetAverageTotalRange());

		runfile->Close();
		delete runfile;
		delete dat;
		delete newInteraction;
	}
	inputfile.close();

	if (relcomp == 1) {
		label.str("");
		label.clear();
		label << "dat/" << elementName << "_" << compfile << ".root";
		if (gSystem->AccessPathName(label.str().c_str(), kFileExists) == 0)
			rcomp->OpenCompFile(label.str(), "READ");

		if (compfile_new.size() > 0) {
			label.str("");
			label.clear();
			label << "dat/" << elementName << "_" << compfile_new << ".root";
			rcomp_new->CreateNewCompFile(label.str());
			rcomp_new->OpenCompFile(label.str(), "UPDATE");
			cout << "opening file " << label.str() << "\n";
		}

	}

	TLegend *leg = new TLegend(0.75,0.99,0.99,0.86);
	TGraphErrors *gr_sim = new TGraphErrors(attenuation_simulation.size());
	TGraphErrors *gr_comp = new TGraphErrors(attenuation_simulation.size());
	for(int i=0;i<attenuation_simulation.size();i++){

		gr_sim->SetPoint(i,energy_simulation[i],attenuation_simulation[i]);
		gr_sim->SetPointError(i,0.0,0.0);

		if(relcomp){
			s = token[i];
			outputfile << s.substr(0,s.size()-3) << "_attenuation_simulation_old:XX\t";
			outputfile << rcomp->GetEntryValue(s.substr(0,s.size()-3) + "_attenuation_simulation:XX") << "\n";
			outputfile << s.substr(0,s.size()-3) << "_attenuation_reldiff:XX\t";
			outputfile << rcomp->GetAgreementColourString(attenuation_simulation[i],0.0,rcomp->GetEntryValue(s.substr(0,s.size()-3) + "_attenuation_simulation:XX"),rcomp->GetEntryValue(s.substr(0,s.size()-3) + "_attenuation_simulation_sigma:XX"),1.0 );
			rcomp_new->WriteEntry(s.substr(0,s.size()-3) + "_attenuation_simulation:XX",attenuation_simulation[i],0.0);

			gr_comp->SetPoint(i,energy_simulation[i], rcomp->GetEntryValue(s.substr(0,s.size()-3) + "_attenuation_simulation:XX"));
			gr_comp->SetPointError(i,0.0,rcomp->GetEntrySigma(s.substr(0,s.size()-3) + "_attenuation_simulation:XX"));
		}

	}

	can->SetLogx();

	gr_sim->SetMarkerColor(kRed);
	gr_sim->SetTitle(elementName.c_str());
	gr_sim->SetMarkerStyle(21);
	gr_sim->GetXaxis()->SetTitle("alpha energy [MeV]");
	gr_sim->GetYaxis()->SetTitle("range [cm]");
	leg->AddEntry(gr_sim,"simulated values","p");
	can->Update();
	gr_sim->Draw("AP");

	if(relcomp){
		gr_comp->SetMarkerStyle(23);
		gr_comp->SetMarkerColor(kGray + 1);
		s = "previous values (" + compfile + ")";
		leg->AddEntry(gr_comp,s.c_str(),"p");
		gr_comp->Draw("P");
	}


	TGraph *gr_lit = new TGraph(energy_literature.size());
	for(int i=0;i<energy_literature.size();i++)
		gr_lit->SetPoint(i,energy_literature[i],attenuation_literature[i]);
	gr_lit->SetMarkerColor(kBlack);
	gr_lit->SetMarkerStyle(24);
	leg->AddEntry(gr_lit,"literature values","p");
	gr_lit->Draw("P");
	leg->Draw();

	s = "pics/" + elementName + "_plot.pdf";
	can->SaveAs(s.c_str());

	outputfile << "XX:" << elementName << "_plot:XX\t";
	outputfile << "AlphaInteractions/pics/" << elementName << "_plot.pdf";

	outputfile.close();
	//theApp.Run();

	if(relcomp == 1){
		rcomp_new->CloseCompFile();
		rcomp->CloseCompFile();
	}

	return 0;
}
