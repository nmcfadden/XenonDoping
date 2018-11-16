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
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh" 

#include "munichteststand/GEMunichTestStandSimple.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandSimple::GEMunichTestStandSimple(GEMunichTestStandDB* teststanddb)
{

  fAluminumHolderHeight    = teststanddb -> GetAluminumHolderHeight(); 
  fAluminumHolderThickness = teststanddb -> GetAluminumHolderThickness(); 
  fAluminumHolderToCrystal = teststanddb -> GetAluminumHolderToCrystal(); 
  fCopperFingerHeight      = teststanddb -> GetCopperFingerHeight(); 
  fSourceHolderHeight      = teststanddb -> GetSourceHolderHeight(); 
  fLeadBrickLengthX        = teststanddb -> GetLeadBrickLengthX(); 
  fLeadBrickLengthY        = teststanddb -> GetLeadBrickLengthY(); 
  fLeadBrickLengthZ        = teststanddb -> GetLeadBrickLengthZ(); 
  fCrystalDistanceToSource = teststanddb -> GetCrystalDistanceToSource(); 
  fSourceDistanceToBrick   = teststanddb -> GetSourceDistanceToBrick(); 
  fMotherVolume            = teststanddb -> GetMotherVolume(); 
  fCrystalActiveLogical    = teststanddb -> GetCrystalActiveLogical(); 
  fCrystalDeadLayerLogical = teststanddb -> GetCrystalDeadLayerLogical(); 
  fAluminumHolderLogical   = teststanddb -> GetAluminumHolderLogical(); 
  fBerylliumWindowLogical  = teststanddb -> GetBerylliumWindowLogical(); 
  fCopperFingerLogical     = teststanddb -> GetCopperFingerLogical(); 
  fSourceLogical           = teststanddb -> GetSourceLogical(); 
  fSourceHolderLogical     = teststanddb -> GetSourceHolderLogical(); 
  fLeadBrickLogical        = teststanddb -> GetLeadBrickLogical(); 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandSimple::~GEMunichTestStandSimple()
{

  delete fCrystalActivePhysical; 
  delete fCrystalDeadLayerPhysical; 
  delete fAluminumHolderPhysical; 
  delete fCopperFingerPhysical; 
  delete fSourcePhysical; 
  delete fSourceHolderPhysical; 
  for (G4int i = 0; i < 6; i++)
    delete fLeadBrickPhysical[i]; 

} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandSimple::ConstructTestStand()
{

  G4ThreeVector crystalposition; 
  G4ThreeVector holderposition; 
  G4ThreeVector windowposition; 
  G4ThreeVector fingerposition; 
  G4ThreeVector sourceposition; 
  G4ThreeVector leadposition; 

  G4double x, y, z; 

  char volumename[30]; 

  // construct physical volume: crystal (active) 

  crystalposition.setX(0.0 * mm); 
  crystalposition.setY(0.0 * mm); 
  crystalposition.setZ(0.0 * mm); 

  sprintf(volumename, "crystalactive"); 

  fCrystalActivePhysical = new G4PVPlacement(0,
					     crystalposition,
					     fCrystalActiveLogical, 
					     volumename, 
					     fMotherVolume, 
					     false, 
					     0); 

  // construct physical volume: crystal (dead layer) 

  crystalposition.setX(0.0 * mm); 
  crystalposition.setY(0.0 * mm); 
  crystalposition.setZ(0.0 * mm); 

  sprintf(volumename, "crystaldeadlayer"); 

  fCrystalDeadLayerPhysical = new G4PVPlacement(0,
						crystalposition,
						fCrystalDeadLayerLogical, 
						volumename, 
						fMotherVolume, 
						false, 
						0); 

  // construct physical volume: holder which surrounds the crystal

  holderposition = crystalposition; 

  sprintf(volumename, "holder"); 
  
  fAluminumHolderPhysical = new G4PVPlacement(0,
					      holderposition, 
					      fAluminumHolderLogical, 
					      volumename, 
					      fMotherVolume, 
					      false, 
					      0);

  // physical volume: Beryllium window
  z = 0.5 * (fAluminumHolderHeight-fAluminumHolderThickness);
  windowposition.setX(0.0*mm);
  windowposition.setY(0.0*mm);
  windowposition.setZ(z);
  sprintf(volumename, "window");
  fBerylliumWindowPhysical = new G4PVPlacement(0,
                                              windowposition,
                                              fBerylliumWindowLogical,
                                              volumename,
                                              fMotherVolume,
                                              false,
                                              0);


  // construct physical volume: finger is below crystal (negative Z)

  z = -0.5 * (fAluminumHolderHeight + fCopperFingerHeight); 

  fingerposition.setX(0.0 * mm); 
  fingerposition.setY(0.0 * mm); 
  fingerposition.setZ(z); 
 
  sprintf(volumename, "finger"); 

  fCopperFingerPhysical = new G4PVPlacement(0,
					    fingerposition, 
					    fCopperFingerLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    0); 
  
  // construct physical volume: source holder

  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource;

  sourceposition.setX(0.0 * mm); 
  sourceposition.setY(0.0 * mm); 
  sourceposition.setZ(z); 

  sprintf(volumename, "sourceholder"); 
	  
  fSourceHolderPhysical = new G4PVPlacement(0,
					    sourceposition, 
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

  y = 0.5 * fLeadBrickLengthY;
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + 0.5 * fLeadBrickLengthZ;

  leadposition.setX(0.0 * mm); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_0"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(0,
					    leadposition, 
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    0); 

  // construct physical volume: lead brick 1 

  y = - 0.5 * fLeadBrickLengthY; 
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + 0.5 * fLeadBrickLengthZ;

  leadposition.setX(0.0 * mm); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_1"); 

  fLeadBrickPhysical[1] = new G4PVPlacement(0,
					    leadposition, 
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    1); 
  
   // construct physical volume: lead brick 2 

  G4double         angle; 
  G4RotationMatrix rm; 

  angle = 90.0 * deg; 
  
  rm.rotateZ(angle); 
  rm.rotateX(angle); 

  x = 0.5 * (fLeadBrickLengthX + fLeadBrickLengthY); 
  y = 0.5 * fLeadBrickLengthY; 
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + fLeadBrickLengthZ - 0.5 * fLeadBrickLengthX;;

  leadposition.setX(x); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_2"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(G4Transform3D(rm, leadposition),
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    2); 

   // construct physical volume: lead brick 3 

  angle = 90.0 * deg; 
  
  x = 0.5 * (fLeadBrickLengthX + fLeadBrickLengthY); 
  y = - 0.5 * fLeadBrickLengthY; 
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + fLeadBrickLengthZ - 0.5 * fLeadBrickLengthX;;

  leadposition.setX(x); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_3"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(G4Transform3D(rm, leadposition),
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    3); 

   // construct physical volume: lead brick 4 

  angle = 90.0 * deg; 
  
  x = - 0.5 * (fLeadBrickLengthX + fLeadBrickLengthY); 
  y = 0.5 * fLeadBrickLengthY; 
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + fLeadBrickLengthZ - 0.5 * fLeadBrickLengthX;;

  leadposition.setX(x); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_4"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(G4Transform3D(rm, leadposition),
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    4); 

   // construct physical volume: lead brick 5 

  angle = 90.0 * deg; 
  
  x = - 0.5 * (fLeadBrickLengthX + fLeadBrickLengthY); 
  y = - 0.5 * fLeadBrickLengthY; 
  z = 0.5 * (fAluminumHolderHeight + fSourceHolderHeight) 
      + fCrystalDistanceToSource + fSourceDistanceToBrick
      + fLeadBrickLengthZ - 0.5 * fLeadBrickLengthX;;

  leadposition.setX(x); 
  leadposition.setY(y); 
  leadposition.setZ(z); 

  sprintf(volumename, "leadbrick_5"); 

  fLeadBrickPhysical[0] = new G4PVPlacement(G4Transform3D(rm, leadposition),
					    fLeadBrickLogical, 
					    volumename, 
					    fMotherVolume, 
					    false,
					    5); 

 } 

// --------------------------------------------------------------------------- 

