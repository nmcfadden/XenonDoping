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
 * FIRST SUBMISSION: 01-23-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 21-04-2005, for non-true-coaxial detector,
 *             put segment into deadlayer, Xiang
 * 06-05-2005, added GetDeadLayerTopThickness
 * 
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
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh" 

#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryCrystalSegment.hh"       

// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryCrystalSegment::GEGeometryCrystalSegment(G4int phiseg, G4int zseg, G4int col, G4int row,  
						   GEGeometryDetectorDB* detectordb)
{
  
  fPhiSegmentNumber              = phiseg; 
  fZSegmentNumber                = zseg; 
  fCrystalColumn                 = col; 
  fCrystalRow                    = row; 

  fCrystalType                   = detectordb -> GetCrystalType(fCrystalColumn, fCrystalRow); 
  fDetectorParameters            = detectordb -> GetDetectorParameters(fCrystalType); 

  fCrystalGeometry               = fDetectorParameters -> GetGeometry(); 
  fCrystalHeight                 = fDetectorParameters -> GetHeight(); 
  fCrystalDLTopThickness         = fDetectorParameters -> GetDLTopThickness();
  fCrystalDLBottomThickness      = fDetectorParameters -> GetDLBottomThickness();
  fCrystalPassivationThickness   = fDetectorParameters -> GetPassivationThickness();
  fPhiSegmentsPerCrystal         = fDetectorParameters -> GetNPhiSegments(); 
  fZSegmentsPerCrystal           = fDetectorParameters -> GetNZSegments(); 
  fSegmentsPerCrystal            = fPhiSegmentsPerCrystal * fZSegmentsPerCrystal; 
  fSegmentHeight                 = fDetectorParameters -> GetSegmentHeight(); 
  fSegmentStartingAngle          = fDetectorParameters -> GetSegmentationAngle(); 
  fRelativeSegmentZPosition      = detectordb -> GetRelativeSegmentZPosition(fDetectorParameters, zseg); 

  fHolderInnerHeight             = detectordb -> GetHolderInnerHeight(); 

  fCrystalPosition               = detectordb -> GetCrystalPosition(fCrystalColumn, 
								    fCrystalRow); 
  fSegmentPosition               = fCrystalPosition; 

  fDetectorMotherVolume          = detectordb -> GetDetectorMotherVolume(); 

  fSegmentNumber                 = detectordb -> GetSegmentNumber(); 
  
  detectordb -> IncreaseSegmentNumber(); 

  fCrystalSegmentLogical       = detectordb -> GetCrystalSegmentLogical(fCrystalType); 

}

// --------------------------------------------------------------------------- 

GEGeometryCrystalSegment::~GEGeometryCrystalSegment()
{

    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------
    // delete fCrystalSegmentPhysical; 

} 

// --------------------------------------------------------------------------- 

G4double GEGeometryCrystalSegment::GetSegmentAngle(G4int seg) 
{

  G4double angle; 
  G4double segmentangle = 360.0 * deg / fPhiSegmentsPerCrystal;  

  angle = fSegmentStartingAngle + seg * segmentangle; 

  return angle; 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryCrystalSegment::GetSegmentPosition(G4int segrow)
{

  G4ThreeVector position = fCrystalPosition; 
  G4double      z; 

  if (fZSegmentsPerCrystal == 1)
    {
      //      z = fCrystalPosition.getZ() - 0.5 * (9.0 * cm - fCrystalHeight); 
      z = fCrystalPosition.getZ(); 
    }
  else 
    {
      z = fCrystalPosition.getZ() 
	+ 0.5 * fCrystalHeight 
	- fCrystalPassivationThickness
	- fCrystalDLTopThickness
	- 0.5 * fSegmentHeight 
	- float(segrow) * fSegmentHeight; 

//       z = fCrystalPosition.getZ() -
// 	0.5 * fHolderInnerHeight + 	
// 	fCrystalHeight - 
// 	fCrystalDeadLayerTopThickness - 
// 	0.5 * fSegmentHeight - 
// 	float(segrow) * fSegmentHeight; 
    }

  position.setZ(z); 

  return position; 

}

// --------------------------------------------------------------------------- 

void GEGeometryCrystalSegment::ConstructCrystalSegment()
{
  
  char namesegment[30]; 

  sprintf(namesegment,"Segment_%d", fSegmentNumber); 

  G4double         angle; 
  G4RotationMatrix rm; 
  G4ThreeVector    segmentposition; 
  
  angle = GetSegmentAngle(fPhiSegmentNumber); 
  
  rm.rotateZ(angle); 

  // non-true coaxial geometry 

  if ((fCrystalGeometry == 1)||(fCrystalGeometry == 4)||(fCrystalGeometry == 5)||(fCrystalGeometry == 11))
    {
      fCrystalSegmentPhysical = new G4PVPlacement(0,
						  fCrystalPosition,
						  fCrystalSegmentLogical,
						  namesegment,
						  fDetectorMotherVolume,
						  false, 
						  fSegmentNumber);      
    }
  
  // true coaxial geometry 
  
  else if (fCrystalGeometry == 2) 
    {
      //      segmentposition = GetSegmentPosition(fZSegmentNumber); 

      fSegmentPosition.setZ(fSegmentPosition.getZ() + fRelativeSegmentZPosition); 

      fCrystalSegmentPhysical = new G4PVPlacement(G4Transform3D(rm, fSegmentPosition), 
						  fCrystalSegmentLogical, 
						  namesegment,
						  fDetectorMotherVolume, 
						  false, 
						  fSegmentNumber);      
    }


} 

// --------------------------------------------------------------------------- 


