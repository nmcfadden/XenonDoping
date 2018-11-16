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
 * $Id: MJGeometryIdealCoaxWithShield.hh,v 1.4 2005-12-15 23:38:35 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryIdealCoaxWithShield.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Takes MJGeometryIdealCoaxCrystal and insert it into a shield.
 * Can be used as a wrapper for just and ideal coaxial crystal w/o any shield
 * by setting fShieldOn = false
 * The shield consists of an endcap (can) and a colplate at the bottom of the
 * can that the crystal is mounted on.
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
 * FIRST SUBMISSION: Thu Jan  6 11:34:32 PST 2005
 * 
 * REVISION:
 * 
 * 01-06-2005, Created, R. Henning
 * 02-14-2005, Added simple shield geometry (endcap + colplate), R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYIDEALCOAXWITHSHIELD_HH
#define _MJGEOMETRYIDEALCOAXWITHSHIELD_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "geometry/MGGeometryIdealCoaxCrystal.hh"

//---------------------------------------------------------------------------//

class G4VisAttributes;

class MJGeometryIdealCoaxWithShieldMessenger;

class MJGeometryIdealCoaxWithShield : public MGGeometryIdealCoaxCrystal 
{
public:

  MJGeometryIdealCoaxWithShield(G4String serNum, G4bool messengerOff);
  MJGeometryIdealCoaxWithShield(const MJGeometryIdealCoaxWithShield &rhs);
  ~MJGeometryIdealCoaxWithShield();
  
  G4double GetAssemblyHeight() { return fAssemblyHeight; }
  G4double GetAssemblyRadius() { return fAssemblyRadius; }
  G4bool   GetShieldOn() { return fShieldOn; }
  G4String GetColdplateMaterialName() { return fColdplateMaterialName; }
  G4double GetColdplateThickness() { return fColdplateThickness; }
  G4String GetEndcapMaterialName() { return fEndcapMaterialName; }
  G4double GetEndcapThickness() { return fEndcapThickness; }

  void SetAssemblyHeight(G4double hh) { fAssemblyHeight = hh; }
  void SetAssemblyRadius(G4double rr) { fAssemblyRadius = rr; }
  void SetColdplateMaterialName(G4String name) {fColdplateMaterialName=name;}
  void SetColdplateThickness(G4double thick) { fColdplateThickness = thick; }
  void SetEndcapMaterialName(G4String name) { fEndcapMaterialName = name; }
  void SetEndcapThickness(G4double thick) { fEndcapThickness = thick; }
  void SetShieldOn(G4bool shieldOn) { fShieldOn = shieldOn; }

  void ConstructDetector();
  void Dump();
  void SetDefaults();

private:
  G4bool                fShieldOn; // Simulate shield, or just bare crystal
  G4double	        fAssemblyRadius; // Radius of crystal assy.
  G4double              fAssemblyHeight; // Height of crystal assy.
  G4double              fEndcapThickness;// Thickness of endcap material.
  G4double              fColdplateThickness;// Thickness of crystal holder at
                                            // bottom

  G4String              fEndcapMaterialName; 
  G4String              fColdplateMaterialName; 

  G4VisAttributes       *fColdplateVisAtt;
  G4VisAttributes       *fCanVisAtt;

  MJGeometryIdealCoaxWithShieldMessenger *fG4Messenger;
};
#endif
