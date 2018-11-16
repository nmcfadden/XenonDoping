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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 01-18-2006
 *
 * The goal of this geometry is to cross-check the neutron tracking done by 
 * MaGe-Geant4 with a MCNP application written by Dano Kollar at MPI-Munich
 */

// ---------------------------------------------------------------------------

#ifndef _GEMCNPTEST_HH
#define _GEMCNPTEST_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"


class GEMCNPTest : public MGGeometryDetector
{

public:
  
  // default constructor
  GEMCNPTest();

  // destructor
  ~GEMCNPTest();

  void ConstructDetector();

}; 

// --------------------------------------------------------------------------- 

#endif
