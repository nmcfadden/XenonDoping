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
 * DESCRIPTION:.
 *      This module uses a look-up table to compute drift velocities.
 *      The code is based on fields_ppc.c by Karin Lagergren. 
 *
 * AUTHOR:
 *      Khang Nguyen & Jason Detwiler
 *
 * CONTACT:
 *      khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 *
 * FIRST SUBMISSION:
 *      03-26-2011 
 *
 * REVISION: MM-DD-YYYY
 *      05-26-2011: Use MGVDriftVelocity base class, J. Detwiler
 *      05-28-2011: Adapt to const protections and const& storage of
 *                  MGVCrystalFields in base class. J. Detwiler
 *
 */

#ifndef _MGSiggenDriftVelocityTable_HH
#define _MGSiggenDriftVelocityTable_HH

#include "MGVDriftVelocity.hh"

/// Class to hold drift velocity info for a particular charge sign and E-field strength
class MGSiggenDriftVelocityPoint
{
  public:
    /// Constructor
    MGSiggenDriftVelocityPoint() : fE(0.0), f100(0.0), f110(0.0), f111(0.0), fTCorr(0.0) {}
    /// Destructor
    virtual ~MGSiggenDriftVelocityPoint() {}

    virtual inline void SetBasicData(double eField, double m100, double m110, double m111, double tCorr)
      { fE = eField; f100 = m100; f110 = m110; f111 = m111; fTCorr = tCorr; }
    virtual inline double GetE() const { return fE; }
    virtual inline double Get100() const { return f100; }
    virtual inline double Get110() const { return f110; }
    virtual inline double Get111() const { return f111; }
    virtual inline double GetTCorr() const { return fTCorr; }

    virtual inline void SetABC(double a, double b, double c) 
      { fA = a; fB = b; fC = c; }
    virtual inline double GetA() const { return fA; }
    virtual inline double GetB() const { return fB; }
    virtual inline double GetC() const { return fC; }

    virtual inline void SetBpCp(double bp, double cp) 
      { fBp = bp; fCp = cp; }
    virtual inline double GetBp() const { return fBp; }
    virtual inline double GetCp() const { return fCp; }

    typedef double (MGSiggenDriftVelocityPoint::*GetFunction)() const;

  protected:
    double fE;     ///< electric field strength
    double f100;   ///< mobility along 100 axis
    double f110;   ///< mobility along 110 axis
    double f111;   ///< mobility along 111 axis
    double fTCorr; ///< temperature correction factor
    double fA;     ///< A parameter for E*m tensor cal
    double fB;     ///< B parameter for E*m tensor cal
    double fC;     ///< C parameter for E*m tensor cal
    double fBp;    ///< Bp parameter for E*m tensor cal
    double fCp;    ///< Cp parameter for E*m tensor cal
};

/// Class to calculate drift velocity corrections and interpolations
class MGSiggenDriftVelocityData
{
  public:
    /// Constructor
    MGSiggenDriftVelocityData() {}
    /// Destructor
    virtual ~MGSiggenDriftVelocityData() {}

    virtual void AddDatum(MGSiggenDriftVelocityPoint &point) { fData.push_back(point); } 
    virtual void CalculateAnisotropyCorrections();

    virtual double Interpolate(MGSiggenDriftVelocityPoint::GetFunction getFunc, double eField) const;

  protected:
    std::vector<MGSiggenDriftVelocityPoint> fData;
};


/// Drift velocity calculation based on lookup table
class MGSiggenDriftVelocityTable : public MGVDriftVelocity
{
  public: 
    /// Constructor
    MGSiggenDriftVelocityTable(const MGVCrystalFields& fields);
    /// Destructor
    virtual ~MGSiggenDriftVelocityTable() {}

    virtual void SetTemperature(double temperature);
    virtual inline double GetTemperature() const { return fTemperature; }

    /// Load temperature information from fields file (same one used by crystal fields)
    void LoadFieldsFile(const char* fieldsFileName); 

    virtual CLHEP::Hep3Vector GetDriftVelocity( const CLHEP::Hep3Vector& eField, double charge ) const;
    virtual inline CLHEP::Hep3Vector GetVe(const CLHEP::Hep3Vector& eVector) const 
      { return GetDriftVelocity(eVector, CLHEP::eplus); }
    virtual inline CLHEP::Hep3Vector GetVh(const CLHEP::Hep3Vector& eVector) const 
      { return GetDriftVelocity(eVector, -CLHEP::eplus); }

    virtual CLHEP::Hep3Vector GetMobilityVector( const double eField, double charge ) const; 
    virtual double GetTemperatureCorrection( const double eField, double charge) const;
    virtual double GetMinimumTemperature() const { return 77.*CLHEP::kelvin; }
    virtual double GetMaximumTemperature() const { return 110.*CLHEP::kelvin; }

  protected:
    void ReadDriftVelocity();

    double fTemperature;
    MGSiggenDriftVelocityData fHData;
    MGSiggenDriftVelocityData fEData;
};

#endif

