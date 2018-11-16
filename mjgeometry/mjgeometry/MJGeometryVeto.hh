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

#ifndef MJGEOMETRYVETO_HH
#define MJGEOMETRYVETO_HH

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

class MJGeometryVeto : public MGGeometryDetector
{
public:
  
  //constructor
  MJGeometryVeto();
  bool UsesMJPSQLDatabase() { return true; }  

  //destructor
  virtual ~MJGeometryVeto(){}

  //public interfaces
  void ConstructDetector();

private:

//  MGGeometryIdealCoaxCrystal  *fTemplateCrystal;
  
  //Logical volumes
  //
  //G4LogicalVolume* Nbox_log;
  G4LogicalVolume* vetoPanelFB_log;
  G4LogicalVolume* vetoPanelFL_log;
  G4LogicalVolume* vetoPanelLSB_log;
  G4LogicalVolume* vetoPanelLSL_log;
  G4LogicalVolume* vetoPanelFT_log;
  G4LogicalVolume* vetoPanelFR_log;
  G4LogicalVolume* vetoPanelLST_log;
  G4LogicalVolume* vetoPanelLSR_log;
  G4LogicalVolume* solidPanelBB_log;
  G4LogicalVolume* solidPanelBT_log;
  G4LogicalVolume* solidPanelBL_log;
  G4LogicalVolume* solidPanelBR_log;
  G4LogicalVolume* solidPanelRSB_log;
  G4LogicalVolume* solidPanelRST_log;
  G4LogicalVolume* solidPanelRSL_log;
  G4LogicalVolume* solidPanelRSR_log;
  G4LogicalVolume* solidPanelTIL_log;
  G4LogicalVolume* solidPanelTIR_log;
  G4LogicalVolume* solidPanelTOF_log;
  G4LogicalVolume* solidPanelTOB_log;
  G4LogicalVolume* overfloorTop_log;
  G4LogicalVolume* overfloorBottom_log;

  G4VPhysicalVolume* vetoPanelFB_phys;
  G4VPhysicalVolume* vetoPanelFT_phys;
  G4VPhysicalVolume* vetoPanelFL_phys;
  G4VPhysicalVolume* vetoPanelFR_phys;
  G4VPhysicalVolume* vetoPanelLSB_phys;
  G4VPhysicalVolume* vetoPanelLST_phys;
  G4VPhysicalVolume* vetoPanelLSL_phys;
  G4VPhysicalVolume* vetoPanelLSR_phys;
  G4VPhysicalVolume* solidPanelBB_phys;
  G4VPhysicalVolume* solidPanelBT_phys;
  G4VPhysicalVolume* solidPanelBL_phys;
  G4VPhysicalVolume* solidPanelBR_phys;
  G4VPhysicalVolume* solidPanelRSB_phys;
  G4VPhysicalVolume* solidPanelRST_phys;
  G4VPhysicalVolume* solidPanelRSL_phys;
  G4VPhysicalVolume* solidPanelRSR_phys;
  G4VPhysicalVolume* solidPanelTIL_phys;
  G4VPhysicalVolume* solidPanelTIR_phys;
  G4VPhysicalVolume* solidPanelTOF_phys;
  G4VPhysicalVolume* solidPanelTOB_phys;
  G4VPhysicalVolume* overfloorTop_phys;
  G4VPhysicalVolume* overfloorBottom_phys;
 };



//---------------------------------------------------------------------------// 

#endif



