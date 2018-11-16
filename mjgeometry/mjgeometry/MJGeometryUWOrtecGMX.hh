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
 * $Id: MJGeometryUWOrtecGMX.hh,v 1.3 2008-08-24 22:39:10 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MJGeometryUWOrtecGMX.hh
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Tue Oct 16 14:36:17 PDT 2007
 * 
 * REVISION:
 * 
 * 10-17-2007, Initial submission, A. Schubert
 */
// --------------------------------------------------------------------------//


#ifndef _MJGEOMETRYORTECGMXCFGGG12_HH
#define _MJGEOMETRYORTECGMXCFGGG12_HH

//---------------------------------------------------------------------------//

//#include <systemStuff.h>		//Language level Headers
//#include "globalsStuff.hh"		//Global Headers
//#include "G4Stuff.hh"		        //G4 Class Headers
//#include "MJStuff.hh"                 //MJ Headers

//---------------------------------------------------------------------------//
#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"
//---------------------------------------------------------------------------//

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;

class MGGeometryIdealCoaxCrystal;

class MJGeometryUWOrtecGMX : public MGGeometryDetector 
{
public:

  //default constructor
  MJGeometryUWOrtecGMX(G4String serNum);

  //copy constructor
  MJGeometryUWOrtecGMX(const MJGeometryUWOrtecGMX &);

  // set and get methods:
  void SetCrystalLength(G4double val) { fCrystalLength = val; }
  G4double GetCrystalLength() { return fCrystalLength; }
  void SetCrystalDiameter(G4double val) { fCrystalDiameter = val; }
  G4double GetCrystalDiameter() { return fCrystalDiameter; }
  bool UsesMJPSQLDatabase() { return true; }  

  //destructor
  ~MJGeometryUWOrtecGMX();

  //public interface
  void ConstructDetector();

  //protected members
protected:


  //private  members
private:

  G4VPhysicalVolume* fCrystalCanCavity_phys;
  G4VPhysicalVolume* fCrystal_phys;
  G4LogicalVolume* fCrystalCanCavity_log; 

  G4double fCrystalLength; 
  G4double fCrystalDiameter;
};
#endif
