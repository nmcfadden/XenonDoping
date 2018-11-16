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
 * 	see waveform/MGWFGenSiggen.hh
 *
 * AUTHOR:
 * 	Jason Detwiler
 *
 * FIRST SUBMISSION: 05-28-2011
 *
 * REVISION: MM-DD-YYYY
 * 	05-28-2011 created, J. Detwiler
 *
 */

#include <cmath>
#include "io/MGLogger.hh"
#include "waveform/MGWFGenSiggen.hh"
#include "TMath.h"

using namespace std;
using namespace CLHEP;

MGWFGenSiggen::MGWFGenSiggen(const MGVDriftVelocity& drift, 
                             size_t wfLen, double sampFreq, 
                             double preTriggerTime)
: MGVWaveformGenerator(drift.GetCrystalData(), wfLen, sampFreq, MGWaveform::kCurrent, preTriggerTime),
  fDrift(&drift)
{
/**
  drift is a required argument: can't generate signals without a
  MGVDriftVelocity. But you can choose your favorite version.

  Other variables set the parameters for the generated waveforms and are
  optional. They can be set later using MGVWaveformGenerator::SetWaveformParameters()
*/
}



void MGWFGenSiggen::Initialize()
{
  /// Currently does nothing
}



void MGWFGenSiggen::GenerateWaveformsForHit(double x, double y, double z, double t, double E, MGWaveformCollection* partialOutput)
{
  /// Converts E into holes and electrons, and calls MakeSignal() to sum the
  /// contributions from each into partialOutput

  // check validity of input variables
  if(!fCrystalData->IsInCrystal(x, y, z)) {
    MGLog(error) << "GenerateWaveformsForHit(): point is not in crystal!" << endlog;
    return;
  }
  if(t<0) MGLog(warning) << "GenerateWaveformsForHit(): part of signal is not captured in the waveform" << endlog;
  if(partialOutput == NULL || partialOutput->GetNWaveforms() == 0) {
    MGLog(error) << "GenerateWaveformsForHit(): Invalid partialOutput" << endlog;
    return;
  }

  const double kEPair = 2.95*eV;
  double q = eplus*(E/kEPair);
  MakeSignal(x, y, z, t, q, partialOutput);  // drift holes
  MakeSignal(x, y, z, t, -q, partialOutput); // drift electrons
}



void MGWFGenSiggen::MakeSignal(double x, double y, double z, double t, double q, MGWaveformCollection* partialOutput)
{
/**
  This is the main workhorse function for waveform generation. Adapted version
  of the function make_signal() from siggen/calc_signal.c by Karin Lagergren.
  Calculates the current pulse for electrons (q<0) or holes (q>0) freed at
  (x,y,z) due to deposition E at time t.

  Note: t is measured in the local coordinates of the waveform (t=0 for sample 0)

  FIXMEs:
  - Siggen hard-codes the time step as 1 ns, then re-packs into output waveform
    as necessary. Implement that?
  - Didn't implement siggen termination condition when last step doesn't go out
    of xtal. Is it necessary? This code will emit a warning when that happens.
  - Siggen did wp threshold checks - are those necessary?
  - Should add interface for charge trapping correction as in siggen
  - Should add interface to integrate signals after generation to give charge
    pulses (overload GenerateWaveformsForHits()?)
*/

  MGWaveform* aWF = partialOutput->GetWaveform(0);
  if(aWF->GetWFType() != MGWaveform::kCurrent) {
    MGLog(warning) << "GenerateWaveformsForHit(): received waveform of the wrong type!" << endlog;
  }

  // step forward in waveform until we are at the first sample with moving
  // charges
  size_t iSample = 0;
  while(aWF->GetTLocal(iSample) < t && iSample < aWF->GetLength()) iSample++;
  if(iSample >= aWF->GetLength()) {
    MGLog(error) << "GenerateWaveformsForHit(): t is beyond the end of the digitization window" << endlog;
    return;
  }


  // Drift charge to the time corresponding to iSample. If t < 0, this
  // corresponds to drifting to the start of the waveform. Otherwise, this is
  // just taking the inter-sample step to get in time-sync with the sampling
  // frequency. Note: this wf generator samples the signal at the instantaneous
  // time coresponding to the sample; there is no integrating or averaging over
  // the signal during the time that passes between samples.
  Hep3Vector point(x,y,z);
  double dt = -1;
  while(t < aWF->GetTLocal(iSample)) {
    // snap to the waveform sampling time grid on first step
    if(dt == -1) dt = fmod(aWF->GetTLocal(iSample)-t, aWF->GetSamplingPeriod());
    else dt = aWF->GetSamplingPeriod();

    // get velocity and return if we have stopped moving
    Hep3Vector v = fDrift->GetDriftVelocityAt(point, q);
    if(v.mag() == 0.0) return;

    point += dt * v;
    t += dt;
  }
  if(t != aWF->GetTLocal(iSample)) {
    MGLog(warning) << "MakeSignal(): t = " << t/ns << " ns, "
                   << "tLocal = " << aWF->GetTLocal(iSample)/ns << " ns" << endlog;
  }
  dt = aWF->GetSamplingPeriod(); // in case the last for loop took 0 or 1 steps!

  // Now we are ready to fill the waveforms
  bool firstTime = true;
  vector<double> wPotSave(partialOutput->GetNWaveforms());
  Hep3Vector dx;
  for( ; iSample < aWF->GetLength(); iSample++) {
    Hep3Vector v = fDrift->GetDriftVelocityAt(point, q);
    if(v.mag() == 0.0) break;
    if(!firstTime && v.angle(dx) > 90.*degree) {
      MGLog(warning) << "MakeSignal(): turned by " << v.angle(dx)/degree << " degrees" << endlog;
    }

    if(firstTime) {
      for(size_t iContact = 0; iContact < partialOutput->GetNWaveforms(); iContact++) {
        wPotSave[iContact] = fDrift->GetCrystalFields().GetWPotential(point, iContact);
      }
      firstTime = false;
    }
    else {
      for(size_t iContact = 0; iContact < partialOutput->GetNWaveforms(); iContact++) {
        double wPot = fDrift->GetCrystalFields().GetWPotential(point, iContact);
        MGWaveform& waveform = *(partialOutput->GetWaveform(iContact));
        waveform[iSample] += q * (wPot - wPotSave[iContact]);
        wPotSave[iContact] = wPot;
      }
    }

    // FIXME: apply charge-trapping correction here 
    // q = charge_trapping(prev_pt,dist, q);
    dx = dt*v;
    point += dx;
  }
  if(iSample == aWF->GetLength()) return;

  // The last step has moved us from a valid point to one where v=0
  // Either the last step stepped us out of the crystal, or into a region where
  // the E-field is 0.
  // In siggen, one takes n more steps using the last calculated dx, where n is
  // at least 1, and the wpot is taken from its last value to 0 or 1 (depending
  // on whether the charge ends up in the corresponding segment) over the last n
  // steps, so that all of the charge is collected
  // FIXME: It seems this is not correct for the case E=0: the charge should get
  // trapped (well, should diffuse) and not be collected.
  // For now do the case where you take one last step, and emit an error when
  // that step doesn't take you out of the crystal
  if(fCrystalData->IsInCrystal(point)) {
    MGLog(warning) << "MakeSignal(): v -> 0 before we left the crystal..." << endl;
  }

  // Find the segment with the largest weighting potential: will let that wp -> 1,
  // take all other to 0.
  // FIXME: siggen does a wp threshold check - is that necessary?
  double wpMax = 0;
  size_t iContactWithMaxWP = 0;
  for(size_t iContact = 0; iContact < partialOutput->GetNWaveforms(); iContact++) {
    if(wPotSave[iContact] > wpMax) {
      wpMax = wPotSave[iContact];
      iContactWithMaxWP = iContact;
    }
  }

  // Now take that one last step to wp = 0 or 1 (same as drifting exactly to xtal edge)
  for(size_t iContact = 0; iContact < partialOutput->GetNWaveforms(); iContact++) {
    double wPot = (iContact == iContactWithMaxWP) ? 1.0 : 0.0;
    if(partialOutput->GetNWaveforms() == 1) {
      // In this case, iContactWithMaxWP = 0 whether wpot should be 1 or 0.
      // Decide whether wp should be 1 or 0 by rounding to whichever is nearest
      wPot = TMath::Nint(wPotSave[0]);
    }
    MGWaveform& waveform = *(partialOutput->GetWaveform(iContact));
    waveform[iSample] += q * (wPot - wPotSave[iContact]);
    wPotSave[iContact] = wPot;
  }
}


void MGWFGenSiggen::SetWaveformParameters(size_t wfLen, double sampFreq, MGWaveform::EWFType type)
{
  if(type != MGWaveform::kCurrent) {
    MGLog(warning) << "SetWaveformParameters(): type (=" << MGWaveform::GetWFTypeName(type) 
                   << ") is not allowed for MGWFGenSiggen. Setting type = kCurrent." << endlog;
    type = MGWaveform::kCurrent;
  }
  MGVWaveformGenerator::SetWaveformParameters(wfLen, sampFreq, type);
}

