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
 * $Id: MGGeneratorWangNeutronsMessenger.hh,v 1.2 2005-10-22 21:11:19 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorWangNeutronsMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for Wang neutrons generator.
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
 * AUTHOR: K.Hudek
 * CONTACT: 
 * FIRST SUBMISSION: Fri Jun 17 14:41:25 PDT 2005
 * 
 * REVISION:
 * 
 * 10-17-2005, Added fMinNeutronEnergyCmd, R. Henning
 * 
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORWANGNEUTRONSMESSENGER_HH
#define _MGGENERATORWANGNEUTRONSMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class MGGeneratorWangNeutrons;

class MGGeneratorWangNeutronsMessenger : public G4UImessenger
{
public:


  //default constructor
  MGGeneratorWangNeutronsMessenger(MGGeneratorWangNeutrons *generator);

  //copy constructor
  MGGeneratorWangNeutronsMessenger(const MGGeneratorWangNeutronsMessenger &);

  //destructor
  ~MGGeneratorWangNeutronsMessenger();

  //public interface

  G4String GetCurrentValue(G4UIcommand *cmd);
  void SetNewValue(G4UIcommand *cmd, G4String newValues);

  //If user defines minimum muon energy in mac, doesn't run calculation
  //to reset min energy in MGGeneratorWangNeutrons.cc
  bool GetMinMuonBool() { return fMinMuonBool; }

  //protected members
protected:



  //private  members
private:
  bool fMinMuonBool;

  MGGeneratorWangNeutrons    *fWangNeutronsGenerator;

  G4UIdirectory              *fWangNeutronsDirectory;

  G4UIcmdWithoutParameter    *fDumpCmd;
  G4UIcmdWithADoubleAndUnit  *fMaxMuonEnergyCmd;
  G4UIcmdWithADoubleAndUnit  *fMinMuonEnergyCmd;
  G4UIcmdWithADoubleAndUnit  *fNeutronEnergyCutoffCmd;
  G4UIcmdWithADoubleAndUnit  *fMinNeutronEnergyCmd;
  G4UIcmdWithAnInteger       *fSamplePointsCmd;
  G4UIcmdWithADoubleAndUnit  *fDepthCmd;
  G4UIcmdWithADoubleAndUnit  *fCaveXCmd;
  G4UIcmdWithADoubleAndUnit  *fCaveYCmd;
  G4UIcmdWith3VectorAndUnit  *fOriginCmd;
  G4UIcmdWithAString         *fHistoFileCmd;
};

#endif
