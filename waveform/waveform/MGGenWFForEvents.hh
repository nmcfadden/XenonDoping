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
 *     Class to generate waveforms according to settings:
 *         - homogeniously distributed in the crystal;
 *         - from root-files with hit information.
 *    Uses MGWFGenFromDrift to calculate and store information about end points of electron/holes and their trajectories
 *
 * AUTHOR:
 *     Alex Volynets
 *
 * FIRST SUBMISSION:
 *     03-29-2010, Alex
 *
 * REVISION: MM-DD-YYYY
 *     20-10-2011 Added setters/getters for crystal, fields, generator, manager and writer. Alex
 *     07-08-2011 Doxygen-friendly comments added. Alex
 *     10-19-2011 Added an option to specify the distance for hit combination. Alex
 *
 */

#ifndef MGGenWFForEvents_hh
#define MGGenWFForEvents_hh

#include "MGCrystalHits.hh"

#include "waveform/MGWaveformManager.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include "waveform/MGSORCrystalFields.hh"
#include "waveform/MGWaveformWriterROOT.hh"

#include "MGCrystalData.hh"

#include "TTree.h"
#include "TFile.h"
#include <TString.h>
#include <TEventList.h>

/// Class to produce waveforms and electron/holes trajectories for a given .root file from MaGe
class MGGenWFForEvents
{
  public:

    /// Constructor.
    MGGenWFForEvents();
    /// Destructor.
    ~MGGenWFForEvents();

    /// Read setup file with simulation parameters
    void ReadSetupDataFromFile(char*);
    /// Initialize and load electrical field
    void InitCrystalFields();
    /// Set output tree and file
    void SetOutput();
    /// Open mage tree with hits
    void OpenTree();

    /// Generate waveforms for hits
    void ProcessEvents();
    /// Calculate holes drift and occupancy of the segment
    void Occupancy();
    /// Generate homogeniously distributed hits
    void GenerateHomogeniousHits();

    /// Create event list
    void CreateEventList();
    /// Set the event list
    void        SetEventList(TEventList *el) { fEventList = el; }
    /// Get event list
    TEventList* GetEventList()               { return fEventList; }
    /// Write the file, delete the objects
    void Finalize();


    /// Get crystal field pointer
    MGSORCrystalFields* GetCrystalFields() { return fFields; }

    /// Set crystal data
    void SetCrystal(MGCrystalData* crystal)             { fCrystal = crystal; }
    /// Get crystal data
    MGCrystalData* GetCrystal()                         { return fCrystal; }

    /// Set waveform generator
    void SetWaveformGenerator(MGWFGenFromDrift* wfGen)  { fWFGenerator = wfGen; }
    /// Get waveform generator
    MGWFGenFromDrift* GetWaveformGenerator()            { return fWFGenerator; }

    /// Set waveform manager
    void SetWaveformManager(MGWaveformManager* wfMan)   { fWFManager = wfMan; }
    /// Get waveform manager
    MGWaveformManager* GetWaveformManager()             { return fWFManager; }

    /// Set waveform writer
    void SetWaveformWriter(MGWaveformWriterROOT* wfWri) { fWFWriter = wfWri; }
    /// Get waveform writer
    MGWaveformWriterROOT* GetWaveformWriter()           { return fWFWriter; }

    /// Set the file name with mage events
    void SetEventFileName (char* filename) { fEventFileName = filename;}
    /// Set the file name of the electrical field
    void SetFieldsFileName(char* filename) { fFieldsFileName = filename;}
    /// Set the file name of the output file
    void SetOutputFileName(char* filename) { fOutputFileName = filename;}

    /// Gt the distance for combining hits
    Float_t GetCombineHitsDistance() { return fCombineHitsDistance; }
    /// Set the distance for combining hits
    void SetCombineHitsDistance(Float_t dist) { fCombineHitsDistance = dist;}

  private:
    MGWaveformWriterROOT *fWFWriter; ///< pointer to waveform writer
    MGWaveformManager *fWFManager;   ///< pointer to the waveform manager
    MGCrystalData* fCrystal;         ///< pointer to the crystal data
    MGSORCrystalFields* fFields;     ///< pointer to the electrical field
    MGWFGenFromDrift* fWFGenerator;  ///< pointer to the waveform generator
    MGCrystalHits fHits;             ///< set of hits. Moved here from the temporary loop variable to save time

    TFile *fEventFile;               ///< file with mage events
    TFile *fOutputFile;              ///< output file

    TTree *fEventTree;               ///< tree with hit information
    //TTree *fSelectedEventsTree;      ///< tree with selected events. Currently not used
    TTree *fOutputTree;              ///< output tree
    TEventList *fEventList;          ///< event list. Currently not used
    TString fEventFileName;          ///< file name with hit information
    TString fFieldsFileName;         ///< electrical field file name
    TString fOutputFileName;         ///< output file name
    Int_t fNEvents;                  ///< number of events to be processed
    Int_t fNSelectedEvents;          ///< number of selected events. Currently not used
    Float_t fCombineHitsDistance;    ///< distance for combining hits

    // Variables for generating homogeniously distributed hits
    Float_t fStartR,   fEndR,   fNR;
    Float_t fStartPhi, fEndPhi, fNPhi;
    Float_t fStartZ,   fEndZ,   fNZ;

    // Variables from the event tree
    Int_t hits_totnum;
    Float_t hits_xpos[1000];
    Float_t hits_ypos[1000];
    Float_t hits_zpos[1000];
    Float_t hits_time[1000];
    Float_t hits_edep[1000];

    // Not so useful variables that can be used as well
    //Float_t hits_tote;
    //Int_t   seg_totnum;
    //Float_t seg_edep[100];
    //Int_t   seg_id[100];
};

#endif
