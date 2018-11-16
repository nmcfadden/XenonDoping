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
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDLAB_HH 
#define _GEMUNICHTESTSTANDLAB_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 

// --------------------------------------------------------------------------- 

class GEMunichTestStandLab
{

public:
  
  // default constructor 

  GEMunichTestStandLab(); 

  // destructor 

  ~GEMunichTestStandLab();

  // getters 

  inline G4LogicalVolume* GetLabVolume()
  { return fLabLogical; }; 

  // methods 

  void ConstructLab(); 

private: 

  G4double fLabBoxX; 
  G4double fLabBoxY;
  G4double fLabBoxZ; 

  G4LogicalVolume* fLabLogical; 

}; 

// --------------------------------------------------------------------------- 

#endif 

