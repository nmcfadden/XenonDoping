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
// $Id: MGGeneratorGeneralSurfaceSamplerMessenger.cc,v 1.1 2006-01-24 02:55:47 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorGeneralSurfaceSamplerMessenger.cc
//
/**
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 18 oct 2010 added extentCenterVector to origin of bounding sphere so the
 *             sphere is centered on the part's extent, not the part's origin,
 *             A. Schubert
 */

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Point3D.hh"
#include "G4VisExtent.hh"

#include "generators/MGGeneratorGeneralSurfaceSampler.hh" 
#include "generators/MGGeneratorGeneralSurfaceSamplerMessenger.hh" 
#include "geometry/MGPhysVolNavigator.hh"


MGGeneratorGeneralSurfaceSamplerMessenger::MGGeneratorGeneralSurfaceSamplerMessenger(MGGeneratorGeneralSurfaceSampler* generator)
{
  fGeneratorGeneralSurfaceSampler = generator;

  // /MG/generator/gss directory definition
  // directory for the General Surface Sampler
  fGSSDirectory = new G4UIdirectory("/MG/generator/gss/");
  fGSSDirectory->SetGuidance("Control commands for generators:");

  fGSSBoundingRCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/gss/boundingR", this);
  fGSSBoundingRCmd->SetGuidance("Sets the bounding radius of a sphere surrounding");
  fGSSBoundingRCmd->SetGuidance("the volumes whose surfaces are to be sampled.");
  fGSSBoundingRCmd->SetParameterName("R", false);
  fGSSBoundingRCmd->SetUnitCategory("Length");
  
  // /MG/generator/gss/origin [x y z]
  // Sets the center of the bounding sphere, in case the volumes to be
  // studied are not centered at (0, 0, 0)
  // /MG/generator/position has the same effect, but the name here is
  // better.
  fGSSOriginCmd = new G4UIcmdWith3VectorAndUnit("/MG/generator/gss/origin", this);
  fGSSOriginCmd->SetGuidance("Set origin for the bounding sphere.");
  fGSSOriginCmd->SetParameterName("X", "Y", "Z", false);
  fGSSOriginCmd->SetUnitCategory("Length");

  // /MG/generator/gss/boundvol [volume]
  // performs both /MG/generator/gss/boundingR and
  // /MG/generator/gss/origin but uses a the boundingR and global
  // coordinates of an existing volume, which the user supplies by name.
  // If more than one volume of the same name exists in the
  // G4PhysicalVolumeStore, then the first one retrieved will be used.
  fGSSBoundVolCmd = new G4UIcmdWithAString("/MG/generator/gss/boundvol", this);
  fGSSBoundVolCmd->SetGuidance("Use the origin and bounding sphere of an existing volume.");
  fGSSBoundVolCmd->SetGuidance("Just supply the name of a volume. If more than one volume of the same name");
  fGSSBoundVolCmd->SetGuidance("in the G4PhysicalVolumeStore, the first one retrieved will be used.");
  fGSSBoundVolCmd->SetParameterName("volumeName", false);
  fGSSBoundVolCmd->AvailableForStates(G4State_Idle);
}

MGGeneratorGeneralSurfaceSamplerMessenger::~MGGeneratorGeneralSurfaceSamplerMessenger()
{
  delete fGSSBoundVolCmd;
  delete fGSSOriginCmd;
  delete fGSSBoundingRCmd;
  delete fGSSDirectory;
}

void MGGeneratorGeneralSurfaceSamplerMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fGSSBoundingRCmd) {
    fGeneratorGeneralSurfaceSampler->SetBoundingRadius(fGSSBoundingRCmd->GetNewDoubleValue(newValues));
  }
  else if (command == fGSSOriginCmd) {
    fGeneratorGeneralSurfaceSampler->SetOrigin(fGSSOriginCmd->GetNew3VectorValue(newValues));
  }
  else if (command == fGSSBoundVolCmd) {
    G4VPhysicalVolume* volume = MGPhysVolNavigator::GetVolumeNamed(newValues);
    if (volume == NULL) {
      MGLog(warning) << "Could not find volume " << newValues << "." << endlog;
      MGLog(warning) << "Available volumes are:\n" << MGPhysVolNavigator::GetListOfVolumes() << endlog;
    }
    else {
      double boundingR = MGPhysVolNavigator::GetBoundingRadius(volume);
      fGeneratorGeneralSurfaceSampler->SetBoundingRadius(boundingR);

      G4ThreeVector globalTranslation = MGPhysVolNavigator::GetGlobalTranslation(volume);
      G4Point3D extentCenter = volume->GetLogicalVolume()->GetSolid()->GetExtent().GetExtentCenter();
      G4ThreeVector extentCenterVector( extentCenter.x(), extentCenter.y(), extentCenter.z() );
      globalTranslation += extentCenterVector; // added 18 oct 2010, A. Schubert
      fGeneratorGeneralSurfaceSampler->SetOrigin(globalTranslation);
    }
  }
}
