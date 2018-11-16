
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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 28 Jun 2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 12-01-2008, added dead layer parameter, z position, Dusan & Marik
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEGDLSTANDMESSENGER_HH
#define _GEGDLSTANDMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"

// --------------------------------------------------------------------------- 

class GEGDLStand; 
class G4UIdirectory; 
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

class GEGDLStandMessenger : public G4UImessenger
{

public:

  //default constructor

  GEGDLStandMessenger(GEGDLStand*);

  // destructor

  ~GEGDLStandMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEGDLStand* fDetector; 
  G4UIdirectory* fGDLStandDirectory;
  G4UIcmdWithADoubleAndUnit* fDeadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fShiftDetectorCmd;
  G4UIcmdWithADoubleAndUnit* fDetectorDiamCmd;
  G4UIcmdWithADoubleAndUnit* fDetectorHeightCmd;
  G4UIcmdWithADoubleAndUnit* fHoleDiamCmd;
  G4UIcmdWithADoubleAndUnit* fDetHoleHeightCmd;
  G4UIcmdWithADoubleAndUnit* fGrooveOutDiamCmd;
  G4UIcmdWithADoubleAndUnit* fGrooveThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fGrooveInDiamCmd;
  G4UIcmdWithADoubleAndUnit* fCutParameterCmd;
  G4UIcmdWithADoubleAndUnit* fCutPositionCmd;
  G4UIcmdWithADoubleAndUnit* fCuttopParameterCmd;
  G4UIcmdWithADoubleAndUnit* fCuttopPositionCmd;

};

// --------------------------------------------------------------------------- 

#endif
