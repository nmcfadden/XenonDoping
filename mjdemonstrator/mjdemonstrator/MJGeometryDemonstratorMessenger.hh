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
 * $Id: MJGeometryDemonstratorMessenger.hh,v 1.2 2005/01/06 01:09:02 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryDemonstratorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Messenger class for the demonstrator geometry. 
 * Define dimensions using G4 macros.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 * If the cables are included, there are overlapping geometries. -Jacquie
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: M. Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: Thursday May 5, 2011
 * 
 * REVISION:
 * 
 * 09-01-2017, Add commands for poly shield, R. Massarczyk
 * 02-24-2016, Add commands for the cryostat locations, T. Caldwell
 * 04-24-2014, Fixed overlapping geometries, J. MacMullin
 * 05-04-2011, Created, M. Kidd
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYDEMONSTRATORMESSENGER_HH
#define _MGGEOMETRYDEMONSTRATORMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometryDemonstrator;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;

//---------------------------------------------------------------------------//

class MJGeometryDemonstratorMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryDemonstratorMessenger(MJGeometryDemonstrator*);

  //destructor
  ~MJGeometryDemonstratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fDemonstratorDirectory;
  G4UIcmdWithAString          *fCryostatChoiceCmd;
  MJGeometryDemonstrator      *fDemonstrator; 
  G4UIcmdWithABool            *fCavernOnCmd;
  G4UIcmdWithABool            *fMuonVetoOnCmd;
  G4UIcmdWithABool            *fRadShieldOnCmd;
  //G4UIcmdWithABool            *fCal1OnCmd;
  //G4UIcmdWithABool            *fCryo1OnCmd;
  G4UIcmdWith3VectorAndUnit   *fCryo1PosCmd;
  G4UIcmdWithADoubleAndUnit   *fCryo1RotCmd;
  //G4UIcmdWithABool            *fThermosiphon1OnCmd;
  //G4UIcmdWithABool            *fThermalShield1OnCmd;
  //G4UIcmdWithABool            *fCables1OnCmd;
  //G4UIcmdWithABool            *fString1OnCmd;
  //G4UIcmdWithABool            *fCal2OnCmd;
  //G4UIcmdWithABool            *fCryo2OnCmd;
  G4UIcmdWith3VectorAndUnit   *fCryo2PosCmd;
  G4UIcmdWithADoubleAndUnit   *fCryo2RotCmd;
  //G4UIcmdWithABool            *fThermosiphon2OnCmd;
  //G4UIcmdWithABool            *fThermalShield2OnCmd;
  //G4UIcmdWithABool            *fCables2OnCmd;
  //G4UIcmdWithABool            *fString2OnCmd;
  G4UIcmdWithABool            *fInnerCopperOnCmd;
  G4UIcmdWithABool            *fTSShieldOnCmd;
  G4UIcmdWithAnInteger        *fShadowShieldThicknessCmd;
//  G4UIcmdWithoutParameter     *fSetDefaultsCmd;
  G4UIcmdWithAString          *fPolyShieldCmd;
};
#endif
