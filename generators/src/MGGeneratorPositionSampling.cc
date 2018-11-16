
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
// This class is supposed to sample the position of the primary
// particle randomly in a volume or on a surface (surface still not working). 
//
// History:
// --------
// 28 Oct 2004   L.Pandola    First implementation (interfaces ok)
// 10 Nov 2004   Xiang        Implemented SampleUniformlyInVolume for Tubs 
// 11 Nov 2004   Luciano      Added check if the sampled point is inside 
//                            (now it should work also if 
//                            the Tub has an asymmetric hole, as Ge detectors)
// 19 Nov 2004   Luciano      A lot of changes: implemented a modified 
//                            version of Dave Jordan algorithm
// 06 Dec 2004   Luciano      Re-naming
// 11 Mar 2005   R. Henning   Modifed G4Tubs and G4Box cases so they do not
//                            return points in daughter volumes of selected
//                            volume. This is consistent with the "bounding 
//                            sphere case.
// 15 Apr 2005   X. Liu       In function InitializeSamplingVolume()
//                            if fSolid_type=G4SubtractionSolid and fSolidDaughter1_type=G4Tubs
//                            generate random points on the surface of Tubs
//                            this function is for Gerda crystaldeadlayer which is a SubtractionSolid
//                            in function SampleOnSurface()
//                            the corresponding part is added
//                            this is not ready for SampleUniformlyInVolume() yet
// 27 Apr 2005  L. Pandola    Bug fixed: for G4SubstractionSolid, fHasDaughter must be true
// 27 Sep 2006  L. Pandola    Bug fixed: the calculation of the translation of the target 
//                            volume was wrong if one of the ancestor volumes was rotated
// 21 Feb 2007  L. Pandola    Corrected SurfaceSampling method. It works only for VERY simple shapes.
//                            For arbitrary surfaces, the GeneralSurfaceSampler has to be used.
// 10 May 2007  Kevin/Daniel  Corrected method SampleUniformlyInVolume for BoundingSphere. fCenter needed rotation
// 22 May 2007  Daniel        Added BoundingBox as sample volume, chosen if volume is smaller than BoundingSphere.
// 02 Nov 2007  Daniel        Added vertex_dummy which will be returned in case that the sampled point is not 
//                            in the right volume
// 19 Dec 2007  Jens          - due to a problem with the BoundingBox the BoundingSphere is set as default
//                              and the BoundingBox is not used by default
//                            - set environment variable useBoundingBox to 'yes' to let the program decide
//                              between box and sphere [that is the old behavior]
//                            
// 22 Feb 2008  Daniel        Added possibility to define your own volume as bounding volume. 
//                            Use commands defined in MGGeneratorPositionSamplingMessenger class.
//                            CAREFUL: You might only sample from parts of the physical volume if you 
//                                     define the bounding volume to small. 
//                                     Or you might not sample at all if your own bounding Volume 
//                                     doesn't overlap with physical volume!
//                                     Be sure what you actually
//                                     do and cross check vertex distribution in root file!!!
//      
// 30 May 2008   Luciano      Fixed possible problem with dummy_vertex: it may be outside the world because 
//                            the dimension of the world volume is not fixed but determined according to the 
//                            bounding radius of the detector. World size is retrieved and stored only once. 
//                            Dummy vertex is generated at one corner of the world box.
// 30 May 2008   Daniel       now all the functions from MGGeneratorPositionSamplingMessenger are usable. 
//                            Before only some were implemented
//                            USE WITH CARE!!!
// 30 May 2008   Daniel       possibility to add an offset to the position of the center of the bounding volume 
//                            through MGGeneratorPositionSamplingMessenger. 
//                            USE WITH CARE!!!
// 03 Jun 2008   Luciano      Changed name of the variable SetCenterManually according to the MaGe coding rules
// 04 Aug 2008   Daniel       Added calculation of "true" size of sampling volume if you use MGGeneratorPositionSamplingMessenger "Sphere". 
//                            Needed i.e. for some estimate of liquid argon contamination
// 07 Jan 2009   Jens         The maximum number of tries to find a vertex inside the volume will be limmited by fNPositionsamplingMax
//                            Example "/MG/generator/Sampling/NPositionsamplingMax 10000000"
// 15 Jul 2009   J. Detwiler, Fixed sampling bug.  Now able to handle
//               P. Finnerty  rotated,translated mothers and daughter volumes
//               
//---------------------------------------------------------------------------

//system headers
#include <iostream>

//Geant4 headers
#include "Randomize.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisExtent.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

//MaGe includes
#include "generators/MGGeneratorPositionSampling.hh"
#include "generators/MGGeneratorPositionSamplingMessenger.hh"
#include "generators/MGGeneratorUtil.hh"
#include "io/MGLogger.hh"
#include "geometry/MGPhysVolNavigator.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"

using namespace std;

using namespace CLHEP;

MGGeneratorPositionSampling::MGGeneratorPositionSampling() :
  fMyVolume(0),
  fNPositionsamplingMax(1000000)
{
  fVolumeName="NULL";
  fMyNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  fGeneratorUtil = new MGGeneratorUtil();
  fCopyNumber = 0;
  fSolid_type = "NULL";
  fSolidDaughter1_type = "NULL";
  fSolidDaughter2_type = "NULL";
  fRadius = 0;
  fMessenger = new MGGeneratorPositionSamplingMessenger(this);

  //default values for changable variables through messenger
  fInnerCylinderRadius   = 0*cm;
  fInnerSphereRadius     = 0*cm;
  fInnerDiskRadius       = 0*cm; 
  fCylinderStartingAngle = 0 * deg;
  fCylinderSpanningAngle = 360 * deg;
  fXCenter = 0.;
  fYCenter = 0.;
  fZCenter = 0.;
  fUseCenterManually = false;
  fNCalls = 0;
  fNTries = 0;

  //half-size of the world volume, used for the dummy vertex
  fWorldVolumeHalfLength = -1;

}

MGGeneratorPositionSampling::~MGGeneratorPositionSampling()
{
  if(GetGeometricalVolumeName()=="Sphere"){
      double Vol_Sphere_in_cm3 =  ((4./3.) * 3.141592654 * 
                                   ( ((fOuterSphereRadius/CLHEP::cm) *(fOuterSphereRadius/CLHEP::cm) * (fOuterSphereRadius/CLHEP::cm)) - 
                                     ((fInnerSphereRadius/CLHEP::cm) * (fInnerSphereRadius/CLHEP::cm) * (fInnerSphereRadius/CLHEP::cm)) ) );
      double ratio = ((double)fNCalls/(double)fNTries);
      MGLog(routine) <<"Estimate of true size of volume (limited by sphere) = "<<endlog;
      MGLog(routine) << Vol_Sphere_in_cm3 <<"  *  (" <<fNCalls<<"/"<<fNTries << ") cm^3" <<endlog;
      MGLog(routine) << Vol_Sphere_in_cm3 * ratio << " cm^3" <<endlog;
  }    
  
  else if(GetGeometricalVolumeName()=="Cylinder"){
      double Vol_Cyl_in_cm3 = ( 3.141592654 * 
                                ( ((fOuterCylinderRadius/CLHEP::cm) *(fOuterCylinderRadius/CLHEP::cm)) - 
                                  ((fInnerCylinderRadius/CLHEP::cm) * (fInnerCylinderRadius/CLHEP::cm)) ) * 
                                (fCylinderHeight/CLHEP::cm) *  
                                (fCylinderSpanningAngle)/(2.0*3.141592654));  
      double ratio = ((double)fNCalls/(double)fNTries);
      MGLog(routine) <<"Estimate of true size of volume (limited by cylinder) = "<<endlog;
      MGLog(routine) << Vol_Cyl_in_cm3 <<"  *  (" <<fNCalls<<"/"<<fNTries << ") cm^3" <<endlog;
      MGLog(routine) << Vol_Cyl_in_cm3 * ratio << " cm^3" <<endlog;
  }    

  delete fGeneratorUtil;
  delete fMessenger;
}

G4ThreeVector MGGeneratorPositionSampling::SampleUniformlyInVolume(G4String volName,G4int copyN)
{
  G4ThreeVector vertex_world(0.,0.,0.); //vertex in world coordinates

  if (fWorldVolumeHalfLength < 0)
    {
      fWorldVolumeHalfLength = MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetWorldHalfLength();
      MGLog(debugging) << "World volume half-length = " <<  fWorldVolumeHalfLength/m << " m" << endlog;
    }
  G4ThreeVector vertex_dummy(-0.999*fWorldVolumeHalfLength,
			     -0.999*fWorldVolumeHalfLength,
			     -0.999*fWorldVolumeHalfLength); //dummy vertex in world coordinates if no point 
  //beloning to sampled volume could be found
  if (volName == "NULL") 
     {
       MGLog(warning) << 
 	"The confinement is set but the volume was not specified" << endlog;
       MGLog(warning) << "Return default position" << endlog; 
       return vertex_world;
     }

  //Step 1: check if the volume changed since the last call
  G4bool ifound = true;
  if (volName != fVolumeName || copyN != fCopyNumber) //volume changed
    {  
      fVolumeName = volName;
      fCopyNumber = copyN;
      ifound = InitializeSamplingVolume();
      if (!ifound)
	{
	  MGLog(warning) << 
	    "The confinement is set but the volume was not specified" << endlog;
	  MGLog(warning) << "Return default position" << endlog; 
	  return vertex_world;
	}
      else 
	{
	  MGLog(debugging) << "Volume initialized" << endlog;
	}
    }

  //Step 2: sample the point in the three cases
  G4ThreeVector vertex;       // vertex in local volume's coordinates
 
  MGLog(debugging) << "Solid type: " << fSolid_type << endlog;

  if(GetGeometricalVolumeName()=="Sphere" || GetGeometricalVolumeName()=="Cylinder"||GetGeometricalVolumeName()=="Box" ||GetGeometricalSurfaceName()=="Sphere"||GetGeometricalSurfaceName() == "Cylinder" || GetGeometricalSurfaceName() == "Disk"){
    if(GetGeometricalVolumeName()=="Sphere"){
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      
      G4bool point_in_volume = false;
      G4int iloop = 0;
      fNCalls++;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_in_shell(r1, r2);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
      fNTries += (iloop);
    }
    else if(GetGeometricalVolumeName()=="Cylinder") {
      //	std::cout<<"Sampling in Cylinder!!!!"<<std::endl;
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      G4double h       = fSolid_par_arr[2] * 2.0;
      G4double theta0  = fSolid_par_arr[3];
      G4double d_theta = fSolid_par_arr[4];
      
      G4bool point_in_volume = false;
      G4int iloop = 0;
      fNCalls++;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_in_annulus(r1, r2, h, theta0,d_theta);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
      fNTries += (iloop);

    }
    else if(GetGeometricalVolumeName()=="Box") {
      G4double x_lo = fSolid_par_arr[0];
      G4double x_hi = fSolid_par_arr[1];
      G4double y_lo = fSolid_par_arr[2];
      G4double y_hi = fSolid_par_arr[3];
      G4double z_lo = fSolid_par_arr[4];
      G4double z_hi = fSolid_par_arr[5];      

      G4bool point_in_volume = false;
      G4int iloop = 0;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_in_box(x_lo, x_hi, y_lo, y_hi, 
						     z_lo, z_hi);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
    }
    else if(GetGeometricalSurfaceName()=="Sphere"){

      G4double r2      = fSolid_par_arr[1];
      
      G4bool point_in_volume = false;
      G4int iloop = 0;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_on_sphere(r2);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
    }
    else if(GetGeometricalSurfaceName()=="Cylinder"){
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      G4double h       = fSolid_par_arr[2] * 2.0;
      
      G4bool point_in_volume = false;
      G4int iloop = 0;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_on_wholetube(r1, r2, h);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
    }
    else if(GetGeometricalSurfaceName()=="Disk"){
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      
      G4bool point_in_volume = false;
      G4int iloop = 0;
      while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	{  
	  vertex = fGeneratorUtil->pick_point_in_annulus(r1, r2,0.0*cm);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	  G4VPhysicalVolume* pVol_sampled = 
	    fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	  if (pVol_sampled == fMyVolume) {
	    point_in_volume = true;
	  }
	  iloop++;
	}
    }
  }

  else{
    //If the type is G4Box or G4Tubs it's not necessary 
    //to shoot and check many times, unless it has daughters
    if (fSolid_type == "G4Box") 
      {
	G4double x_lo = fSolid_par_arr[0];
	G4double x_hi = fSolid_par_arr[1];
	G4double y_lo = fSolid_par_arr[2];
	G4double y_hi = fSolid_par_arr[3];
	G4double z_lo = fSolid_par_arr[4];
	G4double z_hi = fSolid_par_arr[5];
	if (fHasDaughters) {
	  G4bool point_in_volume = false;
	  G4int iloop = 0;
	  while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	    {  
	      vertex = fGeneratorUtil->pick_point_in_box(x_lo, x_hi, y_lo, y_hi, 
							 z_lo, z_hi);
	      vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	      G4VPhysicalVolume* pVol_sampled = 
		fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	      if (pVol_sampled == fMyVolume) {
		point_in_volume = true;
	      }
	      iloop++;
	    }
	} else {
	  vertex = fGeneratorUtil->pick_point_in_box(x_lo, x_hi, y_lo, y_hi, 
						     z_lo, z_hi);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	}
      }
    
    
    if ((fSolid_type == "G4Tubs")  || (fSolid_type == "G4SubtractionSolid" &&
				       fSolidDaughter1_type == "G4Tubs" )    )
      {
	G4double r1      = fSolid_par_arr[0];
	G4double r2      = fSolid_par_arr[1];
	G4double h       = fSolid_par_arr[2] * 2.0;
	G4double theta0  = fSolid_par_arr[3];
	G4double d_theta = fSolid_par_arr[4];
	if (fHasDaughters) {
	  G4bool point_in_volume = false;
	  G4int iloop = 0;
	  while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	    {  
	      vertex = fGeneratorUtil->pick_point_in_annulus(r1, r2, h, theta0, 
							     d_theta);
	      vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	      G4VPhysicalVolume* pVol_sampled = 
		fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	      if (pVol_sampled == fMyVolume) {
		point_in_volume = true;
	      }
	      iloop++;
	    }
	} 
	else {
	  vertex = fGeneratorUtil->pick_point_in_annulus(r1, r2, h, theta0, 
							 d_theta);
	  vertex_world = (GetGlobalRotation()*vertex)+GetGlobalTranslation();
	}
      }
    
    //for other volumes and for non-located boxes/tubs one must instead shoot and try
    
    if (fSolid_type == "BoundingSphere") 
      {
       	G4bool point_in_volume = false;
	G4int iloop=0;
	while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops
	  {  
	    // loop until point falls in physical vol.
	    // vertex = fGeneratorUtil->pick_point_in_sphere(fRadius)+ fCentre; //fCenter needs rotation
	    vertex = fGeneratorUtil->pick_point_in_sphere(fRadius)+ (GetGlobalRotation()*fCentre); 
	    //It is a sphere: no need to rotate!
	    vertex_world = vertex + GetGlobalTranslation();
	    G4VPhysicalVolume* pVol_sampled = 
	      fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	    if (pVol_sampled == fMyVolume) {
	      point_in_volume = true;
	    }
	    iloop++;
	  }
      }
    
    //for other volumes and for non-located boxes/tubs one must instead shoot and try
    if (fSolid_type == "BoundingBox")
      {
	G4double x_lo = fSolid_par_arr[0];
	G4double x_hi = fSolid_par_arr[1];
	G4double y_lo = fSolid_par_arr[2];
	G4double y_hi = fSolid_par_arr[3];
	G4double z_lo = fSolid_par_arr[4];
	G4double z_hi = fSolid_par_arr[5];
	
	G4bool point_in_volume = false;
	G4int iloop=0;
	//while (!point_in_volume && iloop<10000) //prevents infinite loops 
	while (!point_in_volume && iloop<fNPositionsamplingMax) //prevents infinite loops 
	  {  
	    // loop until point falls in physical vol.
	    vertex = fGeneratorUtil->pick_point_in_box(x_lo, x_hi, y_lo, y_hi, 
						       z_lo, z_hi); 
	    vertex_world =  (GetGlobalRotation()*vertex) + GetGlobalTranslation();	  
	    G4VPhysicalVolume* pVol_sampled = 
	      fMyNavigator->LocateGlobalPointAndSetup(vertex_world);
	    if (pVol_sampled == fMyVolume) {
	      point_in_volume = true;
	    }
	    iloop++;
	  }
	
      }
  }


  if (fMyNavigator->LocateGlobalPointAndSetup(vertex_world) != fMyVolume){
    MGLog(warning) << "The sampled point is not in the right volume!?!" << endlog;
    MGLog(warning) << "The point " << vertex_world/cm << " is in " << 
      fMyNavigator->LocateGlobalPointAndSetup(vertex_world)->GetName() << endlog;
    MGLog(warning) << "Returning dummy vertex" << endlog;
    vertex_world = vertex_dummy;
  }

  return vertex_world;
}

G4bool MGGeneratorPositionSampling::InitializeSamplingVolume()
{
  MGLog(debugging) << "Starting InitializeSamplingVolume" << endlog;  
  MGLog(debugging) << "Volume to be found: " << fVolumeName << endlog; 

  //For now it works if there are no copies
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  G4int nVolumes = (G4int) volumeStore->size();
  G4bool ifound = false;
  G4int nfound=0;
  G4int i;
  for (i=0; i<nVolumes; i++) {
    if ((*volumeStore)[i]->GetName() == fVolumeName) {
      ifound = true;
      nfound++;
      fMyVolume = (*volumeStore)[i];   
    }
  }

  if (nfound > 1) {
    ifound = false;
    MGLog(warning) << "The volume name is not unique: retrieve copy number" << endlog;
    for (i=0; i<nVolumes; i++) {
      if ((*volumeStore)[i]->GetName() == fVolumeName && 
	  (*volumeStore)[i]->GetCopyNo() == fCopyNumber && !ifound){
	ifound = true;
	fMyVolume = (*volumeStore)[i];
      }
    }
  }

  if (!ifound) return ifound;

  MGLog(debugging) << "Found volume: " << fMyVolume->GetName() << ", copy " << 
    fMyVolume->GetCopyNo() << endlog;
 
  //Store the dimensions of solids for Tubs and Boxes
  G4VSolid* pSolid = fMyVolume->GetLogicalVolume()->GetSolid();
  fHasDaughters = (fMyVolume->GetLogicalVolume()->GetNoDaughters() != 0);
  fSolid_type = pSolid->GetEntityType();

  MGLog(debugging) << "pSolid->GetEntityType()= " << fSolid_type << endlog;
  MGLog(debugging) << "GeometricalVolumeName() = " << GetGeometricalVolumeName() << endlog;

  //continue here
  if(GetGeometricalVolumeName()=="Sphere" || GetGeometricalVolumeName()=="Cylinder" ||GetGeometricalVolumeName()=="Box"||GetGeometricalSurfaceName()=="Sphere"||GetGeometricalSurfaceName() == "Cylinder" || GetGeometricalSurfaceName() == "Disk")
    {
      if(GetGeometricalVolumeName()=="Sphere"){
	fSolid_par_arr[0] = fInnerSphereRadius;
	fSolid_par_arr[1] = fOuterSphereRadius;
      }
      
      else if(GetGeometricalVolumeName()=="Cylinder"){
	fSolid_par_arr[0] = fInnerCylinderRadius;
	fSolid_par_arr[1] = fOuterCylinderRadius;
	fSolid_par_arr[2] = fCylinderHeight * 0.5;
	fSolid_par_arr[3] = fCylinderStartingAngle;
	fSolid_par_arr[4] = fCylinderSpanningAngle;
      }
      
      else if(GetGeometricalVolumeName()=="Box"){
	fSolid_par_arr[0] = - fXLengthBox * 0.5;
	fSolid_par_arr[1] =   fXLengthBox * 0.5;
	fSolid_par_arr[2] = - fYLengthBox * 0.5;
	fSolid_par_arr[3] =   fYLengthBox * 0.5;
	fSolid_par_arr[4] = - fZLengthBox * 0.5;
	fSolid_par_arr[5] =   fZLengthBox * 0.5;
      }
      
      else if(GetGeometricalSurfaceName()=="Sphere"){
	fSolid_par_arr[0] = fInnerSphereRadius;
	fSolid_par_arr[1] = fOuterSphereRadius;
      }  
      
      else if(GetGeometricalSurfaceName()=="Cylinder"){
	fSolid_par_arr[0] = fInnerCylinderRadius;
	fSolid_par_arr[1] = fOuterCylinderRadius;
	fSolid_par_arr[2] = fCylinderHeight * 0.5;
	fSolid_par_arr[3] = fCylinderStartingAngle;
	fSolid_par_arr[4] = fCylinderSpanningAngle;
      }  
      
      else if(GetGeometricalSurfaceName()=="Disk"){
	fSolid_par_arr[0] = fInnerDiskRadius;
	fSolid_par_arr[1] = fOuterDiskRadius;
      }  
      
    }
  //stop continuing here
  else{
    if (fSolid_type == "G4Box") 
      {
	MGLog(debugging) << "Identified source solid type as G4Box." << endlog;
	//MGLog(debugging) << "Geometry parameters for box shape follow:" << endlog;
	G4double x_half_length = ((G4Box*)pSolid)->GetXHalfLength();
	G4double y_half_length = ((G4Box*)pSolid)->GetYHalfLength();
	G4double z_half_length = ((G4Box*)pSolid)->GetZHalfLength();
	
	//      MGLog(debugging) << "X half-length (mm): " << x_half_length << endlog;
	//      MGLog(debugging) << "Y half-length (mm): " << y_half_length << endlog;
	//      MGLog(debugging) << "Z half-length (mm): " << z_half_length << endlog;
	
	fSolid_par_arr[0] = -x_half_length;
	fSolid_par_arr[1] =  x_half_length;
	fSolid_par_arr[2] = -y_half_length;
	fSolid_par_arr[3] =  y_half_length;
	fSolid_par_arr[4] = -z_half_length;
	fSolid_par_arr[5] =  z_half_length;

      }
    else if (fSolid_type == "G4Tubs") 
      {
	MGLog(debugging) << "Identified source solid type as G4Tubs." << endlog;
      //	MGLog(debugging) << "Geometry parameters for annulus shape follow:" << endlog;
	fSolid_par_arr[0] = ((G4Tubs*)pSolid)->GetInnerRadius();
	fSolid_par_arr[1] = ((G4Tubs*)pSolid)->GetOuterRadius();
	fSolid_par_arr[2] = ((G4Tubs*)pSolid)->GetZHalfLength();
	fSolid_par_arr[3] = ((G4Tubs*)pSolid)->GetStartPhiAngle();
	fSolid_par_arr[4] = ((G4Tubs*)pSolid)->GetDeltaPhiAngle();
	//MGLog(debugging) << "Inner radius (mm):        " << fSolid_par_arr[0] << endlog;
	//MGLog(debugging) << "Outer radius (mm):        " << fSolid_par_arr[1] << endlog;
	//MGLog(debugging) << "Z half-length (mm):       " << fSolid_par_arr[2] << endlog;
	//MGLog(debugging) << "Starting phi angle (rad): " << fSolid_par_arr[3] << endlog;
	//MGLog(debugging) << "Delta phi angle (rad):    " << fSolid_par_arr[4] << endlog;
      }
    else if (fSolid_type == "G4SubtractionSolid")
      {
	G4VSolid* firstSolid = (G4SubtractionSolid*)pSolid->GetConstituentSolid(0);
	fSolidDaughter1_type = firstSolid->GetEntityType();
	G4VSolid* secondSolid = (G4SubtractionSolid*)pSolid->GetConstituentSolid(1);
	fSolidDaughter2_type = secondSolid->GetEntityType();
	if (fSolidDaughter1_type=="G4Tubs") {
	  fSolid_par_arr[0] = ((G4Tubs*)firstSolid)->GetInnerRadius();
	  fSolid_par_arr[1] = ((G4Tubs*)firstSolid)->GetOuterRadius();
	  fSolid_par_arr[2] = ((G4Tubs*)firstSolid)->GetZHalfLength();
	  fSolid_par_arr[3] = ((G4Tubs*)firstSolid)->GetStartPhiAngle();
	  fSolid_par_arr[4] = ((G4Tubs*)firstSolid)->GetDeltaPhiAngle();
	  //cout << "Inner radius (mm):        " << fSolid_par_arr[0] << endl;
	  //cout << "Outer radius (mm):        " << fSolid_par_arr[1] << endl;
	  //cout << "Z half-length (mm):       " << fSolid_par_arr[2] << endl;
	  //cout << "Starting phi angle (rad): " << fSolid_par_arr[3] << endl;
	  //cout << "Delta phi angle (rad):    " << fSolid_par_arr[4] << endl;
	  fHasDaughters = true; //this means that the first volume has an "hole"
	}
	else { //two different bounding volumes available, Sphere or Box. Use the one with smaller volume, to speed up sampling 
	  G4double x_half_length = pSolid->GetExtent().GetXmax();
	  G4double y_half_length = pSolid->GetExtent().GetYmax();
	  G4double z_half_length = pSolid->GetExtent().GetZmax();
	  fSolid_par_arr[0] = -x_half_length;
	  fSolid_par_arr[1] =  x_half_length;
	  fSolid_par_arr[2] = -y_half_length;
	  fSolid_par_arr[3] =  y_half_length;
	  fSolid_par_arr[4] = -z_half_length;
	  fSolid_par_arr[5] =  z_half_length;
	  fRadius = pSolid->GetExtent().GetExtentRadius();
	  
	  G4double vol_boundsphere = 4/3 * 3.14159 * fRadius * fRadius * fRadius;
	  G4double vol_boundbox    = (x_half_length * 2) * (y_half_length * 2) * (z_half_length * 2);
	 
	  
	  fSolid_type = "BoundingSphere";
	  //std::cout << std::endl <<"UseBoundingBox==\""<< UseBoundingBox << "\"" << std::endl;
	  if( getenv("UseBoundingBox")!= NULL  ) {
	    G4String strUseBoundingBox;
	    strUseBoundingBox = getenv("UseBoundingBox");
	    strUseBoundingBox.toLower();
	    if( strUseBoundingBox=="yes" ) {
	      std::cout << "Enable use of Bounding Box for position sampling." << std::endl;
	      if(vol_boundsphere <= vol_boundbox){ fSolid_type = "BoundingSphere";}
	      else fSolid_type = "BoundingBox";
	      //    std::cout << std::endl <<"Chosen volume: "<< fSolid_type << std::endl; //!!!
	    }
	  }
	}
      }  
    else {
      MGLog(debugging) << "The source volume solid is neither G4Box nor G4Tubs." << endlog;
      //two different bounding volumes available. Use the one with smaller volume, to speed up sampling 
      G4double x_half_length = pSolid->GetExtent().GetXmax();
      G4double y_half_length = pSolid->GetExtent().GetYmax();
      G4double z_half_length = pSolid->GetExtent().GetZmax();
      fSolid_par_arr[0] = -x_half_length;
      fSolid_par_arr[1] =  x_half_length;
      fSolid_par_arr[2] = -y_half_length;
      fSolid_par_arr[3] =  y_half_length;
      fSolid_par_arr[4] = -z_half_length;
      fSolid_par_arr[5] =  z_half_length;
      fRadius = pSolid->GetExtent().GetExtentRadius();
    
      G4double vol_boundsphere = 4/3 * 3.1415 * fRadius * fRadius * fRadius;
      G4double vol_boundbox    = ( x_half_length * 2) * ( y_half_length * 2) * ( z_half_length * 2);
      

      fSolid_type = "BoundingSphere";
      //std::cout << std::endl <<"UseBoundingBox==\""<< UseBoundingBox << "\"" << std::endl;
      if( getenv("UseBoundingBox")!= NULL  ) {
        G4String strUseBoundingBox;
        strUseBoundingBox = getenv("UseBoundingBox");
        strUseBoundingBox.toLower();
        if( strUseBoundingBox=="yes" ) {
	  std::cout << "Enable use of Bounding Box for position sampling." << std::endl;
	  if(vol_boundsphere <= vol_boundbox){ fSolid_type = "BoundingSphere";}
	  else fSolid_type = "BoundingBox";
	  //    std::cout << std::endl <<"Chosen volume: "<< fSolid_type << std::endl; //!!!
        }
      }
    }
  }

  MGLog(debugging) << "Retrieving bounding sphere dimensions...." << endlog;
  fRadius = pSolid->GetExtent().GetExtentRadius();
  G4Point3D centrepoint = pSolid->GetExtent().GetExtentCentre(); 
  fCentre = G4ThreeVector(centrepoint.x(), centrepoint.y(), centrepoint.z()); 
  MGLog(debugging) << "Sphere for solid. " << "Radius " << fRadius/mm << " mm" << endlog;

  G4VPhysicalVolume* worldV = fMyNavigator->GetWorldVolume();
  if (!worldV) MGLog(error) << "World volume not defined" << endlog;
  
  G4VPhysicalVolume* testVolume = fMyVolume;
  G4ThreeVector zeroVector(0,0,0);
  G4ThreeVector globTrasl = zeroVector;
  G4RotationMatrix identity;
  G4RotationMatrix  rotM=identity;   // Initialised to identity
  G4int iloop = 0;
  fRotations.clear();
  fTranslations.clear();
  while (testVolume != worldV && iloop < nVolumes) 
    {
      //MGLog(debugging) << "Going up: volume " << testVolume->GetName() << endlog;
      fRotations.push_back((*(testVolume->GetObjectRotation())));
      fTranslations.push_back((testVolume->GetObjectTranslation()));
      rotM = (*(testVolume->GetObjectRotation())) * rotM;
      testVolume = MGPhysVolNavigator::FindDirectMother(testVolume);
      if(testVolume == NULL) return false;
      iloop++; 
    }
  //This is the world volume, which is not included in the previous loop
  fRotations.push_back(identity);
  fTranslations.push_back(zeroVector);

  MGLog(debugging) << "Nested volumes found: " << fRotations.size()  << endlog;
  /*
    fRotations[0] and fTranslations[0] refer to the target volume
    fRotations[1] and fTranslations[1], to the direct ancestor, etc.
    It is necessary to rotate with respect to the frame of the ancestor.
    The loop should be "transparent" (namely, should work as the previous version) 
    if there are no rotations (or only the target volume is rotated): rotations are 
    identity matrices and globTrasl = sum(fTranslations)
  */
  if (fRotations.size() > 1) //otherwise the target is the World!
    {
      for (size_t ii=0;ii<(fRotations.size()-1);ii++)
	{
          G4RotationMatrix tmpRot = identity;
          for(size_t j = ii+1; j<fRotations.size()-1;j++){
            tmpRot = fRotations[j]*tmpRot;
          }
          globTrasl += tmpRot*fTranslations[ii];
	}
    }
  else {
    MGLog(warning) << "Target = world (?)" << endlog;
  }


  SetGlobalRotation(rotM);
  if(!fUseCenterManually){
    //standard functionallity
    SetGlobalTranslation(globTrasl);
    MGLog(debugging) << "Global volume translation: " << globTrasl/mm << " mm " << endlog;
  }
  //if offset is set, then add translation
  else{
    G4ThreeVector ManualTrasl = globTrasl;
    ManualTrasl.setX(fXCenter + globTrasl.x() );
    ManualTrasl.setY(fYCenter + globTrasl.y() );
    ManualTrasl.setZ(fZCenter + globTrasl.z() );
    SetGlobalTranslation(ManualTrasl);
    MGLog(debugging) << "Global volume translation: " << ManualTrasl/mm << " mm " << endlog;
  }
  
  //Just dump infos 
  MGLog(debugging) << "Global volume rotation: " << rotM << endlog; 
  return ifound;
}	  

G4ThreeVector MGGeneratorPositionSampling::SampleOnSurface(G4String volName,G4int copyN)
{
  //This sampler works only for tubes and boxes without daugthers.

  G4ThreeVector vertex_world(0.,0.,0.); //vertex in world coordinates

  if (volName == "NULL") 
     {
       MGLog(warning) << 
 	"The confinement is set but the volume was not specified" << endlog;
       MGLog(warning) << "Return default position" << endlog; 
       return vertex_world;
     }

  //Step 1: check if the volume changed since the last call
  G4bool ifound = true;
  if (volName != fVolumeName || copyN != fCopyNumber) //volume changed
    {  
      fVolumeName = volName;
      fCopyNumber = copyN;
      ifound = InitializeSamplingVolume();
      if (!ifound)
	{
	  MGLog(warning) << 
	    "The confinement is set but the volume was not specified" << endlog;
	  MGLog(warning) << "Return default position" << endlog; 
	  return vertex_world;
	}
      else 
	{
	  MGLog(debugging) << "Volume initialized" << endlog;
	}
    }
  
  //Step 2: sample the point in the three cases
  G4ThreeVector vertex;       // vertex in local volume's coordinates
 
  //This code works only for simple Tubes and Boxes, without any daugther.
  //For complex shapes, one should use the MGGeneratorGeneralSurfaceSampler generator
  if (fSolid_type == "G4Tubs" && !fHasDaughters)
    {
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      G4double h       = fSolid_par_arr[2] * 2.0;

      vertex       = fGeneratorUtil->pick_point_on_wholetube(r1, r2, h);
      vertex_world = (GetGlobalRotation()*vertex) + GetGlobalTranslation();
    } 
  else if (fSolid_type == "G4Box" && !fHasDaughters) 
    {
      G4double x_lo = fSolid_par_arr[0];
      G4double x_hi = fSolid_par_arr[1];
      G4double y_lo = fSolid_par_arr[2];
      G4double y_hi = fSolid_par_arr[3];
      G4double z_lo = fSolid_par_arr[4];
      G4double z_hi = fSolid_par_arr[5];

      vertex       = fGeneratorUtil->pick_point_on_box(x_lo, x_hi, y_lo, y_hi, z_lo, z_hi);
      vertex_world = (GetGlobalRotation()*vertex) + GetGlobalTranslation();
    } 
  else {
    MGLog(warning) << "SampleOnSurface() works only for G4Box and G4Tubes without dauthters " << endlog;
    MGLog(warning) << "Please use the GeneratorGeneralSurfaceSampler generator for arbitrary solids "  
		   << endlog;
    MGLog(warning) << "Zero position is returned " << G4endl;
  }
  
  return vertex_world;
}

