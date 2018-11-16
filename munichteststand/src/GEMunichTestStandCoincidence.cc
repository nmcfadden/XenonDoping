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
 * FIRST SUBMISSION: 05-31-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 02-27-2006, REGe detector from DB as a whole, Xiang
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh" 

#include "munichteststand/GEMunichTestStandCoincidence.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandCoincidence::GEMunichTestStandCoincidence(GEMunichTestStandDB* teststanddb)
{
  fAluminumHolderHeight    = teststanddb -> GetAluminumHolderHeight(); 
  fAluminumHolderThickness = teststanddb -> GetAluminumHolderThickness(); 
  fAluminumHolderToCrystal = teststanddb -> GetAluminumHolderToCrystal(); 
  fCrystalHeight           = teststanddb -> GetCrystalHeight(); 
  fCopperFingerHeight      = teststanddb -> GetCopperFingerHeight(); 
  fREGeHeight              = teststanddb -> GetREGeHeight();
  fSourceHolderHeight      = teststanddb -> GetSourceHolderHeight(); 
  fSourceHolderRadius      = teststanddb -> GetSourceHolderRadius(); 
  fLeadBrickLengthX        = teststanddb -> GetLeadBrickLengthX(); 
  fLeadBrickLengthY        = teststanddb -> GetLeadBrickLengthY(); 
  fLeadBrickLengthZ        = teststanddb -> GetLeadBrickLengthZ(); 
  fCrystalDistanceToSource = teststanddb -> GetCrystalDistanceToSource(); 
  fSourceDistanceToBrick   = teststanddb -> GetSourceDistanceToBrick(); 
  fDistance01              = teststanddb -> GetDistance01(); 
  fMotherVolume            = teststanddb -> GetMotherVolume();
/* 
  fCrystalActiveLogical    = teststanddb -> GetCrystalActiveLogical(); 
  fCrystalDeadLayerLogical = teststanddb -> GetCrystalDeadLayerLogical(); 
  fAluminumHolderLogical   = teststanddb -> GetAluminumHolderLogical(); 
  fBerylliumWindowLogical  = teststanddb -> GetBerylliumWindowLogical(); 
  fCopperFingerLogical     = teststanddb -> GetCopperFingerLogical(); 
*/
  teststanddb -> ConstructREGeDetector(0,0);
  teststanddb -> ConstructREGeDetector(1,1);
  fREGeLogical[0]          = teststanddb -> GetREGeLogical(0); 
  fREGeLogical[1]          = teststanddb -> GetREGeLogical(1); 
  fSourceLogical           = teststanddb -> GetSourceLogical(); 
  fSourceHolderLogical     = teststanddb -> GetSourceHolderLogical(); 
  fLeadBrickLogical        = teststanddb -> GetLeadBrickLogical(); 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandCoincidence::~GEMunichTestStandCoincidence()
{
/*
  delete fCrystalActivePhysical[0]; 
  delete fCrystalDeadLayerPhysical[0]; 
  delete fAluminumHolderPhysical[0]; 
  delete fCopperFingerPhysical[0]; 
  delete fCrystalActivePhysical[1]; 
  delete fCrystalDeadLayerPhysical[1]; 
  delete fAluminumHolderPhysical[1]; 
  delete fCopperFingerPhysical[1]; 
*/
  delete fREGePhysical[0];
  delete fREGePhysical[1];
  delete fSourcePhysical; 
  delete fSourceHolderPhysical; 
  for (G4int i = 0; i < 6; i++)
    delete fLeadBrickPhysical[i]; 

} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandCoincidence::ConstructTestStand()
{

/*
  G4ThreeVector crystalposition; 
  G4ThreeVector holderposition; 
  G4ThreeVector windowposition; 
  G4ThreeVector fingerposition; 
*/
  G4ThreeVector regeposition;
  G4ThreeVector sourceposition; 
  G4ThreeVector leadposition; 

  G4RotationMatrix regerotationone;
  G4RotationMatrix regerotationtwo;
  //G4RotationMatrix crystalrotation; 
  G4RotationMatrix sourcerotation; 
  G4RotationMatrix leadrotation; 

  //  G4double x, y; 
  G4double y;

  char volumename[30]; 

/*
  // detector 0 

  // construct physical volume: crystal (active) 

  crystalrotation.rotateX(90.0 * deg); 

  crystalposition.setX(0.0 * mm); 
  crystalposition.setY(0.5 * fAluminumHolderHeight); 
  crystalposition.setZ(0.0 * mm); 

  sprintf(volumename, "crystalactive_0"); 

  fCrystalActivePhysical[0] = new G4PVPlacement(G4Transform3D(crystalrotation, crystalposition),
						fCrystalActiveLogical, 
						volumename, 
						fMotherVolume, 
						false, 
						0);

  // construct physical volume: crystal (dead layer) 

  sprintf(volumename, "crystaldeadlayer_0"); 

  fCrystalDeadLayerPhysical[0] = new G4PVPlacement(G4Transform3D(crystalrotation, crystalposition), 
						   fCrystalDeadLayerLogical, 
						   volumename, 
						   fMotherVolume, 
						   false, 
						   0); 

  // construct physical volume: holder which surrounds the crystal

  holderposition = crystalposition; 

  sprintf(volumename, "holder_0"); 
  
  fAluminumHolderPhysical[0] = new G4PVPlacement(G4Transform3D(crystalrotation, holderposition), 
						 fAluminumHolderLogical, 
						 volumename, 
						 fMotherVolume, 
						 false, 
						 0);

  // physical volume: Beryllium window

  y = crystalposition.getY() - 0.5 * fCrystalHeight - fAluminumHolderToCrystal - 0.5 * fAluminumHolderThickness;

  windowposition.setX(0.0 * mm);
  windowposition.setY(y);
  windowposition.setZ(0.0 * mm);

  sprintf(volumename, "window_0");

  fBerylliumWindowPhysical[0] = new G4PVPlacement(G4Transform3D(crystalrotation, windowposition), 
						  fBerylliumWindowLogical,
						  volumename,
						  fMotherVolume,
						  false,
						  0);

  // construct physical volume: finger is below crystal (negative Z)

  y = 0.5 * (2.0 * fAluminumHolderHeight + fCopperFingerHeight); 

  fingerposition.setX(0.0 * mm); 
  fingerposition.setY(y); 
  fingerposition.setZ(0.0 * mm); 
 
  sprintf(volumename, "finger_0"); 

  fCopperFingerPhysical[0] = new G4PVPlacement(G4Transform3D(crystalrotation, fingerposition), 
					       fCopperFingerLogical, 
					       volumename, 
					       fMotherVolume, 
					       false,
					       0); 
*/
  //-------------------------------
  // rege0 as a whole component
  //-------------------------------
  regeposition.setX(0.0 * mm);
  regeposition.setY(0.5 * fREGeHeight);
  regeposition.setZ(0.0 * mm); 
  regerotationone.rotateX(90.0 * deg);
  regerotationone.rotateY(0.0 * deg);
  regerotationone.rotateZ(0.0 * deg);

  sprintf(volumename, "rege_0");
  fREGePhysical[0] = new G4PVPlacement(G4Transform3D(regerotationone, regeposition),
                                       fREGeLogical[0], 
                                       volumename, 
                                       fMotherVolume, 
                                       false, 
                                       0);
/*
  // detector 1 

  // construct physical volume: crystal (active) 

  crystalrotation.rotateZ(- 90.0 * deg); 

  x = 0.5 * fAluminumHolderHeight + fDistance01; 

  crystalposition.setX(x); 
  crystalposition.setY(0.5 * fAluminumHolderHeight); 
  crystalposition.setZ(0.0 * mm); 

  sprintf(volumename, "crystalactive_1"); 

  fCrystalActivePhysical[1] = new G4PVPlacement(G4Transform3D(crystalrotation, crystalposition),
						fCrystalActiveLogical, 
						volumename, 
						fMotherVolume, 
						false, 
					        1);

  // construct physical volume: crystal (dead layer) 

  sprintf(volumename, "crystaldeadlayer_1"); 

  fCrystalDeadLayerPhysical[1] = new G4PVPlacement(G4Transform3D(crystalrotation, crystalposition), 
						   fCrystalDeadLayerLogical, 
						   volumename, 
						   fMotherVolume, 
						   false, 
						   1); 

  // construct physical volume: holder which surrounds the crystal

  holderposition = crystalposition; 

  sprintf(volumename, "holder_1"); 
  
  fAluminumHolderPhysical[1] = new G4PVPlacement(G4Transform3D(crystalrotation, holderposition), 
						 fAluminumHolderLogical, 
						 volumename, 
						 fMotherVolume, 
						 false, 
						 1);

  // physical volume: Beryllium window

  //  x = 0.5 * (fAluminumHolderHeight-fAluminumHolderThickness) + fDistance01;

  x = crystalposition.getX() - 0.5 * fCrystalHeight - fAluminumHolderToCrystal - 0.5 * fAluminumHolderThickness;

  windowposition.setX(x);
  windowposition.setY(0.5 * fAluminumHolderHeight);
  windowposition.setZ(0.0 * mm);

  sprintf(volumename, "window_1");

  fBerylliumWindowPhysical[1] = new G4PVPlacement(G4Transform3D(crystalrotation, windowposition), 
						  fBerylliumWindowLogical,
						  volumename,
						  fMotherVolume,
						  false,
						  1);


  // construct physical volume: finger is below crystal (negative Z)

  x = 0.5 * (2.0 * fAluminumHolderHeight + fCopperFingerHeight) + fDistance01; 

  fingerposition.setX(x); 
  fingerposition.setY(0.5 * fAluminumHolderHeight); 
  fingerposition.setZ(0.0 * mm); 
 
  sprintf(volumename, "finger_1"); 

  fCopperFingerPhysical[1] = new G4PVPlacement(G4Transform3D(crystalrotation, fingerposition), 
					       fCopperFingerLogical, 
					       volumename, 
					       fMotherVolume, 
					       false,
					       1); 
*/
  //-------------------
  // new rege 1
  //-------------------
  regeposition.setX(0.5 * fREGeHeight+fDistance01);
  regeposition.setY(fAluminumHolderHeight * 0.5);
  regeposition.setZ(0.0 * mm); 
  regerotationtwo.rotateX(0.0   * deg);
  regerotationtwo.rotateY(-90.0 * deg);
  regerotationtwo.rotateZ(0.0   * deg);

  sprintf(volumename, "rege_1");
  fREGePhysical[1] = new G4PVPlacement(G4Transform3D(regerotationtwo, regeposition),
                                       fREGeLogical[1], 
                                       volumename, 
                                       fMotherVolume, 
                                       false, 
                                       1);

  // source

  // construct physical volume: source holder

  sourcerotation.rotateY(90.0 * deg); 

  y = - fSourceHolderRadius - fCrystalDistanceToSource; 

  sourceposition.setX(0.0 * mm); 
  sourceposition.setY(y); 
  sourceposition.setZ(0.0 * mm); 

  sprintf(volumename, "sourceholder"); 
	  
  fSourceHolderPhysical = new G4PVPlacement(G4Transform3D(sourcerotation, sourceposition), 
					    fSourceHolderLogical, 
					    volumename, 
					    fMotherVolume, 
					    false, 
					    0); 

  // construct physical volume: source 

  sourceposition.setX(0.0 * mm); 
  sourceposition.setY(0.0 * mm); 
  sourceposition.setZ(0.0 * mm); 

  sprintf(volumename, "source"); 

  fSourcePhysical = new G4PVPlacement(0,
				      sourceposition, 
				      fSourceLogical, 
				      volumename, 
				      fSourceHolderLogical, 
				      false, 
				      0); 

  // construct physical volume: lead brick 0 
/*
  leadrotation.rotateY(90.0 * deg); 

  y = - 0.5 * fLeadBrickLengthZ - 2.0 * fAluminumHolderThickness; 
  x = 0.5 * fLeadBrickLengthX + 0.5 * fSourceHolderHeight; 

  leadposition.setX(x); 
  leadposition.setY(y); 
  leadposition.setZ(0.0 * mm); 

  sprintf(volumename, "leadbrick_0"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(0,
					    leadposition, 
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    0); 
*/
  // construct physical volume: lead brick 1 

//   x = 0.5 * (fAluminumHolderHeight + 3.0 * fLeadBrickLengthX); 
//   y = 0.5 * fLeadBrickLengthY; 

//   leadposition.setX(x); 
//   leadposition.setY(y); 
//   leadposition.setZ(0.0 * mm); 

//   sprintf(volumename, "leadbrick_1"); 

//   fLeadBrickPhysical[1] = new G4PVPlacement(0,
// 					    leadposition, 
// 					    fLeadBrickLogical, 
// 					    volumename, 
// 					    fMotherVolume, 
// 					    false,
// 					    1); 

 } 

// --------------------------------------------------------------------------- 

