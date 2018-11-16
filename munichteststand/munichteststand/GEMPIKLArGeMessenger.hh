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
 * FIRST SUBMISSION: 05-03-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMPIKLARGEMESSENGER_HH
#define _GEMPIKLARGEMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"

// --------------------------------------------------------------------------- 

class GEMPIKLArGe; 
class G4UIdirectory; 
class G4UIcmdWithADouble;
class G4UIcmdWithABool;

class GEMPIKLArGeMessenger : public G4UImessenger
{

public:

  //default constructor

  GEMPIKLArGeMessenger(GEMPIKLArGe*);

  // destructor

  ~GEMPIKLArGeMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEMPIKLArGe* fDetector; 

  G4UIdirectory* fMPIKLArGeDirectory;
 
  G4UIcmdWithADouble* fWLSYield;
  G4UIcmdWithABool* fCrystalInCmd;
};

// --------------------------------------------------------------------------- 

#endif
