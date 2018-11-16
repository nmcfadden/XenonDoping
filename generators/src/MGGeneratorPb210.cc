//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id:
//      
// CLASS IMPLEMENTATION:  MGGeneratorPb210.cc
//
//---------------------------------------------------------------------------//
/**
* SPECIAL NOTES:
*/
// 
//---------------------------------------------------------------------------//
/**
* AUTHOR: A. Stevens
* CONTACT: austinst@physics.unc.edu
* FIRST SUBMISSION: 
* 
* REVISION:
*
*
*/
//---------------------------------------------------------------------------//

#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "TMath.h"

#include "generators/MGGeneratorPb210Messenger.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorPb210.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorPb210::MGGeneratorPb210()
{
	fGeneratorName = "Pb210";
	fG4Messenger = new MGGeneratorPb210Messenger(this);
	fParticleGun = new G4ParticleGun(1);


	fXRay = false;	//temporary
	fFirstOrder = true; //temporary
}

//---------------------------------------------------------------------------//

MGGeneratorPb210::MGGeneratorPb210(const MGGeneratorPb210 & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorPb210::~MGGeneratorPb210()
{
	delete fG4Messenger;
	delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorPb210::BeginOfRunAction(const G4Run*)
{
	G4double energyMin = 40*keV;
	G4double energyMax = 1161*keV;

	hFirstOrderEnergy = new TF1("hFirstOrderEnergy", this, 
	&MGGeneratorPb210::FirstOrderEnergyFormula,
	energyMin/keV, energyMax/keV, 0,
	"MGGeneratorPb210",
	"FirstOrderEnergyFormula");

	hSecondOrderEnergy = new TF1("hSecondOrderEnergy", this,
	&MGGeneratorPb210::SecondOrderEnergyFormula,
	energyMin/keV, energyMax/keV, 0,
	"MGGeneratorPb210",
	"SecondOrderEnergyFormula");

	//Note that the Angular Distribution function depends on the current
	//energy, so hAngularDistribution must be re-created at each event, not
	//each run.
}	

//---------------------------------------------------------------------------//

void MGGeneratorPb210::EndOfRunAction(const G4Run*)
{
	delete hFirstOrderEnergy;
	delete hSecondOrderEnergy;
}

//---------------------------------------------------------------------------//

G4double MGGeneratorPb210::FirstOrderEnergyFormula(G4double *pEnergy, G4double*)
{	//Units are keV for energy and energyMax
	G4double energy = pEnergy[0];
	G4double temp = 0;
	G4double energyMax = 1161;
	G4double eps = (energy/energyMax); //eps is epsilon

	if (energy < 40)
	temp = 0;

	else if (energy >= 40 && energy < 78.5)
	temp =	2.757 - 180.1*eps + 4107*eps*eps - 27044*eps*eps*eps;

	else if (energy >= 78.5 && energy < 100)
	temp = 7.763 - 143.9*eps + 656.0*eps*eps;

	else if (energy >= 100 && energy <= 1161)
	temp = 1705*eps*eps*eps
		* TMath::Exp(-18.79*eps + 11.82*eps*eps - 11.07*eps*eps*eps);

	else if (energy > 1161)
	temp = 0;


	return temp;
} 

//---------------------------------------------------------------------------//

G4double MGGeneratorPb210::SecondOrderEnergyFormula(G4double *pEnergy, G4double*)
{	//Units are keV for energy and energyMax
	G4double energy = pEnergy[0];
	G4double temp = 0;
	G4double energyMax = 1161;
	G4double eps = (energy/energyMax);	//eps is epsilon

	if (energy < 40)
	temp = 0;

	else if (energy >= 40 && energy < 100)
	temp =	0.8835 * (1 + 10.91*eps - 205.5*eps*eps);

	else if (energy >= 100 && energy <= 1161)
	temp =	3.012*TMath::Exp(-eps/0.07812);

	else if (energy > 1161)
	temp = 0;


	return temp;
} 

//---------------------------------------------------------------------------//

G4double MGGeneratorPb210::AngularDistributionFormula(G4double *pAngle, 
	G4double *par)
{
	G4double angle = pAngle[0];
	G4double energy = par[0] / keV;
	G4double qValue = par[1];
	G4double normalization;
	G4double temp;


	normalization = TMath::Sin(TMath::ATan( 1/ TMath::Sqrt(qValue)))
	* TMath::Power( TMath::Cos( TMath::ATan(1/ TMath::Sqrt(qValue))), qValue);


	if (energy >= 40 && energy <= 1161)
	temp = (1 / normalization)
	*	TMath::Power(TMath::Cos(angle), qValue)	* TMath::Sin(angle); 
	else
	temp = 0;


	return temp;
} 

//---------------------------------------------------------------------------//

G4double MGGeneratorPb210::FOAngularDistQValue()
{	//energy and energyMax are in keV
	G4double energy = fCurrentEnergy / keV;
	G4double qValue = 0;
	G4double energyMax = 1161;
	G4double eps = (energy/energyMax); //eps is epsilon

	if (fXRay == true)
	qValue = 1.191;

	else
	{	
	if (energy < 40)
	qValue = 0;

	else if (energy >= 40 && energy < 100)
	qValue =	1.045;

	else if (energy >= 100 && energy <= 1161)
	qValue =	1.016 + 0.1057 * TMath::Exp( -( eps - 0.06634)/0.1570);

	else if (energy > 1161)
	{
	qValue = 0;
	MGLog(error) << "FOAngularDistQValue: Energy not within 40-1161 keV"
	<< " range, qValue set to 0." << endlog;
	}
	}	

	return qValue;
}

//---------------------------------------------------------------------------//

G4double MGGeneratorPb210::SOAngularDistQValue()
{	//Units for energy and energyMax are keV
	G4double energy = fCurrentEnergy/keV;
	G4double qValue = 0;	
	G4double energyMax = 1161;
	G4double eps = (energy/energyMax); //eps is epsilon

	if (fXRay == true)
	qValue = 0.4416;

	else
	{
	if (energy < 40)
	qValue = 0;

	else if (energy >= 40 && energy < 203)
	qValue =	0.9133 - 1.757 * eps;

	else if (energy >= 203 && energy <= 1161)
	qValue =	0.6063;

	else if (energy > 1161)
	{
	qValue = 0;
	MGLog(error) << "SOAngularDistQValue: Energy not within 40-1161 keV"
			<< " range, qValue set to 0." << endlog;
	}
	}

	return qValue;
}

//---------------------------------------------------------------------------//

void MGGeneratorPb210::SampleFirstOrderPhotonEnergy()
{	
	fCurrentEnergy = (hFirstOrderEnergy->GetRandom() * keV);
} 

//---------------------------------------------------------------------------//

void MGGeneratorPb210::SampleSecondOrderPhotonEnergy()
{	
	fCurrentEnergy = (hSecondOrderEnergy->GetRandom() * keV);
} 

//---------------------------------------------------------------------------//

void MGGeneratorPb210::SampleAngularDistribution()
{	
	fZenithAngle = (hAngularDistribution->GetRandom());
} 

//---------------------------------------------------------------------------//

void MGGeneratorPb210::DirectionDecider()
{
	if(fLogicalPb210Surface == NULL)
	MGLog(error) << "fLogicalPb210Surface not defined for generator; zenithAxis"
	<< "cannot be found.  Use /MG/generator/Pb210/shieldSurface "
	<< "and enter the name of the physical volume sampled by GSS."
	<< endlog;	

	G4double rotationAngle = G4UniformRand() * 2 * TMath::Pi();
	G4ThreeVector zenithAxis;


	zenithAxis = fLogicalPb210Surface->GetSolid()->SurfaceNormal(fCurrentPosition);


	//ensures photons are generated going inward
	zenithAxis = zenithAxis * -1;

	/*
	//NOTE: the above line (zenith = zenith*-1) works fine as long as you are 
	//sampling the entire volume that is inside of the lead shield.  If you are
	//sampling a thin dummy volume that just covers the surface of the shield, you
	//may want to use the following line instead:
	if ((fCurrentPosition+zenithAxis).mag() > (fCurrentPosition-zenithAxis).mag())
	zenithAxis = zenithAxis * -1;
	//This should work for simpler geometries (box, box with coldfinger hole, etc)
	//but more complicated dummy surfaces may require this to be rethought.
	*/


	fDirection = zenithAxis;
	fDirection.rotate(fZenithAngle, zenithAxis.orthogonal());
	fDirection.rotate(rotationAngle, zenithAxis);
}

//---------------------------------------------------------------------------//

void MGGeneratorPb210::GeneratePrimaryVertex(G4Event *event)
{
	//By the time this is called, MGGeneratorPrimary has already set
	//fCurrentPosition to a random point on the lead shield surface
	//using GSS, as long as a GSS file was set in the macro.  See
	//SampleGenerators.mac.
	if(fCurrentPosition == G4ThreeVector(0,0,0))
	MGLog(error) << "fCurrentPosition is zero.	Check GSS file." << endlog;

	//setting particle definition to a gamma
	fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());	

	//determine particle energy
	if (fFirstOrder == true) SampleFirstOrderPhotonEnergy();
	else SampleSecondOrderPhotonEnergy();

	//determine particle zenith angle
	G4double angleMin = 0;
	G4double angleMax = TMath::Pi()/2;	
	hAngularDistribution = new TF1("hAngularDistribution", this,
	&MGGeneratorPb210::AngularDistributionFormula,
	angleMin, angleMax, 2,
	"MGGeneratorPb210",
	"AngularDistributionFormula");
	G4double qValue;	
	if (fFirstOrder == true) qValue = FOAngularDistQValue();
	else qValue = SOAngularDistQValue();	
	hAngularDistribution->SetParameters(fCurrentEnergy, qValue);
	hAngularDistribution->SetParNames("EnergyOfPhoton","qValue");

	SampleAngularDistribution();
	delete hAngularDistribution;

	//determine particle momentum direction
	DirectionDecider();

	//particle direction, position, and energy sent to ParticleGun
	fParticleGun->SetParticlePosition(fCurrentPosition);
	fParticleGun->SetParticleMomentumDirection(fDirection);
	fParticleGun->SetParticleEnergy(fCurrentEnergy);

	//vertex generated by ParticleGun
	fParticleGun->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//

void MGGeneratorPb210::SetShieldSurfaceVolume(G4String volumeName)
{
	G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
	bool found = false;
	for(size_t n = 0; n < volStore->size(); n++)
	{
	if((*volStore)[n]->GetName() == volumeName)
	{
	fLogicalPb210Surface = ((*volStore)[n])->GetLogicalVolume();
	found = true;
	}
	}
	if(!found)
	MGLog(error) << "Volume: " << volumeName << " not found in "
	<< "G4PhysicalVolumeStore.  Input the physical volume "
	<< "corresponding to what fLogicalPb210Surface should be."
	<< endlog;
}

