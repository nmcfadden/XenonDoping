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

#include <TGraph2D.h>
//#include <TLegend.h>

// MaGe Validation headers
#include "../VaLi/ParticleTrack.h"
#include "MaterialInteraction.h"
#include "MedianOmatic.h"
#include "../VaLi/RComp.h"
//#include "FTrack.h"

#include "../../io/io/MGOutputG4StepsData.hh"

using namespace std;

int main(int argc, char* argv[]) {

	string compfile = "";
	string compfile_new = "";
	int relcomp = 0;

	if (argc < 2) {
		std::cout << "Usage: NeutronInteractionAnalysis <element name> \n";
		return 0;
	}

	if (argc == 3) {
		relcomp = 1;
		compfile = argv[2];
	}

	if (argc == 4) {
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

	can = new TCanvas("can", "InteractionAnalysis", 1100, 500);
	can->SetGrid();
	can->SetLogy();
	can->Connect("Closed()", "TApplication", &theApp, "Terminate()");

	string s;
	string* ps;
	size_t endposition;
	istringstream conv;
	double temp, density, weight;
	int dataType;

	MedianOmatic *median[4];
	for (int i = 0; i < 4; i++)
		median[i] = new MedianOmatic();
	vector<double> energy_simulation[4], neutronXsection[4];
	vector<double> energy_literature[4], neutronXsection_literature[4];

	const string HadronElastic = "HadronElastic";
	const string nInelastic = "nInelastic";
	const string nCapture = "nCapture";
	const Double_t pi = 3.14159265358979323846264338327;



//	vector<FTrack*> mtrack;
//	vector<TGraph*> mtrackgr;



	inputfile.open("dat/neutronEnergies.txt");
	if (inputfile.fail()) {
		cout << "ERROR! File neutronEnergies.txt is missing. NeutronInteractionAnalysis terminated\n";
		return 0;
	}

	vector<double> ene;
	vector<double> eneDiff;

	vector<vector<double> > diffXSectionAngle;
	vector<vector<double> > diffXSection;

	vector<string> eneString;
	double energySet = 0;
	int energyIndex = 0;

	while (getline(inputfile, s)) {
		conv.str(s);
		conv >> temp;
		conv.str("");
		conv.clear();
		ene.push_back(temp);
		eneString.push_back(s);
		for (int i = 0; i < 4; i++) {
			median[i]->AddBinLimit(temp);
		}
	}
	inputfile.close();

	double etemp, xtemp;

	string filename = "dat/" + elementName + ".dat";
	inputfile.open(filename.c_str());
	if (inputfile.fail()) {
		cout << "ERROR! Can't open file \"" << filename << "\"\n";
	}

	while (getline(inputfile, s)) {

		ps = new string[2];

		endposition = s.find(":XX", 0);
		if (s[0] == '#')
			continue;
		if (endposition == string::npos)
			continue;

		ps[0] = s.substr(0, endposition + 3);
		ps[1] = s.substr(endposition + 3);

		conv.str(ps[1]);
		conv >> dataType;

		switch (dataType) {
		case 10:
			conv >> density;
			break;
		case 101:
			conv >> weight;
			break;
		case 23:
			conv >> temp;
			energy_literature[3].push_back(temp);
			conv >> temp;
			neutronXsection_literature[3].push_back(temp);
			break;

		case 200:
			while (getline(inputfile, s)) {
				if(s[0] == '#'){
					for(int i=1;i<ene.size();i++){
						//cout << "average cross section (" << ene[i-1] << " to " << ene[i] << "MeV): " << median[0]->GetLowerMedian(ene[i]) << "b\n";
						energy_literature[0].push_back(ene[i-1]);
						neutronXsection_literature[0].push_back(median[0]->GetLowerMedian(ene[i]));
					}
					break;
				}
				conv.str("");
				conv.clear();
				conv.str(s);
				conv >> etemp;
				etemp/=1e6;
				conv >> xtemp;
				median[0]->AddValues(etemp, xtemp);
			}
			break;

		case 201:
			while (getline(inputfile, s)) {
				if(s[0] == '#'){
					for(int i=1;i<ene.size();i++){
						//cout << "average cross section (" << ene[i-1] << " to " << ene[i] << "MeV): " << median[1]->GetLowerMedian(ene[i]) << "b\n";
						energy_literature[1].push_back(ene[i-1]);
						neutronXsection_literature[1].push_back(median[1]->GetLowerMedian(ene[i]));
					}
					break;
				}
				conv.str("");
				conv.clear();
				conv.str(s);
				conv >> etemp;
				etemp/=1e6;
				conv >> xtemp;
				median[1]->AddValues(etemp, xtemp);
			}
			break;

		case 202:
			while (getline(inputfile, s)) {
				if(s[0] == '#'){
					for(int i=1;i<ene.size();i++){
						//cout << "average cross section (" << ene[i-1] << " to " << ene[i] << "MeV): " << median[2]->GetLowerMedian(ene[i]) << "b\n";
						energy_literature[2].push_back(ene[i-1]);
						neutronXsection_literature[2].push_back(median[2]->GetLowerMedian(ene[i]));
					}
					break;
				}
				conv.str("");
				conv.clear();
				conv.str(s);
				conv >> etemp;
				etemp /= 1e6;
				conv >> xtemp;
				median[2]->AddValues(etemp, xtemp);
			}
			break;
		case 210:
			while (getline(inputfile, s)) {
				if(s == "# start data set"){
					getline(inputfile, s);
					conv.str("");
					conv.clear();
					conv.str(s);
					conv >> etemp;
					eneDiff.push_back(etemp);
					vector<double> alpha;
					vector<double> xsection;
					while (getline(inputfile, s)) {
						if(s == "# end data set"){
							diffXSectionAngle.push_back(alpha);
							diffXSection.push_back(xsection);
							break;
						}
						conv.str("");
						conv.clear();
						conv.str(s);
						conv >> etemp;
						alpha.push_back(etemp);
						conv >> etemp;
						xsection.push_back(etemp);

					}

				}
				if(s == "# end of data")
					break;
			}
			break;
		}

		conv.str("");
		conv.clear();
	}
	inputfile.close();

	// *************** TEMPORARY ****************

	vector<TGraph*> tgr;

	for(int q=0;q<eneDiff.size();q++){
		cout << "diff X energies literature:\t" << eneDiff[q] << "\t" << diffXSection[q].size() << "\n";

		TGraph *xgr = new TGraph(diffXSection[q].size());
		tgr.push_back(xgr);

		for(int r=0; r<diffXSection[q].size(); r++){

			//cout << diffXSectionAngle[q][r] << "\t" << diffXSection[q][r] << "\n";
			tgr[q]->SetPoint(r,diffXSectionAngle[q][r],diffXSection[q][r]);
		}
		if(q==0){
		//	tgr[0]->Draw("APL");
		}
		else{
		//	tgr[q]->Draw("PL");
		}
	}

//	theApp.Run();
//	return 0;

	// ******************************************


	s = elementName + "parameter.txt";
	//cout << "opening file " << s << "\n";
	outputfile.open(s.c_str());


	// ************ DEBUG ***************//
	vector<double> nx, ny, nz;
	int HaE, nIn, nCa;
	int InteractionType = 0;
	int xcount[4] = {0};
	// **********************************//

	s = "dat/" + elementName + "_datnames.txt";
	//cout << "opening file " << s << "\n";
	inputfile.open(s.c_str());

// ****************************************//

	vector<double> datEnergies;
	vector<string> datNames;

	while (getline(inputfile, s)) {
		conv.str(s);
		conv >> temp;
		datEnergies.push_back(temp);
		conv >> filename;
		datNames.push_back(filename);
		conv.str("");
		conv.clear();
	}
	inputfile.close();

	for(int x=0;x<datEnergies.size();x++){

		MaterialInteraction *newInteraction[4];

		filename = datNames[x];
		temp = datEnergies[x];

		runfile = new TFile(filename.c_str());
		if (runfile->IsZombie()) {
			cout << "trouble!\n";
			return 0;
		}

		if(ene[energyIndex] == temp){

			temp = ene[energyIndex];

			energy_simulation[0].push_back(ene[energyIndex]);

			for (int i = 0; i < 4; i++) {
				newInteraction[i] = new MaterialInteraction();
				newInteraction[i]->SetEnergy(temp);
				xcount[i] = 0;
			}

			label.str("");
			label.clear();
			label << "XX:" << elementName << "_neutron_total_cross-section_" << eneString[energyIndex] <<  "MeV:XX";
			newInteraction[0]->SetToken(label.str());

			label.str("");
			label.clear();
			label << "XX:" << elementName << "_neutron_elastic_cross-section_" << eneString[energyIndex] <<  "MeV:XX";
			newInteraction[1]->SetToken(label.str());

			label.str("");
			label.clear();
			label << "XX:" << elementName << "_neutron_inelastic_cross-section_" << eneString[energyIndex] <<  "MeV:XX";
			newInteraction[2]->SetToken(label.str());

			label.str("");
			label.clear();
			label << "XX:" << elementName << "_neutron_capture_cross-section_" << eneString[energyIndex] <<  "MeV:XX";
			newInteraction[3]->SetToken(label.str());

			for(int k=0;k<4;k++){
				if(energy_literature[k].size()>0){
					for(int i=0;i<energy_literature[k].size();i++){
						if(temp == energy_literature[k][i]){
							newInteraction[k]->SetXsectionLiterature(neutronXsection_literature[k][i]);
							break;
						}
					}
				}
			}

		}

		MGOutputG4StepsData *dat = new MGOutputG4StepsData();
		TTree *tree = (TTree*) runfile->Get("fTree");
		TBranch *branch = tree->GetBranch("RawMC");

		branch->SetAddress(&dat);

// --->
		//cout << "number of entries: " << tree->GetEntries() << "\n";

		nx.clear();
		ny.clear();
		nz.clear();

		HaE = 0;
		nIn = 0;
		nCa = 0;

		for (int i = 0; i < tree->GetEntries(); i++) {
			tree->GetEntry(i);

			/*/ ************** TRACK DEBUG INFORMATION ****************** //

			int maxtracks = 0;

			for(int j=0;j<dat->fTotalNumberOfSteps;j++){
				if(dat->fTrackID[j]>maxtracks)
					maxtracks = dat->fTrackID[j];
			}

			if(x==14 && i==6){
				for(int j=0;j<=maxtracks;j++){
					//FTrack *newFTrack = new FTrack();
					mtrack.push_back(new FTrack());
				}
			}

			// ********************************************************* /*/

			int x = 0;

			for (int j = 0; j < dat->fTotalNumberOfSteps; j++) {

				if (j == 0) {
					x = 0;
					s.clear();
					do {
						if (dat->fG4Process[x] == 0 || x >= dat->fProcessNamePos) {
							x++;
							break;
						}
						s += dat->fG4Process[x];
						x++;
					} while (1);

					if (s == HadronElastic) {
						InteractionType = 1;
						xcount[InteractionType]++;
					}
					if (s == nInelastic) {
						InteractionType = 2;
						xcount[InteractionType]++;
					}
					if (s == nCapture) {
						InteractionType = 3;
						xcount[InteractionType]++;
					}

					xcount[0]++;

					newInteraction[0]->AddInteractionNode(xcount[0]-1, 0.0, 0.0, 0.0,temp, 0.0);
					newInteraction[InteractionType]->AddInteractionNode(xcount[InteractionType]-1, 0.0,0.0, 0.0, temp, 0.0);


					if (0) {
						double px, py, pz;
						double c = 1;
						double mN = 939.57e3; //neutron mass in keV

						px = dat->fPx[j];
						py = dat->fPy[j];
						pz = dat->fPz[j];

						double P = sqrt(px * px + py * py + pz * pz);
						double K = sqrt(P * P * c * c + (mN * c * c) * (mN * c * c)) - (mN * c * c);
						cout << "kinetic energy: " << K << "\t" << dat->fEdep[j] << "\t" << dat->fStatus01[j] << "\n";
					}

				}

				if (dat->fTrackID[j] == 1) {
					newInteraction[0]->AddInteractionNode(xcount[0]-1, dat->fX[j], dat->fY[j], dat->fZ[j], 0.0, dat->fEdep[j]);
					newInteraction[InteractionType]->AddInteractionNode(xcount[InteractionType]-1, dat->fX[j], dat->fY[j], dat->fZ[j], 0.0, dat->fEdep[j]);
				}

				/*/ ************** TRACK DEBUG INFORMATION ****************** //
				if(mtrack.size()>0){
					mtrack[dat->fTrackID[j]]->AddStep(dat->fX[j],dat->fY[j],dat->fZ[j],dat->fPx[j],dat->fPy[j],dat->fPz[j],dat->fParticleID[j],dat->fTrackID[j],dat->fPreviousTrackID[j],dat->fEdep[j], dat->fGlobalTime[j]);
				}

				// ********************************************************* /*/


			}
		}


		/*/ ************** TRACK DEBUG INFORMATION ****************** //

		if (mtrack.size()>0) {
			can->SetLogy(0);
			for (int j = 0; j < mtrack.size(); j++) {
				mtrack[j]->PrintTrackInformation();
				mtrackgr.push_back(new TGraph(mtrack[j]->GetNumberOfEntries()));

				//grx = new TGraph(mtrack[j]->GetNumberOfEntries());
				for (int k = 0; k < mtrack[j]->GetNumberOfEntries(); k++) {
					mtrackgr[j]->SetPoint(k, mtrack[j]->GetPositionZ(k), mtrack[j]->GetPositionY(k));

					//if (k > 0 && mtrack[j]->GetGlobalTime(k - 1) > mtrack[j]->GetGlobalTime(k)) {
					//	cout << "deal breaker!!!\n";
					//	break;
					//}
				}


				mtrackgr[j]->SetMarkerSize(0.5);

				if (j == 1) {
					mtrackgr[j]->SetMarkerStyle(21);
					mtrackgr[j]->SetMarkerColor(kBlack);
					mtrackgr[j]->SetLineColor(kRed);
					mtrackgr[j]->SetLineWidth(2);
					//mtrackgr[j]->GetXaxis()->SetRangeUser(-60.0,30.0);
					//mtrackgr[j]->GetYaxis()->SetRangeUser(-20.0,10.0);
					mtrackgr[j]->Draw("APL");
					label.str("");
					label.clear();
					label << "pics/track" << x << "_" << j << ".png";
					can->SaveAs(label.str().c_str());
				}

				if (j > 1 && mtrack[j]->GetParticleID() != 0/*&& mtrack[j]->GetPreviousTrackID() == 1 && mtrack[j]->GetParticleID() == 11 /) {

					cout << mtrack[j]->GetParticleID() << "\n";

					mtrackgr[j]->SetMarkerStyle(21);
					mtrackgr[j]->SetMarkerColor(kGray + 2);
					mtrackgr[j]->SetLineColor(kBlue);
					mtrackgr[j]->Draw("PL");
					label.str("");
					label.clear();
					label << "pics/track" << x << "_" << j << ".png";
					gSystem->ProcessEvents();
					can->SaveAs(label.str().c_str());
				}




			}
			theApp.Run();
			can->SetLogy(1);

			// force quit
			return 0;
		}


		// ********************************************************* /*/

		runfile->Close();

//		mtrack.clear();
		delete dat;
		delete runfile;



		int dc = 0;
		if(energyIndex+1 < ene.size() && x+1 < datEnergies.size()){
			if(ene[energyIndex+1] == datEnergies[x+1]){
				dc = 1;
			}
		}
		if(x+1 == datEnergies.size())
			dc = 2;

		if (dc) {

			cout << "*********** xXx *************\n";
			cout << "nTotal:\t" << newInteraction[0]->GetNumberOfTracks() << "\n";
			cout << HadronElastic << ":\t" << newInteraction[1]->GetNumberOfTracks() << "\n";
			cout << nInelastic << ":\t" << newInteraction[2]->GetNumberOfTracks() << "\n";
			cout << nCapture << ":\t" << newInteraction[3]->GetNumberOfTracks() << "\n";

			for (int k = 0; k < 4; k++) {
				outputfile << newInteraction[k]->GetEnergyToken() << "\n";
				outputfile << newInteraction[k]->GetXsectionLiteratureToken() << "\n";
			}

			neutronXsection[0].push_back(newInteraction[0]->GetXsectionSimulation(density,weight,newInteraction[0]->GetNumberOfTracks()));
			outputfile << "XX:" << elementName << "_neutron_total_cross-section_" << newInteraction[0]->GetEnergy() <<  "MeV_simulation:XX\t" << newInteraction[0]->GetXsectionSimulation(density,weight,newInteraction[0]->GetNumberOfTracks()) << "\n";
			outputfile << newInteraction[0]->GetXsectionDifferenceToken(density, weight, newInteraction[0]->GetNumberOfTracks());

			/*
			 newInteraction[0]->PlotHistogramEndZ();
			 label.str("");
			 label.clear();
			 label << "pics/" << elementName << "_total_" << ene[energyIndex] <<  "MeV.png";
			 can->SaveAs(label.str().c_str());
			//*/

			if (newInteraction[1]->GetNumberOfTracks() > 4) {
				temp = N_a * density / weight;
				temp = 1 / (temp * newInteraction[1]->GetMeanFreePath()) * 1e24;
				temp = temp * ((double) (newInteraction[1]->GetNumberOfTracks()) / (double) (newInteraction[0]->GetNumberOfTracks()));
				neutronXsection[1].push_back(temp);
				energy_simulation[1].push_back(newInteraction[1]->GetEnergy());
				//cout << "neutron elastic cross-section: " << temp << "b\t" << newInteraction[1]->GetXsectionSimulation(density, weight, newInteraction[0]->GetNumberOfTracks()) << "\n";

				outputfile << newInteraction[1]->GetXsectionSimulationToken(density, weight, newInteraction[0]->GetNumberOfTracks()) << "\n";
				outputfile << newInteraction[1]->GetXsectionDifferenceToken(density, weight, newInteraction[0]->GetNumberOfTracks());
			}

			if (newInteraction[2]->GetNumberOfTracks() > 4) {
				temp = N_a * density / weight;
				temp = 1 / (temp * newInteraction[2]->GetMeanFreePath()) * 1e24;
				temp = temp * ((double) (newInteraction[2]->GetNumberOfTracks()) / (double) (newInteraction[0]->GetNumberOfTracks()));
				neutronXsection[2].push_back(temp);
				energy_simulation[2].push_back(newInteraction[2]->GetEnergy());
				//cout << "neutron inelastic cross-section: " << temp << "b\n";

				outputfile << newInteraction[2]->GetXsectionSimulationToken(density, weight, newInteraction[0]->GetNumberOfTracks()) << "\n";
				outputfile << newInteraction[2]->GetXsectionDifferenceToken(density, weight, newInteraction[0]->GetNumberOfTracks());
			}

			if (newInteraction[3]->GetNumberOfTracks() > 4) {
				temp = N_a * density / weight;
				temp = 1 / (temp * newInteraction[3]->GetMeanFreePath()) * 1e24;
				temp = temp * ((double) (newInteraction[3]->GetNumberOfTracks()) / (double) (newInteraction[0]->GetNumberOfTracks()));
				neutronXsection[3].push_back(temp);
				energy_simulation[3].push_back(newInteraction[3]->GetEnergy());
				//cout << "neutron capture cross-section: " << temp << "b\n";

				outputfile << newInteraction[3]->GetXsectionSimulationToken(density, weight, newInteraction[0]->GetNumberOfTracks()) << "\n";
				outputfile << newInteraction[3]->GetXsectionDifferenceToken(density, weight, newInteraction[0]->GetNumberOfTracks());
			}

			// calculate scattering angle of first interaction
			TH1F *hisThetaFirst;
			TGraph *grDiffXsection;
			TGraph *grDiffXsectionLiterature;
			TLegend *leg = new TLegend(0.85,0.99,0.99,0.9);

			TGraph *grDiffXsectionLiterature2;

			const Int_t dAngle = 15;
			vector<Double_t> DiffXsection, DiffXsectionInterpolation;
			vector<Double_t> dOmega;
			Double_t dOmegaTemp;
			Double_t DiffXsectionMaximum = 0.0;
			Double_t DiffXsectionSum = 0.0;
			Double_t DiffXsectionDiff = 0.0;

			// calculate theta limits

			/*

			vector<double> thetaLimits;
			const double d = 0.1;

			thetaLimits.push_back(0.0);
			do{
				thetaLimits.push_back(acos(cos(thetaLimits.at(thetaLimits.size()-1)) - d));

			}while(d*thetaLimits.size()<=2.0);
			*/

			//for(int f = 0; f < thetaLimits.size(); f++){
			//	cout << f << "\t" << thetaLimits[f] << "\n";
			//}


			//hisThetaFirst = new TH1F("hisThetaFirst","first scatter Angle",(int)(2.0/d),0.0,4.0);
			hisThetaFirst = new TH1F("hisThetaFirst","first scatter Angle",180.0/dAngle,0.0,180.0);

			for (int f = 0; f < newInteraction[1]->GetNumberOfTracks(); f++) {
				//newInteraction[0]->TrackReport(f);
				//cout << "theta: " << newInteraction[0]->GetFirstScatterAngle(f) << "\n";


				//-> conversion from deg to srad
				/*
				for(int k = 1; k < thetaLimits.size(); k++){
					if(newInteraction[0]->GetFirstScatterAngle(f) < thetaLimits[k]){
						hisThetaFirst->Fill((k-0.5)*d*2.0);
						break;
					}
				}
				*/

				//only look for elastic scattering -> index 1
				hisThetaFirst->Fill(newInteraction[1]->GetFirstScatterAngle(f)/pi*180.0);

				//hisThetaFirst->Fill(newInteraction[0]->GetFirstScatterAngle(f));
			}

			for(int c=0; c<hisThetaFirst->GetNbinsX(); c++){
				//DiffXsection.push_back(hisThetaFirst->GetBinContent(c+1));
				dOmegaTemp = 2*pi*(cos(c*dAngle/180.0*pi)-cos((c+1)*dAngle/180.0*pi));
				dOmega.push_back(dOmegaTemp);

				temp = N_a * density / weight;
				temp = 1 / (temp * newInteraction[1]->GetMeanFreePath()) * 1e24;
				temp = temp * ((double) (hisThetaFirst->GetBinContent(c+1)) / (double) (newInteraction[0]->GetNumberOfTracks()));
				DiffXsection.push_back(temp/dOmegaTemp);

				if(DiffXsectionMaximum < (temp/dOmegaTemp))
					DiffXsectionMaximum = temp/dOmegaTemp;

			}

			grDiffXsection = new TGraph(DiffXsection.size());

			for(int c=0;c<DiffXsection.size();c++){
				//cout << c << ":\t" << DiffXsection[c] << "\t" << dOmega[c] << "\n";
				grDiffXsection->SetPoint(c,(c+0.5)*dAngle, DiffXsection[c]);
			}

			can->SetLogy(0);

			label.str("");
			label.clear();
			label << elementName << " differential elastic cross section " << ene[energyIndex] << "MeV";
			grDiffXsection->SetTitle(label.str().c_str());
			grDiffXsection->GetXaxis()->SetTitle("#Theta [deg]");
			grDiffXsection->GetYaxis()->SetTitle("d#sigma/d#Omega [b/sr]");
			grDiffXsection->GetYaxis()->SetRangeUser(0.0,DiffXsectionMaximum*1.1);
			grDiffXsection->SetMarkerStyle(8);
			grDiffXsection->SetMarkerSize(1.5);
			grDiffXsection->SetMarkerColor(kRed);
			grDiffXsection->Draw("AP");
			leg->AddEntry(grDiffXsection,"simulation","p");

			for(int c=0;c<eneDiff.size();c++){
				if(eneDiff[c] == newInteraction[1]->GetEnergy()){
					grDiffXsectionLiterature = new TGraph(diffXSection[c].size());

					for(int d=0; d<diffXSection[c].size();d++){
						grDiffXsectionLiterature->SetPoint(d,diffXSectionAngle[c][d],diffXSection[c][d]);
					}
					grDiffXsectionLiterature->SetMarkerStyle(21);
					grDiffXsectionLiterature->SetMarkerSize(0.8);
					grDiffXsectionLiterature->SetMarkerColor(kBlue);
					grDiffXsectionLiterature->SetLineColor(kBlue);
					grDiffXsectionLiterature->SetLineWidth(2);
					grDiffXsectionLiterature->Draw("PL");
					leg->AddEntry(grDiffXsectionLiterature,"literature","pl");

					Int_t index = 0;
					Double_t m,b;
					for(int d=0; d<DiffXsection.size();d++){
						for(int e=0; e < diffXSectionAngle[c].size() - 1;e++){
							if(diffXSectionAngle[c][e] > (d+0.5)*dAngle && diffXSectionAngle[c][e+1] <= (d+0.5)*dAngle){
								//cout << diffXSectionAngle[c][e+1] << "\t" << (d+0.5) * dAngle << "\t" << diffXSectionAngle[c][e] << "\n";
								m = (diffXSection[c][e+1] - diffXSection[c][e])/(diffXSectionAngle[c][e+1] - diffXSectionAngle[c][e]);
								b = diffXSection[c][e+1] - m * diffXSectionAngle[c][e+1];

								DiffXsectionInterpolation.push_back(m*(d+0.5)*dAngle + b);
								DiffXsectionSum += (m*(d+0.5)*dAngle + b);
								DiffXsectionDiff += fabs(DiffXsectionInterpolation[d]-DiffXsection[d]);

							}
						}
					}


					Double_t diffXagreement = (1.0 - DiffXsectionDiff/DiffXsectionSum)*100.0;
					cout << "AGREEMENT:\t" << diffXagreement << "\n";

					outputfile << "XX:" << elementName << "_neutron_elastic_differential_cross-section_" << newInteraction[1]->GetEnergy() << "MeV:XX\t";
					if(diffXagreement >= 80.0){
						outputfile << "\\colorbox{OliveGreen}{\\color{White} " << diffXagreement <<  "}\n";
					}
					if(diffXagreement >= 50.0 && diffXagreement < 80.0){
						outputfile << "\\colorbox{BurntOrange}{\\color{White} " << diffXagreement <<  "}\n";
					}
					if(diffXagreement < 50.0){
						outputfile << "\\colorbox{Red}{\\color{White} " << diffXagreement <<  "}\n";
					}
				}
 			}


			if(newInteraction[1]->GetEnergy() == 2.0 || newInteraction[1]->GetEnergy() == 20.0){

				leg->Draw();
				label.str("");
				label.clear();
				label << "pics/" << elementName << "_DiffXsection_" << ene[energyIndex] << "MeV.pdf";
				can->SaveAs(label.str().c_str());
				//XX:Cu_xsections_plot:XX	NeutronInteractions/pics/Cu_xsections_plot.pdf
				outputfile << "XX:" << elementName << "_diffXsections_plot_" << ene[energyIndex] << "MeV:XX\t";
				outputfile << "NeutronInteractions/" << label.str().c_str() << "\n";
			}

			//*/

			for (int k = 0; k < 4; k++) {
				delete newInteraction[k];
			}

			energyIndex++;
		}
	}

	//can->SetLogy(1);

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
		}

	}

	TGraphErrors *gr_xsec[4];
	TGraphErrors *gr_xsec_lit[4];
	TLegend *leg = new TLegend(0.75,0.99,0.99,0.75);

	gr_xsec[0] = new TGraphErrors(energy_simulation[0].size());
	for (int i = 0; i < energy_simulation[0].size(); i++) {
		gr_xsec[0]->SetPoint(i, energy_simulation[0][i], neutronXsection[0][i]);
		//cout << i << ":\t" << energy_simulation[0][i] << "\t" << neutronXsection[0][i] << "\n";
	}


	for(int k=1;k<4;k++){
		gr_xsec[k] = new TGraphErrors(energy_simulation[k].size());
		for (int i = 0; i < energy_simulation[k].size(); i++) {
			gr_xsec[k]->SetPoint(i, energy_simulation[k][i], neutronXsection[k][i]);
			//cout << i << ":\t" << energy_simulation[k][i] << "\t" << neutronXsection[k][i] << "\n";
		}
		gr_xsec[k]->SetMarkerStyle(21);

	}

	for (int k = 0; k < 4; k++) {
		gr_xsec_lit[k] = new TGraphErrors(energy_literature[k].size());
		gr_xsec_lit[k]->SetMarkerStyle(25);
		if (energy_literature[k].size() > 0) {
			for (int i = 0; i < energy_literature[k].size(); i++) {
				gr_xsec_lit[k]->SetPoint(i, energy_literature[k][i], neutronXsection_literature[k][i]);
			}
		}
	}

	can->SetLogx();
	can->SetLogy();

	gr_xsec[0]->SetMarkerColor(kBlack);
	gr_xsec[0]->SetTitle(elementName.c_str());
	gr_xsec[0]->SetMarkerStyle(21);
	gr_xsec[0]->GetXaxis()->SetTitle("neutron energy [MeV]");
	gr_xsec[0]->GetYaxis()->SetTitle("total cross section [barn]");
	gr_xsec[0]->GetYaxis()->SetRangeUser(0.0001,100);
	can->Update();
	gr_xsec[0]->Draw("AP");
	leg->AddEntry(gr_xsec[0],"total neutron cross-section","p");

	gr_xsec[1]->SetMarkerColor(kAzure-3);
	gr_xsec[1]->Draw("P");
	leg->AddEntry(gr_xsec[1],"neutron elastic cross-section","p");

	gr_xsec[2]->SetMarkerColor(kOrange-3);
	gr_xsec[2]->Draw("P");
	leg->AddEntry(gr_xsec[2],"neutron inelastic cross-section","p");

	gr_xsec[3]->SetMarkerColor(kRed);
	gr_xsec[3]->Draw("P");
	leg->AddEntry(gr_xsec[3],"neutron capture cross-section","p");

	if (energy_literature[0].size() > 0) {
		gr_xsec_lit[0]->SetMarkerColor(kBlack);
		gr_xsec_lit[0]->Draw("P");
		leg->AddEntry(gr_xsec_lit[0], "total neutron cross-section (literature)", "p");
	}

	if (energy_literature[1].size() > 0) {
		gr_xsec_lit[1]->SetMarkerColor(kAzure - 3);
		gr_xsec_lit[1]->Draw("P");
		leg->AddEntry(gr_xsec_lit[1], "neutron elastic cross-section (literature)", "p");
	}

	if (energy_literature[2].size() > 0) {
		gr_xsec_lit[2]->SetMarkerColor(kOrange - 3);
		gr_xsec_lit[2]->Draw("P");
		leg->AddEntry(gr_xsec_lit[2], "neutron inelastic cross-section (literature)", "p");
	}

	if (energy_literature[3].size() > 0) {
		gr_xsec_lit[3]->SetMarkerColor(kRed);
		gr_xsec_lit[3]->Draw("P");
		leg->AddEntry(gr_xsec_lit[3], "neutron capture cross-section (literature)", "p");
	}


	leg->Draw();
	s = "pics/" + elementName + "_xsections_plot.pdf";
	outputfile << "XX:" << elementName << "_xsections_plot:XX\t";
	outputfile << "NeutronInteractions/pics/" << elementName << "_xsections_plot.pdf\n";
	gSystem->ProcessEvents();
	can->SaveAs(s.c_str());

	outputfile.close();

	//theApp.Run();

	return 0;
}
