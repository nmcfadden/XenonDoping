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
// CLASS IMPLEMENTATION:  MJSTCThermalCuFoil.hh
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
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJSTCTHERMALCUFOIL_HH
#define _MJSTCTHERMALCUFOIL_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCThermalCuFoil: public MJVDemoPart
{
  
public:
  MJSTCThermalCuFoil(G4String, G4String);
  MJSTCThermalCuFoil(const MJSTCThermalCuFoil &);
  ~MJSTCThermalCuFoil();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
