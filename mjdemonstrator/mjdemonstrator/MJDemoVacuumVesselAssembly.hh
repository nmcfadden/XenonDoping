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
 * CLASS DECLARATION:  MJDemoVacuumVesselAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The Cryostat assembly
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

#ifndef _MJDEMOVACUUMVESSELASSEMBLY_HH
#define _MJDEMOVACUUMVESSELASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCryostatBottomLid;
class MJDemoCryostatTopLid;
class MJDemoCryostatHoop;
class MJDemoCryostatCrossArmTube;
class MJDemoCryostatRailAssembly;
class MJDemoCryostatSeal;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoVacuumVesselAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoVacuumVesselAssembly(G4String, G4String);
  MJDemoVacuumVesselAssembly(const MJDemoVacuumVesselAssembly &);
  ~MJDemoVacuumVesselAssembly();

  MJDemoCryostatBottomLid* GetBottomLid() {return fBottomLidPtr;}
  MJDemoCryostatTopLid* GetTopLid() {return fTopLidPtr;}
  MJDemoCryostatHoop* GetHoop() {return fHoopPtr;}
  MJDemoCryostatCrossArmTube* GetTube() {return fCrossArmTubePtr;}
  MJDemoCryostatRailAssembly* GetRailAssembly1() {return fRailAssembly1Ptr;}
  MJDemoCryostatRailAssembly* GetRailAssembly2() {return fRailAssembly2Ptr;}
  MJDemoCryostatSeal* GetSeal1() {return fSeal1Ptr;}
  MJDemoCryostatSeal* GetSeal2() {return fSeal2Ptr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoCryostatBottomLid* fBottomLidPtr;
  MJDemoCryostatTopLid* fTopLidPtr;
  MJDemoCryostatHoop* fHoopPtr;
  MJDemoCryostatCrossArmTube* fCrossArmTubePtr;
  MJDemoCryostatRailAssembly* fRailAssembly1Ptr;
  MJDemoCryostatRailAssembly* fRailAssembly2Ptr;
  MJDemoCryostatSeal* fSeal1Ptr;
  MJDemoCryostatSeal* fSeal2Ptr;

};
//
#endif
