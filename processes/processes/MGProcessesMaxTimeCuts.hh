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
 * $Id: MGProcessesMaxTimeCuts.hh,v 1.1 2004-12-07 09:37:19 pandola Exp $
 *      
 * CLASS DECLARATION:  MGProcessesMaxTimeCuts.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This is a special class for handling transportation of neutrons
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
 * FIRST SUBMISSION: Fri Apr 23 11:08:55 PDT 2004
 * 
 * REVISION:
 * 
 * 04-23-2004, Kareem
 *             Initial file submission
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSESMAXTIMECUTS_HH
#define _MGPROCESSESMAXTIMECUTS_HH

//---------------------------------------------------------------------------//

//  GEANT4 headers
#include "G4ios.hh"
#include "globals.hh"

//  MG headers
#include "processes/MGProcessesSpecialCuts.hh"

//---------------------------------------------------------------------------//

class MGProcessesMaxTimeCuts : public MGProcessesSpecialCuts {

	//public interface
	public:

		//default constructor
		MGProcessesMaxTimeCuts(const G4String& processName ="MGProcessesMaxTimeCuts" );

		//copy constructor
		//MGProcessesMaxTimeCuts(const MGProcessesMaxTimeCuts &);

		//destructor
		virtual ~MGProcessesMaxTimeCuts();

		virtual G4double PostStepGetPhysicalInteractionLength( const G4Track& track, G4double previousStepSize, G4ForceCondition* condition );

	//protected members
	protected:


	//private  members
	private:
		// hide assignment operator as private 
		MGProcessesMaxTimeCuts(MGProcessesMaxTimeCuts&);
		MGProcessesMaxTimeCuts& operator=(const MGProcessesMaxTimeCuts& right);


};
#endif
