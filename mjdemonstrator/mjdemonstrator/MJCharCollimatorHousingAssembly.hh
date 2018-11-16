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
 * CLASS DECLARATION:  MJCharCollimatorHousingAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * The collimator housing with screws
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

#ifndef _MJCHARCOLLIMATORHOUSINGASSEMBLY_HH
#define _MJCHARCOLLIMATORHOUSINGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJCharCollimatorHousing;
class MJChar832Screw;
class MJChar1420Screw;

using namespace std;
//---------------------------------------------------------------------------//

class MJCharCollimatorHousingAssembly: public MJVDemoAssembly
{
  
public:
  MJCharCollimatorHousingAssembly(G4String, G4String);
  MJCharCollimatorHousingAssembly(const MJCharCollimatorHousingAssembly &);
  ~MJCharCollimatorHousingAssembly();

  MJCharCollimatorHousing* GetCollimatorHousing() {return fCollimatorHousingPtr;}
  MJChar832Screw* Get832Screw1() {return f832Screw1Ptr;}
  MJChar832Screw* Get832Screw2() {return f832Screw2Ptr;}
  MJChar832Screw* Get832Screw3() {return f832Screw3Ptr;}
  MJChar832Screw* Get832Screw4() {return f832Screw4Ptr;}
  MJChar1420Screw* Get1420Screw1() {return f1420Screw1Ptr;}
  MJChar1420Screw* Get1420Screw2() {return f1420Screw2Ptr;}
  MJChar1420Screw* Get1420Screw3() {return f1420Screw3Ptr;}
  MJChar1420Screw* Get1420Screw4() {return f1420Screw4Ptr;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJCharCollimatorHousing* fCollimatorHousingPtr;
  MJChar832Screw* f832Screw1Ptr;
  MJChar832Screw* f832Screw2Ptr;
  MJChar832Screw* f832Screw3Ptr;
  MJChar832Screw* f832Screw4Ptr;
  MJChar1420Screw* f1420Screw1Ptr;
  MJChar1420Screw* f1420Screw2Ptr;
  MJChar1420Screw* f1420Screw3Ptr;
  MJChar1420Screw* f1420Screw4Ptr;

};
//
#endif
