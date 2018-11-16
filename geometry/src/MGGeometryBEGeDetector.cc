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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeometryBEGeDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: alexis
 * CONTACT: 
 * FIRST SUBMISSION: Mon Oct 26 13:35:21 PDT 2009
 * 
 * REVISION:
 *
 * 10-26-2009, creation, A. Schubert
 * 03-03-2010, fixed ditch depth; was off by a factor of two, A. Schubert
 * 04-28-2010, added protection for no-ditch configurations, L. Pandola
 * 04-28-2010, added boolean flag to decide where to put the ditch 
 *             (upper/lower surface). Upper surface is default. L. Pandola
 * 08-31-2010, set name to serial number, A. Schubert
 * 04 Feb 2011, changed default value of fDitchBelow from false to true to
 *              comply with definition in MGCrystalData, A. Schubert
 * 12-20-2011, added an option for conical corners to the crystal. B. Lehnert
 * 01-15-2012, added option to get G4 solid. B. Lehnert
 * 01-24-2012, added DL option to reduce size of BEGe to active volume (off by default) B. Lehnert
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4Color.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4Cons.hh"

#include "geometry/MGGeometryBEGeDetector.hh"      //Present MJ Class Headers 
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;
using namespace MaGe;

//---------------------------------------------------------------------------//

MGGeometryBEGeDetector::MGGeometryBEGeDetector( G4String serNum ):
  MGGeometryDetector( serNum ), 
  fHeight( 30.0 *  mm ),
  fRadius( 35.0 * mm ),
  fDitchOuterRadius( 0.875 / 2 * inch ),
  fDitchInnerRadius( 0.625 / 2 * inch ),
  fDitchDepth( 0.08 * inch ),
  fCornerDiameter( 0*mm ),
  fCornerHeight( 0*mm ),
  fCornerOnBottom(false), 
  fDLOuterContact( 0*mm ),
  fDLInnerContact( 0*mm ),
  fCrystalMaterial( NULL ),
  fCrystalMaterialName( "Germanium-Enr" ),
  fDitchBelow(true)
{;}

//--------------------------------------------------------------------

MGGeometryBEGeDetector::MGGeometryBEGeDetector(const MGGeometryBEGeDetector & rhs):
  MGGeometryDetector(""),
  fHeight( rhs.fHeight ),
  fRadius( rhs.fRadius ),
  fDitchOuterRadius( rhs.fDitchOuterRadius ),
  fDitchInnerRadius( rhs.fDitchInnerRadius ),
  fDitchDepth( rhs.fDitchDepth ),
  fCrystalMaterial( rhs.fCrystalMaterial )
{
  theDetectorLogical = rhs.theDetectorLogical;
  fDitchBelow = rhs.fDitchBelow;
}

//--------------------------------------------------------------------

MGGeometryBEGeDetector::~MGGeometryBEGeDetector()
{;}

//--------------------------------------------------------------------


void MGGeometryBEGeDetector::ConstructDetector()
{
  if( theDetectorLogical ) return;

  fCrystalMaterial = G4Material::GetMaterial(fCrystalMaterialName);

  if(!fCrystalMaterial) {
    MGLog(error) << "NULL Pointer for crystal material: " 
        << fCrystalMaterialName << endlog;
    Dump();
    MGLog(fatal) << endlog;

  }

  //  G4double eps = 0.00001 * cm;// Small distance to perturb solid placements.
                              // Avoids overlapping surfaces, etc...

  G4Tubs *detectorCylinder = new G4Tubs("crystal", 0*cm,
    fRadius-fDLOuterContact, fHeight/2.0-fDLOuterContact, 0*deg, 360.0*deg );
  G4VSolid*  crystalAfterDitch;
  G4VSolid*  crystalAfterCorner;
  G4VSolid*  crystalAfterInnerContactExtension;
  // the last step is only needed if the class is used to return the active volume of a BEGe
  // which has an extension at the inner contact due to smaller DL


  //Make ditch only if required
  if (fDitchInnerRadius < fDitchOuterRadius && fDitchDepth>0)
    {
      // make a ditch to subtract from the cylinder; this solid is twice the height
      // of the ditch:
      G4Tubs *ditch = new G4Tubs("ditchCylinder", fDitchInnerRadius,
				 fDitchOuterRadius, fDitchDepth, 0*deg, 360*deg ); 
      
      // subtract ditch; only half of the G4Tubs ditch extends into the
      // detectorCylinder and is subtracted; this gives the correct ditch depth:
      G4ThreeVector ditchVector( 0, 0, fHeight/2.0 );
      if (fDitchBelow)
	ditchVector.setZ(-fHeight/2.0);

      //solid with ditch
      crystalAfterDitch = new G4SubtractionSolid("crystalWithDitch", detectorCylinder, ditch, 0, ditchVector ); 
    } 
  else
    {
      //solid without ditch
        crystalAfterDitch = detectorCylinder;      
    }
  


  //Make corner only if required
  if (fCornerDiameter > 0 && fCornerHeight > 0)
  {
      
      // constructing cone for subtraction
      // create solid extending above/below the cylinder in order to avoid surface effects

	  // calculating the projection of the corner DL in the x-y plane
	  G4double alpha = atan(fCornerHeight / (fRadius - fCornerDiameter/2.0));
	  G4double coneDLProjectionX = fDLOuterContact / sin(alpha);

	  // height of corner increases with DL (see These K. von Sturm p.33 Fig 6.2 x-value)
	  G4double coneHeightExtension = fDLOuterContact / cos(alpha) - fDLOuterContact * tan(alpha);
      G4double coneHeight = (fCornerDiameter/2 - coneDLProjectionX)
    						  * (fCornerHeight + coneHeightExtension)
    						  / ((fRadius - fDLOuterContact) - (fCornerDiameter/2 - coneDLProjectionX))
    						  + fCornerHeight + coneHeightExtension;


      G4Cons* corner;
      G4ThreeVector* cornerVector;
      if(fCornerOnBottom)//for corner on bottom
      {          
	  corner = new G4Cons("corner", 0., fRadius-coneDLProjectionX , fRadius-fDLOuterContact, 2*(fRadius-coneDLProjectionX) , coneHeight/2, 0, 360.*deg);
	  cornerVector =  new G4ThreeVector( 0, 0, -(fHeight/2.0-fDLOuterContact) - coneHeight/2 + (fCornerHeight-fDLOuterContact+coneHeightExtension) );
      }
      else //for corner on top
      {
	  corner = new G4Cons("corner", fRadius-fDLOuterContact, 2*(fRadius-coneDLProjectionX), 0., fRadius-coneDLProjectionX, coneHeight/2, 0, 360.*deg);
	  cornerVector =  new G4ThreeVector( 0, 0,  fHeight/2.0-fDLOuterContact + coneHeight/2 - (fCornerHeight-fDLOuterContact+coneHeightExtension) );
      }
	  
      //solid with corner
      crystalAfterCorner = new G4SubtractionSolid("crystalWithCorner", crystalAfterDitch, corner, 0, *cornerVector );     
  }
  else
  {
      crystalAfterCorner = crystalAfterDitch;      
  }


  //Make inner contact extension (add volume at inner context to account for smaller DL)
  if (fDLOuterContact > 0 || fDLInnerContact > 0)
  {

	  //construct extension volume
	  G4Tubs *ICExtension = new G4Tubs("ICExtension", 0,
					 fDitchInnerRadius, (fDLOuterContact-fDLInnerContact)/2.0, 0*deg, 360*deg );
	  G4ThreeVector ICExtensionVector( 0, 0, +fHeight/2.0-fDLInnerContact-(fDLOuterContact-fDLInnerContact)/2.0);
      if (fDitchBelow)
    	  ICExtensionVector.setZ(-fHeight/2.0+fDLInnerContact+(fDLOuterContact-fDLInnerContact)/2.0);

      // combining volumes
      crystalAfterInnerContactExtension = new G4UnionSolid("crystalWithExtension", crystalAfterCorner, ICExtension, 0, ICExtensionVector );

  }
  else
  {
      crystalAfterInnerContactExtension = crystalAfterCorner;
  }

  fBEGeSolid = crystalAfterInnerContactExtension;

  // Create Crystal Geant 4 logical volume.
  theDetectorLogical = new G4LogicalVolume( crystalAfterInnerContactExtension,
					    fCrystalMaterial, "activeCrystalLogical", 0, 0, 0, true);


  // added for stand-alone detector:
  SetDetectorName( GetSerialNumber() );
  Dump();
}

//--------------------------------------------------------------------

void MGGeometryBEGeDetector::Dump()
{
  G4cout << "Settings for MGGeometryBEGeDetector:" << G4endl;                                                                       
  G4cout << "----------------------------------------" << G4endl;                                                                       
  G4cout << "Dimensions are in cm." << G4endl;
  G4cout << "fHeight: " << fHeight/cm << G4endl;                                                                                
  G4cout << "fRadius: " << fRadius/cm << G4endl;                                                                                
  G4cout << "fDitchOuterRadius: " << fDitchOuterRadius/cm << G4endl;
  G4cout << "fDitchInnerRadius: " << fDitchInnerRadius/cm << G4endl;                                           
  G4cout << "fDitchDepth: " << fDitchDepth/cm << G4endl;
  G4cout << "fDitchBelow: " << fDitchBelow << G4endl;                                                                  

  G4cout << "fCornerDiameter: " << fCornerDiameter/cm <<" cm"<< G4endl;
  G4cout << "fCornerHeight: " << fCornerHeight/cm <<" cm"<< G4endl;                                           
  G4cout << "fCornerOnBottom: " << fCornerOnBottom << G4endl;                                                                  

  G4cout << "fDLOuterContact: " << fDLOuterContact/micrometer << " microns" << G4endl;
  G4cout << "fDLInnerContact: " << fDLInnerContact/micrometer << " microns" <<  G4endl;
      
  G4cout << "fCrystalMaterialName: " <<
  fCrystalMaterialName << G4endl;                                                                                
  G4cout << "serial number: " << GetSerialNumber() << G4endl;
  G4cout << "----------------------------------------" << G4endl;                                                                       
}
 
