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
// $Id: MGGeneratorG4Gun.cc,v 1.10 2009-03-25 15:16:56 schubert Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorG4Gun.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 18:13:01 PDT 2004
 * 
 * REVISION:
 *
 * 07-08-2004, Created, R. Henning
 * 01-25-2006, Added methods and variables to generate particles 
 *             with direction isotropically sampled in a given cone around 
 *             a fixed direction, Luciano
 * 04-11-2006, Cone-sampling corrected, Luciano
 * 11-09-2006, Added methods and variables to generate particles with direction 
 *             sampled isotropically in a given cone pointing towards a fixed 
 *             point, Luciano 
 * 27.May 2008 fix a bug in the calculation of the momentum direction when using a cone to 
 *             limit particle direction (Jens)
 * 30.May 2008 add capability to sample the energy spectrum of the primary particle 
 *             according to an histogram read from file (and handled by 
 *             the class MGGeneratorReadSpectrumFile). Luciano
 * 25.Mar 2009 Jens Schubert, the generator needs to have a name
 * 10.Feb 2010 Fixed a bug of units when sampling particles pointing towards a
               cube around a fixed point (center of detector system). Alex.
 */
//---------------------------------------------------------------------------//
//

#include "G4Event.hh"
#include "G4ParticleGun.hh"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorG4Gun.hh"
#include "generators/MGGeneratorG4GunMessenger.hh"
#include "generators/MGGeneratorReadSpectrumFile.hh"
//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeneratorG4Gun::MGGeneratorG4Gun() : 
  fEnergySpectrumTypeGenerator(0)
{
  fGeneratorName = "G4Gun";
  fTheMessenger = new MGGeneratorG4GunMessenger(this);
  
  fParticleGun = new G4ParticleGun();
  if(!fParticleGun) {
    MGLog(error) << "Could not allocate G4ParticleGun! Out of memory?"<<endlog;
    MGLog(fatal) << endlog;
  }
  MGLog(routine) << "G4ParticleGun Constructed." << endlog;

  //Centric effect can be using a smearing vector or a cone
  turn_on_centric_effect = false ;
  fConeCentric = false; 
  xcenter=0.0*cm;
  ycenter=0.0*cm;
  zcenter=-75.0*cm;
  xyz_smear=30.0*cm;
  theta_centric = 0*deg; //no smearing

  fIsotropicConeFlag = false;
  //The default axis of the cone is the z-axis
  fConeAxisDirection.setX(0.);
  fConeAxisDirection.setY(0.);
  fConeAxisDirection.setZ(1.);

  //By default, it generates directions isotropically in the full space
  fConeDeltaTheta = pi;

  //Default energy spectrum is a line
  fEnergySpectrumFromFile = false;
}
//---------------------------------------------------------------------------//

MGGeneratorG4Gun::MGGeneratorG4Gun(const MGGeneratorG4Gun & other) : MGVGenerator(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorG4Gun::~MGGeneratorG4Gun()
{
  delete fTheMessenger;
  delete fParticleGun;
  if (fEnergySpectrumTypeGenerator)
    delete fEnergySpectrumTypeGenerator;
}

//---------------------------------------------------------------------------//

void MGGeneratorG4Gun::SetCenterPosition(G4ThreeVector center_vec)
{
  xcenter=center_vec.x();
  ycenter=center_vec.y();
  zcenter=center_vec.z();
}

//---------------------------------------------------------------------------//

void MGGeneratorG4Gun::GeneratePrimaryVertex(G4Event *event)
{
  //Shoot particles towards a given direction
  if (turn_on_centric_effect) 
    {
      G4double xpos = fParticleGun->GetParticlePosition().x();
      G4double ypos = fParticleGun->GetParticlePosition().y();
      G4double zpos = fParticleGun->GetParticlePosition().z();
      G4ThreeVector targetDirection = G4ThreeVector(xcenter-xpos, 
						    ycenter-ypos,
						    zcenter-zpos);
      // targetDirection.unit();  // <-- has no effect (Jens)
      MGLog(debugging) << "Target direction: " << targetDirection << endlog;
      G4ThreeVector direction;
      
      //First case: if fConeCentric = false, use the smearing
      if (!fConeCentric)
	{
	  G4double xdirection=targetDirection.x()+xyz_smear*(G4UniformRand()-0.5);
	  G4double ydirection=targetDirection.y()+xyz_smear*(G4UniformRand()-0.5);
	  G4double zdirection=targetDirection.z()+xyz_smear*(G4UniformRand()-0.5);
	  direction = G4ThreeVector(xdirection,ydirection,zdirection);
	  // direction.unit();  // <-- has no effect (Jens)
	}
      else //use a cone of given angle
	{
	  G4double cosThetaMin = std::cos(theta_centric);
      
	  //Now sample the azimuthal angle
	  G4double cosTheta = cosThetaMin+(1.0-cosThetaMin)*G4UniformRand();
	  G4double sinTheta = std::sqrt(1.0-cosTheta*cosTheta);
	  G4double phi = twopi*G4UniformRand();
	  
	  direction.setX(sinTheta*std::sin(phi));
	  direction.setY(sinTheta*std::cos(phi));
	  direction.setZ(cosTheta);
	  direction.rotateUz(targetDirection.unit());
	  MGLog(debugging) << "Theta: " << std::acos(cosTheta)/deg << endlog;;
	}
      fParticleGun->SetParticleMomentumDirection(direction);
      MGLog(debugging) << "Direction:" << fParticleGun->GetParticleMomentumDirection() << endlog;
  }

  //Shoot particles isotropically in a given cone
  if (fIsotropicConeFlag)
    {
      G4ThreeVector theDirection;
      
      //cos is a monothonical function
      G4double cosThetaMin = std::cos(fConeDeltaTheta);
      
      //Now sample the azimuthal angle
      G4double cosTheta = cosThetaMin+(1.0-cosThetaMin)*G4UniformRand();
      G4double sinTheta = std::sqrt(1.0-cosTheta*cosTheta);
      G4double phi = twopi*G4UniformRand();
      
      theDirection.setX(sinTheta*std::sin(phi));
      theDirection.setY(sinTheta*std::cos(phi));
      theDirection.setZ(cosTheta);

      MGLog(debugging) << fConeDeltaTheta/deg << " " << cosThetaMin << " " << 
	cosTheta << endlog;

      //Now rotate the direction with respect to the cone axis
      theDirection.rotateUz(fConeAxisDirection.unit());

      fParticleGun->SetParticleMomentumDirection(theDirection);
      MGLog(debugging) << "Direction:" << fParticleGun->GetParticleMomentumDirection() << endlog;
    }

  //Sample energy from the proper energy spectrum, if it is the case
  if (fEnergySpectrumFromFile)
    {
      if (!fEnergySpectrumTypeGenerator)
	{
	  fEnergySpectrumTypeGenerator = new MGGeneratorReadSpectrumFile();
	  MGLog(debugging) << "Instantiate MGGeneratorReadSpectrumFile in GeneratePrimaryVertex()" << endlog;
	}
      if (!fEnergySpectrumTypeGenerator) //still un-initialized = problem
	{
	  MGLog(error) << "Not able to initialize MGGeneratorReadSpectrumFile" << endlog;
	  G4Exception("MGGeneratorG4Gun::GeneratePrimaryVertex()", "err001", FatalException, "Exit MaGe");
	}
      G4double energy = fEnergySpectrumTypeGenerator->ShootEnergy();
      fParticleGun->SetParticleEnergy(energy);
    }
  fParticleGun->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
void MGGeneratorG4Gun::SetCentricEffectConeFlag(G4bool flag)
{
  fConeCentric = flag;
  if (!turn_on_centric_effect)
    {
      MGLog(warning) << "The centric effect is off. Your command will have no effect " << endlog;
      MGLog(routine) << "To activate the centric effect use the command " << endlog;
      MGLog(routine) << "/MG/generator/g4gun/centric_effect_on true " << endlog;
    }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
void MGGeneratorG4Gun::SetFileNameFromFile(G4String filename)
{
  if (!fEnergySpectrumTypeGenerator)
    {
      fEnergySpectrumTypeGenerator = new MGGeneratorReadSpectrumFile();
      MGLog(debugging) << "Instantiate MGGeneratorReadSpectrumFile in SetFileNameFromFile()" << endlog;
    }
  if (fEnergySpectrumTypeGenerator)
    fEnergySpectrumTypeGenerator->ChangeFileName(filename);
  else
    {
      MGLog(error) << "Not able to initialize MGGeneratorReadSpectrumFile" << endlog;
    }
  if (!fEnergySpectrumFromFile)
    {
      MGLog(warning) << "Please, set the energy spectrum from file flag" << endlog;
      MGLog(routine) << "to true, using the appropriate command: " << endlog;
      MGLog(routine) << "/MG/generator/g4gun/spectrum_from_file true " << endlog;
    }
}
