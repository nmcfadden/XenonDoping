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
//
// This class samples the position of the primary 
// particle randomly in a geometrical volume or surface (sphere, disk, cylinder, etc.) 
//
// History:
// --------
// 30 Nov 2007   L.Pandola    First implementation 
//
// ---------------------------------------------------------------------------

#ifndef MGGeneratorPositionSamplingGeometrical_h
#define MGGeneratorPositionSamplingGeometrical_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"


//This class is supposed to sample one random point uniformly from 
//a given geometrical volume or surface

class MGGeneratorUtil;
class MGGeneratorPositionSamplingGeometricalMessenger;

class MGGeneratorPositionSamplingGeometrical 
{
public:
  MGGeneratorPositionSamplingGeometrical(); 
  ~MGGeneratorPositionSamplingGeometrical();
  
  G4ThreeVector SampleInGeometricalVolume();
  G4ThreeVector SampleInGeometricalSurface();

  void SetGeometricalVolumeName(G4String name){fGeometricalVolumeName = name;};
  G4String GetGeometricalVolumeName(){return fGeometricalVolumeName;};
  
  void SetGeometricalSurfaceName(G4String name){fGeometricalSurfaceName = name;};
  G4String GetGeometricalSurfaceName(){return fGeometricalSurfaceName;};

  void SetCenterCoordinates(G4ThreeVector coord){fCenterCoordinates = coord;};

  void SetInnerSphereRadius(G4double isr){fInnerSphereRadius = isr;};
  void SetOuterSphereRadius(G4double osr){fOuterSphereRadius = osr;};
  void SetInnerDiskRadius(G4double idr){fInnerDiskRadius = idr;};
  void SetOuterDiskRadius(G4double odr){fOuterDiskRadius = odr;};
  void SetInnerCylinderRadius(G4double icr){fInnerCylinderRadius = icr;};
  void SetOuterCylinderRadius(G4double ocr){fOuterCylinderRadius = ocr;};
  void SetCylinderHeight(G4double cylh){fCylinderHeight = cylh;};

private:
  G4String  fGeometricalSurfaceName;
  G4String fGeometricalVolumeName;

  MGGeneratorUtil* fGeneratorUtil;

  G4ThreeVector fCenterCoordinates;
  
  //Sphere
  G4double fInnerSphereRadius;
  G4double fOuterSphereRadius;
 
  //Disk
  G4double fInnerDiskRadius;
  G4double fOuterDiskRadius;
 
  //Cylinder
  G4double fInnerCylinderRadius;
  G4double fOuterCylinderRadius;
  G4double fCylinderHeight;

  //Messenger
  MGGeneratorPositionSamplingGeometricalMessenger* fMessenger;
};
#endif
