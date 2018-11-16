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
 * CLASS DECLARATION:  MJDemoCryostatRailAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The CryostatRail assembly
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
 * FIRST SUBMISSION: Oct 1, 2010
 * 
 * REVISION:
 * 
 * 10-01-2010, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCRYOSTATRAILASSEMBLY_HH
#define _MJDEMOCRYOSTATRAILASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCryostatLidRail;
class MJDemoCryostatCenterRail;
class MJDemoCryostatBolt;
class MJDemoCryostatNut;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCryostatRailAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCryostatRailAssembly(G4String, G4String);
  MJDemoCryostatRailAssembly(const MJDemoCryostatRailAssembly &);
  ~MJDemoCryostatRailAssembly();

  MJDemoCryostatLidRail* GetLidRail1() {return fLidRail1Ptr;}
  MJDemoCryostatLidRail* GetLidRail2() {return fLidRail2Ptr;}
  MJDemoCryostatLidRail* GetLidRail3() {return fLidRail3Ptr;}
  MJDemoCryostatLidRail* GetLidRail4() {return fLidRail4Ptr;}
  MJDemoCryostatCenterRail* GetCenterRail1() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail2() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail3() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail4() {return fCenterRail1Ptr;} 
  MJDemoCryostatBolt* GetBolt1() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt2() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt3() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt4() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt5() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt6() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt7() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt8() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt9() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt10() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt11() {return fBolt1Ptr;}
  MJDemoCryostatBolt* GetBolt12() {return fBolt1Ptr;}
  MJDemoCryostatNut* GetNut1() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut2() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut3() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut4() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut5() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut6() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut7() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut8() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut9() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut10() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut11() {return fNut1Ptr;}
  MJDemoCryostatNut* GetNut12() {return fNut1Ptr;}


  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

    
private:
  MJDemoCryostatLidRail*    fLidRail1Ptr;
  MJDemoCryostatLidRail*    fLidRail2Ptr;
  MJDemoCryostatLidRail*    fLidRail3Ptr;
  MJDemoCryostatLidRail*    fLidRail4Ptr;
  MJDemoCryostatCenterRail* fCenterRail1Ptr; 
  MJDemoCryostatCenterRail* fCenterRail2Ptr; 
  MJDemoCryostatCenterRail* fCenterRail3Ptr; 
  MJDemoCryostatCenterRail* fCenterRail4Ptr; 
  MJDemoCryostatBolt*       fBolt1Ptr;
  MJDemoCryostatBolt*       fBolt2Ptr;
  MJDemoCryostatBolt*       fBolt3Ptr;
  MJDemoCryostatBolt*       fBolt4Ptr;
  MJDemoCryostatBolt*       fBolt5Ptr;
  MJDemoCryostatBolt*       fBolt6Ptr;
  MJDemoCryostatBolt*       fBolt7Ptr;
  MJDemoCryostatBolt*       fBolt8Ptr;
  MJDemoCryostatBolt*       fBolt9Ptr;
  MJDemoCryostatBolt*       fBolt10Ptr;
  MJDemoCryostatBolt*       fBolt11Ptr;
  MJDemoCryostatBolt*       fBolt12Ptr;  
  MJDemoCryostatNut*        fNut1Ptr;
  MJDemoCryostatNut*        fNut2Ptr;
  MJDemoCryostatNut*        fNut3Ptr;
  MJDemoCryostatNut*        fNut4Ptr;
  MJDemoCryostatNut*        fNut5Ptr;
  MJDemoCryostatNut*        fNut6Ptr;
  MJDemoCryostatNut*        fNut7Ptr;
  MJDemoCryostatNut*        fNut8Ptr;
  MJDemoCryostatNut*        fNut9Ptr;
  MJDemoCryostatNut*        fNut10Ptr;
  MJDemoCryostatNut*        fNut11Ptr;
  MJDemoCryostatNut*        fNut12Ptr;  
};
//
#endif
