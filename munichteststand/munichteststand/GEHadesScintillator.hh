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
 * FIRST SUBMISSION: 26-06-2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 * 09-04-2007, added G4Region member for SensitiveRegion, Luciano
 */

// ---------------------------------------------------------------------------

#ifndef _GEHADESSCINTILLATOR_HH 
#define _GEHADESSCINTILLATOR_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh" 

#include "munichteststand/GEMunichTestStandDB.hh"

class G4Region;

// --------------------------------------------------------------------------- 

class GEHadesScintillator : public MGGeometryDetector 
{

public:
  
  // default constructor 

  GEHadesScintillator(); 

  // destructor 

  ~GEHadesScintillator();

  // methods 

  void ConstructDetector(); 

private: 

  G4VPhysicalVolume* physical_gedetector1; 
  G4VPhysicalVolume* physical_gedetector2; 
  G4VPhysicalVolume* physical_scintillator1; 
  G4VPhysicalVolume* physical_scintillator2; 
  G4VPhysicalVolume* physical_steelplate1; 
  G4VPhysicalVolume* physical_steelplate2; 
  G4VPhysicalVolume* physical_steelplate3; 
  G4VPhysicalVolume* physical_steelplate4; 
  G4VPhysicalVolume* physical_leadplate; 
  G4VPhysicalVolume* physical_source; 
  G4VPhysicalVolume* physical_muondisk; 

  G4Region* sensitiveRegion;
}; 

// --------------------------------------------------------------------------- 

#endif 

