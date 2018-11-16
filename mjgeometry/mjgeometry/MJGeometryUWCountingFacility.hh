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
 * $Id: MJGeometryUWCountingFacility.hh,v 1.5 2008-08-24 22:39:10 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MJGeometryUWCountingFacility.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This is a model of the counting facility in the Majorana Lab at UW, 
 * as of Oct 2007.  The counting system consists of two Ortec GMX detectors 
 * surrounded by 6 inches of lead shielding.  An 0.5 inch copper plate 
 * supports the roof of the brick enclosure.  
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Mon Oct 15 20:01:34 PDT 2007
 * 
 * REVISION:
 * 
 * 10-17-2007, Initial submission, A. Schubert
 */
// --------------------------------------------------------------------------//


#ifndef _MJGEOMETRYUWCOUNTINGFACILITY_HH
#define _MJGEOMETRYUWCOUNTINGFACILITY_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4LogicalVolume;
class G4Material;
class G4VisAttributes;

class MJGeometryUWOrtecGMX;

class MJGeometryUWCountingFacility: public MGGeometryDetector 
{
public:

  //default constructor
  MJGeometryUWCountingFacility(G4String serNum);

  //copy constructor
  MJGeometryUWCountingFacility(const MJGeometryUWCountingFacility &);

  void ConstructDetector();
  bool UsesMJPSQLDatabase() { return true; }   

  //destructor
  ~MJGeometryUWCountingFacility();

  //public interface

  //protected members
protected:

  //private  members
private:
  G4LogicalVolume* fShieldCavity_log;
  G4LogicalVolume* fShieldWall_log;
  G4LogicalVolume* fCopperPlate_log;
  //G4LogicalVolume* fSample_log;
  //G4LogicalVolume* fBrick_log;
  //G4LogicalVolume* fPlate_log;  // for Katrin welds

  G4VPhysicalVolume* fShieldCavity_phys;
  G4VPhysicalVolume* fShieldWall_phys;
  G4VPhysicalVolume* fDetectorOne_phys;
  G4VPhysicalVolume* fDetectorTwo_phys;
  G4VPhysicalVolume* fCopperPlate_phys;
  //G4VPhysicalVolume* fSample_phys;
  //G4VPhysicalVolume* fBrick_phys;
  //G4VPhysicalVolume* fPlate_phys;  // for Katrin welds
};
#endif
