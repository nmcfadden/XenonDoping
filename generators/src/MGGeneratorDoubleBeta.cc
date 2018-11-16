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
// $Id: MGGeneratorDoubleBeta.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorDoubleBeta.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  This generator is an implementation of BB-decay event generation via the
 *  theoretical phase space factors and related single electron spectral
 *  calculations of J. Kotila and F. Iachello. The datafiles it uses are
 *  provided directly by Kotila & Iachello and can be downloaded here:
 *  http://nucleartheory.yale.edu/double-beta-decay-phase-space-factors
 *  Their work is published in the article "Phase-space Factors for Double-B
 *  Decay": J. Kotila and F. Iachello, Physical Review C 85, 034316 (2012) and 
 *  nucleartheory.yale.edu
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2016
 *
 * REVISION:
 *
 * 03-08-2016, Created, R. Massarczyk
 *
 */
//---------------------------------------------------------------------------//
//

//#include <CLHEP/Random/RandGauss.h>
#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Geantino.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"

#include "generators/MGGeneratorDoubleBetaMessenger.hh"
#include "io/MGLogger.hh"
#include <iostream>
#include <sstream>
#include <string>

#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "TMath.h"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorDoubleBeta.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorDoubleBeta::MGGeneratorDoubleBeta(G4int DecayType)
{
  NeutrinoNumber = DecayType;
  MGLog(routine) << "Double beta decay selected with " << NeutrinoNumber << " neutrinos" <<endlog;
  fGeneratorName = "DoubleBeta";
  fG4Messenger = new MGGeneratorDoubleBetaMessenger(this);
  fPosition = G4ThreeVector(0.0, 0.0, 0.0);
  LoadFiles();


  fParticleGun = new G4ParticleGun(1);

}

//---------------------------------------------------------------------------//

MGGeneratorDoubleBeta::MGGeneratorDoubleBeta(const MGGeneratorDoubleBeta & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorDoubleBeta::~MGGeneratorDoubleBeta()
{
  delete fG4Messenger;
  delete fParticleGun;
}


//---------------------------------------------------------------------------//

void MGGeneratorDoubleBeta::GeneratePrimaryVertex(G4Event *event)
{


  //choose electron1 energy  by random [0,1] and integral of distribution
  //choose electron2 energy by random [0,1] and integral of distribution (depends on E1)

  e1_int = G4UniformRand();
  for (e1_Ekin =0;e1_Ekin<2040;e1_Ekin++)
	if (singleE_int->GetBinContent(e1_Ekin+1)>e1_int) break;

  if (NeutrinoNumber == 0) e2_Ekin = 2039 - e1_Ekin;
  else if (NeutrinoNumber == 2){
	e2_int = G4UniformRand();
	for (e2_Ekin =0;e2_Ekin<2040;e2_Ekin++)
	  if (bothE_int->ProjectionY("",e1_Ekin,e1_Ekin)->GetBinContent(e2_Ekin+1)>e2_int) break;
  }
  if(!(e2_Ekin>0)) e2_Ekin = 0;

  //select angle
  e1e2_int = G4UniformRand();
  for(Int_t j = 0; j <200; j++){
	e1e2_angle = (j+0.5)/100.-1;
	if (prob_angle_int->ProjectionY("",e1_Ekin,e1_Ekin)->GetBinContent(j+1)>e1e2_int) break;
  }
  e1_ptot = sqrt(e1_Ekin*e1_Ekin+2*511*e1_Ekin);
  e2_ptot = sqrt(e2_Ekin*e2_Ekin+2*511*e2_Ekin);

  //random direction for e1
  e1_cosTheta = 2*G4UniformRand() - 1;
  e1_sinTheta = sqrt(1 - e1_cosTheta * e1_cosTheta);
  e1_phi = 2*pi*G4UniformRand();

  //direction e2 depends on e2
  do {
	e2_cosTheta = 2*G4UniformRand() - 1;
	e2_sinTheta = sqrt(1 - e2_cosTheta * e2_cosTheta);
    //c= b*sin phi + a*cos phi
	help_c = e1e2_angle - e1_cosTheta * e2_cosTheta;
	help_R = e1_sinTheta * e2_sinTheta;
	help_alpha =  e1_phi;
	e2_phi = TMath::ACos(help_c/help_R) + help_alpha;
  } while ((fabs(help_R)<fabs(help_c)));

  e1_px = e1_ptot * e1_sinTheta * TMath::Cos(e1_phi);
  e1_py = e1_ptot * e1_sinTheta * TMath::Sin(e1_phi);
  e1_pz = e1_ptot * e1_cosTheta;
  e2_px = e2_ptot * e2_sinTheta * TMath::Cos(e2_phi);
  e2_py = e2_ptot * e2_sinTheta * TMath::Sin(e2_phi);
  e2_pz = e2_ptot * e2_cosTheta;

  eSe_ptot = sqrt(pow(e1_px-e2_px,2) + pow(e1_py-e2_py,2) + pow(e1_pz-e2_pz,2));
  eSe_Ekin = (sqrt(eSe_ptot*eSe_ptot/70718299/70718299+1) - 1)* 70718299;

  //start electron 1
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(e1_px,e1_py,e1_pz));
  fParticleGun->SetParticleEnergy(e1_Ekin*keV);
  fParticleGun->SetParticlePosition(fPosition);
  fParticleGun->GeneratePrimaryVertex(event);
  //start electron 2
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(e2_px,e2_py,e2_pz));
  fParticleGun->SetParticleEnergy(e2_Ekin*keV);
  fParticleGun->SetParticlePosition(fPosition);
  fParticleGun->GeneratePrimaryVertex(event);

  //start the ion
  G4IonTable *theIonTable = (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition *aIon = theIonTable->GetIon(34,76,0);
  fParticleGun->SetParticleDefinition(aIon);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(e1_px-e2_px,e1_py-e2_py,e1_pz-e2_pz));
  fParticleGun->SetParticleEnergy(eSe_Ekin*keV);
  fParticleGun->SetParticlePosition(fPosition);
  fParticleGun->GeneratePrimaryVertex(event);

}

//---------------------------------------------------------------------------//

void MGGeneratorDoubleBeta::LoadFiles()
{

  MGLog(routine) << " Data files from http://nucleartheory.yale.edu/double-beta-decay-phase-space-factors" <<endlog;
  singleE->Reset();
  singleE_int->Reset();
  prob_angle->Reset();
  prob_angle_int->Reset();
  bothE->Reset();
  bothE_int->Reset();

  //check environment
  char* path = getenv("MAGEDIR");
  MGLog(debugging) << "MAGEDIR : " << path << endlog;
  if (!path){
    MGLog(error) << "!!!!!!!!! MAGEDIR environment variable not set!!!!!!!!!" << endlog;
    MGLog(fatal) << endlog;
    G4Exception(" ", "err001", FatalException, "Exiting program");
  }

  filenamestream.str(std::string());
  filenamestream << path << "/generators/data/" << "76Ge_" << NeutrinoNumber << "v_ses.txt";
  filename = filenamestream.str();
  in.open(filename.c_str(),std::ifstream::in);
  if (!in){
	MGLog(error) << "ERROR!!!! Could not open datafile" << endlog;
	MGLog(error) << "need to load single electron energy distribution from file : " << filename << endlog;
    MGLog(fatal) << endlog;
  }
  else{
	MGLog(routine) << " loaded single electron energy distribution from file : " << filename << endlog;
	while (in >> index >> energy >> quantity){
      singleE->Fill(index,quantity);
	 }
	in.close();
	//norm distribution and calculate integrated distribution
	singleE->Scale(1./singleE->Integral());
	for (int i = 0; i<2040;i++){
	  singleE_int->Fill(i,singleE->Integral(0,i));
	}
  }

  //read 2d energy correlation for 2vbb, for 0vbb calculate by using E1 + E2 = Q
  if (NeutrinoNumber == 2){
	filenamestream.str(std::string());
	filenamestream  << path << "/generators/data/" << "76Ge_" << NeutrinoNumber << "v_2ds.txt";
	filename = filenamestream.str();
	in.open(filename.c_str(),std::ifstream::in);
	if (!in){
	  MGLog(error) << "ERROR!!!! Could not open datafile" << endlog;
	  MGLog(error) << "need to load electron energy correlation from file : " << filename << endlog;
	  MGLog(fatal) << endlog;
	}
	else{
	  MGLog(routine) << " loaded electron energy correlation from file : " << filename << endlog;
	  while (in >> index >> index2 >> energy >> energy2 >> quantity){
		bothE->Fill(index,index2,quantity);
	  }
	  in.close();
	}
  }
  else if (NeutrinoNumber == 0){
	for (int i = 0; i<2040;i++){
	  bothE->Fill(i,2040-i,singleE->GetBinContent(i));
	}
  }
  //calculated integrated functions and normalize them for each energy
  for(Int_t i = 0; i <2040; i++){
	for(Int_t j = 0; j <2040; j++){
	  if (i==0) bothE_int->SetBinContent(i,j,bothE->GetBinContent(i,j));
	  else bothE_int->SetBinContent(i,j,bothE->GetBinContent(i,j)+ bothE_int->GetBinContent(i,j-1));
	}
  }
  for(Int_t i = 0; i <2040; i++){
	Double_t scale = bothE_int->ProjectionY("",i,i)->GetBinContent(bothE_int->ProjectionY("",i,i)->GetMaximumBin());
	if (scale == 0) continue;
	for(Int_t j = 0; j <2040; j++){
	  bothE_int->SetBinContent(i,j,bothE_int->GetBinContent(i,j)/scale);
	}
  }

  //read angular functions
  filenamestream.str(std::string());
  filenamestream << path << "/generators/data/" << "76Ge_" << NeutrinoNumber << "v_cor.txt";
  filename = filenamestream.str();
  in.open(filename.c_str(),std::ifstream::in);
  if (!in){
	  MGLog(error) << "ERROR!!!! Could not open datafile" << endlog;
	  MGLog(error) << "need to load angular correlation from file : " << filename << endlog;
	  MGLog(fatal) << endlog;
  }
  else{
	MGLog(routine) << " loaded angular correlation from file : " << filename << endlog;
	while (in >> index >> energy >> quantity){
	  for(Double_t angle = -1; angle <1; angle+=0.01){
		prob_angle->Fill(index,angle,(1+quantity*angle)/200.5);
	  }
	}
	in.close();
  }
  //calcualte integrated pdf
  for(Int_t i = 0; i <2040; i++){
	for(Int_t j = 0; j <200; j++){
	  if (i==0) prob_angle_int->SetBinContent(i,j,prob_angle->GetBinContent(i,j));
	  else prob_angle_int->SetBinContent(i,j,prob_angle->GetBinContent(i,j)+ prob_angle_int->GetBinContent(i,j-1));
	}
  }

}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
