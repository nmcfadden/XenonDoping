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
 * $Id: MJGeometryPPCDetector.hh,v 1.1 2008-09-18 17:34:37 mitzi Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPPCDetector.hh
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

#ifndef _MJGEOMETRYPPCDETECTOR_HH
#define _MJGEOMETRYPPCDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "geometry/MGGeometryIdealCoaxCrystal.hh"

//---------------------------------------------------------------------------//

class G4VisAttributes;
class G4Material;

class MJGeometryPPCDetectorMessenger;

class MJGeometryPPCDetector : public MGGeometryIdealCoaxCrystal 
{
public:

  MJGeometryPPCDetector(G4String serNum, G4bool messengerOff);
  MJGeometryPPCDetector(const MJGeometryPPCDetector &rhs);
  ~MJGeometryPPCDetector();
  
  G4double GetCrystalHeight() { return fCrystalHeight; }
  G4double GetCrystalRadius() { return fCrystalRadius; }

  void SetCrystalHeight(G4double hh) { fCrystalHeight = hh; }
  void SetCrystalRadius(G4double rr) { fCrystalRadius = rr; }

  void ConstructDetector();
  void SetDefaults();

private:

  G4double	        fCrystalRadius; // Radius of crystal assy.
  G4double              fCrystalHeight; // Height of crystal assy.

  G4String              fCrystalMaterialName; 
  //G4Material* CrystalMaterial;

  MJGeometryPPCDetectorMessenger *fG4Messenger;
};
#endif
