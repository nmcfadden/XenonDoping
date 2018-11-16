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
 * 05-26-2009, Added in MaGe, Luciano
 * 04-28-2010, Added command to set detector type, Luciano
 */
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

#ifndef _GELNGSBEGEDETECTORMESSENGER_HH
#define _GELNGSBEGEDETECTORMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"


// --------------------------------------------------------------------------- 

class GELNGSBEGeDetector; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble; 


class GELNGSBEGeDetectorMessenger : public G4UImessenger
{

public:

  //default constructor

  GELNGSBEGeDetectorMessenger(GELNGSBEGeDetector*);

  // destructor

  ~GELNGSBEGeDetectorMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GELNGSBEGeDetector* fDetectorPointer; 

  G4UIdirectory* fDetectorDir;
  G4UIcmdWithADoubleAndUnit* fBottomDLCmd;
  G4UIcmdWithADoubleAndUnit* fTopDLCmd;
  G4UIcmdWithAString* fDetectorTypeCmd;

  //G4UIcmdWithAString*        fSampleTypeCmd;  
};

// --------------------------------------------------------------------------- 

#endif
