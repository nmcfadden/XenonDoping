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
// $Id: MGGeneratorCable.cc,v 1.4 2014-07-14 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorCable.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: B. Zhu
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 09-2016, Created, B. Zhu
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

#include "G4AffineTransform.hh"
#include "G4Geantino.hh"
#include "generators/MGGeneratorCable.hh"
#include "generators/MGGeneratorCableMessenger.hh"
// #include <CLHEP/Random/RandFlat.h>
#include "io/MGLogger.hh"

#include <math.h>
#define pi 3.14159265358979323846

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorCable.hh" 

//---------------------------------------------------------------------------//


using namespace CLHEP;

MGGeneratorCable::MGGeneratorCable()
{
  fGeneratorName = "Cable";
  fG4Messenger = new MGGeneratorCableMessenger(this);
  fParticleGun = new G4ParticleGun(1);

  fCableRadius = 0.02*mm;
  fColdPlateRadius = 317.5/2*mm;
  fColdPlateZ = 5.*mm;
  fCrossArmLength = 27.13*25.4/2*mm;
  fCrossArmWidth = 5./2*mm;
  fCrossArmIRadius = 35.5/2*mm;
  fCrossArmT = 25.4*0.25*mm;

  // Units were originally in inches and then converted to cm
  // The drawing and simulation geometries aren't one-to-one so I made some slight adjustments
  // to make sure all the cables are inside the holes of the cable guides
  fCableOffset[0] = G4ThreeVector(-2.54*1.839*cm, 2.54*0.560*cm, 0.); // P1
  fHVOffset[0] = G4ThreeVector(-2.54*1.839*cm, -2.54*0.560*cm, 0.);  
  fCableOffset[1] = G4ThreeVector(2.54*3.425*cm, 2.54*3.75*cm, 0.); // P2
  fHVOffset[1] = G4ThreeVector(2.54*2.350*cm, 2.54*3.6*cm, 0.);
  fCableOffset[2] = G4ThreeVector(-2.54*0.090*cm, 2.54*5.59*cm, 0.); // P3
  fHVOffset[2] = G4ThreeVector(-2.54*1.15*cm, 2.54*5.25*cm, 0.);
  fCableOffset[3] = G4ThreeVector(-2.54*3.356*cm, 2.54*3.81*cm, 0.); // P4
  fHVOffset[3] = G4ThreeVector(-2.54*4.387*cm, 2.54*3.408*cm, 0.);
  fCableOffset[4] = G4ThreeVector(-2.54*4.387*cm, -2.54*3.408*cm, 0.); // P5
  fHVOffset[4] = G4ThreeVector(-2.54*3.356*cm, -2.54*3.81*cm, 0.);
  fCableOffset[5] = G4ThreeVector(-2.54*1.15 *cm, -2.54*5.25*cm, 0.); // P6
  fHVOffset[5] = G4ThreeVector(-2.54*0.090*cm, -2.54*5.59*cm, 0.);
  fCableOffset[6] = G4ThreeVector(2.54*2.350*cm, -2.54*3.6*cm, 0.); // P7
  fHVOffset[6] = G4ThreeVector(2.54*3.425*cm, -2.54*3.75*cm, 0.);

  fCableLength[0] = 28.0/2*cm;
  fCableLength[1] = 20.8/2*cm;
  fCableLength[2] = 14.1/2*cm;
  fCableLength[3] = 10.6/2*cm;

  fCableCenter[0] = -12.6*cm;
  fCableCenter[1] = -8.9*cm;
  fCableCenter[2] = -5.6*cm;
  fCableCenter[3] = -3.8*cm;

  fHVLength[0] = 23.0/2*cm;
  fHVLength[1] = 16.5/2*cm;
  fHVLength[2] = 10.5/2*cm;
  fHVLength[3] = 3.3/2*cm;

  fHVCenter[0] = -10.0*cm;
  fHVCenter[1] = -6.75*cm;
  fHVCenter[2] = -3.75*cm;
  fHVCenter[3] = -0.15*cm;
}

//---------------------------------------------------------------------------//

MGGeneratorCable::MGGeneratorCable(const MGGeneratorCable & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorCable::~MGGeneratorCable()
{
  delete fG4Messenger;
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorCable::BeginOfRunAction(G4Run const*)
{;}  

//---------------------------------------------------------------------------//

void MGGeneratorCable::SetSourcePos(std::string sourcePos)
{
  fSourcePos = sourcePos;
  G4UIcommandTree* cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/demonstrator/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++)
  {
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "cryo1Pos" || param == "cryo2PosCmd")
    {
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      if(param == "cryo1Pos" && fSourcePos == "W")
  		fColdPlateOffset = G4ThreeVector(x, y, z);
      else if(param == "cryo2PosCmd" && fSourcePos == "E")
  		fColdPlateOffset = G4ThreeVector(x, y, z);
    }
    else if(param == "cryo1Rot" && fSourcePos == "W")
    {
      std::string val =
  		cmdTree->GetCommand(i+1)->GetParameter(0)->GetDefaultValue();
      std::stringstream(val) >> fZrotation;
    }
    else if(param == "cryo2Rot" && fSourcePos == "E")
    {
      std::string val =
      	cmdTree->GetCommand(i+1)->GetParameter(0)->GetDefaultValue();
      std::stringstream(val) >> fZrotation;
    }
  }
  
  cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/mjdemocryoassembly"+fSourcePos+"/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++)
  {
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "calAssemblyPos"){
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      fColdPlateOffset += G4ThreeVector(x, y, z/3); // rough estimate for Z positioning
    }
  }
  cmdTree = G4UImanager::GetUIpointer()->GetTree()->GetTree("/MG/");
  cmdTree = cmdTree->GetTree(G4String("/MG/mjdemocalassembly"+fSourcePos+"/"));
  for(int i=0; i<cmdTree->GetCommandEntry(); i++)
  {
    std::string param = cmdTree->GetCommand(i+1)->GetCommandName();
    if(param == "sourceOffset")
    {
      G4UIcommand* cmd = cmdTree->GetCommand(i+1);
      G4double x, y, z;
      std::stringstream(cmd->GetParameter(0)->GetDefaultValue()) >> x;
      std::stringstream(cmd->GetParameter(1)->GetDefaultValue()) >> y;
      std::stringstream(cmd->GetParameter(2)->GetDefaultValue()) >> z;
      fColdPlateOffset += G4ThreeVector(x, y, z);
    }
  }

  for(int i = 0; i < 7; i++) 
  {
	  fCableOffset[i].rotateZ(-fZrotation);
  	fHVOffset[i].rotateZ(-fZrotation);
  }
}

//---------------------------------------------------------------------------//

void MGGeneratorCable::EndOfRunAction(G4Run const*)
{;}

//---------------------------------------------------------------------------//

void MGGeneratorCable::GeneratePrimaryVertex(G4Event *event)
{
  // Generate random variables
  fRandAngle = 2*pi*G4UniformRand();
  if(fSourceType == "S" || fSourceType == "H")
  {
  	fRandString = G4RandFlat::shootInt(7); // String position
  	fRandPos = G4RandFlat::shootInt(4); // Detector position
  	fRandRadiusSq = fCableRadius*fCableRadius*G4UniformRand();
  	fPositionX = sqrt( fRandRadiusSq ) * cos( fRandAngle );
  	fPositionY = sqrt( fRandRadiusSq ) * sin( fRandAngle );
  	if(fSourceType == "S") // Signal
  	{
  		fPositionZ = (1. - 2.*G4UniformRand())*fCableLength[fRandPos];
  		fPosition = fColdPlateOffset + fCableOffset[fRandString] + G4ThreeVector(fPositionX, fPositionY, fPositionZ + fCableCenter[fRandPos]);
  	}
  	else if(fSourceType == "H") // HV
  	{
    	fPositionZ = (1. - 2.*G4UniformRand())*fHVLength[fRandPos];
  		fPosition = fColdPlateOffset + fHVOffset[fRandString] + G4ThreeVector(fPositionX, fPositionY, fPositionZ + fHVCenter[fRandPos]);
  	}
  }
  else if(fSourceType == "P") // Cold plate
  {
	fRandRadiusSq = fColdPlateRadius*fColdPlateRadius*G4UniformRand();
  	fPositionX = sqrt( fRandRadiusSq ) * cos( fRandAngle );
  	fPositionY = sqrt( fRandRadiusSq ) * sin( fRandAngle );
	fPositionZ = (1. - 2.*G4UniformRand())*fColdPlateZ;
  	fPosition = fColdPlateOffset + G4ThreeVector(fPositionX, fPositionY, fPositionZ + 20.0*mm);
  }
  else if(fSourceType == "C" && fSourcePos == "W") // Cross arm
  { 
    fRandRadiusSq = fCrossArmIRadius*fCrossArmIRadius + (fCrossArmT*fCrossArmT)*G4UniformRand();
    fPositionY = (1. - 2.*G4UniformRand())*fCrossArmLength;
    fPositionX = sqrt( fRandRadiusSq ) * cos( fRandAngle );
    fPositionZ = sqrt( fRandRadiusSq ) * sin( fRandAngle );
    fPosition = fColdPlateOffset + G4ThreeVector(fPositionX, fPositionY - fColdPlateRadius - fCrossArmLength, fPositionZ + 36.75*mm );
  }
  else if(fSourceType == "C" && fSourcePos == "E")
  {
    fRandRadiusSq = fCrossArmIRadius*fCrossArmIRadius + (fCrossArmT*fCrossArmT)*G4UniformRand();
    fPositionX = (1. - 2.*G4UniformRand())*fCrossArmLength;
    fPositionY = sqrt( fRandRadiusSq ) * cos( fRandAngle );
    fPositionZ = sqrt( fRandRadiusSq ) * sin( fRandAngle );
    fPosition = fColdPlateOffset + G4ThreeVector(fPositionX + fColdPlateRadius + fCrossArmLength, fPositionY, fPositionZ + 36.75*mm );
  }

  G4IonTable *theIonTable = (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition *aIon = theIonTable->GetIon(fZ, fA);

  fParticleGun->SetParticleDefinition(aIon);
  fParticleGun->SetParticleEnergy(0.0);
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->SetParticlePosition(fPosition);
  fParticleGun->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//

