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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 02-28-2006, added Siegfried, Xiang
 * 10-30-2006, added Collimator, Manuela 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTAND_HH
#define _GEMUNICHTESTSTAND_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"
#include "munichteststand/GEMunichTestStandDB.hh"
#include "munichteststand/GEMunichTestStandLab.hh" 
#include "munichteststand/GEMunichTestStandSimple.hh" 
#include "munichteststand/GEMunichTestStandCoincidence.hh" 
#include "munichteststand/GEMunichTestStandLN2.hh" 
#include "munichteststand/GEMunichTestStandVacuumChamber.hh"
#include "munichteststand/GEMunichTestStandGERDAlinchenII.hh"
#include "munichteststand/GEMunichTestStandSiegfried.hh"
#include "munichteststand/GEMunichTestStandCollimator.hh"
#include "munichteststand/GEMunichTestStandMilchkanne.hh"
#include "munichteststand/GEMunichTestStandGALATEA.hh"
// --------------------------------------------------------------------------- 

class GEMunichTestStand : public MGGeometryDetector
{

public:
  
  // default constructor

  GEMunichTestStand();

  // destructor

  ~GEMunichTestStand();

  // construction inherited from MJGeometryDetector

  void ConstructDetector();

private:

  GEMunichTestStandDB*             fTestStandDB; 
  GEMunichTestStandLab*            fLab; 
  GEMunichTestStandSimple*         fTestStandSimple; 
  GEMunichTestStandCoincidence*    fTestStandCoincidence; 
  GEMunichTestStandLN2*            fTestStandLN2; 
  GEMunichTestStandVacuumChamber*  fVacuumChamber; 
  GEMunichTestStandGERDAlinchenII* fTestStandGERDAlinchenII; 
  GEMunichTestStandSiegfried*      fTestStandSiegfried;
  GEMunichTestStandCollimator*     fTestStandCollimator;
  GEMunichTestStandMilchkanne*     fTestStandMilchkanne;
  GEMunichTestStandGALATEA*	   fTestStandGALATEA;
}; 

// --------------------------------------------------------------------------- 

#endif
