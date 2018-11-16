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
 * CLASS DECLARATION:  MJDemoCableGuideAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Inner cable guide with clip inserts
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
 * FIRST SUBMISSION: May 6, 2015
 * 
 * REVISION:
 * 
 * 05-06-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCABLEGUIDEASSEMBLY_HH
#define _MJDEMOCABLEGUIDEASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

class MJDemoInnerCableGuide;
class MJDemoCableGuideInsert;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCableGuideAssembly: public MJVDemoAssembly
{
public:
  MJDemoCableGuideAssembly(
	G4String fName, 
	G4String serialNumber
  );
  MJDemoCableGuideAssembly(const MJDemoCableGuideAssembly &);
  ~MJDemoCableGuideAssembly();

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  MJDemoInnerCableGuide* GetInnerCableGuide() {return fInnerCableGuidePtr;}
  MJDemoCableGuideInsert* GetGuideInsert1() {return fGuideInsert1Ptr;}
  MJDemoCableGuideInsert* GetGuideInsert2() {return fGuideInsert2Ptr;}
  

private:
  MJDemoInnerCableGuide* fInnerCableGuidePtr;
  MJDemoCableGuideInsert* fGuideInsert1Ptr;
  MJDemoCableGuideInsert* fGuideInsert2Ptr;
};
//
#endif
