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
// Generator for cosmic ray muons (with the
// correct spectrum) in underground laboratories. 
// 
// History:
// --------
// March  6, 2019 M. Gold
// cosmic ray muon's at earth's surface from ref https://arxiv.org/pdf/1606.06907.pdf 
// 
//
//MaGe includes
#include "generators/MGGeneratorCosmicSurfMuons.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementEventAction.hh"

#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include <math.h>
#include "G4TransportationManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4VPhysicalVolume.hh"

using namespace CLHEP;

MGGeneratorCosmicSurfMuons::MGGeneratorCosmicSurfMuons()
{
  NumberOfParticlesToBeGenerated = 1;
  particle_definition = G4MuonMinus::MuonMinusDefinition();
  G4ThreeVector zero(0., 0., 0.);
  particle_momentum_direction = G4ParticleMomentum(0., 0.,-1.);
  particle_energy = 500*GeV;
  particle_position = zero;
  particle_time = 0.0;
  particle_polarization = zero;
  particle_charge = particle_definition->GetPDGCharge();

  //geometry initialization
  E0 = 4.0; //GeV
  ECut = 0.05;// GeV
  // refer to cryostat in BACON_Baseline.cc
  halfz = 0.4*m;
  Radius = 0.5*m;

  theMessenger = new MGGeneratorCosmicSurfMuonsMessenger(this);
  gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

MGGeneratorCosmicSurfMuons::~MGGeneratorCosmicSurfMuons()
{
  delete theMessenger;
}

void MGGeneratorCosmicSurfMuons::SampleInitialPosition()
{
  //Positions are generated uniformly over circular area 
  G4double r     = Radius*sqrt(G4UniformRand());
  G4double phi   = twopi * G4UniformRand();
  particle_position.setX(r*cos(phi));
  particle_position.setY(r*sin(phi));
  particle_position.setZ(halfz);
}

void MGGeneratorCosmicSurfMuons::GenerateAngularSpectrum()
{
  G4double costheta = 1.0 - pow( G4UniformRand(), 1.0/3.0); //co-latidude
  G4double phi = G4UniformRand()*360*degree;
  G4double sintheta = sqrt(1.0-costheta*costheta);
  particle_momentum_direction.setX(cos(phi)*costheta);
  particle_momentum_direction.setY(sin(phi)*costheta);
  particle_momentum_direction.setZ(-1.0*sintheta);
}


void MGGeneratorCosmicSurfMuons::GetEnergy()
{
  particle_energy = 1000.0*( (E0+ECut)*pow(1.-G4UniformRand(),-0.5)-E0 ); // convert to MeV
}

void MGGeneratorCosmicSurfMuons::SetParticleDefinition
  (G4ParticleDefinition* aParticleDefinition)
{
  particle_definition = aParticleDefinition;
  particle_charge = particle_definition->GetPDGCharge();
}


void MGGeneratorCosmicSurfMuons::GeneratePrimaryVertex(G4Event *evt)
{

  //MGLog(debugging) << "Start generation of primary vertex" << endlog;

  if(particle_definition==NULL) {
    MGLog(error) << "No particle has been defined!" << endlog;
    return;
  }
  
  SampleInitialPosition();
  GetEnergy();
  GenerateAngularSpectrum();
  
  // create a new vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);

  // create new primaries and set them to the vertex
  G4double mass =  particle_definition->GetPDGMass();
  G4double energy = particle_energy + mass;
  G4double pmom = sqrt(energy*energy-mass*mass);
  G4double px = pmom*particle_momentum_direction.x();
  G4double py = pmom*particle_momentum_direction.y();
  G4double pz = pmom*particle_momentum_direction.z();

  // muons, antimuons equally
  if (G4UniformRand() > 0.5) particle_definition = G4MuonMinus::MuonMinusDefinition();
  else particle_definition = G4MuonPlus::MuonPlusDefinition();

  if ( MGLogger::GetSeverity() <= MGLogger::debugging ) {
    MGLog(debugging) << "Particle name: " 
                     << particle_definition->GetParticleName() << endlog; 
    MGLog(debugging) << "       Energy: "<<particle_energy/GeV << " GeV" << endlog;
    MGLog(debugging) << "       pmom: "<<  pmom/GeV << " GeV" << endlog;
    MGLog(debugging) << "     Position: "<<particle_position/m << " m" <<  endlog; 
    MGLog(debugging)<< "    Theta: "<< particle_momentum_direction.theta()*180./pi << endlog;
    MGLog(debugging)<< "    Direction: "<<particle_momentum_direction << endlog;
  }

  for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ ) {
    G4PrimaryParticle* particle =
      new G4PrimaryParticle(particle_definition,px,py,pz);
    particle->SetMass( mass );
    particle->SetCharge( particle_charge );
    particle->SetPolarization(particle_polarization.x(),
                              particle_polarization.y(),
                              particle_polarization.z());
    vertex->SetPrimary( particle );
   if ( MGLogger::GetSeverity() <= MGLogger::debugging ) 
     MGLog(debugging) << "  particle "<< i << " total P  " << particle->GetMomentum().mag()/GeV << endlog;
  }

  // add vertex
  evt->AddPrimaryVertex( vertex );
  
  if ( MGLogger::GetSeverity() <= MGLogger::debugging ) {
      G4String materialName = gNavigator->LocateGlobalPointAndSetup(particle_position)->GetName();
      MGLog(debugging) << "Created primary vertex in " << materialName << endlog;
  }
}

