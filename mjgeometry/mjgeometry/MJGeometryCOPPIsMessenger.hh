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
 *
 *      
 * CLASS DECLARATION: MJGeometryCOPPIsMessenger.hh
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
 * AUTHOR: Mary Kidd
 * CONTACT: 
 * FIRST SUBMISSION:  Jun 26, 2012
 * 
 * REVISION:
 * 
 * 
 * 
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYCOPPIsMESSENGER_HH
#define _MJGEOMETRYCOPPIsMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"
#include "MJGeometryCOPPIs.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class MJGeometryCOPPIs;
class MJGeometryCOPPIsMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryCOPPIsMessenger(MJGeometryCOPPIs *Pauli);

  //destructor
  ~MJGeometryCOPPIsMessenger();

  //public interface
  void SetNewValue(G4UIcommand *cmd, G4String cmdstr);

  //protected members
protected:
  //private  members
private:
  G4UIdirectory             *fCOPPIsDir;
  G4UIcmdWithABool          *fUseOPPI3Cmd;
  G4UIcmdWithABool          *fUseOPPI4Cmd;
  G4UIcmdWithADoubleAndUnit *fSetDetSepCmd;
  G4UIcmdWithADoubleAndUnit *fSetOPPI3TDLCmd;
  G4UIcmdWithADoubleAndUnit *fSetOPPI4TDLCmd;
  G4UIcmdWithADoubleAndUnit *fSetOPPI3SDLCmd;
  G4UIcmdWithADoubleAndUnit *fSetOPPI4SDLCmd;
  G4UIcmdWithABool          *fUsePbCylCmd;
  G4UIcmdWithADoubleAndUnit *fSetPbLCmd;
  G4UIcmdWithABool          *fUseZnCmd;
  G4UIcmdWithABool          *fUseZrCmd;
  G4UIcmdWithABool          *fUseNbCmd;
  G4UIcmdWithABool          *fUseCdCmd;

  MJGeometryCOPPIs *fCOPPIsDetector;
};
#endif
