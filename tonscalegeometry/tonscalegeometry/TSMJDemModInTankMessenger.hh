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
 * $Id: TSMJDemModInTankMessenger.hh,v 1.1 2011-03-11  $
 *      
 * CLASS DECLARATION:  TSMJDemModInTankMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
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
 * AUTHOR: R.Martin
 * CONTACT: 
 * FIRST SUBMISSION: 11 Mar 2011
 * 
 * REVISION:
 * 
 * 03-11-2011 R. Martin created
 * 06-13-2011 Added commands for variable sized modules, R.Martin
 * 03-14-2012 R. Martin added command to set height of world volume
 * 07-30-2012, Added commands to set string radial clearance and radial
 *             spacing between strings, K. Nguyen
 */
// --------------------------------------------------------------------------//

#ifndef _TSMJDEMMODINTANKMESSENGER_HH
#define _TSMJDEMMODINTANKMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class TSMJDemModInTank;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcommand;

class TSMJDemModInTankMessenger : public G4UImessenger
{
  public:
    TSMJDemModInTankMessenger(TSMJDemModInTank *assy);
    ~TSMJDemModInTankMessenger();
  
    void SetNewValue(G4UIcommand* cmd, G4String nV);

  private:
    G4UIdirectory    *fDirectory;
    TSMJDemModInTank *fMJDemInTank;
  
    // Commands.
    G4UIcmdWithADoubleAndUnit *fTankVerticalOffsetCmd;	
    G4UIcmdWithADoubleAndUnit *fCrystalDCmd;
    G4UIcmdWithADoubleAndUnit *fCrystalHCmd;
    G4UIcmdWithADoubleAndUnit *fWorldHCmd;	
    G4UIcmdWithADoubleAndUnit *fStringRadialClearanceCmd;       // FIXME: Added by K. Nguyen
    G4UIcmdWithADoubleAndUnit *fRadialSpacingBetweenStringsCmd;	// FIXME: Added by K. Nguyen
    G4UIcmdWithABool          *fTankIsActiveCmd;
    G4UIcmdWithAString        *fModuleTypeCmd;
    G4UIcmdWithAString        *fCrystalAssemblyTypeCmd;	
    G4UIcmdWithAnInteger      *fCrystalsPerStringCmd;
    G4UIcmdWithAnInteger      *fnModulesCmd;	
};
#endif
