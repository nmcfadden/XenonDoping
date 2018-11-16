
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
// History:
// --------
// 30 Nov 2007   L.Pandola    First implementation 
//                             
//---------------------------------------------------------------------------

//system headers
#include <iostream>

//Geant4 headers
#include "Randomize.hh"


//MaGe includes
#include "generators/MGGeneratorPositionSamplingGeometrical.hh"
#include "generators/MGGeneratorUtil.hh"
#include "generators/MGGeneratorPositionSamplingGeometricalMessenger.hh"
#include "io/MGLogger.hh"
#include "geometry/MGPhysVolNavigator.hh"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace std;
using namespace CLHEP;

MGGeneratorPositionSamplingGeometrical::MGGeneratorPositionSamplingGeometrical() 
{
  fGeometricalSurfaceName = "NULL";
  fGeometricalVolumeName = "NULL";
  fGeneratorUtil = new MGGeneratorUtil();
  fInnerSphereRadius = 0.0*cm;
  fOuterSphereRadius = 0.0*cm;
  fInnerDiskRadius = 0.0*cm;
  fOuterDiskRadius = 0.0*cm;
  fInnerCylinderRadius = 0.0*cm;
  fOuterCylinderRadius = 0.0*cm;
  fCylinderHeight = 0.0*cm;
  fCenterCoordinates.setX(0.*cm);
  fCenterCoordinates.setY(0.*cm);
  fCenterCoordinates.setZ(0.*cm);

  fMessenger = new MGGeneratorPositionSamplingGeometricalMessenger(this);
}

MGGeneratorPositionSamplingGeometrical::~MGGeneratorPositionSamplingGeometrical()
{
  delete fGeneratorUtil;
  delete fMessenger;
}

G4ThreeVector MGGeneratorPositionSamplingGeometrical::SampleInGeometricalVolume()
{
  G4ThreeVector position(0.*cm,0*cm,0*cm);
  if (fGeometricalVolumeName == "NULL")
    {
      MGLog(error) << "You have not set ANY geometrical volume for sampling " << endlog;
      MGLog(error) << "Please set volume shape and dimensions using the commands " << endlog;
      MGLog(error) << "/MG/generator/geomSampling/volume/name " << endlog;
      G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalVolume()", "err001", FatalException, "Exit program");
    }
  if (fGeometricalVolumeName == "Sphere")
    {
      if (fOuterSphereRadius < 1.0*micrometer)
	{
	  MGLog(error) << "Please set the sphere outer radius " << endlog;
          G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalVolume()", "err002", FatalException, "Exit program");
	}
      position = fGeneratorUtil->pick_point_in_shell(fInnerSphereRadius,fOuterSphereRadius);
      position += fCenterCoordinates;
    }
  else if (fGeometricalVolumeName == "Cylinder")
    {
      if (fOuterCylinderRadius < 1.0*micrometer || fCylinderHeight < 1.0*micrometer)
	{
	  MGLog(error) << "Please set the cylinder outer radius and/or height " << endlog;
          G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalVolume()", "err003", FatalException, "Exit program");
	}
      position = fGeneratorUtil->
	pick_point_in_annulus(fInnerCylinderRadius,fOuterCylinderRadius,fCylinderHeight);
      position += fCenterCoordinates;
    }
  else
    {
      MGLog(error) << "Geometrical volume " << fGeometricalVolumeName << " not supported" << endlog;
      G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalVolume()", "err004", FatalException, "Exit program");
    }
  return position;
}

G4ThreeVector MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()
{
  G4ThreeVector position(0.*cm,0*cm,0*cm);
  if (fGeometricalSurfaceName == "NULL")
    {
      MGLog(error) << "You have not set ANY geometrical surface for sampling " << endlog;
      MGLog(error) << "Please set volume shape and dimensions using the commands " << endlog;
      MGLog(error) << "/MG/generator/geomSampling/surface/name " << endlog;
      G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()", "err001", FatalException, "Exit program");
    }
  if (fGeometricalSurfaceName == "Disk")
    {
      if (fOuterDiskRadius < 1.0*micrometer)
	{
	  MGLog(error) << "Please set the disk outer radius " << endlog;
          G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()", "err002", FatalException, "Exit program");
	}
      position = fGeneratorUtil->pick_point_in_annulus(fInnerDiskRadius,fOuterDiskRadius,0.0*cm);
      position += fCenterCoordinates;
      
    }
  else if (fGeometricalSurfaceName == "Sphere")
    {
      if (fOuterSphereRadius < 1.0*micrometer)
	{
	  MGLog(error) << "Please set the sphere outer radius " << endlog;
          G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()", "err003", FatalException, "Exit program");
	}
      position = fGeneratorUtil->pick_point_on_sphere(fOuterSphereRadius);
      position += fCenterCoordinates;
    }
  else if (fGeometricalSurfaceName == "Cylinder")
    {
      if (fOuterCylinderRadius < 1.0*micrometer || fCylinderHeight < 1.0*micrometer)
	{
	  MGLog(error) << "Please set the cylinder outer radius and/or height " << endlog;
          G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()", "err004", FatalException, "Exit program");
	}
      position = fGeneratorUtil->
	pick_point_on_wholetube(fInnerCylinderRadius,fOuterCylinderRadius,fCylinderHeight);
      position += fCenterCoordinates;
    }
  else
    {
      MGLog(error) << "Geometrical surface " << fGeometricalSurfaceName << " not supported" << endlog;
      G4Exception("MGGeneratorPositionSamplingGeometrical::SampleInGeometricalSurface()", "err005", FatalException, "Exit program");
    }
  return position;
}
