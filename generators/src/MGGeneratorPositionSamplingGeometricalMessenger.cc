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
// CLASS IMPLEMENTATION:  MGGeneratorPositionSamplingGeometricalMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: L. Pandola
 * CONTACT: 
 * FIRST SUBMISSION: Mon Dec 3, 2007 
 * 
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

#include "generators/MGGeneratorPositionSamplingGeometricalMessenger.hh" 
#include "generators/MGGeneratorPositionSamplingGeometrical.hh"

MGGeneratorPositionSamplingGeometricalMessenger::MGGeneratorPositionSamplingGeometricalMessenger
(MGGeneratorPositionSamplingGeometrical *theSampler):
  fSampler(theSampler) 
{
  // /MG/generator/geomSampling directory definition
  fSamplerDirectory = new G4UIdirectory("/MG/generator/geomSampling/");
  fSamplerDirectory->SetGuidance("Control commands for the geometry sampler");

  // /MG/generator/geomSampling/surface
  fSurfaceDirectory = new G4UIdirectory("/MG/generator/geomSampling/surface/");
  fSurfaceDirectory->SetGuidance("Control commands for the surface geometry sampler");

  // /MG/generator/geomSampling/volume
  fVolumeDirectory =  new G4UIdirectory("/MG/generator/geomSampling/volume/");
  fVolumeDirectory->SetGuidance("Control commands for the volume geometry sampler");

  // /MG/generator/geomSampling/volume/name
  fVolumeNameCmd = new G4UIcmdWithAString("/MG/generator/geomSampling/volume/name", this);
  fVolumeNameCmd->SetGuidance("Selects the name of the geometrical volume for sampling");
  fVolumeNameCmd->SetGuidance("Options are: ");
  fVolumeNameCmd->SetGuidance("Sphere: sample from the volume of a spherical shell");
  fVolumeNameCmd->SetGuidance("Cylinder: sample from the volume of a cylindrical shell");
  fVolumeNameCmd->SetCandidates("Sphere Cylinder ");

  // /MG/generator/geomSampling/surface/name
  fSurfaceNameCmd = new G4UIcmdWithAString("/MG/generator/geomSampling/surface/name", this);
  fSurfaceNameCmd->SetGuidance("Selects the name of the geometrical surface for sampling");
  fSurfaceNameCmd->SetGuidance("Options are: ");
  fSurfaceNameCmd->SetGuidance("Sphere: sample from the surface of a sphere");
  fSurfaceNameCmd->SetGuidance("Cylinder: sample from the surface of a cylindrical shell");
  fSurfaceNameCmd->SetGuidance("Disk: sample from the surface of a disk");
  fSurfaceNameCmd->SetCandidates("Sphere Cylinder Disk ");

  // /MG/generator/geomSampling/volume/center
  fPositionVolumeCmd = new G4UIcmdWith3VectorAndUnit("/MG/generator/geomSampling/volume/center",this);
  fPositionVolumeCmd->SetGuidance("Set the coordinates of the geometrical volume center");
  fPositionVolumeCmd->SetParameterName("X","Y","Z",true,true);
  fPositionVolumeCmd->SetDefaultUnit("cm");
  fPositionVolumeCmd->SetUnitCategory("Length");
  fPositionVolumeCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/center
  fPositionSurfaceCmd = new G4UIcmdWith3VectorAndUnit("/MG/generator/geomSampling/surface/center",this);
  fPositionSurfaceCmd->SetGuidance("Set the coordinates of the geometrical surface center");
  fPositionSurfaceCmd->SetParameterName("X","Y","Z",true,true);
  fPositionSurfaceCmd->SetDefaultUnit("cm");
  fPositionSurfaceCmd->SetUnitCategory("Length");
  fPositionSurfaceCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/volume/innerSphereRadius
  fInnerSphereRadiusVolCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/volume/innerSphereRadius",this);
  fInnerSphereRadiusVolCmd->SetGuidance("Set the inner radius for sphere (default: 0 cm)");
  fInnerSphereRadiusVolCmd->SetDefaultUnit("cm");
  fInnerSphereRadiusVolCmd->SetUnitCategory("Length");
  fInnerSphereRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/volume/outerSphereRadius
  fOuterSphereRadiusVolCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/volume/outerSphereRadius",this);
  fOuterSphereRadiusVolCmd->SetGuidance("Set the outer radius for sphere ");
  fOuterSphereRadiusVolCmd->SetDefaultUnit("cm");
  fOuterSphereRadiusVolCmd->SetUnitCategory("Length");
  fOuterSphereRadiusVolCmd->SetUnitCandidates("microm mm cm m km");
  
  // /MG/generator/geomSampling/surface/sphereRadius
  fSphereRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/sphereRadius",this);
  fSphereRadiusSurfCmd->SetGuidance("Set the outer radius for sphere ");
  fSphereRadiusSurfCmd->SetDefaultUnit("cm");
  fSphereRadiusSurfCmd->SetUnitCategory("Length");
  fSphereRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/innerDiskRadius
  fInnerDiskRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/innerDiskRadius",this);
  fInnerDiskRadiusCmd->SetGuidance("Set the inner radius for disk (default: 0 cm)");
  fInnerDiskRadiusCmd->SetDefaultUnit("cm");
  fInnerDiskRadiusCmd->SetUnitCategory("Length");
  fInnerDiskRadiusCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/outerDiskRadius
  fOuterDiskRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/outerDiskRadius",this);
  fOuterDiskRadiusCmd->SetGuidance("Set the outer radius for disk sampling");
  fOuterDiskRadiusCmd->SetDefaultUnit("cm");
  fOuterDiskRadiusCmd->SetUnitCategory("Length");
  fOuterDiskRadiusCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/innerCylinderRadius
  fInnerCylinderRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/innerCylinderRadius",this);
  fInnerCylinderRadiusSurfCmd->SetGuidance("Set the inner radius for cylinder surface sampling (default: 0 cm)");
  fInnerCylinderRadiusSurfCmd->SetDefaultUnit("cm");
  fInnerCylinderRadiusSurfCmd->SetUnitCategory("Length");
  fInnerCylinderRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/outerCylinderRadius
  fOuterCylinderRadiusSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/outerCylinderRadius",this);
  fOuterCylinderRadiusSurfCmd->SetGuidance("Set the outer radius for cylinder surface sampling");
  fOuterCylinderRadiusSurfCmd->SetDefaultUnit("cm");
  fOuterCylinderRadiusSurfCmd->SetUnitCategory("Length");
  fOuterCylinderRadiusSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/surface/cylinderHeight
  fCylinderHeightSurfCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/surface/cylinderHeight",this);
  fCylinderHeightSurfCmd->SetGuidance("Set the height for cylinder surface sampling");
  fCylinderHeightSurfCmd->SetDefaultUnit("cm");
  fCylinderHeightSurfCmd->SetUnitCategory("Length");
  fCylinderHeightSurfCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/volume/innerCylinderRadius
  fInnerCylinderRadiusVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/volume/innerCylinderRadius",this);
  fInnerCylinderRadiusVolCmd->SetGuidance("Set the inner radius for cylinder volume sampling (default: 0 cm)");
  fInnerCylinderRadiusVolCmd->SetDefaultUnit("cm");
  fInnerCylinderRadiusVolCmd->SetUnitCategory("Length");
  fInnerCylinderRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/volume/outerCylinderRadius
  fOuterCylinderRadiusVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/volume/outerCylinderRadius",this);
  fOuterCylinderRadiusVolCmd->SetGuidance("Set the outer radius for cylinder volume sampling");
  fOuterCylinderRadiusVolCmd->SetDefaultUnit("cm");
  fOuterCylinderRadiusVolCmd->SetUnitCategory("Length");
  fOuterCylinderRadiusVolCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/geomSampling/volume/cylinderHeight
  fCylinderHeightVolCmd =  new G4UIcmdWithADoubleAndUnit("/MG/generator/geomSampling/volume/cylinderHeight",this);
  fCylinderHeightVolCmd->SetGuidance("Set the height for cylinder volume sampling");
  fCylinderHeightVolCmd->SetDefaultUnit("cm");
  fCylinderHeightVolCmd->SetUnitCategory("Length");
  fCylinderHeightVolCmd->SetUnitCandidates("microm mm cm m km");
  
}

MGGeneratorPositionSamplingGeometricalMessenger::~MGGeneratorPositionSamplingGeometricalMessenger()
{
  delete fSamplerDirectory;
  delete fSurfaceDirectory;
  delete fVolumeDirectory;
  delete fVolumeNameCmd;
  delete fSurfaceNameCmd;
  delete fPositionVolumeCmd;
  delete fPositionSurfaceCmd;
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
}


void MGGeneratorPositionSamplingGeometricalMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  //  /MG/generator/geomSampling/volume/name
  if (command == fVolumeNameCmd) 
    {
      fSampler->SetGeometricalVolumeName(newValues);
      MGLog(trace) << "Volume for geometrical sampling set to " << newValues << endlog;
    }
  // /MG/generator/geomSampling/surface/name
  else if (command == fSurfaceNameCmd)
    {
      fSampler->SetGeometricalSurfaceName(newValues);
      MGLog(trace) << "Surface for geometrical sampling set to " << newValues << endlog;
    }
  // /MG/generator/geomSampling/volume/center
  else if (command == fPositionVolumeCmd)
    fSampler->SetCenterCoordinates(fPositionVolumeCmd->GetNew3VectorValue(newValues));
  // /MG/generator/geomSampling/surface/center
  else if (command == fPositionSurfaceCmd)
    fSampler->SetCenterCoordinates(fPositionSurfaceCmd->GetNew3VectorValue(newValues));
  // /MG/generator/geomSampling/volume/innerSphereRadius
  else if (command == fInnerSphereRadiusVolCmd)
    {
      if (fInnerSphereRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerSphereRadius(fInnerSphereRadiusVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/volume/outerSphereRadius
  else if (command == fOuterSphereRadiusVolCmd)
    {
      if (fOuterSphereRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterSphereRadius(fOuterSphereRadiusVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/surface/sphereRadius
  else if (command == fSphereRadiusSurfCmd)
    {
      if (fSphereRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterSphereRadius(fSphereRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/surface/innerDiskRadius
  else if (command == fInnerDiskRadiusCmd)
    {
      if (fInnerDiskRadiusCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerDiskRadius(fInnerDiskRadiusCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/surface/outerDiskRadius
  else if (command == fOuterDiskRadiusCmd)
    {
      if (fOuterDiskRadiusCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterDiskRadius(fOuterDiskRadiusCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/surface/innerCylinderRadius
  else if (command == fInnerCylinderRadiusSurfCmd)
    {
      if (fInnerCylinderRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerCylinderRadius(fInnerCylinderRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/geomSampling/surface/outerCylinderRadius
  else if (command == fOuterCylinderRadiusSurfCmd)
    {
      if (fOuterCylinderRadiusSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterCylinderRadius(fOuterCylinderRadiusSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/surface/cylinderHeight
  else if (command == fCylinderHeightSurfCmd)
    {
      if (fCylinderHeightSurfCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderHeight(fCylinderHeightSurfCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/volume/innerCylinderRadius
  else if (command == fInnerCylinderRadiusVolCmd)
    {
      if (fInnerCylinderRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetInnerCylinderRadius(fInnerCylinderRadiusVolCmd->GetNewDoubleValue(newValues));
    }
  // /MG/generator/geomSampling/volume/outerCylinderRadius
  else if (command == fOuterCylinderRadiusVolCmd)
    {
      if (fOuterCylinderRadiusVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetOuterCylinderRadius(fOuterCylinderRadiusVolCmd->GetNewDoubleValue(newValues));
    }
   // /MG/generator/geomSampling/volume/cylinderHeight
  else if (command == fCylinderHeightVolCmd)
    {
      if (fCylinderHeightVolCmd->GetNewDoubleValue(newValues) > 0)
	fSampler->SetCylinderHeight(fCylinderHeightVolCmd->GetNewDoubleValue(newValues));
    }
  else
    {
      MGLog(warning) << "Command not known" << endlog;
    }
}

