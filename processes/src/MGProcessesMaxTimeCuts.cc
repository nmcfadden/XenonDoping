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
// $Id: MGProcessesMaxTimeCuts.cc,v 1.1 2004-12-07 09:39:22 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessesMaxTimeCuts.cc
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
 * FIRST SUBMISSION: Fri Apr 23 11:08:55 PDT 2004
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
#include "processes/MGProcessesMaxTimeCuts.hh"      //Present MG Class Headers 

using namespace CLHEP;

//---------------------------------------------------------------------------//

MGProcessesMaxTimeCuts::MGProcessesMaxTimeCuts(const G4String& aName) : MGProcessesSpecialCuts(aName) {
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
   SetProcessType(fUserDefined);
}


//MGProcessesMaxTimeCuts::MGProcessesMaxTimeCuts(const MGProcessesMaxTimeCuts & other)
//{
//
//}

MGProcessesMaxTimeCuts::~MGProcessesMaxTimeCuts() {;}

MGProcessesMaxTimeCuts::MGProcessesMaxTimeCuts(MGProcessesMaxTimeCuts&) : MGProcessesSpecialCuts() {}

G4double MGProcessesMaxTimeCuts::PostStepGetPhysicalInteractionLength( const G4Track& aTrack, G4double, G4ForceCondition* condition ) {
	// condition is set to "Not Forced"
	*condition = NotForced;

	G4double     proposedStep = DBL_MAX;
	// get the pointer to UserLimits
	G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
	const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();

	// can apply cuts for specific particles - use if(particleDef):
	//   G4ParticleDefinition* aParticleDef = aTrack.GetDefinition();

	//   G4cout << " Time: " << pUserLimits->GetUserMaxTime(aTrack) << G4endl;

	if (pUserLimits) {
		G4double temp = DBL_MAX;
		//max time limit
		G4double dTime= (pUserLimits->GetUserMaxTime(aTrack) - aTrack.GetGlobalTime());
		if (dTime < 0. ) {
			proposedStep = 0.;
		} else {  
			G4double beta = (aParticle->GetTotalMomentum())/(aParticle->GetTotalEnergy());
			temp = beta*c_light*dTime;
			if (proposedStep > temp) proposedStep = temp;                  
		}
	}
	return proposedStep;
}
