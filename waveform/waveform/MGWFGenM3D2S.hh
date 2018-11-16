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
 * 	Abstract base class containing fields and methods common to waveforms
 * 	Class is focuessed on the data management and analysis aspects.
 * 	The computing of waveforms is in the MJWaveform Manager class. 
 *
 * AUTHOR:
 * 	Y. Chan
 *
 * FIRST SUBMISSION: 
 * 
 * REVISION: MM-DD-YYYY
 * 
 */

#ifndef _MGWFGENM3D2S_HH
#define _MGWFGENM3D2S_HH

#include "waveform/MGVWaveformGenerator.hh"

#include <string>

class MGWFGenM3D2S : public MGVWaveformGenerator
{
  public:
    // ePair value taken from http://www.ortec-online.com/detectors/review_physics/creation.htm
    MGWFGenM3D2S(const MGCrystalData& crystalData, size_t wfLen = 512, double sampFreq = 1.0*CLHEP::GHz, 
                 double preTriggerTime = 50.0*CLHEP::ns, double ePair = 2.95*CLHEP::eV);
    virtual ~MGWFGenM3D2S(); 

    void SetGrid(int aGridxMin, int aGridxMax, int aGridxSpacing,
                 int aGridyMin, int aGridyMax, int aGridySpacing,
                 int aGridzMin, int aGridzMax, int aGridzSpacing);

    virtual inline const char* GetName() { return "WFGenM3D2S"; }
    void GenerateWaveformsForHit(double x, double y, double z, double t, double E, MGWaveformCollection* partialOutput);


    void InterpolateTrilinearWaveform(double x_mm, double y_mm, double z_mm);
    double GetWFValueAtTime(double t, size_t iSeg);

    virtual inline void SetLookUpTableFileName(const char* filename) { fLookUpTableFileName = filename; }
    virtual inline void SetLookUpTableWFLenAndSampFreq(size_t length, double sampFreq)
      { fWFTWaveformLength = length; fWFTSamplingFreq = sampFreq; }

    virtual void DumpWFTable();

  protected:
    virtual void Initialize();

  protected:
    double fEPair;

    // specific to M3D2S 

    float* fWaveformArray;	//memory array to store waveform table
    std::string fLookUpTableFileName;
    size_t fWFTWaveformLength;
    double fWFTSamplingFreq;
    double* fPresentWaveform;	//interpolated waveform for all segments
    int  fSegmentOffset;
    FILE *fInput;
    int fGridxMin;
    int fGridxMax;
    int fGridxSpacing;
    int fGridyMin;
    int fGridyMax;
    int fGridySpacing;
    int fGridzMin;
    int fGridzMax;
    int fGridzSpacing;
    int fGridyGridzOffset;
    int fGridzOffset;
    int fXSize;
    int fYSize;
    int fZSize;
    int fTrilinearIndex[8];	//for trilinear interpolation
    int fPresentGridIndex;
    int fPreviousGridIndex;
};

#endif
