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
// $Id: MGGeneratorTUNLFEL.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorTUNLFEL.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Aug  4 07:49:57 PDT 2004
 * 
 * REVISION:
 *
 * 08-04-2004, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

//#include <CLHEP/Random/RandGauss.h>
#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ThreeVector.hh"

#include "generators/MGGeneratorTUNLFELMessenger.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorTUNLFEL.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorTUNLFEL::MGGeneratorTUNLFEL()
{
  fGeneratorName = "TUNLFEL";
  fG4Messenger = new MGGeneratorTUNLFELMessenger(this);
  fParticleGun = new G4ParticleGun(1);
  SetDefaults();
}

//---------------------------------------------------------------------------//

MGGeneratorTUNLFEL::MGGeneratorTUNLFEL(const MGGeneratorTUNLFEL & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorTUNLFEL::~MGGeneratorTUNLFEL()
{
  delete fG4Messenger;
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorTUNLFEL::BeginOfRunAction(G4Run const*)
{
  if(fMajorSigma < fMinorSigma){
    G4double temp = fMajorSigma;
    fMajorSigma = fMinorSigma;
    fMinorSigma = temp;
    MGLog(warning) << "Minor sigma > Major Sigma, values swapped." << endlog;
  } 
  if(MGLogger::GetSeverity() <= MGLogger::routine)
    Dump();
}  

//---------------------------------------------------------------------------//

void MGGeneratorTUNLFEL::Dump()
{
  G4cout << "  TUNL FEL Parameters :" << G4endl;
  G4cout << "  -----------------------------------------" << G4endl;
  G4cout << "  Rho (degrees): " << fRho/deg << G4endl;
  G4cout << "  Major sigma (cm): " << fMajorSigma/cm << G4endl;
  G4cout << "  Minor sigma (cm): " << fMinorSigma/cm << G4endl;
  G4cout << "  Mean Energy (MeV): " << fMeanEnergy/MeV << G4endl;
  G4cout << "  Energy sigma (keV): " << fEnergySigma/keV << G4endl;
  G4cout << "  Origin (cm) : " << fOrigin/cm << G4endl;
  G4cout << "-------------------------------------------" << G4endl << G4endl;
}

//---------------------------------------------------------------------------//

void MGGeneratorTUNLFEL::EndOfRunAction(G4Run const*)
{;}

//---------------------------------------------------------------------------//

void MGGeneratorTUNLFEL::GeneratePrimaryVertex(G4Event *event)
{
  fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());
  fParticleGun->SetParticleMomentumDirection(fDirection);
  fCurrentPosition.set(G4RandGauss::shoot(0.0, fMajorSigma), 
		       G4RandGauss::shoot(0.0, fMinorSigma), 0.0);
  fCurrentPosition.rotateZ(fRho);
  fCurrentPosition += fOrigin;
  fParticleGun->SetParticlePosition(fCurrentPosition);
  fCurrentEnergy = G4RandGauss::shoot(fMeanEnergy, fEnergySigma);
  fParticleGun->SetParticleEnergy(fCurrentEnergy);
  fParticleGun->GeneratePrimaryVertex(event);
}
  
//---------------------------------------------------------------------------//

void MGGeneratorTUNLFEL::SetDefaults()
{
  fDirection.set(0.0, 0.0, -1.0);
  fMajorSigma = fMinorSigma = 0.6 * cm;
  fOrigin.set(0.0, 0.0, 10.0 * m);
  fRho = 0.0;
  fMeanEnergy = 2.2 * MeV;
  fEnergySigma = 22.0 * keV;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
