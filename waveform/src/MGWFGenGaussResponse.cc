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
 *   See header.
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 *
 */

#include "TMath.h"
#include "io/MGLogger.hh"
#include "waveform/MGWFGenGaussResponse.hh"

using namespace CLHEP;

MGWFGenGaussResponse::MGWFGenGaussResponse(const MGCrystalData& crystalData, double preTriggerTime, double sigma, double ePair) :
MGVWaveformGenerator(crystalData, 512, 1.0*GHz, MGWaveform::kCharge, preTriggerTime)
{
  fSigma = sigma;
  fEPair = ePair;

  if(!fCrystalData->IsCentralContact(0)) {
    MGLog(warning) << "Bogus iCentralContact" << endlog;
  }
}

void MGWFGenGaussResponse::GenerateWaveformsForHit(double x, double y, double z, double t, double E, 
                                                   MGWaveformCollection* partialOutput)
{
  double chargeDep = E/fEPair*eplus; 
  size_t iContact = fCrystalData->GetIContact(x, y, z);
  size_t iCentralContact = 0;  // see constructor
  for(size_t iSample = 0; iSample < fWFLength; iSample++) {
    double tSample = partialOutput->GetWaveform(iContact)->GetTLocal(iSample);
    double cSample = chargeDep * TMath::Gaus(tSample, t, fSigma, kTRUE);
    (*partialOutput->GetWaveform(iContact))[iSample] += cSample;
    (*partialOutput->GetWaveform(iCentralContact))[iSample] -= cSample;
  }
}
