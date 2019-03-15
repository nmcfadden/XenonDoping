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
// CLASS IMPLEMENTATION:  MGGeneratorLGNDLiquidArgon.cc
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
#include "G4OpticalPhoton.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"

#include "TMath.h"

#include "generators/MGGeneratorLGNDLiquidArgon.hh"
#include "generators/MGGeneratorLGNDLiquidArgonMessenger.hh"
#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_Cryostat.hh"
#include "bacongeometry/BACON_Baseline.hh"

//---------------------------------------------------------------------------//


//---------------------------------------------------------------------------//

using namespace CLHEP;

const G4double MGGeneratorLGNDLiquidArgon::LambdaE = twopi *1.973269602e-16 * m * GeV;

MGGeneratorLGNDLiquidArgon::MGGeneratorLGNDLiquidArgon()
{
	fGeneratorName = "LiquidArgon";
	fG4Messenger = new MGGeneratorLGNDLiquidArgonMessenger(this);
	fParticleGun = new G4ParticleGun(1);
}

MGGeneratorLGNDLiquidArgon::MGGeneratorLGNDLiquidArgon(const MGGeneratorLGNDLiquidArgon & other) : MGVGenerator(other)
{;}                                                                                       


MGGeneratorLGNDLiquidArgon::~MGGeneratorLGNDLiquidArgon() 
{
  delete fG4Messenger;
  delete fParticleGun;
}

void MGGeneratorLGNDLiquidArgon::DirectionDecider()
{
  G4double px = 2*G4UniformRand() -1;//-1 to 1
  G4double py = 2*G4UniformRand() -1;//-1 to 1
  G4double pz = 2*G4UniformRand() -1;//-1 to 1

  fDirection.setX(px);fDirection.setY(py);fDirection.setZ(pz);
}

void MGGeneratorLGNDLiquidArgon::EnergyDecider()
{
  if(fParticleType == "opticalphoton"){
    G4double waveL = 0;//from 115 nm to 136 nm
    //double sigma = 2.929,mean = 128.; //nm units get added later
    if(fSigma == 0)
      fSigma = 2.929;
    if(fMean == 0)
      fMean = 128.; //nm units get added later
    //BoxMuller transform
    waveL = (fMean/m+fSigma/m*sqrt(-2.0*log(G4UniformRand()))*cos(2*pi*G4UniformRand()))*nm;
    //Some materials don't have optical properties bellow 115*nm
    if(waveL < 115*nm) waveL = 115*nm;
    fCurrentEnergy = LambdaE/waveL;
  }
  else if(fEnergy > 0){
    //seems a bit redundant do do this here, but maybe you want to set a distribution or something more fancier-ish
    fCurrentEnergy = fEnergy;
  }
  else 
    MGLog(error)<<"Warned particle energy is <= 0"<<endlog;
}

void MGGeneratorLGNDLiquidArgon::PositionDecider()
{
  //get position of the inner cryostat volume
  //Get instance of cryostat200 for dimensions of interior
  //LGND_200_Cryostat * cryostat = new LGND_200_Cryostat("LGND200generatorCryostat","0101010");
  BACON_Baseline *cryostat = new BACON_Baseline("LGND200generatorCryostat");
  
  if(fInnerHeight ==0)
    fInnerHeight = cryostat->GetCryostatHeight()/2.;
  //can set fRadius in messenger
  if(fRadius == 0) 
    fRadius = cryostat->GetCryostatID()/2.;

  delete cryostat;
  //G4cout<<"fRadius "<<fRadius<<", fInnerHeight "<<fInnerHeight<<G4endl;
  G4ThreeVector rpos(1,1,1);
  G4bool isIn = false;
  int errorCounter = 0;
  //Generate random points in a bound box around inner_cryostat 
  do{
    //pdf_r = (2/R^2)*r
    G4double r = fRadius*std::sqrt(G4UniformRand());
    G4double rand = G4UniformRand();
    //pdf_r = -R/r^2
    //G4double r = fRadius*rand*rand;
    G4double theta = 2*pi*G4UniformRand();
    G4double z = fInnerHeight*(2*G4UniformRand() - 1);
    rpos.setRhoPhiZ(r,theta,z);
    rpos = rpos +fCenterVector;
    
    //check that point in in "Argon-Liq"
    isIn = IsInArgon(rpos);
    errorCounter++;
    if(errorCounter > 1e4){
      G4cout<<"errorCounter has exceeded 1e4 counts!"<<G4endl;
      MGLog(fatal)<< "errorCounter has exceeded 1e4 counts!"<<endlog;
    }
  }
  while(!isIn);

  MGLog(debugging)<<"Generator vertex "<<rpos<<endlog;
  fCurrentPosition = rpos;
}

G4bool MGGeneratorLGNDLiquidArgon::IsInArgon(G4ThreeVector rpos)
{
  bool isit = false;
  // list of all volumes
  G4PhysicalVolumeStore* theStore = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume *pVol = theStore->GetVolume("argonGasPhysical",false);
  G4VSolid *solid = pVol->GetLogicalVolume()->GetSolid();
  G4ThreeVector rtran  = pVol->GetTranslation();
  G4ThreeVector rel = rpos - rtran; // point relative to center of solid
  if(solid->Inside(rel)== EInside::kInside){
    //G4cout<<"Found point in solid "<<solid->DistanceToOut(rpos)<<" "<<solid->DistanceToIn(rpos)<<" "<<pVol->GetName()<<" "<<rtran<<" "<<rpos<<" "<<rel<<G4endl;
    isit=true;  
  }
  pVol = theStore->GetVolume("Detector",false);
  solid = pVol->GetLogicalVolume()->GetSolid();
  rtran  = pVol->GetTranslation();
  rel = rpos - rtran; // point relative to center of solid
  if(solid->Inside(rel)== EInside::kInside){
    //G4cout<<"Found point in solid "<<solid->DistanceToOut(rpos)<<" "<<solid->DistanceToIn(rpos)<<" "<<pVol->GetName()<<" "<<rtran<<" "<<rpos<<" "<<rel<<G4endl;
    isit=true;  
  }
  /*
  for(G4int istore = 0; istore< int(theStore->size()) ; ++istore ){
    G4VPhysicalVolume *pVol = theStore->at(istore);
    G4String sname = pVol->GetName();
    //every point is in the world volume
    if(sname == "World") continue;
    // if this is a volume to reject, see if it is inside
    G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();
    //if( sname != "Detector" && sname != "argonGasPhysical"){
    if(sname != ""){
      G4VSolid *solid = pVol->GetLogicalVolume()->GetSolid();
      G4ThreeVector rtran  = pVol->GetTranslation();
      G4ThreeVector rel = rpos - rtran; // point relative to center of solid
      if(solid->Inside(rel)== EInside::kInside){
        if(sname == "argonGasPhysical" || sname == "Detector"){
          isit = true;
          //G4cout<<"Found point in solid "<<solid->DistanceToOut(rpos)<<" "<<solid->DistanceToIn(rpos)<<" "<<pVol->GetName()<<" "<<rtran<<" "<<rpos<<" "<<rel<<G4endl;
        }
        else{
          isit = false;
          //Uncomment for debugging
          //G4cout<<solid->DistanceToOut(rpos)<<" "<<solid->DistanceToIn(rpos)<<" "<<pVol->GetName()<<" "<<rtran<<" "<<rpos<<" "<<rel<<G4endl;
        }
        break;
      }
    }
  }
  */
  return isit;
}

void MGGeneratorLGNDLiquidArgon::ParticleDecider()
{
  if(fParticleType == "opticalphoton"){
    fParticleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
  }
  else if(fParticleType == "gamma"){
    fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());
  }
  else if(fParticleType == "beta"){
    fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
  }
  else if(fParticleType == "alpha"){
    fParticleGun->SetParticleDefinition(G4Alpha::AlphaDefinition());
  }
  else {
    MGLog(error)<<"Unknow paticle type "<<fParticleType<<", if real particle type, add it to the code!"<<endlog;
  }
}

void MGGeneratorLGNDLiquidArgon::GeneratePrimaryVertex(G4Event *event)
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
  //G4cout<<"Particle type"<< fParticleType<<", Energy "<<fCurrentEnergy<<", Position "<<fCurrentPosition<<G4endl;
  //particle direction, position, and energy sent to ParticleGun
  fParticleGun->SetParticlePosition(fCurrentPosition);
  fParticleGun->SetParticleMomentumDirection(fDirection);
  fParticleGun->SetParticleEnergy(fCurrentEnergy);

  //vertex generated by ParticleGun
  fParticleGun->GeneratePrimaryVertex(event);
}
