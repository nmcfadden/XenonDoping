//
// e_ionization01.c
// Date: 9/23/2004
// Author R. Henning
// Generate plots characterizing electron ionization and paths in Ge.
// Used to generate plots for example 2 in UDG.
// 

{
TFile f1("testion.root");
Int_t nX, nY, nZ;
TBranch *nXBranch = fTree->GetBranch("nX");
nXBranch->SetAddress(&nX);
TBranch *nYBranch = fTree->GetBranch("nY");
nYBranch->SetAddress(&nY);
TBranch *nZBranch = fTree->GetBranch("nZ");
nZBranch->SetAddress(&nZ);
nXBranch->GetEntry(1);
nYBranch->GetEntry(1);
nZBranch->GetEntry(1);

// convert to 2D histogram
Int_t nTot = nX * nZ;
Float_t Edep[nTot];
TBranch *EdepBranch = fTree->GetBranch("Edep");
EdepBranch->SetAddress(Edep);
Int_t i, j, l;

TH2F *EdepHist = new TH2F("foop", "crap", 50, 0.25-0.005, (Float_t) 0.75-0.005, 50, -0.005, (Float_t) 0.5-0.005);
Int_t nEntries = fTree->GetEntries();
for(l = 1; l< nEntries; l++) {
  EdepBranch->GetEntry(l);
   for(i = 25; i < 75; i++){
      for(j = 0; j < 50; j++)
        EdepHist->Fill( (Float_t)i/nX,  (Float_t)j/100.,Edep[i*nZ+j]/nEntries);
  }
}

TCanvas *c1 = new TCanvas("c1", "Reyco's very own canvas",0,0,650,600);
c1->cd();
c1->SetGridx();
c1->SetGridy();

EdepHist->SetTitle("Projected Spatial Energy Deposit Distribution");
EdepHist->SetStats(kFALSE);
EdepHist->GetXaxis()->SetTitle("x (mm)");
EdepHist->GetYaxis()->SetTitle("z (mm)");
EdepHist->GetZaxis()->SetTitle("Energy Deposit (arb. units)");
EdepHist->Draw("contz");
TArrow *arr1 = new TArrow(0.5, 0., 0.5, 0.02, 0.01,">");
arr1->SetLineWidth(2);
arr1->Draw();
gStyle->SetPalette(1);
c1->SaveAs("SumEdep2D.gif");

TH2F *SEdepHist = new TH2F("foop2", "crap", nX, 0.25-0.005, 0.75- 0.005, nZ+2, 
-0.025, 0.5-0.005);
 
EdepBranch->GetEntry(4);
  for(i = 25; i < 75; i++){
      for(j = 0; j < 50; j++)
        SEdepHist->Fill((Float_t)i/100.0,(Float_t)j/100.,Edep[i*nZ+j]/nEntries);  }

SEdepHist->SetTitle("Example Projected Electron Track");
SEdepHist->SetStats(kFALSE);
SEdepHist->GetXaxis()->SetTitle("x (mm)");
SEdepHist->GetYaxis()->SetTitle("z (mm)");
SEdepHist->Draw("box");
arr1->DrawArrow(0.5, 0., 0.5, 0.02, 0.01, ">");
c1->SaveAs("SampleTrack.gif");

TCanvas *c2 = new TCanvas("c2", "Number 2", 0, 0, 700, 600);
c2->cd();
c2->SetLogy();
c2->SetGridx();
c2->SetGridy();

h3->GetXaxis()->SetTitle("mm");
h3->GetYaxis()->SetTitle("Counts");
h3->Draw();
c2->SaveAs("DistanceFromVertex.gif");

h4->GetXaxis()->SetTitle("Number Of Steps");
h4->GetYaxis()->SetTitle("Counts");
h4->Draw();
c2->SaveAs("NumberOfSteps.gif");

h7->SetTitle("Total distance travelled by primary");
h7->GetXaxis()->SetTitle("Total Distance (mm)");
h7->GetYaxis()->SetTitle("Counts");
h7->Draw();
c2->SaveAs("TotalDistance.gif");
}

  
