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
 *      This WF generator is a toy generator meant to serve as an example class
 *      until more relevant generators are written.
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * FIRST SUBMISSION: 
 * 
 * REVISION: MM-DD-YYYY
 * 
 */

#ifndef _MGWFGENGAUSSRESP_HH
#define _MGWFGENGAUSSRESP_HH

#include "waveform/MGVWaveformGenerator.hh"

class MGWFGenGaussResponse : public MGVWaveformGenerator
{
  public:
    // ePair value taken from http://www.ortec-online.com/detectors/review_physics/creation.htm
    MGWFGenGaussResponse(const MGCrystalData& crystalData, double preTriggerTime = 50.0*CLHEP::ns, 
                         double sigma = 10.0*CLHEP::ns, double ePair = 2.95*CLHEP::eV);
    virtual ~MGWFGenGaussResponse() {}

    virtual inline const char* GetName() { return "WFGenGaussResponse"; }
    virtual void GenerateWaveformsForHit(double x, double y, double z,
                                         double t, double E, MGWaveformCollection* partialOutput);

  protected:
    virtual void Initialize() {} // a real class would load "fSigma/fDelay/fEPair" from a file here

  protected:
    double fSigma;
    double fEPair;
};

#endif
