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
 * CLASS DECLARATION:  MJDemoTopThermalShieldAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The thermal shield assembly which resides above the cold plate.
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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 * 
 * REVISION:
 * 
 * 09-10-2010, Created, M. Green
 * 09-07-2014, renamed MJDemoTopThermalShieldAssembly, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOTOPTHERMALSHIELDASSEMBLY_HH
#define _MJDEMOTOPTHERMALSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermalShieldCanTop;
class MJDemoIRShieldTopBaffle;
class MJDemoCableGuardNarrowIRShield;
class MJDemoTopIRShieldSupport;
class MJDemoTopIRShieldSupportFarEnd;
class MJDemo832Screw;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoTopThermalShieldAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoTopThermalShieldAssembly(G4String, G4String);
  MJDemoTopThermalShieldAssembly(const MJDemoTopThermalShieldAssembly &);
  ~MJDemoTopThermalShieldAssembly();

  MJDemoThermalShieldCanTop* GetCan() {return fCanPtr;}
  MJDemoIRShieldTopBaffle* GetBaffle1() {return fBaffle1Ptr;}
  MJDemoIRShieldTopBaffle* GetBaffle2() {return fBaffle2Ptr;}
  MJDemoCableGuardNarrowIRShield* GetCableGuard() {return fCableGuardPtr;}
  MJDemoTopIRShieldSupport* GetNearSupport() {return fNearSupportPtr;}
  MJDemoTopIRShieldSupportFarEnd* GetFarSupport() {return fFarSupportPtr;}
  MJDemo832Screw* GetBaffleScrew1() {return fBaffleScrew1Ptr;}
  MJDemo832Screw* GetBaffleScrew2() {return fBaffleScrew2Ptr;}
  MJDemo832Screw* GetNearScrew1() {return fNearScrew1Ptr;}
  MJDemo832Screw* GetNearScrew2() {return fNearScrew2Ptr;}
  MJDemo832Screw* GetFarScrew() {return fFarScrewPtr;}
  
    
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJDemoThermalShieldCanTop* fCanPtr;
  MJDemoIRShieldTopBaffle* fBaffle1Ptr;
  MJDemoIRShieldTopBaffle* fBaffle2Ptr;
  MJDemoCableGuardNarrowIRShield* fCableGuardPtr;
  MJDemoTopIRShieldSupport* fNearSupportPtr;
  MJDemoTopIRShieldSupportFarEnd* fFarSupportPtr;
  MJDemo832Screw* fBaffleScrew1Ptr;
  MJDemo832Screw* fBaffleScrew2Ptr;
  MJDemo832Screw* fNearScrew1Ptr;
  MJDemo832Screw* fNearScrew2Ptr;
  MJDemo832Screw* fFarScrewPtr;

};
//
#endif
