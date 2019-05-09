//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
// March  6, 2019 M. Gold
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
// March  6, 2019 M. Gold
// cosmic ray muons at earth's surface reference https://arxiv.org/pdf/1606.06907.pdf
// vertical flux above 50 MeV is 89.54/m^2/Sr/second
// total flux is vertical times 2*pi/3
// 
#ifndef MGGeneratorCosmicSurfMuons_h
#define MGGeneratorCosmicSurfMuons_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4Navigator.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"
#include "G4DataVector.hh"
#include "globals.hh"
#include "Randomize.hh"

#include "generators/MGGeneratorCosmicSurfMuonsMessenger.hh"

class MGGeneratorCosmicSurfMuons : public G4VPrimaryGenerator {

public:
  MGGeneratorCosmicSurfMuons (); 
  ~MGGeneratorCosmicSurfMuons ();
  void GeneratePrimaryVertex(G4Event *evt);

public:
  // position distribution  
  void SetHalfZ(G4double zhalf) {halfz = zhalf;};
  void SetRadius(G4double radius) {Radius = radius;};

private: 
  // angular distribution
  void GenerateAngularSpectrum();

  // energy distribution 
  void GetEnergy();

  // initial position
  void SampleInitialPosition();

public:
  //Get methods
  inline G4double GetParticleEnergy() {return particle_energy;}
  inline G4String GetParticleName() {return particle_definition->GetParticleName();};
  
  //Set methods
  void SetParticleDefinition(G4ParticleDefinition * aParticleDefinition);
  inline void SetParticleCharge(G4double aCharge){ 
    particle_charge = aCharge; 
  }
  
private:
  // position distribution
  G4double halfz;
  G4double Radius;
  G4double E0;
  G4double ECut;

  // particle properties 
  G4int                  NumberOfParticlesToBeGenerated;
  G4ParticleDefinition*  particle_definition;
  G4ParticleMomentum     particle_momentum_direction;
  G4double               particle_energy;
  G4double               particle_charge;
  G4ThreeVector          particle_position;
  G4double               particle_time;
  G4ThreeVector          particle_polarization;

private:
  MGGeneratorCosmicSurfMuonsMessenger *theMessenger;
  G4Navigator *gNavigator;
};


#endif

