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
 * 	Class to manage generation of waveforms
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * REVISION: MM-DD-YYYY
 *
 *  10-15-2010   Temporary variable wfcollection is now the class field fWFCollection.
 *               Getter is added, respectively. Alex
 *  05-27-2010   Doxygenate. J. Detwiler
 *
 *
 */

#ifndef _MGVWAVEFORMMANAGER_HH
#define _MGVWAVEFORMMANAGER_HH

#include <map>
#include "waveform/MGVWaveformGenerator.hh"
#include "waveform/MGVWaveformWriter.hh"
#include "MGCrystalHits.hh"

class MGVWaveformTransformer;

/// Class to manage generation of waveforms
class MGWaveformManager 
{
  public:
    /// Constructor
    MGWaveformManager(MGVWaveformWriter* writer = NULL, MGVWaveformTransformer* electronicsModel = NULL); 
    /// Destructor 
    virtual ~MGWaveformManager() {}

    /// Getter for waveform writer
    virtual inline MGVWaveformWriter* GetWaveformWriter() { return fWaveformWriter; }
    /// Setter for waveform writer
    virtual inline void SetWaveformWriter(MGVWaveformWriter* waveformWriter) { fWaveformWriter = waveformWriter; }
    /// Use the output format defined by MGWaveformWriterROOT
    virtual void UseDefaultROOTOutput();

    /// For adding crystal-specific waveform generators
    virtual void AddWaveformGenerator(MGVWaveformGenerator* wfGenerator) { fWFGenerators[wfGenerator->GetCrystalID()] = wfGenerator; }
    /// For adding one waveform generator sufficient for all crystals
    virtual inline void SetDefaultWaveformGenerator(MGVWaveformGenerator* wfGenerator) { fDefaultWFGenerator = wfGenerator; }

    /// Workhorse function for MGWaveformManager
    virtual void GenerateWaveforms(MGCrystalHits* crystalHits, int eventID = 0);
    /// Overloadable function for cleanup actions at the end of a run
    virtual void EndOfRunAction() { fWaveformWriter->CloseOutputFile(); }

    /// Set coincidence time for hit clustering
    virtual inline void SetCoincidenceTime(double dt) { fCoincidenceTime = dt; }
    /// Set coincidence distance for hit clustering
    virtual inline void SetCoincidenceLength(double dr) { fCoincidenceLength = dr; }

    /// Getter for electronics model
    virtual inline MGVWaveformTransformer* GetElectronicsModel() { return fElectronicsModel; }
    /// Setter for electronics model
    virtual inline void SetElectronicsModel(MGVWaveformTransformer* electronicsModel) 
      { fElectronicsModel = electronicsModel; }

    /// Get the most recently generated waveform collection
    virtual inline MGWaveformCollection* GetWaveformCollection() {return fWFCollection;};

  protected:
    std::map<int, MGVWaveformGenerator*> fWFGenerators; ///< map of crystalID to generators
    MGVWaveformGenerator* fDefaultWFGenerator; ///< default generator (used for all crystal IDs)
    MGVWaveformWriter* fWaveformWriter; ///< writes the waveform output

    double fCoincidenceTime; ///< coincidence time for hit clustering
    double fCoincidenceLength; ///< coincidence length for hit clustering

    MGVWaveformTransformer* fElectronicsModel; ///< electronics model to be applied after wf generation
    MGWaveformCollection* fWFCollection; ///< collection of generated waveforms
};


#endif
