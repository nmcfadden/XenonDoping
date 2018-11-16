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
 *      05-27-2011 Don't allow NULL fCrystalData, and remove ChangeCrystalID so
 *                 that it can be made const. J. Detwiler
 *
 */

#include "MGCrystalHits.hh"
#include "io/MGLogger.hh"
#include "waveform/MGVWaveformGenerator.hh"

MGVWaveformGenerator::MGVWaveformGenerator(const MGCrystalData& crystalData,
                                           size_t wfLen, double sampFreq, 
                                           MGWaveform::EWFType type, double preTriggerTime)
{
/**
  crystalData is a required parameter: cannot generate waveforms without knowing
  some geometry information, e.g. the number of contacts. The other parameters
  set characteristics of the generated waveforms and have default values. They
  can be changed after initialization via SetWaveformParameters() and
  SetPreTriggerTime()
*/
  fCrystalData = &crystalData;
  fWFLength = wfLen;
  fSamplingFrequency = sampFreq;
  fWFType = type;
  fPreTriggerTime = preTriggerTime;
  fIsInitialized = false;
}

void MGVWaveformGenerator::SetWaveformParameters(size_t wfLen, double sampFreq, MGWaveform::EWFType type)
{
  /// Sets the length, sampling frequency, and waveform type of the generated waveforms
  fWFLength = wfLen;
  fSamplingFrequency = sampFreq;
  fWFType = type;
}

void MGVWaveformGenerator::GenerateWaveformsForHits(MGCrystalHit** crystalHitArray, size_t nHits, MGWaveformCollection* output)
{
/**
  This function is called by MGWaveformManager for each crystal hit in an event.
  It assumes that crystalHitArray contains time-ordered hits for a single
  crystal. The defautl version loops over hits and calls
  GenerateWaveformsForHit(), but this behavior may be overloaded. If it is
  overload, GenerateWaveformsForHit() must still be defined (even if it is just
  an empty function) in order to compile.

  In the second version, cumulativeOutput should be added to, as it
  contains the cumulative output so far in the wf generation (to save time).
  Also in the second version, time is measured relative to the local time
  of the waveform (t=0 for the first sample), and x, y, z are in the local
  coordinates of fCrystalData. x, y, z, t, and E all use G4 (CLHEP) units.

*/

  if(!fIsInitialized) CallInitialize();

  InitializeWaveformsInCollection(*output, crystalHitArray[0]->fCrystalID);

  // set the tOffsets
  if(nHits == 0) {
    MGLog(warning) << "You tried to generate waveforms for an empty crystalHitArray!" << endlog;
    return; 
  }
  double minTime = crystalHitArray[0]->fT;
  for(size_t i=1; i<nHits; i++) if(crystalHitArray[i]->fT < minTime) minTime = crystalHitArray[i]->fT;
  output->SetTOffsets(minTime - fPreTriggerTime);

  // finally, generate the waveforms
  for(size_t i = 0; i < nHits; i++) {
    if(crystalHitArray[i]->fE <= 0.0) {
      MGLog(warning) << "You tried to generate waveforms for a non-positive energy deposition! Skipping..." << endlog;
      continue; 
    }
    GenerateWaveformsForHit(crystalHitArray[i]->fX, crystalHitArray[i]->fY, crystalHitArray[i]->fZ, 
                            crystalHitArray[i]->fT - minTime + fPreTriggerTime, crystalHitArray[i]->fE, output);
  }
}

void MGVWaveformGenerator::InitializeWaveformsInCollection(MGWaveformCollection& wfCollection, int crystalID)
{
  if(wfCollection.GetNWaveforms() != fCrystalData->GetNContacts()) {
    if(wfCollection.GetNWaveforms() != 1 || fCrystalData->GetNContacts() != 2) {
      MGLog(warning) << "Number of waveforms in collection (" 
                     << wfCollection.GetNWaveforms() 
                     << ") != number of crystal contacts (" 
                     << fCrystalData->GetNContacts() 
                     << "). Returning..." << endlog;
      return;
    }
  }
  for(size_t iWF = 0; iWF < wfCollection.GetNWaveforms(); iWF++) {
    MGWaveform* wf = wfCollection.GetWaveform(iWF);
    wf->SetID(fCrystalData->GetContactID(crystalID, iWF));
    wf->SetLength(fWFLength); 
    wf->SetData(0);
    wf->SetSamplingFrequency(fSamplingFrequency);
    wf->SetWFType(fWFType);
  }
}
