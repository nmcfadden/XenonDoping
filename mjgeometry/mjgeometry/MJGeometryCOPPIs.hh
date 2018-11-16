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
 * CLASS DECLARATION:  MJGeometryCOPPIs.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
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
 * FIRST SUBMISSION: 06-22-2012
 * 
 * REVISION:
 * 
 *
 */
// --------------------------------------------------------------------------//
//---------------------------------------------------------------------------// 
// MJGeometryThermosiphon.hh
//---------------------------------------------------------------------------// 

#ifndef MJGEOMETRYCOPPIs_HH
#define MJGEOMETRYCOPPIs_HH

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
class MJGeometryCOPPIsMessenger;
class MGGeometryIdealCoaxCrystal;

class MJGeometryCOPPIs : public MGGeometryDetector
{
public:
  
  //constructor
  MJGeometryCOPPIs();

  //destructor
  virtual ~MJGeometryCOPPIs();

  //public interfaces
  void ConstructDetector();
  void SetDefaults();
  void Dump();

  bool UsesMJPSQLDatabase() { return true; } 

  void UseOPPI3(G4bool val) {fUseOPPI3 = val;}
  void UseOPPI4(G4bool val) {fUseOPPI4 = val;}
  void UsePbCyl(G4bool val) {fUsePbCyl = val;}
  void SetDetSep(G4double val) {fSetDetSep = val;}
  void SetOPPI3TDL(G4double val) {fSetOPPI3TDL = val;}
  void SetOPPI4TDL(G4double val) {fSetOPPI4TDL = val;}
  void SetOPPI3SDL(G4double val) {fSetOPPI3SDL = val;}
  void SetOPPI4SDL(G4double val) {fSetOPPI4SDL = val;}
  void SetPbL(G4double val) {fSetPbL = val;}
  void UseZn(G4bool val) {fUseZn = val;}
  void UseZr(G4bool val) {fUseZr = val;}
  void UseNb(G4bool val) {fUseNb = val;}
  void UseCd(G4bool val) {fUseCd = val;}

  MGGeometryIdealCoaxCrystal*  GetTemplateCrystal()
    { return fTemplateCrystal; }

  virtual int GetSensitiveVolumeID(const std::string& volName);
  void MakePbCyl(G4double cryoOuterR);
    
private:

  MGGeometryIdealCoaxCrystal  *fTemplateCrystal;
  MGGeometryIdealCoaxCrystal  *fTemplateCrystal2;
  
  //Logical volumes
  //
	G4LogicalVolume* cryostatLog;
	G4LogicalVolume* cryostatLog2;
	G4LogicalVolume* cryovacLog;
	G4LogicalVolume* cryovacLog2;
	G4LogicalVolume* AlCupLog;
	G4LogicalVolume* leadCylinderLog;
	G4LogicalVolume* ZnFoilLog;
	G4LogicalVolume* ZrFoilLog;
	G4LogicalVolume* NbFoilLog;
	G4LogicalVolume* CdFoilLog;

  // Physical volumes
	
	G4VPhysicalVolume* leadCylinderPhys;
	G4VPhysicalVolume* cryostatPhys;
	G4VPhysicalVolume* cryovacPhys;
	G4VPhysicalVolume* AlCupPhys;
	G4VPhysicalVolume* crystal_phys;
	G4VPhysicalVolume* cryostatPhys2;
	G4VPhysicalVolume* cryovacPhys2;
	G4VPhysicalVolume* AlCupPhys2;
	G4VPhysicalVolume* crystal_phys2;
	G4VPhysicalVolume* ZnFoilPhys;
	G4VPhysicalVolume* ZrFoilPhys;
	G4VPhysicalVolume* NbFoilPhys;
	G4VPhysicalVolume* CdFoilPhys;

  // Messenger Values

 	MJGeometryCOPPIsMessenger* fMessenger;
	G4bool fUseOPPI3;
	G4bool fUseOPPI4;
	G4bool fUsePbCyl;
	G4double fSetDetSep;
	G4double fSetOPPI3TDL;
	G4double fSetOPPI4TDL;
	G4double fSetOPPI3SDL;
	G4double fSetOPPI4SDL;
	G4double fSetPbL;
	G4bool fUseZn;
	G4bool fUseZr;
	G4bool fUseNb;
	G4bool fUseCd;
 };

//---------------------------------------------------------------------------// 

#endif


