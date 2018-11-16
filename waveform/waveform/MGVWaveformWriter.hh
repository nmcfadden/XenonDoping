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
 */

#ifndef _MGVWAVEFORMWRITER_HH
#define _MGVWAVEFORMWRITER_HH

#include <map>
#include "waveform/MGWaveformCollection.hh"

class MGVWaveformWriter 
{
  public:
    MGVWaveformWriter() {}
    virtual ~MGVWaveformWriter() {}

    virtual void OpenOutputFile(const char* filenamebase) = 0;
    virtual bool IsOpen() = 0;
    virtual void CloseOutputFile() = 0;

    // MGVWaveformWriter controls the memory for all the waveforms.
    virtual MGWaveformCollection* GimmeWaveformCollection(int crystalID, size_t nWaveforms, const char* label) = 0;

    virtual void WriteEvent() = 0;
};

#endif
