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
 * $Id: TestDriftVelocity.cc,v 1.3 2008-08-19 13:53:08 jliu Exp $
 *
 * DESCRIPTION:
 *
 * 	for the test of MGDriftVelocity class in MaGe/waveform 
 * 
 * Example Usage:
 *
 *
 * AUTHOR: 
 * 	Jing Liu
 * 
 * REVISION:
 * 	13-05-2008, Created, Jing Liu
 * 	08-07-2008, Changed to test MGDriftVelocity in MaGe/waveform, Jing.
 *
 */

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "io/MGLogger.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"
#include "waveform/MGSORCrystalFields.hh"

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TROOT.h"
#include "TLatex.h"

int main()
{
//   MGLogger::SetSeverity(MGLogger::debugging);
   MGLogger::SetSeverity(MGLogger::routine);

   MGCrystalData* crystal = new MGCrystalData(
	 0, 				// crystal ID
	 MGCrystalData::kNType,
	 MGCrystalData::kCoaxial,
	 5.0*CLHEP::mm,  		// inner radius
	 37.5*CLHEP::mm, 		// outer radius
	 70.0*CLHEP::mm, 		// height
	 70.0*CLHEP::mm, 		// well depth
	 0.0, 				// global x
	 0.0, 				// global y
	 0.0, 				// global z
	 3,   				// Nseg in z
	 6);  				// Nseg in phi
   crystal->SetCrystalAxesAngle(0.0*CLHEP::degree);
   crystal->SetSegmentationAngle(0.0*CLHEP::degree);

   MGSORCrystalFields* fields = new MGSORCrystalFields(*crystal);
   fields->SetFileName("TestFileName");
//   fields->SetFileName("/remote/pclg-13/jingliu/data/FirstTests");
   fields->LoadFields();

   MGBruyneelDriftVelocity drift(*fields);

   MGLog(routine)<<"Test calculations:"<<endlog;
   const int n=900;
   double Emag, Eval[n];
   double V100e[n], V111e[n], V110e[n];
   double V100h[n], V111h[n], V110h[n];
   for (int i=0; i<n; i++){
      Eval[i] = (i+3)*1.; // value without unit
      Emag = Eval[i]*(CLHEP::volt/CLHEP::mm); // value with unit
      CLHEP::Hep3Vector eVector(0.,Emag,0.);

      V100e[i] = drift.V100e(Emag)/(CLHEP::mm/CLHEP::ns);
      V111e[i] = drift.V111e(Emag)/(CLHEP::mm/CLHEP::ns);
      V110e[i] = drift.GetVe(eVector).mag()/(CLHEP::mm/CLHEP::ns);

      V100h[i] = drift.V100h(Emag)/(CLHEP::mm/CLHEP::ns);
      V111h[i] = drift.V111h(Emag)/(CLHEP::mm/CLHEP::ns);
      V110h[i] = drift.GetVh(eVector).mag()/(CLHEP::mm/CLHEP::ns);
   }

   gROOT->SetStyle("Plain");
   TCanvas *c1 = new TCanvas ("c1","V vs. E",0,0,1600,800);
   c1->Divide(2);

   TGraph *v100e = new TGraph(n,Eval,V100e);
   TGraph *v110e = new TGraph(n,Eval,V110e);
   TGraph *v111e = new TGraph(n,Eval,V111e);
   TGraph *v100h = new TGraph(n,Eval,V100h);
   TGraph *v110h = new TGraph(n,Eval,V110h);
   TGraph *v111h = new TGraph(n,Eval,V111h);

   c1->cd(1);
   gPad->SetLogy();
   gPad->SetLogx();
   v100e->SetTitle("Electric drift velocities versus electric field");
   v100e->GetXaxis()->SetTitle("E (V/mm)");
   v100e->GetXaxis()->CenterTitle();
   v100e->GetYaxis()->SetTitle("Velocity (mm/ns)");
   v100e->GetYaxis()->CenterTitle();
   v100e->GetYaxis()->SetRangeUser(1e-2, 1.5e-1);

   v100e->SetMarkerStyle(20);
   v100e->SetMarkerSize(0.2);
   v100e->SetMarkerColor(kBlue);
   v100e->Draw("apl");

   v111e->SetMarkerStyle(21);
   v111e->SetMarkerSize(0.2);
   v111e->SetMarkerColor(kRed);
   v111e->Draw("pl");

   v110e->SetMarkerStyle(22);
   v110e->SetMarkerSize(0.2);
   v110e->SetMarkerColor(kGreen);
   v110e->Draw("pl");

   TLegend lege(0.6, 0.5, 0.85, 0.65);
   lege.AddEntry(v100e,"velocity along 100","p");
   lege.AddEntry(v110e,"velocity along 110","p");
   lege.AddEntry(v111e,"velocity along 111","p");
   lege.SetLineColor(kWhite);
   lege.SetFillColor(kWhite);
   lege.Draw();

   c1->cd(2);
   gPad->SetLogy();
   gPad->SetLogx();
   v100h->SetTitle("Hole drift velocities versus electric field");
   v100h->GetXaxis()->SetTitle("E (V/mm)");
   v100h->GetXaxis()->CenterTitle();
   v100h->GetYaxis()->SetTitle("Velocity (mm/ns)");
   v100h->GetYaxis()->CenterTitle();
   v100h->GetYaxis()->SetRangeUser(1e-2, 1.5e-1);

   v100h->SetMarkerStyle(20);
   v100h->SetMarkerSize(0.2);
   v100h->SetMarkerColor(kBlue);
   v100h->Draw("apl");

   v111h->SetMarkerStyle(21);
   v111h->SetMarkerSize(0.2);
   v111h->SetMarkerColor(kRed);
   v111h->Draw("pl");

   v110h->SetMarkerStyle(22);
   v110h->SetMarkerSize(0.2);
   v110h->SetMarkerColor(kGreen);
   v110h->Draw("pl");

   TLegend legh(0.6, 0.5, 0.85, 0.65);
   legh.AddEntry(v100h,"velocity along 100","p");
   legh.AddEntry(v110h,"velocity along 110","p");
   legh.AddEntry(v111h,"velocity along 111","p");
   legh.SetLineColor(kWhite);
   legh.SetFillColor(kWhite);
   legh.Draw();

   c1->Print("driftVvsE.eps");

   CLHEP::Hep3Vector hit;
//   hit.setRhoPhiZ(10*CLHEP::mm,-CLHEP::halfpi,0*CLHEP::mm);
   hit.setRhoPhiZ(37.5*CLHEP::mm,CLHEP::pi/4,-35*CLHEP::mm);
   MGLog(routine)<<"Vx = "<<drift.GetVeAt(hit).x()<<endlog;
   MGLog(routine)<<"Vy = "<<drift.GetVeAt(hit).y()<<endlog;
   MGLog(routine)<<"Vz = "<<drift.GetVeAt(hit).z()<<endlog;
   MGLog(routine)<<"V = "<<drift.GetVeAt(hit).mag()<<endlog;

   delete fields;
   delete v100e;
   delete v110e;
   delete v111e;
   delete v100h;
   delete v110h;
   delete v111h;
   delete c1;
   delete crystal;

   return 0;
}

