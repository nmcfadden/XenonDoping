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
 * CLASS DECLARATION:  GEGeometryDetectorParameters.hh
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 11-18-2005
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-06-2006, added more parameters (material of dead layers, enrichment, etc.) 
 *   30-08-2011, (Neslihan)
 *               added parameters (inner radius, outer radius, height, dead layer thickness of the groove) 
 *               for the non-true coaxial geometry.
 *
 */

#ifndef _GEGEOMETRYCRYSTALPARAMETERS_HH 
#define _GEGEOMETRYCRYSTALPARAMETERS_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh"

// --------------------------------------------------------------------------- 

class GEGeometryDetectorParameters 
{

public:

  /// Default constructor
  GEGeometryDetectorParameters(); 

  /// Descturcot
  ~GEGeometryDetectorParameters();

  // getters 

  G4int GetDetectorID()
  { return fDetectorID; }; 
  
  G4String GetName()
  { return fName; }; 
  
  G4int GetType()
  { return fType; }; 

  G4int GetGeometry()
  { return fGeometry; }; 

  G4double GetInnerRadius()
  { return fInnerRadius; };

  G4double GetOuterRadius()
  { return fOuterRadius; };

  G4double GetHeight()
  { return fHeight; };

  G4double GetBoreDepth() 
  { return fBoreDepth; }; 

  G4double GetDLInnerThickness() 
  { return fDLInnerThickness; }; 
  
  G4double GetDLOuterThickness() 
  { return fDLOuterThickness; }; 

  G4double GetDLTopThickness() 
  { return fDLTopThickness; }; 

  G4double GetDLBottomThickness() 
  { return fDLBottomThickness; }; 

  G4double GetPassivationThickness() 
  { return fPassivationThickness; }; 

  G4double GetGrooveInnerRadius() 
  { return fGrooveInnerRadius; }; 

  G4double GetGrooveOuterRadius() 
  { return fGrooveOuterRadius; };

  G4double GetGrooveHeight() 
  { return fGrooveHeight; };

  G4double GetGrooveDL() 
  { return fGrooveDL; };

  G4double GetImpurityDensityTop() 
  { return fImpurityDensityTop; }; 

  G4double GetImpurityDensityBottom() 
  { return fImpurityDensityBottom; }; 

  G4int    GetNPhiSegments() 
  { return fNPhiSegments; }; 

  G4int    GetNZSegments() 
  { return fNZSegments; }; 

  G4double GetSegmentationAngle() 
  { return fSegmentationAngle; }; 

  G4double GetCrystalAngle() 
  { return fCrystalAngle; }; 

  G4String GetBulkMaterial()
  { return fBulkMaterial; }; 

  G4String GetDLMaterial()
  { return fDLMaterial; }; 

  G4String GetPassivationMaterial()
  { return fPassivationMaterial; }; 

  G4double GetSegmentHeight();  

  // setters 
  
  void SetDetectorID(G4int ID) 
  { fDetectorID = ID; }; 

  void SetName(G4String name) 
  { fName = name; }; 

  void SetType(G4int type) 
  { fType = type; }; 

  void SetGeometry(G4int geometry) 
  { fGeometry = geometry; }; 
  
  void SetInnerRadius(G4double r) 
  { fInnerRadius = r; }; 

  void SetOuterRadius(G4double r) 
  { fOuterRadius = r; }; 

  void SetHeight(G4double h)
  { fHeight = h; }; 

  void SetBoreDepth(G4double d)
  { fBoreDepth = d; }; 

  void SetDLInnerThickness(G4double t)
  { fDLInnerThickness = t; }; 

  void SetDLOuterThickness(G4double t)
  { fDLOuterThickness = t; }; 

  void SetDLTopThickness(G4double t)
  { fDLTopThickness = t; }; 

  void SetDLBottomThickness(G4double t)
  { fDLBottomThickness = t; }; 

  void SetPassivationThickness(G4double t) 
  { fPassivationThickness = t; }; 

  void SetGrooveInnerRadius(G4double r) 
  { fGrooveInnerRadius = r; };

  void SetGrooveOuterRadius(G4double r) 
  { fGrooveOuterRadius = r; };

  void SetGrooveHeight(G4double h) 
  { fGrooveHeight = h; };

  void SetGrooveDL(G4double t) 
  { fGrooveDL = t; };

  void SetImpurityDensityTop(G4double density) 
  { fImpurityDensityTop = density; }; 

  void SetImpurityDensityBottom(G4double density) 
  { fImpurityDensityBottom = density; }; 

  void SetNPhiSegments(G4int n)
  { fNPhiSegments = n; }; 

  void SetNZSegments(G4int n)
  { fNZSegments = n; }; 

  void SetSegmentationAngle(G4double phi)
  { fSegmentationAngle = phi; }; 

  void SetCrystalAngle(G4double phi)
  { fCrystalAngle = phi; }; 

  void SetBulkMaterial(G4String material)
  { fBulkMaterial = material; }; 

  void SetDLMaterial(G4String material)
  { fDLMaterial = material; }; 

  void SetPassivationMaterial(G4String material)
  { fPassivationMaterial = material; }; 

private: 

  G4int fDetectorID; 

  G4String fName; 

  G4int fGeometry; 

  // 0 : no crystal 
  // 1 : Non-true coaxial
  // 2 : True coaxial (segmented) 
  // 3 : back-to-back BEGe (extension of geometry 1)

  G4int fType; 

  // 0 : n-type 
  // 1 : p-type 

  G4double fInnerRadius; 
  G4double fOuterRadius; 
  G4double fHeight; 
  G4double fBoreDepth; 

  G4double fDLInnerThickness; 
  G4double fDLOuterThickness; 
  G4double fDLTopThickness; 
  G4double fDLBottomThickness; 

  G4double fPassivationThickness;

  G4double fGrooveInnerRadius; 
  G4double fGrooveOuterRadius; 
  G4double fGrooveHeight; 
  G4double fGrooveDL; 

  G4double fImpurityDensityTop; 
  G4double fImpurityDensityBottom; 

  G4int    fNPhiSegments; 
  G4int    fNZSegments; 
  G4double fSegmentationAngle; 
  G4double fCrystalAngle; 

  G4String fBulkMaterial; 
  G4String fDLMaterial; 
  G4String fPassivationMaterial; 

};

#endif

