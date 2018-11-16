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
// $Id: MGProcessesSpecialCuts.cc,v 1.2 2005-05-26 17:33:04 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessesSpecialCuts.cc
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
 * FIRST SUBMISSION: Fri Apr 23 11:08:32 PDT 2004
 * 
 * REVISION:
 *
 * 04-23-2004, Kareem
 *             Initial file submission
 * 01-10-2005, R. Henning
 *             Changed names of G4ParticleChange methods called in 
 *             PostStepDoIt()  to comply with Geant4 7.0
 */
//---------------------------------------------------------------------------//
//

//  GEANT4 HEADERS
#include "G4VParticleChange.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

//  MG headers
#include "processes/MGProcessesSpecialCuts.hh"      //Present MG Class Headers 

//---------------------------------------------------------------------------//

MGProcessesSpecialCuts::MGProcessesSpecialCuts( const G4String& aName ) : G4VProcess(aName) {
	if (verboseLevel>1) {
		G4cout << GetProcessName() << " is created "<< G4endl;
	}
}

//MGProcessesSpecialCuts::MGProcessesSpecialCuts(const MGProcessesSpecialCuts & other) {}

MGProcessesSpecialCuts::~MGProcessesSpecialCuts() {}

G4VParticleChange* MGProcessesSpecialCuts::PostStepDoIt( const G4Track& aTrack, const G4Step& ) {
//
// Stop the current particle, if requested by G4UserLimits 
// 			    			    			    
   aParticleChange.Initialize(aTrack);
   aParticleChange.ProposeEnergy(0.) ;
   aParticleChange.ProposeLocalEnergyDeposit (aTrack.GetKineticEnergy()) ;
   aParticleChange.ProposeTrackStatus(fStopButAlive);
   return &aParticleChange;
}

G4double MGProcessesSpecialCuts::PostStepGetPhysicalInteractionLength( const G4Track&, G4double, G4ForceCondition* ) {
  return DBL_MAX;
}
