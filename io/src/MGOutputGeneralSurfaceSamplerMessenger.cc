//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MGOutputGeneralSurfaceSamplerMessenger.cc,v 1.1 2006-01-24 02:55:47 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputGeneralSurfaceSamplerMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Johnson
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISIONS:
 *
 * 11/22/2011 Include a messenger that requires the surface to be ONLY between
 *   two of the selected volumes. Bjoern Lehnert
 * 01/14/2019 Include a messenger that allows exclusion of surfaces between
 *   volumes of the same material. Allows for a small gap between surfaces that
 *   can be set. The impetus for this is that in the MJ geometry, the N2 volume
 *   is modeled as two separate volumes, even though it is in fact a single
 *   volume. Micah Buuck
 */

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"

#include "io/MGOutputGeneralSurfaceSampler.hh"
#include "io/MGOutputGeneralSurfaceSamplerMessenger.hh"
#include "io/MGLogger.hh"

MGOutputGeneralSurfaceSamplerMessenger::MGOutputGeneralSurfaceSamplerMessenger(MGOutputGeneralSurfaceSampler *outputGSS) 
{
  fOutputGeneralSurfaceSampler = outputGSS;

  // /MG/io/gss
  fOutputGeneralSurfaceSamplerDirectory = new G4UIdirectory("/MG/io/gss/");

  // /MG/io/gss/addVolume
  fAddVolumeCmd = new G4UIcmdWithAString("/MG/io/gss/addVolume", this);
  fAddVolumeCmd->SetGuidance("Set the volumes of interest for surface sampling.");
  fAddVolumeCmd->SetParameterName("VolumeName", false);
  fAddVolumeCmd->AvailableForStates(G4State_Idle);

  // /MG/io/gss/removeVolume
  fRemoveVolumeCmd = new G4UIcmdWithAString("/MG/io/removeVolume", this);
  fRemoveVolumeCmd->SetGuidance("Remove volumes from surface sampling.");
  fRemoveVolumeCmd->SetParameterName("VolumeName", false);
  fRemoveVolumeCmd->AvailableForStates(G4State_Idle);

  // /MG/io/gss/setMaxIntersections
  fSetMaxIntersectionsCmd = new G4UIcmdWithAnInteger("/MG/io/gss/setMaxIntersections", this);
  fSetMaxIntersectionsCmd->SetGuidance("Give the maximum number of possible intersections through");
  fSetMaxIntersectionsCmd->SetGuidance("interesting volumes. If you don't know, make a guess. The");
  fSetMaxIntersectionsCmd->SetGuidance("generic surface sampler will let you know if it is not enough.");
  fSetMaxIntersectionsCmd->SetParameterName("MaxIntersections", false);

  // /MG/io/gss/SetOnlyBetweenSelectedVolumes
  fSetOnlyBetweenSelectedVolumesCmd = new G4UIcmdWithABool("/MG/io/gss/SetOnlyBetweenSelectedVolumes", this);
  fSetOnlyBetweenSelectedVolumesCmd->SetGuidance("If true, generates only positions on borders of added volumes");
  fSetOnlyBetweenSelectedVolumesCmd->SetGuidance("default is false");
  fSetOnlyBetweenSelectedVolumesCmd->SetParameterName("OnlyBetweenSelectedVolumesCmd", false);

  // /MG/io/gss/IgnoreIdenticalMaterials
  fIgnoreIdenticalMaterialsCmd = new G4UIcmdWithADouble("/MG/io/gss/IgnoreIdenticalMaterials", this);
  fIgnoreIdenticalMaterialsCmd->SetGuidance("If set, generates only positions on surfaces of added volumes that touch a volume of a different material. If the average of the dot product of the step vector with the pre-step volume and post-step volume normal vectors is smaller than the argument, the position is skipped.");
  fIgnoreIdenticalMaterialsCmd->SetGuidance("default is 0 mm");
  fIgnoreIdenticalMaterialsCmd->SetParameterName("IgnoreIdenticalMaterialsCmd", 0*CLHEP::mm);


  // /MG/io/gss/DefaultVolumeName
  fSetDefaultVolumeNameCmd = new G4UIcmdWithAString("/MG/io/gss/DefaultVolumeName", this);
  fSetDefaultVolumeNameCmd->SetGuidance("Set to name of default detector volume to ignore steps in for IgnoreIdenticalMaterials.");
  fSetDefaultVolumeNameCmd->SetGuidance("default is Detector");
  fSetDefaultVolumeNameCmd->SetParameterName("SetDefaultVolumeNameCmd", "Detector");
}

MGOutputGeneralSurfaceSamplerMessenger::~MGOutputGeneralSurfaceSamplerMessenger()
{
  delete fAddVolumeCmd;
  delete fRemoveVolumeCmd;
  delete fSetMaxIntersectionsCmd;
  delete fOutputGeneralSurfaceSamplerDirectory;
  delete fSetOnlyBetweenSelectedVolumesCmd;
  delete fIgnoreIdenticalMaterialsCmd;
  delete fSetDefaultVolumeNameCmd;

}

void MGOutputGeneralSurfaceSamplerMessenger::SetNewValue(G4UIcommand* cmd, G4String str)
{
  if( cmd == fAddVolumeCmd ) {
    fOutputGeneralSurfaceSampler->AddVolume(str);
  }
  if( cmd == fRemoveVolumeCmd ) {
    fOutputGeneralSurfaceSampler->RemoveVolume(str);
  }
  if( cmd == fSetMaxIntersectionsCmd ) {
    fOutputGeneralSurfaceSampler->SetMaxIntersections(fSetMaxIntersectionsCmd->GetNewIntValue(str));
  }
  if( cmd == fSetOnlyBetweenSelectedVolumesCmd ) {
    fOutputGeneralSurfaceSampler->SetOnlyBetweenSelectedVolumes(fSetOnlyBetweenSelectedVolumesCmd->GetNewBoolValue(str));
  }
  if( cmd == fIgnoreIdenticalMaterialsCmd ) {
    fOutputGeneralSurfaceSampler->IgnoreIdenticalMaterials(fIgnoreIdenticalMaterialsCmd->GetNewDoubleValue(str));
  }
  if( cmd == fSetDefaultVolumeNameCmd ) {
    fOutputGeneralSurfaceSampler->SetDefaultVolumeName(str);
  }

}
