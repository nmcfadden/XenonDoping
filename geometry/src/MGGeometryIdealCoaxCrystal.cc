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
//      
// CLASS IMPLEMENTATION:  MGGeometryIdealCoaxCrystal.cc
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
 * AUTHOR: R. Henning
 * CONTACT: 
 * 
 * REVISION:
 *
 * 10-12-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 12-13-2004, Renamed to MGGeometryIdealCoaxCrystal, R. Henning
 * 11-17-2005, Updated copy constructor to allow use of this class in STL 
 *             container classes. Minor other improvements R. Henning.
 * 12-15-2005, Removed extraneous "placement" variable.
 *             Changed name of active part of crystal to "Active" from 
 *             "physical", R. Henning
 * 02-21-2007, Added option for inactive crystal, A. Schubert
 * 03-16-2007, Added fIsActive(true) to copy constructor and explicity set it to
 *             true in SetDefaults()
 * 03-31-2009, Added fExtraDeadLayer to model extended dead layers in crystals
 * 01-12-2011, Added fBeveledEdgeConsidered to add a beveled edge to crystals.  M. Kidd
 * 01-15-2011, Added extra side dead layer - side dead layer can be different from top.  M. Kidd
*/
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "math.h"
#include "G4Box.hh"
#include "G4Color.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "geometry/MGGeometryIdealCoaxMessenger.hh" 
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "geometry/MGGeometryIdealCoaxCrystal.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;
using namespace MaGe;

MGGeometryIdealCoaxCrystal::MGGeometryIdealCoaxCrystal( G4String serNum, 
							G4bool messengerOff, 
							G4bool many):
  MGGeometryDetector(serNum), fCrystalMaterial(0), fCrystalMaterialName(""),
  fCutInHalf(false), fIsOneOfMany(many), fActiveVolumeLogical(0), 
  fIsActive(true), fWholeCrystalVisAtt(0), fActiveCrystalVisAtt(0)
{
  fCrystalMass = fCrystalVolume = 0.0;
  if(messengerOff)
    fMessenger = 0;
  else
    fMessenger = new MGGeometryIdealCoaxMessenger(this); 
}

//---------------------------------------------------------------------------//

  // Only copies dimensions. Does not copy all pointers and serial number. 
MGGeometryIdealCoaxCrystal::MGGeometryIdealCoaxCrystal(const MGGeometryIdealCoaxCrystal &rhs):
  MGGeometryDetector(""),
  fCoreRadius(rhs.fCoreRadius),
  fCoreDepth(rhs.fCoreDepth),
  fHeight(rhs.fHeight),
  fRadius(rhs.fRadius),
  fCoreBubbleOn(rhs.fCoreBubbleOn),
  fDeadLayerConsidered(rhs.fDeadLayerConsidered),
  fExcessSideDeadLayerConsidered(rhs.fExcessSideDeadLayerConsidered),
  fCornerRadius(rhs.fCornerRadius),
  fOuterDeadLayerThickness(rhs.fOuterDeadLayerThickness),
  fCoreDeadLayerThickness(rhs.fCoreDeadLayerThickness),
  fExtraDeadLayerConsidered(rhs.fExtraDeadLayerConsidered),
  fExtraDeadLayerBottomZ(rhs.fExtraDeadLayerBottomZ),
  fExtraDeadLayerMiddleZ(rhs.fExtraDeadLayerMiddleZ),
  fExtraDeadLayerTopZ(rhs.fExtraDeadLayerTopZ),
  fExtraDeadLayerBottomInnerR(rhs.fExtraDeadLayerBottomInnerR),
  fExtraDeadLayerMiddleInnerR(rhs.fExtraDeadLayerMiddleInnerR),
  fExtraDeadLayerTopInnerR(rhs.fExtraDeadLayerTopInnerR),
  fExtraDeadLayerBottomOuterR(rhs.fExtraDeadLayerBottomOuterR),
  fExtraDeadLayerMiddleOuterR(rhs.fExtraDeadLayerMiddleOuterR),
  fExtraDeadLayerTopOuterR(rhs.fExtraDeadLayerTopOuterR),
  fBeveledEdgeConsidered(rhs.fBeveledEdgeConsidered),
  fBeveledEdgeAngle(rhs.fBeveledEdgeAngle),
  fBeveledEdgeT(rhs.fBeveledEdgeT),
  fCrystalMass(rhs.fCrystalMass),
  fCrystalVolume(rhs.fCrystalVolume),
  fCrystalMaterial(rhs.fCrystalMaterial),
  fCrystalMaterialName(rhs.fCrystalMaterialName),
  fCutInHalf(rhs.fCutInHalf),
  fIsOneOfMany(rhs.fIsOneOfMany),
  fActiveVolumeLogical(0), fIsActive(rhs.fIsActive), fMessenger(0), 
  fWholeCrystalVisAtt(0), fActiveCrystalVisAtt(0)
{;}

//---------------------------------------------------------------------------//

MGGeometryIdealCoaxCrystal::~MGGeometryIdealCoaxCrystal() 
{
  delete fMessenger;
  delete fWholeCrystalVisAtt;
  delete fActiveCrystalVisAtt;
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::CalculateMassAndVolume() 
{
  if(fCoreRadius < 0.0)
    fCoreRadius = 0.0;
  if(fCoreDepth > fHeight) {
    fCoreDepth = fHeight;
    fCoreBubbleOn = false;
  }
  fCrystalVolume = pi * (fRadius * fRadius * fHeight -
			 fCoreRadius * fCoreRadius * fCoreDepth);
  if(fCoreBubbleOn)
    fCrystalVolume -= (2.0/3.0) * fCoreRadius * fCoreRadius * fCoreRadius;
  if(fCornerRadius > fRadius) {
    fCornerRadius = fRadius;
    MGLog(warning) << "Corner Radius too large. Reset to : " 
		   << fCornerRadius / cm << "cm " << endlog;
  }
  if(fCornerRadius > 0.)
    fCrystalVolume -= 0.5 * (4.-pi) * pi * fCornerRadius * fCornerRadius *
                      (fRadius - fCornerRadius);
  if(!fCrystalMaterial){
    MGLog(debugging) << "NULL pointer for material of crystal." 
		   << " Cannot compute mass, setting it to zero. " << endlog;
    fCrystalMass = 0.0;
  } else 
    fCrystalMass = fCrystalVolume * fCrystalMaterial->GetDensity();
  MGLog(trace) << "Mass (gram): " << fCrystalMass / g << endlog;
  MGLog(trace) << "Volume (cm^3): " << fCrystalVolume / cm3 << endlog;
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::ConstructDetector() 
{
  if(!(fCrystalMaterial = G4Material::GetMaterial(fCrystalMaterialName))) {
    MGLog(error) << "NULL Pointer for crystal material: " << fCrystalMaterialName << endlog;
    Dump();
    MGLog(fatal) << endlog;
  }
MGLog(debugging)<<"Made it here\n";
  CalculateMassAndVolume(); // Also check consistency of dimensions.

  if(fIsOneOfMany)
    ConstructDetectorAsOneOfMany();
  else
    ConstructDetectorAsSingle();

  // Visualization attributes for "physical" crystal
  fWholeCrystalVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0.8, .9));
  fWholeCrystalVisAtt->SetVisibility( true );
  fWholeCrystalVisAtt->SetForceSolid( true );
  // fWholeCrystalVisAtt->SetForceAuxEdgeVisible( true );
  theDetectorLogical->SetVisAttributes(fWholeCrystalVisAtt);

  if(fDeadLayerConsidered) {
  // Set active volume's visibility attributes.
    fActiveCrystalVisAtt = new G4VisAttributes(magenta);
    fActiveCrystalVisAtt->SetVisibility(true);
    fActiveCrystalVisAtt->SetForceSolid(true);
    fActiveVolumeLogical->SetVisAttributes(fActiveCrystalVisAtt);
  }
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::ConstructDetectorAsOneOfMany() 
{
  G4double eps = 0.00001 * cm;

  size_t i; 
  G4LogicalVolumeStore *logicalVolumeStore = 
    G4LogicalVolumeStore::GetInstance();
  theDetectorLogical = 0;
  if(!fDeadLayerConsidered) { // just return 'crystalLogical'
    fActiveVolumeLogical = 0;
    for(i = 0; i < logicalVolumeStore->size(); i++)
      if((*logicalVolumeStore)[i]->GetName() == "crystalLogical") {
	 theDetectorLogical = (*logicalVolumeStore)[i];
         break;
      }
    if(!theDetectorLogical) {
      MGLog(error) << "Could not find supposedly pre-existing crystal!"
      	     << endlog;
      MGLog(fatal) << endlog;
    }
  } else { // dead layers considered, provide active volume a unique name
    fActiveVolumeLogical = 0; 
    for(i = 0; i < logicalVolumeStore->size(); i++) {
      if((*logicalVolumeStore)[i]->GetName() == "crystalLogical") 
	 theDetectorLogical = (*logicalVolumeStore)[i];
      else if((*logicalVolumeStore)[i]->GetName() == "activeLogical") 
         fActiveVolumeLogical = (*logicalVolumeStore)[i];
      if(theDetectorLogical && fActiveVolumeLogical) break;
    }
    if(!(fActiveVolumeLogical && theDetectorLogical)) {
	MGLog(error) << "Could not find supposedly pre-existing crystal!"
		     << endlog;
	MGLog(fatal) << endlog;
    }	  
    G4String crystalName = "crystal" + GetSerialNumber();
    G4String activeVolumeName = "";
    G4String placementName = "";
    if(fIsActive){ //A. Schubert 061121
      activeVolumeName = "activeLogical" + GetSerialNumber();
      placementName = "Active" + GetSerialNumber();
    }else{
      activeVolumeName = "dudLogical" + GetSerialNumber();
      placementName = "dud" + GetSerialNumber();
    } 
    fActiveVolumeLogical = 
      new G4LogicalVolume(fActiveVolumeLogical->GetSolid(), 
			  fActiveVolumeLogical->GetMaterial(),
			  activeVolumeName, 0, 0, 0);
    G4LogicalVolume *templateVolume = theDetectorLogical;
    theDetectorLogical = 
      new G4LogicalVolume(templateVolume->GetSolid(),
			  templateVolume->GetMaterial(),
			  crystalName, 0, 0, 0);
    new G4PVPlacement(0, 
		      G4ThreeVector(0.0,0.0,-fOuterDeadLayerThickness/2.+eps),
		      fActiveVolumeLogical,	
		      placementName,
		      theDetectorLogical,  false, 0);      
  }
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::ConstructDetectorAsSingle() 
{
  // Build crystal cylinder and rounded edge

  G4double eps = 0.00001 * cm;// Small distance to perturb solid placements.
                              // Avoids overlapping surfaces, etc...

  G4VSolid *noCoreCrystal;  // Crystal without core.
  G4VSolid *noCoreCrystal0;  // Crystal without core or beveled edge.
  G4VSolid *crystalG4Solid; // Final G4 Solid representing crystal  
	


  // First build a crystal with no core. Add a rounded edge if required.
  if(fCornerRadius > 0. && !fBeveledEdgeConsidered) {
    G4Tubs *mainCylinder1 = new G4Tubs("mainCylinder1", 0., fRadius, 
				       (fHeight - fCornerRadius)/2., 0*deg, 
				       360.0*deg);
    G4Tubs *mainCylinder2 = new G4Tubs("mainCylinder2", 0.,
				       fRadius - fCornerRadius, 
				       fHeight / 2., 0*deg, 360.0*deg);

    G4UnionSolid *mainCylinder = new G4UnionSolid("mainCylinder", 
				       mainCylinder2, mainCylinder1, 0, 
				       G4ThreeVector(0, 0, -fCornerRadius/2.));
    G4Torus *cornerTorus = new G4Torus("cornerTorus", 0., fCornerRadius, 
				       fRadius-fCornerRadius-eps, 0.*deg, 
				       360.*deg);
    noCoreCrystal = new G4UnionSolid("solidCrystal", mainCylinder, 
			  cornerTorus, 0, 
			  G4ThreeVector(0, 0, fHeight/2.-fCornerRadius));
  }   else if(fCornerRadius > 0. && fBeveledEdgeConsidered) {  //add a rounded edge and a bevel

	  G4double coneThickness = 20.0*mm;
	  G4double beveledEdgeZ = (fRadius-fBeveledEdgeT)/tan(fBeveledEdgeAngle)+fHeight/2.0;  //this calculates the height the cone needs to be to take off a bottom edge fBeveledEdgeT thick
	  G4double rMax1 = (beveledEdgeZ/2.0+fHeight/2.0)*tan(fBeveledEdgeAngle);
	  G4ThreeVector subConePos(0,0, -beveledEdgeZ/2.0);
	  
	  G4Tubs *mainCylinder1 = new G4Tubs("mainCylinder1", 0., fRadius, 
										 (fHeight - fCornerRadius)/2., 0*deg, 
										 360.0*deg);
	  G4Tubs *mainCylinder2 = new G4Tubs("mainCylinder2", 0.,
										 fRadius - fCornerRadius, 
										 fHeight / 2., 0*deg, 360.0*deg);
	  
	  G4UnionSolid *mainCylinder = new G4UnionSolid("mainCylinder", 
													mainCylinder2, mainCylinder1, 0, 
													G4ThreeVector(0, 0, -fCornerRadius/2.));
	  G4Torus *cornerTorus = new G4Torus("cornerTorus", 0., fCornerRadius, 
										 fRadius-fCornerRadius-eps, 0.*deg, 
										 360.*deg);
	  noCoreCrystal0 = new G4UnionSolid("solidCrystal0", mainCylinder, 
									   cornerTorus, 0, 
									   G4ThreeVector(0, 0, fHeight/2.-fCornerRadius));
	  
	  G4Cons *subCone = new G4Cons("subCone", 0.0, coneThickness, 
								   rMax1, rMax1+coneThickness, beveledEdgeZ/2.0, 0*deg, 360*deg);
	  // cone to create beveled edge if fBeveledEdgeConsidered = true
	  noCoreCrystal = new G4SubtractionSolid("solidCrystal", noCoreCrystal0, 
											 subCone, 0, subConePos);
	  
	  } else if(fBeveledEdgeConsidered && fCornerRadius <= 0.) {
  
	  G4double coneThickness = 20.0*mm;
	  G4double beveledEdgeZ = (fRadius-fBeveledEdgeT)/tan(fBeveledEdgeAngle)+fHeight/2.0;  //this calculates the height the cone needs to be to take off a bottom edge fBeveledEdgeT thick
	  G4double rMax1 = (beveledEdgeZ/2.0+fHeight/2.0)*tan(fBeveledEdgeAngle);
	  G4ThreeVector subConePos(0,0, -beveledEdgeZ/2.0);
	  
	  G4Tubs *mainCylinder = new G4Tubs("mainCylinder", 0., fRadius, (fHeight - fCornerRadius)/2., 0*deg, 360.0*deg);
	  
	  G4Cons *subCone = new G4Cons("subCone", 0.0, coneThickness, rMax1, rMax1+coneThickness, beveledEdgeZ/2.0, 0*deg, 360*deg);
									 // cone to create beveled edge if fBeveledEdgeConsidered = true
	  noCoreCrystal = new G4SubtractionSolid("solidCrystal", mainCylinder, subCone, 0, subConePos);
	  
  
	} else {
    noCoreCrystal = new G4Tubs("mainCylinder", 0.*cm,
			      fRadius, fHeight/2., 0*deg, 360.0*deg);
  }    

  // Next, remove the core, if required.
  // If fCoreRadius <=0, a core will not be added.
  G4double depth = fCoreDepth + eps;
  if(fCoreRadius > 0.) {
    G4Tubs *coreCylinder = new G4Tubs("crystalCoreCylinder",0*cm, fCoreRadius, 
                                      depth/2., 0*deg, 360.0*deg);
    G4VSolid *core;
    if(fCoreBubbleOn) {
      G4Sphere *bubble = new G4Sphere("crystalCoreBubble", 0*cm, fCoreRadius, 
                                     0*deg, 360.0*deg, 0*deg, 180.0*deg);
      core = new G4UnionSolid("crystalCore", coreCylinder, bubble, 0, 
			    G4ThreeVector(0,0,depth/2.-eps));
    } else
      core = coreCylinder;
    crystalG4Solid = new G4SubtractionSolid("crystalWithCore", noCoreCrystal,
			core, 0, 
			G4ThreeVector(0,0,(fCoreDepth-fHeight-eps)/2.));
					    
  } else  
    crystalG4Solid = noCoreCrystal;

  // crystalG4Solid now has the G4 Solid corresponding to the crystal. 
  

  // Cut off one half of crystal lengthwise. Useful for debugging.
  if(fCutInHalf){
    MGLog(routine) << "Creating half coax." << endlog;
    G4Box *bigBox = new G4Box("bigBox", fHeight, fHeight, fHeight);
    crystalG4Solid = new G4SubtractionSolid(
      "halfCrystal", crystalG4Solid, 
       bigBox, 0, G4ThreeVector(-fHeight, 0., 0.));
  } 
    // Create Crystal Geant 4 logical volume.
  theDetectorLogical = new G4LogicalVolume(crystalG4Solid, 
        				     fCrystalMaterial,
					     "crystalLogical",
					     0, 0, 0, true);

  // Next consider deadlayers, if required.
  // Create an active logical volume that is placed inside theDetectorLogical
  if(fDeadLayerConsidered) {
    G4VSolid *activeNoCoreCrystal;  // Active part of crystal without core.
    G4VSolid *activeNoCoreCrystal0;  // Active part of crystal without core or beveled edge. (M.Kidd 1/18/11)
    G4VSolid *activeCrystalG4Solid; // Final G4 Solid representing active 
                                    // part of crystal  

    // Compute dimensions of active region solid
	//4/5/2011 Added an option for a different dead layer on side compared to top
	G4double activeRadius;
	G4double activeCornerRadius;
	if(fExcessSideDeadLayerConsidered) 
		{
		activeRadius = fRadius - fOuterDeadLayerThickness - fExcessSideDeadLayerThickness;  
		activeCornerRadius = fCornerRadius - fOuterDeadLayerThickness - fExcessSideDeadLayerThickness; 
		//if you want a dead layer on the side which is different from the top M. Kidd 4/13/2011
		}
    	else 
		{
		activeRadius = fRadius - fOuterDeadLayerThickness;
		activeCornerRadius = fCornerRadius - fOuterDeadLayerThickness;
		}
	MGLog(routine)<<"Active radius is:  "<<activeRadius<<"\tOuter dead layer is:  "<<fOuterDeadLayerThickness<<"\n";

	if(fExcessSideDeadLayerConsidered)MGLog(routine)<<"\tExcess dead layer thickness is:  "<<fExcessSideDeadLayerThickness<<"\n";


    G4double activeHeight = fHeight - fOuterDeadLayerThickness;
    G4double activeCoreRadius = fCoreRadius + fCoreDeadLayerThickness;

    // First build active region with no core. Add a rounded edge if required.
    if(activeRadius > 0. && activeCornerRadius  > 0.0 && !fBeveledEdgeConsidered) {
      G4Tubs *activeCylinder1 = 
	new G4Tubs("activeCylinder1", 0., activeRadius,
		   (activeHeight-activeCornerRadius)/2., 0*deg, 360.0*deg);
      G4Tubs *activeCylinder2 = 
	new G4Tubs("activeCylinder2", 0., activeRadius - activeCornerRadius,
		    activeHeight / 2., 0*deg, 360.0*deg);
      G4UnionSolid *activeCylinder = 
	new G4UnionSolid("activeCylinder", activeCylinder2, activeCylinder1,
			  0, G4ThreeVector(0, 0, -activeCornerRadius / 2.));
      // Subract eps from radius so that torus moves in slightly.
      // This prevents a singularity at the center of the torus that also
      // overlaps with the cotner of the two cylinder union.
      G4Torus *activeCornerTorus = 
	new G4Torus("activeCornerTorus", 0., activeCornerRadius, 
		    fRadius - fCornerRadius - eps, 0.*deg, 360.*deg);
      activeNoCoreCrystal = 
	new G4UnionSolid("activeSolidCrystal", activeCylinder, 
			 activeCornerTorus, 0, 
			 G4ThreeVector(0, 0, 
				       activeHeight/2.-activeCornerRadius));
    } else if(activeRadius > 0. && activeCornerRadius  > 0.0 && fBeveledEdgeConsidered) {
		//added 1/18/11 M.Kidd - incorporating beveled edge with active layer

	  G4double coneThickness = 20.0*mm;
	  G4double beveledEdgeZ = (fRadius-fBeveledEdgeT)/tan(fBeveledEdgeAngle)+fHeight/2.0;  //this calculates the height the cone needs to be to take off a bottom edge fBeveledEdgeT thick
	  G4double rMax1 = (beveledEdgeZ/2.0+fHeight/2.0)*tan(fBeveledEdgeAngle);
		
          G4double extraZ = (fRadius-activeRadius)/cos(fBeveledEdgeAngle)+fOuterDeadLayerThickness/2.0; 
										//this is the amount the 
										//subtraction cone needs to 
										//move upwards to account for 
										//dead layer on beveled edge. (M.Kidd)
		MGLog(routine)<<"cosine of angle is:  "<<cos(fBeveledEdgeAngle)<<"\n";

	
		G4ThreeVector subConePos(0,0, -beveledEdgeZ/2.0+extraZ);

		MGLog(routine)<<"Extra Z = "<<extraZ<<"\n";
		
		
		G4Tubs *activeCylinder1 = 
		new G4Tubs("activeCylinder1", 0., activeRadius,
				   (activeHeight-activeCornerRadius)/2., 0*deg, 360.0*deg);
		G4Tubs *activeCylinder2 = 
		new G4Tubs("activeCylinder2", 0., activeRadius - activeCornerRadius,
				   activeHeight / 2., 0*deg, 360.0*deg);
		G4UnionSolid *activeCylinder = 
		new G4UnionSolid("activeCylinder", activeCylinder2, activeCylinder1,
						 0, G4ThreeVector(0, 0, -activeCornerRadius / 2.));
		// Subract eps from radius so that torus moves in slightly.
		// This prevents a singularity at the center of the torus that also
		// overlaps with the cotner of the two cylinder union.
		G4Torus *activeCornerTorus = 
		new G4Torus("activeCornerTorus", 0., activeCornerRadius, 
					fRadius - fCornerRadius - eps, 0.*deg, 360.*deg);
		activeNoCoreCrystal0 = 
		new G4UnionSolid("activeSolidCrystal0", activeCylinder, 
						 activeCornerTorus, 0, 
						 G4ThreeVector(0, 0, activeHeight/2.-activeCornerRadius));
		//bevel the edge (M.Kidd 1/18/11)
		G4Cons *subCone = new G4Cons("subCone", 0.0, coneThickness, rMax1, rMax1+coneThickness, beveledEdgeZ/2.0, 0*deg, 360*deg);
		// cone to create beveled edge if fBeveledEdgeConsidered = true
		activeNoCoreCrystal = new G4SubtractionSolid("solidCrystal", activeNoCoreCrystal0, subCone, 0, subConePos);
		
	} else {
      activeNoCoreCrystal = new G4Tubs("mainCylinder", 0.*cm, activeRadius, 
				 activeHeight/2., 0*deg, 360.0*deg);
    }    

  // Next, remove the core, if required.
  // If fCoreRadius <=0, a core will not be added.
    if(fCoreRadius > 0.) {
      G4Tubs *activeCoreCylinder = new G4Tubs("activeCrystalCoreCylinder",
				      0*cm, activeCoreRadius, 
                                      depth/2., 0*deg, 360.0*deg);
      G4VSolid *activeCore;
      if(fCoreBubbleOn) {
	G4Sphere *activeBubble = new G4Sphere("activeCrystalCoreBubble", 0*cm, 
					      activeCoreRadius, 
                                              0*deg, 360*deg, 0*deg, 180*deg);
	activeCore = new G4UnionSolid("activeCrystalCore", activeCoreCylinder, 
				      activeBubble, 0, 
				      G4ThreeVector(0,0,depth/2. - eps));
      } else { 
	activeCore = activeCoreCylinder;
      }
      activeCrystalG4Solid = new G4SubtractionSolid("activeCrystalWithCore", 
			activeNoCoreCrystal, activeCore, 0, 
			G4ThreeVector(0,0,(fCoreDepth-activeHeight-eps)/2.));
    } else {  
      activeCrystalG4Solid = activeNoCoreCrystal;
    }
  // Next, move the extra dead layer if necessary - Vic Gehman, March 31, 2009...
    if(fExtraDeadLayerConsidered){
	  ExtraDeadLayerZ[0] = fExtraDeadLayerBottomZ - (0.5 * fHeight);
	  ExtraDeadLayerZ[1] = fExtraDeadLayerMiddleZ - (0.5 * fHeight);
	  ExtraDeadLayerZ[2] = fExtraDeadLayerTopZ - (0.5 * fHeight);
	  ExtraDeadLayerInnerR[0] = fExtraDeadLayerBottomInnerR;
	  ExtraDeadLayerInnerR[1] = fExtraDeadLayerMiddleInnerR;
	  ExtraDeadLayerInnerR[2] = fExtraDeadLayerTopInnerR;
	  ExtraDeadLayerOuterR[0] = fExtraDeadLayerBottomOuterR;
	  ExtraDeadLayerOuterR[1] = fExtraDeadLayerMiddleOuterR;
	  ExtraDeadLayerOuterR[2] = fExtraDeadLayerTopOuterR;
	  std::cout << std::endl << "Extra dead layer coordinates:" << std::endl
	                         << "Bottom: z = " << ExtraDeadLayerZ[0] / cm << " cm, Inner r = " << ExtraDeadLayerInnerR[0] / cm << " cm, Outer r = " << ExtraDeadLayerOuterR[0] / cm << " cm" << std::endl
	                         << "Middle: z = " << ExtraDeadLayerZ[1] / cm << " cm, Inner r = " << ExtraDeadLayerInnerR[1] / cm << " cm, Outer r = " << ExtraDeadLayerOuterR[1] / cm << " cm" << std::endl
	                         << "   Top: z = " << ExtraDeadLayerZ[2] / cm << " cm, Inner r = " << ExtraDeadLayerInnerR[2] / cm << " cm, Outer r = " << ExtraDeadLayerOuterR[2] / cm << " cm" << std::endl;
	  G4Polycone *extraDeadLayerSolid = new G4Polycone("extraDeadLayerSolid",
	                                                   0., 360 * deg,
							   3, ExtraDeadLayerZ,
							   ExtraDeadLayerInnerR, ExtraDeadLayerOuterR);
      activeCrystalG4Solid = new G4SubtractionSolid("activeCrystalWithCore", 
                                                    activeCrystalG4Solid, extraDeadLayerSolid, 0,
													G4ThreeVector(0.,0.,0.));
	}
  // Create active volume Geant 4 logical volume.
    fActiveVolumeLogical = new G4LogicalVolume(activeCrystalG4Solid, 
					       fCrystalMaterial,
					       "activeLogical",
					       0, 0, 0, true);

  // Insert active volume into the main crystal.
	if(SetNameOn() == true)
	{
	    new G4PVPlacement(0, G4ThreeVector(0.0,0.0,-fOuterDeadLayerThickness/2.+eps),
		  fActiveVolumeLogical,	fSetName+"activeVolumeInCrystal",
		  theDetectorLogical,  false, 0);
	}
	else
	{
    new G4PVPlacement(0, G4ThreeVector(0.0,0.0,-fOuterDeadLayerThickness/2.+eps),
		  fActiveVolumeLogical,	"activeVolumeInCrystal",
		  theDetectorLogical,  false, 0);
	}
  }			
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::Dump()
{
  G4cout << "Settings for MGGeometryIdealCoaxCrystal:" << G4endl;
  G4cout << "----------------------------------------" << G4endl;
  G4cout << "Dimensions are in cm." << G4endl;
  G4cout << "fCoreRadius: " << fCoreRadius/cm << G4endl;
  G4cout << "fCoreDepth: " << fCoreDepth/cm << G4endl;
  G4cout << "fHeight: " << fHeight/cm << G4endl;
  G4cout << "fRadius: " << fRadius/cm << G4endl;
  G4cout << "fCoreBubbleOn: " << fCoreBubbleOn << G4endl;
  G4cout << "fCornerRadius: " << fCornerRadius/cm << G4endl;
  G4cout << "fBeveledEdgeConsidered:  " << fBeveledEdgeConsidered << G4endl;
  G4cout << "fBeveledEdgeAngle:  "<< fBeveledEdgeAngle/deg << G4endl;
  G4cout << "fBeveledEdgeT:  " << fBeveledEdgeT/mm << G4endl;
  G4cout << "fExcessSideDeadLayerConsidered:  "<<fExcessSideDeadLayerConsidered << G4endl;
  G4cout << "fExcessSideDeadLayerThickness:  "<<fExcessSideDeadLayerThickness<< G4endl;
  G4cout << "fOuterDeadLayerThickness: " << fOuterDeadLayerThickness/cm << G4endl;
  G4cout << "fCoreDeadLayerThickness: " << fCoreDeadLayerThickness/cm << G4endl;
  G4cout << "fDeadLayerConsidered: " <<  fDeadLayerConsidered << G4endl; 
  G4cout << "fExtraDeadLayerConsidered: " <<  fExtraDeadLayerConsidered << G4endl; 
  G4cout << "fExtraDeadLayerBottomZ: " <<  fExtraDeadLayerBottomZ/cm << G4endl; 
  G4cout << "fExtraDeadLayerMiddleZ: " <<  fExtraDeadLayerMiddleZ/cm << G4endl; 
  G4cout << "fExtraDeadLayerTopZ: " <<  fExtraDeadLayerTopZ/cm << G4endl; 
  G4cout << "fExtraDeadLayerBottomInnerR: " <<  fExtraDeadLayerBottomInnerR/cm << G4endl; 
  G4cout << "fExtraDeadLayerMiddleInnerR: " <<  fExtraDeadLayerMiddleInnerR/cm << G4endl; 
  G4cout << "fExtraDeadLayerTopInnerR: " <<  fExtraDeadLayerTopInnerR/cm << G4endl; 
  G4cout << "fExtraDeadLayerBottomOuterR: " <<  fExtraDeadLayerBottomOuterR/cm << G4endl; 
  G4cout << "fExtraDeadLayerMiddleOuterR: " <<  fExtraDeadLayerMiddleOuterR/cm << G4endl; 
  G4cout << "fExtraDeadLayerTopOuterR: " <<  fExtraDeadLayerTopOuterR/cm << G4endl; 
  G4cout << "fCrystalMass (g) : " << fCrystalMass / g << G4endl;
  G4cout << "fCrystalVolume (cm^3): " << fCrystalVolume/cm3 << G4endl;
  G4cout << "fCrystalMaterialName: " << fCrystalMaterialName << G4endl;
  G4cout << "fCutInHalf: " << fCutInHalf << G4endl;
  G4cout << "----------------------------------------" << G4endl;
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxCrystal::SetDefaults()
{
  fCoreRadius = 0.5 * cm;
  fCoreDepth = 5.0 *cm;
  fHeight = 10.0 * cm;
  fRadius = 2.5 * cm;
  fCoreBubbleOn = true;
  fSetNameAct = false;
  fBeveledEdgeConsidered = false;
  fBeveledEdgeAngle = 0.0*deg;
  fBeveledEdgeT = 0.0*cm;
  fDeadLayerConsidered = false;
  fExcessSideDeadLayerConsidered = false;
  fExcessSideDeadLayerThickness = 0.0;
  fCoreDeadLayerThickness = fOuterDeadLayerThickness = 0.0;
  fExtraDeadLayerConsidered = false;
  fExtraDeadLayerBottomZ = fExtraDeadLayerMiddleZ = fExtraDeadLayerTopZ = 0.;
  fExtraDeadLayerBottomInnerR = fExtraDeadLayerMiddleInnerR = fExtraDeadLayerTopInnerR = 0.;
  fExtraDeadLayerBottomOuterR = fExtraDeadLayerMiddleOuterR = fExtraDeadLayerTopOuterR = 0.;
  fCornerRadius = 0.5 * cm;
  fCrystalMaterialName = "Germanium-Enr";
  fCutInHalf = false;
  fIsActive = true;
  CalculateMassAndVolume(); 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
