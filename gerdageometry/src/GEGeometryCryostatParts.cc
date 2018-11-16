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
 * $Id: GEGeometryCryostatParts.cc,v 1.4 2009-03-18 10:04:31 schubert Exp $
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
 *  of all parts of the Cryostat, i.e. everything enclosed by the outermost 
 *  layer on the outer cryostat wall
 *
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
 * 12-05-2007, first version, Jens
 * 12-10-2007, added Torlon Rods and thermal shields, Jens
 *             also added a destructor
 * 18-03-2009, changed order of initialisation in default constructor --> this removes warning messages of the compiler, Jens
 * 19-10-2011, heat exchanger added to cryostat as a simple cylindrical geometry, Neslihan
 */
// --------------------------------------------------------------------------//

#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

#include "gerdageometry/GEGeometryCryostatParts.hh"

using namespace CLHEP;


GEGeometryCryostatParts::GEGeometryCryostatParts():

  fCryoOuterwall_Outside("CryoOuterwallOutside"      ),
  fCryoOuterwall_Inside ("CryoOuterwallInside"       ),
  fCryoInnerwallLayer2_Outside ("CryoInnerwallLayer2Outside"),
  fCryoInnerwallLayer1_Outside ("CryoInnerwallLayer1Outside"),
  fCryoInnerwall_Outside("CryoInnerwallOutside"      ),
  fCryoInnerwall_Inside ("CryoInnerwallInside"       ),

  fCryoGasInNeckSolid        (0),
  fThermalShieldInNeckSolid  (0),
  fHeatExchangerSolid        (0),
  fShroudSolid      (0),
  fSolidCuLayer              (0),
  fSolidCuLayerSupportRing   (0),
  fSolidTorlonBigRod         (0),         
  fSolidTorlonCenteringRodTop(0),
  fSolidTorlonCenteringRodBot(0),

  fCryoGasInNeckLogical        (0),
  fThermalShieldInNeckLogical  (0),
  fHeatExchangerLogical        (0),
  fShroudLogical     (0),
  fCuLayerLogical              (0),
  fCuLayerSupportRingLogical   (0),
  fTorlonBigRodLogical         (0),         
  fTorlonCenteringRodTopLogical(0),         
  fTorlonCenteringRodBotLogical(0),         

  fCuLayerPhysical              (0),
  fCuLayerSupportRingPhysical   (0),
  fCryoGasInNeckPhysical        (0),
  fThermalShieldInNeckPhysical  (0),
  fHeatExchangerPhysical        (0),
  fShroudPhysical    (0),

  Distance_TopLidOuterWall_TopLidInnerWall(  -3000  ),
  CryoSlopeLid                     (  -3000  ),
  CryoSlopeLid_AsAngle             (  -3000  ),

  ThicknessInnerWallTopLid         (  -3000  ),
  ThicknessInnerWallBotLid         (  -3000  ),
  ThicknessInnerWallNeck           (  -3000  ),
  ThicknessInnerWallCyl            (  -3000  ),
  ThicknessInnerWallLittlecylBarrel(  -3000  ),
  ThicknessInnerWallLittlecylBottom(  -3000  ),

  ThicknessOuterWallTopLid         (  -3000  ),
  ThicknessOuterWallBotLid         (  -3000  ),
  ThicknessOuterWallNeck           (  -3000  ),
  ThicknessOuterWallCyl            (  -3000  ),
  ThicknessOuterWallLittlecylBarrel(  -3000  ),
  ThicknessOuterWallLittlecylBottom(  -3000  ),

  ThicknessInnerWallLayer1         (  -3000  ),
  ThicknessInnerWallLayer2         (  -3000  ),

  CuLayerSupportRing_Thickness             (  -3000 ),
  CuLayerSupportRing_InnerRadius           (  -3000 ),
  CuLayerSupportRing_OuterRadius           (  -3000 ),
  CuLayerSupportRing_zPosRelToUpperEdgeCryo(  -3000 ),

  CuLayer_NPlatesInPhi     ( -3000  ),
  CuLayer_PlateWidth       ( -3000  ),
  CuLayer_PlateThickness   ( -3000  ),
  CuLayerTop_Height1       ( -3000  ),
  CuLayerTop_Height2       ( -3000  ),
  CuLayerBot_Height        ( -3000  ),

  CryogasHeightInNeck        (0),

  ThermalShieldInNeck_InnerRadius     (0),
  ThermalShieldInNeck_Thickness       (0),
  ThermalShieldInNeck_DistanceToUpperEdgeOfNeck (-9999999),

  HeatExchanger_Height		(0),
  HeatExchanger_Radius		(0),
  HeatExchanger_Thickness	(0),
  HeatExchanger_InnerRadius	(0),
  HeatExchanger_DistanceToLArFillLevel (-9999999),

  Shroud_Height    (0),
  Shroud_Radius  (0),
  Shroud_Thickness   (0),

  TorlonBigRod_RadiusOfPositionInTank ( -3000 ),
  TorlonBigRod_InnerRadius            ( -3000 ),
  TorlonBigRod_OuterRadius            ( -3000 ),
  TorlonBigRod_Height                 ( -3000 ),

  TorlonCenteringRodTop_ZPositionRelToInnerNeckbottom ( -3000 ),
  TorlonCenteringRodTop_RadiusOfPositionInTank        ( -3000 ),
  TorlonCenteringRodTop_InnerRadius   ( -3000 ),
  TorlonCenteringRodTop_OuterRadius   ( -3000 ),
  TorlonCenteringRodTop_Height        ( -3000 ),

  TorlonCenteringRodBot_ZPositionRelToLittlecylbottom ( -3000 ),
  TorlonCenteringRodBot_RadiusOfPositionInTank        ( -3000 ),
  TorlonCenteringRodBot_InnerRadius   ( -3000 ),
  TorlonCenteringRodBot_OuterRadius   ( -3000 ),
  TorlonCenteringRodBot_Height        ( -3000 ),

  fMaterialCuLayer           (0),
  fMaterialCuLayerSupportRing(0),
  fMaterialCryoGas           (0),
  fMaterialThermalShield     (0),
  fMaterialHeatExchanger     (0),
  fMaterialShroud    (0)

{
    fTorlonBigRodPhysicalContainer.clear();
    fTorlonCenteringRodTopPhysicalContainer.clear();
    fTorlonCenteringRodBotPhysicalContainer.clear();
}

GEGeometryCryostatParts::~GEGeometryCryostatParts()
{
    delete fCuLayerLogical; 
    delete fCuLayerSupportRingLogical;
    delete fCryoGasInNeckLogical;
    delete fTorlonBigRodLogical;
    delete fTorlonCenteringRodTopLogical;
    delete fTorlonCenteringRodBotLogical;
    delete fThermalShieldInNeckLogical;
    delete fHeatExchangerLogical;
    delete fShroudLogical;
}

void
GEGeometryCryostatParts::CreateSolids()
{
  static bool has_been_called(false);
  if(has_been_called) return;
  fCryoOuterwall_Outside       .CreatePolycone();
  fCryoOuterwall_Inside        .CreatePolycone();
  //fCryoInnerwallLayer2_Outside .CreatePolycone();
  fCryoInnerwallLayer1_Outside .CreatePolycone();
  fCryoInnerwall_Outside       .CreatePolycone();
  fCryoInnerwall_Inside        .CreatePolycone();
  CreateSolids_Additional();
  has_been_called = true;
}

void
GEGeometryCryostatParts::CreateSolids_Additional()
{

    // ---------------------
    // Cupper Layer
    // ---------------------

    // Outer upper Layer
    G4double AreaCrossSection = CuLayer_NPlatesInPhi * CuLayer_PlateWidth * CuLayer_PlateThickness;
    G4double TubsOuterRadius   = fCryoInnerwall_Inside.rcyl - 3*mm;
    G4double Rtmp1 = TubsOuterRadius;
    G4double TubsInnerRadius   = sqrt( TubsOuterRadius*TubsOuterRadius - AreaCrossSection/3.141592653589);
    G4double Rtmp2 = TubsInnerRadius;
    G4double TubsHeight        = 0.5 * CuLayerTop_Height1;
    G4double TubsStartAngle    = 0.0   * deg; 
    G4double TubsSpanningAngle = 360.0 * deg; 
    G4Tubs* SolidCuLayerTopOuter = new G4Tubs("SolidCuLayerTopOuter",
                                              TubsInnerRadius, 
                                              TubsOuterRadius, 
                                              TubsHeight, 
                                              TubsStartAngle, 
                                              TubsSpanningAngle); 


    // Inner upper Layer
    TubsInnerRadius   = sqrt( TubsOuterRadius*TubsOuterRadius - 2.0*AreaCrossSection/3.141592653589);; 
    TubsOuterRadius   = Rtmp1-0.1*CuLayer_PlateThickness;
    TubsHeight        = 0.5 * CuLayerTop_Height2;
    G4Tubs* SolidCuLayerTopInner = new G4Tubs("SolidCuLayerTopInner",
                                              TubsInnerRadius, 
                                              TubsOuterRadius, 
                                              TubsHeight, 
                                              TubsStartAngle, 
                                              TubsSpanningAngle); 
    // lower Layer
    TubsInnerRadius   = Rtmp2;
    TubsOuterRadius   = Rtmp1; 
    TubsHeight        = 0.5 * CuLayerBot_Height;
    G4Tubs* SolidCuLayerBot = new G4Tubs("SolidCuLayerBot",
                                          TubsInnerRadius, 
                                          TubsOuterRadius, 
                                          TubsHeight, 
                                          TubsStartAngle, 
                                          TubsSpanningAngle); 

    G4double zShift = - 0.5*CuLayerTop_Height1 + 0.5*CuLayerTop_Height2;
    G4UnionSolid* US1_CuLayer = new G4UnionSolid("US1_CuLayer",
                                                 SolidCuLayerTopOuter,
                                                 SolidCuLayerTopInner,
                                                 0,
                                                 G4ThreeVector(0.0, 0.0 , zShift));

    zShift = - 0.5*CuLayerTop_Height1 - CuLayerSupportRing_Thickness - 0.5*CuLayerBot_Height;
    fSolidCuLayer = new G4UnionSolid("SolidCuLayer",
                                     US1_CuLayer,
                                     SolidCuLayerBot,
                                     0,
                                     G4ThreeVector(0.0, 0.0 , zShift));

    // support ring
    TubsInnerRadius   = CuLayerSupportRing_InnerRadius;
    TubsOuterRadius   = CuLayerSupportRing_OuterRadius;
    TubsHeight        = 0.5 * CuLayerSupportRing_Thickness;
    fSolidCuLayerSupportRing  = new G4Tubs("SolidCuLayerSupportRing",
                                           TubsInnerRadius, 
                                           TubsOuterRadius, 
                                           TubsHeight, 
                                           TubsStartAngle, 
                                           TubsSpanningAngle); 


    // -----------------
    // Cryo gas
    // -----------------
    TubsInnerRadius   = 0.0;
    TubsOuterRadius   = GetCryoInnerwall_Inside()->GetRneck();
    TubsHeight        = 0.5* CryogasHeightInNeck;
    fCryoGasInNeckSolid   = new G4Tubs("CryoGasInNeckSolid",
                                       TubsInnerRadius, 
                                       TubsOuterRadius, 
                                       TubsHeight, 
                                       TubsStartAngle, 
                                       TubsSpanningAngle); 

    // -----------------
    // torlone rods
    // -----------------
    fSolidTorlonBigRod  = new G4Tubs("CryoTorlonBigRodSolid",
                                     TorlonBigRod_InnerRadius, 
                                     TorlonBigRod_OuterRadius,
                                     0.5*TorlonBigRod_Height,
                                     0.0*deg,
                                     360.0*deg); 
    fSolidTorlonCenteringRodTop  = new G4Tubs("CryoTorlonCenteringRodTopSolid",
                                              TorlonCenteringRodTop_InnerRadius, 
                                              TorlonCenteringRodTop_OuterRadius,
                                              0.5*TorlonCenteringRodTop_Height,
                                              0.0*deg,
                                              360.0*deg); 
    fSolidTorlonCenteringRodBot  = new G4Tubs("CryoTorlonCenteringRodBotSolid",
                                              TorlonCenteringRodBot_InnerRadius, 
                                              TorlonCenteringRodBot_OuterRadius,
                                              0.5*TorlonCenteringRodBot_Height,
                                              0.0*deg,
                                              360.0*deg); 

    // ---------------------
    // thermal shield
    // ---------------------
    fThermalShieldInNeckSolid  = new G4Tubs("ThermalShieldInNeckSolid",
                                            ThermalShieldInNeck_InnerRadius,
                                            fCryoInnerwall_Inside.GetRneck(),
                                            ThermalShieldInNeck_Thickness,
                                            0.0*deg,
                                            360.0*deg); 

    // ---------------------
    // heat exchanger
    // ---------------------
    fHeatExchangerSolid  = new G4Tubs("HeatExchangerSolid",
                                       HeatExchanger_InnerRadius,
                                       HeatExchanger_Radius,
                                       0.5* HeatExchanger_Height,
                                       0.0*deg,
                                       360.0*deg);

    // -----------------------
    // shroud
    // -----------------------

    TubsInnerRadius = 0. * mm;
    G4Tubs* ShroudTubsOuter = new G4Tubs("ShroudTubsOuter",
					 TubsInnerRadius,
					 Shroud_Radius,
					 0.5*Shroud_Height,
					 0.0*deg,
					 360.0*deg);
    
    G4Tubs* ShroudTubsInner = new G4Tubs("ShroudTubsInner",
					 TubsInnerRadius,
					 Shroud_Radius-Shroud_Thickness,
					 0.5*( Shroud_Height - Shroud_Thickness ),
					 0.0*deg,
					 360.0*deg);
    
    
    fShroudSolid = new G4SubtractionSolid("ShroudSolid",
					  ShroudTubsOuter,
					  ShroudTubsInner,
					  0,
					  G4ThreeVector(0.0, 0.0, 0.5*Shroud_Thickness));
    
}


void
GEGeometryCryostatParts::CreateLogicalVolumes()
{
  fCryoOuterwall_Outside       .CreateLogicalVolume("CryostatOuterWallLogical");
  fCryoOuterwall_Inside        .CreateLogicalVolume("CryostatInsGapLogical");
  //fCryoInnerwallLayer2_Outside .CreateLogicalVolume("CryostatSuperinsLayerOnInnerWallLogical");
  fCryoInnerwallLayer1_Outside .CreateLogicalVolume("CryostatMakrolonLayerOnInnerWallLogical");
  fCryoInnerwall_Outside       .CreateLogicalVolume("CryostatInnerWallLogical");
  fCryoInnerwall_Inside        .CreateLogicalVolume("CryoLiquidLogical");
  CreateLogicalVolumes_Additional();
}

void
GEGeometryCryostatParts::CreateLogicalVolumes_Additional()
{
    fCuLayerLogical = new G4LogicalVolume(fSolidCuLayer,
                                          fMaterialCuLayer,
                                          "CuLayerLogical");

    fCuLayerSupportRingLogical = new G4LogicalVolume(fSolidCuLayerSupportRing,
                                                     fMaterialCuLayerSupportRing,
                                                     "CuLayerSupportRingLogical");

    fCryoGasInNeckLogical = new G4LogicalVolume(fCryoGasInNeckSolid,
                                                fMaterialCryoGas,
                                                "CryoGasInNeckLogical");

    fTorlonBigRodLogical          = new G4LogicalVolume(fSolidTorlonBigRod,
                                                        fMaterialTorlonRods,
                                                        "CryoTorlonBigRodLogical");

    fTorlonCenteringRodTopLogical          = new G4LogicalVolume(fSolidTorlonCenteringRodTop,
                                                                 fMaterialTorlonRods,
                                                                 "CryoTorlonCenteringRodTopLogical");

    fTorlonCenteringRodBotLogical          = new G4LogicalVolume(fSolidTorlonCenteringRodBot,
                                                                 fMaterialTorlonRods,
                                                                 "CryoTorlonCenteringRodBotLogical");

    fThermalShieldInNeckLogical            = new G4LogicalVolume(fThermalShieldInNeckSolid,
                                                                 fMaterialThermalShield,
                                                                 "ThermalShieldInNeckLogical");

    fHeatExchangerLogical = new G4LogicalVolume(fHeatExchangerSolid,
                                                fMaterialHeatExchanger,
                                                "HeatExchangerLogical");

    fShroudLogical = new G4LogicalVolume( fShroudSolid,
					fMaterialShroud,
					"ShroudLogical" );
}

G4VPhysicalVolume*
GEGeometryCryostatParts::CreatePhysicalVolumes(G4LogicalVolume* aMother,
                                               const G4ThreeVector& positionShift)
{
    fCryoOuterwall_Outside       .CreatePhysicalVolume("CryostatOuterWallPhysical",
                                                       aMother, 
                                                       positionShift);

    fCryoOuterwall_Inside        .CreatePhysicalVolume("CryostatInsGapPhysical"          ,
                                                       fCryoOuterwall_Outside.GetLogicalVolume());



    //  fCryoInnerwallLayer2_Outside .CreatePhysicalVolume("CryostatSuperinsLayerOnInnerWallPhysical",
    //                                                     fCryoOuterwall_Inside.GetLogicalVolume());
    //  
    //  fCryoInnerwallLayer1_Outside .CreatePhysicalVolume("CryostatMakrolonLayerOnInnerWallPhysical",
    //                                                     fCryoInnerwallLayer2_Outside.GetLogicalVolume());

    // Do not put in the superinsulation layer on the inner steel wall,
    // because it is to thin

    fCryoInnerwallLayer1_Outside .CreatePhysicalVolume("CryostatMakrolonLayerOnInnerWallPhysical",
                                                       fCryoOuterwall_Inside.GetLogicalVolume());
    
    fCryoInnerwall_Outside       .CreatePhysicalVolume("CryostatInnerWallPhysical",
                                                       fCryoInnerwallLayer1_Outside.GetLogicalVolume());

    fCryoInnerwall_Inside        .CreatePhysicalVolume("CryoLiquidPhysical",
                                                       fCryoInnerwall_Outside.GetLogicalVolume());
    
    CreatePhysicalVolumes_Additional();


    return fCryoOuterwall_Outside.GetPhysicalVolume();
}



void
GEGeometryCryostatParts::CreatePhysicalVolumes_Additional()
{

    // Cu Layer 
    G4double xShift,yShift,zShift;
    zShift = CuLayerSupportRing_zPosRelToUpperEdgeCryo 
        + 0.5*CuLayerSupportRing_Thickness
        + 0.5*CuLayerTop_Height1
        + GEGeometryCryostatMassiveShape::GeneralPolyconeZShift;
    fCuLayerPhysical = new G4PVPlacement(0, 
                                         G4ThreeVector(0,0,zShift),
                                         fCuLayerLogical,
                                         "CryostatCuLayerPhysical",
                                         fCryoInnerwall_Inside.GetLogicalVolume(),
                                         false, 
                                         0); 

    // cu Layer support ring
    zShift = CuLayerSupportRing_zPosRelToUpperEdgeCryo
        + GEGeometryCryostatMassiveShape::GeneralPolyconeZShift;
    fCuLayerSupportRingPhysical = new G4PVPlacement(0, 
                                                    G4ThreeVector(0,0,zShift),
                                                    fCuLayerSupportRingLogical,
                                                    "CryostatCuLayerSupportRingPhysical",
                                                    fCryoInnerwall_Inside.GetLogicalVolume(),
                                                    false, 
                                                    0); 

    // cryo gas in neck
    // ------------------
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        - 0.5*CryogasHeightInNeck ;
    fCryoGasInNeckPhysical = new G4PVPlacement(0, 
                                               G4ThreeVector(0,0,zShift),
                                               fCryoGasInNeckLogical,
                                               "CryoGasInNeckPhysical",
                                               fCryoInnerwall_Inside.GetLogicalVolume(),
                                               false, 
                                               0); 
    // thermal shield in neck
    // ------------------
    zShift = 
        0.5*CryogasHeightInNeck
        - ThermalShieldInNeck_DistanceToUpperEdgeOfNeck; 
    fThermalShieldInNeckPhysical = new G4PVPlacement(0, 
                                                    G4ThreeVector(0,0,zShift),
                                                    fThermalShieldInNeckLogical,
                                                    "ThermalShieldInNeckPhysical",
                                                    fCryoGasInNeckLogical,
                                                    false, 
                                                    0); 

    // heat exchanger
    // ------------------
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        - (CryogasHeightInNeck + HeatExchanger_DistanceToLArFillLevel + 0.5*HeatExchanger_Height); 
    fHeatExchangerPhysical = new G4PVPlacement(0, 
                                               G4ThreeVector(0,0,zShift),
                                               fHeatExchangerLogical,
                                               "HeatExchangerPhysical",
                                               fCryoInnerwall_Inside.GetLogicalVolume(),
                                               false, 
                                               0);

    // shroud
    // ------------------
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        - (CryogasHeightInNeck + HeatExchanger_DistanceToLArFillLevel + HeatExchanger_Height + 0.5*Shroud_Height); 
    fShroudPhysical = new G4PVPlacement(0, 
					G4ThreeVector(0,0,zShift),
					fShroudLogical,
					"ShroudPhysical",
					fCryoInnerwall_Inside.GetLogicalVolume(),
					false, 
					0);


    // big torlone rods in cryostat
    // -----------------------------
    G4double dz1 = 
        fCryoOuterwall_Inside.GetSlopebotlid()*(TorlonBigRod_RadiusOfPositionInTank-fCryoOuterwall_Inside.GetRlittlecyl())
        + fCryoOuterwall_Inside.GetSlopebotlid()*TorlonBigRod_OuterRadius;
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        -      fCryoOuterwall_Inside.GetHneck()
        -      fCryoOuterwall_Inside.GetHtopcylbot()
        + 0.5*TorlonBigRod_Height
        +      dz1 
        +      1*mm;
    char RodName[100];
    G4int nRods=8;
    for(int iRod=0;iRod<nRods;iRod++) {
        sprintf(RodName,"CryostatTorlonBigRod_%i",iRod);
        G4double alpha = 0.0 + 360.0*deg*iRod/nRods;
        xShift = TorlonBigRod_RadiusOfPositionInTank*cos(alpha);
        yShift = TorlonBigRod_RadiusOfPositionInTank*sin(alpha);
        G4VPhysicalVolume* RodPhysical =  new G4PVPlacement(0, 
                                                            G4ThreeVector(xShift,yShift,zShift),
                                                            fTorlonBigRodLogical,
                                                            RodName,
                                                            fCryoOuterwall_Inside.GetLogicalVolume(),
                                                            false, 
                                                            0); 
        fTorlonBigRodPhysicalContainer.push_back(RodPhysical);
    }

    

    // centering torlone rods in cryostat top
    // ---------------------------------------
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        - fCryoInnerwall_Outside.GetHneck()
        + TorlonCenteringRodTop_ZPositionRelToInnerNeckbottom;
    nRods=6;
    G4RotationMatrix mUnity;
    G4RotationMatrix mApply;
    G4RotationMatrix mRotY_90Deg;      mRotY_90Deg.rotateY(90.0*deg);
    G4RotationMatrix mRotZ_OnePartDeg; mRotZ_OnePartDeg.rotateZ(360*deg/nRods);

    for(int iRod=0;iRod<nRods;iRod++) {
        sprintf(RodName,"CryostatTorlonCenteringRodTop_%i",iRod);
        G4double alpha = 0.0 + 360*deg*iRod/nRods;
        xShift = TorlonCenteringRodTop_RadiusOfPositionInTank*cos(alpha);
        yShift = TorlonCenteringRodTop_RadiusOfPositionInTank*sin(alpha);
        mApply=mUnity;
        mApply = mRotY_90Deg*mApply ;
        for(int iii=0;iii<iRod;iii++) mApply = mRotZ_OnePartDeg*mApply;
        G4VPhysicalVolume* RodPhysical =  new G4PVPlacement(G4Transform3D(mApply, 
                                                                          G4ThreeVector(xShift,yShift,zShift)),
                                                            fTorlonCenteringRodTopLogical,
                                                            RodName,
                                                            fCryoOuterwall_Inside.GetLogicalVolume(),
                                                            //fCryoInnerwall_Inside.GetLogicalVolume(),
                                                            false, 
                                                            0); 
        fTorlonCenteringRodTopPhysicalContainer.push_back(RodPhysical);
    }

    
    // centering torlone rods in cryostat top
    // ---------------------------------------
    zShift = GEGeometryCryostatMassiveShape::GeneralPolyconeZShift
        - fCryoInnerwall_Outside.GetHneck()
        - fCryoInnerwall_Outside.GetHtopcylbot()
        - fCryoInnerwall_Outside.GetHlittlecyl()
        + TorlonCenteringRodBot_ZPositionRelToLittlecylbottom;
    nRods=6;

    for(int iRod=0;iRod<nRods;iRod++) {
        sprintf(RodName,"CryostatTorlonCenteringRodBot_%i",iRod);
        G4double alpha = 0.0 + 360*deg*iRod/nRods;
        xShift = TorlonCenteringRodBot_RadiusOfPositionInTank*cos(alpha);
        yShift = TorlonCenteringRodBot_RadiusOfPositionInTank*sin(alpha);
        mApply=mUnity;
        mApply = mRotY_90Deg*mApply ;
        for(int iii=0;iii<iRod;iii++) mApply = mRotZ_OnePartDeg*mApply;
        G4VPhysicalVolume* RodPhysical =  new G4PVPlacement(G4Transform3D(mApply, 
                                                                          G4ThreeVector(xShift,yShift,zShift)),
                                                            fTorlonCenteringRodBotLogical,
                                                            RodName,
                                                            fCryoOuterwall_Inside.GetLogicalVolume(),
                                                            //fCryoInnerwall_Inside.GetLogicalVolume(),
                                                            false, 
                                                            0); 
        fTorlonCenteringRodBotPhysicalContainer.push_back(RodPhysical);
    }

}


GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoOuterwall_Outside()
{return &fCryoOuterwall_Outside;      }

GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoOuterwall_Inside()
{return &fCryoOuterwall_Inside;       }

GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoInnerwallLayer2_Outside()
{return &fCryoInnerwallLayer2_Outside;}

GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoInnerwallLayer1_Outside()
{return &fCryoInnerwallLayer1_Outside;}

GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoInnerwall_Outside()
{return &fCryoInnerwall_Outside;      }

GEGeometryCryostatMassiveShape* GEGeometryCryostatParts::GetCryoInnerwall_Inside()
{return &fCryoInnerwall_Inside;       }
