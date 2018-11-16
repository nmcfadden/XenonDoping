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
 * CLASS DECLARATION:  GEOutputDarioMessenger.hh
 *
 * DESCRIPTION: 
 *
 *   Messenger class for the GEOutputDario GERDA output scheme
 *
 * AUTHOR: L. Pandola
 *
 * FIRST SUBMISSION: 11-28-2007
 *
 * REVISION: MM-DD-YYYY
 *
 * 11-28-2007, Created, L. Pandola
 * 04-11-2008, Added flag to save no-energy-deposit-in-detector events, X. Liu
 * 01-07-2009  j. schubert  "/MG/output/killGammasOutsideCone true" can be used to kill gamma tracks outside a certain cone
 *                              Example for usage:
 *                                 /MG/output/killGammasOutsideCone true
 *                                 /MG/output/GammaConeCut_ArrayCenter 0 0 19.5 cm 
 *                                 /MG/output/GammaConeCut_StartCutRadius 80 cm
 *                                 /MG/output/GammaConeCut_MaxAllowedOpeningAngle 30 deg
 *                              If  a gamma is created at position vecX and has momentum vecP then it is killed in case of
 *                               - momentum points not into a cone directed towards the array center,
 *                                 i.e. angle[vecP,GammaConeCut_ArrayCenter-vecX] < MaxAllowedOpeningAngle
 *                               - _AND_ it has to be outside a certain spherical range arround 'GammaConeCut_ArrayCenter'
 *
 * 03-18-2009, j. schubert, "/MG/output/WriteNumberOfEventsToFile true" can be used to write the number of events 
 *                           to the output ROOT output-file
 *
 * 02-03-2010, Add commands to manage ASCII output
 */

#ifndef _GEOUTPUTGERMANIUMARRAYMESSENGER_HH
#define _GEOUTPUTGERMANIUMARRAYMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;
class G4UIdirectory;
class GEOutputDario;

//---------------------------------------------------------------------------//

class GEOutputDarioMessenger : public G4UImessenger 
{
public:
  //default constructor
  GEOutputDarioMessenger(GEOutputDario *outputscheme);

  //destructor
  ~GEOutputDarioMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:


  //private  members
private:
  GEOutputDario  *fOutputScheme;
  G4UIdirectory        *fDirectory;

  // Commands
  //G4UIcmdWithABool* fKillDaugNuclei; // /MG/output/killDaughter
  G4UIcmdWithABool* fWriteHitDetails; 
  G4UIcmdWithAnInteger* fSetMajorityCmd;
  G4UIcmdWithABool* fWriteLocalCoordinatesCmd;
};

#endif

