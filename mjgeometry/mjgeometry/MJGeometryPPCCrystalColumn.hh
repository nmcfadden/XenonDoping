//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
 * $Id: MJGeometryPPCCrystalColumn.hh,v 1.1 2008-09-18 17:40:01 mitzi Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPPCCrystalColumn.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Takes MJGeometryPPCDetectorCrystal and insert it into a shield.
 * Can be used as a wrapper for just and ideal coaxial crystal w/o any shield
 * by setting fShieldOn = false
 * The shield consists of an endcap (can) and a colplate at the bottom of the
 * can that the crystal is mounted on.
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
 * AUTHOR: M. Boswell
 * CONTACT: 
 * FIRST SUBMISSION: Wed. July 23, 2008
 * 
 * REVISION:
 * 
 * 07-23-2008, Created, M. Boswell
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYPPCCRYSTALCOLUMN_HH
#define _MJGEOMETRYPPCCRYSTALCOLUMN_HH
//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"
#include "mjgeometry/MJGeometryPPCDetector.hh"

//---------------------------------------------------------------------------//

class G4VisAttributes;
class G4Material;
class G4VPhysicalVolume;
class MJGeometryPPCCrystalColumnMessenger;
class MJGeometryPPCDetector;

class MJGeometryPPCCrystalColumn : public MGGeometryDetector 
{
public:

  MJGeometryPPCCrystalColumn();
  MJGeometryPPCCrystalColumn(G4String serNum);
  MJGeometryPPCCrystalColumn(const MJGeometryPPCCrystalColumn &);
  ~MJGeometryPPCCrystalColumn();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  

  //get info about the assembly
  G4double GetCrystalColumnHeight() {return fCrystalColumnHeight;}
  G4double GetCrystalAssemblyRadius() {return fCrystalColumnRadius;}
  //void SetOpenSpaceMaterialName(G4String val) { fOpenSpaceMaterialName = val};



  //set general info about the assembly
  void SetOneOfMany(G4bool isMany) { fIsOneOfMany = isMany;}
  void SetTemplateCrystalColumnSerNum(G4String val) 
  { fTemplateCrystalColumnSerNum = val;}
  void SetTemplateCrystal(MJGeometryPPCDetector *xtal) {fTemplateCrystal = xtal;}
  //void SetNumberOfCrystals(G4int val) { fNumberOfCrystals = val;}

  //functions for messenger to change crystal parameters
  void SetCrystalHeight(G4double val) { fCrystalHeight = val;};
  void SetCrystalRadius(G4double val) { fCrystalRad = val;};

  //manager FET parameters
  void SetFETHeight(G4double val) { fFETHeight = val;}
  void SetFETLength(G4double val) { fFETLength = val;}
  void SetFETWidth(G4double val) { fFETWidth = val;}
  void SetFETOffsetRadius(G4double val) { fFETOffsetRad = val;}
  void SetFETOffset(G4double val) { fFETOffset = val;}
  void SetFETOffsetAngle(G4double val) { fFETOffsetAng = val;}
  

private:
  void Initialize();

  MJGeometryPPCCrystalColumnMessenger *fG4Messenger;
  MJGeometryPPCDetector     *fTemplateCrystal;
  std::vector<MJGeometryPPCDetector*> fCrystals;

  //various colors in use
  G4VisAttributes* FETAttributes;
  G4VisAttributes* CuAttributes;
  G4VisAttributes* KaptonAttributes;
  G4VisAttributes* EFCuAttributes;
  G4VisAttributes* KelFAttributes;
  G4VisAttributes* GeAttributes;

  //materials
  G4String fOpenSpaceMaterialName;
  //G4Material* fOpenSpaceMaterial;
  G4Material* CanMaterial;
  G4Material* InsulatorMaterial;
  G4Material* KaptonMaterial;
  //G4Material* CrystalMaterial;
  G4Material* FETMaterial;

  //basic variables
  G4double fEps; //small offset to prevent overlaps
  //G4String serialNumber; //serial number of crystal
  G4String fTemplateCrystalColumnSerNum; //serial number of column
  G4bool fIsOneOfMany;
  G4ThreeVector fOffset;
 
  //assembly parameters, these are calculated
  G4double fCrystalColumnHeight;
  G4double fCrystalColumnRadius;
  size_t fNumberOfCrystals;//number of crystals in a column
  G4double fAssemblyOffset;
  G4double fAssemblyOffsetAng;

  //PPC detector parameters
  G4double fCrystalHeight;
  G4double fCrystalRad; 
  //G4LogicalVolume* fcrystalVolumeLogical;

  //FET parameters
  G4double fFETHeight;
  G4double fFETWidth;
  G4double fFETLength;
  G4double fFETOffset;
  G4double fFETOffsetAng;
  G4double fFETOffsetRad;
  //G4ThreeVector moveFET;
  //G4LogicalVolume* FETLogical;


  //Cu house parameters
  G4double fCuCanORad; //outer radius of cu can
  G4double fCuCanIRad;
  G4double fCuCanThick;
  G4double fCuCanHeight;
  G4double fCuCanBaseIRad;
  G4double fCuCanBaseHeight;
  G4double fCuCanRingHeight;
  G4double fCuCanRingOffset;
  G4double fCuCanLipIRad;
  G4double fCuCanLipThick;
  G4double fCuCanSupportAng;
  G4double fCuCanOffset;
  //G4LogicalVolume* CuCanLogical;


  //Cu Can Top parameters
  G4double fCuCanLidIRad;
  G4double fCuCanLidORad;
  G4double fCuCanLidThick;
  G4double fCuCanLidLipRad;
  G4double fCuCanLidLipThick;
  G4double fCuCanLidOffset;
  //G4LogicalVolume* CuCanLidLogical;

  //Insulator parameters
  G4double fInsulatorBaseHeight;
  G4double fInsulatorHeight;
  G4double fInsulatorRadius;
  G4double fInsulatorIRadius;
  G4double fInsulatorIHeight;
  G4double fInsulatorAng;
  G4double fInsulatorOffset;
  G4double fInsulatorOffsetAng;
  G4double fInsulatorCutAwayRad;
  //G4ThreeVector moveInsulator;
  //G4LogicalVolume* InsulatorLogical;


 



};
#endif
