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
/**
   12-11-2006 Luciano, First implementation
   04-02-2007 Luciano, Corrected in order to allow more logical volumes to be 
                       registered as a single SD.
 */
// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "geometry/MGGeometryDetector.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "geometry/MGGeometryFromFile.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

MGGeometryFromFile::MGGeometryFromFile(G4String fn) : 
  MGGeometryDetector("FileGeometry"),fFileName(fn),CrystalSD(0)
{
  fVolumes = new std::vector<MGGeometryFileGeometryObject*>;
  fLogVolumes = new std::vector<G4LogicalVolume*>;
  fSolids = new std::vector<G4VSolid*>; 
}

// --------------------------------------------------------------------------- 

MGGeometryFromFile::~MGGeometryFromFile()
{
  if (!fVolumes)
    {
      for (size_t kk=0;kk<fVolumes->size();kk++)
	{
	  delete (*fVolumes)[kk];;
	}
      delete fVolumes;
    }
  if (!fLogVolumes)
    {
      for (size_t kk=0;kk<fLogVolumes->size();kk++)
	{
	  delete (*fLogVolumes)[kk];
	}
      delete fLogVolumes;
    }
  if (!fSolids)
    {
      for (size_t kk=0;kk<fSolids->size();kk++)
	{
	  delete (*fSolids)[kk];
	}
      delete fSolids;
    }
}

// --------------------------------------------------------------------------- 

void MGGeometryFromFile::ConstructDetector()
{
  //First one has to set a "world" volume. It is a cube of 5m size
  G4double boxSize = 5.0*m;
  G4Box* theFullBox = new G4Box("WorldBox",boxSize/2.,boxSize/2.,boxSize/2.);
  G4LogicalVolume* theFullLogical = new G4LogicalVolume(theFullBox,
							G4Material::GetMaterial("Air"),
						       "WorldLog");
  G4Colour  red     (1.0, 0.0, 0.0) ;
  G4Colour  blue    (0.0, 0.0, 1.0) ;

  fSolids->clear();
  fLogVolumes->clear();
  fFileName = MGManager::GetMGManager()->
    GetManagerDetectorConstruction()->GetFileNameForGeometry();
  MGLog(debugging) << "Open the file: " << fFileName << endlog;
  ReadFile(fFileName);
  for (G4int k=0; k < (G4int) fVolumes->size();k++)
    {
      G4int shapeCode = (*fVolumes)[k]->GetShapeCode();
      G4int detectorCode = (*fVolumes)[k]->GetDetectorCode();
      G4ThreeVector dimensions = (*fVolumes)[k]->GetDimensions();
      G4String solidName = (*fVolumes)[k]->GetVolumeName()+"Sol";
      G4VSolid* theSolid = NULL;
      if (shapeCode == 1) 
	{
	  MGLog(debugging) << "Defining a box: " << dimensions/cm << " cm" << endlog;
	  theSolid = new G4Box(solidName,dimensions.x()/2.,dimensions.y()/2.,
			       dimensions.z()/2.);
	  fSolids->push_back(theSolid);
	}
      else if (shapeCode == 2)
	{
	  MGLog(debugging) << "Defining a cylinder. Radius " << dimensions.x()/cm << 
	    " cm; Height: " << dimensions.y()/cm << " cm" << endlog;
	  theSolid = new G4Tubs(solidName,0.,dimensions.x(),dimensions.y()/2.,
				0.,twopi);
	  fSolids->push_back(theSolid);
	}
      else if (shapeCode == 3)
	{
	  MGLog(debugging) << "Defining a sphere. Radius: " << dimensions.x()/cm 
			   << " cm" << endlog;
	  theSolid = new G4Sphere(solidName,0.,dimensions.x(),0,twopi,0,pi);
	  fSolids->push_back(theSolid);
	}
      else
	{
	  MGLog(warning) << "Shape number " << shapeCode << 
	    " is not defined. Line ignored" << endlog;
	}
      G4LogicalVolume* theLogicalVolume = NULL;
      G4String logicalName = (*fVolumes)[k]->GetVolumeName()+"Log";
      if (G4Material::GetMaterial((*fVolumes)[k]->GetMaterialName()) && theSolid)
	{
	  theLogicalVolume = new G4LogicalVolume(theSolid,
						 G4Material::GetMaterial((*fVolumes)[k]->GetMaterialName()),
						 logicalName);
	  fLogVolumes->push_back(theLogicalVolume);
	  if (detectorCode == 1) //sensitive detector
	    {
	      if (!CrystalSD)
		{
		  CrystalSD      = new GEGeometrySD("/mydet/gerda/gecrystal");
		  G4SDManager::GetSDMpointer()->AddNewDetector(CrystalSD);
		}
	      theLogicalVolume->SetSensitiveDetector(CrystalSD);
	      theLogicalVolume->SetVisAttributes(new G4VisAttributes(red));
	    }
	  else
	    {
	      theLogicalVolume->SetVisAttributes(new G4VisAttributes(blue));
	    }
	}
      else
	{
	  MGLog(warning) << "Material " << (*fVolumes)[k]->GetMaterialName() << 
	    " not found. Line ignored" << endlog;
	}
      if (theLogicalVolume)
	{
	  G4ThreeVector position = (*fVolumes)[k]->GetCenterPosition();
	  G4ThreeVector rotation = (*fVolumes)[k]->GetRotationAngles(); 
	  G4RotationMatrix* rotMatrix = new G4RotationMatrix(rotation.x(),
							     rotation.y(),
							     rotation.z());
	  MGLog(debugging) << "Rotation matrix for volume " << (*fVolumes)[k]->GetVolumeName() << 
	    ":" << endlog;
	  MGLog(debugging) << *rotMatrix << endlog;
	  G4int daughterCode = (*fVolumes)[k]->GetDaughterCode();
	  if (daughterCode == 0) //volume is not daughter of an other, just of the world
	    {
	      new G4PVPlacement(rotMatrix,
				position,
				theLogicalVolume,
				(*fVolumes)[k]->GetVolumeName(),
				theFullLogical,
				false,0);
	    }
	  else
	    {
	      G4int theIndex = daughterCode-1;
	      if (theIndex >= k)
		{
		  MGLog(error) << "Error in the geometry: " << endlog;
		  MGLog(error) << "Volume #" << (k+1) << " cannot be daughter of volume " << 
		    daughterCode << endlog;
		  MGLog(error) << "Please re-order the volumes" << endlog;
                  G4Exception("MGGeometryFromFile::ConstructDetector()", "err001", FatalException, "Abort");
		}
	      else
		{
		  G4LogicalVolume* theAncestor = (*fLogVolumes)[theIndex];
		  new G4PVPlacement(rotMatrix,
				    position,
				    theLogicalVolume,
				    (*fVolumes)[k]->GetVolumeName(),
				    theAncestor,
				    false,0);
		  MGLog(trace) << "Set volume " << (*fVolumes)[k]->GetVolumeName() << 
		    " as a daughter of volume " <<  (*fVolumes)[theIndex]->GetVolumeName() << 
		    endlog;
			       
		}
	    }
	}
    }

  SetDetectorLogical(theFullLogical);
  theFullLogical->SetVisAttributes(G4VisAttributes::Invisible);
  SetDetectorName("FileGeometry"); 
}


void MGGeometryFromFile::ReadFile(G4String filename)
{
  /* The format of the file is the following:
     Each volume --> 4 lines
     1) first line:
     Ordering number
     NameOfDetector,shape,sensitive/not,daugther,MaterialName,Position,Dimensions,RotationAngles
     shape: 1=box,2=cylinder,3=sphere. 
     sensitive/not: if 1, the detector is registered as a sensitive volume
     daugther: number of the ancestor volume. If 0 the ancestor is the world.
     2) second line: position (3-vector), coordinates in cm 
     Position has to be expressed with respect to the ancestor volume
     3) third line: dimensions (3-vector), in cm
     Dimensions: for boxes --> x,y,z
                 for cylinders --> radius,z,-
		 for spheres --> radius,-,-
     4) fourth line:
     RotationAngles: Euler angles expressed in degrees
  */

  std::ifstream inputFile(filename);
  G4int orderNumber,shapeCode,detectorCode,daughterCode;
  G4String materialName,volumeName;
  G4double centerPositionx,dimensionsx,rotationAnglesx;
  G4double centerPositiony,dimensionsy,rotationAnglesy;
  G4double centerPositionz,dimensionsz,rotationAnglesz;
  
  if (!(inputFile.is_open())) {//not open correctly
    MGLog(error) << "Invalid file name " << filename << endlog;
    return;
  }
  G4String oldVolumeName = " ";
  for ( ;!inputFile.eof(); )
    {
      //Here we read 4 lines
      inputFile >> orderNumber >> volumeName >>  shapeCode >> detectorCode >> 
	daughterCode >> materialName;
      inputFile >> centerPositionx >> centerPositiony >> centerPositionz; 
      inputFile >> dimensionsx >> dimensionsy >> dimensionsz;
      inputFile >> rotationAnglesx >> rotationAnglesy >> rotationAnglesz;
      if (oldVolumeName != volumeName)
	{
	  MGGeometryFileGeometryObject* aVolume = new MGGeometryFileGeometryObject();
	  aVolume->SetShapeCode(shapeCode);
	  aVolume->SetDetectorCode(detectorCode);
	  aVolume->SetDaughterCode(daughterCode);
	  aVolume->SetMaterialName(materialName);
	  aVolume->SetCenterPosition(G4ThreeVector(centerPositionx*cm,
						   centerPositiony*cm,
						   centerPositionz*cm));
	  aVolume->SetDimensions(G4ThreeVector(dimensionsx*cm,
					       dimensionsy*cm,
					       dimensionsz*cm));
	  aVolume->SetRotationAngles(G4ThreeVector(rotationAnglesx*deg,
						   rotationAnglesy*deg,
						   rotationAnglesz*deg));
	  aVolume->SetVolumeName(volumeName);
	  fVolumes->push_back(aVolume);
	  MGLog(debugging) << "Read volume: " << volumeName << endlog;
	  oldVolumeName = volumeName;
	}
      if (orderNumber != (G4int) fVolumes->size())
	{
	  MGLog(warning) << "Is the ordering of the volumes correct?" << endlog;
	  MGLog(warning) << "Volume " << volumeName << "is the " << fVolumes->size() << "-th one" << endlog;
	  MGLog(warning) << "but is identified as number " << orderNumber << endlog;
	}
    }
  MGLog(trace) << "Read " << fVolumes->size() << " volumes from file" << endlog;
  inputFile.close();
}
