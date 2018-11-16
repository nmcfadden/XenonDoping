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
 * CLASS DECLARATION:  MJDemoThermosyphonAndShieldAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *This assembly consists of the Thermosyphon, supports, connectors, cable
 *takeups, top and bottom ir shield assemblies
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
 * 09-07-2014, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOTHERMOSYPHONANDSHIELDASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONANDSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonAssembly;
class MJDemoBottomThermalShieldAssembly;
class MJDemoTopThermalShieldAssembly;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonAndShieldAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonAndShieldAssembly(G4String, G4String);
  MJDemoThermosyphonAndShieldAssembly(const MJDemoThermosyphonAndShieldAssembly &);
  ~MJDemoThermosyphonAndShieldAssembly();

  MJDemoThermosyphonAssembly* GetThermosyphon() const {return fTSPtr;}
  MJDemoThermosyphonAssembly* GetThermosyphon() {return fTSPtr;}
  MJDemoBottomThermalShieldAssembly* GetBottomShield() {return fBShieldPtr;}
  MJDemoTopThermalShieldAssembly* GetTopShield() {return fTShieldPtr;}
    
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoThermosyphonAssembly* fTSPtr;
  MJDemoBottomThermalShieldAssembly* fBShieldPtr;
  MJDemoTopThermalShieldAssembly* fTShieldPtr;

};
//
#endif
