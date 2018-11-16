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
 * CLASS DECLARATION:  MJDemoMuonVetoAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The muon veto sheild assembly
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
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: never
 * 
 * REVISION:
 * 
 * 12-07-2010, Created, M. Kidd
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOMUONVETOASSEMBLY_HH
#define _MJDEMOMUONVETOASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoMuonVetoSolidPanel;
class MJDemoMuonVetoThroughPanel;
class MJDemoMuonVetoOverFloorAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoMuonVetoAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoMuonVetoAssembly(G4String, G4String);
  MJDemoMuonVetoAssembly(const MJDemoMuonVetoAssembly &);
  ~MJDemoMuonVetoAssembly();

  MJDemoMuonVetoSolidPanel* GetSolidPanel1() {return fShortSolidTopPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel2() {return fShortSolidBottomPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel3() {return fShortSolidLeftPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel4() {return fShortSolidRightPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel5() {return fLongSolidTopPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel6() {return fLongSolidBottomPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel7() {return fLongSolidLeftPtr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel8() {return fLongSolidRightPtr;}

  MJDemoMuonVetoSolidPanel* GetSolidPanel9() {return fTopOuter1Ptr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel10() {return fTopOuter2Ptr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel11() {return fTopInner1Ptr;}
  MJDemoMuonVetoSolidPanel* GetSolidPanel12() {return fTopInner2Ptr;}

  MJDemoMuonVetoThroughPanel* GetThroughPanel1() {return fShortThroughTopPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel2() {return fShortThroughBottomPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel3() {return fShortThroughLeftPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel4() {return fShortThroughRightPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel5() {return fLongThroughTopPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel6() {return fLongThroughBottomPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel7() {return fLongThroughLeftPtr;}
  MJDemoMuonVetoThroughPanel* GetThroughPanel8() {return fLongThroughRightPtr;}

  MJDemoMuonVetoOverFloorAssembly* GetOverFloorAssm1() {return fOverFloorAssmPtr1;}
  MJDemoMuonVetoOverFloorAssembly* GetOverFloorAssm2() {return fOverFloorAssmPtr2;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoMuonVetoSolidPanel* fShortSolidTopPtr;
  MJDemoMuonVetoSolidPanel* fShortSolidBottomPtr;
  MJDemoMuonVetoSolidPanel* fShortSolidLeftPtr;
  MJDemoMuonVetoSolidPanel* fShortSolidRightPtr;
  MJDemoMuonVetoSolidPanel* fLongSolidTopPtr;
  MJDemoMuonVetoSolidPanel* fLongSolidBottomPtr;
  MJDemoMuonVetoSolidPanel* fLongSolidLeftPtr;
  MJDemoMuonVetoSolidPanel* fLongSolidRightPtr;

  MJDemoMuonVetoSolidPanel* fTopOuter1Ptr;
  MJDemoMuonVetoSolidPanel* fTopOuter2Ptr;
  MJDemoMuonVetoSolidPanel* fTopInner1Ptr;
  MJDemoMuonVetoSolidPanel* fTopInner2Ptr;

  MJDemoMuonVetoThroughPanel* fShortThroughTopPtr;
  MJDemoMuonVetoThroughPanel* fShortThroughBottomPtr;
  MJDemoMuonVetoThroughPanel* fShortThroughLeftPtr;
  MJDemoMuonVetoThroughPanel* fShortThroughRightPtr;
  MJDemoMuonVetoThroughPanel* fLongThroughTopPtr;
  MJDemoMuonVetoThroughPanel* fLongThroughBottomPtr;
  MJDemoMuonVetoThroughPanel* fLongThroughLeftPtr;
  MJDemoMuonVetoThroughPanel* fLongThroughRightPtr;

  MJDemoMuonVetoOverFloorAssembly* fOverFloorAssmPtr1;
  MJDemoMuonVetoOverFloorAssembly* fOverFloorAssmPtr2;
};
//
#endif
