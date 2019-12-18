//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id:
//      
// CLASS IMPLEMENTATION:  MGGeneratorBACoNSurfaceMuons.cc
//
//---------------------------------------------------------------------------//
/**
* SPECIAL NOTES:
*/
// 
//---------------------------------------------------------------------------//
/**
* AUTHOR: Neil McFadden
* CONTACT: nmcfadde@unm.edu
 * FIRST SUBMISSION: 
* 
* REVISION:
*
*
*/
//---------------------------------------------------------------------------//

#include "Randomize.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Alpha.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"

#include "TMath.h"

#include "generators/MGGeneratorBACoNSurfaceMuons.hh"
#include "generators/MGGeneratorBACoNSurfaceMuonsMessenger.hh"
#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_Cryostat.hh"
#include "bacongeometry/BACON_Baseline.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "TH2D.h"
#include "TFile.h"
//---------------------------------------------------------------------------//


//---------------------------------------------------------------------------//

using namespace CLHEP;
const G4double MGGeneratorBACoNSurfaceMuons::inch = 2.54*cm;

const G4double MGGeneratorBACoNSurfaceMuons::LambdaE = twopi *1.973269602e-16 * m * GeV;

MGGeneratorBACoNSurfaceMuons::MGGeneratorBACoNSurfaceMuons()
{
	fGeneratorName = "BACoNSurfaceMuons";
	fG4Messenger = new MGGeneratorBACoNSurfaceMuonsMessenger(this);
	fParticleGun = new G4ParticleGun(1);
  //Generate above scint panels
  //Center of scint panel is placed 4" above top of outer can,
  //panels are 2" thick, 
  //thus muon show is generated 0.1" above plain defined by panels
  fZ = 32.41/2*inch + 8*inch;
  //fRadiusMax = 22.750/2*inch;
  fRadiusMax = 22.750/2*inch;
  //geometry initialization
  fE0 = 4.0 *GeV; //GeV
  //orginal cut
  //fECut = 0.05 *GeV;// GeV
  //Maybe the cut is to high?!
  fECut = 0.001 *GeV;// GeV

}

MGGeneratorBACoNSurfaceMuons::MGGeneratorBACoNSurfaceMuons(const MGGeneratorBACoNSurfaceMuons & other) : MGVGenerator(other)
{;}                                                                                       


MGGeneratorBACoNSurfaceMuons::~MGGeneratorBACoNSurfaceMuons() 
{
  delete fG4Messenger;
  delete fParticleGun;
}

void MGGeneratorBACoNSurfaceMuons::DirectionDecider()
{

  G4double costheta = 1.0 - pow( G4UniformRand(), 1.0/3.0); //co-latidude
  G4double phi = G4UniformRand()*360*degree;
  G4double sintheta = sqrt(1.0-costheta*costheta);
  fDirection.setX(cos(phi)*costheta);
  fDirection.setY(sin(phi)*costheta);
  fDirection.setZ(-1.0*sintheta);

}

void MGGeneratorBACoNSurfaceMuons::EnergyDecider()
{
  fEnergy = ( (fE0+fECut)*pow(1.-G4UniformRand(),-0.5)-fE0 ); 
}

void MGGeneratorBACoNSurfaceMuons::PositionDecider()
{
  /*
  //Positions are generated uniformly over circular area 
  G4double r     = fRadiusMax*sqrt(G4UniformRand());
  G4double phi   = twopi * G4UniformRand();
  fCurrentPosition.setX(r*cos(phi));
  fCurrentPosition.setY(r*sin(phi));
  fCurrentPosition.setZ(fZ);
  */
  //Generate over top Scint panel plus some small extra volume
  G4double extra = 2*inch;
  G4double xMin = 8.5/2*inch + extra;
  G4double yMin = 18.0/2*inch + extra;  

  G4double x = 2*xMin*G4UniformRand() - xMin;
  G4double y = 2*yMin*G4UniformRand() - yMin;
  fCurrentPosition.setX(x);
  fCurrentPosition.setY(y);
  fCurrentPosition.setZ(fZ);
  
}

G4bool MGGeneratorBACoNSurfaceMuons::IsInArgon(G4ThreeVector rpos)
{
  bool isit = false;
  //This is how Geant4 suggests you should randomly generate a point in a volume
  G4ThreeVector myPoint = rpos;
  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(myPoint);
  if(myVolume->GetName() == "Detector") isit = true;
  if(myVolume->GetName() == "argonGasPhysical") isit = true;

  return isit;
}

void MGGeneratorBACoNSurfaceMuons::ParticleDecider()
{
  if (G4UniformRand() > 0.5) 
    fParticleGun->SetParticleDefinition( G4MuonMinus::MuonMinusDefinition());
  else
    fParticleGun->SetParticleDefinition( G4MuonPlus::MuonPlusDefinition());
}

void MGGeneratorBACoNSurfaceMuons::GeneratePrimaryVertex(G4Event *event)
{

    fParticleGun->SetParticlePolarization(G4ThreeVector(2*G4UniformRand()-1,2*G4UniformRand()-1,2*G4UniformRand()-1 ) );

    //what is the particle
    ParticleDecider();
    //determine particle momentum direction
    DirectionDecider();

    //determine particle position  
    PositionDecider();
    //determine particle energy
    EnergyDecider();

    //particle direction, position, and energy sent to ParticleGun
    fParticleGun->SetParticlePosition(fCurrentPosition);
    //G4cout<<"("<<fCurrentPosition.x()<<","<<fCurrentPosition.y()<<","<<fCurrentPosition.z()<<")"<<G4endl;
    fParticleGun->SetParticleMomentumDirection(fDirection);
    fParticleGun->SetParticleEnergy(fEnergy);
    fParticleGun->SetNumberOfParticles(fNParticles);

    //vertex generated by ParticleGun
    fParticleGun->GeneratePrimaryVertex(event);

}
