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
 * $Id: MJGeometryVT1Detector.hh,v 1.4 2009/01/30 23:16:58 kmacon Exp $
 *      
 * CLASS DECLARATION:  VT1
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: VT1
 *
 */
/**  
 * SPECIAL NOTES:
 * 
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: K. Macon
 * CONTACT: mokevin@physics.unc.edu
 * FIRST SUBMISSION:
 * 
 * REVISION: Added Calibration Source Geometry
 * 
 *           01-30-2009 Added fAlCylinderOffset to make correct dimensions
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMERYVT1DETECTOR_HH
#define _MJGEOMERYVT1DETECTOR_HH

//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//
class MJGeometryVT1DetectorMessenger;
class G4Material;
class MGGeometryIdealCoaxCrystal;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class MJGeometryVT1Detector : public MGGeometryDetector
{
public:

  MJGeometryVT1Detector(G4String serNum);
  ~MJGeometryVT1Detector();

  void ConstructDetector();
  void InitializeSample();
  bool UsesMJPSQLDatabase() { return true; }  

  void InitializeTeflon();
  void InitializeRockBucket();
  void InitializeRockBag();
  void InitializeSuperInsulation();
  void InitializeCalibrationSource();
  void InitializePMT();
  void InitializePicoCoax();
  void InitializeMarinelli();

  void SetDefaults();

  void SetUseSample(G4bool val) {fUsingSamples = val;}
  void SetSample(G4String val) {fSample = val;}
  void SetPlacement(G4String val) {fPlacementOption = val;}
  void SetPosition(G4ThreeVector val) {fSamplePlacement = val;}
  void SetPosition(G4double x,G4double y, G4double z) {
  fSamplePlacement = G4ThreeVector(x,y,z);}


private:
  MGGeometryIdealCoaxCrystal *fIdealCoaxCrystal;

  G4LogicalVolume* fLeadShieldLogical;
  G4LogicalVolume* fCopperLayerLogical;
  G4LogicalVolume* fInnerVolumeLogical;
  G4LogicalVolume* fCryostatLogical;
  G4LogicalVolume* fCryostatCavityLogical;
  //G4LogicalVolume* fCrystalLogical;
  G4LogicalVolume* fIRShieldLogical;
  G4LogicalVolume* fMountCupWallLogical;
  G4LogicalVolume* fMountCupBaseLogical;
  G4LogicalVolume* fCopperPlateLogical;
  G4LogicalVolume* fAlCylinderLogical;
  G4LogicalVolume* fAlMountLogical;
  G4LogicalVolume* fColdFingerLogical;
  G4LogicalVolume* fCopperRodLogical;
  G4LogicalVolume* fCryostatCopperRodLogical; 
  G4LogicalVolume* fColdFingerCopperRodLogical; 

  G4VPhysicalVolume* fLeadShieldPhysical;
  G4VPhysicalVolume* fCopperLayerPhysical;
  G4VPhysicalVolume* fInnerVolumePhysical;
  G4VPhysicalVolume* fCryostatPhysical;
  G4VPhysicalVolume* fCryostatCavityPhysical;
  G4VPhysicalVolume* fCrystalPhysical;
  G4VPhysicalVolume* fIRShieldPhysical;
  G4VPhysicalVolume* fMountCupWallPhysical;
  G4VPhysicalVolume* fMountCupBasePhysical;
  G4VPhysicalVolume* fCopperPlatePhysical;
  G4VPhysicalVolume* fAlCylinderPhysical;
  G4VPhysicalVolume* fAlMountPhysical;
  G4VPhysicalVolume* fColdFingerPhysical;
  G4VPhysicalVolume* fCopperRodPhysical;
  G4VPhysicalVolume* fCryostatCopperRodPhysical;
  G4VPhysicalVolume* fColdFingerCopperRodPhysical;

  G4double  fCrystalHeight;
  G4double  fCrystalRadius;
  G4double  fSampleHeight;
  G4double  fSampleWidth;
  G4double  fSampleLength;
  G4double  fMgPlateThickness;
  G4double  fCopperLayerThickness;
  G4double  fCryostatHeight;  
  G4double  fCryostatOuterRadius;
  G4double  fCryostatOffset;            
  G4double  fCryostatThickness;
  G4double  fCrystalOffset;
  G4double  fInnerVolumeHeight; 
  G4double  fInnerVolumeRadius;
  G4double  fLeadShieldHeight;
  G4double  fLeadShieldThickness;
  G4double  fLeadShieldRadius;
  G4double  fOuterVolumeRadius;
  G4double  fMountCupInnerRadius;
  G4double  fMountCupOuterRadius;
  G4double  fMountCupWallHeight;
  G4double  fMountCupAssemblyHeight;
  G4double  fMountCupWallThickness;
  G4double  fIRShieldThickness;
  G4double  fMountCupBaseThickness;
  G4double  fCopperPlateThickness;
  G4double  fCopperPlateRadius;
  G4double  fColdFingerLength;
  G4double  fColdFingerRadius;
  G4double  fAlCylinderHeight;
  G4double  fAlCylinderOffset;
  G4double  fAlMountHeight;
  G4double  fCopperRodHeight;
  G4double  fCopperRodRadius;
  G4double  fCryostatCopperRodRadius;
  G4double  fCryostatCopperRodHeight;
  G4double  fColdFingerCopperRodRadius;
  G4double  fColdFingerCopperRodLength;
  G4double  fTopOfCryostat;

  MJGeometryVT1DetectorMessenger *fMessenger;

  G4LogicalVolume* fTeflonLogical;
  G4VPhysicalVolume* fTeflonPhysical;
  G4LogicalVolume* fRockBucketLogical;
  G4VPhysicalVolume* fRockBucketPhysical;
  G4LogicalVolume* fRockBagLogical;
  G4VPhysicalVolume* fRockBagPhysical;
  G4LogicalVolume* fSuperInsulationLogical;
  G4VPhysicalVolume* fSuperInsulationPhysical;
  G4LogicalVolume* fCalibrationSourceLogical;
  G4VPhysicalVolume* fCalibrationSourcePhysical;
  G4LogicalVolume* fPMTLogical;
  G4VPhysicalVolume* fPMTPhysical;  
  G4LogicalVolume* fPicoCoaxLogical;
  G4VPhysicalVolume* fPicoCoaxPhysical;
  G4LogicalVolume* fMarinelliLogical;
  G4VPhysicalVolume* fMarinelliPhysical;

  G4ThreeVector fSamplePlacement;

  G4bool fUsingSamples;
  G4String fSample;
  G4String fPlacementOption;
};
#endif
