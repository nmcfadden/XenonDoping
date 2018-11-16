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
// $Id: MGGeneratorGeneralSurfaceSampler.cc,v 1.3 2007-02-21 08:11:52 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorGeneralSurfaceSampler.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//

#include "Randomize.hh"
#include "G4Geantino.hh"

#include "generators/MGGeneratorGeneralSurfaceSampler.hh"
#include "generators/MGGeneratorGeneralSurfaceSamplerMessenger.hh"

#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"

using namespace CLHEP;

MGGeneratorGeneralSurfaceSampler::MGGeneratorGeneralSurfaceSampler()
{
  fMessenger = new MGGeneratorGeneralSurfaceSamplerMessenger(this);
  fOrigin.set(0, 0, 0);
  fBoundingRadius = 0.0;
}

void MGGeneratorGeneralSurfaceSampler::GeneratePrimaryVertex(G4Event* event)
{
  if(fBoundingRadius == 0.0) {
    fBoundingRadius = MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetWorldHalfLength();
  }

  // start on z-axis, pointing down.
  G4ThreeVector pos(0.0, 0.0, fBoundingRadius);
  G4ThreeVector dir(0.0, 0.0, -1.0);

  // push in rho direction by some impact parameter
  double diskPhi = 2.0*pi*G4UniformRand();
  double diskR = sqrt(G4UniformRand())*fBoundingRadius;
  G4ThreeVector push(cos(diskPhi)*diskR, sin(diskPhi)*diskR, 0);
  pos += push;

  // now rotate pos and dir by some random direction
  double theta = acos(2.0*G4UniformRand() - 1.0);
  double phi = 2.0*pi*G4UniformRand();
  pos.rotateY(theta);
  pos.rotateZ(phi);
  dir.rotateY(theta);
  dir.rotateZ(phi);

  // pos has to be pushed now in case the volumes are not centered at (0,0,0)
  pos += fOrigin;

  fParticleGun.SetParticleDefinition(G4Geantino::Geantino());
  fParticleGun.SetParticleEnergy(1.0*GeV); // doesn't matter, as long as it has some energy
  fParticleGun.SetParticlePosition(pos);
  fParticleGun.SetParticleMomentumDirection(dir);
  fParticleGun.GeneratePrimaryVertex(event);
}


void MGGeneratorGeneralSurfaceSampler::SetOrigin(const G4ThreeVector& origin)
{ 
  fOrigin = origin; 
  MGLog(routine) << "GSS origin set to " << origin.x()/cm << ' ' 
                 << origin.y()/cm << ' ' << origin.z()/cm << " cm" 
                 << endlog;
}


void MGGeneratorGeneralSurfaceSampler::SetBoundingRadius(G4double radius)
{ 
  fBoundingRadius = radius;
  MGLog(routine) << "GSS bounding radius set to " << radius/cm << " cm " << endlog;
}
