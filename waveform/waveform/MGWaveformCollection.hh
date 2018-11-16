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
 * 	Class for managing the collection of waveforms generated simultaneously
 * 	for a multi-contact crystal. 
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 * 
 */

#ifndef _MGWAVEFORMCOLLECTION_HH
#define _MGWAVEFORMCOLLECTION_HH

#include <vector>
#include "MGWaveform.hh"

/// Class for managing the collection of waveforms generated simultaneously for a multi-contact crystal. 
class MGWaveformCollection 
{
  public:
    /// Constructor
    MGWaveformCollection() {}
    /// Destructor
    virtual ~MGWaveformCollection() {}

    /// Add waveform to the collection (by pointer)
    virtual void AddWaveform(MGWaveform* waveform);
    /// Return the number of waveforms in the collection
    virtual inline size_t GetNWaveforms() const { return fWaveforms.size(); }
    /// Return waveform i
    virtual inline MGWaveform* GetWaveform(size_t i) { return fWaveforms[i]; }
    /// Clear the internal list of waveform pointers
    virtual inline void Clear() { fWaveforms.clear(); }

    /// Zero out all waveforms in the collection (set their samples to 0.0)
    virtual void Zero();
    /// Set all waveform t-offsets to 0.0
    virtual void SetTOffsets(double tOffset);

  protected:
    std::vector<MGWaveform*> fWaveforms; ///< internal list of waveform pointers
};

#endif
