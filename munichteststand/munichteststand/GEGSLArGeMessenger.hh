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
 * 01-04-2006, Added commands, Luciano
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEGSLARGEMESSENGER_HH
#define _GEGSLARGEMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"

// --------------------------------------------------------------------------- 

class GEGSLArGe; 
class G4UIdirectory; 
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class GEGSLArGeMessenger : public G4UImessenger
{

public:

  //default constructor

  GEGSLArGeMessenger(GEGSLArGe*);

  // destructor

  ~GEGSLArGeMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEGSLArGe* fDetector; 

  G4UIdirectory* fGSLArGeDirectory;
  
  G4UIcmdWithAnInteger* fNumCol;
  G4UIcmdWithAnInteger* fCryPerCol;

  G4UIcmdWithADoubleAndUnit* fColDistance;
  G4UIcmdWithADoubleAndUnit* fColGap;

  G4UIcmdWithABool* fTrueCoax;

  G4UIcmdWithABool* fEncapsulation;

  G4UIcmdWithAnInteger* fNumPhiSegments;
  G4UIcmdWithAnInteger* fNumZSegments;

};

// --------------------------------------------------------------------------- 

#endif
