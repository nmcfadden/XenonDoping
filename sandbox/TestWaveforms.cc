
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
 * $Id: TestWaveforms.cc,v 1.2 2007-11-16 08:59:37 jasondet Exp $
 *      
 * CLASS DECLARATION:  MJAnalysisWF.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Sample wf generation program for Majorana.
 *
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jan  4 15:01:33 PST 2006
 * 
 * REVISION:
 * 
 * 05-01-2006, Created, J. Detwiler
 *
 */

#include <time.h>
#include "TChain.h"
#include "io/MGLogger.hh"
#include "MGCrystalHits.hh"
#include "io/MGOutputG4StepsData.hh"
#include "waveform/MGWaveformManager.hh"
#include "waveform/MGWFGenGaussResponse.hh"
#include "MGCrystalData.hh"

// Example Usage: TestWaveforms [G4StepsFile]

using namespace CLHEP;

int main(int argc, char *argv[])
{
  MGLog(routine) << "Going..." << endlog;
  MGLogger::SetSeverity(MGLogger::routine);
  if(argc == 1) {
    MGLog(error) << "No file specified!" << endlog;
    return 1;
  } 

  MGOutputG4StepsData* stepsData = new MGOutputG4StepsData;
  int maxSteps = 25000;
  stepsData->AllocateArrays(maxSteps);

  TChain* chain = new TChain("fTree");
  for(int i = 1; i < argc; i++) {
    chain->Add(argv[i]);
  }
  MGLog(routine) << "Chain has " << chain->GetEntries() << " events." << endlog;
  chain->SetBranchStatus("*", 1); // Read all by default.
  chain->SetBranchAddress("RawMC", &stepsData);

  MGWaveformManager wfManager;
  wfManager.SetCoincidenceTime(1*ns);
  wfManager.SetCoincidenceLength(0.2*mm);
  /*
  for(int i=0; i<57; i++) {
    MGCrystalData* crystalData = 
      new MGCrystalData(i, MGCrystalData::kPType, MGCrystalData::kClosedEnd, 0.4*cm, 3.1*cm, 7.0*cm, 6.0*cm, 0, 0, 0, 6, 1); // 6z x 1phi
    MGWFGenGaussResponse* wfGenerator = new MGWFGenGaussResponse(crystalData);
    wfManager.AddWaveformGenerator(wfGenerator);
  }
  */
  MGCrystalData* crystalData = 
      new MGCrystalData(0, MGCrystalData::kPType, MGCrystalData::kClosedEnd, 0.4*cm, 3.1*cm, 7.0*cm, 6.0*cm, 0, 0, 0, 6, 1); // 6z x 1phi
  MGWFGenGaussResponse* wfGenerator = new MGWFGenGaussResponse(*crystalData);
  wfManager.SetDefaultWaveformGenerator(wfGenerator);

  MGCrystalHits xtalHits;
  int nEntries = chain->GetEntries();
  int reportingInterval = (int) pow(10, ceil(log10(((double) nEntries)/100.0)));
  time_t startTime = time(NULL);
  for(int iEntry = 0; iEntry < chain->GetEntries(); iEntry++) {
    if(iEntry > 0 && iEntry % reportingInterval == 0) {
      time_t currentTime = time(NULL);
      MGLog(routine) << "entry " << iEntry << " (rate = " 
                     << (currentTime-startTime)/((double) reportingInterval) 
                     << " s / entry)" << endlog;
      startTime = currentTime;
    }
    chain->GetEntry(iEntry);
    stepsData->FillCrystalHits(&xtalHits);
    wfManager.GenerateWaveforms(&xtalHits);
  }

  wfManager.EndOfRunAction();

  MGLog(routine) << "Done..." << endlog;

  stepsData->DeallocateArrays();
  delete stepsData;
}

