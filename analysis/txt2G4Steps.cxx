//
// ROOT program to convert simple text file into ROOT tree filled
// with MGOutG4StepsData events. Only uses entries that are relevant
// to MGProcessors. Use for debugging MGProcessors. 
// 
// Text file consists of rows with the following entries:
// Event N | fEdep | fPhysicalVolume | fStatus01 | fLocalX | fLocalY | fLocal Z
// | fGlobalTime
// Each entry is separated by a white space.
// A line that starts with "-1 " is considered a comment and ignored.
// Load analysis/libROOT.so before executing.
//
// R. Henning
// 9/15/2006
//

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

#include "TFile.h"
#include "TTree.h"

#include "/home/rhenning/Majorana/code/MaGe/io/io/MGOutputG4StepsData.hh"

void txt2G4Steps(string fileName)
{
  ifstream  txtFile(fileName.c_str(), ios::in);
  Char_t    commentString[256];
  if(!txtFile)
    std::cout << "File not found : " << fileName << std::endl;
  else {
    Long64_t eventN = -9999, oldEventN = -9999;
    MGOutputG4StepsData *sD = new MGOutputG4StepsData();
    sD->AllocateArrays();
    TFile *rootFile = new TFile((fileName + ".root").c_str(), "RECREATE");
    TTree *rawMC = new TTree("fTree", "G4Step Test Tree");
    rawMC->Branch("RawMC", "MGOutputG4StepsData", &sD, 32000, 2);
    txtFile >> eventN;
    while(!txtFile.eof()) {
      if(eventN == -1) {
	txtFile.getline(commentString, 256);
	std::cout << commentString << std::endl;
      }
      else {
	if(eventN != oldEventN) {
	  if(oldEventN != -9999) rawMC->Fill();
	  sD->fTotalNumberOfSteps = 0; sD->fVolumeNamePos = 0;
	}
	txtFile >> sD->fEdep[sD->fTotalNumberOfSteps] 
		>> &sD->fPhysicalVolume[sD->fVolumeNamePos]
		>> sD->fStatus01[sD->fTotalNumberOfSteps]  
		>> sD->fLocalX[sD->fTotalNumberOfSteps] 
		>> sD->fLocalY[sD->fTotalNumberOfSteps] 
		>> sD->fLocalZ[sD->fTotalNumberOfSteps]
		>> sD->fGlobalTime[sD->fTotalNumberOfSteps];
	std::cout << eventN << "  "
		  << sD->fTotalNumberOfSteps << "  "
		  << sD->fVolumeNamePos << "  "
		  << sD->fEdep[sD->fTotalNumberOfSteps] << "  "
		  << &sD->fPhysicalVolume[sD->fVolumeNamePos] << "  "
		  << sD->fStatus01[sD->fTotalNumberOfSteps] << "  "
		  << sD->fLocalX[sD->fTotalNumberOfSteps] << "  " 
		  << sD->fLocalY[sD->fTotalNumberOfSteps] << "  " 
		  << sD->fLocalZ[sD->fTotalNumberOfSteps] << " " 
		  << sD->fGlobalTime[sD->fTotalNumberOfSteps] << std::endl;
	sD->fVolumeNamePos += strlen(&sD->fPhysicalVolume[sD->fVolumeNamePos])+1;
	sD->fTotalNumberOfSteps++;
	oldEventN = eventN;
      }
      txtFile >> eventN;
    }
    rawMC->Fill();
    rawMC->Write();
    rootFile->Close();
    delete sD;
  }
}
