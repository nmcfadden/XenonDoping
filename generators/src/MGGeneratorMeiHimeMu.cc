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
// $Id: MGGeneratorMeiHimeMu.cc,v 1.10 2007-12-11 07:57:04 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorMeiHimeMu
//
//
// This generator is based on Mei and Hime's paper on muons and 
// muon-induced neutrons.  It has the option of generating either
// just muons, or just the muon-induced neutons described by equations
// in section C of the paper.  See Physical Review D 73 053004 (2006) for 
// further details.
//
// Converted from LUXSimGeneratorMASN by the LUX collaboration (thank you guys).
//
//---------------------------------------------------------------------------//

#include "generators/MGGeneratorMeiHimeMu.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "Randomize.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

MGGeneratorMeiHimeMu::MGGeneratorMeiHimeMu()
{
  fMaxImpactPar = 10.*m;
  fVertDepth = 1.95; // SURF

  fDirectory = new G4UIdirectory("/MG/generator/MeiHimeMu/");
  fDirectory->SetGuidance("Control of Mei-Hime muon event generator");

  fVertDepthCmd = new G4UIcmdWithADouble("/MG/generator/MeiHimeMu/vertDepth_kmwe", this);
  fVertDepthCmd->SetGuidance("set vertical depth in kmwe");
  fVertDepthCmd->SetParameterName("h", false);

  fOriginCmd = new G4UIcmdWith3VectorAndUnit("/MG/generator/MeiHimeMu/origin", this);
  fOriginCmd->SetGuidance("Set origin for muon position distribution.");
  fOriginCmd->SetParameterName("X", "Y", "Z", false);
  fOriginCmd->SetUnitCategory("Length");

  fMaxBCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/MeiHimeMu/maxImpactParameter", this);
  fMaxBCmd->SetGuidance("Sets the maximum impact parameter for the muon vertex distribution");
  fMaxBCmd->SetParameterName("R", false);
  fMaxBCmd->SetUnitCategory("Length");
}


MGGeneratorMeiHimeMu::~MGGeneratorMeiHimeMu()
{
  delete fDirectory;
  delete fVertDepthCmd;
  delete fOriginCmd;
  delete fMaxBCmd;
}


void MGGeneratorMeiHimeMu::GeneratePrimaryVertex( G4Event *event )
{
  MGLog(debugging) << "Entering GeneratePrimaryVertex" << endlog;

  // Get the Muon angle:
  MGLog(debugging) << "Generating muon angle" << endlog;
  G4double h_0 = fVertDepth; //km.w.e
  G4double I_1 = 8.6e-6;     //s^-1 cm^-2 sr^-1
  G4double I_2 = 0.44e-6;    //s^-1 cm^-2 sr^-1
  G4double lambda_1 = 0.45;  //km.w.e
  G4double lambda_2 = 0.87;  //km.w.e
  G4double pdf[kN];
  G4double area = 0.;
  for(int i=0; i<kN; i++){
    fMuonAngle[i] = pi/2.*i/(kN-1);
    G4double sectheta = 1./cos(fMuonAngle[i]);
    pdf[i] = (I_1*exp(-h_0*sectheta/lambda_1)+ I_2*exp(-h_0*sectheta/lambda_2))*sectheta;
    if(i != kN-1) area += pdf[i];
  }
  fMuonAngleCDF[0] = 0;
  pdf[0] /= area;
  for(int i=1; i<kN; i++) {
    pdf[i] /= area;
    fMuonAngleCDF[i] = fMuonAngleCDF[i-1] + pdf[i-1];
  }
  MGLog(debugging) << "first angle cdf entry = " << fMuonAngleCDF[0] << endlog;
  MGLog(debugging) << "final angle cdf entry = " << fMuonAngleCDF[kN-1] << endlog;
  G4double muonAngle = SampleMuonAngle();
  G4ThreeVector muonDir;
  muonDir.setRThetaPhi(1., muonAngle, G4UniformRand()*2.*pi);
  muonDir *= -1;

  // Get the muon energy:
  MGLog(debugging) << "Generating muon energy" << endlog;
  G4double h = h_0/cos(muonAngle); //km.w.e
  G4double b = 0.4;		      //km.w.e^{-1}
  G4double gamma_mu = 3.77;
  G4double epsilon_mu = 693.;	      //GeV
  area = 0;
  for(int i=0; i<kN; i++){
    fMuonEnergy[i] = 4.*i*GeV;
    pdf[i] = exp(-b*h*(gamma_mu-1)) * pow(fMuonEnergy[i]/GeV + epsilon_mu*(1-exp(-b*h)),-gamma_mu);
    if(i != kN-1) area += pdf[i];
  }
  fMuonEnergyCDF[0] = 0;
  pdf[0] /= area;
  for(int i=1; i<kN; i++){
    pdf[i] /= area;
    fMuonEnergyCDF[i] = fMuonEnergyCDF[i-1] + pdf[i-1];
  }
  MGLog(debugging) << "first energy cdf entry = " << fMuonEnergyCDF[0] << endlog;
  MGLog(debugging) << "final energy cdf entry = " << fMuonEnergyCDF[kN-1] << endlog;
  double kineticE = SampleMuonEnergy();

  // make the muon
  MGLog(debugging) << "making muon" << endlog;
  G4ParticleDefinition* muonDef;
  if(G4UniformRand() < 0.5) muonDef = G4MuonPlus::MuonPlusDefinition(); // original was 0.65?!?
  else muonDef = G4MuonMinus::MuonMinusDefinition();
  G4double mass = muonDef->GetPDGMass();
  G4double totalE = kineticE + mass;
  G4double mom = sqrt(totalE*totalE - mass*mass);
  G4PrimaryParticle* muon = new G4PrimaryParticle(muonDef, mom*muonDir.x(), mom*muonDir.y(), mom*muonDir.z());

  // generate a vertex
  MGLog(debugging) << "generating vertex" << endlog;
  particle_position.setRhoPhiZ(sqrt(G4UniformRand())*fMaxImpactPar, G4UniformRand()*2.*pi, -fMaxImpactPar);
  particle_position.rotateUz(muonDir);
  particle_position += fOrigin;
  particle_time = 0;
  G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position, particle_time);
  vertex->SetPrimary(muon);
  MGLog(trace) << "vertex generated at = " << particle_position/m << endlog;

  // finally, add the primary vertex
  event->AddPrimaryVertex(vertex);
  MGLog(debugging) << "GeneratePrimaryVertex done" << endlog;
}


G4double MGGeneratorMeiHimeMu::SampleObservable(G4double vals[kN], G4double cdf[kN])
{
  G4double prob = G4UniformRand();
  G4int indexLo = 0, indexHi = kN-1;

  while(!(cdf[indexLo+1] > prob && cdf[indexHi-1] < prob) ) {
    if(cdf[(indexLo+indexHi)/2] < prob ) indexLo = (indexLo + indexHi)/2;
    else indexHi = (indexLo + indexHi)/2;
  }
  G4double split = (prob - cdf[indexLo]) / (cdf[indexHi] - cdf[indexLo]);
  G4double value = vals[indexLo] + split*(vals[indexHi] - vals[indexLo]);
  return value;
}

void MGGeneratorMeiHimeMu::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fVertDepthCmd) {
    SetVerticalDepth_kmwe(fVertDepthCmd->GetNewDoubleValue(newValues));
  }
  else if (command == fOriginCmd) {
    SetOrigin(fOriginCmd->GetNew3VectorValue(newValues));
  }
  else if (command == fMaxBCmd) {
    SetMaxImpactParameter(fMaxBCmd->GetNewDoubleValue(newValues));
  }
}

