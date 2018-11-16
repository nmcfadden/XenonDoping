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
// Generator for neutrons from (alpha,n), from fission (with the
// correct spectrum) and cosmigenic in hall A of Gran Sasso Laboratory
// 
// Energy and angular spectra provided by H.Wulandari et al. (CREEST Collaboration)
//
// History:
// --------
// 13 Dec 2004   L.Pandola    First implementation
// 11 Jan 2005   L.Pandola    Added information of total rate
// 24 Jan 2005   L.Pandola    walls/floor/roof
// 25 Jan 2005   L.Pandola    Added messenger
// 26 Jan 2005   L.Pandola    Removed un-necessary methods

//MaGe includes
#include "generators/MGGeneratorNeutronsAtGS.hh"
#include "io/MGLogger.hh"
#include "generators/MGGeneratorNeutronsAtGSMessenger.hh"

#include "globals.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include <math.h>
//#include <iostream>
//#include <vector.h>
#include "G4TransportationManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Neutron.hh"


using namespace CLHEP;


MGGeneratorNeutronsAtGS::MGGeneratorNeutronsAtGS()
{
  NumberOfParticlesToBeGenerated = 1;
  particle_definition = G4Neutron::NeutronDefinition();
  G4ThreeVector zero(0., 0., 0.);
  particle_momentum_direction = G4ParticleMomentum(0., 0.,-1.);
  particle_energy = 1*MeV;
  particle_position = zero;
  particle_time = 0.0;
  particle_polarization = zero;
  particle_charge = particle_definition->GetPDGCharge();

  neutronFrom = "roof";

  theMessenger = new MGGeneratorNeutronsAtGSMessenger(this);
  halfz = 8.1*m;
  Radius = 8.0*m;
  samplerInitialized = false;
  activateFission = false;
}

MGGeneratorNeutronsAtGS::~MGGeneratorNeutronsAtGS()
{
  delete theMessenger;
}

void MGGeneratorNeutronsAtGS::SampleInitialPosition()
{
  G4ThreeVector RandPos;
  G4double z=0.;
  G4double r=0,phi=0;

  if (neutronFrom == "roof")
    {
      r     = sqrt(Radius*Radius*G4UniformRand());
      phi   = twopi * G4UniformRand();
      RandPos.setX(r*cos(phi));
      RandPos.setY(r*sin(phi));
      RandPos.setZ(z);
      G4ThreeVector CentreCoords(0.,0.,halfz);
      particle_position = CentreCoords + RandPos;
    }
  else if (neutronFrom == "floor")
    {
      r     = sqrt(Radius*Radius*G4UniformRand());
      phi   = twopi * G4UniformRand();
      RandPos.setX(r*cos(phi));
      RandPos.setY(r*sin(phi));
      RandPos.setZ(z);
      G4ThreeVector CentreCoords2(0.,0.,-1.0*halfz);
      particle_position = CentreCoords2 + RandPos;
    }
  else if (neutronFrom == "walls")
    {
      r = Radius;
      z = 2.0*halfz*(G4UniformRand()-0.5);
      phi = twopi * G4UniformRand();
      RandPos.setX(r*cos(phi));
      RandPos.setY(r*sin(phi));
      RandPos.setZ(z);
      particle_position = RandPos;
    }
  else
    {
      MGLog(error) << "Something wrong! " << endlog;
    }
}


void MGGeneratorNeutronsAtGS::GenerateAngularSpectrum()
{
  G4double xrand = G4UniformRand();
  G4double costheta = 0;
  G4double phi = 0;
  G4int j=0;
  
  for (j=0;j<(nbin_ang-1);j++)
    {
      if (xrand > userpdf_ang[j] && xrand <= userpdf_ang[j+1]) break; 
    }
  G4double sampled_bin = (G4double) j;
  if (userpdf_ang[j+1] != userpdf_ang[j]) sampled_bin += ((userpdf_ang[j+1]-xrand)/(userpdf_ang[j+1] - userpdf_ang[j]));
  costheta = -1.0+(sampled_bin*0.1);
  //MGLog(debugging) << "Costheta: " << costheta << " " << xrand << " " << sampled_bin <<endlog;
  if (neutronFrom == "walls")
    {
      G4ThreeVector zero;
      G4double phigoal = (zero-particle_position).phi();
      phi = (G4UniformRand()-0.5)*90*deg + phigoal;
    }
  else
    {
      phi = twopi*G4UniformRand();
    }
  G4double sintheta = std::sqrt(1.0-costheta*costheta);
  particle_momentum_direction.setX(sintheta*std::cos(phi));
  particle_momentum_direction.setY(sintheta*std::sin(phi));
  particle_momentum_direction.setZ(-1.0*costheta);
  return;
}


void MGGeneratorNeutronsAtGS::ShootEnergyFission()
{
  G4int j=0;
  //initialize the sampler: read spectrum from file
  if (!samplerInitialized)
    {
      MGLog(trace) << "Neutrons generated in: " << neutronFrom << endlog;
      MGLog(warning) << "Only the component > 1 MeV is accounted " << endlog;
       char* path = getenv("MGGENERATORDATA");
       if (!path)
	 {
	   MGLog(error)<< "MGGENERATORDATA environment variable not set!" << endlog;
	   MGLog(fatal) << endlog;
	 }
       G4String pathString(path);
       G4String pathFile = pathString + "/nflux_gs_fine_bins.txt";
       std::ifstream file(pathFile);
       if (!(file.is_open()))
	 {
	   MGLog(error) << "Data file " + pathFile + "not found!" << endlog;
	   MGLog(fatal) << endlog;
	 }
       //energybin is the lower edge of the bin
       G4double tempVar=0,tempEn=0;
       userpdf[j] = 0;
       for (j=0;j<(nbin-1);j++)
	 {
	   file >> tempEn >> tempVar;
	   energybin[j]=tempEn*MeV;
	   userpdf[j+1]=userpdf[j]+tempVar;
	 }
       energybin[nbin-1]=energybin[nbin-2];
       G4double xnormalization = userpdf[nbin-1];
       MGLog(trace) << "Total neutron rate above 1 MeV " << "12 neutrons/m2 h (wet rock)" << endlog;
       MGLog(trace) << "Total neutron rate above 1 MeV " << "21 neutrons/m2 h (dry rock)" << endlog;
       for (j=0;j<nbin;j++)
	{
	  userpdf[j]=userpdf[j]/xnormalization;
	}
      MGLog(trace) << "Energy sampler for fission initialized" << endlog;
      file.close();

      //Angular spectrum is isotropic
      G4double temp=0;
      userpdf_ang[0]=0.;
      for (j=0;j<(nbin_ang-1);j++)
	{
	  if (neutronFrom == "roof") {
	    if (j>(nbin_ang/2)) 
	      {
		temp =1;
	      }
	    else 
	      {
		temp = 0;
	      }
	  }
	  else if (neutronFrom == "floor") 
	    {
	      if (j>(nbin_ang/2)) 
		{
		  temp =0;
		}
	    else 
	      {
		temp = 1;
	      }
	  }
	  else if (neutronFrom == "walls") 
	    {
	      temp =1;
	    }
	  userpdf_ang[j+1] = userpdf_ang[j]+temp;
	}
      xnormalization = userpdf_ang[nbin_ang-1];
      for (j=0;j<nbin_ang;j++)
	{
	  userpdf_ang[j]=userpdf_ang[j]/xnormalization;
	  MGLog(debugging) << "userpdf_ang[" << j << "] = " << userpdf_ang[j] << endlog;
	}
      MGLog(trace) << "Angular sampler for fission initialized" << endlog;
      samplerInitialized = true;
    }
  G4double xrand = G4UniformRand();

 //  for (j=0;j<nbin;j++)
//     {
//       G4cout << j << " " << energybin[j]/MeV << " " << userpdf[j] << G4endl;
//     } 
//   G4cout << "Random value: " << xrand << G4endl;

  for (j=0;j<(nbin-1);j++)
    {
      if (xrand > userpdf[j] && xrand <= userpdf[j+1]) break; 
    }

  if (userpdf[j+1] != userpdf[j]) 
    {
      particle_energy = energybin[j]+G4UniformRand()*(energybin[j+1]-energybin[j]);
      //MGLog(debugging) << xrand << " " << userpdf[j] << " " << userpdf[j+1] << 
      //" " << energybin[j] << " " << energybin[j+1] << " " << particle_energy << endlog;
    }
  else 
      particle_energy = energybin[j];
  return;
}

void MGGeneratorNeutronsAtGS::ShootEnergyMuons()
{
  G4double energysup = 100*GeV;
  G4double energyinf = 1*MeV;
  G4double step = (log10(energysup)-log10(energyinf))/((float) (nbin_mu-1));

  G4int j=0;

  //initialize the sampler: logarithmic sampling between 1 MeV and 10 TeV
  if (!samplerInitialized)
    {
      MGLog(trace) << "Neutrons generated in: " << neutronFrom << endlog;
      char* path = getenv("MGGENERATORDATA");
      if (!path)
	{
	  MGLog(error)<< "MGGENERATORDATA environment variable not set!" << endlog;
	  MGLog(fatal) << endlog;
	}
      G4String pathString(path);
      G4String pathFile = pathString + "/nflux_gs_muons_" + neutronFrom + ".txt";
      std::ifstream file(pathFile);
      if (!(file.is_open()))
	{
	  MGLog(error) << "Data file " + pathFile + "not found!" << endlog;
	  MGLog(fatal) << endlog;
	}

      //G4double log_energy = 0;
      //G4double true_energy = 0; 
      G4double temp;
      userpdf[0]=0.;
      for (j=0;j<(nbin_mu-1);j++)
	{
	  file >> temp;
	  userpdf_mu[j+1] = userpdf_mu[j]+temp;
	}
      G4double xnormalization = userpdf_mu[nbin_mu-1];
      for (j=0;j<nbin_mu;j++)
	{
	  userpdf_mu[j]=userpdf_mu[j]/xnormalization;
	  MGLog(debugging) << "userpdf[" << j << "] = " << userpdf_mu[j] << endlog;
	}
      MGLog(trace) << "Energy sampler for cosmogenic initialized" << endlog;
      MGLog(trace) << "Total neutron rate: 269 n/m^2 y above 1 MeV" << endlog;
      MGLog(trace) << "  71 n/m^2 y above 1 MeV from the roof" << endlog;
      MGLog(trace) << "  159 n/m^2 y above 1 MeV from the walls" << endlog;
      MGLog(trace) << "  38 n/m^2 y above 1 MeV from the floor" << endlog;
      MGLog(warning) << "Neutrons generated from: " << neutronFrom << endlog;
      file.close();
      
      //Read also the angular spectrum
      pathFile = pathString + "/nflux_ang_muons_" + neutronFrom + ".txt";
      std::ifstream file_a(pathFile);
      if (!(file_a.is_open()))
	{
	  MGLog(error) << "Data file " + pathFile + "not found!" << endlog;
	  MGLog(fatal) << endlog;
	}

      //G4double log_energy = 0;
      //G4double true_energy = 0; 
      userpdf_ang[0]=0.;
      for (j=0;j<(nbin_ang-1);j++)
	{
	  file_a >> temp;
	  userpdf_ang[j+1] = userpdf_ang[j]+temp;
	}
      xnormalization = userpdf_ang[nbin_ang-1];
      for (j=0;j<nbin_ang;j++)
	{
	  userpdf_ang[j]=userpdf_ang[j]/xnormalization;
	  MGLog(debugging) << "userpdf_ang[" << j << "] = " << userpdf_ang[j] << endlog;
	}
      MGLog(trace) << "Angular sampler for cosmogenic initialized" << endlog;
      samplerInitialized = true;
      file_a.close();
    }
  G4double xrand = G4UniformRand();
  for (j=0;j<(nbin_mu-1);j++)
    {
      if (xrand > userpdf_mu[j] && xrand <= userpdf_mu[j+1]) break; 
    }
  G4double sampled_bin = (G4double) j;
  if (userpdf_mu[j+1] != userpdf_mu[j]) sampled_bin += ((userpdf_mu[j+1]-xrand)/(userpdf_mu[j+1] - userpdf_mu[j]));
  particle_energy = energyinf*pow(10,sampled_bin*step);
  //MGLog(debugging) << xrand << " " << sampled_bin << " " << step <<  endlog;
  //MGLog(debugging) << "Particle energy " << particle_energy/MeV << " MeV" << endlog;
  return; 
}

void MGGeneratorNeutronsAtGS::SetParticleDefinition
  (G4ParticleDefinition* aParticleDefinition)
{
  particle_definition = aParticleDefinition;
  particle_charge = particle_definition->GetPDGCharge();
}


void MGGeneratorNeutronsAtGS::GeneratePrimaryVertex(G4Event *evt)
{

  if(particle_definition==NULL) {
    MGLog(error) << "No particle has been defined!" << endlog;
    return;
  }
  
  SampleInitialPosition();
  if (activateFission)  
    {
      ShootEnergyFission();
    }
  else 
    {
      ShootEnergyMuons();
    }
  GenerateAngularSpectrum();
  
  // create a new vertex
  G4PrimaryVertex* vertex = 
    new G4PrimaryVertex(particle_position,particle_time);

  // create new primaries and set them to the vertex
  G4double mass =  particle_definition->GetPDGMass();
  G4double energy = particle_energy + mass;
  G4double pmom = sqrt(energy*energy-mass*mass);
  
  G4double px = pmom*particle_momentum_direction.x();
  G4double py = pmom*particle_momentum_direction.y();
  G4double pz = pmom*particle_momentum_direction.z();


  MGLog(debugging) << "Particle name: " 
		   << particle_definition->GetParticleName() << endlog; 
  MGLog(debugging) << "       Energy: "<<particle_energy/MeV << " MeV" << endlog;
  MGLog(debugging) << "     Position: "<<particle_position/m << " m; (" << neutronFrom << ")" << endlog; 
  MGLog(debugging)<< "    Direction: "<<particle_momentum_direction << endlog;
  MGLog(debugging) << " NumberOfParticlesToBeGenerated: "
		   << NumberOfParticlesToBeGenerated << endlog;

  //   std::ofstream file1("test.out",std::ios::app);
//   std::ofstream file2("test2.out",std::ios::app);
//   //file1 << particle_position.x() <<" " << particle_position.y() << " " << 
//   //  particle_position.z() << G4endl;
//   file2 << particle_momentum_direction.x() << " " << particle_momentum_direction.y() << 
//     " " << particle_momentum_direction.z() << G4endl;
  //  file1 << particle_energy/MeV << " " << log10(particle_energy/MeV) << endl;
  // file1.close();
//   file2.close();

  particle_definition = G4Neutron::NeutronDefinition();

  for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ ) {
    G4PrimaryParticle* particle =
      new G4PrimaryParticle(particle_definition,px,py,pz);
    particle->SetMass( mass );
    particle->SetCharge( particle_charge );
    particle->SetPolarization(particle_polarization.x(),
                              particle_polarization.y(),
                              particle_polarization.z());
    vertex->SetPrimary( particle );
  }

  evt->AddPrimaryVertex( vertex );
}

void MGGeneratorNeutronsAtGS::SetMuonOrigin(G4String val)
{
  if (val == "roof") 
    {
      if (val != neutronFrom) 
	{
	  neutronFrom = val;
	  samplerInitialized = false;
	  MGLog(trace) << "Set roof" << endlog;
	}
    }
  else if (val == "floor")
    {
      if (val != neutronFrom) 
	{
	  neutronFrom = val;
	  samplerInitialized = false;
	  MGLog(trace) << "Set floor" << endlog;
	}
    }
  else if (val == "walls")
    {
      if (val != neutronFrom) 
	{
	  neutronFrom = val;
	  samplerInitialized = false;
	  MGLog(trace) << "Set walls" << endlog;
	}
    }
  else
    {
      MGLog(warning) << "Wrong value, nothing happens" << endlog;
    }
}

void MGGeneratorNeutronsAtGS::SetFissionFlag(G4bool bol)
{
  if (bol != activateFission)
    {
      activateFission = bol;
      samplerInitialized = false;
    }
}
