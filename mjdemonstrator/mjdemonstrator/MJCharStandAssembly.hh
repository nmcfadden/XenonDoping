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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJCharStandAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * The Characterization Stand assembly: button source with lead collimator, housing
 * and mounting plate
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
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#ifndef _MJCHARSTANDASSEMBLY_HH
#define _MJCHARSTANDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJCharButtonSourceAssembly;
class MJCharCollimator;
class MJCharCollimatorHousingAssembly;
class MJCharCollimatorMtgPlate;

using namespace std;
//---------------------------------------------------------------------------//

class MJCharStandAssembly: public MJVDemoAssembly
{
  
public:
  MJCharStandAssembly(G4String, G4String, G4String);
  MJCharStandAssembly(const MJCharStandAssembly &);
  ~MJCharStandAssembly();

  G4String fSourceMaterialName;

  G4String GetSourceMaterialName() const {return fSourceMaterialName;}
  MJCharButtonSourceAssembly* GetButtonSourceAssembly() {return fButtonSourceAssemblyPtr;}
  MJCharCollimator* GetCollimator() {return fCollimatorPtr;}
  MJCharCollimatorHousingAssembly* GetCollimatorHousingAssembly() {return fCollimatorHousingAssemblyPtr;}
  MJCharCollimatorMtgPlate* GetCollimatorMtgPlate() {return fCollimatorMtgPlatePtr;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
	     G4LogicalVolume* motherLogical);
  
private:
  MJCharButtonSourceAssembly* fButtonSourceAssemblyPtr;
  MJCharCollimator* fCollimatorPtr;
  MJCharCollimatorHousingAssembly* fCollimatorHousingAssemblyPtr;
  MJCharCollimatorMtgPlate* fCollimatorMtgPlatePtr;

};
//
#endif
