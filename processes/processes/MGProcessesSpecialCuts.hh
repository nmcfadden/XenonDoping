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
 * $Id: MGProcessesSpecialCuts.hh,v 1.1 2004-12-07 09:37:19 pandola Exp $
 *      
 * CLASS DECLARATION:  MGProcessesSpecialCuts.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * Used to handle special cuts in the physics list.
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
 * AUTHOR: Kareem
 * CONTACT: 
 * FIRST SUBMISSION: Fri Apr 23 11:08:32 PDT 2004
 * 
 * REVISION:
 * 
 * 04-23-2004, Kareem
 *             Initial file creation
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSESSPECIALCUTS_HH
#define _MGPROCESSESSPECIALCUTS_HH

//---------------------------------------------------------------------------//

//  GEANT4 headers
#include "G4ios.hh"
#include "globals.hh"
#include "G4VProcess.hh"

//---------------------------------------------------------------------------//

class MGProcessesSpecialCuts : public G4VProcess {
	//public interface
	public:

		//default constructor
		MGProcessesSpecialCuts(const G4String& processName ="MGProcessesSpecialCuts" );

		//copy constructor
		//MGProcessesSpecialCuts(const MGProcessesSpecialCuts &);

		//destructor
		virtual ~MGProcessesSpecialCuts();

		 virtual G4double PostStepGetPhysicalInteractionLength( const G4Track& track, G4double previousStepSize, G4ForceCondition* condition );

		 virtual G4VParticleChange* PostStepDoIt( const G4Track&, const G4Step& );
					
		 //  no operation in  AtRestGPIL
		 virtual G4double AtRestGetPhysicalInteractionLength( const G4Track&, G4ForceCondition* ) { return -1.0; };
					
		 //  no operation in  AtRestDoIt      
		 virtual G4VParticleChange* AtRestDoIt( const G4Track&, const G4Step& ) {return NULL;};

		 //  no operation in  AlongStepGPIL
		 virtual G4double AlongStepGetPhysicalInteractionLength( const G4Track&, G4double, G4double, G4double&, G4GPILSelection* ) { return -1.0; };

		 //  no operation in  AlongStepDoIt
		 virtual G4VParticleChange* AlongStepDoIt( const G4Track&, const G4Step& ) {return NULL;};

	//protected members
	protected:


	//private  members
	private:
		// hide assignment operator as private 
		MGProcessesSpecialCuts& operator=(const MGProcessesSpecialCuts&) {return *this;};
};
#endif
