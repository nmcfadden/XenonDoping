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
// $Id: MGGeneratorPNNL.cc,v 1.6 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorPNNL.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: D. Jordan
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 18:12:47 PDT 2004
 * 
 * REVISION:
 *
 * 07-08-2004, Created and included D. Jordan's Code, R. Henning
 */
//---------------------------------------------------------------------------//
//


#include <math.h>

//#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "Randomize.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"

#include "generators/MGGeneratorPNNLMessenger.hh"
#include "generators/MGGeneratorPNNLRadioisotope.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementRunAction.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorPNNL.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorPNNL::MGGeneratorPNNL():
  fParticleGun(0)
{
  fGeneratorName = "PNNLiso";
  fG4Messenger = new MGGeneratorPNNLMessenger(this);
  fPosition = G4ThreeVector(0.0, 0.0, 0.0);
}

//---------------------------------------------------------------------------//

MGGeneratorPNNL::MGGeneratorPNNL(const MGGeneratorPNNL & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorPNNL::~MGGeneratorPNNL()
{
  delete fParticleGun;
  delete fG4Messenger;
}

//---------------------------------------------------------------------------//

void MGGeneratorPNNL::BeginOfRunAction(G4Run const*)
{
  for (G4int i=0; i<10; i++)
    hist_isotope[i] = 0;
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fFirstCall  = true;
  fUsePNNLGen = false;
  MGLog(trace) << " called." << endlog;
}

//---------------------------------------------------------------------------//

void MGGeneratorPNNL::EndOfRunAction(G4Run const*)
{
  MGLog(routine) << "Number of times each radioisotope was sampled:"<<endlog;
  for (G4int i=0; i<10; i++)
    G4cout << i << " " << hist_isotope[i] << G4endl;
  MGLog(routine) << endlog;
}

//---------------------------------------------------------------------------//

void MGGeneratorPNNL::GeneratePrimaryVertex(G4Event *event)
{
  if (fFirstCall) {
    MGLog(routine) << "Called for first time." << endlog;
    MGLog(routine) << "fPNNL_generate_beta = "
                   << fPNNL_generate_beta << endlog;
     MGLog(routine) << "fPNNL_source_age = " << fPNNL_source_age << endlog;
    MGLog(routine)<<"fPNNL_DecayChain_file = "<<fPNNL_DecayChain_file<<endlog;

    f_pPNNLDecayChain = new MGGeneratorPNNLDecayChain(fPNNL_DecayChain_file,
                                                      fPNNL_source_age);
    fUsePNNLGen = true;
    fFirstCall = false;
  }

  const G4double PI = (G4double) pi; // Hack. RH. Needs improvement.
  
  G4int num_events = event->GetEventID();
  if ((num_events % fReportingFrequency) == 0) {
    MGManager *manager = MGManager::GetMGManager();
	time_t endTime = time( NULL );
	G4double timeDifference = difftime( endTime,
	                                 manager->GetMGRunAction()->GetStartTime()
									 );
    MGLog(routine) << "Processing Event # " << num_events << " at "
	               << timeDifference << " seconds (real time )" << endlog;
  }

  G4int num_gammas = 0;
  G4int indx_isotope;
  while (fUsePNNLGen && num_gammas == 0) {
    MGLog(debugging) << "Before DoCascadeEvent." << endlog;
    MGGeneratorPNNLCascadeEvent CascadeEvent = 
          f_pPNNLDecayChain->DoCascadeEvent();
    MGLog(debugging) << "Before GetIndex..." << endlog;
    indx_isotope = f_pPNNLDecayChain->GetIndexOfLastIsotopeSampled();
    MGLog(debugging) << "hist_isotopes" << endlog;
    if ((indx_isotope >= 0) && (indx_isotope < 10))
      hist_isotope[indx_isotope]++;
    else {
      MGLog(error)<<"PrimaryGeneratorAction: Anomalous sampled isotope index:" 
           << indx_isotope << " Aborting..." << endlog;
      MGLog(fatal) << endlog;
    }      

    G4double E_charged_particle = CascadeEvent.GetChargedParticleE();
    MGLog(debugging)<<"Charged particle energy: "<<E_charged_particle<<endlog;

    G4double cos_theta = -1.0 + 2.0*G4UniformRand();
    G4double phi       = 2.0*PI*G4UniformRand();
    G4double theta     = acos(cos_theta);
    G4double sin_theta = sin(theta);
    G4double cos_x     = sin_theta*cos(phi);
    G4double cos_y     = sin_theta*sin(phi);
    G4double cos_z     = cos_theta;

    if(fPNNL_generate_beta) {
      fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
      MGLog(debugging) << "Momentum." << cos_x << cos_y << cos_z << endlog;
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos_x,cos_y,cos_z));
      fParticleGun->SetParticleEnergy(E_charged_particle);
      fParticleGun->SetParticlePosition(fPosition);
      fParticleGun->GeneratePrimaryVertex(event);
    }
 
    num_gammas = CascadeEvent.GetNumGammas();
    MGLog(debugging) << "Number of gammas: " << num_gammas << endlog;

    if (num_gammas > 0) {
      G4double* pGammaList = CascadeEvent.GetGammaList();

      for (G4int igamma=0; igamma<num_gammas; igamma++) {
        G4double Energy_gamma = pGammaList[igamma];
        //        G4cout << igamma << " " << Energy_gamma << G4endl;
        G4double cos_theta_gamma = -1.0 + 2.0*G4UniformRand();
        G4double phi_gamma       = 2.0*PI*G4UniformRand();
        G4double theta_gamma     = acos(cos_theta_gamma);
        G4double sin_theta_gamma = sin(theta_gamma);
        G4double cos_x_gamma     = sin_theta_gamma*cos(phi_gamma);
        G4double cos_y_gamma     = sin_theta_gamma*sin(phi_gamma);
        G4double cos_z_gamma     = cos_theta_gamma;

	fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());
	MGLog(debugging) << "Momentum." << cos_x_gamma << cos_y_gamma << cos_z_gamma << endlog;
	fParticleGun->SetParticleMomentumDirection(
                     G4ThreeVector(cos_x_gamma,cos_y_gamma,cos_z_gamma));
	MGLog(debugging) << "Energy." << Energy_gamma << endlog;
	fParticleGun->SetParticleEnergy(Energy_gamma);
	MGLog(debugging) << "Setting Particle Position." << endlog;
	fParticleGun->SetParticlePosition(fPosition);
	MGLog(debugging) << "Position : " << fPosition << endlog;
	fParticleGun->GeneratePrimaryVertex(event);
      } // for (G4int igamma=0; igamma<num_gammas; igamma++) 
    } // if(num_gammas > 0)
  } // while (fUsePNNLGen && num_gammas == 0) 
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
