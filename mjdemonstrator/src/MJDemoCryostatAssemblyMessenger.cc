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
//                                                          
// $Id: MJDemoCryostatAssemblyMessenger.cc,v 1.1 2016/02/24 17:30:00 tcald Exp$ 
//      
// CLASS IMPLEMENTATION:  MJDemoCryostatAssemblyMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Messenger class for the cryostat assembly
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Tom Caldwell
 * CONTACT: tcald@unc.edu
 * FIRST SUBMISSION: Feb 24, 2016
 * 
 * REVISION:
 * 
 * 02-24-2016, Created, T. Caldwell
 */
//---------------------------------------------------------------------------//
//

#include "mjdemonstrator/MJDemoCryostatAssemblyMessenger.hh"
#include "io/MGLogger.hh"
#include "G4UImanager.hh"

//---------------------------------------------------------------------------//

MJDemoCryostatAssemblyMessenger::MJDemoCryostatAssemblyMessenger(G4String index)
{
  std::string dir = "/MG/mjdemocryoassembly" + index + "/";
  fCryoAssembly = new MJDemoCryostatAssembly("tmp_cal", "tmp_cal");
  fCalAssemblyMessenger = new MJDemoCalibrationAssemblyMessenger(index);
  
  // /MG/mjdemocryoassembly
  fCryoAssemblyDir = new G4UIdirectory(G4String(dir));

  // /MG/mjdemocryoassembly/calAssemblyPos
  fCalAssemblyPosCmd =
    new G4UIcmdWith3VectorAndUnit(G4String(dir + "calAssemblyPos"), this);
  fCalAssemblyPosCmd->SetGuidance("Position of the calibration assembly.");
  fCalAssemblyPosCmd->SetDefaultValue(fCryoAssembly->GetCalAssemblyPos());
  fCalAssemblyPosCmd->SetDefaultUnit("cm");
  fCalAssemblyPosCmd->SetUnitCandidates("mm cm m");
}

//---------------------------------------------------------------------------//

MJDemoCryostatAssemblyMessenger::~MJDemoCryostatAssemblyMessenger()
{
  delete fCalAssemblyMessenger;
  delete fCalAssemblyPosCmd;
  delete fCryoAssemblyDir;
}

//---------------------------------------------------------------------------//

void MJDemoCryostatAssemblyMessenger::SetNewValue(G4UIcommand* cmd,
						     G4String nv)
{
  if(cmd == fCalAssemblyPosCmd)
    fCryoAssembly->SetCalAssemblyPos(fCalAssemblyPosCmd->GetNew3VectorValue(nv));
    fCalAssemblyPosCmd->SetDefaultValue(fCryoAssembly->GetCalAssemblyPos());
}

//---------------------------------------------------------------------------//

void MJDemoCryostatAssemblyMessenger::SetValues(MJDemoCryostatAssembly* cryoAssembly)
{
  fCalAssemblyMessenger->SetValues(cryoAssembly->GetCalibration());
  MJVDemoAssembly* tmp = new MJVDemoAssembly();
  *tmp = *(MJVDemoAssembly*) cryoAssembly;
  cryoAssembly->SetCalAssemblyPos(fCryoAssembly->GetCalAssemblyPos());
  *(MJVDemoAssembly*) cryoAssembly = *tmp;
  delete tmp;
  delete fCryoAssembly;
  fCryoAssembly = cryoAssembly;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
