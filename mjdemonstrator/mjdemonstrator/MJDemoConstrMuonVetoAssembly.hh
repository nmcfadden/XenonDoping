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

#ifndef _MJDEMOCONSTRMUONVETOASSEMBLY_HH
#define _MJDEMOCONSTRMUONVETOASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoMuonVetoSolidPanel;
class MJDemoMuonVetoThroughPanel;
class MJDemoMuonVetoOverFloorAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoConstrMuonVetoAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoConstrMuonVetoAssembly(G4String, G4String, G4String);
  MJDemoConstrMuonVetoAssembly(const MJDemoConstrMuonVetoAssembly &);
  ~MJDemoConstrMuonVetoAssembly();

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
  G4String fConstrConfig;
  
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
