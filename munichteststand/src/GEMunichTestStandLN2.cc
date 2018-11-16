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
 * 05-31-2005, Kevin, added DSG detector 
 * 10-21-2005, Kevin, added additional shielding material 
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

#include "munichteststand/GEMunichTestStandLN2.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandLN2::GEMunichTestStandLN2(GEMunichTestStandDB* teststanddb)
{

  fMotherVolume                          = teststanddb -> GetMotherVolume(); 
  fCrystalDSGUnsegmentedActiveLogical    = teststanddb -> GetCrystalDSGUnsegmentedActiveLogical(); 
  fCrystalDSGUnsegmentedDeadLayerLogical = teststanddb -> GetCrystalDSGUnsegmentedDeadLayerLogical(); 
  fSourceHolderAEALogical                = teststanddb -> GetSourceHolderAEALogical(); 
  fSourceAEALogical                      = teststanddb -> GetSourceAEALogical(); 
  fStringDetectorLogical                 = teststanddb -> GetStringDetectorLogical();
  fStringSourceLogical                   = teststanddb -> GetStringSourceLogical(); 
  fDewarLogical                          = teststanddb -> GetDewarLogical(); 
  fLN2Logical                            = teststanddb -> GetLN2Logical(); 
  fCrystalHolderDSGUnsegmentedRing1Logical  = teststanddb -> GetCrystalHolderDSGUnsegmentedRing1Logical();
  fCrystalHolderDSGUnsegmentedRing2Logical  = teststanddb -> GetCrystalHolderDSGUnsegmentedRing2Logical(); 
  fCrystalHolderDSGUnsegmentedSkrewLogical  = teststanddb -> GetCrystalHolderDSGUnsegmentedSkrewLogical(); 
  fCrystalHolderDSGUnsegmentedBarLogical    = teststanddb -> GetCrystalHolderDSGUnsegmentedBarLogical(); 
  fCrystalHolderDSGUnsegmentedStringLogical = teststanddb -> GetCrystalHolderDSGUnsegmentedStringLogical(); 
  fCrystalHolderDSGUnsegmentedPlateLargeLogical = teststanddb -> GetCrystalHolderDSGUnsegmentedPlateLargeLogical(); 
  fCrystalHolderDSGUnsegmentedPlateSmallLogical = teststanddb -> GetCrystalHolderDSGUnsegmentedPlateSmallLogical(); 
  fCrystalHolderDSGUnsegmentedRodLogical        = teststanddb -> GetCrystalHolderDSGUnsegmentedRodLogical(); 
  fCrystalHolderDSGUnsegmentedClampLogical      = teststanddb -> GetCrystalHolderDSGUnsegmentedClampLogical(); 
  
  fSkrewPosition0                        = teststanddb -> GetDSGHolderSkrewPosition(0); 
  fSkrewPosition1                        = teststanddb -> GetDSGHolderSkrewPosition(1); 
  fSkrewPosition2                        = teststanddb -> GetDSGHolderSkrewPosition(2); 
  fCrystalHolderDSGUnsegmentedGap        = teststanddb -> GetCrystalHolderDSGUnsegmentedGap(); 
  fCrystalHolderDSGUnsegmentedRing1Height = teststanddb -> GetCrystalHolderDSGUnsegmentedRing1Height(); 
  fCrystalHolderDSGUnsegmentedRing2Height = teststanddb -> GetCrystalHolderDSGUnsegmentedRing2Height(); 
  fCrystalHolderDSGUnsegmentedSkrewHeight = teststanddb -> GetCrystalHolderDSGUnsegmentedSkrewHeight(); 
  fCrystalHolderDSGUnsegmentedBarLengthX  = teststanddb -> GetCrystalHolderDSGUnsegmentedBarLengthX(); 
  fCrystalHolderDSGUnsegmentedBarLengthY  = teststanddb -> GetCrystalHolderDSGUnsegmentedBarLengthY(); 
  fCrystalHolderDSGUnsegmentedBarLengthZ  = teststanddb -> GetCrystalHolderDSGUnsegmentedBarLengthZ(); 
  fCrystalHolderDSGUnsegmentedPlateSmallHeight = teststanddb -> GetCrystalHolderDSGUnsegmentedPlateSmallHeight(); 
  fCrystalHolderDSGUnsegmentedRodHeight   = teststanddb -> GetCrystalHolderDSGUnsegmentedRodHeight(); 
  fCrystalHolderDSGUnsegmentedClampHeight = teststanddb -> GetCrystalHolderDSGUnsegmentedClampHeight(); 

  fCrystalHeight           = teststanddb -> GetCrystalDSGUnsegmentedHeight(); 
  fSourceHoleNumber        = teststanddb -> GetSourceHoleNumber(); 
  fSourceHeightAEA         = teststanddb -> GetSourceHeightAEA(); 
  fDewarHeight             = teststanddb -> GetDewarHeight(); 
  fLN2Height               = teststanddb -> GetLN2Height(); 
  fSourceHolderHeightAEA   = teststanddb -> GetSourceHolderHeightAEA(); 
  fStringDetectorHeight    = teststanddb -> GetStringDetectorHeight(); 
  fStringSourceHeight      = teststanddb -> GetStringSourceHeight(); 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandLN2::~GEMunichTestStandLN2()
{

  delete fDewarPhysical; 
  delete fLN2Physical; 
  delete fCrystalActivePhysical; 
  delete fCrystalDeadLayerPhysical; 
  delete fSourceAEAPhysical; 
  delete fSourceHolderAEAPhysical; 
  delete fStringDetectorPhysical; 
  delete fStringSourcePhysical; 
  delete fCrystalHolderDSGUnsegmentedUpperRing1Physical; 
  delete fCrystalHolderDSGUnsegmentedUpperRing2Physical; 
  delete fCrystalHolderDSGUnsegmentedLowerRing1Physical; 
  delete fCrystalHolderDSGUnsegmentedLowerRing2Physical; 
  delete fCrystalHolderDSGUnsegmentedSkrew1Physical; 
  delete fCrystalHolderDSGUnsegmentedSkrew2Physical; 
  delete fCrystalHolderDSGUnsegmentedSkrew3Physical; 
  delete fCrystalHolderDSGUnsegmentedBarPhysical; 
  delete fCrystalHolderDSGUnsegmentedString1Physical; 
  delete fCrystalHolderDSGUnsegmentedString2Physical; 
  delete fCrystalHolderDSGUnsegmentedString3Physical;    

} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandLN2::ConstructTestStand()
{

  G4ThreeVector dewarposition; 
  G4ThreeVector ln2position; 
  G4ThreeVector crystalposition; 
  G4ThreeVector sourceposition; 

  G4double z; 

  char volumename[30]; 

  // construct physical volume: dewar 

  dewarposition.setX(0.0 * mm); 
  dewarposition.setY(0.0 * mm); 
  dewarposition.setZ(0.0 * mm); 

  sprintf(volumename, "dewar"); 

  fDewarPhysical = new G4PVPlacement(0,
				     dewarposition,
				     fDewarLogical, 
				     volumename, 
				     fMotherVolume, 
				     false, 
				     0); 


  // construct physical volume: lN2 

  z = 0.5 * (fLN2Height - fDewarHeight); 

  ln2position.setX(0.0 * mm); 
  ln2position.setY(0.0 * mm); 
  ln2position.setZ(z); 

  sprintf(volumename, "ln2"); 
  
  fLN2Physical = new G4PVPlacement(0,
				   ln2position, 
				   fLN2Logical, 
				   volumename, 
				   fMotherVolume, 
				   false, 
				   0);


  // construct physical volume: crystal (active) 

  G4double d2 = fStringDetectorHeight - 0.5 * (fLN2Height - fDewarHeight); 
  z = 0.5 * fLN2Height - 0.5 * fCrystalHeight - d2 - 10.0 * cm;  

  crystalposition.setX(0.0 * mm); 
  crystalposition.setY(0.0 * mm); 
  crystalposition.setZ(z); 

  sprintf(volumename, "crystalactive"); 

  fCrystalActivePhysical = new G4PVPlacement(0,
					     crystalposition,
					     fCrystalDSGUnsegmentedActiveLogical, 
					     volumename, 
					     fLN2Logical, 
					     false, 
					     0); 


  // construct physical volume: crystal (dead layer) 

  crystalposition.setX(0.0 * mm); 
  crystalposition.setY(0.0 * mm); 
  crystalposition.setZ(z); 

  sprintf(volumename, "crystaldeadlayer"); 

  fCrystalDeadLayerPhysical = new G4PVPlacement(0,
						crystalposition,
						fCrystalDSGUnsegmentedDeadLayerLogical, 
						volumename, 
						fLN2Logical, 
						false, 
						0); 


  // ------------------------------------------------
  // construct holder 
  // ------------------------------------------------

  // upper ring 1

  G4ThreeVector holderposition = crystalposition; 

  holderposition.setZ(holderposition.getZ() 
		      + 0.5 * fCrystalHeight 
		      + fCrystalHolderDSGUnsegmentedGap 
		      - 0.5 * fCrystalHolderDSGUnsegmentedRing1Height); 
  
  fCrystalHolderDSGUnsegmentedUpperRing1Physical = new G4PVPlacement(0, 
								    holderposition, 
								    fCrystalHolderDSGUnsegmentedRing1Logical, 
								    "CrystalHolderDSGUnsegmentedUpperRing1Physical", 
								    fLN2Logical, 
								    false, 
								    0); 

  // upper ring 2 

  holderposition = crystalposition; 

  holderposition.setZ(holderposition.getZ() 
		      + 0.5 * fCrystalHeight 
		      + fCrystalHolderDSGUnsegmentedGap 
		      + 0.5 * fCrystalHolderDSGUnsegmentedRing2Height); 

  fCrystalHolderDSGUnsegmentedUpperRing2Physical = new G4PVPlacement(0, 
								    holderposition, 
								    fCrystalHolderDSGUnsegmentedRing2Logical, 
								    "CrystalHolderDSGUnsegmentedUpperRing2Physical", 
								    fLN2Logical, 
								    false, 
								    0); 

  // lower ring 1 

  holderposition = crystalposition; 

  holderposition.setZ(holderposition.getZ() 
		      - 0.5 * fCrystalHeight 
		      - fCrystalHolderDSGUnsegmentedGap 
		      + 0.5 * fCrystalHolderDSGUnsegmentedRing1Height); 

  fCrystalHolderDSGUnsegmentedLowerRing1Physical = new G4PVPlacement(0, 
								    holderposition, 
								    fCrystalHolderDSGUnsegmentedRing1Logical, 
								    "CrystalHolderDSGUnsegmentedLowerRing1Physical", 
								    fLN2Logical, 
								    false, 
								    0); 

  // lower ring 2 

  holderposition = crystalposition; 

  holderposition.setZ(holderposition.getZ() 
		      - 0.5 * fCrystalHeight 
		      - fCrystalHolderDSGUnsegmentedGap 
		      - 0.5 * fCrystalHolderDSGUnsegmentedRing2Height); 

  fCrystalHolderDSGUnsegmentedLowerRing2Physical = new G4PVPlacement(0, 
								    holderposition, 
								    fCrystalHolderDSGUnsegmentedRing2Logical, 
								    "CrystalHolderDSGUnsegmentedLowerRing2Physical", 
								    fLN2Logical, 
								    false, 
								    0); 

  // skrew 1 

  holderposition = fSkrewPosition0; 
  
  holderposition.setZ(crystalposition.getZ() 
		      - 0.5 * fCrystalHeight 
		      - fCrystalHolderDSGUnsegmentedGap 
		      - fCrystalHolderDSGUnsegmentedRing2Height
		      + 0.5 * fCrystalHolderDSGUnsegmentedSkrewHeight); 

  fCrystalHolderDSGUnsegmentedSkrew1Physical = new G4PVPlacement(0,
								 holderposition, 
								 fCrystalHolderDSGUnsegmentedSkrewLogical, 
								 "CrystalHolderDSGUnsegmentedSkrew1Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // skrew 2 

  holderposition = fSkrewPosition1; 
  
  holderposition.setZ(crystalposition.getZ() 
		      - 0.5 * fCrystalHeight 
		      - fCrystalHolderDSGUnsegmentedGap 
		      - fCrystalHolderDSGUnsegmentedRing2Height
		      + 0.5 * fCrystalHolderDSGUnsegmentedSkrewHeight); 

  fCrystalHolderDSGUnsegmentedSkrew2Physical = new G4PVPlacement(0,
								 holderposition, 
								 fCrystalHolderDSGUnsegmentedSkrewLogical,
								 "CrystalHolderDSGUnsegmentedSkrew2Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // skrew 3 

  holderposition = fSkrewPosition2; 
  
  holderposition.setZ(crystalposition.getZ() 
		      - 0.5 * fCrystalHeight 
		      - fCrystalHolderDSGUnsegmentedGap 
		      - fCrystalHolderDSGUnsegmentedRing2Height
		      + 0.5 * fCrystalHolderDSGUnsegmentedSkrewHeight); 

  fCrystalHolderDSGUnsegmentedSkrew3Physical = new G4PVPlacement(0,
								 holderposition, 
								 fCrystalHolderDSGUnsegmentedSkrewLogical, 
								 "CrystalHolderDSGUnsegmentedSkrew3Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // bar

  holderposition = crystalposition; 
  
  holderposition.setZ(crystalposition.getZ() 
		      + 0.5 * fCrystalHeight 
		      + fCrystalHolderDSGUnsegmentedGap 
		      + fCrystalHolderDSGUnsegmentedRing2Height
		      + 0.5 * fCrystalHolderDSGUnsegmentedBarLengthZ); 

  fCrystalHolderDSGUnsegmentedBarPhysical = new G4PVPlacement(0, 
							      holderposition, 
							      fCrystalHolderDSGUnsegmentedBarLogical, 
							      "CrystalHolderDSGUnsegmentedBarPhysical", 
							      fLN2Logical, 
							      false, 
							      0); 

  // small plate 

  G4ThreeVector plateposition = holderposition; 
  plateposition.setZ(crystalposition.getZ() 
		     + 0.5 * fCrystalHeight 
		     + fCrystalHolderDSGUnsegmentedGap 
		     + fCrystalHolderDSGUnsegmentedRing2Height
		     + fCrystalHolderDSGUnsegmentedClampHeight);  
		     
  fCrystalHolderDSGUnsegmentedPlateSmallPhysical = new G4PVPlacement(0, 
								     plateposition, 
								     fCrystalHolderDSGUnsegmentedPlateSmallLogical, 
								     "CrystalHolderDSGUnsegmentedPlateSmallPhysical", 
								     fLN2Logical, 
								     false, 
								     0); 

  // large plate 

  plateposition.setZ(plateposition.getZ() 
		     + 2.0 * cm); 
		     
  fCrystalHolderDSGUnsegmentedPlateLargePhysical = new G4PVPlacement(0, 
								     plateposition, 
								     fCrystalHolderDSGUnsegmentedPlateLargeLogical, 
								     "CrystalHolderDSGUnsegmentedPlateLargePhysical", 
								     fLN2Logical, 
								     false, 
								     0); 

  // rod 

  plateposition.setZ(crystalposition.getZ() 
		     + 0.5 * fCrystalHeight 
		     + fCrystalHolderDSGUnsegmentedGap 
		     + fCrystalHolderDSGUnsegmentedRing2Height
		     + fCrystalHolderDSGUnsegmentedClampHeight
		     + 0.5 * fCrystalHolderDSGUnsegmentedRodHeight
		     + 0.5 * fCrystalHolderDSGUnsegmentedPlateSmallHeight); 
		     
  fCrystalHolderDSGUnsegmentedRodPhysical = new G4PVPlacement(0, 
							      plateposition, 
							      fCrystalHolderDSGUnsegmentedRodLogical, 
							      "CrystalHolderDSGUnsegmentedRodPhysical", 
							      fLN2Logical, 
							      false, 
							      0); 
  
  // clamp 1 

  holderposition = crystalposition; 
  
  holderposition.setZ(crystalposition.getZ() 
		     + 0.5 * fCrystalHeight 
		     + fCrystalHolderDSGUnsegmentedGap 
		     + fCrystalHolderDSGUnsegmentedRing2Height
		     + fCrystalHolderDSGUnsegmentedClampHeight
		     - 0.5 * fCrystalHolderDSGUnsegmentedClampHeight); 

  G4RotationMatrix rm1; 

  rm1.rotateZ(0.0 * rad); 

  fCrystalHolderDSGUnsegmentedClamp1Physical = new G4PVPlacement(G4Transform3D(rm1, holderposition), 
								 fCrystalHolderDSGUnsegmentedClampLogical, 
								 "CrystalHolderDSGUnsegmentedClamp1Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // clamp 2 

  holderposition = crystalposition; 
  
  holderposition.setZ(crystalposition.getZ() 
		     + 0.5 * fCrystalHeight 
		     + fCrystalHolderDSGUnsegmentedGap 
		     + fCrystalHolderDSGUnsegmentedRing2Height
		     + fCrystalHolderDSGUnsegmentedClampHeight
		     - 0.5 * fCrystalHolderDSGUnsegmentedClampHeight); 

  G4RotationMatrix rm2; 

  rm2.rotateZ(2.0 * 3.1416 / 3.0 * rad); 

  fCrystalHolderDSGUnsegmentedClamp2Physical = new G4PVPlacement(G4Transform3D(rm2, holderposition), 
								 fCrystalHolderDSGUnsegmentedClampLogical,
								 "CrystalHolderDSGUnsegmentedClamp2Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // clamp 3 

  holderposition = crystalposition; 
  
  holderposition.setZ(crystalposition.getZ() 
		     + 0.5 * fCrystalHeight 
		     + fCrystalHolderDSGUnsegmentedGap 
		     + fCrystalHolderDSGUnsegmentedRing2Height
		     + fCrystalHolderDSGUnsegmentedClampHeight
		     - 0.5 * fCrystalHolderDSGUnsegmentedClampHeight); 

  G4RotationMatrix rm3; 

  rm3.rotateZ(2.0 * 3.1416 / 3.0 * 2.0 *  rad); 

  fCrystalHolderDSGUnsegmentedClamp3Physical = new G4PVPlacement(G4Transform3D(rm3, holderposition), 
								 fCrystalHolderDSGUnsegmentedClampLogical, 
								 "CrystalHolderDSGUnsegmentedClamp3Physical", 
								 fLN2Logical, 
								 false, 
								 0); 

  // ------------------------------------------------
  // construct source 
  // ------------------------------------------------


  // construct physical volume: source holder

  sourceposition = GetSourcePosition(fSourceHoleNumber); 

  sprintf(volumename, "sourceholder"); 
	  
  fSourceHolderAEAPhysical = new G4PVPlacement(0,
					       sourceposition, 
					       fSourceHolderAEALogical, 
					       volumename, 
					       fLN2Logical, 
					       false, 
					       0); 

  // construct physical volume: source 

  sourceposition.setX(0.0 * mm); 
  sourceposition.setY(0.0 * mm); 
  sourceposition.setZ(- 0.5 * fSourceHolderHeightAEA + 0.5 * fSourceHeightAEA + 1.2 * mm); 

  sprintf(volumename, "source"); 

  fSourceAEAPhysical = new G4PVPlacement(0,
					 sourceposition, 
					 fSourceAEALogical, 
					 volumename, 
					 fSourceHolderAEALogical, 
					 false, 
					 0); 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEMunichTestStandLN2::GetSourcePosition(G4int n)
{
  
  G4ThreeVector positions[5]; 
  G4ThreeVector holeposition; 

  G4double d2 = fStringSourceHeight - 0.5 * (fLN2Height - fDewarHeight); 
  G4double z = 0.5 * fLN2Height - 0.5 * fCrystalHeight - d2;  

  positions[0] = G4ThreeVector( 7.5 * cm,  0.0 * cm, z); 
  positions[1] = G4ThreeVector( 0.0 * cm,  7.5 * cm, z); 
  positions[2] = G4ThreeVector( 7.5 * cm,  0.0 * cm, z); 
  positions[3] = G4ThreeVector(-7.5 * cm,  0.0 * cm, z); 
  positions[4] = G4ThreeVector( 0.0 * cm, -7.5 * cm, z); 
  
  if (n >=0 && n < 5)
    holeposition = positions[n]; 

  else 
    holeposition = positions[0]; 

  return holeposition; 

}

// --------------------------------------------------------------------------- 
 


