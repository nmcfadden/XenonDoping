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
// particle randomly in a volume or on a surface. The surface 
// algorithms are not implemented, but the interfaces work.
//
// History:
// --------
// 28 Oct 2004   L.Pandola    First implementation (interfaces ok)
// 19 Nov 2004   L.Pandola    Implemented a modified 
//                            version of Dave Jordan algorithm
// 06 Dec 2004   L.Pandola    Re-naming
// 11 Mar 2005   R. Henning   Added fHasDaughters. 
// 15 Apr 2005   X. Liu       Added surface sampling for Subtraction Solid
// 27 Sep 2006   L. Pandola   Added two vectors with translations and 
//                            rotations
// 22 Feb 2008   D. Lenz      Added new variables for sampling from self defined bounding volumes 
//                            (Sphere,Cylinder,Box)
// 30 May 2008   L. Pandola   Added fWorldVolumeHalfLength private data member to store the 
//                            dimensions of the world box, used for dummy vertex.  
// 30 May 2008   Daniel       possibility to add an offset to the position of the center of the bounding volume 
//                            through MGGeneratorPositionSamplingMessenger. 
//                            USE WITH CARE!!!
// 03 Jun 2008   Luciano      Changed name of the variable SetCenterManually according to the MaGe coding rules
//                            Added Getter/Setter for fUseCenterManually.
// 04 Aug 2008   Daniel       Added  two variable for calculation of "true" size of sampling volume if you use MGGeneratorPositionSamplingMessenger "Sphere". 
//                            Needed i.e. for some estimate of liquid argon contamination
// 07 Jan 2009   Jens         The maximum number of tries to find a vertex inside the volume will be limmited by fNPositionsamplingMax
//                            Example "/MG/generator/Sampling/NPositionsamplingMax 10000000"
// ---------------------------------------------------------------------------

#ifndef MGGeneratorPositionSampling_h
#define MGGeneratorPositionSampling_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Point3D.hh"
#include <vector>
#include "G4Navigator.hh"
#include "G4TouchableHistory.hh"


//This class is supposed to sample one random point uniformly from 
//a given volume or from a surface

class MGGeneratorUtil;
class G4Navigator;
class G4VPhysicalVolume;
class MGGeneratorPositionSamplingMessenger;
class MGGeneratorPositionSampling 
 
{
public:
  MGGeneratorPositionSampling(); 
  ~MGGeneratorPositionSampling();
  
  G4ThreeVector SampleUniformlyInVolume(G4String volName,G4int copynumber);
  G4ThreeVector SampleOnSurface(G4String volName,G4int copynumber);

  void SetGlobalTranslation(G4ThreeVector vect) {fGlobalTranslation = vect;};
  void SetGlobalRotation(G4RotationMatrix mat) {fGlobalRotation = mat;};
  G4ThreeVector GetGlobalTranslation() {return fGlobalTranslation;};
  G4RotationMatrix GetGlobalRotation() {return fGlobalRotation;};


  //messenger stuff
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
  void SetCylinderStartingAngle(G4double san){fCylinderStartingAngle = san;};
  void SetCylinderSpanningAngle(G4double span){fCylinderSpanningAngle = span;};
  void SetXLengthBox(G4double xlb){fXLengthBox = xlb;};
  void SetYLengthBox(G4double ylb){fYLengthBox = ylb;};
  void SetZLengthBox(G4double zlb){fZLengthBox = zlb;};

  //setCenter X Y Z of Volume
  void SetXCenter(G4double xcenter){fXCenter = xcenter; fUseCenterManually = true;};
  void SetYCenter(G4double ycenter){fYCenter = ycenter; fUseCenterManually = true;};
  void SetZCenter(G4double zcenter){fZCenter = zcenter; fUseCenterManually = true;};

  void SetUseCenterManually (G4bool aBool){fUseCenterManually = aBool;};
  G4bool GetUseCenterManually(){return fUseCenterManually;};

  void SetNPositionsamplingMax(const G4int ii) {fNPositionsamplingMax=ii;};

  G4int GetNPositionsamplingMax() const {return  fNPositionsamplingMax;};



private:
  G4String fVolumeName;
  MGGeneratorUtil* fGeneratorUtil;
  G4bool InitializeSamplingVolume();
  G4int fCopyNumber;

  G4Navigator* fMyNavigator;
  G4VPhysicalVolume* fMyVolume;

  G4RotationMatrix fGlobalRotation;
  G4ThreeVector fGlobalTranslation;

  std::vector<G4RotationMatrix> fRotations;
  std::vector<G4ThreeVector> fTranslations;

  G4double fRadius;
  G4ThreeVector fCentre; 

  G4GeometryType fSolid_type;
  G4GeometryType fSolidDaughter1_type;
  G4GeometryType fSolidDaughter2_type;

  G4double fSolid_par_arr[6];
  G4bool         fHasDaughters;

  G4VPhysicalVolume* FindTheDirectMother(G4VPhysicalVolume*);

  //to calculate real volume need overall number of tries
  G4int fNTries;
  G4int fNCalls;

//Messenger stuff

  G4String  fGeometricalSurfaceName;
  G4String fGeometricalVolumeName;

  //  MGGeneratorUtil* fGeneratorUtil;

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
  G4double fCylinderStartingAngle;
  G4double fCylinderSpanningAngle;

  //Box
  G4double fXLengthBox;
  G4double fYLengthBox;
  G4double fZLengthBox; 

  //Center Position
  G4double fXCenter;
  G4double fYCenter;
  G4double fZCenter; 
  G4bool   fUseCenterManually;


  //Size of the World Volume Box (used for the dummy vertex)
  G4double fWorldVolumeHalfLength;

  G4int    fNPositionsamplingMax;

  MGGeneratorPositionSamplingMessenger* fMessenger;

};
#endif
