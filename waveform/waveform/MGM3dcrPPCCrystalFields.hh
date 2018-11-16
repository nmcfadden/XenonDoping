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
 *      This module handles the electric field and weighting potential calculations.
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
 *   05-28-2011  Adapt to new const& storage of crystal data in crystal fields, J. Detwiler
 *
 *
 */


#ifndef _MGM3dcrPPCCrystalFields_HH
#define _MGM3dcrPPCCrystalFields_HH

#include <iostream>

#include "MGVCrystalFields.hh"


class MGM3dcrEFieldPoint 
{
  public:
    MGM3dcrEFieldPoint() : fEFieldR(0.0), fEFieldPhi(0.0), fEFieldZ(0.0) {}
    virtual ~MGM3dcrEFieldPoint() {}

    virtual void SetEFieldR(double eFieldR) { fEFieldR = eFieldR; }
    virtual inline double GetEFieldR() const { return fEFieldR; }

    virtual void SetEFieldPhi(double eFieldPhi) { fEFieldPhi = eFieldPhi; }
    virtual inline double GetEFieldPhi() const { return fEFieldPhi; };

    virtual void SetEFieldZ(double eFieldZ) { fEFieldZ = eFieldZ; }
    virtual inline double GetEFieldZ() const { return fEFieldZ; };

  protected:
    double fEFieldR;
    double fEFieldPhi;
    double fEFieldZ;
};


class MGM3dcrWPotentialPoint 
{
  public:
    MGM3dcrWPotentialPoint(size_t nContacts = 1) { fWPotential.resize(nContacts); }
    virtual ~MGM3dcrWPotentialPoint() {}

    virtual void SetWPotentialPoint( double wPotential, size_t iContacts ) { fWPotential[iContacts] = wPotential; }
    virtual inline double GetWPotentialPoint(size_t iContacts) const { return fWPotential[iContacts]; }

  protected:
    std::vector<double> fWPotential;
};


class MGM3dcrGridWeights
{
  public:
    MGM3dcrGridWeights() {}
    virtual ~MGM3dcrGridWeights() {}

    double fCurrentR, fCurrentPhi, fCurrentZ;
    size_t fVoxelStartR, fVoxelStartPhi, fVoxelStartZ;
    //size_t fNearestRIndex, fNearestPhiIndex, fNearestZIndex;

    double fWeights[2][2];
};


class MGM3dcrPPCCrystalFields : public MGVCrystalFields
{
  public:
    MGM3dcrPPCCrystalFields(const MGCrystalData& crystalData) : MGVCrystalFields(crystalData), 
      fRMin(0.0), fRMax(0.0), fRStep(0.0), fZMin(0.0), fZMax(0.0), fZStep(0.0) {}
    virtual ~MGM3dcrPPCCrystalFields() {} 

    virtual inline void SetFieldsFileName(const char* fieldsFileName) { fFieldsFileName = fieldsFileName; }
    virtual inline std::string GetFieldsFileName() const { return fFieldsFileName; }

    // Field setup data
    virtual inline void SetRMin(double rMin) { fRMin = rMin; }
    virtual inline double GetRMin() const { return fRMin; }
    virtual inline void SetRMax(double rMax) { fRMax = rMax; }
    virtual inline double GetRMax() const { return fRMax; }
    virtual inline void SetRStep(double rStep) { fRStep = rStep; }
    virtual inline double GetRStep() const { return fRStep; }

    virtual inline void SetZMin(double zMin) { fZMin = zMin; }
    virtual inline double GetZMin() const { return fZMin; }
    virtual inline void SetZMax(double zMax) { fZMax = zMax; }
    virtual inline double GetZMax() const { return fZMax; }
    virtual inline void SetZStep(double zStep) { fZStep = zStep; }
    virtual inline double GetZStep() const { return fZStep; }

    // Load and save fields
    void LoadFields();
    void SaveFields() {}

    // Methods to access the electric field, weighting potential field, and electric potential
    CLHEP::Hep3Vector GetEField(const CLHEP::Hep3Vector &coordinates) const;
    CLHEP::Hep3Vector GetWField( const CLHEP::Hep3Vector &coordinates, size_t contact = 0 ) const;
    double GetWPotential( const CLHEP::Hep3Vector &coordinates, size_t contact = 0 ) const;

    // Potentially useful functions
    //virtual size_t NearestFieldGridRIndex(const CLHEP::Hep3Vector &coordinates) const;
    //virtual inline size_t NearestFieldGridPhiIndex(const CLHEP::Hep3Vector &coordinates) const { return 0; }
    //virtual size_t NearestFieldGridZIndex(const CLHEP::Hep3Vector &coordinates) const;
    //virtual bool FindNearestFieldGridIndices( const CLHEP::Hep3Vector &coordinates ) const;
    //virtual bool CheckEFieldExists( const CLHEP::Hep3Vector &coordinates ) const;

  protected:
    std::string fFieldsFileName;   

    double fRMin, fRMax, fRStep;
    double fZMin, fZMax, fZStep;

    bool ReadEField(const char* eFieldFileName);
    bool ReadWPotential(const char* wPotentialFileName);

    size_t NearestVoxelGridRIndex(double rho) const;
    inline size_t NearestVoxelGridPhiIndex(double /*phi*/) const { return 0; }
    size_t NearestVoxelGridZIndex(double z) const;

    bool MoveToNearestValidVoxel(const CLHEP::Hep3Vector &coordinates) const;

    bool CheckVoxelEFieldExists( double voxelStartR, double voxelStartZ ) const;

    void CalculateGridWeights(const CLHEP::Hep3Vector &coordinates) const;

    MGM3dcrGridWeights fGridWeights;
    MGM3dcrGridWeights* GetChangeableGridWeights() const
      { return const_cast <MGM3dcrGridWeights*> (&fGridWeights); }

    // arrays for electric field and potential field
    std::vector< std::vector<MGM3dcrEFieldPoint> > fEFieldVector;
    std::vector< std::vector<MGM3dcrWPotentialPoint> > fWPotentialVector;
}; 

#endif /*ifndef _MGM3dcrPPCCrystalFields_HH*/

