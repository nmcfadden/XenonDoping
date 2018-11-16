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
// 13 Dec 2004   L.Pandola    First implementation
// 13 Jan 2005   L.Pandola    Distinguish fission and (alpha,n) from muon-induced
// 24 Jan 2005   L.Pandola    Added distinction floor/wall/roof
// 25 Jan 2005   L.Pandola    Added messenger
// 26 Jan 2005   L.Pandola    Removed un-necessary methods, added switch fission/cosmogenics 

#ifndef MGGeneratorNeutronsAtGS_h
#define MGGeneratorNeutronsAtGS_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4Navigator.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

class MGGeneratorNeutronsAtGSMessenger;

class MGGeneratorNeutronsAtGS : public G4VPrimaryGenerator {

public:
  MGGeneratorNeutronsAtGS (); 
  ~MGGeneratorNeutronsAtGS ();
  void GeneratePrimaryVertex(G4Event *evt);

public:

  // position distribution  
  void SetHalfZ(G4double zhalf) {halfz = zhalf;};
  void SetRadius(G4double radius) {Radius = radius;};
  void SetSamplerInitialized(G4bool samp) {samplerInitialized = samp;};
  void SetMuonOrigin(G4String val);
  void SetFissionFlag(G4bool bol);

private: 
  // angular distribution
  void GenerateAngularSpectrum();

  // energy distribution 
  void ShootEnergyFission();
  void ShootEnergyMuons();

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
  G4bool samplerInitialized; 
  
  static const G4int nbin=22;
  static const G4int nbin_mu = 6;
  static const G4int nbin_ang = 21;

  G4double energybin[nbin];
  G4double userpdf[nbin]; //nbin

  //G4double energybin_mu[nbin_mu];
  G4double userpdf_mu[nbin_mu]; //nbin

  G4double userpdf_ang[nbin_ang]; //angular spectrum

  G4String neutronFrom;
  G4bool activateFission;

  // particle properties 
  G4int                  NumberOfParticlesToBeGenerated;
  G4ParticleDefinition*  particle_definition;
  G4ParticleMomentum     particle_momentum_direction;
  G4double               particle_energy;
  G4double               particle_charge;
  G4ThreeVector          particle_position;
  G4double               particle_time;
  G4ThreeVector          particle_polarization;
  
  MGGeneratorNeutronsAtGSMessenger* theMessenger;

};


#endif

