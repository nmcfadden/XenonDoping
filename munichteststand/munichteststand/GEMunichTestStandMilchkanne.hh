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
 * AUTHOR: Daniel Lenz
 * CONTACT: lenz *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 22-05-2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDMILCHKANNE_HH 
#define _GEMUNICHTESTSTANDMILCHKANNE_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 

#include "munichteststand/GEMunichTestStandDB.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandMilchkanne
{

public:
  
  // default constructor 

  GEMunichTestStandMilchkanne(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandMilchkanne();

  // methods 

  void ConstructTestStand(); 

private: 
  //G4double fMikesch_CoreElectrode_Height;
  //G4double fMikesch_CoreElectrode_InnerRadius;
  //G4double fMikesch_CoreElectrode_OuterRadius;

  //G4double fMikesch_InnerDeadLayer_Height;
  //G4double fMikesch_InnerDeadLayer_InnerRadius; 
  //G4double fMikesch_InnerDeadLayer_OuterRadius;

  //G4double fMikesch_InnerDeadLayerSubtr_Height;
  //G4double fMikesch_InnerDeadLayerSubtr_InnerRadius; 
  //G4double fMikesch_InnerDeadLayerSubtr_OuterRadius;

  G4double fMikesch_Height;
  //G4double fMikesch_InnerRadius;
  //G4double fMikesch_OuterRadius;

  G4double fMikesch_SubtrHeight;
  //G4double fMikesch_SubtrInnerRadius;
  //G4double fMikesch_SubtrOuterRadius;

  //G4double fMikesch_OuterDeadLayer_Height;
  //G4double fMikesch_OuterDeadLayer_InnerRadius; 
  //G4double fMikesch_OuterDeadLayer_OuterRadius;

  //G4double fMikesch_OuterDeadLayerSubtr_Height;
  //G4double fMikesch_OuterDeadLayerSubtr_InnerRadius; 
  //G4double fMikesch_OuterDeadLayerSubtr_OuterRadius;

  //G4double fMikesch_OuterAluLayer_Height;
  //G4double fMikesch_OuterAluLayer_InnerRadius; 
  //G4double fMikesch_OuterAluLayer_OuterRadius;

  //G4double fMikesch_OuterAluLayerSubtr_Height;
  //G4double fMikesch_OuterAluLayerSubtr_InnerRadius; 
  //G4double fMikesch_OuterAluLayerSubtr_OuterRadius;

  //G4double fMikesch_CoolingStick_Height;
  G4double fMikesch_CoolingStickInside_Height;
  G4double fMikesch_CoolingStickOutside_Height;
  //G4double fMikesch_CoolingStick_InnerRadius; 
  //G4double fMikesch_CoolingStick_OuterRadius;

  //G4double fMilchkanne_OuterWall_Height;
  //G4double fMilchkanne_OuterWall_InnerRadius; 
  //G4double fMilchkanne_OuterWall_OuterRadius;

  //G4double fMilchkanne_OuterWallSubtr_Height;
  //G4double fMilchkanne_OuterWallSubtr_InnerRadius; 
  //G4double fMilchkanne_OuterWallSubtr_OuterRadius;

  //G4double fMilchkanne_InnerWall_Height;
  //G4double fMilchkanne_InnerWall_InnerRadius; 
  //G4double fMilchkanne_InnerWall_OuterRadius;

  //G4double fMilchkanne_InnerWallSubtr_Height;
  //G4double fMilchkanne_InnerWallSubtr_InnerRadius; 
  //G4double fMilchkanne_InnerWallSubtr_OuterRadius;

  //G4double fMilchkanne_Lid_Height;
  //G4double fMilchkanne_Lid_InnerRadius; 
  //G4double fMilchkanne_Lid_OuterRadius;

  //G4double fMilchkanne_LidHole_Height;
  //G4double fMilchkanne_LidHole_InnerRadius; 
  //G4double fMilchkanne_LidHole_OuterRadius;

  G4double fMilchkanne_LidHole_XPosition;
  G4double fMilchkanne_LidHole_YPosition;
  G4double fMilchkanne_SourceXPosition;


  G4LogicalVolume* fMotherVolume;
  G4LogicalVolume* fMkDewarLogical;
  G4LogicalVolume* fMkMikeschLogical;
  G4LogicalVolume* fMkHolderLogical;
  G4LogicalVolume* fMkTeflonWrapLogical;
  G4LogicalVolume* fMkCoolingStickInsideLogical;  
  G4LogicalVolume* fMkCoolingStickOutsideLogical; 
  G4LogicalVolume* fMkDeadLayerLogical;  
  G4LogicalVolume* fMkCoreElectrodeLogical;
  G4LogicalVolume* fMkSourceAEALogical;
  G4LogicalVolume* fMkSourceHolderAEALogical;
  G4LogicalVolume* fMkLn2Logical;
  G4LogicalVolume* fMkDewarBetweenWallsVacuumLogical;
  //  G4LogicalVolume* fMkSourceMotherVolumeLogical;


  G4VPhysicalVolume* fMkDewarPhysical;
  G4VPhysicalVolume* fMkMikeschPhysical;  
  G4VPhysicalVolume* fMkHolderPhysical;  
  G4VPhysicalVolume* fMkTeflonWrapPhysical;  
  G4VPhysicalVolume* fMkCoolingStickInsidePhysical;  
  G4VPhysicalVolume* fMkCoolingStickOutsidePhysical;  
  G4VPhysicalVolume* fMkDeadLayerPhysical;  
  G4VPhysicalVolume* fMkCoreElectrodePhysical;
  G4VPhysicalVolume* fMkSourceAEAPhysical;
  G4VPhysicalVolume* fMkSourceHolderAEAPhysical;
  //  G4VPhysicalVolume* fMkSourceMotherVolumePhysical;
  G4VPhysicalVolume* fMkLn2Physical;
  G4VPhysicalVolume* fMkDewarBetweenWallsVacuumPhysical;

}; 

// --------------------------------------------------------------------------- 

#endif 

