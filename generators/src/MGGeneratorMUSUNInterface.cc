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
// History:
// --------
// 27 Apr 2005   L.Pandola    First implementation
// 24 Jul 2008   L.Pandola    Changed format according to the new MUSUN output to 
//                            be used for the Gerda MCC2. The format is:
//                            eventID, particleID, energy (GeV), coordinates x,y,z (cm),
//                            theta(rad) and phi(rad)

//Geant4 includes
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <string.h>

//MaGe includes
#include "generators/MGGeneratorMUSUNInterface.hh"
#include "io/MGLogger.hh"
#include "generators/MGGeneratorMUSUNInterfaceMessenger.hh"

using namespace CLHEP;

MGGeneratorMUSUNInterface::MGGeneratorMUSUNInterface(G4String File) :
  fFileName(File)
{
  fTheMessenger = new MGGeneratorMUSUNInterfaceMessenger(this);

  //geometry initialization
  fZShift = 0*cm;

  G4ThreeVector zero;
  particle_position =  zero;
  particle_time = 0.0;
}

MGGeneratorMUSUNInterface::~MGGeneratorMUSUNInterface()
{
  delete fTheMessenger;
  if (fInputFile.is_open()) fInputFile.close();
}

void MGGeneratorMUSUNInterface::OpenFile()
{

  fInputFile.open(fFileName);

  if (!(fInputFile.is_open())) {//not open correctly
    MGLog(error) << "File not valid!" << endlog;
    MGLog(fatal) << endlog;
 }
  MGLog(trace) << "MUSUN file " << fFileName << " initialized" << endlog;
}

void MGGeneratorMUSUNInterface::ChangeFileName(G4String newFile)
{
   if (fFileName != newFile) //check if the new file is equal to the other
    {
      if (fInputFile.is_open()) fInputFile.close(); //close the old file
      fFileName = newFile; 
      OpenFile(); //open the new one
    }
}

void MGGeneratorMUSUNInterface::GeneratePrimaryVertex(G4Event* evt)
{
  //The format of the file is:
  // eventNumber, particleID, energy (GeV), x(cm), y(cm), z(cm) theta (rad), phi (rad)

  if (!(fInputFile.is_open())) {
    MGLog(error) << "File must be set!" << endlog;
    MGLog(fatal) << endlog;
  }

  G4int nEvent=0;
  G4double time=0.0;
  G4double energy = 0.0*MeV;
  G4double px,py,pz;
  G4double theta,phi;
  G4double x, y, z; 
  G4int particleID = 0;

  fInputFile >> nEvent >> particleID >> energy >> x >> y >> z >> theta >> phi;
 
  if (fInputFile.eof())
    {
      fInputFile.close();
      MGLog(error) << "File over: not enough events!" << endlog;
      MGLog(fatal) << endlog;
      G4Exception("MGGeneratorMUSUNInterface::GeneratePrimaryVertex()", "err001", FatalException, "Exit MaGe");
      return;
    }
  
  particle_time = time*s;
  energy = energy*GeV;
  theta = theta*rad;
  phi = phi*rad;
  x = x * cm; 
  y = y * cm; 
  z = fZShift + (z * cm); 

  G4ThreeVector position(x, y, z); 

  particle_position = position; 

  MGLog(debugging) << "Primary coordinates: " << position/m << " m" << endlog;
  MGLog(debugging) << "Primary energy: " << energy/GeV << " GeV" << endlog;
  MGLog(debugging) << "Theta: " << theta/deg << " deg; Phi: " << phi/deg << " deg" << endlog;

  G4PrimaryVertex* vertex = new G4PrimaryVertex (particle_position,
  						 particle_time);  

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();

  G4String particleName = " ";
  
  if (particleID == 10) particleName = "mu+";
  else particleName = "mu-";
  
  G4double theMass = theParticleTable->FindParticle(particleName)->GetPDGMass();
  G4double totMomentum = std::sqrt(energy*energy+2*theMass*energy);
  pz = -1*totMomentum*std::cos(theta);
  px = totMomentum*std::sin(theta)*cos(phi);
  py = totMomentum*std::sin(theta)*sin(phi);
  

  //Create primary particle
  G4PrimaryParticle* particle = 
    new G4PrimaryParticle(theParticleTable->FindParticle(particleName),
			  px*MeV,py*MeV,pz*MeV);
     
  
  //Add particle to vertex
  vertex->SetPrimary(particle);
  
  evt->AddPrimaryVertex(vertex);
}

