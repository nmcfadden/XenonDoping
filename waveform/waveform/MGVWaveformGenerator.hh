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
 * 	Abstract base class containing fields and methods common to waveform generation.
 *
 * AUTHOR:
 * 	J. Detwiler
 *
 * REVISION: MM-DD-YYYY
 * 	02-10-2011 Added empty method Zero() for derived classes. Designed to clean all the
 * 	           variables of the derived generator before each event. Alex
 *      05-27-2011 Don't allow NULL fCrystalData, and remove ChangeCrystalID so
 *                 that it can be made const. J. Detwiler
 *      05-30-2011 GetNWaveformsToGenerate() returns 1 (not 2) when there is
 *                 only one segment and one central contact. J. Detwiler
 * 
 */

#ifndef _MGVWAVEFORMGENERATOR_HH
#define _MGVWAVEFORMGENERATOR_HH


#include "MGCrystalData.hh"
#include "waveform/MGWaveformCollection.hh"
#include "MGCrystalHits.hh"


/// Abstract base class containing fields and methods common to waveform generation.
class MGVWaveformGenerator 
{
  public:
    /// Constructor
    MGVWaveformGenerator(const MGCrystalData& crystalData,
                         size_t wfLen = 512, double sampFreq = 1.0*CLHEP::GHz, 
                         MGWaveform::EWFType type = MGWaveform::kCharge, 
                         double preTriggerTime = 0.0*CLHEP::ns); 

    /// Destructor
    virtual ~MGVWaveformGenerator() {}

    /// Calls Initialize() and sets the fIsInitialized flag
    virtual void CallInitialize() { Initialize(); fIsInitialized = true; }

    /// Returns whether Initialize() has been called
    virtual bool IsInitialized() { return fIsInitialized; }

    /// Derived classes may implement resetting the variables (number of hits/trajectories stored etc.)
    virtual void Zero() {}

    /// Derived classes must give a name to identify their algorithm
    virtual const char* GetName() = 0;


    /// Default version loops over hits and calls GenerateWaveformsForHit(), may be overloaded
    virtual void GenerateWaveformsForHits(MGCrystalHit** crystalHitArray, size_t nHits, MGWaveformCollection* output);

    /// This is where an energy deposition is converted into MGWaveforms. Derived classes must define this function. 
    /** Note when overloading: cumulativeOutput should be added to, as it
     *  contains the cumulative output so far in the wf generation (to save
     *  time). Time is measured relative to the local time of the waveform (t=0
     *  for the first sample), and x, y, z are in the local coordinates of
     *  fCrystalData. x, y, z, t, and E all use G4 (CLHEP) units */
    virtual void GenerateWaveformsForHit(double x, double y, double z, double t, double E, 
                                         MGWaveformCollection* cumulativeOutput) = 0;
    virtual void InitializeWaveformsInCollection(MGWaveformCollection& wfCollection, int crystalID);
  
    /// Get ID of associated crystal
    virtual inline int GetCrystalID() { return fCrystalData->GetCrystalID(); }

    /// Returns the duration in time of the digitzation window
    virtual inline double GetDigitizationWindow() 
      { return double(fWFLength)/fSamplingFrequency; }

    /// Tells how many waveforms this generator will generate
    /** An unsegmented crystal will generate 1 waveform, while a segmented
     * crystal will generate N_segments + 1 (by default; may be overloaded) */
    virtual inline size_t GetNWaveformsToGenerate() { size_t n = fCrystalData->GetNContacts(); return n <= 2 ? 1 : n; }

    /// Sets the time to allow between the start of the waveform and the first energy deposition
    virtual inline void SetPreTriggerTime(double preTriggerTime) { fPreTriggerTime = preTriggerTime; }

    /// Sets the length, sampling frequency, and waveform type of the generated waveforms
    virtual void SetWaveformParameters(size_t wfLen, double sampFreq, 
                                       MGWaveform::EWFType type = MGWaveform::kCharge);

  protected:
    /// Derived classes must define Initialize(). 
    /** This is where the field geometry and segment weighting files should be
     *  loaded or calculated. This function is protected because it should only
     *  be called internally (by CallInitialize()) */
    virtual void Initialize() = 0;

  protected:
    const MGCrystalData* fCrystalData; ///< pointer to crystal data (always non-NULL)
    size_t fWFLength;                  ///< length setting for generated waveforms
    double fSamplingFrequency;         ///< sampling frequency setting for generated waveforms
    MGWaveform::EWFType fWFType;       ///< wf type setting for generated waveforms
    double fPreTriggerTime;            ///< time between start of generated waveform and first Edep
    bool fIsInitialized;               ///< flags whether Initialize() has been called for this generator
};

#endif
