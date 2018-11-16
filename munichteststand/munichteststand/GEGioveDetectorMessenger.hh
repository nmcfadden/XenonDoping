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

#ifndef _GEGIOVEDETECTORMESSENGER_HH
#define _GEGIOVEDETECTORMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"


// --------------------------------------------------------------------------- 

class GEGioveDetector; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble; 


class GEGioveDetectorMessenger : public G4UImessenger
{

public:

  //default constructor

  GEGioveDetectorMessenger(GEGioveDetector*);

  // destructor

  ~GEGioveDetectorMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEGioveDetector* fDetectorPointer; 
  G4UIdirectory* fDetectorDir;

  G4UIcmdWithAnInteger* fShieldingConfigCmd;
  G4UIcmdWithAnInteger* fNumberOfLayersCmd;
  G4UIcmdWithADoubleAndUnit* fBoxHeightCmd;
  G4UIcmdWithADoubleAndUnit* fBoxWidthCmd; 
  G4UIcmdWithADoubleAndUnit* fLayerThicknessCmd;  

};

// --------------------------------------------------------------------------- 

#endif
