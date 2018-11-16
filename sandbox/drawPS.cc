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
 * $Id: drawPS.cc,v 1.2 2008-06-13 13:01:42 jliu Exp $
 *
 * DESCRIPTION:
 *
 * 	for the test of MGElectricField, MGDriftVelocity and MGDriftTrajectory
 * 	in MGDO/Base. 
 * 
 * Example Usage:
 *
 *
 * AUTHOR: 
 * 	Jing Liu
 * 
 * REVISION:
 * 	13-05-2008, Created, Jing Liu
 * 	13-06-2008, more comments, Jing Liu
 *
 */
#include <algorithm>

#include "MGDOUtils.hh"
#include "MGElectricField.hh"
#include "MGDriftVelocity.hh"
#include "MGDriftTrajectory.hh"

#include "io/MGLogger.hh"

#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"
using namespace CLHEP;
   
int main(int argc, char** argv)
{
   MGLog(routine) << "Going..." << endlog;
   MGLogger::SetSeverity(MGLogger::routine);
   //MGLogger::SetSeverity(MGLogger::debuggin);
  

   // prepare the electric field
   MGElectricField* theField = new MGElectricField();
   if (argc == 1) // no argument
      theField->SetFileName("EField.txt");
   else // argc > 1
      theField->SetFileName(argv[1]);

   theField->SetEType("Siegfried");
   theField->GetField(); // load field to the memory


   // prepare the charge carrier drift velocity calculation package
   MGDriftVelocity* theDrift = new MGDriftVelocity();
   theDrift->SetupGammaMatrix();


   // prepare the PS parameters
   double dT = 1e-9; // time interval of PS sampling (second)
   const int Ns = 500; // number of sampling points

   
   // prepare the trajectories
   MGDriftTrajectory theTrj(theField, theDrift);

   const int Nphi = 6, Nr = 7;
   const int Nh = Nphi*Nr; // number of hits
   TTree ps[Nh];

   const int Ne = 1; // number of electrodes
   double eps[Ne],hps[Ne];

   double time,etrx,etry,etrz,htrx,htry,htrz;

   Hep3Vector startPoint;
   int q = 0;
   for (int i=0; i<Nphi; i++) {
      for (int r=0; r<Nr; r++) {
	 ps[q].SetTitle(Form("ps (phi=%ld, r=%ld)",i*45/(Nphi-1),(r+1)*5));
	 ps[q].SetName(Form("ps_%ld_%ld",i*45/(Nphi-1),(r+1)*5));
	 ps[q].Branch("time", &time, "time/D");

	 ps[q].Branch("etrx", &etrx, "etrx/D");
	 ps[q].Branch("etry", &etry, "etry/D");
	 ps[q].Branch("etrz", &etrz, "etrz/D");

	 ps[q].Branch("htrx", &htrx, "htrx/D");
	 ps[q].Branch("htry", &htry, "htry/D");
	 ps[q].Branch("htrz", &htrz, "htrz/D");

	 ps[q].Branch("eps", eps, "eps[20]/D");
	 ps[q].Branch("hps", hps, "hps[20]/D");

	 startPoint.setRhoPhiZ((r+1)*5.0/1000., i*halfpi/2/(Nphi-1), 0.035);
	 theTrj.SetStartPoint(startPoint);
	 theTrj.SetDeltaT(dT);
	 theTrj.FillTrajectory();

	 //cout<<"time"<<"\t"<<"etrx"<<"\t"<<"etry"<<"\t"<<"etrz"<<"\t"<<"htrx"<<"\t"<<"eps[0]"<<"\t"<<"hps[0]"<<endl;
	 for (int j=0; j<Ns; j++) {
	    time = j*dT;

	    etrx = theTrj.GetElectraPoint(j).x();
	    etry = theTrj.GetElectraPoint(j).y();
	    etrz = theTrj.GetElectraPoint(j).z();

	    htrx = theTrj.GetHoletraPoint(j).x();
	    htry = theTrj.GetHoletraPoint(j).y();
	    htrz = theTrj.GetHoletraPoint(j).z();

	    for (int k=0; k<Ne; k++) { // k: electrode index. k=0 is core
	       // real codes should be:
	       // eps[k] = -theField->GetWeightingPotential(k, theTrj.GetElectraPoint(j));
	       // hps[k] =  theField->GetWeightingPotential(k, theTrj.GetHoletraPoint(j));
	       // since weighting potential is not ready, I used something similar for test:
	       eps[k] = -theField->GetEVec(theTrj.GetElectraPoint(j)).mag();
	       hps[k] =  theField->GetEVec(theTrj.GetHoletraPoint(j)).mag();
	    }
	    //cout<<time<<"\t"<<etrx<<"\t"<<etry<<"\t"<<etrz<<"\t"<<htrx<<"\t"<<eps[0]<<"\t"<<hps[0]<<endl;

	    ps[q].Fill();
	 }
	 q++;
      }
   }

   TFile file("ps.root", "recreate");
   file.cd();
   for (int i=0; i<Nh; i++) ps[i].Write();
   file.Close();

   delete theField;
   delete theDrift;

   return 0;
}
