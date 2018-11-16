//
// ROOT function to make pretty plots demonstrating segmentation, radial PSD
// and position resolution efficiencies. 
//
// R. Henning
// 8/24/2006
//

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TMath.h"
#include "TStyle.h"

void rootplot()
{

  // Select File and input params.
  string fNameHead = "GeTray2CrystalColumn8Tl208_1";
  Stat_t total =  200000;
  Int_t upperROI = 2090;
  Int_t lowerROI = 1990;
  Int_t dROI = upperROI - lowerROI + 1;
  
  // Local variables.
  Double_t effAll, effGran, effSeg, effSegAndRad;
  Double_t effAllErr, effGranErr, effSegErr, effSegAndRadErr;
  TFile f1((fNameHead + "_seg.root").c_str());
  Size_t maxval;
  Stat_t integralAll, integralGran, integralSeg, integralSegAndRad;
  list<Double_t>  effSegList, effSegErrList;
  
  // Segmentation schemes to be considered.
  list<TH1F*>  hAll;
  list<TH1F*>  hGran;
  list<TH1F*>  hSeg;
  list<TH1F*>  hSegAndRad;
  list<string> names;
  
  names.push_back("1x1"); names.push_back("1x2"); names.push_back("1x3");
  names.push_back("1x5"); names.push_back("1x7"); names.push_back("1x10");
  names.push_back("2x1"); names.push_back("2x2"); names.push_back("2x3");
  names.push_back("2x5"); names.push_back("2x7"); names.push_back("2x10");
  names.push_back("3x1"); names.push_back("4x1"); names.push_back("4x2");
  names.push_back("4x4"); names.push_back("4x8"); names.push_back("5x1");
  names.push_back("6x1"); names.push_back("6x6"); names.push_back("7x1");
  names.push_back("8x1"); names.push_back("8x2"); names.push_back("8x5");
  names.push_back("10x1"); names.push_back("10x10"); 
  
  list<string>::iterator namesIter = names.begin();
  while(namesIter != names.end()) {
    hAll.push_back((TH1F*) f1.Get(("HMGProcessorMCSegmentation" + *namesIter + "All").c_str()));
    hGran.push_back((TH1F*) f1.Get(("HMGProcessorMCSegmentation" + *namesIter + "Gran").c_str()));
    hSeg.push_back((TH1F*) f1.Get(("HMGProcessorMCSegmentation" + *namesIter + "Seg").c_str()));
    hSegAndRad.push_back((TH1F*) f1.Get(("HMGProcessorMCSegmentation" + *namesIter + "SegAndRad").c_str()));
    namesIter++;
  }
  TH1F  *HSumEdep = (TH1F*) f1.Get("HMGProcessorMCSegmentation1x1All");
  TH1F  *HZCut = (TH1F*) f1.Get("HZCutMGProcessorMCRadialDiscriminator");
  TH1F  *HPhiCut = (TH1F*) f1.Get("HPhiCutMGProcessorMCRadialDiscriminator");
  TH1F  *HWAllCuts = (TH1F*) f1.Get("HWAllCutsMGProcessorMCRadialDiscriminator");
  TH1F  *HMultCut = (TH1F*) f1.Get("HMultCutMGProcessorMCRadialDiscriminator");
  TH1F  *HMEMultCut = (TH1F*) f1.Get("HMEMultCutMGProcessorMCRadialDiscriminator");
  TH1F  *HMEWPolarCut = (TH1F*) f1.Get("HMEWPolarCutMGProcessorMCRadialDiscriminator");
  std::cout << HSumEdep << " " << HZCut << " " << HPhiCut << " " << HWAllCuts << " "
	    << HMultCut << " " << HMEMultCut << " " << HMEWPolarCut << std::endl;
  
  // Create canvases
  
  TCanvas *c1 = new TCanvas("c1", "Simple Segmentation", 0, 0, 750, 1000); 
  c1->Divide(1,2);
  gStyle->SetOptStat(0);
  c1->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(1);
  c1->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  
  TCanvas *c2 = new TCanvas("c2", "Position Resolution", 0, 0, 850, 850);
  c2->Divide(2,2);
  gStyle->SetOptStat(0);
  c2->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(1);
  c2->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(1);
  c2->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(1);
  c2->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(1);
    
  
  // Initialize
  TH1F *htemp=0;
  list<TH1F*>::iterator hAllIter = hAll.begin();
  list<TH1F*>::iterator hGranIter = hGran.begin();
  list<TH1F*>::iterator hSegIter = hSeg.begin();
  list<TH1F*>::iterator hSegAndRadIter = hSegAndRad.begin();
  namesIter = names.begin();
 
  // Make plots for all segmentation schemes
  effSegList.clear(); effSegErrList.clear();
  while(hAllIter != hAll.end()) {
   
    // Draw Segmenation and radial PSD histograms.
    c1->cd(1);
    maxval = (*hAllIter)->GetMaximum(1e7);
    (*hAllIter)->SetLineColor(kBlack);
    (*hAllIter)->SetTitle((fNameHead + "_" + *namesIter).c_str());
    (*hAllIter)->GetXaxis()->SetTitle("Energy (keV)");
    (*hAllIter)->GetYaxis()->SetTitle("Counts");
    htemp = (TH1F*) (*hAllIter)->Clone((fNameHead + *namesIter + "_Zoom").c_str());
    (*hAllIter)->GetYaxis()->SetRangeUser(0.8,maxval);
    (*hAllIter)->Draw();
    (*hGranIter)->SetLineColor(kBlue);
    (*hGranIter)->Draw("same");
    (*hSegIter)->SetLineColor(kRed);
    (*hSegIter)->Draw("same");
    (*hSegAndRadIter)->SetLineColor(103);
    (*hSegAndRadIter)->Draw("same");

    c1->cd(2);
    htemp->GetXaxis()->SetRange(1990,2100);
    maxval = htemp->GetMaximum(1e7);
    htemp->GetYaxis()->SetRangeUser(0,maxval);
    htemp->Draw();
    (*hGranIter)->Draw("same");
    (*hSegIter)->Draw("same");
    (*hSegAndRadIter)->Draw("same");
   
    // Compute segmentation efficiencies & errors. 
    integralAll = (*hAllIter)->Integral(lowerROI, upperROI, "");
    integralGran = (*hGranIter)->Integral(lowerROI, upperROI, "");
    integralSeg = (*hSegIter)->Integral(lowerROI, upperROI, "");
    integralSegAndRad = (*hSegAndRadIter)->Integral(lowerROI, upperROI, "");
    effAll = (Double_t) (integralAll/dROI/total);
    effGran = (Double_t) (integralGran/dROI/total);
    effSeg = (Double_t) (integralSeg/dROI/total);
    effSegAndRad =  (Double_t) (integralSegAndRad/dROI/total);
    effAllErr = effAll / TMath::Sqrt(integralAll);
    effGranErr = effGran / TMath::Sqrt(integralGran);
    effSegErr = effSeg / TMath::Sqrt(integralSeg);
    effSegAndRadErr = effSegAndRad / TMath::Sqrt(integralSegAndRad); 
    effSegList.push_back(effSeg);
    effSegList.push_back(effSegAndRad);
    effSegErrList.push_back(effSegErr);
    effSegErrList.push_back(effSegAndRadErr);
   
    // Write segmentation efficiencies on histograms as legends. 
    stringstream legendstream;
    legendstream << std::scientific;
    legendstream.precision(1);
    legendstream.str("");
    TLatex lat;
    lat.SetTextAlign(12);
    lat.SetTextSize(0.04);
    legendstream.str("");
    legendstream << "ROI: "<< effAll << "#pm" << effAllErr << "  " << dROI;
    lat.SetTextColor(kBlack);
    lat.DrawLatex(2045,maxval*0.90,(legendstream.str()).c_str());
    legendstream.str("");
    legendstream << "Decays: " << total;
    lat.DrawLatex(2045,maxval*0.96,(legendstream.str()).c_str());
    legendstream.str("");
    legendstream << "Gran cut: " << effGran << "#pm" << effGranErr;
    lat.SetTextColor(kBlue);
    lat.DrawLatex(2045,maxval*0.84,(legendstream.str()).c_str());
    legendstream.str("");
    legendstream << "Seg cut: " << effSeg << "#pm" << effSegErr;
    lat.SetTextColor(kRed);
    lat.DrawLatex(2045,maxval*0.78,(legendstream.str()).c_str());
    legendstream.str("");
    legendstream << "Seg and rad cut: " << effSegAndRad << "#pm" << effSegAndRadErr;
    lat.SetTextColor(103);
    lat.DrawLatex(2045,maxval*0.72,(legendstream.str()).c_str());
    c1->SaveAs((fNameHead+"_"+*namesIter+".pdf").c_str());
    hAllIter++; hGranIter++; hSegIter++;
    hSegAndRadIter++; namesIter++;
  }

  // Draw position resolution and polar rejection histograms.
  maxval = HSumEdep->GetMaximum(1e7);
  HSumEdep->SetLineColor(kBlack);
  HSumEdep->SetTitle("HPhi_Cut");
  HSumEdep->GetXaxis()->SetTitle("Energy (keV)");
  HSumEdep->GetXaxis()->SetRangeUser(0, 3000);
  HSumEdep->GetYaxis()->SetTitle("Counts");
  HSumEdep->GetYaxis()->SetRangeUser(0.8, maxval);
  TH1F *HTemp01 = (TH1F*) HSumEdep->Clone("HZ_Cut");
  TH1F *HTemp02 = (TH1F*) HSumEdep->Clone("HPos_Cut");
  TH1F *HTemp03 = (TH1F*) HSumEdep->Clone("HPolar_Cut");

  c2->cd(1);
  HSumEdep->Draw();
  HMultCut->SetLineColor(kBlue);
  HMultCut->Draw("same");
  HPhiCut->SetLineColor(kRed);
  HPhiCut->Draw("same");  

  c2->cd(2);
  HTemp01->SetTitle("HZ_Cut");
  HTemp01->Draw();
  HMultCut->Draw("same");
  HZCut->SetLineColor(kRed);
  HZCut->Draw("same");  

  c2->cd(3);
  HTemp02->SetTitle("HPos_Cut");
  HTemp02->Draw();
  HMultCut->Draw("same");
  HWAllCuts->SetLineColor(kRed);
  HWAllCuts->Draw("same");  

  c2->cd(4);
  HTemp03->SetTitle("HPolar_Cut");
  HTemp03->Draw();
  HMEMultCut->SetLineColor(kBlue);
  HMEMultCut->Draw("same");
  HMEWPolarCut->SetLineColor(kRed);
  HMEWPolarCut->Draw("same");  
  c2->SaveAs((fNameHead + "_pos_polar.pdf").c_str());

  std::cin >> maxval; 

  // Compute position resolution and ME efficiencies.
  Stat_t integralPos = HWAllCuts->Integral(lowerROI, upperROI, "");
  Stat_t integralPolar = HMEWPolarCut->Integral(lowerROI, upperROI, "");
  Double_t effPos = (Double_t) (integralPos/dROI/total);
  Double_t effPolar = (Double_t) (integralPolar/dROI/total);
  Double_t effPosErr = effPos / TMath::Sqrt(integralPos);
  Double_t effPolarErr = effPolar / TMath::Sqrt(integralPolar);

  // Write efficiencies to text file easy incorporation into Excel
  // Use same format as spreadsheet, ie. do some rearranging
  ofstream effFile((fNameHead+"_eff.dat").c_str(), ios::out);
  Double_t tempEff = *effSegList.begin();
  Double_t tempEffErr = *effSegErrList.begin();
  effSegList.pop_front();
  effSegErrList.pop_front();
  effSegList.push_front(effPolar);
  effSegList.push_front(effPos);
  effSegList.push_front(tempEff);
  effSegList.push_front(effAll);
  effSegErrList.push_front(effPolarErr);
  effSegErrList.push_front(effPosErr);
  effSegErrList.push_front(tempEffErr);
  effSegErrList.push_front(effAllErr);
  while(!effSegList.empty()) {
    effFile << *effSegList.begin() << '\t';
    // comment previous line and use the following line when computing for 0nubb
    // effFile << dROI * *effSegList.begin() << '\t';
    effSegList.pop_front();
  }
  effFile << '\n';
  while(!effSegErrList.empty()) {
    effFile << *effSegErrList.begin() << '\t';
    effSegErrList.pop_front();
  }
  effFile << '\n';
  effFile.close(); 
  f1.Close();
}

