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
// History:
// --------
// 28 Oct 2004   L.Pandola    First implementation
// 02 Dec 2004   L.Pandola    Working implementation; added energy and angular spectrum
// 08 Feb 2005   L.Pandola    Added angular spectrum read from file. Some new methods and members

#ifndef MGGeneratorCosmicRayMuons_h
#define MGGeneratorCosmicRayMuons_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4Navigator.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"
#include "G4DataVector.hh"
#include "globals.hh"
#include "Randomize.hh"

#include "generators/MGGeneratorCosmicRayMuonsMessenger.hh"

class MGGeneratorCosmicRayMuons : public G4VPrimaryGenerator {

public:
  MGGeneratorCosmicRayMuons (); 
  ~MGGeneratorCosmicRayMuons ();
  void GeneratePrimaryVertex(G4Event *evt);

public:

  // position distribution  
  void SetHalfZ(G4double zhalf) {halfz = zhalf;};
  void SetRadius(G4double radius) {Radius = radius;};
  void SetSpectralIndex(G4double ind) {spectralIndex = ind;};
  void SetRockDepth(G4double rock) {rockDepth = rock;};
  void SetSamplerInitialized(G4bool samp) {samplerInitialized = samp;};
  void SetFileName(G4String stri) {fileName = stri;};
  void SetEnergySup(G4double ene) {energysup = ene;};
  void SetEnergyInf(G4double ene) {energyinf = ene;};
  G4String GetFileName() {return fileName;};

private: 
  // angular distribution
  void GenerateAngularSpectrum();

  // energy distribution 
  void ShootEnergy();

  // initial position
  void SampleInitialPosition();

public:
  //Get methods
  inline G4double GetParticleEnergy() {return particle_energy;}
  inline G4String GetParticleName() {return particle_definition->GetParticleName();};
  
  //Set methods
  void SetParticleDefinition(G4ParticleDefinition * aParticleDefinition);
  inline void SetParticleCharge(G4double aCharge)
  { particle_charge = aCharge; }
  
private:
  // position distribution
  G4double halfz;
  G4double Radius;

   // energy sampling
  static const G4int nbin=1000;
  G4double userpdf[nbin]; //nbin
  G4double spectralIndex;
  G4double rockDepth;
  G4bool samplerInitialized; 
  G4double energysup;
  G4double energyinf;

  //angular sampling
  G4DataVector* angularpdf;
  G4DataVector* costheta_pdf;
  G4DataVector* phi_pdf;
  G4int nx;
  G4int ny;
  G4String fileName; 
  G4bool fileFound;

  // particle properties 
  G4int                  NumberOfParticlesToBeGenerated;
  G4ParticleDefinition*  particle_definition;
  G4ParticleMomentum     particle_momentum_direction;
  G4double               particle_energy;
  G4double               particle_charge;
  G4ThreeVector          particle_position;
  G4double               particle_time;
  G4ThreeVector          particle_polarization;


  G4double MuonSpectrum(G4double ene);

private:
  MGGeneratorCosmicRayMuonsMessenger *theMessenger;
  G4Navigator *gNavigator;
  
};


#endif

