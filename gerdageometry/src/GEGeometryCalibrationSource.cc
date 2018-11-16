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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-07-2005
 *
 * REVISION:
 *
 * 08-25-2008 - Enable volumelist for calibration sources, Francis
 * 04-02-2011 - Eliminate overlap of two volumes, Francis
 * 27-03-2012 - Added possibility to move each calibration source separately. Giovanni Benato
 * 29-04-2016 - Source position independent of detector positions for phase2 (LockType_PhaseII_DN500),Janina
 * 13-07-2016 - Added possibility to move middle point of circle with sources,Janina
 * 14-10-2016 - Copper cap on the calib. source for LAr veto test JJ 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

//#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryCalibrationSource.hh"       

// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryCalibrationSource::GEGeometryCalibrationSource(G4int n, GEGeometryDetectorDB* detectordb)
{
  fDetectordb = detectordb;

  detectordb -> IncreaseCalibrationSourceCounter();

  fCalibrationSourceCounter      = detectordb -> GetCalibrationSourceCounter();  
  fNCalibrationSource            = n; 
  fCalibrationSourcePositionZ    = detectordb -> GetCalibrationSourcePositionZ( n ); 
  fCalSourcePhase2DN500AngleOffset = detectordb -> GetCalibrationSourcePhase2DN500AngleOffset( n );
  fCalSourcePhase2DN500Radius = detectordb -> GetCalibrationSourcePhase2DN500Radius( );
  fCalibrationSourcePhase2DN500OffsetX = detectordb -> GetCalibrationSourcePhase2DN500OffsetX( );
  fCalibrationSourcePhase2DN500OffsetY = detectordb -> GetCalibrationSourcePhase2DN500OffsetY( );
  fCalSourcePhase2CoordTrafo = detectordb -> GetCalibrationSourcePhase2CoordTrafo();
  fPhase2HolderStarPlacement = detectordb -> GetPhase2HolderStarPlacement();

  fCalibrationCylinderHeight     = detectordb -> GetCalibrationCylinderHeight(); 

  fCalibrationSourcePosition     = detectordb -> GetCrystalPosition(n, 0); 

  fDetectorMotherVolume          = detectordb -> GetDetectorMotherVolume(); 
  fCalibrationCylinderLogical    = detectordb -> GetCalibrationCylinderLogical(); 
  fCalibrationSourceInnerLogical = detectordb -> GetCalibrationSourceInnerLogical(); 
  fCalibrationSourceOuterLogical = detectordb -> GetCalibrationSourceOuterLogical(); 
  fCalibrationCylCapLogical      = detectordb -> GetCalibrationSourceCylCapLogical(); 
  fCalibrationCuCap              = detectordb -> GetFlagConstructCalibrationSourceCuCap(); 
}

// --------------------------------------------------------------------------- 

GEGeometryCalibrationSource::~GEGeometryCalibrationSource()
{
    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------

    //delete fCalibrationCylinderPhysical; 
    //delete fCalibrationSourceInnerPhysical; 
    //delete fCalibrationSourceOuterPhysical; 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryCalibrationSource::GetSourcePosition()
{

  G4ThreeVector position = fCalibrationSourcePosition; 
  
  G4double z = position.getZ() + fCalibrationSourcePositionZ; 

  position.setZ(z); 


  if (fDetectordb->GetLockType() == 5) // Phase II lock
	  /*if(fNCalibrationSource==0) // center string
	  {// here the position is forced from messengers for S1 in the new Phase II lock
		  position.setX(fDetectordb->GetCalibrationSourcePositionX_S1());
		  position.setY(fDetectordb->GetCalibrationSourcePositionY_S1());
		  position.setZ(fDetectordb->GetCalibrationSourcePositionZ_S1());

	  }*/
	  //x,y,z coordinates are set independently of the detector position
	  position.setX(cos(30.*deg-((G4double) fNCalibrationSource)*120.*deg+fCalSourcePhase2DN500AngleOffset)*fCalSourcePhase2DN500Radius+fCalibrationSourcePhase2DN500OffsetX);
	  position.setY(sin(30.*deg-((G4double) fNCalibrationSource)*120.*deg+fCalSourcePhase2DN500AngleOffset)*fCalSourcePhase2DN500Radius+fCalibrationSourcePhase2DN500OffsetY);
	  position.setZ(-fCalibrationSourcePositionZ+fCalSourcePhase2CoordTrafo+fPhase2HolderStarPlacement-0.5*fCalibrationCylinderHeight);
	  //transfer from coordinate system of calibration sources in reality to simulation coordinates
	  //in this way the calibration source positions in the GERDA Wiki can be used in macros

  return position; 

}

// --------------------------------------------------------------------------- 

G4double GEGeometryCalibrationSource::GetAngle(G4ThreeVector /*position*/) 
{

  // debug: calculate angle from position so that the opening of the 
  // debug: sources shows towards the inside of the array 

  G4double angle; 

  angle = 0.0 * deg; 

  return angle; 
  
}

// --------------------------------------------------------------------------- 

void GEGeometryCalibrationSource::ConstructCalibrationSource()
{
  
  G4double         angle; 
  G4RotationMatrix rm; 
  G4ThreeVector    position; 
  char             volumename[40]; 


  position = GetSourcePosition(); 
  angle = this -> GetAngle(position); 
  rm.rotateZ(angle);

  MGLog(debugging) << "Calibration source position " << position.x() << "\t" << position.y() << "\t" << position.z()  << endlog;


  // construct physical volume: absorber

  sprintf(volumename, "calibration_cylinder_%d", fCalibrationSourceCounter); 

  fCalibrationCylinderPhysical = new G4PVPlacement(G4Transform3D(rm, position), 
  					   fCalibrationCylinderLogical, 
  					   volumename,
  					   fDetectorMotherVolume, 
  					   false, 
  					   fNCalibrationSource);   


  // construct physical volume : source 

  position.setZ(position.getZ()+35.1*mm);

  sprintf(volumename, "calibration_source_inner_%d", fCalibrationSourceCounter); 

  fCalibrationSourceInnerPhysical = new G4PVPlacement(0,
						      position, 
						      fCalibrationSourceInnerLogical, 
						      volumename,
						      fDetectorMotherVolume, 
						      false, 
						      fNCalibrationSource);   
  


  // construct physical volume: capsule

  sprintf(volumename, "calibration_source_outer_%d", fCalibrationSourceCounter); 

  position.setZ(position.getZ()-4.15*mm);
  // position.setZ(position.getZ()-30.5*mm);    // Used when no absorber is simulated

  fCalibrationSourceOuterPhysical = new G4PVPlacement(0,
						      position, 
						      fCalibrationSourceOuterLogical, 
						      volumename,
						      fDetectorMotherVolume, 
						      false, 
						      fNCalibrationSource);   

  // construct physical volume: copper cap
  if (fCalibrationCuCap){
    sprintf(volumename, "calibration_source_cap_%d", fCalibrationSourceCounter); 

    position.setZ(position.getZ()+5.8*mm);
    // position.setZ(position.getZ()-30.5*mm);    // Used when no absorber is simulated

    fCalibrationCylCapPhysical = new G4PVPlacement(0,
						      position, 
						      fCalibrationCylCapLogical, 
						      volumename,
						      fDetectorMotherVolume, 
			               false, 
			      	      fNCalibrationSource); 
  }    
} 

// --------------------------------------------------------------------------- 


