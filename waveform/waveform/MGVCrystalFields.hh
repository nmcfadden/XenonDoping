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
 * 	Abstract base class containing methods to store and access the electric and weigthing fields  
 *
 * AUTHOR:
 * 	Daniel Lenz
 *
 * REVISION: MM-DD-YYYY
 *   05-28-2011  Make MGCrystalData a required argument for the constructor:
 *               don't allow fields without an associated crystal geometry
 *               J. Detwiler
 * 
 */

#ifndef _MGVCRYSTALFIELDS_HH
#define _MGVCRYSTALFIELDS_HH

#include "MGCrystalData.hh"
#include "io/MGLogger.hh"
#include "CLHEP/Vector/ThreeVector.h"


class MGVCrystalFields 
{
public:

  MGVCrystalFields(const MGCrystalData& crystal) { fCrystalData = &crystal; }
  virtual ~MGVCrystalFields() {}

  // Allow access to change and retrieve MGCrystalData. Use references
  // because fCrystalData is always guaranteed to be non-NULL. Note:
  // derived classes requiring that fCrystalData not change should overload
  // SetCrystalData() to emit an error and not change fCrystalData.
  virtual inline void SetCrystalData(const MGCrystalData& crystal){fCrystalData = &crystal;};
  virtual inline const MGCrystalData& GetCrystalData() const {return *fCrystalData;};


  //Methods to save, load and draw(?) the electric and weighting fields
  virtual void SaveFields() =0;
  virtual void LoadFields() =0;
  //  virtual void DrawFields();

  //Methods to access the electric and weighting field
  virtual CLHEP::Hep3Vector GetEField(const CLHEP::Hep3Vector& coordinates)const =0;
  virtual CLHEP::Hep3Vector GetWField(const CLHEP::Hep3Vector& /*coordinates*/, size_t /*contact*/) const 
    { MGLog(warning) << "MGVCrystalFields::GetWField() not implemented" << endlog; return CLHEP::Hep3Vector(0,0,0); }
  virtual double GetWPotential(const CLHEP::Hep3Vector& coordinates, size_t contact) const =0;


protected:
  const MGCrystalData* fCrystalData;

};
#endif
