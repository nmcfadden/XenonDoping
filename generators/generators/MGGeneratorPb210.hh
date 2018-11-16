//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
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
/**                                                            
* $Id:
*      
* CLASS DECLARATION:  MGGeneratorPb210.hh
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION: 
*
*/ 
// Begin description of class here
/**
*
* Generates gammas as from Pb210 shielding; constructs root TF1 functions
* and TH1F histograms to sample energy, zenith angle, and origin, then
* produces a gamma at a random point on the lead shield, with the chosen
* zenith angle and energy.
* Currently, the simulator can produce second order photons, but will always
* produce first order photons.
* 
*
*/
// End class description
//
/**  
* SPECIAL NOTES:
* Using this class requires GSS.  The macro macros/Pb210Generator_BEGE.mac
* shows an example of how to use GSS with this class.
*
*/
// 
// --------------------------------------------------------------------------//
/** 
* AUTHOR: A. Stevens
* CONTACT: paddy@physics.unc.edu
* FIRST SUBMISSION:
* 
* REVISION:
* 
*/
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORPB210_HH
#define _MGGENERATORPB210_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

#include "TF1.h"
#include "TH1F.h"

#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorPb210 : public MGVGenerator
{
public:

	//default constructor
	MGGeneratorPb210();

	//copy constructor
	MGGeneratorPb210(const MGGeneratorPb210 &);

	//destructor
	~MGGeneratorPb210();

	//public interface
	void BeginOfEventAction(G4Event *event);
	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);
	void GeneratePrimaryVertex(G4Event *event);

	//this method is important for GSS
	void SetParticlePosition(G4ThreeVector pos) {fCurrentPosition = pos;}

	//function determines the direction of particle based on zenith angle
	void DirectionDecider();

	//energy/anglular distribution functions, passed to root function constructors
	G4double FirstOrderEnergyFormula(G4double *energy, G4double *parameters);
	G4double SecondOrderEnergyFormula(G4double *energy, G4double *parameters);
	G4double AngularDistributionFormula(G4double *angle, G4double *parameters);
	G4double FOAngularDistQValue();	//specific to first order
	G4double SOAngularDistQValue();	//specific to second order

	//energy/angular distribution samplers
	void SampleFirstOrderPhotonEnergy();
	void SampleSecondOrderPhotonEnergy();
	void SampleAngularDistribution();

	//important for generator's ability to determine the zenith axis
	void SetShieldSurfaceVolume(G4String volumeName);
	G4LogicalVolume* GetShieldSurfaceVolume() {return fLogicalPb210Surface;} 

	//protected members
protected:

	//private  members
private:
	G4ParticleGun*	fParticleGun;

	//particle properties
	G4double	fCurrentEnergy;	// energy of current particle
	G4ThreeVector	fCurrentPosition; // current position of particle
	G4double	fZenithAngle; //angle between fDirection and zenith
	G4ThreeVector	fDirection;	// direction of momentum   

	//used to determine direction of normal axis
	G4LogicalVolume*	fLogicalPb210Surface;

	//Temporarily, fXRay is always false and fFirstOrder is always true.
	G4bool	fXRay;	//true if current photon is an X-ray,
	//which affects angular distribution
	G4bool	fFirstOrder;	//true if current photon is
	//first order, false if second order

	//energy/angular Distribution Functions
	TF1*	hFirstOrderEnergy;
	TF1*	hSecondOrderEnergy;
	TF1*	hAngularDistribution;
};
#endif

