/*
 * TRootWriter.h
 *
 *  Created on: Feb 18, 2011
 *      Author: fraenkle
 */

#ifndef TROOTWRITER_H_
#define TROOTWRITER_H_

#include "TFile.h"
#include "TTree.h"
#include <string>

class TRootWriter {
public:
	TRootWriter();
	static TRootWriter *GetInstance();
	void CreateRootFile(std::string filename);
	void CloseRootFile();
	void AddEvent(Int_t decay, Int_t particle, Double_t t, Double_t E);
	void AddParticle();
	virtual ~TRootWriter();

private:
	static TRootWriter *rootWriter;
	TFile *rootfile;
	TTree *tree;
	Int_t decayID, PID;
	Double_t time, energy;
};

#endif /* TROOTWRITER_H_ */
