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
 * CLASS DECLARATION:  GEOutputLArGeOpticalMessenger.hh
 *
 * DESCRIPTION:
 *
 *   Messenger class for the GEOutputLArGeOptical GERDA output scheme
 *
 * AUTHOR: N. Barros (forking from L. Pandola's GEOutputGermaniumArrayMessenger)
 *
 * FIRST SUBMISSION: 18-05-2012
 * 
 * REVISION: MM-DD-YYYY
 *
 * 05-18-2012, N. Barros, Forked
 *
 */

#ifndef _GEOUTPUTLARGEOPTICALMESSENGER_HH
#define _GEOUTPUTLARGEOPTICALMESSENGER_HH

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
class GEOutputLArGeOptical;

//---------------------------------------------------------------------------//

class GEOutputLArGeOpticalMessenger : public G4UImessenger
{
public:
	/*
	GEOutputLArGeArrayMessenger(GEOutputLArGeArray *theOutput);
	*/
  //default constructor
  GEOutputLArGeOpticalMessenger(GEOutputLArGeOptical *outputscheme);

  //destructor
  ~GEOutputLArGeOpticalMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:


  //private  members
private:
  GEOutputLArGeOptical  *fOutputScheme;
  G4UIdirectory        *fDirectory;

  // Commands
  G4UIcmdWithABool* fKillDaugNuclei; // /MG/output/killDaughter

  G4UIcmdWithABool           *fSetUseTimeWindowCmd;
  G4UIcmdWithADoubleAndUnit  *fSetTimeWindowCmd;

  G4UIcmdWithABool           *fSaveOnlyEventsWithEnergyDepositCmd;

  G4UIcmdWithABool           *fKillNeutrinos;
  G4UIcmdWithABool           *fReduceHitInfo1Cmd;


  G4UIcmdWithABool           *fCmd_KillGammasOutsideCone;
  G4UIcmdWith3VectorAndUnit  *fCmd_KillGammasOutsideCone_ArrayCenter;
  G4UIcmdWithADoubleAndUnit  *fCmd_KillGammasOutsideCone_StartCutRadius;
  G4UIcmdWithADoubleAndUnit  *fCmd_KillGammasOutsideCone_MaxAllowedOpeningAngle;

  G4UIcmdWithABool           *fWriteNumberOfEvents;

  G4UIcmdWithABool			*fCmd_LArInstApplyGeEnergyCut;
  G4UIcmdWithADoubleAndUnit	*fCmd_LArInstGeEnergyCutLow;
  G4UIcmdWithADoubleAndUnit	*fCmd_LArInstGeEnergyCutHigh;
  G4UIcmdWithABool			*fCmd_LArInstKillWLS;
  G4UIcmdWithADoubleAndUnit* fCmd_EGeHitThreshold;
};

#endif

