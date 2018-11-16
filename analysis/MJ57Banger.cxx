.L analysis/libROOT.so

// Prepare Raw MC data
TChain rawMC("fTree");  
rawMC.Add("/auto/majorana1/MJ/data/MJ57Banger/Ge68L1C18.root");
MGOutputG4StepsData *g4Steps = new MGOutputG4StepsData();
rawMC->SetBranchStatus("*", 1);
rawMC->SetBranchAddress("RawMC", &g4Steps);

// Prepare Crystal Data.
TChain detector("DetectorTree");
detector.Add("/auto/majorana1/MJ/data/MJ57Banger/Ge68L1C18.root");
MJOutputDetectorEventData   *detectorData = new MJOutputDetectorEventData();
detector.SetBranchStatus("*", 1);
detector.SetBranchAddress("Detector_hits", &detectorData);

//Define historgrams
TH2F hedept("hedept", "hedept", 5, -0.5, 4.5, 3000, -0.5, 2999.5);
TH1F hedep("hedep", "hedep", 3000, -0.5, 2999.5);
TH1F hedep_onec("hedep_onec", "hedep_onec", 3000, -0.5, 2999.5);
TH1F hNXtals("hNXtals", "hNXtals", 20, -0.5, 19.5);
TH1F hfoop("hfoop", "hfoop", 20, -0.5, 19.5);

Int_t nentries = detector.GetEntries();
{
  MJOutputSegXtal* xtal;
  Int_t nhit;
  Int_t MCEventNum = 0;
  Int_t foop=0;
  Double_t smearedEdep;
  for(Int_t i = 0; i < nentries; i++) {
    detector->GetEntry(i);
    nhit = 0;
    for(Int_t j = 0; j < detectorData->fNumberOfCrystals; j++) {
      xtal = ((MJOutputSegXtal*)((*detectorData->fCrystals)[j]));
      if(xtal->fEdep > 5.0) nhit++;
    }
    if(detectorData->fNumberOfCrystals > 0) {
      xtal = ((MJOutputSegXtal*)((*detectorData->fCrystals)[0]));
      smearedEdep = gRandom->Gaus(xtal->fEdep, 0.4 + 0.025*TMath::Sqrt(xtal->fEdep));
      if(nhit == 1) 
	hedep_onec->Fill(smearedEdep);
      hedep->Fill(smearedEdep);
      hedept->Fill(foop, smearedEdep);
    }
    if(MCEventNum < detectorData->fMCEventNum) {
      hfoop->Fill(foop);
      foop = 1;
      MCEventNum = detectorData->fMCEventNum;
    } else
      foop++;
    hNXtals->Fill(nhit);
  }
}

TCanvas *c1 = new TCanvas("c1", "Blah", 0, 0, 809, 500);
c1->cd();
c1->SetGridx();
c1->SetGridy();
gStyle->SetOptStat(0);

hedep->SetTitle("^{76}Ge->^{76}Ga Spectrum L1C18");
hedep->GetXaxis()->SetTitle("Energy (keV)");
hedep->GetYaxis()->SetTitle("Counts");
hedep->GetXaxis()->SetRange(20,3000);
hedep->Draw();
hedep_onec->SetLineColor(kRed);
hedep_onec->Draw("same");
TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
leg->AddEntry(&hedep, "All events", "l");
leg->AddEntry(&hedep_onec, "Single crystal events", "l");
leg->Draw();
c1->SaveAs("Ge68L1C18_all.gif");

hedep->GetXaxis()->SetRange(2000,2080);
hedep->Draw();
hedep_onec->SetLineColor(kRed);
hedep_onec->Draw("same");
Int_t ROIint = hedep->Integral(2033, 2043);
Int_t ROIint_err = TMath::Sqrt(ROIint);
Int_t ROIint_onec = hedep_onec->Integral(2033, 2043);;
Int_t ROIint_onec_err = TMath::Sqrt(ROIint_onec);
stringstream counts;
counts << ROIint << "#pm" << ROIint_err << " counts/10^{5} events/keV";
TLatex lat;
lat.SetTextAlign(12);
lat.SetTextSize(0.04);
string countsStr = counts.str();
lat.DrawLatex(2042,27,countsStr.c_str());
counts.str("");
counts << ROIint_onec << "#pm" << ROIint_onec_err << " counts/10^{5} events/keV";
countsStr = counts.str();
lat.SetTextColor(kRed);
lat.DrawLatex(2042,2,countsStr.c_str());
c1->SaveAs("Ge68L1C18_ROI.gif");

hNXtals->GetXaxis()->SetTitle("Number of Crystals with Edep > 5 keV");
hNXtals->GetYaxis()->SetTitle("Counts");
hNXtals->SetTitle("Crystal Multiplicity");
c1->SetLogy(true)
hNXtals->Draw();
c1->SaveAs("Ge68L1C18_NXtals.gif");

