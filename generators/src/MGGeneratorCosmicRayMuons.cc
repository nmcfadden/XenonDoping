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
// 28 Oct 2004   L.Pandola    First implementation (not working)
// 02 Dec 2004   L.Pandola    Working implementation; added energy and angular spectrum
// 22 Dec 2004   L.Pandola    Fixed bug and changed defaults (according to Gerda geometry)
// 13 Jan 2004   L.Pandola    Bug fixed
// 08 Feb 2004   L.Pandola    Angular spectrum read from file (MACRO data)
// 14 Dec 2005   L.Pandola    Corrected energy sampling
// 07 Dec 2011   A. Schubert  Added if statements around debugging output

//MaGe includes
#include "generators/MGGeneratorCosmicRayMuons.hh"
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

MGGeneratorCosmicRayMuons::MGGeneratorCosmicRayMuons()
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
  halfz = 8.1*m;
  Radius = 8.0*m;
  spectralIndex = 3.7;
  rockDepth = 3400.0*m; //m.w.e.

  fileName = "zenith_azimuth.dat";
  samplerInitialized = false;
  fileFound = false;

  //limits for the energy sampling
  energysup = 10*TeV;
  energyinf = 1*GeV;

  theMessenger = new MGGeneratorCosmicRayMuonsMessenger(this);
  gNavigator = G4TransportationManager::GetTransportationManager()
    ->GetNavigatorForTracking();
  
  angularpdf = new G4DataVector();
  costheta_pdf = new G4DataVector();  
  phi_pdf = new G4DataVector;
  nx = 0;
  ny = 0;
}

MGGeneratorCosmicRayMuons::~MGGeneratorCosmicRayMuons()
{
  delete theMessenger;
  delete angularpdf;
  delete costheta_pdf;
  delete phi_pdf;
}

void MGGeneratorCosmicRayMuons::SampleInitialPosition()
{
  G4ThreeVector RandPos;
  G4double z=0.;
  
  //Positions are generated uniformly from a circle
  G4double r     = sqrt(Radius*Radius*G4UniformRand());
  G4double phi   = twopi * G4UniformRand();
  RandPos.setX(r*cos(phi));
  RandPos.setY(r*sin(phi));
  RandPos.setZ(z);
  G4ThreeVector CentreCoords(0.,0.,halfz);
  particle_position = CentreCoords + RandPos;
}

void MGGeneratorCosmicRayMuons::GenerateAngularSpectrum()
{
  G4double costheta = 0.;
  size_t j=0;
  G4double phi = 0;
  G4double sintheta = 0;

  if (!fileFound) 
    {
      //According to Phys. Rev. D 44 (1991) 3543, the angular spectrum underground 
      //is proportional to 1/cos(theta) --> with respect to the horizontal
      G4double costhetamin = 0.00001;
      costheta = pow(costhetamin,(1.0-G4UniformRand()));
      phi = G4UniformRand()*360*degree;
      sintheta = sqrt(1.0-costheta*costheta);
      particle_momentum_direction.setX(cos(phi)*costheta);
      particle_momentum_direction.setY(sin(phi)*costheta);
      particle_momentum_direction.setZ(-1.0*sintheta);
    }
  else //sampling from file
    {
      //warning: the spectrum MUST be evenly-spaced in costheta and in phi
      G4double step1 = 1./((G4double) nx);
      G4double step2 = 360./((G4double) ny);
      
      //explicit sampling of costheta and phi
      G4double xrand = G4UniformRand();
      //this is slow. Something more efficienct would be good
      for (j=0;j<costheta_pdf->size();j++)
	{
	  if (xrand > (*angularpdf)[j] && xrand <= (*angularpdf)[j+1]) break; 
	}
      costheta = step1*(G4UniformRand()-0.5)+(*costheta_pdf)[j];
      phi = (step2*(G4UniformRand()-0.5)+(*phi_pdf)[j])*degree;
      if (costheta < 0) costheta =0;
      if (costheta > 1) costheta =1;
      sintheta = sqrt(1.0-costheta*costheta);

      MGLog(debugging) << xrand << " " << j << " " << costheta << " " << phi/degree << endlog;
      
      particle_momentum_direction.setX(cos(phi)*sintheta);
      particle_momentum_direction.setY(sin(phi)*sintheta);
      particle_momentum_direction.setZ(-1.0*costheta);
    }
}


void MGGeneratorCosmicRayMuons::ShootEnergy()
{
  G4double step = (log10(energysup)-log10(energyinf))/((float) nbin-1);
  G4int j=0;

  //initialize the sampler: logarithmic sampling between 1 GeV and 10 TeV
  //angular distribution is read from file (MACRO) Astrop. Journ. 412: 310-311 1993
  if (!samplerInitialized)
    {
      G4double log_energy = 0;
      G4double true_energy = 0;    
      userpdf[0]=0.;
      for (j=1;j<nbin;j++)
	{
	  log_energy = j*step;
	  true_energy = energyinf*std::pow(10,log_energy);
	  G4double base = energyinf*std::pow(10,log_energy)*(1.0-std::pow(10,-1.0*step));
	  userpdf[j] = userpdf[j-1]+MuonSpectrum(true_energy)*base;
	}
      G4double xnormalization = userpdf[nbin-1];
      for (j=0;j<nbin;j++)
	{
	  userpdf[j]=userpdf[j]/xnormalization;
	}
      MGLog(trace) << "Energy sampler initialized." << endlog; 
      MGLog(trace) << "Spectrum from Lipari and Stanev, Phys. Rev. D 44 (1991) 3543" << endlog;

      //Read from file the MACRO angular spectrum
      size_t jj=0;
      char* path = getenv("MGGENERATORDATA");
      if (!path)
	{
	  MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
	  path = (char*) "generators/data";
	}
      G4String pathString(path);
      G4String pathFile = pathString + "/" + fileName;
      std::ifstream file(pathFile);
      if (file.is_open())
	{
	  G4double bb=0;
	  G4double aa = 0;
	  G4double cc = 0;
	  angularpdf->push_back(bb);
	  
	  do{
	    aa = 0;
	    bb = 0;
	    cc = 0;
	    file >> aa >> cc >> bb; //format: costheta, phi, distribution
	    //	MGLog(debugging) << aa << " " << bb << endlog;
	    if (aa != 0 || bb != 0 || cc !=0) //prevents the last value to be inserted twice
	      {
		costheta_pdf->push_back(aa);
		phi_pdf->push_back(cc);
		bb += (*angularpdf)[angularpdf->size()-1];
		angularpdf->push_back(bb);
	      }
	  }while(!file.eof());
	  
	  xnormalization = (*angularpdf)[angularpdf->size()-1];
	  for (jj=0;jj<angularpdf->size();jj++)
	    {
	      (*angularpdf)[jj]=(*angularpdf)[jj]/xnormalization;
	      //MGLog(debugging) << "angularpdf[" << jj << "] = " << (*angularpdf)[jj] << endlog;
	    }
	  //Finds the number of bins in x (costheta) and y (phi)
	  nx = 0;
	  ny = 0;
	  for (jj=0;jj<costheta_pdf->size();jj++)
	    {
	      if ((*costheta_pdf)[0] == (*costheta_pdf)[jj]) ny++;
	      if ((*phi_pdf)[0] == (*phi_pdf)[jj]) nx++;
	    }
	  MGLog(trace) << "Angular sampler initialized. " << endlog;
	  MGLog(trace) << "Data from MACRO, Astrop. Journ. 412 (1993) 310-311" << endlog;
	  MGLog(debugging) << "Total binning: " << nx << " in costheta" <<  endlog;
	  MGLog(debugging) << "Total binning: " << ny << " in phi" <<  endlog;
	  file.close();
	  fileFound = true;
	}
      else
	{
	  MGLog(warning) << "Data file " + pathFile + "not found!" << endlog;
	  MGLog(warning) << "Using 1/(costheta) angolar distribution " << endlog;
	}

      samplerInitialized = true;
    }
  //The spectrum generated in this way is coincident with the one reported in 
  //Fig.7 of Astrop. Phys. 6 (1997) 129 for the Gran Sasso depth
  

  G4double xrand = G4UniformRand();
  for (j=0;j<(nbin-1);j++)
    {
      if (xrand > userpdf[j] && xrand <= userpdf[j+1]) break; 
    }
  G4double sampled_bin = (G4double) j;
  if (userpdf[j+1] != userpdf[j]) sampled_bin += ((userpdf[j+1]-xrand)/(userpdf[j+1] - userpdf[j]));
  particle_energy = energyinf*pow(10,sampled_bin*step);
  return;
}

void MGGeneratorCosmicRayMuons::SetParticleDefinition
  (G4ParticleDefinition* aParticleDefinition)
{
  particle_definition = aParticleDefinition;
  particle_charge = particle_definition->GetPDGCharge();
}


void MGGeneratorCosmicRayMuons::GeneratePrimaryVertex(G4Event *evt)
{

  //MGLog(debugging) << "Start generation of primary vertex" << endlog;

  if(particle_definition==NULL) {
    MGLog(error) << "No particle has been defined!" << endlog;
    return;
  }
  
  SampleInitialPosition();
    
  //Energy stuff
  ShootEnergy();

  // Angular stuff
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

  if (G4UniformRand() > 0.5) 
   particle_definition = G4MuonMinus::MuonMinusDefinition();
  else
    particle_definition = G4MuonPlus::MuonPlusDefinition();

  if ( MGLogger::GetSeverity() <= MGLogger::debugging ) {
    MGLog(debugging) << "Particle name: " 
                     << particle_definition->GetParticleName() << endlog; 
    MGLog(debugging) << "       Energy: "<<particle_energy/GeV << " GeV" << endlog;
    MGLog(debugging) << "     Position: "<<particle_position/m << " m" <<  endlog; 
    MGLog(debugging)<< "    Direction: "<<particle_momentum_direction << endlog;
    //   MGLog(debugging) << " NumberOfParticlesToBeGenerated: "
    // 		   << NumberOfParticlesToBeGenerated << endlog;
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
  }

  evt->AddPrimaryVertex( vertex );
  
  if ( MGLogger::GetSeverity() <= MGLogger::debugging ) {
      G4String materialName = gNavigator->LocateGlobalPointAndSetup(particle_position)->GetName();
      MGLog(debugging) << "Created primary vertex in " << materialName << endlog;
  }

}


G4double MGGeneratorCosmicRayMuons::MuonSpectrum(G4double ene)
{
  //The parametrization is taken from P. Lipari and T. Stanev
  //Phys. Rev. D 44 (1991) 3543

  //The energy must be expressed in TeV
  //Depth in km w.e.

  G4double beta,epsilon;
  if (spectralIndex == 2.0) 
    {
      beta = 0.465;
      epsilon = 0.569;
    }
  else if (spectralIndex == 2.7)
    {
      beta = 0.418;
      epsilon = 0.557;
    }
  else //this is the default case
    {
      beta = 0.383;
      epsilon = 0.618;
    }

  G4double part1 = epsilon*(1-std::exp(-1.0*beta*rockDepth/km));
  G4double part2 = std::pow((ene/TeV)+part1,-1*spectralIndex);
  G4double part3 = std::exp(beta*(rockDepth/km)*(1-spectralIndex));
  return part2*part3;
}
  
