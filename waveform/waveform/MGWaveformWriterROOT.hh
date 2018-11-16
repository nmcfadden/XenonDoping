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
 * REVISION: MM-DD-YYYY
 *
 *   10-14-2010   Added possibility to set the tree and the output file by hands.
 *                Useful if you want to add waveforms to the already existing tree. Alex
 *
 */

#ifndef _MGWAVEFORMWRITERROOT_HH
#define _MGWAVEFORMWRITERROOT_HH

#include <map>
#include <string>
#include "TClonesArray.h"
#include "TFile.h"
#include "waveform/MGVWaveformWriter.hh"

class TTree;

class MGWaveformWriterROOT : public MGVWaveformWriter
{
  public:
    MGWaveformWriterROOT();
    virtual ~MGWaveformWriterROOT() {}

    virtual void OpenOutputFile(const char* filenamebase);
    virtual bool IsOpen() { return (fFile != NULL && fFile->IsOpen()); }
    virtual MGWaveformCollection* GimmeWaveformCollection(int crystalID, size_t nWaveforms, const char* label);
    virtual void WriteEvent();
    virtual void CloseOutputFile();

    virtual void SetOpenOption(std::string option) { fOpenOption = option; }

    TTree* GetTree() { return fTree; }
    void SetTree(TTree* t1) { fTree = t1; }
    void SetFile(TFile* f1) { fFile = f1; }

  protected:
    std::string fOpenOption;
    TFile* fFile;
    TTree* fTree;

    // allow for multiple clones arrays for different algorithms in the same wfTree
    std::map<std::string, TClonesArray*> fWFClonesArrays;

    // hold on to the wfCollections to manage their memory
    std::map< std::pair<std::string, int>, MGWaveformCollection*> fWFCollections;
};

#endif
