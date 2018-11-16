//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MGGeneratorWangNeutrons.cc,v 1.8 2007-03-29 22:25:37 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorWangNeutrons.cc
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
 * AUTHOR: K.Hudek
 * CONTACT: 
 * FIRST SUBMISSION: Thu Jun 16 09:29:25 PDT 2005
 * 
 * REVISION:
 *
 * 10-24-2005, Various optimizations. R. Henning
 * 10-17-2005, Removed boundX from 
 *             MGGeneratorWangNeutrons::SetLinearPiecewiseTransformationVars() 
 *             to replace with fMinNeutronEnergy that is settable by messenger,
 *             R. Henning
 * 09-07-2005, First Submission, R. Henning for K. Hudek. Some bugfixes.
 *
 */
//---------------------------------------------------------------------------//
//

#include "generators/MGGeneratorWangNeutronsMessenger.hh"

//---------------------------------------------------------------------------//

//#include <CLHEP/Random/RandFlat.h>
#include "Randomize.hh"
#include <math.h>

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Neutron.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "G4Timer.hh"

#include "TROOT.h"

#include "generators/MGGeneratorWangNeutrons.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MGGeneratorWangNeutrons::MGGeneratorWangNeutrons()
{
  fGeneratorName = "wangneutrons";
  fG4Messenger = new MGGeneratorWangNeutronsMessenger(this);
  fParticleGun = new G4ParticleGun(1);
  fTotalTimer = new G4Timer();
  fTimer = new G4Timer();
  SetDefaults();
}

//---------------------------------------------------------------------------//

MGGeneratorWangNeutrons::MGGeneratorWangNeutrons(const MGGeneratorWangNeutrons & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorWangNeutrons::~MGGeneratorWangNeutrons()
{
  delete fG4Messenger;
  delete fParticleGun;
  delete fTotalTimer;
  delete fTimer;
  CloseLinearPiecewiseTransformation();
  CloseRootFile();
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::BeginOfEventAction(G4Event*)
{;}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::BeginOfRunAction(G4Run const*)
{
    CalculateMinMuonSurfaceEnergy();//Resets fMinMuonEnergy once
    if(fHistoFile != " ")
      InitializeRootFile(); //Initializes hFile and defines histograms once
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::Dump()
{
  G4cout << "  Wang Neutron Parameters: " << G4endl;
  G4cout << "  -----------------------------------------" << G4endl;
  G4cout << "  Muon surface energy (GeV): " << fMuonSurfaceEnergy/GeV <<G4endl;
  G4cout << "  Max muon energy (GeV): " << fMaxMuonEnergy/GeV << G4endl;
  G4cout << "  Min muon energy (GeV): " << fMinMuonEnergy/GeV << G4endl;
  G4cout << "  Current neutron energy (MeV): " << fCurrentEnergy/MeV << G4endl;
  G4cout << "  Current position (cm): " << fCurrentPosition/cm << G4endl;
  G4cout << "  Muon direction: " << fMuonDirection << G4endl;
  G4cout << "  Neutron direction: " << fDirection << G4endl;
  G4cout << "  Neutron multiplicity: " << fMultiplicity << G4endl;
  G4cout << "  Depth (m) : " << fDepth/cm << G4endl;
  G4cout << "  Origin (cm) : " << fOrigin/cm << G4endl;
  G4cout << "  Cave bounds on current position (cm) x: " << fCaveX/cm
	 << "  y: " << fCaveY/cm << G4endl;
  G4cout << "  Min muon energy cutoff set?: " << fMinMuonEnergyBool << G4endl;
  G4cout << "  Root file initialized?: " << fInitializeRootFileBool << G4endl;
  G4cout << "-------------------------------------------" << G4endl << G4endl;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::EndOfRunAction(G4Run const*)
{
  if(fHistoFile != " ")
    WriteRootFile();
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::GeneratePrimaryVertex(G4Event *event)
{
  fEventNumber++;
  fParticleGun->SetParticleDefinition(G4Neutron::NeutronDefinition());

  fWangCosPhiCount = 0;
  fWangEnergyCount = 0; 
  fWangMultiplicityCount = 0; 
  fMuonSurfaceEnergyAndAngleCount = 0;
  fWangCosPhiRate = 0.0;
  fWangEnergyRate = 0.0; 
  fWangMultiplicityRate = 0.0; 
  fMuonSurfaceEnergyAndAngleRate = 0.0;

  SampleCosmicMuonDirectionAndEnergy();
  SampleWangNeutronEnergy();
  SampleWangNeutronDirection();

  //  SampleWangNeutronMultiplicity();//sets fMultiplicity
  // Removed multiplicity sampling for now. We can use it later.
  // R. Henning, 09-07-2005. 

  fParticleGun->SetParticleMomentumDirection(fDirection);
  G4double x = G4UniformRand()*fCaveX - fCaveX/2.;//RandFlat::shoot(-fCaveX/2.,fCaveX/2.);
  G4double y = G4UniformRand()*fCaveY - fCaveY/2.;//RandFlat::shoot(-fCaveY/2.,fCaveY/2.);
  fCurrentPosition.set(x, y, 0.0); //sets fCurentPosition, start point 
                                   //to a random X,Y and Z=0
  fCurrentPosition += fOrigin;
  fParticleGun->SetParticlePosition(fCurrentPosition);
  fParticleGun->SetParticleEnergy(fCurrentEnergy);

  fTotalTimer->Stop();
  fTotalRate = fTotalTimer->GetSystemElapsed();

  if(fHistoFile != " ")
    FillRootFile();

  fParticleGun->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::SetDefaults()
{
  fEventNumber = 0;
  fErrorNumber = 0;
  fOrigin.set(0.0, 0.0, 10.0*m); //need to know where the origin will be?!
  fDepth = 0.0 * m;
  fMuonDepth = 0.0 * m;
  fMuonCosPhi = 1.0;
  fMaxMuonEnergy = 20000 * GeV;
  fMinMuonEnergy = 1000 * GeV;
  fMinNeutronEnergy = 2 * MeV;
  fNeutronEnergyCutoff = 2 * GeV;
  fNeutronEnergyCutoffMax = 2 * GeV;
  fCaveX = 10.0 * m;
  fCaveY = 10.0 * m;
  fP = 21;
  fAN = 1000;
  fInitializeRootFileBool = false;
  fInitializeLinearPiecewiseTransformationBool = false;
  fHistoFile = " ";
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//Cosmic muon distribution sampler functions
//Physical Letters B
//Volume 592, Issues 1-4, July 2004
//Article 24, pp. 228-234

inline void MGGeneratorWangNeutrons::CalculateMinMuonSurfaceEnergy()
{
  G4double testMuonEnergy = GetCosmicMuonDepthAttenuation(fDepth, fMinMuonEnergy);
  //2 MeV to the power of 27 is O(134 TeV), which gives an accuracy of 0.2 MeV
  //if fMaxMuonEnergy is O(20) TeV
  if(testMuonEnergy < 0) {
    for(G4int i = 1; i < 27; i++){
      if(testMuonEnergy < 0)
	fMinMuonEnergy += fMaxMuonEnergy/pow(2,i);
      if(testMuonEnergy > 0)
	fMinMuonEnergy -= fMaxMuonEnergy/pow(2,i);
      testMuonEnergy = GetCosmicMuonDepthAttenuation(fDepth, fMinMuonEnergy);
    }
  }
  if(testMuonEnergy < 0)
    fMinMuonEnergy += 1*MeV;

  fMaxMuonDepthEnergy = GetCosmicMuonDepthAttenuation(fDepth, fMaxMuonEnergy);
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::SampleCosmicMuonDirectionAndEnergy()
{
  //specifies angular distribution of the momentum direction of the muon
  //phi is the azimuthal angle
  G4double maxMuonFlux = GetCosmicMuonFlux(fMinMuonEnergy, 0.3);

  G4double randMuonEnergy = 0.0;
  G4double randCosPhi = 0.0, randCosTheta = 0.0;
  G4double randMuonFlux = 0.0;
  G4double cosmicMuonFlux = 0.0;

  //second do-while sets rand muon energy and phi angle
  //first do-while samples proper muon spectra and rejects zero events
  do {
    do {
      fMuonSurfaceEnergyAndAngleCount++;
      randCosPhi = G4UniformRand()*.7 + 0.3;//RandFlat::shoot(0.3,1.0);
      randMuonEnergy = G4UniformRand()*(fMaxMuonEnergy - fMinMuonEnergy) + fMinMuonEnergy;//RandFlat::shoot(fMinMuonEnergy,fMaxMuonEnergy);
      randMuonFlux = G4UniformRand()*maxMuonFlux;//RandFlat::shoot(0.0,maxMuonFlux);
      cosmicMuonFlux = GetCosmicMuonFlux(randMuonEnergy, randCosPhi);
      randCosTheta = G4UniformRand()*2.0 - 1.0;//RandFlat::shoot(-1.0,1.0);
    }while(randMuonFlux > cosmicMuonFlux);
    if(GetCosmicMuonDepthAttenuation(fDepth/randCosPhi,randMuonEnergy) <= 0)
      continue;
    else
      break;
  }while(true);

  G4double sinPhi = sqrt(1.0 - randCosPhi*randCosPhi);
  G4double sinTheta = sqrt(1.0 - randCosTheta*randCosTheta);

  fMuonSurfaceEnergy = randMuonEnergy;
  fMuonDepthEnergy=GetCosmicMuonDepthAttenuation(fDepth/randCosPhi,randMuonEnergy);
  fMuonDirection.set(randCosTheta*sinPhi, sinTheta*sinPhi, -randCosPhi);
  fMuonCosPhi = randCosPhi;
}

//---------------------------------------------------------------------------//

//Physical review letters equation 24.5
inline G4double MGGeneratorWangNeutrons::GetCosmicMuonFlux(G4double muonEnergy, G4double cosAngle)
{
  //Make sure the input muon energy is in MeV
  return 0.14*pow((muonEnergy/GeV),-2.7)*
                         (1/(1+(1.1*muonEnergy/GeV*cosAngle)/115)
	                  +0.054/(1+(1.1*muonEnergy/GeV*cosAngle)/850));
}

//---------------------------------------------------------------------------//

//depth attenuator: physical review letters equation 24.7
inline G4double MGGeneratorWangNeutrons::GetCosmicMuonDepthAttenuation(G4double depth, G4double surfaceMuonEnergy)
{
  //Make sure the input muon energy is in MeV and depth in m.w.e.
  G4double a = 2.93*1e-3*GeV;
  G4double b = 4.35*1e-6;
  G4double epsilon = a/b;

  //depth (m.w.e) is in units of m, change to cm
  return (surfaceMuonEnergy+epsilon)*exp(-b*depth/cm)-epsilon;
}

//End of cosmic muon calculations
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//Wang neutron distribution sampler functions
//Physical Review D, Volume 64, 013012
//Predicting neutron production from cosmic-ray muons
//Wang et al.

void MGGeneratorWangNeutrons::SampleWangNeutronEnergy()
{
  if(!fInitializeLinearPiecewiseTransformationBool){
    InitializeLinearPiecewiseTransformation();
    fInitializeLinearPiecewiseTransformationBool = true;
  }

  SetLinearPiecewiseTransformationVars();

  fS = 0;
  G4double sumArea = 0.0;
  G4double randArea = G4UniformRand()*fTA;//RandFlat::shoot(0.0, fTA);
  for(G4int i = 0; i < fP - 1; i++){
    sumArea += fA[i];
    if(randArea < sumArea){
      fS = i;
      break;
    }
  }

  G4double randNeutronEnergy = 0.0;
  G4double randNeutronFlux = 0.0;
  G4double wangNeutronFlux = 0.0;
  do
{
    fWangEnergyCount++;
    //fD=-(1/2*fM[fS]*fX[fS]*fX[fS]+fC[fS]*fX[fS]+fN[fS]*RandFlat::shoot())/GeV;
    fD=-(1/2*fM[fS]*fX[fS]*fX[fS]+fC[fS]*fX[fS]+fN[fS]*G4UniformRand())/GeV;
    randNeutronEnergy = GeV*(-fC[fS]+sqrt(fC[fS]*fC[fS]-2*fM[fS]*fD))/fM[fS];
    if((randNeutronEnergy>fNeutronEnergyCutoff)||(randNeutronEnergy<0))
      continue; //this is because the transformation isn't quite perfect
    randNeutronFlux = G4UniformRand()*(fY[fS] - fY[fP]) + fY[fP];//RandFlat::shoot(fY[fP],fY[fS]);
    wangNeutronFlux = GetWangNeutronEnergyFlux(randNeutronEnergy, fMuonDepthEnergy);
}while(randNeutronFlux > wangNeutronFlux);

  fCurrentEnergy = randNeutronEnergy;
  fCurrentNflux = wangNeutronFlux/fY[0];
}

//---------------------------------------------------------------------------//

inline void MGGeneratorWangNeutrons::InitializeLinearPiecewiseTransformation()
{
  //Neutron energy limits
  fX = new G4double[fP];
  //Area for weighting interpolated from lookup table
  fA = new G4double[fP - 1];
  //Generating lookup table of fAN(default 1000) muon energies
  fAL = new G4double*[fAN+1];
  G4double boundX, dx;
  G4double tempMuonEnergy;
  for(G4int k = 0; k < fAN+1; k++){
    *(fAL+k) = new G4double[fP-1];
    tempMuonEnergy = fMaxMuonDepthEnergy*k/fAN;
    fNeutronEnergyCutoff = fNeutronEnergyCutoffMax;
    CalculateNeutronEnergyCutoff(tempMuonEnergy);
    boundX = 20*MeV;
    for(G4int i = 0; i < fP; i++){
      fX[i] = boundX;
      boundX += (fNeutronEnergyCutoff - 20*MeV)/(fP-1);
    }
    for(G4int i = 0; i < fP - 1; i++){
      fAL[k][i] = 0.0;
      for(G4int j = (int)(fX[i]); j <= (int)(fX[i+1]); j++) {
	dx = j; //put dx in 1 MeV intervals
	for(G4int a = 0; a < 10; a++) {
	  dx += a/10; //put dx in .1 MeV intervals
	  fAL[k][i]+=GetWangNeutronEnergyFlux((double)(dx),fMuonDepthEnergy)
	    *.1; //take the Riemann sum style integral
	}
      }
    }
  }
  //LinearPiecewise limits
  fY = new G4double[fP];
  //Slope
  fM = new G4double[fP - 1];
  //LinearPiecewise intercept
  fC = new G4double[fP - 1];
  //Normalization factor
  fN = new G4double[fP - 1];
}


//---------------------------------------------------------------------------//

inline void MGGeneratorWangNeutrons::SetLinearPiecewiseTransformationVars()
{
  CalculateNeutronEnergyCutoff(fMuonDepthEnergy);
  G4double boundX = fMinNeutronEnergy;
  for(G4int i = 0; i < fP; i++){
    fX[i] = boundX;
    fY[i] = GetWangNeutronEnergyFlux(fX[i],fMuonDepthEnergy);
    boundX += (fNeutronEnergyCutoff - 20.0*MeV)/(G4double)(fP-1);
  }

  //Finds where to interpolated muon energy from
  G4int k = (int)((fMuonDepthEnergy/fMaxMuonDepthEnergy)*fAN); //rounds down
  G4double x = (fMaxMuonDepthEnergy*(k+1)/fAN - fMuonDepthEnergy)
            /(fMaxMuonDepthEnergy*(k+1)/fAN-fMaxMuonDepthEnergy*k/fAN);
  fTA = 0.0;
  for(G4int i = 0; i < fP - 1; i++){
    fM[i] = (fY[i + 1] - fY[i])/(fX[i + 1] - fX[i]);
    fC[i] = fY[i] - (fX[i] - fX[0])*fM[i];
    fN[i] = 1/2*fM[i]*(fX[i+1]*fX[i+1]-fX[i]*fX[i])+fC[i]*(fX[i+1]-fX[i]);
    //% from lower bound * lower bound + % from upper bound * upper bound
    fA[i] = x*fAL[k][i] + (1-x)*fAL[k+1][i];
    fTA += fA[i];
  }
}

//---------------------------------------------------------------------------//

inline void MGGeneratorWangNeutrons::CloseLinearPiecewiseTransformation()
{
  delete fX;
  delete fY;
  delete fM;
  delete fC;
  delete fA;
  delete fAL;
  delete fN;
}

//---------------------------------------------------------------------------//

//Equations 6 and 7 in Wang et. al.
inline G4double MGGeneratorWangNeutrons::GetWangNeutronEnergyFlux(
				   G4double neutronEnergy, G4double muonEnergy)
{
  //assuming the function is in units of GeV
  return  exp(-7.0*(neutronEnergy/GeV))/(neutronEnergy/GeV)
			 +(0.52-0.58*exp(-0.0099*(muonEnergy/GeV)))
		         *exp(-2.0*(neutronEnergy/GeV));
}

//---------------------------------------------------------------------------//

inline void MGGeneratorWangNeutrons::CalculateNeutronEnergyCutoff(G4double muonEnergy)
{
  fNeutronEnergyCutoff = fNeutronEnergyCutoffMax;
  G4double tempWangNeutronFlux = GetWangNeutronEnergyFlux(fNeutronEnergyCutoff, muonEnergy);

  if(tempWangNeutronFlux < 0) {
    for(G4int i = 1; i < 13; i++){
      if(tempWangNeutronFlux < 0)
	fNeutronEnergyCutoff -= fNeutronEnergyCutoffMax/pow(2,i);
      if(tempWangNeutronFlux > 0)
	fNeutronEnergyCutoff += fNeutronEnergyCutoffMax/pow(2,i);
      tempWangNeutronFlux = GetWangNeutronEnergyFlux(fNeutronEnergyCutoff, muonEnergy);
    }
  }
  if(tempWangNeutronFlux < 0)
    fNeutronEnergyCutoff -= 1*MeV;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::SampleWangNeutronDirection()
{
  //specifies angular distribution of the momentum direction of the neutron
  //phi is the azimuthal angle
  G4double maxNeutronFlux= GetWangNeutronAngularFlux(fMuonDepthEnergy,1.0);

  G4double wangNeutronFlux;
  G4double randCosPhi, randCosTheta;
  G4double randNeutronFlux;

  do {
    randNeutronFlux = G4UniformRand()*maxNeutronFlux;//RandFlat::shoot(0.0,maxNeutronFlux);
    randCosPhi = G4UniformRand()*2.0 - 1.0;//RandFlat::shoot(-1.0,1.0);
    randCosTheta = G4UniformRand()*2.0 - 1.0;//RandFlat::shoot(-1.0,1.0);
    wangNeutronFlux=GetWangNeutronAngularFlux(fMuonDepthEnergy,randCosPhi);
    fWangCosPhiCount++;
  } while(randNeutronFlux > wangNeutronFlux);

  G4double sinPhi = sqrt(1.0 - randCosPhi*randCosPhi);
  G4double sinTheta = sqrt(1.0 - randCosTheta*randCosTheta);

  //sets fDirection to the Wang angular distribution
  //fDirection = neutron direction + muon direction 
  //           = direction w.r.t. horizon
  fDirection.set(randCosTheta*sinPhi, sinTheta*sinPhi, -randCosPhi);
  fDirection.rotateUz(fMuonDirection);
  fWangCosPhi = randCosPhi;
}

//---------------------------------------------------------------------------//
  
//Equation 12 in Wang et al
inline G4double MGGeneratorWangNeutrons::GetWangNeutronAngularFlux(
				    G4double muonEnergy, G4double cosAngle)
{
  //Make sure energy input is in MeV
  G4double neutronFlux = 1/(pow((1-cosAngle),0.6)
			    +0.699*pow((muonEnergy/GeV),-0.136));
  return neutronFlux;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons::SampleWangNeutronMultiplicity()
{
  G4double maxNeutronFlux = GetWangNeutronMultiplicityFlux(fMuonDepthEnergy,0.001);
  G4double maxNeutronMultiplicity = 100;
  G4double randNeutronMultiplicity = 0.0;
  G4double randNeutronFlux = 0.0;
  G4double wangNeutronFlux = 0.0;

  do
  {
    randNeutronMultiplicity = G4UniformRand()*maxNeutronMultiplicity;//RandFlat::shoot(0.0,maxNeutronMultiplicity);
    randNeutronFlux = G4UniformRand()*maxNeutronFlux;//RandFlat::shoot(0.0,maxNeutronFlux);
    wangNeutronFlux = GetWangNeutronMultiplicityFlux(fMuonDepthEnergy, randNeutronMultiplicity);
    fWangMultiplicityCount++;
  }while(randNeutronFlux > wangNeutronFlux);

  fMultiplicity = randNeutronMultiplicity;
}

//---------------------------------------------------------------------------//

//Equations 8, 9, 10, 11 in Wang et al
inline G4double MGGeneratorWangNeutrons:: GetWangNeutronMultiplicityFlux(
		            G4double muonEnergy, G4double neutronMultiplicity)
{
  //Make sure energy input is in GeV
  G4double A = 0.085+0.54*exp(-0.075*muonEnergy/GeV);
  G4double B = 27.2/(1+7.2*exp(-0.076*muonEnergy/GeV));
  G4double C = 0.67+1.4*exp(-0.12*muonEnergy/GeV);

  G4double neutronFlux = exp(-A*neutronMultiplicity)
                         +B*exp(-C*neutronMultiplicity);
  return neutronFlux;
}

//End of wang neutron calculations
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//"Sanity check" root files containing histograms containing the generated
//spectra

void MGGeneratorWangNeutrons:: InitializeRootFile()
{
  hFile = (TFile*)gROOT->FindObject(fHistoFile);
  if (hFile) hFile->Close();
  hFile = new TFile(fHistoFile, "RECREATE", "Test generator outputs");
  MGLog(routine) << "Opened ROOT file for histogram data: " << fHistoFile << endlog;

  InitializeSpectra();
  InitializeSampleCount();
  InitializeSampleRate();
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: InitializeSpectra()
{  
  G4double neutronMultiplicityCutoff = 100;

  hWangEnergy = new TH1F("hWangEnergy","Neutron energy distribution",1000,0,fNeutronEnergyCutoffMax/GeV);
  hWangEnergyWang = new TH2F("hWangEnergyWang","Neutron energy-flux calculated distribution at non-constant muon energy",1000,0,fNeutronEnergyCutoffMax/GeV,100,0,1);

  hWangCosPhi = new TH1F("hWangCosPhi","Neutron angular distribution",1000,-1,1);
  hWangMultiplicity = new TH1F("hWangMultiplicity","Neutron multiplicity distribution",1000,0,neutronMultiplicityCutoff);
  hMuonDepthEnergy = new TH1F("hMuonDepthEnergy","Attenuated muon energy distribution",1000,0,fMaxMuonDepthEnergy/GeV);
  hMuonSurfaceEnergy = new TH1F("hMuonSurfaceEnergy","Sampled muon surface energy distribution",1000,0.0,fMaxMuonEnergy/GeV);
  hMuonCosPhi = new TH1F("hMuonCosPhi","Sampled muon surface angular distribution",100,0.3,1);
  hMuonEnergyAndNeutronEnergy = new TH2F("hMuonEnergyAndNeutronEnergy","Muon surface energy and neutron energy flux distribution",1000,fMinMuonEnergy/GeV,fMaxMuonEnergy/GeV,1000,0,fNeutronEnergyCutoff/GeV);
  hMuonEnergyAndCosAngle = new TH2F("hMuonEnergyAndCosAngle","Muon surface energy and cos anglular flux distribution",1000,fMinMuonEnergy/GeV,fMaxMuonEnergy/GeV,1000,0.3,1.0);

  hWangEnergy->SetXTitle("Neutron Energy (GeV)");
  hWangEnergy->SetYTitle("Number of events");
  hWangEnergyWang->SetXTitle("Neutron Energy (GeV)");
  hWangEnergyWang->SetYTitle("Number of events");
  hWangCosPhi->SetXTitle("Cos(Phi)");
  hWangCosPhi->SetYTitle("Number of events");
  hWangMultiplicity->SetXTitle("Multiplicity");
  hWangMultiplicity->SetYTitle("Number of events");
  hMuonDepthEnergy->SetXTitle("Muon Energy (GeV)");
  hMuonDepthEnergy->SetYTitle("Number of events");
  hMuonSurfaceEnergy->SetXTitle("Muon Surface Energy (GeV)");
  hMuonSurfaceEnergy->SetYTitle("Number of events");
  hMuonCosPhi->SetXTitle("Cos(Phi)");
  hMuonCosPhi->SetYTitle("Number of events");
  hMuonEnergyAndNeutronEnergy->SetXTitle("Muon Energy(GeV)");
  hMuonEnergyAndNeutronEnergy->SetYTitle("Neutron Energy (GeV)");
  hMuonEnergyAndCosAngle->SetXTitle("Muon Energy (GeV)");
  hMuonEnergyAndCosAngle->SetYTitle("Cos(Theta)");
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: InitializeSampleCount()
{
  hTotalCount  = new TH1I("hTotalCount", "Total iterations to generate an event", 350, 0, 3500);
  hWangEnergyCount = new TH1I("hWangEnergyCount", "Number of iterations to generate a 'hit' for fWangEnergyCount", 100, 0, 100);
  hWangCosPhiCount = new TH1I("hWangCosPhiCount", "Number of iterations to generate a 'hit' for fWangCosPhiCount", 50, 0, 50);
  hWangMultiplicityCount = new TH1I("hWangMultiplicityCount", "Number of iterations to generate a 'hit' for fWangMultiplicityCount", 200, 0, 2000);
  hMuonSurfaceEnergyAndAngleCount = new TH1I("hMuonSurfaceEnergyAndAngleCount", "Number of iterations to generate a 'hit' for hMuonSurfaceEnergyAndAngleCount", 300, 0, 3000);

  hWangEnergyCount->SetXTitle("Neutron Energy (GeV) count");
  hWangEnergyCount->SetYTitle("Number of events");
  hWangCosPhiCount->SetXTitle("Cos(Phi) count");
  hWangCosPhiCount->SetYTitle("Number of events");
  hWangMultiplicityCount->SetXTitle("Multiplicity count");
  hWangMultiplicityCount->SetYTitle("Number of events");
  hMuonSurfaceEnergyAndAngleCount->SetXTitle("Muon Surface Energy(GeV) count");
  hMuonSurfaceEnergyAndAngleCount->SetYTitle("Number of events");
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: InitializeSampleRate()
{
  hTotalRate = new TH1F("hTotalRate", "Number of seconds to generate an event", 1000, 0, 1e-5);
  hWangEnergyRate = new TH1F("hWangEnergyRate", "Number of seconds to generate a 'hit' for fWangEnergyRate", 1000, 0, 2e-7);
  hWangCosPhiRate = new TH1F("hWangCosPhiRate", "Number of seconds to generate a 'hit' for fWangCosPhiRate", 1000, 0, 2e-7);
  hWangMultiplicityRate = new TH1F("hWangMultiplicityRate", "Number of seconds to generate a 'hit' for fWangMultiplicityRate", 1000, 0, 2e-7);
  hMuonSurfaceEnergyAndAngleRate = new TH1F("hMuonSurfaceEnergyAndAngleRate", "Number of seconds to generate a 'hit' for fMuonSurfaceEnergyAndAngleRate", 1000, 0, 2e-7);

  hWangEnergyRate->SetXTitle("Neutron Energy (GeV) rate");
  hWangEnergyRate->SetYTitle("Number of events");
  hWangCosPhiRate->SetXTitle("Cos(Phi) rate");
  hWangCosPhiRate->SetYTitle("Number of events");
  hWangMultiplicityRate->SetXTitle("Multiplicity rate");
  hWangMultiplicityRate->SetYTitle("Number of events");
  hMuonSurfaceEnergyAndAngleRate->SetXTitle("Muon Surface Energy (GeV) rate");
  hMuonSurfaceEnergyAndAngleRate->SetYTitle("Number of events");
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: FillRootFile()
{
  FillSpectra();
  FillSampleCount();
  FillSampleRate();
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: FillSpectra()
{
  hWangEnergy->Fill(fCurrentEnergy/GeV);
  hWangEnergyWang->Fill(fCurrentEnergy/GeV, fCurrentNflux);
  hWangMultiplicity->Fill(fMultiplicity);
  hWangCosPhi->Fill(fWangCosPhi);
  hMuonSurfaceEnergy->Fill(fMuonSurfaceEnergy/GeV);
  hMuonDepthEnergy->Fill(fMuonDepthEnergy/GeV);
  hMuonCosPhi->Fill(fMuonCosPhi);
  hMuonEnergyAndNeutronEnergy->Fill(fMuonSurfaceEnergy/GeV,fCurrentEnergy/GeV);
  hMuonEnergyAndCosAngle->Fill(fMuonSurfaceEnergy/GeV,fMuonCosPhi,1.0);//???
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: FillSampleCount()
{
  hWangEnergyCount->Fill(fWangEnergyCount);
  hWangCosPhiCount->Fill(fWangCosPhiCount);
  hWangMultiplicityCount->Fill(fWangMultiplicityCount);
  hMuonSurfaceEnergyAndAngleCount->Fill(fMuonSurfaceEnergyAndAngleCount);
  G4int Total = fWangEnergyCount + fWangCosPhiCount + fWangMultiplicityCount
                + fMuonSurfaceEnergyAndAngleCount;
  hTotalCount->Fill(Total);
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: FillSampleRate()
{
  hWangEnergyRate->Fill(fWangEnergyRate);
  hWangCosPhiRate->Fill(fWangCosPhiRate);
  hWangMultiplicityRate->Fill(fWangMultiplicityRate);
  hMuonSurfaceEnergyAndAngleRate->Fill(fMuonSurfaceEnergyAndAngleRate);
  hTotalRate->Fill(fTotalRate);
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: WriteRootFile()
{
  hFile->Write();
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: CloseRootFile()
{
  CloseSpectra();
  CloseSampleCount();
  CloseSampleRate();
  delete hFile;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: CloseSpectra()
{
  delete hWangEnergy;
  delete hWangEnergyWang;
  delete hWangCosPhi;
  delete hWangMultiplicity;
  delete hMuonDepthEnergy;
  delete hMuonSurfaceEnergy;
  delete hMuonCosPhi;
  delete hMuonEnergyAndNeutronEnergy;
  delete hMuonEnergyAndCosAngle;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: CloseSampleCount()
{
  delete hTotalCount;
  delete hWangEnergyCount;
  delete hWangCosPhiCount;
  delete hWangMultiplicityCount;
  delete hMuonSurfaceEnergyAndAngleCount;
}

//---------------------------------------------------------------------------//

void MGGeneratorWangNeutrons:: CloseSampleRate()
{
  delete hTotalRate;
  delete hWangEnergyRate;
  delete hWangCosPhiRate;
  delete hWangMultiplicityRate;
  delete hMuonSurfaceEnergyAndAngleRate;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
