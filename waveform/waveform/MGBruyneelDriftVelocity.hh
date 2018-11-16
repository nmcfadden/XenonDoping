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
 * 	Class to calculate the drift velocities of charge carriers in Ge 
 * 	detectors.
 *
 * AUTHOR:
 * 	Jing Liu
 *
 * FIRST SUBMISSION:
 * 	06-30-2008, Jing Liu
 *
 * REVISION: MM-DD-YYYY
 * 	07-09-2008 moved some functions from public to private, Jing
 *      05-27-2011, Change name to MGBruyneelDriftVelocity, use base class
 *                  MGVDriftVelocity, call SetupGammaMatrix() in constructor
 *                  to adapt to const protections in base class. J.Detwiler
 *
 */

#ifndef _MGBRUYNEELDRIFTVELOCITY_HH
#define _MGBRUYNEELDRIFTVELOCITY_HH

#include "CLHEP/Vector/ThreeVector.h" // Linear algebra system used here is
#include "CLHEP/Matrix/Matrix.h"      // from CLHEP. The speed is fast enough.

#include "waveform/MGVDriftVelocity.hh" // to get electric field vector

class MGBruyneelDriftVelocity : public MGVDriftVelocity
{
public:
   MGBruyneelDriftVelocity(const MGVCrystalFields& fields);
   virtual ~MGBruyneelDriftVelocity(){}; 

   CLHEP::Hep3Vector GetVe(const CLHEP::Hep3Vector& eVector) const;
   CLHEP::Hep3Vector GetVh(const CLHEP::Hep3Vector& eVector) const;

   virtual void SetCrystalFields(const MGVCrystalFields& fields);
   void SetupGammaMatrix(); // needed by calculations of electron drift
   void PrintGammaMatrix();

   // empirical formula for drift velocities along special directions
   // (needed by the calculation).
   // Coefficients of formula could be different from crystal to crystal
   // in general they should be read in from a coefficient list
   double V100e(double eMag) const;
   double V111e(double eMag) const;
   double V100h(double eMag) const;
   double V111h(double eMag) const;

private:
   CLHEP::HepMatrix fGamma[5];

   CLHEP::HepMatrix RotationAroundX(double angle);
   CLHEP::HepMatrix RotationAroundZ(double angle);

   double A(double eMag) const;
   double R(double eMag) const;

   double K0(double vrel) const
   {return 9.2652-26.3467*vrel+29.6137*vrel*vrel-12.3689*vrel*vrel*vrel;}
   double Lambda(double k0) const
   {return -0.01322*k0+0.41145*k0*k0-0.23657*k0*k0*k0+0.04077*k0*k0*k0*k0;}
   double Omega(double k0) const
   {return 0.00655*k0-0.19946*k0*k0+0.09859*k0*k0*k0-0.01559*k0*k0*k0*k0;}
   
}; 

#endif

