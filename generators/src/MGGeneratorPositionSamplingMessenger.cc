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
//      
// CLASS IMPLEMENTATION:  MGGeneratorPositionSamplingMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: D. Lenz
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 22, 2008 
 *
 * 30 May 2008   Daniel       possibility to add an offset to the position of the center of the bounding volume 
 * 07 Jan 2009   Jens         The maximum number of tries to find a vertex inside the volume will be limmited by 
 *                            fNPositionsamplingMax
 *                            Example "/MG/generator/Sampling/NPositionsamplingMax 10000000"
 * 29 Sep 2010   Luciano      Bug fix. Dropped the >0 condition on the coordinate shift. Probably 
 *                            a wrong copy/paste.
 * 06 Oct 2011   Neslihan     command to add an offset to the position of the sampling surface
 *
 */

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "io/MGLogger.hh"

#include "generators/MGGeneratorPositionSamplingMessenger.hh" 
#include "generators/MGGeneratorPositionSampling.hh"

MGGeneratorPositionSamplingMessenger::MGGeneratorPositionSamplingMessenger
(MGGeneratorPositionSampling *theSampler):
  fSampler(theSampler) 
{
  // /MG/generator/Sampling directory definition
  fSamplerDirectory = new G4UIdirectory("/MG/generator/Sampling/");
  fSamplerDirectory->SetGuidance("Control commands for the geometry sampler");

  // /MG/generator/Sampling/surface
  fSurfaceDirectory = new G4UIdirectory("/MG/generator/Sampling/surface/");
  fSurfaceDirectory->SetGuidance("Control commands for the surface geometry sampler");

  // /MG/generator/Sampling/volume
  fVolumeDirectory =  new G4UIdirectory("/MG/generator/Sampling/volume/");
  fVolumeDirectory->SetGuidance("Control commands for the volume geometry sampler");

  // /MG/generator/Sampling/volume/name 
  fVolumeNameCmd = new G4UIcmdWithAString("/MG/generator/Sampling/volume/name", this);
  fVolumeNameCmd->SetGuidance("Selects the name of the geometrical volume for sampling");
  fVolumeNameCmd->SetGuidance("Options are: ");
  fVolumeNameCmd->SetGuidance("Sphere: sample from the volume of a spherical shell");
  fVolumeNameCmd->SetGuidance("Cylinder: sample from the volume of a cylindrical shell");
  fVolumeNameCmd->SetGuidance("Box: sample from the volume of a box");
  fVolumeNameCmd->SetCandidates("Sphere Cylinder Box ");

  // /MG/generator/Sampling/surface/name
  fSurfaceNameCmd = new G4UIcmdWithAString("/MG/generator/Sampling/surface/name", this);
  fSurfaceNameCmd->SetGuidance("Selects the name of the geometrical surface for sampling");
  fSurfaceNameCmd->SetGuidance("Options are: ");
  fSurfaceNameCmd->SetGuidance("Sphere: sample from the surface of a sphere");
  fSurfaceNameCmd->SetGuidance("Cylinder: sample from the surface of a cylindrical shell");
  fSurfaceNameCmd->SetGuidance("Disk: sample from the surface of a disk");
  fSurfaceNameCmd->SetCandidates("Sphere Cylinder Disk ");

  // /MG/generator/Sampling/volume/innerSphereRadius
  fInnerSphereRadiusVolCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/innerSphereRadius",this);
  fInnerSphereRadiusVolCmd->SetGuidance("Set the inner radius for sphere (default: 0 cm)");
  fInnerSphereRadiusVolCmd->SetDefaultUnit("cm");
  fInnerSphereRadiusVolCmd->SetUnitCategory("Length");
  fInnerSphereRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/outerSphereRadius
  fOuterSphereRadiusVolCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/outerSphereRadius",this);
  fOuterSphereRadiusVolCmd->SetGuidance("Set the outer radius for sphere ");
  fOuterSphereRadiusVolCmd->SetDefaultUnit("cm");
  fOuterSphereRadiusVolCmd->SetUnitCategory("Length");
  fOuterSphereRadiusVolCmd->SetUnitCandidates("microm mm cm m km");
  
  // /MG/generator/Sampling/surface/sphereRadius
  fSphereRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/sphereRadius",this);
  fSphereRadiusSurfCmd->SetGuidance("Set the outer radius for sphere ");
  fSphereRadiusSurfCmd->SetDefaultUnit("cm");
  fSphereRadiusSurfCmd->SetUnitCategory("Length");
  fSphereRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/innerDiskRadius
  fInnerDiskRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/innerDiskRadius",this);
  fInnerDiskRadiusCmd->SetGuidance("Set the inner radius for disk (default: 0 cm)");
  fInnerDiskRadiusCmd->SetDefaultUnit("cm");
  fInnerDiskRadiusCmd->SetUnitCategory("Length");
  fInnerDiskRadiusCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/outerDiskRadius
  fOuterDiskRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/outerDiskRadius",this);
  fOuterDiskRadiusCmd->SetGuidance("Set the outer radius for disk sampling");
  fOuterDiskRadiusCmd->SetDefaultUnit("cm");
  fOuterDiskRadiusCmd->SetUnitCategory("Length");
  fOuterDiskRadiusCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/innerCylinderRadius
  fInnerCylinderRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/innerCylinderRadius",this);
  fInnerCylinderRadiusSurfCmd->SetGuidance("Set the inner radius for cylinder surface sampling (default: 0 cm)");
  fInnerCylinderRadiusSurfCmd->SetDefaultUnit("cm");
  fInnerCylinderRadiusSurfCmd->SetUnitCategory("Length");
  fInnerCylinderRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/outerCylinderRadius
  fOuterCylinderRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/outerCylinderRadius",this);
  fOuterCylinderRadiusSurfCmd->SetGuidance("Set the outer radius for cylinder surface sampling");
  fOuterCylinderRadiusSurfCmd->SetDefaultUnit("cm");
  fOuterCylinderRadiusSurfCmd->SetUnitCategory("Length");
  fOuterCylinderRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/cylinderHeight
  fCylinderHeightSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/cylinderHeight",this);
  fCylinderHeightSurfCmd->SetGuidance("Set the height for cylinder surface sampling");
  fCylinderHeightSurfCmd->SetDefaultUnit("cm");
  fCylinderHeightSurfCmd->SetUnitCategory("Length");
  fCylinderHeightSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/cylinderStartingAngle
  fCylinderStartingAngleSurfCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/cylinderStartingAngle",this);
  fCylinderStartingAngleSurfCmd->SetGuidance("Set the starting angle for cylinder surface sampling (default: 0 deg)");
  fCylinderStartingAngleSurfCmd->SetDefaultUnit("deg");
  fCylinderStartingAngleSurfCmd->SetUnitCategory("Angle");
  fCylinderStartingAngleSurfCmd->SetUnitCandidates("deg");

  // /MG/generator/Sampling/surface/cylinderSpanningAngle
  fCylinderSpanningAngleSurfCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/cylinderSpanningAngle",this);
  fCylinderSpanningAngleSurfCmd->SetGuidance("Set the spanning angle for cylinder volume sampling (default: 360 deg )");
  fCylinderSpanningAngleSurfCmd->SetDefaultUnit("deg");
  fCylinderSpanningAngleSurfCmd->SetUnitCategory("Angle");
  fCylinderSpanningAngleSurfCmd->SetUnitCandidates("deg");


  // /MG/generator/Sampling/volume/innerCylinderRadius
  fInnerCylinderRadiusVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/innerCylinderRadius",this);
  fInnerCylinderRadiusVolCmd->SetGuidance("Set the inner radius for cylinder volume sampling (default: 0 cm)");
  fInnerCylinderRadiusVolCmd->SetDefaultUnit("cm");
  fInnerCylinderRadiusVolCmd->SetUnitCategory("Length");
  fInnerCylinderRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/outerCylinderRadius
  fOuterCylinderRadiusVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/outerCylinderRadius",this);
  fOuterCylinderRadiusVolCmd->SetGuidance("Set the outer radius for cylinder volume sampling");
  fOuterCylinderRadiusVolCmd->SetDefaultUnit("cm");
  fOuterCylinderRadiusVolCmd->SetUnitCategory("Length");
  fOuterCylinderRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/cylinderHeight
  fCylinderHeightVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/cylinderHeight",this);
  fCylinderHeightVolCmd->SetGuidance("Set the height for cylinder volume sampling");
  fCylinderHeightVolCmd->SetDefaultUnit("cm");
  fCylinderHeightVolCmd->SetUnitCategory("Length");
  fCylinderHeightVolCmd->SetUnitCandidates("microm mm cm m km");
  
  // /MG/generator/Sampling/volume/cylinderStartingAngle
  fCylinderStartingAngleVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/cylinderStartingAngle",this);
  fCylinderStartingAngleVolCmd->SetGuidance("Set the starting angle for cylinder volume sampling (default: 0 deg)");
  fCylinderStartingAngleVolCmd->SetDefaultUnit("deg");
  fCylinderStartingAngleVolCmd->SetUnitCategory("Angle");
  fCylinderStartingAngleVolCmd->SetUnitCandidates("deg");

  // /MG/generator/Sampling/volume/cylinderSpanningAngle
  fCylinderSpanningAngleVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/cylinderSpanningAngle",this);
  fCylinderSpanningAngleVolCmd->SetGuidance("Set the spanning angle for cylinder volume sampling (default: 360 deg )");
  fCylinderSpanningAngleVolCmd->SetDefaultUnit("deg");
  fCylinderSpanningAngleVolCmd->SetUnitCategory("Angle");
  fCylinderSpanningAngleVolCmd->SetUnitCandidates("deg");

  // /MG/generator/Sampling/volume/BoxXLength
  fBoxXLengthVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/boxXLength",this);
  fBoxXLengthVolCmd->SetGuidance("Set the X-dimension for box volume sampling");
  fBoxXLengthVolCmd->SetDefaultUnit("cm");
  fBoxXLengthVolCmd->SetUnitCategory("Length");
  fBoxXLengthVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/BoxYLength
  fBoxYLengthVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/boxYLength",this);
  fBoxYLengthVolCmd->SetGuidance("Set the Y-dimension for box volume sampling");
  fBoxYLengthVolCmd->SetDefaultUnit("cm");
  fBoxYLengthVolCmd->SetUnitCategory("Length");
  fBoxYLengthVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/BoxZLength
  fBoxZLengthVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/boxZLength",this);
  fBoxZLengthVolCmd->SetGuidance("Set the X-dimension for box volume sampling");
  fBoxZLengthVolCmd->SetDefaultUnit("cm");
  fBoxZLengthVolCmd->SetUnitCategory("Length");
  fBoxZLengthVolCmd->SetUnitCandidates("microm mm cm m km");
 
  // /MG/generator/Sampling/volume/offsetCenterXPos
  fVolCenterXCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/offsetCenterXPos",this);
  fVolCenterXCmd->SetGuidance("Set the X Position of the volume center");
  fVolCenterXCmd->SetDefaultUnit("cm");
  fVolCenterXCmd->SetUnitCategory("Length");
  fVolCenterXCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/offsetCenterYPos
  fVolCenterYCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/offsetCenterYPos",this);
  fVolCenterYCmd->SetGuidance("Set the Y Position of the volume center");
  fVolCenterYCmd->SetDefaultUnit("cm");
  fVolCenterYCmd->SetUnitCategory("Length");
  fVolCenterYCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/volume/offsetCenterZPos
  fVolCenterZCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/volume/offsetCenterZPos",this);
  fVolCenterZCmd->SetGuidance("Set the Z Position of the volume center");
  fVolCenterZCmd->SetDefaultUnit("cm");
  fVolCenterZCmd->SetUnitCategory("Length");
  fVolCenterZCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/offsetCenterXPos
  fSurfCenterXCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/offsetCenterXPos",this);
  fSurfCenterXCmd->SetGuidance("Set the X Position of the surface center");
  fSurfCenterXCmd->SetDefaultUnit("cm");
  fSurfCenterXCmd->SetUnitCategory("Length");
  fSurfCenterXCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/offsetCenterYPos
  fSurfCenterYCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/offsetCenterYPos",this);
  fSurfCenterYCmd->SetGuidance("Set the Y Position of the surface center");
  fSurfCenterYCmd->SetDefaultUnit("cm");
  fSurfCenterYCmd->SetUnitCategory("Length");
  fSurfCenterYCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/surface/offsetCenterZPos
  fSurfCenterZCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/Sampling/surface/offsetCenterZPos",this);
  fSurfCenterZCmd->SetGuidance("Set the Z Position of the surface center");
  fSurfCenterZCmd->SetDefaultUnit("cm");
  fSurfCenterZCmd->SetUnitCategory("Length");
  fSurfCenterZCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/Sampling/NPositionsamplingMax
  fNPositionsamplingMaxCmd =  new G4UIcmdWithAnInteger("/MG/generator/Sampling/NPositionsamplingMax",this);
  fNPositionsamplingMaxCmd->SetGuidance("Set the maximum number of tries used to find a vertex inside a volume/ on a surface.");

}

MGGeneratorPositionSamplingMessenger::~MGGeneratorPositionSamplingMessenger()
{
  delete fSamplerDirectory;
  delete fSurfaceDirectory;
  delete fVolumeDirectory;
  delete fVolumeNameCmd;
  delete fSurfaceNameCmd;
  delete fInnerSphereRadiusVolCmd;
  delete fOuterSphereRadiusVolCmd;
  delete fSphereRadiusSurfCmd;
  delete fInnerDiskRadiusCmd;
  delete fOuterDiskRadiusCmd;
  delete fInnerCylinderRadiusSurfCmd;
  delete fOuterCylinderRadiusSurfCmd;
  delete fCylinderHeightSurfCmd;
  delete fInnerCylinderRadiusVolCmd;
  delete fOuterCylinderRadiusVolCmd;
  delete fCylinderHeightVolCmd;
  delete fCylinderStartingAngleVolCmd;
  delete fCylinderSpanningAngleVolCmd;
  delete fBoxXLengthVolCmd;
  delete fBoxYLengthVolCmd;
  delete fBoxZLengthVolCmd;
  delete fNPositionsamplingMaxCmd;
}


void MGGeneratorPositionSamplingMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  //  /MG/generator/Sampling/volume/name
  if (command == fVolumeNameCmd) 
    {
      fSampler->SetGeometricalVolumeName(newValues);
      MGLog(trace) << "Volume for sampling set to " << newValues << endlog;
    }
  // /MG/generator/Sampling/surface/name
  else if (command == fSurfaceNameCmd)
    {
      fSampler->SetGeometricalSurfaceName(newValues);
      MGLog(trace) << "Surface for sampling set to " << newValues << endlog;
    }
  // /MG/generator/Sampling/volume/innerSphereRadius
  else if (command == fInnerSphereRadiusVolCmd)
    {
      if (fInnerSphereRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerSphereRadius(fInnerSphereRadiusVolCmd->GetNewDoubleValue(newValues));
    } 
  // /MG/generator/Sampling/volume/outerSphereRadius
  else if (command == fOuterSphereRadiusVolCmd)
    {
      if (fOuterSphereRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterSphereRadius(fOuterSphereRadiusVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/surface/sphereRadius
  else if (command == fSphereRadiusSurfCmd)
    {
      if (fSphereRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterSphereRadius(fSphereRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/surface/innerDiskRadius
  else if (command == fInnerDiskRadiusCmd)
    {
      if (fInnerDiskRadiusCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerDiskRadius(fInnerDiskRadiusCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/surface/outerDiskRadius
  else if (command == fOuterDiskRadiusCmd)
    {
      if (fOuterDiskRadiusCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterDiskRadius(fOuterDiskRadiusCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/surface/innerCylinderRadius
  else if (command == fInnerCylinderRadiusSurfCmd)
    {
      if (fInnerCylinderRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerCylinderRadius(fInnerCylinderRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/surface/outerCylinderRadius
  else if (command == fOuterCylinderRadiusSurfCmd)
    {
      if (fOuterCylinderRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterCylinderRadius(fOuterCylinderRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/surface/cylinderHeight
  else if (command == fCylinderHeightSurfCmd)
    {
      if (fCylinderHeightSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderHeight(fCylinderHeightSurfCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/surface/cylinderStartingAngle
  else if (command == fCylinderStartingAngleSurfCmd)
    {
      if (fCylinderStartingAngleSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderStartingAngle(fCylinderStartingAngleSurfCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/surface/cylinderSpanningAngle
  else if (command == fCylinderSpanningAngleSurfCmd)
    {
      if (fCylinderSpanningAngleSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderSpanningAngle(fCylinderSpanningAngleSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/innerCylinderRadius
  else if (command == fInnerCylinderRadiusVolCmd)
    {
      if (fInnerCylinderRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerCylinderRadius(fInnerCylinderRadiusVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/outerCylinderRadius
  else if (command == fOuterCylinderRadiusVolCmd)
    {
      if (fOuterCylinderRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterCylinderRadius(fOuterCylinderRadiusVolCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/volume/cylinderHeight
  else if (command == fCylinderHeightVolCmd)
    {
      if (fCylinderHeightVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderHeight(fCylinderHeightVolCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/volume/cylinderStartingAngle
  else if (command == fCylinderStartingAngleVolCmd)
    {
      if (fCylinderStartingAngleVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderStartingAngle(fCylinderStartingAngleVolCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/Sampling/volume/cylinderSpanningAngle
  else if (command == fCylinderSpanningAngleVolCmd)
    {
      if (fCylinderSpanningAngleVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderSpanningAngle(fCylinderSpanningAngleVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/BoxXLength
  else if (command == fBoxXLengthVolCmd)
    {
      if (fBoxXLengthVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetXLengthBox(fBoxXLengthVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/BoxYLength
  else if (command == fBoxYLengthVolCmd)
    {
      if (fBoxYLengthVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetYLengthBox(fBoxYLengthVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/BoxZLength
  else if (command == fBoxZLengthVolCmd)
    {
      if (fBoxZLengthVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetZLengthBox(fBoxZLengthVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/Sampling/volume/offsetCenterXPosition
  else if (command == fVolCenterXCmd)    
    fSampler->SetXCenter(fVolCenterXCmd->GetNewDoubleValue(newValues));    
  // /MG/generator/Sampling/volume/offsetCenterYPosition
  else if (command == fVolCenterYCmd)    
    fSampler->SetYCenter(fVolCenterYCmd->GetNewDoubleValue(newValues));    
  // /MG/generator/Sampling/volume/offsetCenterZPosition
  else if (command == fVolCenterZCmd)    
    fSampler->SetZCenter(fVolCenterZCmd->GetNewDoubleValue(newValues));  
  // /MG/generator/Sampling/surface/offsetCenterXPosition
  else if (command == fSurfCenterXCmd)    
    fSampler->SetXCenter(fSurfCenterXCmd->GetNewDoubleValue(newValues));    
  // /MG/generator/Sampling/surface/offsetCenterYPosition
  else if (command == fSurfCenterYCmd)    
    fSampler->SetYCenter(fSurfCenterYCmd->GetNewDoubleValue(newValues));    
  // /MG/generator/Sampling/surface/offsetCenterZPosition
  else if (command == fSurfCenterZCmd)    
    fSampler->SetZCenter(fSurfCenterZCmd->GetNewDoubleValue(newValues));
  // /MG/generator/Sampling/NPositionsamplingMax
  else if (command == fNPositionsamplingMaxCmd)
    {
	fSampler->SetNPositionsamplingMax( fNPositionsamplingMaxCmd->GetNewIntValue(newValues));
        MGLog(trace) << "fNPositionsamplingMax="<<fSampler->GetNPositionsamplingMax() << endlog;
    }
  else
    {
      MGLog(warning) << "Command not known" << endlog;
    }
}
