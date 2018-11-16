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
// $Id: MGGeneratorShowersFromFile.cc,v 1.5 2007-02-21 09:31:33 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorG4Gun.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: This File should be used in combination with the muon simulation made
 *                for SouthDakota.
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: 2015

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
#include "G4Neutron.hh"
#include "G4Alpha.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4VPhysicalVolume.hh"

#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagementEventAction.hh"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <string>

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorShowersFromFile.hh"
#include "generators/MGGeneratorShowersFromFileMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorShowersFromFile::MGGeneratorShowersFromFile()
{
  fileName = "Muonbackground.root"; //default fileName
  theMessenger = new MGGeneratorShowersFromFileMessenger(this);
  G4ThreeVector zero;
  particle_position = zero;
  MuonsOnly = false;
}

//---------------------------------------------------------------------------//

MGGeneratorShowersFromFile::MGGeneratorShowersFromFile(const MGGeneratorShowersFromFile & other) : G4VPrimaryGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorShowersFromFile::~MGGeneratorShowersFromFile()
{
  delete theMessenger;
}



//---------------------------------------------------------------------------//

void MGGeneratorShowersFromFile::GeneratePrimaryVertex(G4Event *event)
{
  //open file
  G4String filename = GetFileName();
  char* path = getenv("MGGENERATORDATA");
  MGLog(debugging) << "MGGenerator : " << path << endlog;

  if (!path)
    {
      MGLog(error) << "!!!!!!!!! MGGENERATORDATA environment variable not set!!!!!!!!!" << endlog;
      MGLog(fatal) << endlog;
      G4Exception("MGGeneratorShowersFromFile::OpenDataFile()", "err001", FatalException, "Exiting program");
    }
  else
   {
     MGLog(debugging) << "Using shower data: " << filename << endlog;
    }

  G4String pathString(path);
  //G4String pathFile = pathString + "/" + filename;
  G4String pathFile = "./" + filename;
  MGLog(debugging) << "Pathfile is : " << pathFile << endlog;
  std::ifstream infile(pathFile);
  if (!infile)
    {
      MGLog(error) << "ERROR!!!! Could not open datafile" << endlog;
      MGLog(fatal) << endlog;
    }
  infile.close();
  max_entries = 0;
  TFile *inputfile= TFile::Open(pathFile,"READ");
  TTree *datatree = (TTree*)inputfile->Get("tree");

  max_entries = datatree->GetEntries();


  //data structure

   Int_t 		   n = 0;
   Double_t        start_energy = 0;	  //muon start energy in surface GeV
   Double_t        start_costheta = 0;    //muon start angle
   std::vector<double>  *particle_energy = 0;
   std::vector<double>  *particle_momentumX = 0;  //GeV
   std::vector<double>  *particle_momentumY = 0;  //GeV
   std::vector<double>  *particle_momentumZ = 0;  //GeV
   std::vector<double>  *particle_positionX = 0;  //m 0,0 is yates
   std::vector<double>  *particle_positionY = 0;  //m 0,0 is yates
   std::vector<double>  *particle_positionZ = 0;  //m 0,0 is yates
   std::vector<std::string>  *particle_name = 0;

   TBranch        *b_n;   //!
   TBranch        *b_start_energy;   //!
   TBranch        *b_start_costheta;   //!
   TBranch        *b_particle_energy;   //!
   TBranch        *b_particle_momentumX;   //!
   TBranch        *b_particle_momentumY;   //!
   TBranch        *b_particle_momentumZ;   //!
   TBranch        *b_particle_positionX;   //!
   TBranch        *b_particle_positionY;   //!
   TBranch        *b_particle_positionZ;   //!
   TBranch        *b_particle_name;   //!

   datatree->SetBranchAddress("n", &n, &b_n);
   datatree->SetBranchAddress("start_energy", &start_energy, &b_start_energy);
   datatree->SetBranchAddress("start_costheta", &start_costheta, &b_start_costheta);
   datatree->SetBranchAddress("particle_energy", &particle_energy, &b_particle_energy);
   datatree->SetBranchAddress("particle_momentumX", &particle_momentumX, &b_particle_momentumX);
   datatree->SetBranchAddress("particle_momentumY", &particle_momentumY, &b_particle_momentumY);
   datatree->SetBranchAddress("particle_momentumZ", &particle_momentumZ, &b_particle_momentumZ);
   datatree->SetBranchAddress("particle_positionX", &particle_positionX, &b_particle_positionX);
   datatree->SetBranchAddress("particle_positionY", &particle_positionY, &b_particle_positionY);
   datatree->SetBranchAddress("particle_positionZ", &particle_positionZ, &b_particle_positionZ);
   datatree->SetBranchAddress("particle_name", &particle_name, &b_particle_name);

  ev_ID = (int)(G4UniformRand()*max_entries);

  datatree->GetEntry(ev_ID);
  inputfile->Close();  //close file

  MGLog(routine) << ev_ID << " of "  << max_entries<< " shower with " << n << " particles "
                 << MuonsOnly<< " weight "<<Distribution(start_energy,start_costheta)<< endlog;
  //2) Now the data have been loaded. Let's generate the event

  //transform my coordinates to MJGeometryDemonstrator Cavern position
  // in m
  //ralph 0,0 = -62.6425 ,  96.5094, rotated 10 degrees
  //mjd   0,0 =   4.6478 , -16.5611, rotated 0 degrees


  //for all shower particles
  for (size_t j=0; j<particle_energy->size(); j++) {
	particle_time = 0.0*s;

	//shift to zero, rotate, shift to MJD
	double x_MJD =    (particle_positionX->at(j) + 62.6425)*0.984807753 + (particle_positionY->at(j) - 96.5094)*0.173648177;
	       x_MJD+=4.6478;
	double y_MJD = -1*(particle_positionX->at(j) + 62.6425)*0.173648177 + (particle_positionY->at(j) - 96.5094)*0.984807753;
	       y_MJD-=16.5611;
	double z_MJD = particle_positionZ->at(j) + 1466.99+1.6764;



    particle_position.setX(x_MJD*m);
	//particle_position.setX(0*m); //just for testing
    particle_position.setY(y_MJD*m);
    //particle_position.setY(0*m); //just for testing
    particle_position.setZ(z_MJD*m);

	MGLog(debugging) << particle_positionX->at(j) << " "
				   << particle_positionY->at(j) << " "
				   << particle_positionZ->at(j) << " "
				   << "changed to "
				   << particle_position/m << endlog;

	G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);

	G4ParticleDefinition* particle_definition = 0;
	if (!(particle_name->at(j).compare("mu-")))     particle_definition = G4MuonMinus::MuonMinusDefinition();
	if (MuonsOnly && !particle_definition) continue;
	if (!(particle_name->at(j).compare("neutron"))) particle_definition = G4Neutron::NeutronDefinition();
	if (!(particle_name->at(j).compare("alpha")))   particle_definition = G4Alpha::AlphaDefinition();
	if (!(particle_name->at(j).compare("gamma")))   particle_definition = G4Gamma::GammaDefinition();
	if (!(particle_name->at(j).compare("e-")))      particle_definition = G4Electron::ElectronDefinition();
	if (!(particle_name->at(j).compare("e+")))      particle_definition = G4Positron::PositronDefinition();
	if (particle_definition == 0) continue;


	// rotate momentum
    double px_MJD =    particle_momentumX->at(j)*0.984807753 + particle_momentumY->at(j)*0.173648177;
    // px_MJD =    0; //just for testing
	double py_MJD = -1*particle_momentumX->at(j)*0.173648177 + particle_momentumY->at(j)*0.984807753;
	// py_MJD =    0; //just for testing
	double pz_MJD = particle_momentumZ->at(j);
	G4ThreeVector momentum(px_MJD*GeV,py_MJD*GeV,pz_MJD*GeV);

	MGLog(debugging) << "Eventnumber: " << ev_ID << endlog;
    MGLog(debugging) << "Particle: " << particle_definition->GetParticleName() << endlog;
    MGLog(debugging) << "Position " << particle_position/m << " m " << endlog;
    MGLog(debugging) << "Momentum direction " << momentum << endlog;
    MGLog(debugging) << "Energy " << particle_energy->at(j)/GeV << " GeV" << endlog;

	G4PrimaryParticle* thePrimaryParticle =
    new G4PrimaryParticle(particle_definition,
						  px_MJD*GeV,
						  py_MJD*GeV,
						  pz_MJD*GeV);
	vertex->SetPrimary(thePrimaryParticle);
	vertex->SetWeight(Distribution(start_energy,start_costheta));


    event->AddPrimaryVertex(vertex);
  }
}
//---------------------------------------------------------------------------//

void MGGeneratorShowersFromFile::SetFileName(G4String stri)
{
  fileName = stri; //if different, assign the new name
  return;
}
//---------------------------------------------------------------------------//

void MGGeneratorShowersFromFile::OnlyMuons(G4bool boo)
{
  MuonsOnly = boo;
  if (boo) MGLog(routine) <<  "Only muons are simulated " << endlog;
  return;
}

//---------------------------------------------------------------------------//

//weighting with incident muon distribution
//https://escholarship.org/uc/item/6jm8g76d#page-3
Double_t MGGeneratorShowersFromFile::Distribution(Double_t Energy, Double_t CosTheta)
{
  Double_t CosThetaStar = sqrt(
	(pow(CosTheta,2) + pow(0.102573,2) -0.068287*pow(CosTheta,0.958633)+0.0407253*pow(CosTheta,0.817285) )/
	(1+pow(0.102573,2)-0.068287+0.0407253));

  Double_t I;
  I = 0.14*pow(Energy*(1+3.64/(Energy*pow(CosThetaStar,1.29))),-2.7)
	  *((1./(1+(1.1*Energy*CosThetaStar)/115.))+(0.054/(1+(1.1*Energy*CosThetaStar)/850.)));

  return (I);

}
