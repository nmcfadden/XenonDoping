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
 * 03-03-2006, Added Siegfried parameters and volumes, Xiang
 * 03-04-2006, Added cooling, preamp boxes and fanout boxes, Xiang
 * 20-04-2006, Siegfried IR Can position wrong, fixed
 *             removed all preamp boxes, Xiang
 * June-July,2006, Jing, For siegfried, add preamp boxes, collimator, 
 *                       modify can, IR can, AEA source geometry, 
 *                       redefine the source system.
 * Nov-Dec-2006 Manuela, added Detector-Collimator combination,
 * Jan 2007, Manuela, added special detector
 * 10-23-2009, Neslihan, fCollimator_HalfLength parameter is added to set the collimator length in the macro
 * 06-16-2010  Alex. in SetSiegfriedSourceSystemToCanTop(): Position of source for Siegfried Test Stand is
 *                   now related to source itself, not to the center of its mother volume
 * 10-14-2010  Alex. 1. Added variables for the HolderStick as another ~11 cm of material
                     2. Removed references to the crystal segments in SiegfriedTestStand(K1) which are not treated correctly.
                        Division for segments should be done in the G4Step analysis, not as separate volumes
 * 11-23-2010  Alex. 1. The method SetSiegfriedSourceSystemToCanSide moved to .cc
                     2. Added method to return the outer radius of the vacuum can
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDDB_HH 
#define _GEMUNICHTESTSTANDDB_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 
#include "munichteststand/GEMunichTestStandDBMessenger.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandDB
{

public:

  // default constructor 

  GEMunichTestStandDB(); 

  // destructor 

  ~GEMunichTestStandDB();

  // getters 

  inline G4LogicalVolume* GetMotherVolume() 
  { return fMotherVolume; };

  inline G4LogicalVolume* GetCrystalDSGUnsegmentedActiveLogical() 
  { return fCrystalDSGUnsegmentedActiveLogical; };

  inline G4LogicalVolume* GetCrystalDSGUnsegmentedDeadLayerLogical() 
  { return fCrystalDSGUnsegmentedDeadLayerLogical; };

  inline G4LogicalVolume* GetSourceAEALogical() 
  { return fSourceAEALogical; };

  inline G4LogicalVolume* GetSourceHolderAEALogical() 
  { return fSourceHolderAEALogical; };

  inline G4LogicalVolume* GetStringDetectorLogical() 
  { return fStringDetectorLogical; };

  inline G4LogicalVolume* GetStringSourceLogical() 
  { return fStringSourceLogical; };

  inline G4LogicalVolume* GetCrystalActiveLogical() 
  { return fCrystalActiveLogical; };

  inline G4LogicalVolume* GetCrystalDeadLayerLogical() 
  { return fCrystalDeadLayerLogical; };

  inline G4LogicalVolume* GetAluminumHolderLogical() 
  { return fAluminumHolderLogical; };

  inline G4LogicalVolume* GetBerylliumWindowLogical() 
  { return fBerylliumWindowLogical; };

  inline G4LogicalVolume* GetCopperFingerLogical() 
  { return fCopperFingerLogical; };

  inline G4LogicalVolume* GetREGeLogical(int rege_copy_number)
  { return fREGeLogical[rege_copy_number]; };

  inline G4LogicalVolume* GetSourceLogical() 
  { return fSourceLogical; };

  inline G4LogicalVolume* GetSourceHolderLogical() 
  { return fSourceHolderLogical; };

  inline G4LogicalVolume* GetLeadBrickLogical() 
  { return fLeadBrickLogical; };

  inline G4LogicalVolume* GetDewarLogical()
  { return fDewarLogical; }; 

  inline G4LogicalVolume* GetLN2Logical()
  { return fLN2Logical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedRing1Logical()
  { return fCrystalHolderDSGUnsegmentedRing1Logical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedRing2Logical()
  { return fCrystalHolderDSGUnsegmentedRing2Logical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedSkrewLogical()
  { return fCrystalHolderDSGUnsegmentedSkrewLogical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedBarLogical()
  { return fCrystalHolderDSGUnsegmentedBarLogical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedStringLogical()
  { return fCrystalHolderDSGUnsegmentedStringLogical; }; 

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedPlateSmallLogical() 
  { return fCrystalHolderDSGUnsegmentedPlateSmallLogical; };

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedPlateLargeLogical() 
  { return fCrystalHolderDSGUnsegmentedPlateLargeLogical; };

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedRodLogical() 
  { return fCrystalHolderDSGUnsegmentedRodLogical; };

  inline G4LogicalVolume* GetCrystalHolderDSGUnsegmentedClampLogical() 
  { return fCrystalHolderDSGUnsegmentedClampLogical; };

  G4double GetCrystalHeight()
  { return fCrystalHeight; }; 

  G4double GetCrystalDSGUnsegmentedHeight()
  { return fCrystalDSGUnsegmentedHeight; }; 

  G4double GetCrystalInnerRadius()
  { return fCrystalInnerRadius; }; 

  G4double GetCrystalOuterRadius()
  { return fCrystalOuterRadius; }; 

  G4double GetCrystalDeadLayer()
  { return fCrystalDeadLayer; }; 

  G4double GetAluminumHolderHeight()
  { return fAluminumHolderHeight; }; 

  G4double GetAluminumHolderThickness()
  { return fAluminumHolderThickness; }; 

  G4double GetAluminumHolderToCrystal()
  { return fAluminumHolderToCrystal; }; 

  G4double GetCopperFingerHeight()
  { return fCopperFingerHeight; }; 

  G4double GetREGeHeight()
  { return fREGeHeight; };

  G4double GetREGeRadius()
  { return fREGeRadius; };

  G4double GetSourceHolderHeight()
  { return fSourceHolderHeight; };

  G4double GetSourceHolderHeightAEA()
  { return fSourceHolderHeightTotalAEA; };

  G4double GetSourceHeightAEA()
  { return fSourceHeightAEA; };

  G4double GetSourceHolderRadius()
  { return fSourceHolderRadius; };

  G4double GetLeadBrickLengthX()
  { return fLeadBrickLengthX; };

  G4double GetLeadBrickLengthY()
  { return fLeadBrickLengthY; };

  G4double GetLeadBrickLengthZ()
  { return fLeadBrickLengthZ; };

  G4double GetCrystalDistanceToSource()
  { return fCrystalDistanceToSource; };

  G4double GetSourceDistanceToBrick()
  { return fSourceDistanceToBrick; };

  G4double GetDistance01()
  { return fDistance01; };

  G4double GetStringDetectorHeight()
  { return fStringDetectorHeight; };

  G4double GetDewarHeight()
  { return fDewarHeight; };
  
  G4double GetDewarInnerRadius()
  { return fDewarInnerRadius; };

  G4double GetDewarOuterRadius()
  { return fDewarOuterRadius; };
  
  G4double GetLN2Height()
  { return fLN2Height; }

  G4double GetStringSourceHeight()
  { return fStringSourceHeight; };

  G4String GetTestStandType()
  { return fTestStandType; };

  G4int    GetSourceHoleNumber()
  { return fSourceHoleNumber; };

  G4String GetCrystalType()
  { return fCrystalType; };

  G4ThreeVector GetDSGHolderSkrewPosition(G4int n);

  G4double GetCrystalHolderDSGUnsegmentedGap()
  { return fCrystalHolderDSGUnsegmentedGap; };

  G4double GetCrystalHolderDSGUnsegmentedRing1Height()
  { return fCrystalHolderDSGUnsegmentedRing1Height; };

  G4double GetCrystalHolderDSGUnsegmentedRing2Height()
  { return fCrystalHolderDSGUnsegmentedRing2Height; }; 

  G4double GetCrystalHolderDSGUnsegmentedSkrewHeight()
  { return fCrystalHolderDSGUnsegmentedSkrewHeight; }; 

  G4double GetCrystalHolderDSGUnsegmentedBarLengthX() 
  { return fCrystalHolderDSGUnsegmentedBarLengthX; }; 

  G4double GetCrystalHolderDSGUnsegmentedBarLengthY() 
  { return fCrystalHolderDSGUnsegmentedBarLengthY; }; 

  G4double GetCrystalHolderDSGUnsegmentedBarLengthZ()
  { return fCrystalHolderDSGUnsegmentedBarLengthZ; }; 

  G4double GetCrystalHolderDSGUnsegmentedRodHeight()
  { return fCrystalHolderDSGUnsegmentedRodHeight; }; 

  G4double GetCrystalHolderDSGUnsegmentedPlateSmallHeight()
  { return fCrystalHolderDSGUnsegmentedPlateSmallHeight; }; 

  G4double GetCrystalHolderDSGUnsegmentedClampHeight()
  { return fCrystalHolderDSGUnsegmentedClampHeight; }; 

  //----------------------------------------------
  // getters for Siegfried
  //----------------------------------------------

  inline G4LogicalVolume* GetSiegfriedCrystalDeadLayerLogical()
  { return fSiegfried_CrystalDeadLayerLogical; };

  inline G4LogicalVolume* GetSiegfriedCrystalLogical()
  { return fSiegfried_CrystalLogical; };

  inline G4LogicalVolume* GetSiegfriedCrystalSiliconLogical()
  { return fSiegfried_CrystalSiliconLogical; };

  inline G4LogicalVolume* GetSiegfriedCanLogical()
  { return fSiegfried_CanLogical; };

  inline G4LogicalVolume* GetSiegfriedVacuumLogical()
  { return fSiegfried_VacuumLogical; };

  inline G4LogicalVolume* GetSiegfriedIRCanLogical()
  { return fSiegfried_IRCanLogical; };

  inline G4LogicalVolume* GetSiegfriedIRBaseLogical()
  { return fSiegfried_IRBaseLogical; };

  inline G4LogicalVolume* GetSiegfriedRodLogical()
  { return fSiegfried_RodLogical; };

  inline G4LogicalVolume* GetSiegfriedBaseLogical()
  { return fSiegfried_BaseLogical; };

  inline G4LogicalVolume* GetSiegfriedBaseRodLogical()
  { return fSiegfried_BaseRodLogical; };

  inline G4LogicalVolume* GetSiegfriedCoolingFingerLogical()
  { return fSiegfried_CoolingFingerLogical; };

  inline G4LogicalVolume* GetSiegfriedLN2Logical()
  { return fSiegfried_LN2Logical; };

  inline G4LogicalVolume* GetSiegfriedDewarNeckLogical()
  { return fSiegfried_DewarNeckLogical; };

  inline G4LogicalVolume* GetSiegfriedDewarExpandLogical()
  { return fSiegfried_DewarExpandLogical; };

  inline G4LogicalVolume* GetSiegfriedDewarLogical()
  { return fSiegfried_DewarLogical; };

  inline G4LogicalVolume* GetSiegfriedFanoutBoxLogical()
  { return fSiegfried_FanoutBoxLogical; };

  //inline G4LogicalVolume* GetSiegfriedPreampBoxLogical()
  //{ return fSiegfried_PreampBoxLogical; };

  inline G4LogicalVolume* GetSiegfriedPreampRingLogical()
  { return fSiegfried_PreampRingLogical; };

  inline G4LogicalVolume* GetSiegfriedSourceHolderLogical()
  { return fSiegfried_SourceHolderLogical; };

  inline G4LogicalVolume* GetSiegfriedSourceHolderStickLogical() 
  { return fSiegfried_SourceHolderStickLogical; };

  inline G4LogicalVolume* GetSiegfriedSourceLogical()
  { return fSiegfried_SourceLogical; };

  inline G4LogicalVolume* GetSiegfriedCollimatorLogical()
  { return fSiegfried_CollimatorLogical; };

  inline G4LogicalVolume* GetSiegfriedPreTubLogical()
  { return fSiegfried_PreTubLogical; };

  inline G4LogicalVolume* GetSiegfriedPreBottomLogical()
  { return fSiegfried_PreBottomLogical; };

  inline G4LogicalVolume* GetSiegfriedSourceMotherVolume()
  { return fSiegfried_SourceMotherVolume; };

  G4double GetSiegfriedCrystalInnerRadius()
  { return fSiegfried_Crystal_InnerRadius; };

  G4double GetSiegfriedCrystalOuterRadius()
  { return fSiegfried_Crystal_OuterRadius; };

  G4double GetSiegfriedCrystalHeight()
  { return fSiegfried_Crystal_Height; };

  G4double GetSiegfriedCrystalSiliconThickness()
  { return fSiegfried_Crystal_SiliconThickness; };

  G4double GetSiegfriedCanHeight()
  { return fSiegfried_Can_Height; };

  G4double GetSiegfriedCanOuterRadius()
  { return fSiegfried_Can_OuterRadius; };

  G4double GetSiegfriedRodHeight()
  { return fSiegfried_Rod_Height; };

  G4double GetSiegfriedFanoutBoxOffsetY()
  { return fSiegfried_FanoutBox_OffsetY; };

/*
  G4double GetSiegfriedPreampBoxlengthX()
  { return fSiegfried_PreampBox_lengthX; };
*/

  G4double GetSiegfriedPreampRingOuterRadius()
  { return fSiegfried_PreampRing_OuterRadius; };

  //source system
  G4double GetSiegfriedSourceSystemZ()
  { return fSiegfried_SourceSystem_Z; };

  G4double GetSiegfriedSourceSystemR()
  { return fSiegfried_SourceSystem_R; };

  G4double GetSiegfriedSourceSystemPhi()
  { return fSiegfried_SourceSystem_Phi; };

  G4bool GetSiegfriedCollimatorStatus()
  { return fSiegfried_useCollimator; };

  G4double GetSiegfriedCollimatorHeight()
  { return fSiegfried_collimator_Height; };

  G4double GetSiegfriedCollimatorHoleUpperHeight()
  { return fSiegfried_collimatorHole_UpperHeight; };

  G4double GetSiegfriedSourceHolderHeight()
  { return fSiegfried_SourceHolder_Height; };

  G4double GetSiegfriedSourceHolderInnerRadius()
  { return fSiegfried_SourceHolder_InnerRadius; };

  G4double GetSiegfriedSourceHolderOuterRadius()
  { return fSiegfried_SourceHolder_OuterRadius; };

  G4double GetSiegfriedSourceHolderDistanceToCanTopSurface()
  { return fSiegfried_SourceHolder_DistanceToCanTopSurface; };

  G4double GetSiegfriedSourceHolderrPosition()
  { return fSiegfried_SourceHolder_rPosition; };

  G4double GetSiegfriedSourceHolderphiPosition()
  { return fSiegfried_SourceHolder_phiPosition; };

  G4double GetSiegfriedSourceHolderzPosition()
  { return fSiegfried_SourceHolder_zPosition; };

  G4double GetSiegfriedSourceHolderAEArPosition()
  { return fSiegfried_SourceHolderAEA_rPosition; };

  G4double GetSiegfriedSourceHolderAEAphiPosition()
  { return fSiegfried_SourceHolderAEA_phiPosition; };

  G4double GetSiegfriedSourceHolderAEAzPosition()
  { return fSiegfried_SourceHolderAEA_zPosition; };

  G4bool GetSiegfriedSourceHolderStickStatus()
  { return fSiegfried_useSourceHolderStick; };

  G4double GetSiegfriedSourceHolderStickLength()
  { return fSiegfried_SourceHolderStickLength; };

  G4double GetSiegfriedREGeToCanSurfaceDistance()
  { return fSiegfried_REGe_DistanceToCanOuterSurface; };

  G4double GetSiegfriedCanCenterZ()
  { return fSiegfried_Can_CenterZ; };

  G4double GetSiegfriedIRCanCenterZ()
  { return fSiegfried_IRCan_CenterZ; };

  G4double GetSiegfriedIRBaseCenterZ()
  { return fSiegfried_IRBase_CenterZ; };

  G4double GetSiegfriedRodCenterZ()
  { return fSiegfried_Rod_CenterZ; };

  G4double GetSiegfriedBaseCenterZ()
  { return fSiegfried_Base_CenterZ; };

  G4double GetSiegfriedBaseRodCenterZ()
  { return fSiegfried_BaseRod_CenterZ; };

  G4double GetSiegfriedCoolingFingerCenterZ()
  { return fSiegfried_CoolingFinger_CenterZ; };

  G4double GetSiegfriedDewarNeckCenterZ()
  { return fSiegfried_DewarNeck_CenterZ; };

  G4double GetSiegfriedDewarExpandCenterZ()
  { return fSiegfried_DewarExpand_CenterZ; };

  G4double GetSiegfriedDewarCenterZ()
  { return fSiegfried_Dewar_CenterZ; };

  G4double GetSiegfriedLN2CenterZ()
  { return fSiegfried_LN2_CenterZ; };

  G4double GetSiegfriedPreTubCenterY()
  { return fSiegfried_PreTub_CenterY; };

  G4double GetSiegfriedPreBottomCenterY()
  { return fSiegfried_PreBottom_CenterY; };


  //----------------------------------------------
  // setters for Siegfried
  //----------------------------------------------
  void SetSiegfriedSourceSystemZ(G4double zpos)
  { fSiegfried_SourceSystem_Z = zpos; }

  void SetSiegfriedSourceSystemToCanTop(G4double zpos)
  { fSiegfried_SourceSystem_Z = fSiegfried_Can_CenterZ 
      + 0.5 * fSiegfried_Can_Height 
      + zpos ;} // In this case, fSiegfried_SourceSystem_Z is position of source, not source system. It is needed when calculating where to put source for SII: rotate or not

  void SetSiegfriedSourceSystemR(G4double rpos)
  { fSiegfried_SourceSystem_R = rpos;}

  void SetSiegfriedSourceSystemToCanSide(G4double rpos);
//  { fSiegfried_SourceSystem_R = fSiegfried_Can_OuterRadius
//      + rpos
//      + fSiegfried_collimator_Height; }
//  Now moved to GEMunichTestStandDB.cc to include also the length of the holder stick

  void SetSiegfriedSourceSystemPhi(G4double phipos)
  { fSiegfried_SourceSystem_Phi = phipos; }

  void SetSiegfriedCollimatorStatus(G4bool status)
  { fSiegfried_useCollimator = status; };

  void SetSiegfriedSourceHolderxPosition(G4double xpos)
  { fSiegfried_SourceHolder_xPosition = xpos; };

  void SetSiegfriedSourceHolderyPosition(G4double ypos)
  { fSiegfried_SourceHolder_yPosition = ypos; };

  void SetSiegfriedSourceHolderzPosition(G4double zpos)
  { 
    fSiegfried_SourceHolder_zPosition = zpos; 
    fSiegfried_SourceHolderAEA_zPosition = zpos; 
  };

  void SetSiegfriedSourceHolderrPosition(G4double rpos)
  { 
    fSiegfried_SourceHolder_rPosition = rpos; 
    fSiegfried_SourceHolderAEA_rPosition = rpos; 
  };

  void SetSiegfriedSourceHolderphiPosition(G4double phipos)
  { 
    fSiegfried_SourceHolder_phiPosition = phipos; 
    fSiegfried_SourceHolderAEA_phiPosition = phipos; 
  };

  void SetSiegfriedSourceHolderCenterzToCanTop(G4double zpos);
  //{ fSiegfried_SourceHolder_DistanceToCanTopSurface = zpos; };

  void SetSiegfriedREGeToCanSurfaceDistance(G4double dis)
  { fSiegfried_REGe_DistanceToCanOuterSurface = dis; };
 
  // holder stick
  void SetSiegfriedSourceHolderStickStatus(G4bool status)
  { fSiegfried_useSourceHolderStick = status; };

  void SetSiegfriedSourceHolderStickLength(G4double len)
  { fSiegfried_SourceHolderStickLength = len;};

  //----------------------------------------------
  // getters for Collimator
  //----------------------------------------------


 inline G4LogicalVolume* GetCollimatorVacuumLogical()
  { return fCollimatorVacuumLogical; };

inline G4LogicalVolume* GetCollimatorCrystalSegmentLogical()
  { return fCollimatorCrystalSegmentLogical; };

 inline G4LogicalVolume* GetCollimatorCrystalDeadLayerLogical()
  { return fCollimatorCrystalDeadLayerLogical; };

 inline G4LogicalVolume* GetCollimatorCrystalSiliconLogical()
  { return fCollimatorCrystalSiliconLogical; };

 inline G4LogicalVolume* GetCollimatorActiveLogical()
  { return fCollimatorActiveLogical; };

 inline G4LogicalVolume* GetCollimatorSliceLogical()
  { return fCollimatorSliceLogical; };

 inline G4LogicalVolume* GetCollimatorFoilLogical()
   { return fCollimatorFoilLogical; };

 inline G4LogicalVolume* GetCollimatorMotherVolumeLogical()
  { return fCollimatorMotherVolumeLogical; };

 inline G4LogicalVolume* GetCollimatorSourceHolderLogical()
 { return fCollimatorSourceHolderLogical; };
  
 inline G4LogicalVolume* GetCollimatorSourceLogical()
 {return fCollimatorSourceLogical; };

 G4double GetCollimatorCrystalCenterZ()
  { return fCollimator_Crystal_CenterZ; };

 G4double GetCollimatorCrystalInnerRadius()
  { return fCollimator_Crystal_InnerRadius; };

 G4double GetCollimatorCrystalOuterRadius()
  { return fCollimator_Crystal_OuterRadius; };

 G4double GetCollimatorCrystalHeight()
  { return fCollimator_Crystal_Height; };

 G4int    GetCollimatorSegmentNphi()
  { return iCollimator_Segment_Nphi; };

  G4int    GetCollimatorSegmentNz()
  { return iCollimator_Segment_Nz; };

  G4double GetCollimatorSegmentHeight()
  { return fCollimator_Segment_Height; };

 G4double GetCollimatorCrystalSiliconThickness()
  { return fCollimator_Crystal_SiliconThickness; };
  
//collimator system

  G4int GetCollimatorPosition()
  { return fCollimatorPosition; };
  
  G4double GetCollimatorSystemZ()
  { return fCollimatorSystem_Z; };

  G4double GetCollimatorSystemR()
  { return fCollimatorSystem_R; };

  G4double GetCollimatorSystemPhi()
  { return fCollimatorSystem_Phi; };
 
  G4double GetCollimatorMotherVolumeR()
  { return fCollimator_MotherVolume_R; };

  G4double GetCollimatorMotherVolumeHeight()
  { return fCollimator_MotherVolume_Height; };
 
  G4double GetCollimatorHeight()
  { return fCollimator_Height; };
 
  G4double GetCollimatorOuterRadius()
  { return fCollimator_OuterRadius; };

  G4double GetCollimatorLowerHeight()
  { return fCollimator_LowerHeight; };
  
  G4double GetCollimatorHalfLength() // new parameter to set the length of the collimator
  { return fCollimator_HalfLength; };  // default 25 mm

  G4double GetCollimatorInnerRadius()
  { return fCollimator_InnerRadius; };

  G4double GetCollimatorSliceHeight()
  { return fCollimator_Slice_Height; };

  G4int    GetCollimatorSliceNz()
  { return iCollimator_Slice_Nz; };

 G4double GetCollimatorFoilHeight()
  { return fCollimator_Foil_Height; };
  
  G4double GetCollimatorSourceHolderHeight()
  { return fCollimator_SourceHolder_Height; };
  
  G4double GetCollimatorSourceHolderInnerRadius()
  { return fCollimator_SourceHolder_InnerRadius; };
  
  G4double  GetCollimatorSourceHolderOuterRadius()
  { return fCollimator_SourceHolder_OuterRadius; };
  
  G4double GetCollimatorSourceHolderrPosition()
  { return fCollimator_SourceHolder_rPosition; };
  
  G4double GetCollimatorSourceHolderphiPosition()
  { return fCollimator_SourceHolder_phiPosition; };

  G4double GetCollimatorSourceHolderxPosition()
  { return fCollimator_SourceHolder_xPosition; };
  
  G4double GetCollimatorSourceHolderyPosition()
  { return fCollimator_SourceHolder_yPosition; };

  G4double GetCollimatorSourceHolderzPosition() 
  { return fCollimator_SourceHolder_zPosition; };

  G4double GetCollimatorDetectorDistance()
  { return fCollimator_Detector_Distance; };

  G4double GetCollimatorSystemRotation()
  { return fCollimator_System_Rotation; };

  G4String GetCollimatorMaterial()
  { return fCollimatorMaterial; };

  G4String GetCollimatorFoilMaterial()
  { return fCollimatorFoilMaterial; };

  G4String GetCollimatorType()
  { return fCollimatorType; };

 
  //----------------------------------------------
  // setters for Collimator
  //----------------------------------------------
  
  void SetCollimatorPosition (G4int collimatorposition)
  { fCollimatorPosition = collimatorposition; }

  void SetCollimatorCrystalCenterZ (G4double z)
  { fCollimator_Crystal_CenterZ = z; };

  void SetCollimatorSystemZ(G4double zpos)
  { fCollimatorSystem_Z = zpos; }

  void SetCollimatorSystemR(G4double z)
  { fCollimatorSystem_R = z; }

  void SetCollimatorSystemPhi(G4double phipos)
  { fCollimatorSystem_Phi = phipos; }
 
  void SetCollimatorMotherVolumeR(G4double rpos)
  { fCollimator_MotherVolume_R = rpos; }

  void SetCollimatorMotherVolumeHeight(G4double zpos)
  { fCollimator_MotherVolume_Height = zpos; }
  
  void SetCollimatorInnerRadius(G4double rpos)
  { fCollimator_InnerRadius = rpos; }

  void SetCollimatorOuterRadius(G4double rpos)
  { fCollimator_OuterRadius = rpos; }

 void SetCollimatorLowerHeight(G4double zpos)
  { fCollimator_LowerHeight = zpos; }
  
 void SetCollimatorHalfLength(G4double zpos)
 { fCollimator_HalfLength = zpos; }

 void SetCollimatorSliceHeight(G4double zpos)
  { fCollimator_Slice_Height = zpos; }

 void SetCollimatorFoilHeight(G4double zpos)
  { fCollimator_Foil_Height = zpos; }

 void SetCollimatorDetectorDistance(G4double zpos)
  { fCollimator_Detector_Distance = zpos; }

 void SetCollimatorSystemRotation(G4double phipos)
  { fCollimator_System_Rotation = phipos; }

 void SetCollimatorMaterial(G4String collimatormaterial)
 {fCollimatorMaterial = collimatormaterial; }

 void SetCollimatorFoilMaterial(G4String collimatorfoilmaterial)
 {fCollimatorFoilMaterial = collimatorfoilmaterial; }
 
 void SetCollimatorType(G4String collimatortype)
 {fCollimatorType = collimatortype; }
 
 
  //----------------------------------------------
  // getters for Milchkanne
  //----------------------------------------------
  G4double GetLidHoleXPosition(){return fMilchkanne_LidHole_XPosition;};
  G4double GetLidHoleYPosition(){return fMilchkanne_LidHole_YPosition;};

  inline G4LogicalVolume* GetMkDewarLogical(){return MkDewarLogical;};
  inline G4LogicalVolume* GetMkMikeschLogical(){return MkMikeschLogical;};
  inline G4LogicalVolume* GetMkHolderLogical(){return MkHolderLogical;};
  inline G4LogicalVolume* GetMkTeflonWrapLogical(){return MkTeflonWrapLogical;};
  inline G4LogicalVolume* GetMkCoolingStickInsideLogical(){return MkCoolingStickInsideLogical;};
  inline G4LogicalVolume* GetMkCoolingStickOutsideLogical(){return MkCoolingStickOutsideLogical;};
  inline G4LogicalVolume* GetMkDeadLayerLogical(){return MkDeadLayerLogical;};
  inline G4LogicalVolume* GetMkCoreElectrodeLogical(){return MkCoreElectrodeLogical;};
  inline G4LogicalVolume* GetMkLn2Logical(){return MkLn2Logical;};
  inline G4LogicalVolume* GetMkDewarBetweenWallsVacuumLogical(){return MkDewarBetweenWallsVacuumLogical;}; 
  inline G4LogicalVolume* GetMkSourceLogical(){return fSourceAEALogical;}; 

  inline G4double GetMkSourceXPosition(){return fMilchkanne_SourceXPosition;};
  inline G4double GetMkMikeschHeight(){return fMikesch_Height;};
  inline G4double GetMkMikeschSubtrHeight(){return fMikesch_SubtrHeight;}
  inline G4double GetMkCoolingstickHeight(){return fMikesch_CoolingStick_Height;}
  inline G4double GetMkCoolingstickInsideHeight(){return fMikesch_CoolingStickInside_Height;}
  inline G4double GetMkCoolingstickOutsideHeight(){return fMikesch_CoolingStickOutside_Height;}

  //----------------------------------------------
  // setters for Milchkanne
  //----------------------------------------------
  void SetMkSourceXPosition (G4double sourcepositionx)
  {  fMilchkanne_SourceXPosition = sourcepositionx + fMilchkanne_OuterWall_OuterRadius; }


  //----------------------------------------------
  // setters 
  //----------------------------------------------

  void SetMotherVolume(G4LogicalVolume* mothervolume) 
  { fMotherVolume = mothervolume; }; 

  void SetCrystalHeight(G4double h) 
  { fCrystalHeight = h; }; 
  
  void SetCrystalInnerRadius(G4double r) 
  { fCrystalInnerRadius = r; }; 

  void SetCrystalOuterRadius(G4double r) 
  { fCrystalOuterRadius = r; }; 

  void SetCrystalDeadLayer(G4double d) 
  { fCrystalDeadLayer = d; }; 

  void SetCrystalDistanceToSource(G4double d)
  { fCrystalDistanceToSource = d; }; 

  void SetSourceDistanceToBrick(G4double d)
  { fSourceDistanceToBrick = d; }; 

  void SetStringDetectorHeight(G4double d)
  { fStringDetectorHeight = d; }; 

  void SetStringSourceHeight(G4double d)
  { fStringSourceHeight = d; }; 

  void SetTestStandType(G4String teststandtype)
  { fTestStandType = teststandtype; }; 

  void SetSourceHoleNumber(G4int n) 
  { fSourceHoleNumber = n; }; 

  void SetCrystalType(G4String crystaltype) 
  { fCrystalType = crystaltype; };  

  // methods 

  void UpdateTestStand(); 
  void ConstructTestStandDB(); 
  void ConstructREGeDetector(int irege, int icopynumber);

private: 

  G4LogicalVolume* fMotherVolume; 
//------------------------------------------------------------------
// simple test stand 
// crystal aluminum beryllium and copper are all for REGe detector
//------------------------------------------------------------------

  G4LogicalVolume* fCrystalActiveLogical; 
  G4LogicalVolume* fCrystalDeadLayerLogical; 
  G4LogicalVolume* fAluminumHolderLogical; 
  G4LogicalVolume* fBerylliumWindowLogical;
  G4LogicalVolume* fCopperFingerLogical; 
  G4LogicalVolume* fREGeLogical[2];

  G4LogicalVolume* fSourceLogical; 
  G4LogicalVolume* fSourceHolderLogical; 
  G4LogicalVolume* fLeadBrickLogical; 

  G4double fCrystalHeight;
  G4double fCrystalInnerRadius; 
  G4double fCrystalOuterRadius; 
  G4double fCrystalDeadLayer; 

  G4double fCrystalHoleHeight;
  G4double fCrystalHoleInnerRadius; 
  G4double fCrystalHoleOuterRadius; 

  G4double fAluminumHolderHeight;
  G4double fAluminumHolderToCrystal; // holder inner surface to crystal outer surface
  G4double fAluminumHolderThickness; 
  G4double fAluminumHolderInnerRadius; 
  G4double fAluminumHolderOuterRadius; 

  G4double fCopperFingerHeight;
  G4double fCopperFinderRadius; 

  G4double fREGeRadius;
  G4double fREGeHeight;

  G4double fSourceHeight; 
  G4double fSourceRadius; 
  G4double fSourceHolderHeight; 
  G4double fSourceHolderRadius; 

  G4double fLeadBrickLengthX;
  G4double fLeadBrickLengthY;
  G4double fLeadBrickLengthZ;

  G4double fCrystalDistanceToSource; 
  G4double fSourceDistanceToBrick; 

  // simple test stand expansion : coincidence 

  G4double fDistance01; 

  // ----------------------------------------------
  // liquid N2 test stand 
  // ----------------------------------------------

  G4LogicalVolume* fCrystalDSGUnsegmentedActiveLogical; 
  G4LogicalVolume* fCrystalDSGUnsegmentedDeadLayerLogical; 

  G4LogicalVolume* fSourceAEALogical; 
  G4LogicalVolume* fSourceHolderAEALogical; 

  G4LogicalVolume* fStringDetectorLogical; 
  G4LogicalVolume* fStringSourceLogical; 

  G4LogicalVolume* fDewarLogical; 
  G4LogicalVolume* fLN2Logical; 

  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRing1Logical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRing2Logical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedSkrewLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedBarLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedStringLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedPlateSmallLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedPlateLargeLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRodLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedClampLogical; 

  G4double fCrystalDSGUnsegmentedHeight; 
  G4double fCrystalDSGUnsegmentedInnerRadius; 
  G4double fCrystalDSGUnsegmentedOuterRadius;                   
  G4double fCrystalDSGUnsegmentedDeadLayer; 
  							     
  G4double fCrystalHolderDSGUnsegmentedRing1OuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedRing1InnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedRing1Height; 
				           	     
  G4double fCrystalHolderDSGUnsegmentedRing2OuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedRing2InnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedRing2Height; 
				           	     							     
  G4double fCrystalHolderDSGUnsegmentedGap;                    
							     
  G4double fCrystalHolderDSGUnsegmentedSkrewHole1OuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHole1InnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHole1Height; 
							     
  G4double fCrystalHolderDSGUnsegmentedSkrewHole2OuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHole2InnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHole2Height; 
							     
  G4double fCrystalHolderDSGUnsegmentedSkrewOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHeight; 
				
  G4double fCrystalHolderDSGUnsegmentedPlateSmallOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedPlateSmallInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedPlateSmallHeight; 
  
  G4double fCrystalHolderDSGUnsegmentedPlateLargeOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedPlateLargeInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedPlateLargeHeight; 

  G4double fCrystalHolderDSGUnsegmentedRodOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedRodInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedRodHeight; 

  G4double fCrystalHolderDSGUnsegmentedClampOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedClampInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedClampHeight; 
			     
  G4double fCrystalHolderDSGUnsegmentedBarLengthX; 
  G4double fCrystalHolderDSGUnsegmentedBarLengthY;  
  G4double fCrystalHolderDSGUnsegmentedBarLengthZ; 
							     
  G4double fCrystalHolderDSGUnsegmentedStringOuterRadius; 
  G4double fCrystalHolderDSGUnsegmentedStringInnerRadius; 
  G4double fCrystalHolderDSGUnsegmentedStringHeight; 

  G4String fCrystalType; 

  G4double fSourceHeightAEA; 
  G4double fSourceRadiusAEA; 
  G4double fSourceHolderHeightTotalAEA; 
  G4double fSourceHolderHeightNeckAEA; 
  G4double fSourceHolderRadiusAEA; 
  G4double fSourceHolderRadiusNeckAEA; 
  G4int    fSourceHoleNumber; 

  G4double fStringRadius; 
  G4double fStringDetectorHeight; 
  G4double fStringSourceHeight; 

  G4double fDewarHeight; 
  G4double fDewarInnerRadius; 
  G4double fDewarOuterRadius; 

  G4double fLN2Height; 

  G4String fTestStandType; 

  // ----------------------------------------------
  // Siegfried
  // ----------------------------------------------
  
  G4LogicalVolume* fSiegfried_CrystalDeadLayerLogical; 
  G4LogicalVolume* fSiegfried_CrystalLogical; 
  //G4LogicalVolume* fSiegfried_CrystalSiLayerLogical; 
  G4LogicalVolume* fSiegfried_CrystalSiliconLogical; 

  G4LogicalVolume* fSiegfried_CanLogical; 
  G4LogicalVolume* fSiegfried_VacuumLogical; 
  G4LogicalVolume* fSiegfried_IRCanLogical; 
  G4LogicalVolume* fSiegfried_IRBaseLogical; 
  G4LogicalVolume* fSiegfried_RodLogical; 
  G4LogicalVolume* fSiegfried_BaseLogical; 
  G4LogicalVolume* fSiegfried_BaseRodLogical; 
  G4LogicalVolume* fSiegfried_CoolingFingerLogical; 
  G4LogicalVolume* fSiegfried_DewarNeckLogical; 
  G4LogicalVolume* fSiegfried_DewarExpandLogical; 
  G4LogicalVolume* fSiegfried_DewarLogical; 
  G4LogicalVolume* fSiegfried_LN2Logical; 
  G4LogicalVolume* fSiegfried_PreampRingLogical; 
  //G4LogicalVolume* fSiegfried_PreampBoxLogical; 
  G4LogicalVolume* fSiegfried_FanoutBoxLogical; 

  G4LogicalVolume* fSiegfried_SourceMotherVolume;
  G4LogicalVolume* fSiegfried_SourceHolderLogical;
  G4LogicalVolume* fSiegfried_SourceLogical;
  G4LogicalVolume* fSiegfried_CollimatorLogical;

  G4LogicalVolume* fSiegfried_SourceHolderStickLogical;

  G4LogicalVolume* fSiegfried_PreTubLogical;
  G4LogicalVolume* fSiegfried_PreBottomLogical;
  
  G4double fSiegfried_Crystal_InnerRadius;
  G4double fSiegfried_Crystal_OuterRadius;
  G4double fSiegfried_Crystal_Height;
  G4double fSiegfried_Crystal_DeadLayerOuterThickness;
  G4double fSiegfried_Crystal_DeadLayerInnerThickness;
  G4double fSiegfried_Crystal_DeadLayerTopThickness;
  G4double fSiegfried_Crystal_SiliconThickness;

  G4double fSiegfried_Can_OuterRadius;
  G4double fSiegfried_Can_Height;
  G4double fSiegfried_Can_Thickness;
  G4double fSiegfried_Can_CenterZ;

  G4double fSiegfried_IRCan_OuterRadius;
  G4double fSiegfried_IRCan_Height;
  G4double fSiegfried_IRCan_Thickness;
  G4double fSiegfried_IRCan_CenterZ;

  G4double fSiegfried_IRBase_OuterRadius;
  G4double fSiegfried_IRBase_InnerRadius;
  G4double fSiegfried_IRBase_Height;
  G4double fSiegfried_IRBase_CenterZ;

  G4double fSiegfried_Rod_Height;
  G4double fSiegfried_Rod_Radius;
  G4double fSiegfried_Rod_CenterZ;

  G4double fSiegfried_Base_OuterRadius;
  G4double fSiegfried_Base_Height;
  G4double fSiegfried_Base_Thickness;
  G4double fSiegfried_Base_CenterZ;

  G4double fSiegfried_BaseRod_OuterRadius;
  G4double fSiegfried_BaseRod_Height;
  G4double fSiegfried_BaseRod_CenterZ;

  G4double fSiegfried_CoolingFinger_OuterRadius;
  G4double fSiegfried_CoolingFinger_Height;
  G4double fSiegfried_CoolingFinger_CenterZ;

  G4double fSiegfried_DewarNeck_OuterRadius;
  G4double fSiegfried_DewarNeck_InnerRadius;
  G4double fSiegfried_DewarNeck_Height;
  G4double fSiegfried_DewarNeck_CenterZ;

  G4double fSiegfried_DewarExpand_TopOuterRadius;
  G4double fSiegfried_DewarExpand_TopInnerRadius;
  G4double fSiegfried_DewarExpand_BottomOuterRadius;
  G4double fSiegfried_DewarExpand_BottomInnerRadius;
  G4double fSiegfried_DewarExpand_Height;
  G4double fSiegfried_DewarExpand_CenterZ;

  G4double fSiegfried_Dewar_OuterRadius;
  G4double fSiegfried_Dewar_InnerRadius;
  G4double fSiegfried_Dewar_Height;
  G4double fSiegfried_Dewar_CenterZ;

  G4double fSiegfried_LN2_OuterRadius;
  G4double fSiegfried_LN2_InnerRadius;
  G4double fSiegfried_LN2_Height;
  G4double fSiegfried_LN2_CenterZ;

  G4double fSiegfried_FanoutBox_lengthX;
  G4double fSiegfried_FanoutBox_lengthY;
  G4double fSiegfried_FanoutBox_lengthZ;
  G4double fSiegfried_FanoutBox_Thickness;
  G4double fSiegfried_FanoutBox_OffsetY;

  G4double fSiegfried_PreampRing_OuterRadius;
  G4double fSiegfried_PreampRing_InnerRadius;
  G4double fSiegfried_PreampRing_Height;
  G4double fSiegfried_PreampRing_CenterZ;

/*
  G4double fSiegfried_PreampBox_lengthX;
  G4double fSiegfried_PreampBox_lengthY;
  G4double fSiegfried_PreampBox_lengthZ;
  G4double fSiegfried_PreampBox_Thickness;
  int      iSiegfried_PreampBox_Totnum;
  G4double fSiegfried_PreampBox_CenterZ;
*/

//source system
  G4double fSiegfried_SourceSystem_Z;
  G4double fSiegfried_SourceSystem_R;
  G4double fSiegfried_SourceSystem_Phi;
  
  G4bool   fSiegfried_useCollimator;
  G4double fSiegfried_collimator_OuterRadius;
  G4double fSiegfried_collimator_InnerRadius;
  G4double fSiegfried_collimator_Height;
  
  G4double fSiegfried_collimatorHole_UpperHeight;
  G4double fSiegfried_collimatorHole_LowerHeight;
  G4double fSiegfried_collimatorDiaphragm_Height;
  G4double fSiegfried_collimatorDiaphragm_InnerRadius;

  G4double fSiegfried_SourceHolder_Height; 
  G4double fSiegfried_SourceHolder_InnerRadius;
  G4double fSiegfried_SourceHolder_OuterRadius;
  G4double fSiegfried_SourceHolder_DistanceToCanTopSurface;
  G4double fSiegfried_SourceHolder_rPosition;
  G4double fSiegfried_SourceHolder_phiPosition;
  G4double fSiegfried_SourceHolder_xPosition;
  G4double fSiegfried_SourceHolder_yPosition;
  G4double fSiegfried_SourceHolder_zPosition;
  
  G4double fSiegfried_SourceHolderAEA_rPosition;
  G4double fSiegfried_SourceHolderAEA_phiPosition;
  G4double fSiegfried_SourceHolderAEA_zPosition;

  G4bool   fSiegfried_useSourceHolderStick;
  G4double fSiegfried_SourceHolderStickLength;

  G4double fSiegfried_Source_Radius;
  G4double fSiegfried_Source_Height;

  //G4bool   fSiegfried_coincidenceflag;
  G4double fSiegfried_REGe_DistanceToCanOuterSurface;

  //preamp tubs
  G4double fSiegfried_PreFringe_Radius;
  G4double fSiegfried_PreFringe_Thickness;

  G4double fSiegfried_PreTub_Radius;
  G4double fSiegfried_PreTub_Height;
  G4double fSiegfried_PreTub_Thickness;

  G4double fSiegfried_PreTop_Thickness;
  G4double fSiegfried_PreTop_Radius;
  
  G4double fSiegfried_PreBottomHole_X;
  G4double fSiegfried_PreBottomHole_Y;
 
  G4double fSiegfried_PreTub_CenterY;
  G4double fSiegfried_PreBottom_CenterY;

// --------------------------------------------
// Collimator and Detector
// --------------------------------------------

  G4LogicalVolume* fCollimatorVacuumLogical;
  G4LogicalVolume* fCollimatorCrystalSegmentLogical; 
  G4LogicalVolume* fCollimatorCrystalDeadLayerLogical;
  G4LogicalVolume* fCollimatorCrystalSiliconLogical;
  G4LogicalVolume* fCollimatorActiveLogical;
  G4LogicalVolume* fCollimatorSliceLogical;
  G4LogicalVolume* fCollimatorFoilLogical;
  G4LogicalVolume* fCollimatorMotherVolumeLogical;
  G4LogicalVolume* fCollimatorSourceHolderLogical;
  G4LogicalVolume* fCollimatorSourceLogical;     
   
  
 
  G4double fCollimator_Crystal_CenterZ;
  G4double fCollimator_Crystal_InnerRadius;
  G4double fCollimator_Crystal_OuterRadius;
  G4double fCollimator_Crystal_Height;
  int      iCollimator_Segment_Nphi;
  int      iCollimator_Segment_Nz;
  G4double fCollimator_Segment_Height;
  G4double fCollimator_Segment_dPhi;
 
  G4double fCollimator_Crystal_DeadLayerOuterThickness;
  G4double fCollimator_Crystal_DeadLayerInnerThickness;
  G4double fCollimator_Crystal_DeadLayerTopThickness;
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
  G4double fCollimator_HalfLength;
  G4double fCollimator_Slice_Height;
  int      iCollimator_Slice_Nz; 
  G4double fCollimator_Foil_Height;

  G4double fCollimator_SourceHolder_Height;
  G4double fCollimator_SourceHolder_InnerRadius;
  G4double fCollimator_SourceHolder_OuterRadius;
  G4double fCollimator_SourceHolder_rPosition;
  G4double fCollimator_SourceHolder_phiPosition;
  G4double fCollimator_SourceHolder_xPosition;
  G4double fCollimator_SourceHolder_yPosition;
  G4double fCollimator_SourceHolder_zPosition ;
  G4double fCollimator_Source_Radius;
  G4double fCollimator_Source_Height;
  G4double fCollimator_Detector_Distance; 
  G4double fCollimator_System_Rotation;
  G4String fCollimatorMaterial;
  G4String fCollimatorFoilMaterial;
  G4String fCollimatorType;
  
  // ----------------------------------------------
  // Milchkanne && mikesch
  // ----------------------------------------------

  G4double fMikesch_CoreElectrode_Height;
  G4double fMikesch_CoreElectrode_InnerRadius;
  G4double fMikesch_CoreElectrode_OuterRadius;

  G4double fMikesch_InnerDeadLayer_Height;
  G4double fMikesch_InnerDeadLayer_InnerRadius; 
  G4double fMikesch_InnerDeadLayer_OuterRadius;

  G4double fMikesch_InnerDeadLayerSubtr_Height;
  G4double fMikesch_InnerDeadLayerSubtr_InnerRadius; 
  G4double fMikesch_InnerDeadLayerSubtr_OuterRadius;

  G4double fMikesch_Height;
  G4double fMikesch_InnerRadius;
  G4double fMikesch_OuterRadius;

  G4double fMikesch_SubtrHeight;
  G4double fMikesch_SubtrInnerRadius;
  G4double fMikesch_SubtrOuterRadius;

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
                       
  G4double fMikesch_Holder_Height; 
  G4double fMikesch_Holder_InnerRadius; 
  G4double fMikesch_Holder_OuterRadius;

  G4double fMikesch_HolderSubtr_Height;
  G4double fMikesch_HolderSubtr_InnerRadius; 
  G4double fMikesch_HolderSubtr_OuterRadius;

  G4double fMikesch_Teflon_Height; 
  G4double fMikesch_Teflon_InnerRadius; 
  G4double fMikesch_Teflon_OuterRadius;

  G4double fMikesch_TeflonSubtr_Height;
  G4double fMikesch_TeflonSubtr_InnerRadius; 
  G4double fMikesch_TeflonSubtr_OuterRadius;

  G4double fMikesch_CoolingStick_Height;
  G4double fMikesch_CoolingStickInside_Height;
  G4double fMikesch_CoolingStickOutside_Height;
  G4double fMikesch_CoolingStick_InnerRadius; 
  G4double fMikesch_CoolingStick_OuterRadius;

  G4double fMilchkanne_OuterWall_Height;
  G4double fMilchkanne_OuterWall_InnerRadius; 
  G4double fMilchkanne_OuterWall_OuterRadius;

  G4double fMilchkanne_OuterWallSubtr_Height;
  G4double fMilchkanne_OuterWallSubtr_InnerRadius; 
  G4double fMilchkanne_OuterWallSubtr_OuterRadius;

  G4double fMilchkanne_InnerWall_Height;
  G4double fMilchkanne_InnerWall_InnerRadius; 
  G4double fMilchkanne_InnerWall_OuterRadius;

  G4double fMilchkanne_InnerWallSubtr_Height;
  G4double fMilchkanne_InnerWallSubtr_InnerRadius; 
  G4double fMilchkanne_InnerWallSubtr_OuterRadius;

  G4double fMilchkanne_Lid_Height;
  G4double fMilchkanne_Lid_InnerRadius; 
  G4double fMilchkanne_Lid_OuterRadius;

  G4double fMilchkanne_LidHole_Height;
  G4double fMilchkanne_LidHole_InnerRadius; 
  G4double fMilchkanne_LidHole_OuterRadius;

  G4double fMilchkanne_LidHole_XPosition;
  G4double fMilchkanne_LidHole_YPosition;

  G4double fMilchkanne_SourceXPosition;
  
  G4LogicalVolume* MkDewarLogical;
  G4LogicalVolume* MkMikeschLogical; 
  G4LogicalVolume* MkHolderLogical;
  G4LogicalVolume* MkTeflonWrapLogical;
  G4LogicalVolume* MkCoolingStickInsideLogical;  
  G4LogicalVolume* MkCoolingStickOutsideLogical;  
  G4LogicalVolume* MkDeadLayerLogical;  
  G4LogicalVolume* MkCoreElectrodeLogical;
  G4LogicalVolume* MkLn2Logical;
  G4LogicalVolume* MkDewarBetweenWallsVacuumLogical;
 
  //---------------
  // messenger 
  //---------------

  GEMunichTestStandDBMessenger* fMessenger; 

}; 

// --------------------------------------------------------------------------- 

#endif 


