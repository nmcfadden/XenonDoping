

//---------------------------------------------------------------------------//
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
 * 03-17-2005, Xiang, corrected the crystal hole size
 *                    added messenger for adjusting distance
 *                     from source to lead brick
 *                default crystal size now 6x6 instead of 4x4
 * 05-05-2005, Xiang, added the Be window
 * 05-31-2005, Kevin, added DSG detector for lN2 teststand  
 * 11-21-2005, Kevin, modification to LN2 teststand 
 * 27-02-2006, Xiang, create REGe logical volume which contains 
 *                    crystal and support
 * 03-03-2006, Xiang, added parameters and logical volumes for Siegfried
 * 20-04-2006, Xiang, removed Siegfried preamp boxes, 
 *                    corrected the Siegfried IR Can z position bug
 * June-July,2006, Jing, For siegfried, add preamp boxes, collimator, 
 *                       modify can, IR can, AEA source geometry, 
 *                       redefine the source system.
 * Nov-Dec-2006, Manuela, added Detector-Collimator combination 
 * 01-15-2007, Manuela, added special Collimator to combination
 * 06-18-2007, Daniel,  Added mikesch and Milchkanne Teststand
 * 10-23-2009, Neslihan, fCollimator_HalfLength parameter is added to set the collimator length in the macro
 * 11-17-2010, Alex,  Moved from .hh the method for setting the position of the source
 *                    if the holder stick is present
 * 02-17-2011, Alex,  Changed the radius of the holder stick to 3.5 mm
 * 07-15-2011, Alex,  Changed standard Siegfried dead layer thickness
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh" 
#include "G4UnionSolid.hh" 
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include "munichteststand/GEMunichTestStandDB.hh"       
#include "munichteststand/GEMunichTestStandDBMessenger.hh" 
#include "gerdageometry/GEGeometrySD.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandDB::GEMunichTestStandDB()
{

//---------------------------------------------------
// parameters for REGe detectors
//---------------------------------------------------
  fCrystalHeight        = 4.95  * cm; 
  fCrystalInnerRadius   = 0.0   * cm; 
  fCrystalOuterRadius   = 2.525 * cm; 
  //fCrystalDeadLayer     = 0.00025 * mm; 
  fCrystalDeadLayer     = 0.1 * mm; 

  fCrystalHoleInnerRadius    = 0.0 * cm; 
  fCrystalHoleOuterRadius    = 0.5 * cm; 
  fCrystalHoleHeight         = 3.70 * cm; 

  fAluminumHolderThickness = 0.5 * mm; 
  fAluminumHolderToCrystal = 5.5 * mm;
  fAluminumHolderHeight    = fCrystalHeight + 
                             2.0 * fAluminumHolderToCrystal +
                             2.0 * fAluminumHolderThickness; 
  fAluminumHolderInnerRadius    = fCrystalOuterRadius +
                                  fAluminumHolderToCrystal -
                                  fAluminumHolderThickness ; 
  fAluminumHolderOuterRadius    = fCrystalOuterRadius +
                                  fAluminumHolderToCrystal;

  fCopperFingerHeight   = 8.0 * cm; 
  fCopperFinderRadius   = 1.0 * cm; 

  fREGeRadius = fAluminumHolderOuterRadius;
  fREGeHeight = fAluminumHolderHeight + fCopperFingerHeight;

  fSourceHeight         = 0.1 * cm; 
  fSourceRadius         = 0.1 * cm; 
  fSourceHolderHeight   = 0.2 * cm; 
  fSourceHolderRadius   = 0.5 * cm; 

  fLeadBrickLengthX     = 20.0 * cm; 
  fLeadBrickLengthY     = 10.0 * cm; 
  fLeadBrickLengthZ     = 10.0  * cm; 

  fCrystalDistanceToSource     =  1.0 * cm; 
  fSourceDistanceToBrick       =  3.0 * cm; 
  fDistance01                  = 20.0 * cm - fAluminumHolderOuterRadius; 

//---------------------------------------------------
// liquid N2 test stand 
//---------------------------------------------------

  fCrystalDSGUnsegmentedHeight                      =  8.0  * cm; 
  fCrystalDSGUnsegmentedInnerRadius                 =  0.5  * cm; 
  fCrystalDSGUnsegmentedOuterRadius                 =  4.0  * cm; 
  fCrystalDSGUnsegmentedDeadLayer                   =  0.1  * mm; 
  
  fCrystalHolderDSGUnsegmentedRing1OuterRadius      =  5.30  * cm; 
  fCrystalHolderDSGUnsegmentedRing1InnerRadius      =  4.05 * cm; 
  fCrystalHolderDSGUnsegmentedRing1Height           =  1.22 * cm; 
			      
  fCrystalHolderDSGUnsegmentedRing2OuterRadius      =  5.3  * cm; 
  fCrystalHolderDSGUnsegmentedRing2InnerRadius      =  2.0  * cm; 
  fCrystalHolderDSGUnsegmentedRing2Height           =  0.62 * cm; 

  fCrystalHolderDSGUnsegmentedGap                   =  0.1  * cm; 

  fCrystalHolderDSGUnsegmentedSkrewOuterRadius      =  0.31 * cm; 
  fCrystalHolderDSGUnsegmentedSkrewInnerRadius      =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedSkrewHeight           = 11.0  * cm; 

  fCrystalHolderDSGUnsegmentedSkrewHole1OuterRadius =  fCrystalHolderDSGUnsegmentedSkrewOuterRadius;  
  fCrystalHolderDSGUnsegmentedSkrewHole1InnerRadius =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedSkrewHole1Height      =  fCrystalHolderDSGUnsegmentedRing1Height; 

  fCrystalHolderDSGUnsegmentedSkrewHole2OuterRadius =  fCrystalHolderDSGUnsegmentedSkrewOuterRadius;  
  fCrystalHolderDSGUnsegmentedSkrewHole2InnerRadius =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedSkrewHole2Height      =  fCrystalHolderDSGUnsegmentedRing2Height; 

  fCrystalHolderDSGUnsegmentedBarLengthX            =  10.6 * cm; 
  fCrystalHolderDSGUnsegmentedBarLengthY            =  1.0  * cm; 
  fCrystalHolderDSGUnsegmentedBarLengthZ            =  0.5  * cm; 

  fCrystalHolderDSGUnsegmentedStringOuterRadius     =  0.1  * cm; 
  fCrystalHolderDSGUnsegmentedStringInnerRadius     =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedStringHeight          = 40.0  * cm; 

  fCrystalHolderDSGUnsegmentedPlateSmallOuterRadius =  5.3  * cm; 
  fCrystalHolderDSGUnsegmentedPlateSmallInnerRadius =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedPlateSmallHeight      =  0.3  * cm; 
  
  fCrystalHolderDSGUnsegmentedPlateLargeOuterRadius = 12.0  * cm; 
  fCrystalHolderDSGUnsegmentedPlateLargeInnerRadius =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedPlateLargeHeight      =  0.2  * cm; 

  fCrystalHolderDSGUnsegmentedRodOuterRadius        =  1.5  * cm; 
  fCrystalHolderDSGUnsegmentedRodInnerRadius        =  0.0  * cm; 
  fCrystalHolderDSGUnsegmentedRodHeight             = 20.0  * cm; 

  fCrystalHolderDSGUnsegmentedClampOuterRadius      =  5.50 * cm; 
  fCrystalHolderDSGUnsegmentedClampInnerRadius      =  5.30 * cm; 
  fCrystalHolderDSGUnsegmentedClampHeight           =  5.5  * cm; 

  fSourceHeightAEA            =  2.0 * mm; 
  fSourceRadiusAEA            =  2.0 * mm; 
  fSourceHolderHeightTotalAEA = 17.6 * mm; 
  fSourceHolderHeightNeckAEA  =  7.0 * mm; 
  fSourceHolderRadiusAEA      =  3.2 * mm;  
  fSourceHolderRadiusNeckAEA  =  2.0 * mm; 
  fSourceHoleNumber           =  0; 

  fStringRadius         = 0.75 * mm; 
  fStringDetectorHeight = 20 * cm; 
  fStringSourceHeight   = 23 * cm; 

  fDewarHeight          = 65.5 * cm; 
  fDewarInnerRadius     = 34.4 / 2.0 * cm; 
  fDewarOuterRadius     = 33.4 / 2.0 * cm; 

  fLN2Height            = 55.0 * cm; 

//---------------------------------------------------
// Siegfried
//---------------------------------------------------
  fSiegfried_Crystal_InnerRadius     = 10.0 * mm * 0.5;
  fSiegfried_Crystal_OuterRadius     = 75.0 * mm * 0.5; // radius includes crystal and deadlayer
  fSiegfried_Crystal_Height          = 70.0 * mm; // Height includes crystal, deadlayer and silicon
  fSiegfried_Crystal_DeadLayerOuterThickness     = 0.5 * micrometer;
  fSiegfried_Crystal_DeadLayerInnerThickness     = 0.5 * mm;
  fSiegfried_Crystal_DeadLayerTopThickness       = 0.1 * cm;   // bottom is the same
  fSiegfried_Crystal_SiliconThickness    = 0.001 * mm;

  fSiegfried_Can_OuterRadius = 143.0 * 0.5 * mm;
  fSiegfried_Can_Height      = 162.0 * mm;
  fSiegfried_Can_Thickness   =   2.0 * mm;
  fSiegfried_Can_CenterZ     = 0.5*fSiegfried_Crystal_Height + 49.0 * mm 
    - 0.5*fSiegfried_Can_Height; //respect to Crystal

  fSiegfried_IRCan_OuterRadius = 44.0 * mm;
  fSiegfried_IRCan_Height      = 87.0 * mm;
  fSiegfried_IRCan_Thickness   =  2.0 * mm;
  fSiegfried_IRCan_CenterZ     = 0.5*fSiegfried_Can_Height
    - 49*mm + 11*mm - 0.5*fSiegfried_IRCan_Height; //respect to Can

  fSiegfried_IRBase_OuterRadius = fSiegfried_IRCan_OuterRadius;
  fSiegfried_IRBase_InnerRadius = 18.0 * mm;
  fSiegfried_IRBase_Height      = 10.0 * mm;
  fSiegfried_IRBase_CenterZ     = 0.5*fSiegfried_Can_Height
    - 49*mm - fSiegfried_Crystal_Height-6*mm-0.5*fSiegfried_IRBase_Height; //respect to Can

  fSiegfried_Rod_Height = fSiegfried_Can_Height - 49*mm - fSiegfried_Crystal_Height 
    - 6*mm - fSiegfried_IRBase_Height - fSiegfried_Can_Thickness;
  fSiegfried_Rod_Radius = 20.0 * mm;
  fSiegfried_Rod_CenterZ = -0.5*fSiegfried_Can_Height + fSiegfried_Can_Thickness
    + fSiegfried_Rod_Height*0.5; //respect to Can

  fSiegfried_Base_OuterRadius = 169.0 * 0.5 * mm;
  fSiegfried_Base_Height      =  80.0 * mm;
  fSiegfried_Base_Thickness   =   1.0 * cm;
  fSiegfried_Base_CenterZ     = fSiegfried_Can_CenterZ - fSiegfried_Can_Height*0.5
                              - fSiegfried_Base_Height*0.5;

  fSiegfried_BaseRod_OuterRadius = 3.2 * 0.5 * cm;
  fSiegfried_BaseRod_Height      = fSiegfried_Base_Height - fSiegfried_Base_Thickness*2.0; 
  fSiegfried_BaseRod_CenterZ     = fSiegfried_Base_CenterZ;

  fSiegfried_CoolingFinger_OuterRadius = 1.4 * 0.5 * cm;
  fSiegfried_CoolingFinger_Height      = 53.0 * cm;
  fSiegfried_CoolingFinger_CenterZ     = fSiegfried_Base_CenterZ - fSiegfried_Base_Height*0.5
                                       - fSiegfried_CoolingFinger_Height*0.5;

  fSiegfried_DewarNeck_OuterRadius     = 50.0 * mm;
  fSiegfried_DewarNeck_InnerRadius     = 40.0 * mm;
  fSiegfried_DewarNeck_Height          = 45.0 * mm;
  fSiegfried_DewarNeck_CenterZ         = fSiegfried_Base_CenterZ - fSiegfried_Base_Height*0.5
                                       - fSiegfried_DewarNeck_Height*0.5;

  fSiegfried_DewarExpand_TopOuterRadius = 50.0 * mm;
  fSiegfried_DewarExpand_TopInnerRadius = 40.0 * mm;
  fSiegfried_DewarExpand_BottomOuterRadius = 225.0 * mm;
  fSiegfried_DewarExpand_BottomInnerRadius = 215.0 * mm;
  fSiegfried_DewarExpand_Height         = 45.0 * mm;
  fSiegfried_DewarExpand_CenterZ        = fSiegfried_DewarNeck_CenterZ - fSiegfried_DewarNeck_Height*0.5
                                        - fSiegfried_DewarExpand_Height*0.5;

  fSiegfried_Dewar_OuterRadius  = fSiegfried_DewarExpand_BottomOuterRadius;
  fSiegfried_Dewar_InnerRadius  = fSiegfried_DewarExpand_BottomInnerRadius;
  fSiegfried_Dewar_Height       = 540.0 * mm - fSiegfried_DewarNeck_Height - fSiegfried_DewarExpand_Height;
  fSiegfried_Dewar_CenterZ      = fSiegfried_DewarExpand_CenterZ - fSiegfried_DewarExpand_Height*0.5
                                - fSiegfried_Dewar_Height*0.5;

  fSiegfried_LN2_OuterRadius    = fSiegfried_Dewar_InnerRadius;
  fSiegfried_LN2_InnerRadius    = fSiegfried_CoolingFinger_OuterRadius;
  fSiegfried_LN2_Height         = fSiegfried_Dewar_Height;
  fSiegfried_LN2_CenterZ        = fSiegfried_Dewar_CenterZ;

  fSiegfried_FanoutBox_lengthX  = 30.0 * mm;
  fSiegfried_FanoutBox_lengthY  = 38.0 * mm;
  fSiegfried_FanoutBox_Thickness= 10.0 * mm;
  fSiegfried_FanoutBox_lengthZ  =  0.5 * fSiegfried_FanoutBox_Thickness;
  fSiegfried_FanoutBox_OffsetY  =  fSiegfried_FanoutBox_lengthZ + fSiegfried_Base_OuterRadius;

  fSiegfried_PreampRing_OuterRadius = 280.0 * mm;
  fSiegfried_PreampRing_InnerRadius = 277.0 * mm;
  fSiegfried_PreampRing_Height      =  20.0 * mm;
  fSiegfried_PreampRing_CenterZ     = fSiegfried_DewarExpand_CenterZ;
/*
  fSiegfried_PreampBox_lengthZ  = 121.0 * mm;
  fSiegfried_PreampBox_lengthY  =  80.3 * mm;
  fSiegfried_PreampBox_lengthX  =  54.5 * mm;
  fSiegfried_PreampBox_Thickness=   3.0 * mm;
  iSiegfried_PreampBox_Totnum         = 19;
  fSiegfried_PreampBox_CenterZ        = fSiegfried_PreampRing_CenterZ;
*/

  //source system:
  fSiegfried_SourceSystem_Z   = 200.0 * mm;
  fSiegfried_SourceSystem_R   =   0.0 * mm;
  fSiegfried_SourceSystem_Phi =   0.0 * deg;

  //collimator
  fSiegfried_useCollimator          = false;
  fSiegfried_collimator_OuterRadius = 24.92 * mm * 0.5;
  fSiegfried_collimator_InnerRadius = 11.11 * mm * 0.5;
  fSiegfried_collimator_Height      = 51.96 * mm;

  fSiegfried_collimatorHole_UpperHeight = 10.02 * mm;
  fSiegfried_collimatorHole_LowerHeight = 40.01 * mm;

  fSiegfried_collimatorDiaphragm_InnerRadius = 1.0 * mm * 0.5;
  fSiegfried_collimatorDiaphragm_Height = fSiegfried_collimator_Height 
    - fSiegfried_collimatorHole_UpperHeight - fSiegfried_collimatorHole_LowerHeight;

  //holder
  fSiegfried_SourceHolder_Height      = 5.0 * cm;
  fSiegfried_SourceHolder_InnerRadius = 0.5 * cm;
  fSiegfried_SourceHolder_OuterRadius = 2.0 * cm;
  fSiegfried_SourceHolder_DistanceToCanTopSurface = 5.0 * cm;
  //fSiegfried_SourceHolder_xPosition   = 2.0 * cm;
  //fSiegfried_SourceHolder_yPosition   = 2.0 * cm;
  fSiegfried_SourceHolder_rPosition     = 2.0 * cm;
  fSiegfried_SourceHolder_phiPosition   = 0.0 * deg;
  fSiegfried_SourceHolder_xPosition   = fSiegfried_SourceHolder_rPosition *
                                    cos(fSiegfried_SourceHolder_phiPosition);
  fSiegfried_SourceHolder_yPosition   = fSiegfried_SourceHolder_rPosition *
                                    sin(fSiegfried_SourceHolder_phiPosition);
  fSiegfried_SourceHolder_zPosition   = fSiegfried_Can_CenterZ
                            + 0.5 * fSiegfried_Can_Height 
                            + fSiegfried_SourceHolder_DistanceToCanTopSurface
                            + 0.5 * fSiegfried_SourceHolder_Height;

  fSiegfried_Source_Radius            = fSiegfried_SourceHolder_InnerRadius * 0.5;
  fSiegfried_Source_Height            = fSiegfried_SourceHolder_InnerRadius * 0.5;

  // holder stick
  fSiegfried_useSourceHolderStick     = false;
  fSiegfried_SourceHolderStickLength  = 10 * cm;

  //for AEA source------------------------------------------------------------------
  fSiegfried_SourceHolderAEA_rPosition   = 2.0 * cm;
  fSiegfried_SourceHolderAEA_phiPosition = 0.0 * deg;
  fSiegfried_SourceHolderAEA_zPosition   = fSiegfried_Can_CenterZ 
    + 0.5 * fSiegfried_Can_Height 
    + fSiegfried_SourceHolder_DistanceToCanTopSurface
    + 0.5 * fSourceHolderHeightTotalAEA;
  //-------------------------------------------------------------------------------

  fSiegfried_REGe_DistanceToCanOuterSurface = 20.0 * cm;

  //preamp tubs
  fSiegfried_PreFringe_Radius    =  90.0 * mm;
  fSiegfried_PreFringe_Thickness =   4.0 * mm;

  fSiegfried_PreTub_Radius       =  80.0 * mm;
  fSiegfried_PreTub_Height       = 119.0 * mm;
  fSiegfried_PreTub_Thickness    =   2.0 * mm;

  fSiegfried_PreTop_Thickness    =   4.0 * mm;
  fSiegfried_PreTop_Radius       = 110.0 * mm * 0.5;
  
  fSiegfried_PreBottomHole_X     =   25.0 * mm;
  fSiegfried_PreBottomHole_Y     =   20.0 * mm;
  
  fSiegfried_PreTub_CenterY      = fSiegfried_Base_OuterRadius 
    + 0.5 * fSiegfried_PreTub_Height + fSiegfried_PreFringe_Thickness;
  fSiegfried_PreBottom_CenterY   = fSiegfried_Base_OuterRadius 
    + 0.5 * fSiegfried_PreFringe_Thickness;


// ---------------------------------------------------------------------------
// Collimator and Detector
// ---------------------------------------------------------------------------
 
  fCollimator_Crystal_CenterZ = 0.0*cm;

  fCollimator_Crystal_InnerRadius     = 10.0 * mm * 0.5;
  fCollimator_Crystal_OuterRadius     = 75.0 * mm * 0.5; // radius includes crystal and deadlayer
  fCollimator_Crystal_Height          = 70.0 * mm; // Height includes crystal, deadlayer and silicon

  iCollimator_Segment_Nphi = 6;
  iCollimator_Segment_Nz   = 3;
  fCollimator_Segment_dPhi = 360.0 * deg / float(iCollimator_Segment_Nphi);

  fCollimator_Crystal_DeadLayerOuterThickness     = 0.1 * cm;
  fCollimator_Crystal_DeadLayerInnerThickness     = 0.1 * cm;
  fCollimator_Crystal_DeadLayerTopThickness       = 0.1 * cm;   // bottom is the same
  fCollimator_Crystal_SiliconThickness    = 0.001 * mm;


  fCollimator_Segment_Height = (fCollimator_Crystal_Height - 2 * fCollimator_Crystal_DeadLayerTopThickness
                            - 2 * fCollimator_Crystal_SiliconThickness) / float(iCollimator_Segment_Nz);

  //collimator system
  fCollimatorSystem_Z   =   0.0 * mm;
  fCollimatorSystem_R   =   20.0 * mm;
  fCollimatorSystem_Phi =    90.0 * deg;

  fCollimator_MotherVolume_R = 61.0 * mm * 0.5;
  fCollimator_MotherVolume_Height = 70.0 * mm;

  fCollimatorMaterial = "Tungsten";

  fCollimator_OuterRadius = 30.00 * mm * 0.5;
  fCollimator_InnerRadius =   2.0 * mm * 0.5;
  fCollimator_LowerHeight = 50.0 * mm;
  
  fCollimator_HalfLength = 25.0 * mm; // new parameter to set the length of the collimator
 
  fCollimator_SourceHolder_Height      = 17.6 * mm;

  fCollimator_Height      = fCollimator_LowerHeight + fCollimator_SourceHolder_Height + 0.02 * mm;
  fCollimator_Slice_Height = 4.0 * mm;
  iCollimator_Slice_Nz = 6; 

  fCollimator_Foil_Height = 0.001*mm; 
  fCollimatorFoilMaterial = "gold";
 
  fCollimator_SourceHolder_OuterRadius = 6.6 * mm * 0.5;
  fCollimator_SourceHolder_InnerRadius = 4.2 * mm * 0.5;
  fCollimator_SourceHolder_rPosition     = 0.0 * cm;
  fCollimator_SourceHolder_phiPosition   = 0.0 * deg;
  fCollimator_SourceHolder_xPosition   = fCollimator_SourceHolder_rPosition *
                                    cos(fCollimator_SourceHolder_phiPosition);
  fCollimator_SourceHolder_yPosition   = fCollimator_SourceHolder_rPosition *
                                    sin(fCollimator_SourceHolder_phiPosition);
  fCollimator_SourceHolder_zPosition   = fCollimator_Crystal_CenterZ 
                            + 0.5 * fCollimator_Crystal_Height 
                            + 0.5 * fCollimator_SourceHolder_Height;

  fCollimator_Source_Radius            = fCollimator_SourceHolder_InnerRadius-0.2*mm;
  fCollimator_Source_Height            = 4.0 * mm;

  fCollimator_Detector_Distance = 25 * mm;



// ---------------------------------------------------------------------------
// Milchkanne and Mikesch
// ---------------------------------------------------------------------------
  fMikesch_CoreElectrode_Height      = 61.* 0.5 * mm;
  fMikesch_CoreElectrode_InnerRadius = 0. * mm;
  fMikesch_CoreElectrode_OuterRadius = 10 * 0.5 * mm;
  
  fMikesch_InnerDeadLayer_Height      =  fMikesch_CoreElectrode_Height + 600 * micrometer ;
  fMikesch_InnerDeadLayer_InnerRadius = 0.; 
  fMikesch_InnerDeadLayer_OuterRadius = fMikesch_CoreElectrode_OuterRadius + 600 * micrometer ;

  fMikesch_InnerDeadLayerSubtr_Height      =  fMikesch_CoreElectrode_Height;
  fMikesch_InnerDeadLayerSubtr_InnerRadius = fMikesch_CoreElectrode_InnerRadius ; 
  fMikesch_InnerDeadLayerSubtr_OuterRadius = fMikesch_CoreElectrode_OuterRadius;

  fMikesch_Height      = 77.2 * 0.5 * mm;
  fMikesch_InnerRadius = 0.;
  fMikesch_OuterRadius = 64.5 * 0.5 * mm;

  fMikesch_SubtrHeight      = fMikesch_InnerDeadLayer_Height;
  fMikesch_SubtrInnerRadius = 0.;
  fMikesch_SubtrOuterRadius = fMikesch_InnerDeadLayer_OuterRadius;

  fMikesch_Holder_Height      = 77.2 * 0.5 *mm;
  fMikesch_Holder_InnerRadius = 0.; 
  fMikesch_Holder_OuterRadius = 70 * 0.5 * mm ;

  fMikesch_HolderSubtr_Height      = 77.2 * 0.5 *mm;
  fMikesch_HolderSubtr_InnerRadius = 0.; 
  fMikesch_HolderSubtr_OuterRadius = fMikesch_Holder_OuterRadius - 0.8 * mm;

  fMikesch_Teflon_Height       = 8 * cm * 0.5; 
  fMikesch_Teflon_InnerRadius  = 0.; 
  fMikesch_Teflon_OuterRadius  = fMikesch_Holder_OuterRadius + 0.55 * mm;

  fMikesch_TeflonSubtr_Height        = 8 * cm * 0.5;
  fMikesch_TeflonSubtr_InnerRadius   = 0.; 
  fMikesch_TeflonSubtr_OuterRadius   = fMikesch_Holder_OuterRadius;

  fMilchkanne_OuterWall_Height      = 410 * 0.5 * mm;
  fMilchkanne_OuterWall_InnerRadius = 0.; 
  fMilchkanne_OuterWall_OuterRadius = 203 * 0.5 * mm;
  
  fMilchkanne_OuterWallSubtr_Height      = fMilchkanne_OuterWall_Height ;
  fMilchkanne_OuterWallSubtr_InnerRadius = fMilchkanne_OuterWall_InnerRadius ; 
  fMilchkanne_OuterWallSubtr_OuterRadius = fMilchkanne_OuterWall_OuterRadius - 1.2 * mm; //wall would be 1.2 mm thick

  fMilchkanne_InnerWall_Height      = fMilchkanne_OuterWall_Height;
  fMilchkanne_InnerWall_InnerRadius = 0.; 
  fMilchkanne_InnerWall_OuterRadius = fMilchkanne_OuterWallSubtr_OuterRadius - 23.8 * mm; //23.8 mm distance between inner and outer wall

  fMilchkanne_InnerWallSubtr_Height      = fMilchkanne_InnerWall_Height;
  fMilchkanne_InnerWallSubtr_InnerRadius = fMilchkanne_InnerWall_InnerRadius; 
  fMilchkanne_InnerWallSubtr_OuterRadius = fMilchkanne_InnerWall_OuterRadius - 0.3 * mm; //wall would be 0.3 mm thick

  fMilchkanne_Lid_Height      = 20 * 0.5 * mm ; //checked!
  fMilchkanne_Lid_InnerRadius = 0.; 
  fMilchkanne_Lid_OuterRadius = 198.0 * 0.5 * mm;

  fMilchkanne_LidHole_Height      = fMilchkanne_Lid_Height;
  fMilchkanne_LidHole_InnerRadius = 0.; 
  fMilchkanne_LidHole_OuterRadius =  85 * 0.5 * mm; //checked!

  fMilchkanne_LidHole_XPosition = 1.25 * cm; //checked!
  fMilchkanne_LidHole_YPosition = 1.25 * cm; //checked!

  fMikesch_CoolingStick_Height      = 600 * 0.5 * mm;

  fMikesch_CoolingStickInside_Height      = (fMilchkanne_InnerWallSubtr_Height - fMikesch_Height) * 0.5;
  fMikesch_CoolingStickOutside_Height     = fMikesch_CoolingStick_Height - fMikesch_CoolingStickInside_Height;

  fMikesch_CoolingStick_InnerRadius = 0.; 
  fMikesch_CoolingStick_OuterRadius = 32 * 0.5 * mm;


  fMessenger = new GEMunichTestStandDBMessenger(this); 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandDB::~GEMunichTestStandDB()
{
/*
  delete fCrystalActiveLogical; 
  delete fCrystalDeadLayerLogical; 
  delete fAluminumHolderLogical; 
  delete fCopperFingerLogical; 
  delete fREGeLogical;
  delete fLeadBrickLogical; 
  delete fSourceLogical; 
  delete fSourceHolderLogical; 

  // liquid N2 test stand 

  delete fCrystalDSGUnsegmentedActiveLogical; 
  delete fCrystalDSGUnsegmentedDeadLayerLogical; 
  delete fCrystalHolderDSGUnsegmentedRing1Logical; 
  delete fCrystalHolderDSGUnsegmentedRing2Logical; 
  delete fCrystalHolderDSGUnsegmentedSkrewLogical; 
  delete fCrystalHolderDSGUnsegmentedBarLogical; 
  delete fCrystalHolderDSGUnsegmentedStringLogical; 
  delete fCrystalHolderDSGUnsegmentedPlateSmallLogical; 
  delete fCrystalHolderDSGUnsegmentedPlateLargeLogical; 
  delete fCrystalHolderDSGUnsegmentedRodLogical; 
  delete fCrystalHolderDSGUnsegmentedClampLogical; 
  delete fSourceAEALogical; 
  delete fSourceHolderAEALogical; 
  delete fStringDetectorLogical; 
  delete fStringSourceLogical; 
  delete fDewarLogical; 
  delete fLN2Logical; 
*/
}

// --------------------------------------------------------------------------- 
void GEMunichTestStandDB::SetSiegfriedSourceHolderCenterzToCanTop(G4double zpos)
{
  fSiegfried_SourceHolder_DistanceToCanTopSurface = zpos;
  fSiegfried_SourceHolder_zPosition   = fSiegfried_Can_CenterZ 
    + 0.5 * fSiegfried_Can_Height 
    + fSiegfried_SourceHolder_DistanceToCanTopSurface 
    + fSiegfried_collimator_Height;
  
  fSiegfried_SourceHolderAEA_zPosition   = fSiegfried_Can_CenterZ 
    + 0.5 * fSiegfried_Can_Height 
    + fSiegfried_SourceHolder_DistanceToCanTopSurface
    + 0.5 * fSourceHolderHeightTotalAEA;
}

void GEMunichTestStandDB::SetSiegfriedSourceSystemToCanSide(G4double rpos)
{
  fSiegfried_SourceSystem_R = fSiegfried_Can_OuterRadius
                            + rpos
                            + fSiegfried_collimator_Height;
  if (fSiegfried_useSourceHolderStick)
    fSiegfried_SourceSystem_R += fSiegfried_SourceHolderStickLength;
}

// --------------------------------------------------------------------------- 

void GEMunichTestStandDB::ConstructTestStandDB()
{

  // materials 
  G4Material* vacuum   = G4Material::GetMaterial("Vacuum");
  G4Material* enrGe    = G4Material::GetMaterial("EnrichedGe");
  G4Material* natGe    = G4Material::GetMaterial("NaturalGe");
  G4Material* aluminum = G4Material::GetMaterial("MetalAluminium"); 
  G4Material* copper   = G4Material::GetMaterial("MetalCopper"); 
  G4Material* lead     = G4Material::GetMaterial("MetalLead");
  G4Material* tungsten = G4Material::GetMaterial("MetalTungsten"); 
  G4Material* plastic  = G4Material::GetMaterial("PlasticScintillator"); 
  G4Material* liquidN2 = G4Material::GetMaterial("LiquidNitrogen"); 
  G4Material* steel    = G4Material::GetMaterial("Steel"); 
  G4Material* beryllium= G4Material::GetMaterial("MetalBeryllium");
  G4Material* iron     = G4Material::GetMaterial("MetalIron"); 
  G4Material* si       = G4Material::GetMaterial("MetalSilicon");
  G4Material* air      = G4Material::GetMaterial("Air");
  /*G4Material* gold     =*/ G4Material::GetMaterial("MetalGold");
  G4Material* silver   = G4Material::GetMaterial("MetalSilver");
  G4Material* teflon   = G4Material::GetMaterial("Teflon");
  G4Material* PVC      = G4Material::GetMaterial("PVC");

  // colors 

  G4Colour white    (1.0,  1.0,  1.0,  0.5); 
  G4Colour red      (1.0,  0.0,  0.0,  0.5); 
  G4Colour darkred  (0.5,  0.0,  0.0,  0.5); 
  G4Colour orange   (0.75, 0.55, 0.0,  0.5);
  G4Colour lightgrey(0.9,  0.9,  0.9,  0.5); 
  G4Colour darkgrey (0.4,  0.4,  0.4,  0.5); 
  G4Colour green    (0.0,  1.0,  0.0,  0.5); 
  G4Colour blue     (0.0,  0.0,  1.0,  0.5); 
  G4Colour lightblue(0.0,  0.0,  0.5,  0.5); 

  // construct logical volumes 

  G4double TubsInnerRadius; 
  G4double TubsOuterRadius; 
  G4double TubsHeight; 
  G4double TubsStartAngle; 
  G4double TubsSpanningAngle; 

  G4double BoxLengthX; 
  G4double BoxLengthY; 
  G4double BoxLengthZ; 

  // ------------------ //
  // simple test stand  // 
  // ------------------ // 

  // volume : crystal (active) 

  TubsInnerRadius   = fCrystalInnerRadius;  
  TubsOuterRadius   = fCrystalOuterRadius - fCrystalDeadLayer; 
  TubsHeight        = 0.5 * fCrystalHeight - fCrystalDeadLayer; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
  
  G4Tubs* CrystalActiveTubs = new G4Tubs("CrystalActiveTubs",
					 TubsInnerRadius,
					 TubsOuterRadius,
					 TubsHeight,
					 TubsStartAngle,
					 TubsSpanningAngle); 

  TubsInnerRadius   = fCrystalHoleInnerRadius;  
  TubsOuterRadius   = fCrystalHoleOuterRadius;
  TubsHeight        = 0.5 * fCrystalHoleHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* CrystalHoleTubs = new G4Tubs("CrystalHoleTubs",
				       TubsInnerRadius,
				       TubsOuterRadius,
				       TubsHeight,
				       TubsStartAngle,
				       TubsSpanningAngle); 
 
  G4double ztemp = 0.0-0.5*(fCrystalHeight - fCrystalHoleHeight); 
  G4SubtractionSolid* CrystalSubstractionSolid =
        new G4SubtractionSolid("CrystalSubstractionSolid",
                               CrystalActiveTubs,
                               CrystalHoleTubs,
                               0,
                               G4ThreeVector(0, 0, ztemp));

  fCrystalActiveLogical = new G4LogicalVolume(CrystalSubstractionSolid,
					      enrGe,
					      "CrystalActiveLogical");

  G4VisAttributes* CrystalActiveVisAtt = new G4VisAttributes(red);
  
  CrystalActiveVisAtt -> SetVisibility(true);
  CrystalActiveVisAtt -> SetForceSolid(true);
  
  fCrystalActiveLogical -> SetVisAttributes(CrystalActiveVisAtt); 

  // fCrystalActiveLogical is used to build REGe detector

  // volume : crystal (dead layer) 

  TubsInnerRadius   = fCrystalOuterRadius - fCrystalDeadLayer;  
  TubsOuterRadius   = fCrystalOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
  
  G4Tubs* CrystalDeadLayerTubs = new G4Tubs("CrystalDeadLayerTubs",
					    TubsInnerRadius,
					    TubsOuterRadius,
					    TubsHeight,
					    TubsStartAngle,
					    TubsSpanningAngle);
  
  fCrystalDeadLayerLogical = new G4LogicalVolume(CrystalDeadLayerTubs,
					enrGe,
					"CrystalDeadLayerLogical");

  G4VisAttributes* CrystalDeadLayerVisAtt = new G4VisAttributes(darkred);
  
  CrystalDeadLayerVisAtt -> SetVisibility(true);
  CrystalDeadLayerVisAtt -> SetForceSolid(true);

  fCrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt); 


  // volume : aluminum holder 

  // outer part
  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fAluminumHolderOuterRadius; 
  TubsHeight        = 0.5 * fAluminumHolderHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* HolderTubs = new G4Tubs("HolderTubs",
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);
  
  // inner part
  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fAluminumHolderInnerRadius; 
  TubsHeight        = 0.5 * fAluminumHolderHeight - fAluminumHolderThickness; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* HolderHoleTubs = new G4Tubs("HolderHoleTubs",
				      TubsInnerRadius,
				      TubsOuterRadius,
				      TubsHeight,
				      TubsStartAngle,
				      TubsSpanningAngle);
  
  G4SubtractionSolid* HolderSubtractionSolid = new G4SubtractionSolid("HolderSubtractionSolid",
								      HolderTubs,
								      HolderHoleTubs,
								      0,
								      G4ThreeVector(0,0,0)); 
  // beryllium window part
  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fCrystalOuterRadius;
  TubsHeight        = fAluminumHolderThickness;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;
  G4Tubs* HolderBeWindowTubs = new G4Tubs("HolderBeWindowTubs",
                                      TubsInnerRadius,
                                      TubsOuterRadius,
                                      TubsHeight,
                                      TubsStartAngle,
                                      TubsSpanningAngle);

  ztemp=0.5*(fAluminumHolderHeight-fAluminumHolderThickness);
  G4SubtractionSolid* HolderSubSolid = new G4SubtractionSolid("HolderSubSolid",
                                                               HolderSubtractionSolid,
                                                               HolderBeWindowTubs,
                                                               0,
                                                               G4ThreeVector(0,0,ztemp));

  
  fAluminumHolderLogical = new G4LogicalVolume(HolderSubSolid,
					       aluminum,
					       "AluminumHolderLogical");

  G4VisAttributes* AluminumHolderVisAtt = new G4VisAttributes(lightgrey); 

  AluminumHolderVisAtt -> SetVisibility(true); 
  AluminumHolderVisAtt -> SetForceSolid(true); 

  fAluminumHolderLogical -> SetVisAttributes(AluminumHolderVisAtt); 

  // volume : beryllium window
  
  fBerylliumWindowLogical = new G4LogicalVolume(HolderBeWindowTubs,
                                                beryllium,
                                                "BerylliumWindowLogical");

  G4VisAttributes* BerylliumWindowVisAtt = new G4VisAttributes(lightblue);
  BerylliumWindowVisAtt -> SetVisibility(true);
  BerylliumWindowVisAtt -> SetForceSolid(true);
  fBerylliumWindowLogical -> SetVisAttributes(BerylliumWindowVisAtt);

  // volume : copper finger 

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fCopperFinderRadius; 
  TubsHeight        = 0.5 * fCopperFingerHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;
  
  G4Tubs* FingerTubs = new G4Tubs("FingerTubs",
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);
  
  fCopperFingerLogical = new G4LogicalVolume(FingerTubs,
					     copper, 
					     "CopperFingerLogical");

  G4VisAttributes* CuVisAtt = new G4VisAttributes(orange);

  CuVisAtt -> SetVisibility(true);
  CuVisAtt -> SetForceSolid(true);

  fCopperFingerLogical -> SetVisAttributes(CuVisAtt);

  // ------------------ //
  // REGe detector (moved to another function)
  // ------------------ // 

  // volume : lead brick 

  BoxLengthX = 0.5 * fLeadBrickLengthX; 
  BoxLengthY = 0.5 * fLeadBrickLengthY; 
  BoxLengthZ = 0.5 * fLeadBrickLengthZ; 

  G4Box* LeadBrickBox = new G4Box("LeadBrickBox", 
				  BoxLengthX , 
				  BoxLengthY, 
				  BoxLengthZ); 

  fLeadBrickLogical = new G4LogicalVolume(LeadBrickBox,
					  lead,
					  "LeadBrickLogical");

  G4VisAttributes* LeadVisAtt = new G4VisAttributes(darkgrey); 
  
  LeadVisAtt -> SetVisibility(true);
  LeadVisAtt -> SetForceSolid(true);

  fLeadBrickLogical -> SetVisAttributes(LeadVisAtt); 

  // volume : source 

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fSourceRadius; 
  TubsHeight        = 0.5 * fSourceHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;
  
  G4Tubs* SourceTubs = new G4Tubs("Source",
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);
  
  fSourceLogical = new G4LogicalVolume(SourceTubs,
				       plastic,
				       "SourceLogical"); 

  G4VisAttributes* SourceVisAtt = new G4VisAttributes(green); 
  
  SourceVisAtt -> SetVisibility(true);
  SourceVisAtt -> SetForceSolid(true);

  fSourceLogical -> SetVisAttributes(SourceVisAtt); 


  // volume : source holder 

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fSourceHolderRadius; 
  TubsHeight        = 0.5 * fSourceHolderHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;
  
  G4Tubs* SourceHolderTubs = new G4Tubs("SourceHolder",
					TubsInnerRadius,
					TubsOuterRadius,
					TubsHeight,
					TubsStartAngle,
					TubsSpanningAngle);
  
  fSourceHolderLogical = new G4LogicalVolume(SourceHolderTubs,
					     copper,
					     "SourceHolderLogical"); 
  
  // --------------------- //
  // liquid N2 test stand  // 
  // --------------------- // 

  // volume : unsegmented DSG crystal (active) 

  TubsInnerRadius   = fCrystalDSGUnsegmentedInnerRadius;  
  TubsOuterRadius   = fCrystalDSGUnsegmentedOuterRadius - fCrystalDSGUnsegmentedDeadLayer; 
  TubsHeight        = 0.5 * fCrystalDSGUnsegmentedHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
  
  G4Tubs* CrystalDSGUnsegmentedActiveTubs = new G4Tubs("CrystalDSGUnsegmentedActiveTubs",
						       TubsInnerRadius,
						       TubsOuterRadius,
						       TubsHeight,
						       TubsStartAngle,
						       TubsSpanningAngle); 
  
  fCrystalDSGUnsegmentedActiveLogical = new G4LogicalVolume(CrystalDSGUnsegmentedActiveTubs,
							    enrGe,
							    "CrystalDSGUnsegmentedActiveLogical"); 
  
  G4VisAttributes* CrystalDSGUnsegmentedActiveVisAtt = new G4VisAttributes(red);
  
  CrystalDSGUnsegmentedActiveVisAtt -> SetVisibility(true);
  CrystalDSGUnsegmentedActiveVisAtt -> SetForceSolid(true);
  
  fCrystalDSGUnsegmentedActiveLogical -> SetVisAttributes(CrystalDSGUnsegmentedActiveVisAtt); 

  // volume : unsegmented DSG crystal (dead layer) 

  TubsInnerRadius   = fCrystalDSGUnsegmentedOuterRadius - fCrystalDSGUnsegmentedDeadLayer;  
  TubsOuterRadius   = fCrystalDSGUnsegmentedOuterRadius; 
  TubsHeight        = 0.5 * fCrystalDSGUnsegmentedHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
  
  G4Tubs* CrystalDSGUnsegmentedDeadLayerTubs = new G4Tubs("CrystalDSGUnsegmentedDeadLayerTubs",
							  TubsInnerRadius,
							  TubsOuterRadius,
							  TubsHeight,
							  TubsStartAngle,
							  TubsSpanningAngle);
  
  fCrystalDSGUnsegmentedDeadLayerLogical = new G4LogicalVolume(CrystalDSGUnsegmentedDeadLayerTubs,
							       enrGe,
							       "CrystalDSGUnsegmentedDeadLayerLogical");

  G4VisAttributes* CrystalDSGUnsegmentedDeadLayerVisAtt = new G4VisAttributes(darkred);
  
  CrystalDSGUnsegmentedDeadLayerVisAtt -> SetVisibility(true);
  CrystalDSGUnsegmentedDeadLayerVisAtt -> SetForceSolid(true);

  fCrystalDSGUnsegmentedDeadLayerLogical -> SetVisAttributes(CrystalDSGUnsegmentedDeadLayerVisAtt); 

  // volume : 

  // ring 1 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedRing1InnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedRing1OuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedRing1Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* ring1 = new G4Tubs("ring1",
			     TubsInnerRadius,
			     TubsOuterRadius,
			     TubsHeight,
			     TubsStartAngle,
			     TubsSpanningAngle); 
  
  // skrewhole 1 
 
  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedSkrewHole1InnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedSkrewHole1OuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedSkrewHole1Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* skrewhole1 = new G4Tubs("skrewhole1", 
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle); 
 
  G4ThreeVector skrewholeposition = this -> GetDSGHolderSkrewPosition(0); 

  G4SubtractionSolid* ring1subtractionsolid1 = new G4SubtractionSolid("ring1subtractionsolid1",
								     ring1,
								     skrewhole1, 
								     0,
								     skrewholeposition);
  // hole 2

  skrewholeposition = this -> GetDSGHolderSkrewPosition(1); 

  G4SubtractionSolid* ring1subtractionsolid2 = new G4SubtractionSolid("ring1subtractionsolid2",
								     ring1subtractionsolid1,
								     skrewhole1, 
								     0,
								     skrewholeposition);
  // hole 3 
 
  skrewholeposition = this -> GetDSGHolderSkrewPosition(2); 

  G4SubtractionSolid* ring1subtractionsolid3 = new G4SubtractionSolid("ring1subtractionsolid3",
								     ring1subtractionsolid2, 
								     skrewhole1, 
								     0,
								     skrewholeposition);
  			  
  // ring with holes 

  fCrystalHolderDSGUnsegmentedRing1Logical = new G4LogicalVolume(ring1subtractionsolid3, 
								 plastic, 
								 "CrystalHolderDSGUnsegmentedRing1Logical"); 

  G4VisAttributes* RingVisAtt = new G4VisAttributes(white); 
  RingVisAtt -> SetVisibility(true);
  RingVisAtt -> SetForceSolid(true);

  fCrystalHolderDSGUnsegmentedRing1Logical -> SetVisAttributes(RingVisAtt); 

  // ring 2  

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedRing2InnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedRing2OuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedRing2Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* ring2 = new G4Tubs("ring2",
			     TubsInnerRadius,
			     TubsOuterRadius,
			     TubsHeight,
			     TubsStartAngle,
			     TubsSpanningAngle); 

  // skrewhole 2  
 
  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedSkrewHole1InnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedSkrewHole1OuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedSkrewHole1Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* skrewhole2 = new G4Tubs("skrewhole2", 
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle); 

  skrewholeposition = this -> GetDSGHolderSkrewPosition(0); 

  G4SubtractionSolid* ring2subtractionsolid1 = new G4SubtractionSolid("ring2subtractionsolid1",
								      ring2,
								      skrewhole2, 
								      0,
								      skrewholeposition);
  
  // hole 2

  skrewholeposition = this -> GetDSGHolderSkrewPosition(1); 

  G4SubtractionSolid* ring2subtractionsolid2 = new G4SubtractionSolid("ring2subtractionsolid2",
								     ring2subtractionsolid1,
								     skrewhole2, 
								     0,
								     skrewholeposition);
  // hole 3 
 
  skrewholeposition = this -> GetDSGHolderSkrewPosition(2); 

  G4SubtractionSolid* ring2subtractionsolid3 = new G4SubtractionSolid("ring2subtractionsolid3",
								     ring2subtractionsolid2, 
								     skrewhole2, 
								     0,
								     skrewholeposition);  			  

  fCrystalHolderDSGUnsegmentedRing2Logical = new G4LogicalVolume(ring2subtractionsolid3, 
								 plastic, 
								 "CrystalHolderDSGUnsegmentedRing1Logical"); 


  fCrystalHolderDSGUnsegmentedRing2Logical -> SetVisAttributes(RingVisAtt); 

  // volume : skrew 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedSkrewInnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedSkrewOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedSkrewHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 

  G4Tubs* holderskrew = new G4Tubs("holderskrew", 
				   TubsInnerRadius,
				   TubsOuterRadius,
				   TubsHeight,
				   TubsStartAngle,
				   TubsSpanningAngle);

  fCrystalHolderDSGUnsegmentedSkrewLogical = new G4LogicalVolume(holderskrew, 
								 copper, 
								 "CrystalHolderDSGUnsegmentedSkrewLogical"); 
  
  G4VisAttributes* SkrewVisAtt = new G4VisAttributes(orange); 
  SkrewVisAtt -> SetVisibility(true);
  SkrewVisAtt -> SetForceSolid(true);

  fCrystalHolderDSGUnsegmentedSkrewLogical -> SetVisAttributes(SkrewVisAtt); 

  // volume : bar 

  BoxLengthX = 0.5 * fCrystalHolderDSGUnsegmentedBarLengthX; 
  BoxLengthY = 0.5 * fCrystalHolderDSGUnsegmentedBarLengthY; 
  BoxLengthZ = 0.5 * fCrystalHolderDSGUnsegmentedBarLengthZ; 

  G4Box* holderbar = new G4Box("holderbar", 
			       BoxLengthX , 
			       BoxLengthY, 
			       BoxLengthZ); 
  
  fCrystalHolderDSGUnsegmentedBarLogical = new G4LogicalVolume(holderbar,
							       plastic,
							       "CrystalHolderDSGUnsegmentedBarLogical");

  fCrystalHolderDSGUnsegmentedBarLogical -> SetVisAttributes(RingVisAtt); 

  // volume : small iron plate 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedPlateSmallInnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedPlateSmallOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedPlateSmallHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* smallplate = new G4Tubs("smallplate", 
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);
  
  fCrystalHolderDSGUnsegmentedPlateSmallLogical = new G4LogicalVolume(smallplate, 
								      iron, 
								      "CrystalHolderDSGUnsegmentedPlateSmallLogical"); 

  G4VisAttributes* IronPlateVisAtt = new G4VisAttributes(lightgrey);
  IronPlateVisAtt -> SetVisibility(true);
  IronPlateVisAtt -> SetForceSolid(true);
  
  fCrystalHolderDSGUnsegmentedPlateSmallLogical -> SetVisAttributes(IronPlateVisAtt); 

  // volume : large iron plate 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedPlateLargeInnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedPlateLargeOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedPlateLargeHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* largeplate = new G4Tubs("largeplate", 
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);
  
  TubsInnerRadius   = 0.0 * cm; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedRodOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedPlateLargeHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* largeplatehole = new G4Tubs("largeplatehole", 
				      TubsInnerRadius,
				      TubsOuterRadius,
				      TubsHeight,
				      TubsStartAngle,
				      TubsSpanningAngle);
  
  G4SubtractionSolid*  fCrystalHolderDSGUnsegmentedPlateLargeSubtractionSolid = 
    new G4SubtractionSolid("CrystalHolderDSGUnsegmentedPlateLargeSubtractionSolid", 
			   largeplate, 
			   largeplatehole, 
			   0, 
			   G4ThreeVector(0, 0, 0));

  fCrystalHolderDSGUnsegmentedPlateLargeLogical = 
    new G4LogicalVolume(fCrystalHolderDSGUnsegmentedPlateLargeSubtractionSolid, 
			iron, 
			"CrystalHolderDSGUnsegmentedPlateLargeLogical"); 

  fCrystalHolderDSGUnsegmentedPlateLargeLogical -> SetVisAttributes(IronPlateVisAtt); 

  // volume : teflon rod 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedRodInnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedRodOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedRodHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* teflonrod = new G4Tubs("teflonrod", 
				 TubsInnerRadius,
				 TubsOuterRadius,
				 TubsHeight,
				 TubsStartAngle,
				 TubsSpanningAngle);
  
  fCrystalHolderDSGUnsegmentedRodLogical = new G4LogicalVolume(teflonrod, 
							       plastic, 
							       "CrystalHolderDSGUnsegmentedRodLogical"); 

  fCrystalHolderDSGUnsegmentedRodLogical -> SetVisAttributes(RingVisAtt); 

  // volume : clamp 

  TubsInnerRadius   = fCrystalHolderDSGUnsegmentedClampInnerRadius; 
  TubsOuterRadius   = fCrystalHolderDSGUnsegmentedClampOuterRadius; 
  TubsHeight        = 0.5 * fCrystalHolderDSGUnsegmentedClampHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 10.0 * deg;

  G4Tubs* clamp = new G4Tubs("clamp", 
			     TubsInnerRadius,
			     TubsOuterRadius,
			     TubsHeight,
			     TubsStartAngle,
			     TubsSpanningAngle);
  
  fCrystalHolderDSGUnsegmentedClampLogical = new G4LogicalVolume(clamp, 
								 iron, 
								 "CrystalHolderDSGUnsegmentedClampLogical"); 

  fCrystalHolderDSGUnsegmentedClampLogical -> SetVisAttributes(IronPlateVisAtt); 

  // volume : AEA source

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fSourceRadiusAEA; 
  TubsHeight        = 0.5 * fSourceHeightAEA; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;
  
  G4Tubs* SourceAEATubs = new G4Tubs("SourceAEATubs",
				     TubsInnerRadius,
				     TubsOuterRadius,
				     TubsHeight,
				     TubsStartAngle,
				     TubsSpanningAngle);
  
  fSourceAEALogical = new G4LogicalVolume(SourceAEATubs,
					  steel,
					  "SourceAEALogical"); 
  
  // volume : AEA source holder

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fSourceHolderRadiusAEA; 
  TubsHeight        = 0.5 * fSourceHolderHeightTotalAEA; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SourceHolderEnvelope = new G4Tubs("SourceHolderEnvelope", 
					    TubsInnerRadius,
					    TubsOuterRadius,
					    TubsHeight,
					    TubsStartAngle,
					    TubsSpanningAngle);

  TubsInnerRadius   = fSourceHolderRadiusNeckAEA; 
  TubsOuterRadius   = fSourceHolderRadiusAEA*2.0; 
  TubsHeight        = fSourceHolderHeightNeckAEA; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SourceHolderAntiNeck = new G4Tubs("SourceHolderAntiNeck", 
					    TubsInnerRadius,
					    TubsOuterRadius,
					    TubsHeight,
					    TubsStartAngle,
					    TubsSpanningAngle);
  
  ztemp = 0.5 * fSourceHolderHeightTotalAEA;

  G4SubtractionSolid* SourceHolderAEASubstractionSolid1 =
    new G4SubtractionSolid("SourceHolderAEASubstractionSolid1", 
			   SourceHolderEnvelope, 
			   SourceHolderAntiNeck, 
			   0, 
			   G4ThreeVector(0, 0, ztemp));

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fSourceRadiusAEA;
  TubsHeight        = fSourceHeightAEA+1.2*mm; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SourceHolderHole = new G4Tubs("SourceHolderHole", 
					TubsInnerRadius,
					TubsOuterRadius,
					TubsHeight,
					TubsStartAngle,
					TubsSpanningAngle);

  G4SubtractionSolid* SourceHolderAEASubstractionSolid2 =
    new G4SubtractionSolid("SourceHolderAEASubstractionSolid2", 
			   SourceHolderAEASubstractionSolid1,
			   SourceHolderHole, 
			   0, 
			   G4ThreeVector(0, 0, -0.5*fSourceHolderHeightTotalAEA));

  fSourceHolderAEALogical = new G4LogicalVolume(SourceHolderAEASubstractionSolid2, 
						steel, 
						"SourceHolderAEALogical"); 


  // volume : detector string 

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fStringRadius; 
  TubsHeight        = 0.5 * fStringDetectorHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* DetectorStringTubs = new G4Tubs("DetectorStringTubs", 
					  TubsInnerRadius,
					  TubsOuterRadius,
					  TubsHeight,
					  TubsStartAngle,
					  TubsSpanningAngle);

  fStringDetectorLogical = new G4LogicalVolume(DetectorStringTubs, 
					       copper, 
					       "StringDetectorLogical");


  // volume : source string 

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fStringRadius; 
  TubsHeight        = 0.5 * fStringSourceHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SourceStringTubs = new G4Tubs("SourceStringTubs", 
					TubsInnerRadius,
					TubsOuterRadius,
					TubsHeight,
					TubsStartAngle,
					TubsSpanningAngle);
  
  fStringSourceLogical = new G4LogicalVolume(SourceStringTubs, 
					     copper, 
					     "StringSourceLogical");
  
  
  // volume : dewar 

  TubsInnerRadius   = 0.0 * cm; 
  TubsOuterRadius   = fDewarOuterRadius; 
  TubsHeight        = 0.5 * fDewarHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* DewarEnvelope = new G4Tubs("DewarEnvelope", 
				     TubsInnerRadius,
				     TubsOuterRadius,
				     TubsHeight,
				     TubsStartAngle,
				     TubsSpanningAngle); 
  
  TubsInnerRadius   = 0.0 * cm; 
  TubsOuterRadius   = fDewarInnerRadius; 
  TubsHeight        = 0.5 * fDewarHeight; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* DewarAntiTubs = new G4Tubs("DewarAntiTubs", 
				     TubsInnerRadius,
				     TubsOuterRadius,
				     TubsHeight,
				     TubsStartAngle,
				     TubsSpanningAngle); 
				      
  G4SubtractionSolid* DewarSubrationSolid =
    new G4SubtractionSolid("DewarSubtractionSolid", 
			   DewarEnvelope, 
			   DewarAntiTubs, 
			   0, 
			   G4ThreeVector(0, 0, 0));

  fDewarLogical = new G4LogicalVolume(DewarSubrationSolid, 
				      aluminum, 
				      "DewarLogical"); 
  

  // volume : liquid N2

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fDewarInnerRadius; 
  TubsHeight        = 0.5 * fLN2Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* LN2Tubs = new G4Tubs("LN2Tubs", 
			       TubsInnerRadius,
			       TubsOuterRadius,
			       TubsHeight,
			       TubsStartAngle,
			       TubsSpanningAngle); 
  
  fLN2Logical = new G4LogicalVolume(LN2Tubs, 
				    liquidN2, 
				    "LN2Logical"); 

  // ------------------ // 
  // Siegfried          //
  // ------------------ // 


  G4ThreeVector fComponentsPosition;
  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(0.0*cm);

  // can 

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = fSiegfried_Can_OuterRadius; 
  TubsHeight        = 0.5 * fSiegfried_Can_Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* SiegfriedCanTubs = new G4Tubs("SiegfriedCanTubs", 
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);      
                               
  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = 40.0 * mm;
  TubsHeight        =  1.0 * mm;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* SiegfriedCanHoleTubs = new G4Tubs("SiegfriedCanHoleTubs", 
                                   TubsInnerRadius,
                                   TubsOuterRadius,
                                   TubsHeight,
                                   TubsStartAngle,
                                   TubsSpanningAngle);  

  G4SubtractionSolid*  SiegfriedCanSubtractionSolid1 = 
    new G4SubtractionSolid("SiegfriedCanSubtractionSolid1",  
                           SiegfriedCanTubs, 
                           SiegfriedCanHoleTubs,
                           0, 
                           G4ThreeVector(0.0, 0.0, 0.5*fSiegfried_Can_Height)); 

  fSiegfried_CanLogical = new G4LogicalVolume(SiegfriedCanSubtractionSolid1, 
                                    aluminum, 
                                    "Siegfried_CanLogicalVolume"); 


  // vacuum

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_Can_OuterRadius - 4.0 * mm;
  TubsHeight        = 0.5 * fSiegfried_Can_Height - 2.0 * mm;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedVacuumTubs = new G4Tubs("SiegfriedVacuumTubs",
                                  TubsInnerRadius,
                                  TubsOuterRadius,
                                  TubsHeight,
                                  TubsStartAngle,
                                  TubsSpanningAngle);

  fSiegfried_VacuumLogical = new G4LogicalVolume(SiegfriedVacuumTubs,
                                       vacuum,
                                       "Siegfried_VacuumLogical");
  // IRcan

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_IRCan_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_IRCan_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedIRCanTubs = new G4Tubs("SiegfriedIRCanTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_IRCan_OuterRadius  - fSiegfried_IRCan_Thickness;
  TubsHeight        = 0.5 * fSiegfried_IRCan_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedIRCanHoleTubs = new G4Tubs("SiegfriedIRCanHoleTubs",
                                   TubsInnerRadius,
                                   TubsOuterRadius,
                                   TubsHeight,
                                   TubsStartAngle,
                                   TubsSpanningAngle);

  G4SubtractionSolid*  SiegfriedIRCanSubtractionSolid1 =
    new G4SubtractionSolid("SiegfriedIRCanSubtractionSolid1",
                           SiegfriedIRCanTubs,
                           SiegfriedIRCanHoleTubs,
                           0,
                           G4ThreeVector(0.0, 0.0, -1.5 * mm));

  fSiegfried_IRCanLogical = new G4LogicalVolume(SiegfriedIRCanSubtractionSolid1,
						aluminum,
						"Siegfried_IRCanLogicalVolume");

  // IRBase

  TubsInnerRadius   = fSiegfried_IRBase_InnerRadius;
  TubsOuterRadius   = fSiegfried_IRBase_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_IRBase_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedIRBaseTubs = new G4Tubs("SiegfriedIRBaseTubs",
					   TubsInnerRadius,
					   TubsOuterRadius,
					   TubsHeight,
					   TubsStartAngle,
					   TubsSpanningAngle);

  fSiegfried_IRBaseLogical = new G4LogicalVolume(SiegfriedIRBaseTubs,
						 aluminum,
						 "Siegfried_IRBaseLogicalVolume");

  // rod 

  TubsInnerRadius   = 15.0*mm; 
  TubsOuterRadius   = fSiegfried_Rod_Radius;
  TubsHeight        = 0.5 * fSiegfried_Rod_Height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
 
  G4Tubs* SiegfriedRodTubs = new G4Tubs("SiegfriedRodTubs", 
					TubsInnerRadius,
					TubsOuterRadius,
					TubsHeight,
					TubsStartAngle,
					TubsSpanningAngle);      
  
  fSiegfried_RodLogical = new G4LogicalVolume(SiegfriedRodTubs,
					      copper, 
					      "Siegfried_RodLogical"); 

  // base can

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_Base_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_Base_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedBaseTubs = new G4Tubs("SiegfriedBaseTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_Base_OuterRadius  - fSiegfried_Base_Thickness;
  TubsHeight        = 0.5 * fSiegfried_Base_Height - fSiegfried_Base_Thickness;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedBaseHoleTubs = new G4Tubs("SiegfriedBaseHoleTubs",
                                   TubsInnerRadius,
                                   TubsOuterRadius,
                                   TubsHeight,
                                   TubsStartAngle,
                                   TubsSpanningAngle);

  G4SubtractionSolid*  SiegfriedBaseSubtractionSolid1 =
    new G4SubtractionSolid("SiegfriedBaseSubtractionSolid1",
                           SiegfriedBaseTubs,
                           SiegfriedBaseHoleTubs,
                           0,
                           G4ThreeVector(0.0, 0.0, 0.0));

  fSiegfried_BaseLogical = new G4LogicalVolume(SiegfriedBaseSubtractionSolid1,
                                    aluminum,
                                    "Siegfried_BaseLogicalVolume");

  // base rod

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_BaseRod_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_BaseRod_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedBaseRodTubs = new G4Tubs("SiegfriedBaseRodTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_BaseRodLogical = new G4LogicalVolume(SiegfriedBaseRodTubs,
                                    copper,
                                    "Siegfried_BaseRodLogical");

  // cooling finger part inside LN2

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = fSiegfried_CoolingFinger_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_CoolingFinger_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedCoolingFingerTubs = new G4Tubs("SiegfriedCoolingFingerTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_CoolingFingerLogical = new G4LogicalVolume(SiegfriedCoolingFingerTubs,
                                    copper,
                                    "Siegfried_CoolingFingerLogical");

  // dewar neck

  TubsInnerRadius   = fSiegfried_DewarNeck_InnerRadius;
  TubsOuterRadius   = fSiegfried_DewarNeck_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_DewarNeck_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* DewarNeckTubs = new G4Tubs("DewarNeckTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_DewarNeckLogical = new G4LogicalVolume(DewarNeckTubs,
                                    steel,
                                    "Siegfried_DewarNeckLogical");

  // dewar expand

  G4Cons* DewarExpandCons = new G4Cons("DewarExpandCons",
                            fSiegfried_DewarExpand_BottomInnerRadius, 
                            fSiegfried_DewarExpand_BottomOuterRadius,
                            fSiegfried_DewarExpand_TopInnerRadius, 
                            fSiegfried_DewarExpand_TopOuterRadius,
                            fSiegfried_DewarExpand_Height*0.5,
                            0.0 * deg, 360.0 * deg);

  fSiegfried_DewarExpandLogical = new G4LogicalVolume(DewarExpandCons,
                                    steel,
                                    "Siegfried_DewarExpandLogical");

  // dewar bottom

  TubsInnerRadius   = fSiegfried_Dewar_InnerRadius;
  TubsOuterRadius   = fSiegfried_Dewar_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_Dewar_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* DewarTubs = new G4Tubs("DewarTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_DewarLogical = new G4LogicalVolume(DewarTubs,
                                    steel,
                                    "Siegfried_DewarLogical");

  // LN2

  TubsInnerRadius   = fSiegfried_LN2_InnerRadius;
  TubsOuterRadius   = fSiegfried_LN2_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_LN2_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SLN2Tubs = new G4Tubs("SLN2Tubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_LN2Logical = new G4LogicalVolume(SLN2Tubs,
                                    liquidN2,
                                    "Siegfried_LN2Logical");

  // fanout box
  G4Box* FanoutBoxOrig = new G4Box("FanoutBoxOrig", 
                               fSiegfried_FanoutBox_lengthX,
                               fSiegfried_FanoutBox_lengthY,
                               fSiegfried_FanoutBox_lengthZ);

  G4Box* FanoutBoxInner = new G4Box("FanoutBoxInner", 
                               fSiegfried_PreBottomHole_X,
                               fSiegfried_PreBottomHole_Y,
                               fSiegfried_FanoutBox_lengthZ*2);

  G4SubtractionSolid* FanoutBox 
    = new G4SubtractionSolid("FanoutBox",
			     FanoutBoxOrig,
			     FanoutBoxInner,
			     0,
			     G4ThreeVector(0,0,0));

  fSiegfried_FanoutBoxLogical = new G4LogicalVolume(FanoutBox,
                                    aluminum,
                                    "Siegfried_FanoutBoxLogical");

  // preamp ring

  TubsInnerRadius   = fSiegfried_PreampRing_InnerRadius;
  TubsOuterRadius   = fSiegfried_PreampRing_OuterRadius;
  TubsHeight        = 0.5 * fSiegfried_PreampRing_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* PreampRingTubs = new G4Tubs("PreampRingTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_PreampRingLogical = new G4LogicalVolume(PreampRingTubs,
                                    steel,
                                    "Siegfried_PreampRingLogical");

  // preamp box
/*
  G4Box* PreampBoxOrig = new G4Box("PreampBoxOrig",
                               fSiegfried_PreampBox_lengthX*0.5,
                               fSiegfried_PreampBox_lengthY*0.5,
                               fSiegfried_PreampBox_lengthZ*0.5 );

  G4Box* PreampBoxInner = new G4Box("PreampBoxInner",
                               fSiegfried_PreampBox_lengthX*0.5 - fSiegfried_PreampBox_Thickness,
                               fSiegfried_PreampBox_lengthY*0.5 - fSiegfried_PreampBox_Thickness,
                               fSiegfried_PreampBox_lengthZ*0.5 - fSiegfried_PreampBox_Thickness);

  G4SubtractionSolid* PreampBox = new G4SubtractionSolid("PreampBox",
                                        PreampBoxOrig,
                                        PreampBoxInner,
                                        0,
                                        G4ThreeVector(0,0,0));

  fSiegfried_PreampBoxLogical = new G4LogicalVolume(PreampBox,
                                    steel,
                                    "Siegfried_PreampBoxLogical");
*/
  // dead layer 

  TubsInnerRadius   = fSiegfried_Crystal_InnerRadius; 
  TubsOuterRadius   = fSiegfried_Crystal_OuterRadius; 
  TubsHeight        = 0.5 * fSiegfried_Crystal_Height - fSiegfried_Crystal_SiliconThickness;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* SiegfriedCrystalDeadLayerTubs = new G4Tubs("SiegfriedCrystalDeadLayerTubs", 
                                            TubsInnerRadius,
                                            TubsOuterRadius,
                                            TubsHeight,
                                            TubsStartAngle,
                                            TubsSpanningAngle); 
          
  TubsInnerRadius   = fSiegfried_Crystal_InnerRadius + fSiegfried_Crystal_DeadLayerInnerThickness; 
  TubsOuterRadius   = fSiegfried_Crystal_OuterRadius - fSiegfried_Crystal_DeadLayerOuterThickness; 
  TubsHeight        = 0.5 * fSiegfried_Crystal_Height 
                    - fSiegfried_Crystal_DeadLayerTopThickness 
                    - fSiegfried_Crystal_SiliconThickness;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
          
  G4Tubs* SiegfriedCrystalDeadLayerHoleTubs = new G4Tubs("SiegfriedCrystalDeadLayerHoleTubs", 
                                                TubsInnerRadius, 
                                                TubsOuterRadius, 
                                                TubsHeight,
                                                TubsStartAngle,
                                                TubsSpanningAngle);
          
  G4SubtractionSolid* SiegfriedCrystalDeadLayerSubtractionSolid = 
    new G4SubtractionSolid("SiegfriedCrystalSubtractionSolid",  
                           SiegfriedCrystalDeadLayerTubs, 
                           SiegfriedCrystalDeadLayerHoleTubs,
                           0, 
                           G4ThreeVector(0.0, 0.0, 0.0)); 
  
  fSiegfried_CrystalDeadLayerLogical = new  G4LogicalVolume(SiegfriedCrystalDeadLayerSubtractionSolid,
                                                  natGe, 
                                                  "Siegfried_CrystalDeadLayerLogical"); 


  // silicon 

  TubsInnerRadius   = fSiegfried_Crystal_InnerRadius; 
  TubsOuterRadius   = fSiegfried_Crystal_OuterRadius; 
  TubsHeight        = 0.5 * fSiegfried_Crystal_SiliconThickness; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* CrystalSiliconTubs = new G4Tubs("CrystalSiliconTubs", 
                                          TubsInnerRadius, 
                                          TubsOuterRadius, 
                                          TubsHeight,
                                          TubsStartAngle,
                                          TubsSpanningAngle);
  
  fSiegfried_CrystalSiliconLogical = new G4LogicalVolume(CrystalSiliconTubs, 
                                               si, 
                                               "Siegfried_CrystalSiliconTubs"); 

  // Crystal
  TubsInnerRadius   = fSiegfried_Crystal_InnerRadius + fSiegfried_Crystal_DeadLayerInnerThickness; 
  TubsOuterRadius   = fSiegfried_Crystal_OuterRadius - fSiegfried_Crystal_DeadLayerOuterThickness; 
  TubsHeight        = 0.5 * fSiegfried_Crystal_Height - fSiegfried_Crystal_DeadLayerTopThickness;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360 * deg; 

  G4Tubs* CrystalTubs = new G4Tubs("CrystalTubs", 
                                   TubsInnerRadius,
                                   TubsOuterRadius,
                                   TubsHeight,
                                   TubsStartAngle,
                                   TubsSpanningAngle);
          
  fSiegfried_CrystalLogical = new G4LogicalVolume(CrystalTubs, 
                                                  natGe, 
                                                  "Siegfried_CrystalLogical"); 

  // source system
  //--------------
  
  //mother volume
  G4Tubs* SiegfriedSourceMotherVolume
    = new G4Tubs("SiegfriedSourceMotherVolume", 
                 0,
		 fSiegfried_collimator_OuterRadius,
                 fSiegfried_collimator_Height,
		 0.0 * deg, 
		 360.0 * deg);      
  
  fSiegfried_SourceMotherVolume
    = new G4LogicalVolume(SiegfriedSourceMotherVolume,
			  air, 
			  "Siegfried_SourceMotherVolume"); 

  // collimator
  G4Tubs* SiegfriedCollimatorShell 
    = new G4Tubs("SiegfriedCollimatorShell", 
                 fSiegfried_collimator_InnerRadius,
		 fSiegfried_collimator_OuterRadius,
                 fSiegfried_collimator_Height * 0.5,
		 0.0 * deg, 360.0 * deg);      
  
  G4Tubs* SiegfriedCollimatorDiaphragm 
    = new G4Tubs("SiegfriedCollimatorDiaphram", 
                 fSiegfried_collimatorDiaphragm_InnerRadius,
		 fSiegfried_collimator_OuterRadius-1.0*mm,
                 fSiegfried_collimatorDiaphragm_Height * 0.5,
		 0.0 * deg, 360.0 * deg);      
  
  ztemp = fSiegfried_collimator_Height * 0.5 - fSiegfried_collimatorHole_UpperHeight
    - fSiegfried_collimatorDiaphragm_Height * 0.5;
  G4UnionSolid* SiegfriedCollimator = 
    new G4UnionSolid("SiegfriedCollimator",  
                     SiegfriedCollimatorShell,
                     SiegfriedCollimatorDiaphragm,
		     0, 
		     G4ThreeVector(0.0, 0.0, ztemp)); 

  fSiegfried_CollimatorLogical 
    = new G4LogicalVolume(SiegfriedCollimator,
			  copper, 
			  "Siegfried_CollimatorLogical"); 

  // source holder
  TubsInnerRadius   = fSiegfried_SourceHolder_InnerRadius; 
  TubsOuterRadius   = fSiegfried_SourceHolder_OuterRadius; 
  TubsHeight        = 0.5 * fSiegfried_SourceHolder_Height;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* SiegfriedSourceHolderTubs = new G4Tubs("SiegfriedSourceHolderTubs", 
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_SourceHolderLogical = new G4LogicalVolume(SiegfriedSourceHolderTubs,
                                    copper, 
                                    "Siegfried_SourceHolderLogical"); 

  // Source holder stick
  TubsInnerRadius   = 0; 
  TubsOuterRadius   = 3.5 * mm;
  TubsHeight        = 0.5 * fSiegfried_SourceHolderStickLength;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* SiegfriedSourceHolderStickTubs = new G4Tubs("SiegfriedSourceHolderStickTubs", 
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_SourceHolderStickLogical
    = new G4LogicalVolume(SiegfriedSourceHolderStickTubs,
			  PVC,
//			  lead,
			  "Siegfried_SourceHolderStickLogical"); 


  // source (same source as other test-stands?)

  TubsInnerRadius   = 0.0*cm;
  TubsOuterRadius   = fSiegfried_Source_Radius;
  TubsHeight        = 0.5 * fSiegfried_Source_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedSourceTubs = new G4Tubs("SiegfriedSourceTubs",
                               TubsInnerRadius,
                               TubsOuterRadius,
                               TubsHeight,
                               TubsStartAngle,
                               TubsSpanningAngle);

  fSiegfried_SourceLogical = new G4LogicalVolume(SiegfriedSourceTubs,
                                    plastic,
                                    "Siegfried_SourceLogical");


  // preamp tubs
  // -----------

  //main tub
  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fSiegfried_PreFringe_Radius;
  TubsHeight        = 0.5 * fSiegfried_PreTub_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedMainTubs = new G4Tubs("SiegfriedMainTubs",
					 TubsInnerRadius,
					 TubsOuterRadius,
					 TubsHeight,
					 TubsStartAngle,
					 TubsSpanningAngle);

  //outer tub
  TubsInnerRadius   = fSiegfried_PreTub_Radius;
  TubsOuterRadius   = fSiegfried_PreFringe_Radius + 1*mm;
  TubsHeight        = 0.5 * fSiegfried_PreTub_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedOuterTubs = new G4Tubs("SiegfriedOuterTubs",
					  TubsInnerRadius,
					  TubsOuterRadius,
					  TubsHeight,
					  TubsStartAngle,
					  TubsSpanningAngle);

  G4SubtractionSolid* SiegfriedMO = 
    new G4SubtractionSolid("SiegfriedMO",  
			   SiegfriedMainTubs,
			   SiegfriedOuterTubs,
			   0, 
			   G4ThreeVector(0.0, 0.0, fSiegfried_PreFringe_Thickness)); 

  //inner tub
  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fSiegfried_PreTub_Radius - fSiegfried_PreTub_Thickness;
  TubsHeight        = 0.5 * fSiegfried_PreTub_Height;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedInnerTubs = new G4Tubs("SiegfriedInnerTubs",
					  TubsInnerRadius,
					  TubsOuterRadius,
					  TubsHeight,
					  TubsStartAngle,
					  TubsSpanningAngle);

  G4SubtractionSolid* SiegfriedMOI = 
    new G4SubtractionSolid("SiegfriedMOI",  
			   SiegfriedMO,
			   SiegfriedInnerTubs,
			   0, 
			   G4ThreeVector(0.0, 0.0, -fSiegfried_PreTop_Thickness)); 

  //top tub
  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fSiegfried_PreTop_Radius;
  TubsHeight        = 2*fSiegfried_PreTop_Thickness;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedTopTubs = new G4Tubs("SiegfriedTopTubs",
					TubsInnerRadius,
					TubsOuterRadius,
					TubsHeight,
					TubsStartAngle,
					TubsSpanningAngle);

  G4SubtractionSolid* SiegfriedMOIT 
    = new G4SubtractionSolid("SiegfriedMOIT",  
			     SiegfriedMOI,
			     SiegfriedTopTubs,
			     0, 
			     G4ThreeVector(0.0, 0.0, 0.5 * fSiegfried_PreTub_Height)); 

  fSiegfried_PreTubLogical = new G4LogicalVolume(SiegfriedMOIT,
						 copper,
						 "Siegfried_PreTubLogical");

  //Preamp tub bottom
  TubsInnerRadius   = 0.0 * mm;
  TubsOuterRadius   = fSiegfried_PreFringe_Radius;
  TubsHeight        = 0.5 * fSiegfried_PreFringe_Thickness;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* SiegfriedBottomTubs = new G4Tubs("SiegfriedBottomTubs",
					   TubsInnerRadius,
					   TubsOuterRadius,
					   TubsHeight,
					   TubsStartAngle,
					   TubsSpanningAngle);
  
  BoxLengthX = fSiegfried_PreBottomHole_X;
  BoxLengthY = fSiegfried_PreBottomHole_Y;
  BoxLengthZ = fSiegfried_PreFringe_Thickness; 

  G4Box* SiegfriedBottomBox = new G4Box("SiegfriedBottomBox", 
					BoxLengthX , 
					BoxLengthY, 
					BoxLengthZ); 
  G4SubtractionSolid* SiegfriedPreBottom = 
    new G4SubtractionSolid("SiegfriedPreBottom",  
			   SiegfriedBottomTubs,
			   SiegfriedBottomBox,
			   0,
			   G4ThreeVector(0.0, 0.0, 0.0)); 
  
  fSiegfried_PreBottomLogical = new G4LogicalVolume(SiegfriedPreBottom,
						    copper,
						    "Siegfried_PreBottomLogical");

  // ------------------------------
  // sensitive detector definition
  // ------------------------------

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String CrystalSDname      = "/mydet/gerda/gecrystal"; 

  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);

  SDman -> AddNewDetector(CrystalSD);

  fCrystalActiveLogical -> SetSensitiveDetector(CrystalSD); 

  fCrystalDSGUnsegmentedActiveLogical -> SetSensitiveDetector(CrystalSD); 
  
  fSiegfried_CrystalLogical -> SetSensitiveDetector(CrystalSD);

// -------------------------------------------------
// Collimator and Detector
// -------------------------------------------------
 
  G4ThreeVector fCollComponentsPosition;
  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ(0.0*cm);


  // vacuum

  TubsInnerRadius   = 0.0;
  TubsOuterRadius   = 0.5 * m;
  TubsHeight        = 1.0 * m;
  TubsStartAngle    = 0.0 * deg;
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* CollimatorVacuumTubs = new G4Tubs("CollimatorVacuumTubs",
                                  TubsInnerRadius,
                                  TubsOuterRadius,
                                  TubsHeight,
                                  TubsStartAngle,
                                  TubsSpanningAngle);

  fCollimatorVacuumLogical = new G4LogicalVolume(CollimatorVacuumTubs,
                                       vacuum,
                                       "CollimatorVacuumLogical");





 // dead layer 

  TubsInnerRadius   = fCollimator_Crystal_InnerRadius; 
  TubsOuterRadius   = fCollimator_Crystal_OuterRadius; 
  TubsHeight        = 0.5 * fCollimator_Crystal_Height - fCollimator_Crystal_SiliconThickness;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* CollimatorCrystalDeadLayerTubs = new G4Tubs("CollimatorCrystalDeadLayerTubs", 
                                            TubsInnerRadius,
                                            TubsOuterRadius,
                                            TubsHeight,
                                            TubsStartAngle,
                                            TubsSpanningAngle); 
          
  TubsInnerRadius   = fCollimator_Crystal_InnerRadius + fCollimator_Crystal_DeadLayerInnerThickness; 
  TubsOuterRadius   = fCollimator_Crystal_OuterRadius - fCollimator_Crystal_DeadLayerOuterThickness; 
  TubsHeight        = 0.5 * fCollimator_Crystal_Height 
                    - fCollimator_Crystal_DeadLayerTopThickness 
                    - fCollimator_Crystal_SiliconThickness;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
          
  G4Tubs* CollimatorCrystalDeadLayerHoleTubs = new G4Tubs("CollimatorCrystalDeadLayerHoleTubs", 
                                                TubsInnerRadius, 
                                                TubsOuterRadius, 
                                                TubsHeight,
                                                TubsStartAngle,
                                                TubsSpanningAngle);
          
  G4SubtractionSolid* CollimatorCrystalDeadLayerSubtractionSolid = 
    new G4SubtractionSolid("CollimatorCrystalSubtractionSolid",  
                           CollimatorCrystalDeadLayerTubs, 
                           CollimatorCrystalDeadLayerHoleTubs,
                           0, 
                           G4ThreeVector(0.0, 0.0, 0.0)); 
  
  fCollimatorCrystalDeadLayerLogical = new  G4LogicalVolume(CollimatorCrystalDeadLayerSubtractionSolid,
                                                  natGe, 
                                                  "CollimatorCrystalDeadLayerLogical"); 
  
  // silicon 

  TubsInnerRadius   = fCollimator_Crystal_InnerRadius; 
  TubsOuterRadius   = fCollimator_Crystal_OuterRadius; 
  TubsHeight        = 0.5 * fCollimator_Crystal_SiliconThickness; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* Crystal_SiliconTubs = new G4Tubs("Crystal_SiliconTubs", 
                                          TubsInnerRadius, 
                                          TubsOuterRadius, 
                                          TubsHeight,
                                          TubsStartAngle,
                                          TubsSpanningAngle);
  
  fCollimatorCrystalSiliconLogical = new G4LogicalVolume(Crystal_SiliconTubs, 
                                               si, 
                                               "CollimatorCrystalSiliconTubs"); 

  // segments 

  TubsInnerRadius   = fCollimator_Crystal_InnerRadius + fCollimator_Crystal_DeadLayerInnerThickness; 
  TubsOuterRadius   = fCollimator_Crystal_OuterRadius - fCollimator_Crystal_DeadLayerOuterThickness; 
  TubsHeight        = 0.5 * fCollimator_Segment_Height;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = fCollimator_Segment_dPhi; 
          
  G4Tubs* Crystal_SegmentTubs = new G4Tubs("Crystal_SegmentTubs", 
                                          TubsInnerRadius,
                                          TubsOuterRadius,
                                          TubsHeight,
                                          TubsStartAngle,
                                          TubsSpanningAngle);
          
  fCollimatorCrystalSegmentLogical = new G4LogicalVolume(Crystal_SegmentTubs, 
                                               natGe, 
                                               "CollimatorCrystalSegmentLogical"); 

  // ------------------------------
  // sensitive detector definition
  // ------------------------------

 /* G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String CrystalSDname      = "/mydet/gerda/gecrystal"; 

  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);

  SDman -> AddNewDetector(CrystalSD);

  fCrystalActiveLogical -> SetSensitiveDetector(CrystalSD); 

  fCrystalDSGUnsegmentedActiveLogical -> SetSensitiveDetector(CrystalSD); */
  
  fCollimatorCrystalSegmentLogical -> SetSensitiveDetector(CrystalSD);


  // collimator system
  //--------------
  
  //mother volume
  G4Tubs* CollimatorMotherVolumeLogical
    = new G4Tubs("CollimatorMotherVolume", 
                 0,
		 fCollimator_MotherVolume_R,
                 fCollimator_MotherVolume_Height*0.5,
		 0.0 * deg, 
		 360.0 * deg);      
  
  fCollimatorMotherVolumeLogical
    = new G4LogicalVolume(CollimatorMotherVolumeLogical,
			  vacuum, 
			  "CollimatorMotherVolume"); 

  // collimator

  G4Tubs* CollimatorUpperHole 
    = new G4Tubs("CollimatorUpperHole", 
                 0.0,
                 fCollimator_SourceHolder_OuterRadius,
                 fCollimator_SourceHolder_Height *0.5 + 0.1*mm,
		 0.0 * deg, 
		 360.0 * deg);

  G4Tubs* CollimatorTube 
    = new G4Tubs("CollimatorTube", 
                 0.1 * mm,
		 fCollimator_OuterRadius,
                 fCollimator_Height *0.5,
		 0.0 * deg, 
		 360.0 * deg);


  G4SubtractionSolid* CollimatorShell
    = new G4SubtractionSolid("CollimatorShell",
		            CollimatorTube,
		            CollimatorUpperHole,
		            0,
		            G4ThreeVector(0.0,0.0,0.5*(fCollimator_Height-fCollimator_SourceHolder_Height)));

 G4SubtractionSolid* Collimator
    = new G4SubtractionSolid("Collimator",
		            CollimatorShell,
		            0,
		            0,
	            	    G4ThreeVector(0.0,0.0,0.0));
  
 if (fCollimatorType =="Tube")
 {
   G4Tubs* CollimatorHole
   = new G4Tubs("CollimatorHole",
                0.0,
		fCollimator_InnerRadius,
		fCollimator_LowerHeight*0.5 + 0.1 * mm,
		0.0*deg,
		360.0*deg);

   Collimator
    = new G4SubtractionSolid("Collimator",
		            CollimatorShell,
		            CollimatorHole,
		            0,
	            	    G4ThreeVector(0.0,0.0,0.5*(fCollimator_LowerHeight-fCollimator_Height)));
 
 }

 if (fCollimatorType =="Conical")
 {
 G4Cons* CollimatorHole
   = new G4Cons("CollimatorHole",
		0.0,
                fCollimator_InnerRadius,
                0.0,
                0.1 * mm,
	        fCollimator_LowerHeight*0.5 + 0.1 * mm,
                0.0 * deg,
                360.0 * deg);
 
 Collimator
    = new G4SubtractionSolid("Collimator",
		            CollimatorShell,
		            CollimatorHole,
		            0,
	            	    G4ThreeVector(0.0,0.0,0.5*(fCollimator_LowerHeight-fCollimator_Height)));
 
 }

 if (fCollimatorType == "Slices")
 {

G4Tubs* CollimatorSlice 
    = new G4Tubs("CollimatorSlice", 
                 0.0 * mm,
		 fCollimator_OuterRadius - 1.0 * mm,
                 fCollimator_Slice_Height * 0.5,
		 0.0 * deg, 
		 360.0 * deg);

 for (int iz = 0; iz < iCollimator_Slice_Nz; iz++) 
  {
   G4SubtractionSolid*  CollimatorSkeletton
    = new G4SubtractionSolid("CollimatorSkeletton",
		            CollimatorShell,
		            CollimatorSlice,
		            0,
		            G4ThreeVector(0.0,0.0,2.0*mm - 0.5*fCollimator_Height + float(4*iz+1)*0.5*fCollimator_Slice_Height));
  CollimatorShell = CollimatorSkeletton;
  }

  G4Cons* CollimatorHole
   = new G4Cons("CollimatorHole",
		0.0,
                fCollimator_InnerRadius,
                0.0,                0.5 * mm,
	        fCollimator_LowerHeight*0.5 + 0.1 * mm,
                0.0 * deg,
                360.0 * deg);
 
 Collimator
    = new G4SubtractionSolid("Collimator",
		            CollimatorShell,
		            CollimatorHole,
		            0,
	            	    G4ThreeVector(0.0,0.0,0.5*(fCollimator_LowerHeight-fCollimator_Height)));
 
 }

 if (fCollimatorMaterial=="Vacuum")
 {fCollimatorActiveLogical 
    = new G4LogicalVolume(Collimator,
                          vacuum,
                          "CollimatorActiveLogical");}

 if (fCollimatorMaterial=="Copper")
 {fCollimatorActiveLogical 
    = new G4LogicalVolume(Collimator,
                          copper,
                          "CollimatorActiveLogical");}
			  
 if (fCollimatorMaterial=="Lead")
 {fCollimatorActiveLogical 
    = new G4LogicalVolume(Collimator,
                          lead,
                          "CollimatorActiveLogical");}
			  
 if (fCollimatorMaterial=="Tungsten")
 {fCollimatorActiveLogical 
    = new G4LogicalVolume(Collimator,
                          tungsten,
                          "CollimatorActiveLogical");}
	

  // ------------------------------
  // sensitive collimator definition
  // ------------------------------

 /* G4SDManager* SDman = G4SDManager::GetSDMpointer();*/

  G4String CollimatorSDname      = "/mydet/gerda/collimatorsd"; 

  GEGeometrySD* CollimatorSD      = new GEGeometrySD(CollimatorSDname);

  SDman -> AddNewDetector(CollimatorSD);

  fCollimatorActiveLogical -> SetSensitiveDetector(CollimatorSD); 

 // -----------------------------------------
 // thin foil between collimator and detector
 // -----------------------------------------

   G4Box *CollimatorFoil = new G4Box("CollimatorFoil",
                                     fCollimator_OuterRadius,
                                     fCollimator_OuterRadius,
                                     fCollimator_Foil_Height*0.5); 

   fCollimatorFoilLogical = new G4LogicalVolume(CollimatorFoil,
                                    silver,
                                    "CollimatorFoilLogical");

  // ------------------------------
  // source
  // ------------------------------

 

 G4cout << "source holder geometry" << G4endl; 

 // source holder
   G4Tubs* CollimatorSourceHolderTubs = new G4Tubs("CollimatorSourceHolderTubs", 
                               fCollimator_SourceHolder_InnerRadius,
                               fCollimator_SourceHolder_OuterRadius,
                               fCollimator_SourceHolder_Height * 0.5,
                               0.0 * deg,
                               360 * deg);      

  fCollimatorSourceHolderLogical = new G4LogicalVolume(CollimatorSourceHolderTubs,
                                    copper, 
                                    "CollimatorSourceHolderLogical"); 

  // source (same source as other test-stands?)

  G4Tubs* CollimatorSourceTubs = new G4Tubs("CollimatorSourceTubs",
                               0.0 * mm,
                               fCollimator_Source_Radius,
                               fCollimator_Source_Height * 0.5,
                               0.0 * deg,
                               360.0 * deg);

  fCollimatorSourceLogical = new G4LogicalVolume(CollimatorSourceTubs,
                                    plastic,
                                    "SourceLogical");


 G4cout << "source geometry" << G4endl;

 

  // ------------------------------
  // Milchkanne / Mikesch
  // ------------------------------

 G4Tubs* MkCoreElectrode = new G4Tubs("MkCoreElectrode",
				      fMikesch_CoreElectrode_InnerRadius,
				      fMikesch_CoreElectrode_OuterRadius,
				      fMikesch_CoreElectrode_Height,
				      0. * deg,
				      360. * deg);
 
 MkCoreElectrodeLogical = new G4LogicalVolume(MkCoreElectrode,
					      copper,
					      "MkCoreElectrode");
 
 G4VisAttributes* CoreElectrodeVisAtt = new G4VisAttributes(red);
 
 MkCoreElectrodeLogical->SetVisAttributes( CoreElectrodeVisAtt);

//  G4Tubs* MkTubsInnerDeadLayer = new G4Tubs("MkTubsInnerDeadLayer",
// 					   fMikesch_InnerDeadLayer_InnerRadius,
// 					   fMikesch_InnerDeadLayer_OuterRadius,
// 					   fMikesch_InnerDeadLayer_Height,
// 					   0. * deg,
// 					   360. * deg);
 
//  G4Tubs* MkTubsInnerDeadLayerSubtr = new G4Tubs("MkTubsInnerDeadLayerSubtr",
// 						fMikesch_InnerDeadLayerSubtr_InnerRadius,
// 						fMikesch_InnerDeadLayerSubtr_OuterRadius,
// 						fMikesch_InnerDeadLayerSubtr_Height,
// 						0. * deg,
// 						360*deg);
 
//  G4SubtractionSolid* MkInnerDeadLayer = new G4SubtractionSolid("MkInnerDeadLayer",
// 							       MkTubsInnerDeadLayer,
// 							       MkTubsInnerDeadLayerSubtr,
// 							       0.,
// 							       0.);
 

//  MkInnerDeadLayerLogical = new G4LogicalSolid(MkInnerDeadLayer,
// 					      lithium, //!!!
//					      "MkInnerDeadLayer");

 G4Tubs* MkTubsMikesch = new G4Tubs("MkTubsMikesch",
				    fMikesch_InnerRadius,
				    fMikesch_OuterRadius,
				    fMikesch_Height,
 				    0.* deg,
				    360. * deg);

 G4Tubs* MkTubsMikeschSubtr = new G4Tubs("MkTubsMikeschSubtr",
					 fMikesch_SubtrInnerRadius,
					 fMikesch_SubtrOuterRadius,
					 fMikesch_SubtrHeight,
					 0.* deg,
					 360. * deg);

 



 G4SubtractionSolid* MkMikesch = new G4SubtractionSolid("MkMikesch",
							MkTubsMikesch,
							MkTubsMikeschSubtr,
							0,
							G4ThreeVector(0,0, fMikesch_Height - fMikesch_SubtrHeight));


 MkMikeschLogical = new G4LogicalVolume(MkMikesch,
					natGe,
					"MkMikeschLogical"); 

 MkMikeschLogical -> SetSensitiveDetector(CrystalSD);




  fMikesch_Holder_Height      = 77.2 * 0.5 *mm;
  fMikesch_Holder_InnerRadius = 0.; 
  fMikesch_Holder_OuterRadius = 70 * 0.5 * mm ;

  fMikesch_HolderSubtr_Height      = 77.2 * 0.5 *mm;
  fMikesch_HolderSubtr_InnerRadius = 0.; 
  fMikesch_HolderSubtr_OuterRadius = 67 * 0.5 * mm;


 G4Tubs* MkTubsHolder = new G4Tubs("MkMikeschHoder",
				   fMikesch_Holder_InnerRadius,
				   fMikesch_Holder_OuterRadius,
				   fMikesch_Holder_Height,
 				    0.* deg,
				    360. * deg);

 G4Tubs* MkTubsHolderSubtr = new G4Tubs("MkTubsMikeschHolderSubtr",
					 fMikesch_HolderSubtr_InnerRadius,
					 fMikesch_HolderSubtr_OuterRadius,
					 fMikesch_HolderSubtr_Height,
					 0.* deg,
					 360. * deg);

 

 

 G4SubtractionSolid* MkHolder1 = new G4SubtractionSolid("MkHolder1",
							MkTubsHolder,
							MkTubsHolderSubtr,
							0,
							G4ThreeVector(0,0, -3 * mm));

 G4Tubs* MkTubsHolderHole = new G4Tubs("MkTubsMikeschHolderhole",
				       fMikesch_CoolingStick_InnerRadius,
				       fMikesch_CoolingStick_OuterRadius,
				       fMikesch_HolderSubtr_Height,
				       0.* deg,
				       360. * deg);

 G4SubtractionSolid* MkHolder = new G4SubtractionSolid("MkHolder",
						       MkHolder1,
						       MkTubsHolderHole,
						       0,
						       G4ThreeVector(0,0,5 * mm));


 MkHolderLogical = new G4LogicalVolume(MkHolder,
				       aluminum,
				       "MkHolderLogical"); 

 G4Tubs* MkTubsTeflonWrap = new G4Tubs("MkTubsTeflonWrap",
				   fMikesch_Teflon_InnerRadius,
				   fMikesch_Teflon_OuterRadius,
				   fMikesch_Teflon_Height,
				   0.* deg,
				   360. * deg);

 G4Tubs* MkTubsTeflonWrapSubtr = new G4Tubs("MkTubsTeflonWrapSubtr",
					    fMikesch_TeflonSubtr_InnerRadius,
					    fMikesch_TeflonSubtr_OuterRadius,
					    fMikesch_TeflonSubtr_Height,
					    0.* deg,
					    360. * deg);
 
 G4SubtractionSolid* MkTeflonWrap = new G4SubtractionSolid("MkTeflonWrap",
							   MkTubsTeflonWrap,
							   MkTubsTeflonWrapSubtr,
							   0,
							   G4ThreeVector(0,0,0));


 MkTeflonWrapLogical = new G4LogicalVolume(MkTeflonWrap,
					   teflon,
					   "MkTeflonWrapLogical"); 
 

 G4Tubs* MkCoolingStickInside = new G4Tubs("MkCoolingStickInside",
				    fMikesch_CoolingStick_InnerRadius,
				    fMikesch_CoolingStick_OuterRadius,
				    fMikesch_CoolingStickInside_Height,
				    0. * deg,
				    360. * deg);


 MkCoolingStickInsideLogical = new G4LogicalVolume(MkCoolingStickInside,
						   copper,
						   "MkCoolingStickInsideLogical");

 G4Tubs* MkCoolingStickOutside = new G4Tubs("MkCoolingStickOutside",
				    fMikesch_CoolingStick_InnerRadius,
				    fMikesch_CoolingStick_OuterRadius,
				    fMikesch_CoolingStickOutside_Height,
				    0. * deg,
				    360. * deg);


 MkCoolingStickOutsideLogical = new G4LogicalVolume(MkCoolingStickOutside,
						   copper,
						   "MkCoolingStickOutsideLogical");


 G4VisAttributes* CoolingStickVisAtt = new G4VisAttributes(orange);
 
 MkCoolingStickInsideLogical  -> SetVisAttributes(CoolingStickVisAtt);
 MkCoolingStickOutsideLogical -> SetVisAttributes(CoolingStickVisAtt);



 G4Tubs* MkDewar = new G4Tubs("MkDewar",
			      fMilchkanne_OuterWall_InnerRadius,
			      fMilchkanne_OuterWall_OuterRadius,
			      fMilchkanne_OuterWall_Height,
			      0. * deg,
			      360. * deg);


 G4Tubs* MkDewarSubtract = new G4Tubs("MkDewarSubtract",
				      fMilchkanne_OuterWallSubtr_InnerRadius,
				      fMilchkanne_OuterWallSubtr_OuterRadius,
				      fMilchkanne_OuterWallSubtr_Height,
				      0. * deg,
				      360. * deg);


 G4SubtractionSolid* MkOuterDewarSubtract = new G4SubtractionSolid("MkOuterDewarSubtract",
								   MkDewar,
								   MkDewarSubtract,
 								   0,
								   G4ThreeVector(0,0, fMilchkanne_OuterWall_OuterRadius - fMilchkanne_OuterWallSubtr_OuterRadius ));

  

 G4Tubs* MkDewarInner = new G4Tubs("MkDewarInner",
				   fMilchkanne_InnerWall_InnerRadius,
				   fMilchkanne_InnerWall_OuterRadius,
				   fMilchkanne_InnerWall_Height,
				   0. * deg,
				   360. * deg);

 
 G4Tubs* MkDewarInnerSubtract = new G4Tubs("MkDewarInnerSubtract",
					   fMilchkanne_InnerWallSubtr_InnerRadius,
					   fMilchkanne_InnerWallSubtr_OuterRadius,
					   fMilchkanne_InnerWallSubtr_Height,
					   0. * deg,
					   360. * deg);
 
 


 G4SubtractionSolid* MkInnerDewarSubtract = new G4SubtractionSolid("MkInnerDewarSubtract",
								   MkDewarInner,
								   MkDewarInnerSubtract,
 								   0,
								   G4ThreeVector(0,0, fMilchkanne_InnerWall_OuterRadius - fMilchkanne_InnerWallSubtr_OuterRadius ));


 G4UnionSolid* MkDewarWalls = new G4UnionSolid("MkDewarWalls",
					       MkOuterDewarSubtract,
					       MkInnerDewarSubtract,
					       0,
					       G4ThreeVector());

 G4Tubs* MkLid = new G4Tubs("MKLid",
			    fMilchkanne_Lid_InnerRadius,
			    fMilchkanne_Lid_OuterRadius,
			    fMilchkanne_Lid_Height,
			    0. * deg,
			    360. * deg);

 G4Tubs* MkLidHole = new G4Tubs("MKLidHole",
				fMilchkanne_LidHole_InnerRadius,
				fMilchkanne_LidHole_OuterRadius,
				fMilchkanne_LidHole_Height,
				0. * deg,
				360. * deg);


 // G4Translation = ;

 G4SubtractionSolid* MkLidSubtraction = new G4SubtractionSolid("MkLidSubtraction",
							       MkLid,
							       MkLidHole,
							       0,
							       G4ThreeVector(fMilchkanne_LidHole_XPosition,fMilchkanne_LidHole_YPosition,0));

 G4UnionSolid* MkDewarComplete = new G4UnionSolid("MkDewarComplete",
						  MkDewarWalls,
						  MkLidSubtraction,
						  0,
						  G4ThreeVector(0,0,fMilchkanne_OuterWall_Height + fMilchkanne_Lid_Height)); 

 MkDewarLogical = new G4LogicalVolume(MkDewarComplete,
				      aluminum,
				      "DewarCompleteLogical");



 G4Tubs* MkLn2 = new G4Tubs("MKLn2",
			    fMilchkanne_InnerWallSubtr_InnerRadius,
			    fMilchkanne_InnerWallSubtr_OuterRadius,
			    fMilchkanne_InnerWallSubtr_Height,
			    0. * deg,
			    360. * deg);
 
 MkLn2Logical = new G4LogicalVolume(MkLn2,
				    liquidN2,
				    "MkLn2Logical");
 
 G4VisAttributes* Ln2VisAtt = new G4VisAttributes(blue);
 
 MkLn2Logical -> SetVisAttributes(Ln2VisAtt);


 G4SubtractionSolid* MkVacuum = new G4SubtractionSolid("MkVacuum",
						       MkDewarSubtract,
						       MkDewarInner,
						       0,
						       G4ThreeVector());
 
 

 MkDewarBetweenWallsVacuumLogical =  new G4LogicalVolume(MkVacuum,
							 vacuum,
							 "DewarBetweenWallsVacuumLogical");
 
 G4VisAttributes* VacuumVisAtt = new G4VisAttributes(green);
 
 MkDewarBetweenWallsVacuumLogical -> SetVisAttributes(VacuumVisAtt);



 
}

// ---------------------------------------------------------------------------  

void GEMunichTestStandDB::ConstructREGeDetector(int irege, int icopynumber)
{
  // materials 
  G4Material* vacuum   = G4Material::GetMaterial("Vacuum");
//   G4Material* enrGe    = G4Material::GetMaterial("EnrichedGe");
//   G4Material* aluminum = G4Material::GetMaterial("MetalAluminium"); 
//   G4Material* copper   = G4Material::GetMaterial("MetalCopper"); 
//   G4Material* lead     = G4Material::GetMaterial("MetalLead"); 
//   G4Material* plastic  = G4Material::GetMaterial("PlasticScintillator"); 
//   G4Material* liquidN2 = G4Material::GetMaterial("LiquidNitrogen"); 
//   G4Material* steel    = G4Material::GetMaterial("Steel"); 
//   G4Material* beryllium= G4Material::GetMaterial("MetalBeryllium");


  G4double TubsInnerRadius;
  G4double TubsOuterRadius;
  G4double TubsHeight;
  G4double TubsStartAngle;
  G4double TubsSpanningAngle;

  TubsInnerRadius   = 0.0 * mm; 
  TubsOuterRadius   = fREGeRadius;
  TubsHeight        = 0.5 * fREGeHeight;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;

  G4Tubs* REGeTubs = new G4Tubs("REGeTubs",
                                TubsInnerRadius,
                                TubsOuterRadius,
                                TubsHeight,
                                TubsStartAngle,
                                TubsSpanningAngle);

  fREGeLogical[irege] = new G4LogicalVolume(REGeTubs,
                                     vacuum,
                                    "REGeLogical");

  G4ThreeVector fREGeComponentsPosition;

  fREGeComponentsPosition.setX(0.0*cm);
  fREGeComponentsPosition.setY(0.0*cm);
  fREGeComponentsPosition.setZ(fREGeHeight*0.5-fAluminumHolderThickness
                                              -fAluminumHolderToCrystal
                                              -fCrystalHeight*0.5);
  //  G4VPhysicalVolume* fREGeCrystalActivePhysical = 
                  new G4PVPlacement(0,fREGeComponentsPosition,
                                    fCrystalActiveLogical,
                                    "REGecrystalactive",
                                    fREGeLogical[irege],
                                    false,
                                    icopynumber);

		  //  G4VPhysicalVolume* fREGeCrystalDeadLayerPhysical = 
                  new G4PVPlacement(0,fREGeComponentsPosition,
                                    fCrystalDeadLayerLogical,
                                    "REGecrystaldeadlayer",
                                    fREGeLogical[irege],
                                    false,
                                    icopynumber);

  fREGeComponentsPosition.setX(0.0*cm);
  fREGeComponentsPosition.setY(0.0*cm);
  fREGeComponentsPosition.setZ(fREGeHeight*0.5-fAluminumHolderHeight*0.5);
  // same center position as crystal and crystaldeadlayer
  //G4VPhysicalVolume* fREGeAluminumHolderPhysical =
                  new G4PVPlacement(0,fREGeComponentsPosition,
                                    fAluminumHolderLogical,
                                    "REGeAluminumHolder",
                                    fREGeLogical[irege],
                                    false,
                                    icopynumber);

  fREGeComponentsPosition.setX(0.0*cm);
  fREGeComponentsPosition.setY(0.0*cm);
  fREGeComponentsPosition.setZ(fREGeHeight*0.5-fAluminumHolderThickness*0.5);
  //G4VPhysicalVolume* fREGeBerylliumWindowPhysical =
                  new G4PVPlacement(0,fREGeComponentsPosition,
                                    fBerylliumWindowLogical,
                                    "REGeBerylliumWindow",
                                    fREGeLogical[irege],
                                    false,
                                    icopynumber);


  fREGeComponentsPosition.setX(0.0*cm);
  fREGeComponentsPosition.setY(0.0*cm);
  fREGeComponentsPosition.setZ(0.0-fREGeHeight*0.5+fCopperFingerHeight*0.5);
  //G4VPhysicalVolume* fREGeCopperFingerPhysical =
                  new G4PVPlacement(0,fREGeComponentsPosition,
                                    fCopperFingerLogical,
                                    "REGeCopperFinger",
                                    fREGeLogical[irege],
                                    false,
                                    icopynumber);

}
// ---------------------------------------------------------------------------  
 
void GEMunichTestStandDB::UpdateTestStand()
{
 
}

// ---------------------------------------------------------------------------  

G4ThreeVector GEMunichTestStandDB:: GetDSGHolderSkrewPosition(G4int n)  
{
  
  G4ThreeVector position; 

  G4double r = (fCrystalHolderDSGUnsegmentedRing1OuterRadius + fCrystalHolderDSGUnsegmentedRing1InnerRadius) / 2.0; 
  G4double x = r * cos(n * 2 * 3.1415927 / 3.0 + 3.1415927 / 6.0); 
  G4double y = r * sin(n * 2 * 3.1415927 / 3.0 + 3.1415927 / 6.0); 

  position = G4ThreeVector(x, y, 0.0); 

  return position; 

}
// ---------------------------------------------------------------------------  
 

