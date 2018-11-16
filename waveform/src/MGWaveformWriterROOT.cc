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
 *   10-14-2010   Added possibility to set the tree and the output file by hands.
 *                Useful if you want to add waveforms to the already existing tree. Alex
 *
 */

#include "TTree.h"
#include "MGTWaveform.hh"

#include "io/MGLogger.hh"
#include "waveform/MGWaveformWriterROOT.hh"

using namespace std;

MGWaveformWriterROOT::MGWaveformWriterROOT()
{
  fOpenOption = "RECREATE";
  fFile = NULL;
  fTree = NULL;
}

void MGWaveformWriterROOT::OpenOutputFile(const char* filenamebase)
{
  if(fFile != NULL) {
    MGLog(warning) << "fFile is already open as " << fFile->GetName() << endlog;
  }
  else fFile = TFile::Open(Form("%s.root", filenamebase), fOpenOption.data());

  if(fTree != NULL) {
    MGLog(warning) << "fTree is already created " << endlog;
  }
  else fTree = new TTree("wfTree", "wfTree");
  return;
}

MGWaveformCollection* MGWaveformWriterROOT::GimmeWaveformCollection(int crystalID, size_t nWaveforms, const char* label)
{
  if(fTree == NULL) {
    MGLog(warning) << "fTree is not initialized" << endlog;
    return NULL;
  }

  MGWaveformCollection* wfCollection = fWFCollections[pair<string, int>(label, crystalID)];
  if(wfCollection == NULL) {
    wfCollection = new MGWaveformCollection;
    fWFCollections[pair<string, int>(label, crystalID)] = wfCollection;
  }
  else { 
    if(wfCollection->GetNWaveforms() == nWaveforms) return wfCollection;
    else if (wfCollection->GetNWaveforms() != 0) {
      MGLog(warning) << "nWaveforms has changed for crystalID " << crystalID << ", label " << label << endlog; 
      return NULL;
    }
  }

  // If we make it to this point, wfCollection is allocated but has 0 waveforms
  fFile->cd();

  TClonesArray* wfClonesArray = fWFClonesArrays[label];
  if(wfClonesArray == NULL) {
    wfClonesArray = new TClonesArray("MGTWaveform");
    fTree->Branch(label, &wfClonesArray);
    fWFClonesArrays[label] = wfClonesArray;
  }

  for(size_t iWF = 0; iWF < nWaveforms; iWF++) {
    // can use the GetEntriesFast() because we allocate so that there are no empty slots.
    size_t position = wfClonesArray->GetEntriesFast(); 
    MGTWaveform* wf = new((*wfClonesArray)[position]) MGTWaveform;
    wfCollection->AddWaveform(wf);
  }
  
  return wfCollection;
}

void MGWaveformWriterROOT::WriteEvent()
{
  fTree->Fill();

  map<string, TClonesArray*>::iterator iClonesArray;
  iClonesArray = fWFClonesArrays.begin();
  while(iClonesArray != fWFClonesArrays.end()) {
    iClonesArray->second->Delete();
    iClonesArray++;
  }

  map< pair<string, int>, MGWaveformCollection*>::iterator iCollection;
  iCollection = fWFCollections.begin();
  while(iCollection != fWFCollections.end()) {
    iCollection->second->Clear();
    iCollection++;
  }
}

void MGWaveformWriterROOT::CloseOutputFile()
{
  fTree->Write(fTree->GetName(), TObject::kOverwrite);
  fFile->Close(); // deletes fTree, all the fWFClonesArrays, and all their waveforms
  fTree = NULL;
  fWFClonesArrays.clear();

  // need to hand-delete the wfCollections
  map< pair<std::string, int>, MGWaveformCollection*>::iterator iCollection;
  iCollection = fWFCollections.begin();
  while(iCollection != fWFCollections.end()) {
    delete iCollection->second;
    iCollection++;
  }

  delete fFile;
}
