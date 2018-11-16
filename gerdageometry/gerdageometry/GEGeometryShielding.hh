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
/**
 * CLASS DECLARATION:  GEGeometryShielding.hh
 *
 * DESCRIPTION:
 *
 *   A class to hold all the shielding materials
 *
 * AUTHOR: Xiang  Liu
 *
 * REVISION: MM-DD-YYYY
 *
 *   11-16-2004, created, Xiang
 *     now the set up is as following:
 *     crystal array:
 *        3*7 Ge crystals with number 0 at the very center
 *        horizontal gap 1cm, vertical gap 5cm
 *     shielding materials:
 *        name              inner-radius(cm)   outer-radius(cm)  height(cm)   position
 *        ----------------|------------------|-----------------|------------|-------------
 *        liquid-N2         0                  194               500          center
 *        copper            195                198               500          center
 *        water             0                  500               900          center
 *        Cu-plate (up)     49.5               198               3            z= 450+1.5
 *        Cu-plate (down)   0                  198               3            z=-450-1.5
 *        lead-plate(up)    49.5               198               3            z= 453+1.5
 *        lead-plate(down)  0                  198               3            z=-453-1.5
 *
 *   11-16-2004, added messenger, Xiang
 *   12-06-2004, C.Tomei, defined AirBox as the most external logical volume
 *   12-14-2004, C.Tomei, completely new design of the shielding geometry
 *                        according to the proposal
 *   01-21-2005, added method GetDewarHeight, Kevin
 *   01-24-2005  added method ConstructMotherVolume, Kevin
 *   02-18-2004, added Steel Water Tank, Claudia
 *   03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 *   04-06-2005  Fixed bug with the LN2/LAr flag (it was always LN), Luciano
 *   07-12-2005  Cryotank is no longer centered in the Water tank. Add variable, Luciano
 *   07-22-2005  Added air in the neck above the LN2, Luciano
 *   06-05-2006  Cryostat changed from copper to steel. Dimensions taken from a 
 *               design provided by K.Tasso, still preliminary. Claudia and Assunta
 *   12-05-2007 Cryostat is now one object  (Jens)
 *              - neck Cu layer, and so on belong to this object
 *              - makrolon layer has been added, addition of a further layer should be easy
 *              - the former neck radius was to big by a factor of two
 *              - further little changes
 *              - all parts belonging to the Cryostat should now belong to the class "GEGeometryCryostatParts"
 *   12-10-2007 Put a flange with right radii between collar and bellow above neck of cryostat (Jens)
 *   04-29-2008 Added fExternalRockLogical (logical volume for Hall A), Luciano
 */

#ifndef _GEGEOMETRYSHIELDING_HH
#define _GEGEOMETRYSHIELDING_HH

//---------------------------------------------------------------------------//
//      Geant4 headers
#include "globals.hh"
#include "G4LogicalVolume.hh"

// MG headers
//#include "geometry/MGGeometryDetector.hh"
#include "GEGeometryDetectorDB.hh"

//---------------------------------------------------------------------------//

class GEGeometryCryostatParts;


class GEGeometryShielding
{

public:

  /// Default constructor
  GEGeometryShielding(GEGeometryDetectorDB* detectordb);

  /// Destructor
  ~GEGeometryShielding();

  // construction
  void ConstructShielding();

  // getters

private:

  GEGeometryCryostatParts* fCryostatParts;

  G4double fShieldingWaterTankHeight; 
  G4double fShieldingWaterTankThickness; 
  G4double fShieldingWaterHeight; 
  G4double fShieldingAirBufferHeight; 
  G4double fShieldingFootHeight; 
  G4double fShieldingCopperShieldingLargeHeight; 
  G4double fShieldingCopperShieldingSmallHeight; 
  G4double fCleanRoomHeight; 
  G4double fCleanRoomThickness; 
  G4double fSuperStructureHeight; 
  G4double fSuperStructureCollarHeight; 
  G4double fSuperStructureBellowHeight; 
  G4double fSuperStructureFlangeHeight; 
  G4double fSSFlangeBellowToCollarHeight;
  G4double fOuterLockHeight; 
  G4double fInnerLockHeight; 
  G4double fLockCopperPlateHeight; 
  G4double fLockSteelPlateHeight; 
  G4double fLockSteelPlateZPosRelToLock; 
  G4double fHallSizeHeight;

  G4LogicalVolume* fAirBoxLogical; 
  G4LogicalVolume* fExternalRockLogical;
  G4LogicalVolume* fWaterTankLogical; 
  G4LogicalVolume* fWaterLogical; 
  G4LogicalVolume* fAirBufferLogical; 
  G4LogicalVolume* fFootLogical;

  G4LogicalVolume* fCopperShieldingLargeLogical; 
  G4LogicalVolume* fCopperShieldingSmallLogical;  
  G4LogicalVolume* fCleanRoomLogical; 
  G4LogicalVolume* fSuperStructureCollarLogical; 
  G4LogicalVolume* fSuperStructureBellowLogical; 
  G4LogicalVolume* fSuperStructureFlangeLogical; 
  G4LogicalVolume* fSSFlangeBellowToCollarLogical;
  G4LogicalVolume* fSuperStructureBeamGridLogical; 
  G4LogicalVolume* fOuterLockLogical; 
  G4LogicalVolume* fInnerLockLogical; 
  G4LogicalVolume* fLockCopperPlateLogical; 
  G4LogicalVolume* fLockSteelPlateLogical; 

  
  G4VPhysicalVolume* fAirBoxPhysical; 
  G4VPhysicalVolume* fWaterTankPhysical; 
  G4VPhysicalVolume* fWaterPhysical; 
  G4VPhysicalVolume* fAirBufferPhysical; 
  G4VPhysicalVolume* fFootPhysical;
  G4VPhysicalVolume* fCopperShieldingLargePhysical; 
  G4VPhysicalVolume* fCopperShieldingSmallPhysical; 
  G4VPhysicalVolume* fCleanRoomPhysical; 
  G4VPhysicalVolume* fSuperStructureCollarPhysical; 
  G4VPhysicalVolume* fSuperStructureBellowPhysical; 
  //G4VPhysicalVolume* fSuperStructureFlangePhysical; 
  G4VPhysicalVolume* fSSFlangeBellowToCollarPhysical;
  G4VPhysicalVolume* fSuperStructureBeamGridPhysical; 
  G4VPhysicalVolume* fOuterLockPhysical; 
  G4VPhysicalVolume* fInnerLockPhysical; 
  //G4VPhysicalVolume* fLockCopperPlatePhysical; 
  G4VPhysicalVolume* fLockSteelPlatePhysical; 

};

#endif

