//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
/**                                                            
 * $Id: WaveformBrowser.cc,v 1.7 2008-12-02 13:10:15 jliu Exp $
 *      
 * CLASS DECLARATION:  MJAnalysisWF.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Sample wf generation program for Majorana.
 *
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jan  4 15:01:33 PST 2006
 * 
 * REVISION:
 * 
 * 05-01-2006, Created, J. Detwiler
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRint.h"
#include "TString.h"
#include "TStyle.h"
#include "MGCrystalData.hh"
#include "MGTWaveform.hh"
#include "TH1D.h"

using namespace std;
using namespace CLHEP;

// Example Usage: WaveformBrowser (WFBranchName) [WFOutputFiles]

TApplication* gApp = NULL;

int main(int argc, char *argv[])
{
  if(argc < 2) {
    cout << "Usage: WaveformBrowser (WFBranchName) [WFOutputFiles]" << endl;
    return 1;
  }

  gApp = new TApplication("gApp", 0, NULL);
  gStyle->SetOptStat(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleAlign(23);
  gStyle->SetTitleX(0.5);
  gStyle->SetFillColor(0);

  TChain* wfTree = new TChain("wfTree", "wfTree");
  string wfBranchName = argv[1];
  // if no waveform branch name was specified, argv[1] will be a root file.
  // In this case, just take the first branch in the tree whose name ends
  // in "WFs".
  if(wfBranchName.rfind(".root") == wfBranchName.size()-5) {
    wfTree->AddFile(wfBranchName.c_str());
    wfBranchName = "";
    TObjArray* branchList = wfTree->GetListOfBranches();
    for(int i=0; i<branchList->GetEntries(); i++) {
      TBranch* branch = (TBranch*) branchList->At(i);
      string branchName = branch->GetName();
      if(branchName.rfind("WFs") == branchName.size()-3) wfBranchName = branchName;
      else {
        cout << "Couldn't find waveforms branch in wfTree in " << argv[1] << endl;
        return 1;
      }
    }
  }
  for(int i=2; i<argc; i++) {
    wfTree->AddFile(argv[i]);
  }

  cout << "Displaying waveforms for branch " << wfBranchName << endl;
  TClonesArray* wfClonesArray = new TClonesArray("MGTWaveform");
  wfTree->SetBranchAddress(wfBranchName.c_str(), &wfClonesArray);

  cout << "Commands: " << endl
       << "n = next entry " << endl
       << "p = previous entry " << endl
       << "s = skip to another entry " << endl
       << "r = restrict x-axis range" << endl
       << "g = granularity cut " << endl
       << "print = print canvas to png" << endl
       << "d = dump waveforms to stdout" << endl
       << "q = quit" << endl;

  double startTime_ns = 0, stopTime_ns = 0;
  size_t granMin = 1, granMax = 0;
  bool dumpWF = false;

  for(int iEntry=0; iEntry<wfTree->GetEntries(); iEntry++) {
    wfTree->GetEntry(iEntry);
    int nWaveforms = wfClonesArray->GetEntries();
    if(nWaveforms == 0) {
      cout << "Warning: no waveforms in entry " << iEntry << endl;
      continue;
    }

    // Pull out the histograms for each waveform in each crystal. While
    // we're at it, calculate the number of crystals
    vector<int> crystalIDs;
    vector<double> tOffsets;
    map< int, map< size_t, TH1D* > > crystalHists;
    for(int iWaveform = 0; iWaveform < nWaveforms; iWaveform++) {
      MGTWaveform* waveform = (MGTWaveform*) wfClonesArray->At(iWaveform);
      if(waveform == NULL) {
        cout << "Error: Got NULL waveform" << endl;
        break;
      }

      int crystalID = MGCrystalData::GetCrystalID(waveform->GetID());
      size_t iID = 0;
      while(iID < crystalIDs.size() && crystalIDs[iID] != crystalID) iID++;
      if(iID == crystalIDs.size()) {
        crystalIDs.push_back(crystalID);
        tOffsets.push_back(waveform->GetTOffset());
      }

      size_t iContact = MGCrystalData::GetIContact(waveform->GetID());
      TH1D* hist = waveform->GimmeHist();
      crystalHists[crystalID][iContact] = hist;

      if(dumpWF) {
        cout << "Waveform for crystal " << crystalID << ", contact " << iContact << ":" << endl;
        waveform->DumpWF(cout, 2, 16);
      }
    }

    if(crystalIDs.size() < granMin || (granMax > 0 && crystalIDs.size() > granMax)) continue;

    cout << "entry " << iEntry << ": " << nWaveforms << " waveforms" << endl;

    // Divide canvas to accommodate the number of crystals hit
    TCanvas* canvas = new TCanvas("WaveformBrowser", "WaveformBrowser", 1024, 768);
    int nColumns = TMath::CeilNint(TMath::Sqrt(crystalIDs.size()));
    int nRows = TMath::CeilNint((double) nColumns / (double) crystalIDs.size());
    canvas->Divide(nColumns, nRows);

    // now loop over the crystals and draw the waveforms onto each canvas
    for(size_t iCrystal = 0; iCrystal < crystalIDs.size(); iCrystal++) {
      canvas->cd(iCrystal+1);
      int crystalID = crystalIDs[iCrystal];
      TH1D* centralContactHist = crystalHists[crystalID][0]; 
      if(centralContactHist == NULL) {
        cout << "Error: no central contact hist for crystal id " << crystalID << endl;
        break;
      }

      // Calc min/max
      double yMin = centralContactHist->GetMinimum();
      double yMax = centralContactHist->GetMaximum();
      for(size_t iContact = 1; iContact < crystalHists[crystalID].size(); iContact++) {
        TH1D* hist = crystalHists[crystalID][iContact];
        if(hist == NULL) {
          cout << "Error: no hist for crystal id " << crystalID  << ", contact " << iContact << endl;
          break;
        }
        if(hist->GetMinimum() < yMin) yMin = hist->GetMinimum();
        if(hist->GetMaximum() > yMax) yMax = hist->GetMaximum();
      }
      if(yMax == 0 && yMin == 0) {
        yMax = 1.0;
        yMin = -1.0;
      }
      else {
        double yCenter = (yMin + yMax)/2.;
        yMax = yCenter + (yMax - yCenter)*1.05;
        yMin = yCenter - (yCenter - yMin)*1.05;
      }

      // Set x-axis range if necessary. Reset if you get nonsense.
      if(startTime_ns >= stopTime_ns) {
        startTime_ns = centralContactHist->GetXaxis()->GetXmin();
        stopTime_ns = centralContactHist->GetXaxis()->GetXmax();
      }
      if(startTime_ns < centralContactHist->GetXaxis()->GetXmin()) {
        startTime_ns = centralContactHist->GetXaxis()->GetXmin();
      }
      if(stopTime_ns > centralContactHist->GetXaxis()->GetXmax()) {
        stopTime_ns = centralContactHist->GetXaxis()->GetXmax();
      }
      centralContactHist->GetXaxis()->SetRangeUser(startTime_ns, stopTime_ns);

      // Now draw
      centralContactHist->SetTitle(Form("Crystal %d, T_{0} = %g ns", crystalID, tOffsets[iCrystal]/ns));
      centralContactHist->SetMinimum(yMin);
      centralContactHist->SetMaximum(yMax);
      centralContactHist->GetXaxis()->CenterTitle();
      centralContactHist->GetYaxis()->CenterTitle();
      centralContactHist->Draw("L");

      TLegend* legend = new TLegend(0.6, 0.85-0.05*crystalHists[crystalID].size(), 0.85, 0.85);
      legend->SetFillColor(0);
      legend->SetFillStyle(0);
      legend->SetBorderSize(0);
      legend->AddEntry(centralContactHist, "Central Contact", "L");

      for(size_t iContact = 1; iContact < crystalHists[crystalID].size(); iContact++) {
        TH1D* hist = crystalHists[crystalID][iContact];
        if(hist == NULL) {
          cout << "Error: no hist for crystal id " << crystalID  << ", contact " << iContact << endl;
          break;
        }
        int baseColor = 1;
        hist->SetLineColor(baseColor + iContact);
        hist->Draw("L SAME");
        legend->AddEntry(hist, Form("Segment %u", (unsigned int) iContact), "L");
      }
      legend->Draw();
    }
    canvas->cd(0);
    canvas->Update();

    dumpWF = false;

    // Handle user input
    string command = "";
    bool firstTime = true;
    while(command != "n" && command != "p" && command != "s" && 
          command != "r" && command != "g" && 
          command != "print" && command != "d" && command != "q") {
      if(!firstTime) {
        cout << "You must enter n, p, s, r, g, print, or q" << endl;
      }
      cout << "Enter a command: ";
      cin >> command;
      firstTime = false;
    }

    if(command == "p") {
      if(iEntry == 0) iEntry--;
      else iEntry -= 2;
    }
    else if (command == "s") {
      string buffer;
      cout << "What entry number would you like to skip to (0 - " << wfTree->GetEntries()-1 << "): ";
      cin >> buffer;
      int entry = atoi(buffer.c_str());
      if(entry > 0 || (entry == 0  && buffer == "0")) iEntry = entry - 1;
      else iEntry--;
    }
    else if (command == "r") {
      string buffer;
      cout << "Enter a start time [ns]: ";
      cin >> buffer;
      startTime_ns = atof(buffer.c_str());
      cout << "Enter a stop time [ns]: ";
      cin >> buffer;
      stopTime_ns = atof(buffer.c_str());
      iEntry--;
    }
    else if (command == "g") {
      string buffer;
      cout << "set the granularity minimum (will cut g >= minimum) :";
      cin >> buffer;
      granMin = atoi(buffer.c_str());
      if(granMin < 1) granMin = 1;
      cout << "set the granularity maximum (will cut g <= maximum; set to 0 for no maximum) :";
      cin >> buffer;
      granMax = atoi(buffer.c_str());
      iEntry--;
    }
    else if (command == "print") {
      string title;
      string yesno = "n";
      while(yesno != "y") {
        cout << "Enter a file name: ";
        cin >> title;
        for(size_t i=0; i<title.size(); i++) {
          if(!((title[i] >= 'A' && title[i] <= 'Z') ||
               (title[i] >= 'a' && title[i] <= 'z') ||
               (title[i] >= '0' && title[i] <= '9') ||
               (title[i] == '.' || title[i] == '-' || title[i] == '\0'))) {
            title[i] = '_';
          }
        }
        if(title.rfind(".png") != title.size()-4 &&
           title.rfind(".gif") != title.size()-4 &&
           title.rfind(".jpg") != title.size()-4 &&
           title.rfind(".jpeg") != title.size()-4 &&
           title.rfind(".C") != title.size()-4 &&
           title.rfind(".root") != title.size()-4) {
          title += ".png";
        }
        cout << "Will save as " << title << ", ok? (y/n): ";
        cin >> yesno;
      }
      canvas->Print(title.c_str());
      iEntry--;
    }
    else if (command == "d") {
      dumpWF = true;
      iEntry--;
    }
    else if (command == "q") break;
    delete canvas;
  }

}

