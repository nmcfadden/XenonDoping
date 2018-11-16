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
 * FIRST SUBMISSION: 02-23-2006
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDGERDAlinchenII_HH 
#define _GEMUNICHTESTSTANDGERDAlinchenII_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh" 
#include "G4Box.hh"
#include "G4Sphere.hh" 
#include "G4Trd.hh" 
#include "G4SubtractionSolid.hh" 
#include "G4UnionSolid.hh" 
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4Polyhedra.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "munichteststand/GEMunichTestStandDB.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandGERDAlinchenII
{

public:
  
  // default constructor 

  GEMunichTestStandGERDAlinchenII(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandGERDAlinchenII();

  // methods 

  void ConstructTestStand(); 

private: 

  // mother volume 

  G4LogicalVolume* fMotherVolume; 
  
  // logical volumes 
  
  G4LogicalVolume* fCrystalDeadLayerLogical; 
  G4LogicalVolume* fCrystalSegmentLogical; 
  //G4LogicalVolume* fCrystalSiLayerLogical; 
  G4LogicalVolume* fCrystalSiliconLogical; 

  G4LogicalVolume* fCanLogical; 
  G4LogicalVolume* fVacuumLogical; 
  G4LogicalVolume* fRodLogical; 

  // physical volumes 

  G4VPhysicalVolume* fCanPhysical; 
  G4VPhysicalVolume* fVacuumPhysical; 
  G4VPhysicalVolume* fRodPhysical; 
  G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  G4VPhysicalVolume* fCrystalSiliconBottomPhysical; 
  G4VPhysicalVolume* fCrystalSiliconTopPhysical; 

}; 

// --------------------------------------------------------------------------- 

#endif 

