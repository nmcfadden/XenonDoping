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
 * $Id: MGGeneratorTUNLFEL.hh,v 1.2 2007-02-23 09:24:09 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorTUNLFEL.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Generate Beam of Gammas as from the TUNL FEL
 * Currently the beam is simulated with a Gaussian energy and
 * width distribution in x and y.
 * The beam is created with the major axis along the x-axis and the minor
 * axis along the y axis. It is then rotated by fRho degrees on the x-y plane
 * fRho is relative to the x-axis. Particles are then generated parallel to 
 * the z-axis.
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Aug  4 07:49:57 PDT 2004
 * 
 * REVISION:
 * 
 * 08-04-2004, Created, R. Henning
 * 11-08-2004, Implemented virtual method SetParticlePosition of MGVGenerator, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORTUNLFEL_HH
#define _MGGENERATORTUNLFEL_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorTUNLFEL : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorTUNLFEL();

  //copy constructor
  MGGeneratorTUNLFEL(const MGGeneratorTUNLFEL &);

  //destructor
  ~MGGeneratorTUNLFEL();

  //public interface
  void BeginOfEventAction(G4Event *event);
  void BeginOfRunAction(G4Run const *run);
  void Dump();
  void EndOfRunAction(G4Run const *run);
  void GeneratePrimaryVertex(G4Event *event);
  void SetDefaults();

  G4double GetCurrentEnergy() { return fCurrentEnergy; }
  G4ThreeVector GetCurrentPosition() { return fCurrentPosition; }
  
  //This method is not used but it is necessary 
  //because it is purely virtual in MGVGenerator
  void SetParticlePosition(G4ThreeVector) {;}

  void SetEnergySigma(G4double energysigma) { fEnergySigma = energysigma; }
  void SetMajorSigma(G4double majorsig) { fMajorSigma = majorsig; }
  void SetMeanEnergy(G4double energy) { fMeanEnergy = energy; }
  void SetMinorSigma(G4double minorsig) { fMinorSigma = minorsig; }
  void SetOrigin(G4ThreeVector origin) { fOrigin = origin; }
  void SetRho(G4double rho) { fRho = rho; }

  //protected members
protected:

  //private  members
private:
  G4ParticleGun      *fParticleGun;

  G4double           fCurrentEnergy;  // Energy of current particle.
  G4ThreeVector      fCurrentPosition; // Current position of particle 
                                       // generated.
  G4ThreeVector      fDirection;      // Direction of Beam    
  G4double           fEnergySigma; // Energy spread of beam (sigma).
  G4double           fMajorSigma;  // Beam width (sigma) along major axis
  G4double           fMeanEnergy;    // Mean energy of beam
  G4double           fMinorSigma;  // Beam width along minor axis.
  G4ThreeVector      fOrigin;      // Beam origin. Particles will be 
                                   // distributed around this point.
  G4double           fRho; // Major angle of beam ellips relative to x-axis.
};
#endif
