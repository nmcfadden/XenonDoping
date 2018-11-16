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
// CLASS IMPLEMENTATION:  MJSTCThermalAssemblyThin.hh
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
 * AUTHOR: Chris O'Shaughnessy
 * CONTACT: chriso@unc.edu
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 09-28-2016, Created, C. O'Shaughnessy 
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJSTCTHERMALASSEMBLYTHIN_HH
#define _MJSTCTHERMALASSEMBLYTHIN_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCThermalAlFoil;
class MJSTCThermalIRShield;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCThermalAssemblyThin: public MJVDemoAssembly
{
  
public:
  MJSTCThermalAssemblyThin(G4String, G4String);
  MJSTCThermalAssemblyThin(const MJSTCThermalAssemblyThin &);
  ~MJSTCThermalAssemblyThin();

  MJSTCThermalIRShield* GetIRShield() {return fIRShieldPtr;}
  MJSTCThermalAlFoil* GetAlFoil() {return fAlFoilPtr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJSTCThermalIRShield* fIRShieldPtr;
  MJSTCThermalAlFoil* fAlFoilPtr;

};
//
#endif
