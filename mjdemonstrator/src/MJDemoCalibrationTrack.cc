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
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJDemoCalibrationTrack.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Class for the calibration track
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Mitzi Boswell
 * CONTACT: mitzbo@gmail.com
 * FIRST SUBMISSION: Dec 07, 2010
 * 
 * REVISION:
 * 
 * 12-07-2010, Created, M. Boswell
 * 01-12-2012, Changed track color from red to copper, K. Nguyen
 * 02-24-2016, Approximate helix with an arbitrary number of torus sections, 
 *             changed material to PEEK, modified default dimensions T. Caldwell
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Torus.hh"
#include "G4UnionSolid.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCalibrationTrack.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCalibrationTrack::MJDemoCalibrationTrack(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CalTrack", "PEEK")
{
  fHelixRadius = 18 * cm;
  fTubeInnerRadius = 0.476 * cm;
  fTubeOuterRadius = 1.27 / 2 * cm;
  fStartAngle = 60 * deg;
  fTotalAngle = 740 * deg;
  fHelixAngle = 3.328 * deg;
  fAngleBuffer = 0.25 * deg;
  fNsegments = 16;
}

MJDemoCalibrationTrack::MJDemoCalibrationTrack(const MJDemoCalibrationTrack & rhs) : 
  MJVDemoPart(rhs)
{
  fHelixRadius = rhs.fHelixRadius;
  fTubeInnerRadius = rhs.fTubeInnerRadius;
  fTubeOuterRadius = rhs.fTubeOuterRadius;
  fStartAngle = rhs.fStartAngle;
  fHelixAngle = rhs.fHelixAngle;
  fAngleBuffer = rhs.fAngleBuffer;
  fNsegments = rhs.fNsegments;
}

MJDemoCalibrationTrack::~MJDemoCalibrationTrack()
{;}

G4double MJDemoCalibrationTrack::GetHelixRadius() const
{
  return fHelixRadius;
}

G4double MJDemoCalibrationTrack::GetTubeInnerRadius() const
{
  return fTubeInnerRadius;
}

G4double MJDemoCalibrationTrack::GetTubeOuterRadius() const
{
  return fTubeOuterRadius;
}

G4double MJDemoCalibrationTrack::GetStartAngle() const
{
  return fStartAngle;
}

G4double MJDemoCalibrationTrack::GetTotalAngle() const
{
  return fTotalAngle;
}

G4double MJDemoCalibrationTrack::GetHelixAngle() const
{
  return fHelixAngle;
}

G4double MJDemoCalibrationTrack::GetAngleBuffer() const
{
  return fAngleBuffer;
}

G4int MJDemoCalibrationTrack::GetNsegments() const
{
  return fNsegments;
}

void MJDemoCalibrationTrack::SetHelixRadius(G4double helixRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fHelixRadius = helixRadius;
}

void MJDemoCalibrationTrack::SetTubeInnerRadius(G4double tubeInnerRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fTubeInnerRadius = tubeInnerRadius;
}

void MJDemoCalibrationTrack::SetTubeOuterRadius(G4double tubeOuterRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fTubeOuterRadius = tubeOuterRadius;
}

void MJDemoCalibrationTrack::SetStartAngle(G4double startAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fStartAngle = startAngle;
}

void MJDemoCalibrationTrack::SetTotalAngle(G4double totalAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fTotalAngle = totalAngle;
}

void MJDemoCalibrationTrack::SetHelixAngle(G4double helixAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fHelixAngle = helixAngle;
}

void MJDemoCalibrationTrack::SetAngleBuffer(G4double angleBuffer)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fAngleBuffer = angleBuffer;
}

void MJDemoCalibrationTrack::SetNsegments(G4int nsegments)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationTrack after "
		   << "construction" << endlog;
  else
    fNsegments = nsegments;
}

G4LogicalVolume* MJDemoCalibrationTrack::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    MGLog(debugging) << "Constructing the Calibration Track" << endlog;

    // compute dimensions for the torus sections that ensure the center
    // point at the end of each section follows a torus once rotated
    const G4double dphi = fTotalAngle / fNsegments;
    const G4double rot_ang_torus = atan(dphi *tan(fHelixAngle) / sin(dphi));
    const G4double dphi_torus =
      2 * atan((1 - cos(dphi)) / sqrt(pow(sin(dphi), 2) +
				      pow(dphi * tan(fHelixAngle), 2)));
    const G4double r_torus =
      fHelixRadius * (1 - cos(dphi)) / (1 - cos(dphi_torus));

    // running total of the angular and vertical offsets
    G4double phi = fStartAngle;
    G4double zoffset = 0.0;

    // small unrotated solid to start the solid union
    G4UnionSolid* unionSolid = NULL;
    G4Torus* torus_start = new G4Torus("calibTrackStart",
				       fTubeInnerRadius, fTubeOuterRadius,
				       r_torus, phi, fAngleBuffer);
    // add on the torus segments
    for(int i=0; i<fNsegments; i++){
      stringstream ssi;
      ssi << i;
      G4double angle = dphi_torus + fAngleBuffer;
      if(i > 0 && i < fNsegments - 1)
	angle += fAngleBuffer;
      G4Torus* torus = new G4Torus(("calibTrack" + ssi.str()).c_str(),
				   fTubeInnerRadius, fTubeOuterRadius,
				   r_torus, 0.0, angle);
      G4RotationMatrix* rot = new G4RotationMatrix();
      if(i == 0)
	rot->rotateZ(-phi);
      else
	rot->rotateZ(-phi + fAngleBuffer);
      rot->rotateX(rot_ang_torus);
      stringstream ss;
      ss << i - 1;
      if(i == 0)
	unionSolid = new G4UnionSolid(("body" + ss.str()).c_str(),
				      torus_start, torus, rot,
				      G4ThreeVector(0.0, 0.0, zoffset));
      else{
	string name = "calibTrack";
	if(i < fNsegments - 1)
	  name = "body" + ss.str();
	unionSolid = new G4UnionSolid(name.c_str(),
				      unionSolid, torus, rot,
				      G4ThreeVector(0.0, 0.0, zoffset));
      }
      phi += dphi;
      if(phi > twopi)
	phi -= twopi;
      zoffset -= fHelixRadius * tan(fHelixAngle) * dphi;
    }

    G4VisAttributes* TrackVisAtt =
      new G4VisAttributes(G4Colour(0.8, 0.36, 0.27, 0.8)); // coral
    TrackVisAtt->SetForceWireframe( false );
    
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(unionSolid, material, logicalName);
    pVol->SetVisAttributes(TrackVisAtt); 
    MGLog(debugging) << "Created Calibration Track Logical" << endlog;
  }
  else
    MGLog(debugging) << "Using pre-existing Calibration Track Logical" << endlog;
  return pVol;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

