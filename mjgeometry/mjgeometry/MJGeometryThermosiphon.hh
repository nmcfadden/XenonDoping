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
 * $Id: MJGeometry17A.hh,v 1.1 2006-01-05 07:16:11 rojo Exp $
 *      
 * CLASS DECLARATION:  MJGeometry17A.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry of the ultra low background detector in room 17-A at PNNL
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
 * AUTHOR: M. Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 
 *
 */
// --------------------------------------------------------------------------//
//---------------------------------------------------------------------------// 
// MJGeometryThermosiphon.hh
//---------------------------------------------------------------------------// 

#ifndef MJGEOMETRYTHERMOSIPHON_HH
#define MJGEOMETRYTHERMOSIPHON_HH

//---------------------------------------------------------------------------// 

#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------// 

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

class MGGeometryIdealCoaxCrystal;

class MJGeometryThermosiphon : public MGGeometryDetector
{
public:
  
  //constructor
  MJGeometryThermosiphon();

  //destructor
  virtual ~MJGeometryThermosiphon(){}

  //public interfaces
  void ConstructDetector();
  void SetDefaults();
  void Dump();
  bool UsesMJPSQLDatabase() { return true; }  

  MGGeometryIdealCoaxCrystal*  GetTemplateCrystal()
    { return fTemplateCrystal; }
    
private:

  MGGeometryIdealCoaxCrystal  *fTemplateCrystal;
  
  //Logical volumes
  //
  G4LogicalVolume* Nbox_log;
  G4LogicalVolume* outer_tube_log;
  G4LogicalVolume* inner_tube_log;
  G4LogicalVolume* elbow_log;
  G4LogicalVolume* PEEK_log;
  G4LogicalVolume* vetoPanel_log;
  G4LogicalVolume* vetoPanelcp_log;
  G4LogicalVolume* cryostat_log;
//  G4LogicalVolume* cryoFill_log;
  G4LogicalVolume* inner_can_log;
  G4LogicalVolume* active_crystal_log;
  G4LogicalVolume* LN_tube_log;
  G4LogicalVolume* PbShield_log;
  G4LogicalVolume* spacer_log;
  G4LogicalVolume* spacer_cp_log;
  G4LogicalVolume* flange_log;
  G4LogicalVolume* spacerCube_log;
  G4LogicalVolume* dewar_log;
  G4LogicalVolume* cylinder_log;
  G4LogicalVolume* plate_log;
  G4LogicalVolume* rn_enc_log;
  G4LogicalVolume* CuShield_log;
  G4LogicalVolume* efCuShield_log;

  // Physical volumes
  //
//  G4VPhysicalVolume* Nbox_phys;
  G4VPhysicalVolume* outer_tube_phys;
  G4VPhysicalVolume* inner_tube_phys;
  G4VPhysicalVolume* elbow_phys;
  G4VPhysicalVolume* PEEK_phys;
  G4VPhysicalVolume* vetoPanel_phys;
  G4VPhysicalVolume* vetoPanelcp_phys;
  G4VPhysicalVolume* cryostat_phys;
//  G4VPhysicalVolume* cryoFill_phys;
  G4VPhysicalVolume* inner_can_phys;
  G4VPhysicalVolume* crystal_phys;
  G4VPhysicalVolume* LN_tube_phys;
  G4VPhysicalVolume* PbShield_phys;
  G4VPhysicalVolume* spacer_phys;
  G4VPhysicalVolume* spacer_cp_phys;
  G4VPhysicalVolume* flange_phys;
  G4VPhysicalVolume* spacerCube_phys;
  G4VPhysicalVolume* dewar_phys;
  G4VPhysicalVolume* cylinder_phys;
  G4VPhysicalVolume* plate_phys;
  G4VPhysicalVolume* rn_enc_phys;
  G4VPhysicalVolume* CuShield_phys;
  G4VPhysicalVolume* efCuShield_phys;
 };

//---------------------------------------------------------------------------// 

#endif


