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
 * FIRST SUBMISSION: 02-23-2006
 *
 * REVISION:
 *
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

#include "munichteststand/GEMunichTestStandGERDAlinchenII.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandGERDAlinchenII::GEMunichTestStandGERDAlinchenII(GEMunichTestStandDB* teststanddb)
{

  fMotherVolume = teststanddb -> GetMotherVolume(); 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandGERDAlinchenII::~GEMunichTestStandGERDAlinchenII()
{

  delete fCanPhysical; 
  delete fVacuumPhysical; 
  delete fRodPhysical; 
  delete fCrystalDeadLayerPhysical; 
  delete fCrystalSiliconBottomPhysical; 
  delete fCrystalSiliconTopPhysical; 
} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandGERDAlinchenII::ConstructTestStand()
{
  
  // --------------------------------------------------------------------------- 
  // materials 
  // --------------------------------------------------------------------------- 

  G4Material* copper          = G4Material::GetMaterial("MetalCopper");
  G4Material* vacuum          = G4Material::GetMaterial("Vacuum");
  G4Material* enrGe           = G4Material::GetMaterial("EnrichedGe");
  G4Material* si              = G4Material::GetMaterial("MetalSilicon");
  G4Material* aluminium       = G4Material::GetMaterial("MetalAluminium"); 

  // --------------------------------------------------------------------------- 
  // colors 
  // --------------------------------------------------------------------------- 

  G4Colour red     (1.0,  0.0,  0.0); 
  G4Colour darkred (0.5,  0.0,  0.0); 
  G4Colour orange  (0.75, 0.55, 0.0);
//  G4Colour darkgrey(0.7,  0.7,  0.7); 
  G4Colour silver  (0.2,  0.2,  0.2); 
  G4Colour white   (1.0,  1.0,  1.0); 
  G4Colour lgrey   (0.8,  0.8,  0.8); 
//  G4Colour lgrey2  (0.85, 0.85, 0.85); 
//  G4Colour lblue   (0.0,  0.0,  0.75); 

  // --------------------------------------------------------------------------- 
  // visual attributes 
  // --------------------------------------------------------------------------- 

  G4VisAttributes* VacuumVisAtt = new G4VisAttributes(white); 
  VacuumVisAtt -> SetVisibility(true); 
  VacuumVisAtt -> SetForceSolid(true); 

  G4VisAttributes* LN2VisAtt = new G4VisAttributes(lgrey); 
  LN2VisAtt -> SetVisibility(true); 
  LN2VisAtt -> SetForceSolid(true); 

  G4VisAttributes* CrystalSegmentVisAtt = new G4VisAttributes(red);
  CrystalSegmentVisAtt -> SetVisibility(true);
  CrystalSegmentVisAtt -> SetForceSolid(true);

  G4VisAttributes* CrystalDeadLayerVisAtt = new G4VisAttributes(darkred);
  CrystalDeadLayerVisAtt -> SetVisibility(true);
  CrystalDeadLayerVisAtt -> SetForceSolid(true);

  G4VisAttributes* AlVisAtt = new G4VisAttributes(silver); 
  AlVisAtt -> SetVisibility(true); 
  AlVisAtt -> SetForceSolid(true); 

  G4VisAttributes* SiVisAtt = new G4VisAttributes(silver); 
  SiVisAtt -> SetVisibility(true); 
  SiVisAtt -> SetForceSolid(true); 

  G4VisAttributes* CuVisAtt = new G4VisAttributes(orange);
  CuVisAtt -> SetVisibility(true);
  CuVisAtt -> SetForceSolid(true);

  // --------------------------------------------------------------------------- 
  // variables 
  // --------------------------------------------------------------------------- 

  G4double TubsInnerRadius; 
  G4double TubsOuterRadius; 
  G4double TubsHeight; 
  G4double TubsStartAngle; 
  G4double TubsSpanningAngle; 

  G4double dPhi; 

  G4double z; 

//   G4double crystal_ir     = 0.5 * cm; 
//   G4double crystal_or     = 4.0 * cm; 
//   G4double crystal_height = 8.0 * cm; 
//   G4double crystal_it     = 600 * micrometer; 
//   G4double crystal_ot     = 0.3 * micrometer; 
//   G4double crystal_tt     = 0.3 * micrometer; 
//   G4double crystal_sit    = 0.5 * micrometer; 
//   G4double segment_height = (crystal_height - 2 * crystal_tt - 2 * crystal_sit) / 3.0; 

  // debug 
  G4double crystal_ir     = 0.5 * cm; 
  G4double crystal_or     = 4.0 * cm; 
  G4double crystal_height = 8.0 * cm; 
  G4double crystal_it     = 0.1 * cm; 
  G4double crystal_ot     = 0.1 * cm; 
  G4double crystal_tt     = 0.1 * cm; 
  G4double crystal_sit    = 0.2 * cm; 
  G4double segment_height = (crystal_height - 2 * crystal_tt - 2 * crystal_sit) / 3.0; 

  G4double can_or = crystal_or + 0.2 * cm; 
  G4double can_t  = 0.1 * cm; 
  G4double can_height = crystal_height + 0.4 * cm; 
  
  G4double rod_radius = 2.0 * cm; 
  G4double rod_height = 40.0 * cm; 

  // --------------------------------------------------------------------------- 
  // logical volumes 
  // --------------------------------------------------------------------------- 

  // can 

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = can_or; 
  TubsHeight        = 0.5 * can_height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* CanTubs = new G4Tubs("CanTubs", 
			       TubsInnerRadius,
			       TubsOuterRadius,
			       TubsHeight,
			       TubsStartAngle,
			       TubsSpanningAngle);	
			       
  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = can_or - can_t; 
  TubsHeight        = 0.5 * can_height - can_t; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* CanHoleTubs = new G4Tubs("CanHoleTubs", 
				   TubsInnerRadius,
				   TubsOuterRadius,
				   TubsHeight,
				   TubsStartAngle,
				   TubsSpanningAngle);	

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = rod_radius; 
  TubsHeight        = 0.5 * can_t; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* CanBottomHoleTubs = new G4Tubs("CanBottomHoleTubs", 
					 TubsInnerRadius,
					 TubsOuterRadius,
					 TubsHeight,
					 TubsStartAngle,
					 TubsSpanningAngle);	
  
  G4SubtractionSolid*  CanSubtractionSolid1 = 
    new G4SubtractionSolid("CanSubtractionSolid1",  
			   CanTubs, 
			   CanHoleTubs,
			   0, 
			   G4ThreeVector(0.0, 0.0, 0.0)); 

  G4SubtractionSolid*  CanSubtractionSolid2 = 
    new G4SubtractionSolid("CanSubtractionSolid2",  
			   CanSubtractionSolid1, 
			   CanBottomHoleTubs,
			   0, 
			   G4ThreeVector(0.0, 0.0, - 0.5 * (can_height - can_t))); 

  fCanLogical = new G4LogicalVolume(CanSubtractionSolid2, 
				    aluminium, 
				    "CanLogicalVolume"); 

  fCanLogical -> SetVisAttributes(AlVisAtt); 

  // vacuum 

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = can_or - can_t; 
  TubsHeight        = 0.5 * can_height - can_t; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  

  G4Tubs* VacuumTubs = new G4Tubs("VacuumTubs", 
				  TubsInnerRadius,
				  TubsOuterRadius,
				  TubsHeight,
				  TubsStartAngle,
				  TubsSpanningAngle);	

  fVacuumLogical = new G4LogicalVolume(VacuumTubs, 
				       vacuum, 
				       "VacuumLogical"); 

  fVacuumLogical -> SetVisAttributes(VacuumVisAtt); 

  // rod 

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = rod_radius; 
  TubsHeight        = 0.5 * rod_height; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
 
  G4Tubs* RodTubs = new G4Tubs("RodTubs", 
			       TubsInnerRadius,
			       TubsOuterRadius,
			       TubsHeight,
			       TubsStartAngle,
			       TubsSpanningAngle);	
  
  fRodLogical = new G4LogicalVolume(RodTubs, 
				    copper, 
				    "RodLogical"); 

  fRodLogical -> SetVisAttributes(CuVisAtt); 

  // dead layer 

  TubsInnerRadius   = crystal_ir; 
  TubsOuterRadius   = crystal_or; 
  TubsHeight        = 0.5 * crystal_height - crystal_sit; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* CrystalDeadLayerTubs = new G4Tubs("CrystalDeadLayerTubs", 
					    TubsInnerRadius,
					    TubsOuterRadius,
					    TubsHeight,
					    TubsStartAngle,
					    TubsSpanningAngle);	
	  
  TubsInnerRadius   = crystal_ir + crystal_it; 
  TubsOuterRadius   = crystal_or - crystal_ot; 
  TubsHeight        = 0.5 * crystal_height - crystal_tt;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg; 
	  
  G4Tubs* CrystalDeadLayerHoleTubs = new G4Tubs("CrystalDeadLayerHoleTubs", 
						TubsInnerRadius, 
						TubsOuterRadius, 
						TubsHeight,
						TubsStartAngle,
						TubsSpanningAngle);
	  
  G4SubtractionSolid* CrystalDeadLayerSubtractionSolid = 
    new G4SubtractionSolid("CrystalSubtractionSolid",  
			   CrystalDeadLayerTubs, 
			   CrystalDeadLayerHoleTubs,
			   0, 
			   G4ThreeVector(0.0, 0.0, 0.0)); 
  
  fCrystalDeadLayerLogical = new  G4LogicalVolume(CrystalDeadLayerSubtractionSolid,
						  enrGe, 
						  "CrystalDeadLayerLogical"); 
  
  fCrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt); 
  
  // silicon 

  TubsInnerRadius   = 0.0; 
  TubsOuterRadius   = crystal_or; 
  TubsHeight        = 0.5 * crystal_sit; 
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = 360.0 * deg;  
  
  G4Tubs* CrystalSiliconTubs = new G4Tubs("CrystalSiliconTubs", 
					  TubsInnerRadius, 
					  TubsOuterRadius, 
					  TubsHeight,
					  TubsStartAngle,
					  TubsSpanningAngle);
  
  fCrystalSiliconLogical = new G4LogicalVolume(CrystalSiliconTubs, 
					       si, 
					       "CrystalSiliconTubs"); 

  fCrystalSiliconLogical -> SetVisAttributes(SiVisAtt); 

  // segments 
	  
  dPhi = 360.0 * deg / 6.0; 
  
  TubsInnerRadius   = crystal_ir + crystal_it; 
  TubsOuterRadius   = crystal_or - crystal_ot; 
  TubsHeight        = 0.5 * segment_height;
  TubsStartAngle    = 0.0 * deg; 
  TubsSpanningAngle = dPhi; 
	  
  G4Tubs* CrystalSegmentTubs = new G4Tubs("CrystalSegmentTubs", 
					  TubsInnerRadius,
					  TubsOuterRadius,
					  TubsHeight,
					  TubsStartAngle,
					  TubsSpanningAngle);
	  
  fCrystalSegmentLogical = new G4LogicalVolume(CrystalSegmentTubs, 
					       enrGe, 
					       "CrystalSegmentLogical"); 
  
  fCrystalSegmentLogical -> SetVisAttributes(CrystalSegmentVisAtt); 
  
  // --------------------------------------------------------------------------- 
  // physical volumes 
  // --------------------------------------------------------------------------- 

  // can 

  z = 0.0; 

  fCanPhysical = new G4PVPlacement(0, 
				   G4ThreeVector(0.0, 0.0, z), 
				   fCanLogical, 
				   "can", 
				   fMotherVolume, 
				   false, 
				   0); 

  // vacuum 

  z = 0.0; 

  fVacuumPhysical = new G4PVPlacement(0, 
				      G4ThreeVector(0.0, 0.0, z), 
				      fVacuumLogical, 
				      "vacuum", 
				      fMotherVolume, 
				      false, 
				      0); 

  // rod 

  z = - 0.5 * crystal_height - 0.5 * rod_height; 

  fRodPhysical = new G4PVPlacement(0, 
				   G4ThreeVector(0.0, 0.0, z), 
				   fRodLogical, 
				   "rod", 
				   fMotherVolume, 
				   false, 
				   0); 

  // dead layer 

  z = 0.0; 

  fCrystalDeadLayerPhysical = new G4PVPlacement(0, 
						G4ThreeVector(0.0, 0.0, z), 
						fCrystalDeadLayerLogical, 
						"deadlayer", 
						fVacuumLogical, 
						false, 
						0); 
  
  // silicon 

  z = - 0.5 * (crystal_height - crystal_sit); 

  fCrystalSiliconBottomPhysical = new G4PVPlacement(0, 
						    G4ThreeVector(0.0, 0.0, z), 
						    fCrystalSiliconLogical, 
						    "siliconbottom", 
						    fVacuumLogical, 
						    false, 
						    0); 

  z = 0.5 * (crystal_height - crystal_sit); 
  
  fCrystalSiliconTopPhysical = new G4PVPlacement(0, 
						 G4ThreeVector(0.0, 0.0, z), 
						 fCrystalSiliconLogical, 
						 "silicontop", 
						 fVacuumLogical, 
						 false, 
						 1); 

  

}

// --------------------------------------------------------------------------- 
 

