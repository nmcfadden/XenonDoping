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
 * $Id: MJGeometryRDDetectorWBasicShield.hh,v 1.2 2009-07-15 16:43:10 paddy Exp $
 *      
 * CLASS DECLARATION:  MJGeometryRDDetectorWBasicShield.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Majorana Reference design consisting of "Russian Doll" type shield
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
 * FIRST SUBMISSION: Thu Dec 15 17:23:12 PST 2005
 * 
 * REVISION:
 * 
 * 12-15-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYRDDETECTORWBASICSHIELD_HH
#define _MJGEOMETRYRDDETECTORWBASICSHIELD_HH

//---------------------------------------------------------------------------//

#include <vector>

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;

class MJGeometryRDDetectorWBasicShield :
public MGGeometryDetector
{
public:

  MJGeometryRDDetectorWBasicShield(G4String serNum);
  ~MJGeometryRDDetectorWBasicShield();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  

private:
  void Initialize();

  // Shield constituents, from the inside out.
  // The first (innermost) shield is the one that contains the detector 
  // (fEnclosedDetector).
  // Its "thickness" is the half-width of the enclosing box.
  std::vector<G4ThreeVector> fShieldThickness;    // Thickness in x,y,z.
  std::vector<G4ThreeVector> fShieldDisplacement; // Relative to volume outside it.
  std::vector<G4String>      fShieldMaterialName;
  std::vector<G4Material*>   fShieldMaterial;
  std::vector<G4String>      fShieldName;

  MGGeometryDetector   *fEnclosedDetector;
  
  G4LogicalVolume*     fCFCavityLogical;      //cold finger cavity in shield
  G4VPhysicalVolume*   fCFCavityPhysical;  
  
  G4LogicalVolume*     fCuCFLogical;          //Cu cold finger
  G4VPhysicalVolume*   fCuCFPhysical;  

  G4LogicalVolume*     fPlasticFootLogical;   //plastic foot 
  G4VPhysicalVolume*   fPlasticFootPhysical;  
};
#endif
