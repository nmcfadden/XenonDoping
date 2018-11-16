//$Id: TestWFResampler.cc,v 1.1 2008-10-15 14:51:26 jliu Exp $
//Author: Jing Liu
//Reversion:
//	15.10.2008 created. Jing
#include <string>
#include <cmath>

#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"

#include "MGTWaveform.hh"
#include "MGWFResampler.hh"
#include "MGWFAddNoise.hh"
#include "MGWFRCDifferentiation.hh"
#include "MGWFRCIntegration.hh"

#include "io/MGLogger.hh"
#include "waveform/MGWaveformWriterROOT.hh"

using namespace std;
using namespace CLHEP;

int main(int argc, char *argv[])
{
   //MGLogger::SetSeverity(MGLogger::trace);
   MGLogger::SetSeverity(MGLogger::routine);

   if(argc==1 || argc>3) {
      cout << "Usage: TestResampler inputWFfile [outputWFfile]" << endl;
      return 1;
   }

   //output
   MGWaveformWriterROOT outputFile;
   if (argc==2) {
      outputFile.OpenOutputFile("resampledWFs");
   }
   else { //argc==3
      outputFile.OpenOutputFile(argv[2]);
   }
      
   //input
   TChain wfTree("wfTree", "wfTree");
   wfTree.AddFile(argv[1]);
   string wfBranchName = wfTree.GetListOfBranches()->At(0)->GetName();
   TClonesArray wfClonesArray("MGTWaveform");
   TClonesArray* pa = &wfClonesArray;
   wfTree.SetBranchAddress(wfBranchName.c_str(), &pa);

   MGWFResampler resampler;
   MGWFAddNoise addNoise;
   MGWFRCDifferentiation decay;
   MGWFRCIntegration bandwidth;

   addNoise.SetNoiseAmplitude(5000.0/2.95/eplus); // 5 keV / 2.95 eV (pair production energy)
   decay.SetTimeConstant(5000*ns);
   bandwidth.SetTimeConstant(4*ns);
   int Nentries = wfTree.GetEntries();
   //cout<<"Nentries = "<<Nentries<<endl;
   MGWaveform *outputwf=NULL;
   //for(int iEntry=0; iEntry<1; iEntry++) {
   for(int iEntry=0; iEntry<Nentries; iEntry++) {
      wfTree.GetEntry(iEntry);
      size_t nWaveforms = wfClonesArray.GetEntries();

      MGWaveformCollection outputWFs 
	 = *(outputFile.GimmeWaveformCollection(
		  0, nWaveforms, wfBranchName.c_str()));

      //for(size_t iWaveform = 0; iWaveform < 1; iWaveform++) {
      for(size_t iWaveform = 0; iWaveform < nWaveforms; iWaveform++) {
	 MGTWaveform& waveform = *((MGTWaveform*) wfClonesArray.At(iWaveform));
	 outputwf = outputWFs.GetWaveform(iWaveform);
	 outputwf->SetID(waveform.GetID());
	 size_t newLen=(size_t) floor(waveform.GetLength()*75*MHz/waveform.GetSamplingFrequency());
	 //cout<<"newLen = "<<newLen<<endl;
	 outputwf->SetLength(newLen);
	 outputwf->SetSamplingFrequency(75*MHz); 
	 outputwf->SetTOffset(waveform.GetTOffset());
	 outputwf->SetWFType(waveform.GetWFType());
	 //waveform.DumpWF();
	 resampler.TransformOutOfPlace(waveform, *outputwf);
	 addNoise.TransformInPlace(*outputwf);
	 bandwidth.TransformInPlace(*outputwf);
	 decay.TransformInPlace(*outputwf);
 	 //outputwf->DumpWF();
      }
      outputFile.WriteEvent();
   }
   outputFile.CloseOutputFile();

   return 0;
}
