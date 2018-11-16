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
 * 10-23-2009, fCollimatorHalfLength parameter is added to set the collimator
 length in the macro, Neslihan.  
 *                
 */

// ---------------------------------------------------------------------------

#ifndef _GEMunichTestStandGALATEA_HH 
#define _GEMunichTestStandGALATEA_HH 

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

class GEMunichTestStandGALATEA
{

public:
  
  // default constructor 

  GEMunichTestStandGALATEA(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandGALATEA();

  // methods 

  void ConstructTestStand(); 

private: 

  // mother volume 

  G4LogicalVolume* fMotherVolume; 
  
  G4double fCrystal_InnerRadius; 
  G4double fCrystal_OuterRadius;
  G4double fCrystal_Height;
  //G4double fCrystal_SiliconThickness;
  //G4int	 iSegment_Nphi;
  //G4int	 iSegment_Nz;
  //G4double fSegment_Height;
  //G4double fSegment_dPhi;
  
  G4double fCollimatorInnerRadius;
  G4int    fCollimatorPosition;
  G4double fCollimatorSystem_R;
  //G4double fCollimatorSystem_Phi;
  G4double fCollimator_System_Rotation;
  G4double fCollimator_LowerHeight;
  G4double fCollimatorHalfLength; // new parameter  
  
  // logical volumes 
  
  //G4LogicalVolume* fCrystalDeadLayerLogical; 
  //G4LogicalVolume* fCrystalSegmentLogical; 
  //G4LogicalVolume* fCrystalSiLayerLogical; 
  //G4LogicalVolume* fCrystalSiliconLogical; 
  //G4LogicalVolume*  fVacuumChamberLogical;
  
  //G4LogicalVolume* fCanLogical; 
  //G4LogicalVolume* fVacuumLogical; 
  //G4LogicalVolume* fRodLogical; 
  //G4LogicalVolume* fCrystalLogical;
  // physical volumes 

  //G4VPhysicalVolume* fCanPhysical; 
  //G4VPhysicalVolume* fVacuumPhysical; 
  //G4VPhysicalVolume* fRodPhysical; 
  //G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  //G4VPhysicalVolume* fCrystalSiliconBottomPhysical; 
  //G4VPhysicalVolume* fCrystalSiliconTopPhysical; 
  //G4VPhysicalVolume* fVacuumChamberPhysical;
  //G4VPhysicalVolume* fCrystalPhysical;
  //G4VPhysicalVolume*  fCrystalSegmentPhysical[30];
  
 G4VPhysicalVolume* fCollVolPhysical;
 G4VPhysicalVolume* fVacuumTopPhysical;
 G4VPhysicalVolume* fVacuumBottomPhysical;
 G4VPhysicalVolume* fVacuumRimPhysical;
 G4VPhysicalVolume* fVacuumChamberPhysical;
 G4VPhysicalVolume* fRasterPlatePhysical;
 G4VPhysicalVolume* fTeflonBox;
 G4VPhysicalVolume* fTnkhltrPhysical;
 G4VPhysicalVolume* KryoTankPhysical;
 G4VPhysicalVolume* KryoTankTopPhysical;
 G4VPhysicalVolume* KryoTankCuPhysical;
 G4VPhysicalVolume* KuehlFingrPhysical;
 G4VPhysicalVolume* fShieldPhysical;
 G4VPhysicalVolume* fColl_Plate_Physical;
 G4VPhysicalVolume* fColl_Plate_Physical_2;
 G4VPhysicalVolume* fCollPhysical;
 G4VPhysicalVolume* Coll5Physical;
 G4VPhysicalVolume* Coll6Physical;
 G4VPhysicalVolume* SourcePhysical;
 G4VPhysicalVolume* fCrystalPhysical;
}; 

// --------------------------------------------------------------------------- 

#endif 

