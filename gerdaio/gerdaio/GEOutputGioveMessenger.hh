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
 * CLASS DECLARATION:  GEOutputGioveMessenger
 *
 * DESCRIPTION:
 *
 *   Messenger class for the GEOutputGiove GERDA output scheme
 *
 * REVISION: MM-DD-YYYY
 *
 *   06-09-2009 First version, Alan Poon
 *
 */
// --------------------------------------------------------------------------//

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
class GEOutputGiove;

//---------------------------------------------------------------------------//

class GEOutputGioveMessenger : public G4UImessenger 
{
public:
  //default constructor
  GEOutputGioveMessenger(GEOutputGiove *outputscheme);

  //destructor
  ~GEOutputGioveMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:


  //private  members
private:
  GEOutputGiove  *fOutputScheme;        
  G4UIdirectory        *fDirectory;

  // Commands
  //G4UIcmdWithABool* fKillDaugNuclei; // /MG/output/killDaughter
  G4UIcmdWithABool* fWriteHitDetails; 

};
#endif
