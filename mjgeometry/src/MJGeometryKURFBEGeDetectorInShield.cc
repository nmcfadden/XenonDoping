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
// CLASS IMPLEMENTATION:  MJGeometryKURFBEGeDetectorInShield.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: 
 */
//---------------------------------------------------------------------------//
/**
 * AUTHOR: P. Finnerty
 * CONTACT: P. Finnerty paddy@physics.unc.edu
 * FIRST SUBMISSION: 03-02-2009 Kevin Macon
 * 
 * REVISION: 
 *           03-11-2009 P. Finnerty - corrected inner cavity dimensions, and 
 * 		 		removed outer spherical shell, no need for it.
 *           05-20-2009 P. Finnerty - Added Polyethylene and Borated-Poly-10% 
 *                               to the shield geometry.
 *           05-26-2009 P. Finnerty - removed the inner inch of lead-mod and
 *                               replaced it with ancient lead.  you can now
 *                               sample the inner-most inch of lead with lead
 *                               210 and model bremmstr. radiation.
 *           05-27-2009 P. Finnerty - converted shield style to a russian doll
 *                               type of shield.  the cold finger was making
 *                               things difficult.  I also added the
 *                               scintillators to the geometry.  AND, I added
 *                               an inner 1" thick of sullivan lead, just
 *                               outside the ancient lead, so we can model the
 *                               effectiveness of the ancient lead on Pb210.
 *           01-26-2010 A. Schubert - split original MJGeometryKURFBEGeDetector
 *                               class into two.  This class contains the shield
 *                               and MJGeometryKURFBEGeDetector now contains
 *                               only the detector and cryostat.
 *           01-29-2010 A. Schubert - moved MJGeometryKURFBEGeDetector
 *                                construction to start of ConstructDetector()
 *                                to avoid errors.
 *           04-19-2010 A. Schubert - modified lead shielding dimensions
 *                                according to Gary Swift's shielding 
 *                                drawings, MDS*.
 *           04-22-2010 A. Schubert - renamed cryostatPhysical to
 *                                begeCryostatPhysical to avoid using same
 *                                volume name as MJGeometryKURFBEGeDetector.
 *           08-27-2010 A. Schubert - Added messenger argument to 
 *                                MJGeometryKURFBEGeDetector constructor.
 *            12-03-2010 Removed borated poly layer per P. Finnerty email from 19 July
 *                                2010, made scintillator into 1 m x 1m panel,
 *                                removed some old unused code, increased fEps
 *                                from 1 micron to 0.1 mm, A. Schubert 
 *           01-11-2011 A. Schubert - Added volume to subtract during visualization.
 *           03 Feb 2011 A. Schubert - Added rock.
 *           15 Aug 2011 Added rock sampling volume, A. Schubert
 *           30 Aug 2011 Added trailer, more realistic geometry for poly, A. Schubert
 *           16 Nov 2011 A. Schubert - Added parallel geometry
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
#include "mjgeometry/MJGeometryKURFBEGeDetectorInShield.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UnitsTable.hh"
#include <sstream>
#include "mjgeometry/MJGeometryKURFBEGeDetectorInShieldMessenger.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//--------------------------------------------------------------------------//

MJGeometryKURFBEGeDetectorInShieldParallelWorld::MJGeometryKURFBEGeDetectorInShieldParallelWorld(
) :
  MGGeometryParallelWorld(),
  fWorldVolume(NULL),
  fX(0.0),
  fY(0.0),
  fZ(0.0),
  fPbMinLength(0.0),
  fPbDeltaLength(0.0),
  fNPbVolumes(0),
  fPolyMinLength(0.0),
  fPolyDeltaLength(0.0),
  fNPolyVolumes(0)
{
}

//--------------------------------------------------------------------------//

void MJGeometryKURFBEGeDetectorInShieldParallelWorld::Construct()
{
  MGLog(routine) 
    << "constructing parallel geometry centered at "
    << "(x= " << G4BestUnit(fX, "Length")
    << ", y= " << G4BestUnit(fY, "Length")
    << ", z= " << G4BestUnit(fZ, "Length")
    << ")" << endlog;
  
  //G4VUserParallelWorld::GetWorld creates a clone of the mass world!
  fWorldVolume = GetWorld();



  G4VisAttributes* cyanva = new G4VisAttributes(G4Colour(0.58, 0.0, 0.82, 1.0));

  G4LogicalVolume* worldLogical = fWorldVolume->GetLogicalVolume();
  G4LogicalVolume* motherVolume = worldLogical;

  double polyOffsetZ = - 0.34*m;
   
  /* place nested volumes in and around the poly */

  for(int i = 0; i < fNPolyVolumes; i++) {

    // construct solid
    ostringstream parallelBoxName;
    parallelBoxName << "parallelPolyBox" << i;

    // the first solid is the largest, since later solids must be placed within
    double halfLength = fPolyMinLength + fPolyDeltaLength * (fNPolyVolumes - 1 -i);
    
    G4Box* parallelSolid = new G4Box(parallelBoxName.str(), 
        halfLength, halfLength, halfLength + 0.45*m);

    // create logical volume
    ostringstream parallelLogicalName;
    parallelLogicalName << "parallelPolyLogical" << i;

    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0,
        parallelLogicalName.str(), 0, 0, 0);

    parallelLogical->SetVisAttributes(cyanva);

    // place physical volume centered around detector
    ostringstream parallelPhysicalName;
    parallelPhysicalName << "parallelPolyPhysical" << i;

    double x = (i == 0) ? fX : 0.0;
    double y = (i == 0) ? fY : 0.0;
    double z = (i == 0) ? fZ + polyOffsetZ: 0.0;

    G4ThreeVector origin(x, y, z);

    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
        origin, parallelLogical, parallelPhysicalName.str(),
        motherVolume, 0, 0); 

    // set volume importance as 2^i
    SetImportanceValueForRegion(physicalVolume, pow(2.0, i + 1.0) );

    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;

  } // end loop over shield volumes


  /* place intermediate volume, intersecting with muon veto panel */ 

  // construct solid
  double halfIntLength = 41.0*cm;
  G4Box* parallelSolid = new G4Box("parallelIntermediateBox", 
      halfIntLength, halfIntLength, halfIntLength);

  // create logical volume
  G4LogicalVolume* parallelIntLogical = new G4LogicalVolume(parallelSolid, 0,
      "parallelIntLogical", 0, 0, 0);

  parallelIntLogical->SetVisAttributes(cyanva);

  // place physical volume centered around detector
  G4VPhysicalVolume* physicalIntVolume = new G4PVPlacement(0,
      G4ThreeVector(0, 0, -polyOffsetZ - 5.0*cm),
      parallelIntLogical, "parallelIntPhysical",
      motherVolume, 0, 0); 

  // set volume importance as 2^x
  SetImportanceValueForRegion(physicalIntVolume, pow(2.0, fNPolyVolumes + 1.0) );

  // set motherVolume to parallelIntLogical for the next placement
  motherVolume = parallelIntLogical;


  /* place nested volumes in and around the Pb shield */

  for(int i = 0; i < fNPbVolumes; i++) {

    // construct solid
    ostringstream parallelBoxName;
    parallelBoxName << "parallelPbBox" << i;

    // the first solid is the largest, since later solids must be placed within
    double halfLength = fPbMinLength + fPbDeltaLength * (fNPbVolumes - 1 -i);
    
    parallelSolid = new G4Box(parallelBoxName.str(), 
        halfLength, halfLength, halfLength + 4.0*cm);

    // create logical volume
    ostringstream parallelLogicalName;
    parallelLogicalName << "parallelPbLogical" << i;

    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0,
        parallelLogicalName.str(), 0, 0, 0);

    parallelLogical->SetVisAttributes(cyanva);

    // place physical volume centered around detector
    ostringstream parallelPhysicalName;
    parallelPhysicalName << "parallelPbPhysical" << i;

    G4ThreeVector origin(0.0, 0.0, 0.0);

    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
        origin, parallelLogical, parallelPhysicalName.str(),
        motherVolume, 0, 0); 

    // set volume importance as 2^i
    SetImportanceValueForRegion(physicalVolume, pow(2.0, i + 1.0 + fNPolyVolumes+1) );

    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;

  } // end loop over shield volumes

  MGLog(routine) 
    << "There are " << fNPbVolumes << " Pb parallel volumes with "
    << fPbDeltaLength/cm << "-cm spacing; the first volume has width "
    << fPbMinLength/cm << " cm." << endlog; 
  
  MGLog(routine) 
    << "There are " << fNPolyVolumes << " poly parallel volumes with "
    << fPolyDeltaLength/cm << "-cm spacing; the first volume has width " 
    << fPolyMinLength/cm << " cm" << endlog; 

}

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//

MJGeometryKURFBEGeDetectorInShield::MJGeometryKURFBEGeDetectorInShield(
  G4String serNum,
  G4bool messOff
) :
  MJGeometryKURFBEGeDetector(serNum, false), 
  fMessenger(NULL),
  fIsPolyPresent(true),
  fIsDetectorInShield(true),
  fParallelWorld(NULL),
  fRockLogical(0),
  fRockSamplingLogical(0),
  fExperimentalHallLogical(0), 
  fTrailerLogical(0),
  fTrailerAirSpaceLogical(0),
  fLeadShieldLogical(0), 
  fPolyLogical(0), 
  fPolyTopLogical(0), 
  fInnerAinLeadLogical(0),
  fScintillatorLogical(0),
  fRockPhysical(0),
  fRockSamplingPhysical(0),
  fExperimentalHallPhysical(0), 
  fTrailerPhysical(0),
  fTrailerAirSpacePhysical(0),
  fLeadShieldPhysical(0), 
  fCryostatPhysical(0),
  fPolyPhysical(0),
  fPolyTopPhysical(0), 
  fInnerAinLeadPhysical(0), 
  fScintillatorPhysical(0)

{
  SetDefaults();

  if(!messOff) {
    fMessenger = new MJGeometryKURFBEGeDetectorInShieldMessenger(this);
  } 

}

//---------------------------------------------------------------------------//

MJGeometryKURFBEGeDetectorInShield::~MJGeometryKURFBEGeDetectorInShield()
{
  delete fMessenger;
}

//---------------------------------------------------------------------------//

MGGeometryParallelWorld* MJGeometryKURFBEGeDetectorInShield::GimmeParallelWorld()
{ 
  if (fParallelWorld) {
    
    MGLog(warning) << "parallel world already exists!" << endlog;

  } else {
    
    MGLog(routine) << "creating parallel world" << endlog;

    fParallelWorld = new MJGeometryKURFBEGeDetectorInShieldParallelWorld(); 

  }

  return fParallelWorld;
}

//---------------------------------------------------------------------------//

void MJGeometryKURFBEGeDetectorInShield::ConstructDetector()
{

  // first create the detector:
  MJGeometryKURFBEGeDetector::ConstructDetector();

  MGLog(routine) << "fEps = " << G4BestUnit(fEps, "Length") << endlog;

  G4LogicalVolume* kurfBEGeDetecorLogical =
  MJGeometryKURFBEGeDetector::GetDetectorLogical();


  //World Materials
  G4Material *Vacuum   = G4Material::GetMaterial("Vacuum");
  G4Material *AirMat   = G4Material::GetMaterial("Air");

  //Shield Materials
  G4Material *AncientLead = G4Material::GetMaterial("Lead-Ain");
  G4Material *PolyMat = G4Material::GetMaterial("Polyethylene");
  G4Material *LeadMat  = G4Material::GetMaterial("Lead-Mod");
  G4Material *ScintMat = G4Material::GetMaterial("Scintillator-Pla");
  G4Material *Rock = G4Material::GetMaterial("Rock");
  // not sure of correct steel for trailer; it's probably not stainless, though -- A.G. Schubert 30 Aug 2011
  G4Material *Steel = G4Material::GetMaterial("StainlessSteel304");
  G4Material *Aluminum = G4Material::GetMaterial("Aluminum");

  // FIXME -- for testing only:
  //G4Material *AirMat   = G4Material::GetMaterial("Vacuum");
  //G4Material *AncientLead = G4Material::GetMaterial("Vacuum");
  //G4Material *PolyMat = G4Material::GetMaterial("Vacuum");
  //G4Material *LeadMat  = G4Material::GetMaterial("Vacuum");
  //G4Material *ScintMat = G4Material::GetMaterial("Vacuum");
  //G4Material *Rock = G4Material::GetMaterial("Vacuum");
  //G4Material *Steel = G4Material::GetMaterial("Vacuum");
  
  if(!( AirMat || LeadMat || Vacuum || Steel || 
        AncientLead || PolyMat || ScintMat || Rock || Aluminum)) {

    MGLog(error) << "NULL G4Material pointer!  Cannot find material!" << endlog;
    MGLog(fatal) << endlog;
  }

  //--------------------------------------------------------------------
  // placement
  //--------------------------------------------------------------------

  // dimensions of the inside of the MALBEK detector trailer:
  double trailerInnerHalfLength = 121.0/2.0*inch;
  double trailerInnerHalfWidth = 92.0/2.0*inch;
  double trailerInnerHalfHeight = 96.0/2.0*inch;
  double trailerWallThickness = 0.075*inch;

  // location of the trailer
  // +x direction is toward door of MALBEK detector trailer (away from office
  // trailer)
  // +y direction is away from DAQ trailer 
  double trailerX = -fHallLength + trailerInnerHalfLength + 16.5*12.0*inch; 
  double trailerY = -fHallWidth + trailerInnerHalfWidth + 13.0*12.0*inch;
  double trailerZ = -fHallHeight + trailerInnerHalfHeight + 8.0*inch; 


  // per KURF measurements on 17 Aug 2011:
  G4double leadShieldX = - trailerInnerHalfLength + fLeadShieldWidth +
      47.0*inch;
  G4double leadShieldY = - trailerInnerHalfWidth + fLeadShieldWidth +
      38.5*inch;
  G4double leadShieldZ = - trailerInnerHalfHeight + 57.0*inch +
      fLeadShieldHeight; 

  
  //------------- A VOLUME TO SUBTRACT FOR VISUALIZATION --------------------//
  // This volume is empty between y = +/- 0.11 inches.  When subtracted from a
  // piece of shielding, it will leave a slice of the shield. This is meant to
  // be used for visualization only.  The volume is subtracted from shielding
  // layers if MJGeometryKURFBEGeDetector's GetCylinderAngleInUnitsOfPi() is
  // less than 1.5. 
  //-------------------------------------------------------------------------//

  G4VSolid* subtractionBox = NULL;

  if ( MJGeometryKURFBEGeDetector::GetCylinderAngleInUnitsOfPi() < 1.5 ) { 

    MGLog(routine) << "****** CREATING A BOX TO USE FOR SUBTRACTION" << endlog;

    double subtractionBoxLength = 1.0*km;

    // a 1-km cube:
    subtractionBox 
      = new G4Box("subtractionBox", subtractionBoxLength, subtractionBoxLength,
                  subtractionBoxLength); 

    // a 1-inch x 1-km x 1-km box to subtract from the cube
    G4Box* boxToMakeSlice = new G4Box(
      "boxToMakeSlice", 
      subtractionBoxLength - 1*inch, 
      0.1*inch, // to make a slice at y=0
      subtractionBoxLength - 1*inch 
    );

    subtractionBox = new G4SubtractionSolid(
      "subtractionBoxWithSlice",
      subtractionBox,
      boxToMakeSlice,
      0,
      G4ThreeVector(0) 
    );

  }

  //----------------------------- WORLD VOLUME ------------------------------//
  //-------------------------------------------------------------------------//
  G4Sphere *outerVolumeSolid
    = new G4Sphere("outerVolumeSolid", 0.,  fWorldVolumeRadius, 
		   0.*degree, 360.0*degree, 0.0*degree, 180.0*degree);
    					//WORLD LOGICAL VOLUME
  theDetectorLogical
    = new G4LogicalVolume(outerVolumeSolid, AirMat, 
			  "outerVolumeLogical", 0, 0, 0, true);

  // ----------------- ROCK -------------------------------------------------//

  
  G4VSolid *rockBox 
    = new G4Box("rock",  fHallLength + 20.0*m,
                    fHallWidth + 20*m,
                    fHallHeight + 20*m);

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    rockBox = new G4SubtractionSolid(
      "rockBoxSubtracted", 
      rockBox, 
      subtractionBox, 
      0,
      G4ThreeVector(trailerX + leadShieldX, trailerY + leadShieldY, 0)
    );
  }

  fRockLogical
    = new G4LogicalVolume(rockBox, Rock, "rockLogical", 0, 0, 0, true);

  fRockPhysical 
    = new G4PVPlacement(  0, G4ThreeVector(0), fRockLogical, "rockPhysical",
                          theDetectorLogical, false, 0);


  // create a rock volume in which decays can be simulated:
  double rockSamplingThickness = 0.1*m;

  G4VSolid *rockSamplingBox 
    = new G4Box("rockSampling",  
                    fHallLength + rockSamplingThickness,
                    fHallWidth + rockSamplingThickness,
                    fHallHeight + rockSamplingThickness + 2.0*fEps);

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    rockSamplingBox = new G4SubtractionSolid(
      "rockSamplingBoxSubtracted", 
      rockSamplingBox, 
      subtractionBox, 
      0,
      G4ThreeVector(trailerX + leadShieldX, trailerY + leadShieldY, 0)
    );
  }

  fRockSamplingLogical = new G4LogicalVolume(rockSamplingBox, Rock,
      "rockSamplingLogical", 0, 0, 0, true);

  fRockSamplingPhysical = new G4PVPlacement(  0, G4ThreeVector(0),
      fRockSamplingLogical, "rockSampling", fRockLogical, false, 0);

 
  //----------------------------- Experimental Hall VOLUME ------------------//  
  //This is the experimental hall; it is placed in the rock sampling volume.
  //-------------------------------------------------------------------------//
  G4VSolid *outsideTestSolid
    = new G4Box("outsideTestSolid", fHallLength, 
                                    fHallWidth, 
                                    fHallHeight);

  // Make a hole in the sampled volume -- underneath the trailer and shield,
  // below the entry point of the vertical dipstick.  This allows separate
  // sampling for gammas from the rock that must penetrate the lead and gammas
  // that make it through the crack around the dipstick.

  double rockHoleHalfLength = 2.0*m;

  G4ThreeVector rockHolePos(
      trailerX + leadShieldX, 
      trailerY + leadShieldY,
      -fHallHeight - rockSamplingThickness/2.0
  );

  G4VSolid *rockHole 
    = new G4Box("rockHole",  
                    rockHoleHalfLength + fEps,
                    rockHoleHalfLength + fEps,
                    rockSamplingThickness/2.0 + fEps);

  // make a bump out into the rock to encapsulate the smallRockSampling volume
  outsideTestSolid = new G4UnionSolid(
    "rockSamplingBoxWithHold", 
    outsideTestSolid, 
    rockHole, 
    0,
    rockHolePos
  );

    					//LOGICAL VOLUME
  fExperimentalHallLogical
    = new G4LogicalVolume(outsideTestSolid, AirMat, 
			  "outsideTestLogical", 0, 0, 0, true);
  					//PLACEMENT			  
  fExperimentalHallPhysical
    = new G4PVPlacement(0, G4ThreeVector(0), fExperimentalHallLogical, 
			"outsideTestPhysical", fRockSamplingLogical, false, 0);
 


  //--------- Rock under trailer --------------------------------------------//
  // small volume of rock directly below Pb shield:
  //-------------------------------------------------------------------------//

  G4VSolid *smallRockSamplingBox
    = new G4Box("smallRockSamplingBox",  
                    rockHoleHalfLength - fEps,
                    rockHoleHalfLength - fEps,
                    rockSamplingThickness/2.0 - fEps);


  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    smallRockSamplingBox = new G4SubtractionSolid(
      "smallRockSamplingBoxSubtracted", 
      smallRockSamplingBox, 
      subtractionBox, 
      0,
      G4ThreeVector(0, 0, 0)
    );
  }

  G4LogicalVolume* smallRockSamplingLogical = new G4LogicalVolume(smallRockSamplingBox, Rock,
      "smallRockSamplingLogical", 0, 0, 0, true);

  new G4PVPlacement(0, rockHolePos, smallRockSamplingLogical,
      "smallRockSampling", fExperimentalHallLogical, false, 0);



  //----------------------------- Trailer  ----------------------------------//  
  //This is the malbek detector trailer; it is placed in the experimental hall
  //-------------------------------------------------------------------------//

  G4VSolid *trailerSolid
    = new G4Box("trailerSolid", trailerInnerHalfLength + trailerWallThickness, 
                                trailerInnerHalfWidth + trailerWallThickness, 
                                trailerInnerHalfHeight + trailerWallThickness);

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    trailerSolid = new G4SubtractionSolid(
      "trailerSolidSubtracted", 
      trailerSolid, 
      subtractionBox, 
      0,
      G4ThreeVector(leadShieldX, leadShieldY, 0)
    );
  }

    					//LOGICAL VOLUME
  fTrailerLogical
    = new G4LogicalVolume(trailerSolid, Steel, 
			  "trailerLogical", 0, 0, 0, true);
  					//PLACEMENT			  
  
  G4ThreeVector trailerPosition(trailerX, trailerY, trailerZ);

  fTrailerPhysical
    = new G4PVPlacement(0, trailerPosition, fTrailerLogical, 
                        "trailerPhysical", fExperimentalHallLogical, false, 0);


  //----------------------- Trailer  air space ------------------------------//  
  //This is the air space in the malbek detector trailer; it is placed in the
  //trailer solid.
  //-------------------------------------------------------------------------//

  G4VSolid *trailerAirSpaceSolid
    = new G4Box("trailerAirSpace", trailerInnerHalfLength, 
                                   trailerInnerHalfWidth, 
                                   trailerInnerHalfHeight);

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    trailerAirSpaceSolid = new G4SubtractionSolid(
      "trailerAirSpaceSolidSubtracted", 
      trailerAirSpaceSolid, 
      subtractionBox, 
      0,
      G4ThreeVector(leadShieldX, leadShieldY, 0)
    );
  }

    					//LOGICAL VOLUME
  fTrailerAirSpaceLogical
    = new G4LogicalVolume(trailerAirSpaceSolid, AirMat, 
			  "trailerAirSpaceLogical", 0, 0, 0, true);
  					//PLACEMENT			  
  fTrailerAirSpacePhysical
    = new G4PVPlacement(0, G4ThreeVector(0), fTrailerAirSpaceLogical, 
			"trailerAirSpace", fTrailerLogical, false, 0);


  //----------------------------- POLY SHIELD ------------------------------//  

  
  // half width:
  G4double polyCavityWidth = 49.0/2.0 * inch;
  G4double polyWidth = polyCavityWidth + fPolyThickness; 
  G4double polyHeight = trailerInnerHalfHeight - 10.0*inch / 2.0;

  G4VSolid *polySolid
    = new G4Box("polySolid",  polyWidth,
                              polyWidth,
                              polyHeight);

  G4VSolid* polyCavity
    = new G4Box("polyCavity", polyCavityWidth,
                              polyCavityWidth,
                              polyHeight);

  polySolid = new G4SubtractionSolid(
    "polySolidWithCavity",
    polySolid,
    polyCavity,
    0,
    G4ThreeVector(0, 0, fPolyThickness)
  );


  G4VSolid* polyTopSolid              
    = new G4Box("polyTopSolid",  polyWidth,
                                 polyWidth,
                                 fPolyThickness / 2.0 );


  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {

    polySolid = new G4SubtractionSolid(
      "polySolidSubtracted", 
      polySolid, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );
    
    polyTopSolid = new G4SubtractionSolid(
      "polySolidSubtracted", 
      polyTopSolid, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );

  }

                                        //LOGICAL VOLUME
  fPolyLogical
    = new G4LogicalVolume(polySolid, PolyMat, 
                          "polyLogical", 0, 0, 0, true);

  fPolyTopLogical
    = new G4LogicalVolume(polyTopSolid, PolyMat, 
                          "polyTopLogical", 0, 0, 0, true);

  if ( fIsPolyPresent ) {

    MGLog(routine) << "poly shield *IS* present!" << endlog;

    G4double polyZ = - trailerInnerHalfHeight + polyHeight + fEps;

    G4double polyTopX = trailerX + leadShieldX;
    G4double polyTopY = trailerY + leadShieldY;
    G4double polyTopZ = trailerZ + trailerInnerHalfHeight + trailerWallThickness
    + fPolyThickness/2.0 + fEps; 

    G4ThreeVector polyPosition(leadShieldX, leadShieldY, polyZ);
    G4ThreeVector polyTopPosition(polyTopX, polyTopY, polyTopZ);

                                          //PLACEMENT			  
    fPolyTopPhysical
      = new G4PVPlacement(0, polyTopPosition, fPolyTopLogical, 
                          "polyTopPhysical", fExperimentalHallLogical, false, 0);

    fPolyPhysical
      = new G4PVPlacement(0, polyPosition, fPolyLogical, 
                          "polyPhysical", fTrailerAirSpaceLogical, false, 0);

  } else {
    MGLog(routine) << "poly shield is *NOT* present!" << endlog;
  }


  //----------------------------- Outer LEAD SHIELD VOLUME ------------------//  
  //-------------------------------------------------------------------------//
  //build lead shield and remove volumes to place cryostat and cold finger
  G4Box *leadShieldSolid
    = new G4Box("leadShieldBox", fLeadShieldWidth,
                                 fLeadShieldWidth, 
                                 fLeadShieldHeight);
 

  // space inside the shield:
  G4Box *leadShieldCavity 
    = new G4Box("leadShieldCavity", fLeadShieldCavityWidth,
                                    fLeadShieldCavityWidth,
                                    fLeadShieldCavityLength*2.0);

  // wider space at bottom of shield:
  G4Box *leadShieldWiderCavity
    = new G4Box("leadShieldCavity", fLeadShieldWiderCavityWidth,
                                    fLeadShieldWiderCavityWidth,
                                    fLeadShieldWiderCavityLength*2.0);

  G4ThreeVector leadShieldCavityPlacement(0, 0, -fLeadShieldHeight);

  // subtract the longer, narrower part of the hole from leadShieldSolid:
  G4SubtractionSolid *leadShield 
    = new G4SubtractionSolid("leadShield0", leadShieldSolid, leadShieldCavity, 0,
                             leadShieldCavityPlacement);


  // subtract the shorter, wider part of the hole:
  leadShield 
    = new G4SubtractionSolid("leadShield1", leadShield, leadShieldWiderCavity, 0,
                             leadShieldCavityPlacement);

  // make two parts to form a solid to subtract for the waist:
  // first make the solid -- make the width larger than needed to avoid
  // intersecting surfaces during subtraction:
  G4Box* leadShieldWaistSolid
    = new G4Box("leadShieldCavity", fLeadShieldWidth*2.0,
                                    fLeadShieldWidth*2.0,
                                    fLeadShieldWaistLength);

  // make a part to subtract from the solid:
  G4Box* leadShieldWaistCavity
    = new G4Box("leadShieldCavity", fLeadShieldWaistWidth,
                                    fLeadShieldWaistWidth,
                                    fLeadShieldWaistLength*2.0);

  // combine the waist and waist cavity to form a part to use for later
  // subtraction:
  G4SubtractionSolid *leadShieldWaist 
    = new G4SubtractionSolid("leadShieldWaist", leadShieldWaistSolid,
    leadShieldWaistCavity, 0, G4ThreeVector());

  G4double leadShieldWaistZ = - fLeadShieldHeight +
  fLeadShieldWiderCavityLength*2.0 + fLeadShieldWaistLength; 

  G4ThreeVector leadShieldWaistPlacement(0, 0, leadShieldWaistZ);

  // subtract the waist from the shield:
  leadShield 
    = new G4SubtractionSolid("leadShield2", leadShield, leadShieldWaist, 0,
                             leadShieldWaistPlacement);

  // there are two layers of normal lead around the cold finger and underneath
  // the cryostat:
  G4Box *leadShieldUnderCryostatSolid
    = new G4Box("leadShieldUnderCryostatSolid", fAncLeadWidth,
                                           fAncLeadWidth,
                                           fLeadShieldUnderCryostatHeight);

  // make a small part to connect the lead under the cryostat to the rest of the
  // normal lead -- this part doesn't exist in the real geometry -- it is added
  // so the two sections of normal lead can be represented as one solid:
  G4Box *leadShieldConnector
    = new G4Box("leadShieldConnector", 5.0*inch,
                                       0.25*inch,
                                       0.25*inch);

  G4ThreeVector leadShieldConnectorPlacement( -fAncLeadWidth, 0, 0);

  G4UnionSolid *leadShieldUnderCryostat 
    = new G4UnionSolid("leadShieldUnderCryostat", leadShieldUnderCryostatSolid,
    leadShieldConnector, 0, leadShieldConnectorPlacement);
 
  G4ThreeVector leadShieldUnderCryostatPlacement(0, 0, -fLeadShieldHeight +
  fLeadShieldUnderCryostatHeight - fAncLeadZOffset);  

  G4UnionSolid *leadShieldWithBase
    = new G4UnionSolid("leadShieldWithBase", leadShield,
    leadShieldUnderCryostat, 0, leadShieldUnderCryostatPlacement ); 

  // cylinder to form hole around cryostat:
  G4Tubs *ancLeadHole
    = new G4Tubs("ancLeadHole", 0, fAncLeadHoleRadius, fAncLeadHeight, 0,
                 twopi);

  G4ThreeVector coldFingerHolePlacement(0, 0, -fLeadShieldHeight );

  // subtract the hole from the shield:
  leadShield 
    = new G4SubtractionSolid("leadShield3", leadShieldWithBase, ancLeadHole, 0,
                             coldFingerHolePlacement);

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    leadShield = new G4SubtractionSolid(
      "leadShieldSubtracted", 
      leadShield, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );
  }

  fLeadShieldLogical
    = new G4LogicalVolume(leadShield, LeadMat, 
			  "leadShieldLogical", 0, 0, 0, true);
  					//PLACEMENT			  

  G4ThreeVector leadShieldPlacement(leadShieldX, leadShieldY, leadShieldZ);


  // make a volume for sampling the lead, one inch thick
  G4VSolid* leadShieldSamplingBox
    = new G4Box("leadShieldSamplingBox",
                            fLeadShieldCavityWidth + 1.0*inch,
                            fLeadShieldCavityWidth + 1.0*inch,
                            fLeadShieldWaistLength + 0.5*inch + fEps);

   G4VSolid* leadShieldSamplingBoxCavity
    = new G4Box("leadShieldSamplingBoxCavity",
                            fLeadShieldCavityWidth + fEps*2.0,
                            fLeadShieldCavityWidth + fEps*2.0,
                            fLeadShieldWaistLength + 0.5*inch + fEps);
    
  leadShieldSamplingBox = new G4SubtractionSolid(
      "leadShieldSamplingBoxWithHole",
      leadShieldSamplingBox,
      leadShieldSamplingBoxCavity,
      0,
      G4ThreeVector(0, 0, -1.0*inch)
  );

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    leadShieldSamplingBox = new G4SubtractionSolid(
      "leadShieldSamplingBoxSubtracted", 
      leadShieldSamplingBox, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );
  }

  G4LogicalVolume* leadShieldSamplingLogical = new G4LogicalVolume(
      leadShieldSamplingBox, LeadMat, "leadShieldSamplingLogical", 0, 0, 0, true);




  //------------------------ Air sampling volume ----------------------------//  
  //
  //-------------------------------------------------------------------------//


  G4VSolid *trailerAirSpaceSamplingBox
    = new G4Box("trailerAirSpaceSamplingBox", 
                          fLeadShieldWidth + fEps, 
                          fLeadShieldWidth + fEps, 
                          fLeadShieldHeight + 2.0*inch);

  // an addition to cover the protruding cold finger:
  G4VSolid *trailerAirSpaceSamplingBoxAddition
    = new G4Box("trailerAirSpaceSamplingBoxAddition", 
                          4.0*inch, 
                          4.0*inch, 
                          4.0*inch);


  trailerAirSpaceSamplingBox 
    = new G4UnionSolid("trailerAirSpaceSamplingBoxWithAddition",
            trailerAirSpaceSamplingBox,
            trailerAirSpaceSamplingBoxAddition, 0, 
            G4ThreeVector(0, 0, -fLeadShieldHeight));

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    trailerAirSpaceSamplingBox = new G4SubtractionSolid(
      "trailerAirSpaceSamplingBoxSubtracted", 
      trailerAirSpaceSamplingBox, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );
  }

    					//LOGICAL VOLUME
  G4LogicalVolume* trailerAirSpaceSamplingLogical 
    = new G4LogicalVolume(trailerAirSpaceSamplingBox, AirMat, 
			  "trailerAirSpaceSamplingLogical", 0, 0, 0, true);
  					//PLACEMENT			  
  new G4PVPlacement(0, leadShieldPlacement, trailerAirSpaceSamplingLogical, 
			"trailerAirSpaceSampling", fTrailerAirSpaceLogical, false, 0);

  fLeadShieldPhysical
    = new G4PVPlacement(0, G4ThreeVector(0), fLeadShieldLogical, 
			"leadShieldPhysical", trailerAirSpaceSamplingLogical, false, 0);
 
  // place the lead shield sampling volume
  new G4PVPlacement(0, 
      G4ThreeVector(0, 0, leadShieldWaistZ + 0.5*inch + fEps), 
      leadShieldSamplingLogical, "leadShieldSampling", fLeadShieldLogical, false, 0);
 
  //----------------------------- Scintillator Panels------------------------//  
  //
  //-------------------------------------------------------------------------//
  G4VSolid *scintillatorSolid
    = new G4Box("scintillatorSolid", fScintillatorWidth,
                                     fScintillatorWidth,
                                     fScintillatorThickness);
    					
  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    scintillatorSolid = new G4SubtractionSolid(
      "scintillatorSolidSubtracted", 
      scintillatorSolid, 
      subtractionBox, 
      0,
      G4ThreeVector(0,0,0)
    );
  }

  fScintillatorLogical
    = new G4LogicalVolume(scintillatorSolid, ScintMat, 
			  "scintillatorLogical", 0, 0, 0, true);

  G4ThreeVector scintillatorPanelPlacement(leadShieldX, leadShieldY, leadShieldZ
    + fRadonExclusionBoxHeight + fScintillatorThickness + fEps);

  fScintillatorPhysical
    = new G4PVPlacement(0, scintillatorPanelPlacement, fScintillatorLogical, 
        "scintillatorPhysical", fTrailerAirSpaceLogical, false, 0);


  //----------------------------- Ancient Lead ------------------------------//
  //This box sits inside the lead shield cut out and is offset by 1 micron.
  //I can sample this surface for lead210 if I want to.
  // heights and widths are half lengths
  //-------------------------------------------------------------------------//

  // box with the ancient lead's largest dimensions:
  G4Box *ancLeadBoxSolid 
    = new G4Box("ancLeadBoxSolid", fAncLeadWidth,
                                   fAncLeadWidth,
                                   fAncLeadHeight);


  // temporary part to use for subtraction:
  G4Box *ancLeadTempPart
    = new G4Box("ancLeadTempPart", fAncLeadWidth*2.0,
                                   fAncLeadWidth*2.0,
                                   fAncLeadNarrowLength*2.0);

  // subtract this solid from the temporary solid:
  // make this part extra long to avoid intersecting surfaces:
  G4Box *ancLeadTempPartHole
    = new G4Box("ancLeadTempPartHole", fAncLeadNarrowWidth,
                                   fAncLeadNarrowWidth,
                                   fAncLeadNarrowLength*4.0);


  // perform subtraction from temp solid to make a square tube:
  G4SubtractionSolid *ancLeadTemp
    = new G4SubtractionSolid("ancLeadTemp", ancLeadTempPart, ancLeadTempPartHole, 0,
                             G4ThreeVector());

  // now the temporary solid is created and can be used to make a step in the
  // ancient lead shield:
  G4ThreeVector ancLeadTempSubtractionPlacement(0, 0, fAncLeadHeight);

  // perform subtraction from ancient lead solid:
  G4SubtractionSolid *ancLead
    = new G4SubtractionSolid("ancLead0", ancLeadBoxSolid, ancLeadTemp, 0,
                             ancLeadTempSubtractionPlacement);

  // space inside the shield:
  G4Box *ancLeadShieldCavity 
    = new G4Box("ancLeadShieldCavity", fAncLeadCavityWidth,
                                       fAncLeadCavityWidth,
                                       fAncLeadCavityHeight);

  G4ThreeVector ancLeadCavityPlacement(0, 0, fAncLeadHeight -
                                       fAncLeadCavityOffsetFromTop -
                                       fAncLeadCavityHeight);


  // subtract the cavity:
  ancLead 
    = new G4SubtractionSolid("ancLead1", ancLead, ancLeadShieldCavity, 0,
                             ancLeadCavityPlacement);

  G4ThreeVector ancLeadHolePlacement(0, 0, -fAncLeadHeight);

  ancLead 
    = new G4SubtractionSolid("ancLead2", ancLead, ancLeadHole, 0, 
                             ancLeadHolePlacement);

  G4double ancLeadZ = -fLeadShieldHeight + fAncLeadHeight +
  fLeadShieldUnderCryostatHeight*2.0 + fEps - fAncLeadZOffset;

  MGLog(routine) << "ancLeadZ: " << ancLeadZ/cm << " cm" << endlog;
  MGLog(routine) << "top of anc lead: " << (ancLeadZ + fAncLeadHeight)/cm << " cm" << endlog;
  MGLog(routine) << "bot of anc lead: " << (ancLeadZ - fAncLeadHeight)/cm << " cm" << endlog;

  // cut away part of the solid in case of visualization:
  if ( subtractionBox ) {
    ancLead = new G4SubtractionSolid(
      "ancLeadShieldSubtracted", 
      ancLead, 
      subtractionBox, 
      0,
      G4ThreeVector(0)
    );
  }

  fInnerAinLeadLogical 
    = new G4LogicalVolume(ancLead, AncientLead, "innerAinLeadLogical", 0, 0, 0, true);

  fInnerAinLeadPhysical 
    = new G4PVPlacement(0, G4ThreeVector(0, 0, ancLeadZ), fInnerAinLeadLogical, 
                       "innerAinLeadPhysical", trailerAirSpaceSamplingLogical, false, 0);


  //----------------------------- CRYOSTAT ----------------------------------//
  //This is placed inside the inner volume logical, not the lead shield logical.
  //-------------------------------------------------------------------------//


  G4double cryostatStandPlateZ =
    MJGeometryKURFBEGeDetector::GetZCoordinateOfTopOfStandTopPlate();

  G4double begeX = 0.0;
  G4double begeY = 0.0;

  G4double begeZ = - fLeadShieldHeight - fAncLeadZOffset -
  cryostatStandPlateZ - fEps; 

  if ( fIsDetectorInShield ) {
    MGLog(routine) << "detector is *INSIDE* shield" << endlog;

  } else {

    /* put the detector in position for source tests on 16 and 17 Aug 2011 */

    MGLog(routine) << "detector is *OUTSIDE* shield" << endlog;

    begeX = trailerInnerHalfLength - 35.0*inch; 
    begeY = trailerInnerHalfWidth - 38.5*inch; 
    begeZ = - trailerInnerHalfHeight + 39.5*inch; 

  }

  G4ThreeVector cryostatPlacement(begeX, begeY, begeZ);

  MGLog(routine) 
    << "distance from top of Al stand to floor of trailer:" 
    << G4BestUnit( begeZ + cryostatStandPlateZ, "Length") 
    << endlog;

  MGLog(routine) 
    << "cryostat coords within trailer: (x=" 
    << G4BestUnit( begeX, "Length")
    << ", y=" 
    << G4BestUnit( begeY, "Length")
    << endlog;

  double topOfCryostatInBEGeDetectorCoords = 
      MJGeometryKURFBEGeDetector::GetZCoordinateOfTopOfCryostat();

  double cryostatX = begeX + trailerX;
  double cryostatY = begeY + trailerY;
  double cryostatZ = topOfCryostatInBEGeDetectorCoords + begeZ + trailerZ;

  if ( fParallelWorld ) {
    // set the origin of parallel volumes to the top of the cryostat
    fParallelWorld->SetOrigin(cryostatX, cryostatY, cryostatZ);

    // set up Pb volumes:
    fParallelWorld->SetPbMinLength(fParallelPbMinLength);
    fParallelWorld->SetNPbVolumes(fParallelNPbVolumes);
    fParallelWorld->SetPbDeltaLength(fParallelPbDeltaLength);

    // set up poly volumes:
    fParallelWorld->SetPolyMinLength(fParallelPolyMinLength);
    fParallelWorld->SetNPolyVolumes(fParallelNPolyVolumes);
    fParallelWorld->SetPolyDeltaLength(fParallelPolyDeltaLength);

  }

  MGLog(routine) 
    << "cryostat coords within world volume: (x=" 
    << G4BestUnit( cryostatX, "Length")
    << ", y=" 
    << G4BestUnit( cryostatY, "Length")
    << ")" << endlog;

  MGLog(routine) << "top of cryostat z coord relative to floor of trailer:  " 
    << G4BestUnit( topOfCryostatInBEGeDetectorCoords + begeZ, "Length") 
    << endlog;

  MGLog(routine) << "top of cryostat z coord relative to world volume:  " 
    << G4BestUnit( cryostatZ, "Length") 
    << endlog;

  if ( fIsDetectorInShield ) {
    fCryostatPhysical 
      = new G4PVPlacement(0, cryostatPlacement, kurfBEGeDetecorLogical,
                          "begeCryostatPhysical", trailerAirSpaceSamplingLogical, false, 0);
  } else {
    fCryostatPhysical 
      = new G4PVPlacement(0, cryostatPlacement, kurfBEGeDetecorLogical,
                          "begeCryostatPhysical", fTrailerAirSpaceLogical, false, 0);
    }

  double alPlateHalfWidth = 18.0/2.0*inch;
  double alPlateHalfHeight = 0.5/2.0*inch;

  // two Al plates under cryostat -- part of dewar stand
  // these two have slightly different widths; but they are modeled as the same

  G4Box *alPlateBox = new G4Box("alPlateBox", alPlateHalfWidth, alPlateHalfWidth,
    alPlateHalfHeight);

  G4LogicalVolume* alPlateLogical= new G4LogicalVolume(alPlateBox, Aluminum,
    "alPlateLogical", 0, 0, 0, true );

  new G4PVPlacement( 0,
    G4ThreeVector(begeX, begeY, cryostatStandPlateZ - 32.0*inch + 5.0*inch),
    alPlateLogical, "alPlate1", fTrailerAirSpaceLogical, false, 0);
 
  new G4PVPlacement( 0, 
    G4ThreeVector(begeX, begeY, 
      cryostatStandPlateZ - 32.0*inch - 1.25*inch - 2.0*alPlateHalfHeight + 5.0*inch),
    alPlateLogical, "alPlate2", fTrailerAirSpaceLogical, false, 0);
 

  //*************************************************************************//
  //------------------------VISUALIZATION ATTRIBUTES-------------------------//
  //-------------------------------------------------------------------------//

  G4double alpha = 1.0; // transparency

  //G4VisAttributes* grey = new G4VisAttributes(G4Color(0.6,0.6,0.6, 0.05)); 
  G4VisAttributes* redva =    new G4VisAttributes(G4Color(1, 0, 0, alpha));
  G4VisAttributes* purple =    new G4VisAttributes(G4Color(0.41, 0, 0.6, 0.05));
  //G4VisAttributes* darkgrey = new G4VisAttributes(G4Color(0.9,0.9,0.9, alpha));
  //G4VisAttributes* lightmagenta = new G4VisAttributes(G4Color(1,0,1, alpha));
  //G4VisAttributes* magenta = new G4VisAttributes(G4Color(1, 0, 1, alpha));
  G4VisAttributes* blueva =   new G4VisAttributes(G4Colour(0, 0, 1, 1.0));
  G4VisAttributes* greenva =  new G4VisAttributes(G4Colour(0, 1, 0, 0.01));
  G4VisAttributes* darkgreen =  new G4VisAttributes(G4Colour(0, 0.6, 0, 1.0));
  G4VisAttributes* yellowva = new G4VisAttributes(G4Colour(1, 1, 0, 0.01));
  G4VisAttributes* cyanva =   new G4VisAttributes(G4Colour(0, 1, 0, alpha));
  G4VisAttributes* blackva =  new G4VisAttributes(G4Colour(0, 0, 0, alpha));

  theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
  //theDetectorLogical->SetVisAttributes(blue);

  //fExperimentalHallLogical->SetVisAttributes(yellow);
  fExperimentalHallLogical->SetVisAttributes(G4VisAttributes::Invisible);

  fPolyLogical->SetVisAttributes(blueva);
  fPolyTopLogical->SetVisAttributes(blueva);
  //fPolyLogical->SetVisAttributes(G4VisAttributes::Invisible);

  fTrailerLogical->SetVisAttributes(blackva);
  //fTrailerAirSpaceLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fTrailerAirSpaceLogical->SetVisAttributes(purple);

  //fLeadShieldLogical->SetVisAttributes(black);
  fLeadShieldLogical->SetVisAttributes(darkgreen);
  //fLeadShieldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  
  fInnerAinLeadLogical->SetVisAttributes(redva);

  fScintillatorLogical->SetVisAttributes(cyanva);
  //fScintillatorLogical->SetVisAttributes(G4VisAttributes::Invisible);

  kurfBEGeDetecorLogical->SetVisAttributes(G4VisAttributes::Invisible); 

  fRockLogical->SetVisAttributes(yellowva);
  fRockSamplingLogical->SetVisAttributes(greenva);

} // end ConstructDetector


//---------------------------------------------------------------------------//
void MJGeometryKURFBEGeDetectorInShield::SetDefaults()
{ 
  fEps = 0.1*mm;

  //WORLD VOLUME
  fWorldVolumeRadius = 50 * m;

  //Shielding dimensions
  fInnerCavityWidth = 6. * cm;
  fInnerCavityHeight = 8. * cm;

  // radon exclusion -- dimensions from mds drawings by G. Swift
  fRadonExclusionBoxHeight = 24.50 * inch / 2.0; // dwg mds-03-00-00

  // ancient lead -- dimensions from MDS drawings by G. Swift:
  fAncLeadHeight = 9.0 * inch / 2.0;
  fAncLeadWidth = 7.50 * inch / 2.0;
  fAncLeadNarrowLength = 8.0 * inch / 2.0;
  fAncLeadNarrowWidth = 5.75 * inch / 2.0;
  fAncLeadCavityHeight = 7.0 * inch / 2.0;
  fAncLeadCavityWidth = 3.75 * inch / 2.0;
  fAncLeadCavityOffsetFromTop = 1.0 * inch;
  fAncLeadHoleRadius = 1.50 * inch / 2.0;
  fAncLeadZOffset = 1.0*inch;


  // use half widths and lengths to be consistent 
  fLeadShieldWidth = 24.0 * inch / 2.0; 
  fLeadShieldHeight = 20.0 * inch / 2.0;
  fLeadShieldCavityWidth = 6.25 * inch / 2.0; // use half widths to be consistent
  fLeadShieldCavityLength = 12.0 * inch / 2.0;
  fLeadShieldWiderCavityWidth = 8.0 * inch / 2.0;
  fLeadShieldWiderCavityLength = 4.0 * inch / 2.0;
  fLeadShieldWaistLength = fLeadShieldCavityLength -
                           fLeadShieldWiderCavityLength;
  fLeadShieldWaistWidth = 22.0 * inch / 2.0;
  fLeadShieldUnderCryostatHeight = 3.85 * inch / 2.0;

  fDetectorStandHeight = 55.0 * inch / 2.0; // estimate from shield drawings
  fDetectorStandWidth = 33.50 * inch / 2.0;


  fPolyThickness = 10.0 * inch;
  fScintillatorThickness = 2.0 * inch / 2.0;
  fScintillatorWidth = 1.0 * m / 2.0;

  // add a 2-inch buffer:
  fTotalShieldWidth = fScintillatorWidth + fPolyThickness + 2.0 * inch; 
  fTotalShieldHeight = fDetectorStandHeight + fLeadShieldHeight +
                       fScintillatorThickness + fPolyThickness;


  // experimental hall dimensions from
  // http://www.physics.unc.edu/research/nuclear/particle_astro/kimballton.php
  fHallHeight = 20*12.0/2.0 * inch;
  fHallWidth = 40*12.0/2.0 * inch;
  fHallLength = 100*12.0/2.0 * inch; 

  fParallelPbMinLength = 7.0*cm;
  fParallelPbDeltaLength = 1.0*cm;
  fParallelNPbVolumes = 23;

  fParallelPolyMinLength = 49.0/2.0*inch + 4.0*cm;;
  fParallelPolyDeltaLength = 2.7*cm;
  fParallelNPolyVolumes = 9;

}
//---------------------------------------------------------------------------//
