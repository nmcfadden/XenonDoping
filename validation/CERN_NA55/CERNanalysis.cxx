#include <iostream>
#include <iomanip>
#include <sstream>
#include <TNamed.h>
#include <TParameter.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1F.h>

using namespace std;

int main(int argc, char *argv[])
{
    Double_t pi = TMath::Pi();
    Double_t density;
    Double_t x,y;
    stringstream cutString,filePath1,filePath2,filePath3,plotName,outPath,titleString;
    Double_t angLo;
    Double_t angHi;
    Double_t dCos;
    ULong_t nMu = 0;
    Double_t scale;
    Double_t cosPlotRangeUpper,cosPlotRangeLower;
    
    if (strcmp(argv[1],"Copper")==0){
        density = 2.12;
        cosPlotRangeLower = 1e-4;
        cosPlotRangeUpper = 4e-3;
        cout << "drawing plots for copper" << endl;
    }
    else if(strcmp(argv[1],"Graphite")==0){
        density = 8.52;
        cosPlotRangeLower = 9e-6;
        cosPlotRangeUpper = 4e-4;
        cout << "drawing plots for graphite" << endl;
    }
    else if(strcmp(argv[1],"Lead")==0){
        density = 0.33;
        cosPlotRangeLower = 2e-4;
        cosPlotRangeUpper = 4e-2;
        cout << "drawing plots for lead" << endl;
    }
    else{
        cout << "Not a valid material" << endl;
        return 0;
    }
    string material = argv[1];
    filePath1 << "./" << material << "/cern*.root";
    filePath2 << "/eliza10/majorana/old/majorana1/users/mgmarino/neutrons/CERNBD/" << material << "/MJCERN*root";
    filePath3 << "/eliza10/majorana/old/majorana1/users/mgmarino/neutrons/CERNBD/" << material << "/" << material << "Plots.root";
    
    TFile * fp = new TFile(filePath3.str().c_str());
    TChain * ch = new TChain("CERN_NA55Tree");
    ch->Add(filePath1.str().c_str());
    TChain * chmgm = new TChain("CERN_NA55Tree");
    chmgm->Add(filePath2.str().c_str());
    
    TObjArray *fileElements=ch->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    while ( chEl=(TChainElement*)next() ) {
        TFile f(chEl->GetTitle());
        TParameter<long> * numEvents = (TParameter<long> *)f.Get("SuccessfulEvents");
        nMu += numEvents->GetVal();
    }        
    cout << "number of events " << nMu << endl;
    for(Double_t angle = 45;angle<140; angle+=45){
        angLo = angle-2;
        angHi = angle+2;
        dCos = 2*pi*(cos(angLo*pi/180)-cos(angHi*pi/180));
    
        cutString.str("");
        plotName.str("");
        outPath.str("");
        titleString.str("");
        
        cutString << "E>10 && ";
        cutString << "cosTheta<cos(" << angLo << "*pi/180) &&";
        cutString << "cosTheta>cos(" << angHi << "*pi/180)";
        
        plotName << angle << "Deg";
        outPath << "./plots/" << material << "_" << angle << ".pdf";
        titleString << material;
        titleString << ", #theta = " << angle << "^{o}";

        ch->Draw("E_MeV>>h(210,10,2110)",cutString.str().c_str(),"goff");

        TCanvas * cnew = new TCanvas("cnew");
        cnew->SetLogx(1);
        cnew->SetLogy(1);

        scale = 1/dCos/nMu/density;
        
        TH1D * h = (TH1D *) gDirectory->Get("h");
        h->Sumw2();
        h->Scale(scale);
        h->SetStats(0);
        h->SetTitle(titleString.str().c_str());
        h->SetLineColor(kBlue);
        h->GetXaxis()->SetRangeUser(10,1000);
        h->GetXaxis()->SetTitleOffset(1.2);    
        h->GetXaxis()->SetTitle("E (MeV)");    
        h->GetYaxis()->SetRangeUser(1.1e-6,4e-3);
        h->GetYaxis()->SetTitleOffset(1.2);    
        h->GetYaxis()->SetTitle("d^{2}#sigma/d#Omega/dE (barn/sr/10MeV)");
        h->Draw("EP");

        chmgm->Draw("E_MeV>>hmgm(210,10,2110)",cutString.str().c_str(),"goff");
        chmgm->Draw("cosTheta>>hcosmgm(200,-1,1)","E_MeV>10","goff");

        scale = 1/dCos/1e7/density;
        TH1D * hmgm = (TH1D *) gDirectory->Get("hmgm");
        hmgm->Sumw2();
        hmgm->Scale(scale);
        hmgm->SetStats(0);
        hmgm->SetLineColor(kRed);
        hmgm->Draw("EPsame");
        
        TGraph * grexp = (TGraph *)fp->Get(plotName.str().c_str());
        if (!grexp){
            cout << "grexp not found" << endl;
        }
        grexp->SetName("grexp");
        grexp->SetMarkerStyle(20);
        for(int i=0;i<grexp->GetN();i++){
            grexp->GetPoint(i,x,y);
            grexp->SetPoint(i,pow(10,x),y);
        }
        grexp->Draw("P");

        TLegend * leg = new TLegend(0.65,0.7,0.85,0.8);
        leg->AddEntry("h","GEANT4 9.5","l");
        leg->AddEntry("hmgm","GEANT4 8.0","l");
        leg->AddEntry(grexp, "Experiment","p");
        leg->SetFillColor(0);
        leg->SetLineColor(0);
        leg->Draw("same");
    
        cnew->SaveAs(outPath.str().c_str());
    }
    cutString.str("");
    outPath.str("");
    titleString.str("");
    
    cutString << "E>10";
    outPath << "./plots/" << material << "_Cos.pdf";
    titleString << material;
    
    ch->Draw("E_MeV>>h(210,10,2110)",cutString.str().c_str(),"goff");
    ch->Draw("cosTheta>>hcos(200,-1,1)","E_MeV>10","goff");
    chmgm->Draw("cosTheta>>hcosmgm(200,-1,1)","E_MeV>10","goff");
    
    dCos = 2*pi/100;
    scale = 1/dCos/nMu/density;
    
    TCanvas * cnew = new TCanvas("cnew");
    cnew->SetLogy(1);
    
    TH1D * hcos = (TH1D *) gDirectory->Get("hcos");
    hcos->SetTitle(material.c_str());
    hcos->SetLineColor(kBlue);
    hcos->SetStats(0);
    hcos->Sumw2();
    hcos->Scale(scale);
    hcos->Draw("EP");
    hcos->GetXaxis()->SetTitle("Cos(#theta)");
    hcos->GetYaxis()->SetTitle("d#sigma/d#Omega (barn/sr)");
    hcos->GetYaxis()->SetRangeUser(cosPlotRangeLower,cosPlotRangeUpper);
    
    scale = 1/dCos/1e7/density;
    TH1D * hcosmgm = (TH1D *) gDirectory->Get("hcosmgm");
    hcosmgm->SetStats(0);
    hcosmgm->Sumw2();
    hcosmgm->Scale(scale);
    hcosmgm->SetLineColor(kRed);
    hcosmgm->Draw("EPsame");
    
    TGraphErrors * grexp = (TGraphErrors *)fp->Get("CosTheta");
    grexp->SetName("grexp");
    grexp->SetMarkerStyle(20);
    grexp->Draw("P");
    
    TLegend * leg = new TLegend(0.65,0.2,0.85,0.3);
    leg->AddEntry("hcos","GEANT4 9.5","l");
    leg->AddEntry("hcosmgm","GEANT4 8.0","l");
    leg->AddEntry(grexp, "Experiment","p");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw("same");

    cnew->SaveAs(outPath.str().c_str());

}
