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
 *      This waveform generator replicates the signal generation code "siggen"
 *      by Karin Lagergren. 
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

#ifndef _MGWFGENSIGGEN_HH
#define _MGWFGENSIGGEN_HH

#include "waveform/MGVDriftVelocity.hh"
#include "waveform/MGVWaveformGenerator.hh"

/// Replicates the waveform generation code "siggen" by Karin Lagergren. 
class MGWFGenSiggen : public MGVWaveformGenerator
{
  public:
    /// constructor
    MGWFGenSiggen(const MGVDriftVelocity& drift,
                  size_t wfLen = 1000, double sampFreq = 1.0*CLHEP::GHz, 
                  double preTriggerTime = 50.0*CLHEP::ns);

    /// destructor
    virtual ~MGWFGenSiggen() {}

    /// required by MGVWaveformGenerator interface
    virtual inline const char* GetName() { return "WFGenSiggen"; }

    /// required by MGVWaveformGenerator interface
    virtual void GenerateWaveformsForHit(double x, double y, double z, double t, double E, MGWaveformCollection* partialOutput);

    virtual void SetWaveformParameters(size_t wfLen, double sampFreq, MGWaveform::EWFType type = MGWaveform::kCurrent);
  protected:
    /// Main wf generation function 
    virtual void MakeSignal(double x, double y, double z, double t, double q, MGWaveformCollection* partialOutput);

    /// required by MGVWaveformGenerator interface
    virtual void Initialize();
    //virtual void Zero();

  protected:
    const MGVDriftVelocity* fDrift; ///< drift velocity calculator
};

#endif
