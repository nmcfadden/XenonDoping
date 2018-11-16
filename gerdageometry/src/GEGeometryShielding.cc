//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
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
// $Id: GEGeometryShielding.cc,v 1.20 2008-05-02 11:22:58 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * this is the class for the whole Gerda enviroument setup
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Xiang Liu
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 11-09-2004, Chaged the name of "theDetectorLogical", Luciano
 * 11-12-2004, add the copper and water shielding, Xiang
    now the set up is as following:
    crystal array:
     3*7 Ge crystals with number 0 at the very center
      horizontal gap 1cm, vertical gap 5cm
    shielding materials:
     name     inner-radius(cm)   outer-radius(cm)  height(cm)  position        
  liquid-N2    0                  194              500          center
  copper       195                198              500          center
  water        0                  500              900          center
Cu-plate (up)  49.5               198              3            z=450+1.5
Cu-plate (down)0                  198              3            z=-450-1.5
lead-plate(up)  49.5              198              3            z=453+1.5
lead-plate(down)0                 198              3            z=-453-1.5
 *
 * 11-12-2004, moved element and material definition to 
               GEGerdaLocalMaterialTable
 * 11-30-2004, Registered water and nitrogen as sensitive detectors, Luciano
 * 12-06-2004, C.Tomei, changed material names according to the new
                        GerdaLocalMaterialTable
               C.Tomei, added plastic scintillator for muon detection 
                        (sensitive)
               C.Tomei, define AirBox as the most external logical volume
 * 12-14-2004  C.Tomei, completely new design of the shielding geometry
                        according to the proposal 
 * 12-15-2004  C.Tomei, changed position of the scintillation plate
 * 01-24-2005  added method ConstructMotherVolume, Kevin
 * 02-16-2005  Set AirBox invisible, Luciano 
 * 02-18-2005  Added Steel water tank, Claudia
 * 03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 * 04-06-2005  Fixed bug with the LN2/LAr flag (it was always LN), Luciano
 * 05-09-2005  Changed height of the water tank from 9 m to 8.5 m, Claudia
 * 07-12-2005  Update to new cryotank design. Now neck is longer and 
 *             cryovessel is not centered in the water tank, Luciano
 * 07-22-2005  Added air in the neck, above the nitrogen, Luciano
 * 06-05-2006  Cryostat changed from copper to steel. Dimensions taken from a 
 *             design provided by K.Tasso, still preliminary. Claudia and Assunta
 * 08-10-2007  -Put steel plate from wrong position (outside the lock) into the 
 *             lock
 *             -take out the copper plate from the lock
 *             (Jens)
 * 12-05-2007 Cryostat is now one object  (Jens)
 *            - neck Cu layer, and so on belong to this object
 *            - makrolon layer has been added, addition of a further layer should be easy
 *            - the former neck radius was to big by a factor of two
 *            - further little changes
 *            - all parts belonging to the Cryostat should now belong to the class "GEGeometryCryostatParts"
 * 12-10-2007 Put a flange with right radii between collar and bellow above neck of cryostat (Jens)
 * 04-29-2008 Start to include rock of Hall A. Luciano
 * 11-08-2011 fSuperStructureBeamGridPhysical potision changed to correspond to the "new" fSuperStructureBeamGridLogical, Alex.
 *
 */
//---------------------------------------------------------------------------//
//
// G4 includes
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// MG include
#include "io/MGLogger.hh"

// Gerda include

#include "gerdageometry/GEGeometryShielding.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

GEGeometryShielding::GEGeometryShielding(GEGeometryDetectorDB* detectordb) 
{

  fCryostatParts              = detectordb -> GetCryostatParts(); 

  fHallSizeHeight                      = detectordb -> GetHallSizeHeight();
  fShieldingWaterTankHeight            = detectordb -> GetShieldingWaterTankHeight(); 
  fShieldingWaterTankThickness         = detectordb -> GetShieldingWaterTankThickness(); 
  fShieldingWaterHeight                = detectordb -> GetShieldingWaterHeight(); 
  fShieldingAirBufferHeight            = detectordb -> GetShieldingAirBufferHeight(); 
  fShieldingFootHeight                 = detectordb -> GetShieldingFootHeight(); 
  fShieldingCopperShieldingLargeHeight = detectordb -> GetShieldingCopperShieldingLargeHeight(); 
  fShieldingCopperShieldingSmallHeight = detectordb -> GetShieldingCopperShieldingSmallHeight(); 
  fCleanRoomHeight                     = detectordb -> GetCleanRoomHeight(); 
  fCleanRoomThickness                  = detectordb -> GetCleanRoomThickness(); 
  fSuperStructureHeight                = detectordb -> GetSuperStructureHeight(); 
  fSuperStructureCollarHeight          = detectordb -> GetSuperStructureCollarHeight(); 
  fSuperStructureBellowHeight          = detectordb -> GetSuperStructureBellowHeight(); 
  fSuperStructureFlangeHeight          = detectordb -> GetSuperStructureFlangeHeight(); 
  fSSFlangeBellowToCollarHeight        = detectordb -> GetSSFlangeBellowToCollarHeight(); 

  fOuterLockHeight                     = detectordb -> GetOuterLockHeight(); 
  fInnerLockHeight                     = detectordb -> GetInnerLockHeight(); 
  fLockCopperPlateHeight               = detectordb -> GetLockCopperPlateHeight(); 
  fLockSteelPlateHeight                = detectordb -> GetLockSteelPlateHeight(); 

  fLockSteelPlateZPosRelToLock         = detectordb -> GetLockSteelPlateZPosRelToLock(); 

  fExternalRockLogical         = detectordb -> GetExternalRockLogical();
  fAirBoxLogical               = detectordb -> GetAirBoxLogical(); 
  fWaterTankLogical            = detectordb -> GetWaterTankLogical(); 
  fWaterLogical                = detectordb -> GetWaterLogical(); 
  fAirBufferLogical            = detectordb -> GetAirBufferLogical(); 
  fFootLogical                 = detectordb -> GetFootLogical(); 
  fCopperShieldingLargeLogical = detectordb -> GetCopperShieldingLargeLogical(); 
  fCopperShieldingSmallLogical = detectordb -> GetCopperShieldingSmallLogical();  
  fCleanRoomLogical            = detectordb -> GetCleanRoomLogical(); 
  fSuperStructureCollarLogical = detectordb -> GetSuperStructureCollarLogical(); 
  fSuperStructureBellowLogical = detectordb -> GetSuperStructureBellowLogical(); 
  fSuperStructureFlangeLogical = detectordb -> GetSuperStructureFlangeLogical(); 
  fSSFlangeBellowToCollarLogical = detectordb -> GetSSFlangeBellowToCollarLogical(); 
  fSuperStructureBeamGridLogical = detectordb -> GetSuperStructureBeamGridLogical(); 
  fOuterLockLogical            = detectordb -> GetOuterLockLogical(); 
  fInnerLockLogical            = detectordb -> GetInnerLockLogical(); 
  fLockCopperPlateLogical      = detectordb -> GetLockCopperPlateLogical(); 
  fLockSteelPlateLogical       = detectordb -> GetLockSteelPlateLogical(); 

}

// ---------------------------------------------------------------------------

GEGeometryShielding::~GEGeometryShielding()
{

}

// ---------------------------------------------------------------------------

void GEGeometryShielding::ConstructShielding()
{
  
  G4double tankoffset = 0.5 * (fShieldingWaterTankHeight - 2.0 * fShieldingWaterTankThickness - fShieldingWaterHeight); 
  
  //hall A
  fAirBoxPhysical = new G4PVPlacement(0,G4ThreeVector(),
				      fAirBoxLogical,
				      "HallA",
				      fExternalRockLogical,
				      false,0);

  // water tank
  G4double x = 0.0; 
  G4double y = 0.0;
  G4double z = tankoffset;

  G4ThreeVector position; 

  position = G4ThreeVector(x, y, z); 

  fWaterTankPhysical = new G4PVPlacement(0, 
					 position, 
					 fWaterTankLogical, 
					 "WaterTankPhysical", 
					 fAirBoxLogical, 
					 false, 
					 0); 

  // water 

  x = 0.0; 
  y = 0.0; 
  z = 0.0; 

  position = G4ThreeVector(x, y, z); 

  fWaterPhysical = new G4PVPlacement(0, 
				     position, 
				     fWaterLogical, 
				     "WaterPhysical", 
				     fAirBoxLogical, 
				     false, 
				     0); 

  // air buffer 

  x = 0.0; 
  y = 0.0; 
  z = 0.5 * (fShieldingWaterHeight + fShieldingAirBufferHeight);  

  position = G4ThreeVector(x, y, z); 

  fAirBufferPhysical = new G4PVPlacement(0, 
					 position, 
					 fAirBufferLogical, 
					 "AirBufferPhysical", 
					 fAirBoxLogical, 
					 false, 
					 0); 


  // Cryostat
  //    - outer steel wall
  //    - insulation gap
  //    - Layer2 on inner steel wall // <-- this is not an extra Volume
  //    - Layer1 on inner steel wall
  //    - inner steel wall
  //    - cryo liquid
  // ----------------------------------------------------------------------
  MGLog(debugging) << "STart to create physical volumes of Cryostat" << endlog;
  x = 0.0; 
  y = 0.0; 
  z = 0.0; 
  position = G4ThreeVector(x, y, z); 
  fCryostatParts->CreatePhysicalVolumes(fAirBoxLogical,position);
  MGLog(debugging) << "Pysical Volumes of Cryostat are created "  << endlog;

  // foot 

  x = 0.0; 
  y = 0.0; 
  z = - 0.5*fShieldingWaterHeight + fShieldingCopperShieldingLargeHeight + 0.5*fShieldingFootHeight;
  
  position = G4ThreeVector(x, y, z); 
  
  fFootPhysical = new G4PVPlacement(0, 
				    position, 
				    fFootLogical, 
				    "FootPhysical", 
				    fWaterLogical, 
				    false,
				    0); 


  // large copper shielding 

  x = 0.0; 
  y = 0.0; 
  z = - 0.5 * fShieldingWaterHeight + 0.5 * fShieldingCopperShieldingLargeHeight; 

  position = G4ThreeVector(x, y, z); 

  fCopperShieldingLargePhysical = new G4PVPlacement(0, 
						    position, 
						    fCopperShieldingLargeLogical, 
						    "CopperShieldingLargePhysical", 
						    fWaterLogical, 
						    false,
						    0); 
  
  // small copper shielding 

  x = 0.0; 
  y = 0.0; 
  z = - 0.5 * fShieldingWaterHeight + fShieldingCopperShieldingLargeHeight + 0.5 * fShieldingCopperShieldingSmallHeight; 

  position = G4ThreeVector(x, y, z); 

  fCopperShieldingSmallPhysical = new G4PVPlacement(0, 
						    position, 
						    fCopperShieldingSmallLogical, 
						    "CopperShieldingSmallPhysical", 
						    fWaterLogical, 
						    false,
						    0); 
  
  // clean room 

  x = 0.0 * m; 
  y = 0.0 * m; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureHeight + 0.5 * fCleanRoomHeight; 

  position = G4ThreeVector(x, y, z); 

  fCleanRoomPhysical = new G4PVPlacement(0, 
					 position, 
					 fCleanRoomLogical, 
					 "CleanRoomPhysical", 
					 fAirBoxLogical, 
					 false, 
					 0); 

  // super structure collar 

  x = 0.0; 
  y = 0.0; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + 0.5 * fSuperStructureCollarHeight; 

  position = G4ThreeVector(x, y, z); 

  fSuperStructureCollarPhysical = new G4PVPlacement(0, 
						    position, 
						    fSuperStructureCollarLogical, 
						    "SSCollarPhysical", 
						    fAirBoxLogical, 
						    false, 
						    0); 

  // super structure bellow 

  x = 0.0; 
  y = 0.0; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureCollarHeight + 0.5 * fSuperStructureBellowHeight; 

  position = G4ThreeVector(x, y, z); 

  fSuperStructureBellowPhysical = new G4PVPlacement(0, 
						    position, 
						    fSuperStructureBellowLogical, 
						    "SSBellowPhysical", 
						    fAirBoxLogical, 
						    false, 
						    0); 

  // // super structure flange 
  // 
  // x = 0.0; 
  // y = 0.0; 
  // z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureCollarHeight + 0.5 * fSuperStructureFlangeHeight; 
  // 
  // position = G4ThreeVector(x, y, z); 
  // 
  // fSuperStructureFlangePhysical = new G4PVPlacement(0, 
  //       					    position, 
  //       					    fSuperStructureFlangeLogical, 
  //       					    "SSFlangePhysical", 
  //       					    fAirBoxLogical, 
  //       					    false, 
  // 						    0); 


  // super structure FlangeBellowToCollar

  x = 0.0; 
  y = 0.0; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureCollarHeight + 0.5 * fSSFlangeBellowToCollarHeight;

  position = G4ThreeVector(x, y, z); 

  fSSFlangeBellowToCollarPhysical = new G4PVPlacement(0, 
                                                      position, 
                                                      fSSFlangeBellowToCollarLogical, 
                                                      "SSFlangeBellowToCollarPhysical", 
                                                      fAirBoxLogical, 
                                                      false, 
                                                      0); 


  // super structure beam grid 

  x = -2.0 * m;
  y =  0.0 * m;
//   x = 0.0;
//   y = -1.0 * m;
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + 10.0 * cm; 

  position = G4ThreeVector(x, y, z); 

  fSuperStructureBeamGridPhysical = new G4PVPlacement(0, 
						      position, 
						      fSuperStructureBeamGridLogical, 
						      "SSBeamGridPhysical", 
						      fAirBoxLogical, 
						      false, 
						      0); 

  // outer lock 

  x = 0.0 * m; 
  y = 0.0 * m; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureHeight + fCleanRoomThickness + 0.5 * fOuterLockHeight; 

  position = G4ThreeVector(x, y, z); 

  fOuterLockPhysical = new G4PVPlacement(0, 
					 position, 
					 fOuterLockLogical, 
					 "OuterLockPhysical", 
					 fAirBoxLogical, 
					 false, 
					 0); 

  // inner lock 

  x = 0.0 * m; 
  y = 1.0 * m; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight + fSuperStructureHeight + fCleanRoomThickness + 0.5 * fInnerLockHeight; 

  position = G4ThreeVector(x, y, z); 

  fInnerLockPhysical = new G4PVPlacement(0, 
					 position, 
					 fInnerLockLogical, 
					 "InnerLockPhysical", 
					 fAirBoxLogical, 
					 false, 
					 0); 

  // lock steel plate 

  x = 0.0 * m; 
  y = 0.0 * m; 
  z = tankoffset + 0.5 * fShieldingWaterTankHeight 
      + fSuperStructureCollarHeight 
      + fSuperStructureBellowHeight 
      + fLockSteelPlateZPosRelToLock
      + 0.5 * fLockSteelPlateHeight ;


  position = G4ThreeVector(x, y, z); 

  fLockSteelPlatePhysical = new G4PVPlacement(0, 
					      position, 
					      fLockSteelPlateLogical, 
					      "LockSteelPlatePhysical", 
					      fAirBoxLogical, 
					      false, 
					      0); 

}
