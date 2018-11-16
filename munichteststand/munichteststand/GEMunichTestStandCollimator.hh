
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
 * AUTHOR: Manuela Jelen
 * CONTACT: mjelen *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: ..-..-2006
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 *
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDCollimator_HH 
#define _GEMUNICHTESTSTANDCollimator_HH 

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
#include "G4RegionStore.hh"
#include "G4Polyhedra.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "munichteststand/GEMunichTestStandDB.hh"
// ---------------------------------------------------------------------------

class GEMunichTestStandCollimator
{

public:
  
  // default constructor 

  GEMunichTestStandCollimator(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandCollimator();


  void ConstructTestStand(); 

private: 

  // mother volume 

  G4LogicalVolume* fMotherVolume; 
  
  // logical volumes 
 
  G4LogicalVolume* fCollimatorVacuumLogical;
  G4LogicalVolume* fCollimatorCrystalSegmentLogical;
  G4LogicalVolume* fCollimatorCrystalDeadLayerLogical;
  G4LogicalVolume* fCollimatorCrystalSiliconLogical;
  G4LogicalVolume* fCollimatorActiveLogical;
  G4LogicalVolume* fCollimatorSliceLogical;
  G4LogicalVolume* fCollimatorMotherVolumeLogical;
  G4LogicalVolume* fCollimatorSourceHolderLogical;
  G4LogicalVolume* fCollimatorSourceLogical;  
  G4LogicalVolume* fCollimatorFoilLogical;
   

 // physical volumes 


  G4VPhysicalVolume* fCollimatorVacuumPhysical; 
  G4VPhysicalVolume* fCollimatorCrystalSegmentPhysical[18];
  G4VPhysicalVolume* fCollimatorCrystalDeadLayerPhysical;
  G4VPhysicalVolume* fCollimatorCrystalSiliconBottomPhysical; 
  G4VPhysicalVolume* fCollimatorCrystalSiliconTopPhysical;
  G4VPhysicalVolume* fCollimatorPhysical;
  //G4VPhysicalVolume* fCollimatorSlicePhysical;
  G4VPhysicalVolume* fCollimatorMotherPhysical;
  G4VPhysicalVolume* fCollimatorSourceHolderPhysical;
  G4VPhysicalVolume* fCollimatorSourcePhysical;
  G4VPhysicalVolume* fCollimatorFoilPhysical;
 

  // geometry parameters


  G4double fCollimator_Crystal_CenterZ;
  G4double fCollimator_Crystal_InnerRadius;
  G4double fCollimator_Crystal_OuterRadius;
  G4double fCollimator_Crystal_Height;
  int      iCollimator_Segment_Nphi;
  int      iCollimator_Segment_Nz;
  G4double fCollimator_Segment_Height;
  G4double fCollimator_Segment_dPhi;

  //G4double fCollimator_Crystal_DeadLayerOuterThickness;
  //G4double fCollimator_Crystal_DeadLayerInnerThickness;
  //G4double fCollimator_Crystal_DeadLayerTopThickness;
  G4double fCollimator_Crystal_SiliconThickness;

  G4int    fCollimatorPosition;
  G4double fCollimatorSystem_Z;
  G4double fCollimatorSystem_R;
  G4double fCollimatorSystem_Phi;

  G4double fCollimator_MotherVolume_R;
  G4double fCollimator_MotherVolume_Height;
  
  G4double fCollimator_Height;
  G4double fCollimator_OuterRadius;
  G4double fCollimator_LowerHeight;
  G4double fCollimator_InnerRadius;
  G4double fCollimator_Slice_Height;
  int      iCollimator_Slice_Nz;
  G4double fCollimator_Foil_Height;
 
  G4double fCollimator_SourceHolder_Height;
  G4double fCollimator_SourceHolder_InnerRadius;
  G4double fCollimator_SourceHolder_OuterRadius;
  G4double fCollimator_SourceHolder_rPosition;
  G4double fCollimator_SourceHolder_xPosition;
  G4double fCollimator_SourceHolder_yPosition;
  G4double fCollimator_SourceHolder_phiPosition;
  G4double fCollimator_SourceHolder_zPosition ;
  G4double fCollimator_Source_Radius;
  G4double fCollimator_Source_Height;
  G4double fCollimator_System_Rotation;
  G4double fCollimator_Detector_Distance;

 
};

#endif
