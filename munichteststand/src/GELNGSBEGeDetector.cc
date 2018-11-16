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
// $Id: GELNGSBEGeDetector.cc,v 1.4 2009-06-05 11:23:04 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  GELNGSBEGeDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Luciano Pandola
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION: 
 *  04 Jun 2009, A. di Vacri, Revised geometry according to 
 *                                     Canberra drawings
 *  28 Apr 2010, L. Pandola, Main revision to accomodate depGe BEGes. 
 *                           Now the basic construction via 
 *                           MGGeometryBEGeDetector is used
 *
 */
//---------------------------------------------------------------------------//
//

//#include "globals.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"

#include "munichteststand/GELNGSBEGeDetectorMessenger.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "geometry/MGGeometryBEGeDetector.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "munichteststand/GELNGSBEGeDetector.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GELNGSBEGeDetector::GELNGSBEGeDetector() : MGGeometryDetector("")
{
  fMessenger = new GELNGSBEGeDetectorMessenger(this);
  top_lateral_deadlayer = 0.8*mm;
  bottom_deadlayer = 0.8*mm;
  fUseCoincidentDetector = false;
  fDetectorType = "Mi";
}

//---------------------------------------------------------------------------//

GELNGSBEGeDetector::~GELNGSBEGeDetector()
{
  if (fMessenger) delete fMessenger;
}

//---------------------------------------------------------------------------//

void GELNGSBEGeDetector::ConstructDetector()
{
  
  //------------------------------------------------ 
  // Initialize sensitive detectors
  //------------------------------------------------ 
  G4SDManager* SDman   =  G4SDManager::GetSDMpointer();
  G4String CrystalSDname      = "/mydet/gerda/gecrystal";
  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);
  SDman->AddNewDetector(CrystalSD);

  // Retrieve materials from the Table

  //G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  //G4Material* Brass = G4Material::GetMaterial("Brass");
  //G4Material* ProportionalGas = G4Material::GetMaterial("ProportionalGas");
  /*G4Material* N2Gas =*/ G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  /*G4Material* Pb =*/ G4Material::GetMaterial("MetalLead");
  /*G4Material* Fe =*/ G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  //G4Material* Vespel = G4Material::GetMaterial("Vespel");
  //G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  //G4Material* PE = G4Material::GetMaterial("PE");
  //G4Material* Teflon = G4Material::GetMaterial("Teflon");
  G4Material* Air = G4Material::GetMaterial("Air");

  //G4Material* Quartz = G4Material::GetMaterial("Quartz");

  /*G4VisAttributes * ProportionalGascolour=*/ new G4VisAttributes( G4Colour(154/255. ,237/255. ,193/255. ));  //blue-green
  /*G4VisAttributes * Pbcolour =*/ new G4VisAttributes( G4Colour(171/255. ,171/255. ,195/255. ));  //gray
  G4VisAttributes * Cucolour= new G4VisAttributes( G4Colour(255/255. ,71/255. ,33/255. ));  //red
  G4VisAttributes * Gecolour= new G4VisAttributes( G4Colour(133/255. ,133/255. ,133/255. ));  //dark grey
  G4VisAttributes * GeLicolour= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));  //almost black ( -> 'dead'...)
  /*G4VisAttributes * N2Gascolour =*/ new G4VisAttributes( G4Colour(150/255. ,150/255. ,255/255. ));   //light blue
  G4VisAttributes * Alcolour= new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));   //light gray
  //G4VisAttributes * Vespelcolour= new G4VisAttributes( G4Colour(224/255. ,224/255. ,190/255. ));  //yellow-gray
  /*G4VisAttributes * Acryliccolour=*/ new G4VisAttributes( G4Colour(231/255. ,217/255. ,240/255. ));  //light blue-grey
 
  
  //------------------------------------------------------
  //Common parameters
  //
  G4double detectorRadius = 35.5*mm;
  G4double detectorHeight = 32*mm; 
  G4double detectorDitchOuterRadius = 0.;
  G4double detectorDitchInnerRadius = 0.*mm;
  G4double detectorDitchDepth = 0.*mm;
  G4double AlThickness = 1.5*mm;
  G4double DetectorToWindow = 5*mm;
  G4double windowthickness = 0.8*mm;
  G4double distanceToCrystalInHeight = 1.5*mm; //how much the crystal extrudes from the ring

  if (fDetectorType == "CC")
    {
      detectorRadius = 37.25*mm;
      detectorHeight = 33*mm; 
      detectorDitchOuterRadius = 11.75*mm;
      detectorDitchInnerRadius = 4.5*mm;
      detectorDitchDepth = 2.*mm;
      AlThickness = 1.5*mm;
      DetectorToWindow = 4.5*mm;
      windowthickness = 0.45*mm;
      distanceToCrystalInHeight = 1.5*mm; 
    }
  else if (fDetectorType == "DD")
    {
      detectorRadius = 37.*mm;
      detectorHeight = 32*mm; 
      detectorDitchOuterRadius = 11.*mm;
      detectorDitchInnerRadius = 7.*mm;
      detectorDitchDepth = 2.5*mm;
      AlThickness = 1.5*mm;
      DetectorToWindow = 4.5*mm;
      windowthickness = 0.45*mm;
      distanceToCrystalInHeight = 1.5*mm; 
    }
  else if (fDetectorType == "Mi")
    {;}
  else
    {
      MGLog(warning) << "Detector type " << fDetectorType << " is invalid" << endlog;
      MGLog(warning) << "Using Milano-BEGE detector" << endlog;
    }
  

  //------------------------------------------------------ volumes
  //------------------------------ experimental hall (world volume) 
  G4double expHall_x = 1*m;
  G4double expHall_y = 1*m;
  G4double expHall_z = 1*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  G4LogicalVolume* experimentalHall_log = new 
    G4LogicalVolume(experimentalHall_box, Air, "expHall_log", 0, 0, 0);
  experimentalHall_log -> SetVisAttributes(G4VisAttributes::Invisible);

  // ----------- detector housing ---------------------------------- 
  
  G4double AlInnerRadius = detectorRadius+8.8*mm; //According to Dusan's
  G4double AlOuterRadius = AlInnerRadius+AlThickness;
  G4double AlHeight = 83*mm;
  G4double zpos = -1*(AlHeight/2.-detectorHeight/2.-DetectorToWindow);

  //External shell (made of Al)
  G4Tubs* Alwall = new G4Tubs("Alwall",AlInnerRadius,AlOuterRadius,AlHeight/2., 0, 360.*deg);    //Al wall
  G4LogicalVolume *logicAlwall = new G4LogicalVolume(Alwall, Al, "logicAlwall");
  logicAlwall -> SetVisAttributes(Alcolour);
  new G4PVPlacement(0, G4ThreeVector(0,0,zpos), logicAlwall,
		    "physilAlwall", experimentalHall_log, false, 0);	

  //Cover window
  G4Tubs* Alcover1 = new G4Tubs("Alcover1", 0,AlOuterRadius,AlThickness/2., 0, 360.*deg);    //Al cover
  G4Tubs* Alcover2 = new G4Tubs("Alcover2", 0,detectorRadius,(AlThickness-windowthickness)/2., 0, 360.*deg);
  G4SubtractionSolid* Alcover = new G4SubtractionSolid("Alcover",Alcover1,Alcover2,0,G4ThreeVector(0,0,-windowthickness/2.));
  G4LogicalVolume *logicAlcover = new G4LogicalVolume(Alcover, Al, "logicAlcover");
  logicAlcover -> SetVisAttributes(Alcolour);
  zpos = detectorHeight/2.+DetectorToWindow+AlThickness/2.;
  new G4PVPlacement(0, G4ThreeVector(0,0,zpos), logicAlcover,
		    "physilAlcover", experimentalHall_log, false, 0);	
  
  // ----------- detector housing ----------------------------------
					   
  G4double innerShieldRadius = detectorRadius+0.1*mm; 
  G4double CuShieldThickness = 1.5*mm;
  G4double outerShieldRadius = innerShieldRadius+CuShieldThickness;
  G4double CuShieldHeight = 57.0*mm; 
  G4double tolerance = 3*micrometer;

  //Full shield
  G4Tubs* Sh1 = new G4Tubs("Sh1",innerShieldRadius,outerShieldRadius,CuShieldHeight/2.,0.,360.*deg);
  //Add first ring
  G4double ringHeight = 7*mm;
  G4double ringOuterRadius = outerShieldRadius+1.5*mm;
  G4Tubs* Sh2 = new G4Tubs("Sh2",outerShieldRadius-tolerance,ringOuterRadius,ringHeight/2.,0.,360.*deg);
  G4double TopToFirstRing = 1.5*mm;
  G4double UnionzShift = CuShieldHeight/2.-ringHeight/2.-TopToFirstRing;
  G4UnionSolid* sum1 = new G4UnionSolid("sum1",Sh1,Sh2,0,G4ThreeVector(0,0,UnionzShift));
  //Add second ring
  ringHeight = 7.0*mm;
  G4Tubs* Sh3 = new G4Tubs("Sh2",outerShieldRadius-tolerance,ringOuterRadius,ringHeight/2.,0.,360.*deg);
  G4double TopToSecondRing = 13.0*mm;
  UnionzShift = CuShieldHeight/2.-ringHeight/2.-TopToSecondRing;
  G4UnionSolid* sum2 = new G4UnionSolid("sum2",sum1,Sh3,0,G4ThreeVector(0,0,UnionzShift));
  G4LogicalVolume *logiclSh1 = new G4LogicalVolume(sum2, Cu, "logiclSh1");
  logiclSh1 -> SetVisAttributes(Cucolour);
  zpos = -CuShieldHeight/2.+ detectorHeight/2. - distanceToCrystalInHeight;
  new G4PVPlacement(0, G4ThreeVector(0,0,zpos), logiclSh1,
		    "physilSh1", experimentalHall_log, false, 0);
			
  //Closing ring on the bottom (h=10 mm)
  G4double closingRingRadius = detectorRadius; 
  G4double closingRingHeight = 10.0*mm;
  G4Tubs* Sh6 = new G4Tubs("Sh6", 0, closingRingRadius,closingRingHeight/2., 0, 360.*deg);    //Cu shield 6
  G4LogicalVolume *logiclSh6 = new G4LogicalVolume(Sh6, Cu, "logiclSh6");
  logiclSh6 -> SetVisAttributes(Cucolour);
  zpos = -CuShieldHeight/2.+ detectorHeight/2. - distanceToCrystalInHeight-CuShieldHeight/2.
    -closingRingHeight/2.-tolerance;
  new G4PVPlacement(0, G4ThreeVector(0,0,zpos), logiclSh6,
		    "physilSh6", experimentalHall_log, false, 0);



  // ------------------- detector with dead layer -----------------------

  MGLog(trace) << "Top and lateral deadlayer: " << top_lateral_deadlayer/mm << " mm " << endlog;
  MGLog(trace) << "Bottom dead layer: " << bottom_deadlayer/mm << " mm " << endlog;

  G4double dead_layer = top_lateral_deadlayer; 

  MGGeometryBEGeDetector* theBEGeBuilder = new MGGeometryBEGeDetector("logicDet");
  theBEGeBuilder->SetRadius(detectorRadius);
  theBEGeBuilder->SetHeight(detectorHeight);
  theBEGeBuilder->SetDitchOuterRadius(detectorDitchOuterRadius);
  theBEGeBuilder->SetDitchInnerRadius(detectorDitchInnerRadius);
  theBEGeBuilder->SetDitchDepth(detectorDitchDepth);
  theBEGeBuilder->SetG4MaterialName("NaturalGe");
  theBEGeBuilder->SetDitchBelow(true);
  theBEGeBuilder->ConstructDetector();
  G4LogicalVolume *logicDet = theBEGeBuilder->GetDetectorLogical();
  logicDet -> SetVisAttributes(Gecolour);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicDet,
		    "physiDet", experimentalHall_log, false, 0);

  G4Tubs* SolidDeadLayer = new G4Tubs("SolidDeadLayer", 
				      0*mm, detectorRadius-dead_layer, dead_layer/2, 0, 360.*deg);   // dead layer up
  G4LogicalVolume *logicDeadLayer = new G4LogicalVolume(SolidDeadLayer, GeLi, "logicDeadLayer");
  logicDeadLayer -> SetVisAttributes(GeLicolour);
  new G4PVPlacement(0, G4ThreeVector(0,0,detectorHeight/2.-dead_layer/2), logicDeadLayer,
					       "physiDeadLayer", logicDet, false, 0);	
	
  G4Tubs* TubDeadLayer = new G4Tubs("TubDeadLayer", detectorRadius-dead_layer,detectorRadius,detectorHeight/2., 
				    0, 360.*deg);   // dead layer  around
  G4LogicalVolume *logicTubDeadLayer = new G4LogicalVolume(TubDeadLayer, GeLi, "logicTubDeadLayer");
  logicTubDeadLayer -> SetVisAttributes(GeLicolour);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicTubDeadLayer,
					       "physiTubDeadLayer", logicDet, false, 0);	
		
  double dead_layer2 = bottom_deadlayer; 
				   
  //two parts: outside the groove and inside the groove
  //1) inside the groove
  if (detectorDitchInnerRadius > 0)
    {
      G4Tubs* SolidDeadLayer2 =  new G4Tubs("SolidDeadLayer2",0.*mm,detectorDitchInnerRadius,dead_layer2/2,
					    0,twopi);
      
      G4LogicalVolume *logicDeadLayer2 = new G4LogicalVolume(SolidDeadLayer2, GeLi, "logicDeadLayer2");
      new G4PVPlacement(0, G4ThreeVector(0,0,-detectorHeight/2.+dead_layer2/2), 
			logicDeadLayer2,
			"physiDeadLayer2", logicDet, false, 0);
    }
  //2) outside the groove (or anyway, along the full surface)
  G4double innerDL = 0.*mm;
  if (detectorDitchInnerRadius > 0 && detectorDitchInnerRadius < detectorDitchOuterRadius && 
      detectorDitchDepth>0)
    innerDL = detectorDitchOuterRadius;
  if (innerDL > detectorRadius-dead_layer) 
    innerDL = detectorRadius-dead_layer-tolerance;
  G4Tubs* SolidDeadLayer3 =  new G4Tubs("SolidDeadLayer3",innerDL,detectorRadius-dead_layer,dead_layer2/2,
					0,twopi);
  G4LogicalVolume *logicDeadLayer3 = new G4LogicalVolume(SolidDeadLayer3, GeLi, "logicDeadLayer3");
  new G4PVPlacement(0, G4ThreeVector(0,0,-detectorHeight/2.+dead_layer2/2), 
		    logicDeadLayer3,
		    "physiDeadLayer3", logicDet, false, 0);

  //3) IN the groove, if it is the case
  if (detectorDitchDepth > 0 && detectorDitchInnerRadius > 0 && 
      detectorDitchInnerRadius < detectorDitchOuterRadius)
    {
      G4Tubs* SolidDeadLayer4 =  new G4Tubs("SolidDeadLayer4",detectorDitchInnerRadius,detectorDitchOuterRadius,dead_layer2/2,
					0,twopi);
      G4LogicalVolume *logicDeadLayer4 = new G4LogicalVolume(SolidDeadLayer4, GeLi, "logicDeadLayer4");
        new G4PVPlacement(0, G4ThreeVector(0,0,-detectorHeight/2.+dead_layer2/2+detectorDitchDepth), 
		    logicDeadLayer4,
		    "physiDeadLayer4", logicDet, false, 0);
    }

  
  // BEGe
  logicDet->SetSensitiveDetector(CrystalSD);
    
  
  //---------------------------------------------------------------------------
  // Create coincident detector, if it is the case
  //---------------------------------------------------------------------------
  if (fUseCoincidentDetector)
    {
      G4double coaxInnerRadius = 0.0*mm;
      G4double coaxRadius = 35.5*mm;
      G4double coaxHeight = 80*mm;
      G4double coaxHoleRadius = 5*mm;
      G4double coaxHoleDepth = 30.0*mm;

      G4Tubs* coaxSol1 = new G4Tubs("coaxSol1",coaxInnerRadius,coaxRadius,coaxHeight/2.,
				   0,twopi);
      G4Tubs* holeCoaxSol = new G4Tubs("coaxSolHole",coaxInnerRadius,coaxHoleRadius,
				       coaxHoleDepth/2.,0,twopi);
      G4double z_shift = -coaxHeight/2.+coaxHoleDepth/2.+1.*micrometer;
      G4SubtractionSolid* coaxSol = new G4SubtractionSolid("coaxSol",coaxSol1,holeCoaxSol,
							   0,G4ThreeVector(0,0,z_shift));
      G4LogicalVolume* coaxLog = new G4LogicalVolume(coaxSol,Ge,"coaxLog");
      G4RotationMatrix* rotMatrix = new G4RotationMatrix();
      rotMatrix->rotateY(90*deg);
      G4double distanceAlongY = 15*cm;
      new G4PVPlacement(rotMatrix, G4ThreeVector(0,distanceAlongY,0),
			coaxLog,
			"coaxDet",experimentalHall_log,false,0); 

      coaxLog->SetSensitiveDetector(CrystalSD);
    }


 

  SetDetectorLogical(experimentalHall_log);
  SetDetectorName("ExperimentalHall");
 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
