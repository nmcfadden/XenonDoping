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
 *     Generate waveforms based on MGBruyneelDriftVelocity.
 *     Store combined hits, end points of electrons/holes and their trajectories
 *
 * AUTHOR:
 *     Jing Liu
 *
 * FIRST SUBMISSION:
 *     10-10-2008, Jing.
 *
 *
 * REVISION: MM-DD-YYYY
 *      11-24-2008, added Runge-Kutta method for calculation, Jing.
 *      10-02-2011, 1. Added variables for root branches for hits, trajectories and end points
 *                  2. Added enum for save option: save hits, end points, trajectories, waveforms.
 *                     Waveforms suppress in the output file is not yet implemented, but in plans.
 *                     Added correspondent method to specify the save option.
 *                     Alex.
 *      05-27-2011, Pass drift calculator in via constructor, and get crystal
 *                  fields and crystal data from it. J. Detwiler
 *      07-08-2011, Added doxygen-friendly comments. Alex
 *
 */

#ifndef _MGWFGENFROMDRIFT_HH
#define _MGWFGENFROMDRIFT_HH

#include <string>

#include "waveform/MGVDriftVelocity.hh"
#include "waveform/MGVWaveformGenerator.hh"
#include "waveform/MGWaveformWriterROOT.hh"

/// Class to calculate waveforms and trajectories of electron/holes by drift simulation
class MGWFGenFromDrift : public MGVWaveformGenerator
{
  public:

    /// Donstructor.
    MGWFGenFromDrift(
      const MGVDriftVelocity& drift,
      size_t wfLen = 1000,
      double sampFreq = 1.0*CLHEP::GHz,
      double preTriggerTime = 50.0*CLHEP::ns,
      double ePair = 2.95*CLHEP::eV,
      MGWaveform::EWFType wfType = MGWaveform::kCharge);

    /// Destructor.
    virtual ~MGWFGenFromDrift();

    /// Set sampling frequency
    void SetSampFreq(double sampFreq=1.*CLHEP::GHz)   { fSamplingFrequency = sampFreq; }
    /// Get sampling frequency
    double GetSampFreq()                              { return fSamplingFrequency; }

    /// Set waveform length
    void SetWFLength(size_t waveformLength=1000)      { fWFLength = waveformLength; }
    /// Get waveform length
    size_t GetWFLength()                              { return fWFLength; }

    /// Set waveform type
    void SetWFType(MGWaveform::EWFType wfType = MGWaveform::kCharge)  { fWFType = wfType; }
    /// Get waveform type
    MGWaveform::EWFType GetWFType()                   { return fWFType; }

    /** Set save options
     * Specifies output branches to be saved */
    void SetSaveOptions( unsigned char opt )          { fSaveOpt = opt; }
    /// Get save options
    unsigned char GetSaveOptions()                    { return fSaveOpt; }

    /// Set additional branches according to fSaveOpt: hits/end points/trajectories
    void SetExtraBranches(MGWaveformWriterROOT* wfwriter);

    /// Calculate occupancy for a set of hits
    void CalculateOccupancyForHits(MGCrystalHits& hits);

    /// Function migrated from MGWFlibGenEuler.cc. Currently commented out
    void Anisotropy(char* output);

    /// Function migrated from MGVWFlibGenerator.cc. Currently commented out
    void WriteWFlib(char* output);

    /// Set calculation precision
    void SetPrecision(int precision);
    /// Get calculation precision
    int GetPrecision()                                { return fPrecision; }

    /// Get generator name
    virtual inline const char* GetName()              { return "WFGenFromDrift"; }

    /// Overloaded MGVWaveformGenerator::GenerateWaveformsForHit()
    void GenerateWaveformsForHit(double x, double y, double z, double t, double E, MGWaveformCollection* partialOutput);

protected:
    ///Calculate waveform for a given hit
    void CalculateWFsForHit( CLHEP::Hep3Vector hit, MGWaveform::EWFType wfType );
    /// Calculate waveform at any time t
    double GetWFValueAtTime( double t, size_t iContact);

    /// Overloaded MGVWaveformGenerator::Initialize()
    virtual void Initialize();
    /// Zero the variables
    virtual void Zero();


  public:
    /// Specifies output branches
    enum SaveOptions {
      kSaveHits                  = 1,  // 0b00000001
      kSaveTrajectories          = 2,  // 0b00000010
      kSaveEndPoints             = 4,  // 0b00000100
      kSaveWaveforms             = 8,  // 0b00001000
//    Now separately
      kSaveEndPointsHoles        = 16, // 0b00010000
      kSaveEndPointsElectrons    = 32, // 0b00100000
      kSaveTrajectoriesHoles     = 64, // 0b01000000
      kSaveTrajectoriesElectrons = 128 // 0b10000000
    };

  private:
    static const int MAX_NHITS=100;       ///< Maximal number of hits in the output tree
    static const int MAX_NTRJPOINTS=1000; ///< Maximal number of trajectories in the output tree

    // trajectories of electrones/holes in the crystal
    int    hits_totnum;                   ///< Number of hits
    float  hits_edep[MAX_NHITS];          ///< Hit energy
    float  hits_xpos[MAX_NHITS];          ///< Hit X position
    float  hits_ypos[MAX_NHITS];          ///< Hit Y position
    float  hits_zpos[MAX_NHITS];          ///< Hit Z position
    float  hits_time[MAX_NHITS];          ///< Hit time

    // end points of electrones/holes after the drift
//    float  endp_elec_xpos[MAX_NHITS];
//    float  endp_elec_ypos[MAX_NHITS];
    float  endp_elec_zpos[MAX_NHITS];     ///< Electron drift end point Z
    float  endp_elec_rho[MAX_NHITS];      ///< Electron drift end point rho
    float  endp_elec_phi[MAX_NHITS];      ///< Electron drift end point phi

//    float  endp_hole_xpos[MAX_NHITS];
//    float  endp_hole_ypos[MAX_NHITS];
    float  endp_hole_zpos[MAX_NHITS];     ///< Hole drift end point Z
    float  endp_hole_rho[MAX_NHITS];      ///< Hole drift end point Z
    float  endp_hole_phi[MAX_NHITS];      ///< Hole drift end point Z

    // trajectories of electrones/holes drift in the crystal
    int    trj_numpoints;                          ///< Number of trajectories
    float  trj_elec_x[MAX_NHITS][MAX_NTRJPOINTS];  ///< Electron trajectory X position
    float  trj_elec_y[MAX_NHITS][MAX_NTRJPOINTS];  ///< Electron trajectory Y position
    float  trj_elec_z[MAX_NHITS][MAX_NTRJPOINTS];  ///< Electron trajectory Z position

    float  trj_hole_x[MAX_NHITS][MAX_NTRJPOINTS];  ///< Hole trajectory X position
    float  trj_hole_y[MAX_NHITS][MAX_NTRJPOINTS];  ///< Hole trajectory Y position
    float  trj_hole_z[MAX_NHITS][MAX_NTRJPOINTS];  ///< Hole trajectory Z position

  protected:
    const MGVDriftVelocity* fDrift;    ///< pointer to drift data
    double fEPair;                     ///< ionization energy
    int fPrecision;                    ///< precision

    size_t fNcontacts;                 ///< number of contacts
    double* fPresentWaveform;          ///< pointer to the presently calculated waveform

    unsigned char fSaveOpt;            ///< save option to specify output branches
};

#endif
