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
 *   10-09-2006   Add one REGe at 70 degrees, Xiang Liu
 *   06-16-2010   Added +CollimatorHeight(SourceMotherVolume half height) to R(if rotated) or Z(if not rotated)
 *                   to source position so that the distance between source
 *                   and vacuum can (not collimator center and can) is as pointed in macro.   Alex.
 *   10-14-2010   When using the collimator at the side, then there might be another
 *                   ~11 cm of material as implemented by SourceHolderStick now.     Alex.
 *   11-23-2010   Fixed some minor bugs and "features" about the holder stick position. Alex
 *   02-17-2011   Changed color of the holder stick and commented out the preamp ring volume. Alex
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDManager.hh"

#include "munichteststand/GEMunichTestStandSiegfried.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandSiegfried::GEMunichTestStandSiegfried(GEMunichTestStandDB* teststanddb)
{

  fMotherVolume = teststanddb -> GetMotherVolume(); 
  teststanddb -> ConstructREGeDetector(0,18);
  teststanddb -> ConstructREGeDetector(1,19);
  fREGeLogical[0] = teststanddb -> GetREGeLogical(0);
  fREGeLogical[1] = teststanddb -> GetREGeLogical(1);

  fCrystalDeadLayerLogical   = teststanddb -> GetSiegfriedCrystalDeadLayerLogical();
  fCrystalLogical            = teststanddb -> GetSiegfriedCrystalLogical();
  fCrystalSiliconLogical     = teststanddb -> GetSiegfriedCrystalSiliconLogical();
  fCanLogical                = teststanddb -> GetSiegfriedCanLogical();
  fVacuumLogical             = teststanddb -> GetSiegfriedVacuumLogical();
  fIRCanLogical              = teststanddb -> GetSiegfriedIRCanLogical();
  fIRBaseLogical             = teststanddb -> GetSiegfriedIRBaseLogical();
  fRodLogical                = teststanddb -> GetSiegfriedRodLogical();
  fBaseLogical               = teststanddb -> GetSiegfriedBaseLogical();
  fBaseRodLogical            = teststanddb -> GetSiegfriedBaseRodLogical();
  fCoolingFingerLogical      = teststanddb -> GetSiegfriedCoolingFingerLogical();
  fDewarNeckLogical          = teststanddb -> GetSiegfriedDewarNeckLogical();
  fDewarExpandLogical        = teststanddb -> GetSiegfriedDewarExpandLogical();
  fDewarLogical              = teststanddb -> GetSiegfriedDewarLogical();
  fFanoutBoxLogical          = teststanddb -> GetSiegfriedFanoutBoxLogical();
  //fPreampBoxLogical          = teststanddb -> GetSiegfriedPreampBoxLogical();
  fPreampRingLogical         = teststanddb -> GetSiegfriedPreampRingLogical();
  fLN2Logical                = teststanddb -> GetSiegfriedLN2Logical();
  fSourceHolderLogical       = teststanddb -> GetSiegfriedSourceHolderLogical();
  fSourceLogical             = teststanddb -> GetSiegfriedSourceLogical();
  fSourceHolderAEALogical    = teststanddb -> GetSourceHolderAEALogical();
  fSourceAEALogical          = teststanddb -> GetSourceAEALogical();
  fPreTubLogical             = teststanddb -> GetSiegfriedPreTubLogical();
  fPreBottomLogical          = teststanddb -> GetSiegfriedPreBottomLogical();
  fCollimatorLogical         = teststanddb -> GetSiegfriedCollimatorLogical();
  fSourceMotherVolume        = teststanddb -> GetSiegfriedSourceMotherVolume();
  fSourceHolderStickLogical  = teststanddb -> GetSiegfriedSourceHolderStickLogical();
  //--------------------------------------------------------------------
  // Z positions of all components
  //--------------------------------------------------------------------
  fCan_CenterZ               = teststanddb -> GetSiegfriedCanCenterZ();
  fIRCan_CenterZ             = teststanddb -> GetSiegfriedIRCanCenterZ();
  fIRBase_CenterZ            = teststanddb -> GetSiegfriedIRBaseCenterZ();
  fRod_CenterZ               = teststanddb -> GetSiegfriedRodCenterZ();
  fBase_CenterZ              = teststanddb -> GetSiegfriedBaseCenterZ();
  fBaseRod_CenterZ           = teststanddb -> GetSiegfriedBaseRodCenterZ();
  fCoolingFinger_CenterZ     = teststanddb -> GetSiegfriedCoolingFingerCenterZ();
  fDewarNeck_CenterZ         = teststanddb -> GetSiegfriedDewarNeckCenterZ();
  fDewarExpand_CenterZ       = teststanddb -> GetSiegfriedDewarExpandCenterZ();
  fDewar_CenterZ             = teststanddb -> GetSiegfriedDewarCenterZ();
  fLN2_CenterZ               = teststanddb -> GetSiegfriedLN2CenterZ();
  fFanoutBox_OffsetY         = teststanddb -> GetSiegfriedFanoutBoxOffsetY();
  //fPreampBox_lengthX         = teststanddb -> GetSiegfriedPreampBoxlengthX();
  fPreampRing_OuterRadius    = teststanddb -> GetSiegfriedPreampRingOuterRadius();
  fPreTub_CenterY            = teststanddb -> GetSiegfriedPreTubCenterY();
  fPreBottom_CenterY         = teststanddb -> GetSiegfriedPreBottomCenterY();
  //--------------------------------------------------------------------
  // geometry parameter
  // Vacuum volume which contains Can, Crystal and Rod
  // Vacuum has the same size as Can
  //--------------------------------------------------------------------
  fCrystal_InnerRadius     = teststanddb -> GetSiegfriedCrystalInnerRadius();
  fCrystal_OuterRadius     = teststanddb -> GetSiegfriedCrystalOuterRadius();
  fCrystal_Height          = teststanddb -> GetSiegfriedCrystalHeight();
  fCrystal_SiliconThickness= teststanddb -> GetSiegfriedCrystalSiliconThickness();

  fCan_Height              = teststanddb -> GetSiegfriedCanHeight();
  fCan_OuterRadius         = teststanddb -> GetSiegfriedCanOuterRadius();
  fRod_Height              = teststanddb -> GetSiegfriedRodHeight();

  useCollimator            = teststanddb -> GetSiegfriedCollimatorStatus();
  fCollimator_Height       = teststanddb -> GetSiegfriedCollimatorHeight();
  fCollimatorHole_UpperHeight = teststanddb -> GetSiegfriedCollimatorHoleUpperHeight();

  //source system
  fSourceSystem_Z          = teststanddb -> GetSiegfriedSourceSystemZ();
  fSourceSystem_R          = teststanddb -> GetSiegfriedSourceSystemR();
  fSourceSystem_Phi        = teststanddb -> GetSiegfriedSourceSystemPhi();

  fSourceHolder_Height     = teststanddb -> GetSiegfriedSourceHolderHeight();
  fSourceHolder_InnerRadius= teststanddb -> GetSiegfriedSourceHolderInnerRadius();
  fSourceHolder_OuterRadius= teststanddb -> GetSiegfriedSourceHolderOuterRadius();
  fSourceHolder_DistanceToCanTopSurface 
                           = teststanddb -> GetSiegfriedSourceHolderDistanceToCanTopSurface();
  fSourceHolder_rPosition  = teststanddb -> GetSiegfriedSourceHolderrPosition();
  fSourceHolder_phiPosition= teststanddb -> GetSiegfriedSourceHolderphiPosition();
  //fSourceHolder_xPosition  = fSourceHolder_rPosition * cos(fSourceHolder_phiPosition);
  //fSourceHolder_yPosition  = fSourceHolder_rPosition * sin(fSourceHolder_phiPosition);
  fSourceHolder_zPosition  = teststanddb -> GetSiegfriedSourceHolderzPosition();
  fSource_Radius           = fSourceHolder_InnerRadius;
  fSource_Height           = fSourceHolder_InnerRadius;

  useSourceHolderStick     = teststanddb -> GetSiegfriedSourceHolderStickStatus();
  fSourceHolderStickLength = teststanddb -> GetSiegfriedSourceHolderStickLength();

  fSourceHolder_zLimit     = fCan_Height*0.5 + fSourceHolder_Height*0.5;

  //AEA source:----------------------------------------------------------------------
  fSourceHolderAEA_rPosition  = teststanddb -> GetSiegfriedSourceHolderAEArPosition();
  fSourceHolderAEA_phiPosition= teststanddb -> GetSiegfriedSourceHolderAEAphiPosition();
  fSourceHolderAEA_zPosition  = teststanddb -> GetSiegfriedSourceHolderAEAzPosition();

  fSourceHolderHeightAEA      = teststanddb -> GetSourceHolderHeightAEA();
  fSourceHeightAEA            = teststanddb -> GetSourceHeightAEA();

  fSourceHolderAEA_zLimit     = fCan_CenterZ + 0.5 * (fCan_Height + fSourceHolderHeightAEA);
  //-----------------------------------------------------------------------------------

  fCoincidence_Flag = false;
// at the moment the two reges are put along x axis

  fREGe_Height  =  teststanddb->GetREGeHeight();
  fREGe_Radius  =  teststanddb->GetREGeRadius();
  fREGe_DistanceToCanOuterSurface = teststanddb -> GetSiegfriedREGeToCanSurfaceDistance();
}

// --------------------------------------------------------------------------- 

GEMunichTestStandSiegfried::~GEMunichTestStandSiegfried()
{
/*
  delete fCanPhysical; 
  delete fVacuumPhysical; 
  delete fIRCanPhysical; 
  delete fIRBasePhysical; 
  delete fRodPhysical; 

  delete fBasePhysical; 
  delete fBaseRodPhysical; 

  delete fCoolingFingerPhysical; 
  delete fDewarNeckPhysical; 
  delete fDewarExpandPhysical; 
  delete fDewarPhysical; 
  delete fLN2Physical; 

  for (int i=0; i<=2; i++) delete fFanoutBoxPhysical[i];
  delete fPreTubPhysical;
  delete fPreBottomPhysical;
  delete fPreampRingPhysical;

  delete fCrystalDeadLayerPhysical; 
  delete fCrystalSiliconTopPhysical; 
  delete fCrystalSiliconBottomPhysical; 

  delete fSourceMotherPhysical;
  delete fCollimatorPhysical;
  delete fSourceHolderAEAPhysical;
  delete fSourceAEAPhysical;

  delete fREGePhysical[0];
  delete fREGePhysical[1];
  */
} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandSiegfried::ConstructTestStand()
{
  // --------------------------------------------------------------------------- 
  // colors 
  // --------------------------------------------------------------------------- 

  G4Colour red     (1.0,  0.0,  0.0,  0.5); 
  G4Colour darkred (0.5,  0.0,  0.0,  0.5); 
  G4Colour orange  (0.75, 0.55, 0.0,  0.5);
//  G4Colour darkgrey(0.7,  0.7,  0.7,  0.5); 
  G4Colour silver  (0.2,  0.2,  0.2,  0.5); 
  G4Colour white   (1.0,  1.0,  1.0,  0.5); 
  G4Colour lgrey   (0.8,  0.8,  0.8,  0.5); 
  G4Colour lgrey2  (0.85, 0.85, 0.85, 0.5); 
  G4Colour lblue   (0.0,  0.0,  0.75, 0.5); 

  // --------------------------------------------------------------------------- 
  // visual attributes 
  // --------------------------------------------------------------------------- 

  G4VisAttributes* VacuumVisAtt = new G4VisAttributes(white); 
  VacuumVisAtt -> SetVisibility(true); 
  VacuumVisAtt -> SetForceSolid(true); 

  G4VisAttributes* AirVisAtt = new G4VisAttributes(); 
  VacuumVisAtt -> SetVisibility(false); 

  G4VisAttributes* LN2VisAtt = new G4VisAttributes(lgrey); 
  LN2VisAtt -> SetVisibility(true); 
  LN2VisAtt -> SetForceSolid(true); 

  G4VisAttributes* CrystalVisAtt = new G4VisAttributes(red);
  CrystalVisAtt -> SetVisibility(true);
  CrystalVisAtt -> SetForceSolid(true);

  G4VisAttributes* CrystalDeadLayerVisAtt = new G4VisAttributes(darkred);
  CrystalDeadLayerVisAtt -> SetVisibility(true);
  CrystalDeadLayerVisAtt -> SetForceSolid(true);

  G4VisAttributes* AlVisAtt = new G4VisAttributes(silver); 
  AlVisAtt -> SetVisibility(true); 
  AlVisAtt -> SetForceSolid(true); 

  G4VisAttributes* SiVisAtt = new G4VisAttributes(silver); 
  SiVisAtt -> SetVisibility(true); 
  SiVisAtt -> SetForceSolid(true); 

  G4VisAttributes* CuVisAtt = new G4VisAttributes(orange);
  CuVisAtt -> SetVisibility(true);
  CuVisAtt -> SetForceSolid(true);

  G4VisAttributes* DewarVisAtt = new G4VisAttributes(lblue);
  DewarVisAtt -> SetVisibility(true);
  DewarVisAtt -> SetForceSolid(true);

  G4VisAttributes* PreampVisAtt = new G4VisAttributes(lgrey2);
  PreampVisAtt -> SetVisibility(true);
  PreampVisAtt -> SetForceSolid(true);

  // --------------------------------------------------------------------------- 
  // variables 
  // --------------------------------------------------------------------------- 

//   G4double TubsInnerRadius; 
//   G4double TubsOuterRadius; 
//   G4double TubsHeight; 
//   G4double TubsStartAngle; 
//   G4double TubsSpanningAngle; 

//   G4double dPhi; 

//   G4double z; 

  // --------------------------------------------------------------------------- 
  // physical volumes 
  // --------------------------------------------------------------------------- 

  G4ThreeVector fComponentsPosition;
  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(0.0*cm);

  // can 

  fCanLogical -> SetVisAttributes(AlVisAtt); 

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fCan_CenterZ);
  fCanPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fCanLogical,
                                      "Can",
                                      fMotherVolume,
                                      false,
                                      0);


  // vacuum

  fVacuumLogical -> SetVisAttributes(VacuumVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(0.0*cm);
  fVacuumPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fVacuumLogical,
                                      "Vacuum",
                                      fCanLogical,
                                      false,
                                      0);
  // IR can

  fIRCanLogical -> SetVisAttributes(AlVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fIRCan_CenterZ);
  fIRCanPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fIRCanLogical,
                                      "IRCan",
                                      fVacuumLogical,
                                      false,
                                      0);

  // IR Base

  fIRBaseLogical -> SetVisAttributes(AlVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fIRBase_CenterZ);
  fIRBasePhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fIRBaseLogical,
                                      "IRBase",
                                      fVacuumLogical,
                                      false,
                                      0);


  // rod 

  fRodLogical -> SetVisAttributes(CuVisAtt); 

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fRod_CenterZ);
  fRodPhysical = new G4PVPlacement(0,fComponentsPosition,
                                   fRodLogical,
                                   "Rod",
                                   fVacuumLogical,
                                   false,
                                   0);

  // base

  fBaseLogical -> SetVisAttributes(AlVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fBase_CenterZ);
  fBasePhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fBaseLogical,
                                      "Base",
                                      fMotherVolume,
                                      false,
                                      0);

  // base rod

  fBaseRodLogical -> SetVisAttributes(CuVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fBaseRod_CenterZ);
  fBaseRodPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fBaseRodLogical,
                                      "BaseRod",
                                      fMotherVolume,
                                      false,
                                      0);

  // cooling finger

  fCoolingFingerLogical -> SetVisAttributes(CuVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fCoolingFinger_CenterZ);
  fCoolingFingerPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fCoolingFingerLogical,
                                      "CoolingFinger",
                                      fMotherVolume,
                                      false,
                                      0);

  // dewar neck

  fDewarNeckLogical -> SetVisAttributes(DewarVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fDewarNeck_CenterZ);
  fDewarNeckPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fDewarNeckLogical,
                                      "DewarNeck",
                                      fMotherVolume,
                                      false,
                                      0);

  // dewar expand

  fDewarExpandLogical -> SetVisAttributes(DewarVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fDewarExpand_CenterZ);
  fDewarExpandPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fDewarExpandLogical,
                                      "DewarExpand",
                                      fMotherVolume,
                                      false,
                                      0);

  // dewar 

  fDewarLogical -> SetVisAttributes(DewarVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fDewar_CenterZ);
  fDewarPhysical = new G4PVPlacement(0,fComponentsPosition,
                                      fDewarLogical,
                                      "Dewar",
                                      fMotherVolume,
                                      false,
                                      0);

  // LN2

  fLN2Logical -> SetVisAttributes(DewarVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fLN2_CenterZ);
  fLN2Physical = new G4PVPlacement(0,fComponentsPosition,
                                      fLN2Logical,
                                      "LN2",
                                      fMotherVolume,
                                      false,
                                      0);

  // preamp tubs
  // -----------

  //Main tubs
  fFanoutBoxLogical -> SetVisAttributes(AlVisAtt);
  fPreTubLogical    -> SetVisAttributes(CuVisAtt);
  fPreBottomLogical -> SetVisAttributes(CuVisAtt);

  G4RotationMatrix fPreTubRotation;
  fPreTubRotation.rotateX(-90.0 * deg);
  fPreTubRotation.rotateY(0.0 * deg);
  fPreTubRotation.rotateZ(0.0 * deg);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(fFanoutBox_OffsetY);
  fComponentsPosition.setZ(fBase_CenterZ);
  fFanoutBoxPhysical[0] 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fFanoutBoxLogical,
			"FanoutBox1",
			fMotherVolume,
			false,
			0);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(10.0 * mm + fPreTub_CenterY);
  fComponentsPosition.setZ(fBase_CenterZ);
  fPreTubPhysical 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fPreTubLogical,
			"PreTub",
			fMotherVolume,
			false,
			0);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(10.0 * mm + fPreBottom_CenterY);
  fComponentsPosition.setZ(fBase_CenterZ);

  fPreBottomPhysical 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fPreBottomLogical,
			"PreBottom",
			fMotherVolume,
			false,
			0);

  fPreTubRotation.rotateX(180.0 * deg);
  fPreTubRotation.rotateY(0.0 * deg);
  fPreTubRotation.rotateZ(0.0 * deg);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(-fFanoutBox_OffsetY);
  fComponentsPosition.setZ(fBase_CenterZ);
  fFanoutBoxPhysical[1] 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fFanoutBoxLogical,
			"FanoutBox2",
			fMotherVolume,
			false,
			0);
  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(-fFanoutBox_OffsetY-10*mm);
  fComponentsPosition.setZ(fBase_CenterZ);
  fFanoutBoxPhysical[2] 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fFanoutBoxLogical,
			"FanoutBox3",
			fMotherVolume,
			false,
			0);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(-20.0*mm-fPreTub_CenterY);
  fComponentsPosition.setZ(fBase_CenterZ);

  fPreTubPhysical 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fPreTubLogical,
			"PreTub",
			fMotherVolume,
			false,
			1);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(-20.0*mm-fPreBottom_CenterY);
  fComponentsPosition.setZ(fBase_CenterZ);

  fPreBottomPhysical 
    = new G4PVPlacement(G4Transform3D(fPreTubRotation,fComponentsPosition),
			fPreBottomLogical,
			"PreBottom",
			fMotherVolume,
			false,
			1);


/*
  Removed by Alex
  // preamp ring

  fPreampRingLogical -> SetVisAttributes(PreampVisAtt);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(fDewarExpand_CenterZ);

  fPreampRingPhysical = new G4PVPlacement(0,fComponentsPosition,
                                            fPreampRingLogical,
                                            "PreampRing",
                                            fMotherVolume,
                                            false,
                                            0);

*/
  // dead layer 

  fCrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt); 
 
  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(-fCan_CenterZ);
  fCrystalDeadLayerPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                fCrystalDeadLayerLogical,
                                                "CrystalDeadLayer",
                                                fVacuumLogical,
                                                false,
                                                0);


  // silicon 

  fCrystalSiliconLogical -> SetVisAttributes(SiVisAtt); 

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ( (fCrystal_Height - fCrystal_SiliconThickness) * 0.5-fCan_CenterZ);
  fCrystalSiliconTopPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                 fCrystalSiliconLogical,
                                                 "CrystalSiliconTop",
                                                 fVacuumLogical,
                                                 false,
                                                 0);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ( 0.0 - (fCrystal_Height - fCrystal_SiliconThickness) * 0.5-fCan_CenterZ);
  fCrystalSiliconBottomPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                 fCrystalSiliconLogical,
                                                 "CrystalSiliconBottom",
                                                 fVacuumLogical,
                                                 false,
                                                 0);

  // Crystal: testing by Alex.
  fCrystalLogical -> SetVisAttributes(CrystalVisAtt); 

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(-fCan_CenterZ);
  fCrystalPhysical = new G4PVPlacement(0,fComponentsPosition,
                                       fCrystalLogical,
                                       "sensitive_Siegfried",
                                       fVacuumLogical,
                                       false,
                                       0);


  //source system
  //-------------

  //source mother volume
  fSourceMotherVolume -> SetVisAttributes(AirVisAtt);

  G4ThreeVector SourceSystemPosition;


  if (fSourceSystem_Z >= (fCan_CenterZ + fCan_Height*0.5 )) { // do not rotate
    fSourceSystem_Z += fCollimator_Height; // fCollimator_Height = half height of its mother volume!
    SourceSystemPosition.setX(fSourceSystem_R * cos(fSourceSystem_Phi));
    SourceSystemPosition.setY(fSourceSystem_R * sin(fSourceSystem_Phi));
    SourceSystemPosition.setZ(fSourceSystem_Z); // Shift it on height up so the center is half height higher

    fSourceMotherPhysical
      = new G4PVPlacement(0,
			  SourceSystemPosition,
			  fSourceMotherVolume,
			  "SourceMotherVolume",
			  fMotherVolume,
			  false,
			  0);
  }
  else {			// rotate

    G4RotationMatrix* SourceSystemRotation = new G4RotationMatrix();
    SourceSystemRotation->rotateY(-90.0 * deg);
    SourceSystemRotation->rotateX(fSourceSystem_Phi);

    if (useSourceHolderStick) // Use HolderStick - another some 11 cm of polyvinylchloride between the source and the detector
    {
      fSourceSystem_R = fCan_OuterRadius + fSourceHolderStickLength + 2*mm;
      G4ThreeVector SourceHolderStickPosition;
      SourceHolderStickPosition.setX((fCan_OuterRadius + 0.5*fSourceHolderStickLength + 1*mm) * cos(fSourceSystem_Phi));
      SourceHolderStickPosition.setY((fCan_OuterRadius + 0.5*fSourceHolderStickLength + 1*mm) * sin(fSourceSystem_Phi));
      SourceHolderStickPosition.setZ(fSourceSystem_Z);

      fSourceHolderStickLogical -> SetVisAttributes(CrystalVisAtt); // to be also in red
      fSourceHolderStickPhysical
          = new G4PVPlacement(SourceSystemRotation,
			  SourceHolderStickPosition,
			  fSourceHolderStickLogical,
			  "SourceHolderStick",
			  fMotherVolume,
			  false,
			  0);
    }
    fSourceSystem_R += fCollimator_Height; // Shift it on height up so the center is half height higher (fCollimator_Height = half height of its mother volume!)
    SourceSystemPosition.setX(fSourceSystem_R * cos(fSourceSystem_Phi));
    SourceSystemPosition.setY(fSourceSystem_R * sin(fSourceSystem_Phi));
    SourceSystemPosition.setZ(fSourceSystem_Z);

    fSourceMotherPhysical
      = new G4PVPlacement(SourceSystemRotation, 
			  SourceSystemPosition,
			  fSourceMotherVolume,
			  "SourceMotherVolume",
			  fMotherVolume,
			  false,
			  0);

  }

  if ( useCollimator ) {
    //collimator
    fCollimatorLogical -> SetVisAttributes(CuVisAtt);
    fCollimatorPhysical
      = new G4PVPlacement(0, 
			  G4ThreeVector(0,0,-0.5*fCollimator_Height),
			  fCollimatorLogical,
			  "Collimator",
			  fSourceMotherVolume,
			  false,
			  0);
    
    //AEA source holder
    fSourceHolderAEALogical -> SetVisAttributes(AlVisAtt);
    fSourceHolderAEAPhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*fSourceHolderHeightAEA-fCollimatorHole_UpperHeight),
			  fSourceHolderAEALogical,
			  "SourceHolderAEA",
			  fSourceMotherVolume,
			  false,
			  0);
    
    //AEA source
    fSourceAEALogical -> SetVisAttributes(DewarVisAtt);
    fSourceAEAPhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,-fCollimatorHole_UpperHeight+1.2*mm+0.5*fSourceHeightAEA),
			  fSourceAEALogical,
			  "SourceAEA",
			  fSourceMotherVolume,
			  false,
			  0);
  }
  else {
    //AEA source holder
    fSourceHolderAEALogical -> SetVisAttributes(AlVisAtt);
    fSourceHolderAEAPhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*fSourceHolderHeightAEA-fCollimator_Height),
			  fSourceHolderAEALogical,
			  "SourceHolderAEA",
			  fSourceMotherVolume,
			  false,
			  0);
    
    //AEA source
    fSourceAEALogical -> SetVisAttributes(DewarVisAtt);
    fSourceAEAPhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*fSourceHeightAEA-fCollimator_Height+1.2*mm),
			  fSourceAEALogical,
			  "SourceAEA",
			  fSourceMotherVolume,
			  false,
			  0);
    
  }

// two rege detectors
// just one rege detector  at 70 degree with repect to -y axis 09-10-2006 Xiang Liu

  if (fCoincidence_Flag) { 
  G4RotationMatrix fREGeRotation;
  fComponentsPosition.setX( ( fCan_OuterRadius 
                            + fREGe_DistanceToCanOuterSurface
                            + fREGe_Height*0.5 ) * cos( 200.0*deg ) );
  fComponentsPosition.setY( ( fCan_OuterRadius 
                            + fREGe_DistanceToCanOuterSurface
                            + fREGe_Height*0.5 ) * sin( 200.0*deg ) );
  fComponentsPosition.setZ(0.0 * cm);
  fREGeRotation.rotateX(0.0 * deg);
  fREGeRotation.rotateY(90.0 * deg);
  //fREGeRotation.rotateZ(0.0 * deg);
  fREGeRotation.rotateZ(20.0 * deg);

  fREGePhysical[0] = new G4PVPlacement(G4Transform3D(fREGeRotation, fComponentsPosition),
                                       fREGeLogical[0], 
                                       "REGe0", 
                                       fMotherVolume,
                                       false, 
                                       0);

/*
  fREGeRotation.rotateY(90.0 * deg); // recover RotationMatrix
  fComponentsPosition.setX(0.0 - fCan_OuterRadius
                          - fREGe_DistanceToCanOuterSurface
                          - fREGe_Height*0.5);
  fComponentsPosition.setY(0.0 * cm);
  fComponentsPosition.setZ(0.0 * cm);
  fREGeRotation.rotateX(0.0 * deg);
  fREGeRotation.rotateY(90.0 * deg);
  fREGeRotation.rotateZ(0.0 * deg);

  fREGePhysical[1] = new G4PVPlacement(G4Transform3D(fREGeRotation, fComponentsPosition),
                                       fREGeLogical[1],
                                       "REGe1", 
                                       fMotherVolume,
                                       false,
                                       1);
*/

  }

}

// --------------------------------------------------------------------------- 
 

