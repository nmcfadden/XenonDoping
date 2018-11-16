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
 * $Id: MJGeometryRDCrystalColumn.hh,v 1.4 2009-04-23 18:32:24 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryCrystalColumn.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geant 4 geometrical description of 3 Crystal assembly, as per Majorana 
 * Reference Design provide by email from PNNL, 10/13/2005.
 * Generalized to allow any number of crystal in a column.
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
 * AUTHOR: R. Henning
 * FIRST SUBMISSION: Wed Nov  9 15:43:44 PST 2005
 * 
 * REVISION:
 * 
 * 11-09-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYRDCRYSTALCOLUMN_HH
#define _MJGEOMETRYRDCRYSTALCOLUMN_HH

//---------------------------------------------------------------------------//

#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

using namespace std;

class G4Material;
class G4PVPlacement;
class G4VisAttributes;

class MGGeometryIdealCoaxWithShield;
class MJGeometryRDCrystalColumnMessenger;

class MJGeometryRDCrystalColumn : public MGGeometryDetector
{
public:

  MJGeometryRDCrystalColumn();
  MJGeometryRDCrystalColumn(G4String serNum, G4bool messOff);
  MJGeometryRDCrystalColumn(const MJGeometryRDCrystalColumn &);
  ~MJGeometryRDCrystalColumn();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  
  void SetDefaultsDEMONSTRATOR();

  // Setters/Getters/Adders
  G4double GetAssemblyHeight() { return fAssemblyHeight; }
  G4double GetAssemblyRadius() { return fAssemblyRadius; }
  G4double GetSupportRodLength() { return fSupportRodLength; }
  
  void  SetTemplateCrystal(MJGeometryIdealCoaxWithShield *xtal) {fTemplateCrystal = xtal;}
  void  SetStringerLidThickness(G4double val) { fStringerLidThickness = val; }
  void  SetStringerLidRadius(G4double val) { fStringerLidRadius = val; }
  void  SetStringerLidMaterialName(G4String val) { fStringerLidMaterialName = val; }
  void  SetStringerSupportThickness(G4double val) { fStringerSupportThickness = val; }
  void  SetSupportRodLength(G4double val) { fSupportRodLength = val; } 
  void  SetSupportRodInnerRadius(G4double val) { fSupportRodInnerRadius = val; }
  void  SetSupportRodOuterRadius(G4double val) { fSupportRodOuterRadius = val; }
  void  SetSupportRodMaterialName(G4String val) { fSupportRodMaterialName = val; }
  void  SetContactPressureRingHeight(G4double val) { fContactPressureRingHeight = val; }
  void  SetContactPressureRingInnerRadius(G4double val) { fContactPressureRingInnerRadius = val; }
  void  SetContactPressureRingOuterRadius(G4double val) { fContactPressureRingOuterRadius = val; }
  void  SetContactPressureRingMaterialName(G4String val) { fContactPressureRingMaterialName = val; }
  void  SetGeTrayHeight(G4double val) { fGeTrayHeight = val; }
  void  SetGeTrayInnerRadius(G4double val) { fGeTrayInnerRadius = val; }
  void  SetGeTrayOuterRadius(G4double val) { fGeTrayOuterRadius = val; }
  void  SetGeTrayArmWidth(G4double val) { fGeTrayArmWidth = val; }
  void  SetGeTrayStubRadius(G4double val) { fGeTrayStubRadius = val; }
  void  SetGeTrayStubHeight(G4double val) { fGeTrayStubHeight = val; }
  void  SetGeTrayMaterialName(G4String val) { fGeTrayMaterialName = val; }
  void  SetLFEPAsmHeight(G4double val) { fLFEPAsmHeight = val; }
  void  SetLFEPAsmWidth(G4double val) { fLFEPAsmWidth = val; }
  void  SetLFEPAsmThickness(G4double val) { fLFEPAsmThickness = val; }
  void  SetLFEPAsmMaterialName(G4String val) { fLFEPAsmMaterialName = val; }
  void  SetOffset(G4ThreeVector val) { fOffset = val; }
  void  SetOneOfMany(G4bool isMany) { fIsOneOfMany = isMany; }
  void  SetTemplateCrystalColumnSerNum(G4String val) { fTemplateCrystalColumnSerNum = val; } 
  void  SetSupportRodRadialPosition(G4double val) { fSupportRodRadialPosition = val; }
  void  SetColumnRotationZ(G4double val) { fColumnRotationZ = val; }
  void  SetLFEPAsmRadialPosition(G4double val) { fLFEPAsmRadialPosition = val; }
  void  SetLFEPAsmRotationZ(G4double val) { fLFEPAsmRotationZ = val; }
  void  SetLFEPAsmSpacing(G4double val) { fLFEPAsmSpacing = val; }
  void  AddGeTrayZPositions(G4double val) { fGeTrayZPositions.push_back(val); }
  void  AddContactPressureRingsZPositions(G4double val) { fContactPressureRingsZPositions.push_back(val); }

private:

  void Initialize();

  // Dimensions of parts to be provided by the user.
  // (from top to bottom)
  // Note that the crystal dimensions have to be set via the crystal messenger.
  // --------------------------------------

  // Stringer Lid
  G4double     fStringerLidThickness;
  G4double     fStringerLidRadius;
  G4String     fStringerLidMaterialName;
  G4double     fStringerSupportThickness; // This one is typically set externally.

  // Support rods
  G4double     fSupportRodLength; 
  G4double     fSupportRodInnerRadius;
  G4double     fSupportRodOuterRadius;
  G4String     fSupportRodMaterialName;

  // Contact Pressure Rings
  G4double    fContactPressureRingHeight;
  G4double    fContactPressureRingInnerRadius;
  G4double    fContactPressureRingOuterRadius;
  G4String    fContactPressureRingMaterialName;

  // Ge Tray
  G4double    fGeTrayHeight;
  G4double    fGeTrayInnerRadius;
  G4double    fGeTrayOuterRadius;
  G4double    fGeTrayArmWidth;
  G4double    fGeTrayStubRadius;
  G4double    fGeTrayStubHeight;
  G4String    fGeTrayMaterialName;

  // LFEP Assembly
  G4double    fLFEPAsmHeight;
  G4double    fLFEPAsmWidth;
  G4double    fLFEPAsmThickness;
  G4String    fLFEPAsmMaterialName;

  // If fMotherVolume is specified (!=NULL), then the crystal assembly is built inside
  // fMotherVolume with fOffset. Otherwise, fMothervolume is set to an enclosing cylinder that
  // is returned as theDetectorLogical.
  // fOffset is set to the location of the bottom of the bottom-most GeTray.
  G4ThreeVector    fOffset;
  G4bool           fIsOneOfMany;
  G4String         fTemplateCrystalColumnSerNum; 
                  // Serial number of crystal column that serves as template.

  // Relative placements of parts to be provided by user.
  // All z coordinates are relative to the bottom of the lowest GeTray
  // Crystals sit right on top of the Ge Trays, ie. their location is defined by them. 
  // ---------------------------------------------------------------------------------

  G4double           fSupportRodRadialPosition;
  G4double           fColumnRotationZ;       // Default=0 => first support rod on +y axis.
                                             // Rotation is clockwise.
  G4double           fLFEPAsmRadialPosition; // Center of middle board.
  G4double           fLFEPAsmRotationZ;      // Default = 30o
  G4double           fLFEPAsmSpacing;        // Spacing between LFEP boards. 
  vector<G4double>   fGeTrayZPositions;      // Bottom of Ge Tray Z position.
  vector<G4double>   fContactPressureRingsZPositions; // Bottom of ring.

  // Automatically computed values.
  // ------------------------------

  G4double               fEps;
  G4double               fAssemblyHeight;  // Height of cylinder enclosing entire column.
  G4double               fAssemblyRadius;  // Radius of cylinder enclosing entire column. 
  size_t                 fNumberOfCrystals;      // # of crystals in column.
  vector<G4ThreeVector>  fGeTrayPositions;
  vector<G4ThreeVector>  fContactPressureRingPositions;
  vector<G4ThreeVector>  fCrystalPositions;
  vector<G4ThreeVector>  fLFEPAsmPositions;
  G4Material             *fStringerLidMaterial;
  G4Material             *fSupportRodMaterial;
  G4Material             *fContactPressureRingMaterial;
  G4Material             *fGeTrayMaterial;
  G4Material             *fLFEPAsmMaterial;

  // Crystal definitions.
  MJGeometryIdealCoaxWithShield          *fTemplateCrystal; // Note this is not owned by this class.
                                                            // The user must instantiate and delete
                                                            // separately. It should be constructed
                                                            // before calling ContructDetector()
  vector<MJGeometryIdealCoaxWithShield*>  fCrystals;

  // Visualization Attributes
  G4VisAttributes       *fCuObjectsVisAtt;
  G4VisAttributes       *fTeflonObjectsVisAtt;
  G4VisAttributes       *fMotherVolumeVisAtt;
 
  // Misc.
  MJGeometryRDCrystalColumnMessenger     *fMessenger;
  G4bool               fIsDEMO;
};
#endif

