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
// CLASS IMPLEMENTATION:  MJDemoCalibrationAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  
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
 * 02-24-2016, Add members for disabling and positioning the
 *             source and track, T. Caldwell
 * 12-07-2010, Created, M. Boswell
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCalibrationAssembly.hh"
#include "mjdemonstrator/MJDemoCalibrationTrack.hh"
#include "mjdemonstrator/MJDemoCalibrationSource.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCalibrationAssembly::MJDemoCalibrationAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Calibration")
{
  fTrackOn = true;
  fSourceOn = false;
  fTrackOffset = G4ThreeVector(0.0, 0.0, 0.0);
  fSourceOffset = G4ThreeVector(0.0, 0.0, 0.0);
  fTrackPtr = new MJDemoCalibrationTrack(fName+"_Track_001", "867-5309");
  fSourcePtr = new MJDemoCalibrationSource(fName+"_Source_001", "867-5309");
}

MJDemoCalibrationAssembly::MJDemoCalibrationAssembly(const MJDemoCalibrationAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fTrackOn = rhs.fTrackOn;
  fSourceOn = rhs.fSourceOn;
  fTrackOffset = rhs.fTrackOffset;
  fSourceOffset = rhs.fSourceOffset;
  fTrackPtr = new MJDemoCalibrationTrack(fName+"_Track_001", "867-5309");
  fSourcePtr = new MJDemoCalibrationSource(fName+"_Source_001", "867-5309");
}

MJDemoCalibrationAssembly::~MJDemoCalibrationAssembly()
{
  delete fTrackPtr;
  delete fSourcePtr;
}

void MJDemoCalibrationAssembly::Place(G4ThreeVector* assemPosition, 
				      G4RotationMatrix* assemRotation,
				      G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,
							 *assemPosition);
  if(fTrackOn){
    G4RotationMatrix *TracklocalRot = new G4RotationMatrix();
    G4AffineTransform *Trackaffine = new G4AffineTransform(TracklocalRot,
							   fTrackOffset);
    *Trackaffine = *assemAffine;  
    G4ThreeVector *TrackglobalPos =
      new G4ThreeVector(Trackaffine->NetTranslation());
    G4RotationMatrix *TrackglobalRot=
      new G4RotationMatrix(Trackaffine->NetRotation());
    fTrackPtr->Place(TrackglobalPos, TrackglobalRot, motherLogical);
    delete TracklocalRot;
    delete Trackaffine;
  }

  if(fSourceOn){
    G4RotationMatrix *SourcelocalRot = new G4RotationMatrix();
    G4AffineTransform *Sourceaffine = new G4AffineTransform(SourcelocalRot,
							    fSourceOffset);
    *Sourceaffine = *assemAffine;  
    G4ThreeVector *SourceglobalPos =
      new G4ThreeVector(Sourceaffine->NetTranslation());
    G4RotationMatrix *SourceglobalRot =
      new G4RotationMatrix(Sourceaffine->NetRotation());
    fSourcePtr->Place(SourceglobalPos, SourceglobalRot, motherLogical);
    delete SourcelocalRot;
    delete Sourceaffine;
  }

  delete assemAffine;
}
