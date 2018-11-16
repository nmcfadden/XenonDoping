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
 *      
 * CLASS DECLARATION:  MGGeneratorPositionSamplingGeometricalMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for commands /MG/generator/geomSampling
 *
 * AUTHOR: L. Pandola
 * CONTACT: 
 * FIRST SUBMISSION: Mon Dec 3, 2007
 * 
 */

#ifndef _MGGENERATORPOSITIONSAMPLINGGEOMETRICALMESSENGER_HH
#define _MGGENERATORPOSITIONSAMPLINGGEOMETRICALMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"


class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger; 
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class MGGeneratorPositionSamplingGeometrical;


class MGGeneratorPositionSamplingGeometricalMessenger : public G4UImessenger
{
 public:
  MGGeneratorPositionSamplingGeometricalMessenger(MGGeneratorPositionSamplingGeometrical *generator);
  ~MGGeneratorPositionSamplingGeometricalMessenger();
  
  void SetNewValue(G4UIcommand *command, G4String newValues);
  

 private:
  MGGeneratorPositionSamplingGeometrical* fSampler;

  // /MG/generator/geomSampling
  G4UIdirectory *fSamplerDirectory;

  // /MG/generator/geomSampling/surface
  G4UIdirectory *fSurfaceDirectory;

  // /MG/generator/geomSampling/volume
  G4UIdirectory *fVolumeDirectory;
  
  // /MG/generator/geomSampling/volume/name
  G4UIcmdWithAString* fVolumeNameCmd;
  
  // /MG/generator/geomSampling/surface/name
  G4UIcmdWithAString* fSurfaceNameCmd;

  // /MG/generator/geomSampling/volume/center
  G4UIcmdWith3VectorAndUnit* fPositionVolumeCmd;

  // /MG/generator/geomSampling/surface/center
  G4UIcmdWith3VectorAndUnit* fPositionSurfaceCmd;

  // /MG/generator/geomSampling/volume/innerSphereRadius
  G4UIcmdWithADoubleAndUnit* fInnerSphereRadiusVolCmd;

  // /MG/generator/geomSampling/volume/outerSphereRadius
  G4UIcmdWithADoubleAndUnit* fOuterSphereRadiusVolCmd;

  // /MG/generator/geomSampling/surface/sphereRadius
  G4UIcmdWithADoubleAndUnit* fSphereRadiusSurfCmd;

  // /MG/generator/geomSampling/surface/innerDiskRadius
  G4UIcmdWithADoubleAndUnit* fInnerDiskRadiusCmd;
  
  // /MG/generator/geomSampling/surface/outerDiskRadius
  G4UIcmdWithADoubleAndUnit* fOuterDiskRadiusCmd;

  // /MG/generator/geomSampling/surface/innerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fInnerCylinderRadiusSurfCmd;
  
  // /MG/generator/geomSampling/surface/outerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fOuterCylinderRadiusSurfCmd;
  
  // /MG/generator/geomSampling/surface/cylinderHeight
  G4UIcmdWithADoubleAndUnit* fCylinderHeightSurfCmd;

  // /MG/generator/geomSampling/volume/innerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fInnerCylinderRadiusVolCmd;
  
  // /MG/generator/geomSampling/volume/outerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fOuterCylinderRadiusVolCmd;
  
  // /MG/generator/geomSampling/volume/cylinderHeight
  G4UIcmdWithADoubleAndUnit* fCylinderHeightVolCmd;
  
};
#endif
