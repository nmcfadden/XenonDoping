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
// CLASS IMPLEMENTATION:  MJDemoCalibrationSource.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for the calibration source
 *
 *  epoxy is not defined in the materials, so PEEK was used 
 *  similar densitys (1.36 epoxy, and 1.3 for PEEK)
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
 * 02-24-2016, Approximate helix with an arbitrary number of 
 *             torus sections, modified default dimensions, T. Caldwell
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
#include "mjdemonstrator/MJDemoCalibrationSource.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCalibrationSource::MJDemoCalibrationSource(G4String partName,
						 G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CalSource", "PEEK")
{
  fHelixRadius = 18 * cm;
  fTubeInnerRadius = 0.0;
  fTubeOuterRadius = 0.08 * cm;
  fStartAngle = 60 * deg;
  fTotalAngle = 740 * deg;
  fHelixAngle = 3.328 * deg;
  fAngleBuffer = 0.25 * deg;
  fNsegments = 16;
}

//---------------------------------------------------------------------------//

MJDemoCalibrationSource::MJDemoCalibrationSource(const MJDemoCalibrationSource & rhs) : 
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

//---------------------------------------------------------------------------//

MJDemoCalibrationSource::~MJDemoCalibrationSource()
{;}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetHelixRadius() const
{
  return fHelixRadius;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetTubeInnerRadius() const
{
  return fTubeInnerRadius;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetTubeOuterRadius() const
{
  return fTubeOuterRadius;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetStartAngle() const
{
  return fStartAngle;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetTotalAngle() const
{
  return fTotalAngle;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetHelixAngle() const
{
  return fHelixAngle;
}

//---------------------------------------------------------------------------//

G4double MJDemoCalibrationSource::GetAngleBuffer() const
{
  return fAngleBuffer;
}

//---------------------------------------------------------------------------//

G4int MJDemoCalibrationSource::GetNsegments() const
{
  return fNsegments;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetHelixRadius(G4double helixRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fHelixRadius = helixRadius;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetTubeInnerRadius(G4double tubeInnerRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fTubeInnerRadius = tubeInnerRadius;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetTubeOuterRadius(G4double tubeOuterRadius)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fTubeOuterRadius = tubeOuterRadius;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetStartAngle(G4double startAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fStartAngle = startAngle;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetTotalAngle(G4double totalAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fTotalAngle = totalAngle;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetHelixAngle(G4double helixAngle)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fHelixAngle = helixAngle;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetAngleBuffer(G4double angleBuffer)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fAngleBuffer = angleBuffer;
}

//---------------------------------------------------------------------------//

void MJDemoCalibrationSource::SetNsegments(G4int nsegments)
{
  if(fHasBeenPlaced)
    MGLog(warning) << "Attempting to modify MJDemoCalibrationSource after "
		   << "construction" << endlog;
  else
    fNsegments = nsegments;
}

//---------------------------------------------------------------------------//

G4LogicalVolume* MJDemoCalibrationSource::ConstructPart()
{
  
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if(pVol == NULL){
    MGLog(debugging) << "Constructing the Calibration Source" << endlog;

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
    G4Torus* torus_start = new G4Torus("calibSourceStart",
				       fTubeInnerRadius, fTubeOuterRadius,
				       r_torus, phi, fAngleBuffer);

    // add on the torus segments
    for(int i=0; i<fNsegments; i++){
      stringstream ssi;
      ssi << i;
      double angle = dphi_torus + fAngleBuffer;
      if(i > 0 && i < fNsegments - 1)
	angle += fAngleBuffer;
      G4Torus* torus = new G4Torus(("calibSource" + ssi.str()).c_str(),
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
	unionSolid = new G4UnionSolid(("sbody" + ss.str()).c_str(),
				      torus_start, torus, rot,
				      G4ThreeVector(0.0, 0.0, zoffset));
      else{
	string name = "calibSource";
	if(i < fNsegments - 1)
	  name = "sbody" + ss.str();
	unionSolid = new G4UnionSolid(name.c_str(),
				      unionSolid, torus, rot,
				      G4ThreeVector(0.0, 0.0, zoffset));
      }
      phi += dphi;
      if(phi > twopi)
	phi -= twopi;
      zoffset -= fHelixRadius * tan(fHelixAngle) * dphi;
    }

    G4VisAttributes* SourceVisAtt =
      new G4VisAttributes(G4Colour(0.8, 0.36, 0.27, 0.8)); // coral
    SourceVisAtt->SetForceWireframe( false );

    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(unionSolid, material, logicalName);
    pVol->SetVisAttributes(SourceVisAtt); 
    MGLog(debugging) << "Created Calibration Source Logical" << endlog;
  }
  else
    MGLog(debugging) << "Using pre-existing Calibration Source Logical"
		     << endlog;
  fHasBeenPlaced = true;
  return pVol;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
