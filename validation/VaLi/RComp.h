/*
 * RComp.h
 *
 *  Created on: Apr 25, 2011
 *      Author: fraenkle
 */

#include <string>
#include "TFile.h"
#include "TTree.h"
#include <TROOT.h>
#include <TSystem.h>

#ifndef RCOMP_H_
#define RCOMP_H_

class RComp {
public:
	RComp();
	void CreateNewCompFile(std::string filename);
	void OpenCompFile(std::string filename, std::string filemode);
	void CloseCompFile();
	void WriteEntry(std::string token, double value, double sigma);
	double GetEntryValue(std::string token);
	double GetEntrySigma(std::string token);
	void PrintContent();
	double CompareValues(double val1, double val2);
	int GetAgreementLevel(double val1, double sig1, double val2, double sig2, double level);
	std::string GetAgreementColourString(double val1, double sig1, double val2, double sig2, double level);
	virtual ~RComp();
private:
	TFile *rootfile;
	TTree *tree;

};

#endif /* RCOMP_H_ */
