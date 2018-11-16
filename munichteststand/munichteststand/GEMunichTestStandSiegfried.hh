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
 * REVISION: MM-DD-YYYY
 *
 * 28-02-2006, Siegfried teststand with Canberra cryostat based on Siegfried. Xiang
 * 10-14-2010, Added the variable for the SourceHolderStick for the collimated
 *               source at the side. Alex.
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDSiegfried_HH 
#define _GEMUNICHTESTSTANDSiegfried_HH 

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

class GEMunichTestStandSiegfried
{

public:
  
  // default constructor 

  GEMunichTestStandSiegfried(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandSiegfried();

  // methods 
  void SetCoincidenceFlag(G4bool myflag)
    { fCoincidence_Flag = myflag; } ;

  void ConstructTestStand(); 

private: 

  // mother volume 

  G4LogicalVolume* fMotherVolume; 
  
  // logical volumes 
  
  G4LogicalVolume* fCrystalDeadLayerLogical; 
  G4LogicalVolume* fCrystalLogical; 
  //G4LogicalVolume* fCrystalSiLayerLogical; 
  G4LogicalVolume* fCrystalSiliconLogical; 

  G4LogicalVolume* fCanLogical; 
  G4LogicalVolume* fVacuumLogical; 
  G4LogicalVolume* fIRCanLogical; 
  G4LogicalVolume* fIRBaseLogical; 
  G4LogicalVolume* fRodLogical; 
  G4LogicalVolume* fBaseLogical; 
  G4LogicalVolume* fBaseRodLogical; 
  G4LogicalVolume* fCoolingFingerLogical; 
  G4LogicalVolume* fDewarNeckLogical; 
  G4LogicalVolume* fDewarExpandLogical; 
  G4LogicalVolume* fDewarLogical; 
  G4LogicalVolume* fFanoutBoxLogical; 
  //G4LogicalVolume* fPreampBoxLogical; 
  G4LogicalVolume* fPreampRingLogical; 
  G4LogicalVolume* fLN2Logical; 
  G4LogicalVolume* fREGeLogical[2];

  G4LogicalVolume* fSourceHolderLogical;
  G4LogicalVolume* fSourceLogical;

  G4LogicalVolume* fSourceHolderStickLogical;

  G4LogicalVolume* fSourceHolderAEALogical;
  G4LogicalVolume* fSourceAEALogical;

  G4LogicalVolume* fPreTubLogical;
  G4LogicalVolume* fPreBottomLogical;
  
  G4LogicalVolume* fCollimatorLogical;
  G4LogicalVolume* fSourceMotherVolume;
  
  // physical volumes 

  G4VPhysicalVolume* fCanPhysical; 
  G4VPhysicalVolume* fVacuumPhysical; 
  G4VPhysicalVolume* fIRCanPhysical; 
  G4VPhysicalVolume* fIRBasePhysical; 
  G4VPhysicalVolume* fRodPhysical; 
  G4VPhysicalVolume* fBasePhysical;
  G4VPhysicalVolume* fBaseRodPhysical;
  G4VPhysicalVolume* fCoolingFingerPhysical;
  G4VPhysicalVolume* fDewarNeckPhysical;
  G4VPhysicalVolume* fDewarExpandPhysical;
  G4VPhysicalVolume* fDewarPhysical;
  G4VPhysicalVolume* fFanoutBoxPhysical[3];
  //G4VPhysicalVolume* fPreampBoxPhysical[19];
  //G4VPhysicalVolume* fPreampRingPhysical;
  G4VPhysicalVolume* fLN2Physical;
  G4VPhysicalVolume* fREGePhysical[2]; 
  G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  G4VPhysicalVolume* fCrystalSiliconBottomPhysical; 
  G4VPhysicalVolume* fCrystalSiliconTopPhysical; 
  G4VPhysicalVolume* fCrystalPhysical;
  //G4VPhysicalVolume* fSourceHolderPhysical;
  //G4VPhysicalVolume* fSourcePhysical;

  G4VPhysicalVolume* fSourceHolderStickPhysical;

  G4VPhysicalVolume* fSourceHolderAEAPhysical;
  G4VPhysicalVolume* fSourceAEAPhysical;

  G4VPhysicalVolume* fPreTubPhysical;
  G4VPhysicalVolume* fPreBottomPhysical;

  G4VPhysicalVolume* fCollimatorPhysical;
  G4VPhysicalVolume* fSourceMotherPhysical;
  
  // geometry parameters
  G4double fCrystal_InnerRadius;
  G4double fCrystal_OuterRadius;
  G4double fCrystal_Height;
  //G4double fCrystal_DeadLayerOuterThickness;
  //G4double fCrystal_DeadLayerInnerThickness;
  //G4double fCrystal_DeadLayerTopThickness;
  G4double fCrystal_SiliconThickness;

  G4double fCan_OuterRadius;
  G4double fCan_Height;
  //G4double fCan_Thickness;

  G4double fRod_Height;
  //G4double fRod_Radius;

  G4double fCan_CenterZ;
  G4double fIRCan_CenterZ;
  G4double fIRBase_CenterZ;
  G4double fRod_CenterZ;
  G4double fBase_CenterZ;
  G4double fBaseRod_CenterZ;
  G4double fCoolingFinger_CenterZ;
  G4double fDewarNeck_CenterZ;
  G4double fDewarExpand_CenterZ;
  G4double fDewar_CenterZ;
  G4double fLN2_CenterZ;
  G4double fFanoutBox_OffsetY;
  //G4double fPreampBox_lengthX;
  G4double fPreampRing_OuterRadius;

  G4double fPreTub_CenterY;
  G4double fPreBottom_CenterY;

  //source system
  G4double fSourceSystem_Z;
  G4double fSourceSystem_R;
  G4double fSourceSystem_Phi;

  G4double fSourceHolder_Height; 
  G4double fSourceHolder_InnerRadius;
  G4double fSourceHolder_OuterRadius;
  G4double fSourceHolder_DistanceToCanTopSurface;
  G4double fSourceHolder_rPosition;
  G4double fSourceHolder_phiPosition;
  //G4double fSourceHolder_xPosition;
  //G4double fSourceHolder_yPosition;
  G4double fSourceHolder_zPosition;
  G4double fSourceHolder_zLimit;
  
  G4double fCollimator_Height;
  G4double fCollimatorHole_UpperHeight;
  
  G4double fSource_Radius;
  G4double fSource_Height;
  //AEA source------------------------------------
  G4double fSourceHolderHeightAEA;
  G4double fSourceHeightAEA;

  G4double fSourceHolderAEA_rPosition;
  G4double fSourceHolderAEA_phiPosition;

  //G4double fSourceHolderAEA_xPosition;
  //G4double fSourceHolderAEA_yPosition;
  G4double fSourceHolderAEA_zPosition;

  G4double fSourceHolderAEA_zLimit;

  G4bool   useCollimator;
  G4bool   useSourceHolderStick;

  G4double fSourceHolderStickLength;

  //----------------------------------------------
  G4bool   fCoincidence_Flag;

  G4double fREGe_Height;
  G4double fREGe_Radius;
  G4double fREGe_DistanceToCanOuterSurface;

}; 

// --------------------------------------------------------------------------- 

#endif 

