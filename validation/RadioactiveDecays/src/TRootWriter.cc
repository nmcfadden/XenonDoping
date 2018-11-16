/*
 * TRootWriter.cxx
 *
 *  Created on: Feb 18, 2011
 *      Author: fraenkle
 */

#include "TRootWriter.hh"

TRootWriter *TRootWriter::rootWriter = 0;

TRootWriter::TRootWriter() {
	// TODO Auto-generated constructor stub
	rootWriter = this;
	rootfile = 0;
}

TRootWriter *TRootWriter::GetInstance(){

	if(rootWriter == 0)
		rootWriter = new TRootWriter();

	return rootWriter;
}

void TRootWriter::CreateRootFile(std::string filename){

	  //hfileROOT = new TFile(fileNameROOT.c_str() ,"RECREATE","ROOT file for exRDM");
	rootfile = new TFile(filename.c_str(),"RECREATE","DecayEngine ROOT file");
	tree = new TTree("decaytree","decay tree");
	tree->Branch("decayID",&decayID,"decayID/i");
	tree->Branch("PID",&PID,"PID/I");
	tree->Branch("time",&time,"time/D");
	tree->Branch("energy",&energy,"energy/D");
}

void TRootWriter::AddEvent(Int_t decay, Int_t particle, Double_t t, Double_t E){

	decayID = decay;
	PID = particle;
	time = t;
	energy = E;

	tree->Fill();
}

void TRootWriter::CloseRootFile(){

	if(rootfile != 0){
		tree->Write();
		rootfile->Close();
	}
}


TRootWriter::~TRootWriter() {
	// TODO Auto-generated destructor stub
}
