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
/**                                                            
 * $Id: MGGeneratorG4Gun.hh,v 1.7 2009-03-25 15:16:56 schubert Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorG4Gun.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 18:13:01 PDT 2004
 * 
 * REVISION:
 * 
 * 07-08-2004, Created, R. Henning
 * 11-08-2004, Implemented virtual method SetParticlePosition of MGVGenerator, Luciano
 * 13-09-2005, Added Messenger function, Xiang
 *             particles can be generated with momentum direction
 *              pointing back to the center of detectors
 *             This function is used for studying LN2 shielding effect
 * 01-25-2006, Added methods and variables to generate particles 
 *             with direction isotropically sampled in a given cone around 
 *             a fixed direction, Luciano
 * 04-11-2006, Cone sampling corrected, Luciano
 * 11-09-2006, Added methods and variables to generate particles with direction 
 *             sampled isotropically in a given cone pointing towards a fixed 
 *             point, Luciano 
 * 05-29-2008, Added methods and variables to sample energy according to an external 
 *             distribution read from a file, Luciano
 * 25.Mar 2009 Jens Schubert, Getters and an include has been added
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORG4GUN_HH
#define _MGGENERATORG4GUN_HH

//---------------------------------------------------------------------------//

#include "generators/MGVGenerator.hh"

#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"

class G4Event;
class G4ParticleGun;
class MGGeneratorG4GunMessenger;
class MGGeneratorReadSpectrumFile;

//---------------------------------------------------------------------------//

class MGGeneratorG4Gun : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorG4Gun();

  //copy constructor
  MGGeneratorG4Gun(const MGGeneratorG4Gun &);

  //destructor
  ~MGGeneratorG4Gun();

  //public interface
  void GeneratePrimaryVertex(G4Event *event);
  void SetParticlePosition(G4ThreeVector vec) {fParticleGun->SetParticlePosition(vec);} ;

  void SetCentricEffectFlag(G4bool flag) {turn_on_centric_effect=flag;};
  void SetCenterPosition(G4ThreeVector center_vec);
  void SetCenterPositionSmear(G4double ss) { xyz_smear=ss; };
  void SetCentricEffectConeFlag(G4bool);
  void SetCentricConeAngle(G4double angle){theta_centric = angle;};

  void SetIsotropicConeFlag(G4bool bb){fIsotropicConeFlag = bb;};
  void SetConeAxisDirection(G4ThreeVector vec){fConeAxisDirection = vec;};
  void SetConeDeltaTheta(G4double bb){fConeDeltaTheta = bb;};

  G4bool GetIsotropicConeFlag(){return fIsotropicConeFlag;};
  G4ThreeVector GetConeAxisDirection(){return fConeAxisDirection;};
  G4double GetConeDeltaTheta(){return fConeDeltaTheta;};

  void SetReadSpectrumFromFile(G4bool bb){fEnergySpectrumFromFile = bb;};
  void SetFileNameFromFile(G4String);

  G4bool GetCentricEffectFlag() {return turn_on_centric_effect;};
  G4bool GetCentricEffectConeFlag() {return fConeCentric;};
  G4double GetCentricConeAngle() {return theta_centric;};
  //protected members
protected:


  //private  members
private:
  G4ParticleGun      *fParticleGun;

  MGGeneratorG4GunMessenger* fTheMessenger;

  // flag for turning on or off the centric effect
  G4bool turn_on_centric_effect;
  G4double xcenter;
  G4double ycenter;
  G4double zcenter;
  G4double xyz_smear;
  G4bool fConeCentric;
  G4double theta_centric;
  
  //flag for turning on/off the isotropic generation over a cone
  G4bool fIsotropicConeFlag;
  G4ThreeVector fConeAxisDirection;
  G4double fConeDeltaTheta;

  //possible flag for energy sampling
  G4bool fEnergySpectrumFromFile; //custom or normal (= beam)
  MGGeneratorReadSpectrumFile* fEnergySpectrumTypeGenerator;

};
#endif
