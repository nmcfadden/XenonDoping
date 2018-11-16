/*
 * G4DemoEventGeneratorAction.cc
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#include "../include/G4DemoEventGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"


G4DemoEventGeneratorAction::G4DemoEventGeneratorAction() {

	G4int n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	//particleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "e-"));
	particleGun->SetParticleDefinition(particleTable->FindParticle(particleName = "e-"));
	particleGun->SetParticleEnergy(0.5 * MeV);
	particleGun->SetParticlePosition(G4ThreeVector(-0.1 * m, 0.0, 0.0));

}

G4DemoEventGeneratorAction::~G4DemoEventGeneratorAction() {
	delete particleGun;
}

void G4DemoEventGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4int i = anEvent->GetEventID() % 3;
	G4ThreeVector v(1.0,0.0,0.0);
	switch(i)
	{
		case 0:
		break;
		case 1:
		v.setY(0.01);
		break;
		case 2:
		v.setZ(0.01);
		break;
	}
	particleGun->SetParticleMomentumDirection(v);
	particleGun->GeneratePrimaryVertex(anEvent);
}
