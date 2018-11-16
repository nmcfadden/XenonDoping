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
 * CLASS DECLARATION:  GEGeometryLArInstHybrid.hh
 *
 * AUTHOR: Nuno Barros
 *
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 *
 * DESCRIPTION:
 *
 *    Class declaration for an hybrid design of LAr Instrumentation merging both fibers and PMTs.
 *
 * FIRST SUBMISSION: 2012-08-14
 *
 * REVISION: MM-DD-YYYY
 * 09-06-2012, Nuno, * Added a pointer to TPB material definition since it is used by both PMTs and fibers.
 * 09-24-2012, Nuno, * Extended class to keep track of additional volumes required to build the hybrid design.
 *
 */

#ifndef GEGEOMETRYLARINSTHYBRID_HH_
#define GEGEOMETRYLARINSTHYBRID_HH_

#include "gerdageometry/GEGeometryLArDesignBase.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class TGraph;
class G4OpticalSurface;

class GEGeometryLArInstHybrid: public GEGeometryLArDesignBase {
public:
  GEGeometryLArInstHybrid(GEGeometryDetectorDB* theGeometryDB);
  virtual ~GEGeometryLArInstHybrid();
  void ConstructDesign();
  void ConstructCable_FE_Stuff();
//   void ConstructHVCables();
  virtual G4LogicalVolume *GetLArInstrSDLogical(G4int index = 0);
  virtual G4int GetNumSDLogicVols();
  virtual G4String GetLogicalSDName(G4int index = 0);
  virtual G4int GetNLogicalInstances(G4int index = 0);

  void SetEnableCentralFibers(G4bool status) {fEnableCentralFibers = status;};
  void SetEnableSegmentedFiberShroud(G4bool status) {fEnableSegmentedFiberShroud = status;};
protected:
  /** To make the design more modular and easier to implement, each sub-design is
   * constructed independently, making this class almost as a wrapper for two separate designs.
   */
  void ConstructPMTSubDesign();
  void ConstructFiberSubDesign();
  /** Build a shroud for the fibers.
   *
   * If the endcap is false, then the solid is just a G4Tubs. If endcaps is true, then the solid
   * is a G4Polycone, with rounded endcaps. The "roundness", is determined by the additional length
   * (as a fraction of the original height) that is allowed the endcap to add to the solid. By default 5% is used.
   *
   * @param outer_radius Outer radius of the fiber.
   * @param thickness Thickness of the shroud (fiber).
   * @param height Minimum height of the volume. If endcap is true, then the total height is \f[height + 2\times endcap\_ratio \times height\f].
   * @param endcap Enable the construction if the rounded endcaps. Chnages the type of solid build from G4Tubs to G4Polycone.
   * @param endcap_ratio Ratio of total height that the endcap adds to the solid as a fraction of the original height.
   * @return Solid building the corresponding "shroud".
   */
  G4VSolid* BuildFiberShroudSolid(G4double outer_radius,G4double thickness, G4double height, G4bool endcap = false, G4double endcap_ratio = 0.05);
  G4VSolid* BuildFiberShroudSegmentSolid(G4double radius, G4int noOfSegments, G4double segment_spacing, G4double thickness, G4double total_thickness, G4double height);



private:
  G4double TPBEmissionSpectrum(G4double);
  void InitializeTPBSpectra();
  void InitializeFiberSpectra();
  G4double FiberEmissionSpectrum(G4double);
  G4double 	CalculateWLSmfp(G4double);

  TGraph  *fTPBspec; // VM2000 re-emission spectrum
  TGraph  *fFibersAbsorptionSpec; // VM2000 re-emission spectrum
  TGraph  *fFibersEmissionSpec; // VM2000 re-emission spectrum

  static const 	G4int npoints = 250;
  G4double 	frequencyV[npoints];
  G4double 	efficiencyV[npoints];

  G4bool 	successfulInitialization;
  G4ThreeVector fDetCenter; /// Position for the center of the detector

  G4bool fEnableCentralFibers;

  // There is only one property that is really shared between both sub-designs
  // the height of the central part of the instrumentation shroud (that is now built in fibers)
  G4double fTotal_height;
  G4double fShroud_TubsHeight;
  G4double fShroud_TubsOuterRadius;
  G4double fShroud_Offset;
  G4double fShroud_CenterHeight;
  G4double fFiberThickness;
  G4double fFiberCladThickness;
  G4double fWLSThickness;
  G4double fFiberLightYield; //limit on scint. photons JJ 
  G4double fShroud_Thickness;
  G4double fVM2000_Thickness;
  G4double fRotatePMTInstAngle;
  
  static const G4double LambdaE;

  G4Material *fTPB;

  // Define the rough optical surface between WLS and LAr. Valid for all designs
  G4OpticalSurface *fWLSoptSurf;
  G4OpticalSurface *fTefOptSurf;
  G4VPhysicalVolume *fMotherPhysical;

  G4VPhysicalVolume *fShroudPhysical_top;
  G4VPhysicalVolume *fShroudPhysical_bottom;

  G4LogicalVolume *fShroudLogical;

  // segmented fiber shroud
  G4bool   fEnableSegmentedFiberShroud;
  G4int    fNoOfSegments;
  G4double fSegmentSpacing;
};

#endif /* GEGEOMETRYLARINSTPMT_HH_ */
