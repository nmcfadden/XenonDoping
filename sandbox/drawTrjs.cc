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
 * $Id: drawTrjs.cc,v 1.1 2008-08-29 09:45:48 jliu Exp $
 *
 * DESCRIPTION:
 *
 * 	a root script to draw the drift trjs. 
 * 
 * Example Usage:
 *
 *
 * AUTHOR: 
 * 	Jing Liu
 * 
 * REVISION:
 * 	29-08-2008, Created, Jing Liu
 *
 */
using namespace TMath;

void drawTrjs(char *filename="trj.root")
{
   TFile file(filename,"READ");
   const Int_t Nhits = file.GetNkeys();
   TTree* hit[Nhits];
   for (Int_t i=0; i<Nhits; i++) {
      file.GetObject(Form("hit%d",i), hit[i]);
   }

   gROOT->SetStyle("Plain");
   TCanvas* c1 = new TCanvas("c1","Drift Trajectories",0,0,1600,800);
   c1->Divide(2);

   const Int_t M = 361;
   Double_t cox[M],coy[M], cix[M],ciy[M];
   for (Int_t i=0; i<M; i++) {
      cox[i] = 37.5*cos(TwoPi()*i/(M-1));
      coy[i] = 37.5*sin(TwoPi()*i/(M-1));
      cix[i] = 5.*cos(TwoPi()*i/(M-1));
      ciy[i] = 5.*sin(TwoPi()*i/(M-1));
   }
   TGraph* g1 = new TGraph(M, cox, coy);
   TGraph* g2 = new TGraph(M, cix, ciy);
   
   c1->cd(1);
   g1->Draw("al");
   g1->SetTitle("");
   g1->GetXaxis()->SetTitle("x [mm]");
   g1->GetXaxis()->CenterTitle();
   g1->GetYaxis()->SetTitle("y [mm]");
   g1->GetYaxis()->CenterTitle();
   g2->Draw("l");

   for (Int_t i=0; i<Nhits/3; i++) {
      hit[i]->Draw("htrj[1]:htrj[0]","","same");
   }
   
   c1->cd(2);
   g1->Draw("al");
   g2->Draw("l");
   for (Int_t i=Nhits/3; i<Nhits*2/3; i++) {
      hit[i]->Draw("etrj[1]:etrj[0]","","same");
   }

   TCanvas* c2 = new TCanvas("c2","Drift Trajectories",0,0,800,800);
   g1->Draw("al");
   g2->Draw("l");
   for (Int_t i=Nhits*2/3; i<Nhits; i++) {
      hit[i]->Draw("etrj[1]:etrj[0]","","same");
      hit[i]->Draw("htrj[1]:htrj[0]","","same");
   }

}
