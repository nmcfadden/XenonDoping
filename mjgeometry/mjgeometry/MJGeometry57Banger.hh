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
/**                                                            
 * $Id: MJGeometry57Banger.hh,v 1.3 2005-03-11 02:21:46 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometry57Banger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Layer of 19 crystals == one layer in 57 Banger
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Jan  6 12:47:55 PST 2005
 * 
 * REVISION:
 * 
 * 01-06-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRY57BANGERLAYER_HH
#define _MJGEOMETRY57BANGERLAYER_HH

//---------------------------------------------------------------------------//

#include <string>
#include <vector>

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4LogicalVolume;
class G4Material;
class G4PVPlacement;
class G4VisAttributes;

class MJGeometryIdealCoaxWithShield; 
class MJVGeometrySupportStructure;

class MJGeometry57Banger : public MGGeometryDetector
{
public:

  MJGeometry57Banger(G4String serNum);
  ~MJGeometry57Banger();

  void ConstructDetector();
  void SetDefaults();

  G4LogicalVolume *GetCavityLogical() { return fCavityLogical; }
  G4double GetCavityHeight() { return fHeight; }
  G4double GetHexagonEdgeRadius() { return fEdgeRadius; }
  std::string GetCrystalName(G4int placement) 
    { return fCrystalNames[placement]; }
  G4ThreeVector GetCrystalLocation(G4int placement)
    { return fCrystalLocations[placement]; }
  MJGeometryIdealCoaxWithShield*  GetTemplateCrystal()
    { return fTemplateCrystal; }

  void SetSupportStructureName(G4String name) { fSupportStructureName = name; }
  bool UsesMJPSQLDatabase() { return true; } 

private:

  void ConstructSupportStructure();

  MJGeometryIdealCoaxWithShield  *fTemplateCrystal;
  G4bool          fShieldOn;
  G4PVPlacement   **fPhysicalCrystals;
  G4int           fTotalNumberOfCrystals;
  G4int           fNumberOfLayers;
  G4int           fNumberOfCrystalsPerLayer;
  
  G4double        fCrystalSeparationXY;
  G4double        fCrystalSeparationZ; // Separation between crystals.
  G4double        fCrystalSpacingXY; // Distance between crystal axes on same 
                                     // layer. (computed)
  G4double        fCrystalSpacingZ;  // Crystal spacing between layers.
                                     // (computed)

  G4LogicalVolume *fCavityLogical;
  G4double        fHexagonEnvelope; // Distance that hexagon should extend on 
                                    // the xy plane beyond the furthest crystal
  G4double        fEdgeRadius;  // Corner radius of enclosing hexagon.
  G4double        fHeight;      // Height of enclosing hexagon.
  G4VisAttributes *fHexagonVisAtt;
  G4VisAttributes *fShieldVisAtt;

  std::vector<std::string>    fCrystalNames;
  std::vector<G4ThreeVector>  fCrystalLocations;

  // Support structure class, if required. 
  G4String                     fSupportStructureName;
  MJVGeometrySupportStructure  *fSupportStructure;
};
#endif
