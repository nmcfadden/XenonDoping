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
 * $Id: MJGeometryIdealCoaxWithShieldMessenger.hh,v 1.1 2005-02-16 18:27:08 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryIdealCoaxWithShieldMessenger.hh
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Feb 14 15:33:13 PST 2005
 * 
 * REVISION:
 * 
 * 02-14-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYIDEALCOAXWITHSHIELDMESSENGER_HH
#define _MJGEOMETRYIDEALCOAXWITHSHIELDMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJGeometryIdealCoaxWithShield;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcommand;

class MJGeometryIdealCoaxWithShieldMessenger : public G4UImessenger
{
public:

  MJGeometryIdealCoaxWithShieldMessenger(MJGeometryIdealCoaxWithShield *assy);

  ~MJGeometryIdealCoaxWithShieldMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nV);

private:
  G4UIdirectory                     *fDirectory;
  MJGeometryIdealCoaxWithShield     *fICWS;

  // Commands.
  G4UIcmdWithADoubleAndUnit         *fAssyHeightCmd;
  G4UIcmdWithADoubleAndUnit         *fAssyRadiusCmd;
  G4UIcmdWithAString                *fColdplateMaterialCmd;
  G4UIcmdWithADoubleAndUnit         *fColdplateThicknessCmd;
  G4UIcmdWithoutParameter           *fDumpCmd; 
  G4UIcmdWithAString                *fEndcapMaterialCmd; 
  G4UIcmdWithADoubleAndUnit         *fEndcapThicknessCmd;
  G4UIcmdWithABool                  *fShieldOnCmd;

};
#endif
