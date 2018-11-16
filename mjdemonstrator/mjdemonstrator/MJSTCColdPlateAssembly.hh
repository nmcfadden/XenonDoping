//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//                                                          
//      
// CLASS IMPLEMENTATION:  MJSTCColdPlateAssembly.hh
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION:
 * 10-17-2013, Created, J. MacMullin
 * 11-14-2013, Added adapter nut for attaching to adapter plate, J. MacMullin
 * 01-21-2014, Added ability to rotate ColdPlate and CMS from messenger. J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#ifndef _MJSTCCOLDPLATEASSEMBLY_HH
#define _MJSTCCOLDPLATEASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCColdPlateBody;
class MJDemo1032Nut;
class MJSTCColdPlateShuntStrap;

using namespace std;

//---------------------------------------------------------------------------//

class MJSTCColdPlateAssembly: public MJVDemoAssembly
{
  
public:
  MJSTCColdPlateAssembly(G4String, G4String);
  MJSTCColdPlateAssembly(const MJSTCColdPlateAssembly &);
  ~MJSTCColdPlateAssembly();

  MJSTCColdPlateBody* GetBody() {return fBodyPtr;}
  MJDemo1032Nut* GetAdapterNut01() {return fAdapterNut01Ptr;}
  MJDemo1032Nut* GetAdapterNut02() {return fAdapterNut02Ptr;}
  MJDemo1032Nut* GetAdapterNut03() {return fAdapterNut03Ptr;}
  MJSTCColdPlateShuntStrap* GetShuntStrap01() {return fShuntStrap01Ptr;}
  MJSTCColdPlateShuntStrap* GetShuntStrap02() {return fShuntStrap02Ptr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJSTCColdPlateBody* fBodyPtr;
  MJDemo1032Nut* fAdapterNut01Ptr;
  MJDemo1032Nut* fAdapterNut02Ptr;
  MJDemo1032Nut* fAdapterNut03Ptr;
  MJSTCColdPlateShuntStrap* fShuntStrap01Ptr;
  MJSTCColdPlateShuntStrap* fShuntStrap02Ptr;

};
//
#endif
