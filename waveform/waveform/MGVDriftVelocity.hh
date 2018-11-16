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
 * 	Virtual base class for Ge charge carrier drift velocity classes
 *
 * AUTHOR:
 * 	Jason Detwiler
 *
 * FIRST SUBMISSION:
 * 	May 2011, J. Detwiler
 *
 * REVISION: MM-DD-YYYY
 * 	05-23-2011: initial revision, J. Detwiler
 *      05-28-2011: Add const protections, and make MGVCrystalFields a required
 *                  argument of the constructor. J. Detwiler
 *
 */

#ifndef _MGVDRIFTVELOCITY_HH
#define _MGVDRIFTVELOCITY_HH

#include "CLHEP/Vector/ThreeVector.h" // Linear algebra system used here is
#include "CLHEP/Matrix/Matrix.h"      // from CLHEP. The speed is fast enough.

#include "MGCrystalData.hh" // to get crystal axes orientation and more...
#include "waveform/MGVCrystalFields.hh" // to get electric field vector

class MGVDriftVelocity
{
public:
   MGVDriftVelocity(const MGVCrystalFields& fields) : fCrystalFields(&fields) {}
   virtual ~MGVDriftVelocity(){}; 

   // Allow access to change and retrieve crystal fields. Use references
   // because fCrystalFields is always guaranteed to be non-NULL. Note: derived
   // classes requiring that fCrystalFields not change should overload
   // SetCrystalFields() to emit an error and not change fCrystalFields.
   virtual inline void SetCrystalFields(const MGVCrystalFields& fields) { fCrystalFields=&fields; }
   virtual inline const MGVCrystalFields& GetCrystalFields() const { return *fCrystalFields; }
   virtual inline const MGCrystalData& GetCrystalData() const { return fCrystalFields->GetCrystalData(); }

   virtual inline CLHEP::Hep3Vector GetDriftVelocity(const CLHEP::Hep3Vector& eVector, double charge) const
     { return charge > 0 ? GetVh(eVector) : GetVe(eVector); }
   virtual inline CLHEP::Hep3Vector GetDriftVelocityAt(const CLHEP::Hep3Vector& coordinates, double charge) const
     { return GetDriftVelocity(fCrystalFields->GetEField(coordinates), charge); }

   virtual CLHEP::Hep3Vector GetVe(const CLHEP::Hep3Vector& eVector) const = 0;
   virtual inline CLHEP::Hep3Vector GetVeAt(const CLHEP::Hep3Vector& coordinates) const
     { return GetVe(fCrystalFields->GetEField(coordinates)); }

   virtual CLHEP::Hep3Vector GetVh(const CLHEP::Hep3Vector& eVector) const = 0;
   virtual inline CLHEP::Hep3Vector GetVhAt(const CLHEP::Hep3Vector& coordinates) const
     { return GetVh(fCrystalFields->GetEField(coordinates)); }

protected:
   const MGVCrystalFields* fCrystalFields;
}; 

#endif

