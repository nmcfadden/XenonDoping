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
// $Id: MGGeneratorLGND200Calibration.cc,v 1.4 2014-07-14 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorLGND200Calibration.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * June-2018, Created, Ralph
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

#include "generators/MGGeneratorLGND200Calibration.hh"
#include "generators/MGGeneratorLGND200CalibrationMessenger.hh"
#include "io/MGLogger.hh"

#include "G4PhysicalVolumeStore.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorLGND200Calibration.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorLGND200Calibration::MGGeneratorLGND200Calibration()
{
	fA = 228;
	fZ = 90;
  fGeneratorName = "LGND200Calibration";
  fG4Messenger = new MGGeneratorLGND200CalibrationMessenger(this);
  fParticleGun = new G4ParticleGun(1);


	//defaults
	fSourcePosition = new G4ThreeVector(0,0,0);

	//get position of the inner gold volume
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  G4int nVolumes = (G4int) volumeStore->size();
  G4String candidateList;
  for(G4int i=0;i<nVolumes;i++) {
    candidateList = (*volumeStore)[i]->GetName();
		if(candidateList.contains("LGND_200_CalibrationSourceInner")){
			fSourcePosition->setX((*volumeStore)[i]->GetTranslation().getX());
			fSourcePosition->setY((*volumeStore)[i]->GetTranslation().getY());
			fSourcePosition->setZ((*volumeStore)[i]->GetTranslation().getZ());
			break;				
		}
  }

}

//---------------------------------------------------------------------------//

MGGeneratorLGND200Calibration::MGGeneratorLGND200Calibration(const MGGeneratorLGND200Calibration & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorLGND200Calibration::~MGGeneratorLGND200Calibration()
{
  delete fG4Messenger;
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorLGND200Calibration::BeginOfRunAction(G4Run const*)
{
  Dump();
}  

//---------------------------------------------------------------------------//

void MGGeneratorLGND200Calibration::Dump()
{
  MGLog(routine) << "-------------------------------------------" << endlog;
  MGLog(routine) << "  LGND200 Calibration Source Parameters:" << endlog;
  MGLog(routine) << "  Ion Z: " << fZ << endlog;
  MGLog(routine) << "  Ion A: " << fA << endlog;
	MGLog(routine) << "  Source X " << fSourcePosition->x()/CLHEP::mm << " mm " << endlog;
	MGLog(routine) << "  Source Y " << fSourcePosition->y()/CLHEP::mm << " mm " << endlog;
	MGLog(routine) << "  Source Z " << fSourcePosition->z()/CLHEP::mm << " mm " << endlog;
  MGLog(routine) << "-------------------------------------------" << endlog;
}

//---------------------------------------------------------------------------//

void MGGeneratorLGND200Calibration::EndOfRunAction(G4Run const*)
{;}

//---------------------------------------------------------------------------//

void MGGeneratorLGND200Calibration::GeneratePrimaryVertex(G4Event *event)
{
	G4ThreeVector fStartPosition;

	//values of inner gold volume
  G4double radius = 2*mm;
  G4double height = 4*mm;
  G4double xvalue = radius*(2*G4UniformRand()-1);
  G4double yvalue = radius*(2*G4UniformRand()-1);
	while(xvalue*xvalue+yvalue*yvalue > radius*radius){
      xvalue = radius*(2*G4UniformRand()-1);
      yvalue = radius*(2*G4UniformRand()-1);
  }
	G4double zvalue = height*0.5*(2*G4UniformRand()-1);

	fStartPosition.setX(fSourcePosition->x() + xvalue);
	fStartPosition.setY(fSourcePosition->y() + yvalue);		
	fStartPosition.setZ(fSourcePosition->z() + zvalue);

  G4IonTable *theIonTable =
    (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition *aIon = theIonTable->GetIon(fZ, fA);
  fParticleGun->SetParticleDefinition(aIon);
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->SetParticleEnergy(0.0);
  fParticleGun->SetParticlePosition(fStartPosition);
  fParticleGun->GeneratePrimaryVertex(event);
}
  
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
