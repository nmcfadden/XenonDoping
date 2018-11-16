/*
 * RComp.cxx
 *
 *  Created on: Apr 25, 2011
 *      Author: fraenkle
 */

#include <iostream>
#include <sstream>
#include <math.h>
#include "RComp.h"

RComp::RComp() {

	rootfile = 0;
	tree = 0;

}

void RComp::CreateNewCompFile(std::string filename){

	rootfile = new TFile(filename.c_str(),"RECREATE","Relative Comparison ROOT file");
	rootfile->Close();
	delete rootfile;
	rootfile = 0;

}

void RComp::OpenCompFile(std::string filename, std::string filemode){

	if(rootfile == 0){
		if (gSystem->AccessPathName(filename.c_str(), kFileExists) == 0){
			rootfile = new TFile(filename.c_str(), filemode.c_str());
			//std::cout << "opened ROOT file " << filename << "\t" << rootfile << "\n";
		}
	}
	else{
		std::cout << "RComp: WARNING! There is already a ROOT file associated with this object.\n";
	}
}

void RComp::CloseCompFile(){

	if(rootfile != 0){
		if(rootfile->IsWritable())
			tree->Write(0,6,0);
		rootfile->Close();
		delete rootfile;
		rootfile = 0;
	}
}

void RComp::WriteEntry(std::string token, double value, double sigma) {

	if(rootfile == 0)
		return;

	char ctoken[128] = {0};
	for(int i=0;i<token.length();i++){
		if(i>127){
			std::cout << "RComp: WARNING! Token too long:\n" << token << "\n";
			break;
		}
		ctoken[i] = token.at(i);
	}

	if((TTree*)rootfile->Get("comptree") == 0){
		tree = new TTree("comptree","comparison tree");
		tree->Branch("token",ctoken,"token[128]/C");
		tree->Branch("value",&value,"value/D");
		tree->Branch("sigma",&sigma,"sigma/D");
		tree->Fill();
		tree->Write(0,6,0);
	}
	else{
		tree = (TTree*)rootfile->Get("comptree");
		tree->SetBranchAddress("token",ctoken);
		tree->SetBranchAddress("value",&value);
		tree->SetBranchAddress("sigma",&sigma);
		tree->Fill();
	}
}

double RComp::GetEntryValue(std::string token){

	if(rootfile == 0)
		return 0.0;

	char ctoken[128];
	double value, sigma;
	std::string stoken;

	if((TTree*)rootfile->Get("comptree") == 0){
		std::cout << "File is empty.\n";
		return 0.0;
	}
	else{
		tree = (TTree*)rootfile->Get("comptree");
		tree->SetBranchAddress("token",ctoken);
		tree->SetBranchAddress("value",&value);
		tree->SetBranchAddress("sigma",&sigma);
	}

	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		stoken = ctoken;
		if(stoken == token){
			return value;
		}
	}

	return 0.0;
}

double RComp::GetEntrySigma(std::string token){

	if(rootfile == 0)
		return 0.0;

	char ctoken[128];
	double value, sigma;
	std::string stoken;

	if((TTree*)rootfile->Get("comptree") == 0){
		std::cout << "File is empty.\n";
		return 0.0;
	}
	else{
		tree = (TTree*)rootfile->Get("comptree");
		tree->SetBranchAddress("token",ctoken);
		tree->SetBranchAddress("value",&value);
		tree->SetBranchAddress("sigma",&sigma);
	}

	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		stoken = ctoken;
		if(stoken == token){
			return sigma;
		}
	}

	return 0.0;
}

void RComp::PrintContent(){

	char ctoken[128];
	double value, sigma;

	if((TTree*)rootfile->Get("comptree") == 0){
		std::cout << "File is empty.\n";
		return;
	}
	else{
		tree = (TTree*)rootfile->Get("comptree");
		tree->SetBranchAddress("token",ctoken);
		tree->SetBranchAddress("value",&value);
		tree->SetBranchAddress("sigma",&sigma);
	}
	std::cout << "Number of entries in file: " << tree->GetEntries() << "\n";
	std::cout << "index\t\ttoken\tvalue\tsigma\n";
	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		std::cout << i << "\t" << ctoken << "\t" << value << "\t" << sigma << "\n";
	}
}

double RComp::CompareValues(double val1, double val2){

	return val1 - val2;
}

int RComp::GetAgreementLevel(double val1, double sig1, double val2, double sig2, double level){

	int rating = 0;

	if (fabs(CompareValues(val1, val2)) <= sig1 + sig2) {
		rating = 0;
	} else if (fabs(CompareValues(val1, val2)) < val2 * level / 100.0) {
		rating = 1;
	} else {
		rating = 10;
	}

	return rating;
}

std::string RComp::GetAgreementColourString(double val1, double sig1, double val2, double sig2, double level) {

	std::string s;
	std::stringstream label;

	label.str("");
	label.clear();

	switch (GetAgreementLevel(val1, sig1, val2, sig2, level)) {
	case 0:
		label << "\\colorbox{OliveGreen}{\\color{White} ";
		break;
	case 1:
		label << "\\colorbox{BurntOrange}{\\color{White} ";
		break;
	case 10:
		label << "\\colorbox{Red}{\\color{White} ";
		break;
	}
	label.precision(2);
	label << CompareValues(val1, val2) << "}\n";

	s = label.str();
	return s;
}

RComp::~RComp() {

}
