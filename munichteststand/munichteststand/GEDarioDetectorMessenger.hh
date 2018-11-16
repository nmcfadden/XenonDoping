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
// ---------------------------------------------------------------------------

#ifndef _GEDARIODETECTORMESSENGER_HH
#define _GEDARIODETECTORMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"


// --------------------------------------------------------------------------- 

class GEDarioDetector; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble; 


class GEDarioDetectorMessenger : public G4UImessenger
{

public:

  //default constructor

  GEDarioDetectorMessenger(GEDarioDetector*);

  // destructor

  ~GEDarioDetectorMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEDarioDetector* fDetectorPointer; 

  G4UIdirectory* fDetectorDir;

  G4UIcmdWithAString*        fSampleTypeCmd;
  G4UIcmdWithAString*        fSampleMaterialCmd;
  G4UIcmdWithADoubleAndUnit* fBoxHeightCmd;
  G4UIcmdWithADoubleAndUnit* fBoxWidthCmd; 
  G4UIcmdWithADoubleAndUnit* fBoxThicknessCmd;  
  G4UIcmdWithADoubleAndUnit* fTubeInnerRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeLengthCmd;
  //G4UIcmdWithADoubleAndUnit* fTubeAngleCmd;
  //G4UIcmdWithADoubleAndUnit* fSphereRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSamplexposCmd;
  G4UIcmdWithADoubleAndUnit* fSampleyposCmd;
  G4UIcmdWithADoubleAndUnit* fSamplezposCmd;
  G4UIcmdWithADoubleAndUnit* fDetzposCmd;
  G4UIcmdWithADoubleAndUnit* fDeadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fDetDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fDetLenghtCmd;
  //G4UIcmdWithADouble* fDetSizeFactorCmd;
  
};

// --------------------------------------------------------------------------- 

#endif
