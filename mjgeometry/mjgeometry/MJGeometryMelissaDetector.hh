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
 *      
 * CLASS DECLARATION:  MJGeometryMelissaDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: Melissa is a low background Canberra HPGe counting detector at
 * the Kimballton Underground Research Facility operated by TUNL.  The cryostat
 * is vertical and is in a typical lead shield with a door.
 *
 */
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: P. Finnerty  K. Macon
 * CONTACT: paddy@physics.unc.edu mokevin@physics.unc.edu
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *          02-22-09, added copper shield to geometry, K. Macon
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYMELISSADETECTOR_HH
#define _MJGEOMETRYMELISSADETECTOR_HH

//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//
class MJGeometryMelissaDetectorMessenger;
class G4Material;
class MGGeometryIdealCoaxCrystal;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class MJGeometryMelissaDetector : public MGGeometryDetector
{
public:

  MJGeometryMelissaDetector(G4String serNum);
  ~MJGeometryMelissaDetector();

  void ConstructDetector();
  void InitializeSample();
  bool UsesMJPSQLDatabase() { return true; }  

  void InitializeTeflon();
  void InitializeSteel();
  void InitializeRockBucket();
  void InitializeRockBag();
  void InitializeSuperInsulation();
  void InitializeAlRings();
  void InitializeCalibrationSource();
  void InitializePMT(); 
  void InitializeZeolite();
  void InitializeFoamInsulation();
  void InitializeMarinelli();

  void SetDefaults();
 
  void SetUseSample(G4bool val) {fUsingSamples = val;}
  void SetSample(G4String val) {fSample = val;}
  void SetPlacement(G4String val) {fPlacementOption = val;}
  void SetPosition(G4ThreeVector val) {fSamplePlacement = val;}
  void SetPosition(G4double x, G4double y, G4double z) {
    fSamplePlacement = G4ThreeVector(x,y,z);}

private:
  MGGeometryIdealCoaxCrystal *fIdealCoaxCrystal;

  G4LogicalVolume* fLeadShieldLogical;
  G4LogicalVolume* fLeadCylinderLogical;
  G4LogicalVolume* fInnerVolumeLogical;
  G4LogicalVolume* fCopperShieldLogical;
  G4LogicalVolume* fCryostatLogical;
  G4LogicalVolume* fCryostatCavityLogical;
  G4LogicalVolume* fCryostatPuckLogical;

  G4VPhysicalVolume* fLeadShieldPhysical;
  G4VPhysicalVolume* fLeadCylinderPhysical;
  G4VPhysicalVolume* fInnerVolumePhysical;
  G4VPhysicalVolume* fCopperShieldPhysical;
  G4VPhysicalVolume* fCryostatPhysical;
  G4VPhysicalVolume* fCryostatCavityPhysical;
  G4VPhysicalVolume* fCryostatPuckPhysical;
  G4VPhysicalVolume* fCrystalPhysical;

  G4double fAlPlateThickness;
  G4double fCryostatHeight;
  G4String fCryostatMaterialName;
  G4double fCryostatOffset;
  G4double fCryostatOuterRadius;
  G4double fCryostatThickness;
  G4double fCrystalOffset;
  G4double fCrystalRadius;
  G4double fCrystalHeight;
  G4double fOuterVolumeRadius;
  G4double fLeadShieldHeight;
  G4double fLeadShieldWidth;
  //G4double fLeadShieldThickness;
  G4double fLeadCylinderHeight;
  G4double fSampleLength;
  G4double fSampleWidth;
  G4double fSampleHeight;
  G4double fInnerVolumeHeight;
  G4double fInnerVolumeWidth;
  G4double fCopperShieldThickness;
  G4double fCopperHoleRadius;
  G4double fCryostatPuckThickness;

  MJGeometryMelissaDetectorMessenger *fMessenger;

  G4LogicalVolume* fTeflonLogical;
  G4VPhysicalVolume* fTeflonPhysical;

  G4LogicalVolume* fSteelLogical;
  G4VPhysicalVolume* fSteelPhysical;

  G4LogicalVolume* fRockBucketLogical;
  G4VPhysicalVolume* fRockBucketPhysical;

  G4LogicalVolume* fRockBagLogical;
  G4VPhysicalVolume* fRockBagPhysical;

  G4LogicalVolume* fSuperInsulationLogical;
  G4VPhysicalVolume* fSuperInsulationPhysical;

  G4LogicalVolume* fAlRingsLogical;
  G4VPhysicalVolume* fAlRingsPhysical;

  G4LogicalVolume* fCalibrationSourceLogical;
  G4VPhysicalVolume* fCalibrationSourcePhysical;

  G4LogicalVolume* fPMTLogical;
  G4VPhysicalVolume* fPMTPhysical;

  G4LogicalVolume* fZeoliteLogical;
  G4VPhysicalVolume* fZeolitePhysical;

  G4LogicalVolume* fFoamInsulationLogical;
  G4VPhysicalVolume* fFoamInsulationPhysical;

  G4LogicalVolume* fMarinelliLogical;
  G4VPhysicalVolume* fMarinelliPhysical;
  
  G4bool   fUsingSamples;
  G4String fSample;
  G4String fPlacementOption;
  G4ThreeVector fSamplePlacement;
};
#endif
