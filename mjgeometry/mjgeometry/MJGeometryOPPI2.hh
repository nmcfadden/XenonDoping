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
 * CLASS DECLARATION:  MJGeometryOPPI2.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry of the OPPI2 detector
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
 * FIRST SUBMISSION: June 21, 2012
 * 
 * REVISION:
 * 
 *
 */
// --------------------------------------------------------------------------//
//---------------------------------------------------------------------------// 
// MJGeometryOPPI2.hh
//---------------------------------------------------------------------------// 

#ifndef MJGEOMETRYOPPI2_HH
#define MJGEOMETRYOPPI2_HH

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

class MJGeometryOPPI2 : public MGGeometryDetector
{
public:
  
  //constructor
  MJGeometryOPPI2();

  //destructor
  virtual ~MJGeometryOPPI2(){}

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

	G4LogicalVolume* Nbox_log;
	G4LogicalVolume* active_crystal_log;
	G4LogicalVolume* cryostatLog;
	G4LogicalVolume* cryovacLog;
	G4LogicalVolume* AlCupLog;
	G4LogicalVolume* polyRingLog;
	G4LogicalVolume* leadCylinderLog;
	G4LogicalVolume* CuRingLog;
	G4LogicalVolume* leadLidLog;

  // Physical volumes
	
	G4VPhysicalVolume* crystal_phys;
	G4VPhysicalVolume* cryostatPhys;
	G4VPhysicalVolume* cryovacPhys;
	G4VPhysicalVolume* AlCupPhys;
	G4VPhysicalVolume* polyRingPhys;
	G4VPhysicalVolume* leadCylinderPhys;
	G4VPhysicalVolume* CuRingPhys;
	G4VPhysicalVolume* leadLidPhys;

 };

//---------------------------------------------------------------------------// 

#endif


