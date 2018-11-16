//
// LANLCLoverCo60Co57.c
// Date: 9/23/2004
// Author: Reyco Henning.
//
// Root script to generate Co57 and Co60 data from clover runs. 
// Used to generate plots for Example 1 in the UDG.
//

// Load Co57 and Co60 MC data into chains.
TChain Co57("fTree");
Co57.Add("Co57_1.root");
Co57.Add("Co57_2.root");

TChain Co60("fTree");
Co60.Add("Co60_1.root");
Co60.Add("Co60_2.root");
Co60.Add("Co60_3.root");
Co60.Add("Co60_4.root");

// Create canvas.
TCanvas *c1b = new TCanvas("c1b", "My 2nd Canvas",0,0,750,450);
c1b->cd();
c1b->SetLogy();

// Plot Generator data.

Co57.Draw("sqrt(MomentumX**2+MomentumY**2+MomentumZ**2)>>hp(500,0.,800)");
hp->GetXaxis()->SetTitle("Energy (keV)");
hp->GetYaxis()->SetTitle("Counts");   
hp->SetStats(kFALSE);   
hp->SetTitle("10^{6} ^{57}Co Generated Gammas (PNNL)");
hp->Draw();
c1b->SetGridx();
c1b->SetGridy();
TLatex lat;
lat.SetTextAlign(12);
lat.SetTextSize(0.04);
lat.DrawLatex(20,800000,"14.4");
lat.DrawLatex(130,800000,"122.1");
lat.DrawLatex(140,200000,"136.5");
lat.DrawLatex(230,3,"230.3");
lat.DrawLatex(300,270,"339.5");
lat.DrawLatex(360,100,"352.4");
lat.DrawLatex(375,10,"366.8");
lat.DrawLatex(550,300,"569.9");
lat.DrawLatex(692.0,3000,"692.0");
lat.DrawLatex(710,330,"706.4");
c1b->SaveAs("Co57Generated.gif");

hp->Delete();

Co60.Draw("sqrt(MomentumX**2+MomentumY**2+MomentumZ**2)>>hp(500,0.,2600)");
hp->GetXaxis()->SetTitle("Energy (KeV)");
hp->GetYaxis()->SetTitle("Counts");   
hp->GetYaxis()->SetRange(0.3, 2e6);
hp->SetStats(kFALSE);   
hp->SetTitle("10^{6} ^{60}Co Generated Gammas (PNNL)");
hp->Draw();
lat.DrawLatex(346,130,"346.9");
lat.DrawLatex(826,130,"826.1");
lat.DrawLatex(940,9e5,"1173.2");
lat.DrawLatex(1400,9e5,"1332.5");
lat.DrawLatex(2158.6,10,"2158.6");
c1b->SaveAs("Co60Generated.gif");


// Energy deposits from MC.
Co57.Draw("E1_MC+E2_MC+E3_MC+E4_MC>>h10(6000, 0, 3000)");
Co60.Draw("E1_MC+E2_MC+E3_MC+E4_MC>>h11(6000, 0, 3000)");
TH1F *htot_mc = new TH1F("htot_mc", "MC", 6000, 0, 3000);
htot_mc->Add(h10, 1./5.52);
htot_mc->Add(h11, 1./2.32);
htot_mc->SetTitle("Summed energy of all four crystals (MC)");
htot_mc->GetXaxis()->SetTitle("Energy (keV)");
htot_mc->GetYaxis()->SetTitle("Counts");
htot_mc->SetStats(kFALSE);
htot_mc->Draw();
lat.DrawLatex(2100,1000,"^{60}Co(1173+1333=2506)");
lat.DrawLatex(1200,30000,"^{60}Co");
lat.DrawLatex(100,40000,"^{57}Co");
c1b->SaveAs("SummedE4Xtals.gif");

Float_t  cvrt = 2.0;
htot_mc->GetXaxis()->SetRange(200*cvrt, 850*cvrt);
htot_mc->Draw();
c1b->SaveAs("ComptonSummedMC.gif");

htot_mc->GetXaxis()->SetRange(0, 200*cvrt);
htot_mc->Draw();
TArrow *arr1 = new TArrow(140,1e5,125,3e4,.01,">");
arr1->SetLineWidth(2);
arr1->Draw();
arr1->DrawArrow(140,1e5,136,3.5e3,.01,">");
lat.DrawLatex(142, 1.3e5,"^{57}Co");
lat.DrawLatex(70,1000,"Pb K-lines");
lat.DrawLatex(10,1000,"^{57}Co");
arr1->DrawArrow(40,4000,40,100,0.01,">");
lat.DrawLatex(15,7000,"^{57}Co(122KeV) Compton Edge");
c1b->SaveAs("Co57SummedMC.gif");

// chain LANL clover data.
TChain CoData("clover");
CoData.Add("../processeddata/Cobalt002/Cobalt0002.root");

Co57.Draw("E1>>h1(6000, 0 , 60000)");
Co60.Draw("E1>>h2(6000, 0 , 60000)");
CoData.Draw("E1>>h3(6000, 0 , 60000)");

TH1F *htot = new TH1F("htot", "MC", 6000, 0, 60000);
htot->Add(h1, 1./5.52);
htot->Add(h2, 1./2.32);

htot->GetXaxis()->SetTitle("Energy (Channel Number)");
htot->GetYaxis()->SetTitle("Counts");      
htot->SetTitle("E1 (Data is in Red)");
htot->SetStats(kFALSE);
htot->SetMaximum(2000);
htot->Draw();
h3->SetLineColor(kRed);
h3->SetStats(kFALSE);
h3->Draw("same");
lat.DrawLatex(48000,4,"^{60}Co(2506)");
lat.DrawLatex(22000,600,"^{60}Co");
lat.DrawLatex(4000,1000,"^{57}Co");
c1b->SaveAs("FullSpectra.gif");

htot->GetXaxis()->SetRange(1,2600);
htot->Draw();
h3->Draw("same");
lat.DrawLatex(23000,500,"^{60}Co");
lat.DrawLatex(3000,1000,"^{57}Co");
c1b->SaveAs("PrimarySpectra.gif");

htot->GetXaxis()->SetRange(2100,2600);
htot->SetTitle("E1 (Co60 peaks)");
htot->Draw();
h3->Draw("same");
lat.DrawLatex(22000,600,"^{60}Co (1173 keV)");
lat.DrawLatex(25000,600,"^{60}Co (1333 keV)");
c1b->SaveAs("Co60Peaks.gif");

htot->GetXaxis()->SetRange(100,350);
htot->SetTitle("E1 (Co57 peaks + Pb X-Rays)");
htot->Draw();
h3->Draw("same");
lat.DrawLatex(2360,1000,"^{57}Co (122keV)");
lat.DrawLatex(2620,180,"^{57}Co (136.5keV)");
lat.DrawLatex(1500,220,"Pb K-shell X-Rays");
c1b->SaveAs("Co57Peaks.gif");

// -----------------------------------------------------------
