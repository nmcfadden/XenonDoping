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
 * CLASS DECLARATION:  MJDemoCryostatAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The Cryostat assembly, comprising the vaccum vessel, cold plate assembly,
 *thermosyphon and thermal shield assembly, and calibration track assembly.
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
 * CONTACT: mpgreen@ornl.gov
 * FIRST SUBMISSION: Oct 1, 2010
 * 
 * REVISION:
 * 
 * 02-24-2016, Add member for positioning calibration assembly, T. Caldwell
 * 10-01-2014, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCRYOSTATASSEMBLY_HH
#define _MJDEMOCRYOSTATASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoVacuumVesselAssembly;
class MJDemoVacuumCFAssembly;
class MJDemoColdPlateAssembly;
class MJDemoThermosyphonAndShieldAssembly;
class MJDemoCalibrationAssembly;
class MJDemo7StringAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCryostatAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCryostatAssembly(G4String, G4String);
  MJDemoCryostatAssembly(const MJDemoCryostatAssembly &);
  ~MJDemoCryostatAssembly();

  MJDemoVacuumVesselAssembly* GetVessel() {return fVesselPtr;}
  MJDemoVacuumCFAssembly* GetConflat() const {return fCFPtr;}
  MJDemoVacuumCFAssembly* GetConflat() {return fCFPtr;}
  MJDemoColdPlateAssembly* GetColdPlateAssem() {return fCPPtr;}
  MJDemoThermosyphonAndShieldAssembly* GetTSandS() const {return fTSPtr;}
  MJDemoThermosyphonAndShieldAssembly* GetTSandS() {return fTSPtr;}
  MJDemoCalibrationAssembly* GetCalibration() {return fCalPtr;}
  MJDemo7StringAssembly* Get7StringAssembly() const {return fStrPtr;}
  MJDemo7StringAssembly* Get7StringAssembly() {return fStrPtr;}

  void SetCalAssemblyPos(G4ThreeVector calAssemblyPos) {fCalAssemblyPos = calAssemblyPos;}
  G4ThreeVector GetCalAssemblyPos() const {return fCalAssemblyPos;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoVacuumVesselAssembly* fVesselPtr;
  MJDemoVacuumCFAssembly* fCFPtr;
  MJDemoColdPlateAssembly* fCPPtr;
  MJDemoThermosyphonAndShieldAssembly* fTSPtr;
  MJDemoCalibrationAssembly* fCalPtr;
  MJDemo7StringAssembly* fStrPtr;
  G4ThreeVector fCalAssemblyPos;
};
//
#endif
