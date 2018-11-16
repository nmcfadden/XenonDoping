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
 * $Id: MJGeometryPPCCryostat.hh,v 1.1 2008-09-18 17:40:01 mitzi Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPPCCryostat.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Takes MJGeometryPPCDetectorCrystal and insert it into a shield.
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
 * AUTHOR: M. Boswell
 * CONTACT: 
 * FIRST SUBMISSION: Wed. July 23, 2008
 * 
 * REVISION:
 * 
 * 07-23-2008, Created, M. Boswell
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYPPCCRYOSTAT_HH
#define _MJGEOMETRYPPCCRYOSTAT_HH
//---------------------------------------------------------------------------//

#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"


//---------------------------------------------------------------------------//

class G4VisAttributes;
class G4Material;
class G4VPhysicalVolume;
class MJGeometryPPCCryostatMessenger;
class MJGeometryPPCCrystalColumn;
class MJGeometryPPCDetector;

class MJGeometryPPCCryostat : public MGGeometryDetector 
{
public:

  MJGeometryPPCCryostat();
  MJGeometryPPCCryostat(G4String serNum);
  MJGeometryPPCCryostat(const MJGeometryPPCCryostat &);
  ~MJGeometryPPCCryostat();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  

  //get info about the assembly
  G4double GetAssemblyHeight() {return fAssemblyHeight;}
  G4double GetAssemblyRadius() {return fAssemblyRadius;}
  //void SetOpenSpaceMaterialName(G4Material val) {fOpenSpaceMaterialName = val;}

private:
  void Initialize();

  MJGeometryPPCCryostatMessenger *fG4Messenger;


  G4VisAttributes* CuAttributes;
  G4VisAttributes* EFCuAttributes;
  G4Material* CanMaterial;
  G4String fOpenSpaceMaterialName;
  G4Material* fOpenSpaceMaterial;
  G4Material* CrystalMaterial;



  //information about the cryostat
  G4double fEps;
  G4String serialNumber; //serial number of crystal
  size_t fNOfCrystalColumns;
  G4double fAssemblyHeight;
  G4double fAssemblyRadius;

  MJGeometryPPCDetector  *fTemplateCrystal;
  MJGeometryPPCCrystalColumn* fTemplateCrystalColumn;
  std::vector<MJGeometryPPCCrystalColumn*> fCrystalColumns;

  G4double fAssemTopCutawayThick;
  G4double fAssemTopThick;
  G4double fAssemTopORad;
  G4double fAssemTopIRad;
  G4double fAssemTopOffset;
  G4double fAssemTopOffsetAng;

  G4double fAssemblyOffset;

};
#endif
