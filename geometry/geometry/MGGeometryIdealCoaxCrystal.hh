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
 * $Id: MGGeometryIdealCoaxCrystal.hh,v 1.8 2007/02/21 14:22:55 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MGGeometryIdealCoaxCrystal.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This is a idealized coaxial crystal. 
 * It is a cylinder with a hole in it.
 * It can have rounded edges at the side away from the hole.
 * 
 * It can also be repeatedly instantiated to define a set of identical 
 * crystals, but with 
 * unique physical volumes (placements). To use this feature, set the 'many' 
 * parameter in the constructor to true. You must define a 'template' crystal
 * first with the 'many' parameter set to false. This template crystal has 
 * to be constructed (via ConstructDetector()), but does not have to be 
 * placed in the mother detector. 
 * All the copies of the template class should be set equal to it before the
 * invocation of the copies' ConstructDetector().
 *
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
 * AUTHOR: Reyco Henning
 * CONTACT: 

 * 
 * REVISION:
 * 
 * 10-12-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 12-13-2004, Renamed to MGGeometryIdealCoxialCrystal, R. Henning
 * 01-06-2004, Added flag to turn messenger off. R. Henning
 * 02-21-2005, Added IsOneOfMany feature, R. Henning
 * 11-17-2005, Updated copy constructor to allow use of this class in STL 
 *             container classes. R. Henning.
 * 02-21-2007, Added way to make crystal inactive, A. Schubert
 * 03-31-2009, Added ExtraDeadLayer definitions
 * 01-12-2011, Added flag for beveled edge M. Kidd
 * 01-15-2011, Added extra side dead layer M. Kidd
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYIDEALCOAXCRYSTAL_HH
#define _MGGEOMETRYIDEALCOAXCRYSTAL_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;
class G4LogicalVolume;
class G4VisAttributes;

class MGGeometryIdealCoaxMessenger;

class MGGeometryIdealCoaxCrystal : public MGGeometryDetector
{
public:
  MGGeometryIdealCoaxCrystal(G4String serNum, G4bool messOff, 
			     G4bool many = false);
 
  MGGeometryIdealCoaxCrystal(const MGGeometryIdealCoaxCrystal &rhs);
  ~MGGeometryIdealCoaxCrystal();
  virtual bool UsesMJPSQLDatabase() { return true; }  

  G4bool           SetNameOn() { return fSetNameAct; }
  G4String	   GetSetName()  {return fSetName;}
  G4bool           CoreBubbleOn() { return fCoreBubbleOn; }
  G4bool           DeadLayerConsidered() { return fDeadLayerConsidered; }
  G4bool           ExcessSideDeadLayerConsidered() { return fExcessSideDeadLayerConsidered; }  //M.Kidd 4/5/2011
  G4bool           ExtraDeadLayerConsidered() { return fExtraDeadLayerConsidered; }
  G4LogicalVolume *GetActiveVolumeLogical() { return fActiveVolumeLogical; }
  G4double         GetCoreDeadLayerThickness(){return fCoreDeadLayerThickness;}
  G4double         GetCoreDepth() { return fCoreDepth; }
  G4double         GetCoreRadius() { return fCoreRadius; }
  G4double         GetCornerRadius() { return fCornerRadius; }
  G4double         GetCrystalMass() { return fCrystalMass; } 
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
// Back to the regularly scheduled idealCoax
  G4Material      *GetCrystalMaterial() { return fCrystalMaterial; }
  G4double         GetCrystalVolume() { return fCrystalVolume; }
  G4bool           GetCutInHalf() { return fCutInHalf; }
  G4String         GetG4MaterialName() { return fCrystalMaterialName; }
  G4double         GetHeight() { return fHeight; }
  G4bool           GetIsOneOfMany() { return fIsOneOfMany; }
  G4double         GetOuterDeadLayerThickness() 
                     { return fOuterDeadLayerThickness; }
  G4double         GetRadius() { return fRadius; }
  G4bool           GetIsActive() { return fIsActive; }
	G4bool		   BeveledEdgeConsidered() { return fBeveledEdgeConsidered; }   //added by M. Kidd 1/12/11
																				//incorporating beveled edge
  void ActivateSetName(G4bool setNameOn) { fSetNameAct = setNameOn; }											 	//option
  void SetName(G4String addname) {fSetName = addname;} 
  void SetCoreBubble(G4bool bubon) { fCoreBubbleOn = bubon; }
  void SetCoreDeadLayerThickness(G4double th){ fCoreDeadLayerThickness = th; } 
  void SetCoreDepth(G4double depth) { fCoreDepth = depth; }
  void SetCoreRadius(G4double radius) { fCoreRadius = radius; }
  void SetCornerRadius(G4double radius) { fCornerRadius = radius; }
  void SetCrystalMaterial(G4Material *mat) { fCrystalMaterial = mat; }
  void SetCutInHalf(G4bool cut) { fCutInHalf = cut; }
  void SetDeadLayerConsidered(G4bool don) { fDeadLayerConsidered = don; }
  void SetExcessSideDeadLayerConsidered(G4bool sidedon) { fExcessSideDeadLayerConsidered = sidedon; }  //M.Kidd 4/5/2011
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
//M. Kidd 1/12/11 Beveled edge option
	G4bool fBeveledEdgeConsidered;
	G4double fBeveledEdgeAngle;
	G4double fBeveledEdgeT;
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
  MGGeometryIdealCoaxMessenger   *fMessenger;
  G4VisAttributes                *fWholeCrystalVisAtt;
  G4VisAttributes                *fActiveCrystalVisAtt;
};
#endif
