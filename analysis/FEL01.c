// 
// FEL01.c
// Date: 9/22/2004
// Author: R. Henning
// 
// Root script to analyze FEL simulation data.
//  - Characterizes FEL generator
//  - Efficiency of LANL Clover in gamma detection.
//  - Number of DEP events generated. 
//  Used to generate plots for Example 3 in UDG.
// 


// You may want to change the name of the input files. 
TChain chain("fTree");
chain.Add("testFEL30_1.root");
chain.Add("testFEL30_2.root");
chain.Add("testFEL30_3.root");
chain.Add("testFEL30_4.root");
chain.Add("testFEL30_5.root");

//TFile f1("testFEL.root");
TCanvas *c1 = new TCanvas("c1", "Blah", 0, 0, 809, 500);

// Momentum distribution.
c1->cd();
c1->SetGridx();
c1->SetGridy();
chain->Draw("abs(MomentumZ)>>hmom(120,2980,3100)");
hmom->GetXaxis()->SetTitle("Initial Momentum (keV/c)");
hmom->GetYaxis()->SetTitle("Counts");
hmom->SetTitle("Momentum distributions of FEL gammas");
c1->SetLogy();
hmom->Draw();
gStyle->SetOptStat(1110);
c1->SaveAs("OriginalPz.gif");

// Beam profile.
TCanvas *c2 = new TCanvas("c2", "Blah", 0, 0, 500, 500);
c2->cd();
c2->SetLogy(0);
gStyle->SetPalette(1);
chain->Draw("PositionX:PositionY>>hpos(60,-1,5,60,-1,5)");
hpos->SetTitle("Beam profile");
hpos->GetXaxis()->SetTitle("x position (cm)");
hpos->GetYaxis()->SetTitle("y position (cm)");
hpos->GetZaxis()->SetTitle("Counts");
hpos->Draw("conto");
gStyle->SetOptStat(1110);
c2->SaveAs("BeamProfile.gif");

c1->cd();
c1->SetLogy(1);
chain->Draw("PositionX>>hposprof(70,-2,5)");
hposprof->SetTitle("Beam Profile (along X)");
hposprof->GetXaxis()->SetTitle("Vertex x Position (cm)");
hposprof->GetYaxis()->SetTitle("Counts");
hposprof->Draw();
gStyle->SetOptStat(1110);
c1->SaveAs("BeamProfileProj.gif");

// Energy deposit.
c1->cd();
c1->SetLogy(1);
chain->Draw("E1_MC+E2_MC+E3_MC+E4_MC>>hemctot(1650,1.,3300.)");
hemctot->SetTitle("Summed energy deposits in all crystals");
hemctot->GetXaxis()->SetTitle("Energy Deposit (keV)");
hemctot->GetYaxis()->SetTitle("Counts");
hemctot->SetStats(kFALSE);
hemctot->Draw();
c1->SaveAs("EdepMC.gif");

// Efficiency
c1->SetLogy(0);
chain->Draw("(E1_MC+E2_MC+E3_MC+E4_MC)==0>>heff(2,-0.5,1.5)");
heff->SetTitle("Interaction Rates");
heff->GetYaxis()->SetTitle("Counts");
heff->GetXaxis()->SetTitle("0: No Ge interaction | 1: Ge Interaction");
heff->Draw();
gStyle->SetOptStat(10);
TLatex lat;
lat.SetTextAlign(12);
lat.SetTextSize(0.04);
lat.DrawLatex(-0.4, 630000, "Energy deposit in Ge == 0");
lat.DrawLatex(-0.4, 590000, "68\% of Gammas");
lat.DrawLatex(0.6, 260000, "Energy deposit in Ge >= 0");
lat.DrawLatex(0.6, 220000, "32\% of Gammas");
c1->SaveAs("InteractionRates.gif");

// Edep in NaI barrel w/o Gaussian noise.
c1->SetLogy(1);
chain->Draw("TotEdepNaI>>hedepnaimc(1100,1,3300)");
hedepnaimc->SetTitle("Energy Deposit in NaI Barrel");
hedepnaimc->GetXaxis()->SetTitle("Energy (keV)");
hedepnaimc->GetYaxis()->SetTitle("Counts");
hedepnaimc->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("TotEdepNaIMC.gif");

// Edep in NaI barrel w/ Gaussian noise for NaI detector added.

TH1F *hedepnai = new TH1F("hedepnai", "Energy Deposit in NaI w/ Gaussian Noise",1100, 1., 3300.);
TH1F *hedepnai2 = new TH1F("hedepnai2", "Energy Deposit in NaI w/ Gaussian Noise",1100, 1., 3300.);
TH1F *hsegmedep = new TH1F("hsegmedep", "Energy Deposit in NaI",1100, 1., 3300.);
TH1F *hedepwcuts = new TH1F("hedepwcuts", "Energy Deposit in Ge w/ NaI Barrel Cuts.", 1100, 1., 3300.);
TH1F *hedepwcuts2 = new TH1F("hedepwcuts2", "Energy Deposit in Ge w/ NaI Barrel Cuts.", 1100, 1., 3300.);
TH1F *hedepNaIwcuts = new TH1F("hedepNaIwcuts", "Energy Deposit in NaI w/ NaI Barrel Cuts.", 1100, 1., 3300.);
TH1F *hsegmdistr = new TH1F("hsegmdistr", "Energy distribution among NaI segments", 16, -0.5, 15.5);
TH1F *hnhits = new TH1F("hnhits", "Number of NaI Segments hit", 17, -0.5, 16.5);
TH1F *hnvetohits = new TH1F("hnvetohits", "Number of Veto hits", 9, -0.5, 8.5);


Float_t edep, E1_MC, E2_MC, E3_MC, E4_MC;
Float_t segmedep[16];
chain->SetBranchStatus("*", 1);
chain->SetBranchAddress("TotEdepNaI", &edep);
chain->SetBranchAddress("SegEdep", segmedep);
chain->SetBranchAddress("E1_MC", &E1_MC);
chain->SetBranchAddress("E2_MC", &E2_MC);
chain->SetBranchAddress("E3_MC", &E3_MC);
chain->SetBranchAddress("E4_MC", &E4_MC);

Int_t nentries = (Int_t) chain->GetEntries();
Float_t smearedEdep;
Int_t nvetohits, nhits;
Int_t j;
Float_t thresh = 10.0;

cout << "Number of Events: " << nentries << "\n";
{
  for(Int_t i = 0; i < nentries; i++) {
    chain->GetEntry(i);
    hedepnai2->Fill(edep);
    if(edep >= 0.001) {
      smearedEdep=gRandom->Gaus(edep,edep*exp(-2.0-0.5*log(edep/511.0))/2.35);
      hedepnai->Fill(smearedEdep);
      for(j = 0, nhits = 0; j < 16; j++) {
	hsegmdistr->Fill(j, segmedep[j]);
	hsegmedep->Fill(segmedep[j]);
	if(segmedep[j] > thresh) {
	  nhits++;
	}
      }
      hnhits->Fill(nhits);
      for(j = 0, nvetohits = 0; j < 8; j++) {
	if(segmedep[j] > thresh && segmedep[j+8] > thresh){
	  nvetohits++;
        }
      }
      hnvetohits->Fill(nvetohits);
      if(nvetohits == 1 && nhits == 2) {
	hedepwcuts->Fill(E1_MC+E2_MC+E3_MC+E4_MC);
	hedepNaIwcuts->Fill(smearedEdep);
	if(smearedEdep > 900. && smearedEdep < 1100.){
	  hedepwcuts2->Fill(E1_MC+E2_MC+E3_MC+E4_MC);
	}
      }
    } else {
      hedepnai->Fill(edep);
    }
  }
}

// Energy deposit in NaI barrel with Gaussian noise.
hedepnai->GetYaxis()->SetTitle("Counts");
hedepnai->GetXaxis()->SetTitle("Energy (keV)");
hedepnai->Draw();
gStyle->SetOptStat(10);

c1->SaveAs("TotEdepNaIBarrelwNoise.gif");

// Number of NaI crystals hit.
hnhits->GetYaxis()->SetTitle("Counts");
hnhits->GetXaxis()->SetTitle("Number of NaI crystals hit");
hnhits->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("NSegmentsHits.gif");

// Number of "veto" hits.
hnvetohits->GetXaxis()->SetTitle("Number of veto hits");
hnvetohits->GetYaxis()->SetTitle("Counts");
hnvetohits->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("NVetoHits.gif");

// Energy Deposit with nvetohit == 1 && nhits = 2
hedepwcuts->GetXaxis()->SetTitle("Energy (keV)");
hedepwcuts->GetYaxis()->SetTitle("Counts");
hedepwcuts->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("EdepwCuts1.gif");

// Energy Deposit in NaI with nvetohit == 1 && nhits = 2
hedepNaIwcuts->GetXaxis()->SetTitle("Energy (keV)");
hedepNaIwcuts->GetYaxis()->SetTitle("Counts");
hedepNaIwcuts->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("EdepNaIwCuts1.gif");

// Energy Deposit with nvetohit == 1 && nhits = 2 && NaI energy range.
hedepwcuts2->GetXaxis()->SetTitle("Energy (keV)");
hedepwcuts2->GetYaxis()->SetTitle("Counts");
hedepwcuts2->Draw();
gStyle->SetOptStat(10);
c1->SaveAs("EdepwCuts2.gif");

hedepwcuts2->GetXaxis()->SetRange(1950/3,2250/3);
hedepwcuts2->Draw();
gStyle->SetOptStat(10);
TH1F *hclone = (TH1F*) hedepwcuts2->Clone();
hclone->GetXaxis()->SetRange(1990/3,2049/3);
hclone->SetFillColor(kBlue); 
hclone-SetStats(kFalse);
hclone->Draw("same");
TArrow *arr1 = new TArrow(1990,100,1990,30,.01,">");
arr1->SetLineWidth(2);
arr1->Draw();
arr1->DrawArrow(2050,100,2050,30,.01,">");
lat.DrawLatex(2100,60,"770 Events in Selected Range");
c1->SaveAs("EdepwCuts2Zoom.gif");


// ------------------------------------------------------------------- //


