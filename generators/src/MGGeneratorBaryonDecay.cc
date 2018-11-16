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
// $Id: MGGeneratorBaryonDecay.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorBaryonDecay.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * This generator should help us to understand several decay modes that were 
 * investigated for the baryon decay search
 * deltaB = 3
 * case 1 3n->73Ge pi0 neutrino
 * Double_t particle_mass[3] = {72.9235*amu, 0.135, 0.000000000120};
 * case 2 2np->73Ga pi0 e+ 
 * Double_t particle_mass[3] = {72.9251*amu, 0.135, 0.000000511};
 * case 3 2pn->73Zn pi+ e+ 
 * Double_t particle_mass[3] = {72.9298*amu, 0.1395, 0.000000511};
 * case 4 3p->73Cu pi+ pi+ e+ 
 * Double_t particle_mass[4] = {72.9367*amu, 0.1395, 0.1395, 0.000000511};
 *
 * deltaB = 2
 * case 5 nn->74Ge pi0 pi0
 * Double_t particle_mass[3] = {73.9211*amu, 0.135, 0.135};
 * case 6 nn->74Ge pi+ pi-
 * Double_t particle_mass[3] = {73.9211*amu, 0.1395, 0.1395};
 * case 7 np->74Ga pi0 pi+
 * Double_t particle_mass[3] = {73.9269*amu, 0.135, 0.1395};
 * case 8 pp->74Zn pi+ pi+
 * Double_t particle_mass[3] = {73.9294*amu, 0.1395, 0.1395};
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2018
 *
 * REVISION:
 *
 * 02-15-2018, Created, R. Massarczyk
 *
 */
//---------------------------------------------------------------------------//
//

//#include <CLHEP/Random/RandGauss.h>
#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4PionZero.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Geantino.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"

#include "generators/MGGeneratorBaryonDecayMessenger.hh"
#include "io/MGLogger.hh"
#include <iostream>
#include <sstream>
#include <string>

#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "TMath.h"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorBaryonDecay.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorBaryonDecay::MGGeneratorBaryonDecay()
{
  DecayMode = 1;
  
  fGeneratorName = "BaryonDecay";
  fG4Messenger = new MGGeneratorBaryonDecayMessenger(this);
  fPosition = G4ThreeVector(0.0, 0.0, 0.0);

	Target.SetPx(0.);
	Target.SetPy(0.);
	Target.SetPz(0.);
	Target.SetE(75.9214*0.931494); //amu in GeV
	LoadMasses(DecayMode);

  fParticleGun = new G4ParticleGun(1);

}

//---------------------------------------------------------------------------//

MGGeneratorBaryonDecay::MGGeneratorBaryonDecay(const MGGeneratorBaryonDecay & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorBaryonDecay::~MGGeneratorBaryonDecay()
{
  delete fG4Messenger;
  delete fParticleGun;
}


//---------------------------------------------------------------------------//

void MGGeneratorBaryonDecay::GeneratePrimaryVertex(G4Event *event)
{
	Weight = PhaseSpaceEvent.Generate();


	for (unsigned int i = 0;i<particle_definition.size();i++){
		G4PrimaryVertex* vertex = new G4PrimaryVertex(fPosition,0.0*s);
		particle_lorentz[i] = PhaseSpaceEvent.GetDecay(i);
		G4PrimaryParticle* thePrimaryParticle = new G4PrimaryParticle(particle_definition[i], particle_lorentz[i]->Px()*GeV,particle_lorentz[i]->Py()*GeV,particle_lorentz[i]->Pz()*GeV);
		vertex->SetPrimary(thePrimaryParticle);
		vertex->SetWeight(Weight);
    event->AddPrimaryVertex(vertex);
		//MGLog(routine) << i << " " <<  particle_definition[i]->GetParticleName() << " " <<particle_lorentz[i]->E() << endlog;
	}




}

//---------------------------------------------------------------------------//

void MGGeneratorBaryonDecay::LoadMasses(G4int n){
	particle_mass.clear();
	particle_lorentz.clear();
	particle_definition.clear();
	
	G4IonTable *theIonTable = (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());

	switch(n){
		//nnn->73Ge pi0 neutrino
		case 1:
			particle_definition.push_back(theIonTable->GetIon(32,73,0));		
			particle_definition.push_back(G4PionZero::PionZeroDefinition());
			particle_definition.push_back(G4AntiNeutrinoE::AntiNeutrinoEDefinition());
			break;
 		//nnp->73Ga pi0 e+ 
		case 2:
			particle_definition.push_back(theIonTable->GetIon(31,73,0));		
			particle_definition.push_back(G4PionZero::PionZeroDefinition());
			particle_definition.push_back(G4Positron::PositronDefinition());
			break; 		
		//npp->73Zn pi+ e+ 
		case 3:
			particle_definition.push_back(theIonTable->GetIon(30,73,0));		
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4Positron::PositronDefinition());
			break;
		//ppp->73Cu pi+ pi+ e+ 
		case 4:
			particle_definition.push_back(theIonTable->GetIon(29,73,0));		
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4Positron::PositronDefinition());
			break;
		//nn->74Ge pi+ pi- 
		case 5:
			particle_definition.push_back(theIonTable->GetIon(32,74,0));		
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4PionMinus::PionMinusDefinition());
			break;
 		//nn->74Ge pi0 pi0 
		case 6:
			particle_definition.push_back(theIonTable->GetIon(32,74,0));		
			particle_definition.push_back(G4PionZero::PionZeroDefinition());
			particle_definition.push_back(G4PionZero::PionZeroDefinition());
			break; 		
		//nnp->74Ga pi+ pi0 
		case 7:
			particle_definition.push_back(theIonTable->GetIon(31,74,0));		
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4PionZero::PionZeroDefinition());
			break;
		//pp->74Zn pi+ pi+
		case 8:
			particle_definition.push_back(theIonTable->GetIon(30,74,0));		
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			particle_definition.push_back(G4PionPlus::PionPlusDefinition());
			break;

	
	}

	for (unsigned int i = 0;i<particle_definition.size();i++) particle_mass.push_back(particle_definition[i]->GetPDGMass()/(1.*GeV));
	particle_lorentz.reserve(particle_definition.size());
	PhaseSpaceEvent.SetDecay(Target, particle_definition.size(),particle_mass.data());


	MGLog(routine) << "=====================" <<endlog;
	MGLog(routine) << "76Ge BaryonDecay selected " <<endlog;
	MGLog(routine) << "Mode                : " << n << endlog;
	MGLog(routine) << "Number of particles : " << particle_definition.size() << endlog;
	for (unsigned int i = 0;i<particle_definition.size();i++) MGLog(routine) << "-- : " << particle_definition[i]->GetParticleName() << endlog;
	MGLog(routine) << "=====================" <<endlog;

return;
}

