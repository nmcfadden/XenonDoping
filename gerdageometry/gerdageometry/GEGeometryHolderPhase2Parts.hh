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
 * CLASS DECLARATION:  GEGeometryHolderPhase2Parts.hh
 *
 * DESCRIPTION:
 *
 *  A class that contain all
 *    - parameters \n
 *    - logical volumes \n
 *    - physical volumes \n
 *  of all parts of the Phase 2 holders.
 *
 * AUTHOR: Anne Wegmann
 *
 * REVISION: MM-DD-YYYY
 *
 * xx-04-2008, first version, Anne
 * 04-10-2016, adapted to status december 2016, Janina
 *
 */


#ifndef _GEGEOMETRYHOLDERPHASE2PARTS_HH
#define _GEGEOMETRYHOLDERPHASE2PARTS_HH


#include "GEGeometryDetectorDB.hh"


class G4UnionSolid;
class G4SubtractionSolid;
class G4Trap;
class G4LogicalVolume;
class G4Material;
class G4VisAttributes;
class G4Box;
class G4Polycone;

class GEGeometryHolderPhase2Parts {
public:
  GEGeometryHolderPhase2Parts(const G4double gap);
  GEGeometryHolderPhase2Parts(const GEGeometryHolderPhase2Parts& holder);
  ~GEGeometryHolderPhase2Parts();
  void        ResetDependentParameters();

  // Creation of solids, logical volumes, and physical volumes
  void        CreateSolids();
  void        CreateSolids_LowerPlate();
  void        CreateSolids_UpperPlate();
  void        CreateSolids_VerticalBar();

  void        CreateLogicalVolumes();
  void        CreateLogicalVolumes_LowerPlate();
  void        CreateLogicalVolumes_UpperPlate();
  void        CreateLogicalVolumes_VerticalBar();
  void        CreatePhysicalVolume(GEGeometryDetectorDB *db, G4LogicalVolume* motherVol, G4int col, G4int row,
				   G4int ffHolderGeometry1Number,G4int ffHolderGeometry1NumberSlot, 
				   const G4ThreeVector& crystalPos);
  void ResetParametersForAPair(GEGeometryDetectorDB* db,G4int col,
                               G4int row);  void ResetParametersForSingle(GEGeometryDetectorDB* db,G4int col,
                               G4int row);
  void ResetParametersForType3Pair(GEGeometryDetectorDB* db,G4int col,
                               G4int row);
  void ResetParametersForCoax(GEGeometryDetectorDB* db,G4int col,
                               G4int row);
  

  //Getters
  void SetCrystalRadius(G4double rad){fCrystalRadius = rad;};
  void SetCrystalInnerRadius(G4double ir){fCrystalInnerRadius= ir;};
  void SetCrystalHeight(G4double hei){fCrystalHeight = hei;};
  void SetCrystalID(G4int id){fCrystalID=id;};

  void SetVisAtt_LowerPlate(G4VisAttributes* attr){fVisAtt_LowerPlate = attr;};
  void SetVisAtt_UpperPlate(G4VisAttributes* attr){fVisAtt_UpperPlate = attr;};
  void SetVisAtt_VerticalBar(G4VisAttributes* attr){fVisAtt_VerticalBar = attr;};

  void SetMaterial_LowerPlate(G4Material* mat){fMaterial_LowerPlate = mat;};
  void SetMaterial_UpperPlate(G4Material* mat){fMaterial_UpperPlate = mat;};
  void SetMaterial_VerticalBar(G4Material* mat){fMaterial_VerticalBar = mat;};

  G4LogicalVolume* GetLogicalVol_LowerPlate(){return fLogicalVol_LowerPlate;};
  G4LogicalVolume* GetLogicalVol_UpperPlate(){return fLogicalVol_UpperPlate;};
  G4LogicalVolume* GetLogicalVol_VerticalBar(){return fLogicalVol_VerticalBar;};
  

  G4double  GetZDistanceToEndOfSiliconPlate() const;
  G4double  GetZDistanceToEndOfSiliconPlatePair() const;
  G4double  GetHolderPhase2Width() const;
  
  void SetLengthOfCopperBars(G4double copperbarlength){ fCopperBarLength = copperbarlength;};
    
  //TODO check if I need all of them
  G4double GetCrystalRadius(){return fCrystalRadius;};
  G4double GetCrystalInnerRadius(){return fCrystalInnerRadius;};
  G4double GetCrystalHeight(){return fCrystalHeight;};
  G4int GetCrystalID(){return fCrystalID;};


private:  // data members
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
  G4double fCopperBarLength;

    //parameters for Holder plates
  // ---------------------------
  G4double	fSiliconPlate_h;
  G4double      fHolderWidth;
  
  // parameters for vertical bars
  G4double 	fVerticalBar_Ro;

  // solides
  G4VSolid*       fSolid_LowerPlate;
  G4VSolid*       fSolid_UpperPlate;
  G4Tubs*         fSolid_VerticalBar;

  // vis attributes
  G4VisAttributes* fVisAtt_LowerPlate;
  G4VisAttributes* fVisAtt_UpperPlate;
  G4VisAttributes* fVisAtt_VerticalBar;

  // materials
  G4Material* fMaterial_LowerPlate;
  G4Material* fMaterial_UpperPlate;
  G4Material* fMaterial_VerticalBar;

  // logical volumes
  G4LogicalVolume* fLogicalVol_LowerPlate;
  G4LogicalVolume* fLogicalVol_UpperPlate;
  G4LogicalVolume* fLogicalVol_VerticalBar;


  static G4double SmallNumber; // this is to make drawings with WIRED nicer

  G4int fNumberOfPhysicalVolumesSets[100]; // there are 14 phase-1-type crystals, but their ID could be mixed with the IDs of phase-2-type crystals 
                                                    // '100' takes into account crystal IDs from '0' up to '99'
  G4double fCrystalPairGap;
  G4bool fIsAPair;
  G4bool fIsType3;
  G4bool fIsCoax;
};

#endif

