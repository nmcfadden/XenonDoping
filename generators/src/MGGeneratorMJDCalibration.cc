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
// $Id: MGGeneratorMJDCalibration.cc,v 1.4 2014-07-14 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorMJDCalibration.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: K. Vorren
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-24-2016, Modified generator to read calibration source parameters
 *             from messenger commands, T. Caldwell 
 * 07-14-2014, Created, K. Vorren
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>
#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4RandomDirection.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIcommandTree.hh"

#include "generators/MGGeneratorMJDCalibration.hh"
#include "generators/MGGeneratorMJDCalibrationMessenger.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorMJDCalibration.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorMJDCalibration::MGGeneratorMJDCalibration()
{
  fGeneratorName = "MJDCalibration";
  fG4Messenger = new MGGeneratorMJDCalibrationMessenger(this);
  fParticleGun = new G4ParticleGun(1);
}

//---------------------------------------------------------------------------//

MGGeneratorMJDCalibration::MGGeneratorMJDCalibration(const MGGeneratorMJDCalibration & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorMJDCalibration::~MGGeneratorMJDCalibration()
{
  delete fG4Messenger;
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibration::BeginOfRunAction(G4Run const*)
{
  if(MGLogger::GetSeverity() <= MGLogger::routine)
    Dump();
}  

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibration::Dump()
{
  G4cout << "  MJD Calibration Source Parameters:" << G4endl;
  G4cout << "  -----------------------------------------" << G4endl;
  G4cout << "  Helix Angle (degrees): " << fHelixAngle/deg << G4endl;
  G4cout << "  Start Angle (degrees): " << fStartAngle/deg << G4endl;
  G4cout << "  Delta Angle (degrees: " << fTotalAngle/deg << G4endl;
  G4cout << "  Source Track Radius (cm): " << fHelixRadius/cm << G4endl;
  G4cout << "  Ion Z: " << fZ << G4endl;
  G4cout << "  Ion A: " << fA << G4endl;
  G4cout << "-------------------------------------------" << G4endl << G4endl;
}

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibration::SetSourcePos(std::string sourcePos)
{
  G4UIcommandTree* cmdTree =
    G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/mjdemocalsource"+sourcePos+"/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++){
    if(cmdTree->GetCommand(i+1)->GetParameterEntries() == 0)
      continue;
    std::string val =
      cmdTree->GetCommand(i+1)->GetParameter(0)->GetDefaultValue();
    std::string param =
      cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "helixRadius")
      std::stringstream(val) >> fHelixRadius;
    else if(param == "startAngle")
      std::stringstream(val) >> fStartAngle;
    else if(param == "totalAngle")
      std::stringstream(val) >> fTotalAngle;
    else if(param == "helixAngle")
      std::stringstream(val) >> fHelixAngle;
  }
  cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/demonstrator/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++){
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "cryo1Pos" || param == "cryo2Pos"){
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      if(param == "cryo1Pos" && sourcePos == "W")
	fOrigin = G4ThreeVector(x, y, z);
      else if(param == "cryo2Pos" && sourcePos == "E")
	fOrigin = G4ThreeVector(x, y, z);
    }
    else if(param == "cryo1Rot" && sourcePos == "W"){
      std::string val =
	cmdTree->GetCommand(i+1)->GetParameter(0)->GetDefaultValue();
      std::stringstream(val) >> fZrotation;
    }
    else if(param == "cryo2Rot" && sourcePos == "E"){
      std::string val =
	cmdTree->GetCommand(i+1)->GetParameter(0)->GetDefaultValue();
      std::stringstream(val) >> fZrotation;
    }
  }
  cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/mjdemocryoassembly"+sourcePos+"/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++){
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "calAssemblyPos"){
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      fOrigin += G4ThreeVector(x, y, z);
    }
  }
  cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/mjdemocalassembly"+sourcePos+"/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++){
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "sourceOffset"){
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      fOrigin += G4ThreeVector(x, y, z);
    }
  }
  fZrotation *= -1;
}

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibration::EndOfRunAction(G4Run const*)
{;}

//---------------------------------------------------------------------------//

void MGGeneratorMJDCalibration::GeneratePrimaryVertex(G4Event *event)
{
  G4IonTable *theIonTable =
    (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition *aIon = theIonTable->GetIon(fZ, fA);
  fParticleGun->SetParticleDefinition(aIon);
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->SetParticleEnergy(0.0);
  fCurrentAngle = G4RandFlat::shoot(fStartAngle, fStartAngle + fTotalAngle);
  G4double tempZ =
    fHelixRadius * tan(fHelixAngle) * (fCurrentAngle - fStartAngle);
  fCurrentAngle += fZrotation;
  fCurrentPosition.set(fHelixRadius * cos(fCurrentAngle),
		       fHelixRadius * sin(fCurrentAngle),
		       -tempZ);
  fCurrentPosition += fOrigin;
  fParticleGun->SetParticlePosition(fCurrentPosition);
  fParticleGun->GeneratePrimaryVertex(event);
}
  
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
