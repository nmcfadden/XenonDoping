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

#ifndef _GEMUNICHTESTSTANDSIMPLE_HH 
#define _GEMUNICHTESTSTANDSIMPLE_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 

#include "munichteststand/GEMunichTestStandDB.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandSimple
{

public:
  
  // default constructor 

  GEMunichTestStandSimple(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandSimple();

  // methods 

  void ConstructTestStand(); 

private: 

  G4double fAluminumHolderHeight; 
  G4double fAluminumHolderThickness; 
  G4double fAluminumHolderToCrystal; 
  G4double fCopperFingerHeight; 
  G4double fSourceHolderHeight; 
  G4double fLeadBrickLengthX; 
  G4double fLeadBrickLengthY; 
  G4double fLeadBrickLengthZ; 
  G4double fCrystalDistanceToSource; 
  G4double fSourceDistanceToBrick; 

  G4LogicalVolume* fMotherVolume; 
  G4LogicalVolume* fCrystalActiveLogical; 
  G4LogicalVolume* fCrystalDeadLayerLogical; 
  G4LogicalVolume* fAluminumHolderLogical; 
  G4LogicalVolume* fBerylliumWindowLogical; 
  G4LogicalVolume* fCopperFingerLogical; 
  G4LogicalVolume* fSourceLogical; 
  G4LogicalVolume* fSourceHolderLogical; 
  G4LogicalVolume* fLeadBrickLogical; 

  G4VPhysicalVolume* fCrystalActivePhysical; 
  G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  G4VPhysicalVolume* fAluminumHolderPhysical; 
  G4VPhysicalVolume* fBerylliumWindowPhysical; 
  G4VPhysicalVolume* fCopperFingerPhysical; 
  G4VPhysicalVolume* fSourcePhysical; 
  G4VPhysicalVolume* fSourceHolderPhysical; 
  G4VPhysicalVolume* fLeadBrickPhysical[6]; 

}; 

// --------------------------------------------------------------------------- 

#endif 

