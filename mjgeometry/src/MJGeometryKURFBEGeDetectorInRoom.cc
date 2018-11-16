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
// CLASS IMPLEMENTATION:  MJGeometryKURFBEGeDetectorInRoom.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: 
 */
//---------------------------------------------------------------------------//
/**
 * AUTHOR: A. Schubert
 * CONTACT: alexis3@u.washington.edu
 * FIRST SUBMISSION: 
 * 
 * REVISION: 
 * 04-07-2010 Initial submission, A. Schubert
 * 08-27-2010 Added messenger argument to MJGeometryKURFBEGeDetector 
 *            constructor, A. Schubert
 * 08 Sep 2011, Added collimator geometry, position is done with messenger 
 *             commands and this geometry is disabled by default, P. Finnerty
 */
//---------------------------------------------------------------------------//
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4Color.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "TMath.h"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetectorInRoom.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

using namespace CLHEP;
using namespace MaGe;

//--------------------------------------------------------------------------//
MJGeometryKURFBEGeDetectorInRoom::MJGeometryKURFBEGeDetectorInRoom(G4String serNum) :
  MJGeometryKURFBEGeDetector(serNum, false),
  fCryostatPhysical(NULL),
  fCollimatorPhysical(NULL),
  fSourceDiskPhysical(NULL)
{
  SetDefaults();
}
//---------------------------------------------------------------------------//
MJGeometryKURFBEGeDetectorInRoom::~MJGeometryKURFBEGeDetectorInRoom()
{;}
//---------------------------------------------------------------------------//
void MJGeometryKURFBEGeDetectorInRoom::ConstructDetector()
{
  //World Materials
  G4Material *AirMat   = G4Material::GetMaterial("Air");
  G4Material *Vacuum   = G4Material::GetMaterial("Vacuum");
  G4Material *LeadMat  = G4Material::GetMaterial("Lead-Mod");
  
  if(!( AirMat || Vacuum || LeadMat)) {
    MGLog(error) << "NULL G4Material pointer!  Cannot find material!" << endlog;
    MGLog(fatal) << endlog;
  }
 
  //---------------------------------DETECTOR--------------------------------//
  // first construct the cryostat and crystal in the base class,
  // MJGeometryKURFBEGeDetector
  //-------------------------------------------------------------------------//

  MJGeometryKURFBEGeDetector::ConstructDetector();
  
  G4LogicalVolume* kurfBEGeDetecorLogical =
    MJGeometryKURFBEGeDetector::GetDetectorLogical();

  //----------------------------- WORLD VOLUME ------------------------------//
  //-------------------------------------------------------------------------//

  G4Sphere *outerVolumeSolid
    = new G4Sphere("outerVolumeSolid", 0.,  fWorldVolumeRadius, 
		   0.*degree, 360.0*degree, 0.0*degree, 360.0*degree);
    					//WORLD LOGICAL VOLUME
  theDetectorLogical
    = new G4LogicalVolume(outerVolumeSolid, AirMat, 
			  "outerVolumeLogical", 0, 0, 0, true);


  //----------------------------- CRYOSTAT ----------------------------------//
  //-------------------------------------------------------------------------//

  // place detector so top edge of cryostat is at origin:

  G4double zCoord =
  -MJGeometryKURFBEGeDetector::GetZCoordinateOfTopOfCryostat();

  G4ThreeVector kurfBEGePlacement(0, 0, zCoord ); 

  fCryostatPhysical 
    = new G4PVPlacement(0, kurfBEGePlacement, kurfBEGeDetecorLogical,
    "kurfBEGeCryostat", theDetectorLogical, false, 0);

  //----------------------------- COLLIMATOR --------------------------------//
  //-------------------------------------------------------------------------//
  // FIXME/TODO - no check to see if user supplied R,Z,Theta when user places
  // collimator on side.  Also, need to make it an option to make 
  // collimator face perp to cryostat or place it along some 3vector.
  G4bool isCollimatorUsed = MJGeometryKURFBEGeDetector::GetIsCollimatorUsed();
  if (isCollimatorUsed){
    G4Colour darkred (0.5,  0.0,  0.0); 
    G4Colour blueva (0.0,  0.0,  1.0);     
    G4VisAttributes* CollimatorVisAttr = new G4VisAttributes(darkred);
    G4VisAttributes* SourceDiskVisAttr = new G4VisAttributes(blueva);
    CollimatorVisAttr->SetVisibility(true);
    SourceDiskVisAttr->SetVisibility(true);
    
    // Solid Lead Box
    G4double collimatorBox_halfX = 4.0/2.0*inch;
    G4double collimatorBox_halfY = 4.0/2.0*inch;
    G4double collimatorBox_halfZ = 2.5/2.0*inch;
    G4Box* totalCollimatorBox
         = new G4Box("totalCollimatorBox",
                    collimatorBox_halfX,
                    collimatorBox_halfY,
                    collimatorBox_halfZ);
          
    // make solid for the smaller collimator hole
    G4double smaller_outerRadiusOfTheTube = 1.5/2.0*mm;
    G4double smaller_heightOfTheTube = 1.0/2.0*inch;
    G4Tubs* smaller_collimator_tube
        = new G4Tubs("smaller_collimator_tube",
                  0.0*mm,                       // inner radius
                  smaller_outerRadiusOfTheTube,
                  smaller_heightOfTheTube,
                  0.*deg,                       // start angle of tube
                  360.*deg);                    // spanning angle of tube
       
    // make solid for the larger collimator hole
    G4double larger_outerRadiusOfTheTube = 5.0/2.0*mm;
    G4double larger_heightOfTheTube = 1.0/2.0*inch;
    G4Tubs* larger_collimator_tube
        = new G4Tubs("larger_collimator_tube",
                  0.0*mm,                     // inner radius
                  larger_outerRadiusOfTheTube,
                  larger_heightOfTheTube,     
                  0.*deg,                     // start angle of tube
                  360.*deg);                  // spanning angle of tube

    // make solid for the source holder cylinder
    G4double source_outerRadiusOfTheTube = 27.0/2.0*mm;
    G4double source_heightOfTheTube = 3.175/2.0*mm;
    G4Tubs* source_holder_tube
        = new G4Tubs("source_holder_tube",
                   0.0*mm,                     // inner radius
                   source_outerRadiusOfTheTube,
                   source_heightOfTheTube,
                   0.*deg,                     // start angle of tube
                   360.*deg);                  // spanning angle of tube
  
    // now combine the larger and smaller collimators
    G4double z0 = larger_heightOfTheTube + smaller_heightOfTheTube;
    G4ThreeVector zTrans0(0.0*mm, 0.0*mm, z0);
    G4UnionSolid* collimator_tubes_solid1
        = new G4UnionSolid("collimator_tubes_solid1",
                          smaller_collimator_tube, 
                          larger_collimator_tube,
                          0, //no rotation
                          zTrans0); //only move z
                          
    // now combine the source holder and the previous boolean solid
    G4double z1 = smaller_heightOfTheTube + larger_heightOfTheTube*2.0 + source_heightOfTheTube;
    G4ThreeVector zTrans1(0.0*mm, 0.0*mm, z1);
    G4UnionSolid* collimator_tubes_solid
        = new G4UnionSolid("collimator_tubes_solid",
                          collimator_tubes_solid1,
                          source_holder_tube,
                          0, //no rotation
                          zTrans1); //only move z
                           
                        
    // now combine the collimator holes with the lead box
    // compute difference between the total hole height and 
    // the box height, then shift it in -z at that amount
    G4double z2 = collimatorBox_halfZ - smaller_heightOfTheTube;
    G4ThreeVector zTrans2(0.0*mm, 0.0*mm, -z2);
    G4SubtractionSolid* collimator 
        = new G4SubtractionSolid("collimator", 
                                totalCollimatorBox,
                                collimator_tubes_solid,
                                0, //no rotation
                                zTrans2);
  
    // Create a logical volume for the collimator out of Lead-Mod  
    G4LogicalVolume* collimator_log
         = new G4LogicalVolume(collimator,LeadMat,"collimator_log");
    collimator_log->SetVisAttributes(CollimatorVisAttr);
  
    // make solid for the actual source (3mm active diameter based on quote)
    G4double actual_source_outerRadiusOfTheTube = 3.0/2.0*mm;
    G4double actual_source_heightOfTheTube = 1.5875/2.0*mm;
    G4Tubs* actual_source
        = new G4Tubs("actual_source",
                   0.0*mm,                     // inner radius
                   actual_source_outerRadiusOfTheTube,
                   actual_source_heightOfTheTube,
                   0.*deg,                     // start angle of tube
                   360.*deg);                  // spanning angle of tube
  
    // Create a logical volume for the collimator out of Lead-Mod  
    G4LogicalVolume* actual_source_log
         = new G4LogicalVolume(actual_source,AirMat,"actual_source_log");
    actual_source_log->SetVisAttributes(SourceDiskVisAttr);
    
    G4String side_or_top = MJGeometryKURFBEGeDetector::GetCollimatorSideOrTop();
    
    // the user supplies an R distance (from the bottom of the collimator to the cryostat),
    // an angle theta at which the box is rotated, and a height Z, from this I compute where
    // to place the collimator and source using basic trig.
    if (side_or_top == "side") {
      G4double iR     = MJGeometryKURFBEGeDetector::GetCollimatorRCoordinateIn_mm()*mm;
      G4double iTheta = MJGeometryKURFBEGeDetector::GetCollimatorAngleIn_deg()*degree;
      G4double iZ     = MJGeometryKURFBEGeDetector::GetCollimatorZCoordinateIn_mm()*mm;
      
      // collimator placement for side:
      G4double cryostatRadius = 3.25/2.0 * inch;
      G4double rWithCryostatRadiusAndBoxSizeTakenIntoAccount = iR + cryostatRadius + collimatorBox_halfZ;

      // cylindrical coordinates
      G4ThreeVector rHatUnitVector(cos(iTheta/radian),sin(iTheta/radian),0);
      G4ThreeVector thetaHatUnitVector(-sin(iTheta/radian),cos(iTheta/radian),0);
      G4ThreeVector zHatUnitVector(0,0,1);
      
      G4ThreeVector vectorBoxPlacement;
      vectorBoxPlacement.setRhoPhiZ(rWithCryostatRadiusAndBoxSizeTakenIntoAccount,iTheta/radian,iZ);
            
            
      G4RotationMatrix* rot1 = new G4RotationMatrix();
      // the order of these rotations matters!
      rot1->rotate(twopi-halfpi ,thetaHatUnitVector);           // rotate around theta-hat
      rot1->rotate(twopi-iTheta/radian,zHatUnitVector);         // rotate around z-hat
      
      fCollimatorPhysical
           = new G4PVPlacement(rot1,                        // Rotate
                              vectorBoxPlacement,           // translation position
                              collimator_log,               // its logical volume
                              "collimator",                 // its name
                              theDetectorLogical,           // its mother (logical) volume
                              false,                        // no boolean operations
                              0);                           // its copy number
      
      // source placement for side (R of collimator + the distance from the center of the block to the source holder location):
      G4double srcR = rWithCryostatRadiusAndBoxSizeTakenIntoAccount + 
                      (source_heightOfTheTube + 2.0*larger_heightOfTheTube + 2.0*smaller_heightOfTheTube - collimatorBox_halfZ);
      
      G4ThreeVector vectorSrcPlacement;
      vectorSrcPlacement.setRhoPhiZ(srcR,iTheta/radian,iZ);
      
      fSourceDiskPhysical
           = new G4PVPlacement(rot1,                        // Rotate
                              vectorSrcPlacement,           // translation position
                              actual_source_log,            // its logical volume
                              "source_disk",                // its name
                              theDetectorLogical,           // its mother (logical) volume
                              false,                        // no boolean operations
                              0);                           // its copy number
    }
    else { // place it on the top
      G4double iX = MJGeometryKURFBEGeDetector::GetCollimatorXCoordinateIn_mm()*mm;
      G4double iY = MJGeometryKURFBEGeDetector::GetCollimatorYCoordinateIn_mm()*mm;
      G4double iZ = MJGeometryKURFBEGeDetector::GetCollimatorZCoordinateIn_mm()*mm;
      // collimator placement for top:
      iZ = iZ + collimatorBox_halfZ; // account for physical size of collimator so the distance they give us
                          // is the distance from the bottom of the collimator to the top of 
                          // the cryostat
      fCollimatorPhysical
           = new G4PVPlacement(0,                        // no rotation
                              G4ThreeVector(iX,iY,iZ), // translation position
                              collimator_log,            // its logical volume
                              "collimator",              // its name
                              theDetectorLogical,        // its mother (logical) volume
                              false,                     // no boolean operations
                              0);                        // its copy number
                              
      G4double srcZ = iZ + (source_heightOfTheTube + 2.0*larger_heightOfTheTube + 2.0*smaller_heightOfTheTube - collimatorBox_halfZ);
      fSourceDiskPhysical
           = new G4PVPlacement(0,                        // no rotation
                              G4ThreeVector(iX,iY,srcZ), // translation position
                              actual_source_log,            // its logical volume
                              "source_disk",              // its name
                              theDetectorLogical,        // its mother (logical) volume
                              false,                     // no boolean operations
                              0);                        // its copy number                        
                              
    }

  } // end collimator construction

  //*************************************************************************//
  //------------------------VISUALIZATION ATTRIBUTES-------------------------//
  //-------------------------------------------------------------------------//
/*  
  //G4VisAttributes* grey = new G4VisAttributes(G4Color(0.6,0.6,0.6,.5)); 
  G4VisAttributes* red = new G4VisAttributes(G4Color(1.0,0,0,.9));
  //G4VisAttributes* darkgrey = new G4VisAttributes(G4Color(0.9,0.9,0.9,.2));
  //G4VisAttributes* lightmagenta = new G4VisAttributes(G4Color(1,0,1,.4));
  //G4VisAttributes* magenta = new G4VisAttributes(G4Color(1,0,1));
  G4VisAttributes* blue = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* green = new G4VisAttributes(G4Colour(0.0,1,0.0));
  G4VisAttributes* yellow = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* cyan = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes* black = new G4VisAttributes(G4Colour(0.0,0.0,0.0));
*/

  theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
}


//---------------------------------------------------------------------------//
void MJGeometryKURFBEGeDetectorInRoom::SetDefaults()
{ 
  //WORLD VOLUME
  fWorldVolumeRadius = 10.0 * m;
}
//---------------------------------------------------------------------------//
