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
// CLASS IMPLEMENTATION:  MJSTCAssembly.hh
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
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 11-14-2013, Created, J. MacMullin
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield from messenger. J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJSTCASSEMBLY_HH
#define _MJSTCASSEMBLY_HH

//---------------------------------------------------------------------------//
#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCThermalAssembly;
class MJSTCThermalAssemblyThin;
class MJSTCColdPlateAssembly;
class MJSTCCMSAssembly;
class MJSTCMainBodyAssembly;
class MJSTCDipstickAssembly;
class MJSTCDewarAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCAssembly: public MJVDemoAssembly
{  
public:
  MJSTCAssembly(G4String, G4String, G4double, G4double, G4double);
  MJSTCAssembly(const MJSTCAssembly &);
  ~MJSTCAssembly();

  //MJSTCThermalAssembly* GetThermal() {return fThermalPtr;}
  MJSTCColdPlateAssembly* GetColdPlate() {return fColdPlatePtr;}
  MJSTCCMSAssembly* GetCMS() {return fCMSPtr;}
  MJSTCMainBodyAssembly* GetMainBody() {return fMainBodyPtr;}
  MJSTCDipstickAssembly* GetDipstick() {return fDipstickPtr;}
  MJSTCDewarAssembly* GetDewar() {return fDewarPtr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
                       
private:
  G4bool fThin;
  MJSTCThermalAssembly* fThermalPtr;
  MJSTCThermalAssemblyThin* fThermalThinPtr;
  MJSTCColdPlateAssembly* fColdPlatePtr;
  MJSTCCMSAssembly* fCMSPtr;
  MJSTCMainBodyAssembly* fMainBodyPtr;
  MJSTCDipstickAssembly* fDipstickPtr;
  MJSTCDewarAssembly* fDewarPtr;
  G4double fColdPlateAngle;
  G4double fCMSAngle;
  G4double fThermalAngle;

};
//
#endif
