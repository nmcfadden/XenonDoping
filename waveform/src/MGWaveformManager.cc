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
 *
 * DESCRIPTION:
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 *
 *   03-24-2010   Setting all waveforms explicitely to be sure they are generated correctly
 *                for several hits at the same crystal. Alex
 *   10-15-2010   Temporary variable wfcollection is now the class field fWFCollection. Alex
 *   05-27-2010   Don't have the manager changing IDs of waveform generators'
 *                crystal data. Also, add some doxygen-friendly comments. J. Detwiler
 *
 */

#include "io/MGLogger.hh"
#include "MGCrystalHits.hh"
#include "waveform/MGWaveformManager.hh"
#include "waveform/MGWaveformWriterROOT.hh"
#include "MGVWaveformTransformer.hh"

using namespace std;

MGWaveformManager::MGWaveformManager(MGVWaveformWriter* writer, MGVWaveformTransformer* electronicsModel) 
{
/**
  Both parameters are optional. If writer is NULL, MGWaveformManager will
  automatically choose the default MGWaveformWriterROOT output (see
  UseDefaultROOTOutput()). If electronicsModel is NULL, no trasformation of the
  waveforms is applied after generation.
*/
  fDefaultWFGenerator = NULL;
  fWaveformWriter = writer;
  fCoincidenceTime = 0.0;
  fCoincidenceLength = 0.0;
  fElectronicsModel = electronicsModel;
}

void MGWaveformManager::UseDefaultROOTOutput()
{
  /// Use the output format defined by MGWaveformWriterROOT
  MGLog(trace) << "Using default ROOT output." << endlog;
  fWaveformWriter = new MGWaveformWriterROOT(); // note: no delete for this...
}


void MGWaveformManager::GenerateWaveforms(MGCrystalHits* crystalHits, int eventID)
{
/**
  Send in a set of crystalHits to be converted into waveforms. The optional eventID
  argument is for debugging purposes. First the hits are combined if they are
  close enough together in distance and time, and are windowed according to the
  digitization time of the default generator. The hits for each crystal in each
  window are then sent to the appropriate generator for wf generation. An event
  is then written for each digitization window.
*/
  static size_t eventCount = 0;

  if(crystalHits == NULL) {
    MGLog(warning) << "empty crystal hits" << endlog;
  }
  if(crystalHits->GetNHits() == 0) return;

  if(fWFGenerators.size() == 0 && fDefaultWFGenerator == NULL) {
    MGLog(warning) << "I need a generator!!!" << endlog;
  }
  if(fDefaultWFGenerator == NULL) fDefaultWFGenerator = fWFGenerators.begin()->second;

  if(fWaveformWriter == NULL) UseDefaultROOTOutput();
  if(!fWaveformWriter->IsOpen()) {
    fWaveformWriter->OpenOutputFile(Form("%sOutput", fDefaultWFGenerator->GetName()));
  }

  double windowWidth = fDefaultWFGenerator->GetDigitizationWindow();
  if(crystalHits->IsUnsorted()) {
    if(fCoincidenceTime > 0 || fCoincidenceLength > 0) {
      crystalHits->CombineHits(fCoincidenceTime, fCoincidenceLength, windowWidth);
    }
    else crystalHits->WindowAndSort(windowWidth);
  }

  static vector<double> sx, sy, sz, st, sE;
  for(size_t iWindow = 0; iWindow < crystalHits->GetNWindows(); iWindow++) {
    size_t iFirstCrystalHit = 0;
    while(iFirstCrystalHit < crystalHits->GetNHits(iWindow)) {
      int crystalID = crystalHits->GetHit(iFirstCrystalHit, iWindow).fCrystalID;

      // collect the hits for this crystal
      vector<MGCrystalHit*> thisCrystalsHits;
      while(iFirstCrystalHit + thisCrystalsHits.size() < crystalHits->GetNHits(iWindow)) {
        MGCrystalHit* crystalHit = &(crystalHits->GetHit(iFirstCrystalHit + thisCrystalsHits.size(), iWindow));
        if(crystalID != crystalHit->fCrystalID) break;
        thisCrystalsHits.push_back(crystalHit);
      }

      // generate the waveforms:
      MGVWaveformGenerator* wfGen = fWFGenerators[crystalID]; // map's [] does a search, call it just once
      if(wfGen == NULL) {
        static map<int, bool> sUsingDefaultWFGen;
        if(!sUsingDefaultWFGen[crystalID]) {
          MGLog(warning) << "no generator specified for crystalID " << crystalID << ", using default generator" << endlog;
          sUsingDefaultWFGen[crystalID] = true;
        }
        wfGen = fDefaultWFGenerator;
      }
      // CallInitialize() is called only once during the wfGen object life time ...
      if(!wfGen->IsInitialized()) wfGen->CallInitialize();

      // ... and Zero() is called for each event, before waveforms generating.
      // Useful to reset the variables before generating waveforms
      wfGen->Zero();

      fWFCollection =
        fWaveformWriter->GimmeWaveformCollection(crystalID, wfGen->GetNWaveformsToGenerate(), Form("%sWFs", wfGen->GetName()));

      // Setting all waveforms to zero. Otherwise it might cause problems when generating WFs from several hits at the same ctystal
      fWFCollection->Zero();

      wfGen->GenerateWaveformsForHits(&(thisCrystalsHits[0]), thisCrystalsHits.size(), fWFCollection);

      if(fElectronicsModel != NULL) {
        for(size_t iWF = 0; iWF < fWFCollection->GetNWaveforms(); iWF++) {
          MGWaveform* waveform = fWFCollection->GetWaveform(iWF);
          fElectronicsModel->Transform(waveform);
        }
      }

      // go to next crystal 
      iFirstCrystalHit += thisCrystalsHits.size();
    }
    // By now, we have generated the waveforms for all crystals hit in the
    // same time window. Write the event.
    fWaveformWriter->WriteEvent();
    MGLog(trace) << "Waveform event " << eventCount << " on eventID " << eventID << endlog;
    eventCount++;
  }
}

