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
 * $Id: GEGeometryHolderPhase1Parts.cc,v 1.8 2009-07-29 09:17:40 pandola Exp $
 *
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/*
 *  This class should contain all
 *    - parameters
 *    - logical volumes
 *    - physical volumes
 *  of all parts of the Phase 1 holders *
 */
// End class description
//
/**
 * SPECIAL NOTES:
 *
 */
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: Jens Schubert
 * CONTACT:
 * FIRST SUBMISSION: @START_DATE@
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * xx-04-2008, first version, Jens
 * 30-04-2008, (jens) took out a bug fix which had put extra space between the phase-1 crystal and its CopperContactPlate
 *             the original bug (an overlap) was there due to some strange construction of the phase-1 detector
 * 05-05-2008  (jens) changes to make the arms of the holder stars visible in wired 
 * 08-05-2008  (jens) store one more solid needed by suspension of uppermost crystal in a phase-1 string
 * 02-07-2008  (jens) add 7 more volumes namely 
                      UpperChineseHat, UpperSmall_1_Isolator, UpperSmall_2_SiliconGuider, 
                      UpperSmall_3_Isolator, UpperSmall_4_SiliconSpring, 
                      UpperSmall_5_Isolator, UpperSmall_6_InnerSpecialNut
               all of them are inside the inner part of the 'Upper Star Holder'
 * 09-07-2008  (jens) changes in the names of some solids/logVols to obtained better drawings with root/gdml
 * 29-07-2009  (luciano) do not create the ChineseHat when the parameters of the Polycone are illegal
 *             This allows to re-use the same code and holders for BEGe-like detectors.
 * 20-06-2011  Created CreatePhysicalVolume_NoUpperPTFERing(...) to be used with 
 *             the TeflonEncapsulation in Phase0 to avoid overlaps
 */
// --------------------------------------------------------------------------//
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"




#include "gerdageometry/GEGeometryHolderPhase1Parts.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


G4double GEGeometryHolderPhase1Parts::SmallNumber = 0.00123 * mm;


//--------------------------------------------------------------------------------
GEGeometryHolderPhase1Parts::GEGeometryHolderPhase1Parts( G4double gap) :
  fMaximalAllowedCrystalRadius( -DBL_MAX ),
  fRadialDistance_CrystalSurface_Holder( -DBL_MAX ),
  fTopDistance_CrystalSurface_Holder( -DBL_MAX ),
  fBotDistance_CrystalSurface_Holder( -DBL_MAX ),
  fCrystalRadius( -DBL_MAX ),
  fCrystalInnerRadius( -DBL_MAX ),
  fCrystalHeight( -DBL_MAX ),
  fCrystalID( -DBL_MAX ),
  fStarArm_pDz                   ( -DBL_MAX ),
  fStarArm_pDy1                  ( -DBL_MAX ),
  fStarArm_pDx1                  ( -DBL_MAX ),
  fStarArm_pDx2                  ( -DBL_MAX ),
  fStarArm_pAlp1                 ( -DBL_MAX ),
  fStarArm_pDy2                  ( -DBL_MAX ),
  fStarArm_pDx3                  ( -DBL_MAX ),
  fStarArm_pDx4                  ( -DBL_MAX ),
  fStarArm_pAlp2                 ( -DBL_MAX ),
  fStarArm_pTheta                ( -DBL_MAX ),
  fStarArm_pPhi                  ( -DBL_MAX ),
  fStarArm_meanOfCenterTubeRadius( -DBL_MAX ),

  // solides
  fSolid_LowerStar( 0 ),
  fSolid_LowerStarTube(0),
  fSolid_UpperStar( 0 ),
  fSolid_starArm( 0 ),
  fSolid_VerticalBar( 0 ),
  fSolid_LowerContactDisc( 0 ),
  fSolid_LowerPTFETube( 0 ),
  fSolid_LowerPressurePlate( 0 ),
  fSolid_LowerGrubScrew( 0 ),
  fSolid_UpperSpecialNut( 0 ),
  fSolid_UpperCounterNut( 0 ),
  fSolid_UpperPTFERing( 0 ),

  fSolid_UpperChineseHat              (0),
  fSolid_UpperSmall_1_Isolator        (0),
  fSolid_UpperSmall_2_SiliconGuider   (0),
  fSolid_UpperSmall_3_Isolator        (0),
  fSolid_UpperSmall_4_SiliconSpring   (0),
  fSolid_UpperSmall_5_Isolator        (0),
  fSolid_UpperSmall_6_InnerSpecialNut (0),


  // vis attributes
  fVisAtt_LowerStar( 0 ),
  fVisAtt_UpperStar( 0 ),
  fVisAtt_starArm( 0 ),
  fVisAtt_VerticalBar( 0 ),
  fVisAtt_LowerContactDisc( 0 ),
  fVisAtt_LowerPTFETube( 0 ),
  fVisAtt_LowerPressurePlate( 0 ),
  fVisAtt_LowerGrubScrew( 0 ),
  fVisAtt_UpperSpecialNut( 0 ),
  fVisAtt_UpperCounterNut( 0 ),
  fVisAtt_UpperPTFERing( 0 ),

  fVisAtt_UpperChineseHat              ( 0 ),
  fVisAtt_UpperSmall_1_Isolator        ( 0 ),
  fVisAtt_UpperSmall_2_SiliconGuider   ( 0 ),
  fVisAtt_UpperSmall_3_Isolator        ( 0 ),
  fVisAtt_UpperSmall_4_SiliconSpring   ( 0 ),
  fVisAtt_UpperSmall_5_Isolator        ( 0 ),
  fVisAtt_UpperSmall_6_InnerSpecialNut ( 0 ),



  // materials
  fMaterial_LowerStar( 0 ),
  fMaterial_UpperStar( 0 ),
  fMaterial_starArm( 0 ),
  fMaterial_VerticalBar( 0 ),
  fMaterial_LowerContactDisc( 0 ),
  fMaterial_LowerPTFETube( 0 ),
  fMaterial_LowerPressurePlate( 0 ),
  fMaterial_LowerGrubScrew( 0 ),
  fMaterial_UpperSpecialNut( 0 ),
  fMaterial_UpperCounterNut( 0 ),
  fMaterial_UpperPTFERing( 0 ),

  fMaterial_UpperChineseHat              ( 0 ),
  fMaterial_UpperSmall_1_Isolator        ( 0 ),
  fMaterial_UpperSmall_2_SiliconGuider   ( 0 ),
  fMaterial_UpperSmall_3_Isolator        ( 0 ),
  fMaterial_UpperSmall_4_SiliconSpring   ( 0 ),
  fMaterial_UpperSmall_5_Isolator        ( 0 ),
  fMaterial_UpperSmall_6_InnerSpecialNut ( 0 ),

  // logical volumes
  fLogicalVol_LowerStar( 0 ),
  fLogicalVol_UpperStar( 0 ),
  fLogicalVol_starArm( 0 ),
  fLogicalVol_VerticalBar( 0 ),
  fLogicalVol_LowerContactDisc( 0 ),
  fLogicalVol_LowerPTFETube( 0 ),
  fLogicalVol_LowerPressurePlate( 0 ),
  fLogicalVol_LowerGrubScrew( 0 ),
  fLogicalVol_UpperSpecialNut( 0 ),
  fLogicalVol_UpperCounterNut( 0 ),
  fLogicalVol_UpperPTFERing( 0 ),

  fLogicalVol_UpperChineseHat                ( 0 ),
  fLogicalVol_UpperSmall_1_Isolator          ( 0 ),
  fLogicalVol_UpperSmall_2_SiliconGuider     ( 0 ),
  fLogicalVol_UpperSmall_3_Isolator          ( 0 ),
  fLogicalVol_UpperSmall_4_SiliconSpring     ( 0 ),
  fLogicalVol_UpperSmall_5_Isolator          ( 0 ),
  fLogicalVol_UpperSmall_6_InnerSpecialNut   ( 0 ),
  fCrystalPairGap(gap),
  fIsAPair(false)
{
  
  for (size_t i=0;i<100;i++)
    fNumberOfPhysicalVolumesSets[i] = 0;

    // parameters probably beeing reset before creating solids and volumes
    // --------------------------------------------------------------------
    fMaximalAllowedCrystalRadius = 40.*mm;
    fRadialDistance_CrystalSurface_Holder = 5.*mm;
    fTopDistance_CrystalSurface_Holder = 5.*mm;
    fBotDistance_CrystalSurface_Holder = 5.*mm;
    fCrystalRadius = -100;  // must be reset
    fCrystalInnerRadius = -100;  // must be reset
    fCrystalHeight = -100;  // must be reset

    // parameters for upper star tube
    // ------------------------------
    G4double ZzHldStarUpperTube[8] = { 0   *mm,  1.0*mm, 1.0*mm, 12.*mm, 12.*mm, 17.*mm, 17.*mm, 29.*mm };
    G4double RiHldStarUpperTube[8] = { 3.25*mm, 3.25*mm, 4.5*mm, 4.5*mm, 4.5*mm, 4.5*mm, 4.5*mm, 4.5*mm };
    G4double RoHldStarUpperTube[8] = { 6.5 *mm,  6.5*mm, 6.5*mm, 6.5*mm, 5.0*mm, 5.0*mm, 6.0*mm, 6.0*mm };
    for(int ii=0;ii<8;ii++) {
        fZzHldStarUpperTube[ii] = ZzHldStarUpperTube[ii];
        fRiHldStarUpperTube[ii] = RiHldStarUpperTube[ii];
        fRoHldStarUpperTube[ii] = RoHldStarUpperTube[ii];
    }

    // parameters for lower star tube
    // ------------------------------
    G4double ZzHldStarLowerTube[8] = { 0  *mm, -9.0*mm, -9.0*mm,-12.*mm,-12.0*mm,-16.0*mm,-16.0*mm,-18.0*mm };
    G4double RiHldStarLowerTube[8] = { 5.0*mm,  5.0*mm,  2.5*mm, 2.5*mm,  2.5*mm,  2.5*mm, 2.5 *mm, 2.5*mm };
    G4double RoHldStarLowerTube[8] = { 6.5*mm,  6.5*mm,  6.5*mm, 6.5*mm, 3.75*mm, 3.75*mm, 6.75*mm, 6.75*mm };
    for(int ii=0;ii<8;ii++) {
        fZzHldStarLowerTube[ii] = ZzHldStarLowerTube[ii];
        fRiHldStarLowerTube[ii] = RiHldStarLowerTube[ii];
        fRoHldStarLowerTube[ii] = RoHldStarLowerTube[ii];
    }

    // parameters for the star arm
    // -----------------------------
    //fStarArm_meanOfCenterTubeRadius = 0.5*(RiHldStarUpperTube[2]+RoHldStarUpperTube[2]);
    fStarArm_meanOfCenterTubeRadius = fRoHldStarUpperTube[2];
    fStarArm_pDz    = 0.5*(
        fMaximalAllowedCrystalRadius
        + fRadialDistance_CrystalSurface_Holder
        - fStarArm_meanOfCenterTubeRadius);
    fStarArm_pDy1   = 0.5*2.5*mm;
    fStarArm_pDx1   = 0.5*fabs(fZzHldStarUpperTube[3] - fZzHldStarUpperTube[0]);
    fStarArm_pDx2   = fStarArm_pDx1;
    fStarArm_pAlp1  = 0.0;
    fStarArm_pDy2   = 0.5*1.*mm;
    fStarArm_pDx3   = 0.5*6.*mm;
    fStarArm_pDx4   = fStarArm_pDx3;
    fStarArm_pAlp2  = 0.0;
    fStarArm_pTheta = atan(  (fStarArm_pDx1-fStarArm_pDx3)/(2.*fStarArm_pDz) );
    fStarArm_pPhi   = 0.0;

    // parameters for special nut
    // ----------------------------
    //G4double sn1 = 0.001*mm;
    G4double ZzSpclNut[10] = { 0.*mm   , 3.*mm   , 4.*mm  , 7.*mm , 7.*mm    , 8.*mm   , 8.*mm  , 11.*mm , 11.*mm , 14.*mm };
    G4double RiSpclNut[10] = { 6.*mm   , 6.*mm   , 6.*mm  , 6.*mm  , 4.75*mm , 4.75*mm , 6.5*mm , 6.5*mm , 0.*mm  , 0.*mm  };
    G4double RoSpclNut[10] = { 6.75*mm , 6.75*mm , 7.5*mm , 7.5*mm , 7.5*mm  , 7.5*mm  , 7.5*mm , 7.5*mm , 7.5*mm , 7.5*mm };
    for(int ii=0;ii<10;ii++) {
        fZzSpclNut[ii] = ZzSpclNut[ii];
        fRiSpclNut[ii] = RiSpclNut[ii];
        fRoSpclNut[ii] = RoSpclNut[ii];
    }

    fSpclNut_SubtrAlongY1_Zstart = 5.5*mm;
    fSpclNut_SubtrAlongY1_Width  = 5.*mm;

    fSpclNut_SubtrAlongY2_Zstart = 10.5*mm;
    fSpclNut_SubtrAlongY2_Width  = 8.*mm;

    fSpclNut_SubtrAlongX_deltaZ  = 3.5*mm;
    fSpclNut_SubtrAlongX_deltaY  = 8.*mm;
    fSpclNut_ZPosOfSuspensionHoleTop = 12.*mm;

    fZOverlap_UpperStarTube_SpecialNut = 4.*mm;

    // parameters for counter nut
    // ---------------------------
    fCounterNut_Ri = fRoHldStarUpperTube[7];
    fCounterNut_Ro = fCounterNut_Ri + 1.*mm;
    fCounterNut_h  = 3.*mm;

    // parameters for vertical bars
    // -----------------------------
    fVerticalBar_dx = 3.*mm;
    fVerticalBar_dy = 0.7*mm;

    // parameters for upper PTFE ring 
    // -----------------------------
    fUpperPTFERing_Ro = fCrystalRadius-5.*mm;
    fUpperPTFERing_dR = 5.*mm;
    fUpperPTFERing_Ri = fUpperPTFERing_Ro - fUpperPTFERing_dR;
    fUpperPTFERing_h  = 2.*mm;

    // parameters for lower Contact disc
    // ----------------------------------
    fLowerContactDisc_Ro = 0.5 *  14.*mm;
    fLowerContactDisc_h  = 1.5*mm;

    // parameters for lower PTFE tube 
    // ----------------------------------
    fLowerPTFETube_Ro = fRiHldStarLowerTube[0];
    fLowerPTFETube_Ri = 0.5 *  1.0*mm;
    fLowerPTFETube_h  = 10.0 * mm;

    // parameters for lower pressure plate
    // ----------------------------------
    fLowerPressurePlate_Ro = fLowerPTFETube_Ro ;
    fLowerPressurePlate_h  = 1.7  * mm;

    // parameters for lower grub screw
    // ----------------------------------
    fLowerGrubScrew_Ro =  fRiHldStarLowerTube[7];
    fLowerGrubScrew_h  =  10.*mm;



    // parameters added on June 30th
    // --------------------------------------
    fUpperChineseHat_totalHeight            = 20.5 * mm;
    fUpperChineseHat_CylinderInnerRadius    =  1.5 * mm;
    fUpperChineseHat_CylinderOuterRadius    =  2.5 * mm;
    fUpperChineseHat_ContactConeHeight      =  1.5 * mm;
    fUpperChineseHat_ContactConeDeltaRadius =  5.0 * mm;
    fUpperChineseHat_ContactConeRadius1     = -DBL_MAX*mm;  // depends on R_i of crystal
    fUpperChineseHat_ContactConeRadius2     = -DBL_MAX*mm;  // depends on R_i of crystal

    fUpperSmall_1_Isolator_h                =  0.8 * mm;
    fUpperSmall_1_Isolator_Ri               =  fUpperChineseHat_CylinderOuterRadius;
    fUpperSmall_1_Isolator_Ro               =  fRiHldStarUpperTube[7];

    fUpperSmall_2_SiliconGuider_h           =  9.0 * mm;
    fUpperSmall_2_SiliconGuider_Ri          =  fUpperChineseHat_CylinderOuterRadius;
    fUpperSmall_2_SiliconGuider_Ro          =  3.75 * mm;

    fUpperSmall_3_Isolator_h                =  1.3 * mm;
    fUpperSmall_3_Isolator_Ri               =  fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_3_Isolator_Ro               =  3.0 * mm;

    fUpperSmall_4_SiliconSpring_h           = -DBL_MAX; // dependents on the space left between the two isolators it is sandwitched by
    fUpperSmall_4_SiliconSpring_Ri          = fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_4_SiliconSpring_Ro          = 2.6*mm;

    fUpperSmall_5_Isolator_h                = 2.3 * mm;
    fUpperSmall_5_Isolator_Ri               = fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_5_Isolator_Ro               = fRiHldStarUpperTube[7];

    fUpperSmall_6_InnerSpecialNut_h         = (fZzSpclNut[4]-fZzSpclNut[0]);
    fUpperSmall_6_InnerSpecialNut_Ro        = fRiHldStarUpperTube[7];
    fUpperSmall_6_InnerSpecialNut_Ri        = fUpperSmall_6_InnerSpecialNut_Ro - 2.5*mm;

}

//--------------------------------------------------------------------------------
GEGeometryHolderPhase1Parts::GEGeometryHolderPhase1Parts(const GEGeometryHolderPhase1Parts& holder):
  fCrystalRadius( -DBL_MAX ),
  fCrystalInnerRadius( -DBL_MAX ),
  fCrystalHeight( -DBL_MAX ),
  fCrystalID( -DBL_MAX ),

  // solides
  fSolid_LowerStar( 0 ),
  fSolid_LowerStarTube(0),
  fSolid_UpperStar( 0 ),
  fSolid_starArm( 0 ),
  fSolid_VerticalBar( 0 ),
  fSolid_LowerContactDisc( 0 ),
  fSolid_LowerPTFETube( 0 ),
  fSolid_LowerPressurePlate( 0 ),
  fSolid_LowerGrubScrew( 0 ),
  fSolid_UpperSpecialNut( 0 ),
  fSolid_UpperCounterNut( 0 ),
  fSolid_UpperPTFERing( 0 ),

  fSolid_UpperChineseHat               (0),
  fSolid_UpperSmall_1_Isolator         (0),
  fSolid_UpperSmall_2_SiliconGuider    (0),
  fSolid_UpperSmall_3_Isolator         (0),
  fSolid_UpperSmall_4_SiliconSpring    (0),
  fSolid_UpperSmall_5_Isolator         (0),
  fSolid_UpperSmall_6_InnerSpecialNut  (0),


  // vis attributes
  fVisAtt_LowerStar( 0 ),
  fVisAtt_UpperStar( 0 ),
  fVisAtt_starArm( 0 ),
  fVisAtt_VerticalBar( 0 ),
  fVisAtt_LowerContactDisc( 0 ),
  fVisAtt_LowerPTFETube( 0 ),
  fVisAtt_LowerPressurePlate( 0 ),
  fVisAtt_LowerGrubScrew( 0 ),
  fVisAtt_UpperSpecialNut( 0 ),
  fVisAtt_UpperCounterNut( 0 ),
  fVisAtt_UpperPTFERing( 0 ),

  // materials
  fMaterial_LowerStar( 0 ),
  fMaterial_UpperStar( 0 ),
  fMaterial_starArm( 0 ),
  fMaterial_VerticalBar( 0 ),
  fMaterial_LowerContactDisc( 0 ),
  fMaterial_LowerPTFETube( 0 ),
  fMaterial_LowerPressurePlate( 0 ),
  fMaterial_LowerGrubScrew( 0 ),
  fMaterial_UpperSpecialNut( 0 ),
  fMaterial_UpperCounterNut( 0 ),
  fMaterial_UpperPTFERing( 0 ),

  // logical volumes
  fLogicalVol_LowerStar( 0 ),
  fLogicalVol_UpperStar( 0 ),
  fLogicalVol_starArm( 0 ),
  fLogicalVol_VerticalBar( 0 ),
  fLogicalVol_LowerContactDisc( 0 ),
  fLogicalVol_LowerPTFETube( 0 ),
  fLogicalVol_LowerPressurePlate( 0 ),
  fLogicalVol_LowerGrubScrew( 0 ),
  fLogicalVol_UpperSpecialNut( 0 ),
  fLogicalVol_UpperCounterNut( 0 ),
  fLogicalVol_UpperPTFERing( 0 ),

  fLogicalVol_UpperChineseHat             (0),
  fLogicalVol_UpperSmall_1_Isolator       (0),
  fLogicalVol_UpperSmall_2_SiliconGuider  (0),
  fLogicalVol_UpperSmall_3_Isolator       (0),
  fLogicalVol_UpperSmall_4_SiliconSpring  (0),
  fLogicalVol_UpperSmall_5_Isolator       (0),
  fLogicalVol_UpperSmall_6_InnerSpecialNut(0),
  fCrystalPairGap( holder.fCrystalPairGap),
  fIsAPair(holder.fIsAPair)
{

  
  for (size_t i=0;i<100;i++)
    fNumberOfPhysicalVolumesSets[i] = 0;

    // parameters probably beeing reset before creating solids and volumes
    // --------------------------------------------------------------------
    fMaximalAllowedCrystalRadius            = holder.fMaximalAllowedCrystalRadius    ;
    fRadialDistance_CrystalSurface_Holder   = holder.fRadialDistance_CrystalSurface_Holder;
    fTopDistance_CrystalSurface_Holder      = holder.fTopDistance_CrystalSurface_Holder;
    fBotDistance_CrystalSurface_Holder      = holder.fBotDistance_CrystalSurface_Holder;

    // parameters for upper star tube
    // ------------------------------
    for(int ii=0;ii<8;ii++) {
        fZzHldStarUpperTube[ii] = holder.fZzHldStarUpperTube[ii];
        fRiHldStarUpperTube[ii] = holder.fRiHldStarUpperTube[ii];
        fRoHldStarUpperTube[ii] = holder.fRoHldStarUpperTube[ii];
    }

    // parameters for lower star tube
    // ------------------------------
    for(int ii=0;ii<8;ii++) {
        fZzHldStarLowerTube[ii] = holder.fZzHldStarLowerTube[ii];
        fRiHldStarLowerTube[ii] = holder.fRiHldStarLowerTube[ii];
        fRoHldStarLowerTube[ii] = holder.fRoHldStarLowerTube[ii];
    }

    // star arm parameters
    // ----------------------
    fStarArm_pDz                    = holder.fStarArm_pDz                   ;
    fStarArm_pDy1                   = holder.fStarArm_pDy1                  ;
    fStarArm_pDx1                   = holder.fStarArm_pDx1                  ;
    fStarArm_pDx2                   = holder.fStarArm_pDx2                  ;
    fStarArm_pAlp1                  = holder.fStarArm_pAlp1                 ;
    fStarArm_pDy2                   = holder.fStarArm_pDy2                  ;
    fStarArm_pDx3                   = holder.fStarArm_pDx3                  ;
    fStarArm_pDx4                   = holder.fStarArm_pDx4                  ;
    fStarArm_pAlp2                  = holder.fStarArm_pAlp2                 ;
    fStarArm_pTheta                 = holder.fStarArm_pTheta                ;
    fStarArm_pPhi                   = holder.fStarArm_pPhi                  ;
    fStarArm_meanOfCenterTubeRadius = holder.fStarArm_meanOfCenterTubeRadius;




    // parameters for special nut
    // ----------------------------
    for(int ii=0;ii<10;ii++) {
        fZzSpclNut[ii] = holder.fZzSpclNut[ii];
        fRiSpclNut[ii] = holder.fRiSpclNut[ii];
        fRoSpclNut[ii] = holder.fRoSpclNut[ii];
    }

    fSpclNut_SubtrAlongY1_Zstart       = holder.fSpclNut_SubtrAlongY1_Zstart       ;
    fSpclNut_SubtrAlongY1_Width        = holder.fSpclNut_SubtrAlongY1_Width        ;

    fSpclNut_SubtrAlongY2_Zstart       = holder.fSpclNut_SubtrAlongY2_Zstart       ;
    fSpclNut_SubtrAlongY2_Width        = holder.fSpclNut_SubtrAlongY2_Width        ;

    fSpclNut_SubtrAlongX_deltaZ        = holder.fSpclNut_SubtrAlongX_deltaZ        ;
    fSpclNut_SubtrAlongX_deltaY        = holder.fSpclNut_SubtrAlongX_deltaY        ;
    fSpclNut_ZPosOfSuspensionHoleTop   = holder.fSpclNut_ZPosOfSuspensionHoleTop   ;

    fZOverlap_UpperStarTube_SpecialNut = holder.fZOverlap_UpperStarTube_SpecialNut ;


    // parameters for counter nut
    // ---------------------------
    fCounterNut_Ri = holder.fRoHldStarUpperTube[7];
    fCounterNut_Ro = fCounterNut_Ri + 1.*mm;
    fCounterNut_h  = holder.fCounterNut_h;

    // parameters for vertical bars
    // -----------------------------
    fVerticalBar_dx = holder.fVerticalBar_dx;
    fVerticalBar_dy = holder.fVerticalBar_dy;

    // parameters for upper PTFE ring 
    // -----------------------------
    fUpperPTFERing_Ro = holder.fUpperPTFERing_Ro;
    fUpperPTFERing_dR = holder.fUpperPTFERing_dR;
    fUpperPTFERing_Ri = holder.fUpperPTFERing_Ri;
    fUpperPTFERing_h  = holder.fUpperPTFERing_h;

    // parameters for lower Contact disc
    // ----------------------------------
    fLowerContactDisc_Ro = holder.fLowerContactDisc_Ro;
    fLowerContactDisc_h  = holder.fLowerContactDisc_h ;

    // parameters for lower PTFE tube 
    // ----------------------------------
    fLowerPTFETube_Ro = holder.fLowerPTFETube_Ro;
    fLowerPTFETube_Ri = holder.fLowerPTFETube_Ri;
    fLowerPTFETube_h  = holder.fLowerPTFETube_h ;

    // parameters for lower pressure plate
    // ----------------------------------
    fLowerPressurePlate_Ro = holder.fLowerPressurePlate_Ro ;
    fLowerPressurePlate_h  = holder.fLowerPressurePlate_h ;

    // parameters for lower grub screw
    // ----------------------------------
    fLowerGrubScrew_Ro =  holder.fLowerGrubScrew_Ro;
    fLowerGrubScrew_h  =  holder.fLowerGrubScrew_h ;


    // parameters added on June 30th
    // -----------------------------------

    fUpperChineseHat_totalHeight            =   holder.fUpperChineseHat_totalHeight          ;
    fUpperChineseHat_CylinderInnerRadius    =   holder.fUpperChineseHat_CylinderInnerRadius  ;
    fUpperChineseHat_CylinderOuterRadius    =   holder.fUpperChineseHat_CylinderOuterRadius  ;
    fUpperChineseHat_ContactConeHeight      =   holder.fUpperChineseHat_ContactConeHeight    ;
    fUpperChineseHat_ContactConeDeltaRadius =   holder.fUpperChineseHat_ContactConeDeltaRadius;
    fUpperChineseHat_ContactConeRadius1     =   holder.fUpperChineseHat_ContactConeRadius1   ;  // depends on R_i of crystal
    fUpperChineseHat_ContactConeRadius2     =   holder.fUpperChineseHat_ContactConeRadius2   ;  // depends on R_i of crystal

    fUpperSmall_1_Isolator_h                =   holder.fUpperSmall_1_Isolator_h              ;  //   0.8 * mm;
    fUpperSmall_1_Isolator_Ri               =   holder.fUpperSmall_1_Isolator_Ri             ;  //   fUpperChineseHat_CylinderOuterRadius;
    fUpperSmall_1_Isolator_Ro               =   holder.fUpperSmall_1_Isolator_Ro             ;  //   fRiHldStarUpperTube[7];
                                                                                                //  
    fUpperSmall_2_SiliconGuider_h           =   holder.fUpperSmall_2_SiliconGuider_h         ;  //   9.0 * mm;
    fUpperSmall_2_SiliconGuider_Ri          =   holder.fUpperSmall_2_SiliconGuider_Ri        ;  //   fUpperChineseHat_CylinderOuterRadius;
    fUpperSmall_2_SiliconGuider_Ro          =   holder.fUpperSmall_2_SiliconGuider_Ro        ;  //   3.75 * mm;
                                                                                                //  
    fUpperSmall_3_Isolator_h                =   holder.fUpperSmall_3_Isolator_h              ;  //   1.3 * mm;
    fUpperSmall_3_Isolator_Ri               =   holder.fUpperSmall_3_Isolator_Ri             ;  //   fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_3_Isolator_Ro               =   holder.fUpperSmall_3_Isolator_Ro             ;  //   3.0 * mm;
                                                                                                //  
    fUpperSmall_4_SiliconSpring_h           =   holder.fUpperSmall_4_SiliconSpring_h         ;  //  -9999*mm; // dependents on the space left between the two isolators it is sandwitched by
    fUpperSmall_4_SiliconSpring_Ri          =   holder.fUpperSmall_4_SiliconSpring_Ri        ;  //  fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_4_SiliconSpring_Ro          =   holder.fUpperSmall_4_SiliconSpring_Ro        ;  //  2.6*mm
                                                                                                //  
    fUpperSmall_5_Isolator_h                =   holder.fUpperSmall_5_Isolator_h              ;  //  2.3 * mm;
    fUpperSmall_5_Isolator_Ri               =   holder.fUpperSmall_5_Isolator_Ri             ;  //  fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_5_Isolator_Ro               =   holder.fUpperSmall_5_Isolator_Ro             ;  //  fRiHldStarUpperTube[7];
                                                                                                //  
    fUpperSmall_6_InnerSpecialNut_h         =   holder.fUpperSmall_6_InnerSpecialNut_h       ;  //  (fZzSpclNut[4]-fZzSpclNut[0]);
    fUpperSmall_6_InnerSpecialNut_Ro        =   holder.fUpperSmall_6_InnerSpecialNut_Ro      ;  //  fRiHldStarUpperTube[7];
    fUpperSmall_6_InnerSpecialNut_Ri        =   fUpperSmall_6_InnerSpecialNut_Ro - 2.5*mm;


    ResetDependentParameters();
}

//--------------------------------------------------------------------------------

GEGeometryHolderPhase1Parts::~GEGeometryHolderPhase1Parts()
{

    delete  fLogicalVol_LowerStar;
    delete  fLogicalVol_UpperStar;
    delete  fLogicalVol_starArm;
    delete  fLogicalVol_VerticalBar;
    delete  fLogicalVol_LowerContactDisc;
    delete  fLogicalVol_LowerPTFETube;
    delete  fLogicalVol_LowerPressurePlate;
    delete  fLogicalVol_LowerGrubScrew;
    delete  fLogicalVol_UpperSpecialNut;
    delete  fLogicalVol_UpperCounterNut;
    delete  fLogicalVol_UpperPTFERing;
    
    delete  fLogicalVol_UpperChineseHat;
    delete  fLogicalVol_UpperSmall_1_Isolator;
    delete  fLogicalVol_UpperSmall_2_SiliconGuider;
    delete  fLogicalVol_UpperSmall_3_Isolator;
    delete  fLogicalVol_UpperSmall_4_SiliconSpring;
    delete  fLogicalVol_UpperSmall_5_Isolator;
    delete  fLogicalVol_UpperSmall_6_InnerSpecialNut;
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::ResetDependentParameters()
{
    // The order of the dependencies is given in the default constructor


  if( fabs(fRoHldStarUpperTube[2]-fRoHldStarLowerTube[2]) >1.0e-8*mm ) {
        MGLog(error) << "If fRoHldStarUpperTube[2] and fRoHldStarLowerTube[2] are different"<<endlog;
        MGLog(error) << "then one needs to implement a StarArmUpper_solid and a StarArmLower_solid instead of one common StarArm_solid."<<endlog;
        MGLog(error) << "In this case, seperate StarArm-Parameters are needed too."<< endlog;
        abort();
    }

    // parameters for the star arm
    // -----------------------------
    //fStarArm_meanOfCenterTubeRadius = 0.5*(fRiHldStarUpperTube[2]+fRoHldStarUpperTube[2]);
    fStarArm_meanOfCenterTubeRadius = fRoHldStarUpperTube[2];
    fStarArm_pDz    = 0.5*(
        fMaximalAllowedCrystalRadius
        + fRadialDistance_CrystalSurface_Holder
        - fStarArm_meanOfCenterTubeRadius);
    fStarArm_pTheta = atan(  (fStarArm_pDx1-fStarArm_pDx3)/(2.*fStarArm_pDz) );


    // parameters of counter nut
    // --------------------------
    fCounterNut_Ri = fRoHldStarUpperTube[7];
    fCounterNut_Ro = fCounterNut_Ri + 1.*mm;

    // parameters for upper PTFE ring 
    // -----------------------------
    fUpperPTFERing_Ro = fCrystalRadius-5.*mm;
    fUpperPTFERing_dR = 5.*mm;
    fUpperPTFERing_Ri = fUpperPTFERing_Ro - fUpperPTFERing_dR;
    fUpperPTFERing_h  = 2.*mm;


    // parameters for lower PTFE tube 
    // ----------------------------------
    fLowerPTFETube_Ro = fRiHldStarLowerTube[0];

    // parameters for lower pressure plate
    // ----------------------------------
    fLowerPressurePlate_Ro = fLowerPTFETube_Ro ;

    // parameters for lower grub screw
    // ----------------------------------
    fLowerGrubScrew_Ro =  fRiHldStarLowerTube[7];



    //  parameters of parts inside the upper star holder
    // ------------------------------------------------
    fUpperChineseHat_ContactConeRadius1     =   fCrystalInnerRadius - 0.5 * fUpperChineseHat_ContactConeDeltaRadius;
    fUpperChineseHat_ContactConeRadius2     =   fCrystalInnerRadius + 0.5 * fUpperChineseHat_ContactConeDeltaRadius;

    fUpperSmall_1_Isolator_Ri               =   fUpperChineseHat_CylinderOuterRadius;
    fUpperSmall_1_Isolator_Ro               =   fRiHldStarUpperTube[7];

    fUpperSmall_2_SiliconGuider_Ri          =   fUpperChineseHat_CylinderOuterRadius;

    fUpperSmall_3_Isolator_Ri               =   fUpperChineseHat_CylinderInnerRadius;

    fUpperSmall_4_SiliconSpring_Ri          =   fUpperChineseHat_CylinderInnerRadius;

    fUpperSmall_5_Isolator_Ri               =   fUpperChineseHat_CylinderInnerRadius;
    fUpperSmall_5_Isolator_Ro               =   fRiHldStarUpperTube[7];

    fUpperSmall_6_InnerSpecialNut_h         =   (fZzSpclNut[4]-fZzSpclNut[0]);
    fUpperSmall_6_InnerSpecialNut_Ro        =   fRiHldStarUpperTube[7];
    fUpperSmall_6_InnerSpecialNut_Ri        =   fUpperSmall_6_InnerSpecialNut_Ro - 2.5*mm;

    fUpperSmall_4_SiliconSpring_h           =   Get_UpperSmall4SiliconSpring_Height(); // dependents on the space left between the two isolators it is sandwitched by
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids()
{
    MGLog(debugging) << "   Starting to create solids"<<  endlog;
    CreateSolids_UpperStar();
    CreateSolids_LowerStar();
    CreateSolids_VerticalBar();

    CreateSolids_UpperSpecialNut();
    CreateSolids_UpperCounterNut();
    CreateSolids_UpperPTFERing();

    CreateSolids_LowerContactDisc();
    CreateSolids_LowerPTFETube();
    CreateSolids_LowerPressurePlate();
    CreateSolids_LowerGrubScrew();

    CreateSolids_UpperChineseHat();
    CreateSolids_UpperSmall_1_Isolator();
    CreateSolids_UpperSmall_2_SiliconGuider();
    CreateSolids_UpperSmall_3_Isolator();
    CreateSolids_UpperSmall_4_SiliconSpring();
    CreateSolids_UpperSmall_5_Isolator();
    CreateSolids_UpperSmall_6_InnerSpecialNut();

    MGLog(debugging) << "   Ending to create solids"<<  endlog;
}

//--------------------------------------------------------------------------------
void 
GEGeometryHolderPhase1Parts::CreateSolids_UpperChineseHat()
{
    G4double tmp_z [4] = { 0.0   , 
                           fUpperChineseHat_ContactConeHeight , 
                           fUpperChineseHat_ContactConeHeight , 
                           fUpperChineseHat_totalHeight };
    G4double tmp_Ri[4] = { fUpperChineseHat_CylinderInnerRadius, 
                           fUpperChineseHat_CylinderInnerRadius,
                           fUpperChineseHat_CylinderInnerRadius,
                           fUpperChineseHat_CylinderInnerRadius };
    G4double tmp_Ro[4] = { fUpperChineseHat_ContactConeRadius1, 
                           fUpperChineseHat_ContactConeRadius2,
                           fUpperChineseHat_CylinderOuterRadius,
                           fUpperChineseHat_CylinderOuterRadius  };
    
    char solidname[100];
    sprintf(solidname,"solid1_HolderStar_UpperChineseHat_%d",fCrystalID);
    if (tmp_Ri[0] > 0 && tmp_Ri[1] > 0 && tmp_Ri[2] > 0 && tmp_Ri[3] > 0 && 
	tmp_Ro[0] > 0 && tmp_Ro[1] > 0 && tmp_Ro[2] > 0 && tmp_Ro[3])
      {    
	fSolid_UpperChineseHat  = new G4Polycone(solidname,
						 0.0*deg, 360.0*deg,
						 4,
						 tmp_z, tmp_Ri, tmp_Ro);
      }
    else
      {
	MGLog(warning) << "Not creating solids for Chinese Hat (not enough space in the borehole)" << 
	  endlog;
      }
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_1_Isolator()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_1_Isolator_%d",fCrystalID);
    fSolid_UpperSmall_1_Isolator = new G4Tubs(solidname,
                              fUpperSmall_1_Isolator_Ri,
                              fUpperSmall_1_Isolator_Ro,
                              0.5*fUpperSmall_1_Isolator_h,
                              0.0 * deg, 360.0 * deg);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_2_SiliconGuider()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_2_SiliconGuider_%d",fCrystalID);
    fSolid_UpperSmall_2_SiliconGuider = new G4Tubs(solidname,
                              fUpperSmall_2_SiliconGuider_Ri,
                              fUpperSmall_2_SiliconGuider_Ro,
                              0.5*fUpperSmall_2_SiliconGuider_h,
                              0.0 * deg, 360.0 * deg);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_3_Isolator()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_3_Isolator_%d",fCrystalID);
    fSolid_UpperSmall_3_Isolator = new G4Tubs(solidname,
                              fUpperSmall_3_Isolator_Ri,
                              fUpperSmall_3_Isolator_Ro,
                              0.5*fUpperSmall_3_Isolator_h,
                              0.0 * deg, 360.0 * deg);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_4_SiliconSpring()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_4_SiliconSpring_%d",fCrystalID);
    fSolid_UpperSmall_4_SiliconSpring = new G4Tubs(solidname,
                              fUpperSmall_4_SiliconSpring_Ri,
                              fUpperSmall_4_SiliconSpring_Ro,
                              0.5*fUpperSmall_4_SiliconSpring_h,
                              0.0 * deg, 360.0 * deg);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_5_Isolator()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_5_Isolator_%d",fCrystalID);
    fSolid_UpperSmall_5_Isolator = new G4Tubs(solidname,
                              fUpperSmall_5_Isolator_Ri,
                              fUpperSmall_5_Isolator_Ro,
                              0.5*fUpperSmall_5_Isolator_h,
                              0.0 * deg, 360.0 * deg);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSmall_6_InnerSpecialNut()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperSmall_6_InnerSpecialNut_%d",fCrystalID);
    fSolid_UpperSmall_6_InnerSpecialNut = new G4Tubs(solidname,
                              fUpperSmall_6_InnerSpecialNut_Ri,
                              fUpperSmall_6_InnerSpecialNut_Ro,
                              0.5*fUpperSmall_6_InnerSpecialNut_h,
                              0.0 * deg, 360.0 * deg);
}



//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_LowerGrubScrew()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_LowerGrubScrew_%d",fCrystalID);
    fSolid_LowerGrubScrew = new G4Tubs(solidname,
                                       0.0, fLowerGrubScrew_Ro,
                                       0.5*fLowerGrubScrew_h,
                                       TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_LowerPressurePlate()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_LowerPressurePlate_%d",fCrystalID);
    fSolid_LowerPressurePlate = new G4Tubs(solidname,
                                      0.0, fLowerPressurePlate_Ro,
                                      0.5*fLowerPressurePlate_h,
                                      TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_LowerContactDisc()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_LowerContactDisc_%d",fCrystalID);
    fSolid_LowerContactDisc = new G4Tubs(solidname,
                                      0.0 , fLowerContactDisc_Ro,
                                      0.5*fLowerContactDisc_h,
                                      TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_LowerPTFETube()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_LowerPTFETube_%d",fCrystalID);
    fSolid_LowerPTFETube = new G4Tubs(solidname,
                                      fLowerPTFETube_Ri, fLowerPTFETube_Ro,
                                      0.5*fLowerPTFETube_h,
                                      TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperPTFERing()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperPTFERing_%d",fCrystalID);
    fSolid_UpperPTFERing = new G4Tubs(solidname,
                                      fUpperPTFERing_Ri, fUpperPTFERing_Ro,
                                      0.5*fUpperPTFERing_h,
                                      TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_VerticalBar()
{
    char solidname[100];
    sprintf(solidname,"solid_HolderStar_VerticalBar_%d",fCrystalID);
    G4double aGap = (fIsAPair) ? fCrystalPairGap : 0.;
    fSolid_VerticalBar = new G4Box(solidname, 
				   0.5 * fVerticalBar_dx,
				   0.5 * fVerticalBar_dy,
				   0.5 * ( fCrystalHeight //this is the sum of the two heights, for pair
					   +fTopDistance_CrystalSurface_Holder 
					   +fBotDistance_CrystalSurface_Holder
					   +aGap
					   +4.0*fStarArm_pDx3) );
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperCounterNut()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;

    char solidname[100];
    sprintf(solidname,"solid_HolderStar_UpperCounterNut_%d",fCrystalID);
    fSolid_UpperCounterNut = new G4Tubs(solidname,
                                        fCounterNut_Ri, fCounterNut_Ro,
                                        0.5*fCounterNut_h,
                                        TubsStartAngle, TubsSpanningAngle);
    
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperSpecialNut()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;
    G4double xh,yh,zh;
    G4ThreeVector myvec;

    char solidname[100];
    sprintf(solidname,"solid1_HolderStar_UpperSpecialNut_%d",fCrystalID);
    G4Polycone* solid1_HolderStar_specialNut  = new G4Polycone(solidname,
                                                               TubsStartAngle, TubsSpanningAngle,
                                                               10,
                                                               fZzSpclNut, fRiSpclNut, fRoSpclNut);

    // subtract a box
    sprintf(solidname,"solid_HolderStar_UpperSpecialNut_boxAlongY1_%d",fCrystalID);
    xh = 0.5 * fSpclNut_SubtrAlongY1_Width;
    yh = 1.01 * fRoSpclNut[9] ;
    zh = 1.01 * 0.5 * (fZzSpclNut[9]-fSpclNut_SubtrAlongY1_Zstart);
    G4Box* boxAlongY1 = new G4Box(solidname, xh, yh, zh);

    myvec.setX(0.*mm);
    myvec.setY(0.*mm);
    myvec.setZ(fSpclNut_SubtrAlongY1_Zstart+zh);
    sprintf(solidname,"solid2_HolderStar_UpperSpecialNut_%d",fCrystalID);
    G4SubtractionSolid* solid2_HolderStar_specialNut = new G4SubtractionSolid(solidname,
                                                                              solid1_HolderStar_specialNut,
                                                                              boxAlongY1,
                                                                              0,
                                                                              myvec);

    // subtract another box
    sprintf(solidname,"solid_HolderStar_UpperSpecialNut_boxAlongY2_%d",fCrystalID);
    xh = 0.5 * fSpclNut_SubtrAlongY2_Width;
    yh = 1.03 * fRoSpclNut[9] ;
    zh = 1.03 * 0.5 * (fZzSpclNut[9]-fSpclNut_SubtrAlongY2_Zstart);
    G4Box* boxAlongY2 = new G4Box(solidname, xh, yh, zh);

    myvec.setX(0.*mm);
    myvec.setY(0.*mm);
    myvec.setZ(fSpclNut_SubtrAlongY2_Zstart+zh);
    sprintf(solidname,"solid3_HolderStar_UpperSpecialNut_%d",fCrystalID);
    G4SubtractionSolid* solid3_HolderStar_specialNut = new G4SubtractionSolid(solidname,
                                                                              solid2_HolderStar_specialNut,
                                                                              boxAlongY2,
                                                                              0,
                                                                              myvec);
    // subtract a third box
    sprintf(solidname,"solid_HolderStar_UpperSpecialNut_boxAlongX_%d",fCrystalID);
    xh = 1.05 * fRoSpclNut[9] ;
    yh = 0.5 * fSpclNut_SubtrAlongX_deltaY;
    zh = 0.5 * fSpclNut_SubtrAlongX_deltaZ;
    G4Box* boxAlongX = new G4Box(solidname, xh, yh, zh);

    myvec.setX(0.*mm);
    myvec.setY(0.*mm);
    myvec.setZ(fSpclNut_ZPosOfSuspensionHoleTop-zh);
    sprintf(solidname,"solid4_HolderStar_UpperSpecialNut_%d",fCrystalID);
    G4SubtractionSolid* solid4_HolderStar_specialNut = new G4SubtractionSolid(solidname,
                                                                              solid3_HolderStar_specialNut,
                                                                              boxAlongX,
                                                                              0,
                                                                              myvec);

    fSolid_UpperSpecialNut = solid4_HolderStar_specialNut;

}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_LowerStar()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;

    char solidname[100];
    sprintf(solidname,"solid1_HolderStarLowerTube_%d",fCrystalID);
    G4Polycone* solid1_HolderStarLowerTube  = new G4Polycone(solidname,
                                                             TubsStartAngle, TubsSpanningAngle,
                                                             8,
                                                             fZzHldStarLowerTube, fRiHldStarLowerTube, fRoHldStarLowerTube);

    // subtract boxes at lower end
    sprintf(solidname,"solid_HolderStarLowerTube_SubtrBox_%d",fCrystalID);
    G4Box* solid_HolderStarLowerTube_SubtrBox = new G4Box(solidname,
                                                          fRoHldStarLowerTube[7],
                                                          fabs(fRoHldStarLowerTube[5]-fRoHldStarLowerTube[6])/2.,
                                                          1.001*fabs(fZzHldStarLowerTube[6]-fZzHldStarLowerTube[7])/2.);
    G4ThreeVector my3vec1;
    my3vec1.setX(0.0);
    my3vec1.setY(fRoHldStarLowerTube[7] - 0.5*fabs(fRoHldStarLowerTube[5]-fRoHldStarLowerTube[6]) );
    my3vec1.setZ(fZzHldStarLowerTube[7] + 0.5*(fZzHldStarLowerTube[6]-fZzHldStarLowerTube[7]) );
    sprintf(solidname,"solid2_HolderStarLowerTube_%d",fCrystalID);
    G4SubtractionSolid* solid2_HolderStarLowerTube = new G4SubtractionSolid(solidname,
                                                                            solid1_HolderStarLowerTube,
                                                                            solid_HolderStarLowerTube_SubtrBox,
                                                                            0, 
                                                                            my3vec1 ); 
    my3vec1.setY( -(my3vec1.getY()) );
    sprintf(solidname,"solid3_HolderStarLowerTube_%d",fCrystalID);
    G4SubtractionSolid* solid3_HolderStarLowerTube = new G4SubtractionSolid(solidname,
                                                                            solid2_HolderStarLowerTube,
                                                                            solid_HolderStarLowerTube_SubtrBox,
                                                                            0, 
                                                                            my3vec1 ); 
    fSolid_LowerStarTube = solid3_HolderStarLowerTube;

     // add arms
    CreateSolids_StarArm();
    G4ThreeVector ArmTranslation;
    G4double      ArmAngle;
    const G4RotationMatrix mUnit; 
    G4RotationMatrix ArmRot; 
    G4UnionSolid* solid_HolderStarLower_WithArms[3];
    char solidname_HolderStarLower_WithArms[100];
     for (G4int ii=0;ii<3;ii++) {
        ArmAngle = ii*120.*deg;
        ArmTranslation.setX((fStarArm_pDz+fStarArm_meanOfCenterTubeRadius + 0.1*mm)*std::cos(ArmAngle));
        ArmTranslation.setY((fStarArm_pDz+fStarArm_meanOfCenterTubeRadius + 0.1*mm)*std::sin(ArmAngle));
        ArmTranslation.setZ( -0.5*(fStarArm_pDx1+fStarArm_pDx3)  -  0.1*((G4double)(ii+1))*SmallNumber );
        ArmRot = mUnit;
        ArmRot.rotateY(-90.*deg);
        ArmRot.rotateZ(180.*deg + ArmAngle);
        sprintf(solidname_HolderStarLower_WithArms,"solid%d_HolderStarLower_%d",ii,fCrystalID);
        if(ii==0)
            solid_HolderStarLower_WithArms[ii] = new G4UnionSolid(solidname_HolderStarLower_WithArms,
                                                                  solid3_HolderStarLowerTube,
                                                                  fSolid_starArm,
                                                                  G4Transform3D(ArmRot,ArmTranslation));
        else 
            solid_HolderStarLower_WithArms[ii] = new G4UnionSolid(solidname_HolderStarLower_WithArms,
                                                                  solid_HolderStarLower_WithArms[ii-1],
                                                                  fSolid_starArm,
                                                                  G4Transform3D(ArmRot,ArmTranslation));
    }
     fSolid_LowerStar = solid_HolderStarLower_WithArms[2];
}


//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_UpperStar()
{
    G4double TubsStartAngle =  0;
    G4double TubsSpanningAngle = 360 * deg;

    char solidname_Holder[100];
    sprintf(solidname_Holder,"baseSolid_HolderStarUpper_Base_%d",fCrystalID);
    G4Polycone* solid_HolderStarUpperTube  = new G4Polycone(solidname_Holder,
                                                            TubsStartAngle, TubsSpanningAngle,
                                                            8,
                                                            fZzHldStarUpperTube, fRiHldStarUpperTube, fRoHldStarUpperTube);
    // add arms
    CreateSolids_StarArm();
    G4ThreeVector ArmTranslation;
    G4double      ArmAngle;
    const G4RotationMatrix mUnit; 
    G4RotationMatrix ArmRot; 
    G4UnionSolid* solid_HolderStarUpper_WithArms[3];
    char solidname_HolderStarUpper_WithArms[100];
    for (G4int ii=0;ii<3;ii++) {
        ArmAngle = ii*120.*deg;
        ArmTranslation.setX((fStarArm_pDz+fStarArm_meanOfCenterTubeRadius+0.1*mm)*std::cos(ArmAngle));
        ArmTranslation.setY((fStarArm_pDz+fStarArm_meanOfCenterTubeRadius+0.1*mm)*std::sin(ArmAngle));
        ArmTranslation.setZ( 0.5*(fStarArm_pDx1+fStarArm_pDx3) + 0.1*((G4double)(ii+1))*SmallNumber );
        //ArmTranslation.setZ( 0.5*(fStarArm_pDx1+fStarArm_pDx3) + 0.1*((G4double)(ii))*1.*mm );
        ArmRot = mUnit;
        ArmRot.rotateY(90.*deg);
        ArmRot.rotateZ(ArmAngle);
        sprintf(solidname_HolderStarUpper_WithArms,"unionSolid%d_HolderStarUpper_%d",ii,fCrystalID);
        if(ii==0)
            solid_HolderStarUpper_WithArms[ii] = new G4UnionSolid(solidname_HolderStarUpper_WithArms,
                                                                  solid_HolderStarUpperTube,
                                                                  fSolid_starArm,
                                                                  G4Transform3D(ArmRot,ArmTranslation));
        else
            solid_HolderStarUpper_WithArms[ii] = new G4UnionSolid(solidname_HolderStarUpper_WithArms,
                                                                  solid_HolderStarUpper_WithArms[ii-1],
                                                                  fSolid_starArm,
                                                                  G4Transform3D(ArmRot,ArmTranslation));
    }
    fSolid_UpperStar = solid_HolderStarUpper_WithArms[2];

}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateSolids_StarArm()
{
    char tmpName[100];
    sprintf(tmpName,"HolderPhaseI_ArmOfStar_Solid1_%d",fCrystalID);
    if(fSolid_starArm==0) {
        G4Trap* HolderPhaseI_ArmOfStar_Solid1 = new G4Trap(tmpName,
                                                           fStarArm_pDz , fStarArm_pTheta, fStarArm_pPhi, 
                                                           fStarArm_pDy1, fStarArm_pDx1,   fStarArm_pDx2, fStarArm_pAlp1,
                                                           fStarArm_pDy2, fStarArm_pDx3,   fStarArm_pDx4, fStarArm_pAlp2 );
        fSolid_starArm = HolderPhaseI_ArmOfStar_Solid1;        
    }
}
    
//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes()
{
    MGLog(debugging) << "   Starting to create logical volumes"<<  endlog;
    CreateLogicalVolumes_LowerStar();
    CreateLogicalVolumes_UpperStar();
    CreateLogicalVolumes_VerticalBar();

    CreateLogicalVolumes_UpperSpecialNut ();
    CreateLogicalVolumes_UpperCounterNut();
    CreateLogicalVolumes_UpperPTFERing();

    CreateLogicalVolumes_LowerContactDisc();
    CreateLogicalVolumes_LowerPTFETube();
    CreateLogicalVolumes_LowerPressurePlate();
    CreateLogicalVolumes_LowerGrubScrew();

    CreateLogicalVolumes_UpperChineseHat();
    CreateLogicalVolumes_UpperSmall_1_Isolator();
    CreateLogicalVolumes_UpperSmall_2_SiliconGuider();
    CreateLogicalVolumes_UpperSmall_3_Isolator();
    CreateLogicalVolumes_UpperSmall_4_SiliconSpring();
    CreateLogicalVolumes_UpperSmall_5_Isolator();
    CreateLogicalVolumes_UpperSmall_6_InnerSpecialNut();

    MGLog(debugging) << "   Ending to create logical volumes"<<  endlog;
}


//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperChineseHat()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperChineseHat_%d_Logical",fCrystalID);
    if (fSolid_UpperChineseHat)
      {
	fLogicalVol_UpperChineseHat = new G4LogicalVolume( fSolid_UpperChineseHat ,
							   fMaterial_UpperChineseHat ,
							   tmpname); 
	fLogicalVol_UpperChineseHat -> SetVisAttributes(fVisAtt_UpperChineseHat);
      }
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_1_Isolator()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_1_Isolator_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_1_Isolator = new G4LogicalVolume( fSolid_UpperSmall_1_Isolator ,
                                             fMaterial_UpperSmall_1_Isolator ,
                                             tmpname); 
    fLogicalVol_UpperSmall_1_Isolator -> SetVisAttributes(fVisAtt_UpperSmall_1_Isolator);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_2_SiliconGuider()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_2_SiliconGuider_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_2_SiliconGuider = new G4LogicalVolume( fSolid_UpperSmall_2_SiliconGuider ,
                                             fMaterial_UpperSmall_2_SiliconGuider ,
                                             tmpname); 
    fLogicalVol_UpperSmall_2_SiliconGuider -> SetVisAttributes(fVisAtt_UpperSmall_2_SiliconGuider);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_3_Isolator()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_3_Isolator_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_3_Isolator = new G4LogicalVolume( fSolid_UpperSmall_3_Isolator ,
                                             fMaterial_UpperSmall_3_Isolator ,
                                             tmpname); 
    fLogicalVol_UpperSmall_3_Isolator -> SetVisAttributes(fVisAtt_UpperSmall_3_Isolator);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_4_SiliconSpring()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_4_SiliconSpring_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_4_SiliconSpring = new G4LogicalVolume( fSolid_UpperSmall_4_SiliconSpring ,
                                             fMaterial_UpperSmall_4_SiliconSpring ,
                                             tmpname); 
    fLogicalVol_UpperSmall_4_SiliconSpring -> SetVisAttributes(fVisAtt_UpperSmall_4_SiliconSpring);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_5_Isolator()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_5_Isolator_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_5_Isolator = new G4LogicalVolume( fSolid_UpperSmall_5_Isolator ,
                                             fMaterial_UpperSmall_5_Isolator ,
                                             tmpname); 
    fLogicalVol_UpperSmall_5_Isolator -> SetVisAttributes(fVisAtt_UpperSmall_5_Isolator);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSmall_6_InnerSpecialNut()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSmall_6_InnerSpecialNut_%d_Logical",fCrystalID);
    fLogicalVol_UpperSmall_6_InnerSpecialNut = new G4LogicalVolume( fSolid_UpperSmall_6_InnerSpecialNut ,
                                             fMaterial_UpperSmall_6_InnerSpecialNut ,
                                             tmpname); 
    fLogicalVol_UpperSmall_6_InnerSpecialNut -> SetVisAttributes(fVisAtt_UpperSmall_6_InnerSpecialNut);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_LowerContactDisc()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_LowerContactDisc_%d_Logical",fCrystalID);
    fLogicalVol_LowerContactDisc = new G4LogicalVolume( fSolid_LowerContactDisc ,
                                                       fMaterial_LowerContactDisc ,
                                                       tmpname); 
    fLogicalVol_LowerContactDisc -> SetVisAttributes(fVisAtt_LowerContactDisc);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_LowerPTFETube()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_LowerPTFETube_%d_Logical",fCrystalID);
    fLogicalVol_LowerPTFETube = new G4LogicalVolume( fSolid_LowerPTFETube ,
                                                       fMaterial_LowerPTFETube ,
                                                       tmpname); 
    fLogicalVol_LowerPTFETube -> SetVisAttributes(fVisAtt_LowerPTFETube);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_LowerPressurePlate()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_LowerPressurePlate_%d_Logical",fCrystalID);
    fLogicalVol_LowerPressurePlate = new G4LogicalVolume( fSolid_LowerPressurePlate ,
                                                       fMaterial_LowerPressurePlate ,
                                                       tmpname); 
    fLogicalVol_LowerPressurePlate -> SetVisAttributes(fVisAtt_LowerPressurePlate);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_LowerGrubScrew()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_LowerGrubScrew_%d_Logical",fCrystalID);
    fLogicalVol_LowerGrubScrew = new G4LogicalVolume( fSolid_LowerGrubScrew ,
                                                       fMaterial_LowerGrubScrew ,
                                                       tmpname); 
    fLogicalVol_LowerGrubScrew -> SetVisAttributes(fVisAtt_LowerGrubScrew);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_VerticalBar()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_VerticalBar_%d_Logical",fCrystalID);
    fLogicalVol_VerticalBar = new G4LogicalVolume( fSolid_VerticalBar ,
                                                       fMaterial_VerticalBar ,
                                                       tmpname); 
    fLogicalVol_VerticalBar -> SetVisAttributes(fVisAtt_VerticalBar);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperPTFERing()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperPTFERing_%d_Logical",fCrystalID);
    fLogicalVol_UpperPTFERing = new G4LogicalVolume( fSolid_UpperPTFERing ,
                                                       fMaterial_UpperPTFERing ,
                                                       tmpname); 
    fLogicalVol_UpperPTFERing -> SetVisAttributes(fVisAtt_UpperPTFERing);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperCounterNut()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperCounterNut_%d_Logical",fCrystalID);
    fLogicalVol_UpperCounterNut = new G4LogicalVolume( fSolid_UpperCounterNut ,
                                                       fMaterial_UpperCounterNut ,
                                                       tmpname); 
    fLogicalVol_UpperCounterNut -> SetVisAttributes(fVisAtt_UpperCounterNut);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperSpecialNut()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperSpecialNut_%d_Logical",fCrystalID);
    fLogicalVol_UpperSpecialNut = new G4LogicalVolume( fSolid_UpperSpecialNut ,
                                                       fMaterial_UpperSpecialNut ,
                                                       tmpname); 
    fLogicalVol_UpperSpecialNut -> SetVisAttributes(fVisAtt_UpperSpecialNut);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_LowerStar()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_LowerStar_%d_Logical",fCrystalID);
    fLogicalVol_LowerStar = new G4LogicalVolume( fSolid_LowerStar,
                                                 fMaterial_LowerStar,
                                                 tmpname); 
    fLogicalVol_LowerStar -> SetVisAttributes(fVisAtt_LowerStar);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreateLogicalVolumes_UpperStar()
{
    char tmpname[100];
    sprintf(tmpname,"StarHolder_UpperStar_%d_Logical",fCrystalID);
    fLogicalVol_UpperStar = new G4LogicalVolume( fSolid_UpperStar,
                                                 fMaterial_UpperStar,
                                                 tmpname); 
    fLogicalVol_UpperStar -> SetVisAttributes(fVisAtt_UpperStar);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreatePhysicalVolume(G4LogicalVolume* motherVol, 
						  G4int /*col*/, G4int /*row*/, 
						  G4int ffHolderGeometry1Number, 
						  const G4ThreeVector& crystalPos,  
						  G4bool BEGeFlag)
{
  
    char volname[100];
    G4ThreeVector position;

    // the upper star
    // ---------------
    position = crystalPos;
    G4double aGap = (fIsAPair) ? fCrystalPairGap : 0.;
    position.setZ( position.getZ() + 0.5*fCrystalHeight + 0.5*aGap + fTopDistance_CrystalSurface_Holder );
    sprintf(volname,"StarHolder_UpperStar_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperStarPhysical =*/ new G4PVPlacement(0, 
                                                         position, 
                                                         fLogicalVol_UpperStar, 
                                                         volname, 
                                                         motherVol, 
                                                         false, 
                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    // the lower star
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() - 0.5*fCrystalHeight - fBotDistance_CrystalSurface_Holder -0.5*aGap );
    sprintf(volname,"StarHolder_LowerStar_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerStarPhysical =*/ new G4PVPlacement(0, 
                                                         position, 
                                                         fLogicalVol_LowerStar, 
                                                         volname, 
                                                         motherVol, 
                                                         false, 
                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the three vertical bars
    // -----------------------
    //G4PVPlacement* VerticalBarPhysical[3];
    for(int ii=0;ii<3;ii++) {
        G4double VertBarAngle = ii*120.*deg;
        G4ThreeVector VertBarTranslation;
        G4double tmpR = fMaximalAllowedCrystalRadius + fRadialDistance_CrystalSurface_Holder + 0.5*fVerticalBar_dx + 0.1*mm /* for star-bars */;
        VertBarTranslation.setX(tmpR*std::cos(VertBarAngle));
        VertBarTranslation.setY(tmpR*std::sin(VertBarAngle));
        VertBarTranslation.setZ( 0.5*(fTopDistance_CrystalSurface_Holder - fBotDistance_CrystalSurface_Holder) );
        position = crystalPos + VertBarTranslation;
        G4RotationMatrix VertBarRot;;
        VertBarRot.rotateZ(VertBarAngle);
        sprintf(volname,"StarHolder_VerticalBar_%d",  3*ffHolderGeometry1Number+ii );
        /*VerticalBarPhysical[ii]  = */ 
	new G4PVPlacement( G4Transform3D(VertBarRot,position) , 
			   fLogicalVol_VerticalBar, 
			   volname, 
			   motherVol, 
			   false, 
			   3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii );
        MGLog(debugging) << "######## 3*fNumberOfPhysicalVolumesSets["<<fCrystalID<<"] + ii = "
                         << (3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii) << "   volname="<< volname<<endlog;
    }


    // +--------------------------------------+    
    // |  NOW THE PARTS ABOVE THE CRYSTAL     |
    // +--------------------------------------+    

    // the special nut
    // ---------------
    G4double tolerance = 0.1*mm;
    position = crystalPos;
    position.setZ( position.getZ() 
                   + 0.5*fCrystalHeight 
                   + fTopDistance_CrystalSurface_Holder 
                   + ( fZzHldStarUpperTube[7] -fZOverlap_UpperStarTube_SpecialNut) 
                   + tolerance);
    sprintf(volname,"StarHolder_UpperSpecialNut_%d",ffHolderGeometry1Number);
    if(not BEGeFlag)
    /*G4PVPlacement* UpperSpecialNutPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_UpperSpecialNut, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the counter nut
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() 
                   + 0.5*fCrystalHeight 
                   + fTopDistance_CrystalSurface_Holder 
                   + ( fZzHldStarUpperTube[7] -fZOverlap_UpperStarTube_SpecialNut) 
                   - 0.5*fCounterNut_h);
    sprintf(volname,"StarHolder_UpperCounterNut_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperCounterNutPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_UpperCounterNut, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the PTFE Ring
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() + 0.5*fCrystalHeight + 0.5*fUpperPTFERing_h ); 
    sprintf(volname,"StarHolder_UpperPTFERing_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperPTFERingPhysical = new*/ G4PVPlacement(0, 
							     position, 
							     fLogicalVol_UpperPTFERing, 
							     volname, 
							     motherVol, 
							     false, 
							     fNumberOfPhysicalVolumesSets[fCrystalID]);
    




    G4double dZPosUpper;
    // the Upper Contact 'UpperChineseHat'
    // -----------------------------------
    dZPosUpper = 
        0.5*fCrystalHeight
        - 0.5*fUpperChineseHat_ContactConeHeight; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper);
    sprintf(volname,"StarHolder_UpperChineseHat_%d",ffHolderGeometry1Number);
    if (fLogicalVol_UpperChineseHat)
      /*G4PVPlacement* UpperChineseHat_Physical =*/ new G4PVPlacement(0, 
								  position, 
								  fLogicalVol_UpperChineseHat, 
								  volname, 
								  motherVol, 
								  false, 
								  fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the UpperSmall_3_Isolator
    // ---------------
    dZPosUpper += fUpperChineseHat_totalHeight; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_3_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall3Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall3Isolator_Physical =*/ new G4PVPlacement(0, 
                                                     position, 
                                                     fLogicalVol_UpperSmall_3_Isolator, 
                                                     volname, 
                                                     motherVol, 
                                                     false, 
                                                     fNumberOfPhysicalVolumesSets[fCrystalID]);


    // the UpperSmall_4_SiliconSpring
    // ---------------
    dZPosUpper += fUpperSmall_3_Isolator_h; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_4_SiliconSpring_h);
    sprintf(volname,"StarHolder_UpperSmall4SiliconSpring_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall4SiliconSpring_Physical =*/ new G4PVPlacement(0, 
                                                                         position, 
                                                                         fLogicalVol_UpperSmall_4_SiliconSpring, 
                                                                         volname, 
                                                                         motherVol, 
                                                                         false, 
                                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the UpperSmall_5_Isolator
    // ---------------
    dZPosUpper += fUpperSmall_4_SiliconSpring_h; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_5_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall5Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall5Isolator_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_5_Isolator, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the UpperSmall_6_InnerSpecialNut
    // ---------------
    dZPosUpper += fUpperSmall_5_Isolator_h;
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_6_InnerSpecialNut_h);
    sprintf(volname,"StarHolder_UpperSmall6InnerSpecialNut_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall6InnerSpecialNut_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_6_InnerSpecialNut, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);




    // the fUpperSmall_1_Isolator
    // -----------------------------------
    dZPosUpper = 
        0.5*fCrystalHeight
        + fTopDistance_CrystalSurface_Holder
        + fabs(fZzHldStarUpperTube[1]-fZzHldStarUpperTube[0]);
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_1_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall1Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall1Isolator_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_1_Isolator, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the fUpperSmall_2_SiliconGuider
    // -----------------------------------
    dZPosUpper += fUpperSmall_1_Isolator_h;
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_2_SiliconGuider_h);
    sprintf(volname,"StarHolder_UpperSmall2SiliconGuider_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall2SiliconGuider_Physical =*/ new G4PVPlacement(0, 
                                                                         position, 
                                                                         fLogicalVol_UpperSmall_2_SiliconGuider, 
                                                                         volname, 
                                                                         motherVol, 
                                                                         false, 
                                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    



    // +--------------------------------------+    
    // |  NOW THE PARTS BELOW THE CRYSTAL     |
    // +--------------------------------------+    
    G4double dZPosLower;
    dZPosLower = 
        0.5*fCrystalHeight; 

    // the LowerContactDisc
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerContactDisc_h);
    sprintf(volname,"StarHolder_LowerContactDisc_%d",ffHolderGeometry1Number);
    if(not BEGeFlag)
    /*G4PVPlacement* LowerContactDiscPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerContactDisc, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the LowerPTFETube
    // ---------------
    dZPosLower += fLowerContactDisc_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerPTFETube_h);
    sprintf(volname,"StarHolder_LowerPTFETube_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerPTFETubePhysical =*/ new G4PVPlacement(0, 
                                                             position, 
                                                             fLogicalVol_LowerPTFETube, 
                                                             volname, 
                                                             motherVol, 
                                                             false, 
                                                             fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the LowerPressurePlate
    // ---------------
    dZPosLower += fLowerPTFETube_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerPressurePlate_h);
    sprintf(volname,"StarHolder_LowerPressurePlate_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerPressurePlatePhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerPressurePlate, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the LowerGrubScrew
    // ---------------
    dZPosLower += fLowerPressurePlate_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerGrubScrew_h);
    sprintf(volname,"StarHolder_LowerGrubScrew_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerGrubScrewPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerGrubScrew, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);


    fNumberOfPhysicalVolumesSets[fCrystalID]++;

}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase1Parts::CreatePhysicalVolume_NoUpperPTFERing(G4LogicalVolume* motherVol, 
								  G4int /*col*/, G4int /*row*/, 
								  G4int ffHolderGeometry1Number, const G4ThreeVector& crystalPos)
{



    char volname[100];
    G4ThreeVector position;

    // the upper star
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() + 0.5*fCrystalHeight + fTopDistance_CrystalSurface_Holder );
    sprintf(volname,"StarHolder_UpperStar_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperStarPhysical =*/ new G4PVPlacement(0, 
                                                         position, 
                                                         fLogicalVol_UpperStar, 
                                                         volname, 
                                                         motherVol, 
                                                         false, 
                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    // the lower star
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() - 0.5*fCrystalHeight - fBotDistance_CrystalSurface_Holder );
    sprintf(volname,"StarHolder_LowerStar_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerStarPhysical =*/ new G4PVPlacement(0, 
                                                         position, 
                                                         fLogicalVol_LowerStar, 
                                                         volname, 
                                                         motherVol, 
                                                         false, 
                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the three vertical bars
    // -----------------------
    //G4PVPlacement* VerticalBarPhysical[3];
    for(int ii=0;ii<3;ii++) {
        G4double VertBarAngle = ii*120.*deg;
        G4ThreeVector VertBarTranslation;
        G4double tmpR = fMaximalAllowedCrystalRadius + fRadialDistance_CrystalSurface_Holder + 0.5*fVerticalBar_dx + 0.1*mm /* for star-bars */;
        VertBarTranslation.setX(tmpR*std::cos(VertBarAngle));
        VertBarTranslation.setY(tmpR*std::sin(VertBarAngle));
        VertBarTranslation.setZ( 0.5*(fTopDistance_CrystalSurface_Holder - fBotDistance_CrystalSurface_Holder) );
        position = crystalPos + VertBarTranslation;
        G4RotationMatrix VertBarRot;;
        VertBarRot.rotateZ(VertBarAngle);
        sprintf(volname,"StarHolder_VerticalBar_%d",  3*ffHolderGeometry1Number+ii );
        /*VerticalBarPhysical[ii]  =*/ 
	new G4PVPlacement( G4Transform3D(VertBarRot,position) , 
			   fLogicalVol_VerticalBar, 
			   volname, 
			   motherVol, 
			   false, 
			   3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii );
        MGLog(debugging) << "######## 3*fNumberOfPhysicalVolumesSets["<<fCrystalID<<"] + ii = "
                         << (3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii) << "   volname="<< volname<<endlog;
    }


    // +--------------------------------------+    
    // |  NOW THE PARTS ABOVE THE CRYSTAL     |
    // +--------------------------------------+    

    // the special nut
    // ---------------
    G4double tolerance = 0.1*mm;
    position = crystalPos;
    position.setZ( position.getZ() 
                   + 0.5*fCrystalHeight 
                   + fTopDistance_CrystalSurface_Holder 
                   + ( fZzHldStarUpperTube[7] -fZOverlap_UpperStarTube_SpecialNut) 
                   + tolerance);
    sprintf(volname,"StarHolder_UpperSpecialNut_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSpecialNutPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_UpperSpecialNut, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the counter nut
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() 
                   + 0.5*fCrystalHeight 
                   + fTopDistance_CrystalSurface_Holder 
                   + ( fZzHldStarUpperTube[7] -fZOverlap_UpperStarTube_SpecialNut) 
                   - 0.5*fCounterNut_h);
    sprintf(volname,"StarHolder_UpperCounterNut_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperCounterNutPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_UpperCounterNut, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    



    G4double dZPosUpper;
    // the Upper Contact 'UpperChineseHat'
    // -----------------------------------
    dZPosUpper = 
        0.5*fCrystalHeight
        - 0.5*fUpperChineseHat_ContactConeHeight; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper);
    sprintf(volname,"StarHolder_UpperChineseHat_%d",ffHolderGeometry1Number);
    if (fLogicalVol_UpperChineseHat)
      /*G4PVPlacement* UpperChineseHat_Physical =*/ new G4PVPlacement(0, 
								  position, 
								  fLogicalVol_UpperChineseHat, 
								  volname, 
								  motherVol, 
								  false, 
								  fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the UpperSmall_3_Isolator
    // ---------------
    dZPosUpper += fUpperChineseHat_totalHeight; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_3_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall3Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall3Isolator_Physical =*/ new G4PVPlacement(0, 
                                                     position, 
                                                     fLogicalVol_UpperSmall_3_Isolator, 
                                                     volname, 
                                                     motherVol, 
                                                     false, 
                                                     fNumberOfPhysicalVolumesSets[fCrystalID]);


    // the UpperSmall_4_SiliconSpring
    // ---------------
    dZPosUpper += fUpperSmall_3_Isolator_h; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_4_SiliconSpring_h);
    sprintf(volname,"StarHolder_UpperSmall4SiliconSpring_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall4SiliconSpring_Physical =*/ new G4PVPlacement(0, 
                                                                         position, 
                                                                         fLogicalVol_UpperSmall_4_SiliconSpring, 
                                                                         volname, 
                                                                         motherVol, 
                                                                         false, 
                                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the UpperSmall_5_Isolator
    // ---------------
    dZPosUpper += fUpperSmall_4_SiliconSpring_h; 
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_5_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall5Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall5Isolator_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_5_Isolator, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the UpperSmall_6_InnerSpecialNut
    // ---------------
    dZPosUpper += fUpperSmall_5_Isolator_h;
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_6_InnerSpecialNut_h);
    sprintf(volname,"StarHolder_UpperSmall6InnerSpecialNut_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall6InnerSpecialNut_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_6_InnerSpecialNut, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);




    // the fUpperSmall_1_Isolator
    // -----------------------------------
    dZPosUpper = 
        0.5*fCrystalHeight
        + fTopDistance_CrystalSurface_Holder
        + fabs(fZzHldStarUpperTube[1]-fZzHldStarUpperTube[0]);
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_1_Isolator_h);
    sprintf(volname,"StarHolder_UpperSmall1Isolator_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall1Isolator_Physical =*/ new G4PVPlacement(0, 
                                                                    position, 
                                                                    fLogicalVol_UpperSmall_1_Isolator, 
                                                                    volname, 
                                                                    motherVol, 
                                                                    false, 
                                                                    fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the fUpperSmall_2_SiliconGuider
    // -----------------------------------
    dZPosUpper += fUpperSmall_1_Isolator_h;
    position = crystalPos;
    position.setZ( position.getZ() + dZPosUpper + 0.5*fUpperSmall_2_SiliconGuider_h);
    sprintf(volname,"StarHolder_UpperSmall2SiliconGuider_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* UpperSmall2SiliconGuider_Physical =*/ new G4PVPlacement(0, 
                                                                         position, 
                                                                         fLogicalVol_UpperSmall_2_SiliconGuider, 
                                                                         volname, 
                                                                         motherVol, 
                                                                         false, 
                                                                         fNumberOfPhysicalVolumesSets[fCrystalID]);
    



    // +--------------------------------------+    
    // |  NOW THE PARTS BELOW THE CRYSTAL     |
    // +--------------------------------------+    
    G4double dZPosLower;
    dZPosLower = 
        0.5*fCrystalHeight; 

    // the LowerContactDisc
    // ---------------
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerContactDisc_h);
    sprintf(volname,"StarHolder_LowerContactDisc_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerContactDiscPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerContactDisc, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);
    

    // the LowerPTFETube
    // ---------------
    dZPosLower += fLowerContactDisc_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerPTFETube_h);
    sprintf(volname,"StarHolder_LowerPTFETube_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerPTFETubePhysical =*/ new G4PVPlacement(0, 
                                                             position, 
                                                             fLogicalVol_LowerPTFETube, 
                                                             volname, 
                                                             motherVol, 
                                                             false, 
                                                             fNumberOfPhysicalVolumesSets[fCrystalID]);
    
    // the LowerPressurePlate
    // ---------------
    dZPosLower += fLowerPTFETube_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerPressurePlate_h);
    sprintf(volname,"StarHolder_LowerPressurePlate_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerPressurePlatePhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerPressurePlate, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);

    // the LowerGrubScrew
    // ---------------
    dZPosLower += fLowerPressurePlate_h;
    position = crystalPos;
    position.setZ( position.getZ() - dZPosLower - 0.5*fLowerGrubScrew_h);
    sprintf(volname,"StarHolder_LowerGrubScrew_%d",ffHolderGeometry1Number);
    /*G4PVPlacement* LowerGrubScrewPhysical =*/ new G4PVPlacement(0, 
                                                          position, 
                                                          fLogicalVol_LowerGrubScrew, 
                                                          volname, 
                                                          motherVol, 
                                                          false, 
                                                          fNumberOfPhysicalVolumesSets[fCrystalID]);


    fNumberOfPhysicalVolumesSets[fCrystalID]++;

}

//--------------------------------------------------------------------------------
G4double
GEGeometryHolderPhase1Parts::GetZDistanceToSuspensionPlaneTop() const
{
    return (0.5*fCrystalHeight 
            + fTopDistance_CrystalSurface_Holder 
            + ( fZzHldStarUpperTube[7] -fZOverlap_UpperStarTube_SpecialNut)
            + fSpclNut_ZPosOfSuspensionHoleTop);
}

//--------------------------------------------------------------------------------
G4double
GEGeometryHolderPhase1Parts::GetZDistanceToSuspensionPlaneBottom() const
{
    return (0.5*fCrystalHeight + fBotDistance_CrystalSurface_Holder + fabs(fZzHldStarLowerTube[0]-fZzHldStarLowerTube[6]) );
}

//--------------------------------------------------------------------------------
G4double
GEGeometryHolderPhase1Parts::Get_UpperSmall4SiliconSpring_LowerEdge() const
{
    return (0.5*fCrystalHeight 
            - 0.5 * fUpperChineseHat_ContactConeHeight
            + fUpperChineseHat_totalHeight
            + fUpperSmall_3_Isolator_h) ;

}

//--------------------------------------------------------------------------------
G4double
GEGeometryHolderPhase1Parts::Get_UpperSmall4SiliconSpring_UpperEdge() const
{
    return (0.5*fCrystalHeight 
            + fTopDistance_CrystalSurface_Holder
            + fabs(fZzHldStarUpperTube[0]-fZzHldStarUpperTube[7])
            - fUpperSmall_5_Isolator_h
            - fUpperSmall_6_InnerSpecialNut_h) ;
}

//--------------------------------------------------------------------------------
G4double
GEGeometryHolderPhase1Parts::Get_UpperSmall4SiliconSpring_Height() const
{
    return (Get_UpperSmall4SiliconSpring_UpperEdge() - Get_UpperSmall4SiliconSpring_LowerEdge());
}

//--------------------------------------------------------------------------------
void GEGeometryHolderPhase1Parts::ResetParametersForAPair(GEGeometryDetectorDB* db,G4int col,
							  G4int row)
{
  fIsAPair = true;

    
  GEGeometryDetectorParameters* detector1 = 
    db->GetDetectorParameters(db->GetCrystalType(col,row,0));
  GEGeometryDetectorParameters* detector2 = 
    db->GetDetectorParameters(db->GetCrystalType(col,row,1));

  SetCrystalRadius(std::max(detector1->GetOuterRadius(),detector2->GetOuterRadius()));
  SetCrystalInnerRadius(std::min(detector1->GetInnerRadius(),detector2->GetInnerRadius()));
  SetCrystalHeight(detector1->GetHeight()+detector2->GetHeight()); //sum!
  SetCrystalID(detector1->GetDetectorID());
  
  ResetDependentParameters();
  return;
}
