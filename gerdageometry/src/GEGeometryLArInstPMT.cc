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
//
// CLASS IMPLEMENTATION:  GEGeometryLArInstPMT.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: Simple example implementation. See header for details.
 *
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wegmann
 * CONTACT: anne *dot* wegmann *at* mpi-hd *dot* mpg *dot* de
 * FIRST SUBMISSION: 2012-05-08, A. Wegmann
 *
 * REVISION:
 * 05-29-2012, Nuno , Commented out Teflon optical surfaces as they were not properly implemented.
 * 					  Values were also not consistent with measurements in HD.
 * 					  Added backup search path for input files (VM2000 data) pointing to the execution location.
 * 05-30-2012, Anne, Updated PMT efficiency ( QE x CE )
 * 06-04-2012, Anne, Corrected bug in PMT placement
 * 10-17-2012, Anne, Changed VM2000 to have specular reflection.
 *		     Changed index of refraction of wls from 1.15 to 1.6, added surface roughness to WLS and Teflon
 *		     Changed number of PMTs at the top from 6 to 9
 * 10-31-2012, Anne, Added construction of an external source just above the upper PMT support structure (Same as in the hybrid design).
 *
 */
//---------------------------------------------------------------------------//

#include "gerdageometry/GEGeometryLArInstPMT.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "io/MGLogger.hh"

#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

// -- ROOT include
#include "TGraph.h"

using namespace CLHEP;

GEGeometryLArInstPMT::GEGeometryLArInstPMT(GEGeometryDetectorDB* theGeometryDB) 
: fWLSoptSurf(0), fTefOptSurf(0), fMotherPhysical(0)
{ 
  fDetectorDB = theGeometryDB;
  fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
  // For the rough surfaces we also need the physical volume of the mother
  fMotherPhysical = fDetectorDB->GetCryostatParts()->GetCryoInnerwall_Inside()->GetPhysicalVolume();
  MGLog(warning) << "Logical and Physical volumes imported. Describing:" << endlog;
  MGLog(warning) << "LOG : " << fMotherLogical->GetName() << " MAT " << fMotherLogical->GetMaterial()->GetName() << endlog;
  MGLog(warning) << "PHYS : " << fMotherPhysical->GetName() << " LOG " << fMotherPhysical->GetLogicalVolume()->GetName() << endlog;
  
  successfulInitialization = false;
  InitializeVM2000Spectrum();
  return;
}

GEGeometryLArInstPMT::~GEGeometryLArInstPMT() {
  // delete all instances stored in the arrays
  for (std::vector<G4LogicalVolume*>::iterator it = fSDLogicVec.begin(); it != fSDLogicVec.end(); ++it) {
    delete *it;
  }  
}

void GEGeometryLArInstPMT::ConstructDesign() {
  // TODO: Review this geometry to be more realistic when a design is available
  
  /// Temporary variable to use in the WLS volumes to attach the rough surfaces
  G4VPhysicalVolume* wlsPhysical = NULL;
  
  G4double center_xpos   		= 0.0*mm;
  G4double center_ypos   		= 0.0*mm;
  G4double center_zpos   		= 130.*mm;

  G4double Shroud_Thickness		= 2*mm;
  G4double Shroud_TubsOuterRadius	= 250.0*mm;
  G4double Shroud_TubsInnerRadius	= Shroud_TubsOuterRadius - Shroud_Thickness;
  G4double Shroud_TubsHeight		= (2100.0)*mm; //190mm ist die für die PMTs reservierte Höhe

  G4double VM2000_Thickness		= 1*mm;
  G4double VM2000_TubsOuterRadius   	= Shroud_TubsInnerRadius;
  G4double VM2000_TubsInnerRadius   	= VM2000_TubsOuterRadius - VM2000_Thickness;
  
  G4double WLS_Thickness		= 1*mm;
  G4double WLS_TubsOuterRadius   	= VM2000_TubsInnerRadius;
  G4double WLS_TubsInnerRadius   	= WLS_TubsOuterRadius - WLS_Thickness;
  
  G4double TeflonPlate_OuterRadius	= Shroud_TubsOuterRadius; 	//250.0*mm;
  G4double TeflonPlate_InnerRadius	= 0.0*mm;
  G4double TeflonPlate_Height		= 5*mm;
  
  G4double WLS_Plate_InnerRadius	= 0.0*mm;
  G4double WLS_Plate_OuterRadius	= Shroud_TubsOuterRadius; 	//250.0*mm;
  
  G4double CalSourceHole_Radius		= 50*mm;
  
  G4double PMT_Radius			= (3*25.4)/2.*mm;
  G4double PMT_Height			= 123.*mm;
  G4double PMT_MountingRadius		= 55*mm + PMT_Radius;  	//TeflonPlate_OuterRadius-100.*mm-PMT_Radius;
  G4double PMT_OuterMountingRadius	= TeflonPlate_OuterRadius-25.*mm-PMT_Radius;
  
  G4double PMTHousing_Thickness		= 0.6*mm;
  G4double PMTGlass_Thickness		= 3.4*mm;

  
  //Copper shroud
  G4VSolid* fShroudSolid = new G4Tubs("PMTInstr_ShroudSolid",
			    Shroud_TubsInnerRadius,
			    Shroud_TubsOuterRadius,
			    Shroud_TubsHeight/2.,
			    0,
			    2*M_PI);
  G4Material* fMaterialShroud = G4Material::GetMaterial("MetalCopper");

  G4LogicalVolume* fShroudLogical = new G4LogicalVolume(fShroudSolid,
				       fMaterialShroud,
				       "LArInstr_ShroudLogical");				       
  /*G4VPhysicalVolume* fShroudPhysical = */new G4PVPlacement(0,
				      G4ThreeVector(center_xpos,center_ypos,center_zpos),
				      fShroudLogical,
				      "LArInstr_ShroudPhysical",
				      fMotherLogical,
				      false,
				      0);


  //VM2000 reflector foil inside the copper shroud
  G4VSolid* fVM2000CylinderSolid = new G4Tubs("LArInstr_VM2000CylinderSolid",
				    VM2000_TubsInnerRadius,
				    VM2000_TubsOuterRadius,
				    Shroud_TubsHeight/2.,
				    0,
				    2*M_PI);			  
  G4Material* fMaterialVM2000 = G4Material::GetMaterial("VM2000");

  G4LogicalVolume* fVM2000CylinderLogical = new G4LogicalVolume(fVM2000CylinderSolid,
					       fMaterialVM2000,
					       "LArInstr_VM2000CylinderLogical");					       
  /*fVM2000CylinderPhysical = */new G4PVPlacement(0,
					      G4ThreeVector(center_xpos,center_ypos,center_zpos),
					      fVM2000CylinderLogical,
					      "LArInstr_VM2000CylinderPhysical",
					      fMotherLogical,
					      false,
					      0);
  
  //WLS coating on VM2000 foil
  G4VSolid* fWLSCylinderSolid = new G4Tubs("LArInstr_WLSCylinderSolid",
				    WLS_TubsInnerRadius,
				    WLS_TubsOuterRadius,
				    Shroud_TubsHeight/2.,
				    0,
				    2*M_PI);			  
  G4Material* fMaterialWLS = G4Material::GetMaterial("Nylon");

  G4LogicalVolume* fWLSCylinderLogical = new G4LogicalVolume(fWLSCylinderSolid,
					       fMaterialWLS,
					       "LArInstr_WLSCylinderLogical");					       
  wlsPhysical = new G4PVPlacement(0,
				  G4ThreeVector(center_xpos,center_ypos,center_zpos),
				  fWLSCylinderLogical,
				  "LArInstr_WLSCylinderPhysical",
				  fMotherLogical,
				  false,
				  0);
					      
  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);
					      
  
  /// Teflon plates with holes for the PMTs + WLS coating
  ///
  /// *************** bottom Plate: *************************
  /// 1 hole in the center, 6 holes distributed uniformly on a circle with radius "PMT_MountingRadius"
  G4VSolid* fTeflonPlateSolid = new G4Tubs("TeflonPlateSolid",
					   TeflonPlate_InnerRadius,
					   TeflonPlate_OuterRadius,
					   TeflonPlate_Height/2.,
					   0,
					   2*M_PI);
			  
  G4VSolid* fTeflonPlate_hole = new G4Tubs("TeflonPlate_hole",
					   0.,
					   PMT_Radius,
					   TeflonPlate_Height/2.,
					   0,
					   2*M_PI);
					   
  G4VSolid* fWLS_PlateSolid = new G4Tubs("WLSPlateSolid",
					   WLS_Plate_InnerRadius,
					   WLS_Plate_OuterRadius,
					   WLS_Thickness/2.,
					   0,
					   2*M_PI);
			  
  G4VSolid* fWLS_Plate_hole = new G4Tubs("WLSPlate_hole",
					   0.,
					   PMT_Radius,
					   WLS_Thickness/2.,
					   0,
					   2*M_PI);

  //holes for calibration sources in the top plate
  G4VSolid* fTeflonPlate_calhole = new G4Tubs("TeflonPlate_calhole",
					   0.,
					   CalSourceHole_Radius,
					   TeflonPlate_Height/2.,
					   0,
					   2*M_PI);
			  
  G4VSolid* fWLS_Plate_calhole = new G4Tubs("WLSPlate_calhole",
					   0.,
					   CalSourceHole_Radius,
					   WLS_Thickness/2.,
					   0,
					   2*M_PI);
					   
  char solidname[100], solidname_WLS[100];
  G4VSolid* current_TeflonPlate1_solid = fTeflonPlateSolid;
  G4VSolid* current_WLSPlate1_solid = fWLS_PlateSolid;
  for (int i = 0; i < 7; i++) 
  {
    G4VSolid* fTeflonPlate1_nHoles = NULL;
    G4VSolid* fWLSPlate1_nHoles = NULL;
    sprintf(solidname, "solid_TeflonPlate_%dhole", i + 1);
    sprintf(solidname_WLS, "solid_WLSPlate_%dhole", i + 1);
    if(i== 0){
      fTeflonPlate1_nHoles = new G4SubtractionSolid("solidname",
						    current_TeflonPlate1_solid, 
						    fTeflonPlate_hole, 
						    0, 
						    G4ThreeVector(0., 0., 0.));
								   
      fWLSPlate1_nHoles = new G4SubtractionSolid("solidname_WLS",
						 current_WLSPlate1_solid, 
						 fWLS_Plate_hole, 
						 0, 
						 G4ThreeVector(0., 0., 0.));
    }
    else if (i>=1 && i<7){
      fTeflonPlate1_nHoles = new G4SubtractionSolid("solidname",
						    current_TeflonPlate1_solid,
						    fTeflonPlate_hole, 
						    0, 
						    G4ThreeVector((PMT_MountingRadius*cos(((G4double)i -1)*360*deg/6. + 30*deg)), (PMT_MountingRadius*sin(((G4double)i -1)*360*deg/6. + 30*deg)), 0.) );
								   
      fWLSPlate1_nHoles = new G4SubtractionSolid("solidname_WLS",
						 current_WLSPlate1_solid,
						 fWLS_Plate_hole, 
						 0, 
						 G4ThreeVector((PMT_MountingRadius*cos(((G4double)i -1)*360*deg/6. + 30*deg)), (PMT_MountingRadius*sin(((G4double)i -1)*360*deg/6. + 30*deg)), 0.) );						
    }

    current_TeflonPlate1_solid = fTeflonPlate1_nHoles;
    current_WLSPlate1_solid = fWLSPlate1_nHoles;
  }
  G4VSolid* fTeflonPlateSolid_bottom = current_TeflonPlate1_solid;
  G4VSolid* fWLSPlateSolid_bottom = current_WLSPlate1_solid;


  G4Material* fMaterialTeflonPlate = G4Material::GetMaterial("Teflon");
  G4LogicalVolume* fTeflonPlateLogical_bottom = new G4LogicalVolume(fTeflonPlateSolid_bottom,
					     fMaterialTeflonPlate,
					     "LArInstr_TeflonPlateLogical_bottom");					     
  /*fTeflonPlatePhysical_bottom = */new G4PVPlacement(0,
					    G4ThreeVector(center_xpos,center_ypos,(center_zpos-Shroud_TubsHeight/2. -WLS_Thickness - TeflonPlate_Height/2.)),
					    fTeflonPlateLogical_bottom,
					    "LArInstr_TeflonPlatePhysical_bottom",
					    fMotherLogical,
					    false,
					    0);
					    
  G4LogicalVolume* fWLSPlateLogical_bottom = new G4LogicalVolume(fWLSPlateSolid_bottom,
					     fMaterialWLS,
					     "LArInstr_WLSPlateLogical_bottom");					     
  wlsPhysical = new G4PVPlacement(0,
				  G4ThreeVector(center_xpos,center_ypos,(center_zpos-Shroud_TubsHeight/2.-WLS_Thickness/2.)),
				  fWLSPlateLogical_bottom,
				  "LArInstr_WLSPlatePhysical_bottom",
				  fMotherLogical,
				  false,
				  0);

  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_bottom_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_bottom_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);
  
  // Surface of the teflon
  new G4LogicalSkinSurface("LArInstr_TeflonPlatePhysical_bottom_surf", fTeflonPlateLogical_bottom,fTefOptSurf);

  
  /// *************** top plate: *************** 
  /// 6 PMTs (+ WLS coating) distributed uniformly on a circle with radius "PMT_MountingRadius"  
  /// 3 PMTs (+ WLS coating) on a circle with radius "PMT_OuterMountingRadius"  
  G4VSolid* current_TeflonPlate2_solid = fTeflonPlateSolid;
  G4VSolid* current_WLSPlate2_solid = fWLS_PlateSolid;
  for (int i = 0; i < 12; i++) {
    G4VSolid* fTeflonPlate2_nHoles = NULL;
    G4VSolid* fWLSPlate2_nHoles = NULL;
    sprintf(solidname, "solid_TeflonPlate_%dhole", i+1);
    sprintf(solidname_WLS, "solid_WLSPlate_%dhole", i+1);
    if(i<6){
    fTeflonPlate2_nHoles = new G4SubtractionSolid("solidname",
						  current_TeflonPlate2_solid,
						  fTeflonPlate_hole, 
						  0, 
						  G4ThreeVector(PMT_MountingRadius*cos((G4double)i*360*deg/6. + 30*deg), PMT_MountingRadius*sin((G4double)i*360*deg/6. +30*deg), 0.) );
								   
    fWLSPlate2_nHoles = new G4SubtractionSolid("solidname_WLS",
					       current_WLSPlate2_solid,
					       fWLS_Plate_hole, 
					       0, 
					       G4ThreeVector(PMT_MountingRadius*cos((G4double)i*360*deg/6. + 30*deg), PMT_MountingRadius*sin((G4double)i*360*deg/6. + 30*deg), 0.) );
    }
    else if(i>= 6 &&i <9){
    fTeflonPlate2_nHoles = new G4SubtractionSolid("solidname",
						  current_TeflonPlate2_solid,
						  fTeflonPlate_hole, 
						  0, 
						  G4ThreeVector(PMT_OuterMountingRadius*cos(((G4double)i-6)*360*deg/3. + 30*deg), PMT_OuterMountingRadius*sin(((G4double)i-6)*360*deg/3. + 30*deg), 0.) );
								   
    fWLSPlate2_nHoles = new G4SubtractionSolid("solidname_WLS",
					       current_WLSPlate2_solid,
					       fWLS_Plate_hole, 
					       0, 
					       G4ThreeVector(PMT_OuterMountingRadius*cos(((G4double)i-6)*360*deg/3. + 30*deg), PMT_OuterMountingRadius*sin(((G4double)i-6)*360*deg/3. + 30*deg), 0.) );
    }
    else{
    fTeflonPlate2_nHoles = new G4SubtractionSolid("solidname",
						  current_TeflonPlate2_solid,
						  fTeflonPlate_calhole, 
						  0, 
						  G4ThreeVector(PMT_OuterMountingRadius*cos(((G4double)i-9)*360*deg/3. + 90*deg), PMT_OuterMountingRadius*sin(((G4double)i-9)*360*deg/3. + 90*deg), 0.) );
								   
    fWLSPlate2_nHoles = new G4SubtractionSolid("solidname_WLS",
						current_WLSPlate2_solid,
						fWLS_Plate_calhole, 
						0, 
						G4ThreeVector(PMT_OuterMountingRadius*cos(((G4double)i-9)*360*deg/3. + 90*deg), PMT_OuterMountingRadius*sin(((G4double)i-9)*360*deg/3. + 90*deg), 0.) );
    }
    current_TeflonPlate2_solid = fTeflonPlate2_nHoles;
    current_WLSPlate2_solid = fWLSPlate2_nHoles;
  }
  G4VSolid* fTeflonPlateSolid_top = current_TeflonPlate2_solid;
  G4VSolid* fWLSPlateSolid_top = current_WLSPlate2_solid;
  

  G4LogicalVolume* fTeflonPlateLogical_top = new G4LogicalVolume(fTeflonPlateSolid_top,
					     fMaterialTeflonPlate,
					     "LArInstr_TeflonPlateLogical_top");					     
  /*fTeflonPlatePhysical_top = */new G4PVPlacement(0,
					    G4ThreeVector(center_xpos,center_ypos,(center_zpos+Shroud_TubsHeight/2. + WLS_Thickness + TeflonPlate_Height/2.)),
					    fTeflonPlateLogical_top,
					    "LArInstr_TeflonPlatePhysical_top",
					    fMotherLogical,
					    false,
					    0);
					    
					    
  /// External source to study backgrounds in the cable chain.
  // Consider simply a 5*mm cylinder of copper placed just above the teflon plate
  // The thickness is to ensure that any beta from the source is killed.
  if (fDetectorDB->GetLArInstConstructExtSource()) {
    G4Colour light_gray(214./255.,214./255.,214./255.);
    G4Colour dark_gray(135./255.,135./255.,135./255.);
    
    G4VSolid *ext_source_outer_solid = new G4Tubs("ExtSourceSolidOuter",
                                                  0.0,
                                                  5*mm,
                                                  5*mm/2.,
                                                  0,
                                                  2*M_PI);

    G4VSolid *ext_source_inner_solid = new G4Tubs("ExtSourceSolidInner",
                                                  0.0,
                                                  1*mm,
                                                  1*mm/2.,
                                                  0,
                                                  2*M_PI);

    G4LogicalVolume* fExtSourceOuterLogical = new G4LogicalVolume(ext_source_outer_solid,
                                                                  G4Material::GetMaterial("MetalCopper"),
                                                                  "LArInstr_ExtSourceOuter_Logical");

    G4LogicalVolume* fExtSourceInnerLogical = new G4LogicalVolume(ext_source_inner_solid,
                                                                  G4Material::GetMaterial("Steel"),
                                                                  "LArInstr_ExtSourceInner_Logical");

    // Now place the source right above the teflon plate
    new G4PVPlacement(0,
                      G4ThreeVector(center_xpos,center_ypos,(center_zpos +Shroud_TubsHeight/2. + WLS_Thickness + TeflonPlate_Height+2.5*mm)),
                      fExtSourceOuterLogical,
                      "LArInstr_ExtSourceOuter_Physical",
                      fMotherLogical,
                      false,
                      0);

    new G4PVPlacement(0,
                      G4ThreeVector(0.0,0.0,0.0),
                      fExtSourceInnerLogical,
                      "LArInstr_ExtSourceInner_Physical",
                      fExtSourceOuterLogical,
                      false,
                      0);

    G4VisAttributes*source_visatt_outer = new G4VisAttributes(light_gray);
    source_visatt_outer -> SetVisibility(true);
    source_visatt_outer -> SetForceSolid(false);
    fExtSourceOuterLogical-> SetVisAttributes(source_visatt_outer);

    G4VisAttributes*source_visatt_inner = new G4VisAttributes(dark_gray);
    source_visatt_inner -> SetVisibility(true);
    source_visatt_inner -> SetForceSolid(false);
    fExtSourceInnerLogical-> SetVisAttributes(source_visatt_inner);
  }
					    
  G4LogicalVolume* fWLSPlateLogical_top = new G4LogicalVolume(fWLSPlateSolid_top,
					     fMaterialWLS,
					     "LArInstr_WLSPlateLogical_top");					     
  wlsPhysical = new G4PVPlacement(0,
				  G4ThreeVector(center_xpos,center_ypos,(center_zpos+Shroud_TubsHeight/2. + WLS_Thickness/2.)),
				  fWLSPlateLogical_top,
				  "LArInstr_WLSPlatePhysical_top",
				  fMotherLogical,
				  false,
				  0);
					    
  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_top_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_top_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);
  
  // Surface of the teflon
  new G4LogicalSkinSurface("LArInstr_TeflonPlatePhysical_top_surf", fTeflonPlateLogical_top,fTefOptSurf);
			    
					    
  /// PMTs + WLS coating
  G4VSolid* fPMT_OuterVolume = new G4Tubs("PMT_OuterVolume",
					  0,
					  PMT_Radius,
					  PMT_Height/2.,
					  0,
					  2*M_PI);			  
  G4VSolid* fPMT_InnerVolume = new G4Tubs("PMT_InnerVolume",
					  0.,
					  (PMT_Radius-PMTHousing_Thickness),
					  0.5*(PMT_Height-PMTHousing_Thickness),
					  0,
					  2*M_PI);
  G4VSolid* fPMT_housing = new G4SubtractionSolid("PMT_housing", 
						  fPMT_OuterVolume, 
						  fPMT_InnerVolume, 
						  0, 
						  G4ThreeVector(0., 0., -PMTHousing_Thickness/2.));
						  
  G4Material* fMaterialPMTHousing = G4Material::GetMaterial("Kovar");
  G4LogicalVolume* fPMTHousingLogical = new G4LogicalVolume(fPMT_housing,
					     fMaterialPMTHousing,
					     "LArInstr_PMTHousingLogical");  
  char PMTHousingPhysical[50];
  
  G4VSolid* fPMT_glass = new G4Tubs("PMT_glass",
			  0,
			  PMT_Radius,
			  PMTGlass_Thickness/2.,
			  0,
			  2*M_PI);
			  
  G4Material* fMaterialPMTGlass = G4Material::GetMaterial("Quartz"); 
  G4LogicalVolume* fPMTGlassLogical = new G4LogicalVolume(fPMT_glass,
					   fMaterialPMTGlass,
					   "LArInstr_PMTGlassLogical");					   
  char PMTGlassPhysical[50];
  
  G4VSolid* fPMT_WLS = new G4Tubs("PMT_WLS",
			  0,
			  PMT_Radius,
			  WLS_Thickness/2.,
			  0,
			  2*M_PI);
			  
  G4LogicalVolume* fPMTGlassWLS_Logical = new G4LogicalVolume(fPMT_WLS,
					   fMaterialWLS,
					   "LArInstr_PMTGlassWLS_Logical");					   
  char PMTGlassWLS_Physical[50];
  
  for (int i = 0; i < 16; i++) {
    sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
    sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
    sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
    G4RotationMatrix* x_rot_180 = new G4RotationMatrix;
    x_rot_180 -> rotateX(M_PI);
    if (i <6){
      /*fPMTHousingPhysical = */new G4PVPlacement(0,
					      G4ThreeVector( (center_xpos + PMT_MountingRadius*cos((G4double) i*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin((G4double) i*360*deg/6. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2. + WLS_Thickness + PMTGlass_Thickness +PMT_Height/2.)),
					      fPMTHousingLogical,
					      PMTHousingPhysical,
					      fMotherLogical,
					      false,
					      i);
      /*fPMTGlassPhysical = */new G4PVPlacement(0,
					    G4ThreeVector( (center_xpos + PMT_MountingRadius*cos((G4double) i*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin((G4double) i*360*deg/6. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2. + WLS_Thickness + PMTGlass_Thickness/2.)),
					    fPMTGlassLogical,
					    PMTGlassPhysical,
					    fMotherLogical,
					    false,
					    i);
      wlsPhysical = new G4PVPlacement(0,
				      G4ThreeVector( (center_xpos + PMT_MountingRadius*cos((G4double) i*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin((G4double) i*360*deg/6. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2.+ WLS_Thickness/2.)),
				      fPMTGlassWLS_Logical,
				      PMTGlassWLS_Physical,
				      fMotherLogical,
				      false,
				      i);
    }
    else if (i>=6 && i<9){
      /*fPMTHousingPhysical = */new G4PVPlacement(0,
					      G4ThreeVector( (center_xpos + PMT_OuterMountingRadius*cos(((G4double)i-6)*360*deg/3. + 30*deg)), (center_ypos + PMT_OuterMountingRadius*sin(((G4double)i-6)*360*deg/3. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2. + WLS_Thickness + PMTGlass_Thickness +PMT_Height/2.)),
					      fPMTHousingLogical,
					      PMTHousingPhysical,
					      fMotherLogical,
					      false,
					      i);
      /*fPMTGlassPhysical = */new G4PVPlacement(0,
					    G4ThreeVector( (center_xpos + PMT_OuterMountingRadius*cos(((G4double)i-6)*360*deg/3. + 30*deg)), (center_ypos + PMT_OuterMountingRadius*sin(((G4double)i-6)*360*deg/3. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2. + WLS_Thickness + PMTGlass_Thickness/2.)),
					    fPMTGlassLogical,
					    PMTGlassPhysical,
					    fMotherLogical,
					    false,
					    i);
      wlsPhysical = new G4PVPlacement(0,
				      G4ThreeVector( (center_xpos + PMT_OuterMountingRadius*cos(((G4double)i-6)*360*deg/3. + 30*deg)), (center_ypos + PMT_OuterMountingRadius*sin(((G4double)i-6)*360*deg/3. + 30*deg)) ,(center_zpos + Shroud_TubsHeight/2.+ WLS_Thickness/2.)),
				      fPMTGlassWLS_Logical,
				      PMTGlassWLS_Physical,
				      fMotherLogical,
				      false,
				      i);
    }    
    else if (i==9){
	/*fPMTHousingPhysical = */new G4PVPlacement(x_rot_180,
					      G4ThreeVector( (center_xpos), (center_ypos) ,(center_zpos-Shroud_TubsHeight/2. - WLS_Thickness - PMTGlass_Thickness - PMT_Height/2.)),
					      fPMTHousingLogical,
					      PMTHousingPhysical,
					      fMotherLogical,
					      false,
					      i);
	/*fPMTGlassPhysical = */new G4PVPlacement(/*x_rot_180*/0,
					      G4ThreeVector( (center_xpos), (center_ypos) ,(center_zpos-Shroud_TubsHeight/2. - WLS_Thickness -PMTGlass_Thickness/2.)),
					      fPMTGlassLogical,
					      PMTGlassPhysical,
					      fMotherLogical,
					      false,
					      i);					      
	wlsPhysical = new G4PVPlacement(/*x_rot_180*/0,
					G4ThreeVector( (center_xpos), (center_ypos) ,(center_zpos-Shroud_TubsHeight/2.- WLS_Thickness/2.)),
					fPMTGlassWLS_Logical,
					PMTGlassWLS_Physical,
					fMotherLogical,
					false,
					i);
      }
      else if (i>= 10 && i< 16){
	/*fPMTHousingPhysical = */new G4PVPlacement(x_rot_180,
						G4ThreeVector( (center_xpos + PMT_MountingRadius*cos(((G4double)i-10)*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin(((G4double)i-10)*360*deg/6. + 30*deg)), (center_zpos - Shroud_TubsHeight/2. - WLS_Thickness - PMTGlass_Thickness -PMT_Height/2.) ),
						fPMTHousingLogical,
						PMTHousingPhysical,
						fMotherLogical,
						false,
						i);
	/*fPMTGlassPhysical = */new G4PVPlacement(/*x_rot_180*/0,
					      G4ThreeVector( (center_xpos + PMT_MountingRadius*cos(((G4double)i-10)*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin(((G4double)i-10)*360*deg/6. + 30*deg)), (center_zpos - Shroud_TubsHeight/2. - WLS_Thickness - PMTGlass_Thickness/2.) ),
					      fPMTGlassLogical,
					      PMTGlassPhysical,
					      fMotherLogical,
					      false,
					      i);
	wlsPhysical = new G4PVPlacement(/*x_rot_180*/0,
					G4ThreeVector( (center_xpos + PMT_MountingRadius*cos(((G4double)i-10)*360*deg/6. + 30*deg)), (center_ypos + PMT_MountingRadius*sin(((G4double)i-10)*360*deg/6. + 30*deg)), (center_zpos - Shroud_TubsHeight/2. - WLS_Thickness/2.) ),
					fPMTGlassWLS_Logical,
					PMTGlassWLS_Physical,
					fMotherLogical,
					false,
					i);
      }
      
    // consider the surface rough from both sides
    new G4LogicalBorderSurface(PMTGlassWLS_Physical + G4String("_surf_in"),
                               fMotherPhysical,
                               wlsPhysical,
                               fWLSoptSurf);

    new G4LogicalBorderSurface(PMTGlassWLS_Physical + G4String("_surf_out"),
                               wlsPhysical,
                               fMotherPhysical,
                               fWLSoptSurf);
  }


  
  /// Attach a series of optical properties to the detector otherwise the photons will die at the boundary and no registered hit is done
  /// ---------------------------------------------------***************************-------------------------------------------------------------
  
  G4int ji = 0;
  G4double ee=0;
  const G4int num = 500;
  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(650*nanometer); //598
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
    {
      ee=PPSCLowE+ ((G4double)ji) * dee;
      LAr_SCPP[ji]=ee;
    }


  /// Reflector (VM2000)
  //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm
  //and ~15% below it (almost a step-function)

  G4double Reflectivity[num];
  //G4double Efficiency[num];

  for (ji=0;ji < num; ji++)
    {
      if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
	  Reflectivity[ji] = 0.98; //visible
      else
	  Reflectivity[ji] = 0.15; //UV

      //Efficiency[ji] = 0.0;
    }
  G4MaterialPropertiesTable *vmOpTable = new G4MaterialPropertiesTable();
  vmOpTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);
//   	  reflOptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num);

  // Follow method used in WArP:

  G4OpticalSurface* reflOptSurface = new G4OpticalSurface("VM_surface");
  
  new G4LogicalSkinSurface("VM_surface",fVM2000CylinderLogical,reflOptSurface);
  reflOptSurface->SetType(dielectric_dielectric);
  // The reflection should be either spike or Lobe.
  reflOptSurface->SetFinish(polishedfrontpainted); 
  reflOptSurface->SetMaterialPropertiesTable(vmOpTable);
    
/// Optical Properties of WLS
/// -- copied from GEGSLArGeOptical.cc
///

  G4double WLS_absorption[num];
  G4double WLS_emission[num];
  G4double Refraction[num];
//   	  G4double Rayl[num];
  G4MaterialPropertiesTable *tpbTable = new G4MaterialPropertiesTable();
  for (ji=0;ji < num; ji++)
    {
      Refraction[ji] = 1.6; //this is just a guess
//  	      Rayl[ji] = 10*um;
      if (LAr_SCPP[ji] > 8*eV)
	  // For the moment set it to always absorb photons
	  WLS_absorption[ji] = 0.001*nm; //absorbs UV (always)
      else
	  WLS_absorption[ji] = 1000.*m; //otherwise imperturbed

      WLS_emission[ji] = VM2000EmissionSpectrum(LAr_SCPP[ji]);
      /*
      MGLog(routine) << "VM2000:  WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Energy :" << LAr_SCPP[ji]/eV << " eV Emission : " <<
      WLS_emission[ji] << " Absorption : " << WLS_absorption[ji]/m << " m."<< endlog;
      */
    }
  tpbTable->AddProperty("RINDEX",LAr_SCPP,Refraction,num);
  tpbTable->AddProperty("WLSABSLENGTH",LAr_SCPP,WLS_absorption,num);
  tpbTable->AddProperty("WLSCOMPONENT",LAr_SCPP,WLS_emission,num);
  // From WArP
  tpbTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
  WLSyield = 1.2;
  tpbTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSyield);
  fMaterialWLS->SetMaterialPropertiesTable(tpbTable); 
  
  
  // Define a rough optical surface to be used in the interface between WLS and LAr
  // 50% roughness in the surface
  // This surface will be attached between the WLS and the LAr in all instances
  G4double roughness = 0.5;
  fWLSoptSurf = new G4OpticalSurface("WLS_rough_surf",glisur,ground,dielectric_dielectric,roughness);

  // Create a similar surface for Teflon parts
  fTefOptSurf = new G4OpticalSurface("Teflon_rough_surface",glisur,ground,dielectric_metal,roughness);
  // Attach the properties that are currently attached to the Teflon material
  fTefOptSurf->SetMaterialPropertiesTable(G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable());
  
  
  /// Optical properties of the PMT glass: 
  const G4int num_entries = 24;
  G4double PMTGlassEnergy[num_entries] = {LambdaE/(115*nanometer), LambdaE /(160.8*nanometer), LambdaE /(170*nanometer), LambdaE /(182.3*nanometer), LambdaE /(200*nanometer), 
					  LambdaE/(230*nanometer), LambdaE /(250*nanometer), LambdaE /(270*nanometer), LambdaE /(300*nanometer), LambdaE /(320*nanometer),
					  LambdaE/(340*nanometer), LambdaE /(360*nanometer), LambdaE /(380*nanometer), LambdaE /(400*nanometer), LambdaE /(450*nanometer),
					  LambdaE/(500*nanometer), LambdaE /(550*nanometer), LambdaE /(580*nanometer), LambdaE /(600*nanometer), LambdaE /(630*nanometer),
					  LambdaE/(660*nanometer), LambdaE /(700*nanometer), LambdaE /(730*nanometer), LambdaE /(750*nanometer)};  
  G4double PMTGlassReflectivity[num_entries] = {0.0,0.0,0.0,0.0,0.0,
						0.0,0.0,0.0,0.0,0.0,
						0.0,0.0,0.0,0.0,0.0,
						0.0,0.0,0.0,0.0};
  G4double PMTGlassEfficiency[num_entries] = {0.0000,0.0034,0.0322,0.0741,0.1297,
					      0.1450,0.1673,0.1965,0.2348,0.2473,
					      0.2467,0.2399,0.2368,0.2264,0.1847,
					      0.1309,0.0692,0.0371,0.0231,0.0104,
					      0.0036,0.0006,0.0001,0.0000}; 
  
  G4MaterialPropertiesTable *PMTGlassOptTable = new G4MaterialPropertiesTable();
  PMTGlassOptTable->AddProperty("REFLECTIVITY",PMTGlassEnergy,PMTGlassReflectivity,num_entries);
  PMTGlassOptTable->AddProperty("EFFICIENCY",PMTGlassEnergy,PMTGlassEfficiency,num_entries);
  
  fMaterialPMTGlass->SetMaterialPropertiesTable(PMTGlassOptTable);
  
  G4OpticalSurface* PMTGlassOptSurface = new G4OpticalSurface("LArInstrPMTGlassSurface",glisur,polished,dielectric_metal);
  PMTGlassOptSurface->SetMaterialPropertiesTable(PMTGlassOptTable);
   
  new G4LogicalSkinSurface("PMTGlass_surf",fPMTGlassLogical,PMTGlassOptSurface);
  
  fSDLogicVec.push_back(fPMTGlassLogical);
  fSDLogicNameVec.push_back("PMTGlass");
  fSDLogicInstanceVec.push_back(16);
  
  
  // visualization attributes
  G4Colour lgreen	(0.0,  0.4, 0.0) ;
  G4Colour lblue	(0.0,  0.0, 0.4) ;
  G4Colour llblue	(0.,  0.0, 0.04) ;
  G4Colour blue_gray	(175/255. ,157/255. ,189/255. ) ;
  G4Colour lred		(0.4,  0.0, 0.0) ;
    
  G4VisAttributes* fShroudVisAtt = new G4VisAttributes(lgreen);
  fShroudVisAtt -> SetVisibility(true);
  fShroudVisAtt -> SetForceSolid(false);
  fShroudLogical-> SetVisAttributes(fShroudVisAtt);
  
  G4VisAttributes* fVM2000VisAtt = new G4VisAttributes(lblue);
  fVM2000VisAtt -> SetVisibility(true);
  fVM2000VisAtt -> SetForceSolid(false);
  fVM2000CylinderLogical-> SetVisAttributes(fVM2000VisAtt);
  
  G4VisAttributes* fWLSVisAtt = new G4VisAttributes(llblue);
  fWLSVisAtt -> SetVisibility(true);
  fWLSVisAtt -> SetForceSolid(false);
  fWLSCylinderLogical-> SetVisAttributes(fWLSVisAtt);
  
  G4VisAttributes* fTeflonVisAtt = new G4VisAttributes(lblue);
  fTeflonVisAtt -> SetVisibility(true);
  fTeflonVisAtt -> SetForceSolid(false);
  fTeflonPlateLogical_bottom-> SetVisAttributes(fTeflonVisAtt);
  fTeflonPlateLogical_top-> SetVisAttributes(fTeflonVisAtt);
  
  G4VisAttributes* fPMTHousingVisAtt = new G4VisAttributes(lred);
  fPMTHousingVisAtt -> SetVisibility(true);
  fPMTHousingVisAtt -> SetForceSolid(false);
  fPMTHousingLogical-> SetVisAttributes(fPMTHousingVisAtt);
  
  G4VisAttributes* fPMTGlassVisAtt = new G4VisAttributes(blue_gray);
  fPMTGlassVisAtt -> SetVisibility(true);
  fPMTGlassVisAtt -> SetForceSolid(false);
  fPMTGlassLogical-> SetVisAttributes(fPMTGlassVisAtt);
  
}

G4LogicalVolume *GEGeometryLArInstPMT::GetLArInstrSDLogical(G4int index) {
	return fSDLogicVec.at(index);
}
G4int GEGeometryLArInstPMT::GetNumSDLogicVols() {
	return fSDLogicVec.size();
}
G4String GEGeometryLArInstPMT::GetLogicalSDName(G4int index) {
	return fSDLogicNameVec.at(index);
}
G4int GEGeometryLArInstPMT::GetNLogicalInstances(G4int index) {
	return fSDLogicInstanceVec.at(index);
}


//copied from GEGSLArGeOptical.cc
void GEGeometryLArInstPMT::InitializeVM2000Spectrum()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
    {
      MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
      //path = "/d1/neutrino/luciano/MaGe/generators/data";
      path = (char*) ".";
    }
  G4String pathString(path);
  G4String pathFile = pathString + "/VM2000_em_spec.dat";
  fVM2000spec = new TGraph(pathFile.data());
  if (fVM2000spec->GetN() > 0 ) {
	  successfulInitialization = true;
	  MGLog(routine) << "VM2000 re-emission spectrum ( " << fVM2000spec->GetN()
			  << " points) successfully loaded from file." << endlog;
	  /** Uncomment for debugging purposes
	  fVM2000spec->Print();
	  */
  } else {
	  successfulInitialization = false;
	  MGLog(warning) << "VM2000 re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
  }

  /*
  std::ifstream file(pathFile);
  if (file.is_open())
    {
      G4double aa,bb,cc,dd,ee,ff,gg,hh;
      for (G4int i=0;i<npoints;i++)
        {
          file >> aa >> bb >> cc >> dd >> ee >> ff >> gg >> hh;
          frequencyV[i] = gg*nanometer;
          efficiencyV[i] = hh;
        }
      file.close();
      successfulInitialization = true;
      MGLog(routine) << "VM2000 data successfully read from file" << endlog;
    }
  else
    {
      MGLog(warning) << "Data file not found!" << endlog;
      successfulInitialization = false;
    }
    */
}

//copied from GEGSLArGeOptical.cc
G4double GEGeometryLArInstPMT::VM2000EmissionSpectrum(G4double energy)
{
  //G4int j=0;
  if (successfulInitialization)
    {
      //Here the data are loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;

      /*
      if (targetf < frequencyV[0] || targetf > frequencyV[npoints-1])
        return 0.0;

      for(G4int j=0;j<npoints-1;j++)
        {
          if (frequencyV[j]<targetf && targetf <= frequencyV[j+1]) break;
        }
      G4double eff = (targetf-frequencyV[j])
        *(efficiencyV[j+1]-efficiencyV[j])/(frequencyV[j+1]-frequencyV[j]);
      eff += efficiencyV[j];
      */
      G4double eff =fVM2000spec->Eval(targetf/nm);
      if (eff < 0.2) eff = 0.2;
      //MGLog(routine) << "Eval ("<< targetf/nm<< ")yielded a value of " << eff << endlog;
      return eff;
    }
  else
    {
      return 0.2;
    }
}

// G4double GEGeometryLArInstPMT::CalculateWLSmfp(G4double yield)
// {
//   G4double totalPath = 1*mm; //hardcoded, for now
//   if (yield < 0. || yield > 1.) 
//     {
//       MGLog(warning) << "Yield cannot be larger than 100%!" << endlog;
//       MGLog(warning) << "Yield scaled to 100%" << endlog;
//     }
//   if (yield == 0) return 10.*m;
//   if (yield == 1) return 0.01*mm;
//   G4double help = std::log(1.-yield);
//   G4double mfp = -1*totalPath/help;
//   return mfp;
// }

