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
 * CLASS DECLARATION:  MJDemoBottomThermalShieldAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *The thermal shield assembly
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
 * 09-07-2014, renamed MJDemoBottomThermalShieldAssembly, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOBOTTOMTHERMALSHIELDASSEMBLY_HH
#define _MJDEMOBOTTOMTHERMALSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermalShieldCan;
class MJDemoThermalShieldAnnulus;
class MJDemoThermalShieldSupport;
class MJDemoThermalShieldWedge;
class MJDemo832Screw;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoBottomThermalShieldAssembly: public MJVDemoAssembly
{

public:
  MJDemoBottomThermalShieldAssembly(G4String, G4String);
  MJDemoBottomThermalShieldAssembly(const MJDemoBottomThermalShieldAssembly &);
  ~MJDemoBottomThermalShieldAssembly();

  MJDemoThermalShieldCan* GetCan() {return fCanPtr;}
  MJDemoThermalShieldAnnulus* GetAnnulus() {return fAnnulusPtr;}
  MJDemoThermalShieldSupport* GetSupport1() {return fSupport1Ptr;}
  MJDemoThermalShieldSupport* GetSupport2() {return fSupport2Ptr;}
  MJDemoThermalShieldSupport* GetSupport3() {return fSupport3Ptr;}
  MJDemoThermalShieldSupport* GetCenterPin1() {return fCenterPin1Ptr;}
  MJDemoThermalShieldSupport* GetCenterPin2() {return fCenterPin2Ptr;}
  MJDemoThermalShieldSupport* GetCenterPin3() {return fCenterPin3Ptr;}
  MJDemoThermalShieldWedge* GetWedge1() {return fWedge1Ptr;}
  MJDemoThermalShieldWedge* GetWedge2() {return fWedge2Ptr;}
  MJDemoThermalShieldWedge* GetWedge3() {return fWedge3Ptr;}
  MJDemo832Screw* GetScrew1() {return fScrew1Ptr;}
  MJDemo832Screw* GetScrew2() {return fScrew2Ptr;}
  MJDemo832Screw* GetScrew3() {return fScrew3Ptr;}
  MJDemo832Screw* GetScrew4() {return fScrew4Ptr;}
  MJDemo832Screw* GetScrew5() {return fScrew5Ptr;}
  MJDemo832Screw* GetScrew6() {return fScrew6Ptr;}
  MJDemo832Screw* GetScrew7() {return fScrew7Ptr;}
  MJDemo832Screw* GetScrew8() {return fScrew8Ptr;}
  MJDemo832Screw* GetSideScrew1() {return fSideScrew1Ptr;}
  MJDemo832Screw* GetSideScrew2() {return fSideScrew2Ptr;}
  MJDemo832Screw* GetSideScrew3() {return fSideScrew3Ptr;}
  MJDemo832Screw* GetSideScrew4() {return fSideScrew4Ptr;}
  MJDemo832Screw* GetSideScrew5() {return fSideScrew5Ptr;}
  MJDemo832Screw* GetSideScrew6() {return fSideScrew6Ptr;}


  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

private:
  MJDemoThermalShieldCan* fCanPtr;
  MJDemoThermalShieldAnnulus* fAnnulusPtr;
  MJDemoThermalShieldSupport* fSupport1Ptr;
  MJDemoThermalShieldSupport* fSupport2Ptr;
  MJDemoThermalShieldSupport* fSupport3Ptr;
  MJDemoThermalShieldSupport* fCenterPin1Ptr;
  MJDemoThermalShieldSupport* fCenterPin2Ptr;
  MJDemoThermalShieldSupport* fCenterPin3Ptr;
  MJDemoThermalShieldWedge* fWedge1Ptr;
  MJDemoThermalShieldWedge* fWedge2Ptr;
  MJDemoThermalShieldWedge* fWedge3Ptr;
  MJDemo832Screw* fScrew1Ptr;
  MJDemo832Screw* fScrew2Ptr;
  MJDemo832Screw* fScrew3Ptr;
  MJDemo832Screw* fScrew4Ptr;
  MJDemo832Screw* fScrew5Ptr;
  MJDemo832Screw* fScrew6Ptr;
  MJDemo832Screw* fScrew7Ptr;
  MJDemo832Screw* fScrew8Ptr;
  MJDemo832Screw* fSideScrew1Ptr;
  MJDemo832Screw* fSideScrew2Ptr;
  MJDemo832Screw* fSideScrew3Ptr;
  MJDemo832Screw* fSideScrew4Ptr;
  MJDemo832Screw* fSideScrew5Ptr;
  MJDemo832Screw* fSideScrew6Ptr;

};
//
#endif
