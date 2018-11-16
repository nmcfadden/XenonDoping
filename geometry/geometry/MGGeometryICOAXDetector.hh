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
 * $Id: MGGeometryICOAXDetector.hh, v1.0 $
 *      
 * CLASS DECLARATION:  MGGeometryICOAXDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *  Inverted coaxial (ICOAX) detector geometry.
 *  -> BEGe like geometry with a well on the opposite side of the p-contact
 *  -> Available features : - well(core) radius/depth
 *                          - well(core) rounding, i.e. half sphere at the bottom
 *                          - bevel height/angle on the outer upper side 
 *                          - groove(ditch) dimensions 
 *
 *  Essentially based on MGGeometryIdealCoaxCrystal.cc 
 *  
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 * The origin of the crystal is the center of the crystal - radius 0, half way
 * along the Z axis.
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Y. Kermaidic
 * CONTACT: yoann.kermaidic@mpi-hd.mpg.de
 *
 * 
 * REVISION:
 * 
 * 13-12-2016, Created, Y. Kermaidic
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYICOAXDETECTOR_HH
#define _MGGEOMETRYICOAXDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;
class G4LogicalVolume;
class G4VisAttributes;

class MGGeometryICOAXDetectorMessenger;

class MGGeometryICOAXDetector : public MGGeometryDetector
{
public:
  MGGeometryICOAXDetector(G4String serNum, G4bool messOff, 
			     G4bool many = false);
 
  MGGeometryICOAXDetector(const MGGeometryICOAXDetector &rhs);
  ~MGGeometryICOAXDetector();
  virtual bool UsesMJPSQLDatabase() { return true; }  

  G4bool           SetNameOn() { return fSetNameAct; }
  G4String	   GetSetName()  {return fSetName;}
  G4bool           CoreBubbleOn() { return fCoreBubbleOn; }
  G4bool           DeadLayerConsidered() { return fDeadLayerConsidered; }
  G4bool           ExcessSideDeadLayerConsidered() { return fExcessSideDeadLayerConsidered; }  //M.Kidd 4/5/2011
  //  G4bool           ExtraDeadLayerConsidered() { return fExtraDeadLayerConsidered; }
  G4LogicalVolume *GetActiveVolumeLogical() { return fActiveVolumeLogical; }
  G4double         GetCoreDeadLayerThickness(){return fCoreDeadLayerThickness;}
  G4double         GetCoreDepth() { return fCoreDepth; }
  G4double         GetCoreRadius() { return fCoreRadius; }
  G4bool           GetCoreRounding() { return fCoreRounding; }
  G4double         GetCornerRadius() { return fCornerRadius; }
  G4double         GetCrystalMass() { return fCrystalMass; } 
  /*
// Vic Gehman, March 31, 2009...
  G4double         GetExtraDeadLayerBottomZ() {return fExtraDeadLayerBottomZ;}
  G4double         GetExtraDeadLayerMiddleZ() {return fExtraDeadLayerMiddleZ;}
  G4double         GetExtraDeadLayerTopZ() {return fExtraDeadLayerTopZ;}
  G4double         GetExtraDeadLayerBottomInnerR() {return fExtraDeadLayerBottomInnerR;}
  G4double         GetExtraDeadLayerMiddleInnerR() {return fExtraDeadLayerMiddleInnerR;}
  G4double         GetExtraDeadLayerTopInnerR() {return fExtraDeadLayerTopInnerR;}
  G4double         GetExtraDeadLayerBottomOuterR() {return fExtraDeadLayerBottomOuterR;}
  G4double         GetExtraDeadLayerMiddleOuterR() {return fExtraDeadLayerMiddleOuterR;}
  G4double         GetExtraDeadLayerTopOuterR() {return fExtraDeadLayerTopOuterR;}
  */
// Back to the regularly scheduled ICOAX
  G4Material      *GetCrystalMaterial() { return fCrystalMaterial; }
  G4double         GetCrystalVolume() { return fCrystalVolume; }
  G4bool           GetCutInHalf() { return fCutInHalf; }
  G4String         GetG4MaterialName() { return fCrystalMaterialName; }
  G4double         GetHeight() { return fHeight; }
  G4bool           GetIsOneOfMany() { return fIsOneOfMany; }
  G4double         GetOuterDeadLayerThickness() { return fOuterDeadLayerThickness; }
  G4double         GetRadius() { return fRadius; }
  G4bool           GetIsActive() { return fIsActive; }
  G4bool	   BeveledEdgeConsidered() { return fBeveledEdgeConsidered; }   //added by M. Kidd 1/12/11  //incorporating beveled edge
  G4double         GetBeveledEdgeAngle() { return fBeveledEdgeAngle; }
  G4double         GetBeveledEdgeT() { return fBeveledEdgeT; }

  G4double GetDitchDepth(){ return fDitchDepth; }
  G4double GetDitchInnerRadius(){ return fDitchInnerRadius; }
  G4double GetDitchOuterRadius(){ return fDitchOuterRadius; }
  
  void ActivateSetName(G4bool setNameOn) { fSetNameAct = setNameOn; }											 	
//option
  void SetName(G4String addname) {fSetName = addname;} 
  void SetCoreBubble(G4bool bubon) { fCoreBubbleOn = bubon; }
  void SetCoreRounding(G4bool rounding) { fCoreRounding = rounding; }
  void SetCoreDeadLayerThickness(G4double th){ fCoreDeadLayerThickness = th; } 
  void SetCoreDepth(G4double depth) { fCoreDepth = depth; }
  void SetCoreRadius(G4double radius) { fCoreRadius = radius; }
  void SetCornerRadius(G4double radius) { fCornerRadius = radius; }
  void SetCrystalMaterial(G4Material *mat) { fCrystalMaterial = mat; }
  void SetCutInHalf(G4bool cut) { fCutInHalf = cut; }
  void SetDeadLayerConsidered(G4bool don) { fDeadLayerConsidered = don; }
  void SetExcessSideDeadLayerConsidered(G4bool sidedon) { fExcessSideDeadLayerConsidered = sidedon; }  //M.Kidd 4/5/2011
  void SetDitchOuterRadius( G4double rad ){ fDitchOuterRadius = rad; }
  void SetDitchInnerRadius( G4double rad ){ fDitchInnerRadius = rad; }
  void SetDitchDepth( G4double depth ){ fDitchDepth = depth; }
  
  /*
// Adding in extra dead layer...
  void SetExtraDeadLayerConsidered(G4bool edon) { fExtraDeadLayerConsidered = edon; }
  void SetExtraDeadLayerBottomZ(G4double bz){ fExtraDeadLayerBottomZ = bz; } 
  void SetExtraDeadLayerMiddleZ(G4double mz){ fExtraDeadLayerMiddleZ = mz; } 
  void SetExtraDeadLayerTopZ(G4double tz){ fExtraDeadLayerTopZ = tz; } 
  void SetExtraDeadLayerBottomInnerR(G4double bir){ fExtraDeadLayerBottomInnerR = bir; } 
  void SetExtraDeadLayerMiddleInnerR(G4double mir){ fExtraDeadLayerMiddleInnerR = mir; } 
  void SetExtraDeadLayerTopInnerR(G4double tir){ fExtraDeadLayerTopInnerR = tir; } 
  void SetExtraDeadLayerBottomOuterR(G4double bor){ fExtraDeadLayerBottomOuterR = bor; } 
  void SetExtraDeadLayerMiddleOuterR(G4double mor){ fExtraDeadLayerMiddleOuterR = mor; } 
  void SetExtraDeadLayerTopOuterR(G4double tor){ fExtraDeadLayerTopOuterR = tor; } 
  */
// Back to the regularly scheduled idealCoax
  void SetG4MaterialName(G4String name) { fCrystalMaterialName = name; }
  void SetHeight(G4double height) { fHeight = height; }
  void SetIsOneOfMany(G4bool val) { fIsOneOfMany = val;}
  void SetOuterDeadLayerThickness(G4double th) {fOuterDeadLayerThickness = th;}
  void SetExcessSideDeadLayerThickness(G4double sth) {fExcessSideDeadLayerThickness = sth;}
  void SetRadius(G4double radius) { fRadius = radius; }
  void SetIsActive(G4bool val) { fIsActive = val;}
  void SetBeveledEdgeConsidered(G4bool bevEdgeOn) { fBeveledEdgeConsidered = bevEdgeOn;}  //added by M. Kidd 1/12/11
         										//incorporating beveled edge
											//option
  void SetBeveledEdgeAngle(G4double bevAngle)  {fBeveledEdgeAngle = bevAngle;}
  void SetBeveledEdgeT(G4double bevEdgeT)  {fBeveledEdgeT = bevEdgeT;}  //this is the thickness you want shaved off by the bevel - the horizontal distance from the edge of the crystal to the inner edge of the bevel

  void CalculateMassAndVolume();
  void ConstructDetector();
  void Dump();
  virtual void SetDefaults();

private:
  void ConstructDetectorAsOneOfMany();
  void ConstructDetectorAsSingle();

protected:

  G4bool fSetNameAct;
  G4String fSetName;

  // Required Crystal dimensions. Default values assigned in constructor.
  G4double fCoreRadius;  // Radius of inner core
  G4double fCoreDepth;   // Depth of core. If fCoreDepth >= fRadius, then
                         // the crystal is made with through-going hole.
  G4double fHeight;      // Height (z) of cylinder
  G4double fRadius;      // Outer radius of crystal
  
  //Optional crystal dimensions and options.
  G4bool   fCoreRounding; // Make core rounding
  G4bool   fCoreBubbleOn; // Make a bubble in the core, ie. the core has a 
                          // hemispherical bottom.
  G4bool   fDeadLayerConsidered; // Deadlayers are considered.
  G4bool   fExcessSideDeadLayerConsidered; // Deadlayers are considered.
  G4double fCornerRadius; // Make rounded corners on edge on opposite of hole
                          // with this radius. Value <=0 implies no rounded
                          // corner.
  G4double fOuterDeadLayerThickness;
  G4double fExcessSideDeadLayerThickness;
  G4double fCoreDeadLayerThickness;// Thicknesses of core and outer deadlayers.
  /*
// Vic Gehman, March 31, 2009...
  G4bool   fExtraDeadLayerConsidered; // Extra dead layer for Donald/MJ70 considered
  G4double fExtraDeadLayerBottomZ;//Parameters of the extra dead layer for Donald/MJ70
  G4double fExtraDeadLayerMiddleZ;
  G4double fExtraDeadLayerTopZ;
  G4double fExtraDeadLayerBottomInnerR;
  G4double fExtraDeadLayerMiddleInnerR;
  G4double fExtraDeadLayerTopInnerR;
  G4double fExtraDeadLayerBottomOuterR;
  G4double fExtraDeadLayerMiddleOuterR;
  G4double fExtraDeadLayerTopOuterR;
  G4double ExtraDeadLayerZ[3], ExtraDeadLayerInnerR[3], ExtraDeadLayerOuterR[3];
  */
//M. Kidd 1/12/11 Beveled edge option
  G4bool fBeveledEdgeConsidered;
  G4double fBeveledEdgeAngle;
  G4double fBeveledEdgeT;

  // Y. Kermaidic 12/15/2016 Ditch size option
  G4double fDitchOuterRadius;
  G4double fDitchInnerRadius;
  G4double fDitchDepth;

  // Computed and utility values. Self-explanatory
  G4double      fCrystalMass;
  G4double      fCrystalVolume;
  G4Material    *fCrystalMaterial;
  G4String      fCrystalMaterialName;

  // Flag that cuts crystal in half -- useful for debuggin and visualization
  G4bool        fCutInHalf;

  G4bool        fIsOneOfMany; // Set to true to re-use the same logical
                              // volume and solids, but to give each physical
                              // volume (placement) a unique name based on the
                              // serial number.

  // Logical volume of crystal.
  G4LogicalVolume *fActiveVolumeLogical;

  G4bool        fIsActive;  // A. Schubert 061121

  // Messenger

private:
  MGGeometryICOAXDetectorMessenger   *fMessenger;
  G4VisAttributes                *fWholeCrystalVisAtt;
  G4VisAttributes                *fActiveCrystalVisAtt;
};
#endif
