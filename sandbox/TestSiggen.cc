/* TestSiggenUtils - test script for MGSiggenUtils.
 * AUTHORS: Khang Nguyen & Jason Detwiler
 * CONTACT: khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 * LAST MODIFIED: Wednesday, January 12, 2011
 */

#include <iostream>
#include <fstream>
#include <string.h>

#include "waveform/MGSiggenUtils.hh"
#include "waveform/MGM3dcrPPCCrystalFields.hh"
#include "waveform/MGSiggenDriftVelocityTable.hh"
#include "waveform/MGWFGenSiggen.hh"
#include "waveform/MGWaveformCollection.hh"
#include "MGTWaveform.hh"
#include "MGWFIntegral.hh"

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TMath.h"
#include "TStyle.h"

TApplication* gApp = NULL;

using namespace std;
using namespace CLHEP;

int main(int argc, char** argv)
{
  if(argc < 2) {
    cout << "Usage: TestSiggen [configDir]" << endl;
    cout << "configDir must contain the following files:" << endl;
    cout << "geometry_setup_ppc.dat" << endl;
    cout << "field_setup_ppc.dat" << endl;
    cout << "productionBEGe_standard_sigs1.spn" << endl;
    cout << "productionBEGe_standard_sigs2.spn" << endl;
    cout << "productionBEGe_standard_sigs3.spn" << endl;
    return 1;
  }
  string dataDir = argv[1]; // e.g.  /Users/jasondet/Work/MJ/Software/Devel/siggen_jason

  MGLogger::SetSeverity(MGLogger::trace);

  // prepare siggen
  string geomFileName = dataDir + "/geometry_setup_ppc.dat";
  int crystalID = 1;
  MGCrystalData* ppcData = MGSiggenUtils::InitializeNewPPCGeometry(geomFileName.c_str(), crystalID);

  string fieldsFileName = dataDir + "/field_setup_ppc.dat";
  MGM3dcrPPCCrystalFields m3dcrFields(*ppcData);
  m3dcrFields.SetFieldsFileName(fieldsFileName.c_str());
  m3dcrFields.LoadFields();

  MGSiggenDriftVelocityTable siggenDrift(m3dcrFields);
  siggenDrift.LoadFieldsFile(fieldsFileName.c_str());

  size_t wfLen = 2000;
  double sampFreq = 1.0*GHz;
  double preTriggerTime = 0.*ns;
  MGWFGenSiggen siggen(siggenDrift, wfLen, sampFreq, preTriggerTime);

  MGWaveformCollection wfCollection;
  for(size_t i=0; i<siggen.GetNWaveformsToGenerate(); i++) {
    MGTWaveform* waveform = new MGTWaveform;
    wfCollection.AddWaveform(waveform);
  }
  if(wfCollection.GetNWaveforms() == 0) {
    cout << "what, no waveforms?" << endl;
    return 1;
  }

  // open Ren's files
  int wfBuffer[4096];
  vector<MGTWaveform*> rensWaveforms;
  vector<Hep3Vector> rensWaveformPositions;
  double tOffset = 0.0;

  ifstream dataset1((dataDir+"/productionBEGe_standard_sigs1.spn").c_str());
  if(!dataset1.good()) {
    cout << "Couldn't open dataset1" << endl;
    return 1;
  }
  while(1) {
    dataset1.read((char*) wfBuffer, 4096*sizeof(int));
    if(!dataset1.good()) break;
    MGTWaveform* wf = new MGTWaveform(wfLen, sampFreq, tOffset, MGWaveform::kCharge);
    for(int i=0; i<2000; i++) (*wf)[i] = wfBuffer[i];
    rensWaveforms.push_back(wf);
    double r = double(wfBuffer[3000])/10.*mm;
    double phi = double(wfBuffer[3001])/10.;
    //phi = -pi/8.;
    double z = double(wfBuffer[3002])/10.*mm;
    Hep3Vector point;
    point.setRhoPhiZ(r, phi, z-ppcData->GetCrystalHeight()/2.);
    rensWaveformPositions.push_back(point);
  }
  dataset1.close();

  ifstream dataset2((dataDir + "/productionBEGe_standard_sigs2.spn").c_str());
  if(!dataset2.good()) {
    cout << "Couldn't open dataset2" << endl;
    return 1;
  }
  while(1) {
    dataset2.read((char*) wfBuffer, 4096*sizeof(int));
    if(!dataset2.good()) break;
    MGTWaveform* wf = new MGTWaveform(wfLen, sampFreq, tOffset, MGWaveform::kCharge);
    for(int i=0; i<2000; i++) (*wf)[i] = wfBuffer[i];
    rensWaveforms.push_back(wf);
    double r = double(wfBuffer[3000])/10.*mm;
    double phi = double(wfBuffer[3001])/10.;
    //phi = -pi/8.;
    double z = double(wfBuffer[3002])/10.*mm;
    Hep3Vector point;
    point.setRhoPhiZ(r, phi, z-ppcData->GetCrystalHeight()/2.);
    rensWaveformPositions.push_back(point);
  }
  dataset2.close();

  ifstream dataset3((dataDir + "/productionBEGe_standard_sigs3.spn").c_str());
  if(!dataset3.good()) {
    cout << "Couldn't open dataset3" << endl;
    return 1;
  }
  while(1) {
    dataset3.read((char*) wfBuffer, 4096*sizeof(int));
    if(!dataset3.good()) break;
    MGTWaveform* wf = new MGTWaveform(wfLen, sampFreq, tOffset, MGWaveform::kCharge);
    for(int i=0; i<2000; i++) (*wf)[i] = wfBuffer[i];
    rensWaveforms.push_back(wf);
    double r = double(wfBuffer[3000])/10.*mm;
    double phi = double(wfBuffer[3001])/10.;
    //phi = -pi/8.;
    double z = double(wfBuffer[3002])/10.*mm;
    Hep3Vector point;
    point.setRhoPhiZ(r, phi, z-ppcData->GetCrystalHeight()/2.);
    rensWaveformPositions.push_back(point);
  }
  dataset3.close();

  // run siggen
  double superSum = 0.;
  for(size_t iWF = 0; iWF < rensWaveforms.size(); iWF++) {
  //for(size_t iWF = 0; iWF < 1; iWF++) {
    double x = rensWaveformPositions[iWF].x(), 
           y = rensWaveformPositions[iWF].y(), 
           z = rensWaveformPositions[iWF].z(), t = 0, E = 2.95*keV;
    siggen.InitializeWaveformsInCollection(wfCollection, crystalID);
    //wfCollection.GetWaveform(0)->DumpWF();
    siggen.GenerateWaveformsForHit(x, y, z, t, E, &wfCollection);
    MGTWaveform* wf = (MGTWaveform*) wfCollection.GetWaveform(0);
    MGWFIntegral integrator;
    integrator.Transform(wf);
    double sum = 0.;
    double rensLast = 0, lastDelta = 0;
    for(size_t i=0; i<wf->GetLength(); i++) {
      double rens = rensWaveforms[iWF]->At(i);
      double mages = wf->At(i);
      double delta = fabs(rens-mages);
      // near the flat top, the MaGe wf may be within double-point precision of
      // ren's, but will get truncated. Call double-point precision close enough.
      if(delta/rens < 1e-15) continue;
      delta = fabs(rens - TMath::Floor(mages));
      if(delta > 0 && rens == rensLast) continue;
      rensLast = rens;
      if(delta == 1000 && lastDelta == 1000) continue;
      lastDelta = delta;
      sum += delta;
      if(delta > 0) {
        cout << "wf " << iWF << " at (r,z) = (" << sqrt(x*x+y*y)/mm << "," << z/mm << ") mm"
             << " delta = " << delta << " on sample " << i 
             << ": rens = " << rens << ", mages = " << mages 
             << " (rel diff = " << fabs(rens-mages)/rens << ")" << endl;
        cout << iWF << ' ' << sum << ' ' << superSum << endl;
        if(!gApp) gApp = new TApplication("gApp", 0, NULL);
        gStyle->SetOptTitle(0);
        gStyle->SetOptStat(0);
        gStyle->SetFillColor(0);
        TCanvas canvas;
        wf->GimmeHist()->SetLineColor(kRed);
        wf->GimmeHist()->SetMarkerStyle(7);
        wf->GimmeHist()->SetMarkerColor(kRed);
        wf->GimmeHist()->Draw("LP");
        MGTWaveform wfCopy = (*wf);
        for(size_t j=0; j<wf->GetLength(); j++) wfCopy[j] = TMath::Floor(wfCopy[j]);
        wfCopy.GimmeHist("floor")->SetLineColor(kBlue);
        wfCopy.GimmeHist("floor")->SetMarkerStyle(7);
        wfCopy.GimmeHist("floor")->SetMarkerColor(kBlue);
        wfCopy.GimmeHist("floor")->Draw("LP SAME");
        rensWaveforms[iWF]->GimmeHist("Ren's")->SetMarkerStyle(7);
        rensWaveforms[iWF]->GimmeHist("Ren's")->Draw("LP SAME");
        wfCopy.GimmeHist("floor")->Draw("LP SAME");
        cout << "You are now in a ROOT application environment. "
             << "Quit root (e.g.  from the pull-down menu) to continue to the next waveform. "
             << "Press ctrl-C to exit." << endl;
        gApp->Run(true);
      }
    }
    if(0 && sum != 0) {
      cout << iWF << ' ' << sum << ' ' << superSum << endl;
      gApp = new TApplication("gApp", 0, NULL);
      gStyle->SetOptTitle(0);
      gStyle->SetOptStat(0);
      gStyle->SetFillColor(0);
      TCanvas canvas;
      wf->GimmeHist()->SetLineColor(kRed);
      wf->GimmeHist()->SetMarkerStyle(7);
      wf->GimmeHist()->SetMarkerColor(kRed);
      wf->GimmeHist()->Draw("LP");
      MGTWaveform wfCopy = (*wf);
      for(size_t i=0; i<wf->GetLength(); i++) wfCopy[i] = TMath::Floor(wfCopy[i]);
      wfCopy.GimmeHist("floor")->SetLineColor(kBlue);
      wfCopy.GimmeHist("floor")->SetMarkerStyle(7);
      wfCopy.GimmeHist("floor")->SetMarkerColor(kBlue);
      wfCopy.GimmeHist("floor")->Draw("LP SAME");
      rensWaveforms[iWF]->GimmeHist("Ren's")->SetMarkerStyle(7);
      rensWaveforms[iWF]->GimmeHist("Ren's")->Draw("LP SAME");
      wfCopy.GimmeHist("floor")->Draw("LP SAME");
      cout << "You are now in a ROOT application environment. "
           << "Quit root (e.g.  from the pull-down menu) to continue to the next waveform. "
           << "Press ctrl-C to exit." << endl;
      gApp->Run(true);
    }
    superSum += sum;
  }
  cout << superSum << endl;

/*
  double x = rensWaveformPositions[0].x(), 
         y = rensWaveformPositions[0].y(), 
         z = rensWaveformPositions[0].z(), t = 0, E = 2.95*keV;
  siggen.GenerateWaveformsForHit(x, y, z, t, E, &wfCollection);

  gApp = new TApplication("gApp", 0, NULL);
  TCanvas canvas;
  MGTWaveform* wf = (MGTWaveform*) wfCollection.GetWaveform(0);
  MGWFIntegral integrator;
  integrator.Transform(wf);
  wf->GimmeHist()->SetLineColor(kRed);
  wf->GimmeHist()->SetMarkerStyle(7);
  wf->GimmeHist()->SetMarkerColor(kRed);
  wf->GimmeHist()->Draw("LP");
  MGTWaveform wfCopy = (*wf);
  for(size_t i=0; i<wf->GetLength(); i++) wfCopy[i] = TMath::Floor(wfCopy[i]);
  wfCopy.GimmeHist("floor")->SetLineColor(kBlue);
  wfCopy.GimmeHist("floor")->SetMarkerStyle(7);
  wfCopy.GimmeHist("floor")->SetMarkerColor(kBlue);
  wfCopy.GimmeHist("floor")->Draw("LP SAME");
  rensWaveforms[0]->GimmeHist("Ren's")->SetMarkerStyle(7);
  rensWaveforms[0]->GimmeHist("Ren's")->Draw("LP SAME");
  gApp->Run(true);
*/

  return 0;
}

