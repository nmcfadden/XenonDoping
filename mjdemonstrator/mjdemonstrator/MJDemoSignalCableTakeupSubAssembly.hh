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
 * CLASS DECLARATION:  MJDemoSignalCableTakeupSubAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Sub-assembly conisisting of the signal cable takeups and mounting screws
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 * 
 * REVISION:
 * 
 * 07-30-2014, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGNALCABLETAKEUPSUBASSEMBLY_HH
#define _MJDEMOSIGNALCABLETAKEUPSUBASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoSignalCableTakeupLowBulkhead;
class MJDemo832Screw;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSignalCableTakeupSubAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoSignalCableTakeupSubAssembly(G4String, G4String);
  MJDemoSignalCableTakeupSubAssembly(const MJDemoSignalCableTakeupSubAssembly &);
  ~MJDemoSignalCableTakeupSubAssembly();

  MJDemoSignalCableTakeupLowBulkhead* GetClamp() {return fTakeupPtr;}
  MJDemo832Screw* GetScrew01() {return fScrew01Ptr;}
  MJDemo832Screw* GetScrew02() {return fScrew02Ptr;}

    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:

  MJDemoSignalCableTakeupLowBulkhead* fTakeupPtr;
  MJDemo832Screw* fScrew01Ptr;
  MJDemo832Screw* fScrew02Ptr;

    
};
//
#endif
