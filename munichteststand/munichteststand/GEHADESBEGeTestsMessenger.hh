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
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Luciano Pandola
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 11-20-2011, Derived from GELNGSBEGeDetectorMessenger and implemented as new class in MaGe, Bjoern
 * 
 */
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

#ifndef _GEHADESBEGETESTMESSENGER_HH
#define _GEHADESBEGETESTMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"


// --------------------------------------------------------------------------- 

class GEHADESBEGeTests; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble; 
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class GEHADESBEGeTestsMessenger : public G4UImessenger
{

public:

  //default constructor

  GEHADESBEGeTestsMessenger(GEHADESBEGeTests*);

  // destructor

  ~GEHADESBEGeTestsMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEHADESBEGeTests* fDetectorPointer; 

  G4UIdirectory* fDetectorDir;

  G4UIcmdWithADoubleAndUnit* fCryostatWindowThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fCryostatWallThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fCryostatDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fCryostatHeightCmd;

  G4UIcmdWithADoubleAndUnit* fXtalDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fXtalHeightCmd;
  G4UIcmdWithADoubleAndUnit* fXtalDistanceToWindowCmd;
  G4UIcmdWithADoubleAndUnit* fXtalDitchInnerRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fXtalDitchOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fXtalDitchDepthCmd;
  G4UIcmdWithABool* fXtalDitchOnBottomCmd;
  G4UIcmdWithADoubleAndUnit* fXtalCornerDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fXtalCornerHeightCmd;
  G4UIcmdWithABool* fXtalCornerOnBottomCmd;

  G4UIcmdWithAString* fSpecialDetectorTypeCmd;
  G4UIcmdWithAString* fSourceTypeCmd;
  G4UIcmdWithADoubleAndUnit* fSourceDistanceCmd;

  G4UIcmdWithABool* fActivateEnrBEGeCryostatHoldersCmd;
  G4UIcmdWithABool* fActivateDepBEGeCryostatHoldersCmd;

  G4UIcmdWithAnInteger* fSetHADESLeadCastleTypeCmd;

  //For collimated Top and Side Scan
  G4UIcmdWithABool* fSetBRADYEnvCmd;
  G4UIcmdWithAnInteger* fSetCollimatorCmd;
  G4UIcmdWithADoubleAndUnit* fCollimatorDistanceCmd;  
  G4UIcmdWithADoubleAndUnit* fCollimatorPositionCmd;
  
  G4UIcmdWithAString* fCryostatFillMaterialCmd;
};

// --------------------------------------------------------------------------- 

#endif
