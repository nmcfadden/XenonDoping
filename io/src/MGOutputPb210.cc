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
// CLASS IMPLEMENTATION:  MGOutputPb210.cc
//
/**
* AUTHOR: A. Stevens
* CONTACT: paddy@physics.unc.edu
* FIRST SUBMISSION:
* 
* REVISION:
*
* 
* 
*/
//----------------------------------------------------------------------------//
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"

#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"

#include "io/MGOutputPb210.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "generators/MGGeneratorPrimary.hh"

#include "TMath.h"

using namespace CLHEP;

//----------------------------------------------------------------------------//
	// initializing non-int static constants
	const Double_t MGOutputPb210::fE1(50);
	const Double_t MGOutputPb210::fE2(300);
	const Double_t MGOutputPb210::fEnergyLow(40);
	const Double_t MGOutputPb210::fEnergyHigh(1162);

//----------------------------------------------------------------------------//

MGOutputPb210::MGOutputPb210(G4bool isMother): MGOutputRoot(isMother) 
{
	//setting the fTheGenerator pointer to the current generator
	MGVGenerator* tempPointer;

	SetSchemaDefined(false);
	gNavigator = G4TransportationManager::GetTransportationManager() 
					-> GetNavigatorForTracking();
tempPointer = MGManager::GetMGManager()
					->GetMGGeneratorPrimary()
					->GetMGGenerator();

	fTheGenerator = dynamic_cast<MGGeneratorPb210*>(tempPointer);

	//setting other pointers to NULL
	fEnergyHisto = NULL;
	fEnergyAngleHisto = NULL;
	fAngleHistoAtE1 = NULL;
	fAngleHistoAtE2 = NULL;
}

//----------------------------------------------------------------------------//

MGOutputPb210::~MGOutputPb210()
{
	delete fEnergyAngleHisto;
	delete fEnergyHisto;
	//delete fAngleHistoAtE1;
	//delete fAngleHistoAtE2;
}

//----------------------------------------------------------------------------//

void MGOutputPb210::BeginOfRunAction()
{
	DefineSchema(); 

	if(IsMother())
	{ OpenFile(); }
}

//----------------------------------------------------------------------------//

void MGOutputPb210::CalculateEnergy(const G4Event* event)
{
	Double_t momentumMagnitude;

	G4PrimaryVertex *primaryVertex;
	primaryVertex = event->GetPrimaryVertex();
	G4PrimaryParticle *primaryParticle;
	primaryParticle = primaryVertex->GetPrimary();

	fMomentum = primaryParticle->GetMomentum();
	momentumMagnitude = G4ToRoot(fMomentum.mag());

	fEnergy = momentumMagnitude;	//in Geant4 momentum is stored as p*c
}	

//----------------------------------------------------------------------------//

void MGOutputPb210::CalculateZenithAngle(const G4Event* event)
{
	G4ThreeVector axis;

	//Finding momentum direction
	G4PrimaryVertex *primaryVertex;
	primaryVertex = event->GetPrimaryVertex();
	G4PrimaryParticle *primaryParticle;
	primaryParticle = primaryVertex->GetPrimary();

	fMomentum = primaryParticle->GetMomentum();

	fInitialPosition = primaryVertex->GetPosition();

	//Finding zenith axis
	axis = FindZenithAxis();

	//Finding zenith angle
	fZenithAngle = 
	TMath::ACos(fMomentum.dot(axis) / (axis.mag() * fMomentum.mag()) );
}

//----------------------------------------------------------------------------//

G4ThreeVector MGOutputPb210::FindZenithAxis()
{ 
	//Double checking fTheGenerator
	if(fTheGenerator == 0) // happens when output constructed before generator
	{
	MGVGenerator* tempPointer = MGManager::GetMGManager()
					->GetMGGeneratorPrimary()
					->GetMGGenerator();
	fTheGenerator = dynamic_cast<MGGeneratorPb210*>(tempPointer);
	}

	if(fTheGenerator == 0)
	MGLog(error) << "FindZenithAxis: fTheGenerator is zero." << endlog;

	G4ThreeVector axis = fTheGenerator->GetShieldSurfaceVolume()
					->GetSolid()->SurfaceNormal(fInitialPosition);

	if (axis == G4ThreeVector(0,0,0))
	MGLog(error) << "FindZenithAxis: Zenith axis vector is (0,0,0)." << endlog;

	G4double testAngle = 
	TMath::ACos(fMomentum.dot(axis) / (axis.mag() * fMomentum.mag()) );

	if(testAngle >= TMath::Pi()/2 && testAngle <= TMath::Pi())
	axis = axis*-1;

	return axis;
}

//----------------------------------------------------------------------------//

void MGOutputPb210::DefineSchema()
{
	if(!SchemaDefined())
	{
	//A tree is not necessary in this class.
	fEnergyHisto = new TH1D("fEnergyHisto", "Energy Histogram",
														1121, 40, 1161); //keV

	fEnergyAngleHisto = new TH2D("fEnergyAngleHisto", "Energy Angle Histogram",
	fEnergyBins, fEnergyLow, fEnergyHigh,
	fAngleBins, 0, TMath::Pi()/2);

	fEnergyAngleHisto->GetXaxis()->SetTitle("Energy(keV)");
	fEnergyAngleHisto->GetYaxis()->SetTitle("Angle (rad)");
	fEnergyHisto->GetXaxis()->SetTitle("Energy (keV)");

	SetSchemaDefined(true);
	}
}

//----------------------------------------------------------------------------//

void MGOutputPb210::BeginOfEventAction(const G4Event* event)
{
//G4cout << "Event # " << event -> GetEventID() << G4endl;

	CalculateEnergy(event);
	CalculateZenithAngle(event);

	//recording energies and zenith angles.
	fEnergyHisto->Fill(fEnergy/keV, 1);
	fEnergyAngleHisto->Fill(fEnergy/keV, fZenithAngle, 1);
}


//----------------------------------------------------------------------------//

void MGOutputPb210::EndOfRunAction()
{
	// creating fAngleHisto
	TakeAngleHistoSlice();

	//writing and closing file
	if(IsMother())
	{ CloseFile(); }
}

//----------------------------------------------------------------------------//

void MGOutputPb210::TakeAngleHistoSlice()
{
	Int_t binAtE1 = fEnergyHisto->FindBin(fE1);
	Int_t binAtE2 = fEnergyHisto->FindBin(fE2);
	Int_t range = fSliceWidth;

	fAngleHistoAtE1 = fEnergyAngleHisto->ProjectionY("fAngleHistoAtE1", 
	binAtE1 - range, binAtE1 + range);

	fAngleHistoAtE2 = fEnergyAngleHisto->ProjectionY("fAngleHistoAtE2", 
	binAtE2 - range, binAtE2 + range);

	fAngleHistoAtE1->SetTitle("Angle Histogram at E1");
	fAngleHistoAtE2->SetTitle("Angle Histogram at E2");
	fAngleHistoAtE1->GetXaxis()->SetTitle("Angle(rad)");
	fAngleHistoAtE2->GetXaxis()->SetTitle("Angle(rad)");
}

//----------------------------------------------------------------------------//

void MGOutputPb210::WriteFile()
{
	// writing histograms
	if(MGOutputRoot::GetRootFile() != NULL) {
	fEnergyAngleHisto->Write();
	fEnergyHisto->Write(); 
	fAngleHistoAtE1->Write();
	fAngleHistoAtE2->Write();
	}

	// calling parent function
	MGOutputRoot::WriteFile();
}
