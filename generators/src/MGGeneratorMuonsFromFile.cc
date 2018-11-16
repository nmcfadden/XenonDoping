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
// $Id: MGGeneratorMuonsFromFile.cc,v 1.5 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorG4Gun.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: This MuonsFromFile should only be used together with the file, containing the
 *                dangerous muons. These are those muons, that had energy deposition around
 *                Q_betabeta in the Ge-crystals
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M. Knapp
 * CONTACT: msknapp@pit.physik.uni-tuebingen.de 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 28 Dec 2005, L. Pandola, Replaced G4cout with MGLog
 * 30 Dec 2005, L. Pandola, Now this is a class derived from G4VPrimaryGenerator
 *                          still to be corrected: the file is opened and completely read  
 *                          at each single event.
 * 10 Jan 2006, L. Pandola, The file is opened and read only once. 
 * 19 May 2006  M. Knapp,   Corrected the energy of muons [MeV] instead of [GeV]
 * 
 */
//---------------------------------------------------------------------------//
//

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include <math.h>
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4VPhysicalVolume.hh"

#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementEventAction.hh"
//---------------------------------------------------------------------------//

#include "generators/MGGeneratorMuonsFromFile.hh"
#include "generators/MGGeneratorMuonsFromFileMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorMuonsFromFile::MGGeneratorMuonsFromFile()
{
  fileName = "muon00.dat"; //default fileName
  theMessenger = new MGGeneratorMuonsFromFileMessenger(this);
  G4ThreeVector zero;
  particle_position = zero;
  fFileIsOpen = false;
}

//---------------------------------------------------------------------------//

MGGeneratorMuonsFromFile::MGGeneratorMuonsFromFile(const MGGeneratorMuonsFromFile & other) : G4VPrimaryGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorMuonsFromFile::~MGGeneratorMuonsFromFile()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void MGGeneratorMuonsFromFile::OpenDataFile()
{
  G4String filename = GetFileName();

  char* path = getenv("MGGENERATORDATA");
  MGLog(debugging) << "MGGenerator : " << path << endlog;

  if (!path)
    {
      MGLog(error) << "!!!!!!!!! MGGENERATORDATA environment variable not set!!!!!!!!!" << endlog;
      MGLog(fatal) << endlog;
      G4Exception("MGGeneratorMuonsFromFile::OpenDataFile()", "err001", FatalException, "Exiting program");
    }
  else
   {
     MGLog(routine) << "Using dangerous muons data: " << filename << endlog;
    }

  G4String pathString(path);
  G4String pathFile = pathString + "/" + filename;
  MGLog(debugging) << "Pathfile is : " << pathFile << endlog; 
  std::ifstream infile(pathFile);
  if (!infile)
    {
      MGLog(error) << "ERROR!!!! Could not open datafile" << endlog;
      MGLog(fatal) << endlog;
    }
  
  max_entries = 0;
  for(G4int i=0 ;i>(-1) ; i++)
    {
      infile >> x[i] >> y[i] >> z[i] >> px[i] >> py[i] >> pz[i] >> energy[i];
      if (!infile.good()) break;
      max_entries++; 
    }
  MGLog(debugging) << "Total file entries: " << max_entries << endlog;
  fFileIsOpen = true;
}

void MGGeneratorMuonsFromFile::GeneratePrimaryVertex(G4Event *event)
{
  //1) if the data file is still not opened, open it!
  if (!fFileIsOpen) 
    {
      OpenDataFile();
      //Now check if it is really opened
      if (!fFileIsOpen) 
	{
	  MGLog(error) << "Something went wrong with the data file: could not open it" << endlog;
	  MGLog(fatal) << endlog;
          G4Exception("MGGeneratorMuonsFromFile::GeneratePrimaryVertex()", "err001", FatalException, "Exiting program");
	}
    }

  //2) Now the data have been loaded. Let's generate the event
  ev_ID = event->GetEventID(); 
  G4int theMuonNumber= ev_ID;

  while(theMuonNumber >= max_entries)
    {
      theMuonNumber = theMuonNumber-max_entries;
    }

  //Notice: the particle coordinates are expressed in cm
  //The energy in MeV
  particle_time = 0.0*s;
  particle_position.setX(x[theMuonNumber]*cm);
  particle_position.setY(y[theMuonNumber]*cm);
  particle_position.setZ(z[theMuonNumber]*cm);



  G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);
  
  G4ParticleDefinition* particle_definition = 0;

  if (G4UniformRand() > 0.5) 
    {
      particle_definition = G4MuonPlus::MuonPlusDefinition();
    }
  else
    {
      particle_definition = G4MuonMinus::MuonMinusDefinition();
    }

  // create new primaries and set them to the vertex
  G4double mass =  particle_definition->GetPDGMass();
  G4double energyT = energy[theMuonNumber]*MeV + mass; //total energy
  G4double pmom = std::sqrt(energyT*energyT-mass*mass); //total particle momentum
  
  G4ThreeVector momentum(px[theMuonNumber],py[theMuonNumber],pz[theMuonNumber]);

  momentum.unit(); //normalizes momentum to unit (if it is not yet)

  G4double px_n = pmom*momentum.x();
  G4double py_n = pmom*momentum.y();
  G4double pz_n = pmom*momentum.z();

  MGLog(debugging) << "Eventnumber: " << ev_ID << endlog;
  MGLog(debugging) << "Particle: " << particle_definition->GetParticleName() << endlog;
  MGLog(debugging) << "Position " << particle_position/cm << " cm " << endlog; 
  MGLog(debugging) << "Momentum direction " << momentum << endlog;
  MGLog(debugging) << "Energy " << energy[theMuonNumber]/MeV << " MeV" << endlog;

  G4PrimaryParticle* thePrimaryParticle = 
    new G4PrimaryParticle(particle_definition,px_n*MeV,py_n*MeV,pz_n*MeV);
  
  vertex->SetPrimary(thePrimaryParticle);

  event->AddPrimaryVertex(vertex);
}

void MGGeneratorMuonsFromFile::SetFileName(G4String stri) 
{
  if (fileName == stri) return; //the file is the same as the old one:
  //nothing happens

  fileName = stri; //if different, assign the new name
  fFileIsOpen = false; //the file will be re-opened
  return;
}
