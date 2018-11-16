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
 * CLASS DECLARATION:  GEGeometryCryostatParts.hh
 *
 * DESCRIPTION:
 *
 *   This class should contain all \n
 *     - parameters \n
 *     - logical volumes \n
 *     - physical volumes \n
 *   of all parts of the Cryostat, i.e. everything enclosed by the outermost
 *   layer on the outer cryostat wall.
 *
 *   The class contains all parts of the cryostat
 *   including (from outside towards center):
 *     (1) outer steel wall \n
 *     (2) layer 2 on inner steel wall \n
 *     (3) layer 1 on inner steel wall \n
 *     (4) inner steel wall \n
 *     (5) cryoliquid
 *
 *   It holds pointers to all \n
 *      - solids, \n
 *      - logical Volumes \n
 *      - physical Volumes \n
 *   belonging to the cryostat.
 *
 *   It owns all of these cryostat objects.
 *
 *
 * AUTHOR: Jens Schubert
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-05-2007, first version, Jens
 *   12-10-2007, added Torlon Rods and thermal shields, Jens
 *             also added a destructor
 *   19-10-2011, heat exchanger added to cryostat as a simple cylindrical geometry, Neslihan
 */

#ifndef _GEGEOMETRYCRYOSTATPARTS_HH
#define _GEGEOMETRYCRYOSTATPARTS_HH

// //---------------------------------------------------------------------------//
//       Geant4 headers
// #include "globals.hh"
// #include "G4LogicalVolume.hh"
//
// // MG headers
// #include "GEGeometryDetectorDB.hh"
// //---------------------------------------------------------------------------//

#include "gerdageometry/GEGeometryCryostatMassiveShape.hh"


class G4VPhysicalVolume;


class GEGeometryCryostatParts{
  public:
    GEGeometryCryostatParts();
    ~GEGeometryCryostatParts();

    GEGeometryCryostatMassiveShape*  GetCryoOuterwall_Outside()      ;
    GEGeometryCryostatMassiveShape*  GetCryoOuterwall_Inside()       ;
    GEGeometryCryostatMassiveShape*  GetCryoInnerwallLayer2_Outside();
    GEGeometryCryostatMassiveShape*  GetCryoInnerwallLayer1_Outside();
    GEGeometryCryostatMassiveShape*  GetCryoInnerwall_Outside()      ;
    GEGeometryCryostatMassiveShape*  GetCryoInnerwall_Inside()       ;

    // cryostat main parts
    void                 CreateSolids();
    void                 CreateSolids_Additional();

    void                 CreateLogicalVolumes();
    void                 CreateLogicalVolumes_Additional();

    G4VPhysicalVolume*   CreatePhysicalVolumes(G4LogicalVolume* aMother, const G4ThreeVector& positionShift);
    void                 CreatePhysicalVolumes_Additional();


    //private:
  public:
    GEGeometryCryostatMassiveShape  fCryoOuterwall_Outside;
    GEGeometryCryostatMassiveShape  fCryoOuterwall_Inside;
    GEGeometryCryostatMassiveShape  fCryoInnerwallLayer2_Outside;
    GEGeometryCryostatMassiveShape  fCryoInnerwallLayer1_Outside;
    GEGeometryCryostatMassiveShape  fCryoInnerwall_Outside;
    GEGeometryCryostatMassiveShape  fCryoInnerwall_Inside;


    // Additional things
    G4VSolid* fCryoGasInNeckSolid;
    G4VSolid* fThermalShieldInNeckSolid;
    G4VSolid* fHeatExchangerSolid;
    G4VSolid* fShroudSolid;
    G4VSolid* fSolidCuLayer;
    G4VSolid* fSolidCuLayerSupportRing;
    G4VSolid* fSolidTorlonBigRod;
    G4VSolid* fSolidTorlonCenteringRodTop;
    G4VSolid* fSolidTorlonCenteringRodBot;

    G4LogicalVolume* fCryoGasInNeckLogical;
    G4LogicalVolume* fThermalShieldInNeckLogical;
    G4LogicalVolume* fHeatExchangerLogical;
    G4LogicalVolume* fShroudLogical;
    G4LogicalVolume* fCuLayerLogical;
    G4LogicalVolume* fCuLayerSupportRingLogical;
    G4LogicalVolume* fTorlonBigRodLogical;
    G4LogicalVolume* fTorlonCenteringRodTopLogical;
    G4LogicalVolume* fTorlonCenteringRodBotLogical;

    G4VPhysicalVolume* fCuLayerPhysical;
    G4VPhysicalVolume* fCuLayerSupportRingPhysical;
    G4VPhysicalVolume* fCryoGasInNeckPhysical;
    G4VPhysicalVolume* fThermalShieldInNeckPhysical;
    G4VPhysicalVolume* fHeatExchangerPhysical;
    G4VPhysicalVolume* fShroudPhysical;
    std::vector<G4VPhysicalVolume*> fTorlonBigRodPhysicalContainer;
    std::vector<G4VPhysicalVolume*> fTorlonCenteringRodTopPhysicalContainer;
    std::vector<G4VPhysicalVolume*> fTorlonCenteringRodBotPhysicalContainer;


  public:
    G4double Distance_TopLidOuterWall_TopLidInnerWall;
    G4double CryoSlopeLid        ;
    G4double CryoSlopeLid_AsAngle;

    G4double ThicknessInnerWallTopLid         ;
    G4double ThicknessInnerWallBotLid         ;
    G4double ThicknessInnerWallNeck           ;
    G4double ThicknessInnerWallCyl            ;
    G4double ThicknessInnerWallLittlecylBarrel;
    G4double ThicknessInnerWallLittlecylBottom;

    G4double ThicknessOuterWallTopLid         ;
    G4double ThicknessOuterWallBotLid         ;
    G4double ThicknessOuterWallNeck           ;
    G4double ThicknessOuterWallCyl            ;
    G4double ThicknessOuterWallLittlecylBarrel;
    G4double ThicknessOuterWallLittlecylBottom;

    G4double ThicknessInnerWallLayer1         ;
    G4double ThicknessInnerWallLayer2         ;


    // Cu Layer Support ring
    G4double CuLayerSupportRing_Thickness             ;
    G4double CuLayerSupportRing_InnerRadius           ;
    G4double CuLayerSupportRing_OuterRadius           ;
    G4double CuLayerSupportRing_zPosRelToUpperEdgeCryo;

    // Cu Layer
    G4double CuLayer_NPlatesInPhi   ;
    G4double CuLayer_PlateWidth     ;
    G4double CuLayer_PlateThickness ;
    G4double CuLayerTop_Height1     ;
    G4double CuLayerTop_Height2     ;
    G4double CuLayerBot_Height      ;

    // gas in neck
    G4double CryogasHeightInNeck;

    // thermal shield in neck
    G4double ThermalShieldInNeck_InnerRadius;
    G4double ThermalShieldInNeck_Thickness;
    G4double ThermalShieldInNeck_DistanceToUpperEdgeOfNeck;

    // heat exchanger
    G4double HeatExchanger_Height;
    G4double HeatExchanger_Radius;
    G4double HeatExchanger_Thickness;
    G4double HeatExchanger_InnerRadius;
    G4double HeatExchanger_DistanceToLArFillLevel;

    // shroud
    G4double Shroud_Height;
    G4double Shroud_Radius;
    G4double Shroud_Thickness;

    // Torlon Rods
    G4double TorlonBigRod_RadiusOfPositionInTank;
    G4double TorlonBigRod_InnerRadius;
    G4double TorlonBigRod_OuterRadius;
    G4double TorlonBigRod_Height;

    G4double TorlonCenteringRodTop_ZPositionRelToInnerNeckbottom;
    G4double TorlonCenteringRodTop_RadiusOfPositionInTank;
    G4double TorlonCenteringRodTop_InnerRadius;
    G4double TorlonCenteringRodTop_OuterRadius;
    G4double TorlonCenteringRodTop_Height;

    G4double TorlonCenteringRodBot_ZPositionRelToLittlecylbottom;
    G4double TorlonCenteringRodBot_RadiusOfPositionInTank;
    G4double TorlonCenteringRodBot_InnerRadius;
    G4double TorlonCenteringRodBot_OuterRadius;
    G4double TorlonCenteringRodBot_Height;


    // Materials
    G4Material* fMaterialCuLayer;
    G4Material* fMaterialCuLayerSupportRing;
    G4Material* fMaterialCryoGas;
    G4Material* fMaterialTorlonRods;
    G4Material* fMaterialThermalShield;
    G4Material* fMaterialHeatExchanger;
  G4Material* fMaterialShroud;
};

#endif

