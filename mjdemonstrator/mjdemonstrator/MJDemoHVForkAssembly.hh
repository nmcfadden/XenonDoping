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
 * CLASS DECLARATION:  MJDemoHVForkAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Assembly of fork and vespel pin.
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
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Apr 27, 2015
 * 
 * REVISION:
 * 
 * 04-27-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOHVFORKASSEMBLY_HH
#define _MJDEMOHVFORKASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

class MJDemoHVFork;
class MJDemoHVForkClampPin;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoHVForkAssembly: public MJVDemoAssembly
{
public:
  MJDemoHVForkAssembly(
	G4String fName, 
	G4String serialNumber
  );
  MJDemoHVForkAssembly(const MJDemoHVForkAssembly &);
  ~MJDemoHVForkAssembly();

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  MJDemoHVFork* GetHVFork() {return fHVForkPtr;}
  MJDemoHVForkClampPin* GetClampPin() {return fClampPinPtr;}

private:
  MJDemoHVFork* fHVForkPtr;
  MJDemoHVForkClampPin* fClampPinPtr;
};
//
#endif
