#include <iostream>
#include <fstream>
#include <sstream>

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TApplication.h>
#include <TImage.h>

using namespace std;

int main(int argc, char **argv){

	TCanvas* can;
	TApplication theApp("App", &argc, argv);
	TH1F* his;
	ifstream inputfile;
	ofstream outputfile;
	TImage *img;

	double temp;
	istringstream conv;
	string s;
	TF1* landau_fit;
	char* filename = "G4Demo.txt";

	gROOT->SetStyle("Plain");


	can = new TCanvas("can");
	can->SetGrid();
	can->Connect("Closed()", "TApplication", &theApp, "Terminate()");

	his = new TH1F("his","histogram",500,0.0,500);

	inputfile.open(filename);
	if(inputfile.fail()){
		cout << "(DemoAnalysis):\tERROR! Can't open file " << filename << " !\n";
		return 0;
	}
	while (getline(inputfile, s)) {

		conv.str(s);
		conv >> temp;
		his->Fill(temp*1000);
		conv.str("");
		conv.clear();
	}
	inputfile.close();

	landau_fit = new TF1("landau_fit","landau");
	his->Fit("landau_fit","Q","",0,500);

	his->SetXTitle("energy deposit [keV]");
	his->SetFillColor(38);
	his->SetLineColor(9);

	his->Draw();

	/*
	for(int i=0;i<3;i++){
		cout << landau_fit->GetParameter(i) << "\t";
		cout << landau_fit->GetParError(i) << "\n";
	}
	*/

	outputfile.open("parameterlist.txt");
	outputfile << "XX:LandauFit_1_constant:XX\t" << landau_fit->GetParameter(0) << "\n";
	outputfile << "XX:LandauFit_1_constant_error:XX\t" << landau_fit->GetParError(0) << "\n";
	outputfile << "XX:LandauFit_1_MPV:XX\t" << landau_fit->GetParameter(1) << "\n";
	outputfile << "XX:LandauFit_1_MPV_error:XX\t" << landau_fit->GetParError(1) << "\n";
	outputfile << "XX:LandauFit_1_Sigma:XX\t" << landau_fit->GetParameter(2) << "\n";
	outputfile << "XX:LandauFit_1_Sigma_error:XX\t" << landau_fit->GetParError(2) << "\n";
	outputfile << "XX:LandauFit_1_figure:XX\t" << "energydeposit.png" << "\n";
	outputfile << "XX:electron_energy:XX\t" << "0.5 MeV" << "\n";

	// additional test output
	outputfile << "XX:author:XX\t" << "Florian Fraenkle" << "\n";
	outputfile << "XX:header1:XX\t" << "energy deposit test" << "\n";
	outputfile << "XX:text:XX\t" << "LaTeX Generator test text $\\mu^2_x$" << "\n";

	outputfile.close();

	gSystem->ProcessEvents();
	img = TImage::Create();
	img->FromPad(can);
	img->WriteImage("energydeposit.png");

	//theApp.Run();

	return 0;
}
