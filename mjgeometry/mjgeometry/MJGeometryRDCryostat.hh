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
 * $Id: MJGeometryRDCryostat.hh,v 1.2 2009-04-23 18:32:24 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryRDCryostat.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry for Majorana Reference Design Cryostat as of 11/17/2005
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Nov 16 16:25:21 PST 2005
 * 
 * REVISION:
 * 
 * 3-19-2010, Added variables for DEMONSTRATOR, R. Henning
 * 11-17-2005, Created, R. Henning
 *
 **/
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYRDCRYOSTAT_HH
#define _MJGEOMETRYRDCRYOSTAT_HH

//---------------------------------------------------------------------------//

#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

using namespace std;

class G4LogicalVolume;
class G4Material;
class G4PVPlacement;
class G4VisAttributes;

class MJGeometryIdealCoaxWithShield;
class MJGeometryRDCrystalColumn;
class MJGeometryRDCryostatMessenger;

class MJGeometryRDCryostat : public MGGeometryDetector
{
public:

  MJGeometryRDCryostat();
  MJGeometryRDCryostat(G4String serNum);
  MJGeometryRDCryostat(const MJGeometryRDCryostat &);
  ~MJGeometryRDCryostat();

  void ConstructDetector();
  void Dump();
  void SetDefaults();
  void SetDefaultsDEMONSTRATOR();
  bool UsesMJPSQLDatabase() { return true; }  

  //Setter & Getters
  G4double GetAssemblyRadius() { return fAssemblyRadius; }
  G4double GetAssemblyHeight() { return fAssemblyHeight; }

  void  SetThermalShroudThickness(G4double val) { fThermalShroudThickness = val; }
  void  SetCryostatInnerRadius(G4double val) { fCryostatInnerRadius = val; }
  void  SetCryostatOuterRadius(G4double val) { fCryostatOuterRadius = val; }
  void  SetCryostatTotalHeight(G4double val) { fCryostatTotalHeight = val; }
  void  SetCryostatFloorThickness(G4double val) { fCryostatFloorThickness = val; }
  void  SetCryostatLidThickness(G4double val) { fCryostatLidThickness = val; }
  void  SetCryostatMaterialName(G4String val) { fCryostatMaterialName = val; }
  void  SetStringerMountPlateRadius(G4double val) { fStringerMountPlateRadius = val; }
  void  SetStringerMountPlateThickness(G4double val) { fStringerMountPlateThickness = val; }
  void  SetCrystalColumnHoleRadius(G4double val) { fCrystalColumnHoleRadius = val; }
  void  SetHoleSeparationY01(G4double val) { fHoleSeparationY01  = val; }
  void  SetHoleSeparationY02(G4double val) { fHoleSeparationY02 = val; }
  void  SetHoleSeparationY03(G4double val) { fHoleSeparationY03 = val; }
  void  SetHoleSeparationY04(G4double val) { fHoleSeparationY04 = val; }
  void  SetHoleSeparationX01(G4double val) { fHoleSeparationX01 = val; }
  void  SetHoleSeparationX02(G4double val) { fHoleSeparationX02 = val; }
  void  SetThermalShroudHeight(G4double val) { fThermalShroudHeight = val; }
  void  AddHolePositionXY(G4ThreeVector val) { fHolePositionsXY.push_back(val); }    
  void  SetOpenSpaceMaterialName(G4String val) { fOpenSpaceMaterialName = val; }

private:

  void Initialize();

  G4bool        fBuildDEMONSTRATOR;

  // Dimensions and information provided by user.
  G4double      fEps;
  G4double      fThermalShroudThickness; // Thermal shroud added to cryo for now
  G4double      fCryostatInnerRadius;
  G4double      fCryostatOuterRadius;
  G4double      fCryostatTotalHeight;
  G4double      fCryostatFloorThickness;
  G4double      fCryostatLidThickness;
  G4String      fCryostatMaterialName;

  G4double      fDEMOCryostatThickness;  // Used in DEMONSTRATOr Geometry only.

  G4double      fStringerMountPlateRadius;
  G4double      fStringerMountPlateThickness;
  G4double      fCrystalColumnHoleRadius;
  G4double      fHoleSeparationY01;
  G4double      fHoleSeparationY02;
  G4double      fHoleSeparationY03;
  G4double      fHoleSeparationY04;
  G4double      fHoleSeparationX01;
  G4double      fHoleSeparationX02;
  G4double      fThermalShroudHeight;
  vector<G4ThreeVector>   fHolePositionsXY; // In stringer support plate coordinate system.    
  G4String      fOpenSpaceMaterialName;

  // Internal values computed from previous dimensions
  size_t       fNOfCrystalColumns;
  G4double     fInnerCryostatHeight_2;
  G4double     fAssemblyHeight;
  G4double     fAssemblyRadius;
  MJGeometryRDCrystalColumn      *fTemplateCrystalColumn;
  MJGeometryIdealCoaxWithShield  *fTemplateCrystal;
  vector<G4ThreeVector>    fCrystalColumnPositions; // origin of z coord is bottom of lowest
                                                    // Ge tray in column.
  vector<G4double>         fCrystalColumnRotations; // Measured clockwise from top, start on +y-axis
  vector<MJGeometryRDCrystalColumn*>    fCrystalColumns;
  G4Material          *fCryostatMaterial;
  G4Material          *fOpenSpaceMaterial;

  // Miscellaneous
  G4VisAttributes                *fCuVisAtt;
  G4VisAttributes                *fMotherVolumeVisAtt;

  MJGeometryRDCryostatMessenger  *fMessenger;
};
#endif
