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
 * 	Abstract base class containing methods to calculate the electric field   
 *
 * AUTHOR:
 * 	Daniel Lenz
 *
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 * 
 */

#ifndef _MGVFIELDCALCULATION_HH
#define _MGVFIELDCALCULATION_HH

#include "MGVCrystalFields.hh"

class MGVFieldCalculation
{
public:

  MGVFieldCalculation(){;};
  MGVFieldCalculation(MGVCrystalFields* crystalFields){fCrystalFields = crystalFields;};
  virtual ~MGVFieldCalculation() {}


  //Methods to access MGVCrystalFields
  inline void SetCrystalFields(MGVCrystalFields* crystalFields){fCrystalFields = crystalFields;};
  inline MGVCrystalFields* GetCrystalFields(){return fCrystalFields;};


  //Methods to calculate efield and weighting fields
  virtual void CalculateFields() = 0;


protected:
  MGVCrystalFields* fCrystalFields;

};
#endif
