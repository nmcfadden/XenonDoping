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
 * CLASS DECLARATION:  MGGeneratorPositionSamplingMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for commands /MG/generator/Sampling
 *
 * AUTHOR: D. Lenz
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 22, 2008
 *
 * 30 May 2008   Daniel       possibility to add an offset to the position of the center of the bounding volume 
 * 07 Jan 2009   Jens         The maximum number of tries to find a vertex inside the volume will be limmited by fNPositionsamplingMax
 *                            Example "/MG/generator/Sampling/NPositionsamplingMax 10000000"
 * 06 Oct 2011   Neslihan     command to add an offset to the position of the sampling surface
 * 
 */

#ifndef _MGGENERATORPOSITIONSAMPLINGMESSENGER_HH
#define _MGGENERATORPOSITIONSAMPLINGMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"


class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger; 
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class MGGeneratorPositionSampling;


class MGGeneratorPositionSamplingMessenger : public G4UImessenger
{
 public:
  MGGeneratorPositionSamplingMessenger(MGGeneratorPositionSampling *generator);
  ~MGGeneratorPositionSamplingMessenger();
  
  void SetNewValue(G4UIcommand *command, G4String newValues);
  

 private:
  MGGeneratorPositionSampling* fSampler;

  // /MG/generatorSampling
  G4UIdirectory *fSamplerDirectory;

  // /MG/generatorSampling/surface
  G4UIdirectory *fSurfaceDirectory;

  // /MG/generatorSampling/volume
  G4UIdirectory *fVolumeDirectory;
  
  // /MG/generatorSampling/volume/name
  G4UIcmdWithAString* fVolumeNameCmd;
  
  // /MG/generatorSampling/surface/name
  G4UIcmdWithAString* fSurfaceNameCmd;

  // /MG/generatorSampling/volume/innerSphereRadius
  G4UIcmdWithADoubleAndUnit* fInnerSphereRadiusVolCmd;

  // /MG/generatorSampling/volume/outerSphereRadius
  G4UIcmdWithADoubleAndUnit* fOuterSphereRadiusVolCmd;

  // /MG/generatorSampling/surface/sphereRadius
  G4UIcmdWithADoubleAndUnit* fSphereRadiusSurfCmd;

  // /MG/generatorSampling/surface/innerDiskRadius
  G4UIcmdWithADoubleAndUnit* fInnerDiskRadiusCmd;
  
  // /MG/generatorSampling/surface/outerDiskRadius
  G4UIcmdWithADoubleAndUnit* fOuterDiskRadiusCmd;

  // /MG/generatorSampling/surface/innerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fInnerCylinderRadiusSurfCmd;
  
  // /MG/generatorSampling/surface/outerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fOuterCylinderRadiusSurfCmd;
  
  // /MG/generatorSampling/surface/cylinderHeight
  G4UIcmdWithADoubleAndUnit* fCylinderHeightSurfCmd;

  // /MG/generatorSampling/surface/cylinderStartingAngle
  G4UIcmdWithADoubleAndUnit* fCylinderStartingAngleSurfCmd;

  // /MG/generatorSampling/surface/cylinderSpanningAngle
  G4UIcmdWithADoubleAndUnit* fCylinderSpanningAngleSurfCmd;


  // /MG/generatorSampling/volume/innerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fInnerCylinderRadiusVolCmd;
  
  // /MG/generatorSampling/volume/outerCylinderRadius
  G4UIcmdWithADoubleAndUnit* fOuterCylinderRadiusVolCmd;
  
  // /MG/generatorSampling/volume/cylinderHeight
  G4UIcmdWithADoubleAndUnit* fCylinderHeightVolCmd;

  // /MG/generatorSampling/volume/cylinderStartingAngle
  G4UIcmdWithADoubleAndUnit* fCylinderStartingAngleVolCmd;

  // /MG/generatorSampling/volume/cylinderSpanningAngle
  G4UIcmdWithADoubleAndUnit* fCylinderSpanningAngleVolCmd;

  // /MG/generator/Sampling/volume/BoxXLength
  G4UIcmdWithADoubleAndUnit*  fBoxXLengthVolCmd;

  // /MG/generator/Sampling/volume/BoxYLength
  G4UIcmdWithADoubleAndUnit*  fBoxYLengthVolCmd;

  // /MG/generator/Sampling/volume/BoxZLength
  G4UIcmdWithADoubleAndUnit*  fBoxZLengthVolCmd;

  // /MG/generator/Sampling/volume/CenterXPos
  G4UIcmdWithADoubleAndUnit*  fVolCenterXCmd;
  
  // /MG/generator/Sampling/volume/CenterYPos
  G4UIcmdWithADoubleAndUnit*  fVolCenterYCmd;

  // /MG/generator/Sampling/volume/CenterZPos
  G4UIcmdWithADoubleAndUnit*  fVolCenterZCmd;

  // /MG/generator/Sampling/surface/CenterXPos
  G4UIcmdWithADoubleAndUnit*  fSurfCenterXCmd;
  
  // /MG/generator/Sampling/surface/CenterYPos
  G4UIcmdWithADoubleAndUnit*  fSurfCenterYCmd;

  // /MG/generator/Sampling/surface/CenterZPos
  G4UIcmdWithADoubleAndUnit*  fSurfCenterZCmd;

  // /MG/generator/Sampling/NPositionsamplingMax
  G4UIcmdWithAnInteger*       fNPositionsamplingMaxCmd;

};
#endif
