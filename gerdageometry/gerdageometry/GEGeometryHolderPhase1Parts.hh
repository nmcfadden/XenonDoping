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
 * CLASS DECLARATION:  GEGeometryHolderPhase1Parts.hh
 *
 * DESCRIPTION:
 *
 *  A class that contain all
 *    - parameters \n
 *    - logical volumes \n
 *    - physical volumes \n
 *  of all parts of the Phase 1 holders.
 *
 * AUTHOR: Jens Schubert
 *
 * REVISION: MM-DD-YYYY
 *
 *   xx-04-2008, first version, Jens
 *   08-05-2008  (jens) store one more solid needed by suspension of uppermost crystal in a phase-1 string
 *   02-07-2008  (jens) add 7 more volumes namely 
 *                      UpperChineseHat, UpperSmall_1_Isolator, UpperSmall_2_SiliconGuider, 
 *                      UpperSmall_3_Isolator, UpperSmall_4_SiliconSpring, 
 *                      UpperSmall_5_Isolator, UpperSmall_6_InnerSpecialNut
 *               all of them are inside the inner part of the 'Upper Star Holder'
 */


#ifndef _GEGEOMETRYHOLDERPHASE1PARTS_HH
#define _GEGEOMETRYHOLDERPHASE1PARTS_HH


#include "GEGeometryDetectorDB.hh"


class G4UnionSolid;
class G4SubtractionSolid;
class G4Trap;
class G4LogicalVolume;
class G4Material;
class G4VisAttributes;
class G4Box;
class G4Polycone;

class GEGeometryHolderPhase1Parts {
  public:
  GEGeometryHolderPhase1Parts(G4double gap);
  GEGeometryHolderPhase1Parts(const GEGeometryHolderPhase1Parts& holder);
  ~GEGeometryHolderPhase1Parts();

    void        ResetDependentParameters();

    // Creation of solids, logical volumes, and physical volumes
    void        CreateSolids();
    void        CreateSolids_LowerStar();
    void        CreateSolids_UpperStar();
    void        CreateSolids_StarArm();
    void        CreateSolids_VerticalBar();

    void        CreateSolids_UpperPTFERing();
    void        CreateSolids_UpperSpecialNut();
    void        CreateSolids_UpperCounterNut();

    void        CreateSolids_UpperChineseHat();
    void        CreateSolids_UpperSmall_1_Isolator();
    void        CreateSolids_UpperSmall_2_SiliconGuider();
    void        CreateSolids_UpperSmall_3_Isolator();
    void        CreateSolids_UpperSmall_4_SiliconSpring();
    void        CreateSolids_UpperSmall_5_Isolator();
    void        CreateSolids_UpperSmall_6_InnerSpecialNut();

    void        CreateSolids_LowerContactDisc();
    void        CreateSolids_LowerPTFETube();
    void        CreateSolids_LowerPressurePlate();
    void        CreateSolids_LowerGrubScrew();

    void        CreateLogicalVolumes();
    void        CreateLogicalVolumes_LowerStar();
    void        CreateLogicalVolumes_UpperStar();
    void        CreateLogicalVolumes_VerticalBar();

    void        CreateLogicalVolumes_UpperSpecialNut();
    void        CreateLogicalVolumes_UpperCounterNut();
    void        CreateLogicalVolumes_UpperPTFERing();

    void        CreateLogicalVolumes_UpperChineseHat();
    void        CreateLogicalVolumes_UpperSmall_1_Isolator();
    void        CreateLogicalVolumes_UpperSmall_2_SiliconGuider();
    void        CreateLogicalVolumes_UpperSmall_3_Isolator();
    void        CreateLogicalVolumes_UpperSmall_4_SiliconSpring();
    void        CreateLogicalVolumes_UpperSmall_5_Isolator();
    void        CreateLogicalVolumes_UpperSmall_6_InnerSpecialNut();

    void        CreateLogicalVolumes_LowerContactDisc();
    void        CreateLogicalVolumes_LowerPTFETube();
    void        CreateLogicalVolumes_LowerPressurePlate();
    void        CreateLogicalVolumes_LowerGrubScrew();

    void        CreatePhysicalVolume(G4LogicalVolume* motherVol, G4int col, G4int row, 
				     G4int ffHolderGeometry1Number, const G4ThreeVector& crystalPos, 
				     G4bool BEGeFlag);
    void        CreatePhysicalVolume_NoUpperPTFERing (G4LogicalVolume* motherVol, G4int col, 
						      G4int row, G4int ffHolderGeometry1Number, 
						      const G4ThreeVector& crystalPos);

    // Getters
    G4double    GetZDistanceToSuspensionPlaneTop() const;
    G4double    GetZDistanceToSuspensionPlaneBottom() const;
    G4double    Get_UpperSmall4SiliconSpring_LowerEdge() const;
    G4double    Get_UpperSmall4SiliconSpring_UpperEdge() const;
    G4double    Get_UpperSmall4SiliconSpring_Height() const;

  void ResetParametersForAPair(GEGeometryDetectorDB* db,G4int col,
			       G4int row);
    
    G4SubtractionSolid* GetSuspensionTube_TShape() const
    { return fSolid_LowerStarTube;};

  void SetMaximalAllowedCrystalRadius(G4double maxr){fMaximalAllowedCrystalRadius = maxr;};
  void SetRadialDistance_CrystalSurface_Holder(G4double rad){fRadialDistance_CrystalSurface_Holder = rad;};
  void SetCrystalRadius(G4double rad){fCrystalRadius = rad;};
  void SetCrystalInnerRadius(G4double rad){fCrystalInnerRadius = rad;};
  void SetCrystalHeight(G4double hei){fCrystalHeight = hei;};
  void SetCrystalID(G4int id){fCrystalID = id;};
  
  void SetVisAtt_LowerStar(G4VisAttributes* visa){fVisAtt_LowerStar = visa;};
  void SetVisAtt_UpperStar(G4VisAttributes* visa){fVisAtt_UpperStar = visa;};
  void SetVisAtt_starArm(G4VisAttributes* visa){fVisAtt_starArm = visa;};
  void SetVisAtt_VerticalBar(G4VisAttributes* visa){fVisAtt_VerticalBar = visa;};
  void SetVisAtt_LowerContactDisc(G4VisAttributes* visa){fVisAtt_LowerContactDisc = visa;};
  void SetVisAtt_LowerPTFETube(G4VisAttributes* visa){fVisAtt_LowerPTFETube = visa;};
  void SetVisAtt_LowerPressurePlate(G4VisAttributes* visa){fVisAtt_LowerPressurePlate = visa;};
  void SetVisAtt_LowerGrubScrew(G4VisAttributes* visa){fVisAtt_LowerGrubScrew = visa;};
  void SetVisAtt_UpperSpecialNut(G4VisAttributes* visa){fVisAtt_UpperSpecialNut = visa;};
  void SetVisAtt_UpperCounterNut(G4VisAttributes* visa){fVisAtt_UpperCounterNut = visa;};
  void SetVisAtt_UpperPTFERing(G4VisAttributes* visa){fVisAtt_UpperPTFERing = visa;};
  void SetVisAtt_UpperChineseHat(G4VisAttributes* visa){fVisAtt_UpperChineseHat = visa;};
  void SetVisAtt_UpperSmall_1_Isolator(G4VisAttributes* visa){fVisAtt_UpperSmall_1_Isolator = visa;};
  void SetVisAtt_UpperSmall_2_SiliconGuider(G4VisAttributes* visa){fVisAtt_UpperSmall_2_SiliconGuider =
      visa;};
  void SetVisAtt_UpperSmall_3_Isolator(G4VisAttributes* visa){fVisAtt_UpperSmall_3_Isolator = visa;};
  void SetVisAtt_UpperSmall_4_SiliconSpring(G4VisAttributes* visa){fVisAtt_UpperSmall_4_SiliconSpring 
      = visa;};  
  void SetVisAtt_UpperSmall_5_Isolator(G4VisAttributes* visa){fVisAtt_UpperSmall_5_Isolator = visa;};
  void SetVisAtt_UpperSmall_6_InnerSpecialNut(G4VisAttributes* visa){fVisAtt_UpperSmall_6_InnerSpecialNut 
    = visa;};

  
  void SetMaterial_LowerStar(G4Material* mate){fMaterial_LowerStar = mate;};
  void SetMaterial_UpperStar(G4Material* mate){fMaterial_UpperStar = mate;};
  void SetMaterial_starArm(G4Material* mate){fMaterial_starArm = mate;};
  void SetMaterial_VerticalBar(G4Material* mate){fMaterial_VerticalBar = mate;};
  void SetMaterial_LowerContactDisc(G4Material* mate){fMaterial_LowerContactDisc = mate;};
  void SetMaterial_LowerPTFETube(G4Material* mate){fMaterial_LowerPTFETube = mate;};
  void SetMaterial_LowerPressurePlate(G4Material* mate){fMaterial_LowerPressurePlate = mate;};
  void SetMaterial_LowerGrubScrew(G4Material* mate){fMaterial_LowerGrubScrew = mate;};
  void SetMaterial_UpperSpecialNut(G4Material* mate){fMaterial_UpperSpecialNut = mate;};
  void SetMaterial_UpperCounterNut(G4Material* mate){fMaterial_UpperCounterNut = mate;};
  void SetMaterial_UpperPTFERing(G4Material* mate){fMaterial_UpperPTFERing = mate;};
  void SetMaterial_UpperChineseHat(G4Material* mate){fMaterial_UpperChineseHat = mate;};
  void SetMaterial_UpperSmall_1_Isolator(G4Material* mate){fMaterial_UpperSmall_1_Isolator = mate;};
  void SetMaterial_UpperSmall_2_SiliconGuider(G4Material* mate){fMaterial_UpperSmall_2_SiliconGuider =
      mate;};
  void SetMaterial_UpperSmall_3_Isolator(G4Material* mate){fMaterial_UpperSmall_3_Isolator = mate;};
  void SetMaterial_UpperSmall_4_SiliconSpring(G4Material* mate){fMaterial_UpperSmall_4_SiliconSpring 
      = mate;};  
  void SetMaterial_UpperSmall_5_Isolator(G4Material* mate){fMaterial_UpperSmall_5_Isolator = mate;};
  void SetMaterial_UpperSmall_6_InnerSpecialNut(G4Material* mate){fMaterial_UpperSmall_6_InnerSpecialNut 
    = mate;};

  G4LogicalVolume* GetLogicalVol_LowerStar(){return fLogicalVol_LowerStar;};
  G4LogicalVolume* GetLogicalVol_UpperStar(){return fLogicalVol_UpperStar;};
  G4LogicalVolume* GetLogicalVol_starArm(){return fLogicalVol_starArm;};
  G4LogicalVolume* GetLogicalVol_VerticalBar(){return fLogicalVol_VerticalBar;};
  G4LogicalVolume* GetLogicalVol_LowerContactDisc(){return fLogicalVol_LowerContactDisc;};
  G4LogicalVolume* GetLogicalVol_LowerPTFETube(){return fLogicalVol_LowerPTFETube;};
  G4LogicalVolume* GetLogicalVol_LowerPressurePlate(){return fLogicalVol_LowerPressurePlate;};
  G4LogicalVolume* GetLogicalVol_LowerGrubScrew(){return fLogicalVol_LowerGrubScrew;};
  G4LogicalVolume* GetLogicalVol_UpperSpecialNut(){return fLogicalVol_UpperSpecialNut;};
  G4LogicalVolume* GetLogicalVol_UpperCounterNut(){return fLogicalVol_UpperCounterNut;};
  G4LogicalVolume* GetLogicalVol_UpperPTFERing(){return fLogicalVol_UpperPTFERing;};
  G4LogicalVolume* GetLogicalVol_UpperChineseHat(){return fLogicalVol_UpperChineseHat;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_1_Isolator(){return fLogicalVol_UpperSmall_1_Isolator;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_2_SiliconGuider(){return fLogicalVol_UpperSmall_2_SiliconGuider;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_3_Isolator(){return fLogicalVol_UpperSmall_3_Isolator;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_4_SiliconSpring(){return fLogicalVol_UpperSmall_4_SiliconSpring;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_5_Isolator(){return fLogicalVol_UpperSmall_5_Isolator;};
  G4LogicalVolume* GetLogicalVol_UpperSmall_6_InnerSpecialNut()
  {return fLogicalVol_UpperSmall_6_InnerSpecialNut;};

  G4double GetZzHldStarLowerTube(G4int i){return fZzHldStarLowerTube[i];};
  G4double GetZzHldStarUpperTube(G4int i){return fZzHldStarUpperTube[i];};
  G4double GetRoHldStarLowerTube(G4int i){return fRoHldStarLowerTube[i];};
  G4double GetMaximalAllowedCrystalRadius(){return fMaximalAllowedCrystalRadius;};
  G4double GetRadialDistance_CrystalSurface_Holder(){return fRadialDistance_CrystalSurface_Holder;};
  G4double GetBotDistance_CrystalSurface_Holder(){return fBotDistance_CrystalSurface_Holder;};
  G4double GetTopDistance_CrystalSurface_Holder(){return fTopDistance_CrystalSurface_Holder;};
  G4double GetCrystalRadius(){return fCrystalRadius;};
  G4double GetCrystalInnerRadius(){return fCrystalInnerRadius;};
  G4double GetCrystalHeight(){return fCrystalHeight;};
  G4int GetCrystalID(){return fCrystalID;};
  G4double GetZOverlap_UpperStarTube_SpecialNut(){return fZOverlap_UpperStarTube_SpecialNut;};
  G4double GetSpclNut_ZPosOfSuspensionHoleTop(){return fSpclNut_ZPosOfSuspensionHoleTop;};

private:

    // data members
    // ----------------

    // parameters probably beeing reset before creating solids and volumes
    G4double fMaximalAllowedCrystalRadius;
    G4double fRadialDistance_CrystalSurface_Holder;
    G4double fTopDistance_CrystalSurface_Holder;
    G4double fBotDistance_CrystalSurface_Holder;
    G4double fCrystalRadius;
    G4double fCrystalInnerRadius;
    G4double fCrystalHeight;
    G4int    fCrystalID;

    // parameters upper star tube
    G4double fZzHldStarUpperTube[8];
    G4double fRiHldStarUpperTube[8];
    G4double fRoHldStarUpperTube[8];

    // parameters lower star tube
    G4double fZzHldStarLowerTube[8];
    G4double fRiHldStarLowerTube[8];
    G4double fRoHldStarLowerTube[8];

    // starArmParameters
    G4double fStarArm_pDz    ;
    G4double fStarArm_pDy1   ;
    G4double fStarArm_pDx1   ;
    G4double fStarArm_pDx2   ;
    G4double fStarArm_pAlp1  ;
    G4double fStarArm_pDy2   ;
    G4double fStarArm_pDx3   ;
    G4double fStarArm_pDx4   ;
    G4double fStarArm_pAlp2  ;
    G4double fStarArm_pTheta ;
    G4double fStarArm_pPhi   ;
    G4double fStarArm_meanOfCenterTubeRadius;

    // parameters special nut
    G4double fZzSpclNut[10];
    G4double fRiSpclNut[10];
    G4double fRoSpclNut[10];

    G4double fSpclNut_SubtrAlongY1_Zstart;
    G4double fSpclNut_SubtrAlongY1_Width;

    G4double fSpclNut_SubtrAlongY2_Zstart;
    G4double fSpclNut_SubtrAlongY2_Width;

    G4double fSpclNut_SubtrAlongX_deltaZ;
    G4double fSpclNut_SubtrAlongX_deltaY;
    G4double fSpclNut_ZPosOfSuspensionHoleTop;

    G4double fZOverlap_UpperStarTube_SpecialNut;

    // parameters for counter nut
    G4double fCounterNut_Ri;
    G4double fCounterNut_Ro;
    G4double fCounterNut_h ;

    // parameters for vertical bars
    G4double fVerticalBar_dx;
    G4double fVerticalBar_dy;

    // parameters for upper PTFE ring 
    G4double fUpperPTFERing_Ro;
    G4double fUpperPTFERing_dR;
    G4double fUpperPTFERing_Ri;
    G4double fUpperPTFERing_h ;

    // parameters for lower Contact disc
    G4double fLowerContactDisc_Ro;
    G4double fLowerContactDisc_h ;

    // parameters for lower PTFE tube 
    G4double fLowerPTFETube_Ro;
    G4double fLowerPTFETube_Ri;
    G4double fLowerPTFETube_h ;

    // parameters for lower Contact disc
    G4double fLowerPressurePlate_Ro;
    G4double fLowerPressurePlate_h ;

    // parameters for lower Contact disc
    G4double fLowerGrubScrew_Ro;
    G4double fLowerGrubScrew_h ;



    //  parameters of parts inside the upper star holder
    // ------------------------------------------------

    G4double        fUpperChineseHat_totalHeight;
    G4double        fUpperChineseHat_CylinderInnerRadius;
    G4double        fUpperChineseHat_CylinderOuterRadius;
    G4double        fUpperChineseHat_ContactConeHeight;
    G4double        fUpperChineseHat_ContactConeDeltaRadius;
    G4double        fUpperChineseHat_ContactConeRadius1;
    G4double        fUpperChineseHat_ContactConeRadius2;

    G4double        fUpperSmall_1_Isolator_h;
    G4double        fUpperSmall_1_Isolator_Ri;
    G4double        fUpperSmall_1_Isolator_Ro;

    G4double        fUpperSmall_2_SiliconGuider_h;
    G4double        fUpperSmall_2_SiliconGuider_Ri;
    G4double        fUpperSmall_2_SiliconGuider_Ro;

    G4double        fUpperSmall_3_Isolator_h;
    G4double        fUpperSmall_3_Isolator_Ri;
    G4double        fUpperSmall_3_Isolator_Ro;

    G4double        fUpperSmall_4_SiliconSpring_h;
    G4double        fUpperSmall_4_SiliconSpring_Ri;
    G4double        fUpperSmall_4_SiliconSpring_Ro;

    G4double        fUpperSmall_5_Isolator_h;
    G4double        fUpperSmall_5_Isolator_Ri;
    G4double        fUpperSmall_5_Isolator_Ro;

    G4double        fUpperSmall_6_InnerSpecialNut_h;
    G4double        fUpperSmall_6_InnerSpecialNut_Ri;
    G4double        fUpperSmall_6_InnerSpecialNut_Ro;



    // solides
    G4UnionSolid*         fSolid_LowerStar;
    G4SubtractionSolid*   fSolid_LowerStarTube;
    G4UnionSolid*         fSolid_UpperStar;
    G4Trap*               fSolid_starArm;
    G4Box*                fSolid_VerticalBar;
    G4Tubs*               fSolid_LowerContactDisc;
    G4Tubs*               fSolid_LowerPTFETube;
    G4Tubs*               fSolid_LowerPressurePlate;
    G4Tubs*               fSolid_LowerGrubScrew;
    G4SubtractionSolid*   fSolid_UpperSpecialNut;
    G4Tubs*               fSolid_UpperCounterNut;
    G4Tubs*               fSolid_UpperPTFERing;

    G4Polycone*           fSolid_UpperChineseHat;
    G4Tubs*               fSolid_UpperSmall_1_Isolator;
    G4Tubs*               fSolid_UpperSmall_2_SiliconGuider;
    G4Tubs*               fSolid_UpperSmall_3_Isolator;
    G4Tubs*               fSolid_UpperSmall_4_SiliconSpring;
    G4Tubs*               fSolid_UpperSmall_5_Isolator;
    G4Tubs*               fSolid_UpperSmall_6_InnerSpecialNut;


    // vis attributes
    G4VisAttributes* fVisAtt_LowerStar;
    G4VisAttributes* fVisAtt_UpperStar;
    G4VisAttributes* fVisAtt_starArm;
    G4VisAttributes* fVisAtt_VerticalBar;
    G4VisAttributes* fVisAtt_LowerContactDisc;
    G4VisAttributes* fVisAtt_LowerPTFETube;
    G4VisAttributes* fVisAtt_LowerPressurePlate;
    G4VisAttributes* fVisAtt_LowerGrubScrew;
    G4VisAttributes* fVisAtt_UpperSpecialNut;
    G4VisAttributes* fVisAtt_UpperCounterNut;
    G4VisAttributes* fVisAtt_UpperPTFERing;

    G4VisAttributes* fVisAtt_UpperChineseHat;
    G4VisAttributes* fVisAtt_UpperSmall_1_Isolator;
    G4VisAttributes* fVisAtt_UpperSmall_2_SiliconGuider;
    G4VisAttributes* fVisAtt_UpperSmall_3_Isolator;
    G4VisAttributes* fVisAtt_UpperSmall_4_SiliconSpring;
    G4VisAttributes* fVisAtt_UpperSmall_5_Isolator;
    G4VisAttributes* fVisAtt_UpperSmall_6_InnerSpecialNut;

    // materials
    G4Material* fMaterial_LowerStar;
    G4Material* fMaterial_UpperStar;
    G4Material* fMaterial_starArm;
    G4Material* fMaterial_VerticalBar;
    G4Material* fMaterial_LowerContactDisc;
    G4Material* fMaterial_LowerPTFETube;
    G4Material* fMaterial_LowerPressurePlate;
    G4Material* fMaterial_LowerGrubScrew;
    G4Material* fMaterial_UpperSpecialNut;
    G4Material* fMaterial_UpperCounterNut;
    G4Material* fMaterial_UpperPTFERing;

    G4Material* fMaterial_UpperChineseHat;
    G4Material* fMaterial_UpperSmall_1_Isolator;
    G4Material* fMaterial_UpperSmall_2_SiliconGuider;
    G4Material* fMaterial_UpperSmall_3_Isolator;
    G4Material* fMaterial_UpperSmall_4_SiliconSpring;
    G4Material* fMaterial_UpperSmall_5_Isolator;
    G4Material* fMaterial_UpperSmall_6_InnerSpecialNut;


    // logical volumes
    G4LogicalVolume* fLogicalVol_LowerStar;
    G4LogicalVolume* fLogicalVol_UpperStar;
    G4LogicalVolume* fLogicalVol_starArm;
    G4LogicalVolume* fLogicalVol_VerticalBar;
    G4LogicalVolume* fLogicalVol_LowerContactDisc;
    G4LogicalVolume* fLogicalVol_LowerPTFETube;
    G4LogicalVolume* fLogicalVol_LowerPressurePlate;
    G4LogicalVolume* fLogicalVol_LowerGrubScrew;
    G4LogicalVolume* fLogicalVol_UpperSpecialNut;
    G4LogicalVolume* fLogicalVol_UpperCounterNut;
    G4LogicalVolume* fLogicalVol_UpperPTFERing;

    G4LogicalVolume* fLogicalVol_UpperChineseHat;
    G4LogicalVolume* fLogicalVol_UpperSmall_1_Isolator;
    G4LogicalVolume* fLogicalVol_UpperSmall_2_SiliconGuider;
    G4LogicalVolume* fLogicalVol_UpperSmall_3_Isolator;
    G4LogicalVolume* fLogicalVol_UpperSmall_4_SiliconSpring;
    G4LogicalVolume* fLogicalVol_UpperSmall_5_Isolator;
    G4LogicalVolume* fLogicalVol_UpperSmall_6_InnerSpecialNut;

  static G4double SmallNumber; // this is to make drawings with WIRED nicer

  G4int fNumberOfPhysicalVolumesSets[100]; 
  // there are 14 phase-1-type crystals, but their ID could be mixed with the IDs of phase-2-type crystals 
  // '100' takes into account crystal IDs from '0' up to '99'

  G4double fCrystalPairGap;
  G4bool fIsAPair;
};


#endif

