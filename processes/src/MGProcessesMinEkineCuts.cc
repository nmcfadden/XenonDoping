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
// $Id: MGProcessesMinEkineCuts.cc,v 1.1 2004-12-07 09:39:22 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessesMinEkineCuts.cc
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
 * AUTHOR: Kareem
 * CONTACT: 
 * FIRST SUBMISSION: Fri Apr 23 11:08:41 PDT 2004
 * 
 * REVISION:
 *
 * 04-23-2004, Kareem
 *             Initial file submission
 */
//---------------------------------------------------------------------------//
//

//  GEANT4 headers
#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4EnergyLossTables.hh"

//---------------------------------------------------------------------------//

//  MG headers
#include "processes/MGProcessesMinEkineCuts.hh"      //Present MG Class Headers 

//---------------------------------------------------------------------------//

MGProcessesMinEkineCuts::MGProcessesMinEkineCuts( const G4String& aName ) : MGProcessesSpecialCuts(aName) {
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
   SetProcessType(fUserDefined);
}

//MGProcessesMinEkineCuts::MGProcessesMinEkineCuts(const MGProcessesMinEkineCuts & other) {}

MGProcessesMinEkineCuts::~MGProcessesMinEkineCuts() {;}

MGProcessesMinEkineCuts::MGProcessesMinEkineCuts(MGProcessesMinEkineCuts&) : MGProcessesSpecialCuts() {}

G4double MGProcessesMinEkineCuts::PostStepGetPhysicalInteractionLength( const G4Track& aTrack, G4double, G4ForceCondition* condition ) {
	// condition is set to "Not Forced"
	*condition = NotForced;

	G4double     proposedStep = DBL_MAX;
	// get the pointer to UserLimits
	G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
	const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
	G4ParticleDefinition* aParticleDef = aTrack.GetDefinition();

	if (pUserLimits && aParticleDef->GetPDGCharge() != 0.0) {
		//min kinetic energy
		G4double temp = DBL_MAX;
		G4double eKine = aParticle->GetKineticEnergy();
		const G4MaterialCutsCouple* couple = aTrack.GetMaterialCutsCouple();
		G4double eMin = pUserLimits->GetUserMinEkine(aTrack);

		G4double rangeNow = DBL_MAX;

		rangeNow = G4EnergyLossTables::GetRange(aParticleDef,eKine,couple);

		if (eKine < eMin ) {
			proposedStep = 0.;
		} else {
			// charged particles only
			G4double rangeMin = G4EnergyLossTables::GetRange(aParticleDef,eMin,couple);
			temp = rangeNow - rangeMin;
			if (proposedStep > temp) proposedStep = temp;
		}
	}
	return proposedStep;
}
