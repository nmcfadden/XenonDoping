#include "/auto/u/rhenning/majorana/code/MaGe/io/io/MGOutputG4StepsData.hh"
#include "TChain.h"
#include "TBranch.h"
#include "TTree.h"
#include "TObject.h"
#include "TH2F.h"

#include <fstream>
#include <iostream>


Int_t MSUtext(TH2F *h2, TH1F *hspectra)
{

// Co56

TChain myChain("fTree");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co60_0.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co60_1.root");
/*myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_2.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_3.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_4.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_5.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_6.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_7.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_8.root");
myChain.Add("/auto/majorana1/MJ/data/MSU/Co56_9.root");
*/
MGOutputG4StepsData  *myDataPtr = new MGOutputG4StepsData();
myChain.SetBranchStatus("*", 0);
myChain.SetBranchStatus("fX", 1);
myChain.SetBranchStatus("fY", 1);
myChain.SetBranchStatus("fZ", 1);
myChain.SetBranchStatus("fEdep", 1);
myChain.SetBranchStatus("fStatus01", 1);
myChain.SetBranchStatus("fTotalNumberOfSteps", 1);

myDataPtr->fX = new Float_t[1000];
myDataPtr->fY = new Float_t[1000];
myDataPtr->fZ = new Float_t[1000];
myDataPtr->fEdep = new Float_t[1000];
myDataPtr->fStatus01 = new Int_t[1000];

myChain.SetBranchAddress("RawMC", &myDataPtr);
//std::ofstream out;
//out.open("/auto/majorana1/users/rhenning/MSU/Co56Edep.txt");
Int_t nentries = (Int_t) myChain.GetEntries();
//std::cout << "N entries: " << nentries << std::endl;
Bool_t rhitt;
Int_t k = 0;
 Double_t edep;
for(Int_t i = 0; i < nentries; i++) { 
    myChain.GetEntry(i);
    rhitt = false;
    if(i % 10000 == 0)
          std::cout << "Event: " << i << '\n';
    edep = 0;
    for(Int_t j = 0; j < myDataPtr->fTotalNumberOfSteps; j++) 
      if(myDataPtr->fEdep[j] > 0. && myDataPtr->fStatus01[j] !=0) {
	//	out << i  << " " << myDataPtr->fX[j]  << " "<<myDataPtr->fY[j]<<" "; 
	//out << myDataPtr->fZ[j] << " " << myDataPtr->fEdep[j] << std::endl; 
	h2->Fill(myDataPtr->fX[j], myDataPtr->fY[j]); 
	rhitt = true;
	edep += myDataPtr->fEdep[j];
      }
    if(rhitt) k++;
    hspectra->Fill(edep);
  }
//out.close();
// std::cin >> k; 
return k;
}

/*
 TH2F *h2 = new TH2F("h2", "h2", 100, -6.0, 6.0, 200, -6.0, 6.0);
 TH1F *hspectra = new TH1F("hs", "hs", 4500, -0.5, 4499.5);

TCanvas *c1 = new TCanvas("c1", "Blah", 0, 0, 809, 500);
c1->cd();
c1->SetGridx();
c1->SetGridy();
gStyle->SetOptStat(0);

c1->SetLogy();
hspectra->SetTitle("100 000 ^{60}Co Source Decays");
hspectra->GetXaxis()->SetTitle("Energy (keV)");
hspectra->GetYaxis()->SetTitle("Counts");
hspectra->GetXaxis()->SetRange(0,2600);
hspectra->Draw();
c1->SaveAs("MSUCo60Spectra.gif")

TCanvas *c2 = new TCanvas("c2", "Blah2", 0, 0, 500, 500);
c2->SetLogy(false);
h2->SetTitle("^{60}Co Hit Distribution");
h2->GetXaxis()->SetTitle("X (cm)");
h2->GetYaxis()->SetTitle("Y (cm)");
gStyle->SetPalette(1);
h2->Draw("cont1");
c2->SaveAs("MSYXYCo60.gif");


*/
