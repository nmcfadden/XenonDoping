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
// Generator for neutrons from (alpha,n) and fission from materials calculated 
// by the Sources4A code from LANL. The angular distribution is isotropic. To get 
// the real spectrum in the detector, one must track the neutrons through their 
// source material. 
// 
// The file name must be indicated with the path, otherwise it is looked for in the
// current directory (just like it is done for decay0)
// 
// History:
// --------
// 28 Dec 2005   L.Pandola    First implementation


#include "generators/MGGeneratorSources4AInterface.hh"
#include "io/MGLogger.hh"
#include "generators/MGGeneratorSources4AInterfaceMessenger.hh"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4Neutron.hh"
#include <string.h>

using namespace CLHEP;

MGGeneratorSources4AInterface::MGGeneratorSources4AInterface() 
{
  sourceFile = "tape7"; //default
  fTheMessenger = new MGGeneratorSources4AInterfaceMessenger(this);
  
  SourcesInitialized = false;

  SourcesSpectrum = new std::vector<G4double>;
  SourcesEnergy = new std::vector<G4double>;
 
  G4ThreeVector zero;
  particle_position =  zero;
  particle_time = 0.0; 
}

MGGeneratorSources4AInterface::~MGGeneratorSources4AInterface()
{
  delete fTheMessenger;
  delete SourcesSpectrum;
  delete SourcesEnergy;
}


void MGGeneratorSources4AInterface::InitializeSources4A()
{
  std::ifstream file(sourceFile);
  char header[160];
  if (!(file.is_open()))
    {
      MGLog(error) << "Data Sources4A file " + sourceFile + " not found!" << endlog;
      return;
    }
  G4int ifound=0;
  G4int j=0;
  //1) look for the neutron energy groups
  for (j=0;;j++)
    {
      file.getline(header,160);
      ifound = strncmp (header," Neutron Multigroup Structure",15);
      if (!ifound) break;
      if (j == 50) {
	MGLog(error) << "Problem why reading the file: is it corrupted?" << endlog;
	MGLog(fatal) << endlog;
        G4Exception("MGGeneratorSources4AInterface::InitializeSources4A()", "err001", FatalException, " ");
      }
    }
  G4double appoggio=-1;
  for (j=0;;j++)
    {
      file >> appoggio;
      SourcesEnergy->push_back(appoggio*MeV);
      if (appoggio==0) break;
    }

  //2) now look for the corresponding spectrum
  for (j=0;;j++)
    {
      file.getline(header,160);
      ifound = strncmp (header," Total Neutron Spectrum",15);
      if (!ifound) break;
      if (file.eof()) 
	{
	  MGLog(error) << "Problem why reading the file: is it corrupted?" << endlog;
	  MGLog(fatal) << endlog;
          G4Exception("MGGeneratorSources4AInterface::InitializeSources4A()", "err002", FatalException, " ");
	}
    }
  
  SourcesSpectrum->push_back(0.);
  G4double cumulative=0.;
  for (size_t k=0;k<(SourcesEnergy->size()-1);k++)
    {
      file >> appoggio;
      cumulative += appoggio;
      SourcesSpectrum->push_back(cumulative);
    }
  file.close();
  MGLog(trace) << "Data file " << sourceFile << " successfully read" << endlog;

  //Now normalize the spectrum
  G4double normal = (*SourcesSpectrum)[SourcesSpectrum->size() - 1];
  MGLog(trace) << "Integral flux from Sources4A: " << normal << " n/cm3 s" << endlog;
  for (size_t k=0;k<SourcesSpectrum->size();k++)
    {
      (*SourcesSpectrum)[k] = (*SourcesSpectrum)[k]/normal;
    }
  SourcesInitialized = true;
}

G4double MGGeneratorSources4AInterface::ShootSources4A()
{
  if (!SourcesInitialized)
    {
      InitializeSources4A();
      if (!SourcesInitialized) { //if it is still un-initialized = problems!
	MGLog(fatal) << "Sources spectrum not initialized correctly" << endlog;
        G4Exception("MGGeneratorSources4AInterface::ShootSources4A()", "err001", FatalException, " ");
      }
    }
  G4double rand = G4UniformRand();
  size_t k=0;
  for(k=1;k < SourcesSpectrum->size() ; k++)
    {
      if (rand > (*SourcesSpectrum)[k-1] && rand < (*SourcesSpectrum)[k]) break;
    }
  //Bin from k-1 to k
  G4double enelow = (*SourcesEnergy)[k-1];
  G4double eneup = (*SourcesEnergy)[k];
  G4double ene = enelow + (rand - (*SourcesSpectrum)[k-1])/
    ((*SourcesSpectrum)[k] - (*SourcesSpectrum)[k-1])
    * (eneup-enelow);
  return ene;
}

void MGGeneratorSources4AInterface::GeneratePrimaryVertex(G4Event* evt)
{
  G4double px,py,pz;

  G4PrimaryVertex* vertex = new G4PrimaryVertex (particle_position,
						 particle_time);

  G4double kinEnergy = ShootSources4A()*MeV; //this is the kinetic energy
  G4ParticleDefinition* aNeutron = G4Neutron::NeutronDefinition();
  G4double nMass = aNeutron->GetPDGMass();
  G4double momentum = std::sqrt(kinEnergy*kinEnergy+2.0*kinEnergy*nMass);
  G4double cosTheta = -1.0 + 2.0*G4UniformRand();
  G4double sinTheta = std::sqrt(1.0-cosTheta*cosTheta);
  G4double phi = twopi*G4UniformRand();
  px = momentum*sinTheta*std::cos(phi);
  py = momentum*sinTheta*std::sin(phi);
  pz = momentum*cosTheta;

  //Create primary particle
  G4PrimaryParticle* particle = 
    new G4PrimaryParticle(aNeutron,
			  px*MeV,py*MeV,pz*MeV);
  
  particle->SetProperTime(particle_time); 

  //Add particle to vertex
  vertex->SetPrimary(particle);

  evt->AddPrimaryVertex(vertex);
}


void MGGeneratorSources4AInterface::ChangeFileName(G4String newname)
{
  if (newname == sourceFile) return; //nothing happens
  std::ifstream check(newname);
  if (!(check.is_open()))
    {
      MGLog(warning) << "Data Sources4A file " + sourceFile + " does not exist!" << endlog;
      MGLog(warning) << "Nothing happens" << endlog;
      return;
    }
  check.close();
  SourcesInitialized = false;
  SourcesEnergy->clear();
  SourcesSpectrum->clear();
  sourceFile = newname;
  InitializeSources4A();
  if (SourcesInitialized) 
    MGLog(trace) << "Sources4A file initialized" << endlog;
}
