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
 */

#include "io/MGLogger.hh"
#include "waveform/MGWaveformCollection.hh"

void MGWaveformCollection::Zero()
{
  for(size_t i=0; i<GetNWaveforms(); i++) {
    fWaveforms[i]->SetData(0);
  }
}

void MGWaveformCollection::SetTOffsets(double tOffset)
{
  for(size_t i=0; i<GetNWaveforms(); i++) {
    fWaveforms[i]->SetTOffset(tOffset);
  }
}

void MGWaveformCollection::AddWaveform(MGWaveform* waveform)
{
  if(fWaveforms.size() > 0) {
    if(fWaveforms[0]->IsSimilarTo(*waveform, true)) fWaveforms.push_back(waveform);
    else {
      MGLog(warning) << "Can't add dissimilar waveform (id = " << waveform->GetID()
                     << ") to collection" << endlog;
    }
  }
  else fWaveforms.push_back(waveform);
}
