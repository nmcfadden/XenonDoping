//
// ROOT program to dump data from MGOutputG4StepsData to screen.
// Execute from ROOT session via:
//
//
// R. Henning
// 02-01-2006
//

#include "TChain.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TMath.h"
#include "TTree.h"
#include "TObject.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TClonesArray.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TStyle.h"
#include <iostream>
#include <iomanip.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include "/home/rhenning/Majorana/code/MaGe/io/io/MGOutputG4StepsData.hh"

int DumpEvent(TChain *G4Steps, Int_t eventN);

int DumpEvent(TChain *G4Steps, Int_t eventN)
{

  // Note: This is a very slow way to dump events. Use only when debugging and
  // looking at very small event samples.

  if(!G4Steps) {
    std::cout << "Bad Pointer!";
    return 0;
  }

  MGOutputG4StepsData *fT = new MGOutputG4StepsData();
  G4Steps->SetBranchAddress("RawMC", &fT);
  G4Steps->SetBranchStatus("*", 1);
  Int_t MaxArrayLength = 25000;

  fT->fParticleID = new Int_t[MaxArrayLength];
  fT->fPreviousTrackID = new Int_t[MaxArrayLength];
  fT->fTrackID = new Int_t[MaxArrayLength];
  fT->fLocalX = new Float_t[MaxArrayLength];
  fT->fLocalY = new Float_t[MaxArrayLength];
  fT->fLocalZ = new Float_t[MaxArrayLength];
  fT->fX = new Float_t[MaxArrayLength];
  fT->fY = new Float_t[MaxArrayLength];
  fT->fZ = new Float_t[MaxArrayLength];
  fT->fPx = new Float_t[MaxArrayLength];
  fT->fPy = new Float_t[MaxArrayLength];
  fT->fPz = new Float_t[MaxArrayLength];
  fT->fEdep = new Float_t[MaxArrayLength];
  fT->fG4Process = new Char_t[20 * MaxArrayLength];
  fT->fPhysicalVolume = new Char_t[20 * MaxArrayLength];
  fT->fStatus01 = new Int_t[MaxArrayLength];
  fT->fMaterialIndex = new Int_t[MaxArrayLength];
  fT->fGlobalTime = new Double_t[MaxArrayLength];

  std::cout << "Loading event # " << eventN << std::endl;
  G4Steps->GetEntry(eventN);
  std::cout << "Event # " << eventN << " loaded." << std::endl;
  std::cout << "Number Of Steps: " << fT->fTotalNumberOfSteps << std::endl;
  std::cout << "StepNum ";
  std::cout << "fParticleID ";
  std::cout << "fPreviousTrackID ";
  std::cout << "fTrackID";
  std::cout << "        fX";
  std::cout << "        fY";
  std::cout << "        fZ";
  std::cout << "   fLocalX";
  std::cout << "   fLocalY";
  std::cout << "   fLocalZ";
  std::cout << "       fPx";
  std::cout << "       fPy";
  std::cout << "       fPz";
  std::cout << "  Momentum";
  std::cout << "     fEdep";
  std::cout << "            fG4Process";
  std::cout << "                fPhysicalVolume";
  std::cout << "  fStatus01";
  std::cout << " fMaterialIndex";
  std::cout << " fGlobalTime" << std::endl;
  Int_t j = 0;
  Int_t k = 0; 
  for(Int_t i = 0; i < fT->fTotalNumberOfSteps; i++) {
    std::cout << std::setw(5) << i;
    std::cout << std::setw(12) << fT->fParticleID[i];
    std::cout << std::setw(5) << fT->fPreviousTrackID[i];
    std::cout << std::setw(5) << fT->fTrackID[i];
    std::cout << std::setw(8) << fT->fX[i];
    std::cout << std::setw(8) << fT->fY[i];
    std::cout << std::setw(8) << fT->fZ[i];
    std::cout << std::setw(8) << fT->fLocalX[i];
    std::cout << std::setw(8) << fT->fLocalY[i];
    std::cout << std::setw(8) << fT->fLocalZ[i];
    std::cout << std::setw(8) << fT->fPx[i];
    std::cout << std::setw(8) << fT->fPy[i];
    std::cout << std::setw(8) << fT->fPz[i];
    std::cout << std::setw(8) << TMath::Sqrt(fT->fPx[i]*fT->fPx[i]+fT->fPy[i]*fT->fPy[i]+fT->fPz[i]*fT->fPz[i]);
    std::cout << std::setw(8) << fT->fEdep[i];
    std::cout << " " << &fT->fG4Process[j] << " ";
    std::cout << " " << &fT->fPhysicalVolume[k] << " ";
    std::cout << std::setw(4) << fT->fStatus01[i];
    std::cout << std::setw(4) << fT->fMaterialIndex[i];
    std::cout << std::setw(8) << fT->fGlobalTime[i] << std::endl;
    j += strlen(&fT->fG4Process[j]) + 1;
    k += strlen(&fT->fPhysicalVolume[k]) + 1;
  }
  std::cout<<"====================================================================================="<<std::endl;

  return eventN;
}


 
