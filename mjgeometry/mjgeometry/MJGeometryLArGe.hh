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
 * $Id: MJGeometryLArGe.hh,v 1.3 2005-01-06 16:22:09 dimarcom Exp $
 *      
 * CLASS DECLARATION:  MJGeometryLArGe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * dummy to test my MJsim skills
 * Solid, uniform block, size 2x2x2 cm^3
 * For energy deposit studies.
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
 * AUTHOR: dimarcom
 * CONTACT: 
 * FIRST SUBMISSION: Tue Nov  9 10:38:35 PST 2004
 * 
 * REVISION:
 * 
 * 11-09-2004, created, dimarcom
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYLARGE_HH
#define _MJGEOMETRYLARGE_HH

class G4LogicalVolume;
class G4VPhysicalVolume;

//---------------------------------------------------------------------------//

#include "globals.hh"		//Global Headers

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class MJGeometryLArGe  : public MGGeometryDetector
{
public:

  //default constructor
  MJGeometryLArGe(G4String);

  //destructor
  ~MJGeometryLArGe();

  //public interface
  void ConstructDetector();
  void SetMaterialName(G4String name) { fMaterialName = name; }
  bool UsesMJPSQLDatabase() { return true; }  

  //protected members
protected:


  //private  members
private:

  G4String                          fMaterialName;
  // Logical volumes
  //
  //G4LogicalVolume* groove_log;
  G4LogicalVolume* deadlayer_log;
  G4LogicalVolume* crystal_log;
  G4LogicalVolume* cryostat_log;
                                 
  // Physical volumes
  //
  //G4VPhysicalVolume* groove_phys;
  G4VPhysicalVolume* deadlayer_phys;
  G4VPhysicalVolume* crystal_phys;
  //G4VPhysicalVolume* cryostat_phys;

};

#endif
