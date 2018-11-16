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
/*
* 09-01-2017, moved overfloorpanels class to poly assembly, R. Massarczyk
*/

#ifndef _MJDEMOCONSTRRADSHIELDASSEMBLY_HH
#define _MJDEMOCONSTRRADSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoRadShieldCuInner;
class MJDemoRadShieldCuOuter;
class MJDemoRadShieldPb;
class MJDemoRadShieldShadowPb;
class MJDemoConstrRadShieldN2Volume;
class MJDemoRadShieldRnBox;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoConstrRadShieldAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoConstrRadShieldAssembly(G4String, G4String, G4String);
  MJDemoConstrRadShieldAssembly(const MJDemoConstrRadShieldAssembly &);
  ~MJDemoConstrRadShieldAssembly();

  MJDemoRadShieldCuInner* GetCuInner() {return fCuInnerPtr;}
  MJDemoRadShieldCuOuter* GetCuOuter() {return fCuOuterPtr;}
  MJDemoRadShieldPb* GetPb() {return fPbPtr;}
  MJDemoRadShieldShadowPb* GetShadow1() {return fShadow1Ptr;}
  MJDemoRadShieldShadowPb* GetShadow2() {return fShadow2Ptr;}
  MJDemoConstrRadShieldN2Volume* GetN2() {return fN2Ptr;}  
  MJDemoRadShieldRnBox* GetRnBox() {return fRnBoxPtr;}

  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  G4String fConstrConfig;
  MJDemoRadShieldCuInner* fCuInnerPtr;
  MJDemoRadShieldCuOuter* fCuOuterPtr;
  MJDemoRadShieldPb* fPbPtr;
  MJDemoRadShieldShadowPb* fShadow1Ptr;
  MJDemoRadShieldShadowPb* fShadow2Ptr;
  MJDemoConstrRadShieldN2Volume* fN2Ptr;
  MJDemoRadShieldRnBox* fRnBoxPtr;

};
//
#endif
